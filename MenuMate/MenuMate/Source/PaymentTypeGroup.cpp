//---------------------------------------------------------------------------


#pragma hdrstop

#include "PaymentTypeGroup.h"

//---------------------------------------------------------------------------
TPaymentTypeGroup::TPaymentTypeGroup(int dbKey)
{
    DBKey = dbKey;
    Name = "";
    IsDefault = false;
}

TPaymentTypeGroup::TPaymentTypeGroup(int dbKey, UnicodeString name, bool isDefault)
{
    DBKey = dbKey;
    Name = name;
    IsDefault = isDefault;
}
#pragma package(smart_init)
