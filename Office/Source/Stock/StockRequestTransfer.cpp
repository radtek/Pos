//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StockRequestTransfer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"

TfrmStockRequestTransfer *frmStockRequestTransfer;

//---------------------------------------------------------------------------
__fastcall TfrmStockRequestTransfer::TfrmStockRequestTransfer(TComponent* Owner)
    : TForm(Owner)
{
    Source = "";
    Destination = "";
    selectedStockRequestKeys = new TStringList;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::FormShow(TObject *Sender)
{
    vtvStockRequest->Clear();
    vtvStockRequest->NodeDataSize = sizeof(TStockRequestNodeData);
    lbeTitle->Caption = "Stock Request from: " + Source + " to: " + Destination;

    //Collect data from database and load the data into tree for selection.
    LoadData();
}

void TfrmStockRequestTransfer::LoadData()
{
    qrStockRequest->Close();
    try
    {
        if (!Transaction->InTransaction)
            Transaction->StartTransaction();

        qrStockRequest->ParamByName("Location")->AsString = Destination;

        for (qrStockRequest->Open(); !qrStockRequest->Eof; qrStockRequest->Next())
        {
            PVirtualNode OrderItemNode = NULL;
            TStockRequestNodeData *NodeData = NULL;

            OrderItemNode = vtvStockRequest->AddChild(NULL);

            NodeData = (TStockRequestNodeData *)vtvStockRequest->GetNodeData(OrderItemNode);

            NodeData->RequestNumber = qrStockRequest->FieldByName("REQUEST_NUMBER")->AsInteger;
                 NodeData->DateGenerated =qrStockRequest->FieldByName("DateGenerated")->AsDateTime;
                  NodeData->DateRequired = qrStockRequest->FieldByName("DateRequired")->AsDateTime;

           // NodeData->DateGenerated = FormatDateTime("d/m/yyyy", qrStockRequest->FieldByName("DateGenerated")->AsDateTime);
          //  NodeData->DateRequired = FormatDateTime("d/m/yyyy", qrStockRequest->FieldByName("DateRequired")->AsDateTime);
            NodeData->LocationFrom = Source;
            NodeData->LocationTo = Destination;
            NodeData->IsSelectedForTransfer = false;

            OrderItemNode->CheckState = csUncheckedNormal;
            OrderItemNode->CheckType = ctCheckBox;
           
             //NodeData->~TStockRequestNodeData();
        }
               vtvStockRequest->Clear();

        if (Transaction->InTransaction)
            Transaction->Commit();
    }
    catch (Exception &e)
    {
        Transaction->Rollback();
    }

    if (Transaction->InTransaction)
        Transaction->Commit();
}
//------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmStockRequestTransfer::vtvStockRequestAfterPaint(TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
    ((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::vtvStockRequestBeforePaint(TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
    ((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::vtvStockRequestCreateEditor(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink)
{
    //Do Nothing as of now
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::vtvStockRequestEdited(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
    //Do Nothing as of now
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::vtvStockRequestEditing(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column, bool &Allowed)
{
    //Do Nothing as of now   
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::vtvStockRequestFocusChanged(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
    //Do Nothing as of now
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::vtvStockRequestFocusChanging(TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode, TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
    //Do Nothing as of now
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::vtvStockRequestGetImageIndex(TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
    //Do Nothing as of now
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::vtvStockRequestGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType, WideString &CellText)
{
    TStockRequestNodeData *NodeData = (TStockRequestNodeData *)Sender->GetNodeData(Node);
    if (NodeData)
    {
        switch (Column)
        {
            case 0:
                CellText = NodeData->RequestNumber;
                break;
            case 1:
                CellText = NodeData->DateGenerated;
                break;
            case 2:
                CellText = NodeData->DateRequired;
                break;
            case 3:
                CellText = NodeData->LocationFrom;
                break;
            case 4:
                CellText = NodeData->LocationTo;
                break;
        }
    }

    else
    {
        CellText = "";
    }
  
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::vtvStockRequestKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    //Do Nothing as of now 
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::btnOkClick(TObject *Sender)
{
    PVirtualNode Node = vtvStockRequest->GetFirst();
    TStockRequestNodeData *NodeData = NULL;
    selectedStockRequestKeys->Clear();
    while (Node)
    {
        NodeData = (TStockRequestNodeData *)vtvStockRequest->GetNodeData(Node);

        if(NodeData->IsSelectedForTransfer)
            selectedStockRequestKeys->Add((AnsiString)NodeData->RequestNumber);

        Node = vtvStockRequest->GetNext(Node);
    }
    ModalResult = mrOk;
    delete NodeData;
    NodeData=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestTransfer::btnCancelClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

TStringList* TfrmStockRequestTransfer::GetSelectedRequestKeys()
{
    return selectedStockRequestKeys;
}



void __fastcall TfrmStockRequestTransfer::vtvStockRequestChecked(TBaseVirtualTree *Sender, PVirtualNode Node)
{
    TStockRequestNodeData *NodeData = (TStockRequestNodeData *)Sender->GetNodeData(Node);

    if(Node->CheckState == csCheckedNormal)
        NodeData->IsSelectedForTransfer = true;
    else if(Node->CheckState == csUncheckedNormal)
        NodeData->IsSelectedForTransfer = false;
}
//---------------------------------------------------------------------------

