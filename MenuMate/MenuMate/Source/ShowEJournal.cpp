//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ShowEJournal.h"
#include "ReceiptManager.h"
#include "Processing.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

TfrmEJournal *frmEJournal;
//---------------------------------------------------------------------------
__fastcall TfrmEJournal::TfrmEJournal(TComponent* Owner): TZForm(Owner), CurrentPrintout(new TMemoryStream)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::btnCancelMouseClick(TObject *Sender)
{
   ExitCode = 0;
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEJournal::btnGenerateMouseClick(TObject *Sender)
{
   if(FromDateTimePicker->Date <= ToDateTimePicker->Date)
   {
      if(!IsConsolidatedZed)
      {
          std::auto_ptr<TEJournalEngine> EJournalEngine(new TEJournalEngine());
          EJournalType type = EJournalEngine->CategorizeEJournal(FromDateTimePicker->Date,ToDateTimePicker->Date);
          ExtractEJournalReport(type);
      }
      else
      {
          ExtractEJournalReport(eConsolidatedZed);
      }
   }
   else
   {
      MessageBox("From Date can not be more than To Date", "Error", MB_OK + MB_ICONERROR);
   }

}
//---------------------------------------------------------------------------
void __fastcall TfrmEJournal::btnSavePDFMouseClick(TObject *Sender)
{
   ExitCode = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::btnPrintMouseClick(TObject *Sender)
{
    TPrintout *Printout = new TPrintout;
    Printout->Printer = TComms::Instance().ReceiptPrinter;
    Printout->PrintToPrinterStream(ManagerReceipt->Receipt,TComms::Instance().ReceiptPrinter.UNCName());
    delete Printout;
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::btnReportDownAutoRepeat(TObject *Sender)
{
   memReceipt->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::btnReportUpAutoRepeat(TObject *Sender)
{
   memReceipt->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
//---------------------------------------------------------------------------
void TfrmEJournal::Execute()
{

    btnClosePrint->Enabled = false;
    btnSaveAsPDF->Visible = false;
    FromDateTimePicker->Date = Now();
    ToDateTimePicker->Date = Now();
    memReceipt->Clear();
    ShowModal();
}

//---------------------------------------------------------------------------
void __fastcall TfrmEJournal::FromDateTimePickerCloseUp(TObject *Sender)
{

    if(FromDateTimePicker->Date > Now())
    {
       MessageBox("From Date can not be more than today's date", "Error", MB_OK + MB_ICONERROR);
       FromDateTimePicker->Date = Now();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::ToDateTimePickerCloseUp(TObject *Sender)
{
    if((TDateTime)ToDateTimePicker->DateTime.DateString() > Now().CurrentDate())
    {
       MessageBox("To Date Cannot Be More Than Today's Date", "Error", MB_OK + MB_ICONERROR);
       ToDateTimePicker->Date = Now();
    }
}
//---------------------------------------------------------------------------
void TfrmEJournal::PopulateReport(TMemoryStream *Receipt)
{
   	TStringList *Lines = new TStringList;
	try
	{
		ManagerReceipt->Get(Lines);
		memReceipt->Lines->Assign(Lines);
	}
	__finally
	{
		delete Lines;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TfrmEJournal::ExtractEJournalReport(EJournalType type)
{
   std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
   Processing->Message = "Processing Data for E-Journal.Please Wait...";
   Processing->Show();
   AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
   switch(type)
   {
      case eZed:
      ExtractZedReport(deviceName);
      break;
      case eZedX:
      ExtractZedAndXReport(deviceName);
      break;
      case eZEDReceiptX:
      ExtractZedReceiptAndXReport(deviceName);
      break;
      case eZedReceipt:
      ExtractZedReceiptReport(deviceName);
      break;
      case eConsolidatedZed:
      Processing->Refresh();
      Processing->Message = "Processing Data for Consolidated Zed.Please Wait...";
      Processing->Show();
      ExtractConsolidatedZedReport(deviceName);
      break;
   }
   Processing->Close();
}
//---------------------------------------------------------------------------
void TfrmEJournal::ExtractZedReport(AnsiString deviceName)
{
   ManagerReceipt->Receipt->Clear();
   std::auto_ptr<TEJournalEngine> EJournalEngine(new TEJournalEngine());
   ManagerReceipt->Receipt = EJournalEngine->ExtractZedReport(FromDateTimePicker->Date,ToDateTimePicker->DateTime, deviceName);
   CheckAndPopulateData();
}
//---------------------------------------------------------------------------
void TfrmEJournal::ExtractZedAndXReport(AnsiString deviceName)
{
   ManagerReceipt->Receipt->Clear();
   std::auto_ptr<TEJournalEngine> EJournalEngine(new TEJournalEngine());
   ManagerReceipt->Receipt = EJournalEngine->ExtractZedAndXReport(FromDateTimePicker->Date,ToDateTimePicker->DateTime, deviceName);
   CheckAndPopulateData();
}
//---------------------------------------------------------------------------
void TfrmEJournal::ExtractZedReceiptAndXReport(AnsiString deviceName)
{
   ManagerReceipt->Receipt->Clear();
   std::auto_ptr<TEJournalEngine> EJournalEngine(new TEJournalEngine());
   ManagerReceipt->Receipt = EJournalEngine->ExtractZedReceiptAndXReport(FromDateTimePicker->Date,ToDateTimePicker->Date, deviceName);
   CheckAndPopulateData();
}
//---------------------------------------------------------------------------
void TfrmEJournal::ExtractZedReceiptReport(AnsiString deviceName)
{
   ManagerReceipt->Receipt->Clear();
   std::auto_ptr<TEJournalEngine> EJournalEngine(new TEJournalEngine());
   ManagerReceipt->Receipt = EJournalEngine->ExtractZedReceiptReport(FromDateTimePicker->Date,ToDateTimePicker->Date, deviceName);
   CheckAndPopulateData();
}
//---------------------------------------------------------------------------
void TfrmEJournal::ExtractConsolidatedZedReport(AnsiString deviceName)
{
    ManagerReceipt->Receipt->Clear();
    if(CheckDateRangeForConolidatedZed())
    {
        std::auto_ptr<TEJournalEngine> EJournalEngine(new TEJournalEngine());
        ManagerReceipt->Receipt = EJournalEngine->ExtractConsolidatedZedReport(FromDateTimePicker->Date,ToDateTimePicker->Date, deviceName);
        CheckAndPopulateData();
    }
    else
    {
       MessageBox("Date Filter cannot be more than 30 days", "Error", MB_OK + MB_ICONERROR);
    }

}
//---------------------------------------------------------------------------

void TfrmEJournal::CheckAndPopulateData()
{
   if(ManagerReceipt->Receipt->Size > 0)
   {
      PopulateReport(ManagerReceipt->Receipt);
      btnClosePrint->Enabled = true;
   }
   else
   {
      MessageBox("No Sales data found for selected date range. Please check!", "Information", MB_OK + MB_ICONERROR);
      memReceipt->Clear();
      btnClosePrint->Enabled = false;
   }
}

bool TfrmEJournal::CheckDateRangeForConolidatedZed()
{
    bool retVal = true;
    TDateTime date = ToDateTimePicker->Date - FromDateTimePicker->Date;
    int date_val = StrToInt(date.FormatString("d"));

    unsigned short fromdd, fromdm, fromdy;
    unsigned short todd, todm, tody;
    unsigned short th, tm, ts, tms;
    FromDateTimePicker->Date.DecodeDate(&fromdy, &fromdm, &fromdd);
    ToDateTimePicker->Date.DecodeDate(&tody, &todm, &todd);

    int fromDay, toDay;
    int fromMonth, toMonth;
    int fromYear, toYear;

    fromDay = fromdd;
    toDay = todd;

    fromMonth = fromdm;
    toMonth = todm;
    fromYear = fromdy;
    toYear = tody;

    if((toYear - tody) == 0)
    {
       retVal = CalculateDateRangeForConolidatedZed(toMonth, fromMonth, fromDay, toDay, fromYear);
    }
    else
    {
       //retVal = false;
       retVal = CalculateDateRangeForConolidatedZed(toMonth, fromMonth, fromDay, toDay, fromYear);
    }
    return retVal;
}

void __fastcall TfrmEJournal::FormShow(TObject *Sender)
{
    if(IsConsolidatedZed)
    {
       Caption = "Consolidated Zed";
    }
    else
    {
       Caption = "E-Journal";
    }
}
//---------------------------------------------------------------------------

bool TfrmEJournal::CalculateDateRangeForConolidatedZed(int toMonth, int fromMonth, int fromDay, int toDay, int fromYear)
{
    bool retVal = true;
    if((toMonth - fromMonth) >= 0 && (toMonth - fromMonth) < 2)
    {
        if(fromMonth == cJan || fromMonth == 3 || fromMonth == 5 || fromMonth == 7 || fromMonth == 8 || fromMonth == 10 || fromMonth == 12)
        {
            int totalday = 31 - fromDay;
            totalday += toDay;
            if(!(totalday >= 31))
            {
                retVal = false;
            }
        }
        if(fromMonth == 4 || fromMonth == 6 || fromMonth == 9 || fromMonth == 11)
        {
            int totalday = 30 - fromDay;
            totalday += toDay;
            MessageBox(IntToStr(totalday), "30", MB_OK + MB_ICONERROR);
            if(totalday >= 31)
            {
                retVal = false;
            }
        }
        if(fromMonth == 2)
        {
            int totalday = 0;
            if((fromYear % 4) == 0)
            {
                totalday =  29 - fromDay;
                totalday += toDay;
            }
            else
            {
                totalday = 28 - fromDay;
                totalday += toDay;
            }
            if(totalday >= 31)
            {
                retVal = false;
            }
        }

    }
    else
    {
        retVal = false;
    }

  return retVal;
}
