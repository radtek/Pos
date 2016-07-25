//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectSupplierStockRequest.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
//TfrmSelectSupplierStockRequest *frmSelectSupplierStockRequest;
//---------------------------------------------------------------------------
__fastcall TfrmSelectSupplierStockRequest::TfrmSelectSupplierStockRequest(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockRequest::FormShow(TObject *Sender)
{
         SelectedRequestList= new TList;
         vtvStockRequest->Clear();
         vtvStockRequest->NodeDataSize	= sizeof(TSupplierStockRequestData);
	 lbeTitle->Caption = "Select Stock Request";
         LoadSupplierStockRequest();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSupplierStockRequest::btnCancelClick(
      TObject *Sender)
{
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSupplierStockRequest::btnNextClick(
      TObject *Sender)
{
    PVirtualNode Node = vtvStockRequest->GetFirst();
    TSupplierStockRequestData *NodeData = NULL;
    if (!Transaction->InTransaction)
       Transaction->StartTransaction();
    while (Node)
	{
             NodeData = (TSupplierStockRequestData *)vtvStockRequest->GetNodeData(Node);
             if(NodeData->IsSelected)
                GetStockRequestItems(NodeData);
	     Node = vtvStockRequest->GetNext(Node);
	}
    Transaction->Commit();
    GetStockRequestDate();
    ModalResult = mrOk;
}

void TfrmSelectSupplierStockRequest::GetStockRequestDate()
{
   PVirtualNode Node = vtvStockRequest->GetFirst();
   TSupplierStockRequestData *NodeData = NULL;
   DateRequired= 0.0;
   bool IsSameDate = true;
   while (Node)
	{
             NodeData = (TSupplierStockRequestData *)vtvStockRequest->GetNodeData(Node);
             if(NodeData->IsSelected && double(DateRequired)  != 0.0 && DateRequired != NodeData->DateRequired)
             {
               IsSameDate =false;
               break;
             }
             else if(NodeData->IsSelected && double(DateRequired)  == 0.0)
             {
               DateRequired = NodeData->DateRequired;
             }

	     Node = vtvStockRequest->GetNext(Node);
	}
        if(!IsSameDate)
        {
         DateRequired= 0.0;
        }
}


void TfrmSelectSupplierStockRequest::GetStockRequestItems(TSupplierStockRequestData *stockRequest)
{
   TSupplierStockRequestNodeData  *stockrequestItem = NULL;
   // qrGetStockRequestItems->Close();
   // qrGetStockRequestItems->ParamByName("REQUEST_NUMBER")->AsInteger = stockRequest->RequestNumber;
   int RequestNumber =stockRequest->RequestNumber;
           qrStockReqStock->Close();
     	qrStockReqStock->ParamByName("REQUEST_NUMBER")->AsInteger = RequestNumber;

  	for (qrStockReqStock->Open(); !qrStockReqStock->Eof; qrStockReqStock->Next())
 	{
    int supplierStockkey= qrStockReqStock->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger ;
    	qrStockRequestSelect->ParamByName("REQUEST_NUMBER")->AsInteger = RequestNumber;
        qrStockRequestSelect->ParamByName("SUPPLIER_STOCK_KEY")->AsInteger = supplierStockkey;
    	for (qrStockRequestSelect->Open(); !qrStockRequestSelect->Eof; qrStockRequestSelect->Next())
	{
                stockrequestItem = new  TSupplierStockRequestNodeData;
                stockrequestItem->StockRequestKey = qrStockRequestSelect->FieldByName("STOCK_REQUEST_STOCKKEY")->AsInteger;;
                stockrequestItem->StockKey = qrStockRequestSelect->FieldByName("STOCK_KEY")->AsInteger;
                stockrequestItem->SupplierKey= qrStockRequestSelect->FieldByName("SUPPLIER_KEY")->AsInteger;
                 stockrequestItem->StockCode= qrStockRequestSelect->FieldByName("CODE")->AsString;
                 stockrequestItem->SupplierName= qrStockRequestSelect->FieldByName("COMPANY_NAME")->AsString;
                 stockrequestItem->DateGenerated= qrStockRequestSelect->FieldByName("DATE_GENERATED")->AsString;
                 stockrequestItem->DateRequired= qrStockRequestSelect->FieldByName("DATE_REQUIRED")->AsString;
                 stockrequestItem->Location= qrStockRequestSelect->FieldByName("LOCATION")->AsString;
                 stockrequestItem->OrderUnit= qrStockRequestSelect->FieldByName("STOCK_REQUEST_UNIT")->AsString;
                 stockrequestItem->Quantity= qrStockRequestSelect->FieldByName("QTY_REQUIRED")->AsFloat;
                 stockrequestItem->UnitCost= qrStockRequestSelect->FieldByName("STOCK_REQUEST_UNIT_COST")->AsFloat;
                 stockrequestItem->IsProcessed = false;
                 SelectedRequestList->Add(stockrequestItem);
    }
    	qrStockRequestSelect->Close();
   }

   /*

    for (qrGetStockRequestItems->Open(); !qrGetStockRequestItems->Eof; qrGetStockRequestItems->Next())
             {
                stockrequestItem = new  TSupplierStockRequestNodeData;
                stockrequestItem->StockRequestKey = qrGetStockRequestItems->FieldByName("STOCK_REQUEST_STOCKKEY")->AsInteger;;
                stockrequestItem->StockKey = qrGetStockRequestItems->FieldByName("STOCK_KEY")->AsInteger;
                stockrequestItem->SupplierKey= qrGetStockRequestItems->FieldByName("SUPPLIER_KEY")->AsInteger;
                 stockrequestItem->StockCode= qrGetStockRequestItems->FieldByName("CODE")->AsString;
                 stockrequestItem->SupplierName= qrGetStockRequestItems->FieldByName("COMPANY_NAME")->AsString;
                 stockrequestItem->DateGenerated= qrGetStockRequestItems->FieldByName("DATE_GENERATED")->AsString;
                 stockrequestItem->DateRequired= qrGetStockRequestItems->FieldByName("DATE_REQUIRED")->AsString;
                 stockrequestItem->Location= qrGetStockRequestItems->FieldByName("LOCATION")->AsString;
                 stockrequestItem->OrderUnit= qrGetStockRequestItems->FieldByName("STOCK_REQUEST_UNIT")->AsString;
                 stockrequestItem->Quantity= qrGetStockRequestItems->FieldByName("QTY_REQUIRED")->AsFloat;
                 stockrequestItem->UnitCost= qrGetStockRequestItems->FieldByName("STOCK_REQUEST_UNIT_COST")->AsFloat;
                 stockrequestItem->IsProcessed = false;
                 SelectedRequestList->Add(stockrequestItem);
             }
             */


}




//---------------------------------------------------------------------------

void TfrmSelectSupplierStockRequest::LoadSupplierStockRequest()
{
    qrGetStockRequest->Close();
    try
       {
           if (!Transaction->InTransaction)
                Transaction->StartTransaction();
           for (qrGetStockRequest->Open(); !qrGetStockRequest->Eof; qrGetStockRequest->Next())
             {
                PVirtualNode RequestItemNode = NULL;
                TSupplierStockRequestData *NodeData = NULL;
                RequestItemNode	= vtvStockRequest->AddChild(NULL);
                NodeData = (TSupplierStockRequestData *)vtvStockRequest->GetNodeData(RequestItemNode);
                NodeData->RequestNumber = qrGetStockRequest->FieldByName("REQUEST_NUMBER")->AsInteger;
                NodeData->DateRequired = qrGetStockRequest->FieldByName("DATE_REQUIRED")->AsDateTime;
                NodeData->DateGenerated = qrGetStockRequest->FieldByName("DATE_GENERATED")->AsDateTime;
                NodeData->Location = qrGetStockRequest->FieldByName("LOCATION")->AsString;
                NodeData->IsSelected = false;
                RequestItemNode->CheckState = csUncheckedNormal;
                RequestItemNode->CheckType = ctCheckBox;
             }

        Transaction->Commit();
    }
    catch (Exception &e)
    {
        Transaction->Rollback();
    }

    if (Transaction->InTransaction)
        Transaction->Commit();




}

void __fastcall TfrmSelectSupplierStockRequest::vtvStockRequestAfterItemPaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas, PVirtualNode Node,
      TRect &ItemRect)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;        
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSupplierStockRequest::vtvStockRequestAfterPaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;        
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSupplierStockRequest::vtvStockRequestGetText(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      TVSTTextType TextType, WideString &CellText)
{
     TSupplierStockRequestData *NodeData = (TSupplierStockRequestData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Column)
		{
			case 0:	CellText = IntToStr(NodeData->RequestNumber);
						break;
			case 1:	CellText = NodeData->DateGenerated;
						break;
			case 2:	CellText = NodeData->DateRequired;
						break;
			case 3:	CellText = NodeData->Location;
						break;
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmSelectSupplierStockRequest::vtvStockRequestChecked(
      TBaseVirtualTree *Sender, PVirtualNode Node)
{
    TSupplierStockRequestData *NodeData = (TSupplierStockRequestData *)Sender->GetNodeData(Node);

    if(Node->CheckState == csCheckedNormal)
        NodeData->IsSelected = true;
    else if(Node->CheckState == csUncheckedNormal)
        NodeData->IsSelected = false;
}
//---------------------------------------------------------------------------

