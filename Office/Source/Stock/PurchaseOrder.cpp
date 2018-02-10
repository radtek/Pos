//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "StockGroup.h"
#include "SelectSupplierStockItem.h"
#include "PurchaseOrder.h"
#include "StockData.h"
#include "SelectDate.h"
#include "Login.h"
#include "Connections.h"
#include "StockReportData.h"
#include "Reports.h"
#include "Utilities.h"
#include "SelectStockItem.h"
#include "SelectSupplier.h"
#include "MM_Math.h"
#include "MMRegistry.h"
#include "web_order.h"
#include "processingWeborder.h"
#include "SelectSupplier.h"
#include <memory>
//#include <QtGui>
using std::auto_ptr;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma link "NumericEdit"
#pragma link "OpOlkXP"
#pragma link "OpOutlk"
#pragma link "OpShared"
#pragma resource "*.dfm"
//TfrmPurchaseOrder *frmPurchaseOrder;
//---------------------------------------------------------------------------
__fastcall TfrmPurchaseOrder::TfrmPurchaseOrder(TComponent* Owner)
: TForm(Owner),
frmReceiveStockItem(new TfrmReceiveStockItem(NULL))
{
}
//---------------------------------------------------------------------------
TModalResult TfrmPurchaseOrder::Execute()
{	Initialise();
   vtvStockQty->Clear();
   neCost->DecimalPlaces = CurrentConnection.SettingDecimalPlaces;
   neStockQty->DecimalPlaces = CurrentConnection.SettingDecimalPlaces;
   neTotalCost->DecimalPlaces = CurrentConnection.SettingDecimalPlaces;
	vtvStockQty->NodeDataSize = sizeof(TOrderItemNodeData);
	if (OrderKey)
	{
		LoadOrder();
	}
	ShowModal();
	return ModalResult;
}//---------------------------------------------------------------------------
TModalResult TfrmPurchaseOrder::ExecutePreDef()
{	Initialise();
	//Set Stock Request Mode
	//btnAdd->Enabled = !IsStockRequestMode;
	BitBtn1->Enabled = !IsStockRequestMode;
	btnAuto->Enabled = !IsStockRequestMode;
	//btnRemove->Enabled = !IsStockRequestMode;
	ShowModal();
	return ModalResult;
}//---------------------------------------------------------------------------
void TfrmPurchaseOrder::InitialisePreDef(int SupplierKey, AnsiString weborder_uri)
{	ContactKey	= SupplierKey;
	btnCommitAndSubmitWeborder->Enabled =  weborder_uri != NULL && (this->weborder_uri = weborder_uri).Length() > 0;
	OrderKey		= 0;
	ContactName = "";
   vtvStockQty->Clear();
	vtvStockQty->NodeDataSize = sizeof(TOrderItemNodeData);
}//---------------------------------------------------------------------------
void TfrmPurchaseOrder::Initialise()
{	if (!Transaction->InTransaction)
	Transaction->StartTransaction();
	qrSupplier->Close();
	qrSupplier->ParamByName("Contact_Key")->AsInteger	= ContactKey;
	qrSupplier->Open();
	lbeTitle->Caption												= qrSupplier->FieldByName("Company_Name")->AsString;
	// if they can Email the Purchase Order
	if (qrSupplier->FieldByName("Email")->AsString == "")
	{
		btnCommitEmail->Enabled = false;
		EmailAddress = "";
	}
	else
	{
		btnCommitEmail->Enabled = true;
		EmailAddress = qrSupplier->FieldByName("Email")->AsString;
	}
	dtUser->Close();
	dtUser->ParamByName("User_ID")->AsString				= frmLogin->CurrentUser.UserID;
	dtUser->Open();
	if (OrderKey == 0)
	{
		edOrderNumber->Text					= "";
		edFAX->Text								= qrSupplier->FieldByName("FAX")->AsString;
		memSupplierAddress->Lines->Text	= qrSupplier->FieldByName("Postal_Address")->AsString;
		memDeliveryAddress->Text			= CurrentConnection.DeliveryAddress;
		memInstructions->Text				= CurrentConnection.OrderInstructions;
		if(double(DateRequired) != 0.0)
		edDateRequired->Text =  DateRequired ;
		else
		edDateRequired->Text =  "" ;
	}
	if (Transaction->InTransaction)
	Transaction->Commit();
}//---------------------------------------------------------------------------
void TfrmPurchaseOrder::LoadOrder()
{	if (!Transaction->InTransaction)
	Transaction->StartTransaction();
	qrPurchaseOrder->Close();
	qrPurchaseOrder->ParamByName("Order_Key")->AsInteger = OrderKey;
	qrPurchaseOrder->Open();
	edOrderNumber->Text			= qrPurchaseOrder->FieldByName("Order_Number")->AsString;
	edFAX->Text						= qrPurchaseOrder->FieldByName("Supplier_FAX")->AsString;
	memSupplierAddress->Text	= qrPurchaseOrder->FieldByName("Supplier_Address")->AsString;
	memDeliveryAddress->Text	= qrPurchaseOrder->FieldByName("Delivery_Address")->AsString;
	memInstructions->Text		= qrPurchaseOrder->FieldByName("Instructions")->AsString;
	DateRequired					= qrPurchaseOrder->FieldByName("Required")->AsDateTime;
	if (double(DateRequired) != 0.0)
	{
		edDateRequired->Text	= DateRequired.DateString();
	}
	qrPurchaseStock->Close();
	qrPurchaseStock->ParamByName("Order_Key")->AsInteger = OrderKey;
	for (qrPurchaseStock->Open(); !qrPurchaseStock->Eof; qrPurchaseStock->Next())
	{
		PVirtualNode OrderItemNode					= NULL;
		TOrderItemNodeData *NodeData				= NULL;
		OrderItemNode									= vtvStockQty->AddChild(NULL);
		NodeData											= (TOrderItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
		//		NodeData->OrderItemKey						= qrPurchaseStock->FieldByName("Purchase_Stock_LK")->AsInteger;
		NodeData->Text									= qrPurchaseStock->FieldByName("Description")->AsString;
		NodeData->Code									= qrPurchaseStock->FieldByName("Code")->AsString;
		NodeData->GLCode								= qrPurchaseStock->FieldByName("GL_Code")->AsString;
		NodeData->Location							= qrPurchaseStock->FieldByName("Location")->AsString;
		NodeData->GSTPercent							= qrPurchaseStock->FieldByName("GST_Percent")->AsFloat;
		NodeData->StocktakeUnit						= qrPurchaseStock->FieldByName("Stocktake_Unit")->AsString;
		NodeData->SupplierCode						= qrPurchaseStock->FieldByName("Supplier_Code")->AsString;
		NodeData->SupplierUnit						= qrPurchaseStock->FieldByName("Supplier_Unit")->AsString;
        NodeData->SupplierUnitCost                  = StrToFloat(FloatToStrF(qrPurchaseStock->FieldByName("Supplier_Unit_Cost")->AsFloat,ffFixed,19, CurrentConnection.SettingDecimalPlaces));
       	NodeData->SupplierUnitQty                   = StrToFloat(FloatToStrF(qrPurchaseStock->FieldByName("Supplier_Unit_Qty")->AsFloat,ffFixed,19, CurrentConnection.SettingDecimalPlaces));
		//NodeData->SupplierUnitSize					= qrPurchaseStock->FieldByName("Stock_Qty")->AsFloat / qrPurchaseStock->FieldByName("Order_Qty")->AsFloat;
		NodeData->SupplierUnitSize					= qrPurchaseStock->FieldByName("Supplier_Unit_Size")->AsFloat;
		//NodeData->SupplierUnitQty					= qrPurchaseStock->FieldByName("Supplier_Unit_Qty")->AsFloat;
	}
	if (Transaction->InTransaction)
	Transaction->Commit();
}//---------------------------------------------------------------------------
int __fastcall TfrmPurchaseOrder::ShowModal()
{	return TForm::ShowModal();
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::FormShow(TObject *Sender)
{	btnNextOrderNumber->SetFocus();
	ShowTotals();
	btnNextOrderNumberClick(btnNextOrderNumber);
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::btnOkClick(TObject *Sender)
{	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	if (edOrderNumber->Text == "")
	{
		Application->MessageBox("Please enter an order number.", "Error", MB_ICONERROR + MB_OK);
		edOrderNumber->SetFocus();
		return;
	}
	try
	{
		if (!Transaction->InTransaction)
		Transaction->StartTransaction();
		if (SaveOrder())
		{
			if (Transaction->InTransaction)
			Transaction->Commit();
			ModalResult = mrOk;
		}
		else
		{
			if (Transaction->InTransaction) Transaction->Rollback();
		}
	}
	catch (Exception &E)
	{
		if (Transaction->InTransaction)
		Transaction->Rollback();
		Application->ShowException(&E);
		ModalResult = mrCancel;
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::btnCommitClick(TObject *Sender)
{
    if(!CheckNegativeValue())
    {
        return;
    }
    if (!is_okay_to_commit())
	return;
	DoCommit(reinterpret_cast<TBitBtn *>(Sender)->Tag);
	/*
	bool Email = ((TBitBtn *)Sender)->Tag;
	btnCommit->Enabled = false;
	btnCommitEmail->Enabled = false;
	TCursor SaveCursor = Screen->Cursor;
	try
	{
		Screen->Cursor     = crHourGlass;
		if (vtvStockQty->IsEditing())
		{
			vtvStockQty->EndEditNode();
		}
		if (edOrderNumber->Text == "")
		{
			Application->MessageBox("Please enter an order number.", "Error", MB_ICONERROR + MB_OK);
			edOrderNumber->SetFocus();
			btnCommit->Enabled = true;
			if (EmailAddress != "")
				btnCommitEmail->Enabled = true;
			return;
		}
		if (vtvStockQty->GetFirst() == NULL)
		{
			Application->MessageBox("You have not entered any stock.", "Error", MB_ICONERROR + MB_OK);
			btnAdd->SetFocus();
			btnCommit->Enabled = true;
			if (EmailAddress != "")
				btnCommitEmail->Enabled = true;
			return;
		}
		PVirtualNode Node = vtvStockQty->GetFirst();
		while (Node)
		{
			TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(Node);
			if (NodeData->SupplierUnitQty == 0)
			{
				vtvStockQty->FocusedNode		= Node;
				vtvStockQty->FocusedColumn		= 4;
				vtvStockQty->Selected[Node]	= true;
				Application->MessageBox("You have not entered a quantity for this item.", "Error", MB_ICONERROR + MB_OK);
				if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
				{
					vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
				}
				btnCommit->Enabled = true;
				if (EmailAddress != "")
					btnCommitEmail->Enabled = true;
				return;
			}
			Node = vtvStockQty->GetNext(Node);
		}
		try
		{
			if (!Transaction->InTransaction) Transaction->StartTransaction();
			if (SaveOrder())
			{
				Screen->Cursor     = SaveCursor;
				if (Application->MessageBox("You will not be able to make any more changes to this order.\r"
													"Do you want to continue?", "Commit?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
				{
					Screen->Cursor     = crHourGlass;
					CommitOrder();
					Transaction->Commit();
					dmStockReportData->StockTrans->StartTransaction();
					try
					{
						dmStockReportData->SetupPurchaseOrder(OrderKey);
						if (frmReports->rvStock->SelectReport("repPurchaseOrder", false))
						{
							frmReports->rvStock->SetParam("CompanyDetails", CurrentConnection.ReportHeader + "\r\r");
							frmReports->rvStock->SetParam(
							"ItemCost",
							IntToStr(!CurrentConnection.DontShowItemCostInPurchaseOrder));
							Screen->Cursor     = SaveCursor;
							if (Email == true)
							{
								try
								{
									AnsiString EmailFileName = "";
									bool Result = frmReports->SaveReportToFile(EmailFileName);
									if (Result)
									{
										// Purchase Order.htm is sometime written as Purchase Order1.htm
										// Delete all Purchase Order*.htm then get the first one
										if (EmailFileName.AnsiPos("htm") > 0)
										{
											dmUtilities->DeleteWildCardFile(CurrentConnection.MenuSavePath + "\\Purchase Order*.htm");
										}
										// Execute writes to a file
										frmReports->rvStock->Execute();
										if (EmailFileName.AnsiPos("htm") > 0)
										{
											EmailFileName = dmUtilities->FindWildCardFile(CurrentConnection.MenuSavePath + "\\Purchase Order*.htm");
										}
										// Email
										frmReports->SendEmail( EmailFileName, EmailAddress, "Purchase Order");
									}
								}
								__finally
								{
									frmReports->SetToPreview();
								}
							}
							else
							{
								frmReports->rvStock->Execute();
							}
						}
					}
					__finally
					{
						dmStockReportData->StockTrans->Commit();
					}
					ModalResult = mrOk;
				}
			}
			if (Transaction->InTransaction) Transaction->Rollback();
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction) Transaction->Rollback();
			btnCommit->Enabled = true;
			if (EmailAddress != "")
				btnCommitEmail->Enabled = true;
			Application->ShowException(&E);
			ModalResult = mrCancel;
		}
	}
	__finally
	{
		Screen->Cursor     = SaveCursor;
		btnCommit->Enabled = true;
		if (EmailAddress != "")
		btnCommitEmail->Enabled = true;
	}*/
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
        if(dmStockReportData->StockTrans->InTransaction)
        {
            dmStockReportData->StockTrans->Commit();
        }
	}
}//---------------------------------------------------------------------------
// checks whether at least one node contains quantities > 0
bool TfrmPurchaseOrder::VerifyQuantities()
{	bool canProceed = false;
	PVirtualNode Node = vtvStockQty->GetFirst();
	while (Node)
	{
		TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(Node);
		if(NodeData->SupplierUnitQty > 0)
		{
			canProceed = true;
			break;
		}
		Node = vtvStockQty->GetNext(Node);
	}
	return canProceed;
}bool TfrmPurchaseOrder::SaveOrder()
{	bool Success = true;
	// cannot save the purchase order. Halt the execution
	// this can happen if their are no items in the order or the user have not specified quanities to order
	if(!TfrmPurchaseOrder::VerifyQuantities())
	{
		Application->MessageBox("Please specify quantities for at least one item in the order",
		"Error", MB_ICONERROR | MB_OK);
		Success = false;
		return Success;
	}
	MinSupplierkey=0;
	PVirtualNode Node = vtvStockQty->GetFirst();
	while (Node)
	{
		TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(Node);
		MinSupplierkey=  NodeData->SupplierKey;
		ContactKey=       NodeData->SupplierKey;
		break;
	}
	int NewOrderKey = 0;
	try
	{
		if (OrderKey != 0)
		{
			// Check that the order exists. It may have been rolled back.
			qrPurchaseOrder->Close();
			qrPurchaseOrder->ParamByName("Order_Key")->AsInteger = OrderKey;
			qrPurchaseOrder->Open();
			if (qrPurchaseOrder->Eof)
			{
				OrderKey = 0;
			}
			qrPurchaseOrder->Close();
		}
		if (OrderKey != 0)
		{
			NewOrderKey = OrderKey;
			qrClearOrderStock->Close();
			qrClearOrderStock->ParamByName("Order_Key")->AsInteger	= OrderKey;
			qrClearOrderStock->ExecSQL();
			qrUpdateOrder->Close();
			qrUpdateOrder->ParamByName("Order_LK")->AsInteger			= OrderKey;
			qrUpdateOrder->ParamByName("Order_Number")->AsString		= edOrderNumber->Text;
			qrUpdateOrder->ParamByName("Supplier_Address")->AsString	= memSupplierAddress->Text;
			qrUpdateOrder->ParamByName("Supplier_FAX")->AsString		= edFAX->Text;
			qrUpdateOrder->ParamByName("Delivery_Address")->AsString	= memDeliveryAddress->Text;
			qrUpdateOrder->ParamByName("Instructions")->AsString		= memInstructions->Text;
			qrUpdateOrder->ParamByName("Required")->AsDateTime			= DateRequired;
			try
			{
				qrUpdateOrder->ExecSQL();
			}
			catch (EIBError &E)
			{
				Application->MessageBox(AnsiString("You must supply a unique order number, " +
				E.Message).c_str(), "Error", MB_ICONERROR + MB_OK);
				Success = false;
			}
		}
		else
		{
			qrOrderKey->Close();
			qrOrderKey->ExecQuery();
			NewOrderKey = qrOrderKey->Fields[0]->AsInteger;
			qrOrderKey->Close();
			qrAddPurchaseOrder->Close();
			qrAddPurchaseOrder->ParamByName("Order_LK")->AsInteger			= NewOrderKey;
			qrAddPurchaseOrder->ParamByName("Order_Number")->AsString		= edOrderNumber->Text;
			qrAddPurchaseOrder->ParamByName("Contact_FK")->AsInteger			= ContactKey;
			qrAddPurchaseOrder->ParamByName("Supplier_Address")->AsString	= memSupplierAddress->Text;
			qrAddPurchaseOrder->ParamByName("Supplier_FAX")->AsString		= edFAX->Text;
			qrAddPurchaseOrder->ParamByName("Delivery_Address")->AsString	= memDeliveryAddress->Text;
			qrAddPurchaseOrder->ParamByName("Instructions")->AsString		= memInstructions->Text;
			qrAddPurchaseOrder->ParamByName("Creator_Name")->AsString		= frmLogin->CurrentUser.UserName;
			qrAddPurchaseOrder->ParamByName("Creator_ID")->AsString			= frmLogin->CurrentUser.UserID;
			qrAddPurchaseOrder->ParamByName("Required")->AsDateTime			= DateRequired;
			try
			{
				qrAddPurchaseOrder->ExecSQL();
			}
			catch (EIBError &E)
			{
				Application->MessageBox(AnsiString("You must supply a unique order number, " +
				E.Message).c_str(), "Error", MB_ICONERROR + MB_OK);
				Success = false;
			}
		}
		if (Success)
		{
			PVirtualNode Node = vtvStockQty->GetFirst();
			while (Node)
			{
				TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(Node);
				qrOrderStockKey->Close();
				qrOrderStockKey->ExecQuery();
				if(   MinSupplierkey==  NodeData->SupplierKey)
				{
					qrAddOrderStock->Close();
					qrAddOrderStock->ParamByName("Purchase_Stock_LK")->AsInteger	= qrOrderStockKey->Fields[0]->AsInteger;
					qrAddOrderStock->ParamByName("Order_FK")->AsInteger				= NewOrderKey;
					qrAddOrderStock->ParamByName("Code")->AsString						= NodeData->Code;
					qrAddOrderStock->ParamByName("Location")->AsString					= NodeData->Location;
					//				qrAddOrderStock->ParamByName("Stocktake_Unit")->AsString			= NodeData->StocktakeUnit;
					//				qrAddOrderStock->ParamByName("Stock_Qty")->AsFloat					= NodeData->SupplierUnitQty * NodeData->SupplierUnitSize;
					qrAddOrderStock->ParamByName("Supplier_Code")->AsString			= NodeData->SupplierCode;
					qrAddOrderStock->ParamByName("Supplier_Unit")->AsString			= NodeData->SupplierUnit;
					qrAddOrderStock->ParamByName("Supplier_Unit_Qty")->AsFloat		= NodeData->SupplierUnitQty;
					qrAddOrderStock->ParamByName("Supplier_Unit_Cost")->AsFloat		= NodeData->SupplierUnitCost;
					qrAddOrderStock->ParamByName("Supplier_Unit_Size")->AsFloat		= NodeData->SupplierUnitSize;
					ContactKey	= NodeData->SupplierKey;
					//				qrAddOrderStock->ParamByName("Total_Cost")->AsFloat				= NodeData->SupplierUnitCost * NodeData->SupplierUnitQty;
					// we only insert the item to database if the quantity > 0
					if(NodeData->SupplierUnitQty > 0)
					qrAddOrderStock->ExecSQL();
					qrOrderStockKey->Close();
				}
				Node = vtvStockQty->GetNext(Node);
			}
			
			Node = vtvStockQty->GetFirst();
			while (Node)
			{
				TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(Node);
				qrOrderStockKey->Close();
				qrOrderStockKey->ExecQuery();
				if(   MinSupplierkey== NodeData->SupplierKey)
				{
					PVirtualNode NodeToDelete = Node;
					Node = vtvStockQty->GetNext(Node);
					vtvStockQty->DeleteNode(NodeToDelete);
				}
				else
				{
					Node = vtvStockQty->GetNext(Node);
				}
			}
		}
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		Success = false;
	}
	if (Success)
	{
		OrderKey = NewOrderKey;
	}
	return Success;
}//---------------------------------------------------------------------------
bool TfrmPurchaseOrder::CommitOrder()
{	qrPostOrder->Close();
	qrPostOrder->ParamByName("Order_LK")->AsInteger			= OrderKey;
	qrPostOrder->ExecSQL();
	qrPostOrderItems->Close();
	qrPostOrderItems->ParamByName("Order_FK")->AsInteger	= OrderKey;
	qrPostOrderItems->ExecSQL();
	return true;
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::btnRequiredDateClick(TObject *Sender)
{	if (double(DateRequired) != 0.0)
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
	if(btnAdd->Enabled)
	btnAdd->SetFocus();
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::FormCloseQuery(TObject *Sender,
bool &CanClose)
{	if (Transaction->InTransaction)
	{
		CanClose = (Application->MessageBox("Are you sure you want to cancel?", "Cancel?", MB_ICONQUESTION + MB_YESNO) == ID_YES);
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::FormClose(TObject *Sender,
TCloseAction &Action)
{	if (Transaction->InTransaction) Transaction->Rollback();
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	vtvStockQty->Clear();
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::btnAutoClick(TObject *Sender)
{	AnsiString DefaultLocation = dmStockData->DefaultLocation;
	AnsiString LocationCondition = "";
	if(DefaultLocation != "")
	LocationCondition = " AND StockLocation.Location = '" + DefaultLocation + "' ";
	//if(DefaultLocation!="")
   //	{
		if (!TransactionMM->InTransaction)
		TransactionMM->StartTransaction();
		qrLocationSetting->Close();
		qrLocationSetting->ParamByName("NAME")->AsString = DefaultLocation;
		qrLocationSetting->Open();
		int categoryFilterKey = qrLocationSetting->FieldByName("CATEGORY_TYPE")->AsFloat;
		bool CategoryCheckEnable=false;
		if(categoryFilterKey==1||categoryFilterKey==3||categoryFilterKey==5||categoryFilterKey==6)
		{
			CategoryCheckEnable=true;
		}
		if(CategoryCheckEnable)
		{
            PopulateStockWithCategory(DefaultLocation);
		}
		else
		{
            PopulateStockWithOutCategory(DefaultLocation);}
	    }
  //	}
double TfrmPurchaseOrder::GetQuantity(double OnHandQty,double OnOrdered, double MinLevel, double MaxLevel,double QtyUnit)
{	int finalval=( MaxLevel/QtyUnit ) -((OnHandQty/QtyUnit)+(OnOrdered/QtyUnit));
	if((OnHandQty+OnOrdered)<=MinLevel)
	{
		if(finalval<0)
		finalval=0;
	}
	else {finalval = 0;}
	return finalval;
}double TfrmPurchaseOrder::GetOrderQuantity(double OnHandQty,double MinQtyOrdered, double MinLevel, double MaxLevel)
{	if(OnHandQty <= MinLevel)
	{
		return GetActualOrderQuantity(MaxLevel - OnHandQty,MinQtyOrdered);
	}
	return 0;
}double TfrmPurchaseOrder::GetActualOrderQuantity(double QtyRequired,double MinQtyOrdered)
{	int MultiPlier = (int)QtyRequired/MinQtyOrdered;
	if(QtyRequired - MultiPlier*MinQtyOrdered > 0)
	{
		return (MultiPlier+1)*MinQtyOrdered;
	}
	else
	{
		return QtyRequired;
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::btnSelectDeliveryAddClick(TObject *Sender)
{	/*	frmSelectSupplier->Reset = true;
	if (frmSelectSupplier->ShowModal() == mrOk)
	{
		qrDeliveryAddress->Close();
		qrDeliveryAddress->ParamByName("CONTACT_LK")->AsInteger = frmSelectSupplier->ContactKey;
		qrDeliveryAddress->Open();
		if (!qrDeliveryAddress->IsEmpty())
		{
//		dmStockData->dtContact->Locate("CONTACT_LK", frmSelectSupplier->ContactKey, TLocateOptions());
			dtPurchaseOrder->FieldByName("DELIVERY_ADDRESS")->AsString	= qrDeliveryAddress->FieldByName("LOCATION_ADDRESS")->AsString;
			dtPurchaseOrder->FieldByName("DELIVERY_PHONE")->AsString		= qrDeliveryAddress->FieldByName("PHONE")->AsString;
		}
		qrDeliveryAddress->Close();
		DBMemo3->SetFocus();
	} */
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::btnNextOrderNumberClick(TObject *Sender)
{	if (edOrderNumber->Text != "")
	{
		if (Application->MessageBox("This will overwrite the current order number and\r"
					"increment the next order number for this user.\r"
					"Do you wish to continue?", "Overwrite?", MB_ICONQUESTION + MB_OKCANCEL) != ID_OK)
		{
			return;
		}
	}
	if (!Transaction->InTransaction)
	Transaction->StartTransaction();
	dtUser->ParamByName("User_ID")->AsString = frmLogin->CurrentUser.UserID;
	dtUser->Open();
	// No Order Number's of '0' please.
	if (dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsInteger == 0)
	{
		dtUser->Edit();
		dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsInteger = 1;
		dtUser->Post();
	}
	// See if it is available, otherwise increment.
	while (OrderNumberExists(dtUser->FieldByName("ORDER_PREFIX")->AsString +
	dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsString +
	dtUser->FieldByName("ORDER_SUFFIX")->AsString))
	{
		dtUser->Edit();
		dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsInteger = dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsInteger + 1;
		dtUser->Post();
	}
	// Found a spare one. May still not be unique if another user is entering an invoice at the same time with same number.
	edOrderNumber->Text = dtUser->FieldByName("ORDER_PREFIX")->AsString +
	dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsString +
	dtUser->FieldByName("ORDER_SUFFIX")->AsString;
	dtUser->Edit();
	dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsInteger = dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsInteger + 1;
	dtUser->Post();
	/*
	int NextOrderNumber = 0;
	if (OrderNumberExists(dtUser->FieldByName("ORDER_PREFIX")->AsString +
								dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsString +
								dtUser->FieldByName("ORDER_SUFFIX")->AsString))
	{
		AnsiString MaxOrderNumber = "";
		bool Success = GetMaxOrderNumber(dtUser->FieldByName("ORDER_PREFIX")->AsString,
													dtUser->FieldByName("ORDER_SUFFIX")->AsString,MaxOrderNumber,NextOrderNumber);
		if (MaxOrderNumber != "" && Success ==  true)
		{
			if (edOrderNumber->Text == "")
			{
				edOrderNumber->Text = MaxOrderNumber;
			}
			else if (Application->MessageBox(("This Order Number has already been used.\r"
														"The next order number for this user is '" + MaxOrderNumber + "'.\r"
														"Do you wish to use this one?").c_str(), "Overwrite?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
			{
				edOrderNumber->Text = MaxOrderNumber;
			}
		}
	}
	else
	{
		// normal operation
		edOrderNumber->Text = dtUser->FieldByName("ORDER_PREFIX")->AsString +
									dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsString +
									dtUser->FieldByName("ORDER_SUFFIX")->AsString;
		NextOrderNumber = dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsInteger + 1;
	}
	//int NextOrderNumber = dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsInteger + 1;
	dtUser->Edit();
	dtUser->FieldByName("NEXT_ORDER_NUMBER")->AsInteger = NextOrderNumber;
	dtUser->Post(); */
	//	if (Transaction->InTransaction) Transaction->Commit();
	btnRequiredDate->SetFocus();
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::btnAddClick(TObject *Sender)
{	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	frmSelectStockItem->Reset = true;
	if (frmSelectStockItem->ShowModal() == mrOk)
	{
		RecieveStockItem(frmSelectStockItem->StockCode);
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::BitBtn1Click(TObject *Sender)
{	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	std::auto_ptr<TfrmSelectSupplierStockItem> frmSelectSupplierStockItem(new TfrmSelectSupplierStockItem(NULL));
	frmSelectSupplierStockItem->CompanyKey = ContactKey;
	if (frmSelectSupplierStockItem->ShowModal() == mrOk)
	{
		RecieveStockItem(frmSelectSupplierStockItem->StockCode);
	}
}//---------------------------------------------------------------------------
void TfrmPurchaseOrder::RecieveStockItem(AnsiString StockCode)
{	frmReceiveStockItem->StockCode				= StockCode;
	frmReceiveStockItem->SupplierKey				= ContactKey;
	if (frmReceiveStockItem->Execute() == mrOk)
	{
		PVirtualNode Node = vtvStockQty->GetFirst();
		while (Node)
		{
			TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(Node);
			if (NodeData->Text == frmReceiveStockItem->Description &&
					NodeData->Location == frmReceiveStockItem->Location &&
					NodeData->SupplierUnit == frmReceiveStockItem->SupplierUnit)
			{
				vtvStockQty->FocusedNode		= Node;
				vtvStockQty->FocusedColumn		= 4;
				vtvStockQty->Selected[Node]	= true;
				Application->MessageBox("You have already added this stock item.", "Error", MB_ICONERROR + MB_OK);
				return;
			}
			Node = vtvStockQty->GetNext(Node);
		}
		PVirtualNode OrderItemNode					= NULL;
		TOrderItemNodeData *NodeData				= NULL;
		OrderItemNode									= vtvStockQty->AddChild(NULL);
		NodeData											= (TOrderItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
		NodeData->Text									= frmReceiveStockItem->Description;
		NodeData->Code									= frmReceiveStockItem->StockCode;
		NodeData->GLCode								= frmReceiveStockItem->GLCode;
		NodeData->Location							= frmReceiveStockItem->Location;
		NodeData->GSTPercent							= frmReceiveStockItem->GSTPercent;
		NodeData->StocktakeUnit						= frmReceiveStockItem->StocktakeUnit;
		NodeData->SupplierKey               =  frmReceiveStockItem->SupplierKey  ;
		NodeData->SupplierCode						= frmReceiveStockItem->SupplierCode;
		NodeData->SupplierUnit						= frmReceiveStockItem->SupplierUnit;
		NodeData->SupplierUnitCost					= frmReceiveStockItem->SupplierUnitCost;
		NodeData->SupplierUnitSize					= frmReceiveStockItem->SupplierUnitSize;
		NodeData->SupplierUnitQty					= frmReceiveStockItem->SupplierUnitQty;
		vtvStockQty->FocusedNode					= OrderItemNode;
		vtvStockQty->FocusedColumn					= 4;
		vtvStockQty->Selected[OrderItemNode]	= true;
	}
	ShowTotals();
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}//---------------------------------------------------------------------------
void TfrmPurchaseOrder::ShowTotals()
{	double Total = 0, GST = 0;
	PVirtualNode Node = vtvStockQty->GetFirst();
	while (Node)
	{
		TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(Node);
		Total	+= double(NodeData->SupplierUnitCost * NodeData->SupplierUnitQty);
		GST	+= double(NodeData->SupplierUnitCost * NodeData->SupplierUnitQty * NodeData->GSTPercent) / 100;
		Node = vtvStockQty->GetNext(Node);
	}
	lbeTotalExc->Caption	= MMMath::CurrencyString(Total,CurrentConnection.SettingDecimalPlaces);
	lbeTotalInc->Caption	= MMMath::CurrencyString(Total + GST,CurrentConnection.SettingDecimalPlaces);
	lbeGST->Caption		= MMMath::CurrencyString(GST,CurrentConnection.SettingDecimalPlaces);
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::vtvStockQtyGetText(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
TVSTTextType TextType, WideString &CellText)
{	TOrderItemNodeData *NodeData = (TOrderItemNodeData *)Sender->GetNodeData(Node);
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
		case 3:	CellText = NodeData->SupplierUnit;
			break;
		case 4:
         CellText = NodeData->SupplierUnitQty;
			break;
		case 5:
            CellText = NodeData->SupplierUnitCost;
			break;
		case 6:
            CellText = RoundTo(NodeData->SupplierUnitCost * NodeData->SupplierUnitQty, CurrentConnection.SettingDecimalPlaces);
			break;
		}
	}
	else
	{
		CellText = "";
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::vtvStockQtyAfterPaint(
TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::vtvStockQtyBeforePaint(
TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::vtvStockQtyCreateEditor(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
IVTEditLink *EditLink)
{	if (Node && Column == 4)
	{
		TOrderItemNodeData *NodeData = (TOrderItemNodeData *)Sender->GetNodeData(Node);
		neStockQty->Value = NodeData->SupplierUnitQty;
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neStockQty);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(neStockQty->Handle, EM_SETSEL, 0, -1);
	}
	else if (Node && Column == 5)
	{
		TOrderItemNodeData *NodeData = (TOrderItemNodeData *)Sender->GetNodeData(Node);
		neCost->Value = NodeData->SupplierUnitCost;
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neCost);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(neCost->Handle, EM_SETSEL, 0, -1);
	}
	else if (Node && Column == 6)
	{
		TOrderItemNodeData *NodeData = (TOrderItemNodeData *)Sender->GetNodeData(Node);
		neTotalCost->Value = NodeData->SupplierUnitCost * NodeData->SupplierUnitQty;
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neTotalCost);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(neTotalCost->Handle, EM_SETSEL, 0, -1);
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::vtvStockQtyEdited(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{	if (vtvStockQty->FocusedNode)
	{
		if (Column == 4)
		{
			TOrderItemNodeData *NodeData		= (TOrderItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
			NodeData->SupplierUnitQty			= neStockQty->Value;
		}
		else if (Column == 5)
		{
			TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
		   	NodeData->SupplierUnitCost = neCost->Value;
         
		}
		else if (Column == 6)
		{
			TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
			if (NodeData->SupplierUnitQty != 0)
			{
				NodeData->SupplierUnitCost = RoundTo(neTotalCost->Value / NodeData->SupplierUnitQty,-CurrentConnection.SettingDecimalPlaces);
			}
		}
		vtvStockQty->InvalidateNode(vtvStockQty->FocusedNode);
	}
	ShowTotals();
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::vtvStockQtyEditing(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
bool &Allowed)
{	Allowed = (Column == 4 || Column == 5 || Column == 6);
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::vtvStockQtyFocusChanged(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{	if (Node && !Sender->IsEditing())
	{
		if (Sender->FocusedColumn != 4 && Sender->FocusedColumn != 5 && Sender->FocusedColumn != 6)
		{
			Sender->FocusedColumn = 4;
		}
		Sender->EditNode(Node, Column);
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::vtvStockQtyFocusChanging(
TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::vtvStockQtyKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		if (Key == VK_RETURN)
		{
			vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
		}
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::neStockQtyKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{	if (Key == VK_RETURN || Key == VK_DOWN || Key == VK_UP)
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
		if (Key == VK_RETURN && (vtvStockQty->FocusedColumn == 4 || vtvStockQty->FocusedColumn == 5))
		{
			vtvStockQty->FocusedColumn++;
			vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
		}
		else if (Key == VK_DOWN || (Key == VK_RETURN && vtvStockQty->FocusedColumn == 6))
		{
			if (vtvStockQty->GetNext(vtvStockQty->FocusedNode))
			{
				if (Key == VK_RETURN)
				{
					vtvStockQty->FocusedColumn = 4;
				}
				vtvStockQty->FocusedNode = vtvStockQty->GetNext(vtvStockQty->FocusedNode);
				vtvStockQty->Selected[vtvStockQty->FocusedNode] = true;
			}
			else
			{
				btnAddClick(NULL);
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
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::btnRemoveClick(TObject *Sender)
{	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	if (vtvStockQty->FocusedNode)
	{
		TOrderItemNodeData *NodeData	= (TOrderItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
		if (Application->MessageBox(("Do you wish to remove the item \"" + NodeData->Text + "\" from the order?").c_str(),
					"Update?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
		{
			vtvStockQty->DeleteNode(vtvStockQty->FocusedNode);
			ShowTotals();
		}
	}
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::edDateRequiredKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{	if (Key != VK_DELETE && Key != 0x08)
	{
		Key = NULL;
	}
	else
	{
		edDateRequired->Text = "";
		DateRequired = 0;
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::btnCancelClick(TObject *Sender)
{	if (Application->MessageBox("Are you sure you want to cancel?", "Cancel?", MB_ICONQUESTION + MB_YESNO) == ID_YES)
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		ModalResult = mrCancel;
	}
}//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrder::neStockQtyKeyPress(TObject *Sender,
char &Key)
{	if (Key == VK_RETURN)
	{
		Key = NULL;
	}
    if (Key == '-')
	{
		Key = NULL;
	}

}//---------------------------------------------------------------------------
bool TfrmPurchaseOrder::OrderNumberExists(AnsiString OrderNumber)
{	bool Exists = false;
	sqlOrderNumber->Close();
	sqlOrderNumber->SQL->Text = "Select Order_Number from Purchaseorder where Order_Number = :OrderNumber";
	sqlOrderNumber->ParamByName("OrderNumber")->AsString = OrderNumber;
	sqlOrderNumber->ExecQuery();
	if ( !sqlOrderNumber->Eof )
	{
		Exists = true;
	}
	sqlOrderNumber->Close();
	return Exists;
}bool
TfrmPurchaseOrder::validate_order()
{	union {
		TOrderItemNodeData *odata;
		void *raw;
	} un;
	int state = 0;
	int terminate = 0;
	TVirtualNode *walker;
	vtvStockQty->EndEditNode();
	if (!(walker = vtvStockQty->GetFirst())) {
		Application->MessageBox("There're no stock items to order!",
		"Error", MB_ICONERROR | MB_OK);
		return false;
	} else if (!edDateRequired->Text.Length()) {
		Application->MessageBox("Please set an arrival date.",
		"Error", MB_ICONERROR | MB_OK);
		btnRequiredDate->SetFocus();
		return false;
	} else if (!edOrderNumber->Text.Length()) {
		Application->MessageBox("Please enter an order number.",
		"Error", MB_ICONERROR | MB_OK);
		edOrderNumber->SetFocus();
		return false;
	}
	while (walker) {
		un.raw = vtvStockQty->GetNodeData(walker);
		if (!un.odata->SupplierUnitQty) {
			Application->MessageBox(
			"You have not entered a quantity for this item.",
			"Error", MB_ICONERROR | MB_OK);
			vtvStockQty->FocusedColumn    = 4;
			vtvStockQty->FocusedNode      = walker;
			vtvStockQty->Selected[walker] = true;
			vtvStockQty->EditNode(vtvStockQty->FocusedNode,
			vtvStockQty->FocusedColumn);
			return false;
		}
		walker = vtvStockQty->GetNext(walker);
	}
	return true;
}//---------------------------------------------------------------------------
vector<std::string>  TfrmPurchaseOrder::split_crlf_delimited_string(const AnsiString &source)
{	vector<string> split_string;
	int  j = source.Length();
	if (j == 0)
	split_string.push_back("n/a");
	else {
		char *strbuf  = reinterpret_cast<char *>(calloc(1, j + 1));
		memcpy(strbuf, source.c_str(), j);
		for (char *strpos = strtok(strbuf, "\r\n"); strpos;
		strpos = strtok(0x0, "\r\n"))
		split_string.push_back(string(strpos));
		free(strbuf);
	}
	return split_string;
}

void __fastcall TfrmPurchaseOrder::btnCommitAndSubmitWeborderClick(TObject *Sender)
{	union {
		TOrderItemNodeData *od;
		void *raw;
	} u;
	char buf[2048];
	AnsiString host;
	AnsiString port;
	auto_ptr<TfrmProcessingWeborder> pweborder(NULL);
	const char *strbeg = weborder_uri.c_str();
	const char *strpos;
	TVirtualNode *vn = vtvStockQty->GetFirst();
	auto_ptr<web_order> weborder(NULL);
	if (!memDeliveryAddress->Text.Length()) {
		Application->MessageBox("For web-orders, you must specify a delivery "
		"address.",
		"Missing information",
		MB_ICONERROR | MB_OK);
		return;
	} else if (!weborder_uri.Length() || !validate_order()
			|| !is_okay_to_commit())
	return;
	strpos = strchr(strbeg, ':');
	host = AnsiString(strbeg, strpos - strbeg);
	port = AnsiString(&strpos[1], weborder_uri.Length() - host.Length() - 1);
	weborder.reset(
	new web_order(
	DateRequired.FormatString("yyyy-mm-dd hh:nn:ss").c_str(),
	split_crlf_delimited_string(
	memDeliveryAddress->Lines->Text),
	split_crlf_delimited_string(
	memInstructions->Lines->Text),
	CurrentConnection.CompanyName.c_str(),
	edOrderNumber->Text.c_str(),
	!CurrentConnection.DontShowItemCostInPurchaseOrder));
	for ( ; vn; vn = vtvStockQty->GetNext(vn)) {
		u.raw = vtvStockQty->GetNodeData(vn);
		wcstombs(buf, u.od->Text.c_bstr(), 2048);
		weborder->add_item(buf,
		u.od->SupplierUnit.c_str(),
		u.od->Code.c_str(),
		u.od->SupplierUnitCost,
		u.od->GSTPercent,
		u.od->SupplierUnitQty);
	}
	pweborder.reset(new TfrmProcessingWeborder(this, host, port, weborder));
	if (pweborder->ShowModal() == 2) {
		DoCommit(EmailAddress != "");
		ModalResult = mrOk;
	} else ModalResult = mrCancel;
}
bool
TfrmPurchaseOrder::is_okay_to_commit()
{	return Application->MessageBox("You will not be able to make any more "
	"changes to this order.\nDo you want to "
	"continue?",
	"Commit?",
	MB_ICONQUESTION | MB_YESNO) == IDYES;
}//---------------------------------------------------------------------------
void
TfrmPurchaseOrder::DoCommit(bool emailp)
{
    TModalResult mr = mrOk;
	if(vtvStockQty->IsEditing())
    {
	    vtvStockQty->EndEditNode();
    }

    if (!Transaction->InTransaction)
        Transaction->StartTransaction();


   if(!CheckNegativeValue())
   {
      Transaction->Rollback();
      return;
   }

    if (!SaveOrder()) {
        /* Why save the order if the user chooses not to commit? */
        Transaction->Rollback();
        return;
    }

   // Screen->Cursor = crHourGlass;
    CommitOrder();
    //Transaction->Commit();
    Transaction->Commit();
    dmStockReportData->StockTrans->StartTransaction();

    try {
        AnsiString efname;

        dmStockReportData->SetupPurchaseOrder(OrderKey);
        if(CheckImage() > 1)
        {
            CallPurchaseOrderRerport("repPurchaseOrder", emailp, efname);
        }
        else
        {
            CallPurchaseOrderRerport("repPurchaseOrder1", emailp, efname);
        }
    } catch (Exception &e) {
        if (Transaction->InTransaction)
            Transaction->Rollback();
        Application->ShowException(&e);
        mr = mrCancel;
    }

    btnCommit->Enabled = true;
    btnCommitEmail->Enabled = emailp;
    edOrderNumber->Text ="";
      if (!vtvStockQty->GetFirst())
	{
		ModalResult = mr;
	}
	OrderKey=0;
}

void TfrmPurchaseOrder::PopulateStockWithCategory(AnsiString Location)
{	frmStockGroup->StockMode = smSelect;
	frmStockGroup->ContactKey =ContactKey;
	AnsiString LocationCondition = "";
      if(Location != "")
	LocationCondition = " AND StockLocation.Location = '" + Location + "' ";
	if (frmStockGroup->ShowModal() == mrOk)
	{
		for(int i = 0 ; i <frmStockGroup->SelectedCategoryList->Count; i++)
		{
			qrSupplierStockAuto->Close();
			qrSupplierStockAuto->SQL->Text = "SELECT "
			"Stock.Stock_Key,Stock.Code,Stock.Description,Stock.Stocktake_Unit,Stock.GL_Code,Stock.GST_Percent,StockLocation.Location, "
			"StockLocation.DEFAULT_LOCATION,StockLocation.On_Hand,"
			"Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) As Numeric(15,4)) On_Order, "
			"StockLocation.Average_Cost,StockLocation.Min_Level,StockLocation.Max_Level,SUPPLIERSTOCK.STOCK_KEY, "
			"SUPPLIERSTOCK.SUPPLIER_CODE,SUPPLIERSTOCK.LATEST_COST,SUPPLIERSTOCK.QTY,SUPPLIERSTOCK.Min_Order_Qty,SUPPLIERSTOCK.SUPPLIER_UNIT, "
			"SUPPLIERSTOCK.DEFAULT_ORDER_UNIT,STOCKLOCATION.LOCATION  FROM "
			"SUPPLIERSTOCK "
			"left join STOCK on SUPPLIERSTOCK.STOCK_KEY = STOCK.STOCK_KEY "
			"left join StockGroup on STOCKGROUP.STOCK_GROUP_KEY=STOCK.STOCK_GROUP_KEY "
			"Left Join StockCategory On StockCategory.Stock_Category_Key = StockGroup.Stock_Category_Key "
			"left join STOCKLOCATION on STOCK.STOCK_KEY = STOCKLOCATION.STOCK_KEY "
			"Left Join PurchaseStock On PurchaseStock.Code = Stock.Code And PurchaseStock.Location = StockLocation.Location "
			"where StockCategory.Deleted='F' and STOCK.STOCK_KEY is not null and "
			"StockCategory.Stock_Category =:Stock_Category "
			" and STOCK.DELETED = 'F' "
			"and SUPPLIERSTOCK.SUPPLIER_KEY=:SUPPLIER_KEY "  + LocationCondition +
			"group by "
			"1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21  "
			;
			qrSupplierStockAuto->ParamByName("Stock_Category")->AsString =  frmStockGroup->SelectedCategoryList->Strings[i];
			qrSupplierStockAuto->ParamByName("SUPPLIER_KEY")->AsInteger =ContactKey;
			for (qrSupplierStockAuto->Open(); !qrSupplierStockAuto->Eof; qrSupplierStockAuto->Next())
			{
				qrStockAutoPopulate->Close();
				qrStockAutoPopulate->ParamByName("Stock_Key")->AsInteger = qrSupplierStockAuto->FieldByName("Stock_Key")->AsInteger;
				qrStockAutoPopulate->ParamByName("Location")->AsString = Location;
				qrStockAutoPopulate->Open();
				bool isDefaultPrefUnit=  qrStockAutoPopulate->Locate("DEFAULT_ORDER_UNIT", "T", TLocateOptions());
				if(isDefaultPrefUnit)
				{
					qrStockAutoPopulate->Last();
				}
				if(!isDefaultPrefUnit)
				{   qrStockAutoPopulate->First();}
				qrOrderedStock->Close();
				qrOrderedStock->ParamByName("Code")->AsString = qrStockAutoPopulate->FieldByName("Code")->AsString;
				qrOrderedStock->ParamByName("LOCATION")->AsString = qrStockAutoPopulate->FieldByName("LOCATION")->AsString;
				qrOrderedStock->Open();
				double Qty = qrStockAutoPopulate->FieldByName("Qty")->AsCurrency == 0 ? 1 : qrSupplierStockAuto->FieldByName("Qty")->AsCurrency;
				double OnHandQty = qrStockAutoPopulate->FieldByName("ON_HAND")->AsCurrency;
				double MaxLevel = qrStockAutoPopulate->FieldByName("MAX_LEVEL")->AsCurrency;
				double MinLevel = qrStockAutoPopulate->FieldByName("MIN_LEVEL")->AsCurrency;
				double MinQtyOrdered = qrStockAutoPopulate->FieldByName("Min_Order_Qty")->AsCurrency;
				double onOrder = qrStockAutoPopulate->FieldByName("On_Order")->AsFloat;
				double QtyRequired = GetQuantity(OnHandQty,onOrder, MinLevel, MaxLevel,Qty);
				//                           double QtyRequired = GetOrderQuantity(OnHandQty,MinQtyOrdered, MinLevel, MaxLevel);
				bool AlreadyExists = false;
				PVirtualNode Node = vtvStockQty->GetFirst();
				while (Node)
				{
					TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(Node);
					if (NodeData->Text == qrStockAutoPopulate->FieldByName("Description")->AsString &&
							NodeData->Location == qrStockAutoPopulate->FieldByName("Location")->AsString &&
							NodeData->SupplierUnit	== qrStockAutoPopulate->FieldByName("Supplier_Unit")->AsString)
					{
						AlreadyExists = true;
						break;
					}
					Node = vtvStockQty->GetNext(Node);
				}
				if (!AlreadyExists )
				{
					PVirtualNode OrderItemNode = NULL;
					TOrderItemNodeData *NodeData = NULL;
					OrderItemNode	= vtvStockQty->AddChild(NULL);
					NodeData   	= (TOrderItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
					NodeData->Text	= qrStockAutoPopulate->FieldByName("Description")->AsString;
					NodeData->Code	= qrStockAutoPopulate->FieldByName("Code")->AsString;
					NodeData->GLCode = qrStockAutoPopulate->FieldByName("GL_Code")->AsString;
					NodeData->Location = qrStockAutoPopulate->FieldByName("Location")->AsString;
					NodeData->GSTPercent = qrStockAutoPopulate->FieldByName("GST_Percent")->AsCurrency;
					NodeData->StocktakeUnit = qrStockAutoPopulate->FieldByName("Stocktake_Unit")->AsString;
					NodeData->SupplierCode = qrStockAutoPopulate->FieldByName("Supplier_Code")->AsString;
					NodeData->SupplierUnit = qrStockAutoPopulate->FieldByName("Supplier_Unit")->AsString;
					NodeData->SupplierUnitCost = qrStockAutoPopulate->FieldByName("Latest_Cost")->AsCurrency;
					NodeData->SupplierUnitSize = qrStockAutoPopulate->FieldByName("Qty")->AsCurrency;
					NodeData->SupplierUnitQty	= QtyRequired;
					NodeData->SupplierKey = ContactKey;
				}
			}
		}
	}
}

void TfrmPurchaseOrder::PopulateStockWithOutCategory(AnsiString Location)
{	AnsiString LocationCondition = "";
   if(Location != "" && CurrentConnection.SuppliersFromDefaultLocationsOnly)
	LocationCondition = " AND StockLocation.Location = '" + Location + "' ";
	qrSupplierStockAuto->Close();
	qrSupplierStockAuto->SQL->Text = "SELECT "
	"Stock.Stock_Key,Stock.Code,Stock.Description,Stock.Stocktake_Unit,Stock.GL_Code,Stock.GST_Percent,StockLocation.Location, "
	"StockLocation.DEFAULT_LOCATION,StockLocation.On_Hand,"
	"Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) As Numeric(15,4)) On_Order, "
	"StockLocation.Average_Cost,StockLocation.Min_Level,StockLocation.Max_Level,SUPPLIERSTOCK.STOCK_KEY, "
	"SUPPLIERSTOCK.SUPPLIER_CODE,SUPPLIERSTOCK.LATEST_COST,SUPPLIERSTOCK.QTY,SUPPLIERSTOCK.Min_Order_Qty,SUPPLIERSTOCK.SUPPLIER_UNIT, "
	"SUPPLIERSTOCK.DEFAULT_ORDER_UNIT,STOCKLOCATION.LOCATION  FROM "
	"SUPPLIERSTOCK "
	"left join STOCK on SUPPLIERSTOCK.STOCK_KEY = STOCK.STOCK_KEY "
	"left join StockGroup on STOCKGROUP.STOCK_GROUP_KEY=STOCK.STOCK_GROUP_KEY "
	"Left Join StockCategory On StockCategory.Stock_Category_Key = StockGroup.Stock_Category_Key "
	"left join STOCKLOCATION on STOCK.STOCK_KEY = STOCKLOCATION.STOCK_KEY "
	"Left Join PurchaseStock On PurchaseStock.Code = Stock.Code And PurchaseStock.Location = StockLocation.Location "
	"where StockCategory.Deleted='F' and STOCK.STOCK_KEY is not null "
	"and STOCK.DELETED = 'F' "
	"and SUPPLIERSTOCK.SUPPLIER_KEY=:SUPPLIER_KEY "  + LocationCondition +
	"group by "
	"1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21  " ;
		qrSupplierStockAuto->ParamByName("SUPPLIER_KEY")->AsInteger =ContactKey;
	for (qrSupplierStockAuto->Open(); !qrSupplierStockAuto->Eof; qrSupplierStockAuto->Next())
	{
		qrStockAutoPopulate->Close();
        	AnsiString LocationConditionStock = "";
   if(Location != "" && CurrentConnection.SuppliersFromDefaultLocationsOnly)
          LocationConditionStock = " AND StockLocation.Location = '" + Location + "' ";
                 qrStockAutoPopulate->SQL->Text =     	"SELECT "
	"Stock.Stock_Key,Stock.Code,Stock.Description,Stock.Stocktake_Unit,Stock.GL_Code,Stock.GST_Percent,StockLocation.Location, "
	"StockLocation.DEFAULT_LOCATION,StockLocation.On_Hand, "
	"Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) As Numeric(15,4)) On_Order, "
	"StockLocation.Average_Cost,StockLocation.Min_Level,StockLocation.Max_Level,SUPPLIERSTOCK.STOCK_KEY, "
	"SUPPLIERSTOCK.SUPPLIER_CODE,SUPPLIERSTOCK.LATEST_COST,SUPPLIERSTOCK.QTY,SUPPLIERSTOCK.Min_Order_Qty,SUPPLIERSTOCK.SUPPLIER_UNIT, "
	"SUPPLIERSTOCK.DEFAULT_ORDER_UNIT,STOCKLOCATION.LOCATION  FROM "
	"SUPPLIERSTOCK "
	"left join STOCK on SUPPLIERSTOCK.STOCK_KEY = STOCK.STOCK_KEY "
	"left join StockGroup on STOCKGROUP.STOCK_GROUP_KEY=STOCK.STOCK_GROUP_KEY "
	"Left Join StockCategory On StockCategory.Stock_Category_Key = StockGroup.Stock_Category_Key "
	"left join STOCKLOCATION on STOCK.STOCK_KEY = STOCKLOCATION.STOCK_KEY "
	"Left Join PurchaseStock On PurchaseStock.Code = Stock.Code And PurchaseStock.Location = StockLocation.Location "
	"where StockCategory.Deleted='F' and STOCK.STOCK_KEY is not null and STOCK.STOCK_KEY=:STOCK_KEY " + LocationConditionStock +

	"group by "
	"1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21  "
	"order by SUPPLIERSTOCK.DEFAULT_ORDER_UNIT "   ;


		qrStockAutoPopulate->ParamByName("Stock_Key")->AsInteger = qrSupplierStockAuto->FieldByName("Stock_Key")->AsInteger;
       	qrStockAutoPopulate->Open();
		bool isDefaultPrefUnit=  qrStockAutoPopulate->Locate("DEFAULT_ORDER_UNIT", "T", TLocateOptions());
		if(isDefaultPrefUnit)
		{
			qrStockAutoPopulate->Last();
		}
		if(!isDefaultPrefUnit)
		{   qrStockAutoPopulate->First();}
	  /*	qrOrderedStock->Close();
		qrOrderedStock->ParamByName("Code")->AsString = qrStockAutoPopulate->FieldByName("Code")->AsString;

       	qrOrderedStock->ParamByName("LOCATION")->AsString = qrStockAutoPopulate->FieldByName("LOCATION")->AsString;

		qrOrderedStock->Open();
        */
		double Qty = qrStockAutoPopulate->FieldByName("Qty")->AsCurrency == 0 ? 1 : qrSupplierStockAuto->FieldByName("Qty")->AsCurrency;
		double OnHandQty = qrStockAutoPopulate->FieldByName("ON_HAND")->AsCurrency;
		double MaxLevel = qrStockAutoPopulate->FieldByName("MAX_LEVEL")->AsCurrency;
		double MinLevel = qrStockAutoPopulate->FieldByName("MIN_LEVEL")->AsCurrency;
		double MinQtyOrdered = qrStockAutoPopulate->FieldByName("Min_Order_Qty")->AsCurrency;
		double onOrder = qrStockAutoPopulate->FieldByName("On_Order")->AsFloat;
		double QtyRequired = GetQuantity(OnHandQty,onOrder, MinLevel, MaxLevel,Qty);
		//                           double QtyRequired = GetOrderQuantity(OnHandQty,MinQtyOrdered, MinLevel, MaxLevel);
		bool AlreadyExists = false;
		PVirtualNode Node = vtvStockQty->GetFirst();
		while (Node)
		{
			TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(Node);
			if (NodeData->Text == qrStockAutoPopulate->FieldByName("Description")->AsString &&
					NodeData->Location == qrStockAutoPopulate->FieldByName("Location")->AsString &&
					NodeData->SupplierUnit	== qrStockAutoPopulate->FieldByName("Supplier_Unit")->AsString)
			{
				AlreadyExists = true;
				break;
			}
			Node = vtvStockQty->GetNext(Node);
		}
		if (!AlreadyExists )
		{
			PVirtualNode OrderItemNode = NULL;
			TOrderItemNodeData *NodeData = NULL;
			OrderItemNode	= vtvStockQty->AddChild(NULL);
			NodeData   	= (TOrderItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
			NodeData->Text	= qrStockAutoPopulate->FieldByName("Description")->AsString;
			NodeData->Code	= qrStockAutoPopulate->FieldByName("Code")->AsString;
			NodeData->GLCode = qrStockAutoPopulate->FieldByName("GL_Code")->AsString;
			NodeData->Location = qrStockAutoPopulate->FieldByName("Location")->AsString;
			NodeData->GSTPercent = qrStockAutoPopulate->FieldByName("GST_Percent")->AsCurrency;
			NodeData->StocktakeUnit = qrStockAutoPopulate->FieldByName("Stocktake_Unit")->AsString;
			NodeData->SupplierCode = qrStockAutoPopulate->FieldByName("Supplier_Code")->AsString;
			NodeData->SupplierUnit = qrStockAutoPopulate->FieldByName("Supplier_Unit")->AsString;
			NodeData->SupplierUnitCost = qrStockAutoPopulate->FieldByName("Latest_Cost")->AsCurrency;
			NodeData->SupplierUnitSize = qrStockAutoPopulate->FieldByName("Qty")->AsCurrency;
			NodeData->SupplierUnitQty	= QtyRequired;
			NodeData->SupplierKey = ContactKey;
		}
	}

}

int TfrmPurchaseOrder::CheckImage()
{
  int height = 0;
  std::auto_ptr<Graphics::TBitmap> bitmap(new Graphics::TBitmap);
  std::auto_ptr<Graphics::TBitmap> BigBitmap(new Graphics::TBitmap);
  unsigned char *ptr, *bigPtr;   // use a (byte *) for pf8bit color
  TPixelFormat pixForm, bigpixForm;
  unsigned char * ptr1;
  //Graphics::TBitmap *pBitmap = new Graphics::TBitmap();
  try
  {
     bitmap->Width = 3;
     bitmap->Height = 2;
     bitmap->PixelFormat = pf24bit;
     bitmap->LoadFromFile(CurrentConnection.MenuSavePath + "\\OrderLogo.bmp ");
     pixForm = bitmap->PixelFormat;

    bitmap->PixelFormat = pf24bit;
    BigBitmap->PixelFormat = pf24bit;
    BigBitmap->Height = bitmap->Height * 2;
    BigBitmap->Width = bitmap->Width * 2;
    height = bitmap->Height;
    bitmap->FreeImage();
    return height;
  }
  catch (Exception &E)
  {
    //ShowMessage("Could not load or display bitmap");
    height = 1;
    return height;    
  }

}

void TfrmPurchaseOrder::CallPurchaseOrderRerport(AnsiString ReportName, bool emailp, AnsiString efname)
{
     if (frmReports->rvStock->SelectReport(ReportName, false))
     {
        frmReports->rvStock->SetParam(
          "CompanyDetails", CurrentConnection.ReportHeader + "\r\n");
        frmReports->rvStock->SetParam(
          "ItemCost",
          IntToStr(!CurrentConnection.DontShowItemCostInPurchaseOrder));

        if (!emailp)
        {
            frmReports->rvStock->Execute();
        }
        else if (frmReports->SaveReportToFile(efname)) {
            AnsiString sp = CurrentConnection.MenuSavePath
                            + "\\Purchase Order*.*";
            dmUtilities->DeleteWildCardFile(sp);
            frmReports->rvStock->Execute();
            efname = dmUtilities->FindWildCardFile(sp);

            frmReports->SendEmail(efname, EmailAddress, "Purchase Order",ContactName);
            frmReports->SetToPreview();
        }
     }
 
 
}

bool TfrmPurchaseOrder::CheckNegativeValue()
{
    bool retVal = true;
	PVirtualNode Node = vtvStockQty->GetFirst();
	while (Node && retVal)
	{
		TOrderItemNodeData *NodeData = (TOrderItemNodeData *)vtvStockQty->GetNodeData(Node);
        if(NodeData->SupplierUnitCost < 0)
        {
           retVal = false;
           break;
        }
        Node = vtvStockQty->GetNext(Node);
	}
	if(!retVal)
	{
	   Application->MessageBox("You Cannot Process Negative Amount or Quantity", "Error", MB_ICONERROR);
	}
    return retVal;
}
