//---------------------------------------------------------------------------

#ifndef OrderUtilsH
#define OrderUtilsH
//---------------------------------------------------------------------------
#include <system.hpp>
#include <Classes.hpp>

#include "ItemComplete.h"
#include "ItemMinorComplete.h"

class TOrderUtils
{
   public :
	// List Pricing Functions.
   static Currency TotalPrice(TList *Orders);
   static Currency TotalPriceSides(TList *Orders);
   static Currency TotalPriceAdjustment(TList *Orders);
   static Currency TotalPriceAdjustmentSides(TList *Orders);
   static Currency TotalPriceAdjustmentSidesExclGST(TList *Orders);

   static Currency TotalPriceGSTContentSides(TList *Orders);
	static Currency TotalDiscount(TList *Orders);
   static Currency TotalAdjustment(TList *Orders);

   static Currency TotalPriceRedeemed(TList *Orders);
   static Currency TotalPriceRedeemedSides(TList *Orders);
   static Currency GrandTotal(TList *Orders);
   static Currency GrandTotalSides(TList *Orders);
   static Currency GrandTotalGSTContentSides(TList *Orders);

   static bool SeniorCitizensDiscountApplied(TList *Orders);
   static bool AnyDiscountsApplied(TList *Orders);
   static Currency FinalPriceAdjustmentSides(TList *Orders);
   static bool SeperateNmiPriceBarcodedItem( TItemMinorComplete* CurrentItem );
   static bool SoloItem(TItemComplete *CurrentItem);
   static bool SoloItem(TItemMinorComplete *CurrentItem);
   static bool Match(TItemComplete *PrevItem,TItemComplete *CurrentItem );
   static bool Match(TItemMinorComplete *PrevItem,TItemMinorComplete *CurrentItem );
};
#endif
