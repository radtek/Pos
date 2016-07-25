//---------------------------------------------------------------------------


#pragma hdrstop

#include "DealManager.h"
#include "DBDeal.h"
#include <iterator>

//---------------------------------------------------------------------------
IDealManager* TDealManager::instance = NULL;
//---------------------------------------------------------------------------
bool compareDeals (IDeal* lhs, IDeal* rhs)
{
	return lhs->GetPriority() < rhs->GetPriority();
}
//---------------------------------------------------------------------------

TDealManager::TDealManager()
{
	dealStorage = new DBDeal();
    dealStorage->LoadAll(dealList);
}
//---------------------------------------------------------------------------

TDealManager::~TDealManager()
{
	delete dealStorage;
	for (StoredDealVector::iterator it = dealList.begin(); it != dealList.end(); it++)
	{
		delete *it;
	}
}
//---------------------------------------------------------------------------

IDealManager* TDealManager::Instance()
{
	if (instance == NULL)
	{
		instance = new TDealManager();
	}
	return instance;
}
//---------------------------------------------------------------------------

std::vector<int> TDealManager::GetCategoryKeysByPriority()
{
	std::vector<int> catKeys;
	sortDeals();
	for (StoredDealVector::iterator it = dealList.begin(); it != dealList.end(); it++)
	{
        catKeys.push_back((*it)->GetCategoryKey());
    }
	return catKeys;
}
//---------------------------------------------------------------------------

std::vector<TDealResult> TDealManager::GetDiscountedPrices(int inBreakdownCategoryKey, Currency inQty)
{
    std::vector<TDealResult> appliedDeals;

	// get the deals for the category if any
    StoredDealVector dealsForCategory = getDealsForCategory(inBreakdownCategoryKey);

    if(dealsForCategory.size() > 0)
    {
		Currency remainingQuantity = inQty;
        Currency appliedQuantity = 0;

		// iterate through the deals and apply levels accordingly
		for(StoredDealVector::iterator it = dealsForCategory.begin(); it != dealsForCategory.end(); it++)
        {

			appliedQuantity += applyDealLevels((*it), appliedDeals, remainingQuantity);

            remainingQuantity = inQty - appliedQuantity;
        }
	}

    return appliedDeals;
}
//---------------------------------------------------------------------------

StoredDealVector TDealManager::getDealsForCategory(int breakdownCategoryKey)
{
    StoredDealVector deals;

	for(StoredDealVector::iterator it = dealList.begin(); it != dealList.end(); it++)
    {
		if((*it)->GetCategoryKey() == breakdownCategoryKey)
		{
			deals.push_back(*it);
		}
    }

	if(deals.size() > 0)
	{
		std::stable_sort(dealList.begin(), dealList.end(), TDealUtilities::CompareDealsByPriority);
	}

    return deals;
}
//---------------------------------------------------------------------------

Currency TDealManager::applyDealLevels(IStoredDeal *inDeal, std::vector<TDealResult> &dealResults, Currency remainingQty)
{
	Currency appliedQuantity = 0;

	int catKey = inDeal->GetCategoryKey();
	UnicodeString dealName = inDeal->GetName();
	int priority = inDeal->GetPriority();

	StoredDealLevelSet dealLevels = inDeal->GetStoredDealLevels();

	StoredDealLevelSet::reverse_iterator itDealLevels = dealLevels.rbegin();

	while(itDealLevels != dealLevels.rend() && remainingQty > 0)
    {
		IDealLevel *dealLevel = *itDealLevels;
		if(remainingQty < dealLevel->GetQty())
		{
			itDealLevels++;
		}
        else
		{
			TDealResult result(catKey, dealLevel->GetQty(), dealLevel->GetPrice(), dealName, priority);

            dealResults.push_back(result);

            remainingQty = remainingQty - result.Quantity;
            appliedQuantity += result.Quantity;
        }
    }

    return appliedQuantity;
}
//---------------------------------------------------------------------------

void TDealManager::Add(IStoredDeal* inDeal)
{
	dealList.push_back(inDeal);
	dealStorage->Save(inDeal);
	sortDeals();
}
//---------------------------------------------------------------------------

void TDealManager::Remove(int inDealIndex)
{
	StoredDealVector::iterator it = dealList.begin();
	std::advance(it, inDealIndex);
	dealStorage->Delete(*it);
	delete *it;
	dealList.erase(it);
	sortDeals();
}
//---------------------------------------------------------------------------

void TDealManager::Update(int inDealIndex)
{
	StoredDealVector::iterator it = dealList.begin();
	std::advance(it, inDealIndex);
	dealStorage->Update(*it);
	sortDeals();
}
//---------------------------------------------------------------------------

IStoredDeal* TDealManager::GetDealAt(int inDealIndex)
{
	StoredDealVector::iterator it = dealList.begin();
	std::advance(it, inDealIndex);
	return *it;
}
//---------------------------------------------------------------------------

void TDealManager::sortDeals()
{
    std::stable_sort(dealList.begin(), dealList.end(), compareDeals);
}
//---------------------------------------------------------------------------

StoredDealVector::iterator TDealManager::GetDealsBegin()
{
	return dealList.begin();
}
//---------------------------------------------------------------------------

StoredDealVector::iterator TDealManager::GetDealsEnd()
{
	return dealList.end();
}
//---------------------------------------------------------------------------

int TDealManager::GetDealsSize()
{
	return dealList.size();
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
