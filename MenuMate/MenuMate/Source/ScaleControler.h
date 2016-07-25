//---------------------------------------------------------------------------

#ifndef ScaleControlerH
#define ScaleControlerH
//---------------------------------------------------------------------------
#include "WeighScale.h"
#include "PoleDisplay.h"
#include "Forms.hpp"

#include "ItemMinorComplete.h"

class TScaleController
{
   private:
      TForm *Owner;
      TPoleDisplay *PoleDisplay;
      TWeighScale *Scales;
      TItemMinorComplete *Item;
      bool AutoConvertScalesWeight;
   public:
      TScaleController(TForm * inOwner, TWeighScale *inScales,TPoleDisplay *inPoleDisplay,TItemMinorComplete *Item, bool inAutoConvertScalesWeight);
      TModalResult GetWeight(TWeight &Weight);
      bool WeighedItem;
};

#endif
