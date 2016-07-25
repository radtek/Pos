//---------------------------------------------------------------------------

#ifndef DealResultH
#define DealResultH
//---------------------------------------------------------------------------

#include <System.hpp>

class TDealResult
{
    public:
        Currency Quantity;
        Currency Price;
        int BreakdownCategoryKey;
		UnicodeString DealName;
		int Priority;

        TDealResult();

		TDealResult(int breakdownCategoryKey, Currency quantity, Currency price, UnicodeString dealName, int priority);
};

#endif
