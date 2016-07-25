//---------------------------------------------------------------------------

#ifndef DiscountTypesH
#define DiscountTypesH

#include <System.hpp>
//---------------------------------------------------------------------------
class TDiscountTypes
{
    public:
        int DBKey;
        UnicodeString Name;
        TDiscountTypes(int dbKey);
        TDiscountTypes(int dbKey, UnicodeString name);
};
#endif
