// ---------------------------------------------------------------------------

#pragma hdrstop

#include "MMPrice.h"
#include "Rounding.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

TMMPrice::TMMPrice()
{
	 AdjustMode = amSetPrice;
	 Source = psMMDB;
	 Value = 0;
	 Priority = 0;
	 PriceLevel = TIMCPriceLevel0;
}

TMMPrice::TMMPrice(Currency inValue, TIMCPriceLevel inPriceLevel, eAdjustMode inAdjustMode, ePriceSource inSource, int inPriority)
{
	 AdjustMode = inAdjustMode;
	 Source = inSource;
	 Value = RoundToNearest(inValue, MIN_CURRENCY_VALUE, true);
	 Priority = inPriority;
	 PriceLevel = inPriceLevel;
}

bool
TMMPrice::operator==(TMMPrice &rhs)
{
	return AdjustMode == rhs.AdjustMode
          && Source == rhs.Source
          && Value == rhs.Value
          && Priority == rhs.Priority
          && PriceLevel == rhs.PriceLevel;
}

void TMMPrice::Inverse()
{
	 if (AdjustMode == amSetPrice)
	 {
		  Value = -Value;
	 }
	 else if (AdjustMode == amPercent)
	 {
	 }
	 else if (AdjustMode == amCurrency)
	 {
		  Value = -Value;
	 }
}

Currency TMMPrice::Total()
const
{
	return Value;
}
