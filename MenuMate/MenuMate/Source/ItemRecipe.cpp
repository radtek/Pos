//---------------------------------------------------------------------------


#pragma hdrstop

#include "ItemRecipe.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TItemRecipe::TItemRecipe()
{
   Cost = 0;
   CostGSTPercent = 0;
   Qty = 0;
   Enabled = true;
   Reason = NULL;
}