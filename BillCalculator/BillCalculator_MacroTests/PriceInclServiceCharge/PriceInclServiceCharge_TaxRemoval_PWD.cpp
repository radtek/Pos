//---------------------------------------------------------------------------


#pragma hdrstop

#include "PriceInclServiceCharge_TaxRemoval_PWD.h"

//---------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_TaxRemoval_PWD::PriceInclServiceCharge_TaxRemoval_PWD(System::String name)
        : TTestMMBillCalculator_Base(name)
{
}
//------------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_TaxRemoval_PWD::~PriceInclServiceCharge_TaxRemoval_PWD()
{
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Persons with disability discount scenarios
void __fastcall PriceInclServiceCharge_TaxRemoval_PWD::Test_InclSimpleTax_InclSC_PWD()
{
    priceInfoObj->Value = 226.2774;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_18", 18.00, 3 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    // removing the tax
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax_18", 18, 3 );

    execute();

	CheckEquals(  11.6788, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 31.5328, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals(  -32.1168, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_TaxRemoval_PWD::Test_InclCompoundTax_InclSC_PWD()
{
    priceInfoObj->Value = 233.4006;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_18", 18.00, 3 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    // removing the tax
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax_18", 18, 3 );

    execute();

    CheckEquals(  11.5274, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 33.1988, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals( 13.8328, getTaxContentForCurrentResult("SalesTax_12Perc"), 0.0001, ERR_TaxContent);
	CheckEquals( 19.3660, getTaxContentForCurrentResult("SalesTax_15Perc"), 0.0001, ERR_TaxContent);
	CheckEquals(  -31.7003, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_TaxRemoval_PWD::Test_ExemptTax_InclSC_PWD()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 0.00, 1 );  //Exempt Tax
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 0.00, 2 );  //Exempt Tax
    applyTaxToCurrentPriceInfo( "SalesTax_18", 0.00, 3 );      //Exempt Tax
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    // removing the tax
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax_18", 18, 3 );

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
// Persons with disability discount scenarios
void __fastcall PriceInclServiceCharge_TaxRemoval_PWD::Test_InclSimpleTax_InclSC_InclSCT_PWD()
{
    priceInfoObj->Value = 226.0870;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_18", 18.00, 3);
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    // removing the tax
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax_18", 18, 3 );

    execute();

	CheckEquals(  11.5942, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals(  1.1594, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 32.4637, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals(  -31.8841, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_TaxRemoval_PWD::Test_InclCompoundTax_InclSC_InclSCT_PWD()
{
    priceInfoObj->Value = 233.1617;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_18", 18.00, 3);
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    // removing the tax
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax_18", 18, 3 );

    execute();

    CheckEquals(  11.4449, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  1.1445, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
	CheckEquals( 34.1059, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals( 13.7339, getTaxContentForCurrentResult("SalesTax_12Perc"), 0.0001, ERR_TaxContent);
	CheckEquals( 19.2275, getTaxContentForCurrentResult("SalesTax_15Perc"), 0.0001, ERR_TaxContent);
	CheckEquals(  -31.4735, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 160.0001, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
/*
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Price inclusive service charge and service charge tax : No service charge in tax exempt price
void __fastcall PriceInclServiceCharge_TaxRemoval_PWD::Test_InclSimpleTax_InclSC_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 226.2774;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_18", 18.00, 3 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    // removing the tax
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax_18", 18, 3 );

    execute();

	CheckEquals(  11.6788, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 31.5328, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals(  -29.1971, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_TaxRemoval_PWD::Test_InclCompoundTax_InclSC_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 233.4006;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_18", 18.00, 3 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    // removing the tax
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax_18", 18, 3 );

    execute();

    CheckEquals(  11.5274, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 33.1988, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals( 13.8328, getTaxContentForCurrentResult("SalesTax_12Perc"), 0.0001, ERR_TaxContent);
	CheckEquals( 19.3660, getTaxContentForCurrentResult("SalesTax_15Perc"), 0.0001, ERR_TaxContent);
	CheckEquals(  -28.8184, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_TaxRemoval_PWD::Test_ExemptTax_InclSC_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 0.00, 1 );  //Exempt Tax
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 0.00, 2 );  //Exempt Tax
    applyTaxToCurrentPriceInfo( "SalesTax_18", 0.00, 3 );      //Exempt Tax
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    // removing the tax
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax_18", 18, 3 );

    execute();

    CheckEquals(  8.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -20.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 88.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Price inclusive service charge and service charge tax : No service charge in tax exempt price
void __fastcall PriceInclServiceCharge_TaxRemoval_PWD::Test_InclSimpleTax_InclSC_InclSCT_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 226.0870;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_18", 18.00, 3);
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    // removing the tax
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax_18", 18, 3 );

    execute();

	CheckEquals(  11.5942, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals(  1.1594, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 32.4637, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals(  -28.9855, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_TaxRemoval_PWD::Test_InclCompoundTax_InclSC_InclSCT_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 233.1617;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_18", 18.00, 3);
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    // removing the tax
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax_18", 18, 3 );

    execute();

    CheckEquals(  11.4449, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  1.1445, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
	CheckEquals( 34.1059, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals( 13.7339, getTaxContentForCurrentResult("SalesTax_12Perc"), 0.0001, ERR_TaxContent);
	CheckEquals( 19.2275, getTaxContentForCurrentResult("SalesTax_15Perc"), 0.0001, ERR_TaxContent);
	CheckEquals(  -28.6123, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 160.0001, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
*/
static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("PriceInclServiceCharge_TaxRemoval_PWD");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(PriceInclServiceCharge_TaxRemoval_PWD));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 10