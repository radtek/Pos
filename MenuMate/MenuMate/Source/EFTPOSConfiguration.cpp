//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EFTPOSConfiguration.h"
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
#include "MMTouchKeyboard.h"
#include "EftPos.h"
#include "EftposAdyen.h"
#include "EFTPOSPaymentSense.h"
#include "VerticalSelect.h"
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
    EnableEFTPOSTerminal();
    Close();
}
//---------------------------------------------------------------------------
bool TfrmEFTPOSConfig::EnableEFTPOSTerminal()
{
    if(TGlobalSettings::Instance().EnableEftPosAdyen)
    {
        EftPos = new TEftposAdyen();
        EftPos->Initialise();
        if(EftPos->Enabled)
            MessageBox("Details for Adyen integration are verified.\rPlease make sure integrated EFTPOS payment type is configured under Payments.","Info",MB_OK + MB_ICONINFORMATION);
    }
    else if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
    {
        EftPos = new TEftPosPaymentSense();
        EftPos->Initialise();
        if(EftPos->Enabled)
            MessageBox("Details for Payment Sense integration have verified.\rPlease make sure integrated EFTPOS payment type is configured under Payments.","Info",MB_OK + MB_ICONINFORMATION);
    }
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
    else if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
    {
        frmTouchKeyboard->Caption = "Enter Payment Sense URL";
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
        else if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
        {
            TGlobalSettings::Instance().EFTPosURL = frmTouchKeyboard->KeyboardText.Trim();
            tbEFTPOSURL->Caption = "PaymentSense URL\r" + TGlobalSettings::Instance().EFTPosURL;
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
    if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
    {
	    frmTouchKeyboard->Caption = "Enter Payment Sense Password";
    }
    else
    {
        frmTouchKeyboard->Caption = "Enter EFTPOS API Key";
    }
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        TGlobalSettings::Instance().EFTPosAPIKey = frmTouchKeyboard->KeyboardText.Trim();
        tbAPIKey->Caption = "API Key\r" + TGlobalSettings::Instance().EFTPosAPIKey;
        TGlobalSettings::Instance().EFTPosAPIKey = frmTouchKeyboard->KeyboardText;
        if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
        {
            tbAPIKey->Caption = "Payment Sense Password \r" + TGlobalSettings::Instance().EFTPosAPIKey;
        }
        else
        {
            tbAPIKey->Caption = "Adyen API Key\r" + TGlobalSettings::Instance().EFTPosAPIKey;
        }

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
    if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
    {
	    frmTouchKeyboard->Caption = "Enter Payment Sense User Name";
    }
    else
    {
	    frmTouchKeyboard->Caption = "Enter EFTPOS Device ID";
    }
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        TGlobalSettings::Instance().EFTPosDeviceID = frmTouchKeyboard->KeyboardText.Trim();
        tbDeviceID->Caption = "Device ID\r" + TGlobalSettings::Instance().EFTPosDeviceID;
        TGlobalSettings::Instance().EFTPosDeviceID = frmTouchKeyboard->KeyboardText;
        if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
        {
            tbDeviceID->Caption = "User Name \r" + TGlobalSettings::Instance().EFTPosDeviceID;
        }
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
        tbEftPosTerminalID->Enabled = false;
    }
    else if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
    {
        tbEFTPOSURL->Caption = "Payment Sense URL\r" + TGlobalSettings::Instance().EFTPosURL;
        tbAPIKey->Caption = "Password\r" + TGlobalSettings::Instance().EFTPosAPIKey;
        tbDeviceID->Caption = "User Name \r" + TGlobalSettings::Instance().EFTPosDeviceID;
        tbEftPosTerminalID->Caption = "TID \r" + TGlobalSettings::Instance().EftPosTerminalId;
    }
    else
    {
        tbEFTPOSURL->Caption = "Adyen URL\r" + TGlobalSettings::Instance().EFTPosURL;
        tbAPIKey->Caption = "API Key\r" + TGlobalSettings::Instance().EFTPosAPIKey;
        tbDeviceID->Caption = "Device ID\r" + TGlobalSettings::Instance().EFTPosDeviceID;
        tbEftPosTerminalID->Enabled = false;
    }
}

//---------------------------------------------------------------------------
void __fastcall TfrmEFTPOSConfig::tbEftPosTerminalIDMouseClick(TObject *Sender)
{
    if(EftPos)
        delete EftPos;
    EftPos = new TEftPosPaymentSense();
    EftPos->Initialise();
    std::vector<AnsiString>tidList = EftPos->GetAllTerminals();
    if(tidList.size())
    {
        std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));
        TVerticalSelection Item;
        Item.Title = "Cancel";
        Item.Properties["Color"] = "0x000098F5";
        Item.CloseSelection = true;
        SelectionForm->Items.push_back(Item);

        for(int index = 0; index < tidList.size(); index++)
        {
            TVerticalSelection Item1;
            Item1.Title = tidList[index];
            Item1.Properties["Action"] = IntToStr(1);
            Item1.Properties["Color"] = IntToStr(clNavy);
            Item1.CloseSelection = true;
            SelectionForm->Items.push_back(Item1);
        }

        SelectionForm->ShowModal();
        TVerticalSelection SelectedItem;
        if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
        {
            int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
            if(Action)
            {
                TGlobalSettings::Instance().EftPosTerminalId = SelectedItem.Title.Trim();
            }
        }
        else
        {
            TGlobalSettings::Instance().EftPosTerminalId = "";
        }
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmEftPosTerminalId,TGlobalSettings::Instance().EftPosTerminalId);
        DBTransaction.Commit();
        tbEftPosTerminalID->Caption = "TID \r" + TGlobalSettings::Instance().EftPosTerminalId;
    }
}
