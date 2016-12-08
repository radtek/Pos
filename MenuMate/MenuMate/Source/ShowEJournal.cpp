//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ShowEJournal.h"
#include "ReceiptManager.h"
#include "EJournalEngine.h"
//#include "PageSizeHelp.h"
//#include "PDFUnit.h"
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
      EJournalEngine->CategorizeEJournal(FromDateTimePicker->Date,ToDateTimePicker->Date);
   }
   else
   {
      MessageBox("From Date can not be more than To Date", "Error", MB_OK + MB_ICONERROR);
   }
    ManagerReceipt->Receipt->Clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text="select a.REPORT from ZEDS a Where a.TRANS_DATE >= :From_DATE "
                                    "and a.TRANS_DATE < :To_DATE " ;

        IBInternalQuery->ParamByName("From_DATE")->AsDateTime = FromDateTimePicker->Date;
        IBInternalQuery->ParamByName("To_DATE")->AsDateTime = ToDateTimePicker->Date;

        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           IBInternalQuery->FieldByName("REPORT")->SaveToStream(ManagerReceipt->Receipt);
           ManagerReceipt->Receipt->Position++;
        }

        PopulateReport(ManagerReceipt->Receipt);

        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }



}
//---------------------------------------------------------------------------
void __fastcall TfrmEJournal::btnSavePDFMouseClick(TObject *Sender)
{
   ExitCode = 0;
// Decalre a pointer to TFileStream
	/*TFileStream *FStream;

	// Let the user call the Save Dialog
	if( SaveDialog1->Execute() )
	{
		// Use the constructor of the TFileStream to create a file
		try {
			FStream = new TFileStream(SaveDialog1->FileName, fmCreate);
			// In the pointer to FStream, add the contents of the Memo
			FStream->WriteComponent(memReceipt);
			// and the content of the Edit controls
			//FStream->WriteComponent("Hello");
		}
		__finally
		{
			// Since the pointer was created, delete it,
			// whether it was used or not
			delete FStream;
		}
	}*/
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
   //ExitCode = 0;
   memReceipt->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::btnReportUpAutoRepeat(TObject *Sender)
{
   //ExitCode = 0;
   memReceipt->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
//---------------------------------------------------------------------------
void TfrmEJournal::Execute()
{
   ShowModal();
    if(TGlobalSettings::Instance().ExcludeReceipt && TGlobalSettings::Instance().ExcludeXReport)
        btnClosePrint->Visible = false;
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

void __fastcall TfrmEJournal::FromDateTimePickerChange(TObject *Sender)
{
    //if(FromDateTimePicker->Date > Now())
      //  MessageBox("Invalid date", "Error", MB_OK + MB_ICONERROR);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::ToDateTimePickerClick(TObject *Sender)
{
    //if(FromDateTimePicker->Date > Now())
        //MessageBox("Invalid To date", "Error", MB_OK + MB_ICONERROR);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::ToDateTimePickerChange(TObject *Sender)
{
    //if(FromDateTimePicker->Date > Now())
       // MessageBox("Invalid To date", "Error", MB_OK + MB_ICONERROR);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEJournal::ToDateTimePickerCloseUp(TObject *Sender)
{
    if(ToDateTimePicker->Date > Now())
    {
       MessageBox("To Date Cannot Be More Than Today's Date", "Error", MB_OK + MB_ICONERROR);
       ToDateTimePicker->Date = Now();
    }
}
//---------------------------------------------------------------------------
void TfrmEJournal::PopulateReport(TMemoryStream *Receipt)
{
   	//TStringList *Lines = new TStringList;
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
void __fastcall TfrmEJournal::FormShow(TObject *Sender)
{
  Lines = new TStringList;
}
//---------------------------------------------------------------------------

