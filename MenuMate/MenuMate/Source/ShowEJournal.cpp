//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ShowEJournal.h"
#include "ReceiptManager.h"
#include "Processing.h"
#include "ServingTime.h"
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
 //-----------------------------------------------------------------------
 void TfrmEJournal ::FromStartDateTimePicker()
 {

     std::auto_ptr<TfrmServingTime> frmServingTime(TfrmServingTime::Create<TfrmServingTime>(this));
     frmServingTime->Caption = "From Date: " + FromDateTimePicker->Date.FormatString("DD/MM/YYYY");
     TDateTime DateTime;
     string a;
     frmServingTime->btnCancel->Visible = false;
     frmServingTime->Left = (Screen->Width - frmServingTime->Width) / 2;
     frmServingTime->Top  = (Screen->Height - frmServingTime->Height) / 2;
     frmServingTime->ShowModal() ;
     FromDateTimePicker->Time = frmServingTime->Time1;
     lblfromdatetime->Caption =  FromDateTimePicker->Date;
     if(lblfromdatetime->Caption.Pos("a.m.") == 0 && lblfromdatetime->Caption.Pos("p.m.") == 0)
     {

       UnicodeString strValue = lblfromdatetime->Caption;
        strValue += " 00:00:00 a.m.";
        lblfromdatetime->Caption = strValue;
     }

 }

//---------------------------------------------------------------------------
void __fastcall TfrmEJournal::FromDateTimePickerCloseUp(TObject *Sender)
{

   if(IsConsolidatedZed)
    {

      FromStartDateTimePicker();
    }
    if(FromDateTimePicker->Date > Now())
    {
       MessageBox("From Date can not be more than today's date", "Error", MB_OK + MB_ICONERROR);
       FromDateTimePicker->Date = Now();

    }
}
//-------------------------------------------------------------------------
 void TfrmEJournal ::ToStartDateTimePicker()
{

     std::auto_ptr<TfrmServingTime> frmServingTime(TfrmServingTime::Create<TfrmServingTime>(this));
     frmServingTime->Caption = "To Date: " + ToDateTimePicker->DateTime.FormatString("DD/MM/YYYY");
     frmServingTime->btnCancel->Visible = false;
     frmServingTime->Left = (Screen->Width - frmServingTime->Width) / 2;
     frmServingTime->Top  = (Screen->Height - frmServingTime->Height) / 2;
     frmServingTime->ShowModal() ;
     ToDateTimePicker->Time = frmServingTime->Time1;
     lbltodatetime->Caption =  ToDateTimePicker->Date;
     if(lbltodatetime->Caption.Pos("a.m.") == 0 && lbltodatetime->Caption.Pos("p.m.") == 0)
     {
        UnicodeString strValue = lbltodatetime->Caption;
        strValue += " 00:00:00 a.m.";
        lbltodatetime->Caption = strValue;
     }

}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::ToDateTimePickerCloseUp(TObject *Sender)
{

     if(IsConsolidatedZed)
     {
      ToStartDateTimePicker();
     }

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
    if(CompareDateRangeForConolidatedZed(FromDateTimePicker->DateTime, ToDateTimePicker->DateTime))
    {
        std::auto_ptr<TEJournalEngine> EJournalEngine(new TEJournalEngine());
        if(EJournalEngine->CheckZedDataExistsForConolidatedZed(FromDateTimePicker->Date,ToDateTimePicker->Date, deviceName))
        {
            ManagerReceipt->Receipt = EJournalEngine->ExtractConsolidatedZedReport(FromDateTimePicker->Date,ToDateTimePicker->Date, deviceName);
        }
        CheckAndPopulateData();
    }
    else
    {
       MessageBox("Date Filter cannot be more than 1 month", "Error", MB_OK + MB_ICONERROR);
       memReceipt->Clear();
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
      if(!IsConsolidatedZed)
      {
         MessageBox("No Sales data found for selected date range. Please check!", "Information", MB_OK + MB_ICONERROR);
      }
      else
      {
         MessageBox("No Zed data found for selected date range. Please check!", "Information", MB_OK + MB_ICONERROR);
      }
      memReceipt->Clear();
      btnClosePrint->Enabled = false;
   }
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
bool TfrmEJournal::CompareDateRangeForConolidatedZed(TDateTime fromdate, TDateTime todate)
{
    bool retVal = true;
    int value  = Dateutils::DaysBetween(fromdate, todate);
    if(value > 31)
    {
       retVal = false;
    }
    return retVal;

}


