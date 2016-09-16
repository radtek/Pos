//---------------------------------------------------------------------------

#ifndef PnMOrderH
#define PnMOrderH

#include "System.hpp"

//---------------------------------------------------------------------------
class TPnMOrder
{
public:
	TPnMOrder()
	  : Key(0),
		ID(0),
		TabKey(0),
		Type(0),
		Name(""),
		Price(0),
		Qty(0),
		GridIndex(-1),
		Selected(false),
		IsParent(false),
		IsSide(false),
		ParentKey(0),
		TimeStamp(0),
        TimeKey(0),
        PatronCount(0),
        IsWeighted(false) {
	}

	TPnMOrder( const TPnMOrder& inOther );

	long Key; // LK field in dtOrder
	__int32 ID;
	long TabKey; // LK field in dtTab that this order belongs
    int Type;
	UnicodeString Name; // To be displayed in grid
	Currency Price;
	float Qty;
	int GridIndex; // Grid index that contains this order

	int     GroupNumber; // Used for Grouping Sides with their Parent.
	bool    IsParent; // Is a Parent of Sides.
	bool    IsSide;
	__int32 ParentKey;

	TDateTime TimeStamp; // Time Ordered Used for Sorting Billing Lists.

	bool Selected;
    bool IsWeighted;
    int TimeKey;    // Time key value that is same as for orders placed at the same time. Used for chefmate cancellations.
        int     PatronCount;

	// Selected for billing therefore not shown in orders list
	bool operator < (const TPnMOrder & rhs)
	{
		return Key < rhs.Key;
	}bool operator < (const TPnMOrder * rhs)
	{
		return Key < rhs->Key;
	}
};
// ---------------------------------------------------------------------------

#endif
