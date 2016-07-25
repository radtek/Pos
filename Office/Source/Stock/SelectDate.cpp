//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectDate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSelectDate *frmSelectDate;
//---------------------------------------------------------------------------
__fastcall TfrmSelectDate::TfrmSelectDate(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDate::CalendarDblClick(TObject *Sender)
{
	ModalResult = mrOk;	
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDate::FormShow(TObject *Sender)
{
	btnOk->SetFocus();
}
//---------------------------------------------------------------------------

