#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "TestMMBillCalculator_Phillipines.h"

#define ERR_Total "Final price is wrong"
#define ERR_BasePrice "Base price is wrong"
#define ERR_DiscountX "Discount content is wrong"
#define ERR_Discounts "Discounts total is wrong"
#define ERR_Surcharges "Surcharges total is wrong"
#define ERR_ServiceCharge "Service charge is wrong"
#define ERR_ServiceChargeTax "Service charge tax is wrong"
#define ERR_Tax "Total tax is wrong"
#define ERR_TaxContent "Tax content is wrong"
#define ERR_DiscountWT "Discount without tax is wrong"
#define ERR_TaxOnDiscount "Tax on discount is wrong"

__fastcall TTestTMMBillCalculator_Phillipines::TTestTMMBillCalculator_Phillipines(System::String name) : TTestCase(name)
{
}
//------------------------------------------------------------------------------

__fastcall TTestTMMBillCalculator_Phillipines::~TTestTMMBillCalculator_Phillipines()
{
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::SetUp()
{
    priceInfoObj = new TPriceInfo();
    initializePriceInfoToTaxSettings( priceInfoObj );

    delete billCalcResponseObj;
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::TearDown()
{
    delete priceInfoObj;
    delete billCalcResponseObj;
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_Execute_Empty()
{
    execute();

    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.00, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );

}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Tax Inclusive Items Tests
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_NoDiscNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    execute();
    CheckEquals(  12.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 112.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.00, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.00, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_NoDiscServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 122.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.00, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.00, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_DiscNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    execute();
    CheckEquals( 10.8, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( -11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 100.8, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -10.00, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -1.20, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_HigherDiscNoServCharge()
{
    priceInfoObj->Value = 7;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "10 bucks", dtDiscount, dwMoney, 0, 0, 10 );

    execute();

    CheckEquals( -7.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
        CheckEquals( -6.25, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -0.75, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SetPriceDiscNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 0, 50 );

    execute();

    CheckEquals( -62.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 5.3571, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 50.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -55.3571, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -6.6429, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SurchargeNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwPercentage, 10 );
    execute();

    CheckEquals( 11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 123.2, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
        CheckEquals( 10.00, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 1.20, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_DiscServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( -11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(   9.0, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 109.8, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
    CheckEquals( -10.00, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -1.20, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SetPriceDiscServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 0, 50 );

    execute();

    CheckEquals( -62.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 5.3571, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 4.4643, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 54.4643, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -55.3571, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -6.6429, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SurchargeServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwPercentage, 10  );

    execute();

    //HERE!!!
    CheckEquals( 11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 11.0, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 134.2, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
        CheckEquals( 10.00, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 1.20, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_DollarSurchargeServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "20 bucks", dtSurcharge, dwMoney, 0, 1, 20);

    execute();

    //HERE!!!
    CheckEquals(  20.000, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  11.7857, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 143.7857, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
    CheckEquals( 17.8571, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 2.1429, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_DollarDiscountServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "20 bucksc", dtDiscount, dwMoney, 0, 0, 20 );
    execute();
    //HERE!!!
    CheckEquals( -20.000, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(   8.2143, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 100.2143, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -17.8571, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -2.1429, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SCDNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applySCDToCurrentPriceInfo( 10 );
    execute();
    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 90.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SCDServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applySCDToCurrentPriceInfo( 10 );
    execute();
    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_PWDDiscNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    //applyDiscountToCurrentPriceInfo( "20perc pwd", dtDiscount, dwPercentage, 20, 0, 0, true );
    applyPWDToCurrentPriceInfo(20);
    execute();
    CheckEquals( -20.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.60, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 9.60, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 89.6, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_PWDDiscServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
//    applyDiscountToCurrentPriceInfo( "20perc pwd", dtDiscount, dwPercentage, 20, 0, 0, true );
    applyPWDToCurrentPriceInfo(20);
    execute();
    CheckEquals( -20.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 8.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 9.6, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 9.60, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 97.60, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );    // second parameter is for service charge tax
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    execute();
    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 1.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 13.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 123.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_DiscServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    execute();
    CheckEquals( -11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 11.7, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 110.70, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -1.2, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_PWDDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

//    applyDiscountToCurrentPriceInfo( "20perc pwd", dtDiscount, dwPercentage, 20, 0, 0, true );
    applyPWDToCurrentPriceInfo(20);

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( -20.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 8.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.80, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 9.6, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.40, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 98.40, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SCDServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applySCDToCurrentPriceInfo( 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 10.0, getDiscountXForCurrentResult( 0 ), 0.0001, ERR_DiscountX );
    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.90, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.90, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( -10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Tax Exclusive Items Tests
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_NoDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( 12.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 112.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_NoDiscServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = false;

    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( 11.2 , getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 136.64, getFinalPriceForCurrentResult(), 0.0001, ERR_Total ) ;
    CheckEquals( 0.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_DiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 100.8, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------
//*
void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SetPriceDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "set to $35", dtDiscount, dwSetPrice, 0, 1, 35.00 );
    execute();
    CheckEquals(  4.2,  getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( -65.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  39.2, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
    CheckEquals(-65.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_DiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.0, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 10.8, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 109.8, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------
//*
void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SetPriceDiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "set to $35", dtDiscount, dwSetPrice, 0, 1, 35.00 );

    execute();

    CheckEquals( -65.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  3.50,  getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  4.20,  getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  42.70, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
    CheckEquals(-65.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SurchargeServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwPercentage, 10 );

    execute();

    CheckEquals( 11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 12.32, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 150.304, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 11.2, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_DollarSurchargeServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "20 bucks", dtSurcharge, dwMoney, 0, 0, 20);

    execute();

    CheckEquals( 20.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 13.2, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 161.04, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_DollarDiscountServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwMoney, 0, 0, 20 );

    execute();

    CheckEquals( -20.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.2, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 112.24, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SCDNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applySCDToCurrentPriceInfo( 10 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 90.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SCDServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applySCDToCurrentPriceInfo( 10 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_PWDDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );

//    applyDiscountToCurrentPriceInfo( "20perc pwd", dtDiscount, dwPercentage, 20, 0, true );
    applyPWDToCurrentPriceInfo(20);

    execute();

    CheckEquals( -20.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.6, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 9.60, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 89.60, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_PWDDiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

//    applyDiscountToCurrentPriceInfo( "20perc pwd", dtDiscount, dwPercentage, 20, 0, true );
    applyPWDToCurrentPriceInfo(20);

    execute();

    CheckEquals( -20.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 8.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 9.6, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 9.60, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 97.60, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_ServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 1.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 13.00, getTotalTaxForCurrentResult() , 0.0001, ERR_Tax );
    CheckEquals( 123.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_DiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 11.70, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 110.70, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_PWDDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

//    applyDiscountToCurrentPriceInfo( "20perc pwd", dtDiscount, dwPercentage, 20, 0, true );
    applyPWDToCurrentPriceInfo(20);

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( -20.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 8.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.80, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 9.6, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.40, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 98.40, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SCDServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applySCDToCurrentPriceInfo( 10 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.90, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.90, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Tax Exempt Items Tests
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_NoDiscNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 112.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 112.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//-----------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_NoDiscServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    CheckEquals( 11.20, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 123.20, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(0.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_DiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  90.00,  getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

//*
void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SetPriceDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive   = false;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "set to $35", dtDiscount, dwSetPrice, 0, 1, 35.00 );

    execute();

    CheckEquals(  0.00,  getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -65.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  35.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
    CheckEquals(-65.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_DiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.0, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SetPriceDiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 0, 40 );

    execute();

    CheckEquals( -60.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 4.0, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 44.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-60.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SurchargeServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwPercentage, 10 );

    execute();

    CheckEquals( 11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 12.32, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 135.52, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(11.2, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_DollarSurchargeServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwMoney, 0, 0, 20);

    execute();

    CheckEquals( 20.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 13.20, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 145.20, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_DollarDiscountServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwMoney, 0, 0, 20 );

    execute();

    CheckEquals( -20.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.2, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 101.20, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SCDNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applySCDToCurrentPriceInfo( 10 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 90.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SCDServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applySCDToCurrentPriceInfo( 10 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_PWDDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );

//    applyDiscountToCurrentPriceInfo( "20perc pwd", dtDiscount, dwPercentage, 20, 0, true );
    applyPWDToCurrentPriceInfo(20);

    execute();

    CheckEquals( -20.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 80.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_PWDDiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

//    applyDiscountToCurrentPriceInfo( "20perc pwd", dtDiscount, dwPercentage, 20, 0, true );
    applyPWDToCurrentPriceInfo(20);

    execute();

    CheckEquals( -20.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 8.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 88.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_ServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 1.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 1.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 111.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(0.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_DiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.900, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.900, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.9, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_PWDDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

//    applyDiscountToCurrentPriceInfo( "20perc pwd", dtDiscount, dwPercentage, 20, 0, true );
    applyPWDToCurrentPriceInfo(20);

    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    CheckEquals( -20.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 8.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.800, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.800, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 88.80, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SCDServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applySCDToCurrentPriceInfo( 10 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.90, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.90, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Tests for set of items
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_NoDiscNoServCharge()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );

    item1.TaxInclusive = true;
    item1.Value = 112;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );

    item2.TaxInclusive = false;
    item2.Value = 100;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );

    item3.TaxInclusive = true;
    item3.Value = 100;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice,discountWithOutTax,taxOnDiscount );

    CheckEquals( 0.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 0.00, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 24.00, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 324.00, finalPrice, 0.0001, ERR_Total );
    CheckEquals( 0.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_DiscServChargeServChargeTax()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;
    TPriceInfo item4;
    TPriceInfo item5;
    TPriceInfo item6;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );
    initializePriceInfoToTaxSettings( &item4 );
    initializePriceInfoToTaxSettings( &item5 );
    initializePriceInfoToTaxSettings( &item6 );

    item1.TaxInclusive = true;
    item1.Value = 450;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );
    applyDiscountToPriceInfo( item1, "10perc", dtDiscount, dwPercentage, 10 );

    item2.TaxInclusive = false;
    item2.Value = 200;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );
    applyDiscountToPriceInfo( item2, "10perc", dtDiscount, dwPercentage, 10 );

    item3.TaxInclusive = true;
    item3.Value = 350;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );
    applyDiscountToPriceInfo( item3, "10perc", dtDiscount, dwPercentage, 10 );

    item4.TaxInclusive = false;
    item4.Value = 750;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );
    applyDiscountToPriceInfo( item4, "10perc", dtDiscount, dwPercentage, 10 );

    item5.TaxInclusive = true;
    item5.Value = 175;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );
    applyDiscountToPriceInfo( item5, "10perc", dtDiscount, dwPercentage, 10 );

    item6.TaxInclusive = false;
    item6.Value = 525;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );
    applyDiscountToPriceInfo( item6, "10perc", dtDiscount, dwPercentage, 10 );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice,discountWithOutTax,taxOnDiscount );

    CheckEquals( -245.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 216.1607, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 25.9393, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 90.9322, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 2468.70, finalPrice, 0.0001, ERR_Total );
    CheckEquals( -240.1785, discountWithOutTax, 0.0001, ERR_DiscountWT );
    CheckEquals( -4.8215, taxOnDiscount, 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_PWDDiscServChargeServChargeTax()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;
    TPriceInfo item4;
    TPriceInfo item5;
    TPriceInfo item6;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );
    initializePriceInfoToTaxSettings( &item4 );
    initializePriceInfoToTaxSettings( &item5 );
    initializePriceInfoToTaxSettings( &item6 );

    item1.TaxInclusive = true;
    item1.Value = 450;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );

//    applyDiscountToPriceInfo( item1, "10perc pwd", dtDiscount, dwPercentage, 10, 0, 0, true );
    applyPWDToPriceInfo( item1, 10);

    item2.TaxInclusive = false;
    item2.Value = 200;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );

//    applyDiscountToPriceInfo( item2, "10perc pwd", dtDiscount, dwPercentage, 10, 0, 0, true );
    applyPWDToPriceInfo( item2, 10);

    item3.TaxInclusive = true;
    item3.Value = 350;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );

//    applyDiscountToPriceInfo( item3, "10perc pwd", dtDiscount, dwPercentage, 10, 0, 0, true );
    applyPWDToPriceInfo( item3, 10);

    item4.TaxInclusive = false;
    item4.Value = 750;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );

//    applyDiscountToPriceInfo( item4, "10perc pwd", dtDiscount, dwPercentage, 10, 0, 0, true );
    applyPWDToPriceInfo( item4, 10);

    item5.TaxInclusive = true;
    item5.Value = 175;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );

//    applyDiscountToPriceInfo( item5, "10perc pwd", dtDiscount, dwPercentage, 10, 0, 0, true );
    applyPWDToPriceInfo( item5, 10);

    item6.TaxInclusive = false;
    item6.Value = 525;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );

//    applyDiscountToPriceInfo( item6, "10perc pwd", dtDiscount, dwPercentage, 10, 0, 0, true );
    applyPWDToPriceInfo( item6, 10);

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice ,discountWithOutTax,taxOnDiscount );

    CheckEquals( -240.1786, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 216.1607, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 25.9393, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 90.9322, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 2468.7000, finalPrice, 0.0001, ERR_Total );
    CheckEquals( -240.1785, discountWithOutTax, 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, taxOnDiscount, 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_SCDServChargeServChargeTax()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;
    TPriceInfo item4;
    TPriceInfo item5;
    TPriceInfo item6;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );
    initializePriceInfoToTaxSettings( &item4 );
    initializePriceInfoToTaxSettings( &item5 );
    initializePriceInfoToTaxSettings( &item6 );

    item1.TaxInclusive = true;
    item1.PriceTaxExempt = true;
    item1.Value = 450;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );
    applySCDToPriceInfo( item1, 10 );

    item2.TaxInclusive = false;
    item2.PriceTaxExempt = true;
    item2.Value = 200;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );
    applySCDToPriceInfo( item2, 10 );

    item3.TaxInclusive = true;
    item3.PriceTaxExempt = true;
    item3.Value = 350;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );
    applySCDToPriceInfo( item3, 10 );

    item4.TaxInclusive = false;
    item4.PriceTaxExempt = true;
    item4.Value = 750;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );
    applySCDToPriceInfo( item4, 10 );

    item5.TaxInclusive = true;
    item5.PriceTaxExempt = true;
    item5.Value = 175;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );
    applySCDToPriceInfo( item5, 10 );

    item6.TaxInclusive = false;
    item6.PriceTaxExempt = true;
    item6.Value = 525;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );
    applySCDToPriceInfo( item6, 10 );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice ,discountWithOutTax,taxOnDiscount );

    CheckEquals( -240.1785, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 216.1607, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 25.9393, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTaxContentForCurrentResult("Sales tax"), 0.0001, ERR_Tax );
    CheckEquals( 25.9393, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 2403.7071, finalPrice, 0.0001, ERR_Total );
    CheckEquals( -240.1785, discountWithOutTax, 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, taxOnDiscount, 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 1, 0, false );          // Normal discount 100%
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 2, 50, false );       // SetPrice discount $50
    execute();
    CheckEquals( -62.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 4.4643, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.4464, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 5.8035, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 54.9107, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-55.3571, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -6.6429, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 1, 50, false );       // SetPrice discount $50
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 2, 0, false );          // Normal discount 100%

    execute();

    CheckEquals( -112.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-100.00, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -12.00, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 1, 0, false );          // Normal discount 100%
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 2, 50, false );       // SetPrice discount $50

    execute();

    CheckEquals( -62.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 5.000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.500, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 6.500, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 61.500, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-62.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 1, 50, false );       // SetPrice discount $50
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 2, 0, false );          // Normal discount 100%

    execute();

    CheckEquals( -112.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-100.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -12.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 1, 0, false );          // Normal discount 100%
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 2, 50, false );       // SetPrice discount $50

    execute();

    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 5.000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.500, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.500, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 55.500, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 1, 50, false );       // SetPrice discount $50
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 2, 0, false );          // Normal discount 100%

    execute();

    CheckEquals( -100.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-100.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_MultipleDiscounts_NormalPWDSetPriceDollarDisc_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 1, 0, false );            // Normal discount 10%
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 2, 50, false );       // SetPrice discount $50
    applyDiscountToCurrentPriceInfo( "10 bucks", dtDiscount, dwMoney, 0, 4, 10, false );               // Dollar discount $10

//    applyDiscountToCurrentPriceInfo( "10perc pwd", dtDiscount, dwPercentage, 10, 3, 0, true );         // PWD discount 10%
    applyPWDToCurrentPriceInfo(10);

    execute();

//    CheckEquals( -76.4642, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
//    CheckEquals( 3.1268, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
//    CheckEquals( 0.3126, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 4.0647, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
//    CheckEquals( 36.4598, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( -75.5714, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 3.2143, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.3214, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 4.1785, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 39.5356, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_CompoundMultipleNormalDiscountsServChargeServChargeTax()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;
    TPriceInfo item4;
    TPriceInfo item5;
    TPriceInfo item6;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );
    initializePriceInfoToTaxSettings( &item4 );
    initializePriceInfoToTaxSettings( &item5 );
    initializePriceInfoToTaxSettings( &item6 );

    item1.TaxInclusive = true;
    item1.Value = 450;
    item1.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );
    applyDiscountToPriceInfo( item1, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item1, "20perc", dtDiscount, dwPercentage, 20, 2 );

    item2.TaxInclusive = false;
    item2.Value = 200;
    item2.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );
    applyDiscountToPriceInfo( item2, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item2, "20perc", dtDiscount, dwPercentage, 20, 2 );

    item3.TaxInclusive = true;
    item3.Value = 350;
    item3.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );
    applyDiscountToPriceInfo( item3, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item3, "20perc", dtDiscount, dwPercentage, 20, 2 );

    item4.TaxInclusive = false;
    item4.Value = 750;
    item4.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );
    applyDiscountToPriceInfo( item4, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item4, "20perc", dtDiscount, dwPercentage, 20, 2 );

    item5.TaxInclusive = true;
    item5.Value = 175;
    item5.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );
    applyDiscountToPriceInfo( item5, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item5, "20perc", dtDiscount, dwPercentage, 20, 2 );

    item6.TaxInclusive = false;
    item6.Value = 525;
    item6.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );
    applyDiscountToPriceInfo( item6, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item6, "20perc", dtDiscount, dwPercentage, 20, 2 );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice,discountWithOutTax,taxOnDiscount );

    CheckEquals( -686.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 172.9286, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20.7514, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 72.7457, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1974.9600, finalPrice, 0.0001, ERR_Total );
    CheckEquals( -672.5, discountWithOutTax, 0.0001, ERR_DiscountWT );
    CheckEquals( -13.5001, taxOnDiscount, 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_CompoundMultipleMixedDiscountsServChargeServChargeTax_NormalPWD()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;
    TPriceInfo item4;
    TPriceInfo item5;
    TPriceInfo item6;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );
    initializePriceInfoToTaxSettings( &item4 );
    initializePriceInfoToTaxSettings( &item5 );
    initializePriceInfoToTaxSettings( &item6 );

    item1.TaxInclusive = true;
    item1.Value = 450;
    item1.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );
    applyDiscountToPriceInfo( item1, "10perc", dtDiscount, dwPercentage, 10, 1 );
//    applyDiscountToPriceInfo( item1, "20perc", dtDiscount, dwPercentage, 20, 2, 0, false );
    applyPWDToPriceInfo( item1, 20);

    item2.TaxInclusive = false;
    item2.Value = 200;
    item2.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );
    applyDiscountToPriceInfo( item2, "10perc", dtDiscount, dwPercentage, 10, 1 );
//    applyDiscountToPriceInfo( item2, "20perc pwd", dtDiscount, dwPercentage, 20, 2, 0, true );
    applyPWDToPriceInfo( item2, 20);

    item3.TaxInclusive = true;
    item3.Value = 350;
    item3.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );
    applyDiscountToPriceInfo( item3, "10perc", dtDiscount, dwPercentage, 10, 1 );
//    applyDiscountToPriceInfo( item3, "20perc pwd", dtDiscount, dwPercentage, 20, 2, 0, true );
    applyPWDToPriceInfo( item3, 20);

    item4.TaxInclusive = false;
    item4.Value = 750;
    item4.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );
    applyDiscountToPriceInfo( item4, "10perc", dtDiscount, dwPercentage, 10, 1 );
//    applyDiscountToPriceInfo( item4, "20perc pwd", dtDiscount, dwPercentage, 20, 2, 0, true );
    applyPWDToPriceInfo( item4, 20);

    item5.TaxInclusive = true;
    item5.Value = 175;
    item5.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );
    applyDiscountToPriceInfo( item5, "10perc", dtDiscount, dwPercentage, 10, 1 );
//    applyDiscountToPriceInfo( item5, "20perc pwd", dtDiscount, dwPercentage, 20, 2, 0, true );
    applyPWDToPriceInfo( item5, 20);

    item6.TaxInclusive = false;
    item6.Value = 525;
    item6.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );
    applyDiscountToPriceInfo( item6, "10perc", dtDiscount, dwPercentage, 10, 1 );
//    applyDiscountToPriceInfo( item6, "20perc pwd", dtDiscount, dwPercentage, 20, 2, 0, true );
    applyPWDToPriceInfo( item6, 20);

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice,discountWithOutTax,taxOnDiscount );

// individual items

//    CheckEquals( -126.000, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 28.9285, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 3.4714, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 38.1856, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 356.3999, finalPrice, 0.0001, ERR_Total );
//
//    CheckEquals( -56.00, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 14.40, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 1.728, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 19.008, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 177.408, finalPrice, 0.0001, ERR_Total );
//
//    CheckEquals( -98.00, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 25.20, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 3.024, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 3.024, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 280.224, finalPrice, 0.0001, ERR_Total );
//
//    CheckEquals( -210.00, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 54.00, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 6.480, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 6.480, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 600.48, finalPrice, 0.0001, ERR_Total );
//
//    CheckEquals( -49.00, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 12.60, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 1.512, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 1.512, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 140.112, finalPrice, 0.0001, ERR_Total );
//
//    CheckEquals( -147.00, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 37.80, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 4.536, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 4.536, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 420.336, finalPrice, 0.0001, ERR_Total );
//
// totals

    CheckEquals( -677.3214, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 172.9285, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20.7514, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 72.7457, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1974.9600, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_CompoundMultipleMixedDiscounts()
{
    std::vector<TPriceInfo> orders;
    Currency discounts     = 0.00;
    Currency servCharge    = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax      = 0.00;
    Currency finalPrice    = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;

    initializePriceInfoToTaxSettings( &item1 );

    item1.TaxInclusive      = false;
    item1.Value             = 450;
    item1.MultiDiscountType = mdtCompound;

    applyDiscountToPriceInfo( item1, "500 bucks", dtDiscount, dwMoney,       0, 2, 500 );
    applyDiscountToPriceInfo( item1, "10perc",    dtDiscount, dwPercentage, 10, 1 );

    orders.push_back( item1 );

    //execute();
    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice,discountWithOutTax,taxOnDiscount );

    CheckEquals( -450.00, discounts,  0.0001, ERR_Discounts );
    CheckEquals(    0.00, finalPrice, 0.0001, ERR_Total     );
    CheckEquals( -450.0, discountWithOutTax, 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, taxOnDiscount, 0.0001, ERR_TaxOnDiscount );
/*
    CheckEquals( -686.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 172.9285, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20.7514, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 72.7456, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1974.9599, finalPrice, 0.0001, ERR_Total );
*/
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_SimpleMultipleSurchargePercentage()
{
    std::vector<TPriceInfo> orders;

    Currency discounts     = 0.00;
    Currency servCharge    = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax      = 0.00;
    Currency finalPrice    = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );

    item1.TaxInclusive = false;
    item1.Value = 175.00;
    item1.MultiDiscountType = mdtCompound;
    item1.ServiceChargeInfo.Percentage = 14.00;
    item1.ServiceChargeBeforeDiscount = true;

    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item1, "37 bucks", dtSurcharge, dwMoney, 0, 1, 37 );
    //applyDiscountToPriceInfo( item1, "10perc", dtSurcharge, dwPercentage, 10, 1, 0 );

    item2.TaxInclusive = true;
    item2.Value = 175.00;
    item2.MultiDiscountType = mdtSimple;
    item2.ServiceChargeInfo.Percentage = 14.00;
    item2.ServiceChargeBeforeDiscount = true;

    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item2, "37 bucks", dtSurcharge, dwMoney, 0, 1, 37 );
//    applyDiscountToPriceInfo( item2, "10perc", dtSurcharge, dwPercentage, 10, 1 );

    orders.push_back( item1 );
    //orders.push_back( item2 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice ,discountWithOutTax,taxOnDiscount );

// individual items
    CheckEquals(  37.000, discounts,     0.0001, ERR_Discounts );
    CheckEquals(  24.500, servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals(   0.000, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  25.440, totalTax,      0.0001, ERR_Tax );
    CheckEquals( 261.940, finalPrice,    0.0001, ERR_Total );
    CheckEquals( 37.0, discountWithOutTax, 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, taxOnDiscount, 0.0001, ERR_TaxOnDiscount );
/*
Tax Inclusive price:
    CheckEquals(  37.000, discounts,     0.0001, ERR_Discounts );
    CheckEquals(  21.875, servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals(   0.000, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals(   0.000, totalTax,      0.0001, ERR_Tax );
    CheckEquals( 233.875, finalPrice,    0.0001, ERR_Total );
*/

/*
    CheckEquals(  10.000, discounts,     0.0001, ERR_Discounts );
    CheckEquals(   0.000, servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals(   0.000, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals(   0.000, totalTax,      0.0001, ERR_Tax );
    CheckEquals( 110.000, finalPrice,    0.0001, ERR_Total );
*/

// totals
/*
    CheckEquals(  74.000, discounts,     0.0001, ERR_Discounts );
    CheckEquals(  43.750, servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals(   0.000, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals(   0.000, totalTax,      0.0001, ERR_Tax );
    CheckEquals( 467.750, finalPrice,    0.0001, ERR_Total );
*/
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_EqualPriorityDiscountsOver100Perc()
{
    std::vector<TPriceInfo> orders;
    Currency discounts     = 0.00;
    Currency servCharge    = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax      = 0.00;
    Currency finalPrice    = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;
    TPriceInfo item4;
    TPriceInfo item5;
    TPriceInfo item6;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );
    initializePriceInfoToTaxSettings( &item4 );
    initializePriceInfoToTaxSettings( &item5 );
    initializePriceInfoToTaxSettings( &item6 );

    item1.TaxInclusive = true;
    item1.Value = 450;
    item1.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );

    item2.TaxInclusive = false;
    item2.Value = 200;
    item2.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );

    item3.TaxInclusive = true;
    item3.Value = 350;
    item3.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );

    item4.TaxInclusive = false;
    item4.Value = 750;
    item4.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );

    item5.TaxInclusive = true;
    item5.Value = 175;
    item5.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );

    item6.TaxInclusive = false;
    item6.Value = 525;
    item6.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );

    applyDiscountToPriceInfo( item1, "100perc",  dtDiscount, dwPercentage, 100, 0, 0,  false );
    applyDiscountToPriceInfo( item1, "20dollar", dtDiscount, dwMoney,      0,   0, 20, false );
    applyDiscountToPriceInfo( item2, "100perc",  dtDiscount, dwPercentage, 100, 0, 0,  false );
    applyDiscountToPriceInfo( item2, "20dollar", dtDiscount, dwMoney,      0,   0, 20, false );
    applyDiscountToPriceInfo( item3, "100perc",  dtDiscount, dwPercentage, 100, 0, 0,  false );
    applyDiscountToPriceInfo( item3, "20dollar", dtDiscount, dwMoney,      0,   0, 20, false );
    applyDiscountToPriceInfo( item4, "100perc",  dtDiscount, dwPercentage, 100, 0, 0,  false );
    applyDiscountToPriceInfo( item4, "20dollar", dtDiscount, dwMoney,      0,   0, 20, false );
    applyDiscountToPriceInfo( item5, "100perc",  dtDiscount, dwPercentage, 100, 0, 0,  false );
    applyDiscountToPriceInfo( item5, "20dollar", dtDiscount, dwMoney,      0,   0, 20, false );
    applyDiscountToPriceInfo( item6, "100perc",  dtDiscount, dwPercentage, 100, 0, 0,  false );
    applyDiscountToPriceInfo( item6, "20dollar", dtDiscount, dwMoney,      0,   0, 20, false );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice,discountWithOutTax,taxOnDiscount );

// individual items

//    CheckEquals( -126.000, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 28.9285, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 3.4714, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 38.1856, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 356.3999, finalPrice, 0.0001, ERR_Total );
//
//    CheckEquals( -56.00, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 14.40, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 1.728, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 19.008, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 177.408, finalPrice, 0.0001, ERR_Total );
//
//    CheckEquals( -98.00, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 25.20, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 3.024, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 3.024, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 280.224, finalPrice, 0.0001, ERR_Total );
//
//    CheckEquals( -210.00, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 54.00, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 6.480, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 6.480, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 600.48, finalPrice, 0.0001, ERR_Total );
//
//    CheckEquals( -49.00, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 12.60, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 1.512, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 1.512, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 140.112, finalPrice, 0.0001, ERR_Total );
//
//    CheckEquals( -147.00, discounts, 0.0001, ERR_Discounts );
//    CheckEquals( 37.80, servCharge, 0.0001, ERR_ServiceCharge );
//    CheckEquals( 4.536, servChargeTax, 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 4.536, totalTax, 0.0001, ERR_Tax );
//    CheckEquals( 420.336, finalPrice, 0.0001, ERR_Total );
//
// totals

    CheckEquals( -2450.0000, discounts,     0.0001, ERR_Discounts );
    CheckEquals( 0.0000,     servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000,     servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.0000,     totalTax,      0.0001, ERR_Tax );
    CheckEquals( 0.0000,     finalPrice,    0.0001, ERR_Total );
    CheckEquals( -2401.7857, discountWithOutTax, 0.0001, ERR_DiscountWT );
    CheckEquals( -48.2143, taxOnDiscount, 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_HigherSurcharges()
{
    std::vector<TPriceInfo> orders;
    Currency discounts     = 0.00;
    Currency servCharge    = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax      = 0.00;
    Currency finalPrice    = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );

    item1.Value = 10;
    item1.TaxInclusive = true;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item1, "$11 surcharge", dtSurcharge, dwMoney, 0, 0, 11 );

    item2.Value = 20;
    item2.TaxInclusive = true;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item2, "$25 surcharge", dtSurcharge, dwMoney, 0, 0, 25 );

    item3.Value = 30;
    item3.TaxInclusive = true;
    applyTaxToPriceInfo( item3, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item3, "$35 surcharge", dtSurcharge, dwMoney, 0, 0, 35 );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice ,discountWithOutTax,taxOnDiscount );

    CheckEquals( 71.00, discounts,     0.0001, ERR_Discounts );
    CheckEquals( 0.00,     servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00,     servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 14.0357,     totalTax,      0.0001, ERR_Tax );
    CheckEquals( 131.00,     finalPrice,    0.0001, ERR_Total );
    CheckEquals( 63.3925, discountWithOutTax, 0.0001, ERR_DiscountWT );
    CheckEquals( 7.6075, taxOnDiscount, 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_HigherSurchargesWithDiscounts()
{
    std::vector<TPriceInfo> orders;
    Currency discounts     = 0.00;
    Currency servCharge    = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax      = 0.00;
    Currency finalPrice    = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );

    item1.Value = 10;
    item1.TaxInclusive = true;
    item1.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item1, "10% discount", dtDiscount, dwPercentage, 10, 1, 00, false );
    applyDiscountToPriceInfo( item1, "$11 surcharge", dtSurcharge, dwMoney, 0, 2, 11, false );

    item2.Value = 20;
    item2.TaxInclusive = true;
    item2.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item2, "10% discount", dtDiscount, dwPercentage, 10, 1, 00, false );
    applyDiscountToPriceInfo( item2, "$25 surcharge", dtSurcharge, dwMoney, 0, 2, 25 );

    item3.Value = 30;
    item3.TaxInclusive = true;
    item3.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item3, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item3, "10% discount", dtDiscount, dwPercentage, 10, 1, 00, false );
    applyDiscountToPriceInfo( item3, "$35 surcharge", dtSurcharge, dwMoney, 0, 2, 35 );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice ,discountWithOutTax,taxOnDiscount );

    CheckEquals( 65.00, discounts,     0.0001, ERR_Discounts );
    CheckEquals( 0.00,     servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00,     servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 13.3929,     totalTax,      0.0001, ERR_Tax );
    CheckEquals( 125.00,     finalPrice,    0.0001, ERR_Total );
    CheckEquals( 58.0355, discountWithOutTax, 0.0001, ERR_DiscountWT );
    CheckEquals( 6.9645, taxOnDiscount, 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_PartialPaymentNoDiscNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );

    CheckEquals( 0.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 12.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 12.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 112.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 56 ); // paying exactly half

    CheckEquals( 5.9999, partialPaymentOutput.PartialTotalTax(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_BasePrice );
    CheckEquals( 56.00,partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_PartialPaymentDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );

    CheckEquals( -11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 10.80, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 11.7, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 110.70, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 40 ); // paying 0.36 percent

    CheckEquals( -4.0469, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_Discounts );
    CheckEquals( 3.252, partialPaymentOutput.PartialServiceCharge(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.3252, partialPaymentOutput.PartialServiceChargeTax(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 4.2276, partialPaymentOutput.PartialTotalTax(), 0.0001, ERR_Tax );
    CheckEquals( 40.00, partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_PartialPaymentNoDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );
    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 56 ); // paying exactly half

    CheckEquals( 5.9999, partialPaymentOutput.PartialTax(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_BasePrice );
    CheckEquals( 56.00,partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_PartialPaymentDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 10.80, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 11.70, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 110.70, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 80 );

    CheckEquals( -7.2267, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_Discounts );
    CheckEquals( 6.5040, partialPaymentOutput.PartialServiceCharge(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6504, partialPaymentOutput.PartialServiceChargeTax(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 8.4552, partialPaymentOutput.PartialTotalTax(), 0.0001, ERR_Tax );
    CheckEquals( 80.00, partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_PartialPaymentNoDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );

    CheckEquals( 100.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );    // checking whether the total is correct before paying partially

    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 40 ); // paying exactly half

    CheckEquals( 0.00, partialPaymentOutput.PartialTax(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_BasePrice );
    CheckEquals( 40.00,partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------\

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_PartialPaymentDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.900, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.900, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.9, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 70 );

    CheckEquals( -7.0070, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_Discounts );
    CheckEquals( 6.3063, partialPaymentOutput.PartialServiceCharge(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6306, partialPaymentOutput.PartialServiceChargeTax(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.6306, partialPaymentOutput.PartialTotalTax(), 0.0001, ERR_Tax );
    CheckEquals( 70.00, partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_MultipleItems_PartialPaymentDiscServChargeServChargeTax()
{
    // first calculate the combination total as usual

    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;
    Currency taxOnDiscount = 0.00;
    Currency discountWithOutTax = 0.00;
    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;
    TPriceInfo item4;
    TPriceInfo item5;
    TPriceInfo item6;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );
    initializePriceInfoToTaxSettings( &item4 );
    initializePriceInfoToTaxSettings( &item5 );
    initializePriceInfoToTaxSettings( &item6 );

    item1.TaxInclusive = true;
    item1.Value = 450;
    item1.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );
    applyDiscountToPriceInfo( item1, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item1, "20perc", dtDiscount, dwPercentage, 20, 2, 0, false );

    item2.TaxInclusive = false;
    item2.Value = 200;
    item2.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );
    applyDiscountToPriceInfo( item2, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item2, "20perc", dtDiscount, dwPercentage, 20, 2, 0, true );

    item3.TaxInclusive = true;
    item3.Value = 350;
    item3.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );
    applyDiscountToPriceInfo( item3, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item3, "20perc", dtDiscount, dwPercentage, 20, 2, 0, true );

    item4.TaxInclusive = false;
    item4.Value = 750;
    item4.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );
    applyDiscountToPriceInfo( item4, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item4, "20perc", dtDiscount, dwPercentage, 20, 2, 0, true );

    item5.TaxInclusive = true;
    item5.Value = 175;
    item5.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );
    applyDiscountToPriceInfo( item5, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item5, "20perc", dtDiscount, dwPercentage, 20, 2, 0, true );

    item6.TaxInclusive = false;
    item6.Value = 525;
    item6.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );
    applyDiscountToPriceInfo( item6, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item6, "20perc", dtDiscount, dwPercentage, 20, 2, 0, true );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice,discountWithOutTax,taxOnDiscount );

    CheckEquals( -686.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 172.9286, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20.7514, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 72.7457, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1974.9600, finalPrice, 0.0001, ERR_Total );

    // then get the partial totals in the combination

    Currency completeTotal = finalPrice;
    discounts = 0.00;
    servCharge = 0.00;
    servChargeTax = 0.00;
    totalTax = 0.00;
    finalPrice = 0.00;

    calculatePartialValuesForItemSet( orders, completeTotal, 1000, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    // check whether the partial total is matching what we expect
    CheckEquals( -347.3475, discounts, 0.0001, ERR_Discounts );         // todo: rounding issues expected: 347.3488 actual 347.3475
    CheckEquals( 87.56, servCharge, 0.0001, ERR_ServiceCharge );        // todo: rounding issues expected: 87.5605 actual 87.56
    CheckEquals( 10.5066, servChargeTax, 0.0001, ERR_ServiceChargeTax );// todo: rounding issues expected: 10.5072 actual 10.5066
    CheckEquals( 36.833, totalTax, 0.0001, ERR_Tax );                   // todo: rounding issues expected: 36.8339 actual 36.833
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SimpleMultiTaxes_NoDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 27.00,  getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 127.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 12.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 15.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SimpleMultiTaxes_DiscNoSCNoSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),     0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -12.70, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  24.30, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 114.30, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.50,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -2.7, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SimpleMultiTaxes_DiscSCSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -12.70, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 25.20, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 124.20, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.50,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -2.7, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SimpleMultiTaxes_PWDSCSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );

    applyPWDToCurrentPriceInfo( 10 );

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 25.20, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 124.20, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.50,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SimpleMultiTaxes_SCDSCSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->PriceTaxExempt = true;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );

    applySCDToCurrentPriceInfo( 20 );

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -20.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 8.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.80, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.80, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 88.80, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-20.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SimpleMultiTaxes_MultiDisc_PWD_NoSCNoSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney,       0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo(10);

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -42.1013, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 17.6328, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 82.9395, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.8367,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.7960,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );

}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo(10);

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -42.1013, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.5307, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6531, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 18.2859, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 90.1232, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.8367,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.7960,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_CompoundMultiTaxes_NoDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 28.80, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 128.80, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 12.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 16.80,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_CompoundMultiTaxes_DiscNoSCNoSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -12.88, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 25.92, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 115.92, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 15.12,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -2.88, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_CompoundMultiTaxes_DiscSCSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -12.88, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 26.82, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 125.82, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 15.12,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -2.88, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_CompoundMultiTaxes_PWDSCSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyPWDToCurrentPriceInfo( 10 );
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 26.82, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 125.82, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 15.12,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_CompoundMultiTaxes_MultiDisc_PWD_NoSCNoSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo( 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -42.5298, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 18.8225, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 84.1787, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.8427,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 10.9798,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_CompoundMultiTaxes_MultiDisc_PWD_SCSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo( 10 );

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -42.5298, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.5356, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6536, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 19.4761, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 91.3679, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.8427,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 10.9798,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SimpleMultiTaxes_NoDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 27.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 127.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 12.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 15.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SimpleMultiTaxes_DiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 24.30, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 114.30, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.50,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SimpleMultiTaxes_DiscSCSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 25.20, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 124.20, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.50,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SimpleMultiTaxes_PWDSCSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyPWDToCurrentPriceInfo( 10 );
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 25.20, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 124.20, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.50,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SimpleMultiTaxes_MultiDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 3, 0, true );         // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -36.15, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 17.2395, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 81.0895, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.662,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.5775,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-36.15, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo( 10 );

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -35.65, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.435, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6435, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 18.018, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 88.803, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.722,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.6525,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_CompoundMultiTaxes_NoDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 28.80, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 128.80, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 12.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 16.80,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_CompoundMultiTaxes_DiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 25.92, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 115.92, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 15.12,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_CompoundMultiTaxes_DiscSCSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 26.82, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 125.82, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 15.12,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_CompoundMultiTaxes_PWDSCSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyPWDToCurrentPriceInfo( 10 );
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 26.82, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 125.82, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 10.80,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 15.12,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_CompoundMultiTaxes_MultiDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 3, 0, true );         // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -36.15, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 18.3888, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 82.2388, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.662,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 10.7268,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-36.15, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_CompoundMultiTaxes_MultiDiscSCSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 3, 0, true );         // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -36.15, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.385, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6385, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 19.0273, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 89.2623, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.662,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 10.7268,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-36.15, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SimpleMultiTaxes_NoDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 100.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SimpleMultiTaxes_DiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax1_0";
    UnicodeString tax_name_2 = "SalesTax2_0";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 90.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SimpleMultiTaxes_DiscSCSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax1_0";
    UnicodeString tax_name_2 = "SalesTax2_0";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.90, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 99.90, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SimpleMultiTaxes_MultiDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax1_0";
    UnicodeString tax_name_2 = "SalesTax2_0";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 3, 0, true );         // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -36.15, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 63.85, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-36.15, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SimpleMultiTaxes_MultiDiscSCSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax1_00";
    UnicodeString tax_name_2 = "SalesTax2_00";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 3, 0, true );         // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -36.15, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.385, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6385, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.6385, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 70.8735, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-36.15, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_CompoundMultiTaxes_NoDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 100.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_CompoundMultiTaxes_DiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax1_00";
    UnicodeString tax_name_2 = "SalesTax2_00";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 90.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_CompoundMultiTaxes_DiscSCSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax1_00";
    UnicodeString tax_name_2 = "SalesTax2_00";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.90, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 99.90, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-10.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_CompoundMultiTaxes_MultiDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax1_00";
    UnicodeString tax_name_2 = "SalesTax2_00";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 3, 0, true );         // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -36.15, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                            // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 63.85, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-36.15, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_CompoundMultiTaxes_MultiDiscSCSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax1_00";
    UnicodeString tax_name_2 = "SalesTax2_00";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 3, 0, true );         // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -36.15, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.385, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6385, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.6385, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 70.8735, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals(-36.15, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxNoDiscNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 12.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  17.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 117.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxNoDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 12.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  17.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 117.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxNoDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  5.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 105.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxRegularDiscNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -56.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  8.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 58.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -6.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxRegularDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  8.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 58.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxRegularDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  2.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 52.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxPWDNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyPWDToCurrentPriceInfo( 50 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  8.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 58.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxPWDNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyPWDToCurrentPriceInfo( 50 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  8.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 58.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxPWDNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyPWDToCurrentPriceInfo( 50 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  2.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 52.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxSCDNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applySCDToCurrentPriceInfo( 50 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  0.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 50.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxSCDNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applySCDToCurrentPriceInfo( 50 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  0.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 50.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxSCDNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applySCDToCurrentPriceInfo( 50 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  0.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 50.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxNoDiscServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 12.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 17.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 127.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxNoDiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 12.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  17.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 127.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxNoDiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  5.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 115.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxRegularDiscServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -56.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 8.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 63.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -6.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxRegularDiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 8.50, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 63.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxRegularDiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  2.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 57.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxPWDServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyPWDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 8.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 63.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxPWDServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyPWDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 8.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 63.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxPWDServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyPWDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 2.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 57.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxSCDServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applySCDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 55.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxSCDServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applySCDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 55.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxSCDServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applySCDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 55.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxNoDiscServChargeSCTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 12.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 1.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  18.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 128.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxNoDiscServChargeSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 12.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 1.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  18.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 128.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxNoDiscServChargeSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 1.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  6.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 116.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxRegularDiscServChargeSCTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -56.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.50, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  9.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 64.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( -6.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxRegularDiscServChargeSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.50, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  9.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 64.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxRegularDiscServChargeSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.50, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  3.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 58.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxPWDServChargeSCTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyPWDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.50, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 9.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 64.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxPWDServChargeSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyPWDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.50, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 9.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 64.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxPWDServChargeSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applyPWDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.50, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 3.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 58.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_LocalTaxSCDServChargeSCTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applySCDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.50, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 55.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_LocalTaxSCDServChargeSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applySCDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.50, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 55.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_LocalTaxSCDServChargeSCTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );

    applySCDToCurrentPriceInfo( 50 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.50, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 55.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals(-50.0, getDiscountWithoutTaxForCurrent(), 0.0001, ERR_DiscountWT );
    CheckEquals( 0.0, getTaxOnDiscountForCurrent(), 0.0001, ERR_TaxOnDiscount );
}
//------------------------------------------------------------------------------

void _fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );

    applyLocalTaxToCurrentPriceInfo( "LocalTax", 5.00 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo(10);

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -42.1013, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.5307, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6531, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 21.5511, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 93.3885, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.8367,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.7960,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 3.2653,
                 getTaxContentForCurrentResult("LocalTax"),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyLocalTaxToCurrentPriceInfo( "LocalTax", 5.00 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo( 10 );

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -35.65, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.435, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6435, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 21.2355, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 92.0205, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.722,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.6525,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 3.2175,
                 getTaxContentForCurrentResult("LocalTax"),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax1_00";
    UnicodeString tax_name_2 = "SalesTax2_00";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 2 );

    applyLocalTaxToCurrentPriceInfo( "LocalTax", 5.00 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo( 10 );

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -35.65, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.4350, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6435, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 3.8610, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 74.646, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 3.2175,
                 getTaxContentForCurrentResult("LocalTax"),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxIncItem_CompoundMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyLocalTaxToCurrentPriceInfo( "LocalTax", 5.00 );

    applyServiceChargeToCurrentPriceInfo( 10, 10 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo( 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -42.5298, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.5356, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6536, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 22.7439, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 94.6357, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.8427,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 10.9798,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 3.2678,
                 getTaxContentForCurrentResult("LocalTax"),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExcItem_CompoundMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );

    applyLocalTaxToCurrentPriceInfo( "LocalTax", 5.00 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo(10);

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -35.65, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.4350, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6435, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 22.3938, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                          // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 93.1788, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 7.7220,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 10.8108,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 3.2175,
                 getTaxContentForCurrentResult("LocalTax"),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Phillipines::Test_TaxExemptItem_CompoundMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 0.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 0.00, 1 );

    applyLocalTaxToCurrentPriceInfo( "LocalTax", 5.00 );

    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo(10);

    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -35.65, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.4350, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6435, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 3.8610, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                          // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 74.6460, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 3.2175,
                 getTaxContentForCurrentResult("LocalTax"),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TTestTMMBillCalculator_Phillipines::initializePriceInfoToTaxSettings( TPriceInfo* priceInfoObj )
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
    priceInfoObj->CalcDWTOnBasePrice = false;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Phillipines::execute()
{
    billCalcResponseObj = billCalculator.Execute( priceInfoObj );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Phillipines::execute( TPriceInfo &priceInfo )
{
    billCalcResponseObj = billCalculator.Execute( &priceInfo );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Phillipines::applyTaxToCurrentPriceInfo( string name, Currency percentage, int priority )
{
    applyTaxToPriceInfo( *priceInfoObj, name, percentage, priority );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Phillipines::applyLocalTaxToCurrentPriceInfo( string name, Currency percentage )
{
    applyLocalTaxToPriceInfo( *priceInfoObj, name, percentage );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Phillipines::applyServiceChargeToCurrentPriceInfo( Currency scPerc, Currency scTaxPerc )
{
    applyServiceChargeToPriceInfo( *priceInfoObj, scPerc, scTaxPerc );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Phillipines::applyDiscountToCurrentPriceInfo( string name, TDiscountType discType, TDiscountWay mode, Currency percentage, int priority, Currency value, bool isTaxExclusive )
{
    applyDiscountToPriceInfo( *priceInfoObj, name, discType, mode, percentage, priority, value, isTaxExclusive );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Phillipines::applySCDToCurrentPriceInfo( Currency percentage )
{
    applySCDToPriceInfo( *priceInfoObj, percentage );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Phillipines::applyPWDToCurrentPriceInfo( Currency percentage )
{
    applyPWDToPriceInfo( *priceInfoObj, percentage );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Phillipines::applyTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage, int priority )
{
    TTaxInfo tax;
    tax.Name = name;
    tax.Percentage = percentage;
    tax.Priority = priority;
    tax.TaxType = ttSale;

    priceInfo.TaxInfoList.push_back( tax );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Phillipines::applyLocalTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage )
{
    TTaxInfo tax;
    tax.Name = name;
    tax.Percentage = percentage;
    tax.Priority = 1;
    tax.TaxType = ttLocal;

    priceInfo.TaxInfoList.push_back( tax );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Phillipines::applyServiceChargeToPriceInfo( TPriceInfo &priceInfo, Currency scPerc, Currency scTaxPerc )
{
    priceInfo.ServiceChargeInfo.Percentage = scPerc;
    priceInfo.ServiceChargeInfo.TaxPercentage = scTaxPerc;
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Phillipines::applyDiscountToPriceInfo( TPriceInfo &priceInfo, string name, TDiscountType discType, TDiscountWay mode, Currency percentage, int priority, Currency value, bool isTaxExclusive )
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

void TTestTMMBillCalculator_Phillipines::applySCDToPriceInfo( TPriceInfo &priceInfo, Currency percentage )
{
    TDiscountInfo discInfo;
    discInfo.Name = "SCD";
    discInfo.DiscountType = dtDiscount;
    discInfo.DiscountWay = dwPercentage;
    discInfo.Percentage = percentage;

    priceInfo.PriceTaxExempt = true;
    priceInfo.CalcDWTOnBasePrice = true;
    priceInfo.DiscountInfoList.push_back( discInfo );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Phillipines::applyPWDToPriceInfo( TPriceInfo &priceInfo, Currency percentage )
{
    TDiscountInfo discInfo;
    discInfo.Name = "PWD";
    discInfo.DiscountType = dtDiscount;
    discInfo.DiscountWay = dwPercentage;
    discInfo.Percentage = percentage;
    discInfo.RecalcPriceWithTaxAfterDiscount = true;
    discInfo.TaxExclusiveDiscount = true;
    priceInfo.CalcDWTOnBasePrice = true;
    priceInfo.DiscountInfoList.push_back( discInfo );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Phillipines::calculateValuesForItemSet(
                                                    std::vector<TPriceInfo> &items,
                                                    Currency &discounts,
                                                    Currency &servCharge,
                                                    Currency &servChargeTax,
                                                    Currency &totalTax,
                                                    Currency &finalPrice,
                                                    Currency &discountWithOutTax,
                                                    Currency &taxOnDiscount)
{
    std::vector<TPriceInfo>::iterator it = items.begin();

    for( ; it != items.end() ; it++ )
    {
        execute( *it );
        discounts += getTotalDiscountForCurrentResult();
        servCharge += getServiceChargeForCurrentResult();
        servChargeTax += getServiceChargeTaxForCurrentResult();
        totalTax += getTotalTaxForCurrentResult();
        finalPrice += getFinalPriceForCurrentResult();
        discountWithOutTax+=getDiscountWithoutTaxForCurrent();
        taxOnDiscount +=getTaxOnDiscountForCurrent();
    }
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Phillipines::calculatePartialValuesForItemSet(
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
        execute( *it );
        currentItemTotal = getFinalPriceForCurrentResult();
        partialPaymentAmountPerItem = (partialPaymentPercentage * currentItemTotal) / 100.0000;

        partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );
        partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, partialPaymentAmountPerItem );

        discounts += partialPaymentOutput.PartialDiscount();
        servCharge += partialPaymentOutput.PartialServiceCharge();
        servChargeTax += partialPaymentOutput.PartialServiceChargeTax();
        totalTax += partialPaymentOutput.PartialTotalTax();
    }
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getTotalTaxForCurrentResult()
{
    return getTotalTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getDiscountXForCurrentResult( int X )
{
    return getDiscountX( billCalcResponseObj, X );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getTotalDiscountForCurrentResult()
{
    return getTotalDiscount( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getFinalPriceForCurrentResult()
{
    return getFinalPrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getBasePriceForCurrentResult()
{
    return getBasePrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getServiceChargeForCurrentResult()
{
    return getServiceCharge( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getServiceChargeTaxForCurrentResult()
{
    return getServiceChargeTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getTaxContentForCurrentResult( UnicodeString taxName )
{
    return getTaxContent( billCalcResponseObj, taxName );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getTotalTax( TBillCalcResponse* response )
{
    return response->Result.TotalTax;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getDiscountX( TBillCalcResponse* response, int X )
{
    return response->Result.Discount[X].Content;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getTotalDiscount( TBillCalcResponse* response )
{
    return response->Result.TotalDiscount;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getFinalPrice( TBillCalcResponse* response )
{
    return response->Result.FinalPrice;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getBasePrice( TBillCalcResponse* response )
{
    return response->Result.BasePrice;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getServiceCharge( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.Value;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getServiceChargeTax( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.TaxValue;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getTaxContent( TBillCalcResponse* response, UnicodeString taxName )
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
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getDiscountWithoutTaxForCurrent( )
{
    return getDiscountWithoutTax(billCalcResponseObj);
}

//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getTaxOnDiscountForCurrent(  )
{
    return getTaxOnDiscount(billCalcResponseObj);
}

//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getDiscountWithoutTax( TBillCalcResponse* response )
{
    return response->Result.DiscountWithoutTax;
}

//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Phillipines::getTaxOnDiscount( TBillCalcResponse* response )
{
    return response->Result.TaxOnDiscount;
}


static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("Testing_MMBillCalculator_Phillipines");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(TTestTMMBillCalculator_Phillipines));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 1


/* [For debug purposes only - To be removed soon!!]
GenerateHeaderComment=true
DefaultExtension=.cpp
FileName=C:\Code\Git\MenumateGit-LocalMirror\BillCalculator\BillCalculator_MacroTests\TestMMBillCalculator.cpp
TestFramework=DUnit / C++ Win32
OutputPersonality=CPlusPlusBuilder.Personality
TestProject=C:\Code\Git\MenumateGit-LocalMirror\BillCalculator\BillCalculator_MacroTests\BillCalc_MacroTests.cbproj
UnitUnderTest=C:\Code\Git\MenumateGit-LocalMirror\BillCalculator\MMBillCalculator.h
NameOfUnitUnderTest=MMBillCalculator.h
TestCaseBaseClass=TTestCase
TestCasePrefix=Test
UnitName=TestMMBillCalculator
Namespace=TestMMBillCalculator
TestClasses=
  <0>
    Name=TMMBillCalculator
    Methods=
      <0>
        Name=Execute
        Signature= TBillCalcResponse* Execute( const TPriceInfo* const inPriceInfo)
      <1>
        Name=Tax
        Signature= Currency Tax( const TPriceInfo* const inPriceInfo)
      <2>
        Name=Discount
        Signature= Currency Discount( const TPriceInfo* const inPriceInfo)
      <3>
        Name=ServiceCharge
        Signature= Currency ServiceCharge( const TPriceInfo* const inPriceInfo)
TestClass=
Method=
*/
