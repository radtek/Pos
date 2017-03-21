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
	if(!PhoenixHM->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
		std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = PhoenixHM->TCPIPAddress;
		frmTouchKeyboard->Caption = "Enter the IP Address of the PMS Hotel System.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			PhoenixHM->TCPIPAddress = frmTouchKeyboard->KeyboardText;
			tbPhoenixIPAddress->Caption = "Server TCP IP Address\r" + PhoenixHM->TCPIPAddress;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbPhoenixPortNumberClick(TObject *Sender)
{
	if(!PhoenixHM->Registered)
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
		frmTouchNumpad->INTInitial = PhoenixHM->TCPPort;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			PhoenixHM->TCPPort = frmTouchNumpad->INTResult;
			tbPhoenixPortNumber->Caption = "PMS Server Port Number\r" + IntToStr(PhoenixHM->TCPPort);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::tbPhoenixIDClick(TObject *Sender)
{
	if(!PhoenixHM->Registered)
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
		frmTouchNumpad->INTInitial = PhoenixHM->POSID;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			PhoenixHM->POSID = frmTouchNumpad->INTResult;
			tbPhoenixID->Caption = "P.O.S ID\r" + IntToStr(PhoenixHM->POSID);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPHSConfiguration::FormShow(TObject *Sender)
{
   UpdateGUI();
}
//---------------------------------------------------------------------------
void TfrmPHSConfiguration::UpdateGUI()
{
	tbPhoenixIPAddress->Caption = "Server TCP IP Address\r" + PhoenixHM->TCPIPAddress;
	tbPhoenixPortNumber->Caption = "Server Port Number\r" + IntToStr(PhoenixHM->TCPPort);
	tbPhoenixID->Caption = "P.O.S ID\r" + IntToStr(PhoenixHM->POSID);
	tbPaymentDefCat->Caption = "Default Payment Category\r" + PhoenixHM->DefaultPaymentCategory;
	tbItemDefCat->Caption = "Default Item Category\r" + PhoenixHM->DefaultItemCategory;
	tbPointCat->Caption = "Points Category\r" + PhoenixHM->PointsCategory;
	tbCreditCat->Caption = "Credit Category\r" + PhoenixHM->CreditCategory;
	tbDefTransAccount->Caption = "Default Transaction Account\r" + PhoenixHM->DefaultTransactionAccount;
	tbSurchargeCat->Caption = "Surcharge Category\r" + PhoenixHM->DefaultSurchargeAccount;
	tbRoundingCategory->Caption = "Rounding Category\r" + PhoenixHM->RoundingCategory;
}

void __fastcall TfrmPHSConfiguration::btnOkClick(TObject *Sender)
{
    MessageBox("Call to Intialize.", "Info", MB_OK);
	PhoenixHM->Initialise();
	MessageBox("Dont forget to create a PMS payment type.", "Info", MB_OK);
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbPaymentDefCatClick(TObject *Sender)
{
	if(!PhoenixHM->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = PhoenixHM->DefaultPaymentCategory;
		frmTouchKeyboard->Caption = "Enter the default Category Number for Payments.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			PhoenixHM->DefaultPaymentCategory = frmTouchKeyboard->KeyboardText;
			tbPaymentDefCat->Caption = "Default Payment Category\r" + PhoenixHM->DefaultPaymentCategory;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbItemDefCatClick(TObject *Sender)
{
	if(!PhoenixHM->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = PhoenixHM->DefaultItemCategory;
		frmTouchKeyboard->Caption = "Enter the default Category Number for Menu Items.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			PhoenixHM->DefaultItemCategory = frmTouchKeyboard->KeyboardText;
			tbItemDefCat->Caption = "Default Item Category\r" + PhoenixHM->DefaultItemCategory;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbPointCatClick(TObject *Sender)
{
	if(!PhoenixHM->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = PhoenixHM->PointsCategory;
		frmTouchKeyboard->Caption = "Enter the Category Number for Points.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			PhoenixHM->PointsCategory = frmTouchKeyboard->KeyboardText;
			tbPointCat->Caption = "Points Category\r" + PhoenixHM->PointsCategory;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbCreditCatClick(TObject *Sender)
{
	if(!PhoenixHM->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = PhoenixHM->CreditCategory;
		frmTouchKeyboard->Caption = "Enter the Category Number for Credit.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			PhoenixHM->CreditCategory = frmTouchKeyboard->KeyboardText;
			tbCreditCat->Caption = "Credit Category\r" + PhoenixHM->CreditCategory;
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbDefTransAccountClick(
		TObject *Sender)
{
	if(!PhoenixHM->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = PhoenixHM->DefaultTransactionAccount;
		frmTouchKeyboard->Caption = "Enter the Default Transaction Account.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			PhoenixHM->DefaultTransactionAccount = frmTouchKeyboard->KeyboardText;
			tbDefTransAccount->Caption = "Default Transaction Account\r" + PhoenixHM->DefaultTransactionAccount;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbSurchargeCatClick(TObject *Sender)
{
	if(!PhoenixHM->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = PhoenixHM->DefaultSurchargeAccount;
		frmTouchKeyboard->Caption = "Enter the Surcharge Account.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			PhoenixHM->DefaultSurchargeAccount = frmTouchKeyboard->KeyboardText;
			tbSurchargeCat->Caption = "Surcharge Category\r" + PhoenixHM->DefaultSurchargeAccount;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPHSConfiguration::tbRoundingCategoryClick(
      TObject *Sender)
{
	if(!PhoenixHM->Registered)
	{
		MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
	}
	else
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 255;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = PhoenixHM->RoundingCategory;
		frmTouchKeyboard->Caption = "Enter the Rounding Category.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			PhoenixHM->RoundingCategory = frmTouchKeyboard->KeyboardText;
			tbRoundingCategory->Caption = "Rounding Category\r" + PhoenixHM->RoundingCategory;
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
            if(PhoenixHM->TestCode(DestCode))
            {
               if(MessageBox("This will replace all existing orders with code " + SourceCode + " with code " + DestCode + ". Do you wish to continue?", "Third Party Code Change Confirmation",MB_YESNO + MB_ICONWARNING) == IDYES)
               {
                  PhoenixHM->FindAndReplaceCode(SourceCode, DestCode);
                  UpdateGUI();
               }
            }
         }
      }
   }
}
//---------------------------------------------------------------------------

