//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PHSConfiguration.h"
#include "PhoenixHotelSystem.h"
#include "MMMessageBox.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
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
//			PhoenixHM->TCPIPAddress = frmTouchKeyboard->KeyboardText;
//			tbPhoenixIPAddress->Caption = "Server TCP IP Address\r" + PhoenixHM->TCPIPAddress;
			TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress = frmTouchKeyboard->KeyboardText;
			tbPhoenixIPAddress->Caption = "Server TCP IP Address\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSIPAddress,TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress);
            DBTransaction1.Commit();
            if(PMSType == 2)
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
      std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
		frmTouchNumpad->Caption = "Enter the Port Number of the PMS Hotel System.";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->Mode = pmNumber;
		frmTouchNumpad->INTInitial = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
//			PhoenixHM->TCPPort = frmTouchNumpad->INTResult;
//			tbPhoenixPortNumber->Caption = "PMS Server Port Number\r" + IntToStr(PhoenixHM->TCPPort);
			TDeviceRealTerminal::Instance().BasePMS->TCPPort = frmTouchNumpad->INTResult;
			tbPhoenixPortNumber->Caption = "PMS Server Port Number\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->TCPPort);
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceInt(DBTransaction1,vmPMSTCPPort,TDeviceRealTerminal::Instance().BasePMS->TCPPort);
            DBTransaction1.Commit();
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
   UpdateGUI();
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::InitializePMS()
{
   std::auto_ptr<TManagerPMS> managerPMS (new TManagerPMS());
   managerPMS->Initialize(TDeviceRealTerminal::Instance().DBControl,PMSType);
   TDeviceRealTerminal::Instance().BasePMS->Initialise();
   TDeviceRealTerminal::Instance().BasePMS->Registered = true;
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::UpdateGUI()
{
	tbPhoenixIPAddress->Caption = "Server IP Address\r" + TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
	tbPhoenixPortNumber->Caption = "Server Port Number\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->TCPPort);
	tbPhoenixID->Caption = "P.O.S ID\r" + IntToStr(TDeviceRealTerminal::Instance().BasePMS->POSID);
	tbPointCat->Caption = "Points Category\r" + TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
	tbCreditCat->Caption = "Credit Category\r" + TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
    tbPaymentDefCat->Caption = "Default Payment Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
    tbItemDefCat->Caption = "Default Item Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
    // enable default transaction count button for sihot also
    tbDefTransAccount->Caption = "Default Transaction Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
    if(PMSType == siHot)
    {
        //tbDefTransAccount->Caption = "ServiceCharge Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        tbTipAccount->Caption = "Tip Account\r" + TDeviceRealTerminal::Instance().BasePMS->TipAccount;
        tbExpensesAccount-> Caption = "Expenses Account\r" + TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
        TouchBtn1->Enabled = false;
    }
    else
    {
        tbTipAccount->Enabled = false;
        tbExpensesAccount->Enabled = false;
        tbRoundingAccount->Enabled = false;

    }
	tbSurchargeCat->Caption = "Surcharge Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
	tbRoundingCategory->Caption = "Rounding Category\r" + TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    tbServiceCharge->Caption = "Service Charge\r" + TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    tbRoundingAccount->Caption = "Rounding Account\r" + TDeviceRealTerminal::Instance().BasePMS->RoundingAccountSiHot;
    tbDefTransAccount->Caption = "Default Transaction Account\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
}

void __fastcall TfrmPHSConfiguration::btnOkClick(TObject *Sender)
{
    TDeviceRealTerminal::Instance().BasePMS->Initialise();
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
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
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
		frmTouchKeyboard->Caption = "Enter the default Category Number for Menu Items.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory = frmTouchKeyboard->KeyboardText;
			tbItemDefCat->Caption = "Default Item Category\r" + TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
			Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
			DBTransaction1.StartTransaction();
			TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSItemCategory,TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory);
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
		frmTouchKeyboard->MaxLength = 255;
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
		frmTouchKeyboard->MaxLength = 255;
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
        //if(PMSType == SiHot)
    	 //	frmTouchKeyboard->Caption = "Enter the Service Charge Account.";
       /// else
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
			tbRoundingCategory->Caption = "Tip Account\r" + TDeviceRealTerminal::Instance().BasePMS->TipAccount;
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
			TDeviceRealTerminal::Instance().BasePMS->TipAccount = frmTouchKeyboard->KeyboardText;
			tbRoundingCategory->Caption = "Expenses Account\r" + TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSExpensesAccount,TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount);
            DBTransaction1.Commit();
		}
	}
}
//----------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbRoundingAccountClick(TObject *Sender)
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
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().BasePMS->RoundingAccountSiHot;
		frmTouchKeyboard->Caption = "Enter the Default Transaction Account";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction1.StartTransaction();
			TDeviceRealTerminal::Instance().BasePMS->RoundingAccountSiHot = frmTouchKeyboard->KeyboardText;
			tbRoundingAccount->Caption = "Rounding Account\r" + TDeviceRealTerminal::Instance().BasePMS->RoundingAccountSiHot;
            TManagerVariable::Instance().SetDeviceStr(DBTransaction1,vmPMSRoundingAccountSiHot,TDeviceRealTerminal::Instance().BasePMS->RoundingAccountSiHot);
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

