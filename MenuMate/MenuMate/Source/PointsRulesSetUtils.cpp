//---------------------------------------------------------------------------


#pragma hdrstop

#include "PointsRulesSetUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

int TPointsRulesSetUtils::Compress(TPointsRulesSet &PointsRulesSet)
{
   int RetVal = 0;

   if(PointsRulesSet.Contains(eprAllowedNegitive))
   {
      RetVal = RetVal | eprAllowedNegitive;
   }
   if(PointsRulesSet.Contains(eprNeverEarnsPoints))
   {
      RetVal = RetVal | eprNeverEarnsPoints;
   }
   if(PointsRulesSet.Contains(eprEarnsPointsWhileRedeemingPoints))
   {
      RetVal = RetVal | eprEarnsPointsWhileRedeemingPoints;
   }
   if(PointsRulesSet.Contains(eprOnlyEarnsPointsWhileRedeemingPoints))
   {
      RetVal = RetVal | eprOnlyEarnsPointsWhileRedeemingPoints;
   }
   if(PointsRulesSet.Contains(eprNoPointsPurchases))
   {
      RetVal = RetVal | eprNoPointsPurchases;
   }
   if(PointsRulesSet.Contains(eprNoPointsRedemption))
   {
      RetVal = RetVal | eprNoPointsRedemption;
   }

   return RetVal;
}

void TPointsRulesSetUtils::Expand(int BitSet, TPointsRulesSet &PointsRulesSet)
{
   if(BitSet & eprAllowedNegitive)
        PointsRulesSet << eprAllowedNegitive;
   if(BitSet & eprNeverEarnsPoints)
        PointsRulesSet << eprNeverEarnsPoints;
   if(BitSet & eprEarnsPointsWhileRedeemingPoints)
        PointsRulesSet << eprEarnsPointsWhileRedeemingPoints;
   if(BitSet & eprOnlyEarnsPointsWhileRedeemingPoints)
        PointsRulesSet << eprOnlyEarnsPointsWhileRedeemingPoints;
   if(BitSet & eprNoPointsPurchases)
        PointsRulesSet << eprNoPointsPurchases;
   if(BitSet & eprNoPointsRedemption)
        PointsRulesSet << eprNoPointsRedemption;
}
void TPointsRulesSetUtils::ExpandSubs(int BitSet, TPointsRulesSubsSet &PointsRulesSubsSet)
{
   if(BitSet & eprAllowedNegitive)
        PointsRulesSubsSet << eprAllowedNegitive;
   if(BitSet & eprNeverEarnsPoints)
        PointsRulesSubsSet << eprNeverEarnsPoints;
   if(BitSet & eprEarnsPointsWhileRedeemingPoints)
        PointsRulesSubsSet << eprEarnsPointsWhileRedeemingPoints;
   if(BitSet & eprOnlyEarnsPointsWhileRedeemingPoints)
        PointsRulesSubsSet << eprOnlyEarnsPointsWhileRedeemingPoints;
   if(BitSet & eprNoPointsPurchases)
        PointsRulesSubsSet << eprNoPointsPurchases;
   if(BitSet & eprNoPointsRedemption)
        PointsRulesSubsSet << eprNoPointsRedemption;
   if(BitSet & eprFinancial)
        PointsRulesSubsSet << eprFinancial;
   if(BitSet & eprAllowDiscounts)
        PointsRulesSubsSet << eprAllowDiscounts;
}

int TPointsRulesSetUtils::CompressSubs(TPointsRulesSubsSet &PointsRulesSubsSet)
{
   int RetVal = 0;

   if(PointsRulesSubsSet.Contains(eprAllowedNegitive))
   {
      RetVal = RetVal | eprAllowedNegitive;
   }
   if(PointsRulesSubsSet.Contains(eprNeverEarnsPoints))
   {
      RetVal = RetVal | eprNeverEarnsPoints;
   }
   if(PointsRulesSubsSet.Contains(eprEarnsPointsWhileRedeemingPoints))
   {
      RetVal = RetVal | eprEarnsPointsWhileRedeemingPoints;
   }
   if(PointsRulesSubsSet.Contains(eprOnlyEarnsPointsWhileRedeemingPoints))
   {
      RetVal = RetVal | eprOnlyEarnsPointsWhileRedeemingPoints;
   }
   if(PointsRulesSubsSet.Contains(eprNoPointsPurchases))
   {
      RetVal = RetVal | eprNoPointsPurchases;
   }
   if(PointsRulesSubsSet.Contains(eprNoPointsRedemption))
   {
      RetVal = RetVal | eprNoPointsRedemption;
   }
   if(PointsRulesSubsSet.Contains(eprAllowDiscounts))
   {
      RetVal = RetVal | eprAllowDiscounts;
   }
   if(PointsRulesSubsSet.Contains(eprFinancial))
   {
      RetVal = RetVal | eprFinancial;
   }
   return RetVal;
}