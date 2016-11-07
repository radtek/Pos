
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <shlobj.h>
#include <memory>

#include "Reports.h"
#include "SendEmail.h"
#include "StockReportData.h"
#include "Connections.h"
#include "Utilities.h"                                   
#include "MMReportData.h"
#include "MMData.h"
#include "Excel.h"
#include "AuthenticationService.h"
#include "Login.h"
#include "VariableManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RPBase"
#pragma link "RPDefine"
#pragma link "RPRave"
#pragma link "RPSystem"
#pragma link "CSPIN"
#pragma link "NumericEdit"
#pragma link "RpRender"
#pragma link "RpRenderPDF"
#pragma link "RpRenderHTML"
#pragma link "RpRenderRTF"
#pragma link "RpRenderText"
#include "StocktakeVarianceParameters.h"
#pragma resource "*.dfm"
TfrmReports *frmReports;

//---------------------------------------------------------------------------
__fastcall TfrmReports::TfrmReports(TComponent* Owner)
	: TForm(Owner)
{
	TTreeNode *Node = TreeView1->Items->GetFirstNode();
	while (Node)
	{
/*		if (Node->AbsoluteIndex == 0)
		{
			Node->ImageIndex		= ICON_FAVORITES_FOLDER_INDEX;
			Node->SelectedIndex	= ICON_FAVORITES_FOLDER_INDEX;
		}
		else */if (Node->Count > 0)
		{
			Node->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
			Node->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
		}
		else
		{
			Node->ImageIndex			= ICON_NOTEPAD_INDEX;
			Node->SelectedIndex		= ICON_NOTEPAD_INDEX;
		}
		Node = Node->GetNext();
	}
	LastNode = NULL;
	ReportControl = NULL;
	CheckListBoxCtr = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::FormCreate(TObject *Sender)
{

	rvMenuMate->Open();
	rvStock->Open();

	StartTime = EncodeTime(5,0,0,0);
	EndTime = EncodeTime(5,0,0,0);
	mcStartDate->Date = int(Date()-1);
	mcEndDate->Date = int(mcStartDate->Date) + 1;

//	pcReportProperties->ActivePage = tsFolderDescription;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::FormDestroy(TObject *Sender)
{
	if (ReportControl)
	{
		delete ReportControl;
		ReportControl = NULL;
	}
	rvMenuMate->Close();
	rvStock->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::FormShow(TObject *Sender)
{
   fRunningReport = false;
	FormResize(this);

	TreeView1->Select(TreeView1->Items->Item[0]);
	TreeView1Change(TreeView1, TreeView1->Selected);

	ShowDateTimes();

//	dmReportData->qrCancels->Active = true;
//	dmReportData->qrCancelBill->Active = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::FormResize(TObject *Sender)
{
	ClientWidth				= Panel5->Width + (Panel5->Left * 2);
	ClientHeight			= Panel5->Height + (Panel5->Top * 2);
	csStartHour->Top		= mcStartDate->Top + mcStartDate->Height + csStartHour->Left;
	pnlStartTime->Top		= csStartHour->Top - 1;
	csStartMinute->Top	= csStartHour->Top;

	lbeFrom->Top			= csStartHour->Top + csStartHour->Height + csStartHour->Left;

	csStartMinute->Left	= mcStartDate->Left + mcStartDate->Width - csStartMinute->Width;
	pnlStartTime->Width	= csStartMinute->Left - (csStartHour->Left + csStartHour->Width) - ((pnlStartTime->Left - (csStartHour->Left + csStartHour->Width)) * 2);

	lbeFrom->Width			= mcStartDate->Width;

	csEndHour->Top			= csStartHour->Top;
	csEndHour->Left		= mcEndDate->Left;
	csEndMinute->Left		= csEndHour->Left + mcEndDate->Width - csEndMinute->Width;//csStartMinute->Left;
	csEndMinute->Top		= csStartMinute->Top;
	pnlEndTime->Top		= pnlStartTime->Top;
	pnlEndTime->Width		= pnlStartTime->Width;
	pnlEndTime->Width		= pnlStartTime->Width;
	lbeTo->Top				= lbeFrom->Top;
	lbeTo->Width			= lbeFrom->Width;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::tvReportsChange(TObject *Sender,
		TTreeNode *Node)
{
	bool ExpandNode = true;
	if (LastNode)
	{
		if (LastNode == Node)
		{
			ExpandNode = false;
		}
	}
	if (ExpandNode)
	{
		Node->Expand(false);
	}
}
//---------------------------------------------------------------------------
void TfrmReports::ShowDateTimes()
{
	pnlStartTime->Caption			= StartTime.FormatString("HH:nn");
	pnlEndTime->Caption				= EndTime.FormatString("HH:nn");
	lbeFrom->Caption					= "From " + mcStartDate->Date.FormatString("ddddd") + " at " + StartTime.FormatString("HH:nn");
	lbeTo->Caption						= "To " + mcEndDate->Date.FormatString("ddddd") + " at " + EndTime.FormatString("HH:nn");
	lbeCheckFilterRange->Caption	= "From " + mcStartDate->Date.FormatString("ddddd") + " at " + StartTime.FormatString("HH:nn") +
											  " to " + mcEndDate->Date.FormatString("ddddd") + " at " + EndTime.FormatString("HH:nn");
	lbeTreeFilterRange->Caption	= lbeCheckFilterRange->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::mcStartDateClick(TObject *Sender)
{
	mcStartDate->MinDate = 0;
	mcEndDate->MinDate = 0;
	if (int(mcStartDate->Date) > int(mcEndDate->Date))
	{
   	mcEndDate->Date = int(mcStartDate->Date);
	}
	mcEndDate->MinDate = int(mcStartDate->Date);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::csStartHourDownClick(TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(StartTime, Hour, Min, Sec, MSec);

	if (short(--Hour) < 0) Hour = 23;

   StartTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::csStartHourUpClick(TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
   DecodeTime(StartTime, Hour, Min, Sec, MSec);

	if (++Hour >= 24) Hour = 0;

	StartTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::csStartMinuteDownClick(TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(StartTime, Hour, Min, Sec, MSec);

	if (short(--Min) < 0)
	{
		Min = 59;
		if (short(--Hour) < 0) Hour = 23;
	}
	StartTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::csStartMinuteUpClick(TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(StartTime, Hour, Min, Sec, MSec);

	if (++Min >= 59)
	{
		Min = 0;
		if (++Hour >= 24) Hour = 0;
	}
	StartTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::csEndHourDownClick(TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(EndTime, Hour, Min, Sec, MSec);

	if (short(--Hour) < 0) Hour = 23;

	EndTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::csEndHourUpClick(TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(EndTime, Hour, Min, Sec, MSec);

	if (++Hour >= 24) Hour = 0;

	EndTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::csEndMinuteDownClick(TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(EndTime, Hour, Min, Sec, MSec);

	if (short(--Min) < 0)
	{
		Min = 59;
		if (short(--Hour) < 0) Hour = 23;
	}
	EndTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::csEndMinuteUpClick(TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(EndTime, Hour, Min, Sec, MSec);

   if (++Min >= 59)
   {
   	Min = 0;
		if (++Hour >= 24) Hour = 0;
   }
	EndTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnThisDayClick(TObject *Sender)
{
	mcEndDate->MinDate = int(mcStartDate->Date);
	mcEndDate->Date = int(mcStartDate->Date + 1);
	EndTime = StartTime;

	ShowDateTimes();
//	SetupFilterDisplay();
//	pcReportProperties->ActivePage = tsFilter;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnWeekClick(TObject *Sender)
{
	mcEndDate->MinDate = int(mcStartDate->Date);
	mcEndDate->Date = int(mcStartDate->Date + 7);
	EndTime = StartTime;

	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnThisMonthClick(TObject *Sender)
{
	unsigned short year, month, day;
	mcStartDate->Date.DecodeDate(&year, &month, &day);
	mcStartDate->Date		= int(EncodeDate(year, month, 1));
	mcEndDate->MinDate	= int(mcStartDate->Date);
	mcEndDate->Date		= int(IncMonth(mcStartDate->Date, 1));
	EndTime					= StartTime;

	ShowDateTimes();
//	SetupFilterDisplay();
//	pcReportProperties->ActivePage = tsFilter;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::tsEndTimeShow(TObject *Sender)
{
/*	if (tvReports->Selected->Parent->AbsoluteIndex == FAVORITES_INDEX)
	{
		btnFavorites->Visible = true;
		btnFavorites->Enabled = true;
		lbeTitle->Width = btnFavorites->Left - (lbeTitle->Left * 2);
	}
	else
	{
		btnFavorites->Visible = false;
		lbeTitle->Width = Panel3->ClientWidth - (lbeTitle->Left * 2);
	}
	lbeTitle->Caption = "Select end for the " + tvReports->Selected->Text + " report.";*/
}
//---------------------------------------------------------------------------
void TfrmReports::SetupFilterDisplay()
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::ListView1DblClick(TObject *Sender)
{
	if (ListView1->ItemIndex > -1)
	{
		TreeView1->Selected = TreeView1->Selected->Item[ListView1->ItemIndex];
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnPreviewClick(TObject *Sender)
{
	int button_tag = (((TComponent *)Sender)->Tag);
	switch (button_tag)
	{
		case 0 : { ReportType = rtPreview; break; }
		case 1 : { ReportType = rtExcel;   break; }
		case 2 : { ReportType = rtGraph;   break; }
	}
	TCursor Save_Cursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;
	try
	{
		// handles multiple click on the Preview button
		if (fRunningReport)
			return;
		else
			fRunningReport = true;
		// close the finddialog box if they were to lazy to close it
		tsCheckFilterExit(Sender);
		if (ReportControl)
		{
			SaveCurrentFilter();

			(this->*(ReportControl->PrintReport))(ReportControl);
		}
	}
	__finally
	{
		Screen->Cursor = Save_Cursor;
		fRunningReport = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::rbFilterAllClick(TObject *Sender)
{
	for (int i=0; i<chlbFilterList->Items->Count; i++)
	{
		chlbFilterList->Checked[i] = false;
	}
	chlbFilterList->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::rbFilterSelectionClick(TObject *Sender)
{
	chlbFilterList->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::rbTreeAllClick(TObject *Sender)
{
	TTreeNode *CurItem = tvFilterTree->Items->GetFirstNode();
	while (CurItem)
	{
		CurItem->StateIndex = STATE_IMAGE_SMALL_GRAY_UNCHECKED;
		CurItem = CurItem->GetNext();
	}
	tvFilterTree->FullCollapse();
	tvFilterTree->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::rbTreeSelectionClick(TObject *Sender)
{
	TTreeNode *CurItem = tvFilterTree->Items->GetFirstNode();
	while (CurItem)
	{
		CurItem->StateIndex = STATE_IMAGE_SMALL_UNCHECKED;
		CurItem = CurItem->GetNext();
	}
	tvFilterTree->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::tvFilterTreeMouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	 THitTests HitTests = tvFilterTree->GetHitTestInfoAt(X, Y);
	 bool CheckBoxClicked = HitTests.Contains(htOnStateIcon);

	 if (CheckBoxClicked)
	 {
		  TTreeNode *Node = tvFilterTree->GetNodeAt(X, Y);
		  if (Node)
		  {
			  if (Node->Level == 0)
			  {
					if (Node->StateIndex == STATE_IMAGE_SMALL_UNCHECKED || Node->StateIndex == STATE_IMAGE_SMALL_GRAY_CHECKED)
					{
						Node->StateIndex = STATE_IMAGE_SMALL_CHECKED;
					}
					else if (Node->StateIndex == STATE_IMAGE_SMALL_CHECKED)
					{
						Node->StateIndex = STATE_IMAGE_SMALL_UNCHECKED;
					}
					TTreeNode *GroupNode = Node->getFirstChild();
					while (GroupNode)
					{
						GroupNode->StateIndex = Node->StateIndex;
						GroupNode = GroupNode->getNextSibling();
					}
			  }
			  else if (Node->Level == 1)
			  {
					if (Node->StateIndex == STATE_IMAGE_SMALL_UNCHECKED)
					{
						Node->StateIndex = STATE_IMAGE_SMALL_CHECKED;
					}
					else if (Node->StateIndex == STATE_IMAGE_SMALL_CHECKED)
					{
						Node->StateIndex = STATE_IMAGE_SMALL_UNCHECKED;
					}
					bool AllSelected = true;
					bool SomeSelected = false;
					TTreeNode *GroupNode = Node->Parent->getFirstChild();
					while (GroupNode)
					{
						if (GroupNode->StateIndex == STATE_IMAGE_SMALL_CHECKED)
						{
							SomeSelected = true;
						}
						else
						{
							AllSelected = false;
						}
						GroupNode = GroupNode->getNextSibling();
					}
					if (AllSelected)
					{
						Node->Parent->StateIndex = STATE_IMAGE_SMALL_CHECKED;
					}
					else if (SomeSelected)
					{
						Node->Parent->StateIndex = STATE_IMAGE_SMALL_GRAY_CHECKED;
					}
					else
					{
						Node->Parent->StateIndex = STATE_IMAGE_SMALL_UNCHECKED;
					}
			  }
		  }
	 }
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::tvFilterTreeKeyPress(TObject *Sender,
		char &Key)
{
	TTreeNode *Node = tvFilterTree->Selected;
	if (Node)
	{
		if (Node->StateIndex == STATE_IMAGE_SMALL_UNCHECKED)
		{
			Node->StateIndex = STATE_IMAGE_SMALL_CHECKED;
		}
		else if (Node->StateIndex == STATE_IMAGE_SMALL_CHECKED)
		{
			Node->StateIndex = STATE_IMAGE_SMALL_UNCHECKED;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::TreeView1Change(TObject *Sender,
		TTreeNode *Node)
{
	bool ExpandNode = true;
	if (LastNode)
	{
		if (LastNode == Node)
		{
			ExpandNode = false;
		}
	}
	if (ExpandNode)
	{
		Node->Expand(false);
	}
	if (Node->Level == 0)
	{
		ListView1->Items->Clear();
		for (int i=0; i<Node->Count; i++)
		{
			TListItem *ListItem		= ListView1->Items->Add();
			ListItem->Caption			= Node->Item[i]->Text;
			ListItem->ImageIndex		= ICON_NOTEPAD_INDEX;
		}
		pcReportProperties->ActivePage = tsFolderDescription;
	}
	if (ReportControl)
	{
		delete ReportControl;
		ReportControl = NULL;
	}
	static AnsiString MenuListSQL =
		"Select Menu_Name From Menu Order By Menu_Name";

	static AnsiString ConsumptionMenuList =
			"Select Distinct "
				"Archive.Menu_Name "
			"From "
				"Security Left Join Archive on "
					"Security.Security_Ref = Archive.Security_Ref "
			"Where "
				"Archive.TIME_STAMP_BILLED >= :StartTime and "
				"Archive.TIME_STAMP_BILLED < :EndTime and "
				"Security.Security_Event = 'Ordered By' "
			"Group By "
				"Archive.Menu_Name "
			"Having "
				"Count(Archive.Archive_Key) > 0 "

			"Union All "

			"Select Distinct "
				"DayArchive.Menu_Name "
			"From "
				"Security Left Join DayArchive on "
					"Security.Security_Ref = DayArchive.Security_Ref "
			"Where "
				"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
				"DayArchive.TIME_STAMP_BILLED < :EndTime and "
				"Security.Security_Event = 'Ordered By' "
			"Group By "
				"DayArchive.Menu_Name "
			"Having "
				"Count(DayArchive.Archive_Key) > 0 "

			"Union All "

			"Select Distinct "
				"Orders.Menu_Name "
			"From "
				"Security Left Join Orders on "
					"Security.Security_Ref = Orders.Security_Ref "
			"Where "
				"Orders.Time_Stamp >= :StartTime and "
				"Orders.Time_Stamp < :EndTime and "
				"Security.Security_Event = 'Ordered By' "
			"Group By "
				"Orders.Menu_Name "
			"Having "
				"Count(Orders.Order_Key) > 0 "

			"Order By "
				"1 ";
   //MM-10050

   static AnsiString MenuBreakdown_Category =
		"select "

"MENU.MENU_NAME "
  "from ARCCATEGORIES right join ITEMSIZECATEGORY on "
" ARCCATEGORIES.CATEGORY_KEY=ITEMSIZECATEGORY.CATEGORY_KEY "
"left join ITEMSIZE on ITEMSIZECATEGORY.ITEMSIZE_KEY=ITEMSIZE.ITEMSIZE_KEY "
"left join ITEM on ITEMSIZE.ITEM_KEY=ITEM.ITEM_KEY  "
"left join SIZES on ITEMSIZE.SIZES_KEY=SIZES.SIZES_KEY "
"left join MENU on SIZES.MENU_KEY=MENU.MENU_KEY "
"group by "


	"MENU.MENU_NAME "
      

         "Order By "
				"1 ";









	static AnsiString SelectLocationList =
		"Select "
			"Name as Order_Location "
		"From "
			"Locations";

	static AnsiString ConsumptionLocationList =
		"Select "
			"Archive.Order_Location "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
		"Where "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "
		"Group By "
			"Archive.Order_Location "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union "

		"Select "
			"DayArchive.Order_Location "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
		"Where "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "
		"Group By "
			"DayArchive.Order_Location "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

		"Union "

		"Select "
			"Orders.Order_Location "
		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
		"Where "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' "
		"Group By "
			"Orders.Order_Location "
		"Having "
			"Count(Orders.Order_Key) > 0 "

		"Order By "
			"1";

	static AnsiString BillLocationList =
		"Select "
			"ArcBill.Billed_Location "
		"From "
			"Security Left Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
		"Where "
			"ArcBill.TIME_STAMP >= :StartTime and "
			"ArcBill.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Billed By' "
		"Group By "
			"ArcBill.Billed_Location "
		"Having "
			"Count(ArcBill.ArcBill_Key) > 0 "

		"Union "

		"Select "
			"DayArcBill.Billed_Location "
		"From "
			"Security Left Join DayArcBill on "
				"Security.Security_Ref = DayArcBill.Security_Ref "
		"Where "
			"DayArcBill.TIME_STAMP >= :StartTime and "
			"DayArcBill.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Billed By' "
		"Group By "
			"DayArcBill.Billed_Location "
		"Having "
			"Count(DayArcBill.ArcBill_Key) > 0 "

		"Order By "
			"1";

static AnsiString InvoiceNumberList = " Select "
"			ArcBill.INVOICE_NUMBER "
"		From "
"			Security Left Join ArcBill on "
"				Security.Security_Ref = ArcBill.Security_Ref "
"		Where "
"			ArcBill.TIME_STAMP >= :StartTime and ArcBill.TIME_STAMP < :EndTime and "
"			Security.Security_Event = 'Billed By' "
"		Group By "
"			ArcBill.INVOICE_NUMBER "
"		Having "
"			Count(ArcBill.ArcBill_Key) > 0 "

"		Union "

"		Select "
"			DayArcBill.INVOICE_NUMBER "
"		From "
"			Security Left Join DayArcBill on "
"				Security.Security_Ref = DayArcBill.Security_Ref "
"		Where "
"			DayArcBill.TIME_STAMP >= :StartTime and "
"			DayArcBill.TIME_STAMP < :EndTime and "
"			Security.Security_Event = 'Billed By' "
"		Group By "
"			DayArcBill.INVOICE_NUMBER "
"		Having "
"			Count(DayArcBill.ArcBill_Key) > 0 "

"		Order By "
"			1 " ;

	static AnsiString ConsumptionUserList =
		"Select Distinct "
			"Contacts.Name "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
			"Left Join Contacts on "
				"Security.User_Key = Contacts.Contacts_Key "
		"Where "
			"(Archive.Order_Type = 3 or "
			"Archive.Order_Type = 0) and "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Union All "

		"Select "
			"Contacts.Name "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
			"Left Join Contacts on "
				"Security.User_Key = Contacts.Contacts_Key "
		"Where "
			"(DayArchive.Order_Type = 3 or "
			"DayArchive.Order_Type = 0) and "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Union All "

		"Select "
			"Contacts.Name "
		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
			"Left Join Contacts on "
				"Security.User_Key = Contacts.Contacts_Key "
		"Where "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Order By "
			"1 ";

	static AnsiString ConsumptionTableList =
		"Select Distinct "
			"Archive.Table_Number "
		"From "
			"Security Inner Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
		"Where "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Union All "

		"Select Distinct "
			"DayArchive.Table_Number "
		"From "
			"Security Inner Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
		"Where "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and  "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Union All "

		"Select Distinct "
			"Orders.Table_Number "
		"From "
			"Security Inner Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
		"Where "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Order By "
			"1";

	static AnsiString ConsumptionTabList =
		"Select Distinct "
			"Cast(Archive.Tab_Name as VarChar(32)) Tab_Name "
		"From "
			"Security Inner Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
		"Where "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Union All "

		"Select Distinct "
			"Cast(DayArchive.Tab_Name as VarChar(32)) Tab_Name "
		"From "
			"Security Inner Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
		"Where "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and  "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Union All "

		"Select Distinct "
			"Cast(Orders.Tab_Name as VarChar(32)) Tab_Name "
		"From "
			"Security Inner Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
		"Where "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Order By "
			"1";

	static AnsiString ConsumptionTerminalsList =
		"Select Distinct "
			"Security.Terminal_Name "
		"From "
			"Security Inner Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
		"Where "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Union All "

		"Select Distinct "
			"Security.Terminal_Name "
		"From "
			"Security Inner Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
		"Where "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and  "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Union All "

		"Select Distinct "
			"Security.Terminal_Name "
		"From "
			"Security Inner Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
		"Where "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Order By "
			"1";

	static AnsiString OrdersTabList =
		"Select Distinct "
			"Tab.Tab_Name "
		"From "
			"Orders Left Join Security On "
				"Security.Security_Ref = Orders.Security_Ref "
			"Left join Tab On Orders.Tab_Key = Tab.Tab_Key "
		"Where "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' "
		"Order By "
			"Tab.Tab_Name";   //testme

	const AnsiString UsersList =

			"Select Distinct "
				"Contacts.Name "
			"From "
				"Security Inner Join Archive On "
					"Security.Security_Ref = Archive.Security_Ref "
				"Inner Join Contacts On "
					"Contacts.Contacts_Key = Security.User_Key "
			"Where "
				"Archive.TIME_STAMP_BILLED >= :StartTime and "
				"Archive.TIME_STAMP_BILLED < :EndTime and "
				"Security.Security_Event = 'Cancel' and "
				"Archive.Archive_Key Is Not Null "

			"Union "

			"Select Distinct "
				"Contacts.Name "
			"From "
				"Security Inner Join DayArchive On "
					"Security.Security_Ref = DayArchive.Security_Ref "
				"Inner Join Contacts On "
					"Contacts.Contacts_Key = Security.User_Key "
			"Where "
				"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
				"DayArchive.TIME_STAMP_BILLED < :EndTime and "
				"Security.Security_Event = 'Cancel' and "
				"DayArchive.Archive_Key Is Not Null "

			"Union "

			"Select Distinct "
				"Contacts.Name "
			"From "
				"Security Left Join Orders On "
					"Security.Security_Ref = Orders.Security_Ref "
				"Left Join Contacts On "
					"Contacts.Contacts_Key = Security.User_Key "
			"Where "
				"Orders.Time_Stamp >= :StartTime and "
				"Orders.Time_Stamp < :EndTime and "
				"Security.Security_Event = 'Cancel' And "
				"Orders.Order_Key Is Not Null "
			"Order By "
				"1";


	// Only get Location and Member Discounts for the period
	static AnsiString LoyaltyDiscountsList =
		"Select Distinct "
			"Security_Event "
		"From "
			"Security "
		"Where "
			"Time_Stamp >= :StartTime and "
			"Time_Stamp < :EndTime and "
			"Security_Event in ('Location Discount','Member Discount') "
		"Order By "
			"Security_Event";

	static AnsiString BillTerminalsList =
		"Select Distinct "
			"Security.Terminal_Name "
		"From "
			"Security "
		"Where "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' "
		"Order By "
			"Security.Terminal_Name";

    static AnsiString InvoiceList =
        "SELECT DISTINCT "
            "groups.name Name "
        "from "
            "contactgroups "
            "left join groups on contactgroups.groups_key = groups.groups_key "
            "left join contacts on contactgroups.CONTACTS_KEY = contacts.CONTACTS_KEY "
            "left join invoices on contacts.CONTACTS_KEY = invoices.contacts_KEY "
        "where "
            "invoices.creation_date >= :StartTime and "
            "invoices.creation_date < :Endtime and "
            "groups.status = '0';";

    static AnsiString SkimmingList =
        "Select Distinct "
            "Security.Terminal_Name "
        "From "
            "Security "
        "Where "
            "Security.Time_Stamp >= :StartTime and "
            "Security.Time_Stamp < :EndTime";

    static AnsiString RefloatList =
        "Select Distinct "
            "Security.Terminal_Name "
        "From "
            "Security "
        "Where "
            "Security.Time_Stamp >= :StartTime and "
            "Security.Time_Stamp < :EndTime";

	static AnsiString HalfHourlyTerminalsList =
		"Select Distinct "
			"ArcBill.Terminal_Name "
		"From "
			"ArcBill Right Join Security on "
				"ArcBill.Security_Ref = Security.Security_Ref "
		"Where "
			"ArcBill.Time_Stamp >= :StartTime and "
			"ArcBill.Time_Stamp <  :EndTime and "
			"Security.Security_Event = 'Billed By' "
		"Union "
		"Select Distinct "
			"DayArcBill.Terminal_Name "
		"From "
			"DayArcBill Right Join Security on "
				"DayArcBill.Security_Ref = Security.Security_Ref "
		"Where "
			"DayArcBill.Time_Stamp >= :StartTime and "
			"DayArcBill.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' "
		"Order By "
			"1";

	// get Internal and External Loyalty Members
	// They should be using one or the other
	static AnsiString LoyaltyCustomerList =
		"Select "
			"Name "
		"From "
			"Contacts "
		"Where "
			"Contact_Type in (2,4)"
		"Order By "
			"Name";

	 static AnsiString LoyaltyPurchaseCustomerList =
		"Select Distinct "
			"Contacts.Name "
		"From "
			"ContactFreebie Left Join Contacts on "
				"ContactFreebie.Contacts_Key = Contacts.Contacts_Key ";
		"Where "
			"Contacts.Contact_Type in (2,4) "
		"Order By "
			"Contacts.Name";
	// get both from Archive and DayArchive   cww
	static AnsiString LoyaltyArchiveList =
		"Select Distinct "
			"Contacts.Name "
		"From "
			"Archive Inner Join Contacts On Archive.Loyalty_Key = Contacts.Contacts_Key "
			"Inner Join Security On Archive.Security_Ref = Security.Security_Ref "
		"Where "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "
		"Union "
		"Select Distinct "
			"Contacts.Name "
		"From "
			"DayArchive Inner Join Contacts On DayArchive.Loyalty_Key = Contacts.Contacts_Key "
			"Inner Join Security On DayArchive.Security_Ref = Security.Security_Ref "
		"Where "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "
		"Order By "
			"1";

	static AnsiString LoyaltyItemList =
		"Select Distinct "
			"(item_Name || ' - ' || size_name) as Name "
		"From "
			"Archive "
		"Where "
			"Order_Type in (0,3) and "
			"Time_Stamp_Billed >= :StartTime and "
			"Time_Stamp_Billed < :EndTime "
		"Union "
		"Select Distinct "
			"(item_Name || ' - ' || size_name) as Name "
		"From "
			"DayArchive "
		"Where "
			"Order_Type in (0,3) and "
			"Time_Stamp_Billed >= :StartTime and "
			"Time_Stamp_Billed < :EndTime "
		"Order By "
			"1";

	static AnsiString CashDrawerUsersList =
		"Select Distinct "
			"Contacts.Name "
		"From "
			"Security Inner Join Contacts On Contacts.Contacts_Key = Security.User_Key "
		"Where "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Manually Opened Cash Drawer' "
		"Order By "
			"Contacts.Name";

	static AnsiString StockTreeList =
		"Select "
			"StockCategory.Stock_Category,"
			"StockCategory.Stock_Category_Key,"
			"StockGroup.Stock_Group,"
			"StockGroup.Stock_Group_Key "
		"From "
			"StockCategory Inner Join StockGroup on "
				"StockCategory.Stock_Category_Key = StockGroup.Stock_Category_Key "
		"Order By "
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group";




	static AnsiString CategoryTreeListwithoutDatefilter =    //cww
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"ArcCategories.Category_Key "
		"From "
			"Security Inner Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
			"Inner Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
			"Inner Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"Archive.Order_Type in (0,3) and "

			"Security.Security_Event = 'Ordered By' "

		"Union "
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"ArcCategories.Category_Key "
		"From "
			"Security Inner Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
			"Inner Join ArcCategories on "
				"DayArchive.Category_Key = ArcCategories.Category_Key "
			"Inner Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"DayArchive.Order_Type in (0,3) and "
			"Security.Security_Event = 'Ordered By' "

		"Union "
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"ArcCategories.Category_Key "
		"From "
			"Security Inner Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
			"Inner Join ArcCategories on "
				"Orders.Category_Key = ArcCategories.Category_Key "
			"Inner Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"Orders.Order_Type in (0,3) and "
			"Security.Security_Event = 'Ordered By' "
		"Order By "
			"1,2,3 ";



	static AnsiString CategoryTreeList =    //cww
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"ArcCategories.Category_Key "
		"From "
			"Security Inner Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
			"Inner Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
			"Inner Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"Archive.Order_Type in (0,3) and "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Union "
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"ArcCategories.Category_Key "
		"From "
			"Security Inner Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
			"Inner Join ArcCategories on "
				"DayArchive.Category_Key = ArcCategories.Category_Key "
			"Inner Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"DayArchive.Order_Type in (0,3) and "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' "

		"Union "
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"ArcCategories.Category_Key "
		"From "
			"Security Inner Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
			"Inner Join ArcCategories on "
				"Orders.Category_Key = ArcCategories.Category_Key "
			"Inner Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"Orders.Order_Type in (0,3) and "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' "
		"Order By "
			"1,2,3 ";

 	static AnsiString StocktakeHistoryList =
		"Select "
			"Stocktake_Key,"
			"Name || ',  Initialised Time = ' || "
			"Extract (Day From Initialised_Time) || '/' || "
			"Extract (Month From Initialised_Time) || '/' || "
			"Extract (Year From Initialised_Time) || ' ' || "
			"Extract (Hour From Initialised_Time) || ':' || "
			"Extract (Minute From Initialised_Time) Initialised_Time "
		"From "
			"Stocktake "
		"Where "
			"Committed_Time Is Not Null And "
			"Initialised_Time >= :StartTime and "
			"Initialised_Time < :EndTime "
		"Order By "
			"1";

	static AnsiString StockLocationList =
		"Select Distinct "
			"Location "
		"From "
			"StockLocation "
		"Order By "
			"Location";

    static AnsiString StockRecipes =
        "Select Distinct "
            "Note "
        "From "
            "StockTrans "
        "Where "
            "Transaction_Type = 'Manufacture' and "
            "Created >= :StartTime and "
            "Created < :EndTime "
        "Order By "
            "Note";

	static AnsiString StockSupplierList =
		"Select "
			"Company_Name "
		"From "
			"Contact "
		"Order By "
			"Company_Name";

	static AnsiString SalesTypesList =
		"Select "
		"Sales_Type,"
		"Cast('Tabs' as varchar(16)) as Sales_Type_Description "
		"From ArcBill "
		"where Sales_Type = 0 "
		"Group by "
		"Sales_Type "

		"union all "

		"Select "
		"Sales_Type,"
		"Cast('Tables' as varchar(16)) as Sales_Type_Description "
		"From ArcBill "
		"where Sales_Type = 1 "

		"Group by "
		"Sales_Type "

		"union all "

		"Select "
		"Sales_Type,"
		"Cast('Cash' as varchar(16)) as Sales_Type_Description "
		"From ArcBill "
		"where Sales_Type = 2 "

		"Group by "
		"Sales_Type "

		"union all "

		"Select "
		"Sales_Type,"
		"Cast('Account' as varchar(16)) as Sales_Type_Description "
		"From ArcBill "
		"where Sales_Type = 3 "

		"Group by "
		"Sales_Type "

		"union all "

		"Select "
		"Sales_Type,"
		"Cast('Room' as varchar(16)) as Sales_Type_Description "
		"From ArcBill "
		"where Sales_Type = 4 "

		"Group by "
		"Sales_Type "

		"union all "

		"Select "
		"Sales_Type,"
		"Cast('Credits' as varchar(16)) as Sales_Type_Description "
		"From ArcBill "
		"where Sales_Type = 5 "

		"Group by "
		"Sales_Type "

		"union all "

		"Select "
		"Sales_Type,"
		"Cast('Credit Purchase' as varchar(16)) as Sales_Type_Description "
		"From ArcBill "
		"where Sales_Type = 6 "

		"Group by "
		"Sales_Type ";

   static AnsiString TerminalsList =
      "Select Distinct "
		   "Security.Terminal_Name "
      "From "
		   "Security "
		"Where "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' "
		"Order By "
			"Security.Terminal_Name";

 static AnsiString ComplimentaryInvoiceNumberList = " Select "   //MM-3908
"			ArcBill.INVOICE_NUMBER, "
"        ARCBILL.ARCBILL_KEY  "
"		From "
"			Security Left Join Archive on "
"				Security.Security_Ref = Archive.Security_Ref "
"         inner join ARCBILL on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY  "
"         Left Join ArcCategories on  "
"         Archive.Category_Key = ArcCategories.Category_Key "
"			Left Join CategoryGroups on "
"				ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
"     LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME  "
"     FROM ARCORDERDISCOUNTS a  "
"     group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
"     ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY   "
"		Where "
"     (( "
"            COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<>'Non-Chargeable' and "
"          COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)= 'Complimentary') ) and  "
"			ARCBILL.Time_Stamp >= :StartTime and "
"			ARCBILL.Time_Stamp < :EndTime and "
"			Security.Security_Event = 'Ordered By' "
"		Group By "
"			ArcBill.INVOICE_NUMBER, "
"        ARCBILL.ARCBILL_KEY  "
"		Having "
"			Count(Archive.Archive_Key) > 0 "


"		Union "

"		Select "
"			DayArcBill.INVOICE_NUMBER, "
"        DayARCBILL.ARCBILL_KEY  "
"		From "
"		Security Left Join DayArchive on "
"					Security.Security_Ref = DayArchive.Security_Ref "
"         inner join DayARCBILL on DayARCBILL.ARCBILL_KEY = DayARCHIVE.ARCBILL_KEY  "
"         Left Join ArcCategories on  "
"         	DayArchive.Category_Key = ArcCategories.Category_Key "
"			Left Join CategoryGroups on "
"				ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
"     LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME  "
"     FROM DayARCORDERDISCOUNTS a "
"     group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
"    DayARCORDERDISCOUNTS on DayARCHIVE.ARCHIVE_KEY = DayARCORDERDISCOUNTS.ARCHIVE_KEY  "
"		Where "
"     (( "
"           COALESCE(DayARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<>'Non-Chargeable' and "
"           COALESCE(DayARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)= 'Complimentary') ) and "
"			DayARCBILL.Time_Stamp >= :StartTime and "
"			DayARCBILL.Time_Stamp < :EndTime and "
"			Security.Security_Event = 'Ordered By' "
"		Group By "
"			DayArcBill.INVOICE_NUMBER, "
"        DayARCBILL.ARCBILL_KEY  "
"		Having "
"			Count(DayArchive.Archive_Key) > 0 "
"		Order By "
"			2 " ;


 static AnsiString NonComplimentaryInvoiceNumberList = " Select "   //MM-3908
"			ArcBill.INVOICE_NUMBER, "
"        ARCBILL.ARCBILL_KEY  "
"		From "
"		Security Left Join Archive on  "

"				Security.Security_Ref = Archive.Security_Ref "
"     inner join ARCBILL on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY  "
"     Left Join ArcCategories on   "
"     Archive.Category_Key = ArcCategories.Category_Key  "
"     Left Join CategoryGroups on  "
"     ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
"         LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
"     		FROM ARCORDERDISCOUNTS a "
"     		group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
"     ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY  "
"      Where"

"     (( "
"           COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)= 'Non-Chargeable' and  "
"          COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and  "

"			ARCBILL.Time_Stamp >= :StartTime and "
"        ARCBILL.Time_Stamp < :EndTime and "
"			Security.Security_Event = 'Ordered By'  "
"		Group By "
"			ARCBILL.Invoice_Number, "
"        ARCBILL.ARCBILL_KEY  "
"		Having "
"			Count(Archive.Archive_Key) > 0  "

"		Union "

"		Select "
"			DayARCBILL.Invoice_Number, "
"        DAYARCBILL.ARCBILL_KEY "
"		From "
"			Security Left Join DayArchive on "
"				Security.Security_Ref = DayArchive.Security_Ref "
"     inner join DAYARCBILL on DAYARCBILL.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY  "
"     Left Join ArcCategories on  "
"     DayArchive.Category_Key = ArcCategories.Category_Key  "
"     Left Join CategoryGroups on   "
"     ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
"         LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
"     		FROM DayARCORDERDISCOUNTS a "
"     		group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
"     DayARCORDERDISCOUNTS on DayARCHIVE.ARCHIVE_KEY = DayARCORDERDISCOUNTS.ARCHIVE_KEY "
"      Where"

"     (( "
"           COALESCE(DayARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)= 'Non-Chargeable' and "
"           COALESCE(DayARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and  "

"			DAYARCBILL.Time_Stamp >= :StartTime and "
"        DAYARCBILL.Time_Stamp < :EndTime and "
"			Security.Security_Event = 'Ordered by' "
"		Group By "
"			DayArcBill.INVOICE_NUMBER, "
"        DayARCBILL.ARCBILL_KEY  "
"		Having "
"			Count(DayArchive.Archive_Key) > 0 "

"		Order By "
"			2 " ;

static AnsiString PaymentTypeList =
            "SELECT  DABP.PAY_TYPE                                           "
            "FROM DAYARCBILLPAY DABP                                         "
            "INNER JOIN DAYARCBILL DAB ON DABP.ARCBILL_KEY = DAB.ARCBILL_KEY "
            "WHERE DABP.PAY_TYPE <> 'Points' and DABP.PAY_TYPE <> 'Dining' and DABP.SUBTOTAL <> 0 and DAB.TIME_STAMP >= :StartTime and  "
            "DAB.TIME_STAMP < :EndTime "
            "GROUP BY 1                                                      "
            "UNION                                                           "
            "SELECT  ABP.PAY_TYPE                                            "
            "FROM ARCBILLPAY ABP                                             "
            "INNER JOIN ARCBILL AB ON ABP.ARCBILL_KEY = AB.ARCBILL_KEY       "
            "WHERE ABP.PAY_TYPE <> 'Points' and ABP.PAY_TYPE <> 'Dining' and ABP.SUBTOTAL <> 0  and AB.TIME_STAMP >= :StartTime and               "
			"AB.TIME_STAMP < :EndTime "
            "GROUP BY 1 ";


 static AnsiString StockCategoryFilterOnTimeBasis =
 " Select "
 " STOCKTRANS.STOCK_CATEGORY, "
 " STOCKTRANS.STOCK_GROUP "
 " FROM "
 " STOCKTRANS " 
 " WHERE "
 " STOCKTRANS.CREATED >= :StartTime "
 " AND "
 " STOCKTRANS.CREATED <= :EndTime "

 " GROUP BY "
 " STOCKTRANS.STOCK_CATEGORY, "
 " STOCKTRANS.STOCK_GROUP " ;



	Security::PermissionAccess requiredPermission = Security::None;
    bool isEJournalReport = false;

	switch (Node->AbsoluteIndex)
	{
		case MENUS_GP_INDEX:
		{
			requiredPermission = Security::MenuReports;

			ReportControl									= new TReportControl();
			ReportControl->PrintReport					= &TfrmReports::PrintMenuProfit;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Menus Item Prices");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Menus Special Prices");

			TReportCheckboxFilter *ReportFilter		= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter->Caption						= "Select the menus you wish to have appear in this report.";
			//ReportFilter->ShowGST						= true;
			//ReportFilter->GSTChecked					= true; // cww today
//			ReportFilter->ShowPreview					= true;
//			ReportFilter->ShowBack						= false;
			ReportFilter->SQL								= MenuListSQL;
			ReportFilter->DisplayField					= "Menu_Name";
			ReportFilter->SelectionField				= "Menu_Name";
			ReportFilter->SelectionDateRange			= false;
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter);
			//ReportControl->CurrentSubReport			= 0;
			break;
		}
		case MENUS_RECIPES_INDEX:
		{
			requiredPermission = Security::MenuReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintMenuRecipes;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Menu Recipes");

			TReportCheckboxFilter *ReportFilter		= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter->Caption						= "Select the menus you wish to have appear in this report.";
//			ReportFilter->ShowGST						= true;
//			ReportFilter->GSTChecked					= true;
//			ReportFilter->ShowPreview					= true;
//			ReportFilter->ShowBack						= false;
			ReportFilter->SQL								= MenuListSQL;
			ReportFilter->DisplayField					= "Menu_Name";
			ReportFilter->SelectionField				= "Menu_Name";
			ReportFilter->SelectionDateRange			= false;
			SubReport1->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter);
			break;
		}
		case STOCK_REDUCTION_ITEMS:
		{
			requiredPermission = Security::MenuReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockReductionItems;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stock Reduction Items");

			TReportTreeFilter *ReportFilter0			= new TReportTreeFilter(ReportControl, StockFilterTransaction);

			ReportFilter0->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter0->SQL							= StockTreeList;
			ReportFilter0->DisplayRootField			= "Stock_Category";
			ReportFilter0->RootKeyField				= "Stock_Category";
			ReportFilter0->DisplayChildField			= "Stock_Group";
			ReportFilter0->ChildKeyField				= "Stock_Group";
			ReportFilter0->SelectionField				= "Stock_Group";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);

			ReportFilter1->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter1->SQL							= StockLocationList;
			ReportFilter1->DisplayField				= "Location";
			ReportFilter1->SelectionField				= "Location";
			SubReport0->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter1);
			break;
		}


     case Tax_Profile_INDEX:
		{
			requiredPermission = Security::MenuReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintTaxProfile;
			TSubReport *SubReport0						= ReportControl->AddSubReport("TAX PROFILE");

			TReportTreeFilter *ReportFilter0			= new TReportTreeFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the items you wish to appear in the report.";
	      ReportFilter0->SQL							= CategoryTreeListwithoutDatefilter;
         ReportFilter0->DisplayRootField			= "Name";
			ReportFilter0->RootKeyField				= "Name";
			ReportFilter0->DisplayChildField			= "Category";
			ReportFilter0->ChildKeyField				= "Category";
			ReportFilter0->SelectionField				= "Category_Key";
			ReportFilter0->ShowGST						= false;
			ReportFilter0->GSTChecked					= false;
		 	SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);
         		break;
		}




		case MENU_3RD_PARTY_INDEX:
		{
			requiredPermission = Security::MenuReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintMenu3rdPartyCodes;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Third Party Codes");

			TReportCheckboxFilter *ReportFilter		= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter->Caption						= "Select the menus you wish to have appear in this report.";
			ReportFilter->SQL								= MenuListSQL;
			ReportFilter->DisplayField					= "Menu_Name";
			ReportFilter->SelectionField				= "Menu_Name";
			ReportFilter->SelectionDateRange			= false;
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter);
			break;
		}
		case MENU_ITEM_BARCODES:
		{
			requiredPermission = Security::MenuReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintMenuItemBarcodes;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Barcodes");

			TReportCheckboxFilter *ReportFilter		= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter->Caption						= "Select the menus you wish to have appear in this report.";
			ReportFilter->SQL								= MenuListSQL;
			ReportFilter->DisplayField					= "Menu_Name";
			ReportFilter->SelectionField				= "Menu_Name";
			ReportFilter->SelectionDateRange			= false;
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter);
			break;
		}
        	case Breakdown_Category:
		{
			requiredPermission = Security::MenuReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintBreakdownCategory;
			TSubReport *SubReport0						= ReportControl->AddSubReport("BreakdownCategory");

			TReportCheckboxFilter *ReportFilter		= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter->Caption						= "Select the menus you wish to have appear in this report.";
			ReportFilter->SQL								= MenuBreakdown_Category;
			ReportFilter->DisplayField					= "Menu_Name";
			ReportFilter->SelectionField				= "Menu_Name";
			ReportFilter->SelectionDateRange			= false;
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter);
			break;
		}

		case CASHUP_INDEX:
		{
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintCashup;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Cashup");

			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the cashup report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;

			SubReport1->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the terminals you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							= TerminalsList;
			ReportFilter2->DisplayField				= "Terminal_Name";
			ReportFilter2->SelectionField				= "Terminal_Name";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);
			break;
		}
		case CATEGORY_ANALYSIS_INDEX:
		{
			requiredPermission = Security::SalesReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintCategoryAnalysis;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Category Analysis (By Location)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Category Analysis (All Locations)");

			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the category analysis report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							= ConsumptionLocationList;
			ReportFilter2->DisplayField				= "Order_Location";
			ReportFilter2->SelectionField				= "Order_Location";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);
			break;
		}
		case CATEGORY_BREAKDOWN_INDEX:
		{
			requiredPermission = Security::SalesReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintCategoryBreakdown;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Category Breakdown (By Location)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Category Breakdown (All Locations)");

			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the category breakdown report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							= ConsumptionLocationList;
			ReportFilter2->DisplayField				= "Order_Location";
			ReportFilter2->SelectionField				= "Order_Location";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);
			break;
		}
		case HALF_HOULY_SALES_INDEX:
		{
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintHalfHourlySales;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Half Hourly Sales (Daily)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Half Hourly Sales (Consolidated)");

			// Date Range
			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the date range for the half-hourly sales report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter1);

			// Terminals
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the terminals you wish to appear in the report.";
			ReportFilter2->SQL							= HalfHourlyTerminalsList;
			ReportFilter2->DisplayField				= "Terminal_Name";
			ReportFilter2->SelectionField				= "Terminal_Name";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);
			SubReport2->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter2);

			break;
		}
		case TIPS_AND_VOUCHERS_INDEX:
		{
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintTipsAndVouchers;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Tips And Vouchers");

			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the tips and vouchers sales report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the transaction type.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							=
				"Select Distinct "
					"ArcSurcharge.Pay_Type "
				"From "
					"ArcSurcharge Left Join ArcBill On "
					"ArcSurcharge.Arcbill_Key = ArcBill.Arcbill_Key "
					"Left Join Security On "
					"ArcBill.Security_Ref = Security.Security_Ref "
					"Left Join Contacts On "
					"Security.User_Key = Contacts.Contacts_Key "
				"Where "
					"ArcBill.Time_Stamp >= :StartTime and "
					"ArcBill.Time_Stamp < :EndTime and "
					"Security.Security_Event = 'Billed By' "
				"Order By "
					"ArcSurcharge.Pay_Type";
			ReportFilter2->DisplayField				= "Pay_Type";
			ReportFilter2->SelectionField				= "Pay_Type";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);
			break;
		}
		case WAGES_INDEX:
		{
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintWages;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Wages (By Department)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Wages (By Staff Member)");

			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the wages report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the staff you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							=
				"Select Distinct "
					"Name "
				"From "
					"Contacts Inner Join ContactTime on "
					   "Contacts.Contacts_Key = ContactTime.Contacts_Key "
				"Where "
					"ContactTime.Login_DateTime >= :StartTime and "
					"ContactTime.Login_DateTime < :EndTime";
			ReportFilter2->DisplayField				= "Name";
			ReportFilter2->SelectionField				= "Name";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);
			SubReport2->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);
			break;
		}
		case SAVED_SALES_INDEX:
		{
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintSavedSales;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Saved Sales Totals");

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the saved sales types you wish to appear in the report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			ReportFilter1->SQL							=
				"Select Distinct "
					"Tab.Tab_Type,"
					"Cast('Tab' as Varchar(25)) Tab_Type_Name "
				"From "
					"Tab "
				"Where "
					"Tab.Tab_Type = 0 "

				"Union All "

				"Select Distinct "
					"Tab.Tab_Type,"
					"Cast('Staff Tab' as Varchar(25)) Tab_Type_Name "
				"From "
					"Tab "
				"Where "
					"Tab.Tab_Type = 1 "

				"Union All "

				"Select Distinct "
					"Tab.Tab_Type,"
					"Cast('Member' as Varchar(25)) Tab_Type_Name "
				"From "
					"Tab "
				"Where "
					"Tab.Tab_Type = 2 "

				"Union All "

				"Select Distinct "
					"Tab.Tab_Type,"
					"Cast('Table' as Varchar(25)) Tab_Type_Name "
				"From "
					"Tab "
				"Where "
					"Tab.Tab_Type = 3 "

				"Union All "

				"Select Distinct "
					"Tab.Tab_Type,"
					"Cast('Cash Sale' as Varchar(25)) Tab_Type_Name "
				"From "
					"Tab "
				"Where "
					"Tab.Tab_Type = 4 "

				"Union All "

				"Select Distinct "
					"Tab.Tab_Type,"
					"Cast('Rooms' as Varchar(25)) Tab_Type_Name "
				"From "
					"Tab "
				"Where "
					"Tab.Tab_Type = 5 ";

				"Union All "

				"Select Distinct "
					"Tab.Tab_Type,"
					"Cast('Invoice' as Varchar(25)) Tab_Type_Name "
				"From "
					"Tab "
				"Where "
					"Tab.Tab_Type = 6 ";

			ReportFilter1->DisplayField				= "Tab_Type_Name";
			ReportFilter1->SelectionField				= "Tab_Type";
			ReportFilter1->SelectionDateRange		= false;
			SubReport1->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);
			break;
		}
        case SALES_JOURNAL_INDEX:
        {
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintSalesJournal;

			TSubReport *SubReport1						= ReportControl->AddSubReport("Include Service Charge Tax in Tax");
         TSubReport *SubReport2                 = ReportControl->AddSubReport("Include Service Charge Tax in Service Charge");

			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the Sales Journal report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;

			SubReport1->AddFilterIndex(0);
         SubReport2->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *TerminalFilter 	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			TerminalFilter->Caption						= "Select the terminals you wish to appear in the report.";
			TerminalFilter->SQL							= TerminalsList;
			TerminalFilter->DisplayField				= "Terminal_Name";
			TerminalFilter->SelectionField			= "Terminal_Name";
			TerminalFilter->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);
			SubReport2->AddFilterIndex(1);
			ReportControl->AddFilter(TerminalFilter);

			break;
        }

        case ACCUMULATED_ZED_INDEX:
      {
        requiredPermission = Security::FinancialReports;

        ReportControl									= new TReportControl;
        ReportControl->PrintReport					= &TfrmReports::PrintAccumulatedZeds;
        TSubReport *SubReport1						= ReportControl->AddSubReport("Accumulated Z Report");

        TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

        ReportFilter0->Caption						= "Select the date range for the Accumulated Z report.";
        SubReport1->AddFilterIndex(0);
        ReportControl->AddFilter(ReportFilter0);
        break;
      }

        case SALES_SUMMARY_B_INDEX:
		{
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintSalesSummaryB;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Sales Summary B");

			// Date Range SubReport0
			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the date range Sales Summary B report.";
			SubReport1->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							= ConsumptionLocationList;
			ReportFilter2->DisplayField				= "Order_Location";
			ReportFilter2->SelectionField				= "Order_Location";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);
        	break;
		}

        case SALES_SUMMARY_C_INDEX:
		{
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintSalesSummaryC;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Daily Consolidated Revenue Sales Details");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Daily Revenue Sales Details");

			// Date Range
			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the date range for the sales summary report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter1);

			// Terminals
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the terminals you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							= ConsumptionLocationList;
			ReportFilter2->DisplayField				= "Order_Location";
			ReportFilter2->SelectionField				= "Order_Location";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);
			SubReport2->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter2);

			break;
		}

      case Finance_Daily_index:
		{
              	requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintFinanceDaily;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Cashup");

			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the Finance Daily report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);

         // Locations
         TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

		   ReportFilter2->Caption						= "Select the locations you wish to appear in the report.";
		   ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
		   ReportFilter2->SQL							= BillLocationList;
         ReportFilter2->DisplayField				= "Billed_Location";
         ReportFilter2->SelectionField				= "Billed_Location";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);

         TReportCheckboxFilter *ReportFilter3	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter3->Caption						= "Select the Terminal you wish to appear in the report.";
			ReportFilter3->GetSQL						= &TfrmReports::GetTerminalFilter;
			ReportFilter3->DisplayField				= "Terminal_Name";
			ReportFilter3->SelectionField				= "Terminal_Name";
			ReportFilter3->SelectionDateRange		= true;
			ReportFilter3->AddPreviousFilters(ReportFilter2);
			SubReport1->AddFilterIndex(2);

			ReportControl->AddFilter(ReportFilter3);
			break;
        }

		case SALES_SUMMARY_INDEX:
		{
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintSalesSummary;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Sales Summary");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Sales Summary (By Location)");

			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the sales summary report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							= BillLocationList;
			ReportFilter2->DisplayField				= "Billed_Location";
			ReportFilter2->SelectionField				= "Billed_Location";
			ReportFilter2->SelectionDateRange		= true;
			SubReport2->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);
			break;
		}
		case CONSUMPTION_INDEX:
		{
			requiredPermission = Security::SalesReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintConsumption;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Consumption (By Category)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Consumption (By Menu)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Consumption (By Location)");
			TSubReport *SubReport3						= ReportControl->AddSubReport("Consumption (By Tab)");
			TSubReport *SubReport4						= ReportControl->AddSubReport("Consumption (Third Party Codes)");
         TSubReport *SubReport5						= ReportControl->AddSubReport("Consumption (By Day)");
         TSubReport *SubReport6                 = ReportControl->AddSubReport("Consumption (By Category) Excluding Surcharge");

			// Date Range SubReport0,1,2
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the consumption report.";
//			ReportFilter0->ShowGST						= false;
//			ReportFilter0->GSTChecked					= false;
			SubReport0->AddFilterIndex(0);
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);
			SubReport3->AddFilterIndex(0);
			SubReport4->AddFilterIndex(0);
			SubReport5->AddFilterIndex(0);
         SubReport6->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Locations  SubReport1
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter1->SQL							= ConsumptionLocationList;
			ReportFilter1->DisplayField				= "Order_Location";
			ReportFilter1->SelectionField				= "Order_Location";
			ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);
		   	SubReport2->AddFilterIndex(1);
			SubReport3->AddFilterIndex(1);
         SubReport6->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			// Menus SubReport0,1
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the menus you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							= ConsumptionMenuList;
			ReportFilter2->DisplayField				= "Menu_Name";
			ReportFilter2->SelectionField				= "Menu_Name";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(2);
		   //	SubReport2->AddFilterIndex(2);
			SubReport4->AddFilterIndex(2);
			ReportControl->AddFilter(ReportFilter2);

			// Tabs       SubReport2
			TReportCheckboxFilter *ReportFilter3	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter3->Caption						= "Select the tabs you wish to appear in the report.";
			ReportFilter3->ShowGST						= false;
			ReportFilter3->GSTChecked					= false;
			ReportFilter3->SQL							= OrdersTabList;
			ReportFilter3->DisplayField				= "Tab_Name";
			ReportFilter3->SelectionField				= "Tab_Name";
			ReportFilter3->SelectionDateRange		= true;
			SubReport3->AddFilterIndex(3);
			ReportControl->AddFilter(ReportFilter3);

			// Category List
			TReportTreeFilter *ReportFilter4			= new TReportTreeFilter(ReportControl, MMFilterTransaction);
			ReportFilter4->Caption						= "Select the Categories you wish to appear in the report.";
			ReportFilter4->SQL							= CategoryTreeList;
			ReportFilter4->DisplayRootField			= "Name";
			ReportFilter4->RootKeyField				= "Name";

			ReportFilter4->DisplayChildField			= "Category";
			ReportFilter4->ChildKeyField				= "Category";
			ReportFilter4->SelectionField				= "Category_Key";
			ReportFilter4->ShowGST						= false;
			ReportFilter4->GSTChecked					= false;
			ReportFilter4->SelectionDateRange		= true;
			ReportControl->AddFilter(ReportFilter4);
			SubReport0->AddFilterIndex(4);
         SubReport6->AddFilterIndex(4);

			break;
		}
		case CONSUMPTION_BY_HALF_HOUR_INDEX:
		{
			requiredPermission = Security::SalesReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintConsumptionByHalfHour;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Consumption (Category Detailed)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Consumption (Category Summary)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Consumption (Total Daily)");

			// Date Range
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the consumption report.";
			SubReport0->AddFilterIndex(0);
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Category List
			TReportTreeFilter *ReportFilter1			= new TReportTreeFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the Categories you wish to appear in the report.";
			ReportFilter1->SQL							= CategoryTreeList;
			ReportFilter1->DisplayRootField			= "Name";
			ReportFilter1->RootKeyField				= "Name";

			ReportFilter1->DisplayChildField			= "Category";
			ReportFilter1->ChildKeyField				= "Category";
			ReportFilter1->SelectionField				= "Category_Key";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			ReportFilter1->SelectionDateRange		= true;

			int index										= ReportControl->AddFilter(ReportFilter1);
			SubReport0->AddFilterIndex(index);
			SubReport1->AddFilterIndex(index);

			// Terminals
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the terminals you wish to appear in the report.";
			ReportFilter2->SQL							= ConsumptionTerminalsList;
			ReportFilter2->DisplayField				= "Terminal_Name";
			ReportFilter2->SelectionField				= "Terminal_Name";
			ReportFilter2->SelectionDateRange		= true;

			index												= ReportControl->AddFilter(ReportFilter2);
			SubReport2->AddFilterIndex(index);

			break;
		}
		case SALES_BY_SALES_TYPE_INDEX:
		{
			requiredPermission = Security::SalesReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintConsumptionBySalesType;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Consumption By Sales Type");

			// Date Range SubReport0,1,2
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the consumption report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Sales Types  SubReport1
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the sales types you wish to appear in the report.";
			ReportFilter1->SQL							= SalesTypesList;
			ReportFilter1->DisplayField				= "Sales_Type_Description";
			ReportFilter1->SelectionField				= "Sales_Type";
			ReportFilter1->SelectionDateRange		= false;
			SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			// Category List
			TReportTreeFilter *ReportFilter2			= new TReportTreeFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the Categories you wish to appear in the report.";
			ReportFilter2->SQL							= CategoryTreeList;
			ReportFilter2->DisplayRootField			= "Name";
			ReportFilter2->RootKeyField				= "Name";

			ReportFilter2->DisplayChildField			= "Category";
			ReportFilter2->ChildKeyField				= "Category";
			ReportFilter2->SelectionField				= "Category_Key";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SelectionDateRange		= true;
			ReportControl->AddFilter(ReportFilter2);
			SubReport0->AddFilterIndex(2);

			break;
		}
		case USER_SALES_INDEX:
		{
			requiredPermission = Security::SalesReports;

			int FilterIndex;
			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintUserSales;
			TSubReport *SubReport0						= ReportControl->AddSubReport("User Sales (Detailed)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("User Sales (By Category)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("User Sales (Summary)");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the sales report.";
			FilterIndex = ReportControl->AddFilter(ReportFilter0);
			SubReport0->AddFilterIndex(FilterIndex);
			SubReport1->AddFilterIndex(FilterIndex);
			SubReport2->AddFilterIndex(FilterIndex);

			// Category List
			TReportTreeFilter *ReportFilter1			= new TReportTreeFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the Categories you wish to appear in the report.";
			ReportFilter1->SQL							= CategoryTreeList;
			ReportFilter1->DisplayRootField			= "Name";
			ReportFilter1->RootKeyField				= "Name";

			ReportFilter1->DisplayChildField			= "Category";
			ReportFilter1->ChildKeyField				= "Category";
			ReportFilter1->SelectionField				= "Category_Key";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			ReportFilter1->SelectionDateRange		= true;
			FilterIndex = ReportControl->AddFilter(ReportFilter1);
			SubReport0->AddFilterIndex(FilterIndex);

			// users
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the users you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= true;
			ReportFilter2->SQL							= ConsumptionUserList;
			ReportFilter2->DisplayField				= "Name";

			ReportFilter2->SelectionField				= "Name";
			ReportFilter2->SelectionDateRange		= true;
			FilterIndex = ReportControl->AddFilter(ReportFilter2);
			SubReport0->AddFilterIndex(FilterIndex);
			SubReport1->AddFilterIndex(FilterIndex);
			SubReport2->AddFilterIndex(FilterIndex);
			break;
		}
		case PATRON_AVERAGES_INDEX:
		{
			requiredPermission = Security::SalesReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintPatronAverages;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Patron Averages");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the date range for the Patron Averages report.";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);
			break;
		}
		case PATRON_TYPES_INDEX:
		{
			requiredPermission = Security::SalesReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintPatronTypes;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Patron Types");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the Patron Types report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);
                                                 
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the Patron Types you wish to appear in the report.";
			ReportFilter1->SQL							= "Select Patron_Type from PatronTypes";
			ReportFilter1->DisplayField				= "Patron_Type";
			ReportFilter1->SelectionField				= "Patron_Type";
			ReportFilter1->SelectionDateRange		= false;
			SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			break;
		}
case DAILY_SALES_REPORT:
		{

	requiredPermission = Security::SalesReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::DailySalesReport;
			TSubReport *SubReport0						= ReportControl->AddSubReport("DSR (By Category)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("DSR (By Menu)");
            TSubReport *SubReport2						= ReportControl->AddSubReport("DSR (By Day and Time)");
            TSubReport *SubReport3						= ReportControl->AddSubReport("DSR By Receipt");
			TSubReport *SubReport4						= ReportControl->AddSubReport("DSR (Single Receipt Number)");
            TSubReport *SubReport5						= ReportControl->AddSubReport("DSR Consolidated Report");
            TSubReport *SubReport6						= ReportControl->AddSubReport("DSR Category (By Date) ");
            TSubReport *SubReport7						= ReportControl->AddSubReport("DSR Consolidated (By Item) ");
            TSubReport *SubReport8						= ReportControl->AddSubReport("DSR Summary(By Category) ");
            TSubReport *SubReport9						= ReportControl->AddSubReport("DSR Summary(By Menu) ");


			// Date Range SubReport0,1,2
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the consumption report.";
//			ReportFilter0->ShowGST						= false;
//			ReportFilter0->GSTChecked					= false;
			SubReport0->AddFilterIndex(0);
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);
            SubReport3->AddFilterIndex(0);
            SubReport5->AddFilterIndex(0);
            SubReport6->AddFilterIndex(0);
            SubReport7->AddFilterIndex(0);
            SubReport8->AddFilterIndex(0);
            SubReport9->AddFilterIndex(0);
      	    ReportControl->AddFilter(ReportFilter0);

			// Menus SubReport0,1
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the menus you wish to appear in the report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			ReportFilter1->SQL							= ConsumptionMenuList;
			ReportFilter1->DisplayField			    	= "Menu_Name";
			ReportFilter1->SelectionField				= "Menu_Name";
			ReportFilter1->SelectionDateRange		    = true;
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter1);

			// Category List
			TReportTreeFilter *ReportFilter2			= new TReportTreeFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the Categories you wish to appear in the report.";
			ReportFilter2->SQL							= CategoryTreeList;
			ReportFilter2->DisplayRootField			= "Name";
			ReportFilter2->RootKeyField				= "Name";

			ReportFilter2->DisplayChildField			= "Category";
			ReportFilter2->ChildKeyField				= "Category";
			ReportFilter2->SelectionField				= "Category_Key";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SelectionDateRange		= true;
			ReportControl->AddFilter(ReportFilter2);
			SubReport0->AddFilterIndex(2);
            SubReport6->AddFilterIndex(2);

			TReportCheckboxFilter *ReportFilter3	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter3->Caption						= "Select the receipts you wish to appear in the report.";
         ReportFilter3->ShowGST						= false;
			ReportFilter3->GSTChecked					= false;
			ReportFilter3->SQL					    	= InvoiceNumberList;
			ReportFilter3->DisplayField				= "Invoice_Number";
			ReportFilter3->SelectionField				= "Invoice_Number";
			ReportFilter3->SelectionDateRange		= true;
		   SubReport3->AddFilterIndex(3);
			ReportControl->AddFilter(ReportFilter3);

			TReportStringFilter *ReportFilter4		= new TReportStringFilter(ReportControl, MMFilterTransaction);
			ReportFilter4->Caption						= "Select the receipt you wish to appear in the report.";
			ReportFilter4->SelectionField				= "Invoice_Number";
			ReportFilter4->SelectionDateRange		= false;
			SubReport4->AddFilterIndex(4);
			ReportControl->AddFilter(ReportFilter4);


		// Locations
			TReportCheckboxFilter *ReportFilter5	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter5->Caption						= "Select the Locations you wish to appear in the report.";
			ReportFilter5->SQL							= SelectLocationList;
			ReportFilter5->DisplayField				= "Order_Location";
			ReportFilter5->SelectionField				= "Order_Location";
			SubReport5->AddFilterIndex(5);
			ReportControl->AddFilter(ReportFilter5);



			break;

		}

        case Complimentary:
		{
         requiredPermission = Security::SalesReports;

			ReportControl								= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintComplimentary;
			//TSubReport *SubReport0						= ReportControl->AddSubReport("Complimentary (By Category)");
			TSubReport *SubReport0						= ReportControl->AddSubReport("Complimentary By Receipt");

			// Date Range SubReport0
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the Complimentary report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);



			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the receipts you wish to appear in the report.";
         ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			ReportFilter1->SQL					    	= ComplimentaryInvoiceNumberList;      //NonComplimentaryInvoiceNumberList
			ReportFilter1->DisplayField				= "Invoice_Number";
			ReportFilter1->SelectionField				= "Invoice_Number";
			ReportFilter1->SelectionDateRange		= true;
		   SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);
       	break;

		}


        case NonChargeable:
		{
			requiredPermission = Security::SalesReports;                                       //3908

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintNonChargeable;
//			TSubReport *SubReport0						= ReportControl->AddSubReport("NonChargeable (By Category)");
         TSubReport *SubReport0						= ReportControl->AddSubReport("Non-Complimentary By Receipt");
			// Date Range SubReport0
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the NonChargeable report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);



			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the receipts you wish to appear in the report.";
         ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			ReportFilter1->SQL					    	= NonComplimentaryInvoiceNumberList;      //
			ReportFilter1->DisplayField				= "Invoice_Number";
			ReportFilter1->SelectionField				= "Invoice_Number";
			ReportFilter1->SelectionDateRange		= true;
		   SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

        	break;
		}
		case TURN_AROUND_TIMES_INDEX:
		{
			requiredPermission = Security::SalesReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintTurnaroundTimes;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Turnaround Times");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the date range for the turnaround times report.";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);
			break;
		}
		case CHRONOLOGICAL_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintChronological;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Chronological Sales");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the date range for the chronological sales report.";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the terminals you wish to appear in the report.";
//			ReportFilter1->ShowGST						= true;
//			ReportFilter1->GSTChecked					= true;
			ReportFilter1->SQL							= ConsumptionTerminalsList;
			ReportFilter1->DisplayField				= "Terminal_Name";
			ReportFilter1->SelectionField				= "Terminal_Name";
			ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the tables you wish to appear in the report.";
//			ReportFilter2->ShowGST						= true;
//			ReportFilter2->GSTChecked					= true;
			ReportFilter2->GetSQL						= &TfrmReports::GetChronologicalTableFilter;
			ReportFilter2->DisplayField				= "Table_Number";
			ReportFilter2->SelectionField				= "Table_Number";
			ReportFilter2->SelectionDateRange		= true;
			ReportFilter2->AddPreviousFilters(ReportFilter1);
			SubReport0->AddFilterIndex(2);

			ReportControl->AddFilter(ReportFilter2);

			TReportCheckboxFilter *ReportFilter3	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter3->Caption						= "Select the tabs you wish to appear in the report.";
//			ReportFilter3->ShowGST						= true;
//			ReportFilter3->GSTChecked					= true;
			ReportFilter3->GetSQL						= &TfrmReports::GetChronologicalTabFilter;
			ReportFilter3->DisplayField				= "Tab_Name";
			ReportFilter3->SelectionField				= "Tab_Name";
			ReportFilter3->SelectionDateRange		= true;
			ReportFilter3->AddPreviousFilters(ReportFilter1);
			ReportFilter3->AddPreviousFilters(ReportFilter2);
			SubReport0->AddFilterIndex(3);

			ReportControl->AddFilter(ReportFilter3);
			break;
		}
		case BILLPAYMENTS_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintBillPayments;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Bill Payments By Receipt");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Bill Payment (Single Receipt Number)");
//			TSubReport *SubReport2						= ReportControl->AddSubReport("Bill Payments By Tab");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the date range for the bill payments report.";
			SubReport0->AddFilterIndex(0);
//			SubReport2->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the terminals you wish to appear in the report.";
			ReportFilter1->SQL							= BillTerminalsList;
			ReportFilter1->DisplayField				= "Terminal_Name";
			ReportFilter1->SelectionField				= "Terminal_Name";
			ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the receipts you wish to appear in the report.";
			ReportFilter2->GetSQL						= &TfrmReports::GetBillPaymentsReceiptFilter;
			ReportFilter2->DisplayField				= "Invoice_Number";
			ReportFilter2->SelectionField				= "Invoice_Number";
			ReportFilter2->SelectionDateRange		= true;
			ReportFilter2->AddPreviousFilters(ReportFilter1);
			SubReport0->AddFilterIndex(2);

			ReportControl->AddFilter(ReportFilter2);

			TReportStringFilter *ReportFilter3		= new TReportStringFilter(ReportControl, MMFilterTransaction);

			ReportFilter3->Caption						= "Select the receipt you wish to appear in the report.";
			ReportFilter3->SelectionField				= "Invoice_Number";
			ReportFilter3->SelectionDateRange		= false;
			SubReport1->AddFilterIndex(3);

			ReportControl->AddFilter(ReportFilter3);
      		break;
		}
		case BILLTENDERS_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintBillTenders;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Bill Tenders");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the bill tenders report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Terminals
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the terminals you wish to appear in the report.";
			ReportFilter1->SQL							= BillTerminalsList;
			ReportFilter1->DisplayField				= "Terminal_Name";
			ReportFilter1->SelectionField				= "Terminal_Name";
			ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

	 		// Tenders
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the tenders you wish to appear in the report.";
			ReportFilter2->GetSQL						= &TfrmReports::GetBillTendersFilter;
			ReportFilter2->DisplayField				= "Pay_Type";
			ReportFilter2->SelectionField				= "Pay_Type";
			ReportFilter2->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(2);
			ReportControl->AddFilter(ReportFilter2);

			break;
		}
        case INVOICE_INDEX:
        {

            requiredPermission = Security::SecurityReports;

            ReportControl                                   = new TReportControl;
            ReportControl->PrintReport					= &TfrmReports::PrintInvoice;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Account Charges (Summary)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Account Charges (Detailed)");

            // Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the Account Charges report.";
			SubReport1->AddFilterIndex(0);
            SubReport2->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

            // Groups
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the groups you wish to appear in the report.";
			ReportFilter1->SQL							= InvoiceList;
			ReportFilter1->DisplayField				    = "Name";
			ReportFilter1->SelectionField				= "Name";
			ReportFilter1->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);
            SubReport2->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

            // Members
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
            ReportFilter2->Caption						= "Select the members you wish to appear in the report.";
			ReportFilter2->GetSQL						= &TfrmReports::GetInvoiceFilter;
			ReportFilter2->DisplayField				= "Contactname";
			ReportFilter2->SelectionField				= "Contactname";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(2);
            SubReport2->AddFilterIndex(2);
			ReportControl->AddFilter(ReportFilter2);


            break;
        }
        case FLOATAUDIT_INDEX:
        {

   			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintSkimming;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Skimming");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the Skimming report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

	    	break;

        }
		case BILLDETAILS_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintBillDetails;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Bill Details");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Bill Details (Enter Receipt Number)");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the date range for the bill details report.";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the terminals you wish to appear in the report.";
			ReportFilter1->SQL							= BillTerminalsList;
			ReportFilter1->DisplayField				= "Terminal_Name";
			ReportFilter1->SelectionField				= "Terminal_Name";
			ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter1);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the receipts ( only those after a Z report ) you wish to appear in the report.";
			ReportFilter2->GetSQL						= &TfrmReports::GetBillDetailsReceiptFilter;
			ReportFilter2->DisplayField				= "Invoice_Number";
			ReportFilter2->SelectionField				= "Invoice_Number";
			ReportFilter2->SelectionDateRange		= true;
			ReportFilter2->AddPreviousFilters(ReportFilter1);
			SubReport0->AddFilterIndex(2);

			ReportControl->AddFilter(ReportFilter2);

			TReportStringFilter *ReportFilter3		= new TReportStringFilter(ReportControl, MMFilterTransaction);

			ReportFilter3->Caption						= "Select the receipt ( only if after a Z report ) you wish to appear in the report.";
			ReportFilter3->SelectionField				= "Invoice_Number";
			ReportFilter3->SelectionDateRange		= false;
			SubReport1->AddFilterIndex(3);

			ReportControl->AddFilter(ReportFilter3);
			break;
		}
		case TAB_SUMMARY_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintTabSummary;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Current Tab Items");

			TReportCheckboxFilter *ReportFilter0	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the tab types you wish to appear in the report.";
			ReportFilter0->DisplayList->Add("Tabs");			ReportFilter0->SelectionList->Add("0");
			ReportFilter0->DisplayList->Add("Staff Tabs");	ReportFilter0->SelectionList->Add("1");
			ReportFilter0->DisplayList->Add("Member Tabs");	ReportFilter0->SelectionList->Add("2");
            ReportFilter0->DisplayList->Add("Clipp Tabs");	ReportFilter0->SelectionList->Add("13");

			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the tabs you wish to appear in the report.";
			ReportFilter1->GetSQL						= &TfrmReports::GetCurrentTabList;
			//ReportFilter1->SQL							= CurrentTabList;
			ReportFilter1->DisplayField				= "Tab_Name";
			ReportFilter1->SelectionField				= "Tab_Name";
			ReportFilter1->SelectionDateRange		= false;
			SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);
			break;
		}
		case TAB_DETAILS_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintTabDetails;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Tab Details");

			TReportCheckboxFilter *ReportFilter0	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the tab types you wish to appear in the report.";
			ReportFilter0->DisplayList->Add("Tabs");			ReportFilter0->SelectionList->Add("0");
			ReportFilter0->DisplayList->Add("Staff Tabs");	ReportFilter0->SelectionList->Add("1");
			ReportFilter0->DisplayList->Add("Member Tabs");	ReportFilter0->SelectionList->Add("2");
            ReportFilter0->DisplayList->Add("Clipp Tabs");	ReportFilter0->SelectionList->Add("13");
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the tabs you wish to appear in the report.";
			ReportFilter1->GetSQL   					= &TfrmReports::GetDetailTabList;
			ReportFilter1->DisplayField				= "Tab_Name";
			ReportFilter1->SelectionField				= "Tab_Name";
			ReportFilter1->SelectionDateRange		= false;
			SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			break;
		}
		case DISCOUNTS_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintDiscounts;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Discounted Bills");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Discounted Orders Chronological");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Discounted Orders Summary");
			TSubReport *SubReport3						= ReportControl->AddSubReport("Discounts/Surcharges Totals"); //SubReport3 is for new report to show discounts and surcharges totals

			// dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the discounts report.";
			SubReport0->AddFilterIndex(0);
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);
         SubReport3->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Locations
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the Locations you wish to appear in the report.";
			ReportFilter1->SQL							= SelectLocationList;
			ReportFilter1->DisplayField				= "Order_Location";
			ReportFilter1->SelectionField				= "Order_Location";
			SubReport0->AddFilterIndex(1);
			SubReport1->AddFilterIndex(1);
			SubReport2->AddFilterIndex(1);
         SubReport3->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			// Discounts
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the Discounts you wish to appear in the report.";
			//ReportFilter2->SQL							= DiscountsList;
			ReportFilter2->GetSQL					  	= &TfrmReports::GetDiscountsFilter;
			ReportFilter2->DisplayField				= "From_Val";
			ReportFilter2->SelectionField				= "From_Val";
			ReportFilter2->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(2);
			//SubReport1->AddFilterIndex(2);
			SubReport2->AddFilterIndex(2);
			ReportControl->AddFilter(ReportFilter2);

         // Discounts and surcharges ( this filter was created to show only the names of discounts and surcharges.
         // when there are more than one discount applied to items, the above filter combines them into one. This is not what we need in this case
         TReportCheckboxFilter *discSurchNamesFilter  = new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
         discSurchNamesFilter->Caption                = "Select the Discounts/Surcharges you wish to appear in the report";
         discSurchNamesFilter->GetSQL                 = &TfrmReports::GetDiscountsSurchargesNamesFilter;
         discSurchNamesFilter->DisplayField           = "Name";
         discSurchNamesFilter->SelectionField         = "Name";
         discSurchNamesFilter->SelectionDateRange     = true;
         SubReport3->AddFilterIndex(3);
         ReportControl->AddFilter(discSurchNamesFilter);      // this filter will be set at position 4.

			break;
		}
		case CANCELS_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintCancels;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Cancels (Chronological)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Cancels (Entire Bill)");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the date range for the cancels report.";
			SubReport0->AddFilterIndex(0);
			SubReport1->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the staff you wish to appear in the report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			ReportFilter1->SQL							= UsersList;
			ReportFilter1->DisplayField				= "Name";
			ReportFilter1->SelectionField				= "Name";
			ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter1);

			break;
		}
		case CREDITS_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintCredits;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Credits");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the refunds report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);
			// Locations
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the Locations you wish to appear in the report.";
			ReportFilter1->SQL							= SelectLocationList;
			ReportFilter1->DisplayField				= "Order_Location";
			ReportFilter1->SelectionField				= "Order_Location";
			SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			break;
		}
		case PRICE_ADJUST_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintPriceAdjustments;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Price Adjustments");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the price adjustments report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Locations
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the Locations you wish to appear in the report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			ReportFilter1->SQL							= SelectLocationList;
			ReportFilter1->DisplayField				= "Order_Location";
			ReportFilter1->SelectionField				= "Order_Location";
			//ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			// Staff
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the staff you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->GetSQL					  	= &TfrmReports::GetPriceAdjustmentsStaffFilter;
			ReportFilter2->DisplayField				= "Name";
			ReportFilter2->SelectionField				= "Name";
			ReportFilter2->SelectionDateRange		= true;
			ReportFilter2->AddPreviousFilters(ReportFilter1);
			SubReport0->AddFilterIndex(2);
			ReportControl->AddFilter(ReportFilter2);

			// Receipt Filter
			TReportCheckboxFilter *ReportFilter3	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter3->Caption						= "Select the receipts you wish to appear in the report.";
			ReportFilter3->GetSQL						= &TfrmReports::GetPriceAdjustmentsReceiptFilter;
			ReportFilter3->DisplayField				= "Invoice_Number";
			ReportFilter3->SelectionField				= "Invoice_Number";
			ReportFilter3->SelectionDateRange		= true;
			ReportFilter3->AddPreviousFilters(ReportFilter2);
			SubReport0->AddFilterIndex(3);
			ReportControl->AddFilter(ReportFilter3);



			break;
		}
		case TRANSFERS_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintTransfers;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Transfers");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the date range for the transfer report.";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			break;
		}
		case CASH_DRAWER_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintManualCashDrawer;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Manual Cash Drawer");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the date range for the Manual Cash Drawer report.";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the users you wish to appear in the report.";
			ReportFilter1->SQL							= CashDrawerUsersList;
			ReportFilter1->DisplayField				= "Name";
			ReportFilter1->SelectionField				= "Name";
			ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter1);

			break;
		}
		case HAPPY_HOUR_INDEX:
		{
			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintHappyHour;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Happy Hour (All)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Happy Hour (Members)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Happy Hour (Non Members)");

			// Date Selection
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the Happy Hour report.";
			SubReport0->AddFilterIndex(0);
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Locations
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter1->SQL							= SelectLocationList;
			ReportFilter1->DisplayField				= "Order_Location";
			ReportFilter1->SelectionField				= "Order_Location";
			//ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);
			SubReport1->AddFilterIndex(1);
			SubReport2->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			break;
		}
		case LOYALTY_DETAILS_INDEX:
		{
			requiredPermission = Security::LoyaltyReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintLoyaltyCustomerDetails;
			//TSubReport *SubReport0						= ReportControl->AddSubReport("Loyalty Customer Details");

			TSubReport *SubReport0						= ReportControl->AddSubReport("All Customers' Details");
			TSubReport *SubReport1						= ReportControl->AddSubReport("New Customers' Details");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Altered Customers' Details");

			// Date Range
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the date range for the new customers.";
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Customers
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the customers you wish to appear in the report.";
			ReportFilter1->SQL							= LoyaltyCustomerList;
			ReportFilter1->DisplayField				= "Name";
			ReportFilter1->SelectionField				= "Name";
			ReportFilter1->SelectionDateRange		= false;
			SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			break;
		}
		case LOYALTY_POINTS_INDEX:
		{
			requiredPermission = Security::LoyaltyReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintPointsEarned;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Points Balance");

			TReportCheckboxFilter *ReportFilter0	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the customers you wish to appear in the report.";
			ReportFilter0->SQL							= LoyaltyCustomerList;
			ReportFilter0->DisplayField				= "Name";
			ReportFilter0->SelectionField				= "Name";
			ReportFilter0->SelectionDateRange		= false;
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			break;
		}
		case LOYALTY_HISTORY_INDEX:
		{
			requiredPermission = Security::LoyaltyReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintLoyaltyHistory;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Loyalty History (By Customer)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Loyalty History (By Location)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Loyalty History (Dates)");

			// Date Range
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the loyalty report.";
			SubReport0->AddFilterIndex(0);
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Locations
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter1->SQL							= ConsumptionLocationList;
			ReportFilter1->DisplayField				= "Order_Location";
			ReportFilter1->SelectionField				= "Order_Location";
			ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);
			SubReport1->AddFilterIndex(1);
			SubReport2->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			// Customers
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the customers you wish to appear in the report.";
			ReportFilter2->SQL							= LoyaltyArchiveList;
			ReportFilter2->DisplayField				= "Name";
			ReportFilter2->SelectionField				= "Name";
			ReportFilter2->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(2);
			ReportControl->AddFilter(ReportFilter2);

			break;
		}
		case LOYALTY_BIRTHDAYS_INDEX:
		{
			requiredPermission = Security::LoyaltyReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintLoyaltyBirthdays;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Points Earned");

			TReportMonthFilter *ReportFilter0		= new TReportMonthFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the months you wish to appear in the report.";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			break;
		}

	  case LOYALTY_DISC_PRODUCTS_INDEX:      // cww
		{
			requiredPermission = Security::LoyaltyReports;

			int FilterIndex;
			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintLoyaltyDiscountedProducts;

			TSubReport *SubReport0						= ReportControl->AddSubReport("Disc Products");

			// dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the loyalty report.";
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);
			SubReport0->AddFilterIndex(FilterIndex);

			// Discounts Location and Member
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the Discounts you wish to appear in the report.";
			ReportFilter1->SQL							= LoyaltyDiscountsList;
			ReportFilter1->DisplayField				= "Security_Event";
			ReportFilter1->SelectionField				= "Security_Event";
			ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			// Customers - Members
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the customers you wish to appear in the report.";
			ReportFilter2->SQL							= LoyaltyCustomerList;
			ReportFilter2->DisplayField				= "Name";
			ReportFilter2->SelectionField				= "Name";
			ReportFilter2->SelectionDateRange		= false;
			FilterIndex = ReportControl->AddFilter(ReportFilter2);
			SubReport0->AddFilterIndex(FilterIndex);
			break;
		}

                      case LOYALTY_MEMBERSHIP_AUDIT_INDEX:      // cww
		{
			requiredPermission = Security::LoyaltyReports;
			int FilterIndex;
			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintMembershipAudit;

			TSubReport *SubReport0						= ReportControl->AddSubReport("Membership Points Audit");


            	// Date Range
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the loyalty report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);


            	// Customers - Members
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the customers you wish to appear in the report.";
			ReportFilter1->SQL							= LoyaltyCustomerList;
			ReportFilter1->DisplayField				= "Name";
			ReportFilter1->SelectionField				= "Name";
			ReportFilter1->SelectionDateRange		= false;
			SubReport0->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter1);


			break;
		}
/* TODO          case LOYALTY_EXTENDED_MEMBERSHIP_AUDIT_INDEX:
                {
                        requiredPermission = Security::LoyaltyReports;
                        union {
                                TReportDateFilter *DateFilter;
                                TReportCheckboxFilter *CheckboxFilter;
                        } ReusablePointers;
                        ReportControl = new TReportControl;
                        TSubReport *SubReport = ReportControl->AddSubReport("Membership Points Audit");

                        ReportControl->PrintReport =
                            &TfrmReports::PrintExtendedMembershipAudit;

                        __DateFilter =
                            new TReportDateFilter(ReportControl, MMFilterTransaction);
                        __DateFilter->Caption = "Select the dates you'd like to audit between.";
                        SubReport->AddFilterIndex(0);
                        ReportControl->AddFilter(__DateFilter);

                        __CheckboxFilter =
                            new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
                        __CheckboxFilter->Caption = "Select the members you'd like to audit.";
                        __CheckboxFilter->SQL =
                            "select Contacts_key, Name from Contacts";
                        __CheckboxFilter->SelectionField = "Contacts_key";
                        __CheckboxFilter->DisplayField = "Name";

                        SubReport->AddFilterIndex(1);
                        ReportControl->AddFilter(__CheckboxFilter);

                        break;
                } */
#undef                  DateFilter
#undef                  CheckboxFilter
	  case LOYALTY_SALES_INDEX:      // cww
		{
			requiredPermission = Security::LoyaltyReports;

			int FilterIndex;
			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintLoyaltySales;

			TSubReport *SubReport0						= ReportControl->AddSubReport("Loyalty Sale Products (Summary by Item)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Loyalty Sale Products (Summary by Category)");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Loyalty Sale Products (Detailed by Item)");
			TSubReport *SubReport3						= ReportControl->AddSubReport("Loyalty Sale Products (Detailed by Category)");
			TSubReport *SubReport4						= ReportControl->AddSubReport("Loyalty Sale Products External Membership (Detailed by Item)");
			TSubReport *SubReport5						= ReportControl->AddSubReport("Loyalty Sale Products External Membership (Detailed by Category)");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the loyalty report.";
			SubReport0->AddFilterIndex(0);
			SubReport1->AddFilterIndex(0);
			SubReport2->AddFilterIndex(0);
			SubReport3->AddFilterIndex(0);
			SubReport4->AddFilterIndex(0);
			SubReport5->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Item List by Category
			TReportTreeFilter *ReportFilter1			= new TReportTreeFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter1->SQL							= CategoryTreeList;
			ReportFilter1->DisplayRootField			= "Name";
			ReportFilter1->RootKeyField				= "Name";
			ReportFilter1->DisplayChildField			= "Category";
			ReportFilter1->ChildKeyField				= "Category";
			ReportFilter1->SelectionField				= "Category_Key";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			ReportFilter1->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);
			SubReport3->AddFilterIndex(1);
			SubReport5->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			// Item List by Item Size
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter2->SQL							= LoyaltyItemList;
			ReportFilter2->DisplayField				= "Name";
			ReportFilter2->SelectionField				= "Name";
			ReportFilter2->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(2);
			SubReport2->AddFilterIndex(2);
			SubReport4->AddFilterIndex(2);
			ReportControl->AddFilter(ReportFilter2);

			// Customers - Members
			TReportCheckboxFilter *ReportFilter3	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter3->Caption						= "Select the customers you wish to appear in the report.";
			ReportFilter3->SQL							= LoyaltyCustomerList;
			ReportFilter3->DisplayField				= "Name";
			ReportFilter3->SelectionField				= "Name";
			ReportFilter3->SelectionDateRange		= false;
			SubReport0->AddFilterIndex(3);
			SubReport1->AddFilterIndex(3);
			SubReport2->AddFilterIndex(3);
			SubReport3->AddFilterIndex(3);
			SubReport4->AddFilterIndex(3);
			SubReport5->AddFilterIndex(3);
			ReportControl->AddFilter(ReportFilter3);
			break;
		}
      case RESET_POINTS_INDEX:  //MM 4579
      {
         requiredPermission = Security::LoyaltyReports;
         ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintResetPoint;
         TSubReport *SubReport0						= ReportControl->AddSubReport("All");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Point Expired by Purchased");
         TSubReport *SubReport2						= ReportControl->AddSubReport("Point Expired by Earned");
         TSubReport *SubReport3						= ReportControl->AddSubReport("Point Expired by Redeemed");

			//TSubReport *SubReport0						= ReportControl->AddSubReport("Point Expired Report");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter0->Caption						= "Select the date range for the Reset Point.";
			SubReport0->AddFilterIndex(0);
         SubReport1->AddFilterIndex(0);
         SubReport2->AddFilterIndex(0);
         SubReport3->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);
			break;
      }
		case LOYALTY_PURCHASE_COUNT_INDEX:
		{
			requiredPermission = Security::LoyaltyReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintLoyaltyPurchaseCount;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Loyalty Purchase Count (By Customer)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Loyalty Purchase Count (By Location)");

			// Customers - Members
			TReportCheckboxFilter *ReportFilter0	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the customers you wish to appear in the report.";
			ReportFilter0->SQL							= LoyaltyPurchaseCustomerList;
			ReportFilter0->DisplayField				= "Name";
			ReportFilter0->SelectionField				= "Name";
			ReportFilter0->SelectionDateRange		= false;
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Locations
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the Locations you wish to appear in the report.";
			ReportFilter1->SQL							= SelectLocationList;
			ReportFilter1->DisplayField				= "Order_Location";
			ReportFilter1->SelectionField				= "Order_Location";
			ReportFilter1->SelectionDateRange		= false;
			SubReport1->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			break;
		}
		case LOYALTY_AUDIT_INDEX:
		{
			requiredPermission = Security::LoyaltyReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintLoyaltyAudit;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Loyalty Audit (Summary)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Loyalty Audit");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the loyalty report.";
			SubReport0->AddFilterIndex(0);
			SubReport1->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			// Customers - Members
			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the customers you wish to appear in the report.";
			ReportFilter1->SQL							= LoyaltyCustomerList;
			ReportFilter1->DisplayField				= "Name";
			ReportFilter1->SelectionField				= "Name";
			ReportFilter1->SelectionDateRange		= false;
			SubReport0->AddFilterIndex(1);
			SubReport1->AddFilterIndex(1);
			ReportControl->AddFilter(ReportFilter1);

			// Locations
			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);
			ReportFilter2->Caption						= "Select the Locations you wish to appear in the report.";
			ReportFilter2->SQL							= SelectLocationList;
			ReportFilter2->DisplayField				= "Order_Location";
			ReportFilter2->SelectionField				= "Order_Location";
			ReportFilter2->SelectionDateRange		= false;
			SubReport1->AddFilterIndex(2);
			ReportControl->AddFilter(ReportFilter2);

			break;
		}

          case POINT_SPEND_INDEX:
		{
			requiredPermission = Security::LoyaltyReports;

			ReportControl								= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintPointSpend;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Loyalty PointSpend (Summary)");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the loyalty report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

			TReportTreeFilter *ReportFilter1		= new TReportTreeFilter(ReportControl, MMFilterTransaction);
			ReportFilter1->Caption						= "Select the Categories you wish to appear in the report.";
			ReportFilter1->SQL							= CategoryTreeList;
			ReportFilter1->DisplayRootField			= "Name";
			ReportFilter1->RootKeyField				= "Name";

			ReportFilter1->DisplayChildField			= "Category";
			ReportFilter1->ChildKeyField				= "Category";
			ReportFilter1->SelectionField				= "Category_Key";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			ReportFilter1->SelectionDateRange		= true;
			ReportControl->AddFilter(ReportFilter1);
		   	SubReport0->AddFilterIndex(1);

            //Payment Type Filter
            TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the Payment Types you wish to appear in the report.";
			ReportFilter2->SQL							= PaymentTypeList;
			ReportFilter2->DisplayField				    = "PAY_TYPE";
			ReportFilter2->SelectionField				= "PAY_TYPE";            
			ReportFilter2->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(2);

			ReportControl->AddFilter(ReportFilter2);
           
			break;
		}

		case STOCK_DETAILS_INDEX:
		{
			requiredPermission = Security::StockReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockDetails;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stock Details (By Group)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Stock Details (By Location)");

			TReportTreeFilter *ReportFilter0			= new TReportTreeFilter(ReportControl, StockFilterTransaction);

			ReportFilter0->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter0->SQL							= StockTreeList;
			ReportFilter0->DisplayRootField			= "Stock_Category";
			ReportFilter0->RootKeyField				= "Stock_Category";//"Stock_Category_Key";
			ReportFilter0->DisplayChildField			= "Stock_Group";
			ReportFilter0->ChildKeyField				= "Stock_Group";//"Stock_Group_Key";
			ReportFilter0->SelectionField				= "Stock_Group";//"Stock_Group_Key";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);

			ReportFilter1->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter1->SQL							= StockLocationList;
			ReportFilter1->DisplayField				= "Location";
			ReportFilter1->SelectionField				= "Location";
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter1);

			TReportTreeFilter *ReportFilter2			= new TReportTreeFilter(ReportControl, StockFilterTransaction);

			ReportFilter2->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter2->GetSQL						= &TfrmReports::GetStockLocationItemFilter;
			ReportFilter2->SQL							= StockTreeList;
			ReportFilter2->DisplayRootField			= "Stock_Category";
			ReportFilter2->RootKeyField				= "Stock_Category";//"Stock_Category_Key";
			ReportFilter2->DisplayChildField			= "Stock_Group";
			ReportFilter2->ChildKeyField				= "Stock_Group";//"Stock_Group_Key";
			ReportFilter2->SelectionField				= "Stock_Group";//"Stock_Group_Key";
			ReportFilter2->AddPreviousFilters(ReportFilter1);
			SubReport1->AddFilterIndex(2);

			ReportControl->AddFilter(ReportFilter2);

			break;
		}
		case STOCK_VALUE_INDEX:
		{
			requiredPermission = Security::StockReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockValue;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stock Value (By Group)");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Stock Value (By Location)");

			TReportTreeFilter *ReportFilter0			= new TReportTreeFilter(ReportControl, StockFilterTransaction);

			ReportFilter0->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter0->SQL							= StockTreeList;
			ReportFilter0->DisplayRootField			= "Stock_Category";
			ReportFilter0->RootKeyField				= "Stock_Category";//"Stock_Category_Key";
			ReportFilter0->DisplayChildField			= "Stock_Group";
			ReportFilter0->ChildKeyField				= "Stock_Group";//"Stock_Group_Key";
			ReportFilter0->SelectionField				= "Stock_Group";//"Stock_Group_Key";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);

			ReportFilter1->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter1->SQL							= StockLocationList;
			ReportFilter1->DisplayField				= "Location";
			ReportFilter1->SelectionField				= "Location";
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter1);

			TReportTreeFilter *ReportFilter2			= new TReportTreeFilter(ReportControl, StockFilterTransaction);

			ReportFilter2->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter2->GetSQL						= &TfrmReports::GetStockLocationItemFilter;
			ReportFilter2->SQL							= StockTreeList;
			ReportFilter2->DisplayRootField			= "Stock_Category";
			ReportFilter2->RootKeyField				= "Stock_Category";//"Stock_Category_Key";
			ReportFilter2->DisplayChildField			= "Stock_Group";
			ReportFilter2->ChildKeyField				= "Stock_Group";//"Stock_Group_Key";
			ReportFilter2->SelectionField				= "Stock_Group";//"Stock_Group_Key";
			ReportFilter2->AddPreviousFilters(ReportFilter1);
			SubReport1->AddFilterIndex(2);

			ReportControl->AddFilter(ReportFilter2);

			break;
		}
		case STOCK_BARCODES_INDEX:
		{
			requiredPermission = Security::StockReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockBarcodes;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stock Barcodes");

			TReportTreeFilter *ReportFilter0			= new TReportTreeFilter(ReportControl, StockFilterTransaction);

			ReportFilter0->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter0->SQL							= StockTreeList;
			ReportFilter0->DisplayRootField			= "Stock_Category";
			ReportFilter0->RootKeyField				= "Stock_Category";
			ReportFilter0->DisplayChildField			= "Stock_Group";
			ReportFilter0->ChildKeyField				= "Stock_Group";
			ReportFilter0->SelectionField				= "Stock_Group";
			SubReport0->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter0);

			break;
		}
		case STOCK_MOVEMENT_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockMovement;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Inwards Goods");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Transfers");
			TSubReport *SubReport2						= ReportControl->AddSubReport("Stocktake Adjustments");
			TSubReport *SubReport3						= ReportControl->AddSubReport("Sales");
			TSubReport *SubReport4						= ReportControl->AddSubReport("Write-offs");
			TSubReport *SubReport5						= ReportControl->AddSubReport("Manufacture");
			TSubReport *SubReport6						= ReportControl->AddSubReport("All");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the date range for the Stock Movement report.";
			SubReport0->AddFilterIndex(FilterIndex);
			SubReport1->AddFilterIndex(FilterIndex);
			SubReport2->AddFilterIndex(FilterIndex);
			SubReport3->AddFilterIndex(FilterIndex);
			SubReport4->AddFilterIndex(FilterIndex);
			SubReport5->AddFilterIndex(FilterIndex);
			SubReport6->AddFilterIndex(FilterIndex);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter1);

			ReportFilter1->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter1->SQL							= StockLocationList;
			ReportFilter1->DisplayField				= "Location";
			ReportFilter1->SelectionField				= "Location";
			SubReport0->AddFilterIndex(FilterIndex);
			SubReport1->AddFilterIndex(FilterIndex);
			SubReport2->AddFilterIndex(FilterIndex);
			SubReport3->AddFilterIndex(FilterIndex);
			SubReport4->AddFilterIndex(FilterIndex);
			SubReport5->AddFilterIndex(FilterIndex);
			SubReport6->AddFilterIndex(FilterIndex);

			TReportTreeFilter *ReportFilter2			= new TReportTreeFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter2);

			ReportFilter2->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter2->SQL							= StockTreeList;
			ReportFilter2->DisplayRootField			= "Stock_Category";
			ReportFilter2->RootKeyField				= "Stock_Category";
			ReportFilter2->DisplayChildField			= "Stock_Group";
			ReportFilter2->ChildKeyField				= "Stock_Group";
			ReportFilter2->SelectionField				= "Stock_Group";
			SubReport0->AddFilterIndex(FilterIndex);
			SubReport1->AddFilterIndex(FilterIndex);
			SubReport2->AddFilterIndex(FilterIndex);
			SubReport3->AddFilterIndex(FilterIndex);
			SubReport4->AddFilterIndex(FilterIndex);
			SubReport5->AddFilterIndex(FilterIndex);
			SubReport6->AddFilterIndex(FilterIndex);

			break;
		}
		case STOCK_TRANSFER_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockTransfer;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Location Transfers");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Transfer Details");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the date range for the transfer report.";
			SubReport0->AddFilterIndex(FilterIndex);
			SubReport1->AddFilterIndex(FilterIndex);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter1);

			ReportFilter1->Caption						= "Select the source locations you wish to appear in the report.";
			ReportFilter1->SQL							= StockLocationList;
			ReportFilter1->DisplayField				= "Location";
			ReportFilter1->SelectionField				= "Location";
			SubReport0->AddFilterIndex(FilterIndex);

			TReportTreeFilter *ReportFilter2			= new TReportTreeFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter2);

			ReportFilter2->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter2->SQL							= StockTreeList;
			ReportFilter2->DisplayRootField			= "Stock_Category";
			ReportFilter2->RootKeyField				= "Stock_Category";
			ReportFilter2->DisplayChildField			= "Stock_Group";
			ReportFilter2->ChildKeyField				= "Stock_Group";
			ReportFilter2->SelectionField				= "Stock_Group";
			SubReport0->AddFilterIndex(FilterIndex);

			break;
		}
		case STOCK_INQUIRY_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockInquiry;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stock Inquiry");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the date range for the stock inquiry.";
			SubReport0->AddFilterIndex(FilterIndex);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter1);

			ReportFilter1->Caption						= "Select the source locations you wish to appear in the report.";
			ReportFilter1->SQL							= StockLocationList;
			ReportFilter1->DisplayField				= "Location";
			ReportFilter1->SelectionField				= "Location";
			SubReport0->AddFilterIndex(FilterIndex);

			TReportTreeFilter *ReportFilter2			= new TReportTreeFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter2);

			ReportFilter2->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter2->SQL							= StockTreeList;
			ReportFilter2->DisplayRootField			= "Stock_Category";
			ReportFilter2->RootKeyField				= "Stock_Category";
			ReportFilter2->DisplayChildField			= "Stock_Group";
			ReportFilter2->ChildKeyField				= "Stock_Group";
			ReportFilter2->SelectionField				= "Stock_Group";
			SubReport0->AddFilterIndex(FilterIndex);

			TReportCheckboxFilter *ReportFilter3	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter3);

			ReportFilter3->Caption						= "Select the transaction types you wish to appear in the report.";
			ReportFilter3->DisplayList->Add("Inwards Goods");				ReportFilter3->SelectionList->Add("Receipt");
			ReportFilter3->DisplayList->Add("Transfers");					ReportFilter3->SelectionList->Add("Transfer");
			ReportFilter3->DisplayList->Add("Stocktake Adjustments");	ReportFilter3->SelectionList->Add("Stocktake");
			ReportFilter3->DisplayList->Add("Sales");							ReportFilter3->SelectionList->Add("Sale");
			ReportFilter3->DisplayList->Add("Write-offs");					ReportFilter3->SelectionList->Add("Writeoff");
			ReportFilter3->DisplayList->Add("Manufactures");					ReportFilter3->SelectionList->Add("Manufacture");
			SubReport0->AddFilterIndex(FilterIndex);

			break;
		}
        case STOCK_WRITEOFF_INDEX:
        {
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockWriteOff;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stock Write Off");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the date range for the stock write offs.";
			SubReport0->AddFilterIndex(FilterIndex);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter1);

			ReportFilter1->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter1->SQL							= StockLocationList;
			ReportFilter1->DisplayField				= "location";
	  		ReportFilter1->SelectionField				= "location";
	  //		ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(FilterIndex);

			break;

        }
        case STOCK_LOST_REVENUE_INDEX:
        {

   			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockLostRevenue;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stock Lost Revenue");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the stock lost revenue report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

        	break;

        }
		case STOCK_REORDER_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockReorder;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stock Reorder");

			TReportCheckboxFilter *ReportFilter0	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the source locations you wish to appear in the report.";
			ReportFilter0->SQL							= StockLocationList;
			ReportFilter0->DisplayField				= "Location";
			ReportFilter0->SelectionField				= "Location";
			SubReport0->AddFilterIndex(FilterIndex);

			TReportTreeFilter *ReportFilter1			= new TReportTreeFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter1);

			ReportFilter1->Caption						= "Select the items you wish to appear in the report.";
			ReportFilter1->SQL							= StockTreeList;
			ReportFilter1->DisplayRootField			= "Stock_Category";
			ReportFilter1->RootKeyField				= "Stock_Category";
			ReportFilter1->DisplayChildField			= "Stock_Group";
			ReportFilter1->ChildKeyField				= "Stock_Group";
			ReportFilter1->SelectionField				= "Stock_Group";
			SubReport0->AddFilterIndex(FilterIndex);

			break;
		}
		//case STOCKTAKE_HISTORY_INDEX:
		case STOCKTAKE_VARIANCE_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStocktakeVariance;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stocktake Variance");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the date range for the stocktake.";
			SubReport0->AddFilterIndex(FilterIndex);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter1);

			ReportFilter1->Caption						= "Select the stocktake you wish to appear in the report.";
			ReportFilter1->SQL							= StocktakeHistoryList;
			ReportFilter1->DisplayField				= "Initialised_Time";
			ReportFilter1->SelectionField				= "Stocktake_Key";
			ReportFilter1->SelectionDateRange		= true;
			SubReport0->AddFilterIndex(FilterIndex);

			break;
		}
		case STOCKTAKE_VALUE_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStocktakeValue;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stocktake Value");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, StockFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the stocktake.";
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);
			SubReport0->AddFilterIndex(FilterIndex);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);

			ReportFilter1->Caption						= "Select the stocktake you wish to appear in the report.";
			ReportFilter1->SQL							= StocktakeHistoryList;
			ReportFilter1->DisplayField				= "Initialised_Time";
			ReportFilter1->SelectionField				= "Stocktake_Key";
			ReportFilter1->SelectionDateRange		= true;
			FilterIndex										= ReportControl->AddFilter(ReportFilter1);
			SubReport0->AddFilterIndex(FilterIndex);

			break;
		}
        case STOCK_MANUFACTURE_INDEX:
        {
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockManufacture;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stock Manufacture");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the date range for the stock manufactures.";
			SubReport0->AddFilterIndex(FilterIndex);

			break;
        }
		case SUPPLIER_DETAILS_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintSupplierDetails;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Supplier Details");

			TReportCheckboxFilter *ReportFilter0	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the suppliers you wish to appear in the report.";
			ReportFilter0->SQL							= StockSupplierList;
			ReportFilter0->DisplayField				= "Company_Name";
			ReportFilter0->SelectionField				= "Company_Name";
			SubReport0->AddFilterIndex(FilterIndex);

			break;
		}
		case SUPPLIER_PURCHASES_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintPurchaseInvoices;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Purchases by Invoice Number");
			TSubReport *SubReport1						= ReportControl->AddSubReport("Purchases by Stock Receipt Number");


			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the date range for the invoices.";
			SubReport0->AddFilterIndex(FilterIndex);
            SubReport1->AddFilterIndex(FilterIndex);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter1);

			ReportFilter1->Caption						= "Select the Stock Receipt Number you wish to appear in the report.";
			ReportFilter1->GetSQL						= &TfrmReports::GetStockReceiptList;
			ReportFilter1->DisplayField				= "BATCH_KEY";
			ReportFilter1->SelectionField				= "BATCH_KEY";
			ReportFilter1->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(FilterIndex);

			TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter2);

			ReportFilter2->Caption						= "Select the invoices you wish to appear in the report.";
			ReportFilter2->GetSQL						= &TfrmReports::GetStockInvoiceList;
			ReportFilter2->DisplayField				= "Reference";
			ReportFilter2->SelectionField				= "Reference";
			ReportFilter2->SelectionDateRange		= true;
			ReportFilter2->AddPreviousFilters(ReportFilter1);
			SubReport0->AddFilterIndex(FilterIndex);

			break;
		}
		case SUPPLIER_BACKORDER_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintSupplierBackOrders;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Supplier Backorders");

			TReportCheckboxFilter *ReportFilter0	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the suppliers you wish to appear in the report.";
			ReportFilter0->SQL							= StockSupplierList;
			ReportFilter0->DisplayField				= "Company_Name";
			ReportFilter0->SelectionField				= "Company_Name";
			SubReport0->AddFilterIndex(FilterIndex);

			break;
		}
		case SUPPLIER_REORDER_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintSupplierReorder;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Supplier Reorder");

			TReportCheckboxFilter *ReportFilter0	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the suppliers you wish to appear in the report.";
			ReportFilter0->SQL							= StockSupplierList;
			ReportFilter0->DisplayField				= "Company_Name";
			ReportFilter0->SelectionField				= "Company_Name";
			SubReport0->AddFilterIndex(FilterIndex);

			break;
		}
      case SUPPLIER_INVOICE_INDEX:
        {
			requiredPermission = Security::StockReports;

			ReportControl									= new TReportControl;
	  	ReportControl->PrintReport					= &TfrmReports::PrintSupplierInvoice;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Supplier Invoice");
			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the Supplier Invoice report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);
			break;
        }
//--------------------------
      case CASHIER_INDEX:
		{
		    requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintCashier;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Cashier");

			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the Finance Daily report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;

			SubReport1->AddFilterIndex(0);

			ReportControl->AddFilter(ReportFilter1);

         // Locations
         TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

		   ReportFilter2->Caption						= "Select the locations you wish to appear in the report.";
		   ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
		   ReportFilter2->SQL							= BillLocationList;
         ReportFilter2->DisplayField				= "Billed_Location";
         ReportFilter2->SelectionField				= "Billed_Location";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);
          //Terminal Filter
         TReportCheckboxFilter *ReportFilter3	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter3->Caption						= "Select the Terminal you wish to appear in the report.";
			ReportFilter3->GetSQL						= &TfrmReports::GetTerminalFilter;
			ReportFilter3->DisplayField				= "Terminal_Name";
			ReportFilter3->SelectionField				= "Terminal_Name";
			ReportFilter3->SelectionDateRange		= true;
			ReportFilter3->AddPreviousFilters(ReportFilter2);
			SubReport1->AddFilterIndex(2);

			ReportControl->AddFilter(ReportFilter3);


			break;
		}
//-------------------------------
      case DEPT_SALES_SUMMARY:
		{
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintDeptSalesSummary;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Sales Summary by department");


			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the sales summary report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);
		 

			ReportControl->AddFilter(ReportFilter1);

         TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							= BillLocationList;
			ReportFilter2->DisplayField				= "Billed_Location";
			ReportFilter2->SelectionField				= "Billed_Location";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);

			break;
		}

        case CHECK_REMOVAL_INDEX:    //MM-4327
        {

   			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintCheckRemoval;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Check Removal");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the Check Removal report.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

        	break;

        }

        case REPRINT_RECEIPT_INDEX:
        {

   			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintReprintReceipt;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Reprint Receipt");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the Reprint Receipt.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

        	break;

        }
         case REPRINT_ORDER_INDEX:
        {

   			requiredPermission = Security::SecurityReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintReprintOrder;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Reprint Receipt");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the Reprint Receipt.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);

        	break;
        }
        case PROFIT_LOSS_REPORT:
		{
			requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintProfitLossSummary;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Sales Summary by department");


			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the profit/loss report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
			SubReport1->AddFilterIndex(0);
		 

			ReportControl->AddFilter(ReportFilter1);

            TReportCheckboxFilter *ReportFilter2	= new TReportCheckboxFilter(ReportControl, MMFilterTransaction);

			ReportFilter2->Caption						= "Select the locations you wish to appear in the report.";
			ReportFilter2->ShowGST						= false;
			ReportFilter2->GSTChecked					= false;
			ReportFilter2->SQL							= BillLocationList;
			ReportFilter2->DisplayField				    = "Billed_Location";
			ReportFilter2->SelectionField				= "Billed_Location";
			ReportFilter2->SelectionDateRange		= true;
			SubReport1->AddFilterIndex(1);

			ReportControl->AddFilter(ReportFilter2);

			break;
		}

       case STOCK_RECONCIALATION_INDEX:
		{
			requiredPermission = Security::StockReports;

			int FilterIndex;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintStockReconcialation;
			TSubReport *SubReport0						= ReportControl->AddSubReport("Stock Reconcilation");

			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter0);

			ReportFilter0->Caption						= "Select the date range for the stock Reconcilation.";
			SubReport0->AddFilterIndex(FilterIndex);

			TReportCheckboxFilter *ReportFilter1	= new TReportCheckboxFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter1);

			ReportFilter1->Caption						= "Select the source locations you wish to appear in the report.";
			ReportFilter1->SQL							= StockLocationList;
			ReportFilter1->DisplayField				= "Location";
			ReportFilter1->SelectionField				= "Location";
			SubReport0->AddFilterIndex(FilterIndex);

			TReportTreeFilter *ReportFilter2			= new TReportTreeFilter(ReportControl, StockFilterTransaction);
			FilterIndex										= ReportControl->AddFilter(ReportFilter2);

			ReportFilter2->Caption						= "Select the Stock Group you wish to appear in the report.";
			ReportFilter2->SQL							= StockCategoryFilterOnTimeBasis;
			ReportFilter2->DisplayRootField			= "Stock_Category";
			ReportFilter2->RootKeyField				= "Stock_Category";
			ReportFilter2->DisplayChildField			= "Stock_Group";
			ReportFilter2->ChildKeyField				= "Stock_Group";
			ReportFilter2->SelectionField				= "Stock_Group";
            ReportFilter2->SelectionDateRange		    = true;
			SubReport0->AddFilterIndex(FilterIndex);
        	break;
 		}
        case SALES_SUMMARY_D_INDEX:
        {
            requiredPermission = Security::FinancialReports;

			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintSalesSummaryD;
			TSubReport *SubReport1						= ReportControl->AddSubReport("Sales Summary D");

			TReportDateFilter *ReportFilter1			= new TReportDateFilter(ReportControl, MMFilterTransaction);

			ReportFilter1->Caption						= "Select the date range for the Sales Summary D report.";
			ReportFilter1->ShowGST						= false;
			ReportFilter1->GSTChecked					= false;
  			SubReport1->AddFilterIndex(0);
        	ReportControl->AddFilter(ReportFilter1);

			break;
        }
        case E_JOURNAL_INDEX:
        {
            isEJournalReport = true;

   			requiredPermission = Security::SecurityReports;
			ReportControl									= new TReportControl;
			ReportControl->PrintReport					= &TfrmReports::PrintEJournalReport;
			TSubReport *SubReport0						= ReportControl->AddSubReport("E-Journal Report");

			// Dates
			TReportDateFilter *ReportFilter0			= new TReportDateFilter(ReportControl, MMFilterTransaction);
			ReportFilter0->Caption						= "Select the date range for the Reprint Receipt.";
			SubReport0->AddFilterIndex(0);
			ReportControl->AddFilter(ReportFilter0);
        	break;
        }
}
 	if (ReportControl)
	{
		if (requiredPermission != Security::None)
		{
			bool hasPermission = Security::AuthenticationService().HasPermission(
				frmLogin->CurrentUser.UserID,
				requiredPermission);

			if (hasPermission == false)
			{
				Application->MessageBox("You are not authorized for this report", "Error", MB_ICONERROR + MB_OK);

				TreeView1->Select(Node->Parent);
				pcReportProperties->ActivePage = tsFolderDescription;
				return;
			}
		}

		ShowCurrentFilter(isEJournalReport);
	}
}
//---------------------------------------------------------------------------
void TfrmReports::ShowCurrentFilter(bool isEJournalReport)
{
	if (ReportControl)
	{
		if (ReportControl->CurrentSubReport == -1)
		{
			if (ReportControl->SubReportCount() != 1)
			{
				rgReports->Items->Clear();
				pcReportProperties->ActivePage					= tsReportType;
				for (int i=0; i<ReportControl->SubReportCount(); i++)
				{
					rgReports->Items->Add(ReportControl->SubReport(i)->Caption);
				}
				return;
			}
			else
			{
				ReportControl->CurrentSubReport	= 0;
				TSubReport *SubReport				= ReportControl->SubReport(ReportControl->CurrentSubReport);
				SubReport->FirstFilter();
			}
		}
		TSubReport *SubReport			= ReportControl->SubReport(ReportControl->CurrentSubReport);
		TReportFilter *CurrentFilter	= ReportControl->ReportFilter(SubReport->CurrentFilter());
		bool AllowNext						= SubReport->AllowNextFilter();
		bool AllowPrev						= SubReport->AllowPrevFilter() || ReportControl->SubReportCount() > 1;
		TCursor Save_Cursor = Screen->Cursor;
		Screen->Cursor = crHourGlass;
		try
		{
			switch (CurrentFilter->ReportFilterType)
			{
				case Reports::ftDate:
				{
					TReportDateFilter *ReportDateFilter				= (TReportDateFilter *)CurrentFilter;
					pcReportProperties->ActivePage					= tsDateTime;
					lbeDateTitle->Caption								= ReportDateFilter->Caption;
					btnDatePreview->Visible								= !AllowNext;
					btnDateExcel->Visible								= !AllowNext;
					btnDateNext->Visible									= AllowNext;
					chbDateGST->Visible									= ReportDateFilter->ShowGST;
					chbDateGST->Checked									= ReportDateFilter->GSTChecked;
					break;
				}
				case Reports::ftCheckBox:
				{
					TReportCheckboxFilter *ReportCheckboxFilter	= (TReportCheckboxFilter *)CurrentFilter;
					pcReportProperties->ActivePage					= tsCheckFilter;
					lbeCheckFilterTitle->Caption						= ReportCheckboxFilter->Caption;
					btnCheckFilterBack->Visible						= AllowPrev;
					btnCheckFilterPreview->Visible					= !AllowNext;
					btnCheckFilterExcel->Visible				    	= !AllowNext;
					btnCheckFilterNext->Visible						= AllowNext;
					chbCheckFilterGST->Visible							= ReportCheckboxFilter->ShowGST;
					chbCheckFilterGST->Checked							= ReportCheckboxFilter->GSTChecked;
					lbeCheckFilterRange->Visible						= ReportCheckboxFilter->SelectionDateRange;
					rbFilterAll->Checked									= true;

					if (ReportCheckboxFilter->GetSQL)
					{
						(this->*(ReportCheckboxFilter->GetSQL))(ReportCheckboxFilter);
					}
					if (ReportCheckboxFilter->SQL != "")
					{
						try
						{
							Screen->Cursor = crHourGlass;
							ReportCheckboxFilter->Refresh();
						}
						__finally
						{
							Screen->Cursor = crDefault;
						}

					}
					chlbFilterList->Items->Clear();
					chlbFilterList->Items->Assign(ReportCheckboxFilter->DisplayList);
					break;
				}
				case Reports::ftMonthCheckBox:
				{
					TReportMonthFilter *ReportFilter					= (TReportMonthFilter *)CurrentFilter;
					pcReportProperties->ActivePage					= tsCheckFilter;
					lbeCheckFilterTitle->Caption						= ReportFilter->Caption;
					btnCheckFilterBack->Visible						= AllowPrev;
					btnCheckFilterPreview->Visible					= !AllowNext;
					btnCheckFilterExcel->Visible				   	= !AllowNext;
					btnCheckFilterNext->Visible						= AllowNext;
					chbCheckFilterGST->Visible							= ReportFilter->ShowGST;
					chbCheckFilterGST->Checked							= ReportFilter->GSTChecked;
					lbeCheckFilterRange->Visible						= ReportFilter->SelectionDateRange;
					rbFilterAll->Checked									= true;

					ReportFilter->Refresh();
					chlbFilterList->Items->Clear();
					chlbFilterList->Items->Assign(ReportFilter->DisplayList);
					break;
				}
				case Reports::ftTree:
				{
					TReportTreeFilter *ReportTreeFilter				= (TReportTreeFilter *)CurrentFilter;
					pcReportProperties->ActivePage					= tsTreeFilter;
					lbeTreeFilterTitle->Caption						= ReportTreeFilter->Caption;
					btnTreeFilterBack->Visible							= AllowPrev;
					btnTreeFilterPreview->Visible						= !AllowNext;
					btnTreeFilterExcel->Visible						= !AllowNext;
					btnTreeFilterNext->Visible							= AllowNext;
					chbTreeFilterGST->Visible							= ReportTreeFilter->ShowGST;
					chbTreeFilterGST->Checked							= ReportTreeFilter->GSTChecked;
					lbeTreeFilterRange->Visible						= ReportTreeFilter->SelectionDateRange;
					rbTreeAll->Checked									= true;

					if (ReportTreeFilter->GetSQL)
					{
						(this->*(ReportTreeFilter->GetSQL))(ReportTreeFilter);
					}
					ReportTreeFilter->Refresh();
					tvFilterTree->Items->Clear();
					for (int i=0; i<ReportTreeFilter->NodeCount(); i++)
					{
						TFilterTreeNode *Root			= ReportTreeFilter->Node(i);
						TTreeNode *TreeNode				= tvFilterTree->Items->Add(NULL, Root->Caption);
						TreeNode->ImageIndex				= ICON_CLOSED_FOLDER_INDEX;
						TreeNode->SelectedIndex			= ICON_OPEN_FOLDER_INDEX;
						TreeNode->StateIndex				= STATE_IMAGE_SMALL_UNCHECKED;

						for (int j=0; j<Root->NodeCount(); j++)
						{
							TTreeNode *ChildNode			= tvFilterTree->Items->AddChild(TreeNode, Root->Node(j)->Caption);
							ChildNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
							ChildNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
							ChildNode->StateIndex		= STATE_IMAGE_SMALL_UNCHECKED;
						}
					}
					break;
				}
	/*			case Reports::ftNumber:
				{
					TReportNumberFilter *ReportNumberFilter		= (TReportNumberFilter *)CurrentFilter;
					pcReportProperties->ActivePage					= tsNumber;
					lbeNumberTitle->Caption								= ReportNumberFilter->Caption;
					lbeNumberCaption->Caption							= ReportNumberFilter->Caption;
					btnNumberBack->Visible								= AllowPrev;
					btnNumberNext->Visible								= AllowNext;
					btnNumberPreview->Visible							= !AllowNext;
					chbNumberGST->Visible								= ReportNumberFilter->ShowGST;
					chbNumberGST->Checked								= ReportNumberFilter->GSTChecked;
					break;
				}*/
				case Reports::ftString:
				{
					TReportStringFilter *ReportStringFilter		= (TReportStringFilter *)CurrentFilter;
					pcReportProperties->ActivePage					= tsString;
					lbeStringTitle->Caption								= ReportStringFilter->Caption;
					lbeStringCaption->Caption							= ReportStringFilter->Caption;
					btnStringBack->Visible								= AllowPrev;
					btnStringNext->Visible								= AllowNext;
					btnStringPreview->Visible							= !AllowNext;
					btnStringExcel->Visible	   						= !AllowNext;
					chbStringGST->Visible								= ReportStringFilter->ShowGST;
					chbStringGST->Checked								= ReportStringFilter->GSTChecked;
					break;
				}
			}
			if(isEJournalReport)
			{
			   btnDateExcel->Visible = false;  
               //btnStringBack->Visible	= false;
               //btnTreeFilterBack->Visible = false;
               btnBackFromDate->Visible = false;
			}
		}
		__finally
		{
			Screen->Cursor = Save_Cursor;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnTypeNextClick(TObject *Sender)
{
	if (ReportControl)
	{
		if (ReportControl->CurrentSubReport > -1)
		{
			ShowCurrentFilter();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::rgReportsClick(TObject *Sender)
{
	if (ReportControl)
	{
		ReportControl->CurrentSubReport	= rgReports->ItemIndex;
		TSubReport *SubReport				= ReportControl->SubReport(ReportControl->CurrentSubReport);
		if (SubReport->FilterCount() == 0)
		{
			btnTypeNext->Visible			= false;
			btnTypePreview->Visible		= true;
		}
		else
		{
			btnTypeNext->Visible			= true;
			btnTypePreview->Visible		= false;
			SubReport->FirstFilter();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnFilterNextClick(TObject *Sender)
{
	if (ReportControl)
	{
		SaveCurrentFilter();

		TSubReport *SubReport = ReportControl->SubReport(ReportControl->CurrentSubReport);
		SubReport->NextFilter();
		ShowCurrentFilter();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnTreeFilterNextClick(TObject *Sender)
{
	if (ReportControl)
	{
		SaveCurrentFilter();

		TSubReport *SubReport = ReportControl->SubReport(ReportControl->CurrentSubReport);
		SubReport->NextFilter();
		ShowCurrentFilter();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnBackFromFilterClick(TObject *Sender)
{
	if (ReportControl)
	{
		TSubReport *SubReport = ReportControl->SubReport(ReportControl->CurrentSubReport);
		if (SubReport->AllowPrevFilter())
		{
			SubReport->PrevFilter();
		}
		else
		{
			ReportControl->CurrentSubReport = -1;
		}
		ShowCurrentFilter();
	}
}
//---------------------------------------------------------------------------
void TfrmReports::SaveCurrentFilter()
{
	if (ReportControl)
	{
		TSubReport *SubReport = ReportControl->SubReport(ReportControl->CurrentSubReport);
		if (SubReport->FilterCount() > 0)
		{
			TReportFilter *CurrentFilter = ReportControl->ReportFilter(SubReport->CurrentFilter());
			switch (CurrentFilter->ReportFilterType)
			{
				case Reports::ftDate:
				{
	//				TReportDateFilter *ReportDateFilter				= (TReportDateFilter *)CurrentFilter;
					ShowDateTimes();
					ReportControl->Start									= TDateTime(int(mcStartDate->Date) + StartTime);
					ReportControl->End									= TDateTime(int(mcEndDate->Date) + EndTime);
	//				ReportDateFilter->GSTChecked						= chbCheckFilterGST->Checked;
					break;
				}
				case Reports::ftCheckBox:
				{
					TReportCheckboxFilter *ReportCheckboxFilter	= (TReportCheckboxFilter *)CurrentFilter;
					ReportCheckboxFilter->Selection->Clear();
					for (int i=0; i<chlbFilterList->Items->Count; i++)
					{
						if (chlbFilterList->Checked[i])
						{
							ReportCheckboxFilter->Selection->Add(ReportCheckboxFilter->SelectionList->Strings[i]);
						}
					}
					ReportCheckboxFilter->GSTChecked					= chbCheckFilterGST->Checked;
					break;
				}
				case Reports::ftMonthCheckBox:
				{
					TReportMonthFilter *ReportMonthFilter			= (TReportMonthFilter *)CurrentFilter;
					ReportMonthFilter->Selection->Clear();
					for (int i=0; i<chlbFilterList->Items->Count; i++)
					{
						if (chlbFilterList->Checked[i])
						{
							ReportMonthFilter->Selection->Add(ReportMonthFilter->SelectionList->Strings[i]);
						}
					}
					ReportMonthFilter->GSTChecked						= chbCheckFilterGST->Checked;
					break;
				}
				case Reports::ftTree:
				{
					TReportTreeFilter *ReportTreeFilter				= (TReportTreeFilter *)CurrentFilter;
					ReportTreeFilter->GSTChecked						= chbTreeFilterGST->Checked;
					ReportTreeFilter->Selection->Clear();
					TTreeNode *CurItem = tvFilterTree->Items->GetFirstNode();
					while (CurItem)
					{
						if (CurItem->Level == 1 && CurItem->StateIndex == STATE_IMAGE_SMALL_CHECKED)
						{
							ReportTreeFilter->Selection->Add(ReportTreeFilter->Node(CurItem->Parent->Index)->Node(CurItem->Index)->Data);
						}
						CurItem = CurItem->GetNext();
					}
					break;
				}
/*				case Reports::ftNumber:
				{
					TReportNumberFilter *ReportNumberFilter	= (TReportNumberFilter *)CurrentFilter;
					ReportNumberFilter->Selection					= neNumber->Value;
					ReportNumberFilter->GSTChecked				= chbNumberGST->Checked;
					break;
				}*/
				case Reports::ftString:
				{
					TReportStringFilter *ReportStringFilter	= (TReportStringFilter *)CurrentFilter;
					ReportStringFilter->Selection					= edString->Text;
					ReportStringFilter->GSTChecked				= chbStringGST->Checked;
					break;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::AddFilterStringParams(TStrings *SQL, TStrings *Values, AnsiString FieldName, AnsiString Operator)
{
	if (Values->Count)
	{
		SQL->Text = SQL->Text + " " + Operator + " (";
		for (int i=0; i<Values->Count; i++)
		{
			if (i == 0)
			{
				SQL->Text = SQL->Text + FieldName + " = '" + Values->Strings[i] + "' ";
			}
			else
			{
				SQL->Text = SQL->Text + " Or " + FieldName + " = '" + Values->Strings[i] + "' ";
			}
		}
		SQL->Text = SQL->Text + ")";
	}
}
//---------------------------------------------------------------------------
void TfrmReports::AddFilterNumericParams(TStrings *SQL, TStrings *Values, AnsiString FieldName)
{
	if (Values->Count)
	{
		SQL->Text = SQL->Text + " And (";
		for (int i=0; i<Values->Count; i++)
		{
			if (i == 0)
			{
				SQL->Text = SQL->Text + FieldName + " = " + Values->Strings[i];
			}
			else
			{
				SQL->Text = SQL->Text + " Or " + FieldName + " = " + Values->Strings[i];
			}
		}
		SQL->Text = SQL->Text + ")";
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintMenuProfit(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	if (dmMMReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->StockTrans->StartTransaction();
	}
	AnsiString ReportName;
	if (ReportControl->CurrentSubReport == 0)
		ReportName = "repMenuProfit";
	else
		ReportName = "repMenuProfitSpecialPrice";
	try
	{
		TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
		dmMMReportData->SetupMenuProfit(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection/*, ReportCheckboxFilter->GSTChecked*/);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList ->AddObject("Menus",(TObject *)dmMMReportData->cdsMenu);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
		if (dmMMReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintMenuRecipes(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	if (dmMMReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
		dmMMReportData->SetupMenuRecipes(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection, ReportCheckboxFilter->GSTChecked);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Menus",(TObject *)dmMMReportData->qrMenuRecipe);
			//ExcelDataSetsList->AddObject("Stock Costs",(TObject *)dmMMReportData->qrMenuStock);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport("repMenuRecipes", false))
			{
				//rvMenuMate->SetParam("IncludeGST", ReportCheckboxFilter->GSTChecked ? "1":"0");
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
		if (dmMMReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintStockReductionItems(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	if (dmMMReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(0);
		TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		dmMMReportData->SetupStockReductionItems(LocationFilter->Selection, CategoryFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Items",(TObject *)dmMMReportData->qrReductionItems);
			ExcelDataSetsList->AddObject("Stock",(TObject *)dmMMReportData->qrReductionStock);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport("repStockReductionItems", false))
			{
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
		if (dmMMReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintMenu3rdPartyCodes(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}

	try
	{
		TReportCheckboxFilter *MenuFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
		dmMMReportData->SetupMenu3rdPartyCodes(MenuFilter->Selection);

		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList ->AddObject("Menus",(TObject *)dmMMReportData->qrMenu3rdParty);
			ExportToExcel( ExcelDataSetsList.get(), TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport("repMenu3rdPartyCodes", false))
			{
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintMenuItemBarcodes(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}

	try
	{
		TReportCheckboxFilter *MenuFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
		dmMMReportData->SetupMenu3rdPartyCodes(MenuFilter->Selection);

		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList ->AddObject("Menus",(TObject *)dmMMReportData->qrMenu3rdParty);
			ExportToExcel( ExcelDataSetsList.get(), TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport("repMenuItemBarcodes", false))
			{
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintCashup(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repCashup";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
        dmMMReportData->SetupCashup(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Float",(TObject *)dmMMReportData->qrCashupFloat);
			ExcelDataSetsList->AddObject("Cashup",(TObject *)dmMMReportData->qrCashup);
			ExcelDataSetsList->AddObject("Cashup Total",(TObject *)dmMMReportData->qrCashupTotal);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintCategoryAnalysis(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repCategoryAnalysis";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				dmMMReportData->SetupCategoryAnalysis(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection, true);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Category",(TObject *)dmMMReportData->qrCategoryAnalysis);
					ExcelDataSetsList->AddObject("Total",(TObject *)dmMMReportData->qrCatLocTotal);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}

				break;
			}
			case 1:
			{
				dmMMReportData->SetupCategoryAnalysis(ReportControl->Start, ReportControl->End, NULL, false);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Category",(TObject *)dmMMReportData->qrCategoryAnalysis);
					ExcelDataSetsList->AddObject("Total",(TObject *)dmMMReportData->qrCatLocTotal);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}

	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintCategoryBreakdown(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repCategoryBreakdown";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				dmMMReportData->SetupCategoryBreakdown(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection, true);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Category",(TObject *)dmMMReportData->qrCategoryBreakdown);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				dmMMReportData->SetupCategoryBreakdown(ReportControl->Start, ReportControl->End, NULL, false);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Category",(TObject *)dmMMReportData->qrCategoryBreakdown);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}

	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintHalfHourlySales(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repHalfHourlySalesSummary";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	bool ViewCreated = false;
	try
	{
		TReportCheckboxFilter *TerminalFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		switch (ReportControl->CurrentSubReport)
		{
			case 0: // Daily
			{
				dmMMReportData->SetupHalfHourlyDaily(ReportControl->Start, ReportControl->End,TerminalFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Half Hourly",(TObject *)dmMMReportData->qrHalfHoulrySummary);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);

						AnsiString TerminalRange =	"";
						rvMenuMate->SetParam("Terminals", " All Terminals");
						for (int i=0; i<TerminalFilter->Selection->Count; i++)
						{
							TerminalRange = TerminalRange + TerminalFilter->Selection->Strings[i] + ',';
						}
						TerminalRange.Delete(TerminalRange.Length(),1);
						if (TerminalRange != "")
						{
							TerminalRange = "For Terminals - " + TerminalRange;
							rvMenuMate->SetParam("Terminals", TerminalRange);
						}
						rvMenuMate->SetParam("EndTime", " ");
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1: // Consolidated
			{
				ViewCreated = true;
				dmMMReportData->SetupHalfHourlyConsolidated(ReportControl->Start, ReportControl->End,TerminalFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Half Hourly",(TObject *)dmMMReportData->qrHalfHoulrySummary);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);

						AnsiString TerminalRange =	"";
						rvMenuMate->SetParam("Terminals", " All Terminals");
						for (int i=0; i<TerminalFilter->Selection->Count; i++)
						{
							TerminalRange = TerminalRange + TerminalFilter->Selection->Strings[i] + ',';
						}
						TerminalRange.Delete(TerminalRange.Length(),1);
						if (TerminalRange != "")
						{
							TerminalRange = "For Terminals - " + TerminalRange;
							rvMenuMate->SetParam("Terminals", TerminalRange);
						}
						rvMenuMate->SetParam("EndTime", " to " + FormatDateTime("dddd, dd mmmm yyyy",ReportControl->End));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
		// View only created on the consolidated report
		if (ViewCreated == true)
		{
			dmMMReportData->MMTrans->StartTransaction();
			dmMMReportData->sqlView->Close();
			dmMMReportData->sqlView->SQL->Text = "DROP VIEW HalfHour";
			dmMMReportData->sqlView->ExecQuery();
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintTipsAndVouchers(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repTipsAndVouchers";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		dmMMReportData->SetupTipsAndVouchers(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("TipsVouchers",(TObject *)dmMMReportData->qrTipsVouchers);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintWages(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				dmMMReportData->SetupWagesByDepatment(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Wages",(TObject *)dmMMReportData->qrWages);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport("repWages", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");

						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}

				break;
			}
			case 1:
			{
				dmMMReportData->SetupWagesByStaff(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Wages",(TObject *)dmMMReportData->qrWages);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport("repWagesByStaff", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");

						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}

				break;
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintSavedSales(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repSavedSales";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
		dmMMReportData->SetupSavedSales(ReportCheckboxFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Saved Sales",(TObject *)dmMMReportData->qrSavedSales);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintSalesSummary(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				const AnsiString ReportName = "repSalesSummary";

				dmMMReportData->SetupSalesSummaryByLocation(ReportControl->Start, ReportControl->End, NULL);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Sales",(TObject *)dmMMReportData->qrSalesSummary);
					ExcelDataSetsList->AddObject("Patron Count",(TObject *)dmMMReportData->qrPatronCount);
					ExcelDataSetsList->AddObject("Averages",(TObject *)dmMMReportData->qrAveSummary);
					ExcelDataSetsList->AddObject("Discounts",(TObject *)dmMMReportData->qrDiscountSummary);
				  //	ExcelDataSetsList->AddObject("Adjustments",(TObject *)dmMMReportData->qrAdjustSummary);
					ExcelDataSetsList->AddObject("Weighted GP",(TObject *)dmMMReportData->qrWeightedGP);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
							AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				const AnsiString ReportName = "repSalesSummary";

				TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				dmMMReportData->SetupSalesSummaryByLocation(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Sales",(TObject *)dmMMReportData->qrSalesSummary);
					ExcelDataSetsList->AddObject("Patron Count",(TObject *)dmMMReportData->qrPatronCount);
					ExcelDataSetsList->AddObject("Averages",(TObject *)dmMMReportData->qrAveSummary);
					ExcelDataSetsList->AddObject("Discounts",(TObject *)dmMMReportData->qrDiscountSummary);
					ExcelDataSetsList->AddObject("Adjustments",(TObject *)dmMMReportData->qrAdjustSummary);
					ExcelDataSetsList->AddObject("Weighted GP",(TObject *)dmMMReportData->qrWeightedGP);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				  else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintConsumption(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				const AnsiString ReportName = "repConsumption";

				TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(4);
				dmMMReportData->SetupCategoryConsumption(ReportControl->Start, ReportControl->End, CategoryFilter->Selection);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Consumption",(TObject *)dmMMReportData->qrConsumption);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				  else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("IncludeGST", CategoryFilter->GSTChecked ? "1":"0");
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("subCategory", "By Category");
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				const AnsiString ReportName = "repConsumption";

				TReportCheckboxFilter *MenuFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
				dmMMReportData->SetupMenuConsumption(ReportControl->Start, ReportControl->End, MenuFilter->Selection, MenuFilter->GSTChecked);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Consumption",(TObject *)dmMMReportData->qrConsumption);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				  else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("IncludeGST", MenuFilter->GSTChecked ? "1":"0");
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("subCategory", "By Menu");
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 2:
			{
				const AnsiString ReportName = "repConsumption";

				TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				TReportCheckboxFilter *MenuFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);

				dmMMReportData->SetupLocationConsumption(ReportControl->Start, ReportControl->End, LocationFilter->Selection, MenuFilter->Selection, MenuFilter->GSTChecked);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Consumption",(TObject *)dmMMReportData->qrConsumption);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("IncludeGST", MenuFilter->GSTChecked ? "1":"0");
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("subCategory", "By Location");
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 3:
			{
				const AnsiString ReportName = "repConsumption";

				TReportCheckboxFilter *TabFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(3);

				dmMMReportData->SetupTabConsumption(ReportControl->Start, ReportControl->End, TabFilter->Selection, TabFilter->GSTChecked);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Consumption",(TObject *)dmMMReportData->qrConsumption);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("IncludeGST", TabFilter->GSTChecked ? "1":"0");
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("subCategory", "By Tab");
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 4:
			{
				const AnsiString ReportName = "repThirdPartyConsumption";

				TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				TReportCheckboxFilter *MenuFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);

				dmMMReportData->Setup3rdPartyConsumption(ReportControl->Start, ReportControl->End, LocationFilter->Selection, MenuFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Consumption",(TObject *)dmMMReportData->qrConsumption);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 5:
			{
				const AnsiString ReportName = "repConsumptionByDay";

	//			TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(4);
				dmMMReportData->SetupDayConsumption(ReportControl->Start, ReportControl->End);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Consumption",(TObject *)dmMMReportData->qrConsumption);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
                else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
//						rvMenuMate->SetParam("IncludeGST", CategoryFilter->GSTChecked ? "1":"0");
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
         case 6:
         {
				const AnsiString ReportName = "repConsumption";
				TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(4);
				dmMMReportData->SetupCategoryConsumptionExcSurcharge(ReportControl->Start, ReportControl->End, CategoryFilter->Selection);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Consumption",(TObject *)dmMMReportData->qrConsumption);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
            else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("subCategory", "By Category Excluding Surcharge");
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;


         }
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintConsumptionBySalesType(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		const AnsiString ReportName = "repConsumption";

		TReportCheckboxFilter *SalesTypeFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(2);
		dmMMReportData->SetupSalesTypeConsumption(ReportControl->Start, ReportControl->End, CategoryFilter->Selection, SalesTypeFilter->Selection);

		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Consumption",(TObject *)dmMMReportData->qrConsumption);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->SetParam("IncludeGST", CategoryFilter->GSTChecked ? "1":"0");
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                rvMenuMate->SetParam("subCategory", "By Sales Type");
                rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintConsumptionByHalfHour(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		AnsiString ReportName;

		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				ReportName = "repConsumptionByHalfHourDetailed";
				break;
			}
			case 1:
			{
				ReportName = "repConsumptionByHalfHourSummary";
				break;
			}
			case 2: // Daily
			{
				ReportName = "repHalfHourlySummary";
				
				TReportCheckboxFilter *TerminalFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
				dmMMReportData->SetupHalfHourlyDailyByConsumption(ReportControl->Start, ReportControl->End,TerminalFilter->Selection);
				if (ReportType == rtExcel)
				{
                    std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
                    ExcelDataSetsList->AddObject("Consumption",(TObject *)dmMMReportData->qrHalfHoulrySummary);
                    ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);

						AnsiString TerminalRange =	"";
						rvMenuMate->SetParam("Terminals", " All Terminals");
						for (int i=0; i<TerminalFilter->Selection->Count; i++)
						{
							TerminalRange = TerminalRange + TerminalFilter->Selection->Strings[i] + ',';
						}
						TerminalRange.Delete(TerminalRange.Length(),1);
						if (TerminalRange != "")
						{
							TerminalRange = "For Terminals - " + TerminalRange;
							rvMenuMate->SetParam("Terminals", TerminalRange);
						}
						rvMenuMate->SetParam("EndTime", " ");
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				return;
			}
		}

		TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(1);
		dmMMReportData->SetupCategoryConsumptionByHalfHour(ReportControl->Start, ReportControl->End, CategoryFilter->Selection);

		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Consumption",(TObject *)dmMMReportData->qrConsumption);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->SetParam("IncludeGST", CategoryFilter->GSTChecked ? "1":"0");
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintUserSales(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repUserSales";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
                const AnsiString ReportName = "repUserSales";

                TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(1);
                TReportCheckboxFilter *UsersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
                dmMMReportData->SetupUserSales(ReportControl->Start, ReportControl->End, UsersFilter->Selection,CategoryFilter->Selection );
                if (ReportType == rtExcel)
                {
                    std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
                    ExcelDataSetsList->AddObject("User Sales",(TObject *)dmMMReportData->qrUserSales);
                    ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
                }
                else
                {
                    if (rvMenuMate->SelectReport(ReportName, false))
                    {
                      AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                              "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                      rvMenuMate->SetParam("ReportRange", DateRange);
                      rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                      rvMenuMate->Execute();
                    }
                    else
                    {
                      Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
                    }
               }
               break;
			}
			case 1:
			{
                 const AnsiString ReportName = "repUserSalesByCategory";

                TReportCheckboxFilter *UsersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);

                dmMMReportData->SetupUserSalesByCategory(ReportControl->Start, ReportControl->End, UsersFilter->Selection);
                if (ReportType == rtExcel)
                {
                   std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
                   ExcelDataSetsList->AddObject("User Sales",(TObject *)dmMMReportData->qrUserSales);
                   ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
                }
                else
                {
                   if (rvMenuMate->SelectReport(ReportName, false))
                   {
                      AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                              "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                      rvMenuMate->SetParam("ReportRange", DateRange);
                            rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                      rvMenuMate->Execute();
                   }
                   else
                   {
                      Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
                   }
                }
                    break;
			}
			case 2:
			{
                const AnsiString ReportName = "repUserSalesSummary";

                TReportCheckboxFilter *UsersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);

                dmMMReportData->SetupUserSalesSummary(ReportControl->Start, ReportControl->End, UsersFilter->Selection);
                if (ReportType == rtExcel)
                {
                    std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
                    ExcelDataSetsList->AddObject("User Sales",(TObject *)dmMMReportData->qrUserSales);
                    ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
                }
                else
                {
                    if (rvMenuMate->SelectReport(ReportName, false))
                    {
                        AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                              "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                        rvMenuMate->SetParam("ReportRange", DateRange);
                            rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->Execute();
                    }
                    else
                    {
                        Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
                    }
                }
                break;
			}
      }
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintPatronAverages(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repPatronAverages";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		dmMMReportData->SetupPatronAverages(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Patron Averages",(TObject *)dmMMReportData->qrPatronAverages);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintPatronTypes(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repPatronTypes";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *PatronTypes = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		dmMMReportData->SetupPatronTypes(ReportControl->Start, ReportControl->End,PatronTypes->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Patron Types",(TObject *)dmMMReportData->qrPatronTypes);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------

void TfrmReports::PrintTurnaroundTimes(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repTurnAround";

	if (dmMMReportData->ChefMateTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->ChefMateTrans->StartTransaction();
	}
	try
	{
		dmMMReportData->SetupTurnAround(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Turn Around",(TObject *)dmMMReportData->qrTurnAround);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->ChefMateTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->ChefMateTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::GetChronologicalTableFilter(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *Filter = (TReportCheckboxFilter *)ReportFilter->PreviousFilter(0);
	if (Filter->Selection->Count > 0)
	{
		ReportFilter->SQL =
			"Select Distinct "
				"Archive.Table_Number "
			"From "
				"Security Inner Join Archive on "
					"Security.Security_Ref = Archive.Security_Ref "
			"Where "
				"Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp < :EndTime and "
				"Security.Security_Event = 'Ordered By' and "
				"Archive.Terminal_Name in (";
		for (int i=0; i<Filter->Selection->Count; i++)
		{
			if (i == 0)
			{
				ReportFilter->SQL = ReportFilter->SQL + "'" + Filter->Selection->Strings[i] + "'";
			}
			else
			{
				ReportFilter->SQL = ReportFilter->SQL + ",'" + Filter->Selection->Strings[i] + "'";
			}
		}
		ReportFilter->SQL = ReportFilter->SQL + ")"
			"Union All "

			"Select Distinct "
				"DayArchive.Table_Number "
			"From "
				"Security Inner Join DayArchive on "
					"Security.Security_Ref = DayArchive.Security_Ref "
			"Where "
				"Security.Time_Stamp >= :StartTime and  "
				"Security.Time_Stamp < :EndTime and "
				"Security.Security_Event = 'Ordered By' and "
				"DayArchive.Terminal_Name in (";
		for (int i=0; i<Filter->Selection->Count; i++)
		{
			if (i == 0)
			{
				ReportFilter->SQL = ReportFilter->SQL + "'" + Filter->Selection->Strings[i] + "'";
			}
			else
			{
				ReportFilter->SQL = ReportFilter->SQL + ",'" + Filter->Selection->Strings[i] + "'";
			}
		}
		ReportFilter->SQL = ReportFilter->SQL + ")"
			"Union All "

			"Select Distinct "
				"Orders.Table_Number "
			"From "
				"Security Inner Join Orders on "
					"Security.Security_Ref = Orders.Security_Ref "
			"Where "
				"Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp < :EndTime and "
				"Security.Security_Event = 'Ordered By' and "
				"Orders.Terminal_Name in (";
		for (int i=0; i<Filter->Selection->Count; i++)
		{
			if (i == 0)
			{
				ReportFilter->SQL = ReportFilter->SQL + "'" + Filter->Selection->Strings[i] + "'";
			}
			else
			{
				ReportFilter->SQL = ReportFilter->SQL + ",'" + Filter->Selection->Strings[i] + "'";
			}
		}
		ReportFilter->SQL = ReportFilter->SQL + ")"
			"Order By "
				"1";
	}
	else
	{
		ReportFilter->SQL =
			"Select Distinct "
				"Archive.Table_Number "
			"From "
				"Security Inner Join Archive on "
					"Security.Security_Ref = Archive.Security_Ref "
			"Where "
				"Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp < :EndTime and "
				"Security.Security_Event = 'Ordered By' "

			"Union All "

			"Select Distinct "
				"DayArchive.Table_Number "
			"From "
				"Security Inner Join DayArchive on "
					"Security.Security_Ref = DayArchive.Security_Ref "
			"Where "
				"Security.Time_Stamp >= :StartTime and  "
				"Security.Time_Stamp < :EndTime and "
				"Security.Security_Event = 'Ordered By' "

			"Union All "

			"Select Distinct "
				"Orders.Table_Number "
			"From "
				"Security Inner Join Orders on "
					"Security.Security_Ref = Orders.Security_Ref "
			"Where "
				"Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp < :EndTime and "
				"Security.Security_Event = 'Ordered By' "

			"Order By "
				"1";
	}
}
//---------------------------------------------------------------------------
void TfrmReports::GetChronologicalTabFilter(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *TerminalFilter	= (TReportCheckboxFilter *)ReportFilter->PreviousFilter(0);
	TReportCheckboxFilter *TableFilter		= (TReportCheckboxFilter *)ReportFilter->PreviousFilter(1);
	TIBSQL *Query									= ReportFilter->Query;
	TStrings *SQL									= Query->SQL;

	SQL->Text =
		"Select Distinct "
			"Archive.Tab_Name "
		"From "
			"Security Inner Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
		"Where "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";//and "
	AddFilterStringParams(SQL, TerminalFilter->Selection, "Archive.Terminal_Name");
	AddFilterNumericParams(SQL, TableFilter->Selection, "Archive.Table_Number");

	SQL->Text = SQL->Text +
		"Union All "

		"Select Distinct "
			"DayArchive.Tab_Name "
		"From "
			"Security Inner Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
		"Where "
			"Security.Time_Stamp >= :StartTime and  "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";//and "
	AddFilterStringParams(SQL, TerminalFilter->Selection, "DayArchive.Terminal_Name");
	AddFilterNumericParams(SQL, TableFilter->Selection, "DayArchive.Table_Number");

	SQL->Text = SQL->Text +
		"Union All "

		"Select Distinct "
			"Orders.Tab_Name "
		"From "
			"Security Inner Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
		"Where "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";//and "
	AddFilterStringParams(SQL, TerminalFilter->Selection, "Orders.Terminal_Name");
	AddFilterNumericParams(SQL, TableFilter->Selection, "Orders.Table_Number");

	SQL->Text = SQL->Text +
		"Order By "
			"1";
}
//---------------------------------------------------------------------------
void TfrmReports::PrintChronological(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repChronological";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *TerminalFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		TReportCheckboxFilter *TableFilter		= (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
		TReportCheckboxFilter *TabFilter			= (TReportCheckboxFilter *)ReportControl->ReportFilter(3);
		dmMMReportData->SetupChronological(ReportControl->Start, ReportControl->End, TableFilter->Selection, TabFilter->Selection, TerminalFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Sales",(TObject *)dmMMReportData->qrChronological);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::GetBillPaymentsReceiptFilter(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *TerminalFilter	= (TReportCheckboxFilter *)ReportFilter->PreviousFilter(0);
	TIBSQL *Query									= ReportFilter->Query;
	TStrings *SQL									= Query->SQL;

	SQL->Text =
		"Select Distinct "
			"ArcBill.Invoice_Number "
		"From "
			"Security Inner Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
          "left join ARCHIVE  on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
          "left join ARCORDERDISCOUNTS  on ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
		"Where "
         "((COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and "
         " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and COALESCE(ARCORDERDISCOUNTS.DISCOUNT_KEY,0) > 0) or ARCHIVE.DISCOUNT = 0 ) and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	AddFilterStringParams(SQL, TerminalFilter->Selection, "Security.Terminal_Name");

	SQL->Text = SQL->Text +

		"Union All "

		"Select Distinct "
			"DayArcBill.Invoice_Number "
		"From "
			"Security Inner Join DayArcBill on "
				"Security.Security_Ref = DayArcBill.Security_Ref "
           "left join DAYARCHIVE  on DAYARCBILL.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY "
          "left join DAYARCORDERDISCOUNTS  on DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
		"Where "
         "((COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and "
         " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_KEY,0) > 0) or DAYARCHIVE.DISCOUNT = 0 ) and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	AddFilterStringParams(SQL, TerminalFilter->Selection, "Security.Terminal_Name");

	SQL->Text = SQL->Text +

		"Order By "
			"1";
}
//---------------------------------------------------------------------------
void TfrmReports::GetBillDetailsReceiptFilter(TReportFilter *ReportFilter)
{
	// cww limits the invoices to ones after the Z Report
	//otherwise the Master Detail Query gets too Ugly ...
	TReportCheckboxFilter *TerminalFilter	= (TReportCheckboxFilter *)ReportFilter->PreviousFilter(0);
	TIBSQL *Query									= ReportFilter->Query;
	TStrings *SQL									= Query->SQL;
	SQL->Text =
		"Select Distinct "
			"ArcBill.Invoice_Number "
		"From "
			"Security Inner Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
           "left join ARCHIVE  on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
          "left join ARCORDERDISCOUNTS  on ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
		"Where "
         "((COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and "
         " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and COALESCE(ARCORDERDISCOUNTS.DISCOUNT_KEY,0) > 0) or ARCHIVE.DISCOUNT = 0 ) and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	AddFilterStringParams(SQL, TerminalFilter->Selection, "Security.Terminal_Name");

	SQL->Text = SQL->Text +
		"Order By "
			"1";
}
//---------------------------------------------------------------------------
void TfrmReports::GetBillTendersFilter(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *TerminalFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
	TIBSQL *Query									= ReportFilter->Query;
	TStrings *SQL									= Query->SQL;

	SQL->Text =
		"Select Distinct "
			"ArcBillPay.Pay_Type "
		"From "
			"Security Inner Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join ArcBillPay on "
				"ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key "
           "left join ARCHIVE  on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
          "left join ARCORDERDISCOUNTS  on ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
		"Where "
         "((COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and "
         " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and COALESCE(ARCORDERDISCOUNTS.DISCOUNT_KEY,0) > 0) or ARCHIVE.DISCOUNT = 0 ) and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	AddFilterStringParams(SQL, TerminalFilter->Selection, "Security.Terminal_Name");

	SQL->Text = SQL->Text +
		"Union All "

		"Select Distinct "
			"DayArcBillPay.Pay_Type "
		"From "
			"Security Inner Join DayArcBill on "
				"Security.Security_Ref = DayArcBill.Security_Ref "
			"Inner Join DayArcBillPay on "
				"DayArcBill.ArcBill_Key = DayArcBillPay.ArcBill_Key "
		  "left join DAYARCHIVE  on DAYARCBILL.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY "
          "left join DAYARCORDERDISCOUNTS  on DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
		"Where "
         "((COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and "
         " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_KEY,0) > 0) or DAYARCHIVE.DISCOUNT = 0 ) and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	AddFilterStringParams(SQL, TerminalFilter->Selection, "Security.Terminal_Name");

	SQL->Text = SQL->Text +
		"Order By "
			"1";
}
//---------------------------------------------------------------------------
void TfrmReports::GetInvoiceFilter(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *GroupsFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
	TIBSQL *Query									= ReportFilter->Query;
	TStrings *SQL									= Query->SQL;

    SQL->Text =
        "SELECT DISTINCT "
            "groups.name Name, contacts.name Contactname "
        "from "
            "contactgroups "
            "left join groups on contactgroups.groups_key = groups.groups_key "
            "left join contacts on contactgroups.CONTACTS_KEY = contacts.CONTACTS_KEY "
            "left join invoices on contacts.CONTACTS_KEY = invoices.contacts_KEY "
        "where "
            "invoices.creation_date >= :StartTime and "
            "invoices.creation_date < :Endtime";
        AddFilterStringParams(SQL, GroupsFilter->Selection, "groups.name");


}
//---------------------------------------------------------------------------
void TfrmReports::GetCurrentTabList(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *TabTypeFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
	TIBSQL *Query							    	= ReportFilter->Query;
	TStrings *SQL						 		   = Query->SQL;
	SQL->Text =
            "Select "
				    "Tab.Tab_Name "
			    "From "
				    "Tab Left Join Orders On "
                    "Tab.Tab_Key = Orders.Tab_Key "
			    "Where  "
				    "(Orders.Order_Type = 3 Or "
				    "Orders.Order_Type = 0) And "
                    "Tab.TAB_KEY not in (select tab.TAB_KEY from TAB  "
                            "Inner join CLIPP_TAB on tab.TAB_KEY = CLIPP_TAB.CLIPP_TAB_TAB_KEY  "
             "Where CLIPP_TAB.IS_TAB_CLOSED = 'T' and tab.TAB_TYPE = 13) ";
   
	if (TabTypeFilter->Selection->Count == 0)
	{
		SQL->Text = SQL->Text + "And Tab.Tab_Type in (0,1,2,13) ";
	}
	else
	{
		AddFilterStringParams(SQL, TabTypeFilter->Selection, "Tab.Tab_Type" );
	}
	SQL->Text = SQL->Text +
            "Group By "
                 "Tab.Tab_Name "
			"Order By "
				"Tab.Tab_Name ";

}
//---------------------------------------------------------------------------
void TfrmReports::GetDetailTabList(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *TabTypeFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
	TIBSQL *Query							    	= ReportFilter->Query;
	TStrings *SQL						 		   = Query->SQL;
	SQL->Text =
            "Select "
                "Tab.TAB_NAME "
            "From TAB "
            "Where "
            "Tab.TAB_KEY not in (select tab.TAB_KEY from TAB  "
                            "inner join CLIPP_TAB on tab.TAB_KEY = CLIPP_TAB.CLIPP_TAB_TAB_KEY  "
             "where CLIPP_TAB.IS_TAB_CLOSED = 'T' and tab.TAB_TYPE = 13) ";

	if (TabTypeFilter->Selection->Count == 0)
	{
		SQL->Text = SQL->Text + "And Tab.Tab_Type in (0,1,2,13) ";
	}
	else
	{
		AddFilterStringParams(SQL, TabTypeFilter->Selection, "Tab.Tab_Type");
	}
	SQL->Text = SQL->Text +
            "Group By "
                 "Tab.Tab_Name "
			"Order By "
				"Tab.Tab_Name ";

}
//---------------------------------------------------------------------------

void TfrmReports::GetDiscountsFilter(TReportFilter *ReportFilter)
{
	// Custom Discount is hard coded in as a generic discount
	TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
	TIBSQL *Query							    	= ReportFilter->Query;
	TStrings *SQL						 		   = Query->SQL;
	SQL->Text =
		"Select Distinct "
			"Security.From_Val "
		"From "
			"Security Left Join ArcBill On "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join Archive On "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
          "left join ARCORDERDISCOUNTS  on ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
		"Where "
         "((COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and "
         " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and COALESCE(ARCORDERDISCOUNTS.DISCOUNT_KEY,0) > 0) or ARCHIVE.DISCOUNT = 0 ) and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Discounted By' ";
	AddFilterStringParams(SQL, LocationFilter->Selection, "Archive.Order_Location");
	SQL->Text = SQL->Text +
		"Order By "
			"Security.From_Val";
}
//---------------------------------------------------------------------------

void TfrmReports::GetDiscountsSurchargesNamesFilter(TReportFilter *ReportFilter)
{
	// gets discounts and surcharges names from the arcorderdiscounts table
	TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
	TIBSQL *Query							    	= ReportFilter->Query;
	TStrings *SQL						 		   = Query->SQL;
	SQL->Text =
      "Select "
      "  Distinct ARCOD.NAME as NAME "
		"From "
      "  Security SEC "
      "  Join ArcBill ARCB On SEC.Security_Ref = ARCB.Security_Ref "
		"	Join Archive ARC On ARCB.ArcBill_Key = ARC.ArcBill_Key "
		"	Join ARCORDERDISCOUNTS ARCOD on ARCOD.ARCHIVE_KEY = ARC.ARCHIVE_KEY "
		"Where "
         "(COALESCE(ARCOD.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and "
         " COALESCE(ARCOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and COALESCE(ARCOD.DISCOUNT_KEY,0) > 0) and "
		"	SEC.Time_Stamp >= :StartTime and "
		"	SEC.Time_Stamp < :EndTime and "
		"	SEC.Security_Event = 'Discounted By' and "
		"	ARCB.Invoice_Number Is Not Null and "
		"	ARC.Archive_Key Is Not Null ";
	AddFilterStringParams(SQL, LocationFilter->Selection, "ARC.Order_Location");
	SQL->Text = SQL->Text +
		"Order By "
			"ARCOD.NAME asc";
}
//---------------------------------------------------------------------------
void TfrmReports::GetPriceAdjustmentsStaffFilter(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportFilter->PreviousFilter(0);
	TIBSQL *Query							  	= ReportFilter->Query;
	TStrings *SQL						 		= Query->SQL;
	SQL->Text =
	//const AnsiString PriceAdjustUsersList =
		"Select "
			"Contacts.Name "
		"From "
			"Security Left Join Archive On Security.Security_Ref = Archive.Security_Ref "
			"Left Join Contacts  On Contacts.Contacts_Key = Security.User_Key "
		"Where "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp <  :EndTime   and "
			"Security.Security_Event = 'Price Adjust' and "
			"Archive.Price <> Archive.Price_Level0 and "
			"Archive.Order_Type = 0 ";
	AddFilterStringParams(SQL, LocationFilter->Selection, "Archive.Order_Location");
	SQL->Text = SQL->Text +
		"Union "
		"Select "
			"Contacts.Name "
		"From "
			"Security Left Join DayArchive On Security.Security_Ref = DayArchive.Security_Ref "
			"Left Join Contacts  On Contacts.Contacts_Key = Security.User_Key "
		"Where "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp <  :EndTime  and "
			"Security.Security_Event = 'Price Adjust' and "
			"DayArchive.Price != DayArchive.Price_Level0  and "
			"DayArchive.Order_Type = 0 ";
	AddFilterStringParams(SQL, LocationFilter->Selection, "DayArchive.Order_Location");
	SQL->Text = SQL->Text +
		"Order by 1";
}
//---------------------------------------------------------------------------
void TfrmReports::GetPriceAdjustmentsReceiptFilter(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *UserFilter	  = (TReportCheckboxFilter *)ReportFilter->PreviousFilter(0);
	TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
	TIBSQL *Query							  	= ReportFilter->Query;
	TStrings *SQL						 		= Query->SQL;
	SQL->Text =
		"Select Distinct "
			"ArcBill.Invoice_Number "
		"From "
			"Security SecOrder Left Join Archive On SecOrder.Security_Ref = Archive.Security_Ref "
			"Left Join Contacts ConOrder On ConOrder.Contacts_Key = SecOrder.User_Key "
			"Left Join ArcBill On Archive.ArcBill_Key = ArcBill.ArcBill_Key "
			"Left Join Security SecAdjust On Archive.Security_Ref = SecAdjust.Security_Ref "
			"Left Join Contacts ConAdjust On ConAdjust.Contacts_Key = SecAdjust.User_Key "
          "left join ARCORDERDISCOUNTS  on ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
		"Where "
         "((COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and "
         " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and COALESCE(ARCORDERDISCOUNTS.DISCOUNT_KEY,0) > 0) or ARCHIVE.DISCOUNT = 0 ) and "
			"SecOrder.Time_Stamp >= :StartTime and "
			"SecOrder.Time_Stamp <  :EndTime and "
			"SecOrder.Security_Event = 'Ordered By' and "
			"SecAdjust.Security_Event = 'Price Adjust' and "
			"Archive.Price <> Archive.Price_Level0 and "
			"Archive.Order_Type = 0 ";
	AddFilterStringParams(SQL, UserFilter->Selection, "ConAdjust.Name");
	AddFilterStringParams(SQL, LocationFilter->Selection, "Archive.Order_Location");
	SQL->Text = SQL->Text +
		"Union "

		"Select Distinct "
			"DayArcBill.Invoice_Number "
		"From "
			"Security SecOrder Left Join DayArchive On SecOrder.Security_Ref = DayArchive.Security_Ref "
			"Left Join Contacts ConOrder On ConOrder.Contacts_Key = SecOrder.User_Key "
			"Left Join DayArcBill On DayArchive.ArcBill_Key = DayArcBill.ArcBill_Key "
			"Left Join Security SecAdjust On DayArchive.Security_Ref = SecAdjust.Security_Ref "
			"Left Join Contacts ConAdjust On ConAdjust.Contacts_Key = SecAdjust.User_Key "
		    "left join DAYARCORDERDISCOUNTS  on DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
		"Where "
         "((COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and "
         " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_KEY,0) > 0) or DAYARCHIVE.DISCOUNT = 0 ) and "
			"SecOrder.Time_Stamp >= :StartTime and "
			"SecOrder.Time_Stamp <  :EndTime and "
			"SecOrder.Security_Event = 'Ordered By' and "
			"SecAdjust.Security_Event = 'Price Adjust' and "
			"DayArchive.Price <> DayArchive.Price_Level0 and "
			"DayArchive.Order_Type = 0 ";
	AddFilterStringParams(SQL, UserFilter->Selection, "ConAdjust.Name");
	AddFilterStringParams(SQL, LocationFilter->Selection, "DayArchive.Order_Location");
	SQL->Text = SQL->Text +
		"Order By "
			"1";

}
//---------------------------------------------------------------------------
void TfrmReports::PrintBillPayments(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repBillPayments";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				TReportCheckboxFilter *TerminalFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				TReportCheckboxFilter *InvoiceFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
				dmMMReportData->SetupBillPayments(ReportControl->Start, ReportControl->End, InvoiceFilter->Selection, TerminalFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Bill Payments",(TObject *)dmMMReportData->qrBillPayments);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				TReportStringFilter *InvoiceFilter	= (TReportStringFilter *)ReportControl->ReportFilter(3);
				dmMMReportData->SetupBillPayments(InvoiceFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Bill Payments",(TObject *)dmMMReportData->qrBillPayments);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintBillTenders(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repBillTenders";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *TerminalFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		TReportCheckboxFilter *TenderFilter  	= (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
		dmMMReportData->SetupBillTenders(ReportControl->Start, ReportControl->End, TenderFilter->Selection, TerminalFilter->Selection);
		if (ReportType == rtExcel)
		{
        try
        {
             std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Bill Tenders",(TObject *)dmMMReportData->qrBillTenders);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
        }
        catch(EOleError& E)
        { //Excel seems not to be installed
                ShowMessage("Unable to open Excel. Please make sure you have it installed.");
        }
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}

//---------------------------------------------------------------------------
void TfrmReports::PrintSkimming(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repSkimming";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		dmMMReportData->SetupSkimming(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Skimming",(TObject *)dmMMReportData->qrSkimming);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------

void TfrmReports::PrintRefloat(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repRefloat";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		dmMMReportData->SetupRefloat(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Refloat",(TObject *)dmMMReportData->qrRefloat);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------

void TfrmReports::PrintSalesJournal(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repSalesJournalNew";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
      // we include service charge tax in either tax or service charge, depending on the report we pick
      bool includeSCTaxInTax = ReportControl->CurrentSubReport == 0;

      // Get the filter containing the terminals to display information from
		TReportCheckboxFilter *TerminalFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

      // call the setup method passing the variable on including service charge taxes
		dmMMReportData->SetupSalesJournal(
                              includeSCTaxInTax,
                              ReportControl->Start,
                              ReportControl->End,
                              TerminalFilter->Selection);

		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("SalesJournal",(TObject *)dmMMReportData->qrSalesJournal1); // changes for excel export..
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}

//---------------------------------------------------------------------------

void TfrmReports::PrintInvoice(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{

        TReportCheckboxFilter *GroupsFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
        TReportCheckboxFilter *MembersFilter  	= (TReportCheckboxFilter *)ReportControl->ReportFilter(2);

		switch (ReportControl->CurrentSubReport)
		{

			case 0:
			{

		        dmMMReportData->SetupInvoice(ReportControl->Start, ReportControl->End, MembersFilter->Selection, GroupsFilter->Selection);

		        if (ReportType == rtExcel)
		        {
			        std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			        ExcelDataSetsList->AddObject("Invoice",(TObject *)dmMMReportData->qrInvoice);
			        ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		        }
		        else
		        {
			        if (rvMenuMate->SelectReport("repInvoice", false))
			        {
				        AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				        rvMenuMate->SetParam("ReportRange", DateRange);
				        rvMenuMate->Execute();
			        }
			        else
			        {
				        Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			        }
		        }

				break;
			}
			case 1:
			{
		 		dmMMReportData->SetupInvoiceDetailed(ReportControl->Start, ReportControl->End, MembersFilter->Selection);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Invoice",(TObject *)dmMMReportData->qrInvoiceDetailed);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport("repInvoiceDetailed", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");

						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}

				break;
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}

//---------------------------------------------------------------------------

void TfrmReports::PrintBillDetails(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repBillDetails";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				TReportCheckboxFilter *TerminalFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				TReportCheckboxFilter *InvoiceFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
				dmMMReportData->SetupBillDetails(ReportControl->Start, ReportControl->End, InvoiceFilter->Selection, TerminalFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Bill Payment",(TObject *)dmMMReportData->qrBillPayments);
					ExcelDataSetsList->AddObject("Bill Details",(TObject *)dmMMReportData->qrBillDetails);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
					else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				TReportStringFilter *InvoiceFilter = (TReportStringFilter *)ReportControl->ReportFilter(3);
				dmMMReportData->SetupBillDetails(InvoiceFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Bill Payment",(TObject *)dmMMReportData->qrBillPayments);
					ExcelDataSetsList->AddObject("Bill Details",(TObject *)dmMMReportData->qrBillDetails);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
					else
      		{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintTabSummary(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repTabSummary";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *TabTypeFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
		TReportCheckboxFilter *TabFilter     = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		dmMMReportData->SetupTabSummary(TabTypeFilter->Selection,TabFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Tab Summary",(TObject *)dmMMReportData->qrTabSummary);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
			else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintTabDetails(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repTabDetails";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *TabTypeFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
		TReportCheckboxFilter *TabFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		dmMMReportData->SetupTabDetails(TabTypeFilter->Selection,TabFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Tab Details",(TObject *)dmMMReportData->qrTabDetails);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintDiscounts(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				TReportCheckboxFilter *DiscountFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);

				dmMMReportData->SetupDiscounts(ReportControl->Start, ReportControl->End, DiscountFilter->Selection, LocationFilter->Selection );
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Discounts",(TObject *)dmMMReportData->qrDiscounts);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport("repDiscounts", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
                        rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

				dmMMReportData->SetupDiscountedItemsDetails(ReportControl->Start, ReportControl->End, LocationFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Discounts",(TObject *)dmMMReportData->qrDiscounts);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport("repDiscountedItems", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");

                        	rvMenuMate->SetParam("ReportRange", DateRange);
                        rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 2:
			{
				TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				TReportCheckboxFilter *DiscountFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);

				dmMMReportData->SetupDiscountedItemsSummary(ReportControl->Start, ReportControl->End, DiscountFilter->Selection, LocationFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Discounts",(TObject *)dmMMReportData->qrDiscounts);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport("repDiscountedItemsSummary", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
         case 3:  // case 3 is for new discounts/ surcharges totals report
			{
				TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				TReportCheckboxFilter *DiscountFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(3);  // discount and surcharges report filter is at 4th position

				dmMMReportData->SetupDiscountsAndSurchargesTotals(ReportControl->Start, ReportControl->End, DiscountFilter->Selection, LocationFilter->Selection );
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Discounts",(TObject *)dmMMReportData->qrDiscountsSurchargesTotals);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport("repDiscountsSurchargesTotals", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintCancels(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				dmMMReportData->SetupCancels(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Cancels",(TObject *)dmMMReportData->qrCancels);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );

				}
				else
				{
					if (rvMenuMate->SelectReport("repCancels", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				dmMMReportData->SetupCancelsBill(ReportControl->Start, ReportControl->End);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Cancels Bill",(TObject *)dmMMReportData->qrCancelsBill);
					ExcelDataSetsList->AddObject("Cancels Detailed",(TObject *)dmMMReportData->qrCancelsDet);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport("repCancelsBill", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintStockLostRevenue(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{

		dmMMReportData->SetupStockLostRevenue(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
		 	std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Stock Lost Revenue",(TObject *)dmMMReportData->qrLostRevenue);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
		 	if (rvStock->SelectReport("repStockLostRevenue", false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvStock->SetParam("ReportRange", DateRange);
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}

}
//---------------------------------------------------------------------------
void TfrmReports::PrintCredits(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repCredits";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

		dmMMReportData->SetupCredits(ReportControl->Start, ReportControl->End, LocationFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Credits",(TObject *)dmMMReportData->qrCredits);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintPriceAdjustments(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repPriceAdjustments";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *LocationsFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		TReportCheckboxFilter *UsersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
		TReportCheckboxFilter *InvoicesFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(3);

		dmMMReportData->SetupPriceAdjustments(ReportControl->Start, ReportControl->End,LocationsFilter->Selection, UsersFilter->Selection,InvoicesFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Price Adjust",(TObject *)dmMMReportData->qrPriceAdjust);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintTransfers(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repTransfers";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		dmMMReportData->SetupAdjustments(ReportControl->Start, ReportControl->End, "Transferred");
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Adjustments",(TObject *)dmMMReportData->qrAdjustments);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintManualCashDrawer(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repManualCashDrawer";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *UsersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		dmMMReportData->SetupManualCashDrawer(ReportControl->Start, ReportControl->End, UsersFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Cash Drawer",(TObject *)dmMMReportData->qrManualCashDrawer);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
                rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
                rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintPointsEarned(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repLoyalty";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *CustomersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
		dmMMReportData->SetupLoyaltyPoints(CustomersFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Loyalty Points",(TObject *)dmMMReportData->qrLoyalty);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{

              Database::TDBTransaction transaction(dmMMData->dbMenuMate);
              transaction.Start();
              TManagerVariable varManager;



              if(varManager.GetInt(transaction, vmEnableSeperateEarntPts, 1))
              {
                rvMenuMate->SetParam("EarntPoints", "EarntPoints");
                rvMenuMate->SetParam("LoadedPoints", "LoadedPoints");
              }
              else
              {
                rvMenuMate->SetParam("EarntPoints", "");
                rvMenuMate->SetParam("LoadedPoints", "");
              }


              rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
              rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintHappyHour(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		dmMMReportData->SetupHappyHour(ReportControl->Start, ReportControl->End,LocationFilter->Selection,ReportControl->CurrentSubReport);
        if (ReportType == rtExcel)
		{
            try
            {
                std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
                ExcelDataSetsList->AddObject("Happy Hour",(TObject *)dmMMReportData->qrHappyHour);
                ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
            }
             catch(EOleError& E)
            { //Excel seems not to be installed
                    ShowMessage("Unable to open Excel. Please make sure you have it installed.");
            }
		}
		else
		{
            if (rvMenuMate->SelectReport("repHappyHour", false))
            {
                AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                                "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                rvMenuMate->SetParam("ReportRange", DateRange);
                rvMenuMate->SetParam("Heading",ReportControl->GetSubReport(ReportControl->CurrentSubReport));
                rvMenuMate->Execute();
            }
            else
            {
                Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
            }
        }    
	}

	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintLoyaltyCustomerDetails(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repLoyaltyDetails";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			// All
			case 0:
			{
				TReportCheckboxFilter *CustomersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

				dmMMReportData->SetupLoyaltyDetails(CustomersFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Loyalty Details",(TObject *)dmMMReportData->qrLoyalty);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
						rvMenuMate->SetParam("ReportRange", "");
						rvMenuMate->SetParam("Title", "Loyalty Customers");
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			// New
			case 1:
			{
				dmMMReportData->SetupNewLoyaltyDetails(ReportControl->Start, ReportControl->End);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Loyalty Details",(TObject *)dmMMReportData->qrLoyalty);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("Title", "New Loyalty Customers");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			// Altered
			case 2:
			{
				dmMMReportData->SetupAlteredLoyaltyDetails(ReportControl->Start, ReportControl->End);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Loyalty Details",(TObject *)dmMMReportData->qrLoyalty);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("Title", "Altered Loyalty Customers");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
			}
      }
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintLoyaltyHistory(TReportControl *ReportControl)
{
	const AnsiString DetailedReportName = "repLoyaltyHistoryDetailed";
	const AnsiString SummaryReportName  = "repLoyaltyHistorySummary";
	const AnsiString DateReportName     = "repLoyaltyHistoryDates";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{

		switch (ReportControl->CurrentSubReport)
		{
			// Detailed
			case 0:
			{
				TReportCheckboxFilter *CustomersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
				TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				if (CurrentConnection.VIPCreditName == "")
				{
			    dmMMReportData->SetupLoyaltyHistoryCustomer(ReportControl->Start, ReportControl->End, CustomersFilter->Selection,LocationFilter->Selection);

				}
				else
				{
					dmMMReportData->SetupLoyaltyHistoryVIPCredit(ReportControl->Start, ReportControl->End, CustomersFilter->Selection,LocationFilter->Selection);
				}
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Loyalty History",(TObject *)dmMMReportData->qrLoyaltyHistory);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(DetailedReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			// Summary
			case 1:
			{
				TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				if (CurrentConnection.VIPCreditName == "")
				{
					dmMMReportData->SetupLoyaltyHistoryLocation(ReportControl->Start, ReportControl->End, 0, LocationFilter->Selection);
				}
				else
				{
					dmMMReportData->SetupLoyaltyHistoryVIPCredit(ReportControl->Start, ReportControl->End, LocationFilter->Selection);
				}
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Loyalty Summary",(TObject *)dmMMReportData->qrLoyaltyHistory);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(SummaryReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			// Dates
			case 2:
			{
				TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				if (CurrentConnection.VIPCreditName == "")
				{
					dmMMReportData->SetupLoyaltyHistoryDates(ReportControl->Start, ReportControl->End, LocationFilter->Selection);
				}
				else
				{
					dmMMReportData->SetupLoyaltyHistoryVIPCredit(ReportControl->Start, ReportControl->End, LocationFilter->Selection);
				}
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Loyalty Dates",(TObject *)dmMMReportData->qrLoyaltyHistory);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(DateReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
					break;
				}
			}
      }
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintLoyaltyBirthdays(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repLoyaltyBirthdays";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		TReportMonthFilter *MonthFilter = (TReportMonthFilter *)ReportControl->ReportFilter(0);
		dmMMReportData->SetupLoyaltyBirthdays(MonthFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Birthdays",(TObject *)dmMMReportData->qrLoyalty);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintLoyaltyDiscountedProducts(TReportControl *ReportControl) // cww
{
	const AnsiString ReportName = "repLoyaltyDiscountedProducts";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}

	try
	{
		TReportCheckboxFilter *DiscountsFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		// if there is no selection then you need to add both so the report will work
		if (DiscountsFilter->Selection->Count == 0)
		{
			DiscountsFilter->Selection->Add("Location Discount");
			DiscountsFilter->Selection->Add("Member Discount");
		}
		TReportCheckboxFilter *CustomersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
		dmMMReportData->SetupLoyaltyDiscountedProducts(ReportControl->Start, ReportControl->End,DiscountsFilter->Selection, CustomersFilter->Selection);
      if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Discounted Products",(TObject *)dmMMReportData->qrLoyaltyDiscProducts);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintLoyaltySales(TReportControl *ReportControl) // cww
{
	AnsiString ReportName = "";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}

	try
	{
		TReportCheckboxFilter *CustomersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(3);
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
				{  // Summary item
					TReportCheckboxFilter *ItemFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
					dmMMReportData->SetupLoyaltySalesSummaryItem(ReportControl->Start, ReportControl->End, CustomersFilter->Selection,ItemFilter->Selection);
					ReportName = "repLoyaltySalesSummary";
					break;
				}
			case 1:
				{   // Summary category
					TReportTreeFilter     *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(1);
					dmMMReportData->SetupLoyaltySalesSummaryCategory(ReportControl->Start, ReportControl->End, CustomersFilter->Selection,CategoryFilter->Selection);
					ReportName = "repLoyaltySalesSummary";
					break;
				}
			case 2:
				{  // Detailed item
					TReportCheckboxFilter *ItemFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
					dmMMReportData->SetupLoyaltySalesDetailedItem(ReportControl->Start, ReportControl->End, CustomersFilter->Selection,ItemFilter->Selection);
					ReportName = "repLoyaltySalesDetailed";
					break;
				}
			case 3:
				{   // Detailed category
					TReportTreeFilter     *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(1);
					dmMMReportData->SetupLoyaltySalesDetailedCategory(ReportControl->Start, ReportControl->End, CustomersFilter->Selection,CategoryFilter->Selection);
					ReportName = "repLoyaltySalesDetailed";
					break;
				}
			case 4:
				{  // Detailed item
					TReportCheckboxFilter *ItemFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
					dmMMReportData->SetupLoyaltySalesDetailedItemExternalMember(ReportControl->Start, ReportControl->End, CustomersFilter->Selection,ItemFilter->Selection);
					ReportName = "repLoyaltySalesDetailed";
					break;
				}
			case 5:
				{   // Detailed category
					TReportTreeFilter     *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(1);
					dmMMReportData->SetupLoyaltySalesDetailedCategoryExternalMember(ReportControl->Start, ReportControl->End, CustomersFilter->Selection,CategoryFilter->Selection);
					ReportName = "repLoyaltySalesDetailed";
					break;
				}
		}
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Loyalty Sales",(TObject *)dmMMReportData->qrLoyaltySales);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintLoyaltyPurchaseCount(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repLoyaltyPurchaseCounts";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
		AnsiString SubHeading = "";
		if (ReportControl->CurrentSubReport == 0)
		{
			TReportCheckboxFilter *CustomersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
			dmMMReportData->SetupLoyaltyPurchaseCountByContact(CustomersFilter->Selection);
			SubHeading = "by  Customer";
		}
		else
		{
			TReportCheckboxFilter *LocationsFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
			dmMMReportData->SetupLoyaltyPurchaseCountByLocation(LocationsFilter->Selection);
			SubHeading = "by  Location";
		}
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Purchase Count", (TObject *)dmMMReportData->qrLoyaltyPurchaseCount);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				rvMenuMate->SetParam("SubHeading", SubHeading);
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintLoyaltyAudit(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}

	AnsiString ReportName;

	try
	{
		TReportCheckboxFilter *CustomersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

		if (ReportControl->CurrentSubReport == 0)
		{
			dmMMReportData->SetupLoyaltyAuditSummary(ReportControl->Start, ReportControl->End, CustomersFilter->Selection);
			ReportName = "repLoyaltyAuditSummary";
		}
		else if (ReportControl->CurrentSubReport == 1)
		{
			TReportCheckboxFilter *LocationsFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
			dmMMReportData->SetupLoyaltyAudit(ReportControl->Start, ReportControl->End, CustomersFilter->Selection, LocationsFilter->Selection);
			ReportName = "repLoyaltyAudit";
		}

		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Loyalty Audit",(TObject *)dmMMReportData->qrLoyaltyAuditSummary);
			ExportToExcel( ExcelDataSetsList.get(), TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
                Database::TDBTransaction transaction(dmMMData->dbMenuMate);
                transaction.Start();
        	    TManagerVariable varManager;
                int val = 1;
                if(varManager.GetInt(transaction, vmEnableSeperateEarntPts, val))
                {
                    rvMenuMate->SetParam("EarntSpent", "EarntSpent");
                    rvMenuMate->SetParam("LoadedSpent", "LoadedSpent");
                    rvMenuMate->SetParam("Loaded", "Loaded");
                }
                else
                {
                    rvMenuMate->SetParam("EarntSpent", "");
                    rvMenuMate->SetParam("LoadedSpent", "Redeemed");
                    rvMenuMate->SetParam("Loaded", "Purchased");
                }



				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//MM- 4579 --------------------------------------------------------------------
void TfrmReports::PrintResetPoint(TReportControl *ReportControl)
{
   if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
   AnsiString ReportName="repResetPoint";
   try
	{
      switch (ReportControl->CurrentSubReport)
		{
         case 0:
         {
		      dmMMReportData->ResetPoints(ReportControl->Start, ReportControl->End,0);
            break;
         }
         case 1:
         {
		      dmMMReportData->ResetPoints(ReportControl->Start, ReportControl->End,1);
            break;
         }
         case 2:
         {
		      dmMMReportData->ResetPoints(ReportControl->Start, ReportControl->End,2);
            break;
         }
         case 3:
         {
		      dmMMReportData->ResetPoints(ReportControl->Start, ReportControl->End,3);
            break;
         }
      }
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Reset Points",(TObject *)dmMMReportData->qrResetPoints);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
            rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------

void TfrmReports::GetStockLocationItemFilter(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportFilter->PreviousFilter(0);
	TIBSQL *Query									= ReportFilter->Query;
	TStrings *SQL									= Query->SQL;

	SQL->Text =
		"Select Distinct "
			"StockCategory.Stock_Category,"
			"StockCategory.Stock_Category_Key,"
			"StockGroup.Stock_Group,"
			"StockGroup.Stock_Group_Key "
		"From "
			"StockCategory Inner Join StockGroup on "
				"StockCategory.Stock_Category_Key = StockGroup.Stock_Category_Key "
			"Inner Join Stock On "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Inner Join StockLocation On "
				"StockLocation.Stock_Key = Stock.Stock_Key ";
	if (LocationFilter->Selection->Count > 0)
	{
		SQL->Text = SQL->Text + "Where ";
		AddFilterStringParams(SQL, LocationFilter->Selection, "StockLocation.Location", "");
	}

	SQL->Text = SQL->Text +
		"Order By "
			"Stock_Category,"
			"Stock_Group";
}
//---------------------------------------------------------------------------
void TfrmReports::PrintStockDetails(TReportControl *ReportControl)
{
	const AnsiString ReportName1 = "repStockDetailsByItem";
	const AnsiString ReportName2 = "repStockDetailsByLocation";
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(0);
//				dmStockReportData->SetupStockDetailsByGroup(CategoryFilter->Selection);
				dmStockReportData->SetupStockDetails(NULL, CategoryFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Stock Details",(TObject *)dmStockReportData->qrStock);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvStock->SelectReport(ReportName1, false))
					{
						rvStock->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(2);
//				dmStockReportData->SetupStockDetailsByLocation(LocationFilter->Selection, CategoryFilter->Selection);
				dmStockReportData->SetupStockDetails(LocationFilter->Selection, CategoryFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Stock Details",(TObject *)dmStockReportData->qrStock);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvStock->SelectReport(ReportName2, false))
					{
						rvStock->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}

//---------------------------------------------------------------------------
void TfrmReports::PrintStockValue(TReportControl *ReportControl)
{
	// NB repStockValueByLocation is used for Stock Value and Stocktake Value
	const AnsiString ReportName1 = "repStockValueByItem";
	const AnsiString ReportName2 = "repStockValueByLocation";
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(0);
				dmStockReportData->SetupStockValueByGroup(CategoryFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Stock Value",(TObject *)dmStockReportData->qrStockValue);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvStock->SelectReport(ReportName1, false))
					{
						rvStock->SetParam("Heading", "Stock Value by Item");
						rvStock->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(2);
				dmStockReportData->SetupStockValueByLocation(LocationFilter->Selection, CategoryFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Stock Value",(TObject *)dmStockReportData->qrStockValue);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvStock->SelectReport(ReportName2, false))
					{
					rvStock->SetParam("Heading", "Stock Value by Location");
						rvStock->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintStockBarcodes(TReportControl *ReportControl)
{
	const AnsiString ReportName1 = "repStockBarcodes";

	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(0);
		dmStockReportData->SetupStockBarcodes(CategoryFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Stock Barcodes",(TObject *)dmStockReportData->qrStockValue);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvStock->SelectReport(ReportName1, false))
			{
				rvStock->SetParam("Heading", "Stock Barcodes");
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintStockInquiry(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *LocationFilter		= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		TReportTreeFilter *GroupFilter				= (TReportTreeFilter *)ReportControl->ReportFilter(2);
		TReportCheckboxFilter *TransactionFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(3);

		dmStockReportData->SetupStockInquiry(	ReportControl->Start,
															ReportControl->End,
															LocationFilter->Selection,
															GroupFilter->Selection,
															TransactionFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Stock Inquiry",(TObject *)dmStockReportData->qrStockInquiry);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvStock->SelectReport("repStockInquiry", false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvStock->SetParam("ReportRange", DateRange);
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintStockMovement(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		TReportTreeFilter *GroupFilter			= (TReportTreeFilter *)ReportControl->ReportFilter(2);
		AnsiString TransactionType, ReportTitle;

		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				TransactionType	= "Receipt";
				ReportTitle			= "Inwards Goods";
				break;
			}
			case 1:
			{
				TransactionType	= "Transfer";
				ReportTitle			= "Location Transfers";
				break;
			}
			case 2:
			{
				TransactionType	= "Stocktake";
				ReportTitle			= "Stocktake Adjustments";
				break;
			}
			case 3:
			{
				TransactionType	= "Sale";
				ReportTitle			= "MenuMate Sales";
				break;
			}
			case 4:
			{
				TransactionType	= "Writeoff";
				ReportTitle			= "Writeoffs";
				break;
			}
            case 5:
            {
				TransactionType	= "Manufacture";
				ReportTitle			= "Manufactures";
				break;

            }
			case 6:
			{
				TransactionType	= "";
				ReportTitle			= "Stock Movement";
				break;
			}
		}
		dmStockReportData->SetupStockMovement(ReportControl->Start, ReportControl->End, TransactionType, LocationFilter->Selection, GroupFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Stock Movement",(TObject *)dmStockReportData->qrStockMovement);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvStock->SelectReport("repStockMovement", false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvStock->SetParam("ReportRange", DateRange);
				rvStock->SetParam("Title", ReportTitle);
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintStockTransfer(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				TReportTreeFilter *GroupFilter			= (TReportTreeFilter *)ReportControl->ReportFilter(2);

				dmStockReportData->SetupStockTransfer(ReportControl->Start, ReportControl->End, LocationFilter->Selection, GroupFilter->Selection,0);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Stock Transfer",(TObject *)dmStockReportData->qrStockTransferFrom);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvStock->SelectReport("repStockTransferFrom", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvStock->SetParam("ReportRange", DateRange);
                        rvStock->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvStock->SetParam("UserName",  frmLogin->CurrentUserName);
						rvStock->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				dmStockReportData->SetupStockBatchTransfer(ReportControl->Start, ReportControl->End);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Stock Transfer",(TObject *)dmStockReportData->qrStockTransferFrom);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvStock->SelectReport("repBatchTransfers", false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvStock->SetParam("ReportRange", DateRange);
						rvStock->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintStockReorder(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
		TReportTreeFilter *GroupFilter			= (TReportTreeFilter *)ReportControl->ReportFilter(1);

		dmStockReportData->SetupStockReorderItem(LocationFilter->Selection, GroupFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Stock Reorder",(TObject *)dmStockReportData->qrStockReorder);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvStock->SelectReport("repStockReorderByItem", false))
			{
			//AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
			//								"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
			//rvStock->SetParam("ReportRange", DateRange);
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintStocktakeVariance(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *StocktakeFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		//dmStockReportData->SetupStocktakeHistory(ReportControl->Start, ReportControl->End, StocktakeFilter->Selection);

		// only select one report at once
		if (StocktakeFilter->Selection->Count != 1)
		{
			Application->MessageBox("You must select only one stocktake!", "Error", MB_OK + MB_ICONERROR);
			return;
		}
		dmStockReportData->SetupStockVariance(StrToInt(StocktakeFilter->Selection->Strings[0]));
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			//ExcelDataSetsList->AddObject("Stocktake Variance",(TObject *)dmStockReportData->qrStocktakeHistory);
			ExcelDataSetsList->AddObject("Stocktake Variance",(TObject *)dmStockReportData->qrStockVariance);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			//if (rvStock->SelectReport("repStocktakeHistory", false))
			if (rvStock->SelectReport("repStocktakeVariance", false))
			{
				rvStock->SetParam("TotalBankingInc", 0.00);
				rvStock->SetParam("TotalBankingExcl", 0.00);
				rvStock->SetParam("UserVariance", 0.00);

				try
				{
					StocktakeFilter->Query->Transaction->StartTransaction();
					StocktakeFilter->Query->Close();
					for (StocktakeFilter->Query->ExecQuery(); !StocktakeFilter->Query->Eof; StocktakeFilter->Query->Next())
					{
						if (StocktakeFilter->Query->FieldByName("Stocktake_Key")->AsString == StocktakeFilter->Selection->Strings[0])
						{
							rvStock->SetParam("ReportRange", StocktakeFilter->Query->FieldByName("Initialised_Time")->AsString);
  							break;
						}
					}
				}
				__finally
				{
					StocktakeFilter->Query->Close();
					StocktakeFilter->Query->Transaction->Commit();
				}
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintStocktakeValue(TReportControl *ReportControl)
{
   // NB repStockValueByLocation is used for Stock Value and Stocktake Value
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *StocktakeFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		dmStockReportData->SetupStocktakeStockValuation(StocktakeFilter->Selection);

        if (ReportType == rtExcel) {
            std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());

            ExcelDataSetsList->AddObject("Stocktake Value",
                reinterpret_cast<TObject *>(dmStockReportData->qrStockValue));
            ExportToExcel(ExcelDataSetsList.get(), TreeView1->Selected->Text);
        } else if (rvStock->SelectReport("repStockValueByLocation", false)) {
			rvStock->SetParam("Heading", "Stock Value by Stocktake");
			rvStock->Execute();
		} else {
			Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
		}
	}


	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------

void TfrmReports::PrintStockManufacture(TReportControl *ReportControl)
{
   // NB repStockValueByLocation is used for Stock Value and Stocktake Value
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{

		dmStockReportData->SetupManufactureRecipe(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
		 	std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Stock Manufacture",(TObject *)dmStockReportData->qrStockManufacture);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
		 	if (rvStock->SelectReport("repStockManufacture", false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvStock->SetParam("ReportRange", DateRange);
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}


	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------

void TfrmReports::PrintStockWriteOff(TReportControl *ReportControl)
{
  	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

		dmStockReportData->SetupWriteOffLocation(ReportControl->Start, ReportControl->End, LocationFilter->Selection);
		if (ReportType == rtExcel)
		{
		 	std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Stock Write Off",(TObject *)dmStockReportData->qrStockWriteOff);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
		 	if (rvStock->SelectReport("repStockWriteOff", false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvStock->SetParam("ReportRange", DateRange);
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}

}


//---------------------------------------------------------------------------
void TfrmReports::GetStockTransSupplierList(TReportFilter *ReportFilter)
{
	ReportFilter->SQL =
		"Select Distinct "
			"Supplier_Name "
		"From "
			"StockTrans "
		"Where "
			"Created >= :StartTime and "
			"Created < :EndTime "
		"Order By "
			"Supplier_Name";
}
void TfrmReports::GetStockReceiptList(TReportFilter *ReportFilter)
{

	ReportFilter->SQL =
		"Select Distinct "
			"BATCH_KEY "
		"From "
			"StockTrans "
		"Where "
			"Created >= :StartTime and "
			"Created < :EndTime "
		"Order By "
			"BATCH_KEY";
}
//---------------------------------------------------------------------------
void TfrmReports::GetStockInvoiceList(TReportFilter *ReportFilter)
{
	TReportCheckboxFilter *SupplierFilter	= (TReportCheckboxFilter *)ReportFilter->PreviousFilter(0);
	TIBSQL *Query									= ReportFilter->Query;
	TStrings *SQL									= Query->SQL;

	SQL->Text =
		"Select Distinct "
			"Reference "
		"From "
			"StockTrans "
		"Where "
			"Created >= :StartTime and "
			"Created < :EndTime ";
	AddFilterStringParams(SQL, SupplierFilter->Selection, "Supplier_Name");

	SQL->Text = SQL->Text +
		"Order By "
			"Reference";
}
//---------------------------------------------------------------------------
void TfrmReports::PrintPurchaseInvoices(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *StockReceiptFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		TReportCheckboxFilter *InvoiceFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(2);

		dmStockReportData->SetupSupplierPurchases(ReportControl->Start, ReportControl->End, StockReceiptFilter->Selection, InvoiceFilter->Selection,rgReports->ItemIndex);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Supplier Purchase",(TObject *)dmStockReportData->qrStockReceiptByCat);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvStock->SelectReport("repSupplierPurchases", false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvStock->SetParam("ReportRange", DateRange);
                rvStock->SetParam("CURRENTUSER", frmLogin->CurrentUser.UserID +" at  "+ Now());
                rvStock->SetParam("Header", "") ;
                rvStock->SetParam("CompanyName", CurrentConnection.CompanyName);
            if (rgReports->ItemIndex == 0)
            {  // rvStock->SetParam("Header", " Purchases");
            	rvStock->SetParam("Header", "By Invoice Number");
            }
            else
            {  //rvStock->SetParam("Header", " Purchases");
               rvStock->SetParam("Header", "By Stock Receipt Number");
            }
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintSupplierDetails(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *SupplierFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(0);

		dmStockReportData->SetupSupplierDetails(SupplierFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Supplier Details",(TObject *)dmStockReportData->qrContactDetails);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvStock->SelectReport("repSupplierDetails", false))
			{
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintSupplierBackOrders(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *SupplierFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(0);

		dmStockReportData->SetupSupplierBackOrders(SupplierFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Back Orders",(TObject *)dmStockReportData->qrSupplierBackOrders);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvStock->SelectReport("repSupplierBackOrders", false))
			{
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
	
}
//---------------------------------------------------------------------------
void TfrmReports::PrintSupplierReorder(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *SupplierFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(0);

		dmStockReportData->SetupStockReorderBySupplier(SupplierFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Supplier ReOrders",(TObject *)dmStockReportData->qrStockReorder);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvStock->SelectReport("repStockReorderBySupplier", false))
			{
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnFindClick(TObject *Sender)      //cww
{
	// enable  chlbFilterList
	rbFilterSelectionClick(Sender);
	rbFilterSelection->Checked = True;
	// Downward Search and Initialize to Start
	if (FindDialog->Options.Contains(frDown))
	{
		chlbFilterList->ItemIndex = 0;
		chlbFilterList->Refresh();
		CheckListBoxCtr = 0;
	}
	// Upward Search and Initialize to End
	if (!FindDialog->Options.Contains(frDown))
	{
		chlbFilterList->ItemIndex = chlbFilterList->Items->Count -1;
		chlbFilterList->Refresh();
		CheckListBoxCtr = chlbFilterList->Items->Count -1;
	}
	FindDialog->Position = Classes::Point(frmReports->Left + TreeView1->Width + 30, frmReports->Top + 30);
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::FindDialogFind(TObject *Sender)
{
  try{
	int  SaveCheckListBoxCtr = CheckListBoxCtr;
	int  FoundText;
	bool Found  = false;
	// move off the current record else it will just find it again and not start or last record
	if (CheckListBoxCtr != 0 && CheckListBoxCtr != chlbFilterList->Items->Count  )
	{
		if (FindDialog->Options.Contains(frDown)) // downward search
			CheckListBoxCtr++;
		if (!FindDialog->Options.Contains(frDown)) // upward search
			CheckListBoxCtr--;
	}
	//if (FindDialog->Options.Contains(frDown)  && CheckListBoxCtr != 0 && CheckListBoxCtr != chlbFilterList->Items->Count  )
	//  CheckListBoxCtr++;
	// upward and not last or start record
  //	if (!FindDialog->Options.Contains(frDown) && CheckListBoxCtr != 0 && CheckListBoxCtr != chlbFilterList->Items->Count )
	//  CheckListBoxCtr--;
	if (FindDialog->Options.Contains(frDown)) // Downward Search
	{
		for (int i = CheckListBoxCtr; i < chlbFilterList->Items->Count; i++,++CheckListBoxCtr)
		{
			//UpperCase and Pos
			FoundText = AnsiPos(UpperCase(FindDialog->FindTextA),UpperCase(chlbFilterList->Items->Strings[i]));
			if (FoundText > 0)
			{
				chlbFilterList->ItemIndex = i;
				// if not the last record
				if (CheckListBoxCtr != chlbFilterList->Items->Count)
					Found = true;
				break;
			}
		}
	}
	else  // Upward Search
	{
		for (int i = CheckListBoxCtr; i >= 0; i--,--CheckListBoxCtr)
		{
			//UpperCase and Pos
            try{
			FoundText = AnsiPos(UpperCase(FindDialog->FindTextA),UpperCase(chlbFilterList->Items->Strings[i]));
            }
            catch(... )
            {}
			if (FoundText > 0)
			{
				chlbFilterList->ItemIndex = i;
				// if not the start record
				if (CheckListBoxCtr != 0)
					Found = true;
				break;
			}
		}
	}

	if (FindDialog->Options.Contains(frDown)) // Downward Search
	{
		// check to see if it End record and not Found
		if ( CheckListBoxCtr >= chlbFilterList->Items->Count)
		{
			// return to original location
			CheckListBoxCtr = SaveCheckListBoxCtr;
			// if End record is not Found
			if (!Found)
				Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
		}
	}
	else // Upward Search
	{
		// check to see if it Start record
		if ( CheckListBoxCtr <= 0)
		{
			// return to original location
			CheckListBoxCtr = SaveCheckListBoxCtr;
			// if Start record is not Found
			if (!Found)
				Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
		}
	}
    }
    catch( ... )
    {
    }


}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::tsCheckFilterExit(TObject *Sender)
{
  if (FindDialog && FindDialog->Handle > 0)
	  FindDialog->CloseDialog();
}
//---------------------------------------------------------------------------
void	TfrmReports::ExportToExcel( TStringList *ExcelDataSetsList,AnsiString ACaption )
{
	std::auto_ptr<TfrmExcel> frmExcel(new TfrmExcel(NULL));

	frmExcel->ExportToExcel( ExcelDataSetsList,ACaption);
	// When you close Excel you close  frmExcel
	// try to have only 1 copy of Excel open, is tidier
	frmExcel->ShowModal();
}
//---------------------------------------------------------------------------
void TfrmReports::SetToPreview( )
{
	ReportSystem->DefaultDest    = rdPreview;
	ReportSystem->DoNativeOutput = true;
	ReportSystem->RenderObject   = NULL;
	ReportSystem->OutputFileName = "";
}
//---------------------------------------------------------------------------
bool TfrmReports::SaveReportToFile(AnsiString &EmailFileName )
{
	bool Result = false;
	const std::auto_ptr<TfrmVarianceParams> frmVarianceParams(new TfrmVarianceParams(NULL));
	frmVarianceParams->VarianceParams = vReportType;
	if (frmVarianceParams->ShowModal() == mrOk)
	{
		ReportSystem->DefaultDest    = rdFile;
		ReportSystem->DoNativeOutput = false;
		const AnsiString PurchaseOrder = CurrentConnection.MenuSavePath + "\\Purchase Order";
		switch (frmVarianceParams->rgpReportType->ItemIndex)
		{
			case 0 : ReportSystem->RenderObject   = RavePDF;
						ReportSystem->OutputFileName = PurchaseOrder + ".pdf";
						break;
			case 1 : ReportSystem->RenderObject   = RaveHTML;
						ReportSystem->OutputFileName = PurchaseOrder + ".htm";
						break;
			case 2 : ReportSystem->RenderObject   = RaveText;
						ReportSystem->OutputFileName = PurchaseOrder + ".txt";
						break;
			case 3 : ReportSystem->RenderObject   = RaveRTF;
						ReportSystem->OutputFileName = PurchaseOrder + ".rtf";
						break;
		}
		EmailFileName = ReportSystem->OutputFileName;
		Result = true;
	}
	return Result;
}
//---------------------------------------------------------------------------
void TfrmReports::SendEmail( AnsiString EmailFileName, AnsiString EmailAddress, AnsiString EmailSubject, AnsiString ReceiverName)
{
	if (SendEmail::Send(
		EmailFileName,
		EmailSubject + " from " + CurrentConnection.CompanyName,
		EmailAddress,ReceiverName) == false)
	{
		throw Sysutils::Exception("Outlook is not setup");
	}
	//std::auto_ptr<TfrmExcel> frmExcel(new TfrmExcel(NULL));
	//frmExcel->SendEmail(EmailFileName,EmailAddress,EmailSubject);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void TfrmReports::PrintMembershipAudit(TReportControl *ReportControl)
{
        TIBTransaction *MMTrans = dmMMReportData->MMTrans;
        TIBDatabase *DefaultDatabase = MMTrans->DefaultDatabase;

        if (DefaultDatabase->Connected) {
                if (MMTrans->InTransaction)
                        MMTrans->Commit();

                MMTrans->StartTransaction();
        }

        try {
                const AnsiString ReportName = "repLoyaltyMembershipAuditPointsBreakdown";
               TReportCheckboxFilter *CustomersFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
               dmMMReportData->SetupLoyaltyMembershipAuditItem1(ReportControl->Start, ReportControl->End, CustomersFilter->Selection);
                   if (ReportType == rtExcel)
				{
				    std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("membership audit",(TObject *)dmMMReportData->qrMembershipAuditPointsBreakdown);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text ); 
				}

                else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{

					  	AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") + "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn") ;
				  		rvMenuMate->SetParam("ReportRange", DateRange);
                       rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now() );
						rvMenuMate->Execute(); 
					}
              /* AnsiString StartTime =ReportControl->Start.FormatString("dd.mm.yyyy, 00:00:00");
                AnsiString EndTime = ReportControl->End.FormatString("dd.mm.yyyy, 12:59:59");
                GenerateMembershipQuery(Query, ReportControl, startDate, endDate);
                if (!rvMenuMate->SelectReport(
                    "repLoyaltyMembershipAuditPointsBreakdown", false))
                        Application->MessageBox(
                            "Report repLoyaltyMembershipAuditPointsBreakdown couldn't be found!",
                            "Report error", MB_OK | MB_ICONERROR);
                else {
                    rvMenuMate->SetParam("Starting_date", StartTime);
                    rvMenuMate->SetParam("Ending_date", EndTime);
                    rvMenuMate->Execute();

                } */ 
                else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
        } __finally {
                if (DefaultDatabase->Connected)
                        MMTrans->Commit();
        }
}
//---------------------------------------------------------------------------
void TfrmReports::PrintExtendedMembershipAudit(TReportControl *ReportControl)
{
        TIBTransaction *MMTrans = dmMMReportData->MMTrans;
        TIBDatabase *DefaultDatabase = MMTrans->DefaultDatabase;

        if (DefaultDatabase->Connected) {
                if (MMTrans->InTransaction)
                        MMTrans->Commit();

                MMTrans->StartTransaction();
        }

        try {
                TIBQuery *Query =
                    dmMMReportData->qrExtendedMembershipAuditPointsBreakdown;
                AnsiString startDate =
                    ReportControl->Start.FormatString("dd.mm.yyyy");
                AnsiString endDate =
                    ReportControl->End.FormatString("dd.mm.yyyy");

                GenerateExtendedMembershipQuery(Query, ReportControl, startDate, endDate);

                if (!rvMenuMate->SelectReport(
                    "repLoyaltyExtendedMembershipAuditPointsBreakdown", false))
                        Application->MessageBox(
                            "Report repLoyaltyExtendedMembershipAuditPointsBreakdown couldn't be found!",
                            "Report error", MB_OK | MB_ICONERROR);
                else {
                    rvMenuMate->SetParam("Starting_date", startDate);
                    rvMenuMate->SetParam("Ending_date", endDate);
                    rvMenuMate->Execute();
                }
        } __finally {
                if (DefaultDatabase->Connected)
                        MMTrans->Commit();
        }
}

/*
   Description on how the following method works.
   ..............................................
   It primarily uses three tables (POINTSTRANSACTIONS, ARCBILL and ARCHIVE)
   First it will retrieve the points transactions for a given user along with the invoice number,
   Then it uses invoice number to match up with ARCBILL to find the ArcBill key
   Use the ArcBill key to search ARCHIVE table to find out what items involved in producing point transactions
   Sums up the number of items bought ( this includes points purchases and loyalty points discounts as well)
   Calculate the Earned and Redeemed points using adjustment in POINTSTRANSACTIONS.

   How the RaveReport for this query (repLoyaltyMembershipAuditPointsBreakdown) works.
   ...................................................................................
   Rave report uses few number of parameters like CurrentPointsBalance, EndOfDayTransactionBalance etc to calculate different
   points balances on-the-fly.
   These values will be set at the very first time the execution begins and continues to modify throughout the iteration of data band.
*/


void TfrmReports::GenerateExtendedMembershipQuery(
    TIBQuery *query,
    TReportControl *reportControl,
    AnsiString &startDate,
    AnsiString &endDate)
{
        TReportCheckboxFilter *selectionFilter =
            reinterpret_cast<TReportCheckboxFilter *>(
                reportControl->ReportFilter(1));
        TStrings *selectedContacts = selectionFilter->Selection;

        AnsiString contactsClause;
        AnsiString queryTemplate;

        for (int i = 0; i < selectedContacts->Count; i++)
            contactsClause +=
                ("Pt.Contacts_key = " +
                    selectedContacts->Strings[i] + " and ");

        queryTemplate = AnsiString(
			"select "
				"Contacts.Name as Contacts_name, "
				"Contacts.Mailing_address as Contacts_address, "
                "Contacts.Member_number as Contacts_member_number, "
				"case "
					"when (Contacts_balance is null) "
						"then 0 "
					"else Contacts_balance "
				"end as "
					"Contacts_balance, "
				"Companies.Company_name, "
				"Companies.Location_address as Company_address, "
				"Item_breakdown.Day_of_transaction, "
				"Item_breakdown.Hour_of_transaction, "
				"Item_breakdown.Minute_of_transaction, "
				"Item_breakdown.Terminal_name, "
				"Item_breakdown.Item_name, "
                "Item_breakdown.Number_purchased, "
				"Item_breakdown.Points_earned, "
				"Item_breakdown.Redeemed "
			"from "
				"( "
					"select "
					  "distinct "
						"Contacts_key, "
						"Day_of_transaction,"
						"Hour_of_transaction,"
						"Minute_of_transaction,"
						"Terminal_name, "
						"Item_name, "
						"Number_purchased, "
						"(Number_purchased * Redeemed) as Redeemed, "
						"(Number_purchased * Points_earned) as Points_earned "
					"from "
						"( "
							"select "
								"Pt.Contacts_key, "
								"cast(Pt.Time_stamp as date) "
                                    "as Day_of_transaction, "
								"extract(hour from Pt.Time_stamp) "
                                    "as Hour_of_transaction, "
								"extract(minute from Pt.Time_stamp) "
                                    "as Minute_of_transaction, "
								"ArcBill.Terminal_name, "
                                "case "
                                    "when (Adjustment_type = 1) "
                                        "then 'Points purchased' "
                                    "else Item_name "
                                "end as "
                                    "Item_name, "
								"Pt.Adjustment_type, "
                                "case "
                                    "when (adjustment_type = 1) "
                                        "then adjustment "
                                    "else points_earned "
                                "end as "
                                    "points_earned, "
                                "case "
                                    "when (Adjustment_type = 1) "
                                        "then 0 "
                                    "else redeemed "
                                "end as "
                                    "Redeemed, "
								"count(*) as Number_purchased "
							"from "
								"PointsTransactions as Pt "
							"inner join "
								"ArcBill on "
									"ArcBill.Invoice_number = Pt.Invoice_number "
							"left join "
								"Archive on "
									"Archive.ArcBill_key = ArcBill.ArcBill_key "
							"where ");
    if (selectedContacts->Count)
        queryTemplate += contactsClause;

    queryTemplate += AnsiString(
    	        "Pt.Time_stamp >= '" +
                    startDate +
                "' and Pt.Time_stamp <= '" +
                    endDate +
                "' group by "
                    "Pt.Contacts_key, "
                    "Day_of_transaction, "
                    "Hour_of_transaction, "
                    "Minute_of_transaction, "
                    "ArcBill.Terminal_name, "
                    "Archive.Item_name, "
                    "Pt.Adjustment_type, "
                    "Points_earned, "
                    "Redeemed "
                ") "
            ") as "
			    "Item_breakdown "
			"inner join "
				"Contacts on "
					"Contacts.Contacts_key = Item_breakdown.Contacts_key "
			"left join "
				"Companies on "
					"Companies.Company_key = Contacts.Company_key "
			"left join "
				"( "
                    "select "
                        "contacts_key, "
                        "sum(adjustment) as contacts_balance "
                    "from "
                        "( "
                            "select "
                            "   distinct "
                            "       contacts_key, "
                            "       time_stamp, "
                            "       sum(adjustment) as adjustment "
                            "from "
                            "   pointstransactions as pt "
                            "where ");

    if (selectedContacts->Count)
        queryTemplate += contactsClause;

    queryTemplate += AnsiString(
                                "pt.Time_stamp < '" +
                                    startDate +
                                "' and Pt.Adjustment_type > 2 "
                            "group by "
                                "contacts_key, "
                                "time_stamp "
                        ") "
                    "group by "
                        "contacts_key "
				") as "
					"Opening_balance on "
						"Opening_balance.Contacts_key = Item_breakdown.Contacts_key "
            "where "
                "Points_earned <> 0 or "
                "Redeemed <> 0;");

    query->SQL->Clear();
    query->SQL->Add(queryTemplate);
}





void TfrmReports::DailySalesReport(TReportControl *ReportControl)
{	AnsiString ReportName;

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{

		switch (ReportControl->CurrentSubReport)
		{
			case 0:
			{
				ReportName = "repDSRByCategory";

				TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(2);
				dmMMReportData->SetupDailySalesByCategories(ReportControl->Start, ReportControl->End, CategoryFilter->Selection);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("User Category Sales",(TObject *)dmMMReportData->qrDSR);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}

			  else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("IncludeGST", CategoryFilter->GSTChecked ? "1":"0");
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}
			case 1:
			{
				const AnsiString ReportName = "repDSRByCategory";

				TReportCheckboxFilter *MenuFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				dmMMReportData->SetupDailySalesByMenu(ReportControl->Start, ReportControl->End, MenuFilter->Selection, MenuFilter->GSTChecked);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Daily Sales Report By Menu",(TObject *)dmMMReportData->qrDSR);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				  else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("IncludeGST", MenuFilter->GSTChecked ? "1":"0");
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}

			case 2:
			{
				 	const AnsiString ReportName = "repDSRByDay";
	           dmMMReportData->SetupDailySales(ReportControl->Start, ReportControl->End);

            if (ReportType == rtExcel)
            {
               std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
               ExcelDataSetsList->AddObject("User Sales",(TObject *)dmMMReportData->qrDSRInvoice);
               ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
            }
            else
            {

               if (rvMenuMate->SelectReport(ReportName, false))
               {
                  AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                          "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                  rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                  rvMenuMate->Execute();
               }
               else
               {
                  Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
               }
            }
	break;
			}

         	case 3:
			{  const AnsiString ReportName = "repDSRByInvoice";
				TReportCheckboxFilter *InvoiceFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(3);
				dmMMReportData->SetupDailySalesByInvoice(ReportControl->Start, ReportControl->End, InvoiceFilter->Selection, InvoiceFilter->GSTChecked);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Daily Sales Report by Invoice",(TObject *)dmMMReportData->qrDSRInvoice );
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;

			}

case 4:
{
               const AnsiString ReportName = "repDSRByInvoice";
                TReportStringFilter *InvoiceFilter	= (TReportStringFilter *)ReportControl->ReportFilter(4);
				dmMMReportData->SetupDailySalesByInvoice(InvoiceFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Bill Payments",(TObject *)dmMMReportData->qrDSRInvoice);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
}
case 5:
{          	TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(5);
            const AnsiString ReportName = "repDSRConsolidated";
	           dmMMReportData->SetupConsolidatedSales(ReportControl->Start, ReportControl->End, LocationFilter->Selection );

            if (ReportType == rtExcel)
            {
               std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
               ExcelDataSetsList->AddObject("ConsolidatedSalesREPORT",(TObject *)dmMMReportData->qrDSRConsolidated);
               ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
            }
            else
            {

               if (rvMenuMate->SelectReport(ReportName, false))
               {
                  AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                          "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                  rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                  rvMenuMate->Execute();
               }
               else
               {
                  Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
               }
            }
	break;
}

  case 6:
			{
				ReportName = "repDSRByMenuDate";

				TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(2);
				dmMMReportData->SetupDailySalesByCategoriesDate(ReportControl->Start, ReportControl->End, CategoryFilter->Selection);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("User Category Sales",(TObject *)dmMMReportData->qrDSRMenuDay);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}

			  else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("IncludeGST", CategoryFilter->GSTChecked ? "1":"0");
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}
				break;
			}





   case 7:
{
            const AnsiString ReportName = "repDSRConsolidatedByItem";
	           dmMMReportData->SetupConsolidatedSalesByItem(ReportControl->Start, ReportControl->End);

            if (ReportType == rtExcel)
            {
               std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
               ExcelDataSetsList->AddObject("ConsolidatedSalesREPORTByItem",(TObject *)dmMMReportData->qrDSRConsolidatedByItem);
               ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
            }
            else
            {

               if (rvMenuMate->SelectReport(ReportName, false))
               {
                  AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                          "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                  rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                  rvMenuMate->Execute();
               }
               else
               {
                  Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
               }
            }
	break;
}


 case 8:
{
            const AnsiString ReportName = "repDSRSummary";
            TReportCheckboxFilter *MenuFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
	         dmMMReportData->SetupDSRSummary(ReportControl->Start, ReportControl->End, NULL);

            if (ReportType == rtExcel)
            {
               std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
               ExcelDataSetsList->AddObject("DSR Category",(TObject *)dmMMReportData->qrDSRCategory);
               ExcelDataSetsList->AddObject("DSR Tax",(TObject *)dmMMReportData->qrDSRTax);
               ExcelDataSetsList->AddObject("DSR Discounts",(TObject *)dmMMReportData->qrDSRDiscount);
               ExcelDataSetsList->AddObject("DSR Surcharge",(TObject *)dmMMReportData->qrDSRSurcharge);
               ExcelDataSetsList->AddObject("DSR Payment",(TObject *)dmMMReportData->qrDSRPay);
               ExcelDataSetsList->AddObject("DSR Cancels",(TObject *)dmMMReportData->qrDSRrefcan);
                ExcelDataSetsList->AddObject("DSR Totals",(TObject *)dmMMReportData->qrDSRSum);
               ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
            }
            else
            {

               if (rvMenuMate->SelectReport(ReportName, false))
               {
                  AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                          "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                  rvMenuMate->SetParam("ReportRange", DateRange);
				  rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                  rvMenuMate->Execute();
               }
               else
               {
                  Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
               }
            }
	break;
}
case 9:
{
            const AnsiString ReportName = "repDSRSummary";
            TReportCheckboxFilter *MenuFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
	        dmMMReportData->SetupDSRSummary(ReportControl->Start, ReportControl->End,MenuFilter->Selection);

            if (ReportType == rtExcel)
            {
               std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
               ExcelDataSetsList->AddObject("DSR Category",(TObject *)dmMMReportData->qrDSRCategory);
               ExcelDataSetsList->AddObject("DSR Tax",(TObject *)dmMMReportData->qrDSRTax);
               ExcelDataSetsList->AddObject("DSR Discounts",(TObject *)dmMMReportData->qrDSRDiscount);
               ExcelDataSetsList->AddObject("DSR Surcharge",(TObject *)dmMMReportData->qrDSRSurcharge);
               ExcelDataSetsList->AddObject("DSR Payment",(TObject *)dmMMReportData->qrDSRPay);
               ExcelDataSetsList->AddObject("DSR Cancels",(TObject *)dmMMReportData->qrDSRrefcan);
                ExcelDataSetsList->AddObject("DSR Totals",(TObject *)dmMMReportData->qrDSRSum);
               ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
            }
            else
            {

               if (rvMenuMate->SelectReport(ReportName, false))
               {
                  AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                          "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                  rvMenuMate->SetParam("ReportRange", DateRange);
				  rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                  rvMenuMate->Execute();
               }
               else
               {
                  Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
               }
            }
	break;
}

  }
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}



void TfrmReports::PrintComplimentary(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{


				const AnsiString ReportName = "repComplimentary";


				TReportCheckboxFilter *InvoiceFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				dmMMReportData->SetupComplimentary(ReportControl->Start, ReportControl->End, InvoiceFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Complimentary",(TObject *)dmMMReportData->qrComplimentary );
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}



	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}



void TfrmReports::PrintNonChargeable(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
				const AnsiString ReportName = "repNonChargeable";

				TReportCheckboxFilter *InvoiceFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
				dmMMReportData->SetupNonChargeable(ReportControl->Start, ReportControl->End, InvoiceFilter->Selection);

				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("NonChargeable",(TObject *)dmMMReportData->qrComplimentary);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				  else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}


	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}



void TfrmReports::PrintTaxProfile(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repTaxProfile";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{ 	TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(0);

		dmMMReportData->SetupTaxProfile(CategoryFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("TaxProfile",(TObject *)dmMMReportData->qrTaxProfile);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}

//---------------------------------------------------------------------------
void TfrmReports::PrintSalesSummaryB(TReportControl *ReportControl)
{
   const AnsiString ReportName = "repSalesSummaryB";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
        TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

        dmMMReportData->SetupSalesSummaryB(ReportControl->Start, ReportControl->End, LocationFilter->Selection);
        if (ReportType == rtExcel)
        {
            std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
            ExcelDataSetsList->AddObject("Sales Summary B",(TObject *)dmMMReportData->qrSalesSummaryB);
            ExcelDataSetsList->AddObject("Accumulated Totals",(TObject *)dmMMReportData->qrAccumulatedTotals);
            ExcelDataSetsList->AddObject("PaymentTotal",(TObject *)dmMMReportData->qrPaymentTotal);
            ExcelDataSetsList->AddObject("DiscountTotal",(TObject *)dmMMReportData->qrDiscountTotal);
            ExcelDataSetsList->AddObject("CancelTotal",(TObject *)dmMMReportData->qrCancelTotal);
            ExcelDataSetsList->AddObject("DiscountTotalNoGroup",(TObject *)dmMMReportData->qrDiscountTotalNoGroup);
            ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
        }
        else
        {
            if (rvMenuMate->SelectReport(ReportName, false))
            {
                AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                         "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                rvMenuMate->SetParam("ReportRange", DateRange);
                rvMenuMate->Execute();
            }
            else
            {
                Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
            }
        }
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------

void TfrmReports::PrintAccumulatedZeds(TReportControl *ReportControl)
{
   const AnsiString ReportName = "repAccumulatedZeds";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
        TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(0);

        dmMMReportData->SetupAccumulatedZeds(ReportControl->Start, ReportControl->End);
        if (ReportType == rtExcel)
        {
            std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
            ExcelDataSetsList->AddObject("Accumulated Zeds",(TObject *)dmMMReportData->qrAccumulatedZeds);
            ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
        }
        else
        {
            if (rvMenuMate->SelectReport(ReportName, false))
            {
                AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                         "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                rvMenuMate->SetParam("ReportRange", DateRange);
                rvMenuMate->Execute();
            }
            else
            {
                Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
            }
        }
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------

void TfrmReports::PrintSalesSummaryC(TReportControl *ReportControl)
{
    AnsiString ReportName = "";

    if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
    {
        dmMMReportData->MMTrans->StartTransaction();
    }
    try
    {
        switch (ReportControl->CurrentSubReport)
        {
            case 0:
            {
                ReportName = "repSalesSummaryCConsolidated";

                TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

                dmMMReportData->SetupSalesSummaryC(ReportControl->Start, ReportControl->End, LocationFilter->Selection);
                if (ReportType == rtExcel)
                {
                    std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
                    ExcelDataSetsList->AddObject("Sales Consolidated",(TObject *)dmMMReportData->qrSalesConsolidated);
                    ExcelDataSetsList->AddObject("Sales Revenue Total",(TObject *)dmMMReportData->qrSalesRevenueTotal);
                    ExcelDataSetsList->AddObject("PaymentTotal",(TObject *)dmMMReportData->qrPaymentTotal);
                    ExcelDataSetsList->AddObject("DiscountTotal",(TObject *)dmMMReportData->qrDiscountTotal);
                    ExcelDataSetsList->AddObject("DiscountTotalComplimentary",(TObject *)dmMMReportData->qrDiscountTotalComplimentary);
                    ExcelDataSetsList->AddObject("SalesCancelTotal",(TObject *)dmMMReportData->qrSalesConsolidatedCancel);
                    ExcelDataSetsList->AddObject("Sales Menu Summary",(TObject *)dmMMReportData->qrSalesMenuSummary);
                    ExcelDataSetsList->AddObject("Sales Count By Day Part",(TObject *)dmMMReportData->qrSalesCountByDayPart);
                    ExcelDataSetsList->AddObject("SalesDiscountTotalLocation",(TObject *)dmMMReportData->qrSalesDiscountTotalLocation);
                    ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
                }
                break;
           }
           case 1:
           {
                ReportName = "repSalesSummaryCRevenue";

                TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

                dmMMReportData->SetupSalesSummaryC(ReportControl->Start, ReportControl->End, LocationFilter->Selection);
                if (ReportType == rtExcel)
                {
                    std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
                    ExcelDataSetsList->AddObject("Sales Revenue",(TObject *)dmMMReportData->qrSalesRevenue);
                    ExcelDataSetsList->AddObject("Sales Revenue Total",(TObject *)dmMMReportData->qrSalesRevenueTotal);
                    ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
                }
                break;
            }
        }

        if (ReportType != rtExcel)
        {
            if (rvMenuMate->SelectReport(ReportName, false))
            {
                AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
                                       "\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
                rvMenuMate->SetParam("ReportRange", DateRange);
                rvMenuMate->Execute();
            }
            else
            {
                Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
            }
        }
    }
    __finally
    {
        if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
        {
            dmMMReportData->MMTrans->Commit();
        }
    }
}
//---------------------------------------------------------------------------
void TfrmReports::PrintSupplierInvoice(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{

		dmStockReportData->SetupSupplierInvoice(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Supplier Invoice",(TObject *)dmStockReportData->qrSupplierInvoice);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );

		}
		else
		{
			if (rvStock->SelectReport("repSupplierInvoice", false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");

                rvStock->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvStock->SetParam("ReportRange", DateRange);
                rvStock->SetParam("UserName",frmLogin->CurrentUserName  );
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}



void TfrmReports::PrintFinanceDaily(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repFinanceDaily";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{     TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		TReportCheckboxFilter *TerminalFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);


		dmMMReportData->SetupFinanceDaily(ReportControl->Start, ReportControl->End, TerminalFilter->Selection, LocationFilter->Selection)    ;
		if (ReportType == rtExcel)
		{   try{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
		  //	ExcelDataSetsList->AddObject("Float",(TObject *)dmMMReportData->qrCashupFloat);
			ExcelDataSetsList->AddObject("Cashup",(TObject *)dmMMReportData->qrCashup);
            ExcelDataSetsList->AddObject("Sales Summary",(TObject *)dmMMReportData->qrSalesSummary);
            ExcelDataSetsList->AddObject("Bill Tenders",(TObject *)dmMMReportData->qrBillTenders);
            ExcelDataSetsList->AddObject("Discount Surcharges",(TObject *)dmMMReportData->qrDiscountsSurchargesTotals);

		   	ExcelDataSetsList->AddObject("Cashup Total",(TObject *)dmMMReportData->qrCashupTotal);
            ExcelDataSetsList->AddObject("Discount Total",(TObject *)dmMMReportData->qrCatDiscount);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
            }
           catch(EOleError& E)
        { //Excel seems not to be installed
                ShowMessage("Unable to open Excel. Please make sure you have it installed.");
        }
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//-----------------------------------------------------------------------------
void TfrmReports::PrintCashier(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repCashier";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
         TReportCheckboxFilter *LocationFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
         TReportCheckboxFilter *TerminalFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);

			dmMMReportData->SetupCashier(ReportControl->Start, ReportControl->End, TerminalFilter->Selection, LocationFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Sales Summary",(TObject *)dmMMReportData->qrSalesSummary);
			ExcelDataSetsList->AddObject("Cashup",(TObject *)dmMMReportData->qrCashup);
			ExcelDataSetsList->AddObject("Cashup Total",(TObject *)dmMMReportData->qrCashupTotal);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
		  		rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
			Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
 //------------------------------------------------------------------------
void TfrmReports::PrintDeptSalesSummary(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{

			 const AnsiString ReportName = "repDeptSalesSummary";
            TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

				dmMMReportData->SetupSalesSummaryByLocation(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection);
				if (ReportType == rtExcel)
				{
					std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Sales",(TObject *)dmMMReportData->qrSalesSummary);
					ExcelDataSetsList->AddObject("Patron Count",(TObject *)dmMMReportData->qrPatronCount);
					ExcelDataSetsList->AddObject("Averages",(TObject *)dmMMReportData->qrAveSummary);
					ExcelDataSetsList->AddObject("Discounts",(TObject *)dmMMReportData->qrDiscountSummary);
					ExcelDataSetsList->AddObject("Adjustments",(TObject *)dmMMReportData->qrAdjustSummary);
					ExcelDataSetsList->AddObject("Weighted GP",(TObject *)dmMMReportData->qrWeightedGP);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}

			}


	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//------------------------------------------------------
void TfrmReports::GetTerminalFilter(TReportFilter *ReportFilter)
{



	TReportCheckboxFilter *LocationFilter	= (TReportCheckboxFilter *)ReportFilter->PreviousFilter(0);
	TIBSQL *Query							  	= ReportFilter->Query;
	TStrings *SQL						 		= Query->SQL;
	SQL->Text =
	//Select Terminals

        " Select Distinct "
         "SECURITY.TERMINAL_NAME, "
			"ArcBill.BILLED_LOCATION "

		   "From "
			   "Security left Join ArcBill on "
            "Security.Security_Ref = ArcBill.Security_Ref "
            "left join ARCHIVE  on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY  "

	         " Where "
               "ArcBill.Time_Stamp >= :StartTime and "
					"ArcBill.Time_Stamp < :EndTime and "
					"Security.Security_Event = 'Billed By' "   ;

	AddFilterStringParams(SQL, LocationFilter->Selection, "ArcBill.Billed_Location");

	SQL->Text = SQL->Text +
		"Order By "
			"1";
}

//---------------------------------------------------------------------------
void TfrmReports::PrintCheckRemoval(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repCheckRemoval";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{

		dmMMReportData->SetupCheckRemoval(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Sales",(TObject *)dmMMReportData->qrCheckRemoval);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}


void TfrmReports::PrintReprintReceipt(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repReprintRecept";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{

		dmMMReportData->SetupReprintReceipt(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("ReprintRecept",(TObject *)dmMMReportData->qrReprintReceipt);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}


void TfrmReports::PrintReprintOrder(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repReprintOrder";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{

		dmMMReportData->SetupReprintOrder(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("ReprintOrder",(TObject *)dmMMReportData->qrReprintOrder);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//------------------------------------------------------
void TfrmReports::PrintStockReconcialation(TReportControl *ReportControl)
{
	if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
	{
		dmStockReportData->StockTrans->StartTransaction();
	}
	try
	{
		TReportCheckboxFilter *LocationFilter		= (TReportCheckboxFilter *)ReportControl->ReportFilter(1);
		TReportTreeFilter *GroupFilter				= (TReportTreeFilter *)ReportControl->ReportFilter(2);
	

		dmStockReportData->SetupStockReconcialation( ReportControl->Start, ReportControl->End, LocationFilter->Selection, GroupFilter->Selection);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Stock Reconcialation",(TObject *)dmStockReportData->qrStockReconcialation);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvStock->SelectReport("repStockReconcialation", false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvStock->SetParam("ReportRange", DateRange);
                                rvStock->SetParam("ReportRange", DateRange);
				rvStock->SetParam("CompanyName", CurrentConnection.CompanyName);
                rvStock->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
		
				rvStock->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
  //------------------------------------------------------
void TfrmReports::PrintPointSpend(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repPointSpend";

	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
       TReportTreeFilter *CategoryFilter = (TReportTreeFilter *)ReportControl->ReportFilter(1);
       TReportCheckboxFilter *PaymentFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(2);
	   dmMMReportData->SetupPointSpend(ReportControl->Start, ReportControl->End, CategoryFilter->Selection, PaymentFilter->Selection);



		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("PointSpend",(TObject *)dmMMReportData->qrPointSpend);

			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
        	 if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
                rvMenuMate->SetParam("IncludeGST", CategoryFilter->GSTChecked ? "1":"0");
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                      rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
				
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
}	}                                                                                       

//-------------------------------------------------------------------------------------------------------------------------------------------------------
void TfrmReports::PrintProfitLossSummary(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{

				const AnsiString ReportName = "repProfitLossSummary";
                TReportCheckboxFilter *ReportCheckboxFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(1);

				dmMMReportData->SetupProfiltLoss(ReportControl->Start, ReportControl->End, ReportCheckboxFilter->Selection);
				if (ReportType == rtExcel)
				{
				    std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
					ExcelDataSetsList->AddObject("Profit Loss",(TObject *)dmMMReportData->qrProfiltLoss);
					ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text ); 
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
                        rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}

			}


	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmReports::PrintBreakdownCategory(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}

 	try
	{
		TReportCheckboxFilter *MenuFilter = (TReportCheckboxFilter *)ReportControl->ReportFilter(0);
		dmMMReportData->SetupBreakdownCategory(MenuFilter->Selection);

		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList ->AddObject("BreakdownCategory",(TObject *)dmMMReportData->qrBreakdownCategory);
			ExportToExcel( ExcelDataSetsList.get(), TreeView1->Selected->Text );
		}
	else
		{
			if (rvMenuMate->SelectReport("repBreakdownCategory", false))
			{
             //  AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
											 //	"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
			   //	rvMenuMate->SetParam("ReportRange", DateRange);
                rvMenuMate->SetParam("IncludeGST", MenuFilter->GSTChecked ? "1":"0");
				rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
				rvMenuMate->Execute();
			}

        	   
             
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}


		
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------
void TfrmReports::PrintSalesSummaryD(TReportControl *ReportControl)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{
                dmMMReportData->nameOfTaxPayer = "";
                dmMMReportData->addressOfTaxPayer = "";
                dmMMReportData->tinNumber = "";
                dmMMReportData->serialNo = "";
                std::auto_ptr <TStringList> CompanyData (new TStringList);
                AnsiString filename = ExtractFilePath(Application->ExeName);

                std::auto_ptr<TFileStream> FileStream(new TFileStream(filename + "\\Owner Details.txt",  fmOpenRead | fmShareExclusive));
                CompanyData->LoadFromStream(FileStream.get());
                if(CompanyData->Count > 3)
                {
                    dmMMReportData->nameOfTaxPayer = CompanyData->Strings[0].TrimLeft();
                    dmMMReportData->addressOfTaxPayer = CompanyData->Strings[1].TrimLeft();
                    dmMMReportData->tinNumber = CompanyData->Strings[2].TrimLeft();
                    dmMMReportData->serialNo = CompanyData->Strings[3].TrimLeft();
                }
				const AnsiString ReportName = "repSalesSummaryD";

				dmMMReportData->SetupSalesSummaryD(ReportControl->Start, ReportControl->End);
				if (ReportType == rtExcel)
				{
                    std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
                    ExcelDataSetsList->AddObject("Sales Summary Parameters",(TObject *)dmMMReportData->qrSSDParemeter);
				   	ExcelDataSetsList->AddObject("Sales Summary D",(TObject *)dmMMReportData->qrSalesSummaryD);
 				  	ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
				}
				else
				{
					if (rvMenuMate->SelectReport(ReportName, false))
					{
						AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
														"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
						rvMenuMate->SetParam("ReportRange", DateRange);
                        rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
                        rvMenuMate->SetParam("CurrentUser", frmLogin->CurrentUser.UserID +" at "+ Now().FormatString("ddddd 'at' hh:nn"));
                        rvMenuMate->SetParam("NameOfTaxPayer", dmMMReportData->nameOfTaxPayer);
                        rvMenuMate->SetParam("AddressOfTaxPayer", dmMMReportData->addressOfTaxPayer);
                        rvMenuMate->SetParam("TiNNumber", dmMMReportData->tinNumber);
                        rvMenuMate->SetParam("TerminalName", dmMMData->GetTerminalName());
                        rvMenuMate->SetParam("SerialNo", dmMMReportData->serialNo);
						rvMenuMate->Execute();
					}
					else
					{
						Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
					}
				}

			}


	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void TfrmReports::PrintEJournalReport(TReportControl *ReportControl)
{
	const AnsiString ReportName = "repESalesJournal";
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}
	try
	{

		dmMMReportData->SetupEJournal(ReportControl->Start, ReportControl->End);
		if (ReportType == rtExcel)
		{
			std::auto_ptr<TStringList> ExcelDataSetsList(new TStringList());
			ExcelDataSetsList->AddObject("Sales",(TObject *)dmMMReportData->qrEJournal);
			ExportToExcel( ExcelDataSetsList.get(),TreeView1->Selected->Text );
		}
		else
		{
			if (rvMenuMate->SelectReport(ReportName, false))
			{
				AnsiString DateRange =	"From " + ReportControl->Start.FormatString("ddddd 'at' hh:nn") +
												"\rto " + ReportControl->End.FormatString("ddddd 'at' hh:nn");
				rvMenuMate->SetParam("ReportRange", DateRange);
				rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
