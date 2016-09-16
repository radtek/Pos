//---------------------------------------------------------------------------

#ifndef MallExportOtherDetailsUpdateH
#define MallExportOtherDetailsUpdateH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include "PaymentTransaction.h"
#include "GlobalSettings.h"
#include "IBillCalculator.h"
#include "ManagerReports.h"
#include "MallExportDefines.h"
#include "MallExportUpdateAdaptor.h"

using BillCalculator::TBillCalcResult;
using BillCalculator::TDiscountResult;
using BillCalculator::TAX_RESULT_LIST;
using BillCalculator::DISCOUNT_RESULT_LIST;
using BillCalculator::TServiceChargeResult;
using BillCalculator::TTaxType;
//---------------------------------------------------------------------------

class TMallExportOtherDetailsUpdate : public TMallExportUpdateAdaptor
{
public:
    TMallExportOtherDetailsUpdate();
    ~TMallExportOtherDetailsUpdate();

    TExportUpdateResponse UpdateOtherDetailsExportTablesOnTransaction( TPaymentTransaction *payTransac );
    TExportUpdateResponse ResetOtherDetailsExportTablesOnZed();

    void extractCancelStatus(TDateTime value, bool CancelDiscSCDStatus, int TabKey, int patron_count);
    bool getCancelledDiscountStatus(int TabKey);

private:

    TPaymentTransaction *paymentTransaction;
    Database::TDBTransaction *dbTransaction;
    std::vector<TItemMinorComplete*> flatternedOrdersList;
    TGlobalSettings *globalSettings;

    void setPaymentTransaction( TPaymentTransaction *payTransac );
    void setDbTransaction( Database::TDBTransaction *transaction );
    void clearOtherDetailsExportFieldValues();
    void setOtherDetailsExportFieldValuesOnTransaction();
    void deleteOtherDetailsExportContentsOnZed();
    void Commit();

    AnsiString extractDate();
    AnsiString extractMallCode();
    Currency extractCancelAmount();
    int extractTransactionCount();
    int extractFineDineCustCount();
    int extractSCDiscountCount();

    AnsiString dateValue;
    AnsiString mallCode;
    AnsiString discountType;
    AnsiString transactionCode;
    AnsiString refundcancelReason;
    AnsiString paymentCode;
    AnsiString paymentCodeDesc;
    AnsiString paymentClass;
    AnsiString paymentClassDesc;
    Currency discountAmount;
    Currency refundcancelAmount;
    Currency discountPerc;
    Currency paymentAmount;
    int transactionCount;
    int finedinecustCount;
    int scdiscountCount;
    int tabKey;

    // Utility methods
    void flatternOrdersList();
    int getDiscountGroupCountPerTransaction( UnicodeString discountGroupName );
    void getDiscountGroupInfo(AnsiString &discountType, Currency &discountPerc, Currency &discountAmount, AnsiString &transactionCode);
    void extractDiscountInfo(AnsiString &discountType, Currency &discountPerc, Currency &discountAmount, AnsiString &transactionCode);
    void extractRefundInfo(AnsiString &refundcancelReason, Currency &refundcancelAmount, AnsiString &transactionCode);
    bool extractRefundStatus();
    void extractPaymentInfo(AnsiString &refundcancelReason, Currency &refundcancelAmount, AnsiString &transactionCode);
    AnsiString getPaymentClass( TPayment* payment );
    bool isRefundedOrder( const TItemMinorComplete *order );
    TDateTime getRefundQuery();
    TDateTime getCancelQuery();
};
#endif
