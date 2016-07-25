//---------------------------------------------------------------------------


#pragma hdrstop

#include "WeightedPrice.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TWeightedPrice::TWeightedPrice()
{
   WeightedItem = false;
   UnitPrice = 0;
}


void TWeightedPrice::Assign(TWeight &inWeight,Currency inUnitPrice)
{
   WeightedItem = true;
   Weight.Assign(inWeight);
   UnitPrice = inUnitPrice;
}

void TWeightedPrice::Assign(Currency inUnitPrice)
{
   UnitPrice = inUnitPrice;
}


void TWeightedPrice::AssignTareWeight(TWeight &inTareWeight)
{
   TareWeight.Assign(inTareWeight);
}
