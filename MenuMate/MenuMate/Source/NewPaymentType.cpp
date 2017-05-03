// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewPaymentType.h"
#include "DeviceRealTerminal.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "GUIDiscount.h"
#include "MMMessageBox.h"
#include "Rooms.h"
#include <math.h>
#include <bitset>
#include "ManagerThirdParty.h"
#include "PhoenixHotelSystem.h"
#include "MM_DBCore.h"
#include "MMLogging.h"
#include "DBTab.h"
#include "FolderManager.h"
#include "Membership.h"
#include "VerticalSelect.h"
#include "StringTools.h"
#include "PaymentMaintenance.h"
#include "WalletConfiguration.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGRID"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

TForm * TfrmNewPaymentType::WinOwner = NULL;

// ---------------------------------------------------------------------------
__fastcall TfrmNewPaymentType::TfrmNewPaymentType(TComponent* Owner, Database::TDBControl &inDBControl,
   TListPaymentSystem *inPaymentSystem, int inPaymentKey) : TZForm(Owner), DBControl(inDBControl), PaymentSystem(inPaymentSystem)
{
   PaymentKey = inPaymentKey;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::CreateParams(Controls::TCreateParams &params)
{
   TForm::CreateParams(params);
   if (WinOwner)
   {
	  params.WndParent = WinOwner->Handle;
	  WinOwner = NULL;
   }
}
// ---------------------------------------------------------------------------
TfrmNewPaymentType *TfrmNewPaymentType::Create(TForm* Owner, Database::TDBControl &inDBControl, TListPaymentSystem *inPaymentSystem,
   int inPaymentKey)
{
   WinOwner = Owner;
   return new TfrmNewPaymentType(Owner, inDBControl, inPaymentSystem, inPaymentKey);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::pnlOkClick(TObject *Sender)
{

    if (btnName->Caption == "" || btnName->Caption == "Payment Name")
	  {
		 MessageBox("You must enter a payment type name.", "Error", MB_OK);
		 return;
	  }

	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

   try
   {
        Payment.Name = btnName->Caption;
        Payment.DisplayOrder = PaymentPos;
        Payment.Colour = btnName->ButtonColor;
        Payment.GroupNumber = PaymentGroup;
        Payment.PaymentThirdPartyID = PaymentThirdPartyID;
        Payment.VoucherMerchantID = VoucherMerchantID;
        Payment.TaxRate = TaxRate;
        Payment.ClearPaymentAttribute();
        Payment.RoundTo = RoundTo;
        Payment.SecondaryPMSIPAddress = SecondaryPMSIPAddress;
        Payment.SecondaryPMSPortNumber = SecondaryPMSPortNumber;
        Payment.Export = Export;
        Payment.UniVoucherUser = UniUser;
        Payment.UniVoucherPass = UniPass;
        Payment.UniVoucherToken = "";
        Payment.CVSReadLocation = CVSReadLocation;
        Payment.CVSWriteLocation = CVSWriteLocation;
        Payment.TabKey  =TabKey;
        Payment.GLCode = GLCode;
        Payment.AutoPopulateBlindBalance = cbAutoPopulateBlindBalance->Checked;

        Payment.SetPaymentAttribute(ePayTypeCustomSurcharge,cbIsTip->Checked);
        Payment.SetPaymentAttribute(ePayTypeOpensCashDrawer,cbOpendrawer->Checked);
        Payment.SetPaymentAttribute(ePayTypeCash,cbIsCash->Checked); 
        Payment.SetPaymentAttribute(ePayTypeAllowCashOut,cbCashOut->Checked);
        Payment.SetPaymentAttribute(ePayTypeElectronicTransaction,cbElectronicTransaction->Checked);
        Payment.SetPaymentAttribute(ePayTypeCheckAccepted,cbCheckAccepted->Checked);
        Payment.SetPaymentAttribute(ePayTypeGetVoucherDetails,cbGetVoucherDetails->Checked);
        Payment.SetPaymentAttribute(ePayTypeGetCardDetails,cbGetCardDetails->Checked);
        Payment.SetPaymentAttribute(ePayTypeTaxFree,cbTaxFree->Checked);
        Payment.SetPaymentAttribute(ePayTypeReqNote,cbReqNote->Checked);
        Payment.SetPaymentAttribute(ePayTypeSecure1,cbSec1->Checked);
        Payment.SetPaymentAttribute(ePayTypeSecure2,cbSec2->Checked);
        Payment.SetPaymentAttribute(ePayTypeSecure3,cbSec3->Checked);
        Payment.SetPaymentAttribute(ePayTypeCSV,cbCSVPaymentType->Checked);
        Payment.SetPaymentAttribute(ePayTypeSurcharge,cbSurcharge->Checked);
        Payment.SetPaymentAttribute(ePayTypeRoomInterface,tbRoomPayment->Checked)  ;
        Payment.SetPaymentAttribute(ePayTypeIntegratedEFTPOS,cbIntegrated->Checked);
        Payment.SetPaymentAttribute(ePayTypeAllowReversal,cbAllowReversal->Checked);
        Payment.SetPaymentAttribute(ePayTypeAllowMANPAN,cbAllowManPan->Checked);
        Payment.SetPaymentAttribute(ePayTypeCheckSignature,cbCheckSig->Checked);
        Payment.SetPaymentAttribute(ePayTypeChequeVerify,tbChequeVerify->Checked);
        Payment.SetPaymentAttribute(ePayTypeSecondaryPMSExport,SecondaryPMSIPAddress != "");
        Payment.SetPaymentAttribute(ePayTypePocketVoucher,cbPocketVoucher->Checked);
        Payment.SetPaymentAttribute(ePayTypeDispPVMsg,cbPVAcceptedMsg->Checked);
        Payment.SetPaymentAttribute(ePayTypeChargeToAccount,CheckBoxExport->Checked);
        Payment.SetPaymentAttribute(ePayTypeChargeToXero,tbChargeToXero->Checked);
        Payment.SetPaymentAttribute(ePayTypeRMSInterface,cbRMSInterface->Checked);
        Payment.SetPaymentAttribute(ePayTypeAllowTips,cbAllowTips->Checked);
        Payment.SetPaymentAttribute(ePayTypeWallet,cbWalletPayments->Checked);
        if (Reason != "")
        {
            Payment.AdjustmentReason = Reason;
            if (SurchargeIsAPercentAdjust)
            {
                Payment.AmountAdjust = 0;
                Payment.PercentAdjust = SurchargeAmount;
            }
            else
            {
                Payment.AmountAdjust = SurchargeAmount;
                Payment.PercentAdjust = 0;
            }
        }
        else
        {
             Payment.AdjustmentReason = "";
             Payment.AmountAdjust = 0;
             Payment.PercentAdjust = 0;
        }

        PaymentSystem->PaymentSave(DBTransaction, PaymentKey, Payment);
   }
   catch(EDatabaseError & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Err.Message);
	  MessageBox("A Payment type of with name may already exists.\rPlease enter a different Payment name.", "Error", MB_OK);
   }
   //................................
   DBTransaction.Commit();
   Close();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::Panel20Click(TObject *Sender)
{
   Close();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::FormShow(TObject *Sender)
{
   FormResize(NULL);
   Pages->ActivePage = tsPayment;
   tbPayment->ButtonColor = clNavy;
   if (PaymentKey != 0)
   {
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        PaymentSystem->PaymentLoad(DBTransaction, PaymentKey, Payment);
        DBTransaction.Commit();

        btnName->Caption = Payment.Name;
        btnName->ButtonColor = static_cast <TColor> (Payment.Colour);
        PaymentPos = Payment.DisplayOrder;
        PaymentGroup = Payment.GroupNumber;
        ExchangeRate = 0.0;
        Reason = Payment.AdjustmentReason;
        PaymentThirdPartyID = Payment.PaymentThirdPartyID;
        VoucherMerchantID = Payment.VoucherMerchantID;
        SecondaryPMSIPAddress = Payment.SecondaryPMSIPAddress;
        SecondaryPMSPortNumber = Payment.SecondaryPMSPortNumber;
        CVSReadLocation = Payment.CVSReadLocation;
        CVSWriteLocation = Payment.CVSWriteLocation;
        TaxRate = Payment.TaxRate;
        RoundTo = Payment.RoundTo;
        SurchargeIsAPercentAdjust = false;
        UniUser = Payment.UniVoucherUser;
        UniPass = Payment.UniVoucherPass;
        TabKey  = Payment.TabKey;
        GLCode  = Payment.GLCode;
        cbAutoPopulateBlindBalance->Checked = Payment.AutoPopulateBlindBalance;;


        if (Payment.PercentAdjust != 0)
        {
            SurchargeIsAPercentAdjust = true;
            SurchargeAmount = Payment.PercentAdjust;
        }
        else if (Payment.AmountAdjust != 0)
        {
            SurchargeAmount = Payment.AmountAdjust;
        }

        if (Reason != "")
        {
            if (SurchargeIsAPercentAdjust)
            {
                if (SurchargeAmount > 0)
                {
                    cbSurcharge->Caption = Reason + "\rDiscount " + FloatToStrF(fabs(SurchargeAmount), ffGeneral, 15, 2) + "%";
                }
                else
                {
                    cbSurcharge->Caption = Reason + "\rSurcharge " + FloatToStrF(fabs(SurchargeAmount), ffGeneral, 15, 2) + "%";
                }
            }
            else
            {
                if (SurchargeAmount > 0)
                {
                    cbSurcharge->Caption = Reason + "\rDiscount " + FloatToStrF(fabs(SurchargeAmount), ffCurrency, 15, 2);
                }
                else
                {
                    cbSurcharge->Caption = Reason + "\rSurcharge " + FloatToStrF(fabs(SurchargeAmount), ffCurrency, 15, 2);
                }
            }
        }
        else
        {
            cbSurcharge->Caption = "Payment type Surcharge";
        }

        tbExchange->Caption = "Exchange Rate \r" + FloatToStrF(ExchangeRate, ffGeneral, 5, 4);
        tbPosition->Caption = "Position  \r" + IntToStr(PaymentPos);
        tbGroupNumber->Caption = "Group Number\r" + IntToStr(PaymentGroup);
        tbThirdPartyID->Caption = "Third Party Payment ID\r" + PaymentThirdPartyID;
        tbtnVoucherMerchant->Caption = "Voucher Merchant ID\r" + VoucherMerchantID;
        tbtnSecondaryIPAddress->Caption = "Seconday PMS IP Address\r" + SecondaryPMSIPAddress;
        tbtnSecondaryPMSPort->Caption = "Secondary PMS Port\r" + IntToStr(SecondaryPMSPortNumber);
        tbSurchargeTaxRate->Caption = "Surcharge Tax Rate \r" + FloatToStrF(fabs(TaxRate), ffGeneral, 15, 2) + "%";
        tbRounding->Caption = "Round To\r" + FormatFloat("$0.00", RoundTo);

        if(GLCode == "")
            tbGLCode->Caption = "GL Code";
        else
            tbGLCode->Caption = "GL Code\r" +  GLCode;

        DBTransaction.StartTransaction();
        TabName   = TDBTab::GetTabName(DBTransaction,TabKey);
        DBTransaction.Commit();

        if(TabName == "")
            tbTabLink->Caption = "Tab Link";
        else
            tbTabLink->Caption = "Tab Link \r" + TabName;

        double R, G, B; // input RGB values
        R = GetRValue(btnName->Color);
        G = GetGValue(btnName->Color);
        B = GetBValue(btnName->Color);
        double L = (0.2126 * R) + (0.7152 * G) + (0.0722 * B);
        if (L > 128)
        {
            btnName->Font->Color = clBlack;
        }
        else
        {
            btnName->Font->Color = clWhite;
        }



        if (Payment.GetPaymentAttribute(ePayTypeSurcharge))
        {
            cbSurcharge->OnClick = NULL;
            cbSurcharge->Checked = true;
            cbSurcharge->OnClick = cbSurchargeClick;
        }
        else
        {
            cbSurcharge->Checked = false;
        }


        cbOpendrawer->Checked = Payment.GetPaymentAttribute(ePayTypeOpensCashDrawer);
        cbIsTip->OnClick = NULL;
        cbIsTip->Checked = Payment.GetPaymentAttribute(ePayTypeCustomSurcharge);
        cbIsTip->OnClick = cbIsTipClick;
        cbCashOut->Checked = Payment.GetPaymentAttribute(ePayTypeAllowCashOut);

        if (Payment.GetPaymentAttribute(ePayTypeCash))
        {
            cbIsCash->Checked = true;
            tbTabLink->Enabled=false;
        }
        else
        {
            cbIsCash->Checked = false;
            tbTabLink->Enabled=true;
        }
        cbElectronicTransaction->Checked = Payment.GetPaymentAttribute(ePayTypeElectronicTransaction);
        cbCheckAccepted->Checked = Payment.GetPaymentAttribute(ePayTypeCheckAccepted);
        cbGetVoucherDetails->Checked = Payment.GetPaymentAttribute(ePayTypeGetVoucherDetails);
        cbGetCardDetails->Checked = Payment.GetPaymentAttribute(ePayTypeGetCardDetails);
        cbTaxFree->Checked = Payment.GetPaymentAttribute(ePayTypeTaxFree);
        cbReqNote->Checked = Payment.GetPaymentAttribute(ePayTypeReqNote);
        cbSec1->Checked = Payment.GetPaymentAttribute(ePayTypeSecure1);
        cbSec2->Checked = Payment.GetPaymentAttribute(ePayTypeSecure2);
        cbSec3->Checked = Payment.GetPaymentAttribute(ePayTypeSecure3);
        cbCSVPaymentType->Checked = Payment.GetPaymentAttribute(ePayTypeCSV);
        cbPocketVoucher->Checked = Payment.GetPaymentAttribute(ePayTypePocketVoucher);
        tbRoomPayment->Checked = Payment.GetPaymentAttribute(ePayTypeRoomInterface);
        cbIntegrated->Checked = Payment.GetPaymentAttribute(ePayTypeIntegratedEFTPOS);
        cbAllowReversal->Checked = Payment.GetPaymentAttribute(ePayTypeAllowReversal);
        cbAllowManPan->Checked = Payment.GetPaymentAttribute(ePayTypeAllowMANPAN);
        cbCheckSig->Checked = Payment.GetPaymentAttribute(ePayTypeCheckSignature);
        tbChequeVerify->Checked = Payment.GetPaymentAttribute(ePayTypeChequeVerify);
        cbPVAcceptedMsg->Checked = Payment.GetPaymentAttribute(ePayTypeDispPVMsg);
        cbRMSInterface->Checked = Payment.GetPaymentAttribute(ePayTypeRMSInterface);
        cbAllowTips->Checked = Payment.GetPaymentAttribute(ePayTypeAllowTips);
        if (Payment.GetPaymentAttribute(ePayTypeChargeToAccount))
        {
            CheckBoxExport->Checked = true;
        }
        else
        {
            CheckBoxExport->Checked = false;
        }
        if(TGlobalSettings::Instance().IsXeroEnabled && Payment.GetPaymentAttribute(ePayTypeChargeToAccount))
        {
            if(Payment.GetPaymentAttribute(ePayTypeChargeToXero))
            {
                tbChargeToXero->Checked = true;
            }
            else
            {
                tbChargeToXero->Checked = false;
            }


        }
        else
        {
            tbChargeToXero->Enabled = false;
            tbChargeToXero->Checked = false;
        }
        cbWalletPayments->Checked = Payment.GetPaymentAttribute(ePayTypeWallet);
        btnWalletType->Enabled = cbWalletPayments->Checked;
        btnWalletConfig->Enabled = cbWalletPayments->Checked;


	  tbtnUniUser->Caption = "Universal User\r" + UniUser;
	  tbtnUniPass->Caption = "Universal Password\r" + UniPass;
   }
   else
   {
	  btnName->Caption = "Payment Name";
	  cbElectronicTransaction->Checked = false;
	  cbCheckSig->Checked = false;
	  cbCashOut->Checked = false;
	  RoundTo = MIN_CURRENCY_VALUE;
	  tbRounding->Caption = "Round To\r" + FormatFloat("$0.00", RoundTo);
      tbChargeToXero->Enabled =  TGlobalSettings::Instance().IsXeroEnabled && Payment.GetPaymentAttribute(ePayTypeChargeToAccount);
      btnWalletType->Enabled = cbWalletPayments->Checked;
      btnWalletConfig->Enabled = cbWalletPayments->Checked;
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::FormResize(TObject *Sender)
{
   if (Tag != Screen->Width)
   {
	  int Temp = Tag;
	  Tag = Screen->Width;
      if((double)Screen->Width/Screen->Height < 1.4)
      {
        ScaleBy(Screen->Width, Temp);
      }
   }

   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;
   pnlButtons->Height = ClientHeight - pnlButtons->Top;
   Pages->Height = ClientHeight - Pages->Top;
   Pages->Width = pnlButtons->Left - this->BorderWidth;
   TouchBtn2->Top = pnlButtons->Height - TouchBtn2->Height - 8;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::btnNameClick(TObject *Sender)
{
    UnicodeString paymentName = TStringTools::Instance()->UpperCaseWithNoSpace(btnName->Caption);
    bool isPaymentNameEditable = paymentName != "CASH" && paymentName != "CHEQUE" && paymentName != "EFTPOS" && paymentName != "AMEX" && paymentName != "DINERS" && paymentName != "VISA" && paymentName != "MASTERCARD" && paymentName != "TIPS";
    if(isPaymentNameEditable)
    {
           std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
           frmTouchKeyboard->MaxLength = 25;
           frmTouchKeyboard->AllowCarriageReturn = false;
           frmTouchKeyboard->StartWithShiftDown = false;
           if (PaymentKey)
           {
              frmTouchKeyboard->KeyboardText = btnName->Caption;
           }
           else
           {
              frmTouchKeyboard->KeyboardText = "";
           }
           frmTouchKeyboard->Caption = "Enter Payment Name";
           std::auto_ptr<TfrmPaymentMaintenance>(frmPaymentMaintenance)(TfrmPaymentMaintenance::Create(this,TDeviceRealTerminal::Instance().DBControl,TDeviceRealTerminal::Instance().PaymentSystem));
           Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	        DBTransaction.StartTransaction();
           if (frmTouchKeyboard->ShowModal() == mrOk)
           {
                if(!frmPaymentMaintenance->IsPaymentExist(DBTransaction, frmTouchKeyboard->KeyboardText.UpperCase()))
                {
                    if (frmTouchKeyboard->KeyboardText.UpperCase() == "CREDIT")
                      {
                         MessageBox("'Credit' is used internally by MenuMate. Payment Name changed to 'Credit Card'", "Error", MB_OK);
                         frmTouchKeyboard->KeyboardText = "Credit Card";
                      }
                      btnName->Caption = frmTouchKeyboard->KeyboardText;
                }
                else
                {
                    MessageBox("This Payment Name already Exist", "Error", MB_OK);
                }
           }
           DBTransaction.Commit();
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::ccgColourChange(TObject *Sender)
{
   btnName->ButtonColor = ccgColour->ForegroundColor;
   double R, G, B; // input RGB values
   R = GetRValue(btnName->ButtonColor);
   G = GetGValue(btnName->ButtonColor);
   B = GetBValue(btnName->ButtonColor);
   double L = (0.2126 * R) + (0.7152 * G) + (0.0722 * B);
   if (L > 128)
   {
	  btnName->Font->Color = clBlack;
   }
   else
   {
	  btnName->Font->Color = clWhite;
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbPositionClick(TObject *Sender)
{
   std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
   frmTouchNumpad->Caption = "Enter Postional Number";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = PaymentPos;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
	  PaymentPos = frmTouchNumpad->INTResult;
   }
   tbPosition->Caption = "Position  \r" + IntToStr(PaymentPos);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbExchangeClick(TObject *Sender)
{
   std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
   frmTouchNumpad->Caption = "Enter Exchange Rate";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->Mode = pmCurrency;
   frmTouchNumpad->CURInitial = ExchangeRate;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
	  ExchangeRate = frmTouchNumpad->CURResult;
	  tbExchange->Caption = "Exchange Rate \r" + FloatToStrF(ExchangeRate, ffGeneral, 5, 4);
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbSurchargeClick(TObject *Sender)
{
   if (cbSurcharge->Checked)
   {
	  if (cbIsTip->Checked)
	  {
		 cbIsTip->Checked = false;
	  }

	  // Get the surcharge reason.
	  std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	  frmTouchKeyboard->MaxLength = 20;
	  frmTouchKeyboard->AllowCarriageReturn = false;
	  frmTouchKeyboard->StartWithShiftDown = false;
	  frmTouchKeyboard->MustHaveValue = true;
	  if (PaymentKey)
	  {
		 frmTouchKeyboard->KeyboardText = Reason;
	  }
	  else
	  {
		 frmTouchKeyboard->KeyboardText = "";
	  }
	  frmTouchKeyboard->Caption = "Enter Surchage Reason i.e Cheque Fee";
	  if (frmTouchKeyboard->ShowModal() == mrOk)
	  {
		 Reason = frmTouchKeyboard->KeyboardText;
	  }
	  else
	  {
		 cbSurcharge->Checked = false;
	  }

	  std::auto_ptr <TfrmDiscount> frmDiscount(TfrmDiscount::Create <TfrmDiscount> (this));
      frmDiscount->ForceType(avtSurcharge);
	  // Get the surcharge amount.
	  if (SurchargeIsAPercentAdjust)
	  {
		 frmDiscount->Mode = DiscModePercent;
		 frmDiscount->PERCInitial = SurchargeAmount;
		 frmDiscount->CURInitial = 0;
	  }
	  else
	  {
		 frmDiscount->Mode = DiscModeCurrency;
		 frmDiscount->PERCInitial = 0;
		 frmDiscount->CURInitial = SurchargeAmount;
	  }

	  if (frmDiscount->ShowModal() == mrOk)
	  {
		 if(frmDiscount->ValueType ==  avtDiscount)
         {
             MessageBox("Surcharges Only, Discouint are not allowed.", "Error", MB_OK);
			 Reason = "";
			 cbSurcharge->Checked = false;
         }
		 else if (frmDiscount->Mode == DiscModePercent)
		 {
			SurchargeAmount = frmDiscount->PERCResult;
			SurchargeIsAPercentAdjust = true;
		 }
		 else
		 {
			SurchargeAmount = frmDiscount->CURResult;
			SurchargeIsAPercentAdjust = false;
		 }
	  }
	  else
	  {
		 Reason = "";
		 cbSurcharge->Checked = false;
	  }

	  if (Reason != "")
	  {
		 if (SurchargeIsAPercentAdjust)
		 {
			if (SurchargeAmount > 0)
			{
			   cbSurcharge->Caption = Reason + "\rDiscount " + FloatToStrF(fabs(SurchargeAmount), ffGeneral, 15, 2) + "%";
			}
			else
			{
			   cbSurcharge->Caption = Reason + "\rSurcharge " + FloatToStrF(fabs(SurchargeAmount), ffGeneral, 15, 2) + "%";
			}
		 }
		 else
		 {
			if (SurchargeAmount > 0)
			{
			   cbSurcharge->Caption = Reason + "\rDiscount " + FloatToStrF(fabs(SurchargeAmount), ffCurrency, 15, 2);
			}
			else
			{
			   cbSurcharge->Caption = Reason + "\rSurcharge " + FloatToStrF(fabs(SurchargeAmount), ffCurrency, 15, 2);
			}
		 }
	  }
	  else
	  {
		 cbSurcharge->Caption = "Payment type Surcharge";
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbIsTipClick(TObject *Sender)
{
   if (cbIsTip->Checked)
   {
	  if (cbCashOut->Checked)
	  {
		 cbCashOut->Checked = false;
	  }

	  if (cbSurcharge->Checked)
	  {
		 cbSurcharge->Checked = false;
	  }

	  std::auto_ptr <TfrmDiscount> frmDiscount(TfrmDiscount::Create <TfrmDiscount> (this));
      frmDiscount->ForceType(avtSurcharge);
	  frmDiscount->Caption = "Surcharge Tax rate";
	  frmDiscount->tbToggleAmount->Visible = false;
	  frmDiscount->pnlToggle->Visible = false;
	  // Get the surcharge amount.
	  frmDiscount->Mode = DiscModePercent;
	  if (TaxRate == 0)
	  {
		 frmDiscount->PERCInitial = 15;
	  }
	  else
	  {
		 frmDiscount->PERCInitial = TaxRate;
	  }
	  frmDiscount->CURInitial = 0;
	  if (frmDiscount->ShowModal() == mrOk)
	  {
		 TaxRate = frmDiscount->PERCResult;
		 tbSurchargeTaxRate->Caption = "Surcharge Tax Rate \r" + FloatToStrF(fabs(TaxRate), ffGeneral, 15, 2) + "%";
	  }

   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbCashOutClick(TObject *Sender)
{
   if (cbCashOut->Checked)
   {
	  if (cbIsTip->Checked)
	  {
		 cbIsTip->Checked = false;
	  }

	  if (cbIsCash->Checked)
	  {
		 cbIsCash->Checked = false;
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbRoomPaymentClick(TObject *Sender)
{
   if (tbRoomPayment->Checked)
   {
	  if (!TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled)
	  {
		 MessageBox("You must have the Rooms or PMS Module in order to use Room Payments.", "Error", MB_OK);
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbGroupNumberClick(TObject *Sender)
{
   std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
   frmTouchNumpad->Caption = "Enter Group Number";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = PaymentGroup;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
	  PaymentGroup = frmTouchNumpad->INTResult;
   }
   tbGroupNumber->Caption = "Group Number\r" + IntToStr(PaymentGroup);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbIsCashClick(TObject *Sender)
{   tbTabLink->Enabled=true;
   if (cbIsCash->Checked)
   {
        if(cbCashOut->Checked)
        {
           cbCashOut->Checked = false;
        }
        tbTabLink->Enabled=false;
        Payment.TabKey=0;
        TabKey=0;
        tbTabLink->Caption = "Tab Link";
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbIntegratedClick(TObject *Sender)
{
   if (cbIntegrated->Checked)
   {
	  cbElectronicTransaction->Checked = true;
	  if (cbIsTip->Checked)
	  {
		 cbIsTip->Checked = false;
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbChequeVerifyClick(TObject *Sender)
{
   if (tbChequeVerify->Checked)
   {
	  if (cbIsTip->Checked)
	  {
		 cbIsTip->Checked = false;
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbElectronicTransactionClick(TObject *Sender)
{
   if (cbElectronicTransaction->Checked)
   {
     CheckBoxExport->Checked = false;
     tbChargeToXero->Checked = false;
     CheckBoxExport->Enabled = false;
     tbChargeToXero->Enabled = false;
     if (cbIsTip->Checked)
	  {
		 cbIsTip->Checked = false;
	  }
   }
  else
   {
     CheckBoxExport->Enabled = TGlobalSettings::Instance().IsXeroEnabled;
     tbChargeToXero->Enabled = true;
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbAllowReversalClick(TObject *Sender)
{
   if (cbAllowReversal->Checked)
   {
	  if (cbIsTip->Checked)
	  {
		 cbIsTip->Checked = false;
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbThirdPartyIDClick(TObject *Sender)
{
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 25;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->KeyboardText = PaymentThirdPartyID;
   frmTouchKeyboard->Caption = "Enter the third party Payment ID.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  if (TDeviceRealTerminal::Instance().BasePMS->TestCode(frmTouchKeyboard->KeyboardText))
	  {
		 PaymentThirdPartyID = frmTouchKeyboard->KeyboardText;
	  }
	  TDeviceRealTerminal::Instance().BasePMS->ClearCodesTestedOk();
   }

   tbThirdPartyID->Caption = "Third Party Payment ID\r" + PaymentThirdPartyID;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbSurchargeTaxRateClick(TObject *Sender)
{
   std::auto_ptr <TfrmDiscount> frmDiscount(TfrmDiscount::Create <TfrmDiscount> (this));
   frmDiscount->Caption = "Surcharge Tax rate";
   frmDiscount->tbToggleAmount->Visible = false;
   frmDiscount->pnlToggle->Visible = false;
   // Get the surcharge amount.
   frmDiscount->Mode = DiscModePercent;
   frmDiscount->PERCInitial = TaxRate;
   frmDiscount->CURInitial = 0;
   if (frmDiscount->ShowModal() == mrOk)
   {
	  TaxRate = frmDiscount->PERCResult;
	  tbSurchargeTaxRate->Caption = "Surcharge Tax Rate \r" + FloatToStrF(TaxRate, ffGeneral, 15, 2) + "%";
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbRoundingMouseClick(TObject *Sender)
{
   std::auto_ptr <TfrmDiscount> frmDiscount(TfrmDiscount::Create <TfrmDiscount> (this));

   frmDiscount->Mode = DiscModeCurrency;
   frmDiscount->PERCInitial = 0;
   frmDiscount->CURInitial = SurchargeAmount;
   frmDiscount->tbToggleAmount->Visible = false;
   frmDiscount->pnlToggle->Visible = false;

   if (frmDiscount->ShowModal() == mrOk)
   {
	  RoundTo = frmDiscount->CURResult;
	  tbRounding->Caption = "Round To\r" + FormatFloat("$0.00", RoundTo);
   }

}
// ---------------------------------------------------------------------------
//void __fastcall TfrmNewPaymentType::tbInvoiceInterfaceClick(TObject *Sender)
//{
//   if (tbInvoiceInterface->Checked)
//   {
//	  if (!TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
//	  {
//		 MessageBox("You must have the Membership Module in order to use Invoice Payments.", "Error", MB_OK);
//	  }
//   }
//}
// ---------------------------------------------------------------------------
void TfrmNewPaymentType::RedrawButtons(TObject * Sender)
{
   TTouchBtn* CurrentButton = (TTouchBtn*)Sender;
   CurrentButton->ButtonColor = clNavy;

   for (int i = 0; i < pnlButtons->ControlCount; i++)
   {
	  TTouchBtn* Button = (TTouchBtn*)pnlButtons->Controls[i];
	  if (Button->Tag != -1 && Button != CurrentButton)
	  {
		 Button->ButtonColor = clGray; //clMaroon;
	  }
   }
}

void __fastcall TfrmNewPaymentType::tbPaymentMouseClick(TObject *Sender)
{
   Pages->ActivePage = tsPayment;
   RedrawButtons(Sender);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbSecurityMouseClick(TObject *Sender)
{
   Pages->ActivePage = tsSecurity;
   RedrawButtons(Sender);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbInterfacesMouseClick(TObject *Sender)
{
   Pages->ActivePage = tsInterfaces;
   RedrawButtons(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbtnSecondaryIPAddressClick(TObject *Sender)
{
   if (!TDeviceRealTerminal::Instance().BasePMS->Registered)
   {
	  MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
   }
   else
   {
	  std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	  frmTouchKeyboard->MaxLength = 255;
	  frmTouchKeyboard->AllowCarriageReturn = false;
	  frmTouchKeyboard->StartWithShiftDown = false;
	  frmTouchKeyboard->KeyboardText = SecondaryPMSIPAddress;
	  frmTouchKeyboard->Caption = "Enter the IP Address of the Secondary Hotel System.";
	  if (frmTouchKeyboard->ShowModal() == mrOk)
	  {
		 SecondaryPMSIPAddress = frmTouchKeyboard->KeyboardText;
		 tbtnSecondaryIPAddress->Caption = "Server TCP IP Address\r" + SecondaryPMSIPAddress;
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbtnSecondaryPMSPortClick(TObject *Sender)
{
   if (!TDeviceRealTerminal::Instance().BasePMS->Registered)
   {
	  MessageBox("You must have the PMS Module in order to Interface with PMS Hotel System .", "Error", MB_OK);
   }
   else
   {
	  std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
	  frmTouchNumpad->Caption = "Enter the Port Number of the Secondary PMS Hotel System.";
	  frmTouchNumpad->btnSurcharge->Caption = "Ok";
	  frmTouchNumpad->btnSurcharge->Visible = true;
	  frmTouchNumpad->btnDiscount->Visible = false;
	  frmTouchNumpad->Mode = pmNumber;
	  frmTouchNumpad->INTInitial = SecondaryPMSPortNumber;
	  if (frmTouchNumpad->ShowModal() == mrOk)
	  {
		 SecondaryPMSPortNumber = frmTouchNumpad->INTResult;
		 tbtnSecondaryPMSPort->Caption = "PMS Server Port Number\r" + IntToStr(SecondaryPMSPortNumber);
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbtnVoucherMerchantMouseClick(TObject *Sender)

{
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 42;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->KeyboardText = VoucherMerchantID;
   frmTouchKeyboard->Caption = "Enter the Voucher Merchant ID.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  VoucherMerchantID = frmTouchKeyboard->KeyboardText;
   }

   tbtnVoucherMerchant->Caption = "Voucher Merchant ID\r" + VoucherMerchantID;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbGetVoucherDetailsClick(TObject *Sender)
{
   if (cbGetVoucherDetails->Checked)
   {
	  if (cbPocketVoucher->Checked)
	  {
		 cbPocketVoucher->Checked = false;
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbPocketVoucherClick(TObject *Sender)
{
   if (cbPocketVoucher->Checked)
   {
	  if (cbGetVoucherDetails->Checked)
	  {
		 cbGetVoucherDetails->Checked = false;
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::ExportMouseClick(TObject *Sender)
{
   if (CheckBoxExport->Checked)
   {
	  if (!TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
	  {
		 MessageBox("You must have the Membership Module in order to use Invoice Payments.", "Error", MB_OK);
	  }
      tbChargeToXero->Enabled = true;
   }
   else
   {
     tbChargeToXero->Checked = false;
     tbChargeToXero->Enabled = false;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbtnUniUserMouseClick(TObject *Sender)
{
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   if (UniUser == "")
   {
	  frmTouchKeyboard->KeyboardText = "menumate";
   }
   else
   {
	  frmTouchKeyboard->KeyboardText = UniUser;
   }

   frmTouchKeyboard->MaxLength = 50;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->Caption = "Enter the User for Voucher System Auth.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  UniUser = frmTouchKeyboard->KeyboardText;
   }

   tbtnUniUser->Caption = "Universal User\r" + UniUser;
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbtnUniPassMouseClick(TObject *Sender)
{
std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   if (UniPass == "")
   {
	  frmTouchKeyboard->KeyboardText = "";
   }
   else
   {
	  frmTouchKeyboard->KeyboardText = UniPass;
   }

   frmTouchKeyboard->MaxLength = 25;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->Caption = "Password for Voucher System Auth.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  UniPass = frmTouchKeyboard->KeyboardText;
   }

   tbtnUniPass->Caption = "Universal Password\r" + UniPass;
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbChargeToXeroClick(TObject *Sender)
{
	if( !TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"] &&
        ( ( TCheckBox * )Sender )->Checked
       )
	{
	   MessageBox("You must have the Membership Module in order to use Invoice Payments.", "Error", MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbRMSInterfaceClick(TObject *Sender)
{
   /*if (cbRMSInterface->Checked)
   {
	  if (!TRooms::Instance().Enabled )
	  {
		 MessageBox("You must have the Rooms Module in order to use Room Payments.", "Error", MB_OK);
	  }
   } */
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbRMSReadLocationMouseClick(TObject *Sender)
{
   if(CVSReadLocation == "")
   {
	   CVSReadLocation = ExtractFilePath(Application->ExeName) + "Import\\RMS";
   }
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 255;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->KeyboardText = CVSReadLocation;
   frmTouchKeyboard->Caption = "Enter the RMS CSV File Location for reading.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
		CVSReadLocation = frmTouchKeyboard->KeyboardText;
		if (!DirectoryExists(CVSReadLocation))
		{
			if(MessageBox("The Directory Does not exist do you wish to create it? \n" + CVSReadLocation, "Warning",
			MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
			{
				ForceDirectories(CVSReadLocation);
			}
		}
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbRMSWriteLocationMouseClick(TObject *Sender)

{
   if(CVSWriteLocation == "")
   {
	   CVSWriteLocation = ExtractFilePath(Application->ExeName) + "Export";
   }
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 255;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->KeyboardText = CVSWriteLocation;
   frmTouchKeyboard->Caption = "Enter the RMS CSV File Location for writing.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
		 CVSWriteLocation = frmTouchKeyboard->KeyboardText;
		 if (!DirectoryExists(CVSWriteLocation))
		 {
			if(MessageBox("The Directory Does not exist do you wish to create it? \n" + CVSWriteLocation, "Warning",
			MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
			{
				ForceDirectories(CVSWriteLocation);
			}
		}
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::tbTabLinkMouseClick(TObject *Sender)
{
     Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,0);

  GetTabDetailDetails(DBTransaction);
   DBTransaction.Commit();

}

void TfrmNewPaymentType::GetTabDetailDetails(Database::TDBTransaction &DBTransaction)
{
    bool    Retval = mrOk;
    int Count = 0;
    int	tabKey=0;
    std::auto_ptr <TStringList> TabList(new TStringList);
    TDBTab::GetTabs(DBTransaction, TabList.get(), CurrentDestTabType);
    for (int i = 0; i < TabList->Count; i++)
    {
        Count++;
    }

    std::auto_ptr<TfrmVerticalSelect>SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));
    TVerticalSelection Item;
    Item.Title = "Cancel";
    Item.Properties["Color"] = "0x000098F5";
    Item.Properties["FontColor"] = IntToStr(clWhite);;
    Item.CloseSelection = true;

    SelectionForm->Items.push_back(Item);
    TDBTab::GetTabs(DBTransaction, TabList.get(), CurrentDestTabType);
    for (int i = 0; i < TabList->Count; i++)
    {
        Item.Title = TabList->Strings[i];
        Item.Properties["TabKey"] = (int)TabList->Objects[i];
        Item.Properties["Color"] = clInfoBk;
        Item.CloseSelection = true;
        if (Item.Properties["TabKey"]==Payment.TabKey)
        {
            Item.Properties["Color"]  = "0x00BF9D00";//clGreen;
        }
        else
        {
            Item.Properties["Color"]  ="0x00605D5C"; //clRed;
        }
        SelectionForm->Items.push_back(Item);
    }
    SelectionForm->ShowModal();
    TVerticalSelection SelectedItem;
    if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
    {
        std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

        TVerticalSelection Item;
        Item.Title = "Cancel";
        Item.Properties["Color"] = "0x000098F5";
        Item.Properties["FontColor"] = IntToStr(clWhite);;
        Item.CloseSelection = true;
        SelectionForm1->Items.push_back(Item);

        TVerticalSelection Item1;
        Item1.Title = "Enable";
        Item1.Properties["Action"] = IntToStr(1);
        Item1.Properties["Color"] = IntToStr(clGreen);
        Item1.CloseSelection = true;
        SelectionForm1->Items.push_back(Item1);

        TVerticalSelection Item2;
        Item2.Title = "Disable";
        Item2.Properties["Action"] = IntToStr(2);
        Item2.Properties["Color"] = IntToStr(clRed);
        Item2.CloseSelection = true;
        SelectionForm1->Items.push_back(Item2);

        SelectionForm1->ShowModal();
        TVerticalSelection SelectedItem1;
        if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
        {
            int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
            switch(Action)
            {
                case 1 :
                    tabKey=	SelectedItem.Properties["TabKey"];
                    break;
                case 2 :
                    tabKey=0;
                    break;
            }
        }

        if(tabKey==0)
        {
            SelectedItem.Properties["Color"]  ="0x00605D5C";// clRed;
        }
        else
        {
            SelectedItem.Properties["Color"]  ="0x00BF9D00"; //clGreen;
        }

        if (Retval == mrOk)
        {
            TabKey=0;
            TabName="";
            TabKey =tabKey;
            Payment.TabKey =tabKey ;
            TabName =SelectedItem.Title;
            if(tabKey==0)
                tbTabLink->Caption = "Tab Link";
            else
                tbTabLink->Caption = "Tab Link \r" + SelectedItem.Title;

        }
    }
    else
    {
        Retval = mrAbort;
    }

}

void __fastcall TfrmNewPaymentType::tbGLCodeMouseClick(TObject *Sender)
{
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 25;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   if (PaymentKey)
   {
	  frmTouchKeyboard->KeyboardText = GLCode;
   }
   else
   {
	  frmTouchKeyboard->KeyboardText = "";
   }
   frmTouchKeyboard->Caption = "Enter GL Code";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  GLCode = frmTouchKeyboard->KeyboardText;
	  if(GLCode == "")
           tbGLCode->Caption = "GL Code";
      else
          tbGLCode->Caption = "GL Code\r" +  GLCode;
   }

}

void __fastcall TfrmNewPaymentType::btnWalletConfigMouseClick(TObject *Sender)
{
   TfrmWalletConfiguration *frmWalletConfiguration = new TfrmWalletConfiguration(this);
   frmWalletConfiguration->MerchentId = Payment.MerchentId;
   frmWalletConfiguration->TerminalId = Payment.TerminalId;
   frmWalletConfiguration->WalletUserName = Payment.WalletUserName;
   frmWalletConfiguration->WalletPassword = Payment.WalletPassword;
   frmWalletConfiguration->WalletSecurityToken = Payment.WalletSecurityToken;
   if(frmWalletConfiguration->ShowModal() == mrOk)
   {
       Payment.MerchentId = frmWalletConfiguration->MerchentId;
       Payment.TerminalId = frmWalletConfiguration->TerminalId;
       Payment.WalletUserName = frmWalletConfiguration->WalletUserName;
       Payment.WalletPassword = frmWalletConfiguration->WalletPassword;
       Payment.WalletSecurityToken = frmWalletConfiguration->WalletSecurityToken;
   }
   delete frmWalletConfiguration;
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::btnWalletTypeMouseClick(TObject *Sender)
{
    std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

    TVerticalSelection Item;
    Item.Title = "Cancel";
    Item.Properties["Color"] = "0x000098F5";
    Item.Properties["FontColor"] = IntToStr(clWhite);;
    Item.CloseSelection = true;
    SelectionForm->Items.push_back(Item);

    TVerticalSelection Item1;
    Item1.Title = "We Chat";
    Item1.Properties["Action"] = IntToStr(1);
    Item1.Properties["Color"] = (Payment.WalletType == eWeChatWallet) ? IntToStr(clGreen) : IntToStr(clNavy);
    Item1.CloseSelection = true;
    SelectionForm->Items.push_back(Item1);

    TVerticalSelection Item2;
    Item2.Title = "Jio";
    Item2.Properties["Action"] = IntToStr(2);
    Item2.Properties["Color"] = (Payment.WalletType == eJioWallet) ? IntToStr(clGreen) : IntToStr(clNavy);;
    Item2.CloseSelection = true;
    SelectionForm->Items.push_back(Item2);

    SelectionForm->ShowModal();
    TVerticalSelection SelectedItem;
    if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
    {
        int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
        switch(Action)
        {
            case 1 :
                Payment.WalletType = eWeChatWallet;
                break;
            case 2 :
                Payment.WalletType = eJioWallet;
                break;
            default:
                Payment.WalletType = eNoWallet;
                break;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewPaymentType::cbWalletPaymentsClick(TObject *Sender)
{
  btnWalletType->Enabled = cbWalletPayments->Checked;
  btnWalletConfig->Enabled = cbWalletPayments->Checked;
}
//---------------------------------------------------------------------------

