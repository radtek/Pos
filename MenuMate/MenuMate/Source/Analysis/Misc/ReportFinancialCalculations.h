#ifndef ReportFinancialCalculationsH
#define ReportFinancialCalculationsH
//---------------------------------------------------------------------------
#include <memory>
#include "ManagerReports.h"
#include "EnumOrderStatus.h"
#include "ReportUtilities.h"
#include "MM_DBCore.h"


//---------------------------------------------------------------------------

class ReportFinancialCalculations
{

public:
    ReportFinancialCalculations();
    ~ReportFinancialCalculations();

    Currency GetTotalSalesTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName);
    Currency GetTaxExemptSales(Database::TDBTransaction &DBTransaction);
    Currency GetServiceCharge(Database::TDBTransaction &DBTransaction, AnsiString deviceName);
    Currency GetServiceChargeTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName);
    Currency GetLocalTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName);
    Currency GetProfitTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName);
    Currency GetDiscountsAndSurcharges(Database::TDBTransaction &DBTransaction);

    TFinancialDetails GetFinancialDetails(Database::TDBTransaction &DBTransaction, TTransactionInfo &TransactionInfo,AnsiString DeviceName);
    Currency GetZeroRatedSales(Database::TDBTransaction &DBTransaction, AnsiString deviceName);

private:
    void GetBilledSalesDetail(Database::TDBTransaction &DBTransaction,TFinancialDetails &FinancialDetails,AnsiString DeviceName);
    void GetSavedSalesDetail(Database::TDBTransaction &DBTransaction,TFinancialDetails &FinancialDetails,AnsiString DeviceName);
    void GetLoyaltySalesDetail(Database::TDBTransaction &DBTransaction,TTransactionInfo &TransactionInfo,AnsiString DeviceName);
    void GetSummaSalesDetail(Database::TDBTransaction &DBTransaction,TTransactionInfo &TransactionInfo,AnsiString DeviceName);
    TDateTime GetPreviousZedTime(Database::TDBTransaction &DBTransaction);
    void GetBilledSalesQuantity(Database::TDBTransaction &DBTransaction,TFinancialDetails &FinancialDetails,AnsiString DeviceName);
    void GetSavedSalesQuantity(Database::TDBTransaction &DBTransaction,TFinancialDetails &FinancialDetails,AnsiString DeviceName);
    void GetSummaGrossSalesTotal(Database::TDBTransaction &DBTransaction,AnsiString DeviceName, TCalculatedTotals &BaseSales);
    void GetSummaNetSalesTotal(Database::TDBTransaction &DBTransaction,AnsiString DeviceName,TCalculatedTotals &BaseSales);
};

#endif
