//---------------------------------------------------------------------------


#pragma hdrstop

#include "DealResult.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
TDealResult::TDealResult()
{
}
//---------------------------------------------------------------------------
TDealResult::TDealResult(int breakdownCategoryKey, Currency quantity, Currency price, UnicodeString dealName, int priority)
	: Quantity(quantity), Price(price), BreakdownCategoryKey(breakdownCategoryKey), DealName(dealName), Priority(priority)
{
}
//---------------------------------------------------------------------------
