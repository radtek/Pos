//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SetupGlCodes.h"
#include "MMTouchKeyboard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmSetupGlCodes::TfrmSetupGlCodes(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetupGlCodes::btnOkMouseClick(TObject *Sender)
{
 ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetupGlCodes::btnCancelMouseClick(TObject *Sender)
{
 ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetupGlCodes::btnPointPurchasedMouseClick(TObject *Sender)
{
  ShowKeyBoard("Enter GL Code for Points Purchased",PointsPurchased);
  btnPointPurchased->Caption = "Points Purchased \r" + PointsPurchased;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetupGlCodes::btnPointSpentMouseClick(TObject *Sender)
{
  ShowKeyBoard("Enter GL Code for Points Spent",PointsSpent);
  btnPointSpent->Caption = "Points Spent \r" + PointsSpent;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetupGlCodes::btnVoucherPurchasedMouseClick(TObject *Sender)
{
  ShowKeyBoard("Enter GL Code for Voucher Purchased",VoucherPurchased);
  btnVoucherPurchased->Caption = "Voucher Purchased \r" + VoucherPurchased;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetupGlCodes::btnServiceChargeMouseClick(TObject *Sender)
{
  ShowKeyBoard("Enter GL Code for Service Charge",ServiceCharge);
  btnServiceCharge->Caption = "Service Charge \r" + ServiceCharge;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetupGlCodes::btnServiceChargeTaxMouseClick(TObject *Sender)
{
  ShowKeyBoard("Enter GL Code for Service Charge Tax",ServiceChargeTax);
  btnServiceChargeTax->Caption = "Service Charge Tax \r" + ServiceChargeTax;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetupGlCodes::FormShow(TObject *Sender)
{
  btnPointPurchased->Caption = "Points Purchased \r" + PointsPurchased;
  btnPointSpent->Caption = "Points Spent \r" + PointsSpent;
  btnVoucherPurchased->Caption = "Voucher Purchased \r" + VoucherPurchased;
  btnServiceCharge->Caption = "Service Charge \r" + ServiceCharge;
  btnServiceChargeTax->Caption = "Service Charge Tax \r" + ServiceChargeTax;
  btnRounding->Caption = "Rounding \r" + Rounding;
  btnTabAmountReceived->Caption = "Tab Deposit/Credit Received \r" + TabDepositCreditReceived;
  btnTabAmountRefunded->Caption = "Tab Deposit/Credit Refunded \r" + TabDepositCreditRefunded;
  btnFloatGlCode->Caption = "Float \r" + FloatGLCode;
  btnEftPosTip->Caption = "EftPos Tip \r" + EftPosTip;
}
//---------------------------------------------------------------------------
void TfrmSetupGlCodes::ShowKeyBoard(AnsiString KeyBoardCaption,AnsiString& KeyBoardText)
{
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
    frmTouchKeyboard->MaxLength = 25;
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->KeyboardText = KeyBoardText;
    frmTouchKeyboard->Caption = KeyBoardCaption;
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
       KeyBoardText = frmTouchKeyboard->KeyboardText;
    }
}
void __fastcall TfrmSetupGlCodes::btnRoundingMouseClick(TObject *Sender)
{
  ShowKeyBoard("Enter GL Code for Rounding",Rounding);
  btnRounding->Caption = "Rounding \r" + Rounding;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupGlCodes::btnTabAmountReceivedClick(TObject *Sender)
{
  ShowKeyBoard("Enter GL Code for Tab Dep./Cre. Received",TabDepositCreditReceived);
  btnTabAmountReceived->Caption = "Tab Deposit/Credit Received \r" + TabDepositCreditReceived;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupGlCodes::btnTabAmountRefundedClick(TObject *Sender)
{
  ShowKeyBoard("Enter GL Code for Tab Dep./Cre. Refunded",TabDepositCreditRefunded);
  btnTabAmountRefunded->Caption = "Tab Deposit/Credit Refunded \r" + TabDepositCreditRefunded;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupGlCodes::btnFloatGlCodeClick(TObject *Sender)
{
  ShowKeyBoard("Enter GL Code for Float ",FloatGLCode);
  btnFloatGlCode->Caption = "Float \r" + FloatGLCode;
}
//---------------------------------------------------------------------------


void __fastcall TfrmSetupGlCodes::btnEftPosTipMouseClick(TObject *Sender)
{
  ShowKeyBoard("Enter GL Code for EftPos Tip ",EftPosTip);
  btnEftPosTip->Caption = "EftPos Tip \r" + EftPosTip;
}
//---------------------------------------------------------------------------

