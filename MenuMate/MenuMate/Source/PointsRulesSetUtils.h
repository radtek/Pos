//---------------------------------------------------------------------------

#ifndef PointsRulesSetUtilsH
#define PointsRulesSetUtilsH
//---------------------------------------------------------------------------
#include "PointsRulesSet.h"

class TPointsRulesSetUtils
{
   public:
   int Compress(TPointsRulesSet &PointsRulesSet);
   void Expand(int BitSet, TPointsRulesSet &PointsRulesSet);
   void ExpandSubs(int BitSet, TPointsRulesSubsSet &PointsRulesSet);
   int CompressSubs(TPointsRulesSubsSet &PointsRulesSet);
};

#endif
 