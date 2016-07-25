//---------------------------------------------------------------------------

#ifndef TItemRecipeH
#define TItemRecipeH

// ---------------------------------------------------------------------------
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

	TItemRecipe();
};

#endif
