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
	  Payment.Properties = 0;
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
	  if (cbIsTip->Checked)
		 Payment.Properties |= ePayTypeCustomSurcharge;
	  if (cbOpendrawer->Checked)
		 Payment.Properties |= ePayTypeOpensCashDrawer;
	  if (cbIsCash->Checked)
		 Payment.Properties |= ePayTypeCash;
	  if (cbCashOut->Checked)
		 Payment.Properties |= ePayTypeAllowCashOut;
	  if (cbElectronicTransaction->Checked)
		 Payment.Properties |= ePayTypeElectronicTransaction;
	  if (cbCheckAccepted->Checked)
		 Payment.Properties |= ePayTypeCheckAccepted;
	  if (cbGetVoucherDetails->Checked)
		 Payment.Properties |= ePayTypeGetVoucherDetails;
	  if (cbGetCardDetails->Checked)
		 Payment.Properties |= ePayTypeGetCardDetails;
	  if (cbTaxFree->Checked)
		 Payment.Properties |= ePayTypeTaxFree;
	  if (cbReqNote->Checked)
		 Payment.Properties |= ePayTypeReqNote;
	  if (cbSec1->Checked)
		 Payment.Properties |= ePayTypeSecure1;
	  if (cbSec2->Checked)
		 Payment.Properties |= ePayTypeSecure2;
	  if (cbSec3->Checked)
		 Payment.Properties |= ePayTypeSecure3;
	  if (cbCSVPaymentType->Checked)
		 Payment.Properties |= ePayTypeCSV;
	  if (cbSurcharge->Checked)
		 Payment.Properties |= ePayTypeSurcharge;
	  if (tbRoomPayment->Checked)
		 Payment.Properties |= ePayTypeRoomInterface;
	  if (cbIntegrated->Checked)
		 Payment.Properties |= ePayTypeIntegratedEFTPOS;
	  if (cbAllowReversal->Checked)
		 Payment.Properties |= ePayTypeAllowReversal;
	  if (cbAllowManPan->Checked)
		 Payment.Properties |= ePayTypeAllowMANPAN;
	  if (cbCheckSig->Checked)
		 Payment.Properties |= ePayTypeCheckSignature;
	  if (tbChequeVerify->Checked)
		 Payment.Properties |= ePayTypeChequeVerify;
	  if (tbInvoiceInterface->Checked)
		 Payment.Properties |= ePayTypeInvoiceExport;
	  if (SecondaryPMSIPAddress != "")
		 Payment.Properties |= ePayTypeSecondaryPMSExport;
	  if (cbPocketVoucher->Checked)
		 Payment.Properties |= ePayTypePocketVoucher;
	  if (cbPVAcceptedMsg->Checked)
		 Payment.Properties |= ePayTypeDispPVMsg;
	  if(CheckBoxExport->Checked)
		 Payment.Properties |= ePayTypeChargeToAccount;
	  if(tbChargeToXero->Checked)
		 Payment.Properties |= ePayTypeChargeToXero;
	  if(cbRMSInterface->Checked)
		 Payment.Properties |= ePayTypeRMSInterface;
	  if(cbAllowTips->Checked)
		 Payment.Properties |= ePayTypeAllowTips;

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
	  //FixedVoucherCode = Payment.FixedVoucherCode;
	  VoucherMerchantID = Payment.VoucherMerchantID;
	  SecondaryPMSIPAddress = Payment.SecondaryPMSIPAddress;
	  SecondaryPMSPortNumber = Payment.SecondaryPMSPortNumber;
	  CVSReadLocation = Payment.CVSReadLocation;
	  CVSWriteLocation = Payment.CVSWriteLocation;
	  TaxRate = Payment.TaxRate;
	  RoundTo = Payment.RoundTo;
	  SurchargeIsAPercentAdjust = false;
     // UniURL = Payment.UniVoucherURL;
      UniUser = Payment.UniVoucherUser;
      UniPass = Payment.UniVoucherPass;
      TabKey  = Payment.TabKey;
      GLCode  = Payment.GLCode;


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
	  //tbtnFixedVoucherCode->Caption = "Fixed Voucher Code\r" + FixedVoucherCode;
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

	  int Properties = Payment.Properties;

	  if (Properties & ePayTypeSurcharge)
	  {
		 cbSurcharge->OnClick = NULL;
		 cbSurcharge->Checked = true;
		 cbSurcharge->OnClick = cbSurchargeClick;
	  }
	  else
	  {
		 cbSurcharge->Checked = false;
	  }

	  if (Properties & ePayTypeOpensCashDrawer)
		 cbOpendrawer->Checked = true;
	  else
		 cbOpendrawer->Checked = false;
	  cbIsTip->OnClick = NULL;
	  if (Properties & ePayTypeCustomSurcharge)
		 cbIsTip->Checked = true;
	  else
		 cbIsTip->Checked = false;
	  cbIsTip->OnClick = cbIsTipClick;
	  if (Properties & ePayTypeAllowCashOut)
		 cbCashOut->Checked = true;
	  else
		 cbCashOut->Checked = false;
	  if (Properties & ePayTypeCash)
		{
        cbIsCash->Checked = true;
          tbTabLink->Enabled=false;
      }
	  else
      {
		 cbIsCash->Checked = false;
         tbTabLink->Enabled=true;
       }
	  if (Properties & ePayTypeElectronicTransaction)
		 cbElectronicTransaction->Checked = true;
	  else
		 cbElectronicTransaction->Checked = false;
	  if (Properties & ePayTypeCheckAccepted)
		 cbCheckAccepted->Checked = true;
	  else
		 cbCheckAccepted->Checked = false;
	  if (Properties & ePayTypeGetVoucherDetails)
		 cbGetVoucherDetails->Checked = true;
	  else
		 cbGetVoucherDetails->Checked = false;
	  if (Properties & ePayTypeGetCardDetails)
		 cbGetCardDetails->Checked = true;
	  else
		 cbGetCardDetails->Checked = false;
	  if (Properties & ePayTypeTaxFree)
	  {
		 cbTaxFree->Checked = true;
	  }
	  else
	  {
		 cbTaxFree->Checked = false;
	  }
	  if (Properties & ePayTypeReqNote)
	  {
		 cbReqNote->Checked = true;
	  }
	  else
	  {
		 cbReqNote->Checked = false;
	  }
	  if (Properties & ePayTypeSecure1)
	  {
		 cbSec1->Checked = true;
	  }
	  else
	  {
		 cbSec1->Checked = false;
	  }
	  if (Properties & ePayTypeSecure2)
	  {
		 cbSec2->Checked = true;
	  }
	  else
	  {
		 cbSec2->Checked = false;
	  }
	  if (Properties & ePayTypeSecure3)
	  {
		 cbSec3->Checked = true;
	  }
	  else
	  {
		 cbSec3->Checked = false;
	  }
	  if (Properties & ePayTypeCSV)
	  {
		 cbCSVPaymentType->Checked = true;
	  }
	  else
	  {
		 cbCSVPaymentType->Checked = false;
	  }
	  if (Properties & ePayTypePocketVoucher)
	  {
		 cbPocketVoucher->Checked = true;
	  }
	  else
	  {
		 cbPocketVoucher->Checked = false;
	  }
	  if (Properties & ePayTypeRoomInterface)
	  {
		 tbRoomPayment->Checked = true;
	  }
	  else
	  {
		 tbRoomPayment->Checked = false;
	  }

	  if (Properties & ePayTypeIntegratedEFTPOS)
	  {
		 cbIntegrated->Checked = true;
	  }
	  else
	  {
		 cbIntegrated->Checked = false;
	  }
	  if (Properties & ePayTypeAllowReversal)
	  {
		 cbAllowReversal->Checked = true;
	  }
	  else
	  {
		 cbAllowReversal->Checked = false;
	  }
	  if (Properties & ePayTypeAllowMANPAN)
	  {
		 cbAllowManPan->Checked = true;
	  }
	  else
	  {
		 cbAllowManPan->Checked = false;
	  }
	  if (Properties & ePayTypeCheckSignature)
	  {
		 cbCheckSig->Checked = true;
	  }
	  else
	  {
		 cbCheckSig->Checked = false;
	  }
	  if (Properties & ePayTypeChequeVerify)
	  {
		 tbChequeVerify->Checked = true;
	  }
	  else
	  {
		 tbChequeVerify->Checked = false;
	  }
	  if (Properties & ePayTypeInvoiceExport)
	  {
		 tbInvoiceInterface->Checked = true;
	  }
	  else
	  {
		 tbInvoiceInterface->Checked = false;
	  }
	  if (Properties & ePayTypeDispPVMsg)
	  {
		 cbPVAcceptedMsg->Checked = true;
	  }
	  else
	  {
		 cbPVAcceptedMsg->Checked = false;
	  }
      if(TGlobalSettings::Instance().IsXeroEnabled)
      {
          if (Properties & ePayTypeChargeToAccount)
          {
            CheckBoxExport->Checked = true;
          }
          else
          {
            CheckBoxExport->Checked = false;
          }
      }
      else
      {
        CheckBoxExport->Enabled = false;
      }


      if(TGlobalSettings::Instance().IsXeroEnabled)
      {
          if(Properties & ePayTypeChargeToXero)
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
      }

	 if(Payment.Properties & ePayTypeRMSInterface)
	 {
		 cbRMSInterface->Checked = true;
	 }
	 else
	 {
		cbRMSInterface->Checked = false;
	 }

	 cbAllowTips->Checked = Payment.Properties & ePayTypeAllowTips ? true : false;
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
	  if (!TRooms::Instance().Enabled && !PhoenixHM->Enabled)
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
	  if (cbCashOut->Checked)
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
	  if (PhoenixHM->TestCode(frmTouchKeyboard->KeyboardText))
	  {
		 PaymentThirdPartyID = frmTouchKeyboard->KeyboardText;
	  }
	  PhoenixHM->ClearCodesTestedOk();
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

void __fastcall TfrmNewPaymentType::tbInvoiceInterfaceClick(TObject *Sender)
{
   if (tbInvoiceInterface->Checked)
   {
	  if (!TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
	  {
		 MessageBox("You must have the Membership Module in order to use Invoice Payments.", "Error", MB_OK);
	  }
   }
}

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
   if (!PhoenixHM->Registered)
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
   if (!PhoenixHM->Registered)
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

/*void __fastcall TfrmNewPaymentType::tbtnFixedVoucherCodeMouseClick(TObject *Sender)

{
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   if (FixedVoucherCode == "")
   {
	  frmTouchKeyboard->KeyboardText = "639491";
   }
   else
   {
	  frmTouchKeyboard->KeyboardText = FixedVoucherCode;
   }

   frmTouchKeyboard->MaxLength = 25;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->Caption = "Enter the Fixed Voucher Code.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  FixedVoucherCode = frmTouchKeyboard->KeyboardText;
   }

   tbtnFixedVoucherCode->Caption = "Fixed Voucher Code\r" + FixedVoucherCode;
}  */
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
//TDBTab::GetTabName(DBTransaction,Order->TabKey);
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
              if ( Item.Properties["TabKey"]==Payment.TabKey)
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
             {  SelectedItem.Properties["Color"]  ="0x00605D5C";// clRed;

             }
             else{ SelectedItem.Properties["Color"]  ="0x00BF9D00"; //clGreen;

             }
		  // int	tabKey = SelectedItem.Properties["TabKey"];
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
