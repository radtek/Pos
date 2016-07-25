//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PocketVoucher.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmPocketVoucher::TfrmPocketVoucher(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------

AnsiString TfrmPocketVoucher::VoucherCode()
{
	return edVoucherCode->Text;
}

//---------------------------------------------------------------------------

void __fastcall TfrmPocketVoucher::FormShow(TObject *Sender)
{
	FormResize(NULL);
	edVoucherCode->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPocketVoucher::edVoucherCodeMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 20;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edVoucherCode->Text;
	frmTouchKeyboard->Caption = "Enter Voucher Code";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		edVoucherCode->Text = frmTouchKeyboard->KeyboardText;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPocketVoucher::FormResize(TObject *Sender)
{
//	if (Tag != Screen->Width)
//	{
//		int Temp = Tag;
//		Tag = Screen->Width;
//		ScaleBy(Screen->Width, Temp);
//	}
//	Left				= (Screen->Width - Width) / 2;
//   Top				= (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPocketVoucher::tbtnOkMouseClick(TObject *Sender)
{
    if((edVoucherCode->Text != ""))
    {
        ModalResult = mrOk;
    }
    else
    {
        if(MessageBox("Please Enter Voucher Code", "Electronic Voucher Info", MB_OK + MB_ICONWARNING) == IDOK)
        {
           CapturedAllData();
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPocketVoucher::tBtnCancelMouseClick(TObject *Sender)
{
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPocketVoucher::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if(Key == VK_RETURN)
  {
    tbtnOkMouseClick(tbtnOk);
  }
 }
 //---------------------------------------------------------------------------
void TfrmPocketVoucher::CapturedAllData()
{
  if(edVoucherCode->Text == "")
  {
     edVoucherCode->SetFocus();
  }
}
//---------------------------------------------------------------------------





