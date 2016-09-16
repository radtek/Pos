//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "XeroSettings.h"

#include "RemoteHostManager.h"
#include "MMTouchKeyboard.h"
#include "XeroIntegration.h"
#include "GlobalSettings.h"
#include "ManagerVariable.h"
#include "DeviceRealTerminal.h"
#include "MYOBIntegration.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

TfrmXeroSettings *frmXeroSettings;
//---------------------------------------------------------------------------
__fastcall TfrmXeroSettings::TfrmXeroSettings(TComponent* Owner)
	: TZForm(Owner)
{
	FValidated = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmXeroSettings::btnOKMouseClick(TObject *Sender)
{
     AwaitingPayment = cbSendAwaitingPayment->Checked;
	 ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmXeroSettings::btnCancelMouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmXeroSettings::FormShow(TObject *Sender)
{
	Validate();
	InitSettings();
    AwaitingPayment =  TGlobalSettings::Instance().AwaitingPaymentInXero;
    cbSendAwaitingPayment->Checked = AwaitingPayment;
}
//---------------------------------------------------------------------------
void __fastcall TfrmXeroSettings::rbXeroLocalClick(TObject *Sender)
{
	RefreshSettings();
}
//---------------------------------------------------------------------------
void __fastcall TfrmXeroSettings::rbXeroRemoteClick(TObject *Sender)
{
	RefreshSettings();
}
//---------------------------------------------------------------------------
void __fastcall TfrmXeroSettings::btnEditHostNameMouseClick(TObject *Sender)
{
	if( !( ( TControl *)Sender )->Enabled )
		return;

	UnicodeString text = CheckMachineName( AccountingMachineName );

    if(Caption.Pos("MYOB") == 0)
    {
        if( EditText( "Xero Station Name", text ) )
        {
            AccountingMachineName = text;
        }
    }
    else
    {
        if( EditText( "MYOB Station Name", text ) )
        {
            AccountingMachineName = text;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmXeroSettings::btnEditFolderNameMouseClick(TObject *Sender)
{
	if( !( ( TControl *)Sender )->Enabled )
		return;

	UnicodeString text = CheckFolderName( AccountingFolderPath );

    if(Caption.Pos("MYOB") == 0)
    {
        if( EditText( "Xero Folder Name", text ) )
        {
            AccountingFolderPath = text;
        }
    }
    else
    {
        if( EditText( "MYOB Folder Name", text ) )
        {
            AccountingFolderPath = text;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmXeroSettings::btnEditUserNameMouseClick(TObject *Sender)
{
	if( !( ( TControl *)Sender )->Enabled )
		return;

	UnicodeString text = CheckUserName( AccountingUserName );

    if(Caption.Pos("MYOB") == 0)
    {
        if( EditText( "Xero Remote Host: User Name", text ) )
        {
            AccountingUserName = text;
        }
    }
    else
    {
        if( EditText( "MYOB Remote Host: User Name", text ) )
        {
            AccountingUserName = text;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmXeroSettings::btnEditPasswordMouseClick(TObject *Sender)
{
	if( !( ( TControl *)Sender )->Enabled )
		return;

	UnicodeString text = CheckPassword( AccountingPassword );

    if(Caption.Pos("MYOB") == 0)
    {
        if( EditText( "Xero Remote Host:  Password", text ) )
        {
            AccountingPassword = text;
        }
    }
    else
    {
        if( EditText( "MYOB Remote Host:  Password", text ) )
        {
            AccountingPassword = text;
        }
    }
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::SetAndValidate( AnsiString inMachineName, AnsiString inFolderPath, AnsiString inUserName, AnsiString inPassword, AccountingType accountingType)
{
	AccountingMachineName = inMachineName;
	AccountingFolderPath  = inFolderPath;
	AccountingUserName    = inUserName;
	AccountingPassword    = inPassword;
    if(accountingType == eAccountingMYOB)
        CustomiseForMYOB();
	Validate();
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::CustomiseForMYOB()
{
    Caption = "MYOB Integration Settings";
    cbSendAwaitingPayment->Enabled = false;
    cbSendAwaitingPayment->Caption = "Send Account Charges to Awaiting Payment in MYOB";
    rbXeroLocal->Caption = "Local MYOB Service";
    Label1->Caption = "Connect to MYOB Integration Service  in the local machine. The Invoice Folder Name is only needed" ;
    rbXeroRemote->Caption = "Remote MYOB Service";
    Label2->Caption = "Connect to MYOB Integration Service  in an remote machine. ";
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::Validate()
{
	if( FValidated )
	  return;

	bool isLocalHost = TRemoteHostManager::Instance().IsLocalHost( AccountingMachineName );

	if( isLocalHost )
	{
		FRemoteHostName   = "";
		FRemoteUserName   = "";
		FRemotePassword   = "";
		FRemoteFolderName = "";
		FLocalFolderName  = AccountingFolderPath;
	}
	else
	{
		FRemoteHostName   = AccountingMachineName;
		FRemoteUserName   = AccountingUserName;
		FRemotePassword   = AccountingPassword;
		FRemoteFolderName = AccountingFolderPath;
		FLocalFolderName  = "";
	}

	rbXeroLocal->Checked  = isLocalHost;
	rbXeroRemote->Checked = !isLocalHost;
	FValidated = true;
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::LocalHost()
{
   return TRemoteHostManager::Instance().LocalHost();
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::POS1()
{
	return "POS1";
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::XeroInvoices()
{
	return "Xero Invoices";
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::EmptyUnicodeString()
{
	return "";
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::CheckMachineName( UnicodeString inMachineName )
{
	return LocalHostChecked() ? CheckLocalMachineName( inMachineName ) : CheckRemoteMachineName( inMachineName );
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::CheckLocalMachineName( UnicodeString inMachineName )
{
	return ( inMachineName == "" ) ? LocalHost() : inMachineName;
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::CheckRemoteMachineName( UnicodeString inMachineName )
{
	return ( inMachineName == "" ) ? POS1() : inMachineName;
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::CheckFolderName( UnicodeString inFolderName )
{
	return LocalHostChecked() ? CheckLocalFolderName( inFolderName ) : CheckRemoteFolderName( inFolderName );
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::CheckLocalFolderName( UnicodeString inFolderName )
{
	UnicodeString result = inFolderName;

	//...........................................

	UnicodeString text = inFolderName;

	if( text == "" )
	{
        if(Caption.Pos("MYOB") == 0)
        {
            if( DirectoryExists( ExtractFilePath( Application->ExeName ) + XERO_INTEGRATION_FOLDER ) )
            {
                text = ExtractFilePath( Application->ExeName ) + XERO_INTEGRATION_FOLDER;
            }
            else
            {
                text = "C:\\Program Files\\MenuMate\\" + XERO_INTEGRATION_FOLDER;
            }
        }
        else
        {
            if( DirectoryExists( ExtractFilePath( Application->ExeName ) + MYOB_INTEGRATION_FOLDER ) )
            {
                text = ExtractFilePath( Application->ExeName ) + MYOB_INTEGRATION_FOLDER;
            }
            else
            {
                text = "C:\\Program Files\\MenuMate\\" + MYOB_INTEGRATION_FOLDER;
            }
        }
	}

	result = text;

	//...........................................

	return result;
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::CheckRemoteFolderName( UnicodeString inFolderName )
{
	return ( inFolderName == "" ) ? XeroInvoices() : inFolderName;
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::CheckUserName( UnicodeString inUserName )
{
	return LocalHostChecked() ? EmptyUnicodeString() : inUserName;
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::CheckPassword( UnicodeString inPassword )
{
	return LocalHostChecked() ? EmptyUnicodeString() : inPassword;
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::readAccountingMachineName()
{
	return edHostName->Text;
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::readAccountingFolderPath()
{
	return edFolderName->Text;
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::readAccountingUserName()
{
	return edUserName->Text;
}
//---------------------------------------------------------------------------
UnicodeString TfrmXeroSettings::readAccountingPassword()
{
	return edPassword->Text;
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::writeAccountingMachineName( UnicodeString inName )
{
	edHostName->Text = inName;
	if( !LocalHostChecked() )
		FRemoteHostName = inName;

	FValidated = false;
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::writeAccountingFolderPath( UnicodeString inPath )
{
	edFolderName->Text = inPath;
	if( !LocalHostChecked() )
		FRemoteFolderName = inPath;

	FValidated = false;
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::writeAccountingUserName( UnicodeString inName )
{
	edUserName->Text = inName;
	FRemoteUserName  = inName;

	FValidated = false;
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::writeAccountingPassword( UnicodeString inPassw )
{
	edPassword->Text = inPassw;
	FRemotePassword  = inPassw;

	FValidated = false;
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::InitSettings()
{
	EnableAndClearEdit( !rbXeroLocal->Checked, edHostName, LocalHost(), FRemoteHostName  );
	EnableAndClearEdit( !rbXeroLocal->Checked, edUserName, "", FRemoteUserName );
	EnableAndClearEdit( !rbXeroLocal->Checked, edPassword, "", FRemotePassword );
	InitEdit( LocalHostChecked(), edFolderName, FLocalFolderName, FRemoteFolderName );
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::RefreshSettings()
{
	EnableAndClearEdit( !rbXeroLocal->Checked, edHostName, LocalHost(), FRemoteHostName  );
	EnableAndClearEdit( !rbXeroLocal->Checked, edUserName, "", FRemoteUserName );
	EnableAndClearEdit( !rbXeroLocal->Checked, edPassword, "", FRemotePassword );

	RefreshEdit( LocalHostChecked(), edFolderName, FLocalFolderName, FRemoteFolderName );
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::EnableAndClearEdit( bool inEnabled, TEdit* inEdit, UnicodeString inLocalText, UnicodeString &inRemoteText )
{
	inEdit->Enabled = inEnabled;

	inEdit->Text        = inEnabled ? inRemoteText : inLocalText;
	inEdit->Font->Color = inEnabled ? clBlack      : clBlack;
	inEdit->Color       = inEnabled ? clWhite      : clSilver;
}
//---------------------------------------------------------------------------
bool TfrmXeroSettings::LocalHostChecked()
{
	return rbXeroLocal->Checked;
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::InitEdit( bool inLocal, TEdit* inEdit, UnicodeString inLocalText, UnicodeString &inRemoteText )
{
	if( inLocal )
	{
	   inEdit->Text = inLocalText;
	}
	else
	{
	   inEdit->Text = inRemoteText;
	}
}
//---------------------------------------------------------------------------
void TfrmXeroSettings::RefreshEdit( bool inLocal, TEdit* inEdit, UnicodeString inLocalText, UnicodeString &inRemoteText )
{
	if( inLocal )
	{
	   inRemoteText = inEdit->Text;
	   inEdit->Text = inLocalText;
	}
	else
	{
	   inLocalText  = inEdit->Text;
	   inEdit->Text = inRemoteText;
	}
}
//---------------------------------------------------------------------------
bool TfrmXeroSettings::EditText( UnicodeString inCaption, UnicodeString &inText )
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

//---------------------------------------------------------------------------

