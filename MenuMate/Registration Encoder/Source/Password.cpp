//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Password.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmPassword *frmPassword;
//---------------------------------------------------------------------------
__fastcall TfrmPassword::TfrmPassword(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool TfrmPassword::InputQuery(AnsiString &Text)
{
	if (ShowModal() == mrOk)
	{
		Text = edPassword->Text;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPassword::btnOkClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPassword::btnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

