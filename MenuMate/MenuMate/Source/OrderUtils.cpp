//---------------------------------------------------------------------------


#pragma hdrstop

#include "OrderUtils.h"
#include "ItemComplete.h"
#include "ItemMinorComplete.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


Currency TOrderUtils::TotalPrice(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->Price();
	}
   return RetVal;
}

Currency TOrderUtils::TotalDiscount(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->TotalDiscount();
	}
   return RetVal;
}

Currency TOrderUtils::TotalPriceSides(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->TotalPriceSides();
	}
   return RetVal;
}

Currency TOrderUtils::TotalPriceAdjustment(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal +=  Order->TotalPriceAdjustment();
	}
   return RetVal;
}

Currency TOrderUtils::TotalPriceRedeemed(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->TotalPriceRedeemed();
	}
   return RetVal;
}

Currency TOrderUtils::TotalPriceAdjustmentSides(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->TotalPriceAdjustmentSides();
	}
   return RetVal;
}

Currency TOrderUtils::GrandTotal(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->GrandTotal();
	}
   return RetVal;
}

Currency TOrderUtils::TotalAdjustment(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->TotalAdjustment();
	}
   return RetVal;
}

   static Currency TotalAdjustment(TList *Orders);

Currency TOrderUtils::GrandTotalSides(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->GrandTotalSides();
	}
   return RetVal;
}

Currency TOrderUtils::GrandTotalGSTContentSides(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->GrandTotalGSTContentSides();
	}
   return RetVal;
}

Currency TOrderUtils::TotalPriceRedeemedSides(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->TotalPriceRedeemedSides();
	}
   return RetVal;
}

Currency TOrderUtils::TotalPriceAdjustmentSidesExclGST(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->TotalPriceAdjustmentSidesExclGST();
	}
   return RetVal;
}

bool TOrderUtils::SeniorCitizensDiscountApplied(TList *Orders)
{
    bool retVal = false;
	for (int i = 0; i < Orders->Count && !retVal; i++)
	{
        TItemComplete *Order = (TItemComplete *)Orders->Items[i];
        retVal = Order->HasSeniorCitizensDiscountApplied();
	}
    return retVal;
}
bool TOrderUtils::PWDDiscountApplied(TList *Orders)
{
    bool retVal = false;
	for (int i = 0; i < Orders->Count && !retVal; i++)
	{
        TItemComplete *Order = (TItemComplete *)Orders->Items[i];
        retVal = Order->HasPWDApplied();
	}
    return retVal;
}

bool TOrderUtils::AnyDiscountsApplied(TList *Orders)
{
    bool retVal = false;
	for (int i = 0; i < Orders->Count && !retVal; i++)
	{
        TItemComplete *Order = (TItemComplete *)Orders->Items[i];
        retVal = Order->Discounts.size() > 0;
	}
    return retVal;
}

Currency TOrderUtils::FinalPriceAdjustmentSides(TList *Orders)
{
   Currency RetVal = 0;
	for (int i = 0; i < Orders->Count ; i++)
	{
		TItemComplete *Order = (TItemComplete *)Orders->Items[i];
		RetVal += Order->FinalPriceSides_BillCalc();
	}
   return RetVal;
}

//---------------------------------------------------------------------------
bool TOrderUtils::SeperateNmiPriceBarcodedItem( TItemMinorComplete* CurrentItem )
{
	bool result = false;

	result = TGlobalSettings::Instance().EnableNmiDisplay
				&& CurrentItem->IsPriceBarcodedItem;

	return result;
}
//---------------------------------------------------------------------------
bool TOrderUtils::SoloItem(TItemComplete *CurrentItem)
{
	if((CurrentItem->OrderType != NormalOrder)
		|| (CurrentItem->SetMenuMaster)
		|| (CurrentItem->SubOrders->Count != 0)
		|| (CurrentItem->WeightedPrice.WeightedItem == true)
		|| ( SeperateNmiPriceBarcodedItem(CurrentItem)))	// checking to seperate nmi price barcode items
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool TOrderUtils::SoloItem(TItemMinorComplete *CurrentItem)
{
	if((CurrentItem->SetMenuMaster)
		|| (CurrentItem->SubOrders->Count != 0)
		|| (CurrentItem->WeightedPrice.WeightedItem == true)
		|| ( SeperateNmiPriceBarcodedItem(CurrentItem)))   // checking to seperate nmi price barcode items
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool TOrderUtils::Match(TItemComplete *PrevItem,TItemComplete *CurrentItem )
{
   if((CurrentItem->Item != PrevItem->Item) ||
      (PrevItem->Note != CurrentItem->Note) ||
      (PrevItem->Size != CurrentItem->Size) ||
      (PrevItem->TotalPriceAdjustment() != CurrentItem->TotalPriceAdjustment()) ||
      (PrevItem->SetMenu != CurrentItem->SetMenu) ||
      (PrevItem->SetMenuGroup != CurrentItem->SetMenuGroup) ||
      (PrevItem->ServingCourse.ServingCourseKey != CurrentItem->ServingCourse.ServingCourseKey) ||
      (CurrentItem->Item_ID != PrevItem->Item_ID) ||
      (!CurrentItem->OptionsSelected->Compare(PrevItem->OptionsSelected)) ||
      (!CurrentItem->SubOrders->Compare(PrevItem->SubOrders))) //
    {
		return false;
	}
	return true;

}
//---------------------------------------------------------------------------
bool TOrderUtils::Match(TItemMinorComplete *PrevItem,TItemMinorComplete *CurrentItem )
{
   if((CurrentItem->Item != PrevItem->Item) ||
      (PrevItem->Size != CurrentItem->Size) ||
      (PrevItem->TotalPriceAdjustment() != CurrentItem->TotalPriceAdjustment()) ||
      (!CurrentItem->SubOrders->Compare(PrevItem->SubOrders))) //
   {
		return false;
	}
	return true;
}

