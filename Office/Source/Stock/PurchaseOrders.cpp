//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PurchaseOrders.h"
#include "StockData.h"
#include "Utilities.h"
#include "Connections.h"
#include "StockReportData.h"
#include "Reports.h"
#include "SetupPurchaseOrders.h"
#include "MM_PurchaseOrders.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmPurchaseOrders *frmPurchaseOrders;
//---------------------------------------------------------------------------
__fastcall TfrmPurchaseOrders::TfrmPurchaseOrders(TComponent* Owner)
	: TForm(Owner),
	frmPurchaseOrder(new TfrmPurchaseOrder(NULL))
{
	CurrentOrderKey = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::FormShow(TObject *Sender)
{
	FormResize(NULL);
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	qrSupplier->Close();
	qrSupplier->ParamByName("Contact_Key")->AsInteger = ContactKey;
	qrSupplier->Open();
	lbeTitle->Caption = qrSupplier->FieldByName("Company_Name")->AsString;
	EmailAddress = qrSupplier->FieldByName("Email")->AsString;
	if (EmailAddress == "")
	{
   	btnEmailOrder->Enabled = false;
	}
	qrSupplier->Close();

	qrOrder->Close();
	qrPurchStock->Close();
	qrOrder->ParamByName("CONTACT_FK")->AsInteger = ContactKey;
	qrOrder->Open();
	qrPurchStock->Open();

	LoadTree();
	if (!tvInvoices->Selected)
	{
		if (tvInvoices->Items->Count > 0)
		{
			if (tvInvoices->Items->Item[0]->getFirstChild())
			{
				if (tvInvoices->Items->Item[0]->getFirstChild()->getFirstChild())
				{
					tvInvoices->Selected = tvInvoices->Items->Item[0]->getFirstChild()->getFirstChild();
					tvInvoices->Items->Item[0]->Expand(false);
				}
				else
				{
					tvInvoices->Selected = tvInvoices->Items->Item[0]->getFirstChild();
					tvInvoices->Items->Item[0]->Expand(false);
				}
			}
			else
			{
				tvInvoices->Selected = tvInvoices->Items->Item[0];
				tvInvoices->Selected->Expand(false);
			}
		}
	}
	if (btnUpdate->Enabled)
	{
		tvInvoices->SetFocus();
	}
	else
	{
		btnNew->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::FormResize(TObject *Sender)
{
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmPurchaseOrders::ResizeGrids()
{
	const ColCount = 5;
	TDBGrid *Grid = dbgDetails;
	Grid->TitleFont->Size = Grid->Font->Size;
	const PercArray[ColCount] = {33, 23, 22, 22};
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
	}
}
//---------------------------------------------------------------------------
void TfrmPurchaseOrders::Execute()
{
	ShowModal();
}
//---------------------------------------------------------------------------
void TfrmPurchaseOrders::LoadTree()
{
	tvInvoices->Items->BeginUpdate();
	tvInvoices->OnChange = NULL;
	tvInvoices->Items->Clear();
	qrOrder->Close();

	TTreeNode *UncommittedNode			= tvInvoices->Items->Add(NULL, "Uncommitted");
	TTreeNode *CommittedNode			= tvInvoices->Items->Add(NULL, "Committed");
	TTreeNode *RootNode					= NULL;
	TTreeNode *ChildNode					= NULL;

	CommittedNode->Data					= (void *)NULL;
	CommittedNode->ImageIndex			= ICON_CLOSED_FOLDER_INDEX;
	CommittedNode->SelectedIndex		= ICON_OPEN_FOLDER_INDEX;

	UncommittedNode->Data				= (void *)NULL;
	UncommittedNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
	UncommittedNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;

	int LastCommittedMonth = 0, LastCommittedYear = 0;
	int LastUnommittedMonth = 0, LastUnommittedYear = 0;

	for (qrOrder->Open(); !qrOrder->Eof; qrOrder->Next())
	{
		if (qrOrder->FieldByName("Posted")->AsBoolean)
		{
			if (qrOrder->FieldByName("Order_Month")->AsInteger != LastCommittedMonth ||
				 qrOrder->FieldByName("Order_Year")->AsInteger != LastCommittedYear)
			{
				LastCommittedMonth				= qrOrder->FieldByName("Order_Month")->AsInteger;
				LastCommittedYear					= qrOrder->FieldByName("Order_Year")->AsInteger;
				TDateTime Date						= EncodeDate(LastCommittedYear, LastCommittedMonth, 1);

				RootNode								= tvInvoices->Items->AddChild(CommittedNode, FormatDateTime("mmmm, yyyy", Date));
				RootNode->ImageIndex				= ICON_CLOSED_FOLDER_INDEX;
				RootNode->SelectedIndex			= ICON_OPEN_FOLDER_INDEX;
				RootNode->Data						= (void *)NULL;
			}
		}
		else
		{
			if (qrOrder->FieldByName("Order_Month")->AsInteger != LastUnommittedMonth ||
				 qrOrder->FieldByName("Order_Year")->AsInteger != LastUnommittedYear)
			{
				LastUnommittedMonth				= qrOrder->FieldByName("Order_Month")->AsInteger;
				LastUnommittedYear				= qrOrder->FieldByName("Order_Year")->AsInteger;
				TDateTime Date						= EncodeDate(LastUnommittedYear, LastUnommittedMonth, 1);

				RootNode								= tvInvoices->Items->AddChild(UncommittedNode, FormatDateTime("mmmm, yyyy", Date));
				RootNode->ImageIndex				= ICON_CLOSED_FOLDER_INDEX;
				RootNode->SelectedIndex			= ICON_OPEN_FOLDER_INDEX;
				RootNode->Data						= (void *)NULL;
			}
		}
		ChildNode								= tvInvoices->Items->AddChild(RootNode, qrOrder->FieldByName("Order_Number")->AsString);
		ChildNode->ImageIndex				= ICON_NOTEPAD_INDEX;
		ChildNode->SelectedIndex			= ICON_NOTEPAD_INDEX;
		ChildNode->Data						= (void *)qrOrder->FieldByName("Order_LK")->AsInteger;

		if (qrOrder->FieldByName("Order_LK")->AsString == CurrentOrderKey)
		{
			tvInvoices->Selected = ChildNode;
		}
	}
	tvInvoices->Items->EndUpdate();
	tvInvoices->OnChange = tvInvoicesChange;
	tvInvoicesChange(tvInvoices, tvInvoices->Selected);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::btnUpdateClick(TObject *Sender)
{
	if (qrOrder->FieldByName("POSTED")->AsBoolean)
	{
		Application->MessageBox("This purchase order has been committed and cannot be updated.", "Error", MB_ICONQUESTION + MB_OK);
	}
	else
	{
		frmPurchaseOrder->ContactKey = ContactKey;
		frmPurchaseOrder->OrderKey = qrOrder->FieldByName("ORDER_LK")->AsInteger;

		frmPurchaseOrder->Execute();//ShowModal();
		{
			try
			{
				qrOrder->DisableControls();
				if (Transaction->InTransaction) Transaction->Commit();
				tvInvoices->Items->BeginUpdate();
				CurrentOrderKey = frmPurchaseOrder->OrderKey;
				FormShow(NULL);
			}
			__finally
			{
				tvInvoices->Items->EndUpdate();
				qrOrder->EnableControls();
				ResizeGrids();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::btnNewClick(TObject *Sender)
{
	frmPurchaseOrder->ContactKey	= ContactKey;
	frmPurchaseOrder->OrderKey		= 0;
	if (frmPurchaseOrder->Execute/*ShowModal*/() == mrOk)
	{
		try
		{
			qrOrder->DisableControls();
			if (Transaction->InTransaction) Transaction->Commit();
			CurrentOrderKey = frmPurchaseOrder->OrderKey;
			FormShow(NULL);
		}
		__finally
		{
			qrOrder->EnableControls();
			ResizeGrids();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::qrOrderAfterScroll(TDataSet *DataSet)
{
	btnUpdate->Enabled		= !qrOrder->FieldByName("POSTED")->AsBoolean;
	btnPrint->Enabled			= qrOrder->FieldByName("POSTED")->AsBoolean;
	if (EmailAddress != "")
	{
		btnEmailOrder->Enabled  = qrOrder->FieldByName("POSTED")->AsBoolean;
	}	
	btnDelete->Enabled		= true;
	btnDelete->Caption		= qrOrder->FieldByName("POSTED")->AsBoolean?"Close":"Cancel Order";
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::btnPrintClick(TObject *Sender)
{
	bool Email = ((TBitBtn *)Sender)->Tag;
	// If anything has been received, warn them.
	double QtyReceived;
//	dmStockData->dtPurchaseStock->Open();
//	for (dmStockData->dtPurchaseStock->First(); !dmStockData->dtPurchaseStock->Eof; dmStockData->dtPurchaseStock->Next())
	for (qrPurchStock->First(); !qrPurchStock->Eof; qrPurchStock->Next())
	{
		QtyReceived = qrPurchStock->FieldByName("QTY_RECEIVED")->AsFloat;
	}
//	dmStockData->dtPurchaseStock->Close();
	if (QtyReceived > 0)
	{
		if (Application->MessageBox("You have already received stock for this order. Are you sure you wish to reprint it?", "Print", MB_ICONQUESTION + MB_YESNO) != ID_YES)
		{
			return;
		}
	}
	dmStockReportData->StockTrans->StartTransaction();
	try
	{
		dmStockReportData->SetupPurchaseOrder(qrOrder->FieldByName("ORDER_LK")->AsInteger);
		if (frmReports->rvStock->SelectReport("repPurchaseOrder", false))
		{
			frmReports->rvStock->SetParam("CompanyDetails", CurrentConnection.ReportHeader + "\r\r");

            frmReports->rvStock->SetParam(
              "ItemCost",
              IntToStr(!CurrentConnection.DontShowItemCostInPurchaseOrder));

			if (Email == true)
			{
				try
				{
					AnsiString EmailFileName = "";
					bool Result = frmReports->SaveReportToFile(EmailFileName);
					if (Result)
					{
						// Purchase Order.htm is sometime written as Purchase Order.htm
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
						frmReports->SendEmail( EmailFileName, EmailAddress, "Purchase Order","");
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
			//frmReports->rvStock->Execute();
		}
	}
	__finally
	{
		dmStockReportData->StockTrans->Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::dbgPurchaseOrderKeyPress(
      TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		btnUpdateClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::tvInvoicesChange(TObject *Sender,
      TTreeNode *Node)
{
	qrPurchStock->Close();
	if (Node && Node->Level > 1 && qrOrder->Locate("Order_LK", int(Node->Data), TLocateOptions()))
	{
		lbeReference->Caption = "Order: " + qrOrder->FieldByName("Order_Number")->AsString + " - " + FormatDateTime("d mmmm yyyy", qrOrder->FieldByName("Created")->AsDateTime);

		qrPurchStock->Open();
		ResizeGrids();
	}
	else
	{
		lbeReference->Caption	= "";
		btnUpdate->Enabled		= false;
		btnPrint->Enabled			= false;
		btnEmailOrder->Enabled  = false;
		btnDelete->Enabled		= false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::tvInvoicesDblClick(TObject *Sender)
{
	if (tvInvoices->Selected && tvInvoices->Selected->Level > 1 &&
			qrOrder->Locate("Order_LK", int(tvInvoices->Selected->Data), TLocateOptions()) &&
			btnUpdate->Enabled)
	{
		btnUpdateClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::btnFindClick(TObject *Sender)
{
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::FindDialogFind(TObject *Sender)
{
	TTreeNode *CurItem;
	if (tvInvoices->Selected)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = tvInvoices->Selected->GetNext();
		}
		else
		{
			CurItem = tvInvoices->Selected->GetPrev();
		}
	}
	else
	{
		CurItem = tvInvoices->Items->GetFirstNode();
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
				tvInvoices->Selected = CurItem;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				tvInvoices->Selected = CurItem;
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
void __fastcall TfrmPurchaseOrders::dbgDetailsDrawColumnCell(
		TObject *Sender, const TRect &Rect, int DataCol, TColumn *Column,
		TGridDrawState State)
{
	TDBGrid *Grid = (TDBGrid *)Sender;
	if (!State.Contains(gdFixed))
	{
		Grid->Canvas->Brush->Color = Grid->Color;
		Grid->Canvas->Font->Color = Grid->Font->Color;
		Grid->Canvas->FillRect(Rect);
	}
	Grid->DefaultDrawColumnCell(Rect, DataCol, Column, State);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::Button2Click(TObject *Sender)
{
	frmSetupPurchaseOrders->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPurchaseOrders::btnDeleteClick(TObject *Sender)
{
	if (qrOrder->FieldByName("POSTED")->AsBoolean)
	{
		if (Application->MessageBox("Are you sure you wish to close this order?", "Close", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
		{
			try
			{
				qrOrder->DisableControls();

				Stock::TPurchaseOrdersControl PurchaseOrder(dmStockData->dbStock);
				PurchaseOrder.CloseOrder(qrOrder->FieldByName("ORDER_LK")->AsInteger);

				Transaction->Commit();
				tvInvoices->Items->BeginUpdate();
				CurrentOrderKey = 0;
				FormShow(NULL);
			}
			__finally
			{
				tvInvoices->Items->EndUpdate();
				qrOrder->EnableControls();
				ResizeGrids();
			}
		}
	}
	else if (Application->MessageBox("Are you sure you wish to delete this order?", "Delete", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
	{
		try
		{
			qrOrder->DisableControls();

//			qrDeleteOrder->Close();
//			qrDeleteOrder->ParamByName("OrderKey")->AsInteger = qrOrder->FieldByName("ORDER_LK")->AsInteger;
//			qrDeleteOrder->ExecSQL();
			Stock::TPurchaseOrdersControl PurchaseOrder(dmStockData->dbStock);
			PurchaseOrder.DeleteOrder(qrOrder->FieldByName("ORDER_LK")->AsInteger);

			Transaction->Commit();
			tvInvoices->Items->BeginUpdate();
			CurrentOrderKey = 0;
			FormShow(NULL);
		}
		__finally
		{
			tvInvoices->Items->EndUpdate();
			qrOrder->EnableControls();
			ResizeGrids();
		}
	}
}
//---------------------------------------------------------------------------

