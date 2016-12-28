//---------------------------------------------------------------------------


#pragma hdrstop

#include "PaySubsUtility.h"
#include "PointsRulesSetUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//-----------------------------------------------------------------------------
bool TPaySubsUtility::IsLocalLoyalty()
{
  bool retValue = true;
  if((TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate && TGlobalSettings::Instance().LoyaltyMateEnabled) ||
      TGlobalSettings::Instance().MembershipType != MembershipTypeMenuMate)
  {
    retValue = false;
  }
  return retValue;
}
//-----------------------------------------------------------------------------
bool TPaySubsUtility::IsPaySubsEligible(TPaymentTransaction &PaymentTransaction,double &amount)
{
    bool retValue = false;
    for(int itemCount = 0; itemCount< PaymentTransaction.Orders->Count; itemCount++)
    {
        TItemComplete *itemComplete = (TItemComplete*)PaymentTransaction.Orders->Items[itemCount];
        if(itemComplete->Item == "Pay Subs" && itemComplete->TabKey == 0 && itemComplete->GetQty() >=1
           && !PaymentTransaction.CreditTransaction && !itemComplete->wasOpenItem)
        {
            if(itemComplete->HappyHour)
            {
               if(itemComplete->BillCalcResult.FinalPrice >= itemComplete->PriceLevel1)
               {
                 amount = itemComplete->PriceLevel1;
                 retValue = true;
               }
            }
            else
            {
               if(itemComplete->BillCalcResult.FinalPrice >= itemComplete->PriceLevel0)
               {
                 amount = itemComplete->PriceLevel0;
                 retValue= true;
               }
            }
            if(retValue)
              break;
        }
        for(int subOrdersCount = 0; subOrdersCount < itemComplete->SubOrders->Count; subOrdersCount++)
        {
            TItemComplete *subItem = (TItemComplete*)itemComplete->SubOrders->Items[subOrdersCount];
            if(subItem->Item == "Pay Subs" && subItem->TabKey == 0 && subItem->GetQty() >=1
           && !PaymentTransaction.CreditTransaction && !subItem->wasOpenItem)
            {
                if(subItem->HappyHour)
                {
                   if(subItem->BillCalcResult.FinalPrice >= subItem->PriceLevel1)
                   {
                     amount = subItem->PriceLevel1;
                     retValue = true;
                   }
                }
                else
                {
                   if(subItem->BillCalcResult.FinalPrice >= subItem->PriceLevel0)
                   {
                     amount = subItem->PriceLevel0;
                     retValue= true;
                   }
                }
                if(retValue)
                  break;
            }
        }
        if(retValue)
          break;
    }
    if(retValue)
    {
         int PointRules = 0;
         PointRules |= eprFinancial;
         PointRules |= eprAllowDiscounts;
         PaymentTransaction.Membership.Member.Points.PointsRules >> eprNoPointsRedemption;
         PaymentTransaction.Membership.Member.Points.PointsRules >> eprNeverEarnsPoints;
         PaymentTransaction.Membership.Member.Points.PointsRules >> eprNoPointsPurchases;
         PaymentTransaction.Membership.Member.Points.PointsRulesSubs >> eprNoPointsRedemption;
         PaymentTransaction.Membership.Member.Points.PointsRulesSubs >> eprNeverEarnsPoints;
         PaymentTransaction.Membership.Member.Points.PointsRulesSubs >> eprNoPointsPurchases;
         TPointsRulesSetUtils().ExpandSubs(PointRules,PaymentTransaction.Membership.Member.Points.PointsRulesSubs);
         TPointsRulesSetUtils().Expand(PointRules,PaymentTransaction.Membership.Member.Points.PointsRules);
    }
  return retValue;
}


