//---------------------------------------------------------------------------

#ifndef MallExportTransactionUpdateH
#define MallExportTransactionUpdateH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include "PaymentTransaction.h"
#include "GlobalSettings.h"
#include "IBillCalculator.h"
#include "ManagerReports.h"
#include "MallExportDefines.h"
#include "MallExportUpdateAdaptor.h"

#include "ReportUtilities.h"
#include "ReportFinancialCalculations.h"
#include "Analysis.h"

using BillCalculator::TBillCalcResult;
using BillCalculator::TAX_RESULT_LIST;
using BillCalculator::DISCOUNT_RESULT_LIST;
using BillCalculator::TServiceChargeResult;
using BillCalculator::TTaxType;
//---------------------------------------------------------------------------

class TMallExportTransactionUpdate : public TMallExportUpdateAdaptor
{
public:
    TMallExportTransactionUpdate();
    ~TMallExportTransactionUpdate();

    TExportUpdateResponse UpdateTransactionExportTablesOnTransaction( TPaymentTransaction *payTransac );
    TExportUpdateResponse ResetTransactionExportTablesOnZed();

private:

    TPaymentTransaction *paymentTransaction;
    Database::TDBTransaction *dbTransaction;
    std::vector<TItemMinorComplete*> flatternedOrdersList;
    TGlobalSettings *globalSettings;

    void setPaymentTransaction( TPaymentTransaction *payTransac );
    void setDbTransaction( Database::TDBTransaction *transaction );
    void clearTransactionExportFieldValues();
    void setTransactionExportFieldValuesOnTransaction();
    void deleteTransactionExportContentsOnZed();
    void Commit();

    /* individual functions to perform calculations*/
    AnsiString extractDate();
    AnsiString extractTenantId();
    AnsiString extractCurrentDate();
    AnsiString extractTime();
    AnsiString extractTerminalNumber();
    AnsiString extractInvoiceNumber();
    AnsiString extractSalesFlag();
    Currency extractTotalGrossSales();
    Currency extractTotalSalesTax();
    Currency extractTotalNonVatSales();
    Currency extractTotalVatExemptSales();
    Currency extractTotalSeniorCitizensDiscount();
    Currency extractTotalEmployeeDiscount();
    Currency extractTotalPromoDiscount();
    Currency extractTotalOtherDiscount();
    Currency extractTotalRefund();
    Currency extractTotalServiceCharge();
    Currency extractTotalOtherSurcharge();
    Currency extractTotalNetSales();
    Currency extractGrandTotalOld();
    Currency extractGrandTotal();
    Currency extractCashSales();
    Currency extractCardSales();
    Currency extractCheckSales();
    Currency extractGCSales();
    Currency extractEPaySales();
    Currency extractOtherSales();
    Currency extractTotalCancelled();
    Currency extractTotalDeposit();
    Currency extractTotalAppliedDeposit();
    Currency extractVatPercentage();
    Currency extractTotalDiscount();
    int extractZCount();
    int extractVatableCount();
    int extractNonVatCount();
    int extractVatExemptCount();
    int extractSeniorCitizenDiscountCount();
    int extractEmployeeDiscountCount();
    int extractPromoDiscountCount();
    int extractOtherDiscountCount();
    int extractRefundCount();
    int extractServiceChargeCount();
    int extractOtherSurchargeCount();
    int extractCashSalesCount();
    int extractCardSalesCount();
    int extractCheckSalesCount();
    int extractGCSalesCount();
    int extractEPaySalesCount();
    int extractOtherSalesCount();
    int extractCancelledCount();
    int extractCancelledTransactionsCount();
    int extractDepositCount();
    int extractAppliedDepositCount();
    int extractPatronCount();
    int extractTransactionCount();
    int extractNoSaleCount();

    Currency extractTotalServiceChargeTax();
    Currency extractLocalTaxes();
    Currency extractSales();
    Currency extractTotalTaxFromResult( const TBillCalcResult &result, TTaxType taxType );

    // MallExport Fields
    TDateTime dateTime;
    AnsiString Date;
    AnsiString TenantId;
    AnsiString CurrentDate;
    AnsiString Time;
    AnsiString TerminalNumber;
    AnsiString InvoiceNumber;
    AnsiString SalesFlag;
    Currency TotalGrossSales;
    Currency TotalSalesTax;
    Currency TotalNonVatSales;
    Currency TotalVatExemptSales;
    Currency TotalSeniorCitizensDiscount;
    Currency TotalEmployeeDiscount;
    Currency TotalPromoDiscount;
    Currency TotalOtherDiscount;
    Currency TotalRefund;
    Currency TotalServiceCharge;
    Currency TotalOtherSurcharge;
    Currency TotalNetSales;
    Currency GrandTotalOld;
    Currency GrandTotal;
    Currency CashSales;
    Currency CardSales;
    Currency CheckSales;
    Currency GCSales;
    Currency EPaySales;
    Currency OtherSales;
    Currency TotalCancelled;
    Currency TotalDeposit;
    Currency TotalAppliedDeposit;
    Currency VatPercentage;
    int ZCount;
    int VatableCount;
    int NonVatCount;
    int VatExemptCount;
    int SeniorCitizenDiscountCount;
    int EmployeeDiscountCount;
    int PromoDiscountCount;
    int OtherDiscountCount;
    int RefundCount;
    int ServiceChargeCount;
    int OtherSurchargeCount;
    int CashSalesCount;
    int CardSalesCount;
    int CheckSalesCount;
    int GCSalesCount;
    int EPaySalesCount;
    int OtherSalesCount;
    int CancelledCount;
    int CancelledTransactionsCount;
    int DepositCount;
    int AppliedDepositCount;
    int PatronCount;
    int TransactionCount;
    int NoSaleCount;

    // Utility fields
    TDateTime currentDateTime;

    // Utility methods
    Currency getTotalTaxFromResult( const TBillCalcResult &result, TTaxType taxType );
    Currency getPercentageTaxFromResult( const TBillCalcResult &result, TTaxType taxType );
    Currency getPaymentGroupTotal( UnicodeString paymentGroupName );
    Currency getDiscountGroupTotal( UnicodeString discountGroupName );
    void extractLatestInvoiceNumber( AnsiString &InvoiceNumber );
    void flatternOrdersList();
    bool isRefundedOrder( const TItemMinorComplete *order );
    bool isCancelledOrder( const TItemMinorComplete *order );
    int getDiscountGroupCount( UnicodeString discountGroupName );
    int getDiscountGroupCountPerTransaction( UnicodeString discountGroupName );
    int getPaymentGroupCount( UnicodeString paymentGroupName );
};
#endif
