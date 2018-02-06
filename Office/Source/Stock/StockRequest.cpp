//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <set>
#include "CreateOrders.h"
#include "StockData.h"
#include "SelectStockItem.h"
#include "SetupPurchaseOrders.h"
#include "StockRequest.h"
#include "SelectDate.h"
#include "StockReportData.h"
#include "Reports.h"
#include "Utilities.h"
#include "MM_Math.h"
#include "SelectSupplierStockItem.h"
#include "SelectSupplier.h"
#include "SelectLocationSupplier.h"
#include "StockGroup.h"
#include "Connections.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TfrmStockRequest *frmStockRequest;
//---------------------------------------------------------------------------
__fastcall TfrmStockRequest::TfrmStockRequest(TComponent* Owner)
: TForm(Owner),
frmPurchaseOrder(new TfrmPurchaseOrder(NULL)),
frmReceiveStockItem(new TfrmReceiveStockItem(NULL))
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::FormShow(TObject *Sender)
{
	vtvStockQty->Clear();
	vtvStockQty->NodeDataSize = sizeof(TStockRequestItemNodeData);
	lbeTitle->Caption = "Create Stock Request";
	cbRequestType->ItemIndex = -1;
	edDateRequired->Text ="";
	btnAdd->Visible=false;
	BitBtn1->Visible=false;
	if (!Transaction->InTransaction)
	Transaction->StartTransaction();
	cmbLocation->Clear();
	for (qrLocation->Open(); !qrLocation->Eof; qrLocation->Next())
	{
		cmbLocation->Items->AddObject(qrLocation->FieldByName("Location")->AsString,(TObject *)qrLocation->FieldByName("Location"));
	}
	qrLocation->Close();
	Query->Close();
	qrStockReqStock->Close();
	IsUpdateQuery = false;
    	StockRequestNumber= RequestNumber;
	if(RequestNumber > 0)
	{       btnAdd->Visible=true;
		BitBtn1->Visible=true;
		StockRequestNumber= RequestNumber;
		IsUpdateQuery = true;
		qrStockReqStock->ParamByName("REQUEST_NUMBER")->AsInteger = RequestNumber;
		for (qrStockReqStock->Open(); !qrStockReqStock->Eof; qrStockReqStock->Next())
		{
			int supplierStockkey= qrStockReqStock->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger ;
			LoadStockRequest(RequestNumber,supplierStockkey);
		}
		cmbLocation->Enabled = false;
		cbRequestType->Enabled = false;
		edDateRequired->Enabled=false;
	}
	else{ cmbLocation->Enabled = true;
		cbRequestType->Enabled = true;
		edDateRequired->Enabled=true;
	}
	Location = cmbLocation->Text;
	RequestType  =cbRequestType->Text;
	if (Transaction->InTransaction)
	Transaction->Commit();
}

//---------------------------------------------------------------------------
void TfrmStockRequest::LoadStockRequest(int RequestNumber, int supplierStockkey)
{
	PVirtualNode OrderItemNode			= NULL;
	TStockRequestItemNodeData *NodeData	= NULL;
	
	qrStockRequestSelect->Close();
	qrStockRequestSelect->ParamByName("REQUEST_NUMBER")->AsInteger = RequestNumber;
	qrStockRequestSelect->ParamByName("SUPPLIER_STOCK_KEY")->AsInteger = supplierStockkey;

	for (qrStockRequestSelect->Open(); !qrStockRequestSelect->Eof; qrStockRequestSelect->Next())
	{
		OrderItemNode = vtvStockQty->AddChild(NULL);
		NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
		NodeData->StockKey =	qrStockRequestSelect->FieldByName("STOCK_KEY")->AsInteger ;
		NodeData->Location =  	qrStockRequestSelect->FieldByName("LOCATION")->AsString ;
		NodeData->Supplier =   qrStockRequestSelect->FieldByName("COMPANY_NAME")->AsString;
		NodeData->SupplierKey =   qrStockRequestSelect->FieldByName("SUPPLIER_KEY")->AsInteger;
		NodeData->SupplierUnit =   qrStockRequestSelect->FieldByName("STOCK_REQUEST_UNIT")->AsString ;
		NodeData->SupplierUnitCost =   qrStockRequestSelect->FieldByName("STOCK_REQUEST_UNIT_COST")->AsFloat;
		NodeData->SupplierUnitQty = 	qrStockRequestSelect->FieldByName("QTY_REQUIRED")->AsFloat;
		NodeData->Text =qrStockRequestSelect->FieldByName("DESCRIPTION")->AsString ;
		NodeData->RequestType =RequestType;	//qrStockRequestSelect->FieldByName("REQUEST_TYPE")->AsString	;
		NodeData->SupplierCode  = 	qrStockRequestSelect->FieldByName("SUPPLIER_CODE")->AsString  ;
		NodeData->StockReqStockRequestNumber =	qrStockRequestSelect->FieldByName("STOCK_REQUEST_STOCKKEY")->AsInteger ;
		NodeData->RequestNumber 	=	qrStockRequestSelect->FieldByName("REQUEST_NUMBER")->AsInteger ;
		NodeData->onhand    =qrStockRequestSelect->FieldByName("ON_HAND")->AsInteger ;
		NodeData->SupplierCode = qrStockRequestSelect->FieldByName("SUPPLIER_CODE")->AsString;
		NodeData->SUPPLIERSTOCKKEY =qrStockRequestSelect->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger;

		edDateRequired->Text =qrStockRequestSelect->FieldByName("DATE_REQUIRED")->AsDateTime ;

		int locationIndex = cmbLocation->Items->IndexOf(qrStockRequestSelect->FieldByName("LOCATION")->AsString);
		cmbLocation->ItemIndex = locationIndex;

		int requestTypeIndex = cbRequestType->Items->IndexOf(qrStockRequestSelect->FieldByName("REQUEST_TYPE")->AsString);
		cbRequestType->ItemIndex = requestTypeIndex;
	}
	qrStockRequestSelect->Close();
}
void __fastcall TfrmStockRequest::edDateRequiredClick(TObject *Sender)
{
	frmSelectDate->Calendar->MinDate =Now()-1;
	if (double(DateRequired) != 0.0)
	{
		frmSelectDate->Calendar->Date = DateRequired;
	}
	else
	{
		frmSelectDate->Calendar->Date = Now();
	}
	frmSelectDate->lbeTitle->Caption = "Select the date required.";
	if (frmSelectDate->ShowModal() == mrOk)
	{
		DateRequired = frmSelectDate->Calendar->Date;
		edDateRequired->Text = DateRequired.DateString();
		Datetime  =DateRequired.DateString();
	}
}
void __fastcall TfrmStockRequest::btnRequiredDateClick(TObject *Sender)
{
	if (double(DateRequired) != 0.0)
	{
		frmSelectDate->Calendar->Date = DateRequired;
	}
	else
	{
		frmSelectDate->Calendar->Date = Now();
	}
	frmSelectDate->lbeTitle->Caption = "Select the date required.";
	if (frmSelectDate->ShowModal() == mrOk)
	{
		DateRequired = frmSelectDate->Calendar->Date;
		edDateRequired->Text = DateRequired.DateString();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::btnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::FormCloseQuery(TObject *Sender, bool &CanClose)
{    frmSelectStockItem->ClearLocation();     
      if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}

    /*
	if (Application->MessageBox("Are you sure you want to cancel?", "Cancel?", MB_ICONQUESTION + MB_YESNO) == ID_YES)
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		ModalResult = mrCancel;
	}
    */
	if (vtvStockQty->GetFirst() && ModalResult == mrCancel)
	{
		CanClose = (Application->MessageBox("Are you sure  you wish to cancel?", "Cancel?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK);
	}
	else
	{
		CanClose = true;
	}
	//frmSelectStockItem->ClearLocation();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::FormClose(TObject *Sender,
TCloseAction &Action)
{
	FindDialog->CloseDialog();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::vtvStockQtyAfterPaint(
TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::vtvStockQtyBeforePaint(
TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::vtvStockQtyCreateEditor(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
IVTEditLink *EditLink)
{

	if (Node && Column == 4)
	{

		TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)Sender->GetNodeData(Node);
		neStockQty->Value = NodeData->SupplierUnitQty;
        neStockQty->DecimalPlaces = CurrentConnection.SettingDecimalPlaces;
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neStockQty);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(neStockQty->Handle, EM_SETSEL, 0, -1);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::vtvStockQtyEdited(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
    
	if (vtvStockQty->FocusedNode)
	{
		if (Column == 4)
		{
			TStockRequestItemNodeData *NodeData	= (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
            neStockQty->DecimalPlaces = CurrentConnection.SettingDecimalPlaces;
			NodeData->SupplierUnitQty					= neStockQty->Value;
             
		}
		vtvStockQty->InvalidateNode(vtvStockQty->FocusedNode);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::vtvStockQtyEditing(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
bool &Allowed)
{
	Allowed = (Column == 4);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::vtvStockQtyFocusChanged(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	if (Node && !Sender->IsEditing())
	{
		if (Sender->FocusedColumn != 4)
		{
			Sender->FocusedColumn = 4;
		}
		Sender->EditNode(Node, Column);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::vtvStockQtyFocusChanging(
TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::vtvStockQtyGetImageIndex(
TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind,
TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
	ImageIndex = -1;
	TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)Sender->GetNodeData(Node);
	if (Column == 0 && NodeData)
	{
		if (NodeData->SupplierUnitsToReceive > NodeData->SupplierUnitQty + NodeData->SupplierUnitsBackOrder &&
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
void __fastcall TfrmStockRequest::vtvStockQtyGetText(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
TVSTTextType TextType, WideString &CellText)
{
	TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Column)
		{
		case 0:	CellText = NodeData->Supplier;      //tp
			break;
		case 1:	CellText = NodeData->SupplierCode;
			break;
		case 2:	CellText = NodeData->Text;
			break;
		case 3:
        CellText = MMMath::FloatString(NodeData->SupplierUnitCost, CurrentConnection.SettingDecimalPlaces);
			break;
		case 4:
        CellText = MMMath::FloatString(NodeData->SupplierUnitQty,CurrentConnection.SettingDecimalPlaces);
			break;
		case 5:	CellText = NodeData->SupplierUnit;
			break;
		case 6:
        CellText = MMMath::FloatString(NodeData->onhand,CurrentConnection.SettingDecimalPlaces);
			break;
		case 7:
        CellText =  MMMath::FloatString(NodeData->LatestCost,CurrentConnection.SettingDecimalPlaces);
			break;
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::vtvStockQtyKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{

	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		if (Key == VK_RETURN)
		{
			vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
		}
	}
	else if(!vtvStockQty->FocusedNode)
	{
		//btnAddClick(btnCancel);
	}
}
/*
void __fastcall TfrmStockRequest::btnAddClick(TObject *Sender)
{
	int counter = 0;

	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	frmSelectStockItem->Reset = true;
	frmSelectStockItem->Location = Location;
	if (frmSelectStockItem->ShowModal() == mrOk)
	{
		frmReceiveStockItem->StockCode = frmSelectStockItem->StockCode;
		frmReceiveStockItem->SupplierKey = 0;
		if (frmReceiveStockItem->Execute() == mrOk)
		{
			PVirtualNode Node = vtvStockQty->GetFirst();
			while (Node)
			{
				TStockRequestItemNodeData *NodeData	= (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
				if (NodeData->SupplierCode             ==   frmReceiveStockItem->SupplierCode &&
						NodeData->SupplierKey					== frmReceiveStockItem->SupplierKey &&
						NodeData->Text							== frmReceiveStockItem->Description &&
						NodeData->SupplierUnitCost							== frmReceiveStockItem->SupplierUnitCost &&
						NodeData->SupplierUnit					== frmReceiveStockItem->SupplierUnit)
				{
					vtvStockQty->FocusedNode				= Node;
					vtvStockQty->FocusedColumn				= 4;
					vtvStockQty->Selected[Node]			= true;
					Application->MessageBox("You have already this stock item.", "Error", MB_ICONERROR + MB_OK);
					return;
				}
				Node = vtvStockQty->GetNext(Node);
			}
			PVirtualNode OrderItemNode						= NULL;
			TStockRequestItemNodeData *NodeData		= NULL;
			if (!Transaction->InTransaction)
				Transaction->StartTransaction();

			qrStock->Close();
			qrStock->ParamByName("Supplier_Key")->AsString = AnsiString::AnsiString(frmReceiveStockItem->SupplierKey);
			qrStock->ParamByName("Stock_Key")->AsString = frmReceiveStockItem->StockKey;
			for (qrStock->Open(); !qrStock->Eof; qrStock->Next())
			{
				OrderItemNode										= vtvStockQty->AddChild(NULL);
				NodeData   = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
				NodeData->Code										= frmReceiveStockItem->StockCode;
				NodeData->StockKey                        = frmReceiveStockItem->StockKey;
				NodeData->Text										= frmReceiveStockItem->Description;
				NodeData->StockCategory							= frmReceiveStockItem->StockCategory;
				NodeData->StockGroup								= frmReceiveStockItem->StockGroup;
				NodeData->GLCode									= frmReceiveStockItem->GLCode;
				NodeData->Location								= frmReceiveStockItem->Location;
				NodeData->GSTPercent								= frmReceiveStockItem->GSTPercent;
				NodeData->StocktakeUnit							= frmReceiveStockItem->StocktakeUnit;
				NodeData->SupplierWeborderTargetUri    = frmReceiveStockItem->SupplierWeborderTargetUri;
				NodeData->Supplier								= frmReceiveStockItem->Supplier;
				NodeData->SupplierKey							= frmReceiveStockItem->SupplierKey;
				NodeData->SupplierCode							= frmReceiveStockItem->SupplierCode;
				NodeData->SupplierUnit							= frmReceiveStockItem->SupplierUnit;
				NodeData->SupplierUnitCost						= frmReceiveStockItem->SupplierUnitCost;
				NodeData->SupplierUnitSize						= frmReceiveStockItem->SupplierUnitSize;
				NodeData->STOCKGROUPKEY						= qrStock->FieldByName("STOCK_GROUP_KEY")->AsInteger;
				NodeData->STOCKCATEGORYKEY						= qrStock->FieldByName("Stock_Category_Key")->AsInteger;
				if(counter == 0)
				{
					NodeData->SupplierUnitQty						= frmReceiveStockItem->SupplierUnitQty;
				}
				else
					NodeData->SupplierUnitQty = 0;
				vtvStockQty->FocusedNode						= OrderItemNode;
				vtvStockQty->FocusedColumn						= 4;
				vtvStockQty->Selected[OrderItemNode]		= true;
				counter = counter + 1;
			}
			if(Transaction->InTransaction)
				Transaction->Commit();
		}
	}
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::btnRemoveClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	if (vtvStockQty->FocusedNode)
	{
		TStockRequestItemNodeData *NodeData	= (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
		if (Application->MessageBox(("Do you wish to remove the item \"" + NodeData->Text + "\" from the order?").c_str(),
					"Update?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
		{
			vtvStockQty->DeleteNode(vtvStockQty->FocusedNode);
		}
	}
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}*/
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::neStockQtyKeyDown(TObject *Sender,
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
		if (Key == VK_DOWN || (Key == VK_RETURN && vtvStockQty->FocusedColumn == 4))
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
				//btnAddClick(NULL);
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
void __fastcall TfrmStockRequest::Button2Click(TObject *Sender)
{
	frmSetupPurchaseOrders->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::btnCommitClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}


	if(edDateRequired->Text == "")
	{
		Application->MessageBox("Please select Date Required.", "Error", MB_ICONERROR + MB_OK);
		return;
	}
	if(cmbLocation->Text == "")
	{
		Application->MessageBox("Please select Location.", "Error", MB_ICONERROR + MB_OK);
		return;
	}
	if(cbRequestType->Text == "")
	{
		Application->MessageBox("Please select Request Type.", "Error", MB_ICONERROR + MB_OK);
		return;
	}

	int noOfStockItems = 0;
	PVirtualNode Node = vtvStockQty->GetFirst();
	while (Node)
	{
		TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
		if(NodeData->SupplierUnitQty > 0)
		{
			noOfStockItems += 1;
		}
		Node = vtvStockQty->GetNext(Node);
	}

	if(noOfStockItems == 0)
	{
		Application->MessageBox("Please select at least one stock item with a valid quantity.", "Error", MB_ICONERROR + MB_OK);
		return;
	}

	if(IsUpdateQuery)
	bool isSaved = PersistStockRequestUpdate(true);
	else
	bool isSaved = PersistStockRequest(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::btnSaveClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}

	bool isSaved = false;

	if(edDateRequired->Text == "")
	{
		Application->MessageBox("Please select Date Required.", "Error", MB_ICONERROR + MB_OK);
		return;
	}
	if(cmbLocation->Text == "")
	{
		Application->MessageBox("Please select Location.", "Error", MB_ICONERROR + MB_OK);
		return;
	}
	if(cbRequestType->Text == "")
	{
		Application->MessageBox("Please select Request Type.", "Error", MB_ICONERROR + MB_OK);
		return;
	}

	int noOfStockItems = 0;
	PVirtualNode Node = vtvStockQty->GetFirst();
	while (Node)
	{
		TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
		if(NodeData->SupplierUnitQty > 0)
		{
			noOfStockItems += 1;
		}
		Node = vtvStockQty->GetNext(Node);
	}

	if(noOfStockItems == 0)
	{
		Application->MessageBox("Please select at least one stock item with a valid quantity.", "Error", MB_ICONERROR + MB_OK);
		return;
	}

	if(IsUpdateQuery)
	isSaved = PersistStockRequestUpdate(false);
	else
	isSaved = PersistStockRequest(false);

	IsUpdateQuery = false;
}
void __fastcall TfrmStockRequest::cbRequestTypeClick(TObject *Sender)
{
	cbRequestType->Text = cbRequestType->Items->Strings[cbRequestType->ItemIndex].c_str();
	RequestType  =cbRequestType->Text;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool TfrmStockRequest::PersistStockRequest(bool isCommitted)
{
	bool CanClose;
	bool Success = true;
	int NewOrderKey = 0;
	IsSupplierQty = false;
	try
	{
		if (!Transaction->InTransaction)
		Transaction->StartTransaction();

		if (OrderKey != 0)
		{
			qrStockRequest->Close();
			qrStockRequest->ParamByName("REQUEST_NUMBER")->AsInteger = OrderKey;
			qrStockRequest->Open();
			if (qrStockRequest->Eof)
			{
				OrderKey = 0;
			}
			qrStockRequest->Close();
		}
		RemovedStockRequestUpdate();
		int NewOrderKey = 0;
		PVirtualNode Node = vtvStockQty->GetFirst();
		qrAddStockRequest->Close();
		dtStockReqCount->Open();
		dtStockReqCount->Edit();
		dtStockReqCount->FieldByName("REQUEST_NUMBER")->AsInteger = dtStockReqCount->FieldByName("REQUEST_NUMBER")->AsInteger ;
		dtStockReqCount->Post();
		StockReqCounter   = dtStockReqCount->FieldByName("REQUEST_NUMBER")->AsInteger ;
		RequestNumber =    StockReqCounter + 1;
		dtStockReqStockCount->Open();
		dtStockReqStockCount->Edit();
		dtStockReqStockCount->FieldByName("STOCK_REQUEST_STOCKKEY")->AsInteger = dtStockReqStockCount->FieldByName("STOCK_REQUEST_STOCKKEY")->AsInteger ;
		dtStockReqStockCount->Post();
		while (Node)
		{
			TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
			if(NodeData->SupplierUnitQty != 0)
			{
				IsSupplierQty = true;
			}
			Node = vtvStockQty->GetNext(Node);
		}
		StockReqStockCounter   = dtStockReqStockCount->FieldByName("STOCK_REQUEST_STOCKKEY")->AsInteger ;
		StockReqStockRequestNumber=    StockReqStockCounter;
		qrAddStockRequest->ParamByName("REQUEST_NUMBER")->AsInteger = RequestNumber;
		qrAddStockRequest->ParamByName("LOCATION")->AsString  = Location  ;
		qrAddStockRequest->ParamByName("REQUEST_TYPE")->AsString		= RequestType;  //not ok
		qrAddStockRequest->ParamByName("DATE_GENERATED")->AsDateTime =Now();
		qrAddStockRequest->ParamByName("DATE_REQUIRED")->AsDateTime = frmSelectDate->Calendar->Date; //StrToDate(edDateRequired->Text);
		qrAddStockRequest->ParamByName("ISCOMMITTED")->AsString	= isCommitted ? 'T': 'F'; //ok
		qrAddStockRequest->ParamByName("ISDELETED")->AsString	= 'F'; //ok
		if(IsSupplierQty)
		{
			qrAddStockRequest->ExecSQL();
			qrAddStockRequest->Close();

			if(Transaction->InTransaction)
			Transaction->Commit();
		}
		Node = vtvStockQty->GetFirst();
		while (Node)
		{
			if (!Transaction->InTransaction)
			Transaction->StartTransaction();

			StockReqStockRequestNumber=StockReqStockRequestNumber+1;
			TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
			TDateTime Timestamp = TDateTime::CurrentDateTime();
			if(NodeData->SupplierUnitQty!=0)
			{
				IsSupplierQty = true;
				qrAddStockRequestStock->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger = StockReqStockRequestNumber;
				qrAddStockRequestStock->ParamByName("REQUEST_NUMBER")->AsInteger = RequestNumber;
				qrAddStockRequestStock->ParamByName("STOCK_KEY")->AsInteger	= NodeData->StockKey;
				qrAddStockRequestStock->ParamByName("SUPPLIER_KEY")->AsString = NodeData->SupplierKey;  //ok
				qrAddStockRequestStock->ParamByName("QTY_REQUIRED")->AsFloat = NodeData->SupplierUnitQty;  //ok
				qrAddStockRequestStock->ParamByName("STOCK_REQUEST_UNIT")->AsString = NodeData->SupplierUnit;
				qrAddStockRequestStock->ParamByName("STOCK_REQUEST_UNIT_COST")->AsFloat		= NodeData->SupplierUnitCost; //ok
				qrAddStockRequestStock->ParamByName("ISPROCESSED")->AsString	= 'F'; //ok
				qrAddStockRequestStock->ParamByName("ISREMOVED")->AsString	= 	'F'	;
				qrAddStockRequestStock->ParamByName("DATE_ENTERED")->AsDateTime = Datetime;
				qrAddStockRequestStock->ParamByName("DATE_UPDATED")->AsDateTime=Now();   //SUPPLIER_STOCK_KEY //SUPPLIER_CODE
				qrAddStockRequestStock->ParamByName("SUPPLIER_STOCK_KEY")->AsInteger=NodeData->SUPPLIERSTOCKKEY;
				//	qrAddStockRequestStock->ParamByName("SUPPLIER_CODE")->AsString=NodeData->SupplierCode;;
				//	NodeData->SupplierCode

				qrAddStockRequestStock->ExecSQL();
			}
			Node = vtvStockQty->GetNext(Node);
			if(Transaction->InTransaction)
			Transaction->Commit();
		}
		qrAddStockRequestStock->Close();
		ModalResult = mrOk;
	}
	catch (Exception &E)
	{
		if(Transaction->InTransaction)
		Transaction->Rollback();

		Application->ShowException(&E);
		Success = false;
	}
	if(Transaction->InTransaction)
	Transaction->Commit();

	if (Success)
	{
		OrderKey = NewOrderKey;
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected && !dmStockReportData->StockTrans->InTransaction)
		{
			dmStockReportData->StockTrans->StartTransaction();
		}
		try
		{
			try
			{
				Query->SQL->Text =
				"SELECT distinct STOCK_CATEGORY FROM STOCKCATEGORY ";
				for(Query->ExecQuery(); !Query->Eof; Query->Next())
				{
					AnsiString defaultCategory= Query->FieldByName("STOCK_CATEGORY")->AsString;
					qrLocationList->Close();
					qrLocationList->ParamByName("REQUEST_NUMBER")->AsInteger = RequestNumber;
					qrLocationList->ParamByName("Stock_Category")->AsString = defaultCategory;
					qrLocationList->ParamByName("Location")->AsString = Location;
					qrLocationList->Open();
					if(!qrLocationList->Eof )
					{
						dmStockReportData->SetupStockRequestDetails(defaultCategory, RequestNumber, Location);
						if (frmReports->rvStock->SelectReport("repStockRequest", false))
						{
							frmReports->rvStock->Execute();
						}
						else
						{
							Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
						}
					}
				}
			}
			catch (Exception &e)
			{
				Query->Close();
			}
			Query->Close();
		}
		__finally
		{       qrLocationList->Close();
			if (dmStockReportData->StockTrans->DefaultDatabase->Connected && dmStockReportData->StockTrans->InTransaction)
			{
				dmStockReportData->StockTrans->Commit();
			}
		}
	}
}
//.......................................................................................................
bool TfrmStockRequest::PersistStockRequestUpdate(bool isCommitted)
{
	bool CanClose;
	bool Success = true;
	int NewOrderKey = 0;
	IsSupplierQty = false;
	try
	{
		if(!Transaction->InTransaction)
		Transaction->StartTransaction();

		if (OrderKey != 0)
		{
			qrStockRequest->Close();
			qrStockRequest->ParamByName("REQUEST_NUMBER")->AsInteger = OrderKey;
			qrStockRequest->Open();
			if (qrStockRequest->Eof)
			{
				OrderKey = 0;
			}
			qrStockRequest->Close();
		}
		RequestNumber=    StockReqCounter + 1;
		int NewOrderKey = 0;
		dtStockReqStockCount->Open();
		dtStockReqStockCount->Edit();
		dtStockReqStockCount->FieldByName("STOCK_REQUEST_STOCKKEY")->AsInteger = dtStockReqStockCount->FieldByName("STOCK_REQUEST_STOCKKEY")->AsInteger ;
		dtStockReqStockCount->Post();
		StockReqStockCounter   =dtStockReqStockCount->FieldByName("STOCK_REQUEST_STOCKKEY")->AsInteger ;
		StockReqStockRequestNumber=    StockReqStockCounter;
		PVirtualNode Node = vtvStockQty->GetFirst();
		while (Node)
		{
			TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
			if(NodeData->SupplierUnitQty!=0)
			{
				IsSupplierQty = true;
			}
			Node = vtvStockQty->GetNext(Node);
		}

		//tripurary

		RemovedStockRequestUpdate();

		Node = vtvStockQty->GetFirst();
		TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
		qrUpdateStockRequest->ParamByName("REQUEST_NUMBER")->AsInteger = StockRequestNumber;//NodeData->RequestNumber;
		qrUpdateStockRequest->ParamByName("LOCATION")->AsString  =	NodeData->Location  ;
		qrUpdateStockRequest->ParamByName("REQUEST_TYPE")->AsString		= RequestType;//NodeData->RequestType;  //not ok
		qrUpdateStockRequest->ParamByName("DATE_GENERATED")->AsDateTime =Now();
		qrUpdateStockRequest->ParamByName("DATE_REQUIRED")->AsDateTime = edDateRequired->Text;
		qrUpdateStockRequest->ParamByName("ISCOMMITTED")->AsString	= isCommitted ? 'T': 'F'; //ok
		qrUpdateStockRequest->ParamByName("ISDELETED")->AsString	= 'F'; //ok
		RequestNumber= StockRequestNumber;   NodeData->RequestNumber;
		if(IsSupplierQty)
		{
			qrUpdateStockRequest->ExecSQL();
			qrUpdateStockRequest->Close();
		}

		if(Transaction->InTransaction)
		Transaction->Commit();
		Node = vtvStockQty->GetFirst();
		while (Node)
		{
			if (!Transaction->InTransaction)
			Transaction->StartTransaction();

			TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
			TDateTime Timestamp = TDateTime::CurrentDateTime();
			//	if(NodeData->SupplierUnitQty!=0)
			//	{
			IsSupplierQty = true;
			//


			if(NodeData->StockReqStockRequestNumber>0)
			{
				qrUpdateStockRequestStock->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger = NodeData->StockReqStockRequestNumber;
				qrUpdateStockRequestStock->ParamByName("REQUEST_NUMBER")->AsInteger = StockRequestNumber;;
				qrUpdateStockRequestStock->ParamByName("STOCK_KEY")->AsInteger	= NodeData->StockKey;
				qrUpdateStockRequestStock->ParamByName("SUPPLIER_KEY")->AsString = NodeData->SupplierKey;  //ok
				qrUpdateStockRequestStock->ParamByName("QTY_REQUIRED")->AsFloat = NodeData->SupplierUnitQty;  //ok
				qrUpdateStockRequestStock->ParamByName("STOCK_REQUEST_UNIT")->AsString = NodeData->SupplierUnit;
				qrUpdateStockRequestStock->ParamByName("STOCK_REQUEST_UNIT_COST")->AsFloat		= NodeData->SupplierUnitCost; //ok
				qrUpdateStockRequestStock->ParamByName("ISPROCESSED")->AsString	= 'F'; //ok
				qrUpdateStockRequestStock->ParamByName("ISREMOVED")->AsString	= 	'F'	;
				qrUpdateStockRequestStock->ParamByName("DATE_ENTERED")->AsDateTime = DateRequired;
				qrUpdateStockRequestStock->ParamByName("DATE_UPDATED")->AsDateTime=Now();
				qrUpdateStockRequestStock->ParamByName("SUPPLIER_STOCK_KEY")->AsInteger=NodeData->SUPPLIERSTOCKKEY;
				// qrUpdateStockRequestStock->ParamByName("SUPPLIER_CODE")->AsString=NodeData->SupplierCode;;

				if(NodeData->SupplierUnitQty==0)
				{ 	qrUpdateStockRequestStock->ParamByName("ISREMOVED")->AsString	= 	'T'	;  }
				// else{	qrUpdateStockRequestStock->ParamByName("ISREMOVED")->AsString	= 	'F'	;}   */

				qrUpdateStockRequestStock->ExecSQL();
				if(Transaction->InTransaction)
				Transaction->Commit();

			}
			else
			{
				if (!Transaction->InTransaction)
				Transaction->StartTransaction();

				StockReqStockRequestNumber =StockReqStockRequestNumber +1   ;
				qrAddStockRequestStock->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger = StockReqStockRequestNumber;
				qrAddStockRequestStock->ParamByName("REQUEST_NUMBER")->AsInteger = RequestNumber;
				qrAddStockRequestStock->ParamByName("STOCK_KEY")->AsInteger	= NodeData->StockKey;
				qrAddStockRequestStock->ParamByName("SUPPLIER_KEY")->AsString = NodeData->SupplierKey;  //ok
				qrAddStockRequestStock->ParamByName("QTY_REQUIRED")->AsFloat = NodeData->SupplierUnitQty;  //ok
				qrAddStockRequestStock->ParamByName("STOCK_REQUEST_UNIT")->AsString = NodeData->SupplierUnit;
				qrAddStockRequestStock->ParamByName("STOCK_REQUEST_UNIT_COST")->AsFloat		= NodeData->SupplierUnitCost; //ok
				qrAddStockRequestStock->ParamByName("ISPROCESSED")->AsString	= 'F'; //ok
				qrAddStockRequestStock->ParamByName("ISREMOVED")->AsString	= 	'F'	;
				qrAddStockRequestStock->ParamByName("DATE_ENTERED")->AsDateTime = Datetime;
				qrAddStockRequestStock->ParamByName("DATE_UPDATED")->AsDateTime=Now();
				qrAddStockRequestStock->ParamByName("SUPPLIER_STOCK_KEY")->AsInteger=NodeData->SUPPLIERSTOCKKEY;
				// qrAddStockRequestStock->ParamByName("SUPPLIER_CODE")->AsString=NodeData->SupplierCode;;

				if(NodeData->SupplierUnitQty==0)
				{ 	qrUpdateStockRequestStock->ParamByName("ISREMOVED")->AsString	= 	'T'	;  }
				//	 else{	qrUpdateStockRequestStock->ParamByName("ISREMOVED")->AsString	= 	'F'	;}
				qrAddStockRequestStock->ExecSQL();



				if(Transaction->InTransaction)
				Transaction->Commit();

			}


			//	}
			Node = vtvStockQty->GetNext(Node);
		}
		qrUpdateStockRequestStock->Close();
		ModalResult = mrOk;
	}
	catch (Exception &E)
	{
		Transaction->Rollback();
		Application->ShowException(&E);
		Success = false;
	}
	if (Transaction->InTransaction)
	Transaction->Commit();

	if (Success)
	{
		OrderKey = NewOrderKey;
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected && !dmStockReportData->StockTrans->InTransaction)
		{
			dmStockReportData->StockTrans->StartTransaction();
		}
		try
		{
			try
			{
				Query->SQL->Text =
				"SELECT distinct STOCK_CATEGORY FROM STOCKCATEGORY ";
				for(Query->ExecQuery(); !Query->Eof; Query->Next())
				{
					AnsiString defaultCategory= Query->FieldByName("STOCK_CATEGORY")->AsString;
					qrLocationList->Close();
					qrLocationList->ParamByName("REQUEST_NUMBER")->AsInteger = RequestNumber;
					qrLocationList->ParamByName("Stock_Category")->AsString = defaultCategory;
					qrLocationList->ParamByName("Location")->AsString = Location;
					qrLocationList->Open();
					if(!qrLocationList->Eof )
					{
						dmStockReportData->SetupStockRequestDetails(defaultCategory, RequestNumber, Location);
						if (frmReports->rvStock->SelectReport("repStockRequest", false))
						{
							frmReports->rvStock->Execute();
						}
						else
						{
							Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
						}
					}
				}
			}
			catch (Exception &e)
			{        Query->Close();
			}
			Query->Close();
		}
		__finally
		{
			qrLocationList->Close();
			
			if (dmStockReportData->StockTrans->DefaultDatabase->Connected && dmStockReportData->StockTrans->InTransaction)
			{
				dmStockReportData->StockTrans->Commit();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::cmbLocationChange(TObject *Sender)
{
	PopulateStock();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::cbRequestTypeChange(TObject *Sender)
{
	PopulateStock();

}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::edDateRequiredChange(TObject *Sender)
{
	PopulateStock();
}
//---------------------------------------------------------------------------
void TfrmStockRequest::PopulateStock()
{
	AnsiString location = cmbLocation->Text;
	AnsiString requestType = cbRequestType->Text;
	AnsiString dateRequired = edDateRequired->Text;
	if(location == "" || requestType == "" || dateRequired == "")
	{
		return;
	}
	Location  = cmbLocation->Text;
    	if (!TransactionMM->InTransaction)
	TransactionMM->StartTransaction();
	qrLocationSetting->Close();
	qrLocationSetting->ParamByName("NAME")->AsString = location;
	qrLocationSetting->Open();
	int categoryFilterKey = qrLocationSetting->FieldByName("CATEGORY_TYPE")->AsInteger;
    int stockReqAutoKey=    qrLocationSetting->FieldByName("STOCKREQUESTAUTO_KEY")->AsInteger;
	bool CategoryCheckEnable=false;
    if(stockReqAutoKey==1)
    {
	bool canAdd = (Application->MessageBox("Add stock items for the current selected location ?", "Cancel ?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK);
	if(!canAdd)
	return;
    }
	btnAdd->Visible=true;
	BitBtn1->Visible=true;
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	//Clear all the previous data from the previous location..
	vtvStockQty->Clear();
	vtvStockQty->NodeDataSize = sizeof(TStockRequestItemNodeData);
	//We need to add items here...
	PVirtualNode OrderItemNode = NULL;
	TStockRequestItemNodeData *NodeData = NULL;
	if (!Transaction->InTransaction)
	Transaction->StartTransaction();
	/***********************************************************/

	if(categoryFilterKey==2||categoryFilterKey==4||categoryFilterKey==5||categoryFilterKey==6)
	{
		CategoryCheckEnable=true;
	}
     if(stockReqAutoKey==1)
     {
            if(CategoryCheckEnable)
            {
            PopulateStockWithCategory(location);
            }
            else
            {
            PopulateStockWithOutCategory(location);
            }

            btnAuto->Enabled=true;
    }
    else
    {  btnAuto->Enabled=false;
    }
	vtvStockQty->Refresh();
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
	if(Transaction->InTransaction)
	Transaction->Commit();
	if(TransactionMM->InTransaction)
	TransactionMM->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::btnAddClick(TObject *Sender)
{
	int counter = 0;
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	Location  = cmbLocation->Text;
	frmSelectStockItem->Reset = true;
	frmSelectStockItem->Location = cmbLocation->Text;    //Location
	if (frmSelectStockItem->ShowModal() == mrOk)
	{
		frmReceiveStockItem->StockCode = frmSelectStockItem->StockCode;
		frmReceiveStockItem->CheckLocation =cmbLocation->Text;
		frmReceiveStockItem->SupplierKey = 0;
		if (frmReceiveStockItem->Execute() == mrOk)
		{
			PVirtualNode Node = vtvStockQty->GetFirst();
			while (Node)
			{
				TStockRequestItemNodeData *NodeData	= (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
				if (NodeData->SupplierCode             ==   frmReceiveStockItem->SupplierCode &&
						NodeData->SupplierKey					== frmReceiveStockItem->SupplierKey &&
						NodeData->Text							== frmReceiveStockItem->Description &&
						NodeData->SupplierUnitCost							== frmReceiveStockItem->SupplierUnitCost &&
						NodeData->SupplierUnit					== frmReceiveStockItem->SupplierUnit)
				{
					vtvStockQty->FocusedNode				= Node;
					vtvStockQty->FocusedColumn				= 4;
					vtvStockQty->Selected[Node]			= true;
					Application->MessageBox("You have already this stock item.", "Error", MB_ICONERROR + MB_OK);
					return;
				}
				Node = vtvStockQty->GetNext(Node);
			}
			PVirtualNode OrderItemNode						= NULL;
			TStockRequestItemNodeData *NodeData		= NULL;
			if (!Transaction->InTransaction)
			Transaction->StartTransaction();

			qrStock->Close();
			qrStock->ParamByName("Supplier_Key")->AsString = AnsiString::AnsiString(frmReceiveStockItem->SupplierKey);
			qrStock->ParamByName("Stock_Key")->AsString = frmReceiveStockItem->StockKey;
			qrStock->ParamByName("Supplier_Unit")->AsString = frmReceiveStockItem->SupplierUnit;
			for (qrStock->Open(); !qrStock->Eof; qrStock->Next())
			{
				OrderItemNode										= vtvStockQty->AddChild(NULL);
				NodeData   = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
				NodeData->Code										= frmReceiveStockItem->StockCode;
				NodeData->StockKey                        = frmReceiveStockItem->StockKey;
				NodeData->Text										= frmReceiveStockItem->Description;
				NodeData->StockCategory							= frmReceiveStockItem->StockCategory;
				NodeData->StockGroup								= frmReceiveStockItem->StockGroup;
				NodeData->GLCode									= frmReceiveStockItem->GLCode;
				NodeData->Location								= frmReceiveStockItem->Location;
				NodeData->GSTPercent								= frmReceiveStockItem->GSTPercent;
				NodeData->StocktakeUnit							= frmReceiveStockItem->StocktakeUnit;
				NodeData->SupplierWeborderTargetUri    = frmReceiveStockItem->SupplierWeborderTargetUri;
				NodeData->Supplier								= frmReceiveStockItem->Supplier;
				NodeData->SupplierKey							= frmReceiveStockItem->SupplierKey;
				NodeData->SupplierCode							= frmReceiveStockItem->SupplierCode;
				NodeData->SupplierUnit							= frmReceiveStockItem->SupplierUnit;
				NodeData->SupplierUnitCost						= frmReceiveStockItem->SupplierUnitCost;
				NodeData->SupplierUnitSize						= frmReceiveStockItem->SupplierUnitSize;  
				NodeData->RequestNumber= StockRequestNumber;
				NodeData->STOCKGROUPKEY						= qrStock->FieldByName("STOCK_GROUP_KEY")->AsInteger;
				NodeData->STOCKCATEGORYKEY						= qrStock->FieldByName("Stock_Category_Key")->AsInteger;
				NodeData->SUPPLIERSTOCKKEY						= qrStock->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger;
				NodeData->RequestType=RequestType;
				if(counter == 0)
				{
					NodeData->SupplierUnitQty						= frmReceiveStockItem->SupplierUnitQty;
				}
				else
				NodeData->SupplierUnitQty = 0;
				vtvStockQty->FocusedNode						= OrderItemNode;
				vtvStockQty->FocusedColumn						= 4;
				vtvStockQty->Selected[OrderItemNode]		= true;
				counter = counter + 1;
                NodeData->onhand = frmReceiveStockItem->OnHandQty;
                NodeData->LatestCost = frmReceiveStockItem->LatestCost;
			}
			if(Transaction->InTransaction)
			Transaction->Commit();
		}
	}
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::btnRemoveClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	if (vtvStockQty->FocusedNode)
	{
		TStockRequestItemNodeData *NodeData	= (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
		if (Application->MessageBox(("Do you wish to remove the item \"" + NodeData->Text + "\" from the order?").c_str(),
					"Update?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
		{
			vtvStockQty->DeleteNode(vtvStockQty->FocusedNode);
		}
	}
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockRequest::BitBtn1Click(TObject *Sender)
{

	ContactKey=0;
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	frmSelectLocationSupplier-> Location=cmbLocation->Text;
	frmSelectLocationSupplier->Reset = true;
	if (frmSelectLocationSupplier->ShowModal() == mrOk)
	{

		ContactKey = frmSelectLocationSupplier->ContactKey;
		//  frmSelectSupplier-> Location="";

	}
	std::auto_ptr<TfrmSelectSupplierStockItem> frmSelectSupplierStockItem(new TfrmSelectSupplierStockItem(NULL));
	frmSelectSupplierStockItem->CompanyKey = ContactKey;
	frmSelectSupplierStockItem->Location=cmbLocation->Text;
	if (frmSelectSupplierStockItem->ShowModal() == mrOk)
	{
		RecieveStockItem(frmSelectSupplierStockItem->StockCode);
	}
	frmSelectLocationSupplier-> Location="";

}
//---------------------------------------------------------------------------
void TfrmStockRequest::RecieveStockItem(AnsiString StockCode)
{        int counter = 0;
	frmReceiveStockItem->CheckLocation =cmbLocation->Text;
	frmReceiveStockItem->StockCode = StockCode;
	frmReceiveStockItem->StockRequestSupplierKey=frmSelectLocationSupplier->ContactKey;
	frmReceiveStockItem->SupplierKey = 0;
	if (frmReceiveStockItem->Execute() == mrOk)
	{
		PVirtualNode Node = vtvStockQty->GetFirst();
		while (Node)
		{
			TStockRequestItemNodeData *NodeData	= (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
			if (NodeData->SupplierCode             ==   frmReceiveStockItem->SupplierCode &&
					NodeData->SupplierKey					== frmReceiveStockItem->SupplierKey &&
					NodeData->Text							== frmReceiveStockItem->Description &&
					NodeData->SupplierUnitCost							== frmReceiveStockItem->SupplierUnitCost &&
					NodeData->SupplierUnit					== frmReceiveStockItem->SupplierUnit)
			{
				vtvStockQty->FocusedNode				= Node;
				vtvStockQty->FocusedColumn				= 4;
				vtvStockQty->Selected[Node]			= true;
				Application->MessageBox("You have already this stock item.", "Error", MB_ICONERROR + MB_OK);
				return;
			}
			Node = vtvStockQty->GetNext(Node);
		}
		PVirtualNode OrderItemNode						= NULL;
		TStockRequestItemNodeData *NodeData		= NULL;
		if (!Transaction->InTransaction)
		Transaction->StartTransaction();
		qrStock->Close();
		qrStock->ParamByName("Supplier_Key")->AsString = AnsiString::AnsiString(frmReceiveStockItem->SupplierKey);
		qrStock->ParamByName("Stock_Key")->AsString = frmReceiveStockItem->StockKey;
		qrStock->ParamByName("Supplier_Unit")->AsString = frmReceiveStockItem->SupplierUnit;
		for (qrStock->Open(); !qrStock->Eof; qrStock->Next())
		{
			OrderItemNode										= vtvStockQty->AddChild(NULL);
			NodeData   = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
			NodeData->Code										= frmReceiveStockItem->StockCode;
			NodeData->StockKey                        = frmReceiveStockItem->StockKey;
			NodeData->Text										= frmReceiveStockItem->Description;
			NodeData->StockCategory							= frmReceiveStockItem->StockCategory;
			NodeData->StockGroup								= frmReceiveStockItem->StockGroup;
			NodeData->GLCode									= frmReceiveStockItem->GLCode;
			NodeData->Location								= frmReceiveStockItem->Location;
			NodeData->GSTPercent								= frmReceiveStockItem->GSTPercent;
			NodeData->StocktakeUnit							= frmReceiveStockItem->StocktakeUnit;
			NodeData->SupplierWeborderTargetUri    = frmReceiveStockItem->SupplierWeborderTargetUri;
			NodeData->Supplier								= frmReceiveStockItem->Supplier;
			NodeData->SupplierKey							= frmReceiveStockItem->SupplierKey;
			NodeData->SupplierCode							= frmReceiveStockItem->SupplierCode;
			NodeData->SupplierUnit							= frmReceiveStockItem->SupplierUnit;
			NodeData->SupplierUnitCost						= frmReceiveStockItem->SupplierUnitCost;
			NodeData->SupplierUnitSize						= frmReceiveStockItem->SupplierUnitSize;
			NodeData->STOCKGROUPKEY						= qrStock->FieldByName("STOCK_GROUP_KEY")->AsInteger;
			NodeData->STOCKCATEGORYKEY						= qrStock->FieldByName("Stock_Category_Key")->AsInteger;
			NodeData->SUPPLIERSTOCKKEY						= qrStock->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger;
			if(counter == 0)
			{
				NodeData->SupplierUnitQty						= frmReceiveStockItem->SupplierUnitQty;
			}
			else
			NodeData->SupplierUnitQty = 0;
			vtvStockQty->FocusedNode						= OrderItemNode;
			vtvStockQty->FocusedColumn						= 4;
			vtvStockQty->Selected[OrderItemNode]		= true;
			counter = counter + 1;
            NodeData->onhand = frmReceiveStockItem->OnHandQty;
            NodeData->LatestCost = frmReceiveStockItem->LatestCost;
		}
		if(Transaction->InTransaction)
		Transaction->Commit();
	} }
void TfrmStockRequest::RemovedStockRequestUpdate()
{
	/***********************************************************/
	qrStockReqStock->Close();
	qrStockReqStock->ParamByName("REQUEST_NUMBER")->AsInteger = StockRequestNumber;
	for (qrStockReqStock->Open(); !qrStockReqStock->Eof; qrStockReqStock->Next())
	{
		int supplierStockkey= qrStockReqStock->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger ;
		qrStockRequestSelect->Close();
		qrStockRequestSelect->ParamByName("REQUEST_NUMBER")->AsInteger = StockRequestNumber;
		qrStockRequestSelect->ParamByName("SUPPLIER_STOCK_KEY")->AsInteger = supplierStockkey;
		for (qrStockRequestSelect->Open(); !qrStockRequestSelect->Eof; qrStockRequestSelect->Next())
		{     	PVirtualNode Node = vtvStockQty->GetFirst();
			int StockReqStockkey =	qrStockRequestSelect->FieldByName("STOCK_REQUEST_STOCKKEY")->AsInteger;
			Node = vtvStockQty->GetFirst();
			bool isStockreqstockKeyExist=false;
			while (Node)
			{
				TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node);
				if(NodeData->StockReqStockRequestNumber==StockReqStockkey)
				{isStockreqstockKeyExist=true;
					break;}
				Node = vtvStockQty->GetNext(Node);
			}
			if(!isStockreqstockKeyExist)
			{      qrUpdateStockReqKey->Close();
				qrUpdateStockReqKey->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger = StockReqStockkey;
				qrUpdateStockReqKey->ParamByName("ISREMOVED")->AsString = 'T';
				qrUpdateStockReqKey->ExecSQL();
			}
			else
			{      qrUpdateStockReqKey->Close();
				qrUpdateStockReqKey->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger = StockReqStockkey;
				qrUpdateStockReqKey->ParamByName("ISREMOVED")->AsString = 'T';
				qrUpdateStockReqKey->ExecSQL();
			}
		}
	}
}
/***********************************************************/
void TfrmStockRequest::PopulateStockWithCategory(AnsiString Location)
{
	AnsiString location = cmbLocation->Text;
	AnsiString requestType = cbRequestType->Text;
	AnsiString dateRequired = edDateRequired->Text;
	vtvStockQty->Clear();
	vtvStockQty->NodeDataSize = sizeof(TStockRequestItemNodeData);
	//We need to add items here...
	PVirtualNode OrderItemNode = NULL;
	TStockRequestItemNodeData *NodeData = NULL;
	/***********************************************************/
	if(Location!="")
	{
		frmStockGroup->StockMode = smSelect;
		frmStockGroup->Mode ="All";
		frmStockGroup->Location =Location;
		if (frmStockGroup->ShowModal() == mrOk)
		{
			if (!Transaction->InTransaction)
			Transaction->StartTransaction();
			for(int i = 0 ; i <frmStockGroup->SelectedCategoryList->Count; i++)
			{
				/*
				qrStockByLocation->Close();
				qrStockByLocation->ParamByName("Location")->AsString = StockLocation;
				qrStockByLocation->ParamByName("Stock_Category")->AsString =  frmStockGroup->SelectedCategoryList->Strings[i];
				int counter = 0;
				for (qrStockByLocation->Open(); !qrStockByLocation->Eof; qrStockByLocation->Next())
				{
				}
			*/
				/***********************************************************/
				qrStockByLocation->Close();
				qrStockByLocation->ParamByName("Location")->AsString = Location;
				qrStockByLocation->ParamByName("STOCK_CATEGORY")->AsString =  frmStockGroup->SelectedCategoryList->Strings[i];
				int counter = 0;
				for (qrStockByLocation->Open(); !qrStockByLocation->Eof; qrStockByLocation->Next())
				{
					qrCheckSupplier->Close();
					qrCheckSupplier->ParamByName("Stock_Key")->AsInteger = qrStockByLocation->FieldByName("STOCK_KEY")->AsInteger;
					qrCheckSupplier->Open();
					qrCheckSupplier->First();
					AnsiString supplierExist=   qrCheckSupplier->FieldByName("COMPANY_NAME")->AsString;
					if( supplierExist!="")
					{
						OrderItemNode = vtvStockQty->AddChild(NULL);
						NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
						NodeData->Code = qrStockByLocation->FieldByName("CODE")->AsString;
						NodeData->StockKey = qrStockByLocation->FieldByName("STOCK_KEY")->AsInteger;
						NodeData->Text = qrStockByLocation->FieldByName("DESCRIPTION")->AsString;
						NodeData->StockCategory = qrStockByLocation->FieldByName("STOCK_CATEGORY")->AsString;
						NodeData->StockGroup = qrStockByLocation->FieldByName("STOCK_GROUP")->AsString;
						NodeData->GLCode = qrStockByLocation->FieldByName("GL_CODE")->AsString;
						NodeData->Location = qrStockByLocation->FieldByName("LOCATION")->AsString;
						NodeData->GSTPercent = qrStockByLocation->FieldByName("GST_PERCENT")->AsFloat;
						NodeData->StocktakeUnit = qrStockByLocation->FieldByName("STOCKTAKE_UNIT")->AsString;
						NodeData->onhand = qrStockByLocation->FieldByName("ON_HAND")->AsFloat;
						NodeData->LatestCost=    qrStockByLocation->FieldByName("LATEST_COST")->AsFloat;
						qrStockSupplier->Close();
						qrStockSupplier->ParamByName("Stock_Key")->AsInteger = qrStockByLocation->FieldByName("Stock_Key")->AsInteger;
						qrStockSupplier->Open();    //SS.DEFAULT_ORDER_UNIT,
						bool isPrefferedSupplierSelected = qrStockSupplier->Locate("Preferred_Supplier", "T", TLocateOptions());
						bool isDefaultUnit=  qrStockSupplier->Locate("DEFAULT_ORDER_UNIT", "T", TLocateOptions());
						//qrStockPrefferedSupplier
						if(!isPrefferedSupplierSelected )
						{
							qrStockSupplier->First();
						}
						if(isPrefferedSupplierSelected)
						{
							qrStockPrefferedSupplier->Close();
							qrStockPrefferedSupplier->ParamByName("Stock_Key")->AsInteger = qrStockByLocation->FieldByName("Stock_Key")->AsInteger;
							qrStockPrefferedSupplier->Open();
							bool isDefaultPrefUnit=  qrStockPrefferedSupplier->Locate("DEFAULT_ORDER_UNIT", "T", TLocateOptions());
							if(isPrefferedSupplierSelected && isDefaultPrefUnit)
							{
								qrStockPrefferedSupplier->Last();
							}
							if(isPrefferedSupplierSelected && !isDefaultPrefUnit)
							{   qrStockPrefferedSupplier->First();}
						}
						if(!isPrefferedSupplierSelected)
						{
							NodeData->Supplier = qrStockSupplier->FieldByName("COMPANY_NAME")->AsString;
							NodeData->SupplierKey = qrStockSupplier->FieldByName("SUPPLIER_KEY")->AsInteger;
							NodeData->SupplierCode = qrStockSupplier->FieldByName("SUPPLIER_CODE")->AsString;
							NodeData->SUPPLIERSTOCKKEY =qrStockSupplier->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger;
						}
						else
						{    NodeData->Supplier = qrStockPrefferedSupplier->FieldByName("COMPANY_NAME")->AsString;
							NodeData->SupplierKey = qrStockPrefferedSupplier->FieldByName("SUPPLIER_KEY")->AsInteger;
							NodeData->SupplierCode = qrStockPrefferedSupplier->FieldByName("SUPPLIER_CODE")->AsString;
							NodeData->SUPPLIERSTOCKKEY =qrStockPrefferedSupplier->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger;
						}
						if(requestType == "Transfer")
						{
							NodeData->SupplierUnit = qrStockByLocation->FieldByName("STOCKTAKE_UNIT")->AsString;
							if(!isPrefferedSupplierSelected)
							{
								NodeData->SupplierUnitCost = qrStockSupplier->FieldByName("COST")->AsFloat;
							}
							else
							{ NodeData->SupplierUnitCost = qrStockPrefferedSupplier->FieldByName("COST")->AsFloat;
							}
						}
						else
						{
							if(!isPrefferedSupplierSelected)
							{
								NodeData->SupplierUnit = qrStockSupplier->FieldByName("SUPPLIER_UNIT")->AsString;
								NodeData->SupplierUnitCost = qrStockSupplier->FieldByName("LATEST_COST")->AsFloat;
								NodeData->SupplierUnitSize = qrStockSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
							}
							else
							{
								NodeData->SupplierUnit = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT")->AsString;
								NodeData->SupplierUnitCost = qrStockPrefferedSupplier->FieldByName("LATEST_COST")->AsFloat;
								NodeData->SupplierUnitSize = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
							}
						}
						NodeData->STOCKGROUPKEY = qrStockByLocation->FieldByName("STOCK_GROUP_KEY")->AsInteger;
						NodeData->STOCKCATEGORYKEY = qrStockByLocation->FieldByName("STOCK_CATEGORY_KEY")->AsInteger;
						//Set the order quantity to 0 as it will be filled in by the user..
						NodeData->SupplierUnitQty = 0;
						qrStockONhand->Close();
						qrStockONhand->ParamByName("Stock_Key")->AsInteger = qrStockByLocation->FieldByName("Stock_Key")->AsInteger;
						qrStockONhand->ParamByName("Location")->AsString = cmbLocation->Text;
						qrStockONhand->Open();
						//qrStockONhand
						float onhand = qrStockONhand->FieldByName("On_Hand")->AsFloat;
						float onOrder = qrStockONhand->FieldByName("On_Order")->AsFloat;
						float minLevel = qrStockONhand->FieldByName("Min_Level")->AsFloat;
						float maxLevel = qrStockONhand->FieldByName("Max_Level")->AsFloat;
						float qty=0;
						if(!isPrefferedSupplierSelected)
						{
							qty = qrStockSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
						}
						else{
							qty = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
						}
						int finalval=( maxLevel/qty ) -((onhand/qty)+(onOrder/qty));
						if((onhand+onOrder)<=minLevel)
						{  if(finalval<0)
							finalval=0;
							NodeData->SupplierUnitQty= finalval;
						}
						else {NodeData->SupplierUnitQty = 0;}

                        NodeData->SupplierUnitQty = 0;
						if(counter == 0)
						{
							vtvStockQty->FocusedNode = OrderItemNode;
							vtvStockQty->FocusedColumn = 4;
							vtvStockQty->Selected[OrderItemNode] = true;
						}
						++counter;
					}
				}//
			}
		}
	}
	vtvStockQty->Refresh();
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
void TfrmStockRequest::PopulateStockWithOutCategory(AnsiString Location)
{  
	AnsiString location = cmbLocation->Text;
	AnsiString requestType = cbRequestType->Text;
	AnsiString dateRequired = edDateRequired->Text;
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	//Clear all the previous data from the previous location..
	vtvStockQty->Clear();
	vtvStockQty->NodeDataSize = sizeof(TStockRequestItemNodeData);
	//We need to add items here...
	PVirtualNode OrderItemNode = NULL;
	TStockRequestItemNodeData *NodeData = NULL;
	qrStockLcationWithoutCat->Close();
	qrStockLcationWithoutCat->ParamByName("Location")->AsString = location;
	int counter = 0;
	for (qrStockLcationWithoutCat->Open(); !qrStockLcationWithoutCat->Eof; qrStockLcationWithoutCat->Next())
	{
		qrCheckSupplier->Close();
		qrCheckSupplier->ParamByName("Stock_Key")->AsInteger = qrStockLcationWithoutCat->FieldByName("STOCK_KEY")->AsInteger;
		qrCheckSupplier->Open();
		qrCheckSupplier->First();
		AnsiString supplierExist=   qrCheckSupplier->FieldByName("COMPANY_NAME")->AsString;
		if( supplierExist!="")
		{
			OrderItemNode = vtvStockQty->AddChild(NULL);
			NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
			NodeData->Code = qrStockLcationWithoutCat->FieldByName("CODE")->AsString;
			NodeData->StockKey = qrStockLcationWithoutCat->FieldByName("STOCK_KEY")->AsInteger;
			NodeData->Text = qrStockLcationWithoutCat->FieldByName("DESCRIPTION")->AsString;
			NodeData->StockCategory = qrStockLcationWithoutCat->FieldByName("STOCK_CATEGORY")->AsString;
			NodeData->StockGroup = qrStockLcationWithoutCat->FieldByName("STOCK_GROUP")->AsString;
			NodeData->GLCode = qrStockLcationWithoutCat->FieldByName("GL_CODE")->AsString;
			NodeData->Location = qrStockLcationWithoutCat->FieldByName("LOCATION")->AsString;
			NodeData->GSTPercent = qrStockLcationWithoutCat->FieldByName("GST_PERCENT")->AsFloat;
			NodeData->StocktakeUnit = qrStockLcationWithoutCat->FieldByName("STOCKTAKE_UNIT")->AsString;
			NodeData->onhand = qrStockLcationWithoutCat->FieldByName("ON_HAND")->AsFloat;
			NodeData->LatestCost=    qrStockLcationWithoutCat->FieldByName("LATEST_COST")->AsFloat;
			qrStockSupplier->Close();
			qrStockSupplier->ParamByName("Stock_Key")->AsInteger = qrStockLcationWithoutCat->FieldByName("Stock_Key")->AsInteger;
			qrStockSupplier->Open();    //SS.DEFAULT_ORDER_UNIT,
			bool isPrefferedSupplierSelected = qrStockSupplier->Locate("Preferred_Supplier", "T", TLocateOptions());
			bool isDefaultUnit=  qrStockSupplier->Locate("DEFAULT_ORDER_UNIT", "T", TLocateOptions());
			//qrStockPrefferedSupplier
			if(!isPrefferedSupplierSelected )
			{
				qrStockSupplier->First();
			}
			if(isPrefferedSupplierSelected)
			{
				qrStockPrefferedSupplier->Close();
				qrStockPrefferedSupplier->ParamByName("Stock_Key")->AsInteger = qrStockLcationWithoutCat->FieldByName("Stock_Key")->AsInteger;
				qrStockPrefferedSupplier->Open();
				bool isDefaultPrefUnit=  qrStockPrefferedSupplier->Locate("DEFAULT_ORDER_UNIT", "T", TLocateOptions());
				if(isPrefferedSupplierSelected && isDefaultPrefUnit)
				{
					qrStockPrefferedSupplier->Last();
				}
				if(isPrefferedSupplierSelected && !isDefaultPrefUnit)
				{   qrStockPrefferedSupplier->First();}
			}
			if(!isPrefferedSupplierSelected)
			{
				NodeData->Supplier = qrStockSupplier->FieldByName("COMPANY_NAME")->AsString;
				NodeData->SupplierKey = qrStockSupplier->FieldByName("SUPPLIER_KEY")->AsInteger;
				NodeData->SupplierCode = qrStockSupplier->FieldByName("SUPPLIER_CODE")->AsString;
				NodeData->SUPPLIERSTOCKKEY =qrStockSupplier->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger;
			}
			else
			{    NodeData->Supplier = qrStockPrefferedSupplier->FieldByName("COMPANY_NAME")->AsString;
				NodeData->SupplierKey = qrStockPrefferedSupplier->FieldByName("SUPPLIER_KEY")->AsInteger;
				NodeData->SupplierCode = qrStockPrefferedSupplier->FieldByName("SUPPLIER_CODE")->AsString;
				NodeData->SUPPLIERSTOCKKEY =qrStockPrefferedSupplier->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger;
			}
			if(requestType == "Transfer")
			{
				NodeData->SupplierUnit = qrStockLcationWithoutCat->FieldByName("STOCKTAKE_UNIT")->AsString;
				if(!isPrefferedSupplierSelected)
				{
					NodeData->SupplierUnitCost = qrStockSupplier->FieldByName("COST")->AsFloat;
				}
				else
				{ NodeData->SupplierUnitCost = qrStockPrefferedSupplier->FieldByName("COST")->AsFloat;
				}
			}
			else
			{
				if(!isPrefferedSupplierSelected)
				{
					NodeData->SupplierUnit = qrStockSupplier->FieldByName("SUPPLIER_UNIT")->AsString;
					NodeData->SupplierUnitCost = qrStockSupplier->FieldByName("LATEST_COST")->AsFloat;
					NodeData->SupplierUnitSize = qrStockSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
				}
				else
				{
					NodeData->SupplierUnit = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT")->AsString;
					NodeData->SupplierUnitCost = qrStockPrefferedSupplier->FieldByName("LATEST_COST")->AsFloat;
					NodeData->SupplierUnitSize = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
				}
			}
			
			NodeData->STOCKGROUPKEY = qrStockLcationWithoutCat->FieldByName("STOCK_GROUP_KEY")->AsInteger;
			NodeData->STOCKCATEGORYKEY = qrStockLcationWithoutCat->FieldByName("STOCK_CATEGORY_KEY")->AsInteger;
			//Set the order quantity to 0 as it will be filled in by the user..
			NodeData->SupplierUnitQty = 0;
			qrStockONhand->Close();
			qrStockONhand->ParamByName("Stock_Key")->AsInteger = qrStockLcationWithoutCat->FieldByName("Stock_Key")->AsInteger;
			qrStockONhand->ParamByName("Location")->AsString = cmbLocation->Text;
			qrStockONhand->Open();
			//qrStockONhand
			float onhand = qrStockONhand->FieldByName("On_Hand")->AsFloat;
			float onOrder = qrStockONhand->FieldByName("On_Order")->AsFloat;
			float minLevel = qrStockONhand->FieldByName("Min_Level")->AsFloat;
			float maxLevel = qrStockONhand->FieldByName("Max_Level")->AsFloat;
			float qty=0;
			if(!isPrefferedSupplierSelected)
			{
				qty = qrStockSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
			}
			else{
				qty = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
			}
			int finalval=( maxLevel/qty ) -((onhand/qty)+(onOrder/qty));
			if((onhand+onOrder)<=minLevel)
			{  if(finalval<0)
				finalval=0;
				NodeData->SupplierUnitQty= finalval;
			}
			else {NodeData->SupplierUnitQty = 0;}
            NodeData->SupplierUnitQty = 0;
			if(counter == 0)
			{
				vtvStockQty->FocusedNode = OrderItemNode;
				vtvStockQty->FocusedColumn = 4;
				vtvStockQty->Selected[OrderItemNode] = true;
			}
			++counter;
		}
	}
	vtvStockQty->Refresh();
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
void __fastcall TfrmStockRequest::btnAutoClick(TObject *Sender)
{
  	AnsiString location = cmbLocation->Text;
	AnsiString requestType = cbRequestType->Text;
	AnsiString dateRequired = edDateRequired->Text;
	 if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	vtvStockQty->NodeDataSize = sizeof(TStockRequestItemNodeData);
	//We need to add items here...
	PVirtualNode OrderItemNode = NULL;
	TStockRequestItemNodeData *NodeData = NULL;
	qrStockLcationWithoutCat->Close();
	qrStockLcationWithoutCat->ParamByName("Location")->AsString = location;
	int counter = 0;
     	PVirtualNode Node = vtvStockQty->GetFirst();
       while (Node)
	{
	    	TStockRequestItemNodeData *NodeData = (TStockRequestItemNodeData *)vtvStockQty->GetNodeData(Node); 
			qrStockSupplier->Close();
			qrStockSupplier->ParamByName("Stock_Key")->AsInteger = 	NodeData->StockKey  ;
			qrStockSupplier->Open();    //SS.DEFAULT_ORDER_UNIT,
			bool isPrefferedSupplierSelected = qrStockSupplier->Locate("Preferred_Supplier", "T", TLocateOptions());
			bool isDefaultUnit=  qrStockSupplier->Locate("DEFAULT_ORDER_UNIT", "T", TLocateOptions());
			//qrStockPrefferedSupplier
			if(!isPrefferedSupplierSelected )
			{
				qrStockSupplier->First();
			}
			if(isPrefferedSupplierSelected)
			{
				qrStockPrefferedSupplier->Close();
				qrStockPrefferedSupplier->ParamByName("Stock_Key")->AsInteger = 	NodeData->StockKey  ;
				qrStockPrefferedSupplier->Open();
				bool isDefaultPrefUnit=  qrStockPrefferedSupplier->Locate("DEFAULT_ORDER_UNIT", "T", TLocateOptions());
				if(isPrefferedSupplierSelected && isDefaultPrefUnit)
				{
					qrStockPrefferedSupplier->Last();
				}
				if(isPrefferedSupplierSelected && !isDefaultPrefUnit)
				{   qrStockPrefferedSupplier->First();}
			}
			 
		 
			

			NodeData->SupplierUnitQty = 0;
			qrStockONhand->Close();
			qrStockONhand->ParamByName("Stock_Key")->AsInteger = 	NodeData->StockKey  ;
			qrStockONhand->ParamByName("Location")->AsString = cmbLocation->Text;
			qrStockONhand->Open();
			//qrStockONhand
			float onhand = qrStockONhand->FieldByName("On_Hand")->AsFloat;
			float onOrder = qrStockONhand->FieldByName("On_Order")->AsFloat;
			float minLevel = qrStockONhand->FieldByName("Min_Level")->AsFloat;
			float maxLevel = qrStockONhand->FieldByName("Max_Level")->AsFloat;
			float qty=0;
			if(!isPrefferedSupplierSelected)
			{
				qty = qrStockSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
			}
			else{
				qty = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
			}
			int finalval=( maxLevel/qty ) -((onhand/qty)+(onOrder/qty));
			if((onhand+onOrder)<=minLevel)
			{  if(finalval<0)
				finalval=0;
				NodeData->SupplierUnitQty= finalval;
			}
			else {NodeData->SupplierUnitQty = 0;}
			if(counter == 0)
			{
				vtvStockQty->FocusedNode = OrderItemNode;
				vtvStockQty->FocusedColumn = 4;
				vtvStockQty->Selected[OrderItemNode] = true;
			}
			++counter;
             Node = vtvStockQty->GetNext(Node);
		 

	}
 	vtvStockQty->Refresh();
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TfrmStockRequest::neStockQtyKeyPress(TObject *Sender,
      char &Key)
{
   if(Key == '-')
   {
      Key = NULL;
   }
}
//---------------------------------------------------------------------------

