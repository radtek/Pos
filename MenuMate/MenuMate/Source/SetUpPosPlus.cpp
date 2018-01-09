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
    if(TGlobalSettings::Instance().IsFiscalStorageEnabled)
        tbtnConfigure->ButtonColor = clGreen;
    else
        tbtnConfigure->ButtonColor = clRed;
    tbtnPortNumber->Caption = "Port Number/r" + TGlobalSettings::Instance().FiscalServerPortNumber;

    tbtnOrganizationNumber->Caption = TGlobalSettings::Instance().OrganizationNumber;
}
//----------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::tbtnPortNumberMouseClick(TObject *Sender)
{
   	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
    try
    {
        std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
        frmTouchNumpad->Caption = "Enter Port Number for POSPlus.";
        frmTouchNumpad->btnSurcharge->Caption = "Ok";
        frmTouchNumpad->btnSurcharge->Visible = true;
        frmTouchNumpad->btnDiscount->Visible = false;
        frmTouchNumpad->Mode = pmNumber;
        frmTouchNumpad->INTInitial = TGlobalSettings::Instance().FiscalServerPortNumber;
        if (frmTouchNumpad->ShowModal() == mrOk)
        {
            if(TGlobalSettings::Instance().FiscalServerPortNumber != frmTouchNumpad->INTResult)
            {
                TGlobalSettings::Instance().IsFiscalStorageEnabled = false;
                tbtnConfigure->ButtonColor = clRed;
            }
            TGlobalSettings::Instance().FiscalServerPortNumber = frmTouchNumpad->INTResult;
            TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmFiscalServerPortNumber,TGlobalSettings::Instance().FiscalServerPortNumber);
            if(TGlobalSettings::Instance().FiscalServerPortNumber == 0)
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
    catch(Exception &Exc)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::tbtnConfigureMouseClick(TObject *Sender){
    if(TDeviceRealTerminal::Instance().FiscalPort->PortNumber != 0)
    {
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
        DBTransaction1.StartTransaction();
        TGlobalSettings::Instance().IsFiscalStorageEnabled = false;
        try
        {
            std::auto_ptr<TfrmSerialConfig> frmSerialConfig(TfrmSerialConfig::Create<TfrmSerialConfig>(this));
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
}//---------------------------------------------------------------------------

void __fastcall TfrmSetUpPosPlus::tbtnValidateMouseClick(TObject *Sender)
{
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
    DBTransaction1.StartTransaction();
    TGlobalSettings::Instance().IsFiscalStorageEnabled = false;
    try
    {
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
                tbtnConfigure->ButtonColor = clGreen;
            }
            else
                MessageBox("Details are not correct for successful PosPlus communication. Please Check.","Error",MB_OK + MB_ICONERROR);
        }
        TManagerVariable::Instance().SetDeviceBool(DBTransaction1, vmIsFiscalStorageEnabled, TGlobalSettings::Instance().IsFiscalStorageEnabled);
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
    if(TDeviceRealTerminal::Instance().FiscalPort->PortNumber != 0)
    {
        if(!TGlobalSettings::Instance().IsFiscalStorageEnabled)
            MessageBox("Please validate the settings to enable PosPlus communication or set Port Number to 0 to exit.","Information",MB_OK + MB_ICONINFORMATION);
        else
            Close();
    }
    else
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetUpPosPlus::tbtnOrganizationNumberMouseClick(TObject *Sender)
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
        frmTouchNumpad->INTInitial = atoi(TGlobalSettings::Instance().OrganizationNumber.t_str());

        if (frmTouchNumpad->ShowModal() == mrOk)
        {
            TGlobalSettings::Instance().OrganizationNumber = frmTouchNumpad->INTResult;
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
