//---------------------------------------------------------------------------

#ifndef EstanciaMallH
#define EstanciaMallH
#include "MallExport.h"
//---------------------------------------------------------------------------


class TEstanciaMall : public TMallExport
{
    public:
    TEstanciaMall();
    void PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey);
    void PrepareDataForExport();
    void CreateExportMedium();
};
#endif
