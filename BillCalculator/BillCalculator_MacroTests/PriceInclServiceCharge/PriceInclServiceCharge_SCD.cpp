//---------------------------------------------------------------------------


#pragma hdrstop

#include "PriceInclServiceCharge_SCD.h"

//---------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_SCD::PriceInclServiceCharge_SCD(System::String name)
        : TTestMMBillCalculator_Base(name)
{
}
//------------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_SCD::~PriceInclServiceCharge_SCD()
{
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Senior citizen discount scenarios
void __fastcall PriceInclServiceCharge_SCD::Test_InclSimpleTax_InclSC_SCD()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;
    priceInfoObj->AllowServiceChargesOnTaxExemptPrice       = true;

    applySCDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  11.6788, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -32.1168, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 128.4671, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_SCD::Test_InclCompoundTax_InclSC_SCD()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;
    priceInfoObj->AllowServiceChargesOnTaxExemptPrice       = true;

    applySCDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  11.5274, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -31.7003, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
	CheckEquals( 126.8011, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_SCD::Test_ExclSimpleTax_InclSC_SCD()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;
    priceInfoObj->AllowServiceChargesOnTaxExemptPrice       = true;

    applySCDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  8.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -22.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 88.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_SCD::Test_ExclCompoundTax_InclSC_SCD()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;
    priceInfoObj->AllowServiceChargesOnTaxExemptPrice       = true;

    applySCDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  8.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -22.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 88.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_SCD::Test_ExemptTax_InclSC_SCD()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;
    priceInfoObj->AllowServiceChargesOnTaxExemptPrice       = true;

    applySCDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 0.00, 1 );  //Exempt Tax
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 0.00, 2 );  //Exempt Tax
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  8.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -22.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 88.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Senior citizen discount scenarios
void __fastcall PriceInclServiceCharge_SCD::Test_InclSimpleTax_InclSC_InclSCT_SCD()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;
    priceInfoObj->AllowServiceChargesOnTaxExemptPrice       = true;

    applySCDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    CheckEquals(  11.5942, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  1.1594, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 1.1594, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -31.8841, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 128.6956, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("PriceInclServiceCharge_SCD");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(PriceInclServiceCharge_SCD));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 8
