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
private:
    //Check whether transaction is done before zed
    bool CheckTransactionDoneBeforeZed();

protected:

    //Prepare data for inserting into database according to mall type. child class will override it.
    virtual TMallExportSalesWrapper PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey) = 0;

    //Prepare Data For Export it will be overriden by child class.
    virtual TMallExportPrepareData PrepareDataForExport(int zKey = 0) = 0;

    //Create export medium which type of file will be exported
    virtual IExporterInterface* CreateExportMedium() = 0;

    //Insert all data into mall_export_sales table in row based pattern.
    virtual bool InsertInToMallExport_Sales(Database::TDBTransaction &dbTransaction , TMallExportSalesWrapper mallExportSalesData);

    //if sales type is assigned to items then sales total according to sales type will get stored in new table
    virtual void InsertInToMallSalesBySalesType(Database::TDBTransaction &dbTransaction , std::map<int, double> salesBySalesType, int arcBillKey);

public:

    //Overriden functions from TMallExportInterface all overrided.
    bool PushToDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey);
    bool Export();
    void RegenerateMallReport(TDateTime sDate, TDateTime eDate);
};
#endif
