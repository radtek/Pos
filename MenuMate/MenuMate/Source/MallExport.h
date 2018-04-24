//---------------------------------------------------------------------------

#ifndef MallExportH
#define MallExportH
#include "MallExportInterface.h"
#include "PaymentTransaction.h"
#include "MallExportData.h"
#include "IExporterInterface.h"
#include <DateUtils.hpp>
//---------------------------------------------------------------------------

struct TMallExportDiscount
{
    double scdDiscount;
    double pwdDiscount;
    double otherDiscount;
};


class TMallExport: public TMallExportInterface
{
private:
    //Check whether transaction is done before zed
    bool CheckTransactionDoneBeforeZed();

    //Get OldAccumulated Sale
    double GetOldAccumulatedSales(int fieldIndex);

    //Generate SalesKey for MallExport_sales Table
    long GenerateSaleKey(Database::TDBTransaction &dbTransaction);

    TDateTime BilledTimeStamp;

protected:

    //Prepare data for inserting into database according to mall type. child class will override it.
    virtual TMallExportSalesWrapper PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime) = 0;

    //Prepare Data For Export it will be overriden by child class.
    virtual TMallExportPrepareData PrepareDataForExport(int zKey = 0) = 0;

    //Create export medium which type of file will be exported
    virtual IExporterInterface* CreateExportMedium() = 0;

    //Insert all data into mall_export_sales table in row based pattern.
    virtual bool InsertInToMallExport_Sales(Database::TDBTransaction &dbTransaction , TMallExportSalesWrapper mallExportSalesData);

    //if sales type is assigned to items then sales total according to sales type will get stored in new table
    virtual void InsertInToMallSalesBySalesType(Database::TDBTransaction &dbTransaction , std::map<int, double> salesBySalesType, int arcBillKey,
                                                TDateTime billedTime);

    //Insert field in to list so that it can be inserted in db later.
    virtual void PushFieldsInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, UnicodeString field,
                    UnicodeString dataType, UnicodeString fieldValue, int fieldIndex, int arcBillKey);

    //will return type of file which will be exported.
    virtual UnicodeString GetExportType(int mallid);

    //Getting old accumulated total.
    virtual double GetOldAccumulatedSales(Database::TDBTransaction &dbTransaction, int fieldIndex,int mallid);
    
	//prepare SCD, PWD and others discount
	virtual TMallExportDiscount PrepareDiscounts(Database::TDBTransaction &dbTransaction, TItemMinorComplete *order);

	//Get Max Zed Key Present in mall Table..
    virtual int GetMaxZedKey(Database::TDBTransaction &dbTransaction,int mallkey, int zKey = 0);

public:

    //Overriden functions from TMallExportInterface all overrided.
    bool PushToDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime);
    bool Export();
    void RegenerateMallReport(TDateTime sDate, TDateTime eDate);
};
#endif
