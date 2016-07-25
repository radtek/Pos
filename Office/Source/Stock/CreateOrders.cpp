//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <set>
#include "CreateOrders.h"
#include "StockData.h"
#include "SelectStockItem.h"
#include "SetupPurchaseOrders.h"
#include "MM_Math.h"
#include "StockGroup.h"
#include "LocationSelection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
//TfrmCreateOrders *frmCreateOrders;
//---------------------------------------------------------------------------
__fastcall TfrmCreateOrders::TfrmCreateOrders(TComponent* Owner)
: TForm(Owner),
frmPurchaseOrder(new TfrmPurchaseOrder(NULL)),
frmReceiveStockItem(new TfrmReceiveStockItem(NULL))
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::FormShow(TObject *Sender)
{
	vtvStockQty->Clear();
	vtvStockQty->NodeDataSize	= sizeof(TOrderSupplierItemNodeData);
	lbeTitle->Caption				= "Create Orders";
	if(SelectedRequestList->Count > 0)
	{
		btnAdd->Visible =false;
		//btnRemove->Visible =false;
		btnAuto->Visible =false;
		GetStockItemFromRequest();
	}
	AutoEnable=false;
}
void TfrmCreateOrders::GetStockItemFromRequest()
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	if(!Transaction->InTransaction)
	Transaction->StartTransaction();
	TSupplierStockRequestNodeData *StockRequest = NULL;
	for(int i = 0 ; i < SelectedRequestList->Count; i++)
	{
		StockRequest = (TSupplierStockRequestNodeData *)SelectedRequestList->Items[i];
		if(!StockRequest->IsProcessed)
		{
			CompressStockRequest(StockRequest);
			AddItemToOrder(StockRequest);
		}
	}
	if(Transaction->InTransaction)
	Transaction->Commit();
}
void TfrmCreateOrders::CompressStockRequest(TSupplierStockRequestNodeData *stockRequest)
{
	for(int i = 0 ; i < SelectedRequestList->Count; i++)
	{
		TSupplierStockRequestNodeData *tempStockRequest = (TSupplierStockRequestNodeData *)SelectedRequestList->Items[i];
		if(tempStockRequest->StockRequestKey != stockRequest->StockRequestKey &&
				tempStockRequest->StockCode == stockRequest->StockCode &&
				tempStockRequest->Location == stockRequest->Location &&
				tempStockRequest->OrderUnit == stockRequest->OrderUnit &&
				tempStockRequest->SupplierKey == stockRequest->SupplierKey &&
				tempStockRequest->UnitCost == stockRequest->UnitCost &&
				!tempStockRequest->IsProcessed)
		{
			stockRequest->Quantity += tempStockRequest->Quantity;
			tempStockRequest->IsProcessed = true;
		}
	}
}
void TfrmCreateOrders::AddItemToOrder(TSupplierStockRequestNodeData *stockRequest)
{
	qrGetStockInfo->Close();
	qrSupplierStock->Close();
	qrSupplier->Close();
	qrGetStockInfo->ParamByName("Code")->AsString = stockRequest->StockCode;
	qrSupplierStock->ParamByName("Stock_Key")->AsInteger =  stockRequest->StockKey;
	qrSupplierStock->ParamByName("Supplier_Key")->AsInteger =  stockRequest->SupplierKey;
	qrGetStockInfo->Open();
	qrSupplierStock->Open();
	qrSupplier->Open();
	PVirtualNode OrderItemNode = NULL;
	TOrderSupplierItemNodeData *NodeData  = NULL;
	OrderItemNode = vtvStockQty->AddChild(NULL);
	NodeData = (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
	NodeData->Code  = stockRequest->StockCode;
	NodeData->Text  = qrGetStockInfo->FieldByName("Description")->AsString;;
	NodeData->StockCategory = qrGetStockInfo->FieldByName("Stock_Category")->AsString;
	NodeData->StockGroup = qrGetStockInfo->FieldByName("Stock_Group")->AsString;
	NodeData->GLCode = qrGetStockInfo->FieldByName("GL_Code")->AsString;
	NodeData->Location = stockRequest->Location;
	NodeData->GSTPercent = qrGetStockInfo->FieldByName("GST_Percent")->AsFloat;
	NodeData->StocktakeUnit = qrGetStockInfo->FieldByName("Stocktake_Unit")->AsString;
	NodeData->SupplierWeborderTargetUri = qrSupplier->FieldByName("Weborder_target_uri")->AsString;
	NodeData->Supplier 	= stockRequest->SupplierName;
	NodeData->SupplierKey = stockRequest->SupplierKey;
	NodeData->SupplierCode = qrSupplierStock->FieldByName("Supplier_Code")->AsString;
	NodeData->SupplierUnit = stockRequest->OrderUnit;
	NodeData->SupplierUnitCost = stockRequest->UnitCost;
	NodeData->SupplierUnitSize 	= qrSupplierStock->FieldByName("Supplier_Unit_Size")->AsInteger;
	NodeData->SupplierUnitQty  = stockRequest->Quantity;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::FormCloseQuery(TObject *Sender,
bool &CanClose)
{
	if (vtvStockQty->GetFirst())
	{
		CanClose = (Application->MessageBox("Are you sure you you wish to cancel?", "Cancel?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::FormClose(TObject *Sender,
TCloseAction &Action)
{
	FindDialog->CloseDialog();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::vtvStockQtyAfterPaint(
TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::vtvStockQtyBeforePaint(
TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::vtvStockQtyCreateEditor(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
IVTEditLink *EditLink)
{
	if (Node && Column == 4)
	{
		TOrderSupplierItemNodeData *NodeData = (TOrderSupplierItemNodeData *)Sender->GetNodeData(Node);
		neStockQty->Value = NodeData->SupplierUnitQty;
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neStockQty);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(neStockQty->Handle, EM_SETSEL, 0, -1);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::vtvStockQtyEdited(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	if (vtvStockQty->FocusedNode)
	{
		if (Column == 4)
		{
			TOrderSupplierItemNodeData *NodeData	= (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
			NodeData->SupplierUnitQty					= neStockQty->Value;
		}
		vtvStockQty->InvalidateNode(vtvStockQty->FocusedNode);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::vtvStockQtyEditing(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
bool &Allowed)
{
	Allowed = (Column == 4 && SelectedRequestList->Count == 0);
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::vtvStockQtyFocusChanged(
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
void __fastcall TfrmCreateOrders::vtvStockQtyFocusChanging(
TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::vtvStockQtyGetImageIndex(
TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind,
TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
	ImageIndex = -1;
	TOrderSupplierItemNodeData *NodeData = (TOrderSupplierItemNodeData *)Sender->GetNodeData(Node);
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
void __fastcall TfrmCreateOrders::vtvStockQtyGetText(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
TVSTTextType TextType, WideString &CellText)
{
	TOrderSupplierItemNodeData *NodeData = (TOrderSupplierItemNodeData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Column)
		{
		case 0:	CellText = NodeData->Text;
			break;
		case 1:	CellText = NodeData->Location;
			break;
		case 2:	CellText = NodeData->Supplier;
			break;
		case 3:	CellText = NodeData->SupplierUnit;
			break;
		case 4:	CellText = MMMath::FloatString(NodeData->SupplierUnitQty);
			break;
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::vtvStockQtyKeyDown(TObject *Sender,
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
		//btnAddClick(btnAdd);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::btnAddClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	frmSelectStockItem->Reset = true;
	if (frmSelectStockItem->ShowModal()					== mrOk)
	{
		frmReceiveStockItem->StockCode					= frmSelectStockItem->StockCode;
		frmReceiveStockItem->SupplierKey					= 0;
		if (frmReceiveStockItem->Execute()				== mrOk)
		{
			PVirtualNode Node = vtvStockQty->GetFirst();
			while (Node)
			{
				TOrderSupplierItemNodeData *NodeData	= (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(Node);
				if (NodeData->Text							== frmReceiveStockItem->Description &&
						NodeData->Location						== frmReceiveStockItem->Location &&
						NodeData->SupplierKey					== frmReceiveStockItem->SupplierKey &&
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
			TOrderSupplierItemNodeData *NodeData		= NULL;
			OrderItemNode										= vtvStockQty->AddChild(NULL);
			NodeData = (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
			NodeData->Code = frmReceiveStockItem->StockCode;
			NodeData->Text = frmReceiveStockItem->Description;
			NodeData->StockCategory	= frmReceiveStockItem->StockCategory;
			NodeData->StockGroup = frmReceiveStockItem->StockGroup;
			NodeData->GLCode = frmReceiveStockItem->GLCode;
			NodeData->Location = frmReceiveStockItem->Location;
			NodeData->GSTPercent = frmReceiveStockItem->GSTPercent;
			NodeData->StocktakeUnit	= frmReceiveStockItem->StocktakeUnit;
			NodeData->SupplierWeborderTargetUri = frmReceiveStockItem->SupplierWeborderTargetUri;
			NodeData->Supplier = frmReceiveStockItem->Supplier;
			NodeData->SupplierKey = frmReceiveStockItem->SupplierKey;
			NodeData->SupplierCode = frmReceiveStockItem->SupplierCode;
			NodeData->SupplierUnit 	= frmReceiveStockItem->SupplierUnit;
			NodeData->SupplierUnitCost = frmReceiveStockItem->SupplierUnitCost;
			NodeData->SupplierUnitSize = frmReceiveStockItem->SupplierUnitSize;
			NodeData->SupplierUnitQty  = frmReceiveStockItem->SupplierUnitQty;
			vtvStockQty->FocusedNode						= OrderItemNode;
			vtvStockQty->FocusedColumn						= 4;
			vtvStockQty->Selected[OrderItemNode]		= true;
		}
	}
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::btnRemoveClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	if (vtvStockQty->FocusedNode)
	{
		TOrderSupplierItemNodeData *NodeData	= (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
		if (Application->MessageBox(("Do you wish to remove the item \"" + NodeData->Text + "\" from the order?").c_str(),
					"Update?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
		{
			vtvStockQty->DeleteNode(vtvStockQty->FocusedNode);
			UpdateRemovedRequestStatus(NodeData);
		}
	}
	if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
	{
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::btnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::neStockQtyKeyDown(TObject *Sender,
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
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::btnCommitClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	std::set<int> Suppliers;
	std::set<int>::iterator iSupplier;
	std::set<AnsiString> supplierWeborderTargetUris;
	PVirtualNode Node = vtvStockQty->GetFirst();
	while (Node)
	{
		TOrderSupplierItemNodeData *NodeData = (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(Node);
		if( AutoEnable)
		{
			if( NodeData->SupplierUnitQty>0)
			{
				Suppliers.insert(NodeData->SupplierKey);
				supplierWeborderTargetUris.insert(NodeData->SupplierWeborderTargetUri);
			}
			Node = vtvStockQty->GetNext(Node);
		}
		else
		{	Suppliers.insert(NodeData->SupplierKey);
			supplierWeborderTargetUris.insert(NodeData->SupplierWeborderTargetUri);
			Node = vtvStockQty->GetNext(Node);
		}
	}
	std::set<AnsiString>::iterator iwouri = supplierWeborderTargetUris.begin();
	frmPurchaseOrder->IsStockRequestMode = SelectedRequestList->Count > 0;
	frmPurchaseOrder->DateRequired = DateRequired;
	for (iSupplier=Suppliers.begin(); iSupplier!=Suppliers.end(); iSupplier++, iwouri++)
	{
		try
		{
			frmPurchaseOrder->InitialisePreDef(*iSupplier,*iwouri);
			int noOfStockItems = 0;
			PVirtualNode Node = vtvStockQty->GetFirst();
			while (Node)
			{
				TOrderSupplierItemNodeData *NodeData	= (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(Node);
				if(NodeData->SupplierUnitQty > 0)
				{
					noOfStockItems += 1;
				}
				Node = vtvStockQty->GetNext(Node);
			}
			if(noOfStockItems == 0)
			{
				Application->MessageBox("Please select atleast one stock item with a valid quantity.", "Error", MB_ICONERROR + MB_OK);
				return;
			}
			Node = vtvStockQty->GetFirst();
			while (Node)
			{
				TOrderSupplierItemNodeData *NodeData	= (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(Node);
				if (NodeData->SupplierKey == *iSupplier)
				{
					if( AutoEnable)
					{
						if( NodeData->SupplierUnitQty>0)
						{
							PopulateNodeData(NodeData);
						}
					}
					else
					{
						PopulateNodeData(NodeData);
					}
				}
				Node = vtvStockQty->GetNext(Node);
			}
			if (frmPurchaseOrder->ExecutePreDef() == mrOk)
			{
				// Remove all orders for this supplier now that they are a proper order.
				PVirtualNode Node = vtvStockQty->GetFirst();
				while (Node)
				{
					TOrderSupplierItemNodeData *NodeData = (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(Node);
					if(SelectedRequestList->Count > 0)
					UpdateRequestStatus(NodeData);
					if (NodeData->SupplierKey == *iSupplier)
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
			else
			{
			}
		}
		catch (Exception &E)
		{
		}
	}
	if (!vtvStockQty->GetFirst())
	{
		ModalResult = mrOk;
	}
}
void TfrmCreateOrders::UpdateRequestStatus(TOrderSupplierItemNodeData *NodeData)
{
	for(int i = 0; i<SelectedRequestList->Count;i++)
	{
		TSupplierStockRequestNodeData *StockRequest = (TSupplierStockRequestNodeData *)SelectedRequestList->Items[i];
		if(StockRequest->SupplierKey == NodeData->SupplierKey ||  StockRequest->IsRemoved)
		{
			qrUpdateRequestStatus->Close();
			if(!Transaction->InTransaction)
			Transaction->StartTransaction();
			qrUpdateRequestStatus->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger = StockRequest->StockRequestKey;
			qrUpdateRequestStatus->ExecSQL();
			Transaction->Commit();
		}
	}
}
void TfrmCreateOrders::UpdateRemovedRequestStatus(TOrderSupplierItemNodeData *NodeData)
{
	for(int i = 0; i<SelectedRequestList->Count;i++)
	{
		TSupplierStockRequestNodeData *StockRequest = (TSupplierStockRequestNodeData *)SelectedRequestList->Items[i];
		if(StockRequest->StockCode == NodeData->Code &&
				StockRequest->Location == NodeData->Location &&
				StockRequest->OrderUnit == NodeData->SupplierUnit &&
				StockRequest->SupplierKey == NodeData->SupplierKey)
		{
			StockRequest->IsRemoved = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::btnAutoClick(TObject *Sender)
{
	if (vtvStockQty->IsEditing())
	{
		vtvStockQty->EndEditNode();
	}
	if (vtvStockQty->GetFirst())
	{
		Application->MessageBox("You must do this with an empty order.", "Error", MB_ICONERROR + MB_OK);
		return;
	}
	AutoEnable=true;
	if (frmLocationSelection->ShowModal() == mrOk)
	{
		StockLocation= frmLocationSelection->Location;
	}
	if(StockLocation!="")
	{
		if (!TransactionMM->InTransaction)
		TransactionMM->StartTransaction();
		qrLocationSetting->Close();
		qrLocationSetting->ParamByName("NAME")->AsString = StockLocation;
		qrLocationSetting->Open();
		int categoryFilterKey = qrLocationSetting->FieldByName("CATEGORY_TYPE")->AsFloat;
		bool CategoryCheckEnable=false;
		if(categoryFilterKey==0||categoryFilterKey==3||categoryFilterKey==4||categoryFilterKey==6)
		{
			CategoryCheckEnable=true;
		}
		if(CategoryCheckEnable)
		{PopulateStockWithCategory(StockLocation);
		}
		else
		{PopulateStockWithOutCategory(StockLocation);}
	}
	if(Transaction->InTransaction)
	Transaction->Commit();
	frmLocationSelection->Location="";
	frmStockGroup->Mode="";
	frmStockGroup->Location="";
}
///////////////////////////////////
double TfrmCreateOrders::GetOrderQuantity(double OnHandQty,double MinQtyOrdered, double MinLevel, double MaxLevel)
{
	if(OnHandQty <= MinLevel)
	{
		return GetActualOrderQuantity(MaxLevel - OnHandQty,MinQtyOrdered);
	}
	return 0;
}
double TfrmCreateOrders::GetActualOrderQuantity(double QtyRequired,double MinQtyOrdered)
{
	int MultiPlier = (int)QtyRequired/MinQtyOrdered;
	if(QtyRequired - MultiPlier*MinQtyOrdered > 0)
	{
		return (MultiPlier+1)*MinQtyOrdered;
	}
	else
	{
		return QtyRequired;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateOrders::Button2Click(TObject *Sender)
{
	frmSetupPurchaseOrders->ShowModal();
}
//---------------------------------------------------------------------------
void TfrmCreateOrders::PopulateNodeData (TOrderSupplierItemNodeData *NodeData)
{	PVirtualNode OrderItemNode				= NULL;
	TOrderItemNodeData *OrderItemData	= NULL;
	OrderItemNode	= frmPurchaseOrder->vtvStockQty->AddChild(NULL);
	OrderItemData	= (TOrderItemNodeData *)frmPurchaseOrder->vtvStockQty->GetNodeData(OrderItemNode);
	OrderItemData->Text = NodeData->Text;
	OrderItemData->Code = NodeData->Code;
	OrderItemData->GLCode = NodeData->GLCode;
	OrderItemData->Location	= NodeData->Location;
	OrderItemData->GSTPercent = NodeData->GSTPercent;
	OrderItemData->StocktakeUnit = NodeData->StocktakeUnit;
	OrderItemData->SupplierCode  = NodeData->SupplierCode;
	OrderItemData->SupplierUnit  = NodeData->SupplierUnit;
	OrderItemData->SupplierUnitCost	= NodeData->SupplierUnitCost;
	OrderItemData->SupplierUnitSize	= NodeData->SupplierUnitSize;
	OrderItemData->SupplierUnitQty	= NodeData->SupplierUnitQty;
	OrderItemData->SupplierKey   =	 NodeData->SupplierKey;
	frmPurchaseOrder->ContactName = NodeData->Supplier;
}
void TfrmCreateOrders::PopulateStockWithCategory(AnsiString Location)
{
	if(StockLocation!="")
	{
		frmStockGroup->StockMode = smSelect;
		frmStockGroup->Mode ="All";
		frmStockGroup->Location =StockLocation;
		if (frmStockGroup->ShowModal() == mrOk)
		{
			if (!Transaction->InTransaction)
			Transaction->StartTransaction();
			for(int i = 0 ; i <frmStockGroup->SelectedCategoryList->Count; i++)
			{
				qrStockByLocation->Close();
				qrStockByLocation->ParamByName("Location")->AsString = StockLocation;
				qrStockByLocation->ParamByName("Stock_Category")->AsString =  frmStockGroup->SelectedCategoryList->Strings[i];
				int counter = 0;
				for (qrStockByLocation->Open(); !qrStockByLocation->Eof; qrStockByLocation->Next())
				{
					PVirtualNode OrderItemNode						= NULL;
					TOrderSupplierItemNodeData *NodeData		= NULL;
					OrderItemNode										= vtvStockQty->AddChild(NULL);
					NodeData = (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
					NodeData->Code = qrStockByLocation->FieldByName("CODE")->AsString;
					NodeData->Text = qrStockByLocation->FieldByName("DESCRIPTION")->AsString;
					NodeData->StockCategory = qrStockByLocation->FieldByName("STOCK_CATEGORY")->AsString;
					NodeData->StockGroup = qrStockByLocation->FieldByName("STOCK_GROUP")->AsString;
					NodeData->GLCode = qrStockByLocation->FieldByName("GL_CODE")->AsString;
					NodeData->Location = qrStockByLocation->FieldByName("LOCATION")->AsString;
					NodeData->GSTPercent = qrStockByLocation->FieldByName("GST_PERCENT")->AsFloat;
					NodeData->StocktakeUnit = qrStockByLocation->FieldByName("STOCKTAKE_UNIT")->AsString;
					qrStockSupplierAuto->Close();
					qrStockSupplierAuto->ParamByName("Stock_Key")->AsInteger = qrStockByLocation->FieldByName("Stock_Key")->AsInteger;
					qrStockSupplierAuto->ParamByName("LOCATION")->AsString = StockLocation;
					qrStockSupplierAuto->Open();
					bool isPrefferedSupplierSelected = qrStockSupplierAuto->Locate("Preferred_Supplier", "T", TLocateOptions());
					if(!isPrefferedSupplierSelected)
					{    if( qrStockSupplier->RecordCount>0 )
						qrStockSupplier->First();
						if( qrStockSupplierSec->RecordCount>0 )
						qrStockSupplierSec->First();
					}
					NodeData->Supplier = qrStockSupplierAuto->FieldByName("COMPANY_NAME")->AsString;
					NodeData->SupplierKey = qrStockSupplierAuto->FieldByName("SUPPLIER_KEY")->AsInteger;
					NodeData->SupplierCode = qrStockSupplierAuto->FieldByName("SUPPLIER_CODE")->AsString;
					NodeData->SupplierUnit = qrStockSupplierAuto->FieldByName("SUPPLIER_UNIT")->AsString;
					NodeData->SupplierUnitCost = qrStockSupplierAuto->FieldByName("LATEST_COST")->AsFloat;
					NodeData->SupplierUnitSize = qrStockSupplierAuto->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
					NodeData->SupplierUnitQty = 0;
					/*******************************/
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
					if(!isPrefferedSupplierSelected && qrStockSupplierSec->RecordCount>0)
					{
						NodeData->Supplier = qrStockSupplierSec->FieldByName("COMPANY_NAME")->AsString;
						NodeData->SupplierKey = qrStockSupplierSec->FieldByName("SUPPLIER_KEY")->AsInteger;
						NodeData->SupplierCode = qrStockSupplierSec->FieldByName("SUPPLIER_CODE")->AsString;
						NodeData->SupplierUnit = qrStockSupplier->FieldByName("SUPPLIER_UNIT")->AsString;
						NodeData->SupplierUnitCost = qrStockSupplier->FieldByName("LATEST_COST")->AsFloat;
						NodeData->SupplierUnitSize = qrStockSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
					}
					else if(isPrefferedSupplierSelected)
					{
						NodeData->Supplier = qrStockPrefferedSupplier->FieldByName("COMPANY_NAME")->AsString;
						NodeData->SupplierKey = qrStockPrefferedSupplier->FieldByName("SUPPLIER_KEY")->AsInteger;
						NodeData->SupplierCode = qrStockPrefferedSupplier->FieldByName("SUPPLIER_CODE")->AsString;
						NodeData->SupplierUnit = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT")->AsString;
						NodeData->SupplierUnitCost = qrStockPrefferedSupplier->FieldByName("LATEST_COST")->AsFloat;
						NodeData->SupplierUnitSize = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
					}
					/********************************/
					//******************************/
					float onhand = qrStockSupplierAuto->FieldByName("On_Hand")->AsFloat;
					float onOrder = qrStockSupplierAuto->FieldByName("On_Order")->AsFloat;
					float minLevel = qrStockSupplierAuto->FieldByName("Min_Level")->AsFloat;
					float maxLevel = qrStockSupplierAuto->FieldByName("Max_Level")->AsFloat;
					float qty=0;
					if(!isPrefferedSupplierSelected&&qrStockSupplierSec->RecordCount>0)
					{
						qty = qrStockSupplierSec->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
					}
					else if(isPrefferedSupplierSelected){
						qty = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
					}
					int finalval=( maxLevel/qty ) -((onhand/qty)+(onOrder/qty));
					if((onhand+onOrder)<=minLevel)
					{  if(finalval<0)
						finalval=0;
					}
					if(finalval<0)
					finalval=0;
					NodeData->SupplierUnitQty= finalval;
					//*******************************/
					//  NodeData->SupplierUnitQty = 0;
					if(counter == 0)
					{
						vtvStockQty->FocusedNode = OrderItemNode;
						vtvStockQty->FocusedColumn = 4;
						vtvStockQty->Selected[OrderItemNode] = true;
					}
				}
			}
		}
		vtvStockQty->Refresh();
		if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
		{
			vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
		}
	}
}
void TfrmCreateOrders::PopulateStockWithOutCategory(AnsiString Location)
{
	if(StockLocation!="")
	{
		if (!Transaction->InTransaction)
		Transaction->StartTransaction();
		qrStockByLocationCat->Close();
		qrStockByLocationCat->ParamByName("Location")->AsString = StockLocation;
		
		int counter = 0;
		for (qrStockByLocationCat->Open(); !qrStockByLocationCat->Eof; qrStockByLocationCat->Next())
		{
			PVirtualNode OrderItemNode						= NULL;
			TOrderSupplierItemNodeData *NodeData		= NULL;
			OrderItemNode										= vtvStockQty->AddChild(NULL);
			NodeData = (TOrderSupplierItemNodeData *)vtvStockQty->GetNodeData(OrderItemNode);
			NodeData->Code = qrStockByLocationCat->FieldByName("CODE")->AsString;
			NodeData->Text = qrStockByLocationCat->FieldByName("DESCRIPTION")->AsString;
			NodeData->StockCategory = qrStockByLocationCat->FieldByName("STOCK_CATEGORY")->AsString;
			NodeData->StockGroup = qrStockByLocationCat->FieldByName("STOCK_GROUP")->AsString;
			NodeData->GLCode = qrStockByLocationCat->FieldByName("GL_CODE")->AsString;
			NodeData->Location = qrStockByLocationCat->FieldByName("LOCATION")->AsString;
			NodeData->GSTPercent = qrStockByLocationCat->FieldByName("GST_PERCENT")->AsFloat;
			NodeData->StocktakeUnit = qrStockByLocationCat->FieldByName("STOCKTAKE_UNIT")->AsString;
			qrStockSupplierAuto->Close();
			qrStockSupplierAuto->ParamByName("Stock_Key")->AsInteger = qrStockByLocationCat->FieldByName("Stock_Key")->AsInteger;
			qrStockSupplierAuto->ParamByName("LOCATION")->AsString = StockLocation;
			qrStockSupplierAuto->Open();
			bool isPrefferedSupplierSelected = qrStockSupplierAuto->Locate("Preferred_Supplier", "T", TLocateOptions());
			if(!isPrefferedSupplierSelected)
			{    if( qrStockSupplier->RecordCount>0 )
				qrStockSupplier->First();
				if( qrStockSupplierSec->RecordCount>0 )
				qrStockSupplierSec->First();
			} 
			NodeData->Supplier = qrStockSupplierAuto->FieldByName("COMPANY_NAME")->AsString;
			NodeData->SupplierKey = qrStockSupplierAuto->FieldByName("SUPPLIER_KEY")->AsInteger;
			NodeData->SupplierCode = qrStockSupplierAuto->FieldByName("SUPPLIER_CODE")->AsString;
			NodeData->SupplierUnit = qrStockSupplierAuto->FieldByName("SUPPLIER_UNIT")->AsString;
			NodeData->SupplierUnitCost = qrStockSupplierAuto->FieldByName("LATEST_COST")->AsFloat;
			NodeData->SupplierUnitSize = qrStockSupplierAuto->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
			NodeData->SupplierUnitQty = 0;
			/*******************************/
			if(isPrefferedSupplierSelected)
			{
				qrStockPrefferedSupplier->Close();
				qrStockPrefferedSupplier->ParamByName("Stock_Key")->AsInteger = qrStockByLocationCat->FieldByName("Stock_Key")->AsInteger;
				qrStockPrefferedSupplier->Open();
				bool isDefaultPrefUnit=  qrStockPrefferedSupplier->Locate("DEFAULT_ORDER_UNIT", "T", TLocateOptions());
				if(isPrefferedSupplierSelected && isDefaultPrefUnit)
				{
					qrStockPrefferedSupplier->Last();
				}
				if(isPrefferedSupplierSelected && !isDefaultPrefUnit)
				{   qrStockPrefferedSupplier->First();}
			}
			if(!isPrefferedSupplierSelected && qrStockSupplierSec->RecordCount>0)
			{
				NodeData->Supplier = qrStockSupplierSec->FieldByName("COMPANY_NAME")->AsString;
				NodeData->SupplierKey = qrStockSupplierSec->FieldByName("SUPPLIER_KEY")->AsInteger;
				NodeData->SupplierCode = qrStockSupplierSec->FieldByName("SUPPLIER_CODE")->AsString;
				NodeData->SupplierUnit = qrStockSupplier->FieldByName("SUPPLIER_UNIT")->AsString;
				NodeData->SupplierUnitCost = qrStockSupplier->FieldByName("LATEST_COST")->AsFloat;
				NodeData->SupplierUnitSize = qrStockSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
			}
			else if(isPrefferedSupplierSelected)
			{
				NodeData->Supplier = qrStockPrefferedSupplier->FieldByName("COMPANY_NAME")->AsString;
				NodeData->SupplierKey = qrStockPrefferedSupplier->FieldByName("SUPPLIER_KEY")->AsInteger;
				NodeData->SupplierCode = qrStockPrefferedSupplier->FieldByName("SUPPLIER_CODE")->AsString;
				NodeData->SupplierUnit = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT")->AsString;
				NodeData->SupplierUnitCost = qrStockPrefferedSupplier->FieldByName("LATEST_COST")->AsFloat;
				NodeData->SupplierUnitSize = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
			}
			/********************************/
			//******************************/
			float onhand = qrStockSupplierAuto->FieldByName("On_Hand")->AsFloat;
			float onOrder = qrStockSupplierAuto->FieldByName("On_Order")->AsFloat;
			float minLevel = qrStockSupplierAuto->FieldByName("Min_Level")->AsFloat;
			float maxLevel = qrStockSupplierAuto->FieldByName("Max_Level")->AsFloat;
			float qty=0;
			if(!isPrefferedSupplierSelected&&qrStockSupplierSec->RecordCount>0)
			{
				qty = qrStockSupplierSec->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
			}
			else if(isPrefferedSupplierSelected){
				qty = qrStockPrefferedSupplier->FieldByName("SUPPLIER_UNIT_SIZE")->AsFloat;
			}
			int finalval=( maxLevel/qty ) -((onhand/qty)+(onOrder/qty));
			if((onhand+onOrder)<=minLevel)
			{  if(finalval<0)
				finalval=0;
			}
			if(finalval<0)
			finalval=0;
			NodeData->SupplierUnitQty= finalval;
			//*******************************/
			//  NodeData->SupplierUnitQty = 0;
			if(counter == 0)
			{
				vtvStockQty->FocusedNode = OrderItemNode;
				vtvStockQty->FocusedColumn = 4;
				vtvStockQty->Selected[OrderItemNode] = true;
			}
		}
		
		vtvStockQty->Refresh();
		if (!vtvStockQty->IsEditing() && vtvStockQty->FocusedNode)
		{
			vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
		}
	}
}
