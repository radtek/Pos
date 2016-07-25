//---------------------------------------------------------------------------


#pragma hdrstop

#include "WeighScale.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TWeighScale::TWeighScale()
{
   Price = 0;
   UnitPrice  = 0;
   Stable = false;
}

TWeighScale::IsStable()
{
   return Stable;
}
