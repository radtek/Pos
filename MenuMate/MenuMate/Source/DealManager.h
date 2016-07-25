//---------------------------------------------------------------------------

#ifndef DealManagerH
#define DealManagerH

#include "IDealManager.h"
#include "IDealStorage.h"
#include "DealUtilities.h"
#include "DealResult.h"
#include "DealTypeDefs.h"

//---------------------------------------------------------------------------
class TDealManager : IDealManager
{
	public:
		static IDealManager* Instance();

		~TDealManager();

		void Add(IStoredDeal* inDeal);
		void Remove(int inDealIndex);
		void Update(int inDealIndex);

		IStoredDeal* GetDealAt(int inDealIndex);

		StoredDealVector::iterator GetDealsBegin();
		StoredDealVector::iterator GetDealsEnd();
		int GetDealsSize();

		std::vector<TDealResult> GetDiscountedPrices(int inBreakdownCategoryKey, Currency inQty);
		std::vector<int> GetCategoryKeysByPriority();

	protected:
		TDealManager();

	private:
		void loadDeals();
		void sortDeals();
        StoredDealVector getDealsForCategory(int breakdownCategoryKey);
		Currency applyDealLevels(IStoredDeal *inDeal, std::vector<TDealResult> &dealResults, Currency remainingQty);

		static IDealManager* instance;
		IDealStorage* dealStorage;
		StoredDealVector dealList;
};
#endif
