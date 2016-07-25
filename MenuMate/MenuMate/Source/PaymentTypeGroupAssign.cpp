//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PaymentTypeGroupAssign.h"
#include "DBGroups.h"
#include "Contact.h"
#include "DBContacts.h"
#include "MMMessageBox.h"
#include "MMContactInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"
TfrmAssign *frmAssign;
TForm * TfrmAssign::WinOwner = NULL;
//---------------------------------------------------------------------------
__fastcall TfrmAssign::TfrmAssign(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
TfrmAssign *TfrmAssign::Create(TForm* Owner)
{
   WinOwner = Owner;
   return new TfrmAssign(Owner);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAssign::btnCloseMouseClick(TObject *Sender)
{
	ModalResult = mrOk;
}
