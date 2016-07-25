//---------------------------------------------------------------------------

#ifndef WeighScaleH
#define WeighScaleH
//---------------------------------------------------------------------------
#include "Weight.h"
#include "SystemEvents.h"

#define WEIGHSCALES_WEIGHTCHANGED      0x01
#define WEIGHSCALES_STABILITYCHANGED   0x02

class TWeighScale
{
   protected :
      Currency Price;
      Currency UnitPrice;
      bool Stable;
   public :
      TWeight Weight;
      TWeighScale();
      TSystemEvents OnWeightChanged;
      TSystemEvents OnStabilityChanged;
      IsStable();
};
#endif
