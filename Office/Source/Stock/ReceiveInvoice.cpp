//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReceiveInvoice.h"
#include "StockData.h"
#include "Login.h"
#include "Utilities.h"
#include "SelectStockItem.h"
#include "Reports.h"
#include "StockReportData.h"
#include "SelectSupplierStockItem.h"
#include "MM_Math.h"
#include "MM_StockTransactions.h"
#include "MM_PurchaseOrders.h"
#include "Connections.h"
#include "MMData.h"
#include "..\Registration\Registration.h"
#include "..\TinyXml\tinyxml.h"
#include <iostream.h>
#include "..\Helpers.h"

#include <Registry.hpp>
#include "MMRegistry.h"
#include "Consts.h"
#include "RemoteHostManager.h"
#include "XeroInvoiceBuilder.h"
#include <Math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
//TfrmReceiveInvoice *frmReceiveInvoice;
//---------------------------------------------------------------------------
const int GSTDecimals = 2;
//---------------------------------------------------------------------------
__fastcall TfrmReceiveInvoice::TfrmReceiveInvoice(TComponent* Owner)
: TForm(Owner),
frmReceiveStockItem(new TfrmReceiveStockItem(NULL))
{
	DoubleBuffered						= true;
	Panel1->DoubleBuffered			= true;
	Panel4->DoubleBuffered			= true;
	vtvStockQty->DoubleBuffered	= true;

	dtpInvoiceDate->Date				= Date();
	dtpInvoiceDate->Time				= Time();
	IsPackingSlipUpdateMode = false;
	BatchKey = 0;
    Decimalpalaces = CurrentConnection.SettingDecimalPlaces;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::FormShow(TObject *Sender)
{


	IsPurchaseOrderMode = false;
	if(!Transaction->InTransaction)
	Transaction->StartTransaction();
	rbExcludingGST->Checked			= true;
	chbGSTOverride->Checked			= false;
	//btnCommitPackingSlip->Enabled = !IsPackingSlipUpdateMode;

	qrSupplier->Close();
	qrPurchaseOrder->Close();
	qrPurchaseStock->Close();

	vtvStockQty->Visible	= true;
	vtvStockQty->Clear();
	vtvStockQty->NodeDataSize = sizeof(TInvoiceItemNodeData);
 
	if(Decimalpalaces==0)
	{ Decimalpalaces=4;  }
	neGST->DecimalPlaces= Decimalpalaces;
	LoadForm();
	qrSupplier->Close();
    AllowNegativeValue = false;
    IsNegativeQtyOrCost = true;
	if(OrderKey == 0)
	{
		qrSupplier->ParamByName("Supplier_Key")->AsInteger = SupplierKey;
		qrSupplier->Open();
		int ccc =  qrSupplier->FieldByName("Contact_LK")->AsInteger;
		lbeTitle->Caption = "Invoice: " + InvoiceReference + " (" + qrSupplier->FieldByName("Company_Name")->AsString + ")";
        AllowNegativeValue = true;
	}

	ShowTotals();

    if(btnAdd->Enabled)
    {
	    btnAdd->SetFocus();
    }    
	btnPrintInvoice->Enabled = BatchKey != 0;

	InitXeroIntegration();


	if(Decimalpalaces==0)
	{ Decimalpalaces=4;  }
	neGST->DecimalPlaces= Decimalpalaces;
 lblReceiptnumber->Caption="";
 if(BatchKey>0)
 {
   lblReceiptnumber->Caption=BatchKey;//(qrBatchKey->Fields->Fields[0]->AsInteger+1);
   ReceiptNumber= BatchKey;
  }
  else
  {

        qrBatchKey->Close();
        qrBatchKey->SQL->Text = "Select Gen_id(Gen_Stocktrans_Batch_Key, 0) From rdb$database";
        qrBatchKey->Open();
        lblReceiptnumber->Caption=(qrBatchKey->Fields->Fields[0]->AsInteger+1);
        ReceiptNumber=qrBatchKey->Fields->Fields[0]->AsInteger+1;
  }
  reGstValue->Enabled =  false;
  if(!reGstValue->Enabled)
  {
     reGstValue->Color = clBtnFace;
  }

  //myEditBox->Enabled = false;
  //neGst->Enabled = false;
}

void TfrmReceiveInvoice::LoadForm()
{
	if(IsPackingSlipUpdateMode && BatchKey != 0)
	{
		PVirtualNode OrderItemNode = NULL;
		TInvoiceItemNodeData *NodeData = NULL;

		qrStockTransaction->ParamByName("Batch_Key")->AsInteger = BatchKey;

		for (qrStockTransaction->Open(); !qrStockTransaction->Eof; qrStockTransaction->Next())
		{
			if (qrStockTransaction->FieldByName("ORDER_QTY")->AsFloat != 0)
			{
				OrderItemNode = vtvStockQty->AddChild(NULL);
				NodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);

				NodeData->PurchaseStockKey			= 0;
				NodeData->Text						= qrStockTransaction->FieldByName("Description")->AsString;
				NodeData->Code						= qrStockTransaction->FieldByName("Code")->AsString;
				NodeData->StockCategory				= qrStockTransaction->FieldByName("Stock_Category")->AsString;
				NodeData->StockGroup					= qrStockTransaction->FieldByName("Stock_Group")->AsString;
				NodeData->GLCode						= qrStockTransaction->FieldByName("GL_Code")->AsString;
				NodeData->GSTPercent					= qrStockTransaction->FieldByName("GST_Percent")->AsFloat;
				NodeData->Location					= qrStockTransaction->FieldByName("Location")->AsString;
				NodeData->StocktakeUnit				= qrStockTransaction->FieldByName("ORDER_UNIT")->AsString;
				NodeData->StockKey					= qrStockTransaction->FieldByName("STOCK_KEY")->AsInteger;
				NodeData->SupplierCode				= qrStockTransaction->FieldByName("Supplier_Code")->AsString;
				NodeData->SupplierUnit				= qrStockTransaction->FieldByName("UNIT")->AsString; //StrToFloat(FloatToStrF(qrStockTransaction->FieldByName("UNIT")->AsString,ffFixed,19, 4))
				NodeData->DisplayedUnit				= qrStockTransaction->FieldByName("UNIT")->AsString;
				BaseUnit=NodeData->DisplayedUnit;
				NodeData->IsUnitChanged          = false;
			if(IsSavedPackingSlip)
                {	NodeData->IsUnitEditable         = false;}
                else{
                NodeData->IsUnitEditable         = true;
                }
				NodeData->SupplierUnitCost			= (qrStockTransaction->FieldByName("UNIT_COST")->AsFloat);
				NodeData->SupplierUnitsToReceive	    = qrStockTransaction->FieldByName("ORDER_QTY")->AsFloat;
				NodeData->OrderQty					= (qrStockTransaction->FieldByName("ORDER_QTY")->AsFloat);
				NodeData->SupplierKey					= (qrStockTransaction->FieldByName("SUPPLIER_KEY")->AsInteger);
				NodeData->BackOrder					= 0;
				NodeData->TransactionNumber          = qrStockTransaction->FieldByName("TRANS_NUMBER")->AsInteger;
				NodeData->SupplierTotalCost		    = qrStockTransaction->FieldByName("TOTAL_COST")->AsFloat;
				NodeData->SupplierUnitSize			= GetSupplierUnitSize(NodeData->StockKey, NodeData->SupplierKey, NodeData->SupplierUnit);
                if(NodeData->SupplierUnitSize == 0)
                {
                    NodeData->SupplierUnitSize = qrStockTransaction->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
                }
				NodeData->LatestCost			= qrStockTransaction->FieldByName("LATEST_COST")->AsFloat;
				NodeData->OnHandQty			= qrStockTransaction->FieldByName("ON_HAND")->AsFloat;
                NodeData->StockRequestStockKey = qrStockTransaction->FieldByName("STOCK_REQUEST_STOCK_KEY_VAL")->AsInteger;
                NodeData->Createdtime			= qrStockTransaction->FieldByName("CREATED")->AsDateTime;
			}
		}

		qrSupplier->ParamByName("Supplier_Key")->AsInteger = qrStockTransaction->FieldByName("SUPPLIER_KEY")->AsInteger;
		qrSupplier->Open();

		lbeTitle->Caption	= "Packing Slip: " + InvoiceReference + " (" + qrSupplier->FieldByName("Company_Name")->AsString + ")";
	}
	else if(OrderKey != 0)
	{
		PVirtualNode OrderItemNode			= NULL;
		TInvoiceItemNodeData *NodeData	= NULL;

		bool Initialised						= false;
		TDateTime MinInitialiseTime		= 0.0;

		qrPurchaseOrder->ParamByName("Order_Key")->AsInteger = OrderKey;
		qrPurchaseOrder->Open();

		qrSupplier->ParamByName("Supplier_Key")->AsInteger = qrPurchaseOrder->FieldByName("Contact_FK")->AsInteger;
		qrSupplier->Open();

		qrPurchaseStock->ParamByName("Order_Key")->AsInteger = OrderKey;

		for (qrPurchaseStock->Open(); !qrPurchaseStock->Eof; qrPurchaseStock->Next())
		{
			if (qrPurchaseStock->FieldByName("Qty_On_Order")->AsFloat > 0)
			{
				OrderItemNode							= vtvStockQty->AddChild(NULL);
				NodeData									= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);

				NodeData->PurchaseStockKey			= qrPurchaseStock->FieldByName("Purchase_Stock_Key")->AsInteger;

				NodeData->Text							= qrPurchaseStock->FieldByName("Description")->AsString;
				NodeData->Code							= qrPurchaseStock->FieldByName("Code")->AsString;
				NodeData->StockKey					= qrPurchaseStock->FieldByName("Stock_Key")->AsInteger;
				NodeData->StockCategory				= qrPurchaseStock->FieldByName("Stock_Category")->AsString;
				NodeData->StockGroup					= qrPurchaseStock->FieldByName("Stock_Group")->AsString;
				NodeData->GLCode						= qrPurchaseStock->FieldByName("GL_Code")->AsString;
				NodeData->GSTPercent					= qrPurchaseStock->FieldByName("GST_Percent")->AsFloat;
				NodeData->Location					= qrPurchaseStock->FieldByName("Location")->AsString;
				NodeData->StocktakeUnit				= qrPurchaseStock->FieldByName("Stocktake_Unit")->AsString;
				NodeData->SupplierCode				= qrPurchaseStock->FieldByName("Supplier_Code")->AsString;
				NodeData->SupplierUnit				= qrPurchaseStock->FieldByName("Supplier_Unit")->AsString;
				NodeData->DisplayedUnit				= qrPurchaseStock->FieldByName("Supplier_Unit")->AsString;
				BaseUnit=NodeData->DisplayedUnit;
				NodeData->IsUnitChanged          = qrPurchaseStock->FieldByName("SUPPLIER_UNIT_CHANGED")->AsString == 'T'? true : false;
				// NodeData->IsUnitEditable = !NodeData->IsUnitChanged ;
                if(IsSavedPackingSlip)
                {	NodeData->IsUnitEditable         = false;}
                else{
                NodeData->IsUnitEditable         = true;
                }
				NodeData->SupplierUnitCost			= qrPurchaseStock->FieldByName("Supplier_Unit_Cost")->AsFloat;
				NodeData->SupplierUnitsToReceive	= qrPurchaseStock->FieldByName("Qty_On_Order")->AsFloat;
				NodeData->OrderQty					= qrPurchaseStock->FieldByName("Qty_On_Order")->AsFloat;
                //v5 to v6
                NodeData->SupplierKey				= (qrSupplier->ParamByName("Supplier_Key")->AsInteger);
				NodeData->BackOrder					= 0;
				NodeData->Initialised				= qrPurchaseStock->FieldByName("Initialised")->AsString == "T";
				NodeData->InitialisedTime			= qrPurchaseStock->FieldByName("Initialised_Time")->AsDateTime;
				NodeData->SupplierUnitSize			= qrPurchaseStock->FieldByName("Supplier_Unit_Size")->AsFloat;
				NodeData->SupplierTotalCost		= NodeData->SupplierUnitCost * NodeData->OrderQty;
				NodeData->LatestCost			      = qrPurchaseStock->FieldByName("LATEST_COST")->AsFloat;
				NodeData->OnHandQty			      = qrPurchaseStock->FieldByName("ON_HAND")->AsFloat;
				if (NodeData->Initialised)
				{
					Initialised = true;
					if (NodeData->InitialisedTime != TDateTime(0.0))
					{
						if (NodeData->InitialisedTime < MinInitialiseTime || MinInitialiseTime == TDateTime(0.0))
						{
							MinInitialiseTime = NodeData->InitialisedTime;
						}
					}
				}
			}
		}
		lbeTitle->Caption	= "Order: " + qrPurchaseOrder->FieldByName("Order_Number")->AsString + " - Invoice: " + InvoiceReference + " (" + qrSupplier->FieldByName("Company_Name")->AsString + ")";
		if (Initialised && MinInitialiseTime != TDateTime(0.0))
		{
			Application->MessageBox(AnsiString("1 or more items have been initialsed for stocktake.\r\rThe invoice date must be prior to " + MinInitialiseTime.DateTimeString()).c_str(), "Warning", MB_OK + MB_ICONWARNING);
		}
	}
    if(IsSavedPackingSlip)
    {
        btnAdd->Enabled = false;
        BitBtn1->Enabled = false;
        btnRemove->Enabled = false;
        dbcbLocation->DropDownCount = 0;
        dbcbUnit->DropDownCount = 0;
        neStockQty->Enabled = false;
        neBackOrder->Enabled = false;
        btnCommitPackingSlip->Enabled = false;
    }
    else
    {
        btnAdd->Enabled = true;
        BitBtn1->Enabled = true;
        btnRemove->Enabled = true;

        neStockQty->Enabled = true;
        neBackOrder->Enabled = true;
        btnCommitPackingSlip->Enabled = true;
    }
    //reGstValue->Enabled =  false;

}

//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	try
	{
		if (Transaction->InTransaction)
		{
			CanClose = (Application->MessageBox("Are you sure you you wish to cancel?", "Cancel?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK);
		}

      //  if(CanClose)
		IsPackingSlipUpdateMode =false;
		BatchKey = 0;
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		CanClose = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::FormClose(TObject *Sender, TCloseAction &Action)
{
	FindDialog->CloseDialog();
	try
	{
		if (Transaction->InTransaction) Transaction->Rollback();
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::vtvStockQtyAfterPaint(TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::vtvStockQtyBeforePaint(TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::vtvStockQtyCreateEditor(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink)
// before edit
{
	if (Node && Column == 4) // Quantity
	{
		TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)Sender->GetNodeData(Node);
     if(Decimalpalaces == 4)
            {
             neStockQty->Value = StrToFloat(FloatToStrF(NodeData->OrderQty, ffFixed,19, 4));
            }
            else
            {
             neStockQty->Value = StrToFloat(FloatToStrF(NodeData->OrderQty, ffFixed,19, 2));
            }
		
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neStockQty);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(neStockQty->Handle, EM_SETSEL, 0, -1);
	}
	else if (Node && Column == 5) // Unit Cost
	{
		TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)Sender->GetNodeData(Node);
        if(Decimalpalaces == 4)
            {
            neCost->Value = StrToFloat(FloatToStrF(NodeData->SupplierUnitCost, ffFixed,19, 4));
            }
            else
            {
              neCost->Value = StrToFloat(FloatToStrF(NodeData->SupplierUnitCost, ffFixed,19, 2));
            }
	  
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neCost);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(neCost->Handle, EM_SETSEL, 0, -1);
	}
	else if (Node && Column == 6) // Total Cost
	{
		TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)Sender->GetNodeData(Node);
         if(Decimalpalaces == 4)
         {
           neTotalCost->Text = FloatToStrF(NodeData->SupplierTotalCost, ffFixed,19, 4);
         }
         else
         {
           neTotalCost->Text = FloatToStrF(NodeData->SupplierTotalCost, ffFixed,19, 2);
         }
	
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neTotalCost);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(neTotalCost->Handle, EM_SETSEL, 0, -1);
	}
	else if (Node && Column == 7) // Back Order
	{
		TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)Sender->GetNodeData(Node);
		if (NodeData->BackOrder < 0)
		{
			neBackOrder->Value = 0;
		}
		else
		{
			neBackOrder->Value = NodeData->BackOrder;
		}

		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neBackOrder);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(neBackOrder->Handle, EM_SETSEL, 0, -1);
	}
	else if(Node && Column ==3)// Unit
	{
		TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)Sender->GetNodeData(Node);
		LoadUnits(NodeData->StockKey);
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, dbcbUnit);
		dbcbUnit->ItemIndex =  dbcbUnit->Items->IndexOf(NodeData->DisplayedUnit);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(dbcbUnit->Handle, EM_SETSEL, 0, -1);
	}
	else if(Node && Column ==2)// Location
	{
		TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)Sender->GetNodeData(Node);
		LoadLocations(NodeData->StockKey);
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, dbcbLocation);
		dbcbLocation->ItemIndex = Locations->IndexOf(NodeData->Location);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(dbcbLocation->Handle, EM_SETSEL, 0, -1);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::vtvStockQtyEdited(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	// after edit
	if (vtvStockQty->FocusedNode)
	{
		if (Column == 4)  // Quantity
		{
			TInvoiceItemNodeData *NodeData	= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
			NodeData->OrderQty				= neStockQty->Value;
			NodeData->BackOrder				= NodeData->SupplierUnitsToReceive - NodeData->OrderQty;
			if (NodeData->BackOrder < 0)
			{
				NodeData->BackOrder = 0;
			}
		}
		else if (Column == 5) // Unit Cost
		{
			TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
			NodeData->SupplierUnitCost = neCost->Value;
		}
		else if (Column == 6) // Total Cost
		{
			TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
			if (NodeData->OrderQty != 0)
			{
                if(neTotalCost->Text == "")
                {
                   neTotalCost->Text = "0";
                }
                double totalcost = StrToFloat(neTotalCost->Text);
				// Total Cost
				NodeData->SupplierTotalCost = totalcost;
				// work out unit cost
				NodeData->SupplierUnitCost = totalcost / NodeData->OrderQty;
			}
		}
		if (Column == 7)  // Back Order
		{
			TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
			if (neBackOrder->Value + NodeData->OrderQty <= NodeData->SupplierUnitsToReceive)
			{
				NodeData->BackOrder = neBackOrder->Value;
			}
			else
			{
				NodeData->BackOrder = NodeData->SupplierUnitsToReceive - NodeData->OrderQty;
			}
			if (NodeData->BackOrder < 0)
			{
				NodeData->BackOrder = 0;
			}
		}
		if(Column == 3)
		{
			TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
			NodeData->DisplayedUnit = dbcbUnit->Items->Strings[dbcbUnit->ItemIndex];
		}
		if(Column == 2)
		{
			TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
			NodeData->Location =    dbcbLocation->Items->Strings[dbcbLocation->ItemIndex];
		}
		vtvStockQty->InvalidateNode(vtvStockQty->FocusedNode);
	}
	ShowTotals();
}
//--------------------------------------------------------------------------- 
void __fastcall TfrmReceiveInvoice::vtvStockQtyEditing(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
bool &Allowed)
{
	TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)Sender->GetNodeData(Node);
	Allowed = ((Column == 4 && !IsPackingSlipUpdateMode) || Column == 5 || Column == 6 || (Column == 3 && NodeData && NodeData->IsUnitEditable) || (Column == 2 && NodeData && NodeData->IsUnitEditable) || (Column == 7 && !IsPackingSlipUpdateMode) || (Column == 4 && !IsSavedPackingSlip));
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::vtvStockQtyFocusChanged(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	if (Node && !Sender->IsEditing())
	{
		if (Sender->FocusedColumn != 4 && Sender->FocusedColumn != 5 && Sender->FocusedColumn != 6 && Sender->FocusedColumn != 7 && Sender->FocusedColumn != 3 && Sender->FocusedColumn != 2)
		{
			Sender->FocusedColumn = 4;
		}
		Sender->EditNode(Node, Column);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::vtvStockQtyFocusChanging(
TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::vtvStockQtyGetImageIndex(
TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind,
TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
	ImageIndex = -1;
	TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)Sender->GetNodeData(Node);
	if (Column == 0 && NodeData)
	{
		if (NodeData->SupplierUnitsToReceive > NodeData->OrderQty + NodeData->BackOrder &&
				NodeData->PurchaseStockKey != 0)
		{
			if (Kind == ikNormal || Kind == ikSelected)
			{
				ImageIndex = ICON_EXCLAMATION_INDEX;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::vtvStockQtyGetText(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
TVSTTextType TextType, WideString &CellText)
{
	TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Column)
		{
		case 0:	CellText = NodeData->Text;
			break;
		case 1:	CellText = NodeData->SupplierCode;
			break;
		case 2:	CellText = NodeData->Location;
			break;
		case 3:	CellText = NodeData->DisplayedUnit;
			break;
		case 4:	CellText = MMMath::FloatString(NodeData->OrderQty); // Quantity
			break;
		case 5:	CellText = MMMath::FloatString(NodeData->SupplierUnitCost, neCost->DecimalPlaces, ffCurrency);
			break;
		case 6:
             CellText = FormatFloat("0.0000", NodeData->SupplierTotalCost);
			break;
		case 7:  CellText = MMMath::FloatString(NodeData->BackOrder);  // Back Order
			break;
		case 8:  CellText = MMMath::FloatString(NodeData->LatestCost);  // Latest Cost
			break;
		case 9:  CellText = MMMath::FloatString(NodeData->OnHandQty);  // On Hand
			break;
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::vtvStockQtyKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		if (Key == VK_RETURN)
		{
			vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::rbGSTClick(TObject *Sender)
{
	ShowTotals();
}
//---------------------------------------------------------------------------
void TfrmReceiveInvoice::CalculateInvoiceTotals(TInvoiceSummary &InvoiceSummary)
{
	PVirtualNode node							= vtvStockQty->GetFirst();
	double total								= 0.0;
	InvoiceSummary.BackOrderWarning		= false;
	InvoiceSummary.TotalGST					= 0.00;
	//Currency totalCalculatedGST			= 0;
    double totalCalculatedGST			= 0.0;
	while (node)
	{
		TInvoiceItemNodeData *nodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(node);
		total += nodeData->SupplierTotalCost;
		if (!chbGSTOverride->Checked)
		{
			if (rbExcludingGST->Checked)
			{
				totalCalculatedGST += nodeData->SupplierTotalCost * nodeData->GSTPercent / 100;
			}
			else if (rbIncludingGST->Checked)
			{
				totalCalculatedGST += nodeData->SupplierTotalCost * nodeData->GSTPercent / (100 + nodeData->GSTPercent);
			}
		}
		if (nodeData->SupplierUnitsToReceive > nodeData->OrderQty + nodeData->BackOrder && nodeData->PurchaseStockKey != 0)
		{
			InvoiceSummary.BackOrderWarning = true;
		}
		node = vtvStockQty->GetNext(node);
	}
	InvoiceSummary.TotalExcl	= 0.0;
	InvoiceSummary.TotalInc		= 0.0;

	if (chbGSTOverride->Checked)
	{
        if(reGstValue->Text == "")
        {
           InvoiceSummary.TotalGST = 0.0;
        }
        else
        {
           InvoiceSummary.TotalGST = StrToFloat(reGstValue->Text);
        }
	}
	else
	{
		InvoiceSummary.TotalGST = totalCalculatedGST;
	}
	if (rbExcludingGST->Checked)
	{
		InvoiceSummary.TotalExcl	= total;
		InvoiceSummary.TotalInc		= total + InvoiceSummary.TotalGST;
	}
	else if (rbIncludingGST->Checked)
	{
		InvoiceSummary.TotalExcl	= total - InvoiceSummary.TotalGST;
		InvoiceSummary.TotalInc		= total;
	}
}
//---------------------------------------------------------------------------
void TfrmReceiveInvoice::ShowTotals()
{       
	TInvoiceSummary invoiceSummary;
    //neGST->Value = 0.00;
	CalculateInvoiceTotals(invoiceSummary);
	imgWarning->Visible	= invoiceSummary.BackOrderWarning;
	lbeWarning->Visible	= invoiceSummary.BackOrderWarning;
	lbeTotalExc->Caption	= MMMath::FloatString(invoiceSummary.TotalExcl, Decimalpalaces);
	lbeTotalInc->Caption	= MMMath::FloatString(invoiceSummary.TotalInc, Decimalpalaces);
    //myEditBox->Text =  FormatFloat("0.00",invoiceSummary.TotalGST) ;
	//neGST->Value			= invoiceSummary.TotalGST;
	if(Decimalpalaces==2)
	{
		lbeTotalExc->Caption= FormatFloat("0.00",invoiceSummary.TotalExcl);;
		lbeTotalInc->Caption= FormatFloat("0.00",invoiceSummary.TotalInc);;
        //myEditBox->Text =  FormatFloat("0.00",invoiceSummary.TotalGST);
        reGstValue->Text = FormatFloat("0.00",invoiceSummary.TotalGST);

		
	}
	else
	{
		lbeTotalExc->Caption= FormatFloat("0.0000",invoiceSummary.TotalExcl);;
		lbeTotalInc->Caption= FormatFloat("0.0000",invoiceSummary.TotalInc);;
        //myEditBox->Text =  FormatFloat("0.0000",invoiceSummary.TotalGST) ;
        reGstValue->Text = FormatFloat("0.0000",invoiceSummary.TotalGST) ;
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::btnFindClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::FindDialogFind(TObject *Sender)
{
	PVirtualNode CurItem;
	if (vtvStockQty->FocusedNode)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = vtvStockQty->GetNext(vtvStockQty->FocusedNode);
		}
		else
		{
			CurItem = vtvStockQty->GetPrevious(vtvStockQty->FocusedNode);
		}
	}
	else
	{
		CurItem = vtvStockQty->GetFirst();
	}
	while (CurItem)
	{
		TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(CurItem);
		AnsiString FindText, NodeText;
		if (FindDialog->Options.Contains(frMatchCase))
		{
			FindText = FindDialog->FindText;
			NodeText = NodeData->Text;
		}
		else
		{
			FindText = FindDialog->FindText.UpperCase();
			NodeText = AnsiString(NodeData->Text).UpperCase();
		}
		if (FindDialog->Options.Contains(frWholeWord))
		{
			if (FindText == NodeText)
			{
				vtvStockQty->FocusedNode = CurItem;
				vtvStockQty->Selected[CurItem] = true;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				vtvStockQty->FocusedNode = CurItem;
				vtvStockQty->Selected[CurItem] = true;
				break;
			}
		}
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = vtvStockQty->GetNext(CurItem);
		}
		else
		{
			CurItem = vtvStockQty->GetPrevious(CurItem);
		}
	}
	if (!CurItem)
	{
		Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::btnAddClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	frmSelectStockItem->Reset = true;

	if (frmSelectStockItem->ShowModal() == mrOk)
	{
		AddStockItem(frmSelectStockItem->StockCode);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::BitBtn1Click(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	qrSupplier->Open();
	std::auto_ptr<TfrmSelectSupplierStockItem> frmSelectSupplierStockItem(new TfrmSelectSupplierStockItem(NULL));
	frmSelectSupplierStockItem->CompanyKey = qrSupplier->FieldByName("Contact_LK")->AsInteger;
	if (frmSelectSupplierStockItem->ShowModal() == mrOk)
	{
		AddStockItem(frmSelectSupplierStockItem->StockCode);
	}
	qrSupplier->Close();
}
//---------------------------------------------------------------------------
void TfrmReceiveInvoice::AddStockItem(AnsiString StockCode)
{       qrSupplier->Close();
	qrSupplier->Open();
	frmReceiveStockItem->StockCode = StockCode;
	frmReceiveStockItem->SupplierKey = qrSupplier->FieldByName("Contact_LK")->AsInteger;
    frmReceiveStockItem->AllowNegativeQuantity = AllowNegativeStockQuantity;
         if (frmReceiveStockItem->Execute() == mrOk)
	{
		PVirtualNode Node = vtvStockQty->GetFirst();
		while (Node)
		{
			TInvoiceItemNodeData *NodeData			= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(Node);
			if (NodeData->Text == frmReceiveStockItem->Description && NodeData->Location == frmReceiveStockItem->Location && NodeData->SupplierUnit == frmReceiveStockItem->SupplierUnit)
			{
				vtvStockQty->FocusedNode = Node;
				vtvStockQty->FocusedColumn = 4;
				vtvStockQty->Selected[Node] = true;
				Application->MessageBox("You have already this stock item.", "Error", MB_ICONERROR + MB_OK);
				return;
			}
			Node = vtvStockQty->GetNext(Node);
		}
		PVirtualNode OrderItemNode = NULL;
		TInvoiceItemNodeData *NodeData = NULL;
		OrderItemNode	= vtvStockQty->AddChild(NULL);
		NodeData   = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
		NodeData->PurchaseStockKey  = 0;
		NodeData->Text		    = frmReceiveStockItem->Description;
		NodeData->StockCategory	    = frmReceiveStockItem->StockCategory;
		NodeData->StockGroup	    = frmReceiveStockItem->StockGroup;
		NodeData->GLCode	    = frmReceiveStockItem->GLCode;
		NodeData->Location	    = frmReceiveStockItem->Location;
		NodeData->GSTPercent	    = frmReceiveStockItem->GSTPercent;
		NodeData->StocktakeUnit	    = frmReceiveStockItem->StocktakeUnit;
		NodeData->SupplierCode	    = frmReceiveStockItem->SupplierCode;
		NodeData->SupplierUnit	    = frmReceiveStockItem->SupplierUnit;
		NodeData->DisplayedUnit     = frmReceiveStockItem->SupplierUnit;
		NodeData->SupplierUnitCost  = frmReceiveStockItem->SupplierUnitCost;
		NodeData->SupplierUnitSize  = frmReceiveStockItem->SupplierUnitSize;
		NodeData->LatestCost =  frmReceiveStockItem->LatestCost;
		NodeData->OnHandQty  =  frmReceiveStockItem->OnHandQty;
		NodeData->SupplierUnitsToReceive = 0;
		NodeData->OrderQty	    = frmReceiveStockItem->SupplierUnitQty;
		NodeData->BackOrder	    = 0;
		NodeData->Code		    = frmReceiveStockItem->StockCode;
		NodeData->StockKey	    = frmReceiveStockItem->StockKey;
		NodeData->Initialised	    = frmReceiveStockItem->Initialised;
		NodeData->InitialisedTime   = frmReceiveStockItem->InitialisedTime;
		NodeData->SupplierKey  = frmReceiveStockItem->SupplierKey;
		NodeData->SupplierTotalCost = NodeData->SupplierUnitCost  * NodeData->OrderQty;
		NodeData->IsUnitChanged = false;
		NodeData->IsUnitEditable = true;
		vtvStockQty->FocusedNode    = OrderItemNode;
		vtvStockQty->FocusedColumn  = 4;
		vtvStockQty->Selected[OrderItemNode]	= true;
	}
	ShowTotals();
	qrSupplier->Close();
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::btnRemoveClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	if (vtvStockQty->FocusedNode)
	{
		TInvoiceItemNodeData *NodeData	= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
		if (NodeData->PurchaseStockKey)
		{
			Application->MessageBox("This item is part of a purchase order and cannot be removed.\r"
			"Set 'Quantity' to 0 and 'Back Order' to 0 if no items will be received.",
			"Error", MB_ICONERROR + MB_OK);
		}
		else
		{
			if (Application->MessageBox(("Do you wish to remove the item \"" + NodeData->Text + "\" from the invoice?").c_str(),
						"Update?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
			{
				vtvStockQty->DeleteNode(vtvStockQty->FocusedNode);

                qrDeleteStock->Close();
                qrDeleteStock->ParamByName("TRANS_NUMBER")->AsInteger= NodeData->TransactionNumber;
                qrDeleteStock->ExecQuery();

				ShowTotals();
			}
		}
	}
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
void TfrmReceiveInvoice::UpdatePackingSlip()
{
}
void TfrmReceiveInvoice::ProcessPackingSlip()
{
    TDateTime invoice_created_date = 0.0;
	// Verify that totals for the invoice have been entered correctly, extra cautious as the packing slip will be carrying 0 amounts as of now.
	if (CheckInvoice())
	{
		bool Initialised = false;
		TDateTime MinInitialiseTime	= 0.0;
		Currency totalCalculatedGST	= 0;
		// Loop through the list. Each item is an invoice item to add to StockTrans.
		std::vector<Stock::TInvoiceItemInfo> InvoiceItems;
		PVirtualNode Node = vtvStockQty->GetFirst();
		while (Node)
		{
			TInvoiceItemNodeData *NodeData	= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(Node);
			if (NodeData->OrderQty != 0 || NodeData->OrderQty + NodeData->BackOrder != NodeData->SupplierUnitsToReceive)
			{
				Stock::TInvoiceItemInfo InvoiceItemInfo;

                qrSupplierKey->Close();
                qrSupplierKey->ParamByName("SUPPLIER_KEY")->AsInteger = NodeData->SupplierKey;
                qrSupplierKey->ParamByName("STOCK_KEY")->AsInteger = NodeData->StockKey;
                qrSupplierKey->Open();
                
				InvoiceItemInfo.Stock_Key = NodeData->StockKey;
				InvoiceItemInfo.Purchase_Stock_Key = NodeData->PurchaseStockKey;
				InvoiceItemInfo.Location = NodeData->Location;
				InvoiceItemInfo.Supplier_Code = NodeData->SupplierCode;
				InvoiceItemInfo.GLCode = NodeData->GLCode;
				InvoiceItemInfo.Description	= NodeData->Text;
				InvoiceItemInfo.Order_Unit = NodeData->DisplayedUnit;
                InvoiceItemInfo.Supplier_Unit = NodeData->SupplierUnit;
                //start changes from v5 to v6
                InvoiceItemInfo.Supplier_Unit_Qty = GetSupplierUnitSize(NodeData->StockKey, NodeData->SupplierKey, NodeData->SupplierUnit);
                InvoiceItemInfo.Supplier_Unit_Size = NodeData->SupplierUnitSize;
                if(InvoiceItemInfo.Supplier_Unit_Qty == 0)
                {
                    InvoiceItemInfo.Supplier_Unit_Qty = qrSupplierKey->FieldByName("QTY")->AsFloat;
                    InvoiceItemInfo.Supplier_Unit_Size = GetStockTakeUnitSize(NodeData->StockKey, NodeData->SupplierKey);
                }
                //End changes from v5 to v6
				InvoiceItemInfo.Order_Qty = NodeData->OrderQty;
				InvoiceItemInfo.BackOrder = NodeData->BackOrder;
				InvoiceItemInfo.TransactionNumber = NodeData->TransactionNumber;
				
				InvoiceItemInfo.Supplier_Unit_Cost = (NodeData->OrderQty != 0) ? NodeData->SupplierTotalCost / NodeData->OrderQty : 0;
                //}
				// Temporarily store entered cost and unit cost. May need GST removed.
				InvoiceItemInfo.Total_Cost = NodeData->SupplierTotalCost;
                invoice_created_date = NodeData->Createdtime;
				if (rbExcludingGST->Checked)
				{
					InvoiceItemInfo.Total_GST = NodeData->SupplierTotalCost * NodeData->GSTPercent / 100;
				}
				else if (rbIncludingGST->Checked)
				{
					InvoiceItemInfo.Total_GST = NodeData->SupplierTotalCost * NodeData->GSTPercent / (100 + NodeData->GSTPercent);
				}
				totalCalculatedGST += InvoiceItemInfo.Total_GST;
				InvoiceItems.push_back(InvoiceItemInfo);
				if (NodeData->Initialised)
				{
					Initialised = true;
					if (NodeData->InitialisedTime != TDateTime(0.0))
					{
						if (NodeData->InitialisedTime < MinInitialiseTime || MinInitialiseTime == TDateTime(0.0))
						{
							MinInitialiseTime = NodeData->InitialisedTime;
						}
					}
				}
			}
			// All done for this item. Get the next one.
			Node = vtvStockQty->GetNext(Node);
		}
		if (InvoiceItems.size() > 0)
		{
			TDateTime BatchTime = TDateTime(int(dtpInvoiceDate->Date) + Time());
			if (Initialised && int(dtpInvoiceDate->Date) == int(MinInitialiseTime))
			{
				// If the stock take was initialised today, set the time before the initialisation time.
				BatchTime = MinInitialiseTime - EncodeTime(0, 1, 0, 0);
			}
			if (Initialised && MinInitialiseTime < BatchTime && MinInitialiseTime != TDateTime(0.0))
			{
				// Must be before the initialisation date/time!
				Application->MessageBox(AnsiString("As some items are initialised for a stock take, you must select a date prior to " +
				MinInitialiseTime.DateTimeString()).c_str(), "Error", MB_OK + MB_ICONERROR);
			}
			else
			{
				TInvoiceSummary invoiceSummary;
				CalculateInvoiceTotals(invoiceSummary);
				Currency totalEnteredGST = invoiceSummary.TotalGST;
				if (rbExcludingGST->Checked)
				{
					// Totals entered don't include gst. Adding a random amount of gst won't effect the total excl..............
					for (unsigned i = 0; i < InvoiceItems.size(); i++)
					{
						if (i < InvoiceItems.size()-1)
						{
							// Each item except last
							Currency Error = 0;
							if (totalCalculatedGST != 0)
							{
								// Calculated GST * Fraction incorrect compared to entered
								Error = (InvoiceItems[i].Total_GST * (totalCalculatedGST - totalEnteredGST)) / totalCalculatedGST;
							}
							totalCalculatedGST -= InvoiceItems[i].Total_GST;
							// Always round to 2dp. Last item will pick up any remaining fractional cents if necessary.
							InvoiceItems[i].Total_GST = MMMath::RoundCurrency(InvoiceItems[i].Total_GST - Error, GSTDecimals);
							totalEnteredGST -= InvoiceItems[i].Total_GST;
						}
						else
						{
							// Last Item gets rounding error.
							InvoiceItems[i].Total_GST = totalEnteredGST;
						}
					}
				}
				else if (rbIncludingGST->Checked)
				{
					// Totals entered already include gst. Current cost + a random amount of gst must equal the total entered!
					for (unsigned i=0; i<InvoiceItems.size(); i++)
					{
						if (i < InvoiceItems.size()-1)
						{
							// Each item except last
							Currency Error = 0;
							if (totalCalculatedGST != 0)
							{
								// Calculated GST * Fraction incorrect compared to entered
								Error = (InvoiceItems[i].Total_GST * (totalCalculatedGST - totalEnteredGST)) / totalCalculatedGST;
							}
							totalCalculatedGST -= InvoiceItems[i].Total_GST;
							// Always round to 2dp. Last item will pick up any remaining fractional cents if necessary.
							InvoiceItems[i].Total_GST = MMMath::RoundCurrency(InvoiceItems[i].Total_GST - Error, GSTDecimals);
							totalEnteredGST -= InvoiceItems[i].Total_GST;
						}
						else
						{
							// Last Item gets rounding error.
							InvoiceItems[i].Total_GST = totalEnteredGST;
						}
						InvoiceItems[i].Total_Cost = InvoiceItems[i].Total_Cost - InvoiceItems[i].Total_GST;
						if (InvoiceItems[i].Order_Qty != 0)
						{
							InvoiceItems[i].Supplier_Unit_Cost = InvoiceItems[i].Total_Cost / InvoiceItems[i].Order_Qty;
						}
						else
						{
							InvoiceItems[i].Supplier_Unit_Cost = 0; // Wont be used.
						}
					}
				}
				Stock::TReceiveInvoice CreateInvoice(dmStockData->dbStock);
				Stock::TTransactionBatchInfo BatchInfo(Stock::ttReceipt);
                qrSupplier->Close();
                qrSupplier->ParamByName("Supplier_Key")->AsInteger = SupplierKey;
				qrSupplier->Open();
				BatchInfo.User_ID = frmLogin->CurrentUserID;
				BatchInfo.User_Name	= frmLogin->CurrentUserName;
				BatchInfo.Created = BatchTime;
				BatchInfo.Supplier_Key = qrSupplier->FieldByName("Contact_LK")->AsInteger;
				BatchInfo.Supplier_Name	= qrSupplier->FieldByName("Company_Name")->AsString;
				BatchInfo.Supplier_ID = qrSupplier->FieldByName("Supplier_ID")->AsString;
				BatchInfo.Export_1 = qrSupplier->FieldByName("MYOB1")->AsString;
				BatchInfo.Export_2 = qrSupplier->FieldByName("MYOB2")->AsString;
				BatchInfo.Export_3 = qrSupplier->FieldByName("MYOB3")->AsString;
				BatchInfo.Order_Number = (OrderKey == 0)?AnsiString(""):qrPurchaseOrder->FieldByName("Order_Number")->AsString;
				BatchInfo.Reference	= InvoiceReference;
				BatchInfo.Total_Cost = invoiceSummary.TotalExcl;
				BatchInfo.Total_GST	= invoiceSummary.TotalGST;
				BatchInfo.BatchID = BatchKey;
                if(IsSavedPackingSlip)
                    BatchInfo.IsCommitedPS =true;
                else
                 BatchInfo.IsCommitedPS =false;
				if (CreateInvoice.RecievePackingSlip(BatchInfo, InvoiceItems))
				{
					if (OrderKey != 0)
					{
						Stock::TPurchaseOrdersControl PurchaseOrdersControl(dmStockData->dbStock);
						if (PurchaseOrdersControl.OrderComplete(OrderKey))
						{
							PurchaseOrdersControl.CloseOrder(OrderKey);
						}
					}
					if (Transaction->InTransaction)
					Transaction->Commit();
					qrSupplier->Close();
					// all database operations have completed successfully, do export to Xero if configured.
					bool Registered = false;
					unsigned int Modules = 0;
					dmMMData->Registered(&Registered, &Modules);
					if (Modules & eAccounting) {
						if( ( CurrentConnection.AccountSystem == "Xero" ) && ( XeroIntegrationIsInstalled() ) )
						{
							if( TXeroInvoiceBuilder::Instance().BuildXeroInvoice( BatchInfo.Supplier_Name, InvoiceReference, "", dtpInvoiceDate->Date, dtpInvoiceDate->Date, InvoiceItems ) )
							{
								SendInvoiceToXero( TXeroInvoiceBuilder::Instance().XeroInvoice );
							}
						}
					}
                    if ( !(dtpInvoiceDate->DateTime  == invoice_created_date))
                      {

                          if(Transaction->InTransaction)
                               Transaction->StartTransaction();

                              //updating Stock Trans table

                              qrUpdateDateInStockTrans->Close();
                              dtpInvoiceDate->Time = Now().CurrentTime();
                              qrUpdateDateInStockTrans->ParamByName("CREATED")->AsDateTime	=dtpInvoiceDate->DateTime ;
                              qrUpdateDateInStockTrans->ParamByName("BATCH_KEY")->AsInteger	=BatchKey ;
                              qrUpdateDateInStockTrans->ExecSQL();


                             //updating transaction batch table
                           qrUpdateDateInStockTrans->Close();
                           qrUpdateDateInTransactionBatch->ParamByName("CREATED")->AsDateTime	=dtpInvoiceDate->DateTime ;

                           qrUpdateDateInTransactionBatch->ParamByName("BATCH_KEY")->AsInteger	=BatchKey ;

                           qrUpdateDateInTransactionBatch->ParamByName("REFERENCE")->AsString	=InvoiceReference ;

                           qrUpdateDateInTransactionBatch->ExecSQL();

                           Transaction->Commit() ;


                      }



					ModalResult = mrOk;
				}
				else
				{
					Application->MessageBox("An error occured creating the invoice.", "Error", MB_ICONERROR + MB_OK);
				}
			}
		}
		else
		{
			Application->MessageBox("There are no items in this invoice.", "Error", MB_ICONERROR + MB_OK);
		}
	}
    	if (Transaction->InTransaction)
					Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::btnOkClick(TObject *Sender)
{
   // IsPackingSlipUpdateMode=false;
	CommitInvoice();
    if(IsPrintReport)
    {
    	if (Transaction->InTransaction)
					Transaction->Commit();
    }
}

//---------------------------------------------------------------------------
bool TfrmReceiveInvoice::CheckInvoice()
{
	bool Continue  = true;
	PVirtualNode Node = vtvStockQty->GetFirst();
	while (Node && Continue)
	{
		TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(Node);
		if (NodeData->SupplierUnitSize == 0)
		{
			try
			{
				vtvStockQty->OnEdited = NULL;
				vtvStockQty->FocusedNode = Node;
				vtvStockQty->FocusedColumn = 4;
				vtvStockQty->Selected[Node] = true;
				Continue = false;
				Application->MessageBox("This product's supplier unit is incorrect. You must remove this item and rectify this problem before adding.", "Error", MB_ICONERROR + MB_OK);
			}
			__finally
			{
				vtvStockQty->OnEdited = vtvStockQtyEdited;
			}
		}
		// All done. Get the next one.
		Node = vtvStockQty->GetNext(Node);
	}
	if (Continue)
	{
		AnsiString Message = "Please confirm that the total for invoice \"" + InvoiceReference + "\" is correct.\r\rContinue?";
		Continue = (Application->MessageBox(Message.c_str(), "Continue?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK);
	}
    if(!Continue)
    {
       IsPrintReport = false;
    }

	return Continue;
}
bool TfrmReceiveInvoice::CheckPackingSlip()
{
	bool isDataCorrect = true;
	PVirtualNode Node = vtvStockQty->GetFirst();
	while (Node && isDataCorrect)
	{
		TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(Node);
		if (NodeData->SupplierUnitSize == 0)
		{
			try
			{
				vtvStockQty->OnEdited = NULL;
				vtvStockQty->FocusedNode = Node;
				vtvStockQty->FocusedColumn = 4;
				vtvStockQty->Selected[Node] = true;
				isDataCorrect = false;
				Application->MessageBox("This product's supplier unit is incorrect. You must remove this item and rectify this problem before adding.", "Error", MB_ICONERROR + MB_OK);
			}
			__finally
			{
				vtvStockQty->OnEdited = vtvStockQtyEdited;
			}
		}
		// All done. Get the next one.
		Node = vtvStockQty->GetNext(Node);
	}
	return isDataCorrect;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::neStockQtyKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN || Key == VK_DOWN || Key == VK_UP)
	{
		vtvStockQty->BeginUpdate();
		try
		{
			vtvStockQty->EndEditNode();
		}
		__finally
		{
			vtvStockQty->EndUpdate();
		}
		if (Key == VK_RETURN && (vtvStockQty->FocusedColumn == 4 || vtvStockQty->FocusedColumn == 5 || vtvStockQty->FocusedColumn == 6))
		{
			vtvStockQty->FocusedColumn++;
			vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
		}
		else if (Key == VK_DOWN || (Key == VK_RETURN && vtvStockQty->FocusedColumn == 7))
		{
			if (vtvStockQty->GetNext(vtvStockQty->FocusedNode))
			{
				if (Key == VK_RETURN)
				{
					vtvStockQty->FocusedColumn = 4;
				}
				try
				{
					vtvStockQty->OnEdited = NULL;
					vtvStockQty->FocusedNode = vtvStockQty->GetNext(vtvStockQty->FocusedNode);
				}
				__finally
				{
					vtvStockQty->OnEdited = vtvStockQtyEdited;
				}
				vtvStockQty->Selected[vtvStockQty->FocusedNode] = true;
			}
			else
			{
				if (OrderKey == 0)
				{
					btnAddClick(NULL);
				}
				else
				{
					vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
				}
			}
		}
		else if (Key == VK_UP)
		{
			if (vtvStockQty->GetPrevious(vtvStockQty->FocusedNode))
			{
				vtvStockQty->FocusedNode = vtvStockQty->GetPrevious(vtvStockQty->FocusedNode);
				vtvStockQty->Selected[vtvStockQty->FocusedNode] = true;
			}
			else
			{
				vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
			}
		}
	}
	else if (Key == VK_ESCAPE)
	{
		vtvStockQty->CancelEditNode();
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::neStockQtyKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
	}
    if ((Key == '-') && !AllowNegativeValue)
	{
		Key = NULL;
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::neTotalCostExit(TObject *Sender)
{
	vtvStockQty->BeginUpdate();
	try
	{
		vtvStockQty->EndEditNode();
        CalculateQtyValue();
	}
	__finally
	{
		vtvStockQty->EndUpdate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::neCostExit(TObject *Sender)
{
	vtvStockQty->BeginUpdate();
	try
	{
		vtvStockQty->EndEditNode();
		TInvoiceItemNodeData *NodeData	= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
		// if they alter the supplier cost on the way thru then alter the total cost accordingly
		// Total Cost
		NodeData->SupplierTotalCost = NodeData->SupplierUnitCost  * NodeData->OrderQty;
		neTotalCost->Text = FloatToStr(NodeData->SupplierTotalCost);
        //RichEdit1->Text = NodeData->SupplierTotalCost;
	}
	__finally
	{
		vtvStockQty->EndUpdate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::neStockQtyExit(TObject *Sender)
{
	vtvStockQty->BeginUpdate();
	try
	{
		vtvStockQty->EndEditNode();
		// set the SupplierTotalCost and SupplierTotalGST
		TInvoiceItemNodeData *NodeData	= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
		// Total Cost
		NodeData->SupplierTotalCost = NodeData->SupplierUnitCost  * NodeData->OrderQty;
	}
	__finally
	{
		vtvStockQty->EndUpdate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::neGSTKeyPress(TObject *Sender,char &Key)
{
	if(Key == VK_RETURN || Key == '-')
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::neGSTKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN)
	{
		btnOk->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::neGSTExit(TObject *Sender)
{
	ShowTotals();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::chbGSTOverrideClick(TObject *Sender)
{
    reGstValue->Enabled =  chbGSTOverride->Checked;
    if(reGstValue->Enabled)
    {
      reGstValue->Color = clWindow;
    }
	ShowTotals();
}

void TfrmReceiveInvoice::InitXeroIntegration()
{
	TXeroIntegration::Instance().XeroMachineName = LocalHost();
	TXeroIntegration::Instance().XeroFolderPath  = ReadXeroFolderPath();
	TXeroIntegration::Instance().XeroUserName    = "";
	TXeroIntegration::Instance().XeroPassword    = "";

	TXeroIntegration::Instance().Validate();
}
//---------------------------------------------------------------------------

AnsiString TfrmReceiveInvoice::LocalHost()
{
	return TRemoteHostManager::Instance().LocalHost();
}
//---------------------------------------------------------------------------

bool TfrmReceiveInvoice::XeroIntegrationIsInstalled()
{
	return TXeroIntegration::Instance().XeroIntegrationIsInstalled();
}
//---------------------------------------------------------------------------
bool TfrmReceiveInvoice::SendInvoiceToXero( TXeroInvoice *inXeroInvoice  )
{
	bool result = TXeroIntegration::Instance().SendInvoice( inXeroInvoice );

	if( !result )
	{
		AnsiString errorMsg = "Failed to send invoice to Xero! " + TXeroIntegration::Instance().XeroErrorMessage;
		Application->MessageBox( errorMsg.c_str(), AnsiString("Error").c_str(), MB_OK | MB_ICONWARNING);
	}

	return result;
}
//---------------------------------------------------------------------------
AnsiString TfrmReceiveInvoice::ReadXeroFolderPath()
{
	AnsiString result = "";

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		AnsiString Key = OfficeKey + "\\" + ReadCompanyName() + "\\MYOB";
		RegistryRead( Key, "MYOBPath", result );
	}
	__finally
	{
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
AnsiString TfrmReceiveInvoice::ReadCompanyName()
{
	AnsiString result = "";

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	TStringList *companyNames = new TStringList;

	try
	{
		if( RegistryGetKeys( OfficeKey, companyNames ) && ( companyNames->Count > 0 ) )
		{
			result = companyNames->Strings[0];
		}
	}
	__finally
	{
		delete companyNames;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}

void __fastcall TfrmReceiveInvoice::btnCommitPackingSlipClick(TObject*Sender)
{
    if(CheckInvoiceQtyAndPrice())
    {
        CommitPackingSlip(true);
    }
}

//---------------------------------------------------------------------------


void __fastcall TfrmReceiveInvoice::btnPrintInvoiceClick(TObject *Sender)
{
	if(!dmStockReportData->StockTrans->InTransaction)
	dmStockReportData->StockTrans->StartTransaction();

	try
	{
		dmStockReportData->SetupInvoices(BatchKey);
		if(frmReports->rvStock->SelectReport("repCommitedInvoices", false))
		{
			frmReports->rvStock->Execute();
		}
	}
	__finally
	{
		dmStockReportData->StockTrans->Commit();
	}
}
//---------------------------------------------------------------------------
// load unit
void TfrmReceiveInvoice::LoadUnits(int stockKey)
{
	StockUnits = new TStringList;
	dbcbUnit->Items->Clear();
	std::auto_ptr<TIBQuery> query(new TIBQuery(NULL));
	query->Database = dmStockData->dbStock;
	query->Transaction = Transaction;
	try
	{
		if (!Transaction->InTransaction)
		Transaction->StartTransaction();

		qrPurchaseOrder->Close();
		qrPurchaseOrder->ParamByName("Order_Key")->AsInteger = OrderKey;
		qrPurchaseOrder->Open();



		query->SQL->Text = "select distinct SUPPLIER_UNIT, QTY  from SUPPLIERSTOCK Where STOCK_KEY = :STOCK_KEY and SUPPLIERSTOCK.SUPPLIER_KEY=:SUPPLIER_KEY union "
		"select distinct STOCKTAKE_UNIT SUPPLIER_UNIT, 1 as QTY   from STOCK left join SUPPLIERSTOCK on SUPPLIERSTOCK.STOCK_KEY=STOCK.STOCK_KEY Where STOCK.STOCK_KEY = :STOCK_KEY and SUPPLIERSTOCK.SUPPLIER_KEY=:SUPPLIER_KEY  ";
		query->ParamByName("STOCK_KEY")->AsInteger = stockKey;

		if((qrPurchaseOrder->FieldByName("Contact_FK")->AsInteger)==0)
		{query->ParamByName("SUPPLIER_KEY")->AsInteger=SupplierKey;}
		else
		query->ParamByName("SUPPLIER_KEY")->AsInteger = qrPurchaseOrder->FieldByName("Contact_FK")->AsInteger;
		for (query->Open(); !query->Eof; query->Next())
		{
			if (query->FieldByName("SUPPLIER_UNIT")->AsString != "")
			{
				dbcbUnit->Items->Add(query->FieldByName("SUPPLIER_UNIT")->AsString);
				StockUnits->AddObject(query->FieldByName("SUPPLIER_UNIT")->AsString,(TObject *)query->FieldByName("QTY")->AsInteger );
			}
		}

	}
	catch (Exception &e)
	{
	}

}
//..........................................................................................
// load locations
void TfrmReceiveInvoice::LoadLocations(int stockKey)
{
	Locations = new TStringList;
	dbcbLocation->Items->Clear();
	std::auto_ptr<TIBQuery> query(new TIBQuery(NULL));
	query->Database = dmStockData->dbStock;
	query->Transaction = Transaction;
	try
	{
		query->SQL->Text = "select distinct LOCATION from STOCKLOCATION "
		"Where STOCK_KEY = :STOCK_KEY "
		"Order By "
		"LOCATION";
		query->ParamByName("STOCK_KEY")->AsInteger = stockKey;
		for (query->Open(); !query->Eof; query->Next())
		{
			if (query->FieldByName("LOCATION")->AsString != "")
			{
				dbcbLocation->Items->Add(query->FieldByName("LOCATION")->AsString);
				Locations->Add(query->FieldByName("LOCATION")->AsString);
			}
		}
	}
	catch (Exception &e)
	{
	}
}

void __fastcall TfrmReceiveInvoice::dbcbUnitChange(TObject *Sender)
{
	/*************************************************/
    bool calculateStockunit = false;
	qrPurchaseOrder->Close();
	qrSupplier->Close();
	qrSupplierOrderQty->Close();
	qrPurchaseStock->Close();
	qrPurchaseStockAuto->Close();
	qrPurchaseOrder->ParamByName("Order_Key")->AsInteger = OrderKey;
	qrPurchaseOrder->Open();
	qrSupplier->ParamByName("Supplier_Key")->AsInteger = qrPurchaseOrder->FieldByName("Contact_FK")->AsInteger;
	qrSupplier->Open();
	qrPurchaseStock->ParamByName("Order_Key")->AsInteger = OrderKey;
	qrPurchaseStock->Open();
	AnsiString Stockkey=    qrPurchaseStock->FieldByName("Stock_Key")->AsString;
	vtvStockQty->BeginUpdate();
	try
	{
		vtvStockQty->EndEditNode();
		TInvoiceItemNodeData *NodeData	= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
		//  AnsiString Locationsel=  NodeData->Location;

		qrPurchaseStockAuto->ParamByName("Order_Key")->AsInteger = OrderKey;
		qrPurchaseStockAuto->ParamByName("STOCK_KEY")->AsInteger = NodeData->StockKey;;
        qrPurchaseStockAuto->ParamByName("Supplier_Unit")->AsString = dbcbUnit->Items->Strings[dbcbUnit->ItemIndex];
		qrPurchaseStockAuto->Open();
		qrPurchaseStockAuto->First();

		qrSupplierOrderQty->ParamByName("SUPPLIER_KEY")->AsString = qrPurchaseOrder->FieldByName("Contact_FK")->AsInteger;
		qrSupplierOrderQty->ParamByName("Supplier_Unit")->AsString = dbcbUnit->Items->Strings[dbcbUnit->ItemIndex];
		qrSupplierOrderQty->ParamByName("STOCK_KEY")->AsInteger = 	NodeData->StockKey;

		qrSupplierOrderQty->Open();
		qrSupplierOrderQty->First();
		if(qrSupplierOrderQty ->RecordCount <=0)
		{
			qrSupplierOrderQty->Close();
			qrSupplierOrderQty->ParamByName("SUPPLIER_KEY")->AsString = qrPurchaseOrder->FieldByName("Contact_FK")->AsInteger;
			qrSupplierOrderQty->ParamByName("Supplier_Unit")->AsString = 	 qrPurchaseStockAuto->FieldByName("Supplier_Unit")->AsString;
			qrSupplierOrderQty->ParamByName("STOCK_KEY")->AsString = NodeData->StockKey;

			qrSupplierOrderQty->Open();
			qrSupplierOrderQty->First();
			if(qrPurchaseStockAuto->FieldByName("Supplier_Unit_Size")->AsFloat>0)
			{
				NodeData->SupplierUnitCost   = qrPurchaseStockAuto->FieldByName("Supplier_Unit_Cost")->AsFloat/qrPurchaseStockAuto->FieldByName("Supplier_Unit_Size")->AsFloat;
			}
			else
            {
                //change here not to update item price in case of saved packing slip...
                if(!IsPackingSlipUpdateMode)
                {
			       NodeData->SupplierUnitCost=0;
                }
                else
                {
                    if(GetSupplierUnitCost(NodeData->SupplierUnit, NodeData->StockKey, BatchKey, NodeData) <= 0)
                    {
                       NodeData->SupplierUnitCost=0;
                    }
                }
            }
		}
		else
		{
           NodeData->SupplierUnitCost = qrPurchaseStockAuto->FieldByName("Supplier_Unit_Cost")->AsFloat;
           if(NodeData->SupplierUnitCost > 0)
           {
              NodeData->SupplierUnit = qrPurchaseStockAuto->FieldByName("Supplier_Unit")->AsString; //Supplier_Unit
              //UpdateSupplierUnitCost(OrderKey, NodeData);
           }
		}

		if(NodeData->SupplierUnitCost==0)
		{
            UpdateSupplierUnit(NodeData); // update node qty..
		}

		if(NodeData->SupplierUnitCost==0)
		{
            calculateStockunit = true;
            UpdateUnitqty(NodeData);
		}
        neCost->Value = NodeData->SupplierUnitCost;
        double unitCost = NodeData->SupplierUnitCost;
        double QtyToReceive = NodeData->SupplierUnitsToReceive;
        double newSupplierUnitSize = (int)StockUnits->Objects[dbcbUnit->ItemIndex];
        unitCost = unitCost * newSupplierUnitSize; // /NodeData->SupplierUnitSize;

        UpdateNodeQty(NodeData, QtyToReceive, newSupplierUnitSize); // update node qty..

		if(IsPackingSlipUpdateMode)
        {
            if(!calculateStockunit)
            {
              NodeData->SupplierUnitCost = 0;
            }
            GetSupplierUnitCost(dbcbUnit->Items->Strings[dbcbUnit->ItemIndex], NodeData->StockKey, BatchKey, NodeData);
            if(!CheckSupplierUnit(NodeData->StockKey, NodeData->SupplierKey, dbcbUnit->Items->Strings[dbcbUnit->ItemIndex]))
            {
                if(!calculateStockunit)
                {
                    if(NodeData->DisplayedUnit != NodeData->SupplierUnit)
                    {
                        if(NodeData->SupplierUnitCost == unitCost)
                        {
                           NodeData->SupplierUnitCost = unitCost * NodeData->OrderQty;
                           if(NodeData->SupplierUnitCost > NodeData->SupplierTotalCost)
                           {
                              NodeData->SupplierUnitCost = unitCost / NodeData->OrderQty;
                           }
                        }
                        else
                        {
                           if(unitCost > NodeData->SupplierTotalCost)
                           {
                              NodeData->SupplierUnitCost = neCost->Value;
                           }
                           else
                           {
                              if((unitCost * NodeData->OrderQty) != NodeData->SupplierTotalCost)
                              {
                                 NodeData->SupplierUnitCost = NodeData->SupplierTotalCost / NodeData->OrderQty;
                              }
                              else
                              {
                                 //double itemQty = GetSupplierUnitSize(NodeData->StockKey, NodeData->SupplierKey, NodeData->SupplierUnit);
                                 if(QtyToReceive > 0)
                                 {
                                     //NodeData->SupplierUnitCost = unitCost/ QtyToReceive;
                                     UpdateUnitqty(NodeData);
                                     NodeData->SupplierTotalCost = NodeData->SupplierUnitCost  * NodeData->OrderQty;
                                 }
                                 else
                                 {
                                    NodeData->SupplierUnitCost = unitCost;
                                 }
                              }
                           }
                        }
                      }
                 }
               }
                else
                {
                    if(NodeData->SupplierUnitCost == 0)
                    {
                        UpdateSupplierUnit(NodeData);
                        if(NodeData->SupplierUnitCost > 0)
                        {
                            //NodeData->SupplierUnit = NodeData->DisplayedUnit;     // update node qty..
                            if(NodeData->OrderQty == 0)
                            {
                               UpdateNodeQty(NodeData, QtyToReceive, newSupplierUnitSize); // update node qty..
                            }
                            NodeData->SupplierTotalCost = NodeData->SupplierUnitCost  * NodeData->OrderQty;
                        }
                    }

                }
            }

		if (NodeData->BackOrder < 0)
		{
			NodeData->BackOrder = 0;
		}

		ShowTotals();
	}
	__finally
	{
		vtvStockQty->EndUpdate();
	}
}

//---------------------------------------------------------------------------
void TfrmReceiveInvoice::UpdatePurchaseOrder()
{
	try
	{
		if(!Transaction->InTransaction)
		Transaction->StartTransaction();
		qrUpdateOrder->SQL->Text = "UPDATE PURCHASESTOCK a "
		"Set a.SUPPLIER_UNIT = :SUPPLIER_UNIT, "
		"a.SUPPLIER_UNIT_QTY = a.SUPPLIER_UNIT_QTY * a.SUPPLIER_UNIT_SIZE / :NEW_SUPPLIER_UNIT_SIZE, "
		"a.QTY_RECEIVED = a.QTY_RECEIVED * a.SUPPLIER_UNIT_SIZE / :NEW_SUPPLIER_UNIT_SIZE, "
		"a.QTY_ON_ORDER = a.QTY_ON_ORDER * a.SUPPLIER_UNIT_SIZE / :NEW_SUPPLIER_UNIT_SIZE, "
		"a.SUPPLIER_UNIT_COST = a.SUPPLIER_UNIT_COST * :NEW_SUPPLIER_UNIT_SIZE /a.SUPPLIER_UNIT_SIZE, "
		"a.SUPPLIER_UNIT_SIZE = :NEW_SUPPLIER_UNIT_SIZE, "
		"a.SUPPLIER_UNIT_CHANGED = 'T' "
		"where a.PURCHASE_STOCK_LK = :PURCHASE_STOCK_LK ";
		PVirtualNode Node = vtvStockQty->GetFirst();
		while(Node)
		{
			TInvoiceItemNodeData* NodeData = (TInvoiceItemNodeData*)vtvStockQty->GetNodeData(Node);
			if(NodeData->IsUnitChanged)
			{
				qrUpdateOrder->Close();
				qrUpdateOrder->ParamByName("SUPPLIER_UNIT")->AsString = NodeData->DisplayedUnit;
				qrUpdateOrder->ParamByName("NEW_SUPPLIER_UNIT_SIZE")->AsFloat = NodeData->SupplierUnitSize;
				qrUpdateOrder->ParamByName("PURCHASE_STOCK_LK")->AsString = NodeData->PurchaseStockKey;
				qrUpdateOrder->ExecQuery();
			}
			Node = vtvStockQty->GetNext(Node);
		}
		Transaction->Commit();

	}
	__finally
	{
		if(!Transaction->InTransaction)
		Transaction->StartTransaction();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveInvoice::dbcbLocationChange(TObject *Sender)
{        qrLocationUpdate->Close();
	qrPurchaseOrder->Close();
	qrSupplier->Close();
	qrSelectLocationUpdate->Close();
	qrPurchaseStock->Close();
	qrPurchaseOrder->ParamByName("Order_Key")->AsInteger = OrderKey;
	qrPurchaseOrder->Open();
	qrSupplier->ParamByName("Supplier_Key")->AsInteger = qrPurchaseOrder->FieldByName("Contact_FK")->AsInteger;
	qrSupplier->Open();
	qrPurchaseStock->ParamByName("Order_Key")->AsInteger = OrderKey;
	qrPurchaseStock->Open();
	AnsiString Stockkey=    qrPurchaseStock->FieldByName("Stock_Key")->AsString;
	AnsiString Supplier_Unit=qrPurchaseStock->FieldByName("Supplier_Unit")->AsString;
	vtvStockQty->BeginUpdate();
	try{
		vtvStockQty->EndEditNode();
		TInvoiceItemNodeData *NodeData	= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
		AnsiString Locationsel=  NodeData->Location;

		qrLocationUpdate->ParamByName("SUPPLIER_KEY")->AsInteger = SupplierKey;
		qrLocationUpdate->ParamByName("LOCATION")->AsString = NodeData->Location;
        qrLocationUpdate->ParamByName("QTY")->AsString = GetSupplierUnitSize(NodeData->StockKey, NodeData->SupplierKey, NodeData->SupplierUnit);
		qrLocationUpdate->ParamByName("STOCK_KEY")->AsInteger = NodeData->StockKey;
		qrLocationUpdate->Open();
		qrLocationUpdate->First();

		//	NodeData->OrderQty					= qrSelectLocationUpdate->FieldByName("Qty_On_Order")->AsFloat;
		NodeData->LatestCost			      = qrLocationUpdate->FieldByName("LATEST_COST")->AsFloat;
		NodeData->OnHandQty			      = qrLocationUpdate->FieldByName("ON_HAND")->AsFloat;

		NodeData->BackOrder				= NodeData->SupplierUnitsToReceive - NodeData->OrderQty;

		if (NodeData->BackOrder < 0)
		{
			NodeData->BackOrder = 0;
		}
	}
	__finally
	{
		vtvStockQty->EndUpdate();
	}
}

//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::btnPrintCommitInvoiceClick(TObject *Sender)
{
        CommitInvoice();
      /* if(BatchKey==0)
        {
        qrBatchKey->Close();
        qrBatchKey->SQL->Text = "Select Gen_id(Gen_Stocktrans_Batch_Key, 1) From rdb$database";
        qrBatchKey->Open();
        }      */
         if(IsPrintReport)
         {
            if (dmStockReportData->StockTrans->DefaultDatabase->Connected && !dmStockReportData->StockTrans->InTransaction)
            {
             dmStockReportData->StockTrans->StartTransaction();
            }
             dmStockReportData->SetupSupplierPurchasesRuntime(ReceiptNumber, SupplierName, InvoiceReference);;
            if (frmReports->rvStock->SelectReport("repSupplierPurchaseStock", false))
            {
            
                frmReports->rvStock->SetParam("CURRENTUSER", frmLogin->CurrentUser.UserID +" at  "+ Now());
                frmReports->rvStock->SetParam("CompanyName", CurrentConnection.CompanyName);


             frmReports->rvStock->Execute();
            }
            if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
            {
             dmStockReportData->StockTrans->Commit();
            }
        }

        	if (Transaction->InTransaction && IsNegativeQtyOrCost)
					Transaction->Commit();
}
//---------------------------------------------------------------------------
 //---------------------------------------------------------------------------
void  TfrmReceiveInvoice::CommitInvoice()
{
    IsPrintReport=true;
	if(vtvStockQty->IsEditing())
	vtvStockQty->EndEditNode();
    if(!CheckInvoiceQtyAndPrice())
    {
        IsPrintReport = false;
        return;
    }
	if(IsPackingSlipUpdateMode)
	{
		//If its a packing slip we only need to update the cost and average prices for the items. No more calculations are required.
		ProcessPackingSlip();
	}
	else if (CheckInvoice())
	{
		bool Initialised					= false;
		TDateTime MinInitialiseTime	= 0.0;
		Currency totalCalculatedGST	= 0;

		// Loop through the list. Each item is an invoice item to add to StockTrans.
		std::vector<Stock::TInvoiceItemInfo> InvoiceItems;
		PVirtualNode Node = vtvStockQty->GetFirst();
		while (Node)
		{
			TInvoiceItemNodeData *NodeData	= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(Node);
			if (NodeData->OrderQty != 0 || NodeData->OrderQty + NodeData->BackOrder != NodeData->SupplierUnitsToReceive)
			{
				qrSupplierKey->Close();
                qrSupplierKey->ParamByName("SUPPLIER_KEY")->AsInteger = NodeData->SupplierKey;
                qrSupplierKey->ParamByName("STOCK_KEY")->AsInteger = NodeData->StockKey;
                qrSupplierKey->Open();
                AnsiString SUPPLIER_UNIT=    qrSupplierKey->FieldByName("SUPPLIER_UNIT")->AsString;
				Stock::TInvoiceItemInfo InvoiceItemInfo;
				InvoiceItemInfo.Stock_Key = NodeData->StockKey;
				InvoiceItemInfo.Purchase_Stock_Key = NodeData->PurchaseStockKey;
				InvoiceItemInfo.Location = NodeData->Location;
				InvoiceItemInfo.Supplier_Code = NodeData->SupplierCode;
				InvoiceItemInfo.GLCode      = NodeData->GLCode;
				InvoiceItemInfo.Description = NodeData->Text;
				InvoiceItemInfo.Order_Unit  = IsPurchaseOrderMode ? NodeData->SupplierUnit : NodeData->DisplayedUnit;
                InvoiceItemInfo.Supplier_Unit = NodeData->SupplierUnit;
                InvoiceItemInfo.Supplier_Unit_Qty = GetSupplierUnitSize(NodeData->StockKey, NodeData->SupplierKey, NodeData->SupplierUnit);
                if(InvoiceItemInfo.Supplier_Unit_Qty == 0)
                {
                   InvoiceItemInfo.Supplier_Unit_Qty = qrSupplierKey->FieldByName("QTY")->AsFloat;
                   InvoiceItemInfo.Supplier_Unit = SUPPLIER_UNIT;
                }
				InvoiceItemInfo.Order_Qty   = NodeData->OrderQty;
				InvoiceItemInfo.Supplier_Unit_Size = NodeData->SupplierUnitSize;
				InvoiceItemInfo.BackOrder    = NodeData->BackOrder;
				InvoiceItemInfo.TransactionNumber=  NodeData->TransactionNumber;

				if (NodeData->OrderQty != 0)
				{

				   InvoiceItemInfo.Supplier_Unit_Cost = NodeData->SupplierTotalCost / NodeData->OrderQty;
				}
				else
				{
					InvoiceItemInfo.Supplier_Unit_Cost = 0; // Won't be used. No product received.
				}
				InvoiceItemInfo.Total_Cost = NodeData->SupplierTotalCost; // Temporarily store entered cost and unit cost. May need GST removed.
				if (rbExcludingGST->Checked)
				{
					InvoiceItemInfo.Total_GST = NodeData->SupplierTotalCost * NodeData->GSTPercent / 100;
				}
				else if (rbIncludingGST->Checked)
				{
					InvoiceItemInfo.Total_GST = NodeData->SupplierTotalCost * NodeData->GSTPercent / (100 + NodeData->GSTPercent);
				}
				totalCalculatedGST += InvoiceItemInfo.Total_GST;
				InvoiceItems.push_back(InvoiceItemInfo);
				if (NodeData->Initialised)
				{
					Initialised = true;
					if (NodeData->InitialisedTime != TDateTime(0.0))
					{
						if (NodeData->InitialisedTime < MinInitialiseTime || MinInitialiseTime == TDateTime(0.0))
						{
							MinInitialiseTime = NodeData->InitialisedTime;
						}
					}
				}
			}
			// All done for this item. Get the next one.
			Node = vtvStockQty->GetNext(Node);
		}
		if (InvoiceItems.size() > 0)
		{
			TDateTime BatchTime = TDateTime(int(dtpInvoiceDate->Date) + Time());
			if (Initialised && int(dtpInvoiceDate->Date) == int(MinInitialiseTime))
			{
				// If the stock take was initialised today, set the time before the initialisation time.
				BatchTime = MinInitialiseTime - EncodeTime(0, 1, 0, 0);
			}
			if (Initialised && MinInitialiseTime < BatchTime && MinInitialiseTime != TDateTime(0.0))
			{
				// Must be before the initialisation date/time!
				Application->MessageBox(AnsiString("As some items are initialised for a stock take, you must select a date prior to " +
				MinInitialiseTime.DateTimeString()).c_str(), "Error", MB_OK + MB_ICONERROR);
			}
			else
			{
				TInvoiceSummary invoiceSummary;
				CalculateInvoiceTotals(invoiceSummary);
				Currency totalEnteredGST = invoiceSummary.TotalGST;
				if (rbExcludingGST->Checked)
				{
					// Totals entered don't include gst. Adding a random amount of gst won't effect the total excl.
					for (unsigned i=0; i<InvoiceItems.size(); i++)
					{
						if (i < InvoiceItems.size()-1)
						{
							// Each item except last
							Currency Error = 0;
							if (totalCalculatedGST != 0)
							{
								// Calculated GST * Fraction incorrect compared to entered
								Error = (InvoiceItems[i].Total_GST * (totalCalculatedGST - totalEnteredGST)) / totalCalculatedGST;
							}
							totalCalculatedGST -= InvoiceItems[i].Total_GST;
							// Always round to 2dp. Last item will pick up any remaining fractional cents if necessary.
							InvoiceItems[i].Total_GST = MMMath::RoundCurrency(InvoiceItems[i].Total_GST - Error, GSTDecimals);
							totalEnteredGST -= InvoiceItems[i].Total_GST;
						}
						else
						{
							// Last Item gets rounding error.
							InvoiceItems[i].Total_GST = totalEnteredGST;
						}
					}
				}
				else if (rbIncludingGST->Checked)
				{
					// Totals entered already include gst. Current cost + a random amount of gst must equal the total entered!
					for (unsigned i=0; i<InvoiceItems.size(); i++)
					{
						if (i < InvoiceItems.size()-1)
						{
							// Each item except last
							Currency Error = 0;
							if (totalCalculatedGST != 0)
							{
								// Calculated GST * Fraction incorrect compared to entered
								Error = (InvoiceItems[i].Total_GST * (totalCalculatedGST - totalEnteredGST)) / totalCalculatedGST;
							}
							totalCalculatedGST -= InvoiceItems[i].Total_GST;
							// Always round to 2dp. Last item will pick up any remaining fractional cents if necessary.
							InvoiceItems[i].Total_GST = MMMath::RoundCurrency(InvoiceItems[i].Total_GST - Error, GSTDecimals);
							totalEnteredGST -= InvoiceItems[i].Total_GST;
						}
						else
						{
							// Last Item gets rounding error.
							InvoiceItems[i].Total_GST = totalEnteredGST;
						}
						InvoiceItems[i].Total_Cost = InvoiceItems[i].Total_Cost - InvoiceItems[i].Total_GST;
						if (InvoiceItems[i].Order_Qty != 0)
						{
							InvoiceItems[i].Supplier_Unit_Cost = InvoiceItems[i].Total_Cost / InvoiceItems[i].Order_Qty;
						}
						else
						{
							InvoiceItems[i].Supplier_Unit_Cost = 0; // Wont be used.
						}
					}
				}
				if (OrderKey != 0)
				{
					UpdatePurchaseOrder();
					qrPurchaseOrder->Open();
				}

				Stock::TReceiveInvoice			CreateInvoice(dmStockData->dbStock);
				Stock::TTransactionBatchInfo	BatchInfo(Stock::ttReceipt);
                qrSupplier->Close();
                qrSupplier->ParamByName("Supplier_Key")->AsInteger = SupplierKey;
				qrSupplier->Open();
				BatchInfo.User_ID = frmLogin->CurrentUserID;                                //
				BatchInfo.User_Name = frmLogin->CurrentUserName;
				BatchInfo.Created = BatchTime;
				BatchInfo.Supplier_Key	= qrSupplier->FieldByName("Contact_LK")->AsInteger;
				BatchInfo.Supplier_Name	= qrSupplier->FieldByName("Company_Name")->AsString;

                if(BatchInfo.Supplier_Name!="")
                {SupplierName=BatchInfo.Supplier_Name; }
                else{ BatchInfo.Supplier_Name=SupplierName;}
				BatchInfo.Supplier_ID	= qrSupplier->FieldByName("Supplier_ID")->AsString;
				BatchInfo.Export_1 = qrSupplier->FieldByName("MYOB1")->AsString;
				BatchInfo.Export_2 = qrSupplier->FieldByName("MYOB2")->AsString;
				BatchInfo.Export_3 = qrSupplier->FieldByName("MYOB3")->AsString;
				BatchInfo.Order_Number	= (OrderKey == 0)?AnsiString(""):qrPurchaseOrder->FieldByName("Order_Number")->AsString;
				BatchInfo.Reference = InvoiceReference;
				BatchInfo.Total_Cost = invoiceSummary.TotalExcl;//TotalCost;
				BatchInfo.Total_GST = invoiceSummary.TotalGST;//neGST->Value;
                 if(IsSavedPackingSlip)
                BatchInfo.IsCommitedPS =true;
                 else
                 BatchInfo.IsCommitedPS =false;
				if (CreateInvoice.ReceiveInvoice(BatchInfo, InvoiceItems))
				{
					if (OrderKey != 0)
					{
						Stock::TPurchaseOrdersControl PurchaseOrdersControl(dmStockData->dbStock);
						if (PurchaseOrdersControl.OrderComplete(OrderKey))
						{
							PurchaseOrdersControl.CloseOrder(OrderKey);
						}
					}
					if (Transaction->InTransaction)
					{ Transaction->Commit();
					}

					qrSupplier->Close();

					// all database operations have completed successfully, do export to Xero if configured.
					bool Registered = false;
					unsigned int Modules = 0;
					dmMMData->Registered(&Registered, &Modules);
					if (Modules & eAccounting) {
						if( ( CurrentConnection.AccountSystem == "Xero" ) && ( XeroIntegrationIsInstalled() ) )
						{
							if( TXeroInvoiceBuilder::Instance().BuildXeroInvoice( BatchInfo.Supplier_Name, InvoiceReference, "", dtpInvoiceDate->Date, dtpInvoiceDate->Date, InvoiceItems ) )
							{
								SendInvoiceToXero( TXeroInvoiceBuilder::Instance().XeroInvoice );
							}
						}
					}
					ModalResult = mrOk;

             /*  if(BatchKey==0)
              {
                qrBatchKey->Close();
                qrBatchKey->SQL->Text = "Select Gen_id(Gen_Stocktrans_Batch_Key, 1) From rdb$database";
                qrBatchKey->Open();
              }   */


				}
				else
				{
					Application->MessageBox("An error occured creating the invoice.", "Error", MB_ICONERROR + MB_OK);
				}
			}
		}
		else
		{
			Application->MessageBox("There are no items in this invoice.", "Error", MB_ICONERROR + MB_OK);
		}
	}
    else{ IsPrintReport=false;
    }
}

//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice:: btnSaveClick(TObject *Sender)
{
    if(CheckInvoiceQtyAndPrice())
    {
       CommitPackingSlip(false);
    }
}

//---------------------------------------------------------------------------
void TfrmReceiveInvoice::CommitPackingSlip(bool isCommitted)
{
    if(vtvStockQty->IsEditing())
	vtvStockQty->EndEditNode();

	if(CheckPackingSlip())
	{
		Currency totalCalculatedGST	= 0;

		bool Initialised = false;
		TDateTime MinInitialiseTime = 0.0;

		// Loop through the list. Each item is an invoice item to add to StockTrans.
		std::vector<Stock::TInvoiceItemInfo> InvoiceItems;
		PVirtualNode Node = vtvStockQty->GetFirst();

		while(Node)
		{
			TInvoiceItemNodeData* NodeData = (TInvoiceItemNodeData*)vtvStockQty->GetNodeData(Node);
			if(NodeData->OrderQty != 0 || (NodeData->OrderQty + NodeData->BackOrder) != NodeData->SupplierUnitsToReceive)
			{
				Stock::TInvoiceItemInfo InvoiceItemInfo;

                qrSupplierKey->Close();
                qrSupplierKey->ParamByName("SUPPLIER_KEY")->AsInteger = NodeData->SupplierKey;
                qrSupplierKey->ParamByName("STOCK_KEY")->AsInteger = NodeData->StockKey;
                qrSupplierKey->Open();
                AnsiString SUPPLIER_UNIT=    qrSupplierKey->FieldByName("SUPPLIER_UNIT")->AsString;

				InvoiceItemInfo.Stock_Key = NodeData->StockKey;
				InvoiceItemInfo.Purchase_Stock_Key = NodeData->PurchaseStockKey;
				InvoiceItemInfo.Location = NodeData->Location;
				InvoiceItemInfo.Supplier_Code = NodeData->SupplierCode;
				InvoiceItemInfo.GLCode = NodeData->GLCode;
				InvoiceItemInfo.Description = NodeData->Text;
				InvoiceItemInfo.Order_Unit = IsPurchaseOrderMode ?  NodeData->SupplierUnit : NodeData->DisplayedUnit;
                InvoiceItemInfo.Supplier_Unit = SUPPLIER_UNIT;
				InvoiceItemInfo.Order_Qty = NodeData->OrderQty;
				InvoiceItemInfo.Supplier_Unit_Size = NodeData->SupplierUnitSize;
				InvoiceItemInfo.BackOrder = NodeData->BackOrder;
				InvoiceItemInfo.TransactionNumber = NodeData->TransactionNumber;
				if (NodeData->OrderQty != 0)
				{
				    InvoiceItemInfo.Supplier_Unit_Cost = NodeData->SupplierTotalCost / NodeData->OrderQty;
				}
				else
				{
					InvoiceItemInfo.Supplier_Unit_Cost = 0; // Won't be used. No product received.
				}

				InvoiceItemInfo.Total_Cost = NodeData->SupplierTotalCost; // Temporarily store entered cost and unit cost. May need GST removed.

				if (rbExcludingGST->Checked)
				{
					InvoiceItemInfo.Total_GST = NodeData->SupplierTotalCost * NodeData->GSTPercent / 100;
				}
				else if (rbIncludingGST->Checked)
				{
					InvoiceItemInfo.Total_GST 	= NodeData->SupplierTotalCost * NodeData->GSTPercent / (100 + NodeData->GSTPercent);
				}

				totalCalculatedGST += InvoiceItemInfo.Total_GST;
                   
				InvoiceItems.push_back(InvoiceItemInfo);

				if(NodeData->Initialised)
				{
					Initialised = true;
					if(NodeData->InitialisedTime != TDateTime(0.0))
					{
						if(NodeData->InitialisedTime < MinInitialiseTime || MinInitialiseTime == TDateTime(0.0))
						{
							MinInitialiseTime = NodeData->InitialisedTime;
						}
					}
				}
			}
			// All done for this item. Get the next one.
			Node = vtvStockQty->GetNext(Node);
		}

		if(InvoiceItems.size() > 0)
		{
			TDateTime BatchTime = TDateTime(int(dtpInvoiceDate->Date) + Time());
			if(Initialised && int(dtpInvoiceDate->Date) == int(MinInitialiseTime))
			{
				// If the stock take was initialised today, set the time before the initialisation time.
				BatchTime = MinInitialiseTime - EncodeTime(0,1,0,0);
			}
			if(Initialised && MinInitialiseTime < BatchTime && MinInitialiseTime != TDateTime(0.0))
			{
				// Must be before the initialisation date/time!
				Application->MessageBox(AnsiString("As some items are initialised for a stock take, you must select a date prior to " + MinInitialiseTime.DateTimeString()).c_str(),"Error",MB_OK+MB_ICONERROR);
			}
			else
			{
				TInvoiceSummary invoiceSummary;
				CalculateInvoiceTotals(invoiceSummary);
				Currency totalEnteredGST = invoiceSummary.TotalGST;
				if (rbExcludingGST->Checked)
				{
					// Totals entered don't include gst. Adding a random amount of gst won't effect the total excl.
					for (unsigned i=0; i<InvoiceItems.size(); i++)
					{
						if (i < InvoiceItems.size()-1)
						{
							// Each item except last
							Currency Error = 0;
							if (totalCalculatedGST != 0)
							{
								// Calculated GST * Fraction incorrect compared to entered
								Error = (InvoiceItems[i].Total_GST * (totalCalculatedGST - totalEnteredGST)) / totalCalculatedGST;
							}
							totalCalculatedGST -= InvoiceItems[i].Total_GST;

							// Always round to 2dp. Last item will pick up any remaining fractional cents if necessary.
							InvoiceItems[i].Total_GST = MMMath::RoundCurrency(InvoiceItems[i].Total_GST - Error, GSTDecimals);

							totalEnteredGST -= InvoiceItems[i].Total_GST;
						}
						else
						{
							// Last Item gets rounding error.
							InvoiceItems[i].Total_GST = totalEnteredGST;
						}
					}
				}
				else if (rbIncludingGST->Checked)
				{
					// Totals entered already include gst. Current cost + a random amount of gst must equal the total entered!
					for (unsigned i=0; i<InvoiceItems.size(); i++)
					{
						if (i < InvoiceItems.size()-1)
						{
							// Each item except last
							Currency Error = 0;
							if (totalCalculatedGST != 0)
							{
								// Calculated GST * Fraction incorrect compared to entered
								Error = (InvoiceItems[i].Total_GST * (totalCalculatedGST - totalEnteredGST)) / totalCalculatedGST;
							}
							totalCalculatedGST -= InvoiceItems[i].Total_GST;

							// Always round to 2dp. Last item will pick up any remaining fractional cents if necessary.
							InvoiceItems[i].Total_GST = MMMath::RoundCurrency(InvoiceItems[i].Total_GST - Error, GSTDecimals);

							totalEnteredGST -= InvoiceItems[i].Total_GST;
						}
						else
						{
							// Last Item gets rounding error.
							InvoiceItems[i].Total_GST = totalEnteredGST;
						}
						InvoiceItems[i].Total_Cost = InvoiceItems[i].Total_Cost - InvoiceItems[i].Total_GST;

						if (InvoiceItems[i].Order_Qty != 0)
						{
							InvoiceItems[i].Supplier_Unit_Cost = InvoiceItems[i].Total_Cost / InvoiceItems[i].Order_Qty;
						}
						else
						{
							InvoiceItems[i].Supplier_Unit_Cost = 0; // Wont be used.
						}
					}
				}

				Stock::TReceiveInvoice CreateInvoice(dmStockData->dbStock);
				Stock::TTransactionBatchInfo BatchInfo(Stock::ttPackingSlip);
           
			   qrSupplier->Close();
                qrSupplier->ParamByName("Supplier_Key")->AsInteger = SupplierKey;
				qrSupplier->Open();
				BatchInfo.User_ID = frmLogin->CurrentUserID;
				BatchInfo.User_Name = frmLogin->CurrentUserName;
				BatchInfo.Created = BatchTime;
				BatchInfo.Supplier_Key = qrSupplier->FieldByName("Contact_LK")->AsInteger;
				BatchInfo.Supplier_Name = qrSupplier->FieldByName("Company_Name")->AsString;
				BatchInfo.Supplier_ID = qrSupplier->FieldByName("Supplier_ID")->AsString;
				BatchInfo.Export_1 = qrSupplier->FieldByName("MYOB1")->AsString;
				BatchInfo.Export_2 = qrSupplier->FieldByName("MYOB2")->AsString;
				BatchInfo.Export_3 = qrSupplier->FieldByName("MYOB3")->AsString;
    				BatchInfo.Order_Number = (OrderKey == 0)?AnsiString(""):qrPurchaseOrder->FieldByName("Order_Number")->AsString;
				BatchInfo.Reference = InvoiceReference;
				BatchInfo.Total_Cost = invoiceSummary.TotalExcl;//TotalCost;
				BatchInfo.Total_GST = invoiceSummary.TotalGST;//neGST->Value;
				BatchInfo.BatchID = BatchKey;
                BatchInfo.IsCommitedPS  =IsSavedPackingSlip  ;

                 //BatchInfo.check = isinvoice ;

                //check this invoice alredy committed or not
                if (qrCheckCommitted->Database->Connected && !qrCheckCommitted->Transaction->InTransaction)
                {
                    qrCheckCommitted->Transaction->StartTransaction();
                }
                qrCheckCommitted->Close();
                qrCheckCommitted->ParamByName("ISCOMMITTED_PACKINGSLIP")->AsString = "T";
                qrCheckCommitted->ParamByName("SUPPLIER_KEY")->AsInteger = qrSupplier->FieldByName("Contact_LK")->AsInteger;
                qrCheckCommitted->ParamByName("REFERENCE")->AsString = InvoiceReference;
                qrCheckCommitted->Open();
                bool RetVal = (qrCheckCommitted->Fields->Fields[0]->AsString != "");
                qrCheckCommitted->Close();



                
                if(isCommitted || RetVal)
                {
                    BatchInfo.ISCOMMITTED_PACKINGSLIP = true;
                }
                else
                {
                        BatchInfo.ISCOMMITTED_PACKINGSLIP = false;
                }

				if(CreateInvoice.ReceiveInvoice(BatchInfo, InvoiceItems))
				{
					if(OrderKey != 0)
					{
						Stock::TPurchaseOrdersControl PurchaseOrdersControl(dmStockData->dbStock);
						if(PurchaseOrdersControl.OrderComplete(OrderKey))
						{
							PurchaseOrdersControl.CloseOrder(OrderKey);
						}
					}

					if(Transaction->InTransaction)
					Transaction->Commit();

					// all database operations have completed successfully, do export to Xero if configured.
					bool Registered = false;
					unsigned int Modules = 0;
					qrSupplier->Close();
					dmMMData->Registered(&Registered, &Modules);
				    ModalResult = mrOk;
				}
				else
				{
					Application->MessageBox("An error occured creating the packing slip.", "Error", MB_ICONERROR+MB_OK);
				}
			}
		}
		else
		{
			Application->MessageBox("There are no items in this packing slip.", "Error", MB_ICONERROR+MB_OK);
		}
	}
}

//changes for item update
float TfrmReceiveInvoice::GetItemQty(int supplier_key, int stock_key)
{
   float itemQty= 0.0;

   for(int i = 0; i < dbcbUnit->Items->Count; i++)
   {
        qrUnitqty->Close();
        qrUnitqty->ParamByName("SUPPLIER_KEY")->AsInteger = supplier_key;
        qrUnitqty->ParamByName("STOCK_KEY")->AsInteger = stock_key;
        qrUnitqty->ParamByName("SUPPLIER_UNIT")->AsString= dbcbUnit->Items->Strings[i];        //
        qrUnitqty->Open();
        qrUnitqty->First();
        if(qrUnitqty->FieldByName("QTY")->AsFloat>0)
        {
           itemQty = qrUnitqty->FieldByName("QTY")->AsFloat;
           break;
        }
   }

   return itemQty;
}

double TfrmReceiveInvoice::GetSupplierUnitCost(AnsiString supplier_unit, int stock_key, int batch_key, TInvoiceItemNodeData *NodeData)
{
    double unitCost= 0.0;

    qrStockTransaction->Close();
    qrStockTransaction->ParamByName("Batch_Key")->AsInteger = batch_key;

    for (qrStockTransaction->Open(); !qrStockTransaction->Eof; qrStockTransaction->Next())
    {
        if (qrStockTransaction->FieldByName("ORDER_QTY")->AsFloat != 0)
        {
            if(qrStockTransaction->FieldByName("STOCK_KEY")->AsInteger == stock_key && supplier_unit == qrStockTransaction->FieldByName("UNIT")->AsString)
            {
                NodeData->SupplierCode = qrStockTransaction->FieldByName("Supplier_Code")->AsString;
                NodeData->SupplierUnitCost = (qrStockTransaction->FieldByName("UNIT_COST")->AsFloat);
                if(NodeData->OrderQty <= (qrStockTransaction->FieldByName("ORDER_QTY")->AsFloat))
                {
                   NodeData->SupplierTotalCost = qrStockTransaction->FieldByName("TOTAL_COST")->AsFloat;
                }
                if(NodeData->OrderQty == 0)
                {
                    NodeData->OrderQty = (qrStockTransaction->FieldByName("ORDER_QTY")->AsFloat);
                }
                unitCost = NodeData->SupplierUnitCost;
                break;
            }

        }
    }
   return unitCost;
}

bool TfrmReceiveInvoice::CheckSupplierUnit(int stock_key, int supplier_key, AnsiString _supplierunit) //SupplierStockDetails
{
    bool retVal = false;
    qrCheckSupplierUnit->ParamByName("STOCK_KEY")->AsInteger = stock_key;
    qrCheckSupplierUnit->ParamByName("SUPPLIER_KEY")->AsInteger = supplier_key;
    qrCheckSupplierUnit->ParamByName("SUPPLIER_UNIT")->AsString = _supplierunit;
    qrCheckSupplierUnit->Close();
    for(qrCheckSupplierUnit->Open(); !qrCheckSupplierUnit->Eof; qrCheckSupplierUnit->Next())
    {
        retVal = true;
    }
    return retVal;
}

double TfrmReceiveInvoice::GetSupplierUnitSize(int stock_key, int supplier_key, AnsiString _supplierunit)
{
    double retVal = 0.0;
    qrCheckSupplierUnit->ParamByName("STOCK_KEY")->AsInteger = stock_key;
    qrCheckSupplierUnit->ParamByName("SUPPLIER_KEY")->AsInteger = supplier_key;
    qrCheckSupplierUnit->ParamByName("SUPPLIER_UNIT")->AsString = _supplierunit;
    qrCheckSupplierUnit->Close();
    for(qrCheckSupplierUnit->Open(); !qrCheckSupplierUnit->Eof; qrCheckSupplierUnit->Next())
    {
        retVal = qrCheckSupplierUnit->FieldByName("QTY")->AsFloat;
    }
    return retVal;
}


void TfrmReceiveInvoice::UpdateSupplierUnit(TInvoiceItemNodeData *NodeData)
{
    qrUnitChanged->Close();
    qrUnitChanged->ParamByName("SUPPLIER_KEY")->AsInteger = NodeData->SupplierKey;;
    qrUnitChanged->ParamByName("STOCK_KEY")->AsInteger = NodeData->StockKey;;
    qrUnitChanged->ParamByName("SUPPLIER_UNIT")->AsString = NodeData->DisplayedUnit;

    qrUnitChanged->Open();
    qrUnitChanged->First();
    NodeData->SupplierUnitCost = qrUnitChanged->FieldByName("LATEST_COST")->AsFloat;
    if(NodeData->SupplierUnitCost > 0)
    {
        NodeData->SupplierUnit = NodeData->DisplayedUnit;
    }

}

void TfrmReceiveInvoice::UpdateNodeQty(TInvoiceItemNodeData *NodeData, double QtyToReceive, double newSupplierUnitSize)
{
    
    double quantity = NodeData->OrderQty;
    NodeData->DisplayedUnit = dbcbUnit->Items->Strings[dbcbUnit->ItemIndex];

    quantity = MMMath::RoundCurrency(quantity * NodeData->SupplierUnitSize/newSupplierUnitSize, 0);
    QtyToReceive = MMMath::RoundCurrency(QtyToReceive * NodeData->SupplierUnitSize/newSupplierUnitSize,0);

    NodeData->SupplierUnitSize =  newSupplierUnitSize;
    if(NodeData->DisplayedUnit != NodeData->SupplierUnit)
    {
        NodeData->IsUnitChanged = true;
    }
    else
    {
        NodeData->IsUnitChanged = false;
    }


    //if(quantity != 0)
    //{
        //quantity->DecimalPlaces=Decimalpalaces;
        NodeData->OrderQty = quantity;

    //}
   // QtyToReceive->DecimalPlaces= Decimalpalaces
    NodeData->SupplierUnitsToReceive = QtyToReceive;
    neStockQty->Value = NodeData->OrderQty;
    //neStockQty->DecimalPlaces=DecimalPlaces;
    neStockQty->DecimalPlaces=Decimalpalaces;
    NodeData->SupplierTotalCost = NodeData->SupplierUnitCost  * NodeData->OrderQty;
    neTotalCost->Text = FloatToStr(NodeData->SupplierTotalCost);
   // neTotalCost->DecimalPlace=DecimalPlace;
    NodeData->BackOrder	= NodeData->SupplierUnitsToReceive - NodeData->OrderQty;
}


void TfrmReceiveInvoice::UpdateSupplierUnitCost(int order_key, TInvoiceItemNodeData *NodeData)
{
    qrPurchaseStockTakeAuto->Close();
    qrPurchaseStockTakeAuto->ParamByName("Order_Key")->AsInteger = order_key;
    qrPurchaseStockTakeAuto->ParamByName("STOCK_KEY")->AsInteger = NodeData->StockKey;;
    qrPurchaseStockTakeAuto->ParamByName("Supplier_Unit")->AsString = NodeData->SupplierUnit;
    qrPurchaseStockTakeAuto->ParamByName("Stocktake_Unit")->AsString = dbcbUnit->Items->Strings[dbcbUnit->ItemIndex];
    qrPurchaseStockTakeAuto->Open();
    qrPurchaseStockTakeAuto->First();
    NodeData->SupplierUnitCost = qrPurchaseStockTakeAuto->FieldByName("Supplier_Unit_Cost")->AsFloat;
}

void TfrmReceiveInvoice::UpdateUnitqty(TInvoiceItemNodeData *NodeData)
{
    qrUnitqty->Close();
    qrUnitqty->ParamByName("SUPPLIER_KEY")->AsInteger = NodeData->SupplierKey;;
    qrUnitqty->ParamByName("STOCK_KEY")->AsInteger = NodeData->StockKey;
    qrUnitqty->ParamByName("SUPPLIER_UNIT")->AsString=NodeData->SupplierUnit;
    //
    qrUnitqty->Open();
    qrUnitqty->First();
    if(qrUnitqty->FieldByName("QTY")->AsFloat>0)
    {
        NodeData->SupplierUnitCost = qrUnitqty->FieldByName("LATEST_COST")->AsFloat/ qrUnitqty->FieldByName("QTY")->AsFloat;
    }
    else
    {
        float itemqty = GetItemQty(NodeData->SupplierKey, NodeData->StockKey);
        NodeData->SupplierUnitCost = qrUnitqty->FieldByName("LATEST_COST")->AsFloat/ itemqty;
    }
}

double TfrmReceiveInvoice::GetStockTakeUnitSize(int stock_key, int supplier_key)
{
    double size = 0.0;
    qrGetStockUnitSize->Close();
    qrGetStockUnitSize->ParamByName("SUPPLIER_KEY")->AsInteger = supplier_key;
    qrGetStockUnitSize->ParamByName("STOCK_KEY")->AsInteger = stock_key;
    //
    qrGetStockUnitSize->Open();
    qrGetStockUnitSize->First();
    if(qrGetStockUnitSize->FieldByName("QTY")->AsFloat>0)
    {
       size = qrGetStockUnitSize->FieldByName("QTY")->AsFloat;
    }
    return size;
}

bool TfrmReceiveInvoice::CheckInvoiceQtyAndPrice()
{
    IsNegativeQtyOrCost = true;
	PVirtualNode Node = vtvStockQty->GetFirst();
    if(!AllowNegativeValue)
    {
        while (Node && IsNegativeQtyOrCost)
        {
            TInvoiceItemNodeData *NodeData = (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(Node);

            if((NodeData->OrderQty < 0) || (NodeData->SupplierUnitCost < 0))
            {
               IsNegativeQtyOrCost = false;
               break;
            }
            // All done. Get the next one.
            Node = vtvStockQty->GetNext(Node);
        }
        if (!IsNegativeQtyOrCost)
        {
           Application->MessageBox("You Cannot Process Negative Amount or Quantity", "Error", MB_ICONERROR);
        }
    }
	return IsNegativeQtyOrCost;
}

//---------------------------------------------------------------------------
void __fastcall TfrmReceiveInvoice::neCostKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN || Key == '-')
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveInvoice::neTotalCostKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN )
	{
		Key = NULL;
	}
    if(Key != '0' && Key != '1' && Key != '2' && Key != '3' && Key != '4' && Key != '5' && Key != '6' && Key != '7' && Key != '8' && Key != '9' && Key != '.' && Key != '-')
    {
       Key = NULL;
    }
    if ((Key == '-') && !AllowNegativeValue)
	{
		Key = NULL;
	}
    if(CheckPointEntered(neTotalCost) && Key == '.')
    {
       Key = NULL;
    }
    if(CheckNegativeEntered(neTotalCost) && Key == '-')
    {
      Key = NULL;
    }


}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveInvoice::neBackOrderKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN || Key == '-')
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveInvoice::myEditBoxKeyPress(TObject *Sender,
      char &Key)
{
	if(Key == VK_RETURN || Key == '-')
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveInvoice::myEditBoxKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN)
	{
		btnOk->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveInvoice::myEditBoxExit(TObject *Sender)
{
    ShowTotals();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveInvoice::reGstValueKeyPress(TObject *Sender,
      char &Key)
{
	if(Key == VK_RETURN || Key == '-' || (Key != '0' && Key != '1' && Key != '2' && Key != '3' && Key != '4' && Key != '5' && Key != '6' && Key != '7' && Key != '8' && Key != '9' && Key != '.'))
	{
        Key = NULL;
	}
    if(CheckPointEntered(reGstValue) && Key == '.')
    {
       Key = NULL;
    }
    if(CheckNegativeEntered(reGstValue) && Key == '-')
    {
       Key = NULL;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveInvoice::reGstValueKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN)
	{
		btnOk->SetFocus();
	}
    if(Key > 10 && Key != 46)
    {
       Key = NULL;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveInvoice::reGstValueExit(TObject *Sender)
{
   reGstValue->Alignment = taRightJustify;
   ShowTotals();
}
//---------------------------------------------------------------------------
bool TfrmReceiveInvoice::CheckPointEntered(TRichEdit *reValue)
{
   AnsiString value = reValue->Text;
   bool point_value = value.Pos('.');
   return point_value;
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveInvoice::reGstValueMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   reGstValue->Alignment = taLeftJustify;
   if(reGstValue->Text == "0.00" || reGstValue->Text == "0.0000")
   {
      reGstValue->Text = "";
   }
}
//---------------------------------------------------------------------------
void TfrmReceiveInvoice::CalculateQtyValue()
{
    vtvStockQty->EndEditNode();
    TInvoiceItemNodeData *NodeData	= (TInvoiceItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);

    if(IsPackingSlipUpdateMode && IsSavedPackingSlip)
    {
       if(NodeData->OrderQty < 0)
       {
           NodeData->SupplierTotalCost = -fabs(NodeData->SupplierTotalCost);
           neTotalCost->Text = FloatToStr(NodeData->SupplierTotalCost);
       }
       else
       {
          NodeData->SupplierTotalCost = fabs(NodeData->SupplierTotalCost);
          neTotalCost->Text = FloatToStr(NodeData->SupplierTotalCost);
       }

    }
    else
    {
        if(NodeData->SupplierTotalCost > 0)
        {
           NodeData->OrderQty = fabs(NodeData->OrderQty);
           neStockQty->Value = NodeData->OrderQty;
        }
        else
        {
            if(NodeData->SupplierTotalCost < 0)
            {
               NodeData->OrderQty = -fabs(NodeData->OrderQty);
               neStockQty->Value = NodeData->OrderQty;
            }
        }
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool TfrmReceiveInvoice::CheckNegativeEntered(TRichEdit *reValue)
{
   AnsiString value = reValue->Text;
   bool point_value = value.Pos('-');
   return point_value;
}

void __fastcall TfrmReceiveInvoice::neCostChange(TObject *Sender)
{
   CheckNegativeValue(neCost);
}
//---------------------------------------------------------------------------
void TfrmReceiveInvoice::CheckNegativeValue(TNumericEdit *neCost)
{
   AnsiString value = FloatToStr(neCost->Value);
   if(value.Trim().Length() > 0)
   {
      AnsiString cost = value.TrimLeft();
      cost = cost.SubString(0, 1);
       if(cost == '-')
       {
          neCost->Value = 0;
       }
   }
}
