//---------------------------------------------------------------------------

#ifndef IDealStorageH
#define IDealStorageH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include <vector>

#include "MM_DBCore.h"
#include "DealTypeDefs.h"

class IStoredDeal;

//---------------------------------------------------------------------------
class IDealStorage
{
	public:
		virtual ~IDealStorage() {}

		virtual bool Save(IStoredDeal *inItem) = 0;
		virtual bool Update(IStoredDeal *inItem) = 0;
		virtual bool Delete(IStoredDeal *inItem) = 0;

		virtual void Load(int inItemID, IStoredDeal *outItem) = 0;
		virtual void LoadAll(StoredDealVector &allDeals) = 0;
};
#endif
