// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "ManagerLoyaltyVoucher.h"
#include <math.h>
#include "enum.h"
#include "Paytype.h"
#include "Printing.h"
#include "Comms.h"
#include "GUIDiscount.h"
#include "Secure.h"
#include "DeviceRealTerminal.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "Login.h"
#include "Message.h"
#include "MMMessageBox.h"
#include "Rooms.h"
#include "SelectTable.h"
#include "DBOrder.h"
#include "DBTab.h"
#include "DBSaleTimes.h"
#include "Invoice.h"
#include "PhoenixHotelSystem.h"
#include "SelectPhoenixRoom.h"
#include "PatronCount.h"
#include "Membership.h"
#include "SmartCardAPI.h"
#include "Eftpos.h"
#include "PocketVoucher.h"
#include "ReqPrintJob.h"
#include <DateUtils.hpp>
#include "ListSecurityRefContainer.h"
#include "ManagerDiscount.h"
#include "SelectMember.h"
#include "DBContacts.h"
#include "GroupGUI.h"
#include "SelectRMSRoom.h"
#include "SCDPWDChecker.h"
#include "TaxRemoval.h"
#include "DBTables.h"
#include "VerticalSelect.h"
#include "SelectDish.h"
#include "ManagerDelayedPayment.h"
#include "DBWebUtil.h"
#include "GlobalSettings.h"
#include "ComboDiscount.h"
#include "DiscountGroup.h"
#include "CardSwipe.h"
#include "SCDPatronUtility.h"
#include "ItemSizeCategory.h"
// ---------------------------------------------------------------------------
#define NUMBER_OF_PAYMENT_TYPES_IN_VIEW 9
#define ALTCOL 0
#define PAYCOL 1

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchNumpad"
#pragma link "TouchGrid"
#pragma resource "*.dfm"
TForm * TfrmPaymentType::WinOwner = NULL;
bool PayNotCash = false;

// ---------------------------------------------------------------------------
__fastcall TfrmPaymentType::TfrmPaymentType(TComponent* Owner, TPaymentTransaction &inCurrentTransaction) : TZForm(Owner),
CurrentTransaction(inCurrentTransaction)
{
    isRefundButtonVisible = false;
    IsVoucher = true;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::CreateParams(Controls::TCreateParams &params)
{
	TForm::CreateParams(params);
	if (WinOwner)
	{
		params.WndParent = WinOwner->Handle;
		WinOwner = NULL;
	}
}
// ---------------------------------------------------------------------------
TfrmPaymentType *TfrmPaymentType::Create(TForm* Owner, TPaymentTransaction &inCurrentTransaction)
{
	WinOwner = Owner;
	return new TfrmPaymentType(Owner, inCurrentTransaction);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::FormCreate(TObject *Sender)
{
	QueryPatronCount = false;

}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::FormShow(TObject *Sender)
{
	FormResize(this);
	btnCancel->SetFocus();

	for (int i = 0; i < CurrentTransaction.PaymentsCount(); i++)
	{
		TPayment *Payment = CurrentTransaction.PaymentGet(i);
		Payment->SetAdjustment(0);
	}

	tbCredit->Visible = (CurrentTransaction.SalesType == eCash) || (TDeviceRealTerminal::Instance().BasePMS->Enabled &&
                        TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney && !CurrentTransaction.WasSavedSales);
    if(TGlobalSettings::Instance().PMSType == Mews)
       tbCredit->Visible = true;
    CalculatePatrons(CurrentTransaction);
	int TotalCount = 0;
	std::vector <TPatronType> ::iterator ptrPatronTypes = CurrentTransaction.Patrons.begin();
	for (ptrPatronTypes = CurrentTransaction.Patrons.begin(); ptrPatronTypes != CurrentTransaction.Patrons.end(); ptrPatronTypes++)
	{
		TotalCount += ptrPatronTypes->Count;
	}
	tbPatronCount->Caption = "Patron Count \r" + IntToStr(TotalCount);
	PreCreditPatronCount = CurrentTransaction.Patrons;

	tbtnDiscountSurcharge->Caption = "Discount / Surcharge";
	lbeDiscountSurcharge->Caption = "Adjustment";
	tbtnRemoveTaxes->Visible = TGlobalSettings::Instance().EnableDisplayTaxRemoval;
    if(CheckOnlinePaidOrNot())
    {
        ShowWebOrderMembersPayment();
    }
    else
    {
        ShowPaymentTotals(false);
    }

	if (QueryPatronCount)
	{
		tbPatronCountClick(Sender);
		QueryPatronCount = false;
	}

    CheckDiscountPointsPaytype = false;

    PrintCancelEvent = 0;
    //MM-5999
    TGlobalSettings::Instance().RefundingItems = false;
}
// ---------------------------------------------------------------------------
TModalResult TfrmPaymentType::Execute()
{
	TModalResult ModalResult;
    IsMemberSelected = true;
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardInserted.RegisterForEvent(OnSmartCardInserted);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardRemoved.RegisterForEvent(OnSmartCardRemoved);

	if (Recovering)
	{
		if (CurrentTransaction.Money.PaymentDue == 0)
		{
			ModalResult = mrOk;
		}
		else
		{
			Reset();
			ShowPaymentTotals();
			ModalResult = ShowModal();
		}
	}
	else
	{
		Reset();
        if(CheckOnlinePaidOrNot())
        {
            ShowWebOrderMembersPayment();
        }
        else
        {
            ShowPaymentTotals(false);
        }
		//ShowPaymentTotals(false);
        AnsiString tabName = "";
        if(TGlobalSettings::Instance().IsDrinkCommandEnabled && (CurrentTransaction.Orders->Count >0))
        {
           tabName = ((TItemComplete*)CurrentTransaction.Orders->Items[0])->TabName;
        }
		if(CurrentTransaction.TypeOfSale == ComplimentarySale || CurrentTransaction.TypeOfSale == NonChargableSale )
		{
			TPayment *Payment = CurrentTransaction.PaymentGet(0);
			BtnPayment(Payment);
			ModalResult = mrOk;
		}
        else if(tabName == "DC_ITEMS_ZED")
        {
			TPayment *Payment = CurrentTransaction.PaymentGet(2);
			BtnPayment(Payment);
			ModalResult = mrOk;
        }
		else
		{
           if(CurrentTransaction.IsQuickPayTransaction)
           {
                TPayment *Payment = CurrentTransaction.PaymentFind(CurrentTransaction.QuickPaymentName);
                BtnPayment(Payment);
                if(IsMemberSelected) // check member is not selected
                {
                   ModalResult = mrOk;
                }
                else
                {
                   ModalResult = mrCancel;
                }
           }
           else
             ModalResult = ShowModal();
		}
	}

	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardInserted.DeregisterForEvent(OnSmartCardInserted);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardRemoved.DeregisterForEvent(OnSmartCardRemoved);

	return ModalResult;
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::Reset()
{
    IsWrkPayAmountChanged = false;
    Recovering = false;
    int i = 0;
    CurrentTransaction.PaymentsSort();
    TPayment* refundPayment = CurrentTransaction.GetRefundPointsPayment();
    tgPayments->RowCount = 0; // Clears all the Latching.
    tgPayments->ColCount = 2;
    tgPayments->RowCount = refundPayment ? CurrentTransaction.VisiablePaymentsCount() - 1 : CurrentTransaction.VisiablePaymentsCount();
    int ButtonPos = 0;

    for (int i = 0; i < CurrentTransaction.PaymentsCount(); i++)
    {
        TPayment *Payment = CurrentTransaction.PaymentGet(i);
        bool IsSusidizedPaymentType=TDBTab::GetTabDiscountExists(CurrentTransaction.DBTransaction ,Payment->TabKey);

        // input RGB values
        double R, G, B;
        R = GetRValue(Payment->Colour);
        G = GetGValue(Payment->Colour);
        B = GetBValue(Payment->Colour);

        double L = (0.2126 * R) + (0.7152 * G) + (0.0722 * B);
        if(i > 0 && (CheckOnlinePaidOrNot()))
        {
           tgPayments->RowCount = 1;
           return;
        }
        //MM-2850 Hooking the loop since we need to show refund item in alternate column....
        if (Payment->Visible && !Payment->RefundPoints)
        {
            tgPayments->Buttons[ButtonPos][PAYCOL]->Color = Payment->Colour;

            if (L > 128)
            {
               tgPayments->Buttons[ButtonPos][PAYCOL]->FontColor = clBlack;
            }
            else
            {
               tgPayments->Buttons[ButtonPos][PAYCOL]->FontColor = clWhite;
            }
            if(IsSusidizedPaymentType && Payment->TabKey>  0 && frmSelectDish->IsSubSidizeProcessed)
               tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled=false;

            tgPayments->Buttons[ButtonPos][PAYCOL]->LatchedColor = clMaroon;
            tgPayments->Buttons[ButtonPos][PAYCOL]->LatchedFontColor = clWhite;
            tgPayments->Buttons[ButtonPos][PAYCOL]->Caption = Payment->Name;
            tgPayments->Buttons[ButtonPos][PAYCOL]->Tag = i;

            if(Payment->Name == CurrentTransaction.Membership.Member.Name + "'s Points")
            {
                tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled =  (TGlobalSettings::Instance().LoyaltyMateEnabled && !TGlobalSettings::Instance().IsPOSOffline)
                || !TGlobalSettings::Instance().LoyaltyMateEnabled ;
                tgPayments->Buttons[ButtonPos][PAYCOL]->Visible = TGlobalSettings::Instance().AllowPointPaymentByValue;
            }
            if(TGlobalSettings::Instance().MembershipType == MembershipTypeExternal && (Payment->Name == "PtsBal" || Payment->Name == "Dining"))
            {
                tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled =  (TGlobalSettings::Instance().LoyaltyMateEnabled && !TGlobalSettings::Instance().IsPOSOffline)
                || !TGlobalSettings::Instance().LoyaltyMateEnabled ;
                tgPayments->Buttons[ButtonPos][PAYCOL]->Visible = TGlobalSettings::Instance().AllowPointPaymentByValue;


            }

            if(Payment->Name == CurrentTransaction.Membership.Member.Name + "'s Grams")
            {
                tgPayments->Buttons[ButtonPos][ALTCOL]->Visible = false;
                tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled =  (TGlobalSettings::Instance().LoyaltyMateEnabled && !TGlobalSettings::Instance().IsPOSOffline)
                || !TGlobalSettings::Instance().LoyaltyMateEnabled ;

                ButtonPos++;
                continue;
           }

           if(Payment->GetPaymentAttribute(ePayTypeWallet))
           {
              tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled = !CurrentTransaction.CreditTransaction;
              tgPayments->Buttons[ButtonPos][ALTCOL]->Visible = false;
           }
           else if(Payment->GetPaymentAttribute(ePayTypeSmartConnectQR))
           {
              tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled = TGlobalSettings::Instance().EnableEftPosSmartConnect;//!CurrentTransaction.CreditTransaction;
              tgPayments->Buttons[ButtonPos][ALTCOL]->Visible = false;
           }
           else if(Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails) && Payment->IsLoyaltyVoucher())
             {
               tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Purchase";
               tgPayments->Buttons[ButtonPos][ALTCOL]->Visible = Payment->IsLoyaltyGiftCard();
               CopyPaymentColor(ButtonPos);
             }
           else if (Payment->GetPaymentAttribute(ePayTypeAllowCashOut) ||
                   (Payment->GetPaymentAttribute(ePayTypePoints) && TGlobalSettings::Instance().MembershipType != MembershipTypeExternal) ||
                   (Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails) && !Payment->IsLoyaltyVoucher()))
            {
                 if((Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails) || Payment->GetPaymentAttribute(ePayTypePoints)))
                 {
                        if(Payment->GetPaymentAttribute(ePayTypePoints))
                        {
                           purchaseButtonIndex = ButtonPos;
                        }
                        tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Purchase";
                        tgPayments->Buttons[ButtonPos][ALTCOL]->Visible = true;
                 }
                else if(Payment->GetPaymentAttribute(ePayTypeAllowCashOut))
                 {
                    tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Cash Out";
                 }
                CopyPaymentColor(ButtonPos);
                tgPayments->Buttons[ButtonPos][ALTCOL]->Visible = true;

            }
           else if(Payment->GetPaymentAttribute(ePayTypeCash))
            {
                if(refundPayment)
                {
                    tgPayments->Buttons[ButtonPos][ALTCOL]->Visible =  TGlobalSettings::Instance().AllowRefundPoints;
                    tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = refundPayment->Name;
                    CopyPaymentColor(ButtonPos);
                    tgPayments->Buttons[ButtonPos][ALTCOL]->Tag = CurrentTransaction.GetRefundPointPaymentIndex();
                    // MM-3396 : if the user does not have refund points access, disable the button
                    disableButtonIfPermissionNotAllowed(CurrentTransaction.DBTransaction,
                                                        tgPayments->Buttons[ButtonPos][ALTCOL],
                                                        TDeviceRealTerminal::Instance().User,
                                                        CheckAllowPointsRefund );
                }
                else
                {
                    tgPayments->Buttons[ButtonPos][ALTCOL]->Visible = false;
                }
            }
           else
            {
                    tgPayments->Buttons[ButtonPos][ALTCOL]->Visible = false;
            }

            if(TGlobalSettings::Instance().PointOnly)
            {
                tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = false;
                tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled = false;
            }
            else
            {
                 if (!(ButtonPos == 0 && ALTCOL == 0))
                 {
                    tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = true;
                 }
            }

            if(Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails) && Payment->IsLoyaltyVoucher())
             {
               tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = TGlobalSettings::Instance().LoyaltyMateEnabled && ! CurrentTransaction.CreditTransaction;
               tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled = TGlobalSettings::Instance().LoyaltyMateEnabled && ! CurrentTransaction.CreditTransaction;
             }

                   //MM 4775
            if(Payment->Name == CurrentTransaction.Membership.Member.Name + "'s Points")
            {
                tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled =  (TGlobalSettings::Instance().LoyaltyMateEnabled && !TGlobalSettings::Instance().IsPOSOffline)
                || !TGlobalSettings::Instance().LoyaltyMateEnabled ;
                tgPayments->Buttons[ButtonPos][PAYCOL]->Visible = TGlobalSettings::Instance().AllowPointPaymentByValue;
            }
             if(TGlobalSettings::Instance().MembershipType == MembershipTypeExternal && (Payment->Name == "PtsBal" || Payment->Name == "Dining"))
            {
                tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled =  (TGlobalSettings::Instance().LoyaltyMateEnabled && !TGlobalSettings::Instance().IsPOSOffline)
                || !TGlobalSettings::Instance().LoyaltyMateEnabled ;
                tgPayments->Buttons[ButtonPos][PAYCOL]->Visible = TGlobalSettings::Instance().AllowPointPaymentByValue;

            }
            ButtonPos++;
        }
    }

    if (CurrentTransaction.CreditTransaction)
    {
        EnableElectronicPayments();
        DisableCashOutElectronicPayments();
    }
    else
    {
        EnableElectronicPayments();
    }

    //Reset Loyaltymate Information
   CurrentTransaction.RedeemPointsInformation = new TRedeemPointsInformation;
   CurrentTransaction.RedeemWeightInformation = new TRedeemPointsInformation;
   CurrentTransaction.RedeemPocketVoucherInformation = new TRedeemPocketVoucherInformation;
   CurrentTransaction.RedeemGiftVoucherInformation = new TRedeemGiftVoucherInformation;
   CurrentTransaction.PurchasedGiftVoucherInformation = new TRedeemGiftVoucherInformation;

   //Reset redeemption information
   for (int i=0; i < CurrentTransaction.Orders->Count; i++)
    {
        TItemMinorComplete *Order = (TItemMinorComplete *) CurrentTransaction.Orders->Items[i];
        Order->IsPayByPoints = false;
        Order->AmountRedeemed = 0;
        Order->PointRedeemed = 0;
        if(TGlobalSettings::Instance().PontsSpentCountedAsRevenue)
          Order->DiscountByTypeRemove(dsMMMebersPoints);
    }
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::CopyPaymentColor(int rowIndex)
{
        tgPayments->Buttons[rowIndex][ALTCOL]->Color = tgPayments->Buttons[rowIndex][PAYCOL]->Color;
        tgPayments->Buttons[rowIndex][ALTCOL]->Tag = tgPayments->Buttons[rowIndex][PAYCOL]->Tag;
        tgPayments->Buttons[rowIndex][ALTCOL]->FontColor = tgPayments->Buttons[rowIndex][PAYCOL]->FontColor;
        tgPayments->Buttons[rowIndex][ALTCOL]->FontColor = tgPayments->Buttons[rowIndex][PAYCOL]->FontColor;
        tgPayments->Buttons[rowIndex][ALTCOL]->LatchedColor = tgPayments->Buttons[rowIndex][PAYCOL]->LatchedColor;
        tgPayments->Buttons[rowIndex][ALTCOL]->LatchedFontColor = tgPayments->Buttons[rowIndex][PAYCOL]->LatchedFontColor;
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::HideInvisibleRows()
{
    int paymentCount = isRefundButtonVisible ? (CurrentTransaction.PaymentsCount() - 1) : CurrentTransaction.PaymentsCount();

    if(tgPayments->RowCount > paymentCount)
    {
        int difference = paymentCount - tgPayments->RowCount;
        for(int i = (paymentCount - 1); i < tgPayments->RowCount; i++)
        {
            tgPayments->Buttons[i][PAYCOL]->Visible = false;
            tgPayments->Buttons[i][ALTCOL]->Visible = false;
        }
    }
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ShowPaymentTotals(bool MembersDiscount)
{
	CurrentTransaction.Money.Recalc(CurrentTransaction);

	int ButtonPos = 0;

	for (int i = 0; i < CurrentTransaction.PaymentsCount(); i++)
	{
		TPayment *Payment = CurrentTransaction.PaymentGet(i);

		if (Payment->Visible && !Payment->RefundPoints)
		{
			if (Payment->GetPay() == 0)
			{
				if (Payment->GetAdjustment() != 0 && !(Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails)) &&
                   !(Payment->GetPaymentAttribute(ePayTypePoints)))
				{
					tgPayments->Buttons[ButtonPos][PAYCOL]->Caption = Payment->Name + "\r" + CurrToStrF(Payment->GetAdjustment(), ffNumber,
					CurrencyDecimals);
				}
				else
				{
					tgPayments->Buttons[ButtonPos][PAYCOL]->Caption = Payment->Name;
				}
			}
			else
			{
				tgPayments->Buttons[ButtonPos][PAYCOL]->Caption = Payment->Name + "\r" + CurrToStrF
				(Payment->GetPay() + Payment->GetAdjustment(), ffNumber, CurrencyDecimals);
			}

            if(Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails) && Payment->IsLoyaltyGiftCard())
            {
                if (Payment->GetAdjustment() == 0)
				{
                   tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Purchase";
				}
				else
				{
				   	tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Purchase\r" + CurrToStrF(Payment->GetAdjustment(), ffNumber,
					CurrencyDecimals);
				}
            }
            else if ((Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails) && ! Payment->IsLoyaltyVoucher()) || Payment->GetPaymentAttribute(ePayTypePoints))
			{

				if (Payment->GetAdjustment() == 0)
				{
                   tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Purchase";
				}
				else
				{
				   	tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Purchase\r" + CurrToStrF(Payment->GetAdjustment(), ffNumber,
					CurrencyDecimals);
				}

			}
            else if(Payment->GetPaymentAttribute(ePayTypeCash))
            {
                TPayment* refundPayment = CurrentTransaction.GetRefundPointsPayment();
                if(refundPayment)
                {
                   if (Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails) &&  Payment->GetPaymentAttribute(ePayTypeCash) &&
                      (refundPayment->GetRefundPointsValue() == 0))
                    {

                        tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Purchase";

                    }
                    else if (refundPayment->GetRefundPointsValue() == 0)
                    {
                        tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Refund Points";
                    }
                    else
                    {
                        tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Refund Points\r" + CurrToStrF(refundPayment->GetRefundPointsValue(), ffNumber, CurrencyDecimals);
                    }

                }
            }
			else if(Payment->GetPaymentAttribute(ePayTypeAllowCashOut))
			{
				if (Payment->GetCashOutTotal() == 0)
				{
					tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Cash Out";
				}
				else
				{
					tgPayments->Buttons[ButtonPos][ALTCOL]->Caption = "Cash Out\r" + CurrToStrF(Payment->GetCashOutTotal(), ffNumber,
					CurrencyDecimals);
				}
			}
            else
            {
                tgPayments->Buttons[ButtonPos][ALTCOL]->Visible = false;
            }
            //MM 4775
            if(TGlobalSettings::Instance().PointOnly)
            {
                tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = false;
                tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled = false;
            }
            else
            {
              if(!AllowRefund)
              {
                   if(tgPayments->Buttons[ButtonPos][ALTCOL]->Caption != "Refund Points")
                   {
                        if(tgPayments->Buttons[ButtonPos][ALTCOL]->Caption.Pos("Cash Out") != 0)
                        {
                            if(EftPos->IsCashOutSupported())
                            {
                                tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = true;
                            }
                            else
                            {
                                tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = false;
                            }
                        }
                   }
                   else
                   {
                    tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = false;
                   }
              }
              else
              {
                    if(tgPayments->Buttons[ButtonPos][ALTCOL]->Caption.Pos("Cash Out") != 0)
                    {
                        if(EftPos->IsCashOutSupported())
                        {
                            tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = true;
                        }
                        else
                        {
                            tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = false;
                        }
                    }
                    else
                        tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = true;
              }
            }

            if(Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails) && Payment->IsLoyaltyVoucher())
            {
                tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = TGlobalSettings::Instance().LoyaltyMateEnabled && ! CurrentTransaction.CreditTransaction;
                tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled = TGlobalSettings::Instance().LoyaltyMateEnabled && ! CurrentTransaction.CreditTransaction;
            }


            //MM 4775
           if(Payment->Name == CurrentTransaction.Membership.Member.Name + "'s Points")
            {
                tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled =  (TGlobalSettings::Instance().LoyaltyMateEnabled && !TGlobalSettings::Instance().IsPOSOffline)
                || !TGlobalSettings::Instance().LoyaltyMateEnabled ;
                tgPayments->Buttons[ButtonPos][PAYCOL]->Visible = TGlobalSettings::Instance().AllowPointPaymentByValue;

            }
             if(TGlobalSettings::Instance().MembershipType == MembershipTypeExternal && (Payment->Name == "PtsBal" || Payment->Name == "Dining"))
            {
                tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled =  (TGlobalSettings::Instance().LoyaltyMateEnabled && !TGlobalSettings::Instance().IsPOSOffline)
                || !TGlobalSettings::Instance().LoyaltyMateEnabled ;
                tgPayments->Buttons[ButtonPos][PAYCOL]->Visible = TGlobalSettings::Instance().AllowPointPaymentByValue;

                if(Payment->GetAdjustment() == 0.00)
                {
                    if(Payment->Name == "PtsBal")
                        tgPayments->Buttons[ButtonPos][PAYCOL]->Caption = Payment->Name + "\r" + CurrentTransaction.Membership.Member.Points.getPointsBalance();
                    else
                        tgPayments->Buttons[ButtonPos][PAYCOL]->Caption = Payment->Name + "\r" + TGlobalSettings::Instance().DiningBal;
                }
            }
			ButtonPos++;
		}
	}

	stBalance->Caption = CurrToStrF(CurrentTransaction.Money.RoundedPaymentDue, ffNumber, CurrencyDecimals);
	stChange->Caption = CurrToStrF(CurrentTransaction.Money.RoundedChange, ffNumber, CurrencyDecimals);
	stDiscount->Caption = CurrToStrF(CurrentTransaction.Money.RoundedTotalAdjustment, ffNumber, CurrencyDecimals);

	if (TDeviceRealTerminal::Instance().PaymentSystem->ForceTender)
	{
		wrkPayAmount = 0; // Forces them to enter a value.
	}
	else
	{
		if (CurrentTransaction.CreditTransaction)
        {
			wrkPayAmount = -CurrentTransaction.Money.PaymentDue;
		} else
        {
		 	wrkPayAmount = CurrentTransaction.Money.PaymentDue;
		}
	}
	ndWorkingAmount->SetNumeric(RoundToNearest(wrkPayAmount, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));

    if(!TGlobalSettings::Instance().ShowScreenToSelectItemForPoint)
    {
       CurrentTransaction.ProcessPoints();
    }

	ShowReceipt();

	if (CurrentTransaction.Membership.Member.ContactKey != 0)
	{
		lbeMembership->Visible = true;
        double points = GetAvailableRedeemPoints(CurrentTransaction);
            if(CurrentTransaction.Membership.Member.Points.getCurrentPointsRefunded() != 0)
            {
                lbeMembership->Caption = CurrentTransaction.Membership.Member.Name + " "+CurrentTransaction.Membership.Member.Surname + " (" + CurrentTransaction.Membership.Member.MembershipNumber +
                ")" + " Points:" + FormatFloat("0.00", points + CurrentTransaction.Membership.Member.Points.getCurrentPointsRefunded());
            }
            else
            {
                lbeMembership->Caption = CurrentTransaction.Membership.Member.Name + " "+CurrentTransaction.Membership.Member.Surname + " (" + CurrentTransaction.Membership.Member.MembershipNumber +
                ")" + " Points:" + FormatFloat("0.00", points);
            }
         memberNumber = CurrentTransaction.Membership.Member.MembershipNumber;
	}
	else
	{
		lbeMembership->Visible = false;
		lbeMembership->Caption = "";
	}

	if (CurrentTransaction.CreditTransaction)
	{
		lbeMembership->Color = clRed;
		if (lbeMembership->Visible != true)
		{
			lbeMembership->Visible = true;
			lbeMembership->Caption = "Refund Transaction";
		}
	}
	else
	{
		lbeMembership->Color = clGreen;
	}

	if (CurrentTransaction.Money.TotalAdjustment == 0)
	{
		tbtnDiscountSurcharge->Caption = "Discount / Surcharge";
		lbeDiscountSurcharge->Caption = "Adjustment";
	}
	else if (CurrentTransaction.Money.TotalAdjustment < 0)
	{
		tbtnDiscountSurcharge->Caption = "Discount\r" + CurrToStrF(CurrentTransaction.Money.TotalAdjustment, ffNumber, CurrencyDecimals);
		lbeDiscountSurcharge->Caption = "Discount";
	}
	else
	{
		lbeDiscountSurcharge->Caption = "Surcharge";
		tbtnDiscountSurcharge->Caption = "Surcharge\r" + CurrToStrF(CurrentTransaction.Money.TotalAdjustment, ffNumber, CurrencyDecimals);
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::PopulateReceipt(TReqPrintJob *Receipt)
{
	Receipt->Transaction = &CurrentTransaction;
	Receipt->JobType = pjReceiptReceipt;
	Receipt->SenderType = devPC;
 	Receipt->Waiter = TDeviceRealTerminal::Instance().User.Name;

	if (CurrentTransaction.InvoiceKey != 0)
	{
		Receipt->AccountPayment = true;
		Receipt->AccountInvoiceNumber = Invoice->GetInvoiceNumber(CurrentTransaction.DBTransaction, CurrentTransaction.InvoiceKey);
		Receipt->Transaction->Customer = TDBContacts::GetCustomerAndRoomNumber( CurrentTransaction.DBTransaction, CurrentTransaction.InvoiceKey );
	}
    else
    {
        if(TDeviceRealTerminal::Instance().BasePMS->Enabled &&  TGlobalSettings::Instance().PMSType != SiHot &&
           CurrentTransaction.Phoenix.AccountNumber == "")
            CurrentTransaction.Customer = TCustomer( 0,0,"" );

        if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
           CurrentTransaction.Phoenix.AccountNumber == "")
            CurrentTransaction.Customer = TCustomer("",0,"");
    }

	Receipt->WaitTime = TDBSaleTimes::GetAverageWaitTimeMins(CurrentTransaction.DBTransaction);

	if (CurrentTransaction.SalesType == eRoomSale)
	{
		if (CurrentTransaction.Phoenix.AccountNumber != "" && CurrentTransaction.Phoenix.AccountNumber != 0)
		{
            if(TGlobalSettings::Instance().PMSType != SiHot)
            {
        		Receipt->ExtraInfo->Add("Room Number # " + CurrentTransaction.Phoenix.AccountNumber);
            }
            else
            {
                Receipt->ExtraInfo->Add("Room Number # " + CurrentTransaction.Phoenix.RoomNumber);
                if(CurrentTransaction.Customer.Name.Trim() == "")
                    CurrentTransaction.Customer.Name = CurrentTransaction.Phoenix.AccountName;
            }
			Receipt->ExtraInfo->Add("Guest " + CurrentTransaction.Phoenix.AccountName);
		}
		else
		{
			Receipt->ExtraInfo->Add("Room Number # " + IntToStr(CurrentTransaction.RoomNumber));
			Receipt->ExtraInfo->Add("Guest " + TDBRooms::GetPartyName(CurrentTransaction.DBTransaction, CurrentTransaction.RoomNumber));
		}
	}

	std::set <__int64> TabKeys; // Tabs from which the orders come from.
	TDBOrder::GetTabKeysFromOrders(CurrentTransaction.Orders, TabKeys);

	if (CurrentTransaction.SalesType == eCreditPurchase)
	{
		Receipt->ExtraInfo->Add("");
		Receipt->ExtraInfo->Add("Credit Purchase For Tabs... ");
		for (std::map <long, TTabCredit> ::iterator itTabCredit = CurrentTransaction.TabCredit.begin();
		itTabCredit != CurrentTransaction.TabCredit.end(); advance(itTabCredit, 1))
		{
			if (TDBTab::GetTabExists(CurrentTransaction.DBTransaction, itTabCredit->first))
			{
				Receipt->ExtraInfo->Add("Tab " + TDBTab::GetTabName(CurrentTransaction.DBTransaction, itTabCredit->first));

				Receipt->ExtraInfo->Add("Current Credit " + CurrToStrF(TDBTab::GetTabCredit(CurrentTransaction.DBTransaction,
				itTabCredit->first), ffNumber, CurrencyDecimals));
				Receipt->ExtraInfo->Add("Credit Purchased " + CurrToStrF(-itTabCredit->second.CreditRedeemed, ffNumber, CurrencyDecimals));
				Receipt->ExtraInfo->Add("New Tab Credit " + CurrToStrF((TDBTab::GetTabCredit(CurrentTransaction.DBTransaction,
				itTabCredit->first) - itTabCredit->second.CreditRedeemed), ffNumber, CurrencyDecimals));
				Receipt->ExtraInfo->Add("New Tab Balance " + CurrToStrF((TDBTab::GetTabBalance(CurrentTransaction.DBTransaction,
				itTabCredit->first) + itTabCredit->second.CreditRedeemed), ffNumber, CurrencyDecimals));
			}
		}
	}
    bool isTable = false;
    int tabKey = 0;
	std::auto_ptr <TStringList> TabHistory(new TStringList);
	for (std::set <__int64> ::iterator itTabs = TabKeys.begin(); itTabs != TabKeys.end(); advance(itTabs, 1))
	{
        tabKey = *itTabs;
        isTable = false;
        TMMTabType TabType = TDBTab::GetTabType(CurrentTransaction.DBTransaction, *itTabs);
		if (TabType == TabTableSeat)
		   {
              TTableSeat TableSeat;
			  if (TDBTables::GetTableSeat(CurrentTransaction.DBTransaction, *itTabs, &TableSeat))
              {
                 tabKey = TableSeat.TableNo;
                 isTable = true;
              }
           }
		TDBTab::GetPartialPaymentList(CurrentTransaction.DBTransaction, tabKey, TabHistory.get(),isTable);
        for(int x = 0 ; x < TabHistory->Count; x++)
        {
           if(Receipt->TabHistory->IndexOf(TabHistory->Strings[x]) == -1)
           {
             Receipt->TabHistory->Add(TabHistory->Strings[x]);
           }
        }
	}


   if(CurrentTransaction.WebOrderKey != 0)
   {
        TDBWebUtil::getWebOrderExtraData(CurrentTransaction.DBTransaction, CurrentTransaction.WebOrderKey,"DELIVERY", Receipt->DeliveryInfo);
        TDBWebUtil::getWebOrderExtraData(CurrentTransaction.DBTransaction, CurrentTransaction.WebOrderKey,"PAYMENT_INFO", Receipt->PaymentInfo);
        TDBWebUtil::getWebOrderExtraData(CurrentTransaction.DBTransaction, CurrentTransaction.WebOrderKey, "COMMENTS", Receipt->OrderComments);
        if(TGlobalSettings::Instance().CaptureCustomerName)
        {
           if(!CurrentTransaction.ChitNumber.Valid())
           {
              Receipt->DeliveryInfo->Clear();
           }
        }
        if(CurrentTransaction.ChitNumber.Valid())
        {
           if(!CurrentTransaction.ChitNumber.IsCaptureCustomerDetails)
           {
              Receipt->DeliveryInfo->Clear();
           }
        }
   }

	Receipt->MiscData = CurrentTransaction.MiscPrintData;
	Receipt->PaymentType = ptPreliminary;

}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ShowReceipt()
{
	TReqPrintJob *TempReceipt = new TReqPrintJob(&TDeviceRealTerminal::Instance());
	PopulateReceipt(TempReceipt);

	Memo1->Lines->Clear();

	TPrinterPhysical DefaultScreenPrinter;
	DefaultScreenPrinter.NormalCharPerLine = 40;
	DefaultScreenPrinter.BoldCharPerLine = 40;
	DefaultScreenPrinter.PhysicalPrinterKey = TComms::Instance().ReceiptPrinter.PhysicalPrinterKey;
	Receipt->GetPrintouts(CurrentTransaction.DBTransaction, TempReceipt, DefaultScreenPrinter, eDispBCOff);

	TempReceipt->Printouts->PrintToStrings(Memo1->Lines);

	delete TempReceipt;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::FormResize(TObject *Sender)
{
   		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
		Realign();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::btnPrelimClick(TObject *Sender)
{
    int delayedTabKey = 0;
	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		MessageBox("Please select a receipt printer from Setup first.", "Print error", MB_OK + MB_ICONERROR);
	}
	else
	{
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        if(CurrentTransaction.Orders->Count > 0)  //check current transaction has orders
        {
            TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[0];
            if(TGlobalSettings::Instance().TransferTableOnPrintPrelim && Order)
              {
                 bool Allow = false;
                 bool isTable =true;

                 if(Order->TableNo > 0 )
                  {
                     Allow = true;
                  }
                 else if(TManagerDelayedPayment::Instance().IsDelayedPayment(CurrentTransaction) &&
                         !IsAllTabItemSelected())
                 {
                    Allow = true;
                    isTable = false;
                 }

                 if(Allow)
                 {
                     DBTransaction.StartTransaction();
                     if(Order->TableNo > 0)
                        TDBTables::SetTableBillingStatus(DBTransaction,Order->TableNo,eNoneStatus);

                     bool isMixedMenuOrder = true;

                     if(TGlobalSettings::Instance().IsBillSplittedByMenuType && Order->ItemType)
                          isMixedMenuOrder = false;

                     delayedTabKey = TManagerDelayedPayment::Instance().MoveOrderToTab(CurrentTransaction,isTable, isMixedMenuOrder);
                    if(delayedTabKey != 0)
                    {
                        TDBTab::SetDelayedPatronCount(DBTransaction,delayedTabKey,CurrentTransaction.Patrons);
                    }
                     DBTransaction.Commit();
                     ShowReceipt();
                 }
              }
            if(Order && Order->TableNo > 0 && !TGlobalSettings::Instance().TransferTableOnPrintPrelim)
             {
                DBTransaction.StartTransaction();
                TDBTables::SetTableBillingStatus(DBTransaction,Order->TableNo,ePrelim);
                DBTransaction.Commit();
             }
        }
        TReqPrintJob *TempReceipt = new TReqPrintJob(&TDeviceRealTerminal::Instance());
        PopulateReceipt(TempReceipt);
        Receipt->GetPrintouts(CurrentTransaction.DBTransaction, TempReceipt, TComms::Instance().ReceiptPrinter);
        TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
        //if(TGlobalSettings::Instance().UseItalyFiscalPrinter)
        delete TempReceipt;
	}
    PrintCancelEvent++;
}
// ---------------------------------------------------------------------------
bool TfrmPaymentType::IsAllTabItemSelected()
{
    bool AllSelected = true;
    TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[0];
    int tabKey = Order->TabKey;
    std::set <__int64> TabItems;
    std::set <__int64> BilledItems;
    TDBTab::GetOrderKeys(CurrentTransaction.DBTransaction, tabKey, TabItems);

    for(int i=0;i< CurrentTransaction.Orders->Count; i++)
    {
       TItemComplete *Item = (TItemComplete*)CurrentTransaction.Orders->Items[i];
       BilledItems.insert(Item->OrderKey);
    }

    for (std::set <__int64> ::iterator itItem = TabItems.begin(); itItem != TabItems.end(); advance(itItem, 1))
    {
        if (BilledItems.find(*itItem) == BilledItems.end())
        {
            AllSelected = false;
            break;
        }
    }
    return AllSelected;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::pnlCancelClick(TObject *Sender)
{
    TGlobalSettings::Instance().IsThorlinkTender = true;
	ModalResult = mrCancel;
	// Undo Any Credit transactions.
	if (CurrentTransaction.CreditTransaction)
	{
		tbCreditClick(Sender);
	}
    PrintCancelEvent++;

   //Reset redeemption information
   for (int i=0; i < CurrentTransaction.Orders->Count; i++)
    {
        TItemMinorComplete *Order = (TItemMinorComplete *) CurrentTransaction.Orders->Items[i];
        Order->IsPayByPoints = false;
        Order->AmountRedeemed = 0;
        Order->PointRedeemed = 0;
        if(TGlobalSettings::Instance().PontsSpentCountedAsRevenue)
          Order->DiscountByTypeRemove(dsMMMebersPoints);
    }
    CurrentTransaction.IgnoreLoyaltyKey = false;
    CurrentTransaction.Recalc();
    if(TGlobalSettings::Instance().EnableCancelCheckRemoval)
    {
        if(PrintCancelEvent > 0)
        {
            for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
            {
                 TItemComplete *PrelimOrder = (TItemComplete*)CurrentTransaction.Orders->Items[i];
                 PrelimOrder->PrintCancel = true;
                 for( int j = 0; j < PrelimOrder->SubOrders->Count; j++ )
                 {
                    TItemComplete *SideItem = (TItemComplete*)PrelimOrder->SubOrders->SubOrderGet(j);
                    SideItem->PrintCancel = true;
                 }
           }
        }
    }
    else
    {
        if(PrintCancelEvent > 1)
        {

            if(CurrentTransaction.Orders->Count != 0 )  //check current transaction has orders
            {
                for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
                {
                     TItemComplete *PrelimOrder = (TItemComplete*)CurrentTransaction.Orders->Items[i];
                     PrelimOrder->PrintCancel = true;

                     for( int j = 0; j < PrelimOrder->SubOrders->Count; j++ )
                     {
                        TItemComplete *SideItem = (TItemComplete*)PrelimOrder->SubOrders->SubOrderGet(j);
                        SideItem->PrintCancel = true;
                     }
               }
           }
        }
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::stDoneClick(TObject *Sender)
{
	ModalResult = mrOk;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::btnReceiptUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	ScrollUp = true;
	ScrollingControl = Memo1;
	SendMessage(ScrollingControl->Handle, WM_VSCROLL, SB_LINEUP, 0L);
	ButtonClicked = btnReceiptUp;
	Timer1->Enabled = true;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::btnReceiptUpMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Timer1->Enabled = false;
	ScrollingControl = NULL;
	ButtonClicked = NULL;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::btnReceiptDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	ScrollUp = false;
	ScrollingControl = Memo1;
	SendMessage(ScrollingControl->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
	ButtonClicked = btnReceiptDown;
	Timer1->Enabled = true;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::btnReceiptDownMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Timer1->Enabled = false;
	ScrollingControl = NULL;
	ButtonClicked = NULL;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::Timer1Timer(TObject *Sender)
{
	int X = Mouse->CursorPos.x;
	int Y = Mouse->CursorPos.y;

	if (ButtonClicked)
	{
		if (X < Panel8->Left + ButtonClicked->Left || X > Panel8->Left + ButtonClicked->Left + ButtonClicked->Width || Y < Panel8->Top +
				ButtonClicked->Top || Y > Panel8->Top + ButtonClicked->Top + ButtonClicked->Height)
		{
			Timer1->Enabled = false;
			ScrollingControl = NULL;
			ButtonClicked = NULL;
			return;
		}
	}

	if (ScrollingControl)
	{
		if (ScrollUp)
		{
			SendMessage(ScrollingControl->Handle, WM_VSCROLL, SB_LINEUP, 0L);
		}
		else
		{
			SendMessage(ScrollingControl->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
		}
	}
	Timer1->Interval = 20;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::lbeCLRMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	wrkPayAmount = 0;
	ndWorkingAmount->SetNumeric(RoundToNearest(wrkPayAmount, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown));
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::lbeRemoveTaxClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	TempUserInfo = TDeviceRealTerminal::Instance().User;
	TempUserInfo.Clear();
	std::auto_ptr <TContactStaff> Staff(new TContactStaff(CurrentTransaction.DBTransaction));
	TLoginSuccess Result = Staff->Login(this, CurrentTransaction.DBTransaction, TempUserInfo, CheckTaxRemoval);
	if (Result == lsAccepted)
	{
		TStringList* removedTaxes = new TStringList;
		TStringList* taxList = new TStringList;

		GetOrderTaxes(taxList);
		GetRemovedTaxes(removedTaxes);
		std::auto_ptr<TfrmTaxRemoval> taxRemoval(TfrmTaxRemoval::Create<TfrmTaxRemoval>(this,TDeviceRealTerminal::Instance().DBControl));
		taxRemoval->RemovedTaxes = removedTaxes;
		taxRemoval->TaxList = taxList;
		//Tax Removal form will assign removedTaxes list with tax that was unchecked in the form
		if( taxRemoval->ShowModal() == mrOk)
		{
			RemoveTaxes(removedTaxes);
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access rights to Remove Taxes.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}


}
// ---------------------------------------------------------------------------
void TfrmPaymentType::GetOrderTaxes(TStringList* taxList)
{
	for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
	{
		TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[i];
		for( int j = 0; j < Order->TaxProfiles.size(); j++)
		{
			if(!(taxList->IndexOf(Order->TaxProfiles[j].taxProfileName) >=0) )
			{
				taxList->Add(Order->TaxProfiles[j].taxProfileName);
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::GetRemovedTaxes(TStringList* removedTaxes)
{
	if(CurrentTransaction.Orders->Count>0)
	{
		TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[0] ;
		for(int i=0; i< Order->RemovedTaxes->Count;i++)
		{
			removedTaxes->Add(Order->RemovedTaxes->Strings[i]);
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::RemoveTaxes(TStringList* removedTaxes)
{
	//Assign removedtax list to each item
	//Bill calculator will check that if tax is present in removedtax list
	// If tax is present then make tax percentage to zero
	for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
	{
		TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[i];
		Order->RemovedTaxes =removedTaxes;
		for( int j = 0; j < Order->SubOrders->Count; j++ )
		{
			TItemCompleteSub *SideItem = Order->SubOrders->SubOrderGet(j);
			SideItem->RemovedTaxes =removedTaxes;
		}
	}
    CurrentTransaction.IgnoreLoyaltyKey = false;
	CurrentTransaction.Recalc();
	ShowPaymentTotals();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::FormClose(TObject *Sender, TCloseAction &Action)
{
	SplitPayment = false;
	Recovering = false;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::BtnPayment(TPayment *Payment)
{
    bool canProceed = true;
    if(TGlobalSettings::Instance().IsFiscalPostingDisable && (TGlobalSettings::Instance().UseItalyFiscalPrinter ||
    TGlobalSettings::Instance().IsFiscalStorageEnabled)|| TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled)
    {
        for(int i= 0; i<CurrentTransaction.PaymentsCount(); i++)
        {
            TPayment *paymentLocal = CurrentTransaction.PaymentGet(i);
            if(Payment->GetPaymentAttribute(ePayTypeRoomInterface))
            {

                if(!paymentLocal->GetPaymentAttribute(ePayTypeRoomInterface) && paymentLocal->GetPayTendered() != 0)
                {
                    MessageBox("You can not select this payment type.\rPlease pay completely using non Room Charge.","Error",MB_OK+MB_ICONERROR);
                    canProceed = false;
                    break;
                }
            }
            else
            {
                    if(paymentLocal->GetPaymentAttribute(ePayTypeRoomInterface) && paymentLocal->GetPayTendered() != 0)
                    {
                        MessageBox("You can not select this payment type.\rPlease pay completely using Room Charge.","Error",MB_OK+MB_ICONERROR);
                        canProceed = false;
                        break;
                    }
                }
            }
        }
    //}
    if(!canProceed)
        return;
	if (SecurePaymentAccess(Payment))
	{
		bool proceed = true;
		if (Payment->GetPaymentAttribute(ePayTypeChargeToAccount))
		{
			TDBContacts DBContacts;
			std::auto_ptr <TfrmSelectMember> (frmSelectMember)(TfrmSelectMember::Create <TfrmSelectMember> (this));
			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			DBTransaction.StartTransaction();

			bool SelectMember = true;
			if (Payment->GetPaymentAttribute(ePayTypeChargeToAccount) && CurrentTransaction.Membership.Member.Charges)
			{
				SelectMember = false;
			}

			if(SelectMember)
			{
				if (Payment->GetPaymentAttribute(ePayTypeChargeToAccount))
				{
					frmSelectMember->SetFilterlist(true);
				}

				if (frmSelectMember->ShowModal() == mrOk)
				{
					Payment->InvoiceContact = frmSelectMember->getMemberInfo();
					ApplyAccount(Payment->InvoiceContact);

				}
				else
				{
					proceed = false;
					ClearAccount();
                    IsMemberSelected = false;
				}
			}
			else if (CurrentTransaction.Membership.Member.Charges)
			{
				Payment->InvoiceContact = CurrentTransaction.Membership.Member;
				ApplyAccount(Payment->InvoiceContact);
			}
		}

		if (proceed)
		{
			if (CurrentTransaction.CreditTransaction)
			{
                  ProcessCreditPayment(Payment);
			}
			else
			{
               ProcessNormalPayment(Payment);
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ProcessCreditPayment(TPayment *Payment)
{
    Payment->CreditTransaction = true;
    if (Payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
    {
        Payment->AdjustmentReason = Payment->Name;
        Payment->SetAdjustment(-wrkPayAmount);
        ShowPaymentTotals();
    }
    else
    {
        GetPaymentNote(Payment);
        Payment->Result = eProcessing;
        if (TDeviceRealTerminal::Instance().PaymentSystem->ForceTender && TDeviceRealTerminal::Instance().PaymentSystem->FTBypassElecTranTyp)
        {
            if (Payment->GetPaymentAttribute(ePayTypeElectronicTransaction) && wrkPayAmount == 0.0)
            {
               wrkPayAmount = CurrentTransaction.Money.PaymentDue;
            }
        }
        // Warn user about Payment Surcharge.
        Currency Surcharge = 0;
        if ((Payment->GetPaymentAttribute(ePayTypeSurcharge)) && wrkPayAmount > 0)
        {
            bool IsPercent = false;
            if (Payment->PercentAdjust != 0)
            {
                IsPercent = true;
                Surcharge = wrkPayAmount * (Payment->PercentAdjust / 100);
            }
            else
            {
                Surcharge = Payment->AmountAdjust;
            }

            AnsiString SurchargeMsg = "A Surcharge of " + CurrToStrF(Surcharge, ffNumber, CurrencyDecimals)
            + " will be incurred by using this method of payment";
            MessageBox(SurchargeMsg.c_str(), "Warning", MB_OK + MB_ICONINFORMATION);

            Payment->SetAdjustment(Surcharge);
            wrkPayAmount -= Surcharge;
        }
        else
        {
            Payment->SetAdjustment(0);
        }

        if (Payment->GetPaymentAttribute(ePayTypeCredit))
        {
            TTabCredit Credit = CurrentTransaction.TabCredit[Payment->TabCreditKey];
            if (wrkPayAmount > Credit.CurrentCredit)
            {
                MessageBox(AnsiString("Only " + CurrToStrF(Credit.CurrentCredit, ffNumber,
                CurrencyDecimals) + " of credit can be applied to the purchases.").c_str(), "Warning",
                MB_OK + MB_ICONINFORMATION);
                Payment->SetPay(Credit.CurrentCredit);
                Credit.CreditRedeemed = Credit.CurrentCredit;
            }
            else
            {
                Payment->SetPay(wrkPayAmount);
                Credit.CreditRedeemed = wrkPayAmount;
            }
            CurrentTransaction.TabCredit[Payment->TabCreditKey] = Credit;
        }
        else if (fabs(wrkPayAmount) > fabs(CurrentTransaction.Money.PaymentDue + Payment->GetAdjustment()))
        {
            if (!Payment->GetPaymentAttribute(ePayTypeCash)) // Change not allowed
            {
                if (MessageBox(AnsiString("Only " + CurrToStrF(CurrentTransaction.Money.PaymentDue + Payment->GetAdjustment(),
                                    ffNumber, CurrencyDecimals) + " can be applied to the purchases. Continue?").c_str(), "Warning",
                            MB_OKCANCEL + MB_ICONINFORMATION) == ID_OK)
                {
                    Payment->SetPay(-CurrentTransaction.Money.PaymentDue);
                    wrkPayAmount = wrkPayAmount - (CurrentTransaction.Money.PaymentDue + Payment->GetAdjustment());
                }
            }
            else if (wrkPayAmount == 0)
            {
                Payment->SetPay(0);
            }
            else
            {
                MessageBox(AnsiString("Only " + CurrToStrF(CurrentTransaction.Money.PaymentDue + Payment->GetAdjustment(), ffNumber,
                CurrencyDecimals) + " of refunds can be applied.").c_str(), "Warning", MB_OK + MB_ICONINFORMATION);
                Payment->SetPay(CurrentTransaction.Money.PaymentDue + Payment->GetAdjustment());
            }
        }
        else if ((Payment->GetPaymentAttribute(ePayTypeSecondaryPMSExport)) && fabs(wrkPayAmount) != fabs
                (CurrentTransaction.Money.GrandTotal))
        {
            MessageBox(AnsiString("The total of the bill must be charged to the Room").c_str(), "Warning",
            MB_OK + MB_ICONINFORMATION);
        }
        else
        {
            Payment->SetPay(-wrkPayAmount);
            if(Payment->Name == CurrentTransaction.Membership.Member.Name + "'s Points")
            {
                TGlobalSettings::Instance().RefundingItems = true;
            }
             if(TGlobalSettings::Instance().MembershipType == MembershipTypeExternal && (Payment->Name == "PtsBal" || Payment->Name == "Dining"))
            {
                TGlobalSettings::Instance().RefundingItems = true;
            }
        }
        //Voucher
        if (wrkPayAmount != 0 && (Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails)))
        {
            if(Payment->IsLoyaltyVoucher() && TGlobalSettings::Instance().LoyaltyMateEnabled )
            {
               ProcessLoyaltyVoucher(Payment);
            }
            else
            {
                std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
                frmTouchKeyboard->MaxLength = 50;
                frmTouchKeyboard->AllowCarriageReturn = false;
                frmTouchKeyboard->StartWithShiftDown = false;
                frmTouchKeyboard->MustHaveValue = true;
                frmTouchKeyboard->KeyboardText = Payment->ReferenceNumber;
                frmTouchKeyboard->Caption = "Enter Voucher Number";
                if (frmTouchKeyboard->ShowModal() == mrOk)
                {
                    Payment->ReferenceNumber = frmTouchKeyboard->KeyboardText;
                }
                Payment->SysNameOveride = Payment->Name;
            }
        }
        //Pocket Voucher
        if (wrkPayAmount != 0 && (Payment->GetPaymentAttribute(ePayTypePocketVoucher)))
        {
            std::auto_ptr <TfrmPocketVoucher> frmPocketVoucher(TfrmPocketVoucher::Create <TfrmPocketVoucher> (this));
            if (frmPocketVoucher->ShowModal() == mrOk)
            {
                Payment->VoucherCode = frmPocketVoucher->VoucherCode();
            }
            else
            {
                Payment->SetPay(0);
                Payment->VoucherCode = "";
            }
            // Set the System name in case it has been set by a Voucher Purchase attempt.
            Payment->SysNameOveride = Payment->Name;
        }
        if (Payment->GetPaymentAttribute(ePayTypeCSV))
        {
            if(Payment->GetPaymentAttribute(ePayTypeReservationMasterPay))
            {
                do
                {
                    std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
                    frmTouchKeyboard->MaxLength = 30;
                    frmTouchKeyboard->AllowCarriageReturn = false;
                    frmTouchKeyboard->StartWithShiftDown = false;
                    frmTouchKeyboard->MustHaveValue = true;
                    frmTouchKeyboard->KeyboardText = "";
                    frmTouchKeyboard->Caption = "Enter the " + Payment->Name + " number";
                    if(frmTouchKeyboard->ShowModal() == mrOk)
                    {
                        Payment->CSVString = frmTouchKeyboard->KeyboardText ;
                    }
                }
                while (Payment->CSVString == "");
            }

            else
            {
                Payment->CSVNumber = 0;
                do
                {
                    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
                    frmTouchNumpad->Caption = "Enter the " + Payment->Name + " number";
                    frmTouchNumpad->btnSurcharge->Caption = "Ok";
                    frmTouchNumpad->btnSurcharge->Visible = true;
                    frmTouchNumpad->btnDiscount->Visible = false;
                    frmTouchNumpad->Mode = pmNumber;
                    frmTouchNumpad->INTInitial = 0;
                    if (frmTouchNumpad->ShowModal() == mrOk)
                    {
                        Payment->CSVNumber = frmTouchNumpad->INTResult;
                    }
                }
                while (Payment->CSVNumber == 0);
            }
        }

        if (Payment->GetPaymentAttribute(ePayTypeRoomInterface))
        {
          

            bool GuestMasterOk = true;
            AnsiString TabName = "";
            int RoomNumber = 0;
            UnicodeString RoomNumberStr = "";
            if (CurrentTransaction.RoomNumber == 0)
            {
                if (TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled)
                {
                    if (TRooms::Instance().SelectRoom(CurrentTransaction.DBTransaction) == mrOk)
                    {
                        CurrentTransaction.RoomNumber = TRooms::Instance().SelectedRoom->RoomNo;
                        CurrentTransaction.BookingID = TRooms::Instance().SelectedRoom->BookingID;
                        CurrentTransaction.SalesType = eRoomSale;
                        TabName = TRooms::Instance().SelectedRoom->Name;
                        RoomNumber = TRooms::Instance().SelectedRoom->RoomNo;
                    }
                    else
                    {
                        GuestMasterOk = false;
                    }
                }
                else if (!TRooms::Instance().Enabled && TDeviceRealTerminal::Instance().BasePMS->Enabled)
                {
                    // Override the ipaddress and port.
                    AnsiString PMSIPAddress;
                    int PMSPort;
                    if (Payment->SecondaryPMSIPAddress != "")
                    {
                        PMSIPAddress = Payment->SecondaryPMSIPAddress;
                        PMSPort = Payment->SecondaryPMSPortNumber;
                    }
                    else
                    {
                        PMSIPAddress = TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
                        PMSPort = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
                    }
                    // check if guest details already present
                    if(!IsSavedSavesWithSiHot(CurrentTransaction))
                    {
                        std::auto_ptr <TfrmPhoenixRoom> frmPhoenixRoom(TfrmPhoenixRoom::Create <TfrmPhoenixRoom> (this));
                        if (frmPhoenixRoom->SelectRoom(PMSIPAddress, PMSPort) == mrOk)
                        {
                            CurrentTransaction.Phoenix.AccountNumber = frmPhoenixRoom->SelectedRoom.AccountNumber;
                            CurrentTransaction.Phoenix.AccountName = frmPhoenixRoom->SelectedRoom.Folders->Strings
                            [frmPhoenixRoom->SelectedRoom.FolderNumber - 1];
                            CurrentTransaction.Phoenix.FolderNumber = frmPhoenixRoom->SelectedRoom.FolderNumber;
                            CurrentTransaction.SalesType = eRoomSale;
                            RoomNumber = StrToIntDef(frmPhoenixRoom->SelectedRoom.AccountNumber, frmPhoenixRoom->SelectedRoom.FolderNumber);
                            CurrentTransaction.Phoenix.RoomNumber = frmPhoenixRoom->SelectedRoom.SiHotRoom;
							if(TGlobalSettings::Instance().PMSType == Oracle)
							{
								CurrentTransaction.Customer.RoomNumber = atoi(frmPhoenixRoom->SelectedRoom.AccountNumber.c_str());
								TabName = frmPhoenixRoom->SelectedRoom.AccountNumber;
								CurrentTransaction.Customer.RoomNumber =
												atoi(frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].RoomNumber.c_str());
								TabName =
												atoi(frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].RoomNumber.c_str());
								CurrentTransaction.PMSClientDetails.ReservationID =
												frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].ReservationId;
								CurrentTransaction.PMSClientDetails.SequenceNumber =
												frmPhoenixRoom->roomResult.SequenceNumber;
								CurrentTransaction.PMSClientDetails.ProfileID =
												frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].ProfileId;
								CurrentTransaction.PMSClientDetails.CreditLimit =
												frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].CreditLimit;
								CurrentTransaction.PMSClientDetails.HotelID =
												frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].HotelId;
								CurrentTransaction.PMSClientDetails.FirstName =
												frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].FirstName;
								CurrentTransaction.PMSClientDetails.LastName =
												frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].LastName;
								CurrentTransaction.PMSClientDetails.MatchIdentifier =frmPhoenixRoom->SelectedRoom.FolderNumber-1;
								CurrentTransaction.PMSClientDetails.RoomNumber =
												frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].RoomNumber;
								CurrentTransaction.PMSClientDetails.Date =
												frmPhoenixRoom->roomResult.Date;
								CurrentTransaction.PMSClientDetails.Time =
												frmPhoenixRoom->roomResult.Time;
								CurrentTransaction.Phoenix.AccountNumber = CurrentTransaction.Customer.RoomNumber;
								CurrentTransaction.Phoenix.AccountName = frmPhoenixRoom->SelectedRoom.Folders->Strings
								[frmPhoenixRoom->SelectedRoom.FolderNumber - 1];
								CurrentTransaction.Phoenix.FolderNumber = frmPhoenixRoom->SelectedRoom.FolderNumber;
								CurrentTransaction.SalesType = eRoomSale;
								RoomNumber = atoi(frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].RoomNumber.c_str());
								CurrentTransaction.Phoenix.RoomNumber =
									atoi(frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].RoomNumber.c_str());
							}
                            else if(TGlobalSettings::Instance().PMSType == Mews)
                            {
                                    CurrentTransaction.Customer.RoomNumberStr = frmPhoenixRoom->SelectedRoom.SiHotRoom;
                                    CurrentTransaction.Phoenix.FirstName =  frmPhoenixRoom->CustomersMews[frmPhoenixRoom->SelectedRoom.FolderNumber-1].FirstName;
                                    CurrentTransaction.Phoenix.LastName =  frmPhoenixRoom->CustomersMews[frmPhoenixRoom->SelectedRoom.FolderNumber-1].LastName;
                                    CurrentTransaction.Phoenix.AccountNumber = frmPhoenixRoom->CustomersMews[frmPhoenixRoom->SelectedRoom.FolderNumber-1].Id;
                                    TabName = frmPhoenixRoom->SelectedRoom.SiHotRoom;
                            }
                            else if(TGlobalSettings::Instance().PMSType != SiHot)
                            {
                                CurrentTransaction.Customer.RoomNumber = atoi(frmPhoenixRoom->SelectedRoom.AccountNumber.c_str());
                                TabName = frmPhoenixRoom->SelectedRoom.AccountNumber;
                            }
                            else
                            {
//                                CurrentTransaction.Customer.RoomNumber = atoi(frmPhoenixRoom->SelectedRoom.SiHotRoom.c_str());
                                CurrentTransaction.Customer.RoomNumberStr = frmPhoenixRoom->SelectedRoom.SiHotRoom;
                                CurrentTransaction.Phoenix.FirstName =  frmPhoenixRoom->SiHotAccounts[frmPhoenixRoom->SelectedRoom.FolderNumber-1].AccountDetails[0].FirstName;
                                CurrentTransaction.Phoenix.LastName =  frmPhoenixRoom->SiHotAccounts[frmPhoenixRoom->SelectedRoom.FolderNumber-1].AccountDetails[0].LastName;
                                TabName = frmPhoenixRoom->SelectedRoom.SiHotRoom;
                            }
//                            CurrentTransaction.WasSavedSales = false;
                        }
                        else
                        {
                            GuestMasterOk = false;
                        }
                    }
                    else
                    {
                        CurrentTransaction.SalesType = eRoomSale;
                        for(int orderIndex = 0; orderIndex <  CurrentTransaction.Orders->Count; orderIndex++)
                        {
                            if(((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNo != 0)
                            {
//                                CurrentTransaction.Customer.RoomNumber = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNo;
                                CurrentTransaction.Customer.RoomNumberStr = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNoStr;
                                CurrentTransaction.Phoenix.FirstName =  ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->FirstName;
                                CurrentTransaction.Phoenix.LastName =  ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->LastName;
                                TabName = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNo;
//                                RoomNumber = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNo;
                                RoomNumberStr = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNoStr;
                                CurrentTransaction.Phoenix.AccountNumber = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->AccNo;
                                CurrentTransaction.Phoenix.AccountName = CurrentTransaction.Phoenix.FirstName + " " +
                                                                         CurrentTransaction.Phoenix.LastName;
                                CurrentTransaction.Phoenix.RoomNumber = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNo;

//                                CurrentTransaction.WasSavedSales = true;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    MessageBox("Neither Rooms nor Total Hospitality System interfaces are enabled.", "Error",
                    MB_OK + MB_ICONINFORMATION);
                    GuestMasterOk = false;
                }
            }

            if (!GuestMasterOk)
            {
                Payment->SetPay(0);
                Payment->SetAdjustment(0);
                Payment->SetCashOut(0);
                Payment->Result = eFailed;
                CurrentTransaction.BookingID = 0;
                CurrentTransaction.RoomNumber = 0;
            }
            else
            {
                if (CurrentTransaction.Orders != NULL)
                {
                    for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
                    {
                        TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[i];
                        Order->TabContainerName = TabName;
                        Order->TabName = TabName;
                        Order->TabType = TabRoom;
                        if(TGlobalSettings::Instance().PMSType != SiHot )
                            Order->RoomNo = RoomNumber;
                        else
                            Order->RoomNoStr = RoomNumberStr;
                    }
                }
            }
        }

        if (Payment->GetPaymentAttribute(ePayTypeRMSInterface))
        {
            std::auto_ptr <TfrmRMSRoom> frmRoom(TfrmRMSRoom::Create <TfrmRMSRoom> (this));
            if (frmRoom->SelectRoom(Payment->CVSReadLocation) == mrOk)
            {
                Payment->RMSRoom = frmRoom->SelectedRoom;
                CurrentTransaction.SalesType = eRoomSale;
            }
            else
            {
                Payment->RMSRoom.Clear();
                Payment->SetPay(0);
                Payment->SetAdjustment(0);
                Payment->SetCashOut(0);
                Payment->Result = eFailed;
            }
        }

        if ((Payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS)) && EftPos->CheckOverLimitExceeded
                (Payment->GetPayTendered() + Payment->GetCashOutTotal()))
        {
            MessageBox("Eftpos over limit exceeded.", "Eftpos Error.", MB_OK + MB_ICONERROR);
            Payment->SetPay(0);
        }

        ShowPaymentTotals();
        if (NoElectronicPayments())
        {
            EnableElectronicPayments();
        }
        else
        {
            DisableOtherElectronicPayments(Payment);
        }
    }

    if (CurrentTransaction.Money.PaymentDue == 0)
    {
        if (CurrentTransaction.Money.Change != 0 || // Cash out.
                isRefundInProgress ||
                CurrentTransaction.Money.GrandTotal != 0 || CurrentTransaction.Orders->Count > 0)
        {
            ModalResult = mrOk;
        }
        else
        {
            MessageBox("Nothing to Bill.", "Info", MB_OK + MB_ICONINFORMATION);
        }
    }
    else
    {
        btnCancel->SetFocus();
    }
}
//----------------------------------------------------------------------------
bool TfrmPaymentType::IsSavedSavesWithSiHot(TPaymentTransaction &CurrentTransaction)
{
    bool retValue = false;
    if((CurrentTransaction.WasSavedSales == false && CurrentTransaction.SalesType == eRoomSale)
       && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney)
    {
        if(CurrentTransaction.Orders->Count > 0)
        {
            retValue = ((TItemComplete*)CurrentTransaction.Orders->Items[0])->RoomNoStr != "" &&
                       ((TItemComplete*)CurrentTransaction.Orders->Items[0])->RoomNoStr != TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        }
    }
    return retValue;
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ProcessNormalPayment(TPayment *Payment)
{
    Payment->CreditTransaction = false;
    if (Payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
    {
        Payment->AdjustmentReason = Payment->Name;
        Payment->SetAdjustment(wrkPayAmount);
        ShowPaymentTotals();

    }
    else
    {
      if(Payment->Name == "GC" || Payment->Name == "Check")
      {
	    std::auto_ptr<TfrmTouchNumpad>frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
        if(Payment->Name == "GC" )
            frmTouchNumpad->Caption = "Enter Total Amount For GiftCard";

        else if(Payment->Name == "Check")
            frmTouchNumpad->Caption = "Enter Total Amount For Cheque";

        frmTouchNumpad->btnSurcharge->Caption = "Ok";
        frmTouchNumpad->btnDiscount->Visible = false;
        frmTouchNumpad->btnSurcharge->Visible = true;
        frmTouchNumpad->Mode = pmCurrency;
        frmTouchNumpad->CURInitial = 0;
	    if (frmTouchNumpad->ShowModal() == mrOk)
    	{
            if(frmTouchNumpad->CURResult < CurrentTransaction.Money.RoundedPaymentDue)
            {
              MessageBox("Please Enter Value Greater or Equal to Bill Amount","",MB_OK) ;
              return;
            }
		    else
		    {
                if(Payment->Name == "GC" )
                {
                    TGlobalSettings::Instance().GiftCard_Megaworld = frmTouchNumpad->CURResult  ;
                    TGlobalSettings::Instance().GiftCard_MegaworldForDaily = frmTouchNumpad->CURResult  ;
                }
                else if(Payment->Name == "Check")
                {
                    TGlobalSettings::Instance().CheckSaleMegaworld = frmTouchNumpad->CURResult  ;
                    TGlobalSettings::Instance().CheckSaleMegaworldForDaily = frmTouchNumpad->CURResult  ;
                }
		    }
	    }
      }

        GetPaymentNote(Payment);
        Payment->Result = eProcessing;

        if (TDeviceRealTerminal::Instance().PaymentSystem->ForceTender  && TDeviceRealTerminal::Instance().PaymentSystem->FTBypassElecTranTyp)
        {
            if (Payment->GetPaymentAttribute(ePayTypeElectronicTransaction) && wrkPayAmount == 0.0)
            {
                wrkPayAmount = CurrentTransaction.Money.PaymentDue;
            }
        }
        // Warn user about Payment Surcharge.
        Currency Surcharge = 0;
        if ((Payment->GetPaymentAttribute(ePayTypeSurcharge)) && wrkPayAmount > 0)
        {
            bool IsPercent = false;
            if (Payment->PercentAdjust != 0)
            {
                IsPercent = true;
                Surcharge = RoundToNearest(wrkPayAmount*-(Payment->PercentAdjust / 100), MIN_CURRENCY_VALUE,
                TGlobalSettings::Instance().MidPointRoundsDown);
            }
            else
            {
                Surcharge = RoundToNearest(-Payment->AmountAdjust, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
            }

            if (Surcharge > 0)
            {
                AnsiString SurchargeMsg = "A Surcharge of " + CurrToStrF(Surcharge, ffNumber, CurrencyDecimals)
                + " will be incurred by using this method of payment";
                MessageBox(SurchargeMsg.c_str(), "Warning", MB_OK + MB_ICONINFORMATION);
            }
            else
            {
                AnsiString SurchargeMsg = "A Discount of " + CurrToStrF(Surcharge, ffNumber, CurrencyDecimals)
                + " will be incurred by using this method of payment";
                MessageBox(SurchargeMsg.c_str(), "Warning", MB_OK + MB_ICONINFORMATION);
            }

            Payment->SetAdjustment(Surcharge);
            wrkPayAmount += Surcharge;
        }
        else
        {
            Payment->SetAdjustment(0);
        }

		Currency tempPaymentDue;

        if (Payment->GetPaymentAttribute(ePayTypePoints))
        {

            if(TGlobalSettings::Instance().ShowScreenToSelectItemForPoint && !TGlobalSettings::Instance().UseTierLevels &&
               !CurrentTransaction.Membership.Member.Points.PointsRules.Contains(eprNoPointsRedemption))
            {
                ShowFormToSelectItems(Payment);
            }
            else
            {
               ProcessPointPayment(Payment);
            }
            PayNotCash = true;  // add method for member's types...
        }
        else if (Payment->GetPaymentAttribute(ePayTypeCredit))
        {
            TTabCredit Credit = CurrentTransaction.TabCredit[Payment->TabCreditKey];
            if (wrkPayAmount > Credit.CurrentCredit)
            {
                MessageBox(AnsiString("Only " + CurrToStrF(Credit.CurrentCredit, ffNumber,
                CurrencyDecimals) + " of credit can be applied to the purchases.").c_str(), "Warning",
                MB_OK + MB_ICONINFORMATION);
                Payment->SetPay(Credit.CurrentCredit);
                Credit.CreditRedeemed = Credit.CurrentCredit;
            }
            else
            {
                Payment->SetPay(wrkPayAmount);
                Credit.CreditRedeemed = wrkPayAmount;
            }
            CurrentTransaction.TabCredit[Payment->TabCreditKey] = Credit;
        }
        else if (wrkPayAmount > CurrentTransaction.Money.PaymentDue + Payment->GetAdjustment())
        {
            if (Payment->GetPaymentAttribute(ePayTypeElectronicTransaction) && Payment->GetPaymentAttribute(ePayTypeAllowCashOut))
            {
                if(!TGlobalSettings::Instance().EnableEftPosAdyen && !TGlobalSettings::Instance().EnableEftPosPaymentSense)
                {
                    if (MessageBox(AnsiString("Only " + CurrToStrF(CurrentTransaction.Money.PaymentDue + Payment->GetAdjustment(),
                                        ffNumber, CurrencyDecimals) +
                                    " can be applied to the purchases. Do you want the difference to be cash-out?").c_str(), "Warning",
                                MB_OKCANCEL + MB_ICONINFORMATION) == ID_OK)
                    {
                        Payment->SetCashOut(wrkPayAmount - (CurrentTransaction.Money.PaymentDue + Payment->GetAdjustment()));
                        Payment->SetPay(CurrentTransaction.Money.PaymentDue);
                    }
                    else
                    {
                        Payment->SetPay(CurrentTransaction.Money.PaymentDue);
                    }
                }
                else
                {
                    MessageBox(AnsiString("Amount entered i.e. " + CurrencyString+ " " +CurrToStrF(wrkPayAmount,ffNumber, CurrencyDecimals) +
                                    " is more than due amount. This Integrated EFTPOS does not support this feature.").c_str(), "Info",
                                MB_OK + MB_ICONINFORMATION);
                    Payment->SetPay(CurrentTransaction.Money.PaymentDue);
                }
            }
            else if (!Payment->GetPaymentAttribute(ePayTypeCash) && !Payment->GetPaymentAttribute(ePayTypeCredit))
            {
                if (MessageBox(AnsiString("Only " + CurrToStrF(CurrentTransaction.Money.PaymentDue + Payment->GetAdjustment(),
                                    ffNumber, CurrencyDecimals) + " can be applied to the purchases. Continue?").c_str(), "Warning",
                            MB_OKCANCEL + MB_ICONINFORMATION) == ID_OK)
                {

                    Payment->SetPay(CurrentTransaction.Money.PaymentDue);
                    wrkPayAmount = wrkPayAmount - (CurrentTransaction.Money.PaymentDue + Payment->GetAdjustment());
                }
            }
            else
            {
                if(Payment->GetFirstAssignedGroup().Name == "Gift Card")
                {
                    Currency Amount = wrkPayAmount - CurrentTransaction.Money.PaymentDue;
                    Payment->SetPay(wrkPayAmount);
                    Payment->SetAdjustment(Amount);
                    Payment->AdjustmentReason = "Gift Card Surcharges";
                }
                else
                {
                    Payment->SetPay(wrkPayAmount);
                }
            }
        }
        else if (Payment->GetPaymentAttribute(ePayTypeSecondaryPMSExport) && wrkPayAmount != CurrentTransaction.Money.GrandTotal)
        {
            MessageBox(AnsiString("The total of the bill must be charged to the Room").c_str(), "Warning",
            MB_OK + MB_ICONINFORMATION);
        }

         else if(CurrentTransaction.Money.Change != 0 && CurrentTransaction.SalesType == eCreditPurchase )
         {

           wrkPayAmount =  -CurrentTransaction.Money.Change;
           Payment->SetPay(wrkPayAmount);

         }

        else
        {
            tempPaymentDue = CurrentTransaction.Money.PaymentDue;
            Payment->SetPay(wrkPayAmount);
        }

        if (wrkPayAmount != 0 && Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails))
        {
            if(Payment->IsLoyaltyVoucher() && TGlobalSettings::Instance().LoyaltyMateEnabled )
            {
               ProcessLoyaltyVoucher(Payment);
            }
            else
            {
                std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
                frmTouchKeyboard->MaxLength = 50;
                frmTouchKeyboard->AllowCarriageReturn = false;
                frmTouchKeyboard->StartWithShiftDown = false;
                frmTouchKeyboard->MustHaveValue = true;
                frmTouchKeyboard->KeyboardText = Payment->ReferenceNumber;
                frmTouchKeyboard->Caption = "Enter Voucher Number";
                if (frmTouchKeyboard->ShowModal() == mrOk)
                {
                    Payment->ReferenceNumber = frmTouchKeyboard->KeyboardText;
                }
                else
                {
                    CurrentTransaction.Money.PaymentDue = tempPaymentDue;
                    Payment->Reset();
                    return;
                }
                // Set the System name in case it has been set by a Voucher Purchase attempt.
                Payment->SysNameOveride = Payment->Name;
            }
        }
         else if(wrkPayAmount != 0)
        {
          Payment->ReferenceNumber = voucherNumber;
        }

        if (wrkPayAmount != 0 && Payment->GetPaymentAttribute(ePayTypePocketVoucher))
        {
            std::auto_ptr <TfrmPocketVoucher> frmPocketVoucher(TfrmPocketVoucher::Create <TfrmPocketVoucher> (this));

            //frmPocketVoucher->SetFixedVoucherCode(Payment->FixedVoucherCode);
            if (frmPocketVoucher->ShowModal() == mrOk)
            {
                Payment->VoucherCode = frmPocketVoucher->VoucherCode();
            }
            else
            {
                Payment->SetPay(0);
                Payment->VoucherCode = "";
            }

            // Set the System name in case it has been set by a Voucher Purchase attempt.
            //Payment->SysNameOveride = "Redeemed " + Payment->Name;
            Payment->SysNameOveride = Payment->Name;
        }

        if (Payment->GetPaymentAttribute(ePayTypeCSV))
        {
            if(Payment->GetPaymentAttribute(ePayTypeReservationMasterPay))
            {
                do
                {
                    std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
                    frmTouchKeyboard->MaxLength = 30;
                    frmTouchKeyboard->AllowCarriageReturn = false;
                    frmTouchKeyboard->StartWithShiftDown = false;
                    frmTouchKeyboard->MustHaveValue = true;
                    frmTouchKeyboard->KeyboardText = "";
                    frmTouchKeyboard->Caption = "Enter the " + Payment->Name + " number";
                    if(frmTouchKeyboard->ShowModal() == mrOk)
                    {
                        Payment->CSVString = frmTouchKeyboard->KeyboardText ;
                    }
                }
                while (Payment->CSVString == "");
            }
            else
            {
                Payment->CSVNumber = 0;
                do
                {
                    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
                    frmTouchNumpad->Caption = "Enter the " + Payment->Name + " number";
                    frmTouchNumpad->btnSurcharge->Caption = "Ok";
                    frmTouchNumpad->btnSurcharge->Visible = true;
                    frmTouchNumpad->btnDiscount->Visible = false;
                    frmTouchNumpad->Mode = pmNumber;
                    frmTouchNumpad->INTInitial = 0;
                    if (frmTouchNumpad->ShowModal() == mrOk)
                    {
                        Payment->CSVNumber = frmTouchNumpad->INTResult;
                    }
                }
                while (Payment->CSVNumber == 0);
            }
        }

        if (Payment->GetPaymentAttribute(ePayTypeRoomInterface))
        {

//              for (int i = 0; i < CurrentTransaction.PaymentsCount(); i++)
//          {
//                 TPayment *SubPayment = CurrentTransaction.PaymentGet(i);
//
//                 String abc = SubPayment->GetPropertyString();
//                 MessageBox(abc,"SubPayment->GetPropertyString()",MB_OK);
//
//            }

            bool GuestMasterOk = true;
            AnsiString TabName = "";
            int RoomNumber = 0;
            UnicodeString RoomNumberStr = "";
            if (CurrentTransaction.RoomNumber == 0)
            {
                if (TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled)
                {
                    if (TRooms::Instance().SelectRoom(CurrentTransaction.DBTransaction) == mrOk)
                    {
                        CurrentTransaction.RoomNumber = TRooms::Instance().SelectedRoom->RoomNo;
                        CurrentTransaction.BookingID = TRooms::Instance().SelectedRoom->BookingID;
                        CurrentTransaction.SalesType = eRoomSale;
                        TabName = TRooms::Instance().SelectedRoom->Name;
                        RoomNumber = TRooms::Instance().SelectedRoom->RoomNo;
                    }
                    else
                    {
                        GuestMasterOk = false;
                    }
                }
                else if (!TRooms::Instance().Enabled && TDeviceRealTerminal::Instance().BasePMS->Enabled)
                {
                    // Override the ipaddress and port.
                    AnsiString PMSIPAddress;
                    int PMSPort;
                    if (Payment->SecondaryPMSIPAddress != "")
                    {
                        PMSIPAddress = Payment->SecondaryPMSIPAddress;
                        PMSPort = Payment->SecondaryPMSPortNumber;
                    }
                    else
                    {
                        PMSIPAddress = TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
                        PMSPort = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
                    }
                    if(!IsSavedSavesWithSiHot(CurrentTransaction))
                    {
                        std::auto_ptr <TfrmPhoenixRoom> frmPhoenixRoom(TfrmPhoenixRoom::Create <TfrmPhoenixRoom> (this));
                        if (frmPhoenixRoom->SelectRoom(PMSIPAddress, PMSPort) == mrOk)
                        {
                            if(TGlobalSettings::Instance().PMSType == SiHot)
                            {
                                if(((double)Payment->GetPay() > frmPhoenixRoom->LimitSiHot) && (frmPhoenixRoom->LimitSiHot != 0.0))
                                {
                                    GuestMasterOk = false;
                                    MessageBox("Credit Limit Exceeded","Info",MB_OK);
                                }
                            }
							if(TGlobalSettings::Instance().PMSType == Oracle)
							{
								if(Payment->GetPay() > StrToCurr(frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].CreditLimit))
								{
								   GuestMasterOk = false;
								   MessageBox("Credit Limit Exceeded","Info",MB_OK);
								}
							}							
                            if(GuestMasterOk)
                            {
                                CurrentTransaction.Phoenix.AccountNumber = frmPhoenixRoom->SelectedRoom.AccountNumber;
                                CurrentTransaction.Phoenix.AccountName = frmPhoenixRoom->SelectedRoom.Folders->Strings
                                [frmPhoenixRoom->SelectedRoom.FolderNumber - 1];
                                CurrentTransaction.Phoenix.FolderNumber = frmPhoenixRoom->SelectedRoom.FolderNumber;
                                CurrentTransaction.SalesType = eRoomSale;
                                RoomNumber = StrToIntDef(frmPhoenixRoom->SelectedRoom.AccountNumber, frmPhoenixRoom->SelectedRoom.FolderNumber);
                                CurrentTransaction.Phoenix.RoomNumber = frmPhoenixRoom->SelectedRoom.SiHotRoom;
								if(TGlobalSettings::Instance().PMSType == Oracle)
								{
									CurrentTransaction.Customer.RoomNumber = atoi(frmPhoenixRoom->SelectedRoom.AccountNumber.c_str());
									TabName = frmPhoenixRoom->SelectedRoom.AccountNumber;
									CurrentTransaction.Customer.RoomNumber =
													atoi(frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].RoomNumber.c_str());
									TabName =
													atoi(frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].RoomNumber.c_str());
									CurrentTransaction.PMSClientDetails.ReservationID =
													frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].ReservationId;
									CurrentTransaction.PMSClientDetails.SequenceNumber =
													frmPhoenixRoom->roomResult.SequenceNumber;
									CurrentTransaction.PMSClientDetails.ProfileID =
													frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].ProfileId;
									CurrentTransaction.PMSClientDetails.CreditLimit =
													frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].CreditLimit;
									CurrentTransaction.PMSClientDetails.HotelID =
													frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].HotelId;
									CurrentTransaction.PMSClientDetails.FirstName =
													frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].FirstName;
									CurrentTransaction.PMSClientDetails.LastName =
													frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].LastName;
									CurrentTransaction.PMSClientDetails.MatchIdentifier =frmPhoenixRoom->SelectedRoom.FolderNumber;
									CurrentTransaction.PMSClientDetails.RoomNumber =
													frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].RoomNumber;
									CurrentTransaction.PMSClientDetails.Date =
													frmPhoenixRoom->roomResult.Date;
									CurrentTransaction.PMSClientDetails.Time =
													frmPhoenixRoom->roomResult.Time;
									CurrentTransaction.Phoenix.AccountNumber = CurrentTransaction.Customer.RoomNumber;
									CurrentTransaction.Phoenix.AccountName = frmPhoenixRoom->SelectedRoom.Folders->Strings
									[frmPhoenixRoom->SelectedRoom.FolderNumber - 1];
									CurrentTransaction.Phoenix.FolderNumber = frmPhoenixRoom->SelectedRoom.FolderNumber;
									CurrentTransaction.SalesType = eRoomSale;
									RoomNumber = atoi(frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].RoomNumber.c_str());
									CurrentTransaction.Phoenix.RoomNumber =
										atoi(frmPhoenixRoom->roomResult.RoomInquiryItem[frmPhoenixRoom->SelectedRoom.FolderNumber-1].RoomNumber.c_str());
								}
                                else if(TGlobalSettings::Instance().PMSType == Mews)
                                {
                                    CurrentTransaction.Customer.RoomNumberStr = frmPhoenixRoom->SelectedRoom.SiHotRoom;
                                    CurrentTransaction.Phoenix.FirstName =  frmPhoenixRoom->CustomersMews[frmPhoenixRoom->SelectedRoom.FolderNumber-1].FirstName;
                                    CurrentTransaction.Phoenix.LastName =  frmPhoenixRoom->CustomersMews[frmPhoenixRoom->SelectedRoom.FolderNumber-1].LastName;
                                    CurrentTransaction.Phoenix.AccountNumber = frmPhoenixRoom->CustomersMews[frmPhoenixRoom->SelectedRoom.FolderNumber-1].Id;
                                    TabName = frmPhoenixRoom->SelectedRoom.SiHotRoom;
                                }
                                else if(TGlobalSettings::Instance().PMSType != SiHot)
                                {
                                    CurrentTransaction.Customer.RoomNumber = atoi(frmPhoenixRoom->SelectedRoom.AccountNumber.c_str());
                                    TabName = frmPhoenixRoom->SelectedRoom.AccountNumber;
                                }
                                else
                                {
                                    CurrentTransaction.Customer.RoomNumberStr = frmPhoenixRoom->SelectedRoom.SiHotRoom;
                                    CurrentTransaction.Phoenix.FirstName =  frmPhoenixRoom->SiHotAccounts[frmPhoenixRoom->SelectedRoom.FolderNumber-1].AccountDetails[0].FirstName;
                                    CurrentTransaction.Phoenix.LastName =  frmPhoenixRoom->SiHotAccounts[frmPhoenixRoom->SelectedRoom.FolderNumber-1].AccountDetails[0].LastName;
                                    TabName = frmPhoenixRoom->SelectedRoom.SiHotRoom;
                                }
                            }
                        }
                        else
                        {
                            GuestMasterOk = false;
                            //Make select member false because Cancel is Pressed
                            if(CurrentTransaction.IsQuickPayTransaction)
                                IsMemberSelected = false;
                        }
                    }
                    else
                    {
                        CurrentTransaction.SalesType = eRoomSale;
                        for(int orderIndex = 0; orderIndex <  CurrentTransaction.Orders->Count; orderIndex++)
                        {
                            if(((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNoStr != "")
                            {
                                CurrentTransaction.Customer.RoomNumberStr = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNoStr;
                                CurrentTransaction.Phoenix.FirstName =  ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->FirstName;
                                CurrentTransaction.Phoenix.LastName =  ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->LastName;
                                TabName = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNoStr;
                                RoomNumberStr = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNoStr;
                                CurrentTransaction.Phoenix.AccountNumber = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->AccNo;
                                CurrentTransaction.Phoenix.AccountName = CurrentTransaction.Phoenix.FirstName + " " +
                                                                         CurrentTransaction.Phoenix.LastName;
                                CurrentTransaction.Phoenix.RoomNumber = ((TItemComplete*)CurrentTransaction.Orders->Items[orderIndex])->RoomNoStr;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    MessageBox("Neither Rooms nor Total Hospitality System interfaces are enabled.", "Error",
                    MB_OK + MB_ICONINFORMATION);
                    GuestMasterOk = false;

                    //Make select member false because PMS is not enabled
                    if(CurrentTransaction.IsQuickPayTransaction )
                            IsMemberSelected = false;
                }
            }

            if (!GuestMasterOk)
            {
                Payment->SetPay(0);
                Payment->SetAdjustment(0);
                Payment->SetCashOut(0);
                // Payment->GrandTotal = 0;
                Payment->Result = eFailed;
                CurrentTransaction.BookingID = 0;
                CurrentTransaction.RoomNumber = 0;
            }
            else
            {
                if (CurrentTransaction.Orders != NULL)
                {
                    for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
                    {
                        TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[i];
                        Order->TabContainerName = TabName;
                        Order->TabName = TabName;
                        Order->TabType = TabRoom;
                        if(TGlobalSettings::Instance().PMSType == SiHot)
                        {
                            Order->RoomNoStr = CurrentTransaction.Customer.RoomNumberStr;
                            Order->FirstName = CurrentTransaction.Phoenix.FirstName;
                            Order->LastName = CurrentTransaction.Phoenix.LastName;
                            Order->AccNo = CurrentTransaction.Phoenix.AccountNumber;
                        }
                        else
                            Order->RoomNo = RoomNumber;
                    }
                }
            }

        }

        if (Payment->GetPaymentAttribute(ePayTypeRMSInterface))
        {
            std::auto_ptr <TfrmRMSRoom> frmRoom(TfrmRMSRoom::Create <TfrmRMSRoom> (this));
            if ( (TGlobalSettings::Instance().NewBook!=2)&&frmRoom->SelectRoom(Payment->CVSReadLocation) == mrOk)
            {
                Payment->RMSRoom = frmRoom->SelectedRoom;
                CurrentTransaction.SalesType = eRoomSale;
            }
            else if (TDeviceRealTerminal::Instance().BasePMS->Registered&&(TGlobalSettings::Instance().NewBook==2)&&frmRoom->SelectNewBookRoom(Payment->CVSReadLocation) == mrOk)
            {
                Payment->NewBookRoom = frmRoom-> NewBookSelectedRoom;
                CurrentTransaction.RoomNumber=StrToInt(frmRoom-> NewBookSelectedRoom.RoomNo);
                CurrentTransaction.SalesType = eRoomSale;
            }
            else
            {
                Payment->SetPay(0);
                Payment->SetAdjustment(0);
                Payment->SetCashOut(0);
                Payment->Result = eFailed;
            }
        }

        if(Payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS) &&
           EftPos->CheckOverLimitExceeded(Payment->GetPayTendered() + Payment->GetCashOutTotal()))
        {
            MessageBox("Eftpos over limit exceeded.", "Eftpos Error.", MB_OK + MB_ICONERROR);
            Payment->SetPay(0);
        }

        if(wrkPayAmount != 0 && Payment->GetPaymentAttribute(ePayTypeWallet))
        {  
           ProcessWalletTransaction(Payment);
        }
        else if(wrkPayAmount != 0 && Payment->GetPaymentAttribute(ePayTypeSmartConnectQR))
        {
           Payment->SetPay(wrkPayAmount);
        }

        //apply changes here..
        if(CheckOnlinePaidOrNot())
        {
            ShowWebOrderMembersPayment();
        }
        else
        {

            ShowPaymentTotals(false);
        }

        if (NoElectronicPayments())
        {
            EnableElectronicPayments();
        }
        else
        {
            DisableOtherElectronicPayments(Payment);
        }
    }

    if (CurrentTransaction.Money.PaymentDue <= 0.01)
    {
        if (CurrentTransaction.SalesType == eCreditPurchase)
        {
            ModalResult = mrOk;
        }
        else
        {
            if (CurrentTransaction.Money.GrandTotal != 0 || CurrentTransaction.Orders->Count > 0  )
            {
                ModalResult = mrOk;
               
            }
            else if (CurrentTransaction.Money.Change != 0 && CurrentTransaction.Money.PaymentCashOut != 0)
            { // Cashout.
                ModalResult = mrOk;
            }
            else
            {
                MessageBox("Nothing to Bill.", "Info", MB_OK + MB_ICONINFORMATION);
            }
        }
    }
    else
    {
       if(!CurrentTransaction.IsQuickPayTransaction)
            btnCancel->SetFocus();
    }
}
// ---------------------------------------------------------------------------
void  TfrmPaymentType::ProcessPointPayment(TPayment *Payment)
{
  bool isPaymentByWeight =  Payment->Name == CurrentTransaction.Membership.Member.Name + "'s Grams";
  UnicodeString MemberName = "";
  if(TDeviceRealTerminal::Instance().ManagerMembership->Authorise(CurrentTransaction.Membership.Member,wrkPayAmount) == lsAccepted)
    {
        Currency RoundedPoints;
        bool canRedeem = true;
        bool HasWrkPayAmountChanged = false;
        AnsiString str = " ";
        TPaymentTransaction PointsTransaction = CurrentTransaction;
        PointsTransaction.PaymentDelete(PointsTransaction.PaymentFindByProperty(ePayTypePoints));
        bool isAllowEarntRedemption = PointsTransaction.Membership.Member.MemberType == 1 ? TDBTierLevel::IsAllowEarnedRedemption(CurrentTransaction.DBTransaction,
                                 CurrentTransaction.Membership.Member.TierLevel) : true ;
        if (TGlobalSettings::Instance().SpendPtsOnlySelectItems)
        {
            for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
            {
                TItemComplete *order = reinterpret_cast<TItemComplete *>(CurrentTransaction.Orders->Items[i]);
                if (!order->CanBePaidForUsingPoints())
                {
                        PointsTransaction.Orders->Remove(order);
                }
            }
        }
        PointsTransaction.IgnoreLoyaltyKey = false;
        PointsTransaction.Recalc();

        RoundedPoints = GetAvailableRedeemPoints(PointsTransaction);

        if (TGlobalSettings::Instance().EnableSeperateEarntPts)
        {
            str = " loaded ";
        }
        //Check for weight redeemable
        Currency TotalPoints = 0;
        if(isPaymentByWeight)
          {
             RoundedPoints -=  CurrentTransaction.RedeemPointsInformation->TotalPoints;
             Currency TotalWeight = GetTotalWeightInOrder(PointsTransaction.Orders);

             if(TotalWeight ==0)
             {
               MessageBox(AnsiString("You can not purchase non-weighted item with this payment type.").c_str(), "Error", MB_OK + MB_ICONINFORMATION);
                canRedeem = false;
             }
             else if(IsWrkPayAmountChanged)
             {
                 if(TotalWeight < wrkPayAmount)
                 {
                    MessageBox(AnsiString("Weight redeemed is more than weight ordered.").c_str(), "Error", MB_OK + MB_ICONINFORMATION);
                    canRedeem = false;
                 }
                 else
                 {
                    TotalPoints = CalculatePointsForWeight(PointsTransaction.Orders);
                    if(TotalPoints > RoundedPoints)
                    {
                      MessageBox(AnsiString("You don't have enough points to redeem.").c_str(), "Error", MB_OK + MB_ICONINFORMATION);
                      canRedeem = false;
                    }
                 }
             }
             else
             {
                TotalPoints = CalculatePointsForWeight(PointsTransaction.Orders);
             }
          }
        else
          {
             RoundedPoints -=  CurrentTransaction.RedeemWeightInformation->TotalPoints;
             TotalPoints =  RoundedPoints;
          }

        if(!canRedeem)
         {
           return;
         }

        TRedeemPointsInformation *RedeemPointsInformation = new TRedeemPointsInformation;

        if ((TGlobalSettings::Instance().UseTierLevels && PointsTransaction.Membership.Member.MemberType == 1)||
            PointsTransaction.Membership.Member.MemberType == 2)
        {
            RedeemPointsInformation->RemainingPoints = TotalPoints > RoundedPoints ? RoundedPoints :TotalPoints;

            if(!IsWrkPayAmountChanged)
              RedeemPointsInformation->RemainingPoints = RoundedPoints;

            Currency AmountToPay = PointsTransaction.Money.PaymentDue + Payment->GetAdjustment();

            if(!isPaymentByWeight &&  IsWrkPayAmountChanged)
               AmountToPay = wrkPayAmount;

            CalculateRedeemedPoints(PointsTransaction.Orders,RedeemPointsInformation,AmountToPay,isPaymentByWeight);
            if(isPaymentByWeight)
             {
               CurrentTransaction.RedeemWeightInformation->TotalValue = RedeemPointsInformation->TotalValue;
               CurrentTransaction.RedeemWeightInformation->TotalPoints = RedeemPointsInformation->TotalPoints;
             }
             else
             {
                CurrentTransaction.RedeemPointsInformation->TotalValue = RedeemPointsInformation->TotalValue;
                CurrentTransaction.RedeemPointsInformation->TotalPoints = RedeemPointsInformation->TotalPoints;
             }

            wrkPayAmount = RedeemPointsInformation->TotalValue;
        }

        if (wrkPayAmount > PointsTransaction.Money.GrandTotal)
        {
            wrkPayAmount = PointsTransaction.Money.GrandTotal;
            HasWrkPayAmountChanged = true;
        }
        if(Payment->Name == "Dining")
        {
            RoundedPoints = TGlobalSettings::Instance().DiningBal;
        }
        if (PointsTransaction.Membership.Member.Points.PointsRules.Contains(eprNoPointsRedemption))
        {
                MessageBox(AnsiString("You cannot redeem points at this time.").c_str(), "Error", MB_OK + MB_ICONINFORMATION);
        }
        else if(TGlobalSettings::Instance().UseTierLevels && PointsTransaction.Membership.Member.MemberType == 1 &&
        PointsTransaction.Membership.Member.TierLevel == 0)
        {
                MessageBox(AnsiString("You should reach a Tier Level to be able to redeem points.").c_str(), "Error", MB_OK + MB_ICONINFORMATION);
        }
        else if (TGlobalSettings::Instance().UseTierLevels && PointsTransaction.Membership.Member.MemberType == 1
                  && RoundedPoints < TGlobalSettings::Instance().MinRedemptionPoint)
        {
            MessageBox(AnsiString("You must have at least " + CurrToStrF(TGlobalSettings::Instance().MinRedemptionPoint, ffNumber,
            CurrencyDecimals) + " points to start redeeming your points.").c_str(), "Error", MB_OK + MB_ICONINFORMATION);
        }
        else if(TGlobalSettings::Instance().UseTierLevels && !isAllowEarntRedemption && RoundedPoints <= 0)
        {
           MessageBox(AnsiString("Earnt points redemption is not allowed in this tier level.").c_str(), "Error", MB_OK + MB_ICONINFORMATION);
        }
        else if ((((TGlobalSettings::Instance().UseTierLevels && TotalPoints > RoundedPoints)||
                 (!TGlobalSettings::Instance().UseTierLevels && wrkPayAmount > RoundedPoints )) &&
                 //(TGlobalSettings::Instance().MembershipType != MembershipTypeExternal  &&
                 !PointsTransaction.Membership.Member.Points.PointsRules.Contains(eprAllowedNegitive)) ||
                 (PointsTransaction.Membership.Member.MemberType == 2 && (wrkPayAmount > RoundedPoints)))
        {
            if (RoundedPoints <= 0)
            {
                 MessageBox(AnsiString("You have no points left to redeem.").c_str(), "Error", MB_OK + MB_ICONINFORMATION);
            }
            else
            {
                Currency redeemedPoint = RoundedPoints;
                 if (TGlobalSettings::Instance().SpendPtsOnlyFullProduct)
                {
                    RoundedPoints = PointsKnapsack(PointsTransaction.Orders, RoundedPoints);
                    redeemedPoint = RoundedPoints;
                }

                if(TGlobalSettings::Instance().UseTierLevels && PointsTransaction.Membership.Member.MemberType == 1)
                {
                    redeemedPoint = RedeemPointsInformation->TotalPoints;
                    RoundedPoints = RedeemPointsInformation->TotalValue;
                }

                if(isPaymentByWeight)
                {
                   MessageBox(AnsiString("Only " + CurrToStrF(CalculateWeightForPoints(redeemedPoint), ffNumber,
                            CurrencyDecimals) + " Grams can be purchased with" + str + "points.").c_str(), "Warning", MB_OK + MB_ICONINFORMATION);
                }
                else
                {
                  MessageBox(AnsiString("Only " + CurrToStrF(redeemedPoint, ffNumber,
                            CurrencyDecimals) + " can be purchased with" + str + "points.").c_str(), "Warning", MB_OK + MB_ICONINFORMATION);
                }
                if(TGlobalSettings::Instance().PontsSpentCountedAsRevenue)
                {
                   ApplyMemberPointsDiscount(redeemedPoint,RoundedPoints,true);
                }
                else
                {
                  Payment->SetPay(RoundedPoints);
                }
             }
        }
        else if (wrkPayAmount > PointsTransaction.Money.PaymentDue + Payment->GetAdjustment())
        {
            // Do we want the Kanpsack here???
            MessageBox(AnsiString("Only " + CurrToStrF(PointsTransaction.Money.PaymentDue + Payment->GetAdjustment(), ffNumber,
                        CurrencyDecimals) + " of points can be applied to the purchases.").c_str(), "Warning",
                        MB_OK + MB_ICONINFORMATION);
            Payment->SetPay(PointsTransaction.Money.PaymentDue + Payment->GetAdjustment());
            wrkPayAmount = wrkPayAmount - (PointsTransaction.Money.PaymentDue + Payment->GetAdjustment());
        }
        else if (TGlobalSettings::Instance().SpendPtsOnlyFullProduct)
        {
            Currency AmountPtsToUse = 0;
            Currency redeemedPoint = 0;
            if(TGlobalSettings::Instance().UseTierLevels && PointsTransaction.Membership.Member.MemberType == 1)
            {
                 AmountPtsToUse = RedeemPointsInformation->TotalValue;
                 redeemedPoint =  RedeemPointsInformation->TotalPoints;
            }
            else
            {
                AmountPtsToUse = PointsKnapsack(PointsTransaction.Orders, wrkPayAmount);
                redeemedPoint = AmountPtsToUse;
            }

            if (AmountPtsToUse != wrkPayAmount)
            {
                    MessageBox(AnsiString("Only " + CurrToStrF(AmountPtsToUse, ffNumber,
                                        CurrencyDecimals) + " can be purchased with" + str + "points.").c_str(), "Warning",
                                        MB_OK + MB_ICONINFORMATION);
            }

            if(TGlobalSettings::Instance().PontsSpentCountedAsRevenue)
            {
               ApplyMemberPointsDiscount(redeemedPoint,AmountPtsToUse,true);
            }
            else
            {
              Payment->SetPay(AmountPtsToUse);
            }
        }
        else
        {
           Currency AmountPtsToUse = 0;
           Currency redeemedPoint = 0;
           if(TGlobalSettings::Instance().UseTierLevels && PointsTransaction.Membership.Member.MemberType == 1)
            {
                AmountPtsToUse = RedeemPointsInformation->TotalValue;
                redeemedPoint =  RedeemPointsInformation->TotalPoints;
            }
            else
            {
                AmountPtsToUse = wrkPayAmount;
                redeemedPoint =  AmountPtsToUse;

            }
            if(TGlobalSettings::Instance().PontsSpentCountedAsRevenue)
            {
               ApplyMemberPointsDiscount(redeemedPoint,AmountPtsToUse,true);
            }
            else
            {
              Payment->SetPay(AmountPtsToUse);
            }

            if (HasWrkPayAmountChanged)
            {
                MessageBox(AnsiString("Only " + CurrToStrF(wrkPayAmount, ffNumber,
                                        CurrencyDecimals) + " can be purchased with" + str + "points.").c_str(), "Warning",
                                        MB_OK + MB_ICONINFORMATION);
            }
        }

        if (Payment->GetPay() == 0)
        {
            int Index = CurrentTransaction.PaymentIndex(Payment);
            if (Index > -1)
            {
                tgPayments->Buttons[purchaseButtonIndex][ALTCOL]->Enabled = true;
            }
        }
        else
        {
            int Index = CurrentTransaction.PaymentIndex(Payment);
            if (Index > -1)
            {
                tgPayments->Buttons[purchaseButtonIndex][ALTCOL]->Enabled = false;
            }
        }
    }
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ProcessLoyaltyVoucher(TPayment *Payment)
{
    if(Payment->Name == "Voucher")
    {
        std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
        frmTouchKeyboard->MaxLength = 50;
        frmTouchKeyboard->AllowCarriageReturn = false;
        frmTouchKeyboard->StartWithShiftDown = false;
        frmTouchKeyboard->MustHaveValue = true;
        frmTouchKeyboard->KeyboardText = "";
        frmTouchKeyboard->Caption = "Enter Voucher Number";
        if (frmTouchKeyboard->ShowModal() == mrOk)
        {
          ProcessLoyaltyPocketVoucher(frmTouchKeyboard->KeyboardText,Payment);
        }
        else
        {
           CurrentTransaction.RedeemPocketVoucherInformation->VoucherNumber = "";
           Payment->SetPay(0);
        }
    }
    else if(Payment->Name == "Gift Card")
    {
        AnsiString voucherNumber = GetVoucherNumber(Payment->Name,Payment->ReferenceNumber,Payment->IsLoyaltyVoucher());
        if(voucherNumber == "")
        {
           CurrentTransaction.RedeemGiftVoucherInformation->VoucherNumber = "";
           Payment->SetPay(0);
        }
        else
        {
            if(DoLoyaltyGiftCardValidation(voucherNumber,CurrentTransaction.PurchasedGiftVoucherInformation->VoucherNumber))
            {
               ProcessLoyaltyGiftVoucherVoucher(voucherNumber,Payment);
            }
            else
            {
               CurrentTransaction.PurchasedGiftVoucherInformation->VoucherNumber = "";
               Payment->SetPay(0);
            }
        }
    }
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ProcessLoyaltyGiftVoucherVoucher(AnsiString voucherCode,TPayment *Payment)
{
    TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
    TGiftCardDetail GiftCardDetail;
    ManagerLoyaltyVoucher.GetGiftVoucherDetail(voucherCode,GiftCardDetail);
    double balance = GiftCardDetail.PointBalance;
    if(balance > 0 && GiftCardDetail.StatusCode == 1)
    {
       Currency amountToPay = 0;
       Currency amountDeducted = 0;
       if(TGlobalSettings::Instance().UseTierLevels && CurrentTransaction.Membership.Applied() &&
        CurrentTransaction.Membership.Member.MemberType == 1 && CurrentTransaction.Membership.Member.TierLevel > 0)
        {
            TRedeemPointsInformation *RedeemPointsInformation = new TRedeemPointsInformation;
            RedeemPointsInformation->RemainingPoints = balance;
            CalculateRedeemedPoints(CurrentTransaction.Orders,RedeemPointsInformation,wrkPayAmount,false,true);
            amountToPay = RedeemPointsInformation->TotalPoints;
            Payment->SetPay(RedeemPointsInformation->TotalValue);
            amountDeducted = RedeemPointsInformation->TotalValue;
        }
        else
        {
           if(wrkPayAmount > balance)
            {
              amountToPay = balance;

            }
            else
            {
              amountToPay = wrkPayAmount;
            }
            amountDeducted = amountToPay;
            Payment->SetPay(amountToPay);
        }
        MessageBox("Payment of " + FormatFloat("0.00",amountDeducted) +" approved and balance of " + FormatFloat("0.00",balance - amountToPay) +" remaining.", "Information", MB_OK + MB_ICONINFORMATION);
        CurrentTransaction.PurchasedGiftVoucherInformation->VoucherNumber = "";
        CurrentTransaction.RedeemGiftVoucherInformation->VoucherNumber = voucherCode;
        CurrentTransaction.RedeemGiftVoucherInformation->TotalSaleAmount = CurrentTransaction.Money.GrandTotal;
        CurrentTransaction.RedeemGiftVoucherInformation->RedeemedAmount = amountToPay;
        CurrentTransaction.RedeemGiftVoucherInformation->GiftVoucherAmount = balance;
        CurrentTransaction.RedeemGiftVoucherInformation->ExpiryDate = GiftCardDetail.ExpiryDate;
    }
    else
    {
       switch(GiftCardDetail.StatusCode)
       {
         case 1:
         MessageBox("The gift card with code " + voucherCode + " has zero balance.", "Warning", MB_OK + MB_ICONINFORMATION);
         break;
         case 2:
         MessageBox("Gift Card not found please try another card.", "Warning", MB_OK + MB_ICONINFORMATION);
         break;
         case 3:
         MessageBox("Gift Card expired.", "Warning", MB_OK + MB_ICONINFORMATION);
         break;
         case 4:
         MessageBox("Gift Card not valid and will start from " + GiftCardDetail.StartDate.FormatString("DD-MM-YYYY"), "Warning", MB_OK + MB_ICONINFORMATION);
         break;
       }
      CurrentTransaction.RedeemGiftVoucherInformation->VoucherNumber = "";
      Payment->SetPay(0);
    }
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ProcessLoyaltyPocketVoucher(AnsiString voucherCode,TPayment *Payment)
{
     Currency amountToPay = wrkPayAmount;
    TVoucherDetail VoucherDetail;
    TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
    ManagerLoyaltyVoucher.GetPocketVoucherDetail(voucherCode,VoucherDetail);
    if(VoucherDetail.DiscountCode != NULL && VoucherDetail.DiscountCode !="")
    {
       int discountKey = ManagerDiscount->GetDiscountKeyByCode(CurrentTransaction.DBTransaction,VoucherDetail.DiscountCode);
       TDiscount CurrentDiscount;
       if(ManagerDiscount->GetDiscount(CurrentTransaction.DBTransaction,discountKey,CurrentDiscount))
       {
           ManagerDiscount->ClearDiscounts(CurrentTransaction.Orders);
           ApplyDiscount(discountKey,TDeviceRealTerminal::Instance().User.ContactKey);
           CurrentTransaction.IgnoreLoyaltyKey = false;
           CurrentTransaction.Recalc();
           Currency discountAmount = fabs(CurrentTransaction.Money.ProductDiscount);
           ManagerDiscount->ClearDiscounts(CurrentTransaction.Orders);
           CurrentTransaction.IgnoreLoyaltyKey = false;
           CurrentTransaction.Recalc();
           if(discountAmount < amountToPay)
           {
             amountToPay = discountAmount;
           }
           Payment->SetPay(amountToPay);
           CurrentTransaction.RedeemPocketVoucherInformation->VoucherNumber = voucherCode;
           CurrentTransaction.RedeemPocketVoucherInformation->TotalSaleAmount = CurrentTransaction.Money.GrandTotal;
           CurrentTransaction.RedeemPocketVoucherInformation->RedeemedAmount = amountToPay;
       }

    }
    else
    {
       CurrentTransaction.RedeemPocketVoucherInformation->VoucherNumber = "";
       Payment->SetPay(0);
    }
}
// ---------------------------------------------------------------------------
bool TfrmPaymentType::DoLoyaltyGiftCardValidation(AnsiString redeemedGiftCard,AnsiString purchasedGiftcard)
{
   bool retVal = false;
   if(redeemedGiftCard == "" && purchasedGiftcard == "")
      retVal = true;
   else if(redeemedGiftCard != "" && purchasedGiftcard != "")
      retVal = false;
   else
     retVal = true;
    if(!retVal)
    {
      MessageBox(AnsiString("You cannot purchase and redeem gift card at same time.").c_str(), "Warning", MB_OK + MB_ICONINFORMATION);
    }
    return retVal;
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ProcessWalletTransaction(TPayment *Payment)
{
    if(ValidateWalletAccount(Payment))
    {
/*
            std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
            frmTouchKeyboard->MaxLength = 50;
            frmTouchKeyboard->AllowCarriageReturn = false;
            frmTouchKeyboard->StartWithShiftDown = false;
            frmTouchKeyboard->MustHaveValue = true;
            frmTouchKeyboard->KeyboardText = "";
            frmTouchKeyboard->Caption = "Enter/Scan QrCode";
            frmTouchKeyboard->pnlKeyboard->Enabled = false;
            if (frmTouchKeyboard->ShowModal() == mrOk && frmTouchKeyboard->KeyboardText.Trim() != "")
            {
                Payment->WalletQrCode = frmTouchKeyboard->KeyboardText.Trim();
                Payment->SetPay(wrkPayAmount);
            }
            else
            {
                Payment->WalletQrCode = "";
                Payment->SetPay(0);
            }

*/
             UnicodeString barCodeValue = "";
             std::auto_ptr <TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create <TfrmCardSwipe> (this));
             frmCardSwipe->tbOtherOpt->Visible = false;
             frmCardSwipe->Label1->Caption = "Scan QR Code";
             frmCardSwipe->ShowModal();
             if (frmCardSwipe->ModalResult == mrOk)
               {
                  barCodeValue = frmCardSwipe->SwipeString.Trim();
                  Payment->WalletQrCode =  barCodeValue;
                  Payment->SetPay(wrkPayAmount);
               }
             else
                {
               Payment->WalletQrCode = "";
               Payment->SetPay(0);
                }
    }
    else
    {
       MessageBox("Wallet Account information is not set. Please set up account information to use this payment type.", "Error", MB_OK + MB_ICONINFORMATION);
       Payment->WalletQrCode = "";
       Payment->SetPay(0);
    }
}
// ---------------------------------------------------------------------------
bool TfrmPaymentType::ValidateWalletAccount(TPayment *Payment)
{
    bool retVal = true;
    retVal = Payment->WalletUserName != "" && Payment->WalletPassword != "" && Payment->WalletSecurityToken != "";
    if(Payment->WalletType == eJioWallet)
        retVal = retVal && Payment->MerchentId != "" && Payment->TerminalId != "";
    return retVal;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::BtnPaymentAlt(TPayment *Payment)
{


	if (SecurePaymentAccess(Payment))
	{
		if(Payment->GetPaymentAttribute(ePayTypePoints) && !Payment->RefundPoints)
		{
			if (CurrentTransaction.Membership.Member.Points.PointsRules.Contains(eprNoPointsPurchases))
			{
				MessageBox(AnsiString("You cannot purchase points at this time.").c_str(), "Error", MB_OK + MB_ICONINFORMATION);
			}
			else
			{
				Payment->AdjustmentReason = "Purchased " + Payment->SysNameOveride;
				if (CurrentTransaction.CreditTransaction)
				{
					Payment->SetAdjustment(-wrkPayAmount);
					Payment->SetPay(0);
				}
				else
				{
					Payment->SetAdjustment(wrkPayAmount);
					Payment->SetPay(0);
				}
			}
		}
        else if(Payment->GetPaymentAttribute(ePayTypePoints) && Payment->RefundPoints)
		{
            if(TDeviceRealTerminal::Instance().ManagerMembership->Authorise(CurrentTransaction.Membership.Member, wrkPayAmount) == lsAccepted)
            {
                double RoundedPoints;
                if (TGlobalSettings::Instance().EnableSeperateEarntPts)
                {
                    RoundedPoints = CurrentTransaction.Membership.Member.Points.getPointsBalance(pasDatabase, ptstAccount);
                }
                else
                {
                    RoundedPoints = CurrentTransaction.Membership.Member.Points.getPointsBalance(pasDatabase);
                }
                Currency redeemedPoint = RoundedPoints;

                if(redeemedPoint < wrkPayAmount)
                {
                    MessageBox(AnsiString("Unable to refund more than current points total " +
                                            CurrToStrF(redeemedPoint, ffNumber, CurrencyDecimals) + ".").c_str(),
                                            "Error", MB_OK + MB_ICONINFORMATION);
                    ndWorkingAmount->Clear();
                    return;
                }

                Payment->AdjustmentReason = "Refund " + Payment->SysNameOveride;

                Payment->SetPay(0);
                Payment->SetRefundPointsValue(wrkPayAmount);

                isRefundInProgress = true;
                CurrentTransaction.CreditTransaction = true;
            }
        }
		else if(Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails))
		{


             voucherNumber = GetVoucherNumber(Payment->Name,Payment->ReferenceNumber,Payment->IsLoyaltyVoucher());
             Payment->ReferenceNumber = voucherNumber;
           if(voucherNumber == "")
           {
                Payment->Reset();
                return;
           }
           else
           {
              Payment->ReferenceNumber = voucherNumber;
           }

           if(Payment->IsLoyaltyVoucher() && TGlobalSettings::Instance().LoyaltyMateEnabled )
            {
               AnsiString messageString = "";
               bool isInValid = false;
               if(!IsGiftCardNumberValid(voucherNumber))
               {
                  messageString = "Gift Card number is not valid.";
                  isInValid = true;
               }
               else
               {
                   TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
                   TGiftCardDetail GiftCardDetail;
                   ManagerLoyaltyVoucher.GetGiftVoucherDetail(Payment->ReferenceNumber,GiftCardDetail);

                   bool isValidGiftCard = true;
                   if(TGlobalSettings::Instance().GiftCardValidation == CloudValidation)
                     isValidGiftCard = GiftCardDetail.StatusCode != 2;

                   if(isValidGiftCard && DoLoyaltyGiftCardValidation(CurrentTransaction.RedeemGiftVoucherInformation->VoucherNumber,Payment->ReferenceNumber))
                    {
                        CurrentTransaction.RedeemGiftVoucherInformation->VoucherNumber = "";
                        CurrentTransaction.PurchasedGiftVoucherInformation->VoucherNumber = Payment->ReferenceNumber;
                        CurrentTransaction.PurchasedGiftVoucherInformation->TotalSaleAmount = CurrentTransaction.Money.GrandTotal;
                        CurrentTransaction.PurchasedGiftVoucherInformation->RedeemedAmount = wrkPayAmount;
                        CurrentTransaction.PurchasedGiftVoucherInformation->GiftVoucherAmount = GiftCardDetail.PointBalance;
                        CurrentTransaction.PurchasedGiftVoucherInformation->ExpiryDate = GiftCardDetail.ExpiryDate;
                    }
                    else
                    {
                      messageString = "Gift Card not found please try another card.";
                      isInValid = true;
                    }
               }
               if(isInValid)
               {
                   CurrentTransaction.PurchasedGiftVoucherInformation->VoucherNumber = "";
                   Payment->Reset();
                   MessageBox(messageString, "Warning", MB_OK + MB_ICONINFORMATION);
                   return;
               }
            }

			if (CurrentTransaction.CreditTransaction)
			{
				Payment->SetAdjustment(-wrkPayAmount);
				Payment->AdjustmentReason = "Credited " + Payment->Name;
				Payment->SysNameOveride = "Credited " + Payment->Name;
			}
			else
			{
				Payment->SetAdjustment(wrkPayAmount);
				Payment->AdjustmentReason = "Purchased " + Payment->Name;
                Payment->SysNameOveride = Payment->Name;
			}
			Payment->SetPay(0);
		}
		else if (Payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS))
		{
			Payment->SetCashOut(wrkPayAmount);
			if (EftPos->CheckOverLimitExceeded(Payment->GetCashOutTotal() + Payment->GetPayTendered()))
			{
				Payment->SetCashOut(0);
				MessageBox("Eftpos over limit exceeded.", "Eftpos Error.", MB_OK + MB_ICONERROR);
			}
		}
		else
		{
			Payment->SetCashOut(wrkPayAmount);
		}

		ShowPaymentTotals();
		// Disable all other electronic transactions not attached to this payment.
		if (NoElectronicPayments())
		{
			EnableElectronicPayments();
		}
		else
		{
			DisableOtherElectronicPayments(Payment);
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::DisableOtherElectronicPayments(TPayment *inPayment)
{
	for (int i = 0; i < CurrentTransaction.PaymentsCount(); i++)
	{
		TPayment *Payment = CurrentTransaction.PaymentGet(i);
		if ((Payment != inPayment) && Payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS) && (Payment->GetCashOut() == 0) &&
				(Payment->GetPay() == 0))
		{
			int Index = CurrentTransaction.PaymentIndex(Payment);
			if (Index > -1)
			{
				for (int b = 0; b < tgPayments->RowCount; b++)
				{
					if (tgPayments->Buttons[b][PAYCOL]->Tag == Index)
					{
						tgPayments->Buttons[b][PAYCOL]->Enabled = false;
						tgPayments->Buttons[b][ALTCOL]->Enabled = false;
					}
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::DisableCashOutElectronicPayments()
{
	for (int i = 0; i < CurrentTransaction.PaymentsCount(); i++)
	{
		TPayment *Payment = CurrentTransaction.PaymentGet(i);
		if (Payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS))
		{
			int Index = CurrentTransaction.PaymentIndex(Payment);
			if (Index > -1)
			{
				for (int b = 0; b < tgPayments->RowCount; b++)
				{
					if (tgPayments->Buttons[b][ALTCOL]->Tag == Index)
					{
						tgPayments->Buttons[b][ALTCOL]->Enabled = false;
					}
				}
			}
			Payment->SetCashOut(0);
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::EnableCashOutElectronicPayments()
{
	for (int i = 0; i < CurrentTransaction.PaymentsCount(); i++)
	{
		TPayment *Payment = CurrentTransaction.PaymentGet(i);
		if(Payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS))
		{
			int Index = CurrentTransaction.PaymentIndex(Payment);
			if (Index > -1)
			{
				for (int b = 0; b < tgPayments->RowCount; b++)
				{
					if (tgPayments->Buttons[b][ALTCOL]->Tag == Index)
					{
						tgPayments->Buttons[b][ALTCOL]->Enabled = true;
					}
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::EnableElectronicPayments()
{
	for (int i = 0; i < CurrentTransaction.PaymentsCount(); i++)
	{
		TPayment *Payment = CurrentTransaction.PaymentGet(i);
		if(Payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS))
		{
			int Index = CurrentTransaction.PaymentIndex(Payment);
			if (Index > -1)
			{
				for (int b = 0; b < tgPayments->RowCount; b++)
				{
					if (tgPayments->Buttons[b][PAYCOL]->Tag == Index)
					{
                        if (b==0 &&  PAYCOL==0 )
                        {
                           int i=0;
                        }
                        if (b==0 &&  ALTCOL ==0)
                        {
                            int j=0;
                        }

						tgPayments->Buttons[b][PAYCOL]->Enabled = true;
						if (CurrentTransaction.CreditTransaction == false)
						{
    						tgPayments->Buttons[b][ALTCOL]->Enabled = true;
						}
					}
				}
			}
		}
	}
    if(!EftPos->IsCashOutSupported())
    {
        DisableCashOutElectronicPayments();
    }
}
// ---------------------------------------------------------------------------
bool TfrmPaymentType::NoElectronicPayments()
{
	bool Retval = true;
	for (int i = 0; i < CurrentTransaction.PaymentsCount(); i++)
	{
		TPayment *Payment = CurrentTransaction.PaymentGet(i);
		if(Payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS))
		{
			if (Payment->GetCashOut() != 0 || Payment->GetPay() != 0)
			{
				Retval = false;
			}
		}
	}
	return Retval;
}
// ---------------------------------------------------------------------------
__fastcall TPaymentTypeTouchButton::TPaymentTypeTouchButton(Classes::TComponent* AOwner) : TTouchBtn(AOwner)
{

}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::tbPatronCountClick(TObject *Sender)
{
    std::vector<TPatronType> patronsOld = CurrentTransaction.Patrons;
    std::vector<TPatronType> patronsNew;
    patronsNew.clear();
	if (TManagerPatron::Instance().GetCount(CurrentTransaction.DBTransaction) > 0)
	{
		std::auto_ptr <TfrmPatronCount> frmPatronCount(TfrmPatronCount::Create <TfrmPatronCount> (this,
		TDeviceRealTerminal::Instance().DBControl));
		frmPatronCount->InitPatrons = CurrentTransaction.Patrons;
		frmPatronCount->ShowModal();
		tbPatronCount->Caption = "Patron Count \r" + IntToStr(TManagerPatron::Instance().GetTotalPatrons(frmPatronCount->Patrons));
		CurrentTransaction.Patrons = frmPatronCount->Patrons;
        // ReStructure Bill
        patronsNew = CurrentTransaction.Patrons;
        if(ArePatronsChanged(patronsOld,patronsNew))
        {
            RestructureBillForPatrons();
        }
        if(TGlobalSettings::Instance().mallInfo.MallId==4)
          CurrentTransaction.IsPatronAdded = true;
		Reset();
		ShowPaymentTotals();
	}
	else
	{
		MessageBox("There are no Patron Types Configured.", "Patron Error.", MB_OK + MB_ICONERROR);
	}
}
//----------------------------------------------------------------------------
bool TfrmPaymentType::ArePatronsChanged(std::vector<TPatronType> patronsOld,std::vector<TPatronType> patronsNew)
{
    bool retValue = false;
    std::auto_ptr<TSCDPatronUtility> scdpatronUtility(new TSCDPatronUtility());
    retValue = scdpatronUtility->ArePatronsChanged(patronsOld,patronsNew);
    return retValue;
}
//----------------------------------------------------------------------------
void TfrmPaymentType::RestructureBillForPatrons()
{
    std::auto_ptr<TSCDPatronUtility> scdpatronUtility(new TSCDPatronUtility());
    scdpatronUtility->RestructureBill(CurrentTransaction);
}
// ---------------------------------------------------------------------------
bool TfrmPaymentType::SecurePaymentAccess(TPayment * Payment)
{
	// ODO: Add your source code here
	std::auto_ptr <TContactStaff> Staff(new TContactStaff(CurrentTransaction.DBTransaction));

	bool AccessGranted = false;
	if (Payment->GetPaymentAttribute(ePayTypeSecure1) || Payment->GetPaymentAttribute(ePayTypeSecure2) || Payment->GetPaymentAttribute(ePayTypeSecure3))
	{
		TMMContactInfo TempUserInfo;
		TempUserInfo = TDeviceRealTerminal::Instance().User;

		if (Payment->GetPaymentAttribute(ePayTypeSecure1))
		{
			AccessGranted = Staff->TestAccessLevel(TempUserInfo, CheckPaymentTypesSec1);
		}

		if(Payment->GetPaymentAttribute(ePayTypeSecure2) && !AccessGranted)
		{
			AccessGranted = Staff->TestAccessLevel(TempUserInfo, CheckPaymentTypesSec2);
		}

		if(Payment->GetPaymentAttribute(ePayTypeSecure3) && !AccessGranted)
		{
			AccessGranted = Staff->TestAccessLevel(TempUserInfo, CheckPaymentTypesSec3);
		}

		// If not, prompt for a login.
		if (!AccessGranted)
		{
			TempUserInfo.Clear();
			TLoginSuccess Result = Staff->Login(this, CurrentTransaction.DBTransaction, TempUserInfo, CheckNone);

			if (Result == lsPINIncorrect)
			{
				MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
			}
			else if (Payment->GetPaymentAttribute(ePayTypeSecure1))
			{
				AccessGranted = Staff->TestAccessLevel(TempUserInfo, CheckPaymentTypesSec1);
			}
			else if(Payment->GetPaymentAttribute(ePayTypeSecure2) && !AccessGranted)
			{
				AccessGranted = Staff->TestAccessLevel(TempUserInfo, CheckPaymentTypesSec2);
			}
			else if(Payment->GetPaymentAttribute(ePayTypeSecure3) && !AccessGranted)
			{
				AccessGranted = Staff->TestAccessLevel(TempUserInfo, CheckPaymentTypesSec3);
			}
			else if (Result == lsCancel)
			{
				return false;
			}
		}

		if (!AccessGranted)
		{
			MessageBox("You do not have access rights to this payment type.", "Error", MB_OK + MB_ICONERROR);
		}
		else
		{
			// Add Security.
			TSecurityReference *SecRef = new TSecurityReference;
			SecRef->UserKey = TempUserInfo.ContactKey;
			SecRef->Event = SecurityTypes[secSecurePayment];
			SecRef->From = Payment->Name;
			SecRef->To = "";
			SecRef->Note = "";
			SecRef->TimeStamp = Now();
			TDeviceRealTerminal::Instance().PaymentSystem->Security->SecurityAdd(SecRef);
		}
	}
	else
	{
		AccessGranted = true;
	}
	return AccessGranted;
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::GetPaymentNote(TPayment * Payment)
{
	if (Payment->GetPaymentAttribute(ePayTypeReqNote))
	{
		std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
		frmTouchKeyboard->MaxLength = 50;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = true;
		frmTouchKeyboard->KeyboardText = "";
		frmTouchKeyboard->Caption = "Please enter a payment note.";
		frmTouchKeyboard->ShowModal();
		if (frmTouchKeyboard->KeyboardText == "")
		{
			do
			{
				MessageBox("You must enter a payment note.", "Please Comply", MB_OK + MB_ICONINFORMATION);
				frmTouchKeyboard->ShowModal();
			}
			while (frmTouchKeyboard->KeyboardText == "");
		}
		Payment->Note = frmTouchKeyboard->KeyboardText;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::tbCreditClick(TObject *Sender)
{
	if (CurrentTransaction.CreditTransaction)
	{
		CurrentTransaction.CreditTransaction = false;

		TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(CurrentTransaction);
		Reset();

		CurrentTransaction.Patrons = PreCreditPatronCount;
		tbPatronCount->Caption = "Patron Count \r" + IntToStr(TManagerPatron::Instance().GetTotalPatrons(CurrentTransaction.Patrons));

		if (CurrentTransaction.Orders != NULL)
		{
			for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
			{
				TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[i];

				Order->Security->SecurityDelete(Order->Security->SecurityGetType(secCredit));

				Order->ReverseCredit();
				for (int i = 0; i < Order->SubOrders->Count; i++)
				{
					TItemCompleteSub *SubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
					SubOrder->Security->SecurityDelete(SubOrder->Security->SecurityGetType(secCredit));
					SubOrder->ReverseCredit();
				}
			}
		}

		if (NoElectronicPayments())
		{
			EnableElectronicPayments();
		}
		else
		{
			for (int i = 0; i < CurrentTransaction.PaymentsCount(); i++)
			{
				TPayment *Payment = CurrentTransaction.PaymentGet(i);
				if(Payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS) && (Payment->GetCashOut() != 0) && (Payment->GetPay() != 0))
				{
					DisableOtherElectronicPayments(Payment);
				}
			}
		}
		ShowPaymentTotals();
	}
	else
	{
		TMMContactInfo TempUserInfo;
		bool Allowed = false;
		TempUserInfo.Clear();
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(CurrentTransaction.DBTransaction));
		TLoginSuccess Result = Staff->Login(this, CurrentTransaction.DBTransaction, TempUserInfo, CheckCredit);
		if (Result == lsAccepted)
		{
			Allowed = true;
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access rights to Credits.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}

		if (Allowed)
		{
            if(TGlobalSettings::Instance().CaptureRefundRefNo)
            {
                if(!CaptureRefundReference())
                {
                    MessageBox("The Receipt No. is not valid. Either Refund is already taken or Receipt does not exist.", "Error", MB_OK + MB_ICONINFORMATION);
                    return;
                }
            }
			CurrentTransaction.CreditTransaction = true;

			TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(CurrentTransaction);
			Reset();

			EnableElectronicPayments();
			DisableCashOutElectronicPayments();

            if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL || TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
            {
    			PreCreditPatronCount = CurrentTransaction.Patrons;
		    	tbPatronCount->Caption = "Patron Count \r" + IntToStr(TManagerPatron::Instance().GetTotalPatrons(CurrentTransaction.Patrons));
            }
            else
            {
    			PreCreditPatronCount = CurrentTransaction.Patrons;
	    		CurrentTransaction.Patrons.clear();
		    	tbPatronCount->Caption = "Patron Count \r" + IntToStr(0);
            }
			AnsiString Note = "";
			std::auto_ptr <TfrmMessage> frmMessage(TfrmMessage::Create <TfrmMessage> (this, TDeviceRealTerminal::Instance().DBControl));
			frmMessage->MessageType = eCreditReason;
			frmMessage->ShowModal();
			if (TGlobalSettings::Instance().ForcedReasons && frmMessage->TextResult == "")
			{
				do
				{
					MessageBox("You must enter a credit reason", "Please Comply", MB_OK + MB_ICONINFORMATION);
					frmMessage->ShowModal();
				}
				while (frmMessage->TextResult == "");
			}
			Note = frmMessage->TextResult;

			bool WriteOffStock = true;
			if( IsReceipeAdded(CurrentTransaction.Orders))
            {
                if (MessageBox("Do you wish to Return this Item to Stock. \n(i.e. Put it back on the shelf)", "Return To Stock?",
                            MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON1) == IDYES)
                {
                   WriteOffStock = false; // picked yes so reverse stock
                }
            }

			// Add Security.
			if (CurrentTransaction.Orders != NULL)
			{
				for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
				{
					TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[i];
					int n = Order->GetQty();
					Order->Credit(WriteOffStock);
					TSecurityReference *SecRef = new TSecurityReference;
					SecRef->UserKey = TempUserInfo.ContactKey;
                    //Following if is used for differentiating between refund and writeoff in Menumate
                    if(WriteOffStock == true)
                    {

                    SecRef->Event=   SecurityTypes[secWriteOff];


                    }
                    else
                    {         SecRef->Event = SecurityTypes[secCredit];

                    }

					SecRef->From = "";
					SecRef->To = "";
					SecRef->Note = Note;
					SecRef->TimeStamp = Now();
					Order->Security->SecurityAdd(SecRef);

					for (int i = 0; i < Order->SubOrders->Count; i++)
					{
						TItemCompleteSub *SubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
						TSecurityReference *SecRefSubOrderCopy = new TSecurityReference;
						SecRefSubOrderCopy->SecRefCopy(SecRef);
						SubOrder->Security->SecurityAdd(SecRefSubOrderCopy);
						SubOrder->Credit(WriteOffStock);
					}
				}
			}

                      if(IsItemAssignToPrinter(CurrentTransaction.Orders))
                       {
                        if (MessageBox("Do you want to inform the chef?","Inform chef?",MB_YESNO | MB_ICONQUESTION)== IDYES)
                        {
                            /*
                                The following fix to generate refeund orders print to kitchen can be re-done in an efficient way. I was trying to get a complete copy of
                                paymentTransaction and pass it on to the kitchen so it will be used to generate the docket for chef.
                                However I faced some weired access violation issue related to categories of refunding item
                                hence end up with doing the inefficient way. this will need to be re-addressed in a later time.
                            */

                            // resetting quantity values for sending the refund orders to chef
                            if (CurrentTransaction.Orders != NULL)
                            {
                                for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
                                {
                                    TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[i];
                                    Order->ResetQty();

                                    for (int i = 0; i < Order->SubOrders->Count; i++)
                                    {
                                        TItemCompleteSub *SubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
                                        SubOrder->ResetQty();
                                    }
                                }
                            }
                            std::auto_ptr<TReqPrintJob> req(
                            new TReqPrintJob(&TDeviceRealTerminal::Instance()));
                            req->Transaction = &CurrentTransaction;
                            req->SenderType = devPC;
                            req->Waiter = TempUserInfo.Name;
                            CurrentTransaction.Money.Recalc(CurrentTransaction);

                            std::auto_ptr<TKitchen> Kitchen(new TKitchen());
                            Kitchen->Initialise(CurrentTransaction.DBTransaction);
                            Kitchen->GetPrintouts(CurrentTransaction.DBTransaction,
                            req.get());
                            req->Printouts->Print(devPC);
                            if (req->Header.Error != proA_Ok)
                            ShowMessage(req->Header.ErrorMsg);
                        }
                    }

			// setting the quantity to what it was before after printing to kitchen because the CurrentTransaction is used to generate receipts and affects zed as well.
			if (CurrentTransaction.Orders != NULL)
			{
				for (int i = 0; i < CurrentTransaction.Orders->Count; i++)
				{
					TItemComplete *Order = (TItemComplete*)CurrentTransaction.Orders->Items[i];
					Order->CreditQty();

					for (int i = 0; i < Order->SubOrders->Count; i++)
					{
						TItemCompleteSub *SubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
						SubOrder->CreditQty();
					}
				}
			}

			ShowPaymentTotals();
		}
	}
}
//---------------------------------------------------------------------------
bool TfrmPaymentType::CaptureRefundReference()
{
    UnicodeString capturedValue = "";
    bool retValue = false;
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->Caption = "Enter Reference Receipt No for Refund";
    frmTouchNumpad->btnSurcharge->Caption = "Ok";
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->btnSurcharge->Visible = true;
    frmTouchNumpad->Mode = pmNumber;
    if (frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->INTResult > 0)
    {
       UnicodeString capturedValue = frmTouchNumpad->INTResult;
       // Validate Refund Reference
       retValue = ValidateRefundReference(capturedValue);
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TfrmPaymentType::ValidateRefundReference(UnicodeString str)
{
    bool retValue = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    if(ValidateAlreadyRefunded(DBTransaction,str))
    {
        if(!IsRefundReceipt(DBTransaction,str))
        {
            CurrentTransaction.RefundRefReceipt = str;
            retValue = true;
        }
    }
    else
    {
        retValue = false;
    }
//    }
    DBTransaction.Commit();
    return retValue;
}
//----------------------------------------------------------------------------
bool TfrmPaymentType::ValidateAlreadyRefunded(Database::TDBTransaction &DBTransaction,UnicodeString str)
{
    TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery1->Close();
    IBInternalQuery1->SQL->Text =	" SELECT "
                                    " a.ARCBILL_KEY "
                                    " FROM "
                                    "  ARCBILL a"
                                    " WHERE "
                                    " a.REFUND_REFRECEIPT  = :REFUND_REFRECEIPT "

                                    " UNION ALL "

                                    " SELECT "
                                    " d.ARCBILL_KEY "
                                    " FROM "
                                    "  DAYARCBILL d"
                                    " WHERE "
                                    " d.REFUND_REFRECEIPT  = :REFUND_REFRECEIPT ";
    IBInternalQuery1->ParamByName("REFUND_REFRECEIPT")->AsString = str;
    IBInternalQuery1->ExecQuery();
    if(IBInternalQuery1->RecordCount == 0)
        return true;
    else
        return false;
}
//----------------------------------------------------------------------------
bool TfrmPaymentType::IsRefundReceipt(Database::TDBTransaction &DBTransaction,UnicodeString str)
{

    bool retValue = false;
    TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery1->Close();
    IBInternalQuery1->SQL->Text =	" SELECT "
                                    " a.ARCBILL_KEY, "
                                    " REFUND_REFRECEIPT, "
                                    " ORDER_TYPE_MESSAGE "
                                    " FROM "
                                    "  ARCBILL a"
                                    " WHERE "
                                    " a.INVOICE_NUMBER  = :INVOICE_NUMBER "

                                    " UNION ALL "

                                    " SELECT "
                                    " d.ARCBILL_KEY, "
                                    " REFUND_REFRECEIPT, "
                                    " ORDER_TYPE_MESSAGE "
                                    " FROM "
                                    "  DAYARCBILL d"
                                    " WHERE "
                                    " d.INVOICE_NUMBER  = :INVOICE_NUMBER ";
    IBInternalQuery1->ParamByName("INVOICE_NUMBER")->AsString = str;
    IBInternalQuery1->ExecQuery();
    if(IBInternalQuery1->RecordCount > 0)
    {
        if(IBInternalQuery1->FieldByName("REFUND_REFRECEIPT")->AsString != "")
            retValue = true;
        else
            retValue = false;
    }
    else
    {
        retValue = true;
    }
    return retValue;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::tnWorkingAmountClick(TObject *Sender, TNumpadKey Key)
{
	wrkPayAmount = ndWorkingAmount->Numeric();
        IsWrkPayAmountChanged = true;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::tgPaymentsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	TPayment *Payment = CurrentTransaction.PaymentGet(GridButton->Tag);
	int  tabkey=    TDeviceRealTerminal::Instance().PaymentSystem->GetPaymentTabName(CurrentTransaction.DBTransaction,Payment->Name);
	AnsiString  tabName   = TDBTab::GetTabName(CurrentTransaction.DBTransaction,tabkey);
    AnsiString str = Payment->Name;
    if(Payment->GetPaymentAttribute(ePayTypeOpensCashDrawer))
    {
        str += " true for Open Cash Drawer";
    }
    str += " "+Now();
    makeLogFile(str);

	if(tabkey>0 && tabName!="")
	{
        frmSelectDish->tabKey =tabkey;
		ModalResult = mrCancel;
	}
	else
	{
		if (tgPayments->Col(GridButton) == ALTCOL)
		{
			// Cash out Type.
			BtnPaymentAlt(Payment);
		}
		else
		{
            BtnPayment(Payment);
		}
		IsWrkPayAmountChanged = false;
	}

}
// ---------------------------------------------------------------------------
/* ---------------------------------------------------------------------------
* Knapsack Algorithm Implimentation for Case #14449
* Created by Chris Jensen 26/02/2013
* Last Edited by Chris Jensen 26/02/2013
* ---------------------------------------------------------------------------
* This algorithm is designed to take a list of orders, a members number of points
* and a step size. The method is to return the maxium points that can be used to
* pay the whole cost of any items brought with these points.
* Step size must be >= 0.1 or an exception will be thrown.
* ---------------------------------------------------------------------------
*/
Currency TfrmPaymentType::PointsKnapsack(TList *Orders, Currency AvailablePoints, Currency StepSize)
{
	std::vector<Currency> ItemCosts;
	for (int i = 0; i < Orders->Count; i++)
	{
		TItemComplete *Order = (TItemComplete*)Orders->Items[i];
		double OrderQty = Order->GetQty();
		Currency ItemCost = Order->TotalPriceAdjustmentSides() / OrderQty;
		//		Currency ItemCost = Order->TotalPriceSides();
		if (ItemCost < AvailablePoints && OrderQty >= 1)
		{
			for (int j = 0; j < (int) OrderQty; j++)
			{
				ItemCosts.push_back(ItemCost);		//Do I need rounding?
			}
		}
		else if (ItemCost == AvailablePoints && OrderQty >= 1)
		{
			return AvailablePoints;
		}

		OrderQty -= (int)OrderQty;

		if (ItemCost * OrderQty < AvailablePoints && OrderQty > 0)
		{
			ItemCosts.push_back(ItemCost * OrderQty);
		}
		else if (ItemCost * OrderQty == AvailablePoints && OrderQty > 0)
		{
			return AvailablePoints;
		}
	}

	int NumItems = ItemCosts.size();
	if (NumItems == 0)
	{
		return 0;
	}
	else if (NumItems == 1)
	{
		return ItemCosts[0];
	}

	Currency RetVal = 0;
	int MaxWeight = AvailablePoints.Val / StepSize.Val;
	CurrencyMatrix Knapsack(NumItems+1, MaxWeight+1);

	for (int Weight = 0; Weight <= MaxWeight; Weight++)
	{
		Knapsack(0, Weight) = 0;
	}

	for (int CurItemIndex = 0; CurItemIndex < NumItems; CurItemIndex++)
	{
		int MRow = CurItemIndex + 1;
		int ItemWeight = ItemCosts[CurItemIndex].Val / StepSize.Val;
		for (int Weight = 0; Weight <= MaxWeight; Weight++)
		{
			int MCol = Weight;
			if(ItemWeight <= Weight)
			{
				Knapsack(MRow, MCol) = max(Knapsack(MRow-1, MCol), ItemCosts[CurItemIndex] + Knapsack(MRow-1, MCol - ItemWeight));
				Currency Debug = Knapsack(MRow, MCol);
				Currency debugTest = 1.0 + Debug;
			}
			else
			{
				Knapsack(MRow, MCol) = Knapsack(MRow-1, MCol);
			}
		}
		RetVal = Knapsack(MRow, MaxWeight);// * StepSize;
		if (RetVal == AvailablePoints)
		{
			break;
		}
	}

	return RetVal;
}
// ---------------------------------------------------------------------------
CurrencyMatrix::CurrencyMatrix(unsigned Rows, unsigned Cols) : NumRows (Rows), NumCols (Cols)	//Throws length_error exception
{
	if (Rows == 0 || Cols == 0)
	{
		throw std::length_error("CurrencyMatrix::CurrencyMatrix - Constructor has 0 size");
	}
	DataArray = new Currency[(Rows * Cols)];
}
// ---------------------------------------------------------------------------
CurrencyMatrix::~CurrencyMatrix()
{
	delete[] DataArray;
}
// ---------------------------------------------------------------------------
CurrencyMatrix::CurrencyMatrix(const CurrencyMatrix &OtherMatrix) : NumRows (OtherMatrix.Rows), NumCols (OtherMatrix.Cols)
{
	DataArray = new Currency[OtherMatrix.Rows * OtherMatrix.Cols];
	for (int i = 0; i < OtherMatrix.Rows * OtherMatrix.Cols; i++)
	{
		DataArray[i] = OtherMatrix.DataArray[i];
	}
}
// ---------------------------------------------------------------------------
CurrencyMatrix& CurrencyMatrix::operator= (const CurrencyMatrix &OtherMatrix)
{
	// Check for self-assignment
	if (this == &OtherMatrix)
	{
		return *this;
	}

	delete[] DataArray;

	NumRows = OtherMatrix.Rows;
	NumCols = OtherMatrix.Cols;

	for (int i = 0; i < OtherMatrix.Rows * OtherMatrix.Cols; i++)
	{
		DataArray[i] = OtherMatrix.DataArray[i];
	}
}
// ---------------------------------------------------------------------------
Currency& CurrencyMatrix::operator() (unsigned Row, unsigned Col)	//Throws out_of_range exception
{
	if (Row >= Rows || Col >= Cols)
	{
		throw std::out_of_range("CurrencyMatrix::operator() - Matrix subscript out of bounds");
	}
	return DataArray[((Cols*Row) + Col)];
}
// ---------------------------------------------------------------------------
Currency  CurrencyMatrix::operator() (unsigned Row, unsigned Col) const		//Throws out_of_range exception
{
	if (Row >= Rows || Col >= Cols)
	{
		throw std::out_of_range("CurrencyMatrix::operator() - Matrix subscript out of bounds");
	}
	return DataArray[((Cols*Row) + Col)];
}
// ---------------------------------------------------------------------------
Currency TfrmPaymentType::GetAvailableRedeemPoints(TPaymentTransaction PointsTransaction)
{

  Currency points = 0;
  bool isAllowEarntRedemption = CurrentTransaction.Membership.Member.MemberType == 1 ?
                               TDBTierLevel::IsAllowEarnedRedemption(CurrentTransaction.DBTransaction,
                                 CurrentTransaction.Membership.Member.TierLevel) : true ;

  if((TGlobalSettings::Instance().UseTierLevels && isAllowEarntRedemption) || !TGlobalSettings::Instance().UseTierLevels)
  {
     if (TGlobalSettings::Instance().EnableSeperateEarntPts)
        {
           points = PointsTransaction.Membership.Member.Points.getPointsBalance(pasDatabase,ptstAccount);
        }
        else
        {
            points = PointsTransaction.Membership.Member.Points.getPointsBalance(pasDatabase);
        }
  }
  else
  {
     if(TGlobalSettings::Instance().LoyaltyMateEnabled &&  !TGlobalSettings::Instance().IsPOSOffline)
     {
        points =  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableBDPoint +
                  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableFVPoint;
     }
     else
     {
         points = PointsTransaction.Membership.Member.Points.getBirthDayRewardPoints() +
                  PointsTransaction.Membership.Member.Points.getBirthDayRewardRedeemedPoints();

         points += ( PointsTransaction.Membership.Member.Points.getFirstVisitPoints() +
                 PointsTransaction.Membership.Member.Points.getFirstVisitRedeemedPoints());
     }
  }
  return points;
}
// ---------------------------------------------------------------------------
Currency TfrmPaymentType::GetTotalWeightInOrder(TList *Orders)
{
    Currency TotalWeight = 0;
    for (int i = 0; i < Orders->Count; i++)
     {
        TItemComplete *Order = (TItemComplete*)Orders->Items[i];
        if(Order->WeightedPrice.WeightedItem)
         {
            TotalWeight += Order->WeightedPrice.Weight.GetWeight()*1000;
         }
     }
     return TotalWeight;
}
// ---------------------------------------------------------------------------
Currency TfrmPaymentType::CalculatePointsForWeight(TList *Orders)
{
   Currency PointValue = 0;
   TTierLevel *TierLevel = new TTierLevel;
   TDBTierLevel::GetTierLevelOfMember(CurrentTransaction.DBTransaction,CurrentTransaction.Membership.Member);
   TierLevel->Level = CurrentTransaction.Membership.Member.TierLevel;
   TDBTierLevel::GetTierLevelInformationByTierLevel(CurrentTransaction.DBTransaction,TierLevel);
   Currency ratio = ((float)TierLevel->WeighedRedemPoint)/((float)TierLevel->WeighedRedemWeight);
   if(IsWrkPayAmountChanged)
   {
      PointValue =  wrkPayAmount * ratio ;
   }
   else
   {
      PointValue = GetTotalWeightInOrder(Orders)  *  ratio;
   }

   return PointValue;
}
// ---------------------------------------------------------------------------
Currency TfrmPaymentType::CalculateWeightForPoints(Currency Points)
{
   Currency WeightValue = 0;
   TTierLevel *TierLevel = new TTierLevel;
   TDBTierLevel::GetTierLevelOfMember(CurrentTransaction.DBTransaction,CurrentTransaction.Membership.Member);
   TierLevel->Level = CurrentTransaction.Membership.Member.TierLevel;
   TDBTierLevel::GetTierLevelInformationByTierLevel(CurrentTransaction.DBTransaction,TierLevel);
   Currency ratio = ((float)TierLevel->WeighedRedemWeight )/((float)TierLevel->WeighedRedemPoint);
   WeightValue =  Points * ratio ;
   return WeightValue;
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::disableButtonIfPermissionNotAllowed(
                                    Database::TDBTransaction &DBTransaction,
                                    TGridButton *button,
                                    TMMContactInfo &userInfo,
                                    SecurityCheck Permission )
{
    std::auto_ptr<TContactStaff> staff(new TContactStaff(DBTransaction));
     button->Enabled = staff->TestAccessLevel(userInfo, Permission);
    AllowRefund= button->Enabled;
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::CalculateRedeemedPoints(TList *Orders,TRedeemPointsInformation *RedeemPointsInformation,
                                                Currency MaxAmountToRedeem, bool isPaybyWeight,bool isGiftCardPayment)
{
    Currency AmountRedeemed = 0;
    Currency PointRedeemed = 0;
    RedeemPointsInformation->TotalPoints = 0;
    RedeemPointsInformation->TotalValue = 0;
    TTierLevel *TierLevel = new TTierLevel;
    TDBTierLevel::GetTierLevelOfMember(CurrentTransaction.DBTransaction,CurrentTransaction.Membership.Member);
    TierLevel->Level = CurrentTransaction.Membership.Member.TierLevel;
    TDBTierLevel::GetTierLevelInformationByTierLevel(CurrentTransaction.DBTransaction,TierLevel);
    for (int i = 0; i < Orders->Count; i++)
     {
       TItemComplete *Order = (TItemComplete*)Orders->Items[i];
       if(RedeemPointsInformation->RemainingPoints > 0 && MaxAmountToRedeem > RedeemPointsInformation->TotalValue)
        {
            if(Order->WeightedPrice.WeightedItem && (isPaybyWeight ||(!isPaybyWeight && !TGlobalSettings::Instance().AllowPointPaymentByWeight)))
             {
               CalculatePointsWeighedItem(Order, TierLevel ,RedeemPointsInformation,MaxAmountToRedeem);
             }
            else if(!isPaybyWeight && !Order->WeightedPrice.WeightedItem)
             {
               CalculatePointsNonWeighedItem(Order, TierLevel ,RedeemPointsInformation,MaxAmountToRedeem);
             }
         }
         if(!isGiftCardPayment)
         {
             Order->AmountRedeemed = RedeemPointsInformation->TotalValue - AmountRedeemed;
             Order->PointRedeemed = RedeemPointsInformation->TotalPoints - PointRedeemed;
         }
         AmountRedeemed =  RedeemPointsInformation->TotalValue;
         PointRedeemed =   RedeemPointsInformation->TotalPoints;
         for(int j = 0; j < Order->SubOrders->Count;j++)
          {
             TItemComplete *SubOrder = (TItemComplete*)Order->SubOrders->Items[j];
             if(RedeemPointsInformation->RemainingPoints > 0 && MaxAmountToRedeem > RedeemPointsInformation->TotalValue)
                {
                    if(SubOrder->WeightedPrice.WeightedItem && (isPaybyWeight ||(!isPaybyWeight && !TGlobalSettings::Instance().AllowPointPaymentByWeight)))
                     {
                       CalculatePointsWeighedItem(SubOrder, TierLevel ,RedeemPointsInformation,MaxAmountToRedeem);
                     }
                    else if(!isPaybyWeight && !SubOrder->WeightedPrice.WeightedItem)
                     {
                       CalculatePointsNonWeighedItem(SubOrder, TierLevel ,RedeemPointsInformation,MaxAmountToRedeem);
                     }
                 }
                 if(!isGiftCardPayment)
                 {
                     SubOrder->AmountRedeemed = RedeemPointsInformation->TotalValue - AmountRedeemed;
                     SubOrder->PointRedeemed = RedeemPointsInformation->TotalPoints - PointRedeemed;
                 }
                 AmountRedeemed =  RedeemPointsInformation->TotalValue;
                 PointRedeemed =   RedeemPointsInformation->TotalPoints;
          }
     }
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::CalculatePointsWeighedItem(TItemComplete *Order,TTierLevel *TierLevel,TRedeemPointsInformation *RedeemPointsInformation,
Currency MaxAmountToRedeem)
{
    Currency pricePerGram = Order->BillCalcResult.FinalPrice/(Order->WeightedPrice.Weight.GetWeight()*1000);
    CalculatePoints(Order->WeightedPrice.Weight.GetWeight()*1000,pricePerGram,TierLevel,MaxAmountToRedeem,RedeemPointsInformation,true);
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::CalculatePointsNonWeighedItem(TItemComplete *Order,TTierLevel *TierLevel,TRedeemPointsInformation *RedeemPointsInformation,
Currency MaxAmountToRedeem)
{
    Currency ItemCost = Order->BillCalcResult.FinalPrice/Order->GetQty();
    CalculatePoints(Order->GetQty(),ItemCost,TierLevel,MaxAmountToRedeem,RedeemPointsInformation,false);

}
// ---------------------------------------------------------------------------
void TfrmPaymentType::CalculatePoints(Currency Qty, Currency Cost,TTierLevel *TierLevel, Currency MaxValue,
TRedeemPointsInformation *RedeemPointsInformation,bool IsWeighted)
{
	Currency requiredPointsValue = Qty * Cost;
	Currency requiredPoints = 0;
	if(IsWeighted)
	{
		requiredPoints = (Qty * (float)TierLevel->WeighedRedemPoint) /(float)TierLevel->WeighedRedemWeight;
	}
	else
	{
		requiredPoints =   requiredPointsValue * TierLevel->PointRedemRate;
	}
	if(RedeemPointsInformation->RemainingPoints > requiredPoints)
	{
		if(MaxValue - RedeemPointsInformation->TotalValue >= requiredPointsValue)
		{
			RedeemPointsInformation->RemainingPoints -= requiredPoints;
			RedeemPointsInformation->TotalPoints += requiredPoints;
			RedeemPointsInformation->TotalValue +=  requiredPointsValue;
		}
		else
		{
			Currency RemainingValue = MaxValue - RedeemPointsInformation->TotalValue;
			Currency RemainingValuePoint = IsWeighted ?GetPointsForPrice(RemainingValue,Cost,TierLevel) : RemainingValue * TierLevel->PointRedemRate ;
			RedeemPointsInformation->RemainingPoints -= RemainingValuePoint;
			RedeemPointsInformation->TotalPoints += RemainingValuePoint;
			RedeemPointsInformation->TotalValue +=  RemainingValue;
		}
	}
	else
	{
		requiredPoints = RedeemPointsInformation->RemainingPoints;
		if(IsWeighted)
		{
			requiredPointsValue = ((RedeemPointsInformation->RemainingPoints * (float)TierLevel->WeighedRedemWeight)/(float)TierLevel->WeighedRedemPoint)* Cost;
		}
		else
		{
			requiredPointsValue = RedeemPointsInformation->RemainingPoints/TierLevel->PointRedemRate;
		}

		if(MaxValue - RedeemPointsInformation->TotalValue >= requiredPointsValue)
		{
			RedeemPointsInformation->RemainingPoints -= requiredPoints;
			RedeemPointsInformation->TotalPoints += requiredPoints;
			RedeemPointsInformation->TotalValue +=  requiredPointsValue;
		}
		else
		{
			Currency RemainingValue = MaxValue - RedeemPointsInformation->TotalValue;
			Currency RemainingValuePoint = IsWeighted ? GetPointsForPrice(RemainingValue,Cost,TierLevel) : RemainingValue * TierLevel->PointRedemRate ;
			RedeemPointsInformation->RemainingPoints -= RemainingValuePoint;
			RedeemPointsInformation->TotalPoints += RemainingValuePoint;
			RedeemPointsInformation->TotalValue +=  RemainingValue;
		}
	}

}
// ---------------------------------------------------------------------------
Currency TfrmPaymentType::GetPointsForPrice(Currency value, Currency Cost,TTierLevel *TierLevel)
{
	Currency weight = value/Cost;
	return (weight*(float)TierLevel->WeighedRedemPoint) /(float)TierLevel->WeighedRedemWeight;
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ShowFormToSelectItems(TPayment *Payment)
{
    std::auto_ptr<TfrmComboForm>frmComboForm(TfrmComboForm::Create<TfrmComboForm>(this));
    frmComboForm->Caption = " Select a Item";
    frmComboForm->OrdersList = CurrentTransaction.Orders;
    frmComboForm->MemberCurrentPoints = GetAvailableRedeemPoints(CurrentTransaction);//CurrentTransaction.Membership.Member.Points.getPointsBalance();
    frmComboForm->MemberName = CurrentTransaction.Membership.Member.Name;
    if(CheckItemHasPoints())
    {
        if(frmComboForm->ShowModal() == mrOk)
        {
            if(frmComboForm->SelectedPoints > 0)
            {
                Currency selectedPoints = frmComboForm->SelectedPoints;
                Currency selectedPointsValue = frmComboForm->PriceSelected;
                IsWrkPayAmountChanged = true;
                if(TGlobalSettings::Instance().PontsSpentCountedAsRevenue)
                {
                  ApplyMemberPointsDiscount(selectedPoints,selectedPointsValue);

                }
                else if(ProcessMemberPointOnItemBasis(Payment,selectedPoints,selectedPointsValue))
                {
                    CurrentTransaction.RedeemPointsInformation->TotalValue = selectedPointsValue;
                    CurrentTransaction.RedeemPointsInformation->TotalPoints = selectedPoints;
                    CurrentTransaction.ProcessPoints();
                }
            }
        }
    }
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ApplyMemberPointsDiscount(Currency selectedPoints,Currency selectedPointsValue,bool includeAll)
{
   std::auto_ptr<TList> Orders(new TList);
   for(int i = 0; i < CurrentTransaction.Orders->Count; i++)
    {
        TItemComplete *item = (TItemComplete *)CurrentTransaction.Orders->Items[i];
        item->DiscountByTypeRemove(dsMMMebersPoints);
        if(item->IsPayByPoints || includeAll)
        {
          Orders->Add(item);
        }
    }
    TDiscount CurrentDiscount;
    CurrentDiscount.DiscountKey = -2;
    ManagerDiscount->GetDiscount(CurrentTransaction.DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);
    CurrentDiscount.MaximumValue = selectedPointsValue;
    CurrentDiscount.Amount = selectedPointsValue;
    ManagerDiscount->AddDiscount(Orders.get(),CurrentDiscount);
    CurrentTransaction.RedeemPointsInformation->TotalValue = selectedPointsValue;
    CurrentTransaction.RedeemPointsInformation->TotalPoints = selectedPoints;
    TPointsTypePair Pair(pttRedeemed,ptstLoyalty);
    TPointsType Type(pasDiscount,Pair,pesNone);
    CurrentTransaction.Membership.Member.Points.ClearBySource(pasDiscount);
    CurrentTransaction.Membership.Member.Points.Load(Type, selectedPoints);
    CurrentTransaction.IgnoreLoyaltyKey = false;
    CurrentTransaction.Recalc();
}
// ---------------------------------------------------------------------------
bool TfrmPaymentType::CheckItemHasPoints()
{
    bool retVal = false;

    for(int i = 0; i < CurrentTransaction.Orders->Count; i++)
    {
        TItemComplete *order = (TItemComplete *)CurrentTransaction.Orders->Items[i];
        if(order->ItemPriceForPointsOriginal > 0)
        {
           retVal = true;
           break;
        }

        for(int j = 0; j < order->SubOrders->Count; j++)
        {
            TItemCompleteSub *SubOrder = (TItemCompleteSub*)order->SubOrders->Items[j];
            if(SubOrder->ItemPriceForPointsOriginal > 0)
            {
                retVal = true;
                break;
            }
        }
    }
    return retVal;
}
// ---------------------------------------------------------------------------
bool  TfrmPaymentType::ProcessMemberPointOnItemBasis(TPayment *Payment,Currency selectedPoints,Currency selectedPointsValue)
{
    if((TDeviceRealTerminal::Instance().ManagerMembership->Authorise(CurrentTransaction.Membership.Member,selectedPointsValue) == lsAccepted))
    {
         Payment->SetPay(selectedPointsValue);
         return true;
    }
    return false;
}
// ---------------------------------------------------------------------------
bool TfrmPaymentType::CheckOnlinePaidOrNot()
{
    bool retval = false;
    if(CurrentTransaction.WebOrderKey > 0)
    {
        std::auto_ptr<TStringList>webPaymentDetials(new TStringList);
        TDBWebUtil::getWebOrderExtraData(CurrentTransaction.DBTransaction, CurrentTransaction.WebOrderKey, "PAYMENT_INFO", webPaymentDetials.get());
        if(webPaymentDetials->Count > 0)
        {
           AnsiString payment_type = webPaymentDetials->Strings[0];
           if(payment_type == "PAID")
           {
               retval = true;
           }
        }
    }
    return retval;
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ShowWebOrderMembersPayment()
{
	CurrentTransaction.Money.Recalc(CurrentTransaction);

	int ButtonPos = 0;

	for (int i = 0; i < 1; i++)
	{
		TPayment *Payment = CurrentTransaction.PaymentGet(i);

		if (Payment->Visible && !Payment->RefundPoints)
		{
			if (Payment->GetPay() == 0)
			{
				if (Payment->GetAdjustment() != 0 && !Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails) &&
                    !Payment->GetPaymentAttribute(ePayTypePoints))
				{
					tgPayments->Buttons[ButtonPos][PAYCOL]->Caption = Payment->Name + "\r" + CurrToStrF(Payment->GetAdjustment(), ffNumber,
					CurrencyDecimals);
				}
				else
				{
					tgPayments->Buttons[ButtonPos][PAYCOL]->Caption = Payment->Name;
				}
			}
            else
            {
                tgPayments->Buttons[ButtonPos][ALTCOL]->Visible = false;
            }
            //MM 4775
                if(TGlobalSettings::Instance().PointOnly)
                {
                    tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = false;
                    tgPayments->Buttons[ButtonPos][PAYCOL]->Enabled = false;
                }
                else
                {
                  if(!AllowRefund)
                  {
                       if(tgPayments->Buttons[ButtonPos][ALTCOL]->Caption != "Refund Points")
                        tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = true;
                       else
                       tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = false;
                  }
                  else
                  {
                        tgPayments->Buttons[ButtonPos][ALTCOL]->Enabled = true;
                  }
                }
			//ButtonPos++;
		}
	}

	stBalance->Caption = CurrToStrF(CurrentTransaction.Money.RoundedPaymentDue, ffNumber, CurrencyDecimals);
	stChange->Caption = CurrToStrF(CurrentTransaction.Money.RoundedChange, ffNumber, CurrencyDecimals);
	stDiscount->Caption = CurrToStrF(CurrentTransaction.Money.RoundedTotalAdjustment, ffNumber, CurrencyDecimals);

	if (TDeviceRealTerminal::Instance().PaymentSystem->ForceTender)
	{
		wrkPayAmount = 0; // Forces them to enter a value.
	}
	else
	{
		if (CurrentTransaction.CreditTransaction)
        {
			wrkPayAmount = -CurrentTransaction.Money.PaymentDue;
		}
        else
        {
		 	wrkPayAmount = CurrentTransaction.Money.PaymentDue;
		}
	}
	ndWorkingAmount->SetNumeric(RoundToNearest(wrkPayAmount, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
    CurrentTransaction.ProcessPoints();
	ShowReceipt();

	if (CurrentTransaction.Membership.Member.ContactKey != 0)
	{
		lbeMembership->Visible = true;
        lbeMembership->Caption = CurrentTransaction.Membership.Member.Name + " (" + CurrentTransaction.Membership.Member.MembershipNumber +
		")" + " Points:" + FormatFloat("0.00", CurrentTransaction.Membership.Member.Points.getPointsBalance());
        memberNumber = CurrentTransaction.Membership.Member.MembershipNumber;
	}
	else
	{
		lbeMembership->Visible = false;
		lbeMembership->Caption = "";
	}

	if (CurrentTransaction.CreditTransaction)
	{
		lbeMembership->Color = clRed;
		if (lbeMembership->Visible != true)
		{
			lbeMembership->Visible = true;
			lbeMembership->Caption = "Refund Transaction";
		}
	}
	else
	{
		lbeMembership->Color = clGreen;
	}

	if (CurrentTransaction.Money.TotalAdjustment == 0)
	{
		tbtnDiscountSurcharge->Caption = "Discount / Surcharge";
		lbeDiscountSurcharge->Caption = "Adjustment";
	}
	else if (CurrentTransaction.Money.TotalAdjustment < 0)
	{
		tbtnDiscountSurcharge->Caption = "Discount\r" + CurrToStrF(CurrentTransaction.Money.TotalAdjustment, ffNumber, CurrencyDecimals);
		lbeDiscountSurcharge->Caption = "Discount";
	}
	else
	{
		lbeDiscountSurcharge->Caption = "Surcharge";
		tbtnDiscountSurcharge->Caption = "Surcharge\r" + CurrToStrF(CurrentTransaction.Money.TotalAdjustment, ffNumber, CurrencyDecimals);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::CardSwipe(Messages::TMessage& Message)
{
    index_Thor = false;
    IsVoucher = false;
    TGlobalSettings::Instance().IsThorPay = false;
    TGlobalSettings::Instance().IsDiscountSelected = false;

	TMMContactInfo TempUserInfo;
	AnsiString Data = *((AnsiString*)Message.WParam);
	TempUserInfo.CardStr = Data.SubString(1, 80);
	TempUserInfo.ProxStr = Data.SubString(1, 80);

	if (ManagerDiscount->GetDiscount(CurrentTransaction.DBTransaction, Data.SubString(1, 80)) != 0)
	{
		ApplyDiscount(ManagerDiscount->GetDiscount(CurrentTransaction.DBTransaction, Data.SubString(1, 80)),
		TDeviceRealTerminal::Instance().User.ContactKey);
	}
    else if(TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"])
    {
        if(TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate
           && TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
           GetMemberByBarcode(CurrentTransaction.DBTransaction,Data);
        }
        else if (TDeviceRealTerminal::Instance().ManagerMembership->IsCard(CurrentTransaction.DBTransaction, TempUserInfo) == lsAccepted || Data.Pos("MMPROX") != 0)
        {
            TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->FindMember(CurrentTransaction.DBTransaction, TempUserInfo);

            if (Result == lsAccepted)
            {
                ApplyMembership(TempUserInfo);
            }
            else if (Result == lsAccountBlocked)
            {
                MessageBox("Account Blocked " + TempUserInfo.Name + " " + TempUserInfo.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
            }
        }
    }
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::GetMemberByBarcode(Database::TDBTransaction &DBTransaction,AnsiString Barcode)
{
 	TDeviceRealTerminal &drt = TDeviceRealTerminal::Instance();
	TMMContactInfo info;
    bool memberExist = drt.ManagerMembership->LoyaltyMemberSelected(DBTransaction,info,Barcode,true);

	if (info.Valid())
     {
        TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
        ManagerLoyaltyVoucher.DisplayMemberVouchers(DBTransaction,info);
		ApplyMembership(info);
	}

}
void __fastcall TfrmPaymentType::lbeDiscountClick(TObject *Sender)
{
    TGlobalSettings::Instance().IsDiscountSelected = true;
	TMMContactInfo TempUserInfo;
	TempUserInfo = TDeviceRealTerminal::Instance().User;
	bool AllowDiscount = false;
	AnsiString DiscountMenu = "";
    if((CurrentTransaction.Membership.Member.ContactKey != 0) && TPaySubsUtility::IsLocalLoyalty() &&
    !CurrentTransaction.Membership.Member.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
    {
        MessageBox("Discounts are disabled for this Member.", "INFORMATION", MB_OK + MB_ICONINFORMATION);
        return;
    }
	// Secutriy access changes fix when Implementing full security access levbels.
	// If not, prompt for a login.
	if (!AllowDiscount)
	{
		TempUserInfo.Clear();
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(CurrentTransaction.DBTransaction));
		TLoginSuccess Result = Staff->Login(this, CurrentTransaction.DBTransaction, TempUserInfo, CheckDiscountBill);
		if (Result == lsAccepted)
		{
			AllowDiscount = true;
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access rights to Discounts / Surcharges.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}
	}

	if (AllowDiscount)
	{
		TDiscount Discount;

		std::auto_ptr <TfrmMessage> frmMessage(TfrmMessage::Create <TfrmMessage> (this, TDeviceRealTerminal::Instance().DBControl));
		frmMessage->MessageType = eDiscountReason;

		if (frmMessage->ShowModal() == mrOk)
		{
			if (frmMessage->Key == -1)
			{
				CurrentTransaction.Discounts.clear();
				TDeviceRealTerminal::Instance().PaymentSystem->Security->SecurityDelete
				(TDeviceRealTerminal::Instance().PaymentSystem->Security->SecurityGetType(secDiscountedBy));
				ManagerDiscount->ClearDiscounts(CurrentTransaction.Orders);
                CurrentTransaction.IgnoreLoyaltyKey = false;
				CurrentTransaction.Recalc();
				tbtnDiscountSurcharge->Caption = "Discount / Surcharge";
				CurrentTransaction.ProcessPoints();
				ShowPaymentTotals();
			}
			else
			{
				TDiscount CurrentDiscount;
				ManagerDiscount->GetDiscount(CurrentTransaction.DBTransaction, frmMessage->Key, CurrentDiscount);
				if(CurrentDiscount.IsComplimentaryDiscount())
				{
					CurrentTransaction.TypeOfSale = ComplimentarySale;
				}
				else if(CurrentDiscount.IsNonChargableDiscount())
				{
					CurrentTransaction.TypeOfSale = NonChargableSale;
				}
                // check if the same discount is present as Thor in order
				ApplyDiscount(frmMessage->Key, TempUserInfo.ContactKey);
				if(CurrentDiscount.IsNonChargableDiscount() || CurrentDiscount.IsComplimentaryDiscount())
				{
                      if(CheckDiscountPointsPaytype)               //MM-3908
                      {
                          CheckDiscountPointsPaytype = false;      //MM-3908
                      }
                      else
                      {
                        TPayment *Payment = CurrentTransaction.PaymentGet(0);              //MM-3908
                        BtnPayment(Payment);
                      }
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::RemoveMembership()
{
	if (CurrentTransaction.Orders != NULL)
	{
		CurrentTransaction.RemoveMembership();
		TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(CurrentTransaction);
		Reset(); // clears all PaymentTypes and reloads them.
		ShowPaymentTotals();
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ApplyMembership(TMMContactInfo &Member)
{
	eMemberSource MemberSource;
	TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(CurrentTransaction.DBTransaction, Member,
	MemberSource);
	if (Result == lsAccountBlocked)
	{
		MessageBox("Account Blocked " + Member.Name + " " + Member.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
	}
	else if (Result == lsAccepted)
	{
		// Remove the old Member if any.
        //RemoveMembership();
        CurrentTransaction.RemoveMembership();
        if(TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
           TManagerDiscount managerDiscount;
           managerDiscount.GetMembershipDiscounts(CurrentTransaction.DBTransaction,Member.AutoAppliedDiscounts);
        }
		CurrentTransaction.ApplyMembership(Member, MemberSource);
		if (CurrentTransaction.Orders != NULL)
		{
            if(TPaySubsUtility::IsLocalLoyalty() &&
               !CurrentTransaction.Membership.Member.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
               ManagerDiscount->ClearDiscounts(CurrentTransaction.Orders);
            ManagerDiscount->ClearMemberExemtDiscounts(CurrentTransaction.Orders);
			TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(CurrentTransaction);
//            for(int i = 0; i < PaymentTransaction.PaymentList->Count; i++)
//            {
//               MessageBox((PaymentTransaction.PaymentGet(i))->Name,"Payment Names",MB_OK);
//            }
			Reset(); // Reloads the Buttons on the screen.
            CurrentTransaction.IgnoreLoyaltyKey = false;
			CurrentTransaction.Recalc();
			ShowPaymentTotals();
		}
		TMembership *membershipSystem = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentType::ApplyDiscount(int DiscountKey, int ContactKey, TDiscountSource DiscountSource)
{
	bool ProcessDiscount = true;
	TDiscount CurrentDiscount;
	TSCDPWDChecker SCDChecker;
	bool bailout = false;
	CurrentDiscount.DiscountKey = DiscountKey;
	ManagerDiscount->GetDiscount(CurrentTransaction.DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);
    std::auto_ptr<TSCDPatronUtility> patronUtility(new TSCDPatronUtility());
    if(patronUtility->CanByPassSCDValidity(CurrentTransaction.Orders,CurrentTransaction.Patrons,CurrentDiscount))
    {
        ProcessDiscount = SCDChecker.PWDCheck(CurrentDiscount, CurrentTransaction.Orders);
    }
    else
    {
        ProcessDiscount = SCDChecker.SeniorCitizensCheck(CurrentDiscount, CurrentTransaction.Orders) &&
                      SCDChecker.PWDCheck(CurrentDiscount, CurrentTransaction.Orders);
    }
    if(DiscountSource == dsMMMembership)
    {
       CurrentDiscount.IsThorBill = false;
    }
    CurrentDiscount.Source = DiscountSource;
	if(CurrentDiscount.Source == dsMMMebersPoints)
	{
		if(CurrentTransaction.Membership.Applied())
		{
            CurrentTransaction.IgnoreLoyaltyKey = false;
			CurrentTransaction.Recalc();
			Currency ProductValue = CurrentTransaction.Money.PaymentDue;
			CurrentDiscount.MaximumValue = CurrentTransaction.Membership.Member.Points.getPointsBalance(pasDatabase,ptstLoyalty);
			CurrentDiscount.Amount = ProductValue;
		}
	}

	if (ProcessDiscount && (CurrentDiscount.Type == dtPromptDescription || CurrentDiscount.Type == dtPromptDescriptionAmount))
	{
		std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
		do
		{
			frmTouchKeyboard->MaxLength = 200;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = true;
			frmTouchKeyboard->MustHaveValue = true;
			frmTouchKeyboard->KeyboardText = CurrentDiscount.Description;
			frmTouchKeyboard->Caption = "Please enter a discount / surcharge Description";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				CurrentDiscount.Description = frmTouchKeyboard->KeyboardText;
			}
			else
			{
				bailout = true;
                CheckDiscountPointsPaytype = true;      //MM-3908
			}
		}
		while (frmTouchKeyboard->KeyboardText == "" && !bailout);

		if (bailout)
		{
			ProcessDiscount = false;
		}
	}

	if (ProcessDiscount && (CurrentDiscount.Type == dtPromptAmount || CurrentDiscount.Type == dtPromptDescriptionAmount))
	{
		std::auto_ptr <TfrmDiscount> frmDiscount(TfrmDiscount::Create <TfrmDiscount> (this));
		frmDiscount->Mode = CurrentDiscount.Mode;
		frmDiscount->CURInitial = CurrentDiscount.Amount;
		frmDiscount->PERCInitial = CurrentDiscount.PercentAmount;

		// Partial Payments.
		frmDiscount->TotalValue = CurrentTransaction.Money.GrandTotal;

		if (frmDiscount->ShowModal() == mrOk)
		{
			CurrentDiscount.Mode = frmDiscount->Mode;
			if (frmDiscount->Mode == DiscModeCurrency || frmDiscount->Mode == DiscModePoints)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
				TGlobalSettings::Instance().MidPointRoundsDown);
                CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;
				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
			else if (frmDiscount->Mode == DiscModeSetPrice)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
				TGlobalSettings::Instance().MidPointRoundsDown);
                CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;
				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
            else if (frmDiscount->Mode == DiscModeItem)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
				TGlobalSettings::Instance().MidPointRoundsDown);
                CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;
				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
            //add changes for open discount combo...
            else if (frmDiscount->Mode == DiscModeCombo)
            {
                CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
                CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;
                if (CurrentDiscount.Amount != frmDiscount->CURResult)
                {
                   MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
                }
            }
			else
			{
				CurrentDiscount.PercentAmount = frmDiscount->PERCResult;
                if(frmDiscount->Mode == DiscModePercent)
                {
                   CurrentDiscount.OriginalAmount = CurrentDiscount.PercentAmount;
                }
			}
		}
		else
		{
			ProcessDiscount = false;
		}
	}

	if (ProcessDiscount)
	{
         CurrentDiscount.DiscountAppliedTime = Now();
         CurrentTransaction.DiscountReason = CurrentDiscount.Description;
		 CurrentTransaction.Discounts.clear();
         ManagerDiscount->ClearDiscount(CurrentTransaction.Orders, CurrentDiscount);
         // Check if Discount application needs restructure of bill on patron basis
         ApplyDiscount(CurrentDiscount);
//         ManagerDiscount->AddDiscount(CurrentTransaction.Orders, CurrentDiscount);
        if( CurrentDiscount.Source == dsMMMebersPoints && CurrentTransaction.Membership.Member.ContactKey != 0)
            {
                TPointsTypePair Pair(pttRedeemed,ptstLoyalty);
                TPointsType Type(pasDiscount,Pair,pesNone);
                CurrentTransaction.Membership.Member.Points.ClearBySource(pasDiscount);
                CurrentTransaction.Membership.Member.Points.Load(Type, CurrentDiscount.Amount);
            }
        CurrentTransaction.IgnoreLoyaltyKey = false;
        CurrentTransaction.Recalc();
		TDeviceRealTerminal::Instance().PaymentSystem->Security->SecurityDelete
		(TDeviceRealTerminal::Instance().PaymentSystem->Security->SecurityGetType(secDiscountedBy));

		// Add Security.
		TSecurityReference *SecRef = new TSecurityReference;
		SecRef->UserKey = ContactKey;
		SecRef->Event = SecurityTypes[secDiscountedBy];
		SecRef->From = CurrentDiscount.Name;
		SecRef->To = CurrToStrF(CurrentTransaction.TotalAdjustment, ffNumber, 2);
		SecRef->Note = CurrentDiscount.Description;
		SecRef->TimeStamp = Now();
		TDeviceRealTerminal::Instance().PaymentSystem->Security->SecurityAdd(SecRef);
		ShowPaymentTotals(true);
	}
}
//----------------------------------------------------------------------------
void TfrmPaymentType::ApplyDiscount(TDiscount discount)
{
    std::auto_ptr<TSCDPatronUtility> patronUtility(new TSCDPatronUtility());
    if(patronUtility->CheckPatronsAvailable(CurrentTransaction))
    {
        std::auto_ptr <TList> SCDOrders(new TList);
        SCDOrders->Clear();
        std::auto_ptr <TList> NormalOrders(new TList);
        NormalOrders->Clear();

        patronUtility->DivideBill(CurrentTransaction,SCDOrders,NormalOrders);

        if(discount.IsSeniorCitizensDiscount())
           ManagerDiscount->AddDiscount(SCDOrders.get(), discount);
        else
           ManagerDiscount->AddDiscount(NormalOrders.get(), discount);

        CurrentTransaction.Orders->Clear();
        for(int indexSCD = 0; indexSCD < SCDOrders->Count; indexSCD++)
        {
            TItemComplete *Order = (TItemComplete *)SCDOrders->Items[indexSCD];
            CurrentTransaction.Orders->Add(Order);
        }
        for(int indexNormal = 0; indexNormal < NormalOrders->Count; indexNormal++)
        {
            TItemComplete *Order = (TItemComplete *)NormalOrders->Items[indexNormal];
            CurrentTransaction.Orders->Add(Order);
        }
        CurrentTransaction.Money.Recalc(CurrentTransaction);

    }
    else
    {
        ManagerDiscount->AddDiscount(CurrentTransaction.Orders, discount);
    }

}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ApplyAccount(TMMContactInfo &Member)
{
	eMemberSource MemberSource;
	TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(CurrentTransaction.DBTransaction, Member,
	MemberSource);
	if (Result == lsAccountBlocked)
	{
		MessageBox("Account Blocked " + Member.Name + " " + Member.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
	}
	else if (Result == lsAccepted)
	{
		CurrentTransaction.Account.Assign(Member, MemberSource);
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::ClearAccount()
{
	CurrentTransaction.Account.Clear();
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::OnSmartCardInserted(TSystemEvents *Sender)
{
	TMMContactInfo TempUserInfo;
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->GetContactInfo(TempUserInfo);
	if (TempUserInfo.Valid())
	{
        TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
        ManagerLoyaltyVoucher.DisplayMemberVouchers(CurrentTransaction.DBTransaction,TempUserInfo);
		ApplyMembership(TempUserInfo);
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::OnSmartCardRemoved(TSystemEvents *Sender)
{
	if (TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"])
	{
		RemoveMembership();
		TDeviceRealTerminal::Instance().ManagerMembership->EndMemberTransaction();
	}
}
// ---------------------------------------------------------------------------
void TfrmPaymentType::makeLogFile(UnicodeString str)
{
    AnsiString fileName = ExtractFilePath(Application->ExeName) + "EFTPOSCashDrawer_Logs.txt" ;
    std::auto_ptr<TStringList> List(new TStringList);
    if (FileExists(fileName) )
    {
      List->LoadFromFile(fileName);
    }
    List->Add(" "+ str +  "\n");
    List->SaveToFile(fileName );
}
// ---------------------------------------------------------------------------
AnsiString TfrmPaymentType::GetVoucherNumber(AnsiString inPaymentName,AnsiString inReferenceNumber,bool isLoyaltyVoucher)
{
    AnsiString retVal = "";
    if(isLoyaltyVoucher && TGlobalSettings::Instance().GiftCardValidation == MSROnly)
    {
        std::auto_ptr<TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create<TfrmCardSwipe>(this));
        frmCardSwipe->ShowModal();
        if(frmCardSwipe->ModalResult == mrOk)
          retVal = frmCardSwipe->SwipeString.SubString(1, 80).Trim();
    }
    else
    {
        std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
        frmTouchKeyboard->MaxLength = 50;
        frmTouchKeyboard->AllowCarriageReturn = false;
        frmTouchKeyboard->StartWithShiftDown = false;
        frmTouchKeyboard->MustHaveValue = true;
        frmTouchKeyboard->KeyboardText = inReferenceNumber;
        frmTouchKeyboard->Caption = "Enter " + inPaymentName + " Number";
        if (frmTouchKeyboard->ShowModal() == mrOk)
        {
          retVal = frmTouchKeyboard->KeyboardText.Trim();
        }
    }
    return retVal;
}

bool TfrmPaymentType::IsGiftCardNumberValid(AnsiString inGiftCardNumber)
{
   const char* charArray = inGiftCardNumber.c_str();
   int lastChar = -1;
   for(int i = 0; i < inGiftCardNumber.Length(); i++)
    {
        int currentChar = charArray[i];
        if((currentChar == 34) ||
           (currentChar == 39) ||
           (currentChar == 44) ||
            (lastChar != -1 && lastChar == 32 && lastChar == currentChar))
        {
           return false;
        }
        lastChar = currentChar;
    }
    return true;
}
//---------------------------------------------------------------------
void TfrmPaymentType::CalculatePatrons(TPaymentTransaction &CurrentTransaction)
{
    std::vector<TPatronType> patronsStore = CurrentTransaction.Patrons;
    bool isRestoreRequired = false;
	if (TGlobalSettings::Instance().EnableMenuPatronCount)
    {
	    CurrentTransaction.CalculatePatronCountFromMenu();
        isRestoreRequired = true;
        for(int indexPatrons = 0; indexPatrons < CurrentTransaction.Patrons.size(); indexPatrons++)
        {
            if(CurrentTransaction.Patrons[indexPatrons].Default)
            {
                CurrentTransaction.PatronCountFromMenu = CurrentTransaction.Patrons[indexPatrons].Count;
                break;
            }
        }
    }
    if(isRestoreRequired)
    {
        for(int indexPatrons = 0; indexPatrons < CurrentTransaction.Patrons.size(); indexPatrons++)
        {
            if(CurrentTransaction.Patrons[indexPatrons].Default)
            {
                for(int indexStore = 0; indexStore < patronsStore.size(); indexStore++)
                {
                     if(patronsStore[indexStore].Default)
                     {
                       int backStore = patronsStore[indexStore].Count - CurrentTransaction.PatronCountFromMenu;
                       CurrentTransaction.Patrons[indexPatrons].Count += patronsStore[indexStore].Count;
                     }
                }
            }
            else
            {
                for(int indexStore = 0; indexStore < patronsStore.size(); indexStore++)
                {
                     if(!patronsStore[indexStore].Default)
                       CurrentTransaction.Patrons[indexPatrons].Count += patronsStore[indexStore].Count;
                }
            }
        }
    }
    bool isDefaultInitializeRequired = true;
    for(int indexPatron = 0; indexPatron < CurrentTransaction.Patrons.size(); indexPatron++)
    {
        if(CurrentTransaction.Patrons[indexPatron].Count != 0)
        {
             isDefaultInitializeRequired = false;
             break;
        }
    }
    if(isDefaultInitializeRequired)
        TManagerPatron::Instance().SetDefaultPatrons(CurrentTransaction.DBTransaction, CurrentTransaction.Patrons, 1);
}
//---------------------------------------------------------------------
 bool TfrmPaymentType::IsReceipeAdded(TList *Orders)
{
    bool isReceipeAdded = false;
    if (Orders->Count != NULL)
    {
        for (int i = 0; i < Orders->Count; i++)
         {
            TItemComplete *Order = (TItemComplete *)Orders->Items[i];;
             if(Order->SalesRecipesToApply->Count)
                {
                    isReceipeAdded =true;
                    break;
                }
          }

      }
    return isReceipeAdded;
}
//---------------------------------------------------------------------------------------
bool TfrmPaymentType::IsItemAssignToPrinter(TList *Orders)
{
   bool InformToChef = false;
   try
   {
       for (int i = 0; i < Orders->Count; i++)
       {
           Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
           DBTransaction.StartTransaction();
           TItemComplete *Order = (TItemComplete*)Orders->Items[i];
           bool Categorykey = IsCategoryAssignedToKitchenPrinter(DBTransaction,Order);
           int coursekey = GettingCourseKey(DBTransaction, Order->ItemKey);
           bool Itemcoursekey = IsCourseAssignedToKitchenPrinter(DBTransaction, coursekey) ;
           if(Itemcoursekey || Categorykey)
           {
                InformToChef = true;
                 break;
           }
       }
   }
   catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
   return InformToChef;
}

//----------------------------------------------------------------------------------
bool TfrmPaymentType::IsCategoryAssignedToKitchenPrinter(Database::TDBTransaction &DBTransaction,TItemComplete *item)
{

    bool RetVal = false;
    try
    {
       for (int j = 0; j < item->Categories->Count; j++)
       {
			TItemSizeCategory* category = item->Categories->CategoryGet(j);

            if(IsCategoryKeyExistInDB(DBTransaction, category->CategoryKey))
            {
               RetVal = true;
               break;
            }
         }
       }
        catch(Exception &E)
        {
            TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
            throw;
        }
    return RetVal;
}
//-----------------------------------------------------------------------------------
bool TfrmPaymentType::IsCategoryKeyExistInDB(Database::TDBTransaction &DBTransaction, int CategoryKey)
{
    bool RetVal = false;
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT * FROM PRNCAT WHERE CATEGORY_KEY=:CATEGORY_KEY";
        IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
        IBInternalQuery->ExecQuery();
        RetVal = IBInternalQuery->RecordCount;
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------------
bool TfrmPaymentType::IsCourseAssignedToKitchenPrinter(Database::TDBTransaction &DBTransaction, int coursekey)
{
    bool RetVal = false;
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT COURSE_KEY FROM PRNORDER WHERE COURSE_KEY=:COURSE_KEY";
        IBInternalQuery->ParamByName("COURSE_KEY")->AsInteger = coursekey;
        IBInternalQuery->ExecQuery();
        RetVal = IBInternalQuery->RecordCount;
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return RetVal;
}

//------------------------------------------------------------------------

int TfrmPaymentType::GettingCourseKey(Database::TDBTransaction &DBTransaction, int Itemkey)
{
    int RetVal = 0 ;
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT COURSE_KEY FROM ITEM WHERE ITEM_KEY=:ITEM_KEY";
        IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = Itemkey;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
        RetVal = IBInternalQuery->FieldByName("COURSE_KEY")->AsInteger;
        }

    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return RetVal;
}
