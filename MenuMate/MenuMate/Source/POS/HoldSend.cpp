//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HoldSend.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmHoldSend *frmHoldSend;
//---------------------------------------------------------------------------
__fastcall TfrmHoldSend::TfrmHoldSend(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmHoldSend::tbtnHoldMouseClick(TObject *Sender)
{
	RetVal = IDCANCEL;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmHoldSend::tbtnSendMouseClick(TObject *Sender)
{
	RetVal = IDOK;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmHoldSend::FormShow(TObject *Sender)
{
	RetVal = 0;
}
//---------------------------------------------------------------------------


