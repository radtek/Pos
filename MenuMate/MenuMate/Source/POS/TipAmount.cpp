//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TipAmount.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchNumpad"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmTipAmount::TfrmTipAmount(TComponent* Owner)
    : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmTipAmount::btnCancelTippingMouseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTipAmount::btnProcessTipOnReceiptMouseClick(TObject *Sender)
{
	if (CustomMessageBox("Are you sure you wish to add a tip on this transaction ?" ,"Add a tip to transaction",MB_ICONQUESTION,"Yes","No") == IDOK)
	{
		SelectedTipAmount = lbTipDisplay->Numeric();
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmTipAmount::FormResize(TObject *Sender)
{
    //TODO: add code for form resize event
}
//---------------------------------------------------------------------------

void __fastcall TfrmTipAmount::FormCreate(TObject *Sender)
{
    FormResize(this);
}
//---------------------------------------------------------------------------

