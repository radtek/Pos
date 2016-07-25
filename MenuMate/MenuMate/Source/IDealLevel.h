//---------------------------------------------------------------------------

#ifndef IDealLevelH
#define IDealLevelH

#include <System.hpp>

//---------------------------------------------------------------------------
class IDealLevel
{
	public:
		virtual ~IDealLevel() {}

		virtual Currency GetQty() const = 0;
		virtual Currency GetPrice() const = 0;

		virtual void SetQty(Currency inQty) = 0;
		virtual void SetPrice(Currency inPrice) = 0;

		virtual bool operator<(const IDealLevel& rhs) = 0;
		virtual bool operator>(const IDealLevel& rhs) = 0;
		virtual bool operator<=(const IDealLevel& rhs) = 0;
		virtual bool operator>=(const IDealLevel& rhs) = 0;

		virtual bool operator==(const IDealLevel& rhs) = 0;
		virtual bool operator!=(const IDealLevel& rhs) = 0;
};

//---------------------------------------------------------------------------
struct DealLevelComparator
{
	bool operator() (const IDealLevel* lhs, const IDealLevel* rhs) const
	{
		return lhs->GetQty() < rhs->GetQty();
	}
};
#endif
