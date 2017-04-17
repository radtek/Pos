//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WalletConfiguration.h"
#include "MMTouchKeyboard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmWalletConfiguration::TfrmWalletConfiguration(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmWalletConfiguration::FormShow(TObject *Sender)
{
    lbePassword->Caption = WalletPassword;
    lbeUserName->Caption = WalletUserName;
    lbeTerminalId->Caption = TerminalId;
    lbeSecurityToken->Caption = WalletSecurityToken;
    lbeMechentId->Caption = MerchentId;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWalletConfiguration::FormResize(TObject *Sender)
{
   if (Tag != Screen->Width)
   {
	  int Temp = Tag;
	  Tag = Screen->Width;

	  ScaleBy(Screen->Width, Temp);
   }
   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWalletConfiguration::btnMerchentIdMouseClick(TObject *Sender)
{
  MerchentId = GetData(MerchentId,"Enter Wallet Account Merchent Id",25);
  lbeMechentId->Caption = MerchentId;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWalletConfiguration::btnSecurityTokenMouseClick(TObject *Sender)
{
  WalletSecurityToken = GetData(WalletSecurityToken,"Enter Wallet Account Security Token",50);
  lbeSecurityToken->Caption = WalletSecurityToken;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWalletConfiguration::btnTerminalIdMouseClick(TObject *Sender)
{
  TerminalId = GetData(TerminalId,"Enter Wallet Account Terminal Id",25);
  lbeTerminalId->Caption = TerminalId;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWalletConfiguration::btnUserNameMouseClick(TObject *Sender)
{
  WalletUserName = GetData(WalletUserName,"Enter Wallet Account User Name",25);
  lbeUserName->Caption = WalletUserName;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWalletConfiguration::btnWalletPasswordMouseClick(TObject *Sender)

{
  WalletPassword = GetData(WalletPassword,"Enter Wallet Account Password",50);
  lbePassword->Caption = WalletPassword;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWalletConfiguration::btnOkMouseClick(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWalletConfiguration::btnCancelMouseClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
UnicodeString TfrmWalletConfiguration::GetData(UnicodeString inValue,UnicodeString inCaption,int inMaxLength)
{
   UnicodeString retVal = inValue;
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = inMaxLength;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->Caption = inCaption;
   frmTouchKeyboard->KeyboardText = inValue;
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  retVal = frmTouchKeyboard->KeyboardText.Trim();
   }
   return retVal;
}
