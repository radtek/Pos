//---------------------------------------------------------------------------

#ifndef PaymentTypeH
#define PaymentTypeH
#include <System.hpp>;
class TPaymentType
{
    public:
        int DBKey;
        UnicodeString Name;
        TPaymentType(int dbKey);
        TPaymentType(int dbKey, UnicodeString name);
};
//---------------------------------------------------------------------------
#endif
