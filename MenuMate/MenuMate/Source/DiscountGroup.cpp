//---------------------------------------------------------------------------


#pragma hdrstop

#include "DiscountGroup.h"

//---------------------------------------------------------------------------
TDiscountGroup::TDiscountGroup(int dbKey)
{
    DBKey = dbKey;
}

TDiscountGroup::TDiscountGroup(int dbKey, UnicodeString name, bool isDefault)
{
    DBKey = dbKey;
    Name = name;
    IsDefault = isDefault;
}
#pragma package(smart_init)
