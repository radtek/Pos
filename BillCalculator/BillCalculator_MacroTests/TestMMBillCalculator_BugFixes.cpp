//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "TestMMBillCalculator_BugFixes.h"

#define ERR_Total "Final price is wrong"
#define ERR_BasePrice "Base price is wrong"
#define ERR_DiscountX "Discount content is wrong"
#define ERR_Discounts "Discounts total is wrong"
#define ERR_Surcharges "Surcharges total is wrong"
#define ERR_ServiceCharge "Service charge is wrong"
#define ERR_ServiceChargeTax "Service charge tax is wrong"
#define ERR_Tax "Total tax is wrong"
#define ERR_TaxContent "Tax content is wrong"

__fastcall TTestTMMBillCalculator_BugFixes::TTestTMMBillCalculator_BugFixes(System::String name) : TTestCase(name)
{
}
//------------------------------------------------------------------------------

__fastcall TTestTMMBillCalculator_BugFixes::~TTestTMMBillCalculator_BugFixes()
{
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_BugFixes::SetUp()
{
    // no common setup
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_BugFixes::TearDown()
{
    // no common teardown
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_BugFixes::Test_TaxIncItem_TBD_SCAD_SCTEP_DISC()
{
    TPriceInfo priceInfoObj;
    resetPriceInfo( &priceInfoObj );

    priceInfoObj.Value = 112;
    priceInfoObj.TaxBeforeDiscount = true;

    applyTaxToPriceInfo( priceInfoObj, "SalesTax", 12, 1 );
    applyDiscountToPriceInfo( priceInfoObj, "50% regular", dtDiscount, dwPercentage, 50, 1, 0, false );
    applyServiceChargeToPriceInfo( priceInfoObj, 10, 12 );

    execute( priceInfoObj );

    CheckEquals( 12.60, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 12.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.60, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( -56.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 61.60, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_BugFixes::Test_TaxExcItem_TBD_SCAD_SCTEP_DISC()
{
    TPriceInfo priceInfoObj;
    resetPriceInfo( &priceInfoObj );

    priceInfoObj.Value = 100;
    priceInfoObj.TaxBeforeDiscount = true;
    priceInfoObj.TaxInclusive = false;

    applyTaxToPriceInfo( priceInfoObj, "SalesTax", 12, 1 );
    applyDiscountToPriceInfo( priceInfoObj, "50% regular", dtDiscount, dwPercentage, 50, 1, 0, false );
    applyServiceChargeToPriceInfo( priceInfoObj, 10, 12 );

    execute( priceInfoObj );

    CheckEquals( 12.60, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 12.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.60, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 67.60, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_BugFixes::resetPriceInfo( TPriceInfo* priceInfoObj )
{
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->TaxExclusiveServiceCharge = true;

    priceInfoObj->TaxBeforeDiscount = false;
    priceInfoObj->ServiceChargeBeforeDiscount = false;

    priceInfoObj->MultiDiscountType = mdtSimple;
    priceInfoObj->MultiTaxType      = mttSimple;

    priceInfoObj->TaxInfoList.clear();
    priceInfoObj->DiscountInfoList.clear();

    priceInfoObj->ServiceChargeInfo.Percentage  = 0;
    priceInfoObj->ServiceChargeInfo.TaxPercentage = 0;

    priceInfoObj->PriceTaxExempt = false;
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_BugFixes::execute( TPriceInfo &priceInfo )
{
    billCalcResponseObj = billCalculator.Execute( &priceInfo );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_BugFixes::applyTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage, int priority )
{
    TTaxInfo tax;
    tax.Name = name;
    tax.Percentage = percentage;
    tax.Priority = priority;

    priceInfo.TaxInfoList.push_back( tax );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_BugFixes::applyServiceChargeToPriceInfo( TPriceInfo &priceInfo, Currency scPerc, Currency scTaxPerc )
{
    priceInfo.ServiceChargeInfo.Percentage = scPerc;
    priceInfo.ServiceChargeInfo.TaxPercentage = scTaxPerc;
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_BugFixes::applyDiscountToPriceInfo( TPriceInfo &priceInfo, string name, TDiscountType discType, TDiscountWay mode, Currency percentage, int priority, Currency value, bool isTaxExclusive )
{
    TDiscountInfo discInfo;
    discInfo.Name = name;
    discInfo.DiscountType = discType;
    discInfo.DiscountWay = mode;
    discInfo.TaxExclusiveDiscount = isTaxExclusive;
    discInfo.Percentage = percentage;
    discInfo.Value = value;
    discInfo.Priority = priority;

    priceInfo.DiscountInfoList.push_back( discInfo );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_BugFixes::applySCDToPriceInfo( TPriceInfo &priceInfo, Currency percentage )
{
    TDiscountInfo discInfo;
    discInfo.Name = "SCD";
    discInfo.DiscountType = dtDiscount;
    discInfo.DiscountWay = dwPercentage;
    discInfo.Percentage = percentage;

    priceInfo.DiscountInfoList.push_back( discInfo );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_BugFixes::applyPWDToPriceInfo( TPriceInfo &priceInfo, Currency percentage )
{
    TDiscountInfo discInfo;
    discInfo.Name = "PWD";
    discInfo.DiscountType = dtDiscount;
    discInfo.DiscountWay = dwPercentage;
    discInfo.Percentage = percentage;
    discInfo.RecalcPriceWithTaxAfterDiscount = true;
    discInfo.TaxExclusiveDiscount = true;

    priceInfo.DiscountInfoList.push_back( discInfo );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_BugFixes::calculateValuesForItemSet(
                                                    std::vector<TPriceInfo> &items,
                                                    Currency &discounts,
                                                    Currency &servCharge,
                                                    Currency &servChargeTax,
                                                    Currency &totalTax,
                                                    Currency &finalPrice)
{
    std::vector<TPriceInfo>::iterator it = items.begin();

    for( ; it != items.end() ; it++ )
    {
        execute( *it );
//        discounts += getTotalDiscountForCurrentResult();
//        servCharge += getServiceChargeForCurrentResult();
//        servChargeTax += getServiceChargeTaxForCurrentResult();
//        totalTax += getTotalTaxForCurrentResult();
//        finalPrice += getFinalPriceForCurrentResult();
    }
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_BugFixes::calculatePartialValuesForItemSet(
                                                    std::vector<TPriceInfo> &items,
                                                    Currency itemsTotal,
                                                    Currency partialPayment,
                                                    Currency &discounts,
                                                    Currency &servCharge,
                                                    Currency &servChargeTax,
                                                    Currency &totalTax,
                                                    Currency &finalPrice)
{
    std::vector<TPriceInfo>::iterator it = items.begin();
    Currency partialPaymentPercentage = (partialPayment * 100.0000) / itemsTotal;
    Currency currentItemTotal = 0;
    Currency partialPaymentAmountPerItem = 0;

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    for( ; it != items.end() ; it++ )
    {
//        execute( *it );
//        currentItemTotal = getFinalPriceForCurrentResult();
//        partialPaymentAmountPerItem = (partialPaymentPercentage * currentItemTotal) / 100.0000;
//
//        partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );
//        partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, partialPaymentAmountPerItem );
//
//        discounts += partialPaymentOutput.PartialDiscount();
//        servCharge += partialPaymentOutput.PartialServiceCharge();
//        servChargeTax += partialPaymentOutput.PartialServiceChargeTax();
//        totalTax += partialPaymentOutput.PartialTotalTax();
    }
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getTotalTaxForCurrentResult()
{
    return getTotalTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getDiscountXForCurrentResult( int X )
{
    return getDiscountX( billCalcResponseObj, X );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getTotalDiscountForCurrentResult()
{
    return getTotalDiscount( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getFinalPriceForCurrentResult()
{
    return getFinalPrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getBasePriceForCurrentResult()
{
    return getBasePrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getServiceChargeForCurrentResult()
{
    return getServiceCharge( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getServiceChargeTaxForCurrentResult()
{
    return getServiceChargeTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getTaxContentForCurrentResult( UnicodeString taxName )
{
    return getTaxContent( billCalcResponseObj, taxName );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getTotalTax( TBillCalcResponse* response )
{
    return response->Result.TotalTax;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getDiscountX( TBillCalcResponse* response, int X )
{
    return response->Result.Discount[X].Content;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getTotalDiscount( TBillCalcResponse* response )
{
    return response->Result.TotalDiscount;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getFinalPrice( TBillCalcResponse* response )
{
    return response->Result.FinalPrice;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getBasePrice( TBillCalcResponse* response )
{
    return response->Result.BasePrice;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getServiceCharge( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.Value;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getServiceChargeTax( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.TaxValue;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_BugFixes::getTaxContent( TBillCalcResponse* response, UnicodeString taxName )
{
    Currency taxContent = 0;
    TAX_RESULT_LIST::iterator it = response->Result.Tax.begin();

    for( ; it != response->Result.Tax.end(); it++ )
    {
        if( it->Name == taxName )
        {
            taxContent = it->Value;
            break;
        }
    }

    return taxContent;
}
//------------------------------------------------------------------------------

static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("Testing_MMBillCalculator_BugFixes");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(TTestTMMBillCalculator_BugFixes));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}

#pragma startup registerTests 4
