//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ShowEJournal.h"
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

void __fastcall TfrmEJournal::FromDateOnChange(TObject *Sender)
{
    if(FromDateTimePicker->DateTime > Now())
        	  MessageBox("From Date can not more than current Date ", "Warning",
		 MB_ICONWARNING + MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::ToDateOnChange(TObject *Sender)
{
    int i = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEJournal::btnGenerateMouseClick(TObject *Sender)
{
    int i = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEJournal::btnSavePDFMouseClick(TObject *Sender)
{
    int i = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::btnPrintMouseClick(TObject *Sender)
{
    int i = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::btnReportDownAutoRepeat(TObject *Sender)
{
    int i = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::btnReportUpAutoRepeat(TObject *Sender)
{
    int i = 0;
}
//---------------------------------------------------------------------------
void TfrmEJournal::Execute()
{
      FromDateTimePicker->Date = Now();
      ToDateTimePicker->Date = Now();
	  ShowModal();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEJournal::FromDateTimePickerExit(TObject *Sender)
{
    if(FromDateTimePicker->DateTime > Now())
        	  MessageBox("From Date can not more than current Date ", "Warning",
		 MB_ICONWARNING + MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::FromDateTimePickerCloseUp(TObject *Sender)
{
    if(FromDateTimePicker->DateTime > Now())
        	  MessageBox("From Date can not more than current Date ", "Warning",
		 MB_ICONWARNING + MB_OK);
}
//---------------------------------------------------------------------------

