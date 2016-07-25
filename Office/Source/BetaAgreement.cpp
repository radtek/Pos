//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BetaAgreement.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmBetaAgreement::TfrmBetaAgreement(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmBetaAgreement::Button1Click(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBetaAgreement::Button2Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
 