//---------------------------------------------------------------------------

#ifndef MallExportHourlyUpdateH
#define MallExportHourlyUpdateH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include "PaymentTransaction.h"
#include "GlobalSettings.h"
#include "IBillCalculator.h"
#include "ManagerReports.h"
#include "MallExportDefines.h"
#include "MallExportUpdateAdaptor.h"

using BillCalculator::TBillCalcResult;
using BillCalculator::TAX_RESULT_LIST;
using BillCalculator::DISCOUNT_RESULT_LIST;
using BillCalculator::TServiceChargeResult;
using BillCalculator::TTaxType;
//---------------------------------------------------------------------------

class TMallExportHourlyUpdate : public TMallExportUpdateAdaptor
{
public:
    TMallExportHourlyUpdate();
    ~TMallExportHourlyUpdate();

    TExportUpdateResponse UpdateHourlyExportTablesOnTransaction( TPaymentTransaction *payTransac );
    TExportUpdateResponse ResetHourlyExportTablesOnZed();

private:

    TPaymentTransaction *paymentTransaction;
    Database::TDBTransaction *dbTransaction;
    std::vector<TItemMinorComplete*> flatternedOrdersList;
    TGlobalSettings *globalSettings;

    Currency extractTotalGrossSales();
    Currency extractTotalSalesTax();
    Currency extractTotalServiceCharge();
    Currency extractTotalServiceChargeTax();
    Currency extractLocalTaxes();
    Currency extractTotalTaxFromResult( const TBillCalcResult &result, TTaxType taxType );
    Currency extractTotalDiscount();
    Currency extractServiceChargeAmount();
    Currency extractRefundAmount();
    Currency extractVoidAmount();
    Currency extractSCDiscountAmount();

    AnsiString extractTerminalNumber();
    AnsiString extractMallCode();
    AnsiString extractDate();
    AnsiString extractTime();
    AnsiString extractTenantId();
    Currency extractSales();
    int extractTransactionCount();
    int extractRefundCount();
    int extractVoidCount();
    int extractPatronCount();

    Currency getDiscountGroupTotal( UnicodeString discountGroupName );
    Currency getDailySales();
    bool isRefundedOrder( const TItemMinorComplete *order );
    bool isCancelledOrder( const TItemMinorComplete *order );
    bool PerformedLastZed();

    void setPaymentTransaction( TPaymentTransaction *payTransac );
    void setDbTransaction( Database::TDBTransaction *transaction );
    void clearHourlyExportFieldValues();
    void setHourlyExportFieldValuesOnTransaction();
    void deleteHourlyExportContentsOnZed();
    void Commit();

    Currency totalGrossSales;
    Currency totalSalesTax;
    Currency totalServiceCharge;
    Currency totalServiceChargeTax;
    Currency localTaxes;
    Currency vatSales;
    Currency totalDiscount;
    Currency servchargeAmount;
    Currency refundAmount;
    Currency voidAmount;
    Currency scdiscountAmount;


    AnsiString terminalNumber;
    AnsiString mallCode;
    AnsiString dateValue;
    AnsiString timeValue;
    AnsiString tenantId;
    Currency salesValue;
    int transactionCount;
    int refundCount;
    int voidCount;
    int patronCount;

    // Utility methods
    void flatternOrdersList();
};
#endif
