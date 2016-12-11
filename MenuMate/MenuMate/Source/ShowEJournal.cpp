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
      std::auto_ptr<TEJournalEngine> EJournalEngine(new TEJournalEngine());
      EJournalType type = EJournalEngine->CategorizeEJournal(FromDateTimePicker->Date,ToDateTimePicker->Date);
      ExtractEJournalReport(type);
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
void TfrmEJournal::Execute(){

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

