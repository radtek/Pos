//---------------------------------------------------------------------------

#ifndef DealLevelH
#define DealLevelH

#include <System.hpp>
#include "IStoredDealLevel.h"

//---------------------------------------------------------------------------
class TDealLevel : public IStoredDealLevel
{
	public:
		TDealLevel(Currency inQty, Currency inPrice, int inDealLevelID = 0);

		Currency GetQty() const;
		Currency GetPrice() const;
		int GetDealLevelID() const;

		void SetQty(Currency inQty);
		void SetPrice(Currency inPrice);
		void SetDealLevelID(int inDealLevelID);

		bool operator<(const IDealLevel& rhs);
		bool operator>(const IDealLevel& rhs);
		bool operator<=(const IDealLevel& rhs);
		bool operator>=(const IDealLevel& rhs);

		bool operator==(const IDealLevel& rhs);
		bool operator!=(const IDealLevel& rhs);

	private:
		Currency qty;
		Currency price;
		int dealLevelID;
};
#endif
