//---------------------------------------------------------------------------

#ifndef IDealManagerH
#define IDealManagerH

#include <vector>
#include <System.hpp>
#include "IStoredDeal.h"
#include "DealResult.h"
#include "DealTypeDefs.h"

//---------------------------------------------------------------------------
class IDealManager
{
	public:
		virtual ~IDealManager() {}

		virtual void Add(IStoredDeal* inDeal) = 0;
		virtual void Remove(int inDealIndex)  = 0;
		virtual void Update(int inDealIndex)  = 0;

		virtual IStoredDeal* GetDealAt(int inDealIndex) = 0;

		virtual StoredDealVector::iterator GetDealsBegin() = 0;
		virtual StoredDealVector::iterator GetDealsEnd() = 0;
		virtual int GetDealsSize() = 0;

		virtual std::vector<TDealResult> GetDiscountedPrices(int inBreakdownCategoryKey, Currency inQty) = 0;
		virtual std::vector<int> GetCategoryKeysByPriority() = 0;

	protected:
		IDealManager() {};
};
#endif
