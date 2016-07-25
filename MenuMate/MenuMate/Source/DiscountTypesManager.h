//---------------------------------------------------------------------------

#ifndef DiscountTypesManagerH
#define DiscountTypesManagerH

#include "DiscountTypesDriverInterface.h"
//---------------------------------------------------------------------------

class TDiscountTypesManager
{
    public:
        TDiscountTypesManager();
        void LoadAllDiscountTypes(TList* types);

    private:
        std::auto_ptr<TDiscountTypesDriverInterface> _driver;
};
#endif
