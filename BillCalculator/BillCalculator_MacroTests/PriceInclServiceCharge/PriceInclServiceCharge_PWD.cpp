//---------------------------------------------------------------------------


#pragma hdrstop

#include "PriceInclServiceCharge_PWD.h"

//---------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_PWD::PriceInclServiceCharge_PWD(System::String name)
        : TTestMMBillCalculator_Base(name)
{
}
//------------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_PWD::~PriceInclServiceCharge_PWD()
{
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Persons with disability discount scenarios
void __fastcall PriceInclServiceCharge_PWD::Test_InclSimpleTax_InclSC_PWD()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

	CheckEquals(  11.6788, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 31.5328, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals(  -32.1168, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_PWD::Test_InclCompoundTax_InclSC_PWD()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  11.5274, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 33.1988, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals( 13.8328, getTaxContentForCurrentResult("SalesTax_12Perc"), 0.0001, ERR_TaxContent);
	CheckEquals( 19.3660, getTaxContentForCurrentResult("SalesTax_15Perc"), 0.0001, ERR_TaxContent);
	CheckEquals(  -31.7003, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_PWD::Test_ExclSimpleTax_InclSC_PWD()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  8.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 21.6000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -22.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 109.6000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_PWD::Test_ExclCompoundTax_InclSC_PWD()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  8.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 23.0400, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 9.6000, getTaxContentForCurrentResult("SalesTax_12Perc"),      0.0001, ERR_Tax );
    CheckEquals( 13.4400, getTaxContentForCurrentResult("SalesTax_15Perc"),      0.0001, ERR_Tax );
    CheckEquals(  -22.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 111.0400, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_PWD::Test_ExemptTax_InclSC_PWD()
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
// Persons with disability discount scenarios
void __fastcall PriceInclServiceCharge_PWD::Test_InclSimpleTax_InclSC_InclSCT_PWD()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

	CheckEquals(  11.5942, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals(  1.1594, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 32.4637, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals(  -31.8841, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_PWD::Test_InclCompoundTax_InclSC_InclSCT_PWD()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = false;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    CheckEquals(  11.4449, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  1.1445, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
	CheckEquals( 34.1059, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals( 13.7339, getTaxContentForCurrentResult("SalesTax_12Perc"), 0.0001, ERR_TaxContent);
	CheckEquals( 19.2275, getTaxContentForCurrentResult("SalesTax_15Perc"), 0.0001, ERR_TaxContent);
	CheckEquals(  -31.4735, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 160.0001, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Price inclusive service charge and service charge tax : No service charge in tax exempt price

void __fastcall PriceInclServiceCharge_PWD::Test_InclSimpleTax_InclSC_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = true;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

	CheckEquals(  11.6788, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 31.5328, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals(  -29.1971, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_PWD::Test_InclCompoundTax_InclSC_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = true;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  11.5274, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 33.1989, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals( 13.8329, getTaxContentForCurrentResult("SalesTax_12Perc"), 0.0001, ERR_TaxContent);
	CheckEquals( 19.3660, getTaxContentForCurrentResult("SalesTax_15Perc"), 0.0001, ERR_TaxContent);
	CheckEquals(  -28.8184, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 160.0001, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_PWD::Test_ExclSimpleTax_InclSC_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = true;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  8.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 21.6000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -20.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 109.6000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_PWD::Test_ExclCompoundTax_InclSC_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = true;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  8.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 23.0400, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 9.6000, getTaxContentForCurrentResult("SalesTax_12Perc"),      0.0001, ERR_Tax );
    CheckEquals( 13.4400, getTaxContentForCurrentResult("SalesTax_15Perc"),      0.0001, ERR_Tax );
    CheckEquals(  -20.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 111.0400, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_PWD::Test_ExemptTax_InclSC_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = true;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 0.00, 1 );  //Exempt Tax
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 0.00, 2 );  //Exempt Tax
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

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
void __fastcall PriceInclServiceCharge_PWD::Test_InclSimpleTax_InclSC_InclSCT_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = true;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

	CheckEquals(  11.5942, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals(  1.1594, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceCharge );
	CheckEquals( 32.4637, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals(  -28.9855, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 159.9999, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_PWD::Test_InclCompoundTax_InclSC_InclSCT_PWD_NoSCInExemptPrice()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxExemptPriceExclusiveOfServiceCharge    = true;

    applyPWDToCurrentPriceInfo( 20.00 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    CheckEquals(  11.4449, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  1.1445, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
	CheckEquals( 34.1059, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
	CheckEquals( 13.7339, getTaxContentForCurrentResult("SalesTax_12Perc"), 0.0001, ERR_TaxContent);
	CheckEquals( 19.2275, getTaxContentForCurrentResult("SalesTax_15Perc"), 0.0001, ERR_TaxContent);
	CheckEquals(  -28.6123, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 160.0001, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
*/
static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("PriceInclServiceCharge_PWD");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(PriceInclServiceCharge_PWD));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 7
