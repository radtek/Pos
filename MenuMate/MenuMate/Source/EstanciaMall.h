//---------------------------------------------------------------------------

#ifndef EstanciaMallH
#define EstanciaMallH
#include "MallExport.h"
//---------------------------------------------------------------------------


class TEstanciaMall : public TMallExport
{
    public:
    void PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey);
};
#endif
