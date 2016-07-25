//---------------------------------------------------------------------------

#ifndef WeightedPriceH
#define WeightedPriceH

#include "Weight.h"
//---------------------------------------------------------------------------

class TWeightedPrice
{
public:
   TWeightedPrice();

   bool WeightedItem;
   TWeight Weight;
   TWeight TareWeight;
   Currency UnitPrice;
   void Assign(TWeight &inWeight,Currency UnitPrice);
   void Assign(Currency UnitPrice);
   void AssignTareWeight(TWeight &inTareWeight);   
};

#endif
 