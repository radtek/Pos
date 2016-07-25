//---------------------------------------------------------------------------

#ifndef DiscountTypesDriverInterfaceH
#define DiscountTypesDriverInterfaceH

#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

class TDiscountTypesDriverInterface
{
    public:
        virtual void LoadAllDiscountTypes(TList* types) = 0;
};
#endif
