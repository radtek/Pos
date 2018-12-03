//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PHSConfiguration.h"
#include "PhoenixHotelSystem.h"
#include "MMMessageBox.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "VerticalSelect.h"
#include "MessageMaintenance.h"
#include "PMSTaxCodes.h"
#include "ManagerMews.h"
#include "VerticalSelect.h"
#include "MewsDataProcessor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma resource "*.dfm"
TfrmPHSConfiguration *frmPHSConfiguration;
//---------------------------------------------------------------------------
__fastcall TfrmPHSConfiguration::TfrmPHSConfiguration(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbPhoenixIPAddressClick(TObject *Sender)
{
//	if(!PhoenixHM->Registered)
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        int maxLength = 255;
        UnicodeString caption = "Enter the IP Address of the PMS Hotel System.";
		TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress,caption);
        if(PMSType == siHot || PMSType == mews)
            tbPhoenixIPAddress->Caption = "Server URL\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
        else
            tbPhoenixIPAddress->Caption = "Server IP Address\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSIPAddress,TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress);
        DBTransaction1.Commit();
        if(PMSType == 2 && TDeviceRealTerminal::Instance().BasePMS->POSID == 0)
            MessageBox("Please ensure POS ID should be other than 0 for SiHot .", "Info", MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbPhoenixPortNumberClick(TObject *Sender)
{
//	if(!PhoenixHM->Registered)
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        if(PMSType != mews)
        {
            UnicodeString caption = "Enter the Port Number of the PMS Hotel System.";
            int value = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
            TDeviceRealTerminal::Instance().BasePMS->TCPPort = ShowNumPad(caption,value);
            tbPhoenixPortNumber->Caption = "PMS Server Port Number\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->TCPPort);
            TManagerVariable::Instance().SetDeviceInt(DBTransaction1,vmPMSTCPPort,TDeviceRealTerminal::Instance().BasePMS->TCPPort);
        }
        else
        {
            if(PMSType == mews)
            {
               std::auto_ptr<TfrmMessageMaintenance>(frmMessageMaintenance)
                             (TfrmMessageMaintenance::Create<TfrmMessageMaintenance>
                                          (this,TDeviceRealTerminal::Instance().DBControl));
               frmMessageMaintenance->MessageType = ePMSPaymentType;
               frmMessageMaintenance->ShowModal();
            }
        }
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbPhoenixIDClick(TObject *Sender)
{
//	if(!PhoenixHM->Registered)
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        if(PMSType != mews)
        {
            UnicodeString caption = "Enter the Unique ID for this POS.";
            int value = TDeviceRealTerminal::Instance().BasePMS->POSID;
            TDeviceRealTerminal::Instance().BasePMS->POSID = ShowNumPad(caption,value);
            tbPhoenixID->Caption = "P.O.S ID\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->POSID);
            TManagerVariable::Instance().SetDeviceInt(DBTransaction1,vmPMSPOSID,TDeviceRealTerminal::Instance().BasePMS->POSID);
        }
        else
        {
            AnsiString existingValue = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
            AnsiString codeSelected = GetDropDownResult(Outlet);
            if(codeSelected != "")
            {
                TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount = codeSelected;
                AnsiString labelName = GetMewsName(TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount,Outlet);
                tbPhoenixID->Caption = "Outlet\r" + labelName;
                TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultAccount,TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount);
                if( existingValue != TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount)
                {
                    std::auto_ptr<TMewsDataProcessor> processor(new TMewsDataProcessor());
                    processor->InitializeMewsCategories(DBTransaction1);
                }
            }
            DBTransaction1.Commit();
            DBTransaction1.StartTransaction();
        }
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::FormShow(TObject *Sender)
{
   InitializePMS();
   InitializePMSDefaultPayment();
   UpdateGUI();
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::InitializePMS()
{
   std::auto_ptr<TManagerPMS> managerPMS (new TManagerPMS());
   managerPMS->Initialize(TDeviceRealTerminal::Instance().DBControl,PMSType);
   TGlobalSettings::Instance().PMSType = PMSType;
   TDeviceRealTerminal::Instance().BasePMS->Initialise();
   TDeviceRealTerminal::Instance().BasePMS->Registered = true;
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::UpdateGUI()
{
	tbPhoenixID->Caption = "P.O.S ID\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->POSID);
	tbPointCat->Caption = "Points Account\r" + TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
	tbCreditCat->Caption = "Credit Account\r" + TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
    if(PMSType == oracle || PMSType == siHot)
    {
        tbPaymentDefCat->Caption = "Payment Account\r" ;//+ TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
    }
    else
    {
        tbPaymentDefCat->Caption = "Default Payment Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
    }
    if(PMSType != siHot)
        tbItemDefCat->Caption = "Default Item Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
    else
        tbItemDefCat->Caption = "API-KEY\r" + TDeviceRealTerminal::Instance().BasePMS->ApiKey;
    // enable default transaction count button for sihot also
    tbDefTransAccount->Caption = "Default Transaction Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
    tbRevenueCentre->Caption = "Revenue Centre\r" + TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
    tbServiceCharge->Caption = "Service Charge\r" + TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    tbSurchargeCat->Caption = "Surcharge Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
    if(PMSType == siHot)
    {
        tbPhoenixIPAddress->Caption = "Server URL\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
        tbExpensesAccount->Caption = "Expenses Account\r" + TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
		tbTipAccount->Caption = "Tip Account\r" + TDeviceRealTerminal::Instance().BasePMS->TipAccount;
        tbPhoenixPortNumber->Enabled = false;
        TouchBtn1->Enabled = false;
        cbEnableCustomerJourney->Enabled = true;
        cbEnableCustomerJourney->Checked = TGlobalSettings::Instance().EnableCustomerJourney;
        tbServingTime->Enabled = false;
        tbRevenueCentre->Enabled = false;
        tbItemDefCat->Enabled = true;
        tbOracleInterfaceIP->Enabled = false;
        tbOracleInterfacePort->Caption = "Discounts Account\r" + TGlobalSettings::Instance().RevenueCodeDiscountPart;
        cbMakeOracleServer->Enabled = false;
        tbTimeOut->Enabled = true;
        tbTimeOut->Caption = "Request Time Out\r" + IntToStr(TGlobalSettings::Instance().PMSTimeOut);
        cbNoTaxToSihot->Enabled = true;
        cbNoTaxToSihot->Checked = TGlobalSettings::Instance().SendNoTaxToSiHot;
        tbOracleInterfacePort->Enabled = cbNoTaxToSihot->Checked;
    }
    else if(PMSType == oracle)
    {
        tbPhoenixIPAddress->Caption = "Server IP Address\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
        tbPhoenixPortNumber->Caption = "Server Port Number\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->TCPPort);
        tbExpensesAccount->Enabled = false;
		tbTipAccount->Enabled = false;
        tbServiceCharge->Enabled = false;
        TouchBtn1->Enabled = false;
        tbRoundingCategory->Enabled = false;
        tbItemDefCat->Enabled = false;
        tbDefTransAccount->Enabled = false;
        tbSurchargeCat->Enabled = false;
        tbPhoenixID->Enabled = false;
        cbEnableCustomerJourney->Enabled = false;
        tbTimeOut->Enabled = false;
        cbNoTaxToSihot->Enabled = false;
        if(CanEnablePOSServer())
        {
            cbMakeOracleServer->Enabled = true;
            cbMakeOracleServer->OnClick = NULL;
            cbMakeOracleServer->Checked = TGlobalSettings::Instance().IsOraclePOSServer ;
            cbMakeOracleServer->OnClick = cbMakePOSServer;
        }
        else
        {
            cbMakeOracleServer->Enabled = false;
            cbMakeOracleServer->OnClick = NULL;
            cbMakeOracleServer->Checked = TGlobalSettings::Instance().IsOraclePOSServer ;
            cbMakeOracleServer->OnClick = cbMakePOSServer;
        }
        if(TGlobalSettings::Instance().IsOraclePOSServer)
        {
            tbOracleInterfaceIP->Enabled = true;
            tbOracleInterfacePort->Enabled = true;
            tbOracleInterfaceIP->Caption = "Oracle Interface IP\r" + TGlobalSettings::Instance().OracleInterfaceIPAddress;
            tbOracleInterfacePort->Caption = "Oracle Interface Port\r" + IntToStr(TGlobalSettings::Instance().OracleInterfacePortNumber);
        }
        else
        {
            tbOracleInterfaceIP->Enabled = false;
            tbOracleInterfacePort->Enabled = false;
            tbOracleInterfaceIP->Caption = "Oracle Interface IP";
            tbOracleInterfaceIP->Caption = "Oracle Interface Port";
        }
    }
    else if(PMSType == mews)
    {
        TouchBtn1->Enabled = true;
        cbEnableCustomerJourney->Enabled = false;
        cbEnableCustomerJourney->Checked = false;
        tbServingTime->Enabled = false;
        tbRevenueCentre->Enabled = true;
        tbItemDefCat->Enabled = false;
        tbOracleInterfaceIP->Enabled = false;
        cbMakeOracleServer->Enabled = false;
        tbTimeOut->Enabled = false;
        cbNoTaxToSihot->Enabled = false;
        cbNoTaxToSihot->Checked = false;
        tbOracleInterfacePort->Enabled = false;
        tbPointCat->Enabled = true;
        tbCreditCat->Enabled = true;
        tbRoundingCategory->Enabled = false;
        tbExpensesAccount->Enabled = false;
        tbDefTransAccount->Enabled = false;
        tbRoundingCategory->Enabled = false;
        TouchBtn1->Caption = "Get Details";
        tbPhoenixIPAddress->Caption = "Server URL\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
        //tbPhoenixPortNumber->Caption = "Client Token\r" + TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
        tbRevenueCentre->Caption = "Access Token\r" + TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
        AnsiString outletName   = GetMewsName(TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount,Outlet);
        if(outletName.Trim() == "")
        {
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultAccount,"");
            DBTransaction1.Commit();
        }
        tbPhoenixID->Caption = "Outlet\r" + outletName;
        AnsiString serviceName   = GetMewsName(TGlobalSettings::Instance().OracleInterfaceIPAddress,Service);
        if(serviceName.Trim() == "")
        {
            TGlobalSettings::Instance().OracleInterfaceIPAddress = "";
        }
        tbPaymentDefCat->Caption = "Service\r" + serviceName;
        //AnsiString serviceChargeName = GetMewsName(TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount,AccountingCategory);
        tbServiceCharge->Caption = "Service Charge\r" + TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;//serviceChargeName;
        //AnsiString surchargeName = GetMewsName(TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount,AccountingCategory);
        tbSurchargeCat->Caption = "Surcharge Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;//surchargeName;
        //AnsiString tipName = GetMewsName(TDeviceRealTerminal::Instance().BasePMS->TipAccount,AccountingCategory);
        tbTipAccount->Caption = "Tip Account\r" + TDeviceRealTerminal::Instance().BasePMS->TipAccount;//tipName;
        //AnsiString creditName = GetMewsName(TDeviceRealTerminal::Instance().BasePMS->CreditCategory,AccountingCategory);
        tbCreditCat->Caption = "Credit Account\r" + TDeviceRealTerminal::Instance().BasePMS->CreditCategory;//creditName;
        //AnsiString pointsName = GetMewsName(TDeviceRealTerminal::Instance().BasePMS->PointsCategory,AccountingCategory);
        tbPointCat->Caption = "Points Account\r" + TDeviceRealTerminal::Instance().BasePMS->PointsCategory;//pointsName;
        tbRevenueCodes->Caption = "Revenue Mapping";
        tbPhoenixPortNumber->Enabled = true;
        tbPhoenixPortNumber->Caption = "Payments Mapping";
    }
    else
    {
        tbPhoenixIPAddress->Caption = "Server IP Address\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
        tbPhoenixPortNumber->Caption = "Server Port Number\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->TCPPort);
        tbExpensesAccount->Enabled = false;
		tbTipAccount->Enabled = false;
        tbServiceCharge->Enabled = false;
        tbServingTime->Enabled = false;
        tbRevenueCodes->Enabled = false;
        tbRevenueCentre->Enabled = false;
        cbEnableCustomerJourney->Enabled = false;
        tbOracleInterfaceIP->Enabled = false;
        tbOracleInterfacePort->Enabled = false;
        cbMakeOracleServer->Enabled = false;
        tbTimeOut->Enabled = false;
        cbNoTaxToSihot->Enabled = false;
    }
	tbRoundingCategory->Caption = "Rounding Account\r" + TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    tbTimeOut->Caption = "Request Time Out\r" + IntToStr(TGlobalSettings::Instance().PMSTimeOut);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::btnOkClick(TObject *Sender)
{
    if(TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress.Trim() != "")
        TDeviceRealTerminal::Instance().BasePMS->LogPMSEnabling(eUI);
    TMMProcessingState State(Screen->ActiveForm, "Setting up PMS. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(State);
    TDeviceRealTerminal::Instance().BasePMS->Initialise();
    TDeviceRealTerminal::Instance().ProcessingController.Pop();
    if(TDeviceRealTerminal::Instance().BasePMS->Enabled)
    {
       MessageBox("Do not forget to create PMS payment type","Info",MB_OK);
       MessageBox("Other PMS will not work with this type.","Info",MB_OK);
    }
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbPaymentDefCatClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        if(PMSType == oracle || PMSType == siHot)
        {
           std::auto_ptr<TfrmMessageMaintenance>(frmMessageMaintenance)
                         (TfrmMessageMaintenance::Create<TfrmMessageMaintenance>
                                      (this,TDeviceRealTerminal::Instance().DBControl));
           frmMessageMaintenance->MessageType = ePMSPaymentType;
           frmMessageMaintenance->ShowModal();
        }
        else if(PMSType != mews)
        {
            int maxLength = PMSType != oracle ? 255 : 6 ;
            UnicodeString caption = "Enter the default Category Number for Payments.";
            TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory,caption);
            tbPaymentDefCat->Caption = "Default Payment Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSPaymentCategory,TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory);
       }
       else
       {
            AnsiString codeSelected = GetDropDownResult(Service);
            if(codeSelected != "")
            {
                TGlobalSettings::Instance().OracleInterfaceIPAddress = codeSelected;
                AnsiString labelName = GetMewsName(TGlobalSettings::Instance().OracleInterfaceIPAddress,Service);
                tbPaymentDefCat->Caption = "Service\r" + labelName;
                TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmOracleInterfaceIPAddress,TGlobalSettings::Instance().OracleInterfaceIPAddress);
            }
       }
       DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbItemDefCatClick(TObject *Sender)
{
//	if(!PhoenixHM->Registered)
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        int maxLength = 255;
        UnicodeString caption = "";
        if(PMSType != siHot)
        {
            caption = "Enter the default Category Number for Menu Items.";
            TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory,caption);
            tbItemDefCat->Caption = "Default Item Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
        }
        else
        {
            caption = "Enter the API-KEY.";
            TDeviceRealTerminal::Instance().BasePMS->ApiKey = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->ApiKey,caption);
            tbItemDefCat->Caption = "API-KEY\r" + TDeviceRealTerminal::Instance().BasePMS->ApiKey;
        }
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        if(PMSType != siHot)
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSItemCategory,TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory);
        else
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSAPIKey,TDeviceRealTerminal::Instance().BasePMS->ApiKey);
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbPointCatClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else if(PMSType != mews)
	{
        int maxLength = PMSType != oracle ? 255 : 6;
        UnicodeString caption = "Enter the Category Number for Points.";
	  	TDeviceRealTerminal::Instance().BasePMS->PointsCategory = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->PointsCategory,caption);
        tbPointCat->Caption = "Points Account\r" + TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSPointsCategory,TDeviceRealTerminal::Instance().BasePMS->PointsCategory);
        DBTransaction1.Commit();
	}
    else
    {
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        AnsiString codeSelected = GetDropDownResult(AccountingCategory);
        if(codeSelected != "")
        {
            TDeviceRealTerminal::Instance().BasePMS->PointsCategory = codeSelected;
            tbPointCat->Caption = "Points Account\r" + codeSelected;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSPointsCategory,codeSelected);
        }
        DBTransaction1.Commit();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbCreditCatClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
    else if(PMSType != mews)
    {
        int maxLength = PMSType != oracle ? 255 : 6;
        UnicodeString caption = "Enter the Category Number for Credit.";
		TDeviceRealTerminal::Instance().BasePMS->CreditCategory = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->CreditCategory,caption);
        tbCreditCat->Caption = "Credit Account\r" + TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSCreditCategory,TDeviceRealTerminal::Instance().BasePMS->CreditCategory);
        DBTransaction1.Commit();
	}
    else
    {
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        AnsiString codeSelected = GetDropDownResult(AccountingCategory);
        if(codeSelected != "")
        {
            TDeviceRealTerminal::Instance().BasePMS->CreditCategory = codeSelected;
            tbCreditCat->Caption = "Credit Account\r" + codeSelected;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSCreditCategory,codeSelected);
        }
        DBTransaction1.Commit();
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbDefTransAccountClick(
		TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        int maxLength = 255;
        UnicodeString caption = "Enter the Default Transaction Account.";
	  	TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount,caption);
        tbDefTransAccount->Caption = "Default Transaction Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultAccount,TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount);
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbSurchargeCatClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        if(PMSType != mews)
        {
            int maxLength = 255;
            UnicodeString caption = "Enter the Surcharge Account.";
            TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount,caption);
            tbSurchargeCat->Caption = "Surcharge Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultSurchargeAccount,TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount);
        }
        else
        {
            AnsiString codeSelected = GetDropDownResult(AccountingCategory);
            if(codeSelected != "")
            {
//                TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount = codeSelected;
//                AnsiString labelName = GetMewsName(TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount,AccountingCategory);
//                tbSurchargeCat->Caption = "Surcahrge Account\r" + labelName;
//                TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultSurchargeAccount,TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount);
                TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount = codeSelected;
//                AnsiString labelName = GetMewsName(TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount,AccountingCategory);
                tbSurchargeCat->Caption = "Surcahrge Account\r" + codeSelected;
                TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultSurchargeAccount,codeSelected);
            }
        }
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbRoundingCategoryClick(
      TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        int maxLength = 255;
        UnicodeString caption = "Enter the Rounding Category.";
	  	TDeviceRealTerminal::Instance().BasePMS->RoundingCategory = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->RoundingCategory,caption);
        tbRoundingCategory->Caption = "Rounding Account\r" + TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultRoundingAccount,TDeviceRealTerminal::Instance().BasePMS->RoundingCategory);
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::TouchBtn1MouseClick(TObject *Sender)
{
    if(PMSType != mews)
    {
       AnsiString SourceCode;
       AnsiString DestCode;
       // Find the Phoenix Code in the Ram Menus.
       // Find the Phoenix Code in the DB.
       if(MessageBox("This will replace all matching third party codes stored against existing orders. Do you wish to continue?", "Third Party Code Change Confirmation",MB_YESNO + MB_ICONWARNING) == IDYES)
       {
            std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
          frmTouchKeyboard->MaxLength = 255;
          frmTouchKeyboard->AllowCarriageReturn = false;
          frmTouchKeyboard->StartWithShiftDown = false;
          frmTouchKeyboard->KeyboardText = SourceCode;
          frmTouchKeyboard->Caption = "Enter code that failed.";
          if (frmTouchKeyboard->ShowModal() == mrOk)
          {
             SourceCode = frmTouchKeyboard->KeyboardText;
             frmTouchKeyboard->MaxLength = 255;
             frmTouchKeyboard->AllowCarriageReturn = false;
             frmTouchKeyboard->StartWithShiftDown = false;
             frmTouchKeyboard->KeyboardText = DestCode;
             frmTouchKeyboard->Caption = "Enter the new code.";
             if (frmTouchKeyboard->ShowModal() == mrOk)
             {
                DestCode = frmTouchKeyboard->KeyboardText;
                if(TDeviceRealTerminal::Instance().BasePMS->TestCode(DestCode))
                {
                   if(MessageBox("This will replace all existing orders with code " + SourceCode + " with code " + DestCode + ". Do you wish to continue?", "Third Party Code Change Confirmation",MB_YESNO + MB_ICONWARNING) == IDYES)
                   {
                      TDeviceRealTerminal::Instance().BasePMS->FindAndReplaceCode(SourceCode, DestCode);
                      UpdateGUI();
                   }
                }
             }
          }
       }
    }
    else if(PMSType == mews)
    {
        SyncMewsDetailsFromCloud();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbTipAccountClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        if(PMSType != mews)
        {
            int maxLength = 255;
            UnicodeString caption = "Enter the Tip Account.";
            TDeviceRealTerminal::Instance().BasePMS->TipAccount = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->TipAccount,caption);
            tbTipAccount->Caption = "Tip Account\r" + TDeviceRealTerminal::Instance().BasePMS->TipAccount;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSTipAccount,TDeviceRealTerminal::Instance().BasePMS->TipAccount);
        }
        else
        {
            AnsiString codeSelected = GetDropDownResult(AccountingCategory);
            if(codeSelected != "")
            {
                TDeviceRealTerminal::Instance().BasePMS->TipAccount = codeSelected;
                tbTipAccount->Caption = "Tip Account\r" + codeSelected;
                TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSTipAccount,codeSelected);
            }
        }
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbExpensesAccountClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        int maxLength = 255;
        UnicodeString caption = "Enter the Expenses Account.";
	  	TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount,caption);
        tbExpensesAccount->Caption = "Expenses Account\r" + TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSExpensesAccount,TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount);
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbServiceChargeMouseClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        if(PMSType != mews)
        {
            int maxLength = 255;
            UnicodeString caption = "Enter the Service Charge Account";
            TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount,caption);
            tbServiceCharge->Caption = "Service Charge Account\r" + TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSServiceChargeAccount,TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount);
        }
        else
        {
            AnsiString codeSelected = GetDropDownResult(AccountingCategory);
            if(codeSelected != "")
            {
                TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount = codeSelected;
                tbServiceCharge->Caption = "Service Charge Account\r" + codeSelected;
                TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSServiceChargeAccount,codeSelected);
            }
        }
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::cbEnableCustomerJourneyClick(TObject *Sender)
{
    TGlobalSettings::Instance().EnableCustomerJourney = cbEnableCustomerJourney->Checked;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmEnableCustomerJourney, TGlobalSettings::Instance().EnableCustomerJourney);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbRevenueCodesClick(TObject *Sender)
{
   std::auto_ptr<TfrmMessageMaintenance>(frmMessageMaintenance)
                 (TfrmMessageMaintenance::Create<TfrmMessageMaintenance>
                              (this,TDeviceRealTerminal::Instance().DBControl));
   frmMessageMaintenance->MessageType = eRevenueCodes;
   frmMessageMaintenance->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbServingTimeMouseClick(TObject *Sender)
{
   std::auto_ptr<TfrmMessageMaintenance>(frmMessageMaintenance)
                 (TfrmMessageMaintenance::Create<TfrmMessageMaintenance>
                              (this,TDeviceRealTerminal::Instance().DBControl));
   frmMessageMaintenance->MessageType = eServingTimes;
   frmMessageMaintenance->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbRevenueCentreMouseClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        if(PMSType != mews)
        {
            UnicodeString caption = "Enter Revenue Centre.";
            int value = atoi(TDeviceRealTerminal::Instance().BasePMS->RevenueCentre.c_str());
            TDeviceRealTerminal::Instance().BasePMS->RevenueCentre = IntToStr(ShowNumPad(caption,value));
            tbRevenueCentre->Caption = "Revenue Centre ID\r" + TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmRevenueCentre,TDeviceRealTerminal::Instance().BasePMS->RevenueCentre);
        }
        else
        {
            int maxLength = 255;
            UnicodeString caption = "Enter Access Token.";
            TDeviceRealTerminal::Instance().BasePMS->RevenueCentre = ShowKeyBoard(maxLength,TDeviceRealTerminal::Instance().BasePMS->RevenueCentre,caption);
            tbRevenueCentre->Caption = "Access Token\r" + TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmRevenueCentre,TDeviceRealTerminal::Instance().BasePMS->RevenueCentre);
        }
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::cbMakePOSServer(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        if(CanEnablePOSServer())
        {
            if(cbMakeOracleServer->Checked)
                MessageBox("Please make sure, this option is enabled on this system only at the site.","Information",MB_OK + MB_ICONINFORMATION);
            else
                MessageBox("Please make sure, this option should be enabled on atleast 1 POS at the site.","Information",MB_OK + MB_ICONINFORMATION);
            TGlobalSettings::Instance().IsOraclePOSServer = cbMakeOracleServer->Checked;
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsOraclePOSServer, TGlobalSettings::Instance().IsOraclePOSServer);
            DBTransaction.Commit();
        }
        else
        {
            cbMakeOracleServer->Checked = false;
            TGlobalSettings::Instance().IsOraclePOSServer = false;
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsOraclePOSServer, TGlobalSettings::Instance().IsOraclePOSServer);
            DBTransaction.Commit();
            MessageBox("This option is already enabled on a different POS at the site.","Information",MB_OK + MB_ICONINFORMATION);
        }
        UpdateGUI();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbOracleInterfacePortMouseClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        if(PMSType == oracle)
        {
            UnicodeString caption = "Enter Oracle Interface Port Number.";
            int value = atoi(TDeviceRealTerminal::Instance().BasePMS->RevenueCentre.c_str());
            TGlobalSettings::Instance().OracleInterfacePortNumber = ShowNumPad(caption,value);
            tbOracleInterfacePort->Caption = "Oracle Interface Port\r" + IntToStr(TGlobalSettings::Instance().OracleInterfacePortNumber);
            TManagerVariable::Instance().SetDeviceInt(DBTransaction1,vmOracleInterfacePortNumber,TGlobalSettings::Instance().OracleInterfacePortNumber);
        }
        else if(PMSType == siHot)
        {
            int maxLength = 255;
            UnicodeString caption = "Enter the Discounts Account.";
            TGlobalSettings::Instance().RevenueCodeDiscountPart = ShowKeyBoard(maxLength,TGlobalSettings::Instance().RevenueCodeDiscountPart,caption);
            tbOracleInterfacePort->Caption = "Discounts Account\r" + TGlobalSettings::Instance().RevenueCodeDiscountPart;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmRevenueCodeDiscountPart,TGlobalSettings::Instance().RevenueCodeDiscountPart);
        }
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbOracleInterfaceIPMouseClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        int maxLength = 255;
        UnicodeString caption = "Enter Oracle Interface IP Address.";
	  	TGlobalSettings::Instance().OracleInterfaceIPAddress = ShowKeyBoard(maxLength,TGlobalSettings::Instance().OracleInterfaceIPAddress,caption);
        tbOracleInterfaceIP->Caption = "Oracle Interface IP\r" + TGlobalSettings::Instance().OracleInterfaceIPAddress;
        Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction1.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmOracleInterfaceIPAddress,TGlobalSettings::Instance().OracleInterfaceIPAddress);
        DBTransaction1.Commit();
	}
}
//---------------------------------------------------------------------------
bool TfrmPHSConfiguration::CanEnablePOSServer()
{
    bool retValue = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *SelectQuery= DBTransaction.Query(DBTransaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT * FROM VARSPROFILE WHERE VARIABLES_KEY = 9622 AND INTEGER_VAL = 1 AND PROFILE_KEY <> :PROFILE_KEY"; // vmIsOraclePOSServer
        SelectQuery->ParamByName("PROFILE_KEY")->AsInteger = TManagerVariable::Instance().DeviceProfileKey;
        SelectQuery->ExecQuery();
        if(SelectQuery->RecordCount == 0)
        {
            retValue = true;
        }
        else
        {
            TGlobalSettings::Instance().IsOraclePOSServer = false;
            TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsOraclePOSServer, TGlobalSettings::Instance().IsOraclePOSServer);
        }
        DBTransaction.Commit();
    }
    catch(Exception &Ex)
    {
        DBTransaction.Rollback();
        retValue = false;
    }
    return retValue;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbTimeOutMouseClick(TObject *Sender)
{
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        int value = TGlobalSettings::Instance().PMSTimeOut;
        UnicodeString caption = "Enter SiHot Communication Time-Out in seconds.";
        int result = ShowNumPad(caption,value);
        if(result >= 3 && result <= 11)
        {
            TGlobalSettings::Instance().PMSTimeOut = result;
            tbTimeOut->Caption = "Request Time Out\r" + IntToStr(TGlobalSettings::Instance().PMSTimeOut);
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceInt(DBTransaction1,vmPMSTimeOut,TGlobalSettings::Instance().PMSTimeOut);
            DBTransaction1.Commit();
        }
        else
        {
            MessageBox("Please enter a value from 3 to 11 seconds","Information",MB_OK + MB_ICONINFORMATION);
            tbTimeOutMouseClick(NULL);
        }
	}
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::InitializePMSDefaultPayment()
{
    if(DefaultPaymentInitRequired())
    {
        InitDefaultPaymentInDB();
    }
}
//---------------------------------------------------------------------------
bool TfrmPHSConfiguration::DefaultPaymentInitRequired()
{
    bool retValue = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text =
                   "SELECT * FROM PMSPAYMENTSCONFIG WHERE PMS_PAYTYPE_CATEGORY = 0";
        SelectQuery->ExecQuery();
        UnicodeString value = SelectQuery->FieldByName("PMS_PAYTYPE_CODE")->AsString;
        if(value.Trim() == "")
        {
            retValue = true;
        }
        else
        {
            retValue = false;
        }
        DBTransaction.Commit();
    }
    catch(Exception &Ex)
    {
        DBTransaction.Rollback();
    }
    return retValue;
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::InitDefaultPaymentInDB()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
        UpdateQuery->Close();
        UpdateQuery->SQL->Text =
                   "UPDATE PMSPAYMENTSCONFIG SET PMS_PAYTYPE_CODE = :PMS_PAYTYPE_CODE "
                   "WHERE PMS_PAYTYPE_CATEGORY = 0";

        UpdateQuery->ParamByName("PMS_PAYTYPE_CODE")->AsString = TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
        UpdateQuery->ExecQuery();
        DBTransaction.Commit();
    }
    catch(Exception &Ex)
    {
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::cbNoTaxToSihotClick(TObject *Sender)
{
    TGlobalSettings::Instance().SendNoTaxToSiHot = cbNoTaxToSihot->Checked;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmSendNoTaxToSihot, TGlobalSettings::Instance().SendNoTaxToSiHot);
    DBTransaction.Commit();
    tbOracleInterfacePort->Enabled = cbNoTaxToSihot->Checked;
}
//---------------------------------------------------------------------------
AnsiString TfrmPHSConfiguration::GetDropDownResult(eVertSel selectionType)
{
    return ShowVerticalSelection(selectionType);
    // Show Vertical Selection
}
//---------------------------------------------------------------------------
UnicodeString TfrmPHSConfiguration::ShowKeyBoard(int maxLength,UnicodeString value,UnicodeString caption)
{
    UnicodeString retValue = value;
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
int TfrmPHSConfiguration::ShowNumPad(UnicodeString caption,int value)
{
    int retValue = value;
    std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
    frmTouchNumpad->Caption = caption;
    frmTouchNumpad->btnSurcharge->Caption = "Ok";
    frmTouchNumpad->btnSurcharge->Visible = true;
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->Mode = pmNumber;
    frmTouchNumpad->INTInitial = value;
    if (frmTouchNumpad->ShowModal() == mrOk)
        retValue = frmTouchNumpad->INTResult;
    return retValue;
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::SyncMewsDetailsFromCloud()
{
    if(TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress.Trim() == "" ||
        TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount.Trim() == "" ||
        TDeviceRealTerminal::Instance().BasePMS->RevenueCentre.Trim() == "")
    {
        MessageBox("Url,Client Token and Access Token values are required for the operation to complete.\rPlease check.","Info",MB_OK+MB_ICONINFORMATION);
        return;
    }
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    try
    {
        // Sync Outlets Details
        std::auto_ptr<TManagerMews> managerMews(new TManagerMews());
        bool isSetUp = managerMews->SetUpMews(TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress.Trim(),
                                              TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount.Trim(),
                                              TDeviceRealTerminal::Instance().BasePMS->RevenueCentre.Trim(),DBTransaction1);
        if(!isSetUp)
            MessageBox("Could not sync the fresh details from Mews","Info",MB_OK+MB_ICONINFORMATION);
        else
        {
            UpdateGUI();
        }
        DBTransaction1.Commit();
    }
    catch(Exception &ex)
    {
        DBTransaction1.Rollback();
    }
}
//---------------------------------------------------------------------------
AnsiString TfrmPHSConfiguration::ShowVerticalSelection(eVertSel selectionType)
{
    AnsiString retValue = "";
    std::map<AnsiString,AnsiString> detailsFetched;
    detailsFetched.clear();
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    std::auto_ptr<TMewsDataProcessor> processorMews(new TMewsDataProcessor());
    detailsFetched = processorMews->GetAllMewsDetailsFromDB(DBTransaction1,int(selectionType));
    std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));
    TVerticalSelection Item;
    Item.Title = "Cancel";
    Item.Properties["Color"] = IntToStr(clMaroon);
    Item.CloseSelection = true;
    SelectionForm->Items.push_back(Item);
    int i = 1;
    for(std::map<AnsiString,AnsiString> ::iterator itMap = detailsFetched.begin();itMap != detailsFetched.end();advance(itMap,1))
    {
        TVerticalSelection ItemFromList;
        ItemFromList.Title = itMap->second;
        ItemFromList.Properties["Action"] = itMap->first;
        ItemFromList.Properties["Color"] = IntToStr(clNavy);
        ItemFromList.CloseSelection = true;
        SelectionForm->Items.push_back(ItemFromList);
        i++;
    }
    SelectionForm->ShowModal();
    TVerticalSelection SelectedItem;
    if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
    {
        if(selectionType == 3)
            retValue = SelectedItem.Title;//Properties["Action"];
        else
            retValue = SelectedItem.Properties["Action"];
    }
    DBTransaction1.Commit();
    return retValue;
}
//---------------------------------------------------------------------------
AnsiString TfrmPHSConfiguration::GetMewsName(AnsiString code,eVertSel selectionType)
{
    AnsiString name = "";
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    std::auto_ptr<TMewsDataProcessor> processorMews(new TMewsDataProcessor());
    name = processorMews->GetMewsName(DBTransaction1, code,int(selectionType));
    DBTransaction1.Commit();
    return name;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::cbEnableItemDetailsPostingClick(TObject *Sender)
{
    TGlobalSettings::Instance().SendNoTaxToSiHot = cbEnableItemDetailsPosting->Checked;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmEnableItemDetailsPosting, TGlobalSettings::Instance().EnableItemDetailsPosting);
    DBTransaction.Commit();
}
