//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ExportForm.h"
#include "TimeClockForm.h"
#include "AccountsExportForm.h"
#include "MMData.h"
#include "Registration.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmExport::TfrmExport()
	: TForm((TComponent*)NULL)
{
   // TODO implement export of accounts to Xero
	dmMMData->Registered(&Registered, &Modules);
	TouchBtn1->Enabled = (Modules & eAccounting) && (CurrentConnection.AccountSystem != "Xero");
}
//---------------------------------------------------------------------------
void __fastcall TfrmExport::btnEditMenusMouseClick(TObject *Sender)
{
	std::auto_ptr<TfrmTimeClockExport> frm(new TfrmTimeClockExport());
	frm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmExport::btnExitMouseClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmExport::TouchBtn1MouseClick(TObject *Sender)
{
	std::auto_ptr<TfrmAccountsExport> frm(new TfrmAccountsExport());
	frm->ShowModal();
}
//---------------------------------------------------------------------------

