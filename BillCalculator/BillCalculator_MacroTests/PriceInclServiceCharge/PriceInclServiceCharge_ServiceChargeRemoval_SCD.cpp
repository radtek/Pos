//---------------------------------------------------------------------------


#pragma hdrstop

#include "PriceInclServiceCharge_ServiceChargeRemoval_SCD.h"

//---------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_ServiceChargeRemoval_SCD::PriceInclServiceCharge_ServiceChargeRemoval_SCD(System::String name)
        : TTestMMBillCalculator_Base(name)
{
}
//------------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_ServiceChargeRemoval_SCD::~PriceInclServiceCharge_ServiceChargeRemoval_SCD()
{
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Senior citizen discount scenarios
void __fastcall PriceInclServiceCharge_ServiceChargeRemoval_SCD::Test_InclSimpleTax_InclSC_SCD()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -29.1971, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 116.7883, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval_SCD::Test_InclCompoundTax_InclSC_SCD()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -28.8184, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
	CheckEquals( 115.2737, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval_SCD::Test_ExclSimpleTax_InclSC_SCD()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -20.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 80.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval_SCD::Test_ExclCompoundTax_InclSC_SCD()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -20.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 80.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval_SCD::Test_ExemptTax_InclSC_SCD()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -20.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 80.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Senior citizen discount scenarios
void __fastcall PriceInclServiceCharge_ServiceChargeRemoval_SCD::Test_InclSimpleTax_InclSC_InclSCT_SCD()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 10 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  0.0000, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -28.9855, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 115.9420, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("PriceInclServiceCharge_ServiceChargeRemoval_SCD");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(PriceInclServiceCharge_ServiceChargeRemoval_SCD));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 14
