//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReceiveOrder.h"
#include "StockData.h"
//#include "ReceiveStockItem.h"
#include "Utilities.h"
#include "MM_PurchaseOrders.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmReceiveOrder *frmReceiveOrder;
//---------------------------------------------------------------------------
__fastcall TfrmReceiveOrder::TfrmReceiveOrder(TComponent* Owner)
	: TForm(Owner),
	frmReceiveInvoice(new TfrmReceiveInvoice(NULL))
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::FormShow(TObject *Sender)
{
	FormResize(NULL);
	if (!Transaction->InTransaction) Transaction->StartTransaction();

	LoadTree();
	btnNewInvoice->SetFocus();
}
//---------------------------------------------------------------------------
void TfrmReceiveOrder::LoadTree()
{
	tvOrders->Items->BeginUpdate();
	tvOrders->OnChange = NULL;
	tvOrders->Items->Clear();
	qrOrders->Close();

	TTreeNode *SelectedNode		= NULL;
	TTreeNode *SupplierNode		= NULL;
	TTreeNode *ReferenceNode	= NULL;

	AnsiString LastSupplierName;

	for (qrOrders->Open(); !qrOrders->Eof; qrOrders->Next())
	{
		if (LastSupplierName != qrOrders->FieldByName("Company_Name")->AsString || !SupplierNode)
		{
			LastSupplierName					= qrOrders->FieldByName("Company_Name")->AsString;
			// Shows Purchase Orders that are no longer linked to a Supplier
			if (LastSupplierName == "")
			{
				SupplierNode					= tvOrders->Items->Add(NULL, "Deleted Supplier");
			}
			else
			{
				SupplierNode					= tvOrders->Items->Add(NULL, LastSupplierName);
			}
//			SupplierNode						= tvOrders->Items->Add(NULL, LastSupplierName);
			SupplierNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
			SupplierNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
			SupplierNode->Data				= (void *)NULL;
		}
		ReferenceNode							= tvOrders->Items->AddChild(SupplierNode, qrOrders->FieldByName("Order_Number")->AsString);
		ReferenceNode->ImageIndex			= ICON_NOTEPAD_INDEX;
		ReferenceNode->SelectedIndex		= ICON_NOTEPAD_INDEX;
		ReferenceNode->Data					= (void *)qrOrders->FieldByName("Order_LK")->AsInteger;

		if (qrOrders->FieldByName("Order_Number")->AsString == OrderNumber)
		{
			SelectedNode = ReferenceNode;
		}
	}
	tvOrders->Items->EndUpdate();
	if (SelectedNode)
	{
		tvOrders->Selected = SelectedNode;
	}
	tvOrders->OnChange = tvOrdersChange;
	tvOrdersChange(tvOrders, tvOrders->Selected);
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::tvOrdersChange(TObject *Sender,
		TTreeNode *Node)
{
	qrOrder->Close();
	qrOrderStock->Close();
	if (Node && Node->Level == 1)
	{
		qrOrder->ParamByName("Order_Key")->AsString = int(Node->Data);
		qrOrder->Open();
		qrOrderStock->ParamByName("Order_Key")->AsString = int(Node->Data);
		qrOrderStock->Open();
		if (qrOrderStock->IsEmpty())
		{
			lbeTitle->Caption = "Purchase Orders";
			lbeReference->Caption = "";
		}
		else
		{
			lbeTitle->Caption = qrOrder->FieldByName("Company_Name")->AsString;
            frmReceiveInvoice->SupplierName= qrOrder->FieldByName("Company_Name")->AsString;
			lbeReference->Caption = "Order Number: " + qrOrder->FieldByName("ORDER_NUMBER")->AsString + " - " + FormatDateTime("d mmmm yyyy", qrOrder->FieldByName("Created")->AsDateTime);
		}
        	frmReceiveInvoice->SupplierKey		= qrOrder->FieldByName("CONTACT_LK")->AsInteger ;
		ResizeGrids();
	}
	else
	{
		lbeTitle->Caption = "Purchase Orders";
		lbeReference->Caption = "";
		ResizeGrids();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::FormResize(TObject *Sender)
{
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmReceiveOrder::ResizeGrids()
{
	const ColCount = 7;
	TDBGrid *Grid = dbgPurchaseStock;
	Grid->TitleFont->Size = Grid->Font->Size;
	const PercArray[ColCount] = {30, 15, 14, 15, 13, 13};
	int WidthSoFar = 0;
	int PercLeft = 100;
	for (int i=0; i<Grid->Columns->Count; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray[i] / PercLeft - 1;
		if (PercArray[i] != 0)
		{
			Grid->Columns->Items[i]->Width = ColWidth;
			WidthSoFar += ColWidth + 1;
			PercLeft -= PercArray[i];
		}
		if (i == Grid->Columns->Count-1)
		{
			Grid->Columns->Items[i]->Width += 1;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::btnCloseClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	try
	{
		if (Transaction->InTransaction) Transaction->Commit();
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
	}
	FindDialog->CloseDialog();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::qrOrderStockAfterOpen(TDataSet *DataSet)
{
	DataSet->FieldByName("QTY_RECEIVED")->Alignment = taLeftJustify;

}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::btnNewInvoiceClick(TObject *Sender)
{
	if (tvOrders->Selected && tvOrders->Selected->Level == 1)
	{
		AnsiString NewInvoiceRef = "";

		NewInvoiceRef = InputBox("New Invoice", "Enter the invoice reference.", "");
		if (NewInvoiceRef != "")
		{
			if (InvoiceExists(NewInvoiceRef))
			{
				Application->MessageBox("This invoice has already been entered.",
												"Error",
												MB_OK | MB_ICONERROR);
				return;
			}
//			tvOrders->SetFocus();
			frmReceiveInvoice->OrderKey = int(tvOrders->Selected->Data);
			frmReceiveInvoice->InvoiceReference = NewInvoiceRef;
                        frmReceiveInvoice->IsPurchaseOrderMode = true;
                        frmReceiveInvoice->AllowNegativeStockQuantity = false;
                           //frmReceiveInvoice->SupplierKey=
			frmReceiveInvoice->ShowModal();
			if (Transaction->InTransaction) Transaction->Commit();
			FormShow(NULL);
		}
	}                                         
}
//---------------------------------------------------------------------------
bool TfrmReceiveOrder::InvoiceExists(AnsiString InvoiceReference)
{
	if (tvOrders->Selected && tvOrders->Selected->Level == 1)
	{
		qrInvoiceExists->Close();
		qrInvoiceExists->ParamByName("Company_Name")->AsString   = tvOrders->Selected->Parent->Text;
		qrInvoiceExists->ParamByName("Reference")->AsString      = InvoiceReference;
		qrInvoiceExists->Open();
		bool RetVal = (qrInvoiceExists->Fields->Fields[0]->AsInteger != 0);
		qrInvoiceExists->Close();
		return RetVal;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::btnFindClick(TObject *Sender)
{
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::FindDialogFind(TObject *Sender)
{
	TTreeNode *CurItem;
	if (tvOrders->Selected)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = tvOrders->Selected->GetNext();
		}
		else
		{
			CurItem = tvOrders->Selected->GetPrev();
		}
	}
	else
	{
		CurItem = tvOrders->Items->GetFirstNode();
	}
	while (CurItem)
	{
		AnsiString FindText, NodeText;
		if (FindDialog->Options.Contains(frMatchCase))
		{
			FindText = FindDialog->FindText;
			NodeText = CurItem->Text;
		}
		else                                              
		{
			FindText = FindDialog->FindText.UpperCase();
			NodeText = CurItem->Text.UpperCase();
		}
		if (FindDialog->Options.Contains(frWholeWord))
		{
			if (FindText == NodeText)
			{
				tvOrders->Selected = CurItem;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				tvOrders->Selected = CurItem;
				break;
			}
		}
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = CurItem->GetNext();
		}
		else
		{
			CurItem = CurItem->GetPrev();
		}
	}
	if (!CurItem)
	{
		Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveOrder::btnUndeliveredClick(TObject *Sender)
{
	if (Application->MessageBox("Are you sure you wish to close this order?", "Close", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
	{
		Stock::TPurchaseOrdersControl PurchaseOrder(dmStockData->dbStock);
		PurchaseOrder.CloseOrder(qrOrder->FieldByName("ORDER_LK")->AsInteger);

		Transaction->Commit();
		FormShow(NULL);
	}
}
//---------------------------------------------------------------------------

