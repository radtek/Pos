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
    for(int i = 0; i< PaymentTransaction.Orders->Count; i++)
    {
        TItemComplete *itemComplete = (TItemComplete*)PaymentTransaction.Orders->Items[0];
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
            break;
        }
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


