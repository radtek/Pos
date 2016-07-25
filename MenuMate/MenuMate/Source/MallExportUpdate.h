//---------------------------------------------------------------------------

#ifndef MallExportUpdateH
#define MallExportUpdateH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <vector>
#include <map>
#include <set>

//---------------------------------------------------------------------------

// The types are in the same order as they are in the parser code
enum TMallExportDataType { medtString, medtInt, medtCurrency, medtDatetime };

struct TMallExportData
{
    AnsiString StrData;
    int        IntData;
    Currency   CurrencyData;
    TDateTime  DatetimeData;
};

typedef std::map<__int32, TMallExportData> MALL_EXPORT_DATA;

//::::::::::::::::::::::::::::::::::::::::::::::::::

struct TExportUpdateResponse
{
    TExportUpdateResponse();

    bool Successful;
    int  Result;
    AnsiString Message;
    AnsiString Description;
};

//::::::::::::::::::::::::::::::::::::::::::::::::::

class TMMMallExportUpdate : public TObject
{
public:
    TMMMallExportUpdate();
    virtual __fastcall ~TMMMallExportUpdate();

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    // METHODS
    TExportUpdateResponse Load();
    TExportUpdateResponse Commit();
    TExportUpdateResponse Reset();  // intended to use for resetting fields on zed

    TExportUpdateResponse ReadTenantNo( AnsiString& outTenantNo );
    TExportUpdateResponse SaveTenantNo( AnsiString  inTenantNo );

    TExportUpdateResponse ReadTerminalNo( AnsiString& outTerminalNo );
    TExportUpdateResponse SaveTerminalNo( AnsiString  inTerminalNo );

    TExportUpdateResponse ReadTotalGrossSale( Currency& outTotalGrossSale );
    TExportUpdateResponse SaveTotalGrossSale( Currency  inTotalGrossSale );

    TExportUpdateResponse ReadTotalSaleTax( Currency& outTotalSaleTax );
    TExportUpdateResponse SaveTotalSaleTax( Currency  inTotalSaleTax );

    TExportUpdateResponse ReadTotalCancelled( Currency& outTotalCancelled );
    TExportUpdateResponse SaveTotalCancelled( Currency  inTotalCancelled );
    TExportUpdateResponse AddTotalCancelled(  Currency  inTotalCancelled );

    TExportUpdateResponse ReadTotalCancelledCount( int& outTotalCancelledCount );
    TExportUpdateResponse SaveTotalCancelledCount( int  inTotalCancelledCount );

    TExportUpdateResponse ReadTotalRegularDiscount( Currency& outTotalRegularDiscount );
    TExportUpdateResponse SaveTotalRegularDiscount( Currency  inTotalRegularDiscount );

    TExportUpdateResponse ReadTotalRegularDiscountCount( int& outTotalRegularDiscountCount );
    TExportUpdateResponse SaveTotalRegularDiscountCount( int  inTotalRegularDiscountCount );

    TExportUpdateResponse ReadTotalRefund( Currency& outTotalRefund );
    TExportUpdateResponse SaveTotalRefund( Currency  inTotalRefund );

    TExportUpdateResponse ReadTotalRefundCount( int& outTotalRefundCount );
    TExportUpdateResponse SaveTotalRefundCount( int  inTotalRefundCount );

    TExportUpdateResponse ReadTotalSeniorCitizenDiscount( Currency& outTotalSeniorCitizenDiscount );
    TExportUpdateResponse SaveTotalSeniorCitizenDiscount( Currency  inTotalSeniorCitizenDiscount );

    TExportUpdateResponse ReadTotalSeniorCitizenDiscountCount( int& outTotalSeniorCitizenDiscountCount );
    TExportUpdateResponse SaveTotalSeniorCitizenDiscountCount( int  inTotalSeniorCitizenDiscountCount );

    TExportUpdateResponse ReadTotalServiceCharge( Currency& outTotalServiceCharge );
    TExportUpdateResponse SaveTotalServiceCharge( Currency  inTotalServiceCharge );

    TExportUpdateResponse ReadTotalEndOfDayOld( int& outTotalEndOfDayOld );
    TExportUpdateResponse SaveTotalEndOfDayOld( int  inTotalEndOfDayOld );

    TExportUpdateResponse ReadGrandTotalOld( Currency& outGrandTotalOld );
    TExportUpdateResponse SaveGrandTotalOld( Currency  inGrandTotalOld );

    TExportUpdateResponse ReadTotalEndOfDay( int& outTotalEndOfDay );
    TExportUpdateResponse SaveTotalEndOfDay( int  inTotalEndOfDay );

    TExportUpdateResponse ReadGrandTotal( Currency& outGrandTotal );
    TExportUpdateResponse SaveGrandTotal( Currency  inGrandTotal );
    TExportUpdateResponse AddGrandTotal( Currency  inGrandTotal );

    TExportUpdateResponse ReadDate( TDateTime& outDate );
    TExportUpdateResponse SaveDate( TDateTime  inDate  );

    TExportUpdateResponse ReadNoveltySales( Currency& outNoveltySales );
    TExportUpdateResponse SaveNoveltySales( Currency  inNoveltySales );

    TExportUpdateResponse ReadMiscellaneusSales( Currency& outMiscellaneusSales );
    TExportUpdateResponse SaveMiscellaneusSales( Currency  inMiscellaneusSales );

    TExportUpdateResponse ReadLocalTaxes( Currency& outLocalTaxes );
    TExportUpdateResponse SaveLocalTaxes( Currency  inLocalTaxes );

    TExportUpdateResponse ReadChargeCreditSales( Currency& outChargeCreditSales );
    TExportUpdateResponse SaveChargeCreditSales( Currency  inChargeCreditSales );

    TExportUpdateResponse ReadChargeCreditSalesTax( Currency& outChargeCreditSalesTax );
    TExportUpdateResponse SaveChargeCreditSalesTax( Currency  inChargeCreditSalesTax );

    TExportUpdateResponse ReadTaxExemptSales( Currency& outTaxExemptSales );
    TExportUpdateResponse SaveTaxExemptSales( Currency  inTaxExemptSales );

    TExportUpdateResponse ReadPharmaSales( Currency& outPharmaSales );
    TExportUpdateResponse SavePharmaSales( Currency  inPharmaSales );

    TExportUpdateResponse ReadNonPharmaSales( Currency& outNonPharmaSales );
    TExportUpdateResponse SaveNonPharmaSales( Currency  inNonPharmaSales );

    TExportUpdateResponse ReadDisabilityDiscount( Currency& outDisabilityDiscount );
    TExportUpdateResponse SaveDisabilityDiscount( Currency  inDisabilityDiscount );

    TExportUpdateResponse ReadSalesNotSubjectToRent( Currency& outSalesNotSubjectToRent );
    TExportUpdateResponse SaveSalesNotSubjectToRent( Currency  inSalesNotSubjectToRent );

    TExportUpdateResponse ReadTotalReprintedTransc( Currency& outTotalReprintedTransc );
    TExportUpdateResponse SaveTotalReprintedTransc( Currency  inTotalReprintedTransc );
    TExportUpdateResponse AddTotalReprintedTransc( Currency  inTotalReprintedTransc );


    TExportUpdateResponse ReadTotalReprintedTranscCount( int& outTotalReprintedTranscCount );
    TExportUpdateResponse SaveTotalReprintedTranscCount( int  inTotalReprintedTranscCount );
    TExportUpdateResponse AddTotalReprintedTranscCount( int  inTotalReprintedTranscCount );


    TExportUpdateResponse ReadMallCode( AnsiString& outMallCode );
    TExportUpdateResponse SaveMallCode( AnsiString  inMallCode );

    TExportUpdateResponse ReadClassCode( AnsiString& outClassCode );
    TExportUpdateResponse SaveClassCode( AnsiString  inClassCode );

    TExportUpdateResponse ReadTradeCode( AnsiString& outTradeCode );
    TExportUpdateResponse SaveTradeCode( AnsiString  inTradeCode );

    TExportUpdateResponse ReadOutletNumber( AnsiString& outOutletNumber );
    TExportUpdateResponse SaveOutletNumber( AnsiString  inOutletNumber );

    TExportUpdateResponse ReadSalesType( AnsiString& outSalesType );
    TExportUpdateResponse SaveSalesType( AnsiString  inSalesType );

    TExportUpdateResponse ReadEmployeeDiscount( Currency& outEmployeeDiscount );
    TExportUpdateResponse SaveEmployeeDiscount( Currency  inEmployeeDiscount );

    TExportUpdateResponse ReadVIPDiscount( Currency& outVIPDiscount );
    TExportUpdateResponse SaveVIPDiscount( Currency  inVIPDiscount );

    TExportUpdateResponse ReadGPCDiscount( Currency& outGPCDiscount );
    TExportUpdateResponse SaveGPCDiscount( Currency  inGPCDiscount );

    TExportUpdateResponse ReadDiscountG1( Currency& outDiscountG1 );
    TExportUpdateResponse SaveDiscountG1( Currency  inDiscountG1 );

    TExportUpdateResponse ReadDiscountG2( Currency& outDiscountG2 );
    TExportUpdateResponse SaveDiscountG2( Currency  inDiscountG2 );

    TExportUpdateResponse ReadDiscountG3( Currency& outDiscountG3 );
    TExportUpdateResponse SaveDiscountG3( Currency  inDiscountG3 );

    TExportUpdateResponse ReadDiscountG4( Currency& outDiscountG4 );
    TExportUpdateResponse SaveDiscountG4( Currency  inDiscountG4 );

    TExportUpdateResponse ReadDiscountG5( Currency& outDiscountG5 );
    TExportUpdateResponse SaveDiscountG5( Currency  inDiscountG5 );

    TExportUpdateResponse ReadDiscountG6( Currency& outDiscountG6 );
    TExportUpdateResponse SaveDiscountG6( Currency  inDiscountG6 );

    TExportUpdateResponse ReadOtherTax( Currency& outOtherTax );
    TExportUpdateResponse SaveOtherTax( Currency  inOtherTax );

    TExportUpdateResponse ReadAdjustments( Currency& outAdjustments );
    TExportUpdateResponse SaveAdjustments( Currency  inAdjustments );

    TExportUpdateResponse ReadPositiveAdjustments( Currency& outPositiveAdjustments );
    TExportUpdateResponse SavePositiveAdjustments( Currency  inPositiveAdjustments );

    TExportUpdateResponse ReadNegativeAdjustments( Currency& outNegativeAdjustments );
    TExportUpdateResponse SaveNegativeAdjustments( Currency  inNegativeAdjustments );

    TExportUpdateResponse ReadPositiveAdjustmentsNonTax( Currency& outPositiveAdjustmentsNonTax );
    TExportUpdateResponse SavePositiveAdjustmentsNonTax( Currency  inPositiveAdjustmentsNonTax );

    TExportUpdateResponse ReadNegativeAdjustmentsNonTax( Currency& outNegativeAdjustmentsNonTax );
    TExportUpdateResponse SaveNegativeAdjustmentsNonTax( Currency  inNegativeAdjustmentsNonTax );

    TExportUpdateResponse ReadDailySales( Currency& outDailySales );
    TExportUpdateResponse SaveDailySales( Currency  inDailySales );

    TExportUpdateResponse ReadTaxInclusiveSales( Currency& outTaxInclusiveSales );
    TExportUpdateResponse SaveTaxInclusiveSales( Currency  inTaxInclusiveSales );

    TExportUpdateResponse ReadCashSales( Currency& outCashSales );
    TExportUpdateResponse SaveCashSales( Currency  inCashSales );

    TExportUpdateResponse ReadGiftChequeSales( Currency& outGiftChequeSales );
    TExportUpdateResponse SaveGiftChequeSales( Currency  inGiftChequeSales );

    TExportUpdateResponse ReadDebitCardSales( Currency& outDebitCardSales );
    TExportUpdateResponse SaveDebitCardSales( Currency  inDebitCardSales );

    TExportUpdateResponse ReadOtherTender( Currency& outOtherTender );
    TExportUpdateResponse SaveOtherTender( Currency  inOtherTender );

    TExportUpdateResponse ReadTotalMastercard( Currency& outTotalMastercard );
    TExportUpdateResponse SaveTotalMastercard( Currency  inTotalMastercard );

    TExportUpdateResponse ReadTotalVisa( Currency& outTotalVisa );
    TExportUpdateResponse SaveTotalVisa( Currency  inTotalVisa );

    TExportUpdateResponse ReadTotalAmericanExpress( Currency& outTotalAmericanExpress );
    TExportUpdateResponse SaveTotalAmericanExpress( Currency  inTotalAmericanExpress );

    TExportUpdateResponse ReadTotallDiners( Currency& outTotallDiners );
    TExportUpdateResponse SaveTotallDiners( Currency  inTotallDiners );

    TExportUpdateResponse ReadTotalJCB( Currency& outTotalJCB );
    TExportUpdateResponse SaveTotalJCB( Currency  inTotalJCB );

    TExportUpdateResponse ReadTotalOtherCards( Currency& outTotalOtherCards );
    TExportUpdateResponse SaveTotalOtherCards( Currency  inTotalOtherCards );

    TExportUpdateResponse ReadOtherCharges( Currency& outOtherCharges );
    TExportUpdateResponse SaveOtherCharges( Currency  inOtherCharges );

    TExportUpdateResponse ReadFirstTransaction( int& outFirstTransaction );
    TExportUpdateResponse SaveFirstTransaction( int  inFirstTransaction );

    TExportUpdateResponse ReadLastTransaction( int& outLastTransaction );
    TExportUpdateResponse SaveLastTransaction( int  inLastTransaction );

    TExportUpdateResponse ReadTransactionCount( int& outTransactionCount );
    TExportUpdateResponse SaveTransactionCount( int  inTransactionCount  );

    TExportUpdateResponse ReadBeginningInvoiceNo( AnsiString& outBeginningInvoiceNo );
    TExportUpdateResponse SaveBeginningInvoiceNo( AnsiString  inBeginningInvoiceNo );

    TExportUpdateResponse ReadEndingInvoiveNo( AnsiString& outEndingInvoiveNo );
    TExportUpdateResponse SaveEndingInvoiveNo( AnsiString  inEndingInvoiveNo  );

    TExportUpdateResponse ReadCashTransactions( int& outCashTransactions );
    TExportUpdateResponse SaveCashTransactions( int  inCashTransactions );

    TExportUpdateResponse ReadGiftCardChequeTransactions( int& outGiftCardChequeTransactions );
    TExportUpdateResponse SaveGiftCardChequeTransactions( int  inGiftCardChequeTransactions );

    TExportUpdateResponse ReadDebitCardTransactions( int& outDebitCardTransactions );
    TExportUpdateResponse SaveDebitCardTransactions( int  inDebitCardTransactions );

    TExportUpdateResponse ReadOtherTenderTransctions( int& outOtherTenderTransctions );
    TExportUpdateResponse SaveOtherTenderTransctions( int  inOtherTenderTransctions );

    TExportUpdateResponse ReadMasterCardTransactions( int& outMasterCardTransactions );
    TExportUpdateResponse SaveMasterCardTransactions( int  inMasterCardTransactions );

    TExportUpdateResponse ReadVisaTransactions( int& outVisaTransactions );
    TExportUpdateResponse SaveVisaTransactions( int  inVisaTransactions );

    TExportUpdateResponse ReadAmericanExpressTransactions( int& outAmericanExpressTransactions );
    TExportUpdateResponse SaveAmericanExpressTransactions( int  inAmericanExpressTransactions );

    TExportUpdateResponse ReadDinersTransactions( int& outDinersTransactions );
    TExportUpdateResponse SaveDinersTransactions( int  inDinersTransactions );

    TExportUpdateResponse ReadJCBTransactions( int& outJCBTransactions );
    TExportUpdateResponse SaveJCBTransactions( int  inJCBTransactions );

    TExportUpdateResponse ReadOtherCardTransactions( int& outOtherCardTransactions );
    TExportUpdateResponse SaveOtherCardTransactions( int  inOtherCardTransactions );

    TExportUpdateResponse ReadSerialNumber( AnsiString& outSerialNumber );
    TExportUpdateResponse SaveSerialNumber( AnsiString  inSerialNumber );

    TExportUpdateResponse ReadTransactionTime( TDateTime& outTransactionTime );
    TExportUpdateResponse SaveTransactionTime( TDateTime  inTransactionTime );

    TExportUpdateResponse ReadTransactionDate( TDateTime& outTransactionDate );
    TExportUpdateResponse SaveTransactionDate( TDateTime  inTransactionDate );

    TExportUpdateResponse ReadTotalHour( AnsiString& outHour );
    TExportUpdateResponse SaveTotalHour( AnsiString  inHour );

    TExportUpdateResponse ReadTotalDiscount( Currency& outTotalDiscount );
    TExportUpdateResponse SaveTotalDiscount( Currency  inTotalDiscount );

    TExportUpdateResponse ReadBeginningORNo( AnsiString& outBeginningORNo );
    TExportUpdateResponse SaveBeginningORNo( AnsiString  inBeginningORNo );

    TExportUpdateResponse ReadEndingORNo( AnsiString& outEndingORNo );
    TExportUpdateResponse SaveEndingORNo( AnsiString  inEndingORNo );

    TExportUpdateResponse ReadLocalTaxExemptDailySales( Currency& outLocalTaxExemptDailySales );
    TExportUpdateResponse SaveLocalTaxExemptDailySales( Currency  inLocalTaxExemptDailySales );

    TExportUpdateResponse ReadFineDiningCustomerCount( int& outFineDiningCustomerCount );
    TExportUpdateResponse SaveFineDiningCustomerCount( int  inFineDiningCustomerCount );

    TExportUpdateResponse ReadTenderSurcharges( Currency& outTenderSurcharges );
    TExportUpdateResponse SaveTenderSurcharges( Currency inTenderSurcharges );

    TExportUpdateResponse ReadDepartmentSum( Currency& outDepartmentSum );
    TExportUpdateResponse SaveDepartmentSum( Currency inDepartmentSum );

    TExportUpdateResponse ReadTotalNonVatSales( Currency& outTotalNonVatSales );
    TExportUpdateResponse SaveTotalNonVatSales( Currency inTotalNonVatSales );

    TExportUpdateResponse ReadTotalCheckSales( Currency& outTotalCheckSales );
    TExportUpdateResponse SaveTotalCheckSales( Currency inTotalCheckSales );

    TExportUpdateResponse ReadTotalEPaySales( Currency& outTotalEPaySales );
    TExportUpdateResponse SaveTotalEPaySales( Currency inTotalEPaySales );

    TExportUpdateResponse ReadTotalNoSales( Currency& outTotalNoSales );
    TExportUpdateResponse SaveTotalNoSales( Currency inTotalNoSales );

    TExportUpdateResponse ReadTotalCardSales( Currency& outTotalCardSales );
    TExportUpdateResponse SaveTotalCardSales( Currency inTotalCardSales );

    TExportUpdateResponse ReadTotalOtherSales( Currency& outTotalOtherSales );
    TExportUpdateResponse SaveTotalOtherSales( Currency inTotalOtherSales );

    TExportUpdateResponse ReadPreviousEODCounter( int& outPreviousEODCounter );
    TExportUpdateResponse SavePreviousEODCounter( int  inPreviousEODCounter );

    TExportUpdateResponse ReadCurrentEODCounter( int& outCurrentEODCounter );
    TExportUpdateResponse SaveCurrentEODCounter( int  inCurrentEODCounter );

    TExportUpdateResponse ReadDiscountCount( int& outDiscountCount );
    TExportUpdateResponse SaveDiscountCount( int  inDiscountCount );

    TExportUpdateResponse ReadSalesTypeFood( Currency& outSalesTypeFood );
    TExportUpdateResponse SaveSalesTypeFood( Currency  inSalesTypeFood );

    TExportUpdateResponse ReadSalesTypeNonFood( Currency& outSalesTypeNonFood );
    TExportUpdateResponse SaveSalesTypeNonFood( Currency  inSalesTypeNonFood );

    TExportUpdateResponse ReadSalesTypeGroceries( Currency& outSalesTypeGroceries );
    TExportUpdateResponse SaveSalesTypeGroceries( Currency  inSalesTypeGroceries );

    TExportUpdateResponse ReadSalesTypeMedicines( Currency& outSalesTypeMedicines );
    TExportUpdateResponse SaveSalesTypeMedicines( Currency  inSalesTypeMedicines );

    TExportUpdateResponse ReadSalesTypeOthers( Currency& outSalesTypeOthers );
    TExportUpdateResponse SaveSalesTypeOthers( Currency  inSalesTypeOthers );

protected:
    AnsiString       exportDataLoadSQL;
    AnsiString       exportDataCommitSQL;
    MALL_EXPORT_DATA exportData;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TExportUpdateResponse read( int inID, AnsiString& outStr );
    TExportUpdateResponse save( int inID, AnsiString  inStr  );

    TExportUpdateResponse read( int inID, int& outInt );
    TExportUpdateResponse save( int inID, int  inInt  );
    TExportUpdateResponse add ( int inID, int  inInt  );

    TExportUpdateResponse read( int inID, Currency& outCurrency );
    TExportUpdateResponse save( int inID, Currency  inCurrency  );
    TExportUpdateResponse add ( int inID, Currency  inCurrency  );

    TExportUpdateResponse read( int inID, TDateTime& outDatetime );
    TExportUpdateResponse save( int inID, TDateTime  inDatetime  );

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TExportUpdateResponse load(   MALL_EXPORT_DATA& outData );
    TExportUpdateResponse commit( MALL_EXPORT_DATA  &inData  );

    TExportUpdateResponse reset( MALL_EXPORT_DATA& inData, std::set<int> inExceptKeys );

    bool valueInSet( int key, std::set<int> inValueSet );

private:

    static const TMallExportDataType fieldTypes[];
};

//---------------------------------------------------------------------------
#endif

