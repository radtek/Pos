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
   if(BitSet & eprAllowedNegitive)                       PointsRulesSet << eprAllowedNegitive;
   if(BitSet & eprNeverEarnsPoints)                      PointsRulesSet << eprNeverEarnsPoints;
   if(BitSet & eprEarnsPointsWhileRedeemingPoints)       PointsRulesSet << eprEarnsPointsWhileRedeemingPoints;
   if(BitSet & eprOnlyEarnsPointsWhileRedeemingPoints)   PointsRulesSet << eprOnlyEarnsPointsWhileRedeemingPoints;
   if(BitSet & eprNoPointsPurchases)                     PointsRulesSet << eprNoPointsPurchases;
   if(BitSet & eprNoPointsRedemption)                    PointsRulesSet << eprNoPointsRedemption;
}