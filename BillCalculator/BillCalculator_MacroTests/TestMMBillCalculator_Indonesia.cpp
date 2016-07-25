//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "TestMMBillCalculator_Indonesia.h"

#define ERR_Total "Final price is wrong"
#define ERR_BasePrice "Base price is wrong"
#define ERR_DiscountX "Discount content is wrong"
#define ERR_Discounts "Discounts total is wrong"
#define ERR_Surcharges "Surcharges total is wrong"
#define ERR_ServiceCharge "Service charge is wrong"
#define ERR_ServiceChargeTax "Service charge tax is wrong"
#define ERR_Tax "Total tax is wrong"

__fastcall TTestTMMBillCalculator_Indonesia::TTestTMMBillCalculator_Indonesia(System::String name) : TTestCase(name)
{
}
//------------------------------------------------------------------------------

__fastcall TTestTMMBillCalculator_Indonesia::~TTestTMMBillCalculator_Indonesia()
{
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::SetUp()
{
    priceInfoObj = new TPriceInfo();
    initializePriceInfoToTaxSettings( priceInfoObj );

    delete billCalcResponseObj;
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::TearDown()
{
    delete priceInfoObj;
    delete billCalcResponseObj;
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_Execute_Empty()
{
    execute();

    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Tax Inclusive Items Tests
//------------------------------------------------------------------------------
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_NoDiscNoServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( 26785714.2857, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 223214285.7142, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 250000000, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_NoDiscServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( 22321428.5714, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 272321428.5714, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_DiscNoServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( 24107142.8571, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( -25000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 225000000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_HigherDiscNoServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "250150000 bucks", dtDiscount, dwMoney, 0, 0, 250150000 );

    execute();

    CheckEquals( -250000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_SetPriceDiscNoServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "$50000000 set price", dtDiscount, dwSetPrice, 0, 0, 50000000 );

    execute();

    CheckEquals( -200000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 5357142.8571, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 50000000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_SurchargeNoServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwPercentage, 10 );

    execute();

    CheckEquals( 25000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 275000000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_DiscServCharge()
{
    priceInfoObj->Value = 250000000;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( -25000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 24107142.8571, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 20089285.7142, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 245089285.7143, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_SetPriceDiscServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "$50000000 set price", dtDiscount, dwSetPrice, 0, 0, 50000000 );

    execute();

    CheckEquals( -200000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 5357142.8571, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 4464285.7142, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 54464285.7143, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_SurchargeServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwPercentage, 10  );

    execute();

    CheckEquals( 25000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 24553571.4286, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 299553571.4286, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_DollarSurchargeServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "20000000 bucks", dtSurcharge, dwMoney, 0, 1, 20000000.00);

    execute();

    CheckEquals(  20000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  24107142.8571, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 294107142.8571, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_DollarDiscountServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "20000000 bucksc", dtDiscount, dwMoney, 0, 0, 20000000.00 );

    execute();

    CheckEquals( -20000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 20535714.2857, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 250535714.2857, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_TaxExclDiscNoServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 0, 0, true );

    execute();

    CheckEquals( -22321428.5714, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 24394132.6531, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 227678571.4286, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_TaxExclDiscServCharge()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 0, 0, true );

    execute();

    CheckEquals( -22321428.5714, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 20328443.8775, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 24394132.6531, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 248007015.3061, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
*/

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_ServChargeServChargeTax()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( 22321428.5714, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 2232142.8571, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 29017857.1428, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 274553571.4285, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_DiscServChargeServChargeTax()
{
    priceInfoObj->Value = 250000000;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( -25000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 20089285.7142, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 2008928.5714, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 26116071.4285, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 247098214.2857, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_TaxExclDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 250000000.00;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 0, 0, true );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( -22321428.5714, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 20328443.8775, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 2032844.3877, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 26426977.0407, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 250039859.6938, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
*/


//------------------------------------------------------------------------------
//  Tax Exclusive Items Tests
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_NoDiscNoServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( 12000000.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 100000000.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 112000000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_NoDiscServCharge()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = false;

    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( 11200000.00 , getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 136640000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total ) ;
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_DiscNoServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( -10000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 100800000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
//*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_SetPriceDiscNoServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "set to $35000000", dtDiscount, dwSetPrice, 0, 1, 35000000.00 );

    execute();

    CheckEquals(  4200000.00,  getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( -65000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  39200000.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_DiscServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( -10000000.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9000000.0, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 10800000.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 109800000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
//*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_SetPriceDiscServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "set to $35000000", dtDiscount, dwSetPrice, 0, 1, 35000000.00 );

    execute();

    CheckEquals( -65000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  3500000.00,  getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  4200000.00,  getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  42700000.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_SurchargeServCharge()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwPercentage, 10 );

    execute();

    CheckEquals( 11200000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 12320000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 150304000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_DollarSurchargeServCharge()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "20000000 bucks", dtSurcharge, dwMoney, 0, 0, 20000000);

    execute();

    CheckEquals( 20000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 13200000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 161040000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_DollarDiscountServCharge()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwMoney, 0, 0, 20000000 );

    execute();

    CheckEquals( -20000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9200000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 112240000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_TaxExclDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 0, true );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 10.8, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 100.8, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_TaxExclDiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 0, true );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 10.80, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 109.80, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
*/
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_ServChargeServChargeTax()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = false;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( 10000000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 1000000.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 13000000.00, getTotalTaxForCurrentResult() , 0.0001, ERR_Tax );
    CheckEquals( 123000000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_DiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = false;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( -10000000.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9000000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 900000.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 11700000.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 110700000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_TaxExclDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = false;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 0, true );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 11.70, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 110.70, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
*/

//------------------------------------------------------------------------------
//  Tax Exempt Items Tests
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_NoDiscNoServCharge()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 112000000.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 112000000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//-----------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_NoDiscServCharge()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    CheckEquals( 11200000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 123200000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_DiscNoServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( -10000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  90000000.00,  getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_SetPriceDiscNoServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive   = false;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyDiscountToCurrentPriceInfo( "set to $35000000", dtDiscount, dwSetPrice, 0, 1, 35000000.00 );

    execute();

    CheckEquals(  0.00,  getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -65000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  35000000.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_DiscServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );

    execute();

    CheckEquals( -10000000.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9000000.0, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99000000.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_SetPriceDiscServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "$40000000 set price", dtDiscount, dwSetPrice, 0, 0, 40000000 );

    execute();

    CheckEquals( -60000000.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 4000000.0, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 44000000.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_SurchargeServCharge()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwPercentage, 10 );

    execute();

    CheckEquals( 11200000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 12320000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 135520000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_DollarSurchargeServCharge()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "20000000 bucks surcharge", dtSurcharge, dwMoney, 0, 0, 20000000);

    execute();

    CheckEquals( 20000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 13200000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 145200000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_DollarDiscountServCharge()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = false;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "20000000 bucks disc", dtDiscount, dwMoney, 0, 0, 20000000 );

    execute();

    CheckEquals( -20000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9200000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 101200000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_TaxExclDiscNoServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 0, true );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 90.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_TaxExclDiscServCharge()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 0, true );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
*/
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_ServChargeServChargeTax()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    CheckEquals( 10000000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 1000000.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 1000000.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 111000000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_DiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    CheckEquals( -10000000.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9000000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 900000.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 900000.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99900000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_TaxExclDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 0, true );
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.900, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.900, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.9, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
*/

//------------------------------------------------------------------------------
// Tests for set of items
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_MultipleItems_NoDiscNoServCharge()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;

    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );

    item1.TaxInclusive = true;
    item1.Value = 112000000;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );

    item2.TaxInclusive = false;
    item2.Value = 100000000;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );

    item3.TaxInclusive = true;
    item3.Value = 100000000;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( 0.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 0.00, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 24000000.00, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 324000000.00, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_MultipleItems_DiscServChargeServChargeTax()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;

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
    item1.Value = 450000000;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );
    applyDiscountToPriceInfo( item1, "10perc", dtDiscount, dwPercentage, 10 );

    item2.TaxInclusive = false;
    item2.Value = 200000000;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );
    applyDiscountToPriceInfo( item2, "10perc", dtDiscount, dwPercentage, 10 );

    item3.TaxInclusive = true;
    item3.Value = 350000000;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );
    applyDiscountToPriceInfo( item3, "10perc", dtDiscount, dwPercentage, 10 );

    item4.TaxInclusive = false;
    item4.Value = 750000000;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );
    applyDiscountToPriceInfo( item4, "10perc", dtDiscount, dwPercentage, 10 );

    item5.TaxInclusive = true;
    item5.Value = 175000000;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );
    applyDiscountToPriceInfo( item5, "10perc", dtDiscount, dwPercentage, 10 );

    item6.TaxInclusive = false;
    item6.Value = 525000000;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );
    applyDiscountToPriceInfo( item6, "10perc", dtDiscount, dwPercentage, 10 );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -245000000.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 216160714.2857, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 25939285.7142, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 90932142.8572, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 2468700000.00, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_MultipleItems_TaxExclDiscServChargeServChargeTax()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;

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
    applyDiscountToPriceInfo( item1, "10perc", dtDiscount, dwPercentage, 10, 0, 0, true );

    item2.TaxInclusive = false;
    item2.Value = 200;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );
    applyDiscountToPriceInfo( item2, "10perc", dtDiscount, dwPercentage, 10, 0, 0, true );

    item3.TaxInclusive = true;
    item3.Value = 350;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );
    applyDiscountToPriceInfo( item3, "10perc", dtDiscount, dwPercentage, 10, 0, 0, true );

    item4.TaxInclusive = false;
    item4.Value = 750;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );
    applyDiscountToPriceInfo( item4, "10perc", dtDiscount, dwPercentage, 10, 0, 0, true );

    item5.TaxInclusive = true;
    item5.Value = 175;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );
    applyDiscountToPriceInfo( item5, "10perc", dtDiscount, dwPercentage, 10, 0, 0, true );

    item6.TaxInclusive = false;
    item6.Value = 525;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );
    applyDiscountToPriceInfo( item6, "10perc", dtDiscount, dwPercentage, 10, 0, 0, true );

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -240.1785, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 216.5912, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 25.9909, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 91.5003, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 2474.0036, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
*/
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 1, 0, false );                       // Normal discount 100%
    applyDiscountToCurrentPriceInfo( "$50000000 set price", dtDiscount, dwSetPrice, 0, 2, 50000000, false );        // SetPrice discount $50000000

    execute();

    CheckEquals( -62000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 4464285.7142, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 446428.5714, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 5803571.4285, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 54910714.2857, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "$50000000 set price", dtDiscount, dwSetPrice, 0, 1, 50000000, false );        // SetPrice discount $50
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 2, 0, false );                       // Normal discount 100%

    execute();

    CheckEquals( -112000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 1, 0, false );                       // Normal discount 100%
    applyDiscountToCurrentPriceInfo( "$50000000 set price", dtDiscount, dwSetPrice, 0, 2, 50000000, false );        // SetPrice discount $50000000

    execute();

    CheckEquals( -62000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 5000000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 500000.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 6500000.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 61500000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "$50000000 set price", dtDiscount, dwSetPrice, 0, 1, 50000000, false );        // SetPrice discount $50000000
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 2, 0, false );                       // Normal discount 100%

    execute();

    CheckEquals( -112000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 1, 0, false );                       // Normal discount 100%
    applyDiscountToCurrentPriceInfo( "$50000000 set price", dtDiscount, dwSetPrice, 0, 2, 50000000, false );        // SetPrice discount $50

    execute();

    CheckEquals( -50000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 5000000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 500000.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 500000.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 55500000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "$50000000 set price", dtDiscount, dwSetPrice, 0, 1, 50000000, false );        // SetPrice discount $50
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 2, 0, false );                       // Normal discount 100%

    execute();

    CheckEquals( -100000000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_MultipleDiscounts_NormalTaxExclSetPriceDollarDisc_ServChargeServChargeTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 1, 0, false );            // Normal discount 10%
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 2, 50, false );       // SetPrice discount $50
    applyDiscountToCurrentPriceInfo( "10perc tax excl", dtDiscount, dwPercentage, 10, 3, 0, true );    // TaxExcl discount 10%
    applyDiscountToCurrentPriceInfo( "10 bucks", dtDiscount, dwMoney, 0, 4, 10, false );               // Dollar discount $10

    execute();

    CheckEquals( -76.4642, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );               // with all the discounts applied
    CheckEquals( 3.1728, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.3172, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 4.1245, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 39.0258, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

//    CheckEquals( -11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );                // only the first
//    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
//    CheckEquals( 0.9, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 11.7, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
//    CheckEquals( 110.7, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

//    CheckEquals( -62.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );               //  only the second
//    CheckEquals( 4.4642, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
//    CheckEquals( 0.4464, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 5.8035, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
//    CheckEquals( 54.9106, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
                                                                                                     // only the third
//    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
//    CheckEquals( 9.1071, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
//    CheckEquals( 0.9107, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 11.8392, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
//    CheckEquals( 112.0178, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
                                                                                                     // only the fourth
//    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
//    CheckEquals( 9.1071, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
//    CheckEquals( 0.9107, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 11.8392, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
//    CheckEquals( 112.0178, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

//    CheckEquals( -62.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );               // only 1st and 2nd
//    CheckEquals( 4.4642, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
//    CheckEquals( 0.4464, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 5.8035, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
//    CheckEquals( 54.9106, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

//    CheckEquals( -66.4642, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );           // only 1st, 2nd and 3rd
//    CheckEquals( 4.0657, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
//    CheckEquals( 0.4065, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
//    CheckEquals( 5.2853, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
//    CheckEquals( 50.008, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
*/
void __fastcall TTestTMMBillCalculator_Indonesia::Test_MultipleItems_CompoundMultipleNormalDiscountsServChargeServChargeTax()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;

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
    item1.Value = 450000000;
    item1.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );
    applyDiscountToPriceInfo( item1, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item1, "20perc", dtDiscount, dwPercentage, 20, 2 );

    item2.TaxInclusive = false;
    item2.Value = 200000000;
    item2.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );
    applyDiscountToPriceInfo( item2, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item2, "20perc", dtDiscount, dwPercentage, 20, 2 );

    item3.TaxInclusive = true;
    item3.Value = 350000000;
    item3.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );
    applyDiscountToPriceInfo( item3, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item3, "20perc", dtDiscount, dwPercentage, 20, 2 );

    item4.TaxInclusive = false;
    item4.Value = 750000000;
    item4.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );
    applyDiscountToPriceInfo( item4, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item4, "20perc", dtDiscount, dwPercentage, 20, 2 );

    item5.TaxInclusive = true;
    item5.Value = 175000000;
    item5.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );
    applyDiscountToPriceInfo( item5, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyDiscountToPriceInfo( item5, "20perc", dtDiscount, dwPercentage, 20, 2 );

    item6.TaxInclusive = false;
    item6.Value = 525000000;
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

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -686000000.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 172928571.4286, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20751428.5714, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 72745714.2856, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1974959999.9999, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_MultipleItems_CompoundMultipleMixedDiscountsServChargeServChargeTax_NormalTaxExcl()
{
    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;

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

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

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

    CheckEquals( -686.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 172.9285, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20.7514, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 72.7456, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1974.9599, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
*/
void __fastcall TTestTMMBillCalculator_Indonesia::Test_MultipleItems_CompoundMultipleMixedDiscounts()
{
    std::vector<TPriceInfo> orders;
    Currency discounts     = 0.00;
    Currency servCharge    = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax      = 0.00;
    Currency finalPrice    = 0.00;

    TPriceInfo item1;

    initializePriceInfoToTaxSettings( &item1 );

    item1.TaxInclusive      = false;
    item1.Value             = 450000000;
    item1.MultiDiscountType = mdtCompound;

    applyDiscountToPriceInfo( item1, "500000000 bucks", dtDiscount, dwMoney,       0, 2, 500000000 );
    applyDiscountToPriceInfo( item1, "10perc",    dtDiscount, dwPercentage, 10, 1 );

    orders.push_back( item1 );

    //execute();
    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -450000000.00, discounts,  0.0001, ERR_Discounts );
    CheckEquals(    0.00, finalPrice, 0.0001, ERR_Total     );

/*
    CheckEquals( -686.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 172.9285, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20.7514, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 72.7456, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1974.9599, finalPrice, 0.0001, ERR_Total );
*/
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_MultipleItems_SimpleMultipleSurchargePercentage()
{
    std::vector<TPriceInfo> orders;

    Currency discounts     = 0.00;
    Currency servCharge    = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax      = 0.00;
    Currency finalPrice    = 0.00;

    TPriceInfo item1;
    TPriceInfo item2;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );

    item1.TaxInclusive = false;
    item1.Value = 175000000.00;
    item1.MultiDiscountType = mdtCompound;
    item1.ServiceChargeInfo.Percentage = 14.00;
    item1.ServiceChargeBeforeDiscount = true;

    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item1, "37000000 bucks", dtSurcharge, dwMoney, 0, 1, 37000000 );
    //applyDiscountToPriceInfo( item1, "10perc", dtSurcharge, dwPercentage, 10, 1, 0 );

    item2.TaxInclusive = true;
    item2.Value = 175000000.00;
    item2.MultiDiscountType = mdtSimple;
    item2.ServiceChargeInfo.Percentage = 14.00;
    item2.ServiceChargeBeforeDiscount = true;

    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item2, "37000000 bucks", dtSurcharge, dwMoney, 0, 1, 37000000 );
//    applyDiscountToPriceInfo( item2, "10perc", dtSurcharge, dwPercentage, 10, 1 );

    orders.push_back( item1 );
    //orders.push_back( item2 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

// individual items
    CheckEquals(  37000000.000, discounts,     0.0001, ERR_Discounts );
    CheckEquals(  24500000.00, servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals(   0.000, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  25440000.00, totalTax,      0.0001, ERR_Tax );
    CheckEquals( 261940000.00, finalPrice,    0.0001, ERR_Total );

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

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_PartialPaymentNoDiscNoServCharge()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );
    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 56000000 ); // paying exactly half

    CheckEquals( 5999952.00, partialPaymentOutput.PartialTotalTax(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_BasePrice );
    CheckEquals( 56000000.00,partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxIncItem_PartialPaymentDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 112000000;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );

    CheckEquals( -11200000.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9000000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 900000.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 11700000.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 110700000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 40000000 ); // paying 0.36 percent

    CheckEquals( -4046960.00, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_Discounts );
    CheckEquals( 3252000.00, partialPaymentOutput.PartialServiceCharge(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 325200.00, partialPaymentOutput.PartialServiceChargeTax(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 4227600.00, partialPaymentOutput.PartialTotalTax(), 0.0001, ERR_Tax );
    CheckEquals( 40000000.00, partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_PartialPaymentNoDiscNoServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = false;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );
    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 56000000 );

    CheckEquals( 5999952.00, partialPaymentOutput.PartialTax(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_BasePrice );
    CheckEquals( 56000000.00,partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExcItem_PartialPaymentDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = false;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );

    CheckEquals( -10000000.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9000000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 900000.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 11700000.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 110700000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 80000000
 );

    CheckEquals( -7226720.00, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_Discounts );
    CheckEquals( 6504000.00, partialPaymentOutput.PartialServiceCharge(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 650400.00, partialPaymentOutput.PartialServiceChargeTax(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 8455200.00, partialPaymentOutput.PartialTotalTax(), 0.0001, ERR_Tax );
    CheckEquals( 80000000.00, partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_PartialPaymentNoDiscNoServCharge()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );

    CheckEquals( 100000000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );    // checking whether the total is correct before paying partially

    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 40000000 ); // paying exactly half

    CheckEquals( 0.00, partialPaymentOutput.PartialTax(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_BasePrice );
    CheckEquals( 40000000.00,partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------\

void __fastcall TTestTMMBillCalculator_Indonesia::Test_TaxExemptItem_PartialPaymentDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 100000000;
    priceInfoObj->TaxInclusive = true;

    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 0 );

    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );

    CheckEquals( -10000000.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9000000.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 900000.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 900000.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99900000.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 70000000 );

    CheckEquals( -7007000.00, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_Discounts );
    CheckEquals( 6306300.00, partialPaymentOutput.PartialServiceCharge(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 630630.00, partialPaymentOutput.PartialServiceChargeTax(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 630630.00, partialPaymentOutput.PartialTotalTax(), 0.0001, ERR_Tax );
    CheckEquals( 70000000.00, partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
/*
void __fastcall TTestTMMBillCalculator_Indonesia::Test_MultipleItems_PartialPaymentDiscServChargeServChargeTax()
{
    // first calculate the combination total as usual

    std::vector<TPriceInfo> orders;
    Currency discounts = 0.00;
    Currency servCharge = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax = 0.00;
    Currency finalPrice = 0.00;

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

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -686.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 172.9285, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20.7514, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 72.7456, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1974.9599, finalPrice, 0.0001, ERR_Total );

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
*/

//------------------------------------------------------------------------------
// PRIVATE SECTION
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Indonesia::initializePriceInfoToTaxSettings( TPriceInfo* priceInfoObj )
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
void TTestTMMBillCalculator_Indonesia::execute()
{
    billCalcResponseObj = billCalculator.Execute( priceInfoObj );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Indonesia::execute( TPriceInfo &priceInfo )
{
    billCalcResponseObj = billCalculator.Execute( &priceInfo );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Indonesia::applyTaxToCurrentPriceInfo( string name, Currency percentage, int priority )
{
    applyTaxToPriceInfo( *priceInfoObj, name, percentage, priority );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Indonesia::applyServiceChargeToCurrentPriceInfo( Currency scPerc, Currency scTaxPerc )
{
    applyServiceChargeToPriceInfo( *priceInfoObj, scPerc, scTaxPerc );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Indonesia::applyDiscountToCurrentPriceInfo( string name, TDiscountType discType, TDiscountWay mode, Currency percentage, int priority, Currency value, bool isTaxExclusive )
{
    applyDiscountToPriceInfo( *priceInfoObj, name, discType, mode, percentage, priority, value, isTaxExclusive );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Indonesia::applySCDToCurrentPriceInfo( Currency percentage )
{
    applySCDToPriceInfo( *priceInfoObj, percentage );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Indonesia::applyTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage, int priority )
{
    TTaxInfo tax;
    tax.Name = name;
    tax.Percentage = percentage;
    //tax.Priority = priority;  //todo: remove

    priceInfo.TaxInfoList.push_back( tax );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Indonesia::applyServiceChargeToPriceInfo( TPriceInfo &priceInfo, Currency scPerc, Currency scTaxPerc )
{
    priceInfo.ServiceChargeInfo.Percentage = scPerc;
    priceInfo.ServiceChargeInfo.TaxPercentage = scTaxPerc;
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_Indonesia::applyDiscountToPriceInfo( TPriceInfo &priceInfo, string name, TDiscountType discType, TDiscountWay mode, Currency percentage, int priority, Currency value, bool isTaxExclusive )
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

void TTestTMMBillCalculator_Indonesia::applySCDToPriceInfo( TPriceInfo &priceInfo, Currency percentage )
{
    TDiscountInfo discInfo;
    discInfo.Name = "SCD";
    discInfo.DiscountType = dtDiscount;
    discInfo.DiscountWay = dwPercentage;
    discInfo.Percentage = percentage;

    priceInfo.DiscountInfoList.push_back( discInfo );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Indonesia::calculateValuesForItemSet(
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
        discounts += getTotalDiscountForCurrentResult();
        servCharge += getServiceChargeForCurrentResult();
        servChargeTax += getServiceChargeTaxForCurrentResult();
        totalTax += getTotalTaxForCurrentResult();
        finalPrice += getFinalPriceForCurrentResult();
    }
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_Indonesia::calculatePartialValuesForItemSet(
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
Currency TTestTMMBillCalculator_Indonesia::getTotalTaxForCurrentResult()
{
    return getTotalTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getDiscountXForCurrentResult( int X )
{
    return getDiscountX( billCalcResponseObj, X );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getTotalDiscountForCurrentResult()
{
    return getTotalDiscount( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getFinalPriceForCurrentResult()
{
    return getFinalPrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getBasePriceForCurrentResult()
{
    return getBasePrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getServiceChargeForCurrentResult()
{
    return getServiceCharge( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getServiceChargeTaxForCurrentResult()
{
    return getServiceChargeTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getTaxContentForCurrentResult( UnicodeString taxName )
{
    return getTaxContent( billCalcResponseObj, taxName );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getTotalTax( TBillCalcResponse* response )
{
    return response->Result.TotalTax;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getDiscountX( TBillCalcResponse* response, int X )
{
    return response->Result.Discount[X].Content;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getTotalDiscount( TBillCalcResponse* response )
{
    return response->Result.TotalDiscount;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getFinalPrice( TBillCalcResponse* response )
{
    return response->Result.FinalPrice;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getBasePrice( TBillCalcResponse* response )
{
    return response->Result.BasePrice;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getServiceCharge( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.Value;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getServiceChargeTax( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.TaxValue;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_Indonesia::getTaxContent( TBillCalcResponse* response, UnicodeString taxName )
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
  TTestSuite* testSuite = new TTestSuite("Testing_MMBillCalculator_Indonesia");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(TTestTMMBillCalculator_Indonesia));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 2
