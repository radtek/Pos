//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EFTPOSConfiguration.h"
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
#include "MMTouchKeyboard.h"
#include "EftPos.h"
#include "EftposAdyen.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmEFTPOSConfig *frmEFTPOSConfig;
//---------------------------------------------------------------------------
__fastcall TfrmEFTPOSConfig::TfrmEFTPOSConfig(TComponent* Owner)
    : TZForm(Owner)
{
    UpdateGUI();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEFTPOSConfig::tbOKMouseClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().EnableEftPosAdyen)
    {
        EnableEFTPOSTerminal();
    }
    Close();
}
//---------------------------------------------------------------------------
bool TfrmEFTPOSConfig::EnableEFTPOSTerminal()
{
    EftPos = new TEftposAdyen();
    EftPos->Initialise();
    if(EftPos->Enabled)
        MessageBox("Details for Adyen integration are verified.\rPlease make sure integrated EFTPOS payment type is configured under Payments.","Info",MB_OK + MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEFTPOSConfig::tbEFTPOSURLMouseClick(TObject *Sender)
{
	std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	frmTouchKeyboard->MaxLength = 0;
	frmTouchKeyboard->AllowCarriageReturn = true;
	frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = tbEFTPOSURL->Caption;
    if(TGlobalSettings::Instance().EnableEftPosSmartPay)
    {
	    frmTouchKeyboard->Caption = "Enter SmartLink Ip Address";
        frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().EftPosSmartPayIp;
    }
    else if(TGlobalSettings::Instance().EnableEftPosSmartConnect)
    {
	    frmTouchKeyboard->Caption = "Enter SmartConnect Ip Address";
        frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().EftPosSmartPayIp;
    }
    else if(TGlobalSettings::Instance().EnableEftPosAdyen)
    {
        frmTouchKeyboard->Caption = "Enter Adyen URL";
        frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().EFTPosURL;
    }
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        if(TGlobalSettings::Instance().EnableEftPosSmartPay)
        {
            TGlobalSettings::Instance().EftPosSmartPayIp = frmTouchKeyboard->KeyboardText.Trim();
            tbEFTPOSURL->Caption = "SmartLink IP\r" + TGlobalSettings::Instance().EftPosSmartPayIp;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmEftPosSmartPayIp,TGlobalSettings::Instance().EftPosSmartPayIp);
        }
        else if(TGlobalSettings::Instance().EnableEftPosSmartConnect)
        {
            TGlobalSettings::Instance().EftPosSmartPayIp = frmTouchKeyboard->KeyboardText.Trim();
            tbEFTPOSURL->Caption = "SmartConnect IP\r" + TGlobalSettings::Instance().EftPosSmartPayIp;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmEftPosSmartPayIp,TGlobalSettings::Instance().EftPosSmartPayIp);
        }
        else if(TGlobalSettings::Instance().EnableEftPosAdyen)
        {
            TGlobalSettings::Instance().EFTPosURL = frmTouchKeyboard->KeyboardText.Trim();
            tbEFTPOSURL->Caption = "Adyen URL\r" + TGlobalSettings::Instance().EFTPosURL;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmEFTPosURL,TGlobalSettings::Instance().EFTPosURL);
        }

        DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmEFTPOSConfig::tbAPIKeyMouseClick(TObject *Sender)
{
	std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	frmTouchKeyboard->MaxLength = 0;
	frmTouchKeyboard->AllowCarriageReturn = true;
	frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().EFTPosAPIKey;
	frmTouchKeyboard->Caption = "Enter EFTPOS API Key";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        TGlobalSettings::Instance().EFTPosAPIKey = frmTouchKeyboard->KeyboardText.Trim();
        tbAPIKey->Caption = "API Key\r" + TGlobalSettings::Instance().EFTPosAPIKey;
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmEFTPosAPIKey,TGlobalSettings::Instance().EFTPosAPIKey);
        DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmEFTPOSConfig::tbDeviceIDMouseClick(TObject *Sender)
{
	std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	frmTouchKeyboard->MaxLength = 0;
	frmTouchKeyboard->AllowCarriageReturn = true;
	frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().EFTPosDeviceID;
	frmTouchKeyboard->Caption = "Enter EFTPOS Device ID";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        TGlobalSettings::Instance().EFTPosDeviceID = frmTouchKeyboard->KeyboardText.Trim();
        tbDeviceID->Caption = "Device ID\r" + TGlobalSettings::Instance().EFTPosDeviceID;
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmEFTPosDeviceID,TGlobalSettings::Instance().EFTPosDeviceID);
        DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEFTPOSConfig::FormShow(TObject *Sender)
{
   UpdateGUI();
}
//---------------------------------------------------------------------------
void TfrmEFTPOSConfig::UpdateGUI()
{
    if(TGlobalSettings::Instance().EnableEftPosSmartPay)
    {
        tbEFTPOSURL->Caption = "SmartLink IP\r" + TGlobalSettings::Instance().EftPosSmartPayIp;
        tbAPIKey->Enabled = false;
        tbDeviceID->Enabled = false;
    }
    else
    {
        tbEFTPOSURL->Caption = "Adyen URL\r" + TGlobalSettings::Instance().EFTPosURL;
        tbAPIKey->Caption = "API Key\r" + TGlobalSettings::Instance().EFTPosAPIKey;
        tbDeviceID->Caption = "Device ID\r" + TGlobalSettings::Instance().EFTPosDeviceID;
    }
}

//---------------------------------------------------------------------------

