//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "help.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmHelp *frmHelp;
//---------------------------------------------------------------------------
__fastcall TfrmHelp::TfrmHelp(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmHelp::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
