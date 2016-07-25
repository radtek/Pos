//---------------------------------------------------------------------------

#ifndef PaymentTypesDriverInterfaceH
#define PaymentTypesDriverInterfaceH
//---------------------------------------------------------------------------
#include "DeviceRealTerminal.h"

class TPaymentTypesDriverInterface
{
    public:
        virtual void LoadAllPaymentTypes(TList* types) = 0;
};
#endif
