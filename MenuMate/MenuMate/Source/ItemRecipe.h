//---------------------------------------------------------------------------

#ifndef ItemRecipeH
#define ItemRecipeH

#include <Classes.hpp>
// ---------------------------------------------------------------------------
class TListRecipeContainer;

class TItemRecipe
{
public:
	TListRecipeContainer *Owner;
	Currency Cost;
	double CostGSTPercent;
	UnicodeString StockCode;
	UnicodeString StockLocation;
	double Qty;
	bool Enabled;
	AnsiString Reason;

	TItemRecipe();
};

#endif
