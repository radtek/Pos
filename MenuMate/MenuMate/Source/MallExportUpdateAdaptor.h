//---------------------------------------------------------------------------

#ifndef MallExportUpdateAdaptorH
#define MallExportUpdateAdaptorH
//---------------------------------------------------------------------------

#include "PaymentTransaction.h"
#include "GlobalSettings.h"
#include "MallExportUpdate.h"
#include "IBillCalculator.h"
#include "ManagerReports.h"
#include "MallExportDefines.h"
#include "MallExportSalesType.h"

using BillCalculator::TBillCalcResult;
using BillCalculator::TAX_RESULT_LIST;
using BillCalculator::DISCOUNT_RESULT_LIST;
using BillCalculator::TServiceChargeResult;
using BillCalculator::TTaxType;

/*
 * TMallExportUpdateAdaptor
 * Responsbile for gathering all the required information for mall exports
 * Uses TMallExportUpdate class to put the information to database
 */

class TMallExportUpdateAdaptor
{
    private:
        TPaymentTransaction *paymentTransaction;
        Database::TDBTransaction *dbTransaction;
        TGlobalSettings *globalSettings;
        std::auto_ptr<TMMMallExportUpdate> mallExportUpdate;
        std::vector<TItemMinorComplete*> flatternedOrdersList;
        std::map<UnicodeString, UnicodeString> DataRead; // Temporary

        void setPaymentTransaction( TPaymentTransaction *payTransac );
        void setDbTransaction( Database::TDBTransaction *transaction );
        void clearExportFieldValues();
        void setMallExportFieldValuesOnTransaction();
        void updateMallExportFieldValuesOnTransaction();
        void setMallExportFieldValuesOnZed( TFinancialDetails financialDetails );
        void updateMallExportFieldValuesOnZed();
        void updateMallExportFieldValuesOnReprint( Currency reprintValue );
        void updateMallExportFieldValuesOnSettingsChange();
        void resetMallExportFieldValuesOnZed();

        /* individual functions to perform calculations*/
        AnsiString extractTerminalNumber();
        AnsiString extractTenantId();
        Currency extractTotalGrossSales();
        Currency extractTotalSalesTax();
        Currency extractTotalCancelled();
        int extractTotalCancelledCount();
        Currency extractTotalRegularDiscount();
        int extractTotalRegularDiscountCount();  //todo: verify with niko
        Currency extractTotalRefund();
        int extractTotalRefundCount();
        Currency extractTotalSeniorCitizensDiscount();  //todo: verify with niko
        int extractTotalSeniorCitizensDiscountCount();  //todo: verify with niko
        Currency extractTotalServiceCharge();
        int extractTotalEndOfDayOld();
        Currency extractGrandTotalOld();
        int extractTotalEndOfDay();
        Currency extractGrandTotal( TFinancialDetails financialDetails );
        TDateTime extractDateTime();
        Currency extractNoveltySales();
        Currency extractMiscellaneousSales();
        Currency extractLocalTaxes();
        Currency extractChargeCreditSales();
        Currency extractChargeCreditSalesTax();
        Currency extractTaxExemptSales();
        Currency extractPharmaSales();
        Currency extractNonPharmaSales();
        Currency extractDisabilityDiscount();
        Currency extractSalesNotSubjectToRent();
        Currency extractTotalReprintedTransc();
        int extractTotalReprintedTranscCount();
        AnsiString extractMallCode();
        AnsiString extractClassCode();
        AnsiString extractTradeCode();
        AnsiString extractOutletNumber();
        AnsiString extractSalesType();
        Currency extractEmployeeDiscount();
        Currency extractVipDiscount();
        Currency extractGpcDiscount();
        Currency extractDiscountGroup1();
        Currency extractDiscountGroup2();
        Currency extractDiscountGroup3();
        Currency extractDiscountGroup4();
        Currency extractDiscountGroup5();
        Currency extractDiscountGroup6();
        Currency extractOtherTax();
        Currency extractAdjustments();
        Currency extractPositiveAdjustments();
        Currency extractNegativeAdjustments();
        Currency extractPositiveAdjustmentsNonTax();
        Currency extractNegativeAdjustmentsNonTax();
        Currency extractDailySales();
        Currency extractTaxInclusiveSales();
        Currency extractCashSales();
        Currency extractGiftChequeSales();
        Currency extractDebitCardSales();
        Currency extractOtherTender();
        Currency extractTotalMasterCard();
        Currency extractTotalVisa();
        Currency extractTotalAmericanExpress();
        Currency extractTotalDiners();
        Currency extractTotalJCB();
        Currency extractTotalOtherCards();
        Currency extractOtherCharges();
        int extractFirstTransaction();
        int extractLastTransaction();
        int extractTransactionCount();
        AnsiString extractBeginingInvoiceNumber();
        AnsiString extractEndingInvoiceNumber();
        int extractCashTransactions();
        int extractGiftCardChequeTransactions();
        int extractDebitCardTransactions();
        int extractOtherTenderTransactions();
        int extractMasterCardTransactions();
        int extractVisaTransactions();
        int extractAmericanExpressTransactions();
        int extractDinersTransactions();
        int extractJcbTransactions();
        int extractOtherCardTransactions();
        AnsiString extractSerialNumber();
        TDateTime extractTransactionTime();
        TDateTime extractTransactionDate();
        AnsiString extractTotalHour();
        Currency extractTotalDiscount();
        AnsiString extractBeginningORNo();
        AnsiString extractEndingORNo();
        Currency extractLocalTaxExemptDailySales();
        int extractFineDiningCustomerCount();
        Currency extractTenderSurcharges();
        Currency extractDepartmentSum();
        Currency extractTotalServiceChargeTax();
        Currency extractTotalNonVatSales();
        Currency extractTotalCheckSales();
        Currency extractTotalEPaySales();
        Currency extractTotalNoSales();
        Currency extractTotalCardSales();
        Currency extractTotalOtherSales();
        int extractPreviousEODCounter();
        int extractCurrentEODCounter();
        int extractDiscountCount();
        Currency extractTotalSalesTypeFood();
        Currency extractTotalSalesTypeNonFood();
        Currency extractTotalSalesTypeGroceries();
        Currency extractTotalSalesTypeMedicines();
        Currency extractTotalSalesTypeOthers();
        Currency extractTotalRefundAmount();

        /* data update methods */
        void setTerminalNumber();
        void setTenantId();
        void setTotalGrossSales();
        void setTotalSalesTax();
        void setTotalCancelled();
        void setTotalCancelledCount();
        void setTotalRegularDiscount();
        void setTotalRegularDiscountCount();
        void setTotalRefund();
        void setTotalRefundCount();
        void setTotalSeniorCitizensDiscount();
        void setTotalSeniorCitizensDiscountCount();
        void setTotalServiceCharge();
        void setTotalEndOfDayOld();
        void setGrandTotalOld();
        void setTotalEndOfDay();
        void setGrandTotal();
        void setDateTime();
        void setNoveltySales(); //------------ IMPLEMENT !!! -----------------
        void setMiscellaneousSales();
        void setLocalTaxes();
        void setChargeCreditSales();
        void setChargeCreditSalesTax();
        void setTaxExemptSales();
        void setPharmaSales();
        void setNonPharmaSales();
        void setDisabilityDiscount();
        void setSalesNotSubjectToRent();
        void setTotalReprintedTransc();
        void setTotalReprintedTranscCount();
        void setMallCode();
        void setClassCode();
        void setTradeCode();
        void setOutletNumber();
        void setSalesType();
        void setEmployeeDiscount();
        void setVipDiscount();
        void setGpcDiscount();
        void setDiscountGroup1();
        void setDiscountGroup2();
        void setDiscountGroup3();
        void setDiscountGroup4();
        void setDiscountGroup5();
        void setDiscountGroup6();
        void setOtherTax();
        void setAdjustments();
        void setPositiveAdjustments();
        void setNegativeAdjustments();
        void setPositiveAdjustmentsNonTax();
        void setNegativeAdjustmentsNonTax();
        void setDailySales();
        void setTaxInclusiveSales();
        void setCashSales();
        void setGiftChequeSales();
        void setDebitCardSales();
        void setOtherTender();
        void setTotalMasterCard();
        void setTotalVisa();
        void setTotalAmericanExpress();
        void setTotalDiners();
        void setTotalJCB();
        void setTotalOtherCards();
        void setOtherCharges();
        void setFirstTransaction();
        void setLastTransaction();
        void setTransactionCount();
        void setBeginingInvoiceNumber();
        void setEndingInvoiceNumber();
        void setCashTransactions();
        void setGiftCardChequeTransactions();
        void setDebitCardTransactions();
        void setOtherTenderTransactions();
        void setMasterCardTransactions();
        void setVisaTransactions();
        void setAmericanExpressTransactions();
        void setDinersTransactions();
        void setJcbTransactions();
        void setOtherCardTransactions();
        void setSerialNumber();
        void setTransactionTime();
        void setTransactionDate();
        void setTotalHour();
        void setTotalDiscount();
        void setBeginningORNo();
        void setEndingORNo();
        void setLocalTaxExemptDailySales();
        void setFineDiningCustomerCount();
        void setTenderSurcharges();
        void setDepartmentSum();
        void setTotalNonVatSales();
        void setTotalCheckSales();
        void setTotalEPaySales();
        void setTotalNoSales();
        void setTotalCardSales();
        void setTotalOtherSales();
        void setPreviousEODCounter();
        void setCurrentEODCounter();
        void setDiscountCount();
        void setSalesTypeFood();
        void setSalesTypeNonFood();
        void setSalesTypeGroceries();
        void setSalesTypeMedicines();
        void setSalesTypeOthers();
        //todo: add more functions as needed

        // Utility functions to extract information
        void flatternOrdersList();
        Currency getTotalTaxFromResult( const TBillCalcResult &result );
        Currency getTotalTaxFromResult( const TBillCalcResult &result, TTaxType taxType );
        Currency getTotalTaxExemptSalesExeptSC();
        Currency getTotalTaxZeroRatedSalesExeptSC();
        Currency getTotalTaxInclusiveSalesExeptSC();
        bool isRefundedOrder( const TItemMinorComplete *order );
        bool isCancelledOrder( const TItemMinorComplete *order );
        Currency getTotalChargeSales();
        Currency getTotalPaymentByCredit();
        Currency getTotalOtherTender();
        Currency getPaymentGroupTotal( UnicodeString paymentGroupName );
        int getPaymentGroupCount( UnicodeString paymentGroupName );
        Currency getRegularDiscountGroupsTotal();
        Currency getDiscountGroupTotal( UnicodeString discountGroupName );
        int getDiscountGroupCount( UnicodeString discountGroupName );
        int getDiscountGroupCountPerTransaction( UnicodeString discountGroupName );
        int getRegularDiscountGroupCountPerTransaction();
        void extractBeginingAndEndingInvoiceNumbers( AnsiString &beginInvoiceNum, AnsiString &endInvoiceNum );
        Currency getDailySales();
        Currency getDailySalesLocalTaxes();
        void getSalesTypeQuery(int Item_ID);
        Currency getSalesTypeGroupTotal( UnicodeString salestypeGroupName );

        // Utility fields
        TDateTime currentDateTime;
        std::vector<UnicodeString> stName;

        // MallExport Fields
        AnsiString terminalNumber;
        AnsiString tenantId;
        Currency totalGrossSales;
        Currency totalSalesTax;
        Currency totalCancelled;
        int totalCancelledCount;
        Currency totalRegularDiscount;
        Currency totalRegularDiscountCount;
        Currency totalRefund;
        int totalRefundCount;
        Currency totalSeniorCitizensDiscount;
        int totalSeniorCitizensDiscountCount;
        Currency totalServiceCharge;
        int totalEndOfDayOld;
        Currency grandTotalOld;
        int totalEndOfDay;
        Currency grandTotal;
        TDateTime dateTime;
        Currency noveltySales;  //------------ IMPLEMENT !!! -----------------
        Currency miscellaneousSales;
        Currency localTaxes;
        Currency chargeCreditSales;
        Currency chargeCreditSalesTax;
        Currency taxExemptSales;
        Currency pharmaSales;
        Currency nonPharmaSales;
        Currency disabilityDiscount;
        Currency salesNotSubjectToRent;
        Currency totalReprintedTransc;
        int totalReprintedTranscCount;
        AnsiString mallCode;
        AnsiString classCode;
        AnsiString tradeCode;
        AnsiString outletNumber;
        AnsiString salesType;
        Currency employeeDiscount;
        Currency vipDiscount;
        Currency gpcDiscount;
        Currency discountGroup1;
        Currency discountGroup2;
        Currency discountGroup3;
        Currency discountGroup4;
        Currency discountGroup5;
        Currency discountGroup6;
        Currency otherTax;
        Currency adjustments;
        Currency positiveAdjustments;
        Currency negativeAdjustments;
        Currency positiveAdjustmentsNonTax;
        Currency negativeAdjustmentsNonTax;
        Currency dailySales;
        Currency taxInclusiveSales;
        Currency cashSales;
        Currency giftChequeSales;
        Currency debitCardSales;
        Currency otherTender;
        Currency totalMasterCard;
        Currency totalVisa;
        Currency totalAmericanExpress;
        Currency totalDiners;
        Currency totalJCB;
        Currency totalOtherCards;
        Currency otherCharges;
        int firstTransaction;
        int lastTransaction;
        int transactionCount;
        AnsiString beginingInvoiceNumber;
        AnsiString endingInvoiceNumber;
        int cashTransactionsCount;
        int giftCardChequeTransactionsCount;
        int debitCardTransactionsCount;
        int otherTenderTransactionsCount;
        int masterCardTransactionsCount;
        int visaTransactionsCount;
        int americanExpressTransactionsCount;
        int dinersTransactionsCount;
        int jcbTransactionsCount;
        int otherCardTransactionsCount;
        AnsiString serialNumber;
        TDateTime transactionTime;  // whats the difference ?
        TDateTime transactionDate;
        AnsiString totalHour;
        Currency totalDiscount;
        AnsiString beginningORNo;
        AnsiString endingORNo;
        Currency localTaxExemptDailySales;
        int fineDiningCustomerCount;
        Currency tenderSurcharges;
        Currency departmentSum;
        Currency totalServiceChargeTax;
        Currency totalNonVatSales;
        Currency totalCheckSales;
        Currency totalEPaySales;
        Currency totalNoSales;
        Currency totalCardSales;
        Currency totalOtherSales;
        int previousEODCounter;
        int currentEODCounter;
        int discountCount;
        Currency totalSalesTypeFood;
        Currency totalSalesTypeNonFood;
        Currency totalSalesTypeGroceries;
        Currency totalSalesTypeMedicines;
        Currency totalSalesTypeOthers;

    public:
        TMallExportUpdateAdaptor();
        ~TMallExportUpdateAdaptor();

        TExportUpdateResponse UpdateExportTablesOnTransaction( TPaymentTransaction *payTransac );
        TExportUpdateResponse UpdateExportTablesOnZed( TFinancialDetails financialDetails, Database::TDBTransaction *dbTransaction );
        TExportUpdateResponse UpdateExportTableOnReprint( Currency reprintValue, Database::TDBTransaction *dbTransaction );
        TExportUpdateResponse UpdateExportTableOnSettingsChange();

        TExportUpdateResponse ResetExportTables();
};

#endif

