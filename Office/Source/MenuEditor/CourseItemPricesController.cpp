//---------------------------------------------------------------------------
#pragma hdrstop

#include "CourseItemPricesController.h"
#include "MM_Math.h"
#include "MenuEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace MenuEditor
{
//---------------------------------------------------------------------------
class OneCentRoundingStrategy : public IRoundingStrategy
{
public: AnsiString GetName() { return "Nearest 1c"; }
public: Currency GetRoundedValue(Currency value) { return MMMath::RoundCurrency(value, 2); }
};
//---------------------------------------------------------------------------
class FiveCentRoundingStrategy : public IRoundingStrategy
{
public: AnsiString GetName() { return "Nearest 5c"; }
public: Currency GetRoundedValue(Currency value) { return MMMath::RoundCurrencyTo5c(value); }
};
//---------------------------------------------------------------------------
class TenCentRoundingStrategy : public IRoundingStrategy
{
public: AnsiString GetName() { return "Nearest 10c"; }
public: Currency GetRoundedValue(Currency value) { return MMMath::RoundCurrencyTo10c(value); }
};
//---------------------------------------------------------------------------
CourseItemPricesController::CourseItemPricesController(ICourseItemPricesForm* form)
{
	this->form = form;
	this->BuildRoundingStrategies();
}
//---------------------------------------------------------------------------
CourseItemPricesController::~CourseItemPricesController()
{
	this->DeleteRoundingStrategies();
}
//---------------------------------------------------------------------------
void CourseItemPricesController::SetCurrentPrices(std::vector<std::pair<TItemSizeNode*, TItemNode*> > prices)
{
	std::vector<std::pair<TItemSizeNode*, TItemNode*> >::iterator price = prices.begin();
	std::vector<ItemSize> sizes;
	while (price != prices.end())
	{
		ItemSize itemSize;
		itemSize.SizeNode = price->first;

		itemSize.Item = price->second->LongDescription;
		itemSize.Size = price->first->LongDescription;

		itemSize.Price = price->first->Price;
		itemSize.SpecialPrice = price->first->SpecialPrice;

		itemSize.OriginalPrice = price->first->Price;
		itemSize.OriginalSpecialPrice = price->first->SpecialPrice;

        itemSize.ItemSizePriceLevels = price->first->ItemSizePriceLevels;
        itemSize.OriginalItemSizePriceLevels = price->first->ItemSizePriceLevels;

		itemSize.GST = price->first->GST;

		sizes.push_back(itemSize);
		price++;
	}

	this->form->Set(sizes);
}
//---------------------------------------------------------------------------
TStrings* CourseItemPricesController::GetRoundingStrategies()
{
	this->roundingStrategiesAsTStrings.release();
	this->roundingStrategiesAsTStrings = std::auto_ptr<TStringList>(new TStringList());

	std::vector<IRoundingStrategy*>::iterator current = this->roundingStrategies.begin();
	while (current != this->roundingStrategies.end())
	{
		this->roundingStrategiesAsTStrings->AddObject((*current)->GetName(), (TObject*)(*current));
		current++;
	}

	return this->roundingStrategiesAsTStrings.get();
}
//---------------------------------------------------------------------------
void CourseItemPricesController::Calculate(std::vector<ItemSize>& itemSizes, double adjustBy, TObject* roundingStrategy)
{
	double adjustmentFactor = (100 + adjustBy) / 100;

	std::vector<ItemSize>::iterator itemSize = itemSizes.begin();
	while (itemSize != itemSizes.end())
	{
		IRoundingStrategy* rounding = (IRoundingStrategy*)roundingStrategy;
        for(int key=1; key<=frmMenuEdit->noOfPriceLevels; key++)
        {                                                                   	
            itemSize->ItemSizePriceLevels[key].Price = rounding->GetRoundedValue(itemSize->OriginalItemSizePriceLevels[key].Price * adjustmentFactor);
        }
		itemSize->Price = rounding->GetRoundedValue(itemSize->OriginalPrice * adjustmentFactor);
		itemSize->SpecialPrice = rounding->GetRoundedValue(itemSize->OriginalSpecialPrice * adjustmentFactor);

		itemSize++;
	}
}
//---------------------------------------------------------------------------
void CourseItemPricesController::SaveMenuPrices(std::vector<ItemSize>& itemSizes)
{
	std::vector<ItemSize>::iterator itemSize = itemSizes.begin();
	while (itemSize != itemSizes.end())
	{
        for(int key=1; key<=frmMenuEdit->noOfPriceLevels; key++)
        {                                                                   	
            itemSize->SizeNode->ItemSizePriceLevels[key].Price = itemSize->ItemSizePriceLevels[key].Price;
        }
		itemSize->SizeNode->Price = itemSize->Price;
		itemSize->SizeNode->SpecialPrice = itemSize->SpecialPrice;

		itemSize++;
	}
}
//---------------------------------------------------------------------------
void CourseItemPricesController::BuildRoundingStrategies()
{
	this->DeleteRoundingStrategies();

	IRoundingStrategy* oneCent = new OneCentRoundingStrategy();
	this->roundingStrategies.push_back(oneCent);

	IRoundingStrategy* fiveCent = new FiveCentRoundingStrategy();
	this->roundingStrategies.push_back(fiveCent);

	IRoundingStrategy* tenCent = new TenCentRoundingStrategy();
	this->roundingStrategies.push_back(tenCent);
}
//---------------------------------------------------------------------------
void CourseItemPricesController::DeleteRoundingStrategies()
{
	std::vector<IRoundingStrategy*>::iterator current = this->roundingStrategies.begin();
	while (current != this->roundingStrategies.end())
	{
		delete (*current);
		current++;
	}

	this->roundingStrategies.clear();
}
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------

