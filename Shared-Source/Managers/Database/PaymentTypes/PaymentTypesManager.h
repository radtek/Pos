//---------------------------------------------------------------------------

#ifndef PaymentTypesManagerH
#define PaymentTypesManagerH
//---------------------------------------------------------------------------
#include "PaymentTypesDriverInterface.h"

class TPaymentTypesManager
{
    public:
        TPaymentTypesManager();
        void LoadAllPaymentTypes(TList* types);

    private:
        std::auto_ptr<TPaymentTypesDriverInterface> _driver;
};


#endif
