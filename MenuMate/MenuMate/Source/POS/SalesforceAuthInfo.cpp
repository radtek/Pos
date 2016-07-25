//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SalesforceAuthInfo.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmSalesforceAuthInfo *frmSalesforceAuthInfo;
//---------------------------------------------------------------------------
__fastcall TfrmSalesforceAuthInfo::TfrmSalesforceAuthInfo(TComponent* Owner)
    : TZForm(Owner)
{
    //implement method
}
//---------------------------------------------------------------------------

void __fastcall TfrmSalesforceAuthInfo::btnUpdateCredentialsMouseClick(TObject *Sender)

{
    if(isValid())
        ModalResult = mrOk;
    else
        MessageBox("Please ensure every field is filled in before clicking OK button","Invalid data",MB_ICONSTOP);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSalesforceAuthInfo::btnCancelMouseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSalesforceAuthInfo::txtSFUsernameClick(TObject *Sender)
{
   if(EditText("Salesforce Username",Username) && validateUsername())
        txtSFUsername->Text = Username;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSalesforceAuthInfo::txtSFPasswordClick(TObject *Sender)
{
    if(EditText("Salesforce Password",Password) && validatePassword())
        txtSFPassword->Text = Password;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSalesforceAuthInfo::txtSFTokenClick(TObject *Sender)
{
    if(EditText("Salesforce Security Token", SecurityToken) && validateSecurityToken())
        txtSFToken->Text = SecurityToken;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSalesforceAuthInfo::FormResize(TObject *Sender)
{
    //TODO: add code for form resize event
}
//---------------------------------------------------------------------------

void __fastcall TfrmSalesforceAuthInfo::FormCreate(TObject *Sender)
{
    FormResize(this);
}
//---------------------------------------------------------------------------

TfrmSalesforceAuthInfo* TfrmSalesforceAuthInfo::Create(TComponent* Owner)
{
	return new TfrmSalesforceAuthInfo(Owner);
}
//--------------------------------------------------------------------------

bool TfrmSalesforceAuthInfo::EditText( UnicodeString inCaption, UnicodeString &inText )
{
	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 250;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = inText;

	frmTouchKeyboard->Caption = inCaption;
	if( frmTouchKeyboard->ShowModal() == mrOk )
	{
		inText = frmTouchKeyboard->KeyboardText;

		return true;
	}
	else
	{
        return false;
    }
}

bool TfrmSalesforceAuthInfo::isValid()
{
    return validateUsername()
            && validatePassword()
            && validateSecurityToken();
}

void TfrmSalesforceAuthInfo::displaySalesforceCredentials()
{
    txtSFUsername->Text = validateUsername() ? Username : EmptyUnicodeString();
    txtSFPassword->Text = validatePassword() ? Password : EmptyUnicodeString();
    txtSFToken->Text = validateSecurityToken() ? SecurityToken : EmptyUnicodeString();
}

bool TfrmSalesforceAuthInfo::validateUsername()
{
    return Username.Length() > 0;
}

bool TfrmSalesforceAuthInfo::validatePassword()
{
    return Password.Length() > 0;
}

bool TfrmSalesforceAuthInfo::validateSecurityToken()
{
    return SecurityToken.Length() > 0;
}

UnicodeString TfrmSalesforceAuthInfo::EmptyUnicodeString()
{
	return "";
}
