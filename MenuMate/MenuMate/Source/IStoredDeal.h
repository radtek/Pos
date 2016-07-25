//---------------------------------------------------------------------------

#ifndef IStoredDealH
#define IStoredDealH

#include "IDeal.h"
#include "DealTypeDefs.h"

//---------------------------------------------------------------------------
class IStoredDeal : public IDeal
{
	public:
		virtual ~IStoredDeal() {}

		virtual int GetDealID() = 0;
		virtual StoredDealLevelSet GetStoredDealLevels() = 0;

		virtual void SetDealID(int inDealID) = 0;
		virtual void SetStoredDealLevels(StoredDealLevelSet inDealLevels) = 0;
};
#endif
