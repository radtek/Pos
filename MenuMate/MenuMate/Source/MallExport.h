//---------------------------------------------------------------------------

#ifndef MallExportH
#define MallExportH
#include "MallExportInterface.h"
#include "PaymentTransaction.h"
//---------------------------------------------------------------------------
class TMallExport: public TMallExportInterface
{
    protected:
    virtual void PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey) = 0;
    virtual void PrepareDataForExport() = 0;
    virtual void CreateExportMedium() = 0;
};
#endif
