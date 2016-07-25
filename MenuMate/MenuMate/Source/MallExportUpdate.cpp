//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportUpdate.h"
#include "DeviceRealTerminal.h"
#include "MM_DBCore.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// EXPORT UPDATE RESPONSE
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TExportUpdateResponse::TExportUpdateResponse()
{
    Successful = true;
    Result = -1;
    Message = "";
    Description = "";
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TMMMallExportUpdate::TMMMallExportUpdate()
{
    load( exportData );
}
//---------------------------------------------------------------------------
__fastcall TMMMallExportUpdate::~TMMMallExportUpdate()
{
    exportData.clear();
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::Load()
{
    return load( exportData );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::Commit()
{
    return commit( exportData );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::Reset()
{
    std::set<int> exceptKeys;  // keys of fields that must not be reset

    exceptKeys.insert(  1 ); // TenantNo
    exceptKeys.insert(  2 ); // TerminalNo
    exceptKeys.insert( 14 ); // TotalEndOfDayOld
    exceptKeys.insert( 15 ); // GrandTotalOld
    exceptKeys.insert( 16 ); // TotalEndOfDay
    exceptKeys.insert( 17 ); // GrandTotal
    exceptKeys.insert( 31 ); // MallCode
    exceptKeys.insert( 32 ); // ClassCode
    exceptKeys.insert( 33 ); // TradeCode
    exceptKeys.insert( 34 ); // OutletNumber
    exceptKeys.insert( 35 ); // SalesType
    exceptKeys.insert( 79 ); // SerialNumber

    exceptKeys.insert( 67 ); // BeginningInvoiceNo
    exceptKeys.insert( 68 ); // EndingInvoiceNo

    return reset( exportData, exceptKeys );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadTenantNo( AnsiString& outTenantNo )
{
    return read( 1, outTenantNo );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTenantNo( AnsiString inTenantNo )
{
    return save( 1, inTenantNo );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTerminalNo( AnsiString& outTerminalNo )
{
    return read( 2, outTerminalNo );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTerminalNo( AnsiString  inTerminalNo )
{
    return save( 2, inTerminalNo );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalGrossSale( Currency& outTotalGrossSale )
{
    return read( 3, outTotalGrossSale );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalGrossSale( Currency inTotalGrossSale )
{
    return add( 3, inTotalGrossSale );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalSaleTax( Currency& outTotalSaleTax )
{
    return read( 4, outTotalSaleTax );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalSaleTax( Currency inTotalSaleTax )
{
    return add( 4, inTotalSaleTax );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalCancelled( Currency& outTotalCancelled )
{
    return read( 5, outTotalCancelled );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalCancelled( Currency inTotalCancelled )
{
    return save( 5, inTotalCancelled );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::AddTotalCancelled( Currency inTotalCancelled )
{
    return add( 5, inTotalCancelled );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalCancelledCount( int& outTotalCancelledCount )
{
    return read( 6, outTotalCancelledCount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalCancelledCount( int  inTotalCancelledCount )
{
    return add( 6, inTotalCancelledCount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalRegularDiscount( Currency& outTotalRegularDiscount )
{
    return read( 7, outTotalRegularDiscount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalRegularDiscount( Currency  inTotalRegularDiscount )
{
    return add( 7, inTotalRegularDiscount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalRegularDiscountCount( int& outTotalRegularDiscountCount )
{
    return read( 8, outTotalRegularDiscountCount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalRegularDiscountCount( int inTotalRegularDiscountCount )
{
    return add( 8, inTotalRegularDiscountCount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalRefund( Currency& outTotalRefund )
{
    return read( 9, outTotalRefund );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalRefund( Currency inTotalRefund )
{
    return add( 9, inTotalRefund );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalRefundCount( int& outTotalRefundCount )
{
    return read( 10, outTotalRefundCount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalRefundCount( int inTotalRefundCount )
{
    return add( 10, inTotalRefundCount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalSeniorCitizenDiscount( Currency& outTotalSeniorCitizenDiscount )
{
    return read( 11, outTotalSeniorCitizenDiscount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalSeniorCitizenDiscount( Currency inTotalSeniorCitizenDiscount )
{
    return add( 11, inTotalSeniorCitizenDiscount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalSeniorCitizenDiscountCount( int& outTotalSeniorCitizenDiscountCount )
{
    return read( 12, outTotalSeniorCitizenDiscountCount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalSeniorCitizenDiscountCount( int inTotalSeniorCitizenDiscountCount )
{
    return add( 12, inTotalSeniorCitizenDiscountCount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalServiceCharge( Currency& outTotalServiceCharge )
{
    return read( 13, outTotalServiceCharge );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalServiceCharge( Currency inTotalServiceCharge )
{
    return add( 13, inTotalServiceCharge );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalEndOfDayOld( int& outTotalEndOfDayOld )
{
    return read( 14, outTotalEndOfDayOld );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalEndOfDayOld( int inTotalEndOfDayOld )
{
    return save( 14, inTotalEndOfDayOld );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadGrandTotalOld( Currency& outGrandTotalOld )
{
    return read( 15, outGrandTotalOld );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveGrandTotalOld( Currency inGrandTotalOld )
{
    return save( 15, inGrandTotalOld );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalEndOfDay( int& outTotalEndOfDay )
{
    return read( 16, outTotalEndOfDay );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalEndOfDay( int inTotalEndOfDay )
{
    return save( 16, inTotalEndOfDay );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadGrandTotal( Currency& outGrandTotal )
{
    return read( 17, outGrandTotal );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveGrandTotal( Currency inGrandTotal )
{
    return save( 17, inGrandTotal );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::AddGrandTotal( Currency inGrandTotal )
{
    return add( 17, inGrandTotal );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDate( TDateTime& outDate )
{
    return read( 18, outDate );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDate( TDateTime inDate  )
{
    return save( 18, inDate );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadNoveltySales( Currency& outNoveltySales )
{
    return read( 19, outNoveltySales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveNoveltySales( Currency inNoveltySales )
{
    return save( 19, inNoveltySales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadMiscellaneusSales( Currency& outMiscellaneusSales )
{
    return read( 20, outMiscellaneusSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveMiscellaneusSales( Currency  inMiscellaneusSales )
{
    return save( 20, inMiscellaneusSales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadLocalTaxes( Currency& outLocalTaxes )
{
    return read( 21, outLocalTaxes );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveLocalTaxes( Currency inLocalTaxes )
{
    return add( 21, inLocalTaxes );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadChargeCreditSales( Currency& outChargeCreditSales )
{
    return read( 22, outChargeCreditSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveChargeCreditSales( Currency inChargeCreditSales )
{
    return add( 22, inChargeCreditSales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadChargeCreditSalesTax( Currency& outChargeCreditSalesTax )
{
    return read( 23, outChargeCreditSalesTax );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveChargeCreditSalesTax( Currency inChargeCreditSalesTax )
{
    return add( 23, inChargeCreditSalesTax );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTaxExemptSales( Currency& outTaxExemptSales )
{
    return read( 24, outTaxExemptSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTaxExemptSales( Currency inTaxExemptSales )
{
    return add( 24, inTaxExemptSales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadPharmaSales( Currency& outPharmaSales )
{
    return read( 25, outPharmaSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SavePharmaSales( Currency inPharmaSales )
{
    return save( 25, inPharmaSales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadNonPharmaSales( Currency& outNonPharmaSales )
{
    return read( 26, outNonPharmaSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveNonPharmaSales( Currency  inNonPharmaSales )
{
    return save( 26, inNonPharmaSales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDisabilityDiscount( Currency& outDisabilityDiscount )
{
    return read( 27, outDisabilityDiscount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDisabilityDiscount( Currency  inDisabilityDiscount )
{
    return add( 27, inDisabilityDiscount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadSalesNotSubjectToRent( Currency& outSalesNotSubjectToRent )
{
    return read( 28, outSalesNotSubjectToRent );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveSalesNotSubjectToRent( Currency  inSalesNotSubjectToRent )
{
    return save( 28, inSalesNotSubjectToRent );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalReprintedTransc( Currency& outTotalReprintedTransc )
{
    return read( 29, outTotalReprintedTransc );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalReprintedTransc( Currency  inTotalReprintedTransc )
{
    return save( 29, inTotalReprintedTransc );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::AddTotalReprintedTransc( Currency  inTotalReprintedTransc )
{
    return add( 29, inTotalReprintedTransc );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalReprintedTranscCount( int& outTotalReprintedTranscCount )
{
    return read( 30, outTotalReprintedTranscCount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalReprintedTranscCount( int  inTotalReprintedTranscCount )
{
    return save( 30, inTotalReprintedTranscCount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::AddTotalReprintedTranscCount( int  inTotalReprintedTranscCount )
{
    return add( 30, inTotalReprintedTranscCount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadMallCode( AnsiString& outMallCode )
{
    return read( 31, outMallCode );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveMallCode( AnsiString  inMallCode )
{
    return save( 31, inMallCode );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadClassCode( AnsiString& outClassCode )
{
    return read( 32, outClassCode );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveClassCode( AnsiString  inClassCode )
{
    return save( 32, inClassCode );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTradeCode( AnsiString& outTradeCode )
{
    return read( 33, outTradeCode );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTradeCode( AnsiString  inTradeCode )
{
    return save( 33, inTradeCode );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadOutletNumber( AnsiString& outOutletNumber )
{
    return read( 34, outOutletNumber );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveOutletNumber( AnsiString  inOutletNumber )
{
    return save( 34, inOutletNumber );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadSalesType( AnsiString& outSalesType )
{
    return read( 35, outSalesType );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveSalesType( AnsiString  inSalesType )
{
   return save( 35, inSalesType );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadEmployeeDiscount( Currency& outEmployeeDiscount )
{
    return read( 36, outEmployeeDiscount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveEmployeeDiscount( Currency inEmployeeDiscount )
{
   return add( 36, inEmployeeDiscount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadVIPDiscount( Currency& outVIPDiscount )
{
    return read( 37, outVIPDiscount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveVIPDiscount( Currency  inVIPDiscount )
{
   return add( 37, inVIPDiscount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadGPCDiscount( Currency& outGPCDiscount )
{
    return read( 38, outGPCDiscount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveGPCDiscount( Currency  inGPCDiscount )
{
   return add( 38, inGPCDiscount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDiscountG1( Currency& outDiscountG1 )
{
    return read( 39, outDiscountG1 );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDiscountG1( Currency  inDiscountG1 )
{
   return add( 39, inDiscountG1 );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDiscountG2( Currency& outDiscountG2 )
{
    return read( 40, outDiscountG2 );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDiscountG2( Currency  inDiscountG2 )
{
   return add( 40, inDiscountG2 );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDiscountG3( Currency& outDiscountG3 )
{
    return read( 41, outDiscountG3 );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDiscountG3( Currency  inDiscountG3 )
{
   return add( 41, inDiscountG3 );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDiscountG4( Currency& outDiscountG4 )
{
    return read( 42, outDiscountG4 );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDiscountG4( Currency  inDiscountG4 )
{
   return add( 42, inDiscountG4 );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDiscountG5( Currency& outDiscountG5 )
{
    return read( 43, outDiscountG5 );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDiscountG5( Currency  inDiscountG5 )
{
   return add( 43, inDiscountG5 );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDiscountG6( Currency& outDiscountG6 )
{
    return read( 44, outDiscountG6 );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDiscountG6( Currency  inDiscountG6 )
{
   return add( 44, inDiscountG6 );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadOtherTax( Currency& outOtherTax )
{
    return read( 45, outOtherTax );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveOtherTax( Currency  inOtherTax )
{
   return add( 45, inOtherTax );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadAdjustments( Currency& outAdjustments )
{
    return read( 46, outAdjustments );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveAdjustments( Currency  inAdjustments )
{
   return save( 46, inAdjustments );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadPositiveAdjustments( Currency& outPositiveAdjustments )
{
    return read( 47, outPositiveAdjustments );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SavePositiveAdjustments( Currency  inPositiveAdjustments )
{
   return save( 47, inPositiveAdjustments );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadNegativeAdjustments( Currency& outNegativeAdjustments )
{
    return read( 48, outNegativeAdjustments );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveNegativeAdjustments( Currency  inNegativeAdjustments )
{
   return save( 48, inNegativeAdjustments );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadPositiveAdjustmentsNonTax( Currency& outPositiveAdjustmentsNonTax )
{
    return read( 49, outPositiveAdjustmentsNonTax );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SavePositiveAdjustmentsNonTax( Currency  inPositiveAdjustmentsNonTax )
{
   return save( 49, inPositiveAdjustmentsNonTax );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadNegativeAdjustmentsNonTax( Currency& outNegativeAdjustmentsNonTax )
{
    return read( 50, outNegativeAdjustmentsNonTax );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveNegativeAdjustmentsNonTax( Currency  inNegativeAdjustmentsNonTax )
{
   return save( 50, inNegativeAdjustmentsNonTax );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDailySales( Currency& outDailySales )
{
    return read( 51, outDailySales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDailySales( Currency  inDailySales )
{
   return add( 51, inDailySales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTaxInclusiveSales( Currency& outTaxInclusiveSales )
{
    return read( 52, outTaxInclusiveSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTaxInclusiveSales( Currency  inTaxInclusiveSales )
{
   return add( 52, inTaxInclusiveSales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadCashSales( Currency& outCashSales )
{
    return read( 53, outCashSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveCashSales( Currency  inCashSales )
{
   return add( 53, inCashSales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadGiftChequeSales( Currency& outGiftChequeSales )
{
    return read( 54, outGiftChequeSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveGiftChequeSales( Currency  inGiftChequeSales )
{
   return add( 54, inGiftChequeSales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDebitCardSales( Currency& outDebitCardSales )
{
    return read( 55, outDebitCardSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDebitCardSales( Currency  inDebitCardSales )
{
   return add( 55, inDebitCardSales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadOtherTender( Currency& outOtherTender )
{
    return read( 56, outOtherTender );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveOtherTender( Currency  inOtherTender )
{
   return add( 56, inOtherTender );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalMastercard( Currency& outTotalMastercard )
{
    return read( 57, outTotalMastercard );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalMastercard( Currency  inTotalMastercard )
{
   return add( 57, inTotalMastercard );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalVisa( Currency& outTotalVisa )
{
    return read( 58, outTotalVisa );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalVisa( Currency  inTotalVisa )
{
   return add( 58, inTotalVisa );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalAmericanExpress( Currency& outTotalAmericanExpress )
{
    return read( 59, outTotalAmericanExpress );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalAmericanExpress( Currency  inTotalAmericanExpress )
{
   return add( 59, inTotalAmericanExpress );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotallDiners( Currency& outTotallDiners )
{
    return read( 60, outTotallDiners );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotallDiners( Currency  inTotallDiners )
{
   return add( 60, inTotallDiners );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalJCB( Currency& outTotalJCB )
{
    return read( 61, outTotalJCB );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalJCB( Currency  inTotalJCB )
{
   return add( 61, inTotalJCB );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalOtherCards( Currency& outTotalOtherCards )
{
    return read( 62, outTotalOtherCards );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalOtherCards( Currency  inTotalOtherCards )
{
   return add( 62, inTotalOtherCards );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadOtherCharges( Currency& outOtherCharges )
{
    return read( 63, outOtherCharges );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveOtherCharges( Currency  inOtherCharges )
{
   return add( 63, inOtherCharges );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadFirstTransaction( int& outFirstTransaction )
{
    return read( 64, outFirstTransaction );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveFirstTransaction( int  inFirstTransaction )
{
   return save( 64, inFirstTransaction );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadLastTransaction( int& outLastTransaction )
{
    return read( 65, outLastTransaction );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveLastTransaction( int  inLastTransaction )
{
   return add( 65, inLastTransaction );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTransactionCount( int& outTransactionCount )
{
    return read( 66, outTransactionCount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTransactionCount( int  inTransactionCount  )
{
   return add( 66, inTransactionCount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadBeginningInvoiceNo( AnsiString& outBeginningInvoiceNo )
{
    return read( 67, outBeginningInvoiceNo );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveBeginningInvoiceNo( AnsiString  inBeginningInvoiceNo )
{
   return save( 67, inBeginningInvoiceNo );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadEndingInvoiveNo( AnsiString& outEndingInvoiveNo )
{
    return read( 68, outEndingInvoiveNo );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveEndingInvoiveNo( AnsiString  inEndingInvoiveNo  )
{
   return save( 68, inEndingInvoiveNo );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadCashTransactions( int& outCashTransactions )
{
    return read( 69, outCashTransactions );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveCashTransactions( int  inCashTransactions )
{
   return add( 69, inCashTransactions );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadGiftCardChequeTransactions( int& outGiftCardChequeTransactions )
{
    return read( 70, outGiftCardChequeTransactions );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveGiftCardChequeTransactions( int  inGiftCardChequeTransactions )
{
   return add( 70, inGiftCardChequeTransactions );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDebitCardTransactions( int& outDebitCardTransactions )
{
    return read( 71, outDebitCardTransactions );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDebitCardTransactions( int  inDebitCardTransactions )
{
   return add( 71, inDebitCardTransactions );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadOtherTenderTransctions( int& outOtherTenderTransctions )
{
    return read( 72, outOtherTenderTransctions );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveOtherTenderTransctions( int  inOtherTenderTransctions )
{
   return add( 72, inOtherTenderTransctions );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadMasterCardTransactions( int& outMasterCardTransactions )
{
    return read( 73, outMasterCardTransactions );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveMasterCardTransactions( int  inMasterCardTransactions )
{
   return add( 73, inMasterCardTransactions );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadVisaTransactions( int& outVisaTransactions )
{
    return read( 74, outVisaTransactions );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveVisaTransactions( int  inVisaTransactions )
{
   return add( 74, inVisaTransactions );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadAmericanExpressTransactions( int& outAmericanExpressTransactions )
{
    return read( 75, outAmericanExpressTransactions );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveAmericanExpressTransactions( int  inAmericanExpressTransactions )
{
   return add( 75, inAmericanExpressTransactions );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadDinersTransactions( int& outDinersTransactions )
{
    return read( 76, outDinersTransactions );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDinersTransactions( int  inDinersTransactions )
{
   return add( 76, inDinersTransactions );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadJCBTransactions( int& outJCBTransactions )
{
    return read( 77, outJCBTransactions );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveJCBTransactions( int  inJCBTransactions )
{
   return add( 77, inJCBTransactions );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadOtherCardTransactions( int& outOtherCardTransactions )
{
    return read( 78, outOtherCardTransactions );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveOtherCardTransactions( int  inOtherCardTransactions )
{
    return add( 78, inOtherCardTransactions );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadSerialNumber( AnsiString& outSerialNumber )
{
    return read( 79, outSerialNumber );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveSerialNumber( AnsiString  inSerialNumber )
{
    return save( 79, inSerialNumber );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTransactionTime( TDateTime& outTransactionTime )
{
    return read( 80, outTransactionTime );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTransactionTime( TDateTime  inTransactionTime )
{
    return save( 80, inTransactionTime );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTransactionDate( TDateTime& outTransactionDate )
{
    return read( 81, outTransactionDate );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTransactionDate( TDateTime  inTransactionDate )
{
    return save( 81, inTransactionDate );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalHour( AnsiString& outHour )
{
    return read( 82, outHour );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalHour( AnsiString  inHour )
{
    return save( 82, inHour );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTotalDiscount( Currency& outTotalDiscount )
{
    return read( 83, outTotalDiscount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalDiscount( Currency  inTotalDiscount )
{
    return add( 83, inTotalDiscount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadBeginningORNo( AnsiString& outBeginningORNo )
{
    return read( 84, outBeginningORNo );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveBeginningORNo( AnsiString  inBeginningORNo )
{
    return save( 84, inBeginningORNo );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadEndingORNo( AnsiString& outEndingORNo )
{
    return read( 85, outEndingORNo );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveEndingORNo( AnsiString  inEndingORNo )
{
    return save( 85, inEndingORNo );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadLocalTaxExemptDailySales( Currency& outLocalTaxExemptDailySales )
{
    return read( 86, outLocalTaxExemptDailySales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveLocalTaxExemptDailySales( Currency  inLocalTaxExemptDailySales )
{
    return add( 86, inLocalTaxExemptDailySales );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadFineDiningCustomerCount( int& outFineDiningCustomerCount )
{
    return read( 87, outFineDiningCustomerCount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveFineDiningCustomerCount( int  inFineDiningCustomerCount )
{
    return add( 87, inFineDiningCustomerCount );
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::ReadTenderSurcharges( Currency& outTenderSurcharges )
{
    return read( 88, outTenderSurcharges );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTenderSurcharges( Currency  inTenderSurcharges )
{
    return add( 88, inTenderSurcharges );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadDepartmentSum( Currency& outDepartmentSum )
{
    return read( 89, outDepartmentSum );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDepartmentSum( Currency  inDepartmentSum )
{
    return add( 89, inDepartmentSum );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadTotalNonVatSales( Currency& outTotalNonVatSales )
{
    return read( 90, outTotalNonVatSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalNonVatSales( Currency inTotalNonVatSales )
{
    return add( 90, inTotalNonVatSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadTotalCheckSales( Currency& outTotalCheckSales )
{
    return read( 91, outTotalCheckSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalCheckSales( Currency inTotalCheckSales )
{
    return add( 91, inTotalCheckSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadTotalEPaySales( Currency& outTotalEPaySales )
{
    return read( 92, outTotalEPaySales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalEPaySales( Currency inTotalEPaySales )
{
    return add( 92, inTotalEPaySales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadTotalNoSales( Currency& outTotalNoSales )
{
    return read( 93, outTotalNoSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalNoSales( Currency inTotalNoSales )
{
    return add( 93, inTotalNoSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadPreviousEODCounter( int& outPreviousEODCounter )
{
    return read( 94, outPreviousEODCounter );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SavePreviousEODCounter( int  inPreviousEODCounter )
{
    return save( 94, inPreviousEODCounter );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadCurrentEODCounter( int& outCurrentEODCounter )
{
    return read( 95, outCurrentEODCounter );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveCurrentEODCounter( int  inCurrentEODCounter )
{
    return save( 95, inCurrentEODCounter );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadDiscountCount( int& outDiscountCount )
{
    return read( 96, outDiscountCount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveDiscountCount( int  inDiscountCount )
{
    return add( 96, inDiscountCount );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadTotalCardSales( Currency& outTotalCardSales )
{
    return read( 97, outTotalCardSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalCardSales( Currency inTotalCardSales )
{
    return add( 97, inTotalCardSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadTotalOtherSales( Currency& outTotalOtherSales )
{
    return read( 98, outTotalOtherSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveTotalOtherSales( Currency inTotalOtherSales )
{
    return add( 98, inTotalOtherSales );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadSalesTypeFood( Currency& outSalesTypeFood )
{
    return read( 99, outSalesTypeFood );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveSalesTypeFood( Currency inSalesTypeFood )
{
    return add( 99, inSalesTypeFood );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadSalesTypeNonFood( Currency& outSalesTypeNonFood )
{
    return read( 100, outSalesTypeNonFood );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveSalesTypeNonFood( Currency inSalesTypeNonFood )
{
    return add( 100, inSalesTypeNonFood );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadSalesTypeGroceries( Currency& outSalesTypeGroceries )
{
    return read( 101, outSalesTypeGroceries );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveSalesTypeGroceries( Currency inSalesTypeGroceries )
{
    return add( 101, inSalesTypeGroceries );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadSalesTypeMedicines( Currency& outSalesTypeMedicines )
{
    return read( 102, outSalesTypeMedicines );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveSalesTypeMedicines( Currency inSalesTypeMedicines )
{
    return add( 102, inSalesTypeMedicines );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::ReadSalesTypeOthers( Currency& outSalesTypeOthers )
{
    return read( 103, outSalesTypeOthers );
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::SaveSalesTypeOthers( Currency inSalesTypeOthers )
{
    return add( 103, inSalesTypeOthers );
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TExportUpdateResponse TMMMallExportUpdate::read( int inID, AnsiString& outStr )
{
    outStr = exportData[inID].StrData;
    return TExportUpdateResponse::TExportUpdateResponse();
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::save( int inID, AnsiString  inStr  )
{
    exportData[inID].StrData = inStr;
    return TExportUpdateResponse::TExportUpdateResponse();
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::read( int inID, int& outInt )
{
    outInt = exportData[inID].IntData;
    return TExportUpdateResponse::TExportUpdateResponse();
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::save( int inID, int  inInt  )
{
    exportData[inID].IntData = inInt;
    return TExportUpdateResponse::TExportUpdateResponse();
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::add( int inID, int  inInt  )
{
    exportData[inID].IntData += inInt;
    return TExportUpdateResponse::TExportUpdateResponse();
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::read( int inID, Currency& outCurrency )
{
    outCurrency = exportData[inID].CurrencyData;
    return TExportUpdateResponse::TExportUpdateResponse();
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::save( int inID, Currency  inCurrency  )
{
    exportData[inID].CurrencyData = inCurrency;
    return TExportUpdateResponse::TExportUpdateResponse();
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::add( int inID, Currency  inCurrency  )
{
    exportData[inID].CurrencyData += inCurrency;
    return TExportUpdateResponse::TExportUpdateResponse();
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::read( int inID, TDateTime& outDatetime )
{
    outDatetime = exportData[inID].DatetimeData;
    return TExportUpdateResponse::TExportUpdateResponse();
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::save( int inID, TDateTime  inDatetime  )
{
    exportData[inID].DatetimeData = inDatetime;
    return TExportUpdateResponse::TExportUpdateResponse();
}
//---------------------------------------------------------------------------

TExportUpdateResponse TMMMallExportUpdate::load(   MALL_EXPORT_DATA& outData )
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TExportUpdateResponse response;

    try
    {
        TIBSQL *selectQuery = DBTransaction.Query( DBTransaction.AddQuery() );

        selectQuery->SQL->Text = "SELECT * FROM MALLEXPORT ORDER BY MALLEXPORT_KEY";
        selectQuery->ExecQuery();

        int i;
        for (i = 1; !selectQuery->Eof; selectQuery->Next(), i++)
        {
            switch (fieldTypes[i-1])
            {
                case medtString:
                    outData[i].StrData = selectQuery->FieldByName("STRING_VALUE")->AsString;
                    break;
                case medtInt:
                    outData[i].IntData = selectQuery->FieldByName("INTEGER_VALUE")->AsInteger;
                    break;
                case medtCurrency:
                    outData[i].CurrencyData = selectQuery->FieldByName("CURRENCY_VALUE")->AsCurrency;
                    break;
                case medtDatetime:
                    outData[i].DatetimeData = selectQuery->FieldByName("TIMESTAMP_VALUE")->AsDateTime;
                    break;
                default:
                    break;
            }
        }

        DBTransaction.Commit();
    }
    catch (Exception &e)
    {
        DBTransaction.Rollback();
        response.Successful = false;
        response.Message = e.Message;
    }

    return response;
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::commit( MALL_EXPORT_DATA &inData  )
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TExportUpdateResponse response;

    try
    {
        TIBSQL *updateQuery = DBTransaction.Query( DBTransaction.AddQuery() );
        for (int i = 1; i <= inData.size(); i++)
        {
            UnicodeString valueType = "";
            TMallExportDataType fieldType = fieldTypes[i-1];

            switch (fieldType)
            {
                case medtString:
                    valueType = "STRING_VALUE";
                    break;
                case medtInt:
                    valueType = "INTEGER_VALUE";
                    break;
                case medtCurrency:
                    valueType = "CURRENCY_VALUE";
                    break;
                case medtDatetime:
                    valueType = "TIMESTAMP_VALUE";
                    break;
                default:
                    break;
            }

            updateQuery->SQL->Text = "UPDATE MALLEXPORT "
                                     "SET " + valueType + " = :" + valueType + " "
                                     "WHERE MALLEXPORT_KEY = :MALLEXPORT_KEY";

            updateQuery->ParamByName("MALLEXPORT_KEY")->AsInteger = (i);

            switch (fieldType)
            {
                case medtString:
                    updateQuery->ParamByName(valueType)->AsString = inData[i].StrData;
                    break;
                case medtInt:
                    updateQuery->ParamByName(valueType)->AsInteger = inData[i].IntData;
                    break;
                case medtCurrency:
// ----------------------------------------------------------------
                    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
                    {
                        updateQuery->ParamByName(valueType)->AsCurrency = RoundToNearest(
                                                                            inData[i].CurrencyData,
                                                                            0.01,
                                                                            TGlobalSettings::Instance().MidPointRoundsDown);
                    }
                    else
                    {
                        updateQuery->ParamByName(valueType)->AsCurrency = inData[i].CurrencyData;
                    }

// ----------------------------------------------------------------
                    break;
                case medtDatetime:
                    updateQuery->ParamByName(valueType)->AsDateTime = inData[i].DatetimeData;
                    break;
                default:
                    break;
            }

            updateQuery->ExecQuery();
        }

        updateQuery->Close();
        DBTransaction.Commit();
    }
    catch (Exception &e)
    {
        DBTransaction.Rollback();
        response.Successful = false;
        response.Message = e.Message;
    }

    return response;
}
//---------------------------------------------------------------------------
TExportUpdateResponse TMMMallExportUpdate::reset( MALL_EXPORT_DATA &inData, std::set<int> inExceptKeys )
{
    //todo: needs to be modified when multi terminal scenario comes into play
    TExportUpdateResponse response;

    try
    {
        AnsiString terminalNoInExportTable = "";
        ReadTerminalNo( terminalNoInExportTable );

        AnsiString terminalNoInSettings = AnsiString( TGlobalSettings::Instance().TerminalNo );
        bool       isCurrentTerminal    = terminalNoInExportTable.Trim().UpperCase() == terminalNoInSettings.Trim().UpperCase();

        if( !isCurrentTerminal )
            return response;

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::

        for (int i = 1; i <= inData.size(); i++)
        {
            bool ignoreKey = valueInSet( i, inExceptKeys );

            if( !ignoreKey )
            {
                TMallExportDataType fieldType = fieldTypes[i-1];

                switch( fieldType )
                {
                    case medtString:   inData[i].StrData      = ""; break;
                    case medtInt:      inData[i].IntData      = 0;  break;
                    case medtCurrency: inData[i].CurrencyData = 0;  break;

                    default: break;
                }
            }
        }

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::

        inData[67].IntData = inData[68].IntData; // BeginningInvoiceNo = EndingInvoiceNo
        inData[68].IntData = 0;                  // EndingInvoiceNo    = 0
    }
    catch (Exception &e)
    {
        response.Successful = false;
        response.Message = e.Message;
        response.Description = "Failed to reset fields to default values";
    }

    return response;
}
//---------------------------------------------------------------------------
bool TMMMallExportUpdate::valueInSet(
                int key,
                std::set<int>           inValueSet )
{
    return inValueSet.find( key ) != inValueSet.end();
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE CONSTANTS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//const int TMMMallExportUpdate::NUMBER_OF_EXPORT_FIELDS = 87;

const TMallExportDataType TMMMallExportUpdate::fieldTypes[] =
    {
            medtString,
            medtString,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtInt,
            medtCurrency,
            medtInt,
            medtCurrency,
            medtInt,
            medtCurrency,
            medtInt,
            medtCurrency,
            medtInt,
            medtCurrency,
            medtInt,
            medtCurrency,
            medtDatetime,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtInt,
            medtString,
            medtString,
            medtString,
            medtString,
            medtString,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtInt,
            medtInt,
            medtInt,
            medtString,
            medtString,
            medtInt,    // number of cash transactions
            medtInt,
            medtInt,
            medtInt,
            medtInt,
            medtInt,
            medtInt,
            medtInt,
            medtInt,
            medtInt,    // number of other card transactions
            medtString,
            medtDatetime,
            medtDatetime,
            medtString,
            medtCurrency,
            medtString,
            medtString,
            medtCurrency,
            medtInt,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtInt,
            medtInt,
            medtInt,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency,
            medtCurrency
        };
//---------------------------------------------------------------------------

