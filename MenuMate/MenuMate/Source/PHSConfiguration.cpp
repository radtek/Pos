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
		std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
		frmTouchKeyboard->Caption = "Enter the IP Address of the PMS Hotel System.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress = frmTouchKeyboard->KeyboardText.Trim();
            if(PMSType == siHot || PMSType == mews)
            {
			    tbPhoenixIPAddress->Caption = "Server URL\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
            }
            else
            {
                tbPhoenixIPAddress->Caption = "Server IP Address\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
            }
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSIPAddress,TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress);
            DBTransaction1.Commit();
            if(PMSType == 2 && TDeviceRealTerminal::Instance().BasePMS->POSID == 0)
                MessageBox("Please ensure POS ID should be other than 0 for SiHot .", "Info", MB_OK);
		}
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
        if(PMSType != mews)
        {
            std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
            frmTouchNumpad->Caption = "Enter the Port Number of the PMS Hotel System.";
            frmTouchNumpad->btnSurcharge->Caption = "Ok";
            frmTouchNumpad->btnSurcharge->Visible = true;
            frmTouchNumpad->btnDiscount->Visible = false;
            frmTouchNumpad->Mode = pmNumber;
            frmTouchNumpad->INTInitial = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
            if (frmTouchNumpad->ShowModal() == mrOk)
            {
                TDeviceRealTerminal::Instance().BasePMS->TCPPort = frmTouchNumpad->INTResult;
                tbPhoenixPortNumber->Caption = "PMS Server Port Number\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->TCPPort);
                Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
                DBTransaction1.StartTransaction();
                TManagerVariable::Instance().SetDeviceInt(DBTransaction1,vmPMSTCPPort,TDeviceRealTerminal::Instance().BasePMS->TCPPort);
                DBTransaction1.Commit();
            }
        }
        else if(PMSType == mews)
        {
            std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
            frmTouchKeyboard->MaxLength = 255;
            frmTouchKeyboard->AllowCarriageReturn = false;
            frmTouchKeyboard->StartWithShiftDown = false;
            frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->ClientToken;
            frmTouchKeyboard->Caption = "Enter the Client Token.";
            if (frmTouchKeyboard->ShowModal() == mrOk)
            {
                TDeviceRealTerminal::Instance().BasePMS->ClientToken = frmTouchKeyboard->KeyboardText.Trim();
                tbPhoenixPortNumber->Caption = "Client Token\r" + TDeviceRealTerminal::Instance().BasePMS->ClientToken;
                Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
                DBTransaction1.StartTransaction();
                TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmClientToken,TDeviceRealTerminal::Instance().BasePMS->ClientToken);
                DBTransaction1.Commit();
            }
        }

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
        std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
        frmTouchNumpad->Caption = "Enter the Unique ID for this POS.";
        frmTouchNumpad->btnSurcharge->Caption = "Ok";
        frmTouchNumpad->btnSurcharge->Visible = true;
        frmTouchNumpad->btnDiscount->Visible = false;
        frmTouchNumpad->Mode = pmNumber;
        frmTouchNumpad->INTInitial = TDeviceRealTerminal::Instance().BasePMS->POSID;
        if (frmTouchNumpad->ShowModal() == mrOk)
        {
            TDeviceRealTerminal::Instance().BasePMS->POSID = frmTouchNumpad->INTResult;
            tbPhoenixID->Caption = "P.O.S ID\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->POSID);
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceInt(DBTransaction1,vmPMSPOSID,TDeviceRealTerminal::Instance().BasePMS->POSID);
            DBTransaction1.Commit();
        }
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
	tbPointCat->Caption = "Points Category\r" + TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
	tbCreditCat->Caption = "Credit Category\r" + TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
    tbDefTransAccount->Caption = "Default Transaction Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;

    if(PMSType == siHot)
    {
        UpdateSiHotUI();
    }
    else if(PMSType == oracle)
    {
        UpdateOracleUI();
    }
    else if(PMSType == mews)
    {
        UpdateMewsUI();
    }
    else
    {
        UpdateMotelMateUI();
    }
	tbSurchargeCat->Caption = "Surcharge Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
	tbRoundingCategory->Caption = "Rounding Category\r" + TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    tbServiceCharge->Caption = "Service Charge\r" + TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    tbDefTransAccount->Caption = "Default Transaction Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
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
//	if(!PhoenixHM->Registered)
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
        if(PMSType == oracle || PMSType == siHot)
        {
           std::auto_ptr<TfrmMessageMaintenance>(frmMessageMaintenance)
                         (TfrmMessageMaintenance::Create<TfrmMessageMaintenance>
                                      (this,TDeviceRealTerminal::Instance().DBControl));
           frmMessageMaintenance->MessageType = ePMSPaymentType;
           frmMessageMaintenance->ShowModal();
        }
        else
        {
            std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
            if(PMSType != oracle)
                frmTouchKeyboard->MaxLength = 255;
            else
                frmTouchKeyboard->MaxLength = 6;
            frmTouchKeyboard->AllowCarriageReturn = false;
            frmTouchKeyboard->StartWithShiftDown = false;
            frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
            frmTouchKeyboard->Caption = "Enter the default Category Number for Payments.";
            if (frmTouchKeyboard->ShowModal() == mrOk)
            {
                TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory = frmTouchKeyboard->KeyboardText;
                tbPaymentDefCat->Caption = "Default Payment Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
                Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
                DBTransaction1.StartTransaction();
                TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSPaymentCategory,TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory);
                DBTransaction1.Commit();
            }
       }
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
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
        if(PMSType != siHot)
        {
    		frmTouchKeyboard->Caption = "Enter the default Category Number for Menu Items.";
            frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
        }
        else
        {
            frmTouchKeyboard->Caption = "Enter the API-KEY.";
            frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->ApiKey;
        }
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory = frmTouchKeyboard->KeyboardText;
            if(PMSType != siHot)
            {
                TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory = frmTouchKeyboard->KeyboardText;
    			tbItemDefCat->Caption = "Default Item Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
            }
            else
            {
                TDeviceRealTerminal::Instance().BasePMS->ApiKey = frmTouchKeyboard->KeyboardText.Trim();
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
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbPointCatClick(TObject *Sender)
{
//	if(!PhoenixHM->Registered)
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
        if(PMSType != oracle)
		    frmTouchKeyboard->MaxLength = 255;
        else
            frmTouchKeyboard->MaxLength = 6;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
		frmTouchKeyboard->Caption = "Enter the Category Number for Points.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->PointsCategory = frmTouchKeyboard->KeyboardText;
			tbPointCat->Caption = "Points Category\r" + TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
			Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
			DBTransaction1.StartTransaction();
			TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSPointsCategory,TDeviceRealTerminal::Instance().BasePMS->PointsCategory);
			DBTransaction1.Commit();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbCreditCatClick(TObject *Sender)
{
//	if(!PhoenixHM->Registered)
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
        if(PMSType != oracle)
		    frmTouchKeyboard->MaxLength = 255;
        else
            frmTouchKeyboard->MaxLength = 6;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
		frmTouchKeyboard->Caption = "Enter the Category Number for Credit.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->CreditCategory = frmTouchKeyboard->KeyboardText;
			tbCreditCat->Caption = "Credit Category\r" + TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
			Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
			DBTransaction1.StartTransaction();
			TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSCreditCategory,TDeviceRealTerminal::Instance().BasePMS->CreditCategory);
			DBTransaction1.Commit();
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbDefTransAccountClick(
		TObject *Sender)
{
//	if(!PhoenixHM->Registered)
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        frmTouchKeyboard->Caption = "Enter the Default Transaction Account.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount = frmTouchKeyboard->KeyboardText;
            tbDefTransAccount->Caption = "Default Transaction Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultAccount,TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount);
            DBTransaction1.Commit();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbSurchargeCatClick(TObject *Sender)
{
//	if(!PhoenixHM->Registered)
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
		frmTouchKeyboard->Caption = "Enter the Surcharge Account.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount = frmTouchKeyboard->KeyboardText;
			tbSurchargeCat->Caption = "Surcharge Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultSurchargeAccount,TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount);
            DBTransaction1.Commit();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbRoundingCategoryClick(
      TObject *Sender)
{
//	if(!PhoenixHM->Registered)
    if(!TDeviceRealTerminal::Instance().BasePMS->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
		frmTouchKeyboard->Caption = "Enter the Rounding Category.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->RoundingCategory = frmTouchKeyboard->KeyboardText;
			tbRoundingCategory->Caption = "Rounding Category\r" + TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultRoundingAccount,TDeviceRealTerminal::Instance().BasePMS->RoundingCategory);
            DBTransaction1.Commit();
		}
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
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
		frmTouchKeyboard->Caption = "Enter the Tip Account";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->TipAccount = frmTouchKeyboard->KeyboardText;
			tbTipAccount->Caption = "Tip Account\r" + TDeviceRealTerminal::Instance().BasePMS->TipAccount;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSTipAccount,TDeviceRealTerminal::Instance().BasePMS->TipAccount);
            DBTransaction1.Commit();
		}
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
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
		frmTouchKeyboard->Caption = "Enter the Expenses Account";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount = frmTouchKeyboard->KeyboardText;
			tbExpensesAccount->Caption = "Expenses Account\r" + TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSExpensesAccount,TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount);
            DBTransaction1.Commit();
		}
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
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
		frmTouchKeyboard->Caption = "Enter the Service Charge Account";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount = frmTouchKeyboard->KeyboardText;
			tbServiceCharge->Caption = "Service Charge Account\r" + TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSServiceChargeAccount,TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount);
            DBTransaction1.Commit();
		}
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
   // For the selected Menu show categories used in that
   // on same grid table, when 1 selected and Edit clicked, show a list of
   // Mews Categories.
   if(TGlobalSettings::Instance().PMSType == 4)
   {
        if(TDeviceRealTerminal::Instance().BasePMS->SelectedMewsOutlet.Trim() != "")
            MessageBox("Outlet Selection is mandatory to access Revenue Maps./rPlease Select an outlet.","Info",MB_OK);
            return;
   }
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
        if(PMSType != mews)
        {
            std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
            frmTouchNumpad->Caption = "Enter Revenue Centre.";
            frmTouchNumpad->btnSurcharge->Caption = "Ok";
            frmTouchNumpad->btnSurcharge->Visible = true;
            frmTouchNumpad->btnDiscount->Visible = false;
            frmTouchNumpad->Mode = pmNumber;
            frmTouchNumpad->INTInitial = atoi(TDeviceRealTerminal::Instance().BasePMS->RevenueCentre.c_str());
            frmTouchNumpad->SetMaxLengthValue(5);
            if (frmTouchNumpad->ShowModal() == mrOk)
            {
                TDeviceRealTerminal::Instance().BasePMS->RevenueCentre = frmTouchNumpad->INTResult;
                tbRevenueCentre->Caption = "Revenue Centre ID\r" + TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
                Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
                DBTransaction1.StartTransaction();
                TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmRevenueCentre,TDeviceRealTerminal::Instance().BasePMS->RevenueCentre);
                DBTransaction1.Commit();
            }
        }
        else
        {
            std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
            frmTouchKeyboard->MaxLength = 255;
            frmTouchKeyboard->AllowCarriageReturn = false;
            frmTouchKeyboard->StartWithShiftDown = false;
            frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->AccessToken;
            frmTouchKeyboard->Caption = "Enter the Access Token";
            if (frmTouchKeyboard->ShowModal() == mrOk)
            {
                TDeviceRealTerminal::Instance().BasePMS->AccessToken = frmTouchKeyboard->KeyboardText;
                tbRevenueCentre->Caption = "Access Token\r" + TDeviceRealTerminal::Instance().BasePMS->AccessToken;
                Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
                DBTransaction1.StartTransaction();
                TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmAccessToken,TDeviceRealTerminal::Instance().BasePMS->AccessToken);
                DBTransaction1.Commit();
            }
        }
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
        std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
		frmTouchNumpad->Caption = "Enter Oracle Interface Port Number.";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->Mode = pmNumber;
		frmTouchNumpad->INTInitial = TGlobalSettings::Instance().OracleInterfacePortNumber;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			TGlobalSettings::Instance().OracleInterfacePortNumber = frmTouchNumpad->INTResult;
			tbOracleInterfacePort->Caption = "Oracle Interface Port\r" + IntToStr(TGlobalSettings::Instance().OracleInterfacePortNumber);
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceInt(DBTransaction1,vmOracleInterfacePortNumber,TGlobalSettings::Instance().OracleInterfacePortNumber);
            DBTransaction1.Commit();
		}
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
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().OracleInterfaceIPAddress;
		frmTouchKeyboard->Caption = "Enter Oracle Interface IP Address";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TGlobalSettings::Instance().OracleInterfaceIPAddress = frmTouchKeyboard->KeyboardText;
			tbOracleInterfaceIP->Caption = "Oracle Interface IP\r" + TGlobalSettings::Instance().OracleInterfaceIPAddress;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmOracleInterfaceIPAddress,TGlobalSettings::Instance().OracleInterfaceIPAddress);
            DBTransaction1.Commit();
		}
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
        std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
		frmTouchNumpad->Caption = "Enter SiHot Communication Time-Out in seconds.";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->Mode = pmNumber;
		frmTouchNumpad->INTInitial = TGlobalSettings::Instance().PMSTimeOut;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
            if(frmTouchNumpad->INTResult >= 3 && frmTouchNumpad->INTResult <= 11)
            {
                TGlobalSettings::Instance().PMSTimeOut = frmTouchNumpad->INTResult;
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
void __fastcall TfrmPHSConfiguration::comboOutletsChange(TObject *Sender)
{
    UnicodeString existingValue = TDeviceRealTerminal::Instance().BasePMS->SelectedMewsOutlet;
    TDeviceRealTerminal::Instance().BasePMS->SelectedMewsOutlet = outlets[comboOutlets->ItemIndex].Id;
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmOutletIdMewsSelected,TDeviceRealTerminal::Instance().BasePMS->SelectedMewsOutlet);
    DBTransaction1.Commit();
    if( existingValue != TDeviceRealTerminal::Instance().BasePMS->SelectedMewsOutlet)
    {
        std::auto_ptr<TMewsDataProcessor> processor(new TMewsDataProcessor());
        processor->InitializeMewsCategories();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::comboServicesChange(TObject *Sender)
{
    TDeviceRealTerminal::Instance().BasePMS->SelectedMewsService = services[comboServices->ItemIndex].Id;
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmServiceMewsSelected,TDeviceRealTerminal::Instance().BasePMS->SelectedMewsService);
    DBTransaction1.Commit();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TfrmPHSConfiguration::UpdateOracleUI()
{
    tbPhoenixIPAddress->Caption = "Server IP Address\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
    tbPhoenixPortNumber->Caption = "Server Port Number\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->TCPPort);
    tbExpensesAccount->Enabled = false;
    tbTipAccount->Enabled = false;
    tbServiceCharge->Enabled = false;
    TouchBtn1->Enabled = false;
    tbRoundingCategory->Enabled = false;
    tbItemDefCat->Enabled = false;
    tbItemDefCat->Caption = "Default Item Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
    tbDefTransAccount->Enabled = false;
    tbSurchargeCat->Enabled = false;
    tbPhoenixID->Enabled = false;
    cbEnableCustomerJourney->Enabled = false;
    tbTimeOut->Enabled = false;
    comboOutlets->Enabled = false;
    comboServices->Enabled = false;
    comboOutlets->Visible = false;
    comboServices->Visible = false;
    lblOutlets->Enabled = false;
    lblOutlets->Visible = false;
    lblServices->Enabled = false;
    lblServices->Visible = false;
    tbPaymentDefCat->Caption = "Payment Category\r" ;
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
    lbTips->Visible = false;
    lbTips->Enabled = false;
    lbServiceCharge->Visible = false;
    lbServiceCharge->Enabled = false;
    comboTips->Visible = false;
    comboTips->Enabled = false;
    comboServiceCharge->Visible = false;
    comboServiceCharge->Enabled = false;
    lbSurcharge->Visible            = false;
    lbSurcharge->Enabled            = false;
    comboSurcharge->Visible         = false;
    comboSurcharge->Enabled         = false;
    tbRevenueCentre->Caption = "Revenue Centre\r" + TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::UpdateSiHotUI()
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
    tbItemDefCat->Caption = "API-KEY\r" + TDeviceRealTerminal::Instance().BasePMS->ApiKey;
    tbOracleInterfaceIP->Enabled = false;
    tbOracleInterfacePort->Enabled = false;
    cbMakeOracleServer->Enabled = false;
    tbTimeOut->Enabled = true;
    tbTimeOut->Caption = "Request Time Out\r" + IntToStr(TGlobalSettings::Instance().PMSTimeOut);
    tbPaymentDefCat->Caption = "Payment Category\r" ;
    comboOutlets->Enabled = false;
    comboServices->Enabled = false;
    comboOutlets->Visible = false;
    comboServices->Visible = false;
    lblOutlets->Enabled = false;
    lblOutlets->Visible = false;
    lblServices->Enabled = false;
    lblServices->Visible = false;
    lbTips->Visible = false;
    lbTips->Enabled = false;
    lbServiceCharge->Visible = false;
    lbServiceCharge->Enabled = false;
    comboTips->Visible = false;
    comboTips->Enabled = false;
    comboServiceCharge->Visible = false;
    comboServiceCharge->Enabled = false;
    lbSurcharge->Visible            = false;
    lbSurcharge->Enabled            = false;
    comboSurcharge->Visible         = false;
    comboSurcharge->Enabled         = false;
    tbRevenueCentre->Caption = "Revenue Centre\r" + TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::UpdateMotelMateUI()
{
    tbPhoenixIPAddress->Caption = "Server IP Address\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
    tbPhoenixPortNumber->Caption = "Server Port Number\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->TCPPort);
    tbItemDefCat->Caption = "Default Item Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
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
    tbPaymentDefCat->Caption = "Default Payment Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
    comboOutlets->Enabled = false;
    comboServices->Enabled = false;
    lblOutlets->Enabled = false;
    lblOutlets->Visible = false;
    lblServices->Enabled = false;
    lblServices->Visible = false;
    lbTips->Visible = false;
    lbTips->Enabled = false;
    lbServiceCharge->Visible = false;
    lbServiceCharge->Enabled = false;
    comboTips->Visible = false;
    comboTips->Enabled = false;
    comboServiceCharge->Visible = false;
    comboServiceCharge->Enabled = false;
    lbSurcharge->Visible            = false;
    lbSurcharge->Enabled            = false;
    comboSurcharge->Visible         = false;
    comboSurcharge->Enabled         = false;
    tbRevenueCentre->Caption = "Revenue Centre\r" + TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
}
//--------------------------------------------------------------------------
void TfrmPHSConfiguration::UpdateMewsUI()
{
    outlets.clear();
    services.clear();
    categories.clear();
    tbPhoenixIPAddress->Caption     = "Server URL\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
    tbPhoenixPortNumber->Caption    = "Client Token\r" + TDeviceRealTerminal::Instance().BasePMS->ClientToken;
    tbRevenueCentre->Caption        = "AccessToken\r" + TDeviceRealTerminal::Instance().BasePMS->AccessToken;
    comboOutlets->Enabled           = true;
    comboServices->Enabled          = true;
    comboOutlets->Visible           = true;
    comboServices->Visible          = true;
    TouchBtn1->Enabled              = true;
    TouchBtn1->Caption              = "Get Details";
    tbRevenueCentre->Visible        = true;
    tbRevenueCentre->Enabled        = true;
    tbPhoenixID->Enabled            = false;
    tbPhoenixID->Visible            = false;
    tbPaymentDefCat->Visible        = false;
    tbPaymentDefCat->Enabled        = false;
    lblOutlets->Enabled             = true;
    lblOutlets->Visible             = true;
    lblServices->Enabled            = true;
    lblServices->Visible            = true;
    tbTipAccount->Enabled           = false;
    tbServiceCharge->Enabled        = false;
    tbTipAccount->Visible           = false;
    tbServiceCharge->Visible        = false;
    tbPointCat->Enabled             = false;
    tbItemDefCat->Enabled           = false;
    tbCreditCat->Enabled            = false;
    tbRoundingCategory->Enabled     = false;
    tbDefTransAccount->Enabled      = false;
    tbExpensesAccount->Enabled      = false;
    tbServingTime->Enabled          = false;
    tbOracleInterfaceIP->Enabled    = false;
    tbOracleInterfacePort->Enabled  = false;
    tbTimeOut->Enabled              = false;
    cbMakeOracleServer->Enabled     = false;
    comboTips->Enabled              = true;
    comboTips->Visible              = true;
    comboServiceCharge->Enabled     = true;
    comboServiceCharge->Visible     = true;
    lbSurcharge->Visible            = true;
    lbSurcharge->Enabled            = true;
    comboSurcharge->Visible         = true;
    comboSurcharge->Enabled         = true;
    tbSurchargeCat->Enabled         = false;
    tbSurchargeCat->Visible         = false;
    PopulateDropDowns(comboOutlets->Items,comboServices->Items);
    PopulateDropDowns2(comboServiceCharge->Items,comboTips->Items, comboSurcharge->Items);
    comboOutlets->ItemIndex = GetIndexForSelectedMewsOutlet();
    comboServices->ItemIndex = GetIndexForSelectedMewsService();
    comboTips->ItemIndex  = GetIndexForSelectedTips();
    comboServiceCharge->ItemIndex = GetIndexForSelectedServiceCharge();
    comboSurcharge->ItemIndex     = GetIndexForSelectedSurcharge();
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::SyncMewsDetailsFromCloud()
{
    if(TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress.Trim() == "" ||
        TDeviceRealTerminal::Instance().BasePMS->ClientToken.Trim() == "" ||
        TDeviceRealTerminal::Instance().BasePMS->AccessToken.Trim() == "")
    {
        MessageBox("Url,Client Token and Access Token values are required for the operation to complete.\rPlease check.","Info",MB_OK+MB_ICONINFORMATION);
        return;
    }
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    try
    {
        // Sync Outlets Details
        UnicodeString ipAddress = TManagerVariable::Instance().GetStr(DBTransaction1, vmPMSIPAddress, "");
        UnicodeString clientToken = TManagerVariable::Instance().GetStr(DBTransaction1, vmClientToken, "");
        UnicodeString accessToken = TManagerVariable::Instance().GetStr(DBTransaction1, vmAccessToken, "");
        std::auto_ptr<TManagerMews> managerMews(new TManagerMews());
        bool isSetUp = managerMews->SetUpMews(ipAddress,clientToken,accessToken);
        if(!isSetUp)
            MessageBox("Could not sync the fresh details from Mews","Info",MB_OK+MB_ICONINFORMATION);
        else
        {
            UpdateMewsUI();
        }
        DBTransaction1.Commit();
    }
    catch(Exception &ex)
    {
        DBTransaction1.Rollback();
    }
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::PopulateDropDowns(TStrings* outletstrings, TStrings* servicesstrings)
{
    std::auto_ptr<TManagerMews> managerMews(new TManagerMews());
    outlets = managerMews->GetOutletsFromDB();
    services = managerMews->GetServicesFromDB();

    for(int i = 0; i < outlets.size(); i++)
    {
        UnicodeString outletName = outlets[i].Name;
        outletstrings->Add(outletName);
    }
    for(int i = 0; i < services.size(); i++)
    {
        UnicodeString serviceName = services[i].Name;
        servicesstrings->Add(serviceName);
    }
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::PopulateDropDowns2(TStrings* serviceChargeStrings, TStrings* tipsstrings, TStrings* surchargestrings)
{
    std::auto_ptr<TManagerMews> managerMews(new TManagerMews());
    categories = managerMews->GetCategoriesFromDB();

    for(int i = 0; i < categories.size(); i++)
    {
        UnicodeString serviceCharge = categories[i].Code;
        serviceChargeStrings->Add(serviceCharge);
    }

    for(int i = 0; i < categories.size(); i++)
    {
        UnicodeString tips = categories[i].Code;
        tipsstrings->Add(tips);
    }

    for(int i = 0; i < categories.size(); i++)
    {
        UnicodeString surcharge = categories[i].Code;
        surchargestrings->Add(surcharge);
    }
}
//---------------------------------------------------------------------------
int TfrmPHSConfiguration::GetIndexForSelectedMewsService()
{
    for(int i = 0; i < services.size(); i++)
    {
        if(services[i].Id == TDeviceRealTerminal::Instance().BasePMS->SelectedMewsService)
        {
            return i;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
int TfrmPHSConfiguration::GetIndexForSelectedMewsOutlet()
{
    for(int i = 0; i < outlets.size(); i++)
    {
        if(outlets[i].Id == TDeviceRealTerminal::Instance().BasePMS->SelectedMewsOutlet)
        {
            return i;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
int TfrmPHSConfiguration::GetIndexForSelectedTips()
{
    for(int i = 0; i < categories.size(); i++)
    {
        if(categories[i].Id == TDeviceRealTerminal::Instance().BasePMS->TipAccount)
        {
            return i;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
int TfrmPHSConfiguration::GetIndexForSelectedServiceCharge()
{
    for(int i = 0; i < categories.size(); i++)
    {
        if(categories[i].Id == TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount)
        {
            return i;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
int TfrmPHSConfiguration::GetIndexForSelectedSurcharge()
{
    for(int i = 0; i < categories.size(); i++)
    {
        if(categories[i].Id == TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount)
        {
            return i;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::comboServiceChargeChange(TObject *Sender)
{
    TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount = categories[comboServiceCharge->ItemIndex].Id;
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmMewsServiceCharge,TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount);
    DBTransaction1.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::comboTipsChange(TObject *Sender)
{
    TDeviceRealTerminal::Instance().BasePMS->TipAccount = categories[comboTips->ItemIndex].Id;
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmMewsTips,TDeviceRealTerminal::Instance().BasePMS->TipAccount);
    DBTransaction1.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::comboSurchargeChange(TObject *Sender)
{
    TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount = categories[comboSurcharge->ItemIndex].Id;
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSDefaultSurchargeAccount,TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount);
    DBTransaction1.Commit();
}
//------------------------------------------------------------------------------
