//---------------------------------------------------------------------------

#ifndef DiscountTypesDBDriverH
#define DiscountTypesDBDriverH

#include "DiscountTypesDriverInterface.h"
//---------------------------------------------------------------------------

class TDiscountTypesDBDriver : public TDiscountTypesDriverInterface
{
    public:
        void LoadAllDiscountTypes(TList* types);
    private:
        void _loadAllDiscountTypes(Database::TDBTransaction &transaction, TList* types);
};
#endif
