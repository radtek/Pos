//---------------------------------------------------------------------------

#ifndef PaymentTypesDBDriverH
#define PaymentTypesDBDriverH
//---------------------------------------------------------------------------

#include "PaymentTypesDriverInterface.h"

class TPaymentTypesDBDriver : public TPaymentTypesDriverInterface
{
    public:
        void LoadAllPaymentTypes(TList* types);
    private:
        void _loadAllPaymentTypes(Database::TDBTransaction &transaction, TList* types);
};
#endif
