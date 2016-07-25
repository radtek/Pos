//---------------------------------------------------------------------------


#pragma hdrstop

#include "PaymentType.h"
TPaymentType::TPaymentType(int dbKey){
    DBKey = dbKey;
}
TPaymentType::TPaymentType(int dbKey, UnicodeString name){
    DBKey = dbKey;
    Name = name;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
