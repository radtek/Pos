//---------------------------------------------------------------------------

#ifndef IStoredDealLevelH
#define IStoredDealLevelH

#include "IDealLevel.h"

//---------------------------------------------------------------------------
class IStoredDealLevel : public IDealLevel
{
	public:
		virtual ~IStoredDealLevel() {}

		virtual int GetDealLevelID() const = 0;

		virtual void SetDealLevelID(int inDealLevelID) = 0;
};
#endif
