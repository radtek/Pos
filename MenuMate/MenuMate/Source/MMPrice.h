// ---------------------------------------------------------------------------

#ifndef MMPriceH
#define MMPriceH

#include <System.hpp>
// ---------------------------------------------------------------------------
enum TIMCPriceLevel { TIMCPriceLevel0,TIMCPriceLevel1,TIMCPriceLevelCustom,TIMCPriceLevelAdjusted};

enum eAdjustMode
{
	amSetPrice, amCurrency, amPercent
};

enum ePriceSource
{
	psMMDB, psMMSystem
};

class TMMPrice
{
public:
	TMMPrice();
	TMMPrice(	Currency inValue,TIMCPriceLevel inPriceLevel,eAdjustMode inAdjustMode,
				ePriceSource inSource,int inPriority);

	bool operator==(TMMPrice &rhs);

	eAdjustMode AdjustMode;
	ePriceSource Source;
	int Priority;
	int PriceLevel;
	Currency Value;
	void Inverse();
	Currency Total() const;

	bool operator < (const TMMPrice & f1)
	{
		bool Retval = false;
		if (Priority < f1.Priority)
		{
			Retval = true;
		}
		else if (Priority == f1.Priority)
		{
			if (Source < f1.Source)
			{
				Retval = true;
			}
		}
		return Retval;
	}friend bool operator < (const TMMPrice & f1, const TMMPrice & f2);

};

bool operator < (const TMMPrice & f1, const TMMPrice & f2)
{
	bool Retval = false;
	if (f1.Priority < f2.Priority)
	{
		Retval = true;
	}
	else if (f1.Priority == f2.Priority)
	{
		if (f1.Source < f2.Source)
		{
			Retval = true;
		}
	}
	return Retval;
};

#endif
