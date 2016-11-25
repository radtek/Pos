//---------------------------------------------------------------------------

#ifndef MallExportH
#define MallExportH
#include "MallExportInterface.h"
#include "PaymentTransaction.h"
#include "MallExportData.h"
#include "IExporterInterface.h"
//---------------------------------------------------------------------------
class TMallExport: public TMallExportInterface
{
    protected:
    virtual std::list<TMallExportSalesData> PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey) = 0;
    virtual TMallExportPrepareData PrepareDataForExport() = 0;
    virtual IExporterInterface* CreateExportMedium() = 0;
    virtual bool InsertInToMallExport_Sales(Database::TDBTransaction &dbTransaction , std::list<TMallExportSalesData> mallExportSalesData);

    public:
    bool PushToDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey);
    bool Export();
};
#endif