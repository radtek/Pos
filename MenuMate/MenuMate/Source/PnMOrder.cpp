//---------------------------------------------------------------------------


#pragma hdrstop

#include "PnMOrder.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TPnMOrder::TPnMOrder( const TPnMOrder& inOther )
{
	Key       = inOther.Key;
	ID        = inOther.ID;
	TabKey    = inOther.TabKey;
	Type      = inOther.Type;
	Name      = inOther.Name;
	Price     = inOther.Price;
	Qty       = inOther.Qty;
	GridIndex = inOther.GridIndex;
	Selected  = inOther.Selected;
	IsParent  = inOther.IsParent;
	IsSide    = inOther.IsSide;
	ParentKey = inOther.ParentKey;
	TimeStamp = inOther.TimeStamp;
    TimeKey   = inOther.TimeKey;
    PatronCount = inOther.PatronCount;
    ItemType = inOther.ItemType;
    RoomNumber = inOther.RoomNumber;
    AccNumber = inOther.AccNumber;
    FirstName = inOther.FirstName;
    LastName = inOther.LastName;
    IsItemFree = inOther.IsItemFree;
}
//---------------------------------------------------------------------------

