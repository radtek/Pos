// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Money.h"
#include "ManagerVariable.h"
#include "rounding.h"
#include "PaymentTransaction.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif

// ---------------------------------------------------------------------------
#pragma package(smart_init)

void TMoney::Clear()
{
    Total			= 0;
    TotalExclGST	= 0;
    TotalGSTContent	= 0;
    GrandTotal			 = 0;
    GrandTotalExclGST	 = 0;
    GrandTotalGSTContent = 0;
    TotalAdjustment			  = 0;
    TotalAdjustmentExcGST	  = 0;
    TotalAdjustmentGSTContent = 0;
    Discount		   = 0;
    DiscountExcGST	   = 0;
    DiscountGSTContent = 0;
    Surcharge		   	= 0;
    SurchargeExcGST		= 0;
    SurchargeGSTContent	= 0;
    TotalProduct 	  = 0;
    ProductAmount 	  = 0;
    ProductExcGST 	  = 0;
    ProductGSTContent = 0;
    ProductAdjustment 			= 0;    // Amount of Product discounts & Surcharges.
    ProductDiscount	= 0;                // Amount of Product discounts.
    ProductSurcharge		   = 0;     // Amount of Product Surcharges.
    ProductSurchargeExcGST	   = 0;
    ProductSurchargeGSTContent = 0;
    SystemDiscount = 0;
    PaymentAmount  = 0;
    PaymentCashOut = 0;
    PaymentAdjustments			= 0;    // Tips etc.
    PaymentSurcharges			= 0;    // Cheque Fess.
    PaymentSurchargesExcGST 	= 0;
    PaymentSurchargesGSTContent = 0;
    PaymentDiscounts		   = 0;
    PaymentDiscountsExcGST     = 0;
    PaymentDiscountsGSTContent = 0;
    TotalOwing    = 0;
    TotalTendered = 0;
    TotalRounding = 0;
    PaymentDue      = 0;
    PaymentRounding = 0;
    CreditRedeemed  = 0;
    CreditAvailable = 0;
    CreditRemaining = 0;
    Change          = 0;
    UnRoundedChange = 0;
    InvoiceBalance  = 0;
    ServiceCharge = 0;
    ServiceChargeTax = 0;
    FinalPrice = 0;
    RefundPoints = 0;
    RoundedProductAmount = 0;
    RoundingAdjustment = 0;
    IsSCD = false;
    PaymentTip = 0;
}

// ---------------------------------------------------------------------------

void TMoney::Recalc(TPaymentTransaction &Transaction, bool isBilling)
{
    double GST = 0 ;
    bool isSplitPayment = false;
    IsBilling = isBilling;
    Clear();
    for (int i = 0; i < Transaction.Orders->Count; i++)
    {
        TItemComplete *Order = (TItemComplete*)Transaction.Orders->Items[i];

        if(!IsSCD)
           IsSCD = Order->HasSeniorCitizensDiscountApplied() || Order->HasPWDApplied();
        //----------------------------------------------------------------------
        // calling the new bill calculator via the adapter
        //----------------------------------------------------------------------
        billCalcAdapter.CalculateOrderItemPrices( Order );

        for( int j = 0; j < Order->SubOrders->Count; j++ )
        {
            TItemCompleteSub *SideItem = Order->SubOrders->SubOrderGet(j);
            billCalcAdapter.CalculateOrderItemPrices( SideItem );
        }
        //----------------------------------------------------------------------

        /*
         * once we have called the bill calculator above, the following methods from TItemMinorComplete
         * will return the cached result from the class itself.
         */
        RoundedProductAmount += RoundToNearest(Order->TotalPriceSides(), 0.01 , TGlobalSettings::Instance().MidPointRoundsDown);
        ProductAmount += Order->TotalPriceSides();
        ProductDiscount += Order->TotalDiscountSides();
        ProductSurcharge  += Order->TotalSurchargeSides();
        ProductGSTContent += Order->GrandTotalGSTContentSides();          // tax without service charge taxes
        GST += Order->GetGST();
        ServiceCharge   +=  Order->ServiceChargeSides_BillCalc();
        ServiceChargeTax += Order->ServiceChargeTaxSides_BillCalc();
        FinalPrice      +=  Order->FinalPriceSides_BillCalc();    // final price
    }

    for (int i = 0; i < Transaction.PaymentsCount(); i++)
    {
        TPayment *Payment = Transaction.PaymentGet(i);
        PaymentAmount += Payment->GetPay();
        PaymentCashOut += Payment->GetCashOut();
        PaymentRounding += Payment->GetRoundingTotal();
        RefundPoints += Payment->GetRefundPointsValue();
        PaymentSurcharges += Payment->GetSurcharge();
        PaymentSurchargesGSTContent += Payment->GetSurchargeGSTContent();
        PaymentDiscountsGSTContent  += Payment->GetDiscountGSTContent();
        PaymentDiscounts += Payment->GetDiscount();
        if(Payment->TipAmount != 0)
        {
          PaymentTip += Payment->TipAmount;
        }

    }

    TPayment *CashPayment = Transaction.PaymentFind(CASH);
    if (CashPayment != NULL)
    {
        RoundChangeTo = CashPayment->RoundTo;
    }

    for (std::map <long, TTabCredit> ::iterator itTabCredit = Transaction.TabCredit.begin(); itTabCredit != Transaction.TabCredit.end();
    advance(itTabCredit, 1))
    {
        CreditRedeemed += itTabCredit->second.CreditRedeemed;
        CreditAvailable += itTabCredit->second.CurrentCredit;
    }

    Discount = ProductDiscount + PaymentDiscounts;
    Surcharge = ProductSurcharge + PaymentSurcharges - RefundPoints;
    TotalProduct = ProductAmount;
    Total = ProductAmount;
    TotalRounding = PaymentRounding;
    TotalAdjustment = Discount + Surcharge;
    TotalGSTContent = ProductGSTContent + PaymentSurchargesGSTContent + ServiceChargeTax;

    if (TotalAdjustment != 0)
    {
        Transaction.TotalAdjustment = TotalAdjustment;
        Transaction.DiscountReason = TotalAdjustment < 0 ? "Discount " : "Surcharge";
    }

    TotalRounding 	= PaymentRounding;
    TotalAdjustment = Discount + Surcharge;
    if (TotalAdjustment != 0)
    {
        Transaction.TotalAdjustment = TotalAdjustment;
        Transaction.DiscountReason = TotalAdjustment < 0 ? "Discount " : "Surcharge";
    }

    if((Transaction.Type == eTransPartialPayment || Transaction.Type == eTransSplitPayment) &&
        Transaction.RequestPartialPayment > 0 && TotalAdjustment == 0)
    {
       TotalOwing  =   Transaction.RequestPartialPayment;
       isSplitPayment = true;
    }
    else
    {
       TotalOwing  =   FinalPrice;
       Currency TempTotalOwing = TotalOwing;
       if(TGlobalSettings::Instance().RoundOnBilling && isBilling)
        {
          TotalOwing = RoundToNearest(TotalOwing, TGlobalSettings::Instance().RoundOnBillingAmount, TGlobalSettings::Instance().MidPointRoundsDown);
          TotalRounding  += (TotalOwing - TempTotalOwing);
        }
       TotalOwing  += (PaymentSurcharges + PaymentDiscounts - RefundPoints + PaymentTip);


    }

    PaymentDue = TotalOwing - PaymentAmount;

    if(!Transaction.CreditTransaction)
    {
        PaymentDiscountsGSTContent = 0;
        if (TotalOwing <= PaymentAmount)
        {
            UnRoundedChange = PaymentAmount - TotalOwing;
            Change = RoundToNearest(UnRoundedChange, RoundChangeTo, !TGlobalSettings::Instance().MidPointRoundsDown);
            TotalRounding += UnRoundedChange - Change;
            PaymentDue += UnRoundedChange;
            Change += RoundToNearest(PaymentCashOut, RoundChangeTo, TGlobalSettings::Instance().MidPointRoundsDown);
            UnRoundedChange += PaymentCashOut;
        }
        else
        {
            Change += RoundToNearest(PaymentCashOut, RoundChangeTo, TGlobalSettings::Instance().MidPointRoundsDown);
            UnRoundedChange += PaymentCashOut;
            TotalRounding += UnRoundedChange - Change;
        }
    }
    else
    {
        // Only give change for Cash Credits.
        // All other payment types dont give out change.
        if (PaymentDue != 0)
        {
            TotalRounding += PaymentDue - RoundToNearest(PaymentDue, RoundChangeTo, TGlobalSettings::Instance().MidPointRoundsDown);
            Change += RoundToNearest(PaymentCashOut, RoundChangeTo, TGlobalSettings::Instance().MidPointRoundsDown);
            UnRoundedChange += PaymentCashOut;
        }
        else
        {
             Change += RoundToNearest(PaymentCashOut, RoundChangeTo, TGlobalSettings::Instance().MidPointRoundsDown);
             UnRoundedChange += PaymentCashOut;
        }
    }

    if (CashPayment != NULL)
    {
        CashPayment->SetChange(Change);
    }

    //GrandTotal = TotalOwing + TotalRounding;
    GrandTotal =  (TGlobalSettings::Instance().RoundOnBilling && isBilling)? TotalOwing : TotalOwing + TotalRounding - CreditRedeemed;
    GrandTotalGSTContent = ProductGSTContent + PaymentSurchargesGSTContent + PaymentDiscountsGSTContent;
    Round();


    //Logic to calculate rounding adjustment caused by rounding each item and then add all
    // Add all then round
    if(!TGlobalSettings::Instance().RoundOnBilling && !IsSCD && isSplitPayment)
     {
         Currency TempGrandTotal =  RoundedProductAmount + RoundedDiscount + RoundedSurcharge ;
         if(!TGlobalSettings::Instance().ItemPriceIncludeTax)
           {
                TempGrandTotal += ProductGSTContent;
           }
         if(!TGlobalSettings::Instance().ItemPriceIncludeServiceCharge)
           {
                TempGrandTotal += (RoundedServiceCharge + RoundedServiceChargeTax);
           }
         RoundingAdjustment = GrandTotal - TempGrandTotal;
       }
}

void TMoney::Round()
{
    bool MidPointRoundsDown = TGlobalSettings::Instance().MidPointRoundsDown;
    if(TGlobalSettings::Instance().RoundOnBilling && IsBilling && !IsSCD)
     {
        RoundedGrandTotal = GrandTotal;
        RoundedPaymentDue = PaymentDue;
     }
    else
     {
        RoundedGrandTotal       = RoundToNearest(GrandTotal, 0.01, MidPointRoundsDown);
        RoundedPaymentDue       = RoundToNearest(PaymentDue, 0.01, MidPointRoundsDown);
     }
    RoundedDiscount         = RoundToNearest(Discount, 0.01, MidPointRoundsDown);
    RoundedSurcharge        = RoundToNearest(Surcharge, 0.01, MidPointRoundsDown);
    RoundedServiceCharge    = RoundToNearest(ServiceCharge, 0.01, MidPointRoundsDown);
    RoundedServiceChargeTax = RoundToNearest(ServiceChargeTax, 0.01, MidPointRoundsDown);
    RoundedFinalPrice       = RoundToNearest(FinalPrice, 0.01, MidPointRoundsDown);
    RoundedCreditAvailable  = RoundToNearest(CreditAvailable, 0.01, MidPointRoundsDown);
    RoundedCreditRedeemed   = RoundToNearest(CreditRedeemed, 0.01, MidPointRoundsDown);
    RoundedChange           = RoundToNearest(Change, 0.01, MidPointRoundsDown);
    bool AdjustmentMidPointRoundsDown = MidPointRoundsDown == (TotalAdjustment >= Currency(0));
    RoundedTotalAdjustment  = RoundToNearest(TotalAdjustment,  0.01, AdjustmentMidPointRoundsDown);
}



TMoney::TMoney()
{
   RoundChangeTo = 0.05;
}

