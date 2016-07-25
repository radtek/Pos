//---------------------------------------------------------------------------


#pragma hdrstop

#include "DiscountTypes.h"

//---------------------------------------------------------------------------
TDiscountTypes::TDiscountTypes(int dbKey){
    DBKey = dbKey;
}

TDiscountTypes::TDiscountTypes(int dbKey, UnicodeString name){
    DBKey = dbKey;
    Name = name;
}
#pragma package(smart_init)
