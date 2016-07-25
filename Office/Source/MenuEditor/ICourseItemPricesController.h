//---------------------------------------------------------------------------
#ifndef ICourseItemPricesControllerH
#define ICourseItemPricesControllerH
//---------------------------------------------------------------------------
#include <vector>
#include "MenuEdit.h"
#include <map>
//---------------------------------------------------------------------------
class TItemSizeNode;
class TItemNode;
//---------------------------------------------------------------------------
namespace MenuEditor
{
//---------------------------------------------------------------------------
struct ItemSize
{
public:
	TItemSizeNode* SizeNode;

	AnsiString Item;
	AnsiString Size;

	Currency Price;
	Currency SpecialPrice;

	Currency OriginalPrice;
	Currency OriginalSpecialPrice;
    std::map<int,Menu::TItemSizePriceLevel> ItemSizePriceLevels;
    std::map<int,Menu::TItemSizePriceLevel> OriginalItemSizePriceLevels;

	double GST;
};
//---------------------------------------------------------------------------
class IRoundingStrategy
{
public:

	virtual AnsiString GetName() = 0;
	virtual Currency GetRoundedValue(Currency value) = 0;
};
//---------------------------------------------------------------------------
class ICourseItemPricesController
{
public:

	virtual ~ICourseItemPricesController() { };

	virtual void SetCurrentPrices(std::vector<std::pair<TItemSizeNode*, TItemNode*> > prices) = 0;
	virtual void SaveMenuPrices(std::vector<ItemSize>& itemSizes) = 0;

	virtual TStrings* GetRoundingStrategies() = 0;

	virtual void Calculate(std::vector<ItemSize>& itemSizes, double adjustBy, TObject* roundingStrategy) = 0;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
