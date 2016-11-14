//---------------------------------------------------------------------------

#ifndef MallExportH
#define MallExportH
#include "MallExportInterface.h"
#include "PaymentTransaction.h"
#include "MallExportData.h"
//---------------------------------------------------------------------------
class TMallExport: public TMallExportInterface
{
    protected:
    virtual TMallExportPrepareData PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey) = 0;
    virtual void PrepareDataForExport() = 0;
    virtual void CreateExportMedium() = 0;

    public:
    bool PushToDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey);
    bool Export();
};
#endif
