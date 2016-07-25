//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CashupForm.h"
#include "Utilities.h"
#include "MMReportData.h"
#include "Reports.h"
#include "Connections.h"
#include "Utilities.h"
#include <DateUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "VirtualTrees"
#pragma link "NumericEdit"
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
class ScreenZedExporter : public Cashup::ISink<Cashup::Zed>
{
public:
	ScreenZedExporter(TListView* lv) : listView(lv)
	{
	}

	virtual void Start()
	{
		listView->Items->BeginUpdate();
		listView->Items->Clear();
	}

	virtual void End()
	{
		listView->Items->EndUpdate();
		listView->Width++;
		listView->Width--;
	}

	virtual void Sink(const Cashup::Zed& const zed)
   {
		TListItem *listItem = listView->Items->Add();
		listItem->Caption = zed.GetTerminalName();
		listItem->Data = (void *)zed.GetId();

		listItem->SubItems->Add(zed.GetCreated().DateTimeString());
		listItem->SubItems->Add(zed.GetCashier());
		if (zed.GetTotalVariance() != 0)
		{
			listItem->SubItems->Add(FormatCurr("$0.00", zed.GetTotalVariance()));
		}

		if (zed.HasBeenReconciled())
		{
			listItem->StateIndex = -1;
		}
		else
		{
			listItem->StateIndex = ICON_EXCLAMATION_INDEX;
		}
	}

private:

	TListView* listView;
};
//---------------------------------------------------------------------------
using namespace Cashup;
//---------------------------------------------------------------------------
__fastcall TfrmCashup::TfrmCashup() : TForm((TComponent*)NULL),
	dateRangePicker(dtpStartDate, dtpEndDate,
		cbStartHours, cbStartMinutes, cbEndHours, cbEndMinutes)
{
	dateRangePicker.OnDateTimeChanging = DateTimeChanging;
	dateRangePicker.OnDateTimeChanged = DateTimeChanged;
	PageControl1->ActivePage = tsZReports;
	dateRangePicker.SelectToday();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::DateTimeChanging(
	TDateTime OldStart, TDateTime& NewStart,
	TDateTime OldEnd, TDateTime& NewEnd)
{
	if (NewStart != OldStart)
	{
		NewEnd = IncDay(NewStart);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::DateTimeChanged(TDateTime NewStart, TDateTime NewEnd)
{
	LoadZeds();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::btnExitMouseClick(TObject *Sender)
{
	if (PageControl1->ActivePage == tsBalance)
	{
		if (Application->MessageBox("Are you sure you want to cancel?", "Cancel?", MB_ICONQUESTION | MB_YESNO) == ID_NO)
		{
			return;
		}
	}
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::btnSelectTodayMouseClick(TObject *Sender)
{
	dateRangePicker.SelectToday();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::btnSelectYesterdayMouseClick(TObject *Sender)
{
	dateRangePicker.SelectYesterday();
}
//---------------------------------------------------------------------------
void TfrmCashup::LoadZeds()
{
	ScreenZedExporter exporter(ListView1);

	zedCollection.BeginLoad(
		dateRangePicker.GetStart(),
		dateRangePicker.GetEnd(),
		exporter);
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::ListView1Change(TObject *Sender,
		TListItem *Item, TItemChange Change)
{
	btnNext->Enabled = ListView1->Selected;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::btnNextMouseClick(TObject *Sender)
{
	if (ListView1->Selected != NULL)
	{

		PageControl1->ActivePage = tsBalance;
		SelectZed((int)ListView1->Selected->Data);
	}
	else
	{
		Application->MessageBox("Please select a Z-Report first.", "Error", MB_ICONERROR | MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::btnViewReportMouseClick(TObject *Sender)
{
	PageControl1->ActivePage = tsZedReport;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::btnBackOutOfReportMouseClick(TObject *Sender)
{
	PageControl1->ActivePage = tsBalance;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::btnBackOutOfEntryMouseClick(TObject *Sender)
{
	if (Application->MessageBox("Are you sure you want to cancel?", "Cancel?", MB_ICONQUESTION | MB_YESNO) == ID_NO)
	{
		return;
	}

	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->CancelEditNode();
	}

	PageControl1->ActivePage = tsZReports;
}
//---------------------------------------------------------------------------
void TfrmCashup::SelectZed(int zedId)
{
	_zedId = zedId;
	const Zed* zed = zedCollection.GetZed(zedId);

	lbeTitle->Caption = "Cashup - Terminal: " + zed->GetTerminalName() +
		" - Time: " + zed->GetCreated().DateTimeString();

	std::set<Cashup::Payment> payments = zed->GetPayments();
	std::set<Cashup::Payment>::const_iterator i;

	Memo1->Lines->Text = zed->GetOriginalReport();

	vtvStocktake->BeginUpdate();
	vtvStocktake->Clear();
	vtvStocktake->NodeDataSize = sizeof(Cashup::Payment);

	for (i = payments.begin(); i != payments.end(); ++i)
	{
		PVirtualNode node = vtvStocktake->AddChild(NULL);

		Cashup::Payment* payment = (Cashup::Payment*)vtvStocktake->GetNodeData(node);
		*payment = *i;
	}

	vtvStocktake->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::btnSaveMouseClick(TObject *Sender)
{
	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->EndEditNode();
	}

	Zed* zed = zedCollection.GetZed(_zedId);
	std::set<Cashup::Payment> payments;

	PVirtualNode node = vtvStocktake->GetFirst();

	while (node)
	{
		Cashup::Payment* payment = (Cashup::Payment*)vtvStocktake->GetNodeData(node);

		payments.insert(*payment);

		node = vtvStocktake->GetNext(node);
	}

	zed->SavePaymentAdjustments(payments);

	PageControl1->ActivePage = tsZReports;
	LoadZeds();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::vtvStocktakeGetText(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
      WideString &CellText)
{
	Cashup::Payment* payment = (Cashup::Payment*)Sender->GetNodeData(Node);

	if (payment)
	{
		switch (Column)
		{
			case 0:	CellText = payment->GetType(); break;
			case 1:	CellText = FormatCurr("$0.00", payment->GetCalculatedValue()); break;
			case 2:
			{
				if (payment->GetCountedValue() != 0)
				{
					CellText = FormatCurr("$0.00", payment->GetCountedValue());
				}
				else
				{
					CellText = "";
				}
				break;
			}
			case 3:
			{
				if (payment->GetVariance() != 0)
				{
					CellText = FormatCurr("$0.00", payment->GetVariance());
				}
				else
				{
					CellText = "";
				}
				break;
			}
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::vtvStocktakeAfterPaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::vtvStocktakeBeforePaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::vtvStocktakeCreateEditor(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
		IVTEditLink *EditLink)
{
	Cashup::Payment* payment = (Cashup::Payment*)Sender->GetNodeData(Node);

	if (payment != NULL) {
		neStockQty->Value = payment->GetCountedValue();
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neStockQty);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::vtvStocktakeEdited(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column)
{
	Cashup::Payment* payment = (Cashup::Payment* )Sender->GetNodeData(Node);

	if ((payment != NULL) && (payment->GetCountedValue() != neStockQty->Value))
	{
		payment->SetCountedValue(neStockQty->Value);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::vtvStocktakeEditing(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column, bool &Allowed)
{
	Allowed = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::vtvStocktakeFocusChanged(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	if (Node && !Sender->IsEditing())
	{
		if (Column != 2)
		{
			Sender->FocusedColumn = 2;
		}
		else
		{
			Sender->EditNode(Node, Column);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::vtvStocktakeFocusChanging(
      TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
      TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmCashup::neStockQtyKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if (Key == VK_RETURN || Key == VK_DOWN || Key == VK_UP)
	{
		vtvStocktake->BeginUpdate();
		try
		{
			vtvStocktake->EndEditNode();
		}
		__finally
		{
			vtvStocktake->EndUpdate();
		}
		if (Key == VK_RETURN || Key == VK_DOWN)
		{
			if (vtvStocktake->GetNext(vtvStocktake->FocusedNode))
			{
				vtvStocktake->FocusedNode = vtvStocktake->GetNext(vtvStocktake->FocusedNode);
				vtvStocktake->Selected[vtvStocktake->FocusedNode] = true;
			}
			else
			{
				vtvStocktake->EditNode(vtvStocktake->FocusedNode, vtvStocktake->FocusedColumn);
			}
			if (Key == VK_RETURN)
			{
				Key = NULL;
			}
		}
		else if (Key == VK_UP)
		{
			if (vtvStocktake->GetPrevious(vtvStocktake->FocusedNode))
			{
				vtvStocktake->FocusedNode = vtvStocktake->GetPrevious(vtvStocktake->FocusedNode);
				vtvStocktake->Selected[vtvStocktake->FocusedNode] = true;
			}
			else
			{
				vtvStocktake->EditNode(vtvStocktake->FocusedNode, vtvStocktake->FocusedColumn);
			}
		}
	}
	else if (Key == VK_ESCAPE)
	{
		vtvStocktake->CancelEditNode();
		vtvStocktake->EditNode(vtvStocktake->FocusedNode, vtvStocktake->FocusedColumn);
	}
	
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::neStockQtyKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::neStockQtyKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if (Key == VK_DOWN || Key == VK_UP)
	{
		neStockQty->SelectAll();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::btnReportsMouseClick(TObject *Sender)
{
	dmMMReportData->MMTrans->StartTransaction();
	try
	{
		dmMMReportData->SetupCashupReconciliation(
			dateRangePicker.GetStart(),
			dateRangePicker.GetEnd(),
			NULL);

		if (frmReports->rvMenuMate->SelectReport("repCashupRecon", false))
		{
			AnsiString DateRange =
				"From " + dateRangePicker.GetStart().FormatString("ddddd 'at' hh:nn") +
				"\rto " + dateRangePicker.GetEnd().FormatString("ddddd 'at' hh:nn");

			frmReports->rvMenuMate->SetParam("ReportRange", DateRange);
			frmReports->rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
			frmReports->rvMenuMate->Execute();
		}
		else
		{
			Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
		}
	}
	__finally
	{
		dmMMReportData->MMTrans->Commit();
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmCashup::PageControl1Change(TObject *Sender)
{
	if (PageControl1->ActivePage == tsZReports)
	{
		lbeTitle->Caption = "Cashup";
	}
}
//---------------------------------------------------------------------------



