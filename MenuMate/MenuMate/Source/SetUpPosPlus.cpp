//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SetUpPosPlus.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include <MMTouchNumpad.h>
#include "MMTouchKeyboard.h"
#include "SerialConfig.h"
#include "FiscalDataUtility.h"
#include "ManagerAustriaFiscal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmSetUpPosPlus *frmSetUpPosPlus;
//---------------------------------------------------------------------------
__fastcall TfrmSetUpPosPlus::TfrmSetUpPosPlus(TComponent* Owner)
    :TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::FormShow(TObject *Sender)
{
}
//----------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::tbtnPortNumberMouseClick(TObject *Sender)
{
   	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
    try
    {
        if(StorageType == PosPlus)
        {
            std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
            frmTouchNumpad->Caption = "Enter Port Number for POSPlus.";
            frmTouchNumpad->btnSurcharge->Caption = "Ok";
            frmTouchNumpad->btnSurcharge->Visible = true;
            frmTouchNumpad->btnDiscount->Visible = false;
            frmTouchNumpad->Mode = pmNumber;
            frmTouchNumpad->INTInitial = TDeviceRealTerminal::Instance().FiscalPort->PortNumber;
            if (frmTouchNumpad->ShowModal() == mrOk)
            {
                if(TDeviceRealTerminal::Instance().FiscalPort->PortNumber != frmTouchNumpad->INTResult)
                {
                    TGlobalSettings::Instance().IsFiscalStorageEnabled = false;
                    tbtnConfigure->ButtonColor = clRed;
                }
                TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmFiscalServerPortNumber,frmTouchNumpad->INTResult);
                if(frmTouchNumpad->INTResult == 0)
                {
                   TGlobalSettings::Instance().IsFiscalStorageEnabled = false;
                   tbtnConfigure->ButtonColor = clRed;
                   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsFiscalStorageEnabled, TGlobalSettings::Instance().IsFiscalStorageEnabled);
                }
                DBTransaction.Commit();
                DBTransaction.StartTransaction();
                TDeviceRealTerminal::Instance().FiscalPort->Initialise(DBTransaction);
                DBTransaction.Commit();
            }
        }
        else if(StorageType == AustriaFiscal)
        {
            UnicodeString result = ShowKeyBoard(100,TGlobalSettings::Instance().AustriaFiscalUrl,"Enter Url for Fiscal Austria");

            if((TGlobalSettings::Instance().AustriaFiscalUrl != result) || TGlobalSettings::Instance().AustriaFiscalUrl.Trim() == "")
                TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = false;

            TGlobalSettings::Instance().AustriaFiscalUrl = result.Trim();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmAustriaFiscalUrl,TGlobalSettings::Instance().AustriaFiscalUrl);
            tbtnPortNumber->Caption = TGlobalSettings::Instance().AustriaFiscalUrl;
            DBTransaction.Commit();
            ConfigureForMode();
        }
    }
    catch(Exception &Exc)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::tbtnConfigureMouseClick(TObject *Sender){
    if(StorageType == PosPlus)
    {
        if(TDeviceRealTerminal::Instance().FiscalPort->PortNumber != 0)
        {
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
            DBTransaction1.StartTransaction();
            TGlobalSettings::Instance().IsFiscalStorageEnabled = false;
            try
            {
                std::auto_ptr<TfrmSerialConfig> frmSerialConfig(TfrmSerialConfig::Create<TfrmSerialConfig>(this));
                frmSerialConfig->Caption = "POS Plus Port Configuration";
                TDeviceRealTerminal::Instance().FiscalPort->Initialise(DBTransaction1);
                frmSerialConfig->LoadSettings(TDeviceRealTerminal::Instance().FiscalPort->GetTComPort(),TDeviceRealTerminal::Instance().FiscalPort->AsyncMode);
                frmSerialConfig->cbAsync->Enabled = false;
                if(frmSerialConfig->ShowModal() == mrOk)
                {
                    DBTransaction1.Commit();
                    DBTransaction1.StartTransaction();
                    TDeviceRealTerminal::Instance().FiscalPort->Close();
                    frmSerialConfig->AssignSettings(TDeviceRealTerminal::Instance().FiscalPort->GetTComPort(),TDeviceRealTerminal::Instance().FiscalPort->AsyncMode);
                    TDeviceRealTerminal::Instance().FiscalPort->SaveSettings(DBTransaction1);
                    DBTransaction1.Commit();
                    DBTransaction1.StartTransaction();
                    TDeviceRealTerminal::Instance().FiscalPort->Initialise(DBTransaction1);
                    DBTransaction1.Commit();
                    tbtnConfigure->ButtonColor = clRed;
                    MessageBox("Please validate the settings to enable PosPlus communication.","Information",MB_OK + MB_ICONINFORMATION);
                }
            }
            catch(Exception &E)
            {
                DBTransaction1.Rollback();
                MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
                TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
            }
        }
        else
            MessageBox("Please set Port Number First.","Information", MB_OK + MB_ICONINFORMATION);
    }
    else if(StorageType == AustriaFiscal)
    {
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
        DBTransaction1.StartTransaction();
        try
        {
            UnicodeString result = ShowKeyBoard(100,"","Enter Cash Box Id for Fiscal Austria");

            if((TGlobalSettings::Instance().AustriaFiscalCashBoxId != result) || TGlobalSettings::Instance().AustriaFiscalCashBoxId.Trim() == "")
                TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = false;

            TGlobalSettings::Instance().AustriaFiscalCashBoxId = result.Trim();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmAustriaFiscalCashBoxId,TGlobalSettings::Instance().AustriaFiscalCashBoxId);
            tbtnConfigure->Caption = TGlobalSettings::Instance().AustriaFiscalCashBoxId != "" ? "*****" : "";
            DBTransaction1.Commit();
            ConfigureForMode();
        }
        catch(Exception &E)
        {
            DBTransaction1.Rollback();
            MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
            TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        }
    }
}//---------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::tbtnValidateMouseClick(TObject *Sender)
{
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
    DBTransaction1.StartTransaction();

    try
    {
        if(StorageType == PosPlus)
        {
            TGlobalSettings::Instance().IsFiscalStorageEnabled = false;
            if(!TDeviceRealTerminal::Instance().FiscalPort->Open(TDeviceRealTerminal::Instance().FiscalPort->PortNumber))
            {
                MessageBox("Unable to open Port for PosPlus. Please check details.", "Error",MB_OK + MB_ICONERROR);
            }
            else
            {
                AnsiString response = TDeviceRealTerminal::Instance().FiscalPort->SetFiscalData("ver 0000", eFiscalValidate);
                std::auto_ptr<TFiscalDataUtility> dataUtility(new TFiscalDataUtility());
                bool isSuccessful = dataUtility->AnalyzeResponse(response, eFiscalVerNumber);
                if(isSuccessful)
                {
                    TGlobalSettings::Instance().IsFiscalStorageEnabled = true;
                    TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = false;
                    tbtnConfigure->ButtonColor = clGreen;
                    TGlobalSettings::Instance().CaptureRefundRefNo = false;
                    TManagerVariable::Instance().SetDeviceBool(DBTransaction1, vmCaptureRefundRefNo, TGlobalSettings::Instance().CaptureRefundRefNo);
                    MessageBox("Pos Plus details are validated.\rOther Fiscal integrations are disabled now.","Info",MB_OK+MB_ICONINFORMATION);
                }
                else
                    MessageBox("Details are not correct for successful PosPlus communication. Please Check.","Error",MB_OK + MB_ICONERROR);
            }
            TManagerVariable::Instance().SetDeviceBool(DBTransaction1, vmIsFiscalStorageEnabled, TGlobalSettings::Instance().IsFiscalStorageEnabled);
            TManagerVariable::Instance().SetDeviceBool(DBTransaction1, vmIsAustriaFiscalStorageEnabled, TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled);
        }
        else if(StorageType == AustriaFiscal)
        {
            TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = false;
            if(AreDetailsProvidedForAustria())
            {
                // Call Services to Validate details
                std::auto_ptr<TManagerAustriaFiscal> managerAustriaFiscal(new TManagerAustriaFiscal());
                if(managerAustriaFiscal->GetEchoResponseFromConfig())
                {
                    if(TGlobalSettings::Instance().IsAustriaFiscalCommissioned)
                    {
                        if(managerAustriaFiscal->SendZeroReceipt())
                        {
                            TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = true;
                            TGlobalSettings::Instance().IsFiscalStorageEnabled = false;
                            MessageBox("Austria Fiscal details are validated.\rOther Fiscal integrations are disabled now.","Info",MB_OK+MB_ICONINFORMATION);
                        }
                    }
                    else
                    {
                        MessageBox("Austria Fiscal is not commissioned at the moment.\rPlease run Commission command using option available on screen.","Info",MB_OK+MB_ICONINFORMATION);
                    }
                }
            }
            ConfigureForMode();
            TManagerVariable::Instance().SetDeviceBool(DBTransaction1, vmIsFiscalStorageEnabled, TGlobalSettings::Instance().IsFiscalStorageEnabled);
            TManagerVariable::Instance().SetDeviceBool(DBTransaction1, vmIsAustriaFiscalStorageEnabled, TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled);
        }
        DBTransaction1.Commit();
    }
    catch(Exception &Exc)
    {
        DBTransaction1.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::tbtnCloseMouseClick(TObject *Sender)
{
//    tbtnValidateMouseClick(NULL);
    if(StorageType == PosPlus)
    {
        if(TDeviceRealTerminal::Instance().FiscalPort->PortNumber != 0)
        {
            if(!TGlobalSettings::Instance().IsFiscalStorageEnabled)
                MessageBox("Please validate the settings to enable PosPlus communication or set Port Number to 0 to exit.","Information",MB_OK + MB_ICONINFORMATION);
            else
                Close();
        }
        else
        {
            TGlobalSettings::Instance().IsFiscalStorageEnabled = false;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceBool(DBTransaction1, vmIsFiscalStorageEnabled, TGlobalSettings::Instance().IsFiscalStorageEnabled);
            TManagerVariable::Instance().SetDeviceBool(DBTransaction1, vmIsAustriaFiscalStorageEnabled, TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled);
            DBTransaction1.Commit();
            Close();
        }
    }
    else if(StorageType == AustriaFiscal)
    {
        if(TGlobalSettings::Instance().AustriaFiscalUrl != "")
        {
            if(!TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled)
                MessageBox("Please validate the settings to enable Fiscal Austria or set URL to blank to exit.","Information",MB_OK + MB_ICONINFORMATION);
            else
                Close();
        }
        else
        {
            TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = false;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceBool(DBTransaction1, vmIsFiscalStorageEnabled, TGlobalSettings::Instance().IsFiscalStorageEnabled);
            TManagerVariable::Instance().SetDeviceBool(DBTransaction1, vmIsAustriaFiscalStorageEnabled, TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled);
            DBTransaction1.Commit();
            Close();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::tbtnOrganizationNumberMouseClick(TObject *Sender)
{
    if(StorageType == PosPlus)
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
        DBTransaction.StartTransaction();
        try
        {
            std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
            frmTouchNumpad->Caption = "Enter Organization Number.";
            frmTouchNumpad->btnSurcharge->Caption = "Ok";
            frmTouchNumpad->btnSurcharge->Visible = true;
            frmTouchNumpad->btnDiscount->Visible = false;
            frmTouchNumpad->Mode = pmNumber;
            double intialValue = 0;
            TryStrToFloat(TGlobalSettings::Instance().OrganizationNumber,intialValue);
            frmTouchNumpad->INTInitialLong = intialValue;
            frmTouchNumpad->MaxLength = 10;
            if (frmTouchNumpad->ShowModal() == mrOk)
            {
                TGlobalSettings::Instance().OrganizationNumber = frmTouchNumpad->INTResultLong;
                int offset = 10 -TGlobalSettings::Instance().OrganizationNumber.Length();
                UnicodeString value = "";
                if(offset > 0)
                {
                    for(int i = 1; i <= offset; i++)
                    {
                       value += "0";
                    }
                    MessageBox("0 will be added in front of Organization Number","Information",MB_OK);
                }
                TGlobalSettings::Instance().OrganizationNumber = value;
                AnsiString strValue = frmTouchNumpad->INTResultLong;
                TGlobalSettings::Instance().OrganizationNumber += strValue;
                TManagerVariable::Instance().SetDeviceStr(DBTransaction,  vmOrganizationNumber, TGlobalSettings::Instance().OrganizationNumber);
                DBTransaction.Commit();
                tbtnOrganizationNumber->Caption = TGlobalSettings::Instance().OrganizationNumber;
            }
        }
        catch(Exception &Exc)
        {
            DBTransaction.Rollback();
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
        }
    }
    else if(StorageType == AustriaFiscal)
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
        DBTransaction.StartTransaction();
        try
        {
            UnicodeString result = ShowKeyBoard(100,"","Enter Access Token for Fiscal Austria");

            if((TGlobalSettings::Instance().AustriaFiscalAccessToken != result) || TGlobalSettings::Instance().AustriaFiscalAccessToken.Trim() == "")
                TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = false;

            TGlobalSettings::Instance().AustriaFiscalAccessToken = result.Trim();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmAustriaFiscalAccessToken,TGlobalSettings::Instance().AustriaFiscalAccessToken);
            tbtnOrganizationNumber->Caption = TGlobalSettings::Instance().AustriaFiscalAccessToken != "" ? "*****" : "";
            DBTransaction.Commit();
            ConfigureForMode();
        }
        catch(Exception &Exc)
        {
            DBTransaction.Rollback();
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
        }
    }
}
void TfrmSetUpPosPlus::ConfigureForMode()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
    try
    {
        if(StorageType == PosPlus)
        {
            tbtnTerminalId->Enabled         = false;
            tbtnOrganizationNumber->Caption = TGlobalSettings::Instance().OrganizationNumber;
            tbtnPortNumber->Caption         = "Port Number";
            if(!TGlobalSettings::Instance().IsFiscalStorageEnabled)
                tbtnConfigure->ButtonColor  = clRed;
            tbtnCommission->Enabled         = false;
        }
        else if(StorageType == AustriaFiscal)
        {
            Caption = "Fiscal Austria Set Up";
            labelPortNumber->Caption        = "Fiscal Autria Url";
            tbtnPortNumber->Caption         = TGlobalSettings::Instance().AustriaFiscalUrl;
            labelConfigure->Caption         = "Cash Box Id";
            tbtnConfigure->Caption          = TGlobalSettings::Instance().AustriaFiscalCashBoxId.Trim() != "" ? "*****" : "";
            tbtnConfigure->ButtonColor      = clNavy;
            tbtnTerminalId->Enabled         = true;
            tbtnTerminalId->Caption         = TGlobalSettings::Instance().AustriaFiscalTerminalId;
            labelOrganization->Caption      = "Access Token";
            tbtnOrganizationNumber->Enabled = true;
            tbtnOrganizationNumber->Caption = TGlobalSettings::Instance().AustriaFiscalAccessToken.Trim() != "" ? "*****" : "";
            tbtnCommission->Enabled         = true;
            int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, "Globals");
            if (GlobalProfileKey != 0)
            {
                TGlobalSettings::Instance().IsAustriaFiscalCommissioned = GetDBValueForCommission(DBTransaction,GlobalProfileKey);
                //TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmIsAustriaFiscalCommissioned, TGlobalSettings::Instance().IsAustriaFiscalCommissioned);
                DBTransaction.Commit();
            }
            else
            {
                TGlobalSettings::Instance().IsAustriaFiscalCommissioned = false;
            }

            if(TGlobalSettings::Instance().IsAustriaFiscalCommissioned)
            {
                tbtnCommission->ButtonColor  = clGreen;
                tbtnCommission->Caption      = "Commissioned";
            }
            else
            {
                tbtnCommission->ButtonColor  = clRed;
                tbtnCommission->Caption      = "Commission";
            }
            if(TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled)
            {
                tbtnValidate->ButtonColor  = clGreen;
                tbtnValidate->Caption = "Validated";
            }
            else
            {
                tbtnValidate->ButtonColor  = clRed;
                tbtnValidate->Caption = "Validate";
            }
        }
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
        MessageBox(Exc.Message,"Exception in config",MB_OK);
        DBTransaction.Rollback();
    }
}
//-----------------------------------------------------------------------------
UnicodeString TfrmSetUpPosPlus::ShowKeyBoard(int maxLength,UnicodeString value,UnicodeString caption)
{
    UnicodeString retValue = "";
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
    frmTouchKeyboard->MaxLength = maxLength;
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
    frmTouchKeyboard->KeyboardText = value;
    frmTouchKeyboard->Caption = caption;
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
        retValue = frmTouchKeyboard->KeyboardText;
    }
    return retValue;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::tbtnTerminalIdMouseClick(TObject *Sender)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
    try
    {
        UnicodeString result = ShowKeyBoard(100,TGlobalSettings::Instance().AustriaFiscalTerminalId,"Enter Terminal Id for Fiscal Austria");

        if((TGlobalSettings::Instance().AustriaFiscalTerminalId != result) || TGlobalSettings::Instance().AustriaFiscalTerminalId.Trim() == "")
            TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = false;

        TGlobalSettings::Instance().AustriaFiscalTerminalId = result;
        TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmAustriaFiscalTerminalId,TGlobalSettings::Instance().AustriaFiscalTerminalId);
        tbtnTerminalId->Caption = TGlobalSettings::Instance().AustriaFiscalTerminalId;
        DBTransaction.Commit();
        ConfigureForMode();
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
bool TfrmSetUpPosPlus::AreDetailsProvidedForAustria()
{
    bool allProvided = false;
    UnicodeString errorString = "";
    if(TGlobalSettings::Instance().AustriaFiscalUrl.Trim() != "")
        if(TGlobalSettings::Instance().AustriaFiscalCashBoxId.Trim() != "")
            if(TGlobalSettings::Instance().AustriaFiscalTerminalId.Trim() != "")
                if(TGlobalSettings::Instance().AustriaFiscalAccessToken.Trim() != "")
                    allProvided = true;
                else
                    errorString = "Please provide Access Token for Fiscal Austria.\r";
            else
                errorString = "Please provide Terminal Id for Fiscal Austria.\r";
        else
            errorString = "Please provide Cash Box Id for Fiscal Austria.\r";
    else
        errorString = "Please provide URL for Fiscal Austria.\r";
    if(errorString != "")
    {
        errorString += "Fiscal Austria can not be enabled.";
        MessageBox(errorString,"Error,",MB_OK+MB_ICONERROR);
    }
    return allProvided;

}
//---------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::tbtnCommissionMouseClick(TObject *Sender)
{
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
    DBTransaction1.StartTransaction();
    try
    {
        if(AreDetailsProvidedForAustria())
        {
            if(TGlobalSettings::Instance().IsAustriaFiscalCommissioned)
            {
                MessageBox("Austria Fiscal is already commissioned.","Info",MB_OK);
            }
            else
            {
                std::auto_ptr<TManagerAustriaFiscal> managerAustriaFiscal(new TManagerAustriaFiscal());
                if(managerAustriaFiscal->CommissionAustriaFiscal())
                {
                    TGlobalSettings::Instance().IsAustriaFiscalCommissioned = true;
                    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction1,eSystemProfiles,"Globals");
                    if(GlobalProfileKey == 0)
                    {
                        GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction1,eSystemProfiles,"Globals");
                    }
                    TManagerVariable::Instance().SetProfileBool(DBTransaction1, GlobalProfileKey, vmIsAustriaFiscalCommissioned, TGlobalSettings::Instance().IsAustriaFiscalCommissioned);
                }
                DBTransaction1.Commit();
                ConfigureForMode();
            }
        }
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
        DBTransaction1.Rollback();
    }
}
//---------------------------------------------------------------------------
bool TfrmSetUpPosPlus::GetDBValueForCommission(Database::TDBTransaction &DBTransaction,int profileKey)
{
    bool retValue = false;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text = "SELECT * FROM VARSPROFILE WHERE VARIABLES_KEY = 9646 AND PROFILE_KEY = :PROFILE_KEY";
    IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = profileKey;
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount > 0)
    {
        if(IBInternalQuery->FieldByName("INTEGER_VAL")->AsInteger == 1)
            retValue = true;
    }
    return retValue;
}

