//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ShowEJournal.h"
#include "EJournalEngine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmEJournal *frmEJournal;
//---------------------------------------------------------------------------
__fastcall TfrmEJournal::TfrmEJournal(TComponent* Owner): TZForm(Owner), CurrentPrintout(new TMemoryStream)
{
   ExitCode = 0;
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
      EJournalEngine->CheckDataExist(FromDateTimePicker->Date,ToDateTimePicker->Date);
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
   ExitCode = 0;
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::btnReportDownAutoRepeat(TObject *Sender)
{
   ExitCode = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::btnReportUpAutoRepeat(TObject *Sender)
{
   ExitCode = 0;
}
//---------------------------------------------------------------------------
void TfrmEJournal::Execute()
{
    if(TGlobalSettings::Instance().ExcludeReceipt && TGlobalSettings::Instance().ExcludeXReport)
    {
        btnClosePrint->Visible = false;
    }
    btnSaveAsPDF->Enabled = false;
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
    if(ToDateTimePicker->Date > Now())
    {
       MessageBox("To Date can not be more than today's date", "Error", MB_OK + MB_ICONERROR);
       ToDateTimePicker->Date = Now();
    }
}
//---------------------------------------------------------------------------

