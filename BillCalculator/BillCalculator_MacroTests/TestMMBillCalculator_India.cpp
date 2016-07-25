//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TestMMBillCalculator_India.h"

#define ERR_Total "Final price is wrong"
#define ERR_BasePrice "Base price is wrong"
#define ERR_DiscountX "Discount content is wrong"
#define ERR_Discounts "Discounts total is wrong"
#define ERR_Surcharges "Surcharges total is wrong"
#define ERR_ServiceCharge "Service charge is wrong"
#define ERR_ServiceChargeTax "Service charge tax is wrong"
#define ERR_Tax "Total tax is wrong"
#define ERR_TaxContent "Tax content is wrong"
//---------------------------------------------------------------------------

__fastcall TTestTMMBillCalculator_India::TTestTMMBillCalculator_India(System::String name) : TTestCase(name)
{
}
//------------------------------------------------------------------------------

__fastcall TTestTMMBillCalculator_India::~TTestTMMBillCalculator_India()
{
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::SetUp()
{
    priceInfoObj = new TPriceInfo();
    initializePriceInfoToTaxSettings( priceInfoObj );

    delete billCalcResponseObj;
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::TearDown()
{
    delete priceInfoObj;
    delete billCalcResponseObj;
}
//------------------------------------------------------------------------------


void __fastcall TTestTMMBillCalculator_India::Test_Execute_Empty()
{
    execute();
    CheckEquals( 0.00, billCalcResponseObj->Result.TotalTax, 0.0001, ERR_Tax );
    CheckEquals( 0.00, billCalcResponseObj->Result.TotalDiscount, 0.0001, ERR_Discounts );
    CheckEquals( 0.00, billCalcResponseObj->Result.FinalPrice, 0.0001, ERR_Total );
    CheckEquals( 0.00, billCalcResponseObj->Result.BasePrice, 0.0001, ERR_BasePrice );
    CheckEquals( 0.00, billCalcResponseObj->Result.ServiceCharge.Value, 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, billCalcResponseObj->Result.ServiceCharge.TaxValue, 0.0001, ERR_ServiceChargeTax );
}

//------------------------------------------------------------------------------
//  Tax Inclusive Items Tests
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_NoDiscNoServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 12.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 112.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_NoDiscServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 122.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_DiscNoServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.8, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( -11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 100.8, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_HigherDiscNoServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyDiscountToCurrentPriceInfo( "150 bucks", dtDiscount, dwMoney, 0, 0, 150 );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( -112.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SetPriceDiscNoServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 0, 50 );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( -62.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 5.3571, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SurchargeNoServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwPercentage, 10 );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 123.2, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_DiscServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( -11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(   9.0, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 109.8, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SetPriceDiscServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 0, 50 );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( -62.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 5.3571, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 4.4643, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 54.4643, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SurchargeServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "10perc", dtSurcharge, dwPercentage, 10  );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 11.0, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 13.2, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 134.2, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_DollarSurchargeServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "20 bucks", dtSurcharge, dwMoney, 0, 1, 20);
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  20.000, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  11.7857, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 14.1429, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 143.7857, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_DollarDiscountServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyDiscountToCurrentPriceInfo( "20 bucksc", dtDiscount, dwMoney, 0, 0, 20 );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( -20.000, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 8.2143, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 9.8571, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 100.2143, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SCDNoServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applySCDToCurrentPriceInfo( 10 );
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 90.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SCDServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applySCDToCurrentPriceInfo( 10 );
    execute();
    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.0, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_PWDDiscNoServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyPWDToCurrentPriceInfo(20);
    execute();
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( -20.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.60, getTaxContentForCurrentResult("SalesTax1"), 0.0001, ERR_TaxContent );
    CheckEquals( 9.60, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 89.6, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_PWDDiscServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );
    applyPWDToCurrentPriceInfo(20);
    execute();
    CheckEquals( -20.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 8.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 9.6, getTaxContentForCurrentResult("SalesTax1"), 0.0001, ERR_TaxContent );
    CheckEquals( 9.60, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 97.60, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_ServChargeServChargeTax()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );    // second parameter is for service charge tax
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    execute();
    CheckEquals( 0.0, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 1.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 13.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 123.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_DiscServChargeServChargeTax()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10 );
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    execute();
    CheckEquals( 0.0, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( -11.2, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 11.7, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 110.70, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_PWDDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyPWDToCurrentPriceInfo(20);
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    execute();
     CheckEquals( 0.0, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( -20.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 8.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.80, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 9.6, getTaxContentForCurrentResult("SalesTax1"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.40, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 98.40, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SCDServChargeServChargeTax()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applySCDToCurrentPriceInfo( 10 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 10.0, getDiscountXForCurrentResult( 0 ), 0.0001, ERR_DiscountX );
    CheckEquals( -10.0, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
     CheckEquals( 0.0, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax1"), 0.0001, ERR_Tax );
    CheckEquals( 0.90, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 99.90, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Tests for set of items
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_NoDiscNoServCharge()
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
    item1.Value = 124;
    applyTaxToPriceInfo( item1, "Sales tax", 12);
    applyTaxToPriceInfo(item1,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item1, "Sales tax", 12, 1 );
    item2.TaxInclusive = false;
    item2.Value = 100;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    item3.TaxInclusive = true;
    item3.Value = 100;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );
    CheckEquals( 0.00, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 0.00, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 24.00, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 324.00, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_DiscServChargeServChargeTax()
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
    item1.Value = 124;
    applyTaxToPriceInfo( item1, "Sales tax", 12);
    applyTaxToPriceInfo(item1,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item1, "Sales tax", 12, 1 );
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

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -211.2, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 189, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 22.68, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 55.08, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 2134.08, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_PWDDiscServChargeServChargeTax()
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
    applyTaxToPriceInfo(item1,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item1, "Sales tax", 12, 1 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );
    applyPWDToPriceInfo( item1, 10);

    item2.TaxInclusive = false;
    item2.Value = 200;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );
    applyPWDToPriceInfo( item2, 10);

    item3.TaxInclusive = true;
    item3.Value = 350;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );
    applyPWDToPriceInfo( item3, 10);

    item4.TaxInclusive = false;
    item4.Value = 750;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );
    applyPWDToPriceInfo( item4, 10);

    item5.TaxInclusive = true;
    item5.Value = 175;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );
    applyPWDToPriceInfo( item5, 10);

    item6.TaxInclusive = false;
    item6.Value = 525;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );
    applyPWDToPriceInfo( item6, 10);

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -236.2903, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 212.6613, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 25.5194, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 86.313, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 2425.5872, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_SCDServChargeServChargeTax()
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
    item1.PriceTaxExempt = true;
    item1.Value = 450;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyTaxToPriceInfo(item1,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item1, "Sales tax", 12, 1 );
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

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -236.2903, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 212.6613, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 25.5194, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTaxContentForCurrentResult("Sales tax"), 0.0001, ERR_Tax );
    CheckEquals( 25.5194, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 2364.7936, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 1, 0, false );          // Normal discount 100%
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 2, 50, false );       // SetPrice discount $50
    execute();
    CheckEquals( -62.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 4.4643, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.4464, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 5.8035, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 54.9107, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 1, 50, false );       // SetPrice discount $50
    applyDiscountToCurrentPriceInfo( "100perc", dtDiscount, dwPercentage, 100, 2, 0, false );          // Normal discount 100%

    execute();

    CheckEquals( -112.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_MultipleDiscounts_NormalPWDSetPriceDollarDisc_ServChargeServChargeTax()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtCompound;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10perc", dtDiscount, dwPercentage, 10, 1, 0, false );            // Normal discount 10%
    applyDiscountToCurrentPriceInfo( "$50 set price", dtDiscount, dwSetPrice, 0, 2, 50, false );       // SetPrice discount $50
    applyDiscountToCurrentPriceInfo( "10 bucks", dtDiscount, dwMoney, 0, 4, 10, false );               // Dollar discount $10
    applyPWDToCurrentPriceInfo(10);
    execute();
    CheckEquals( -75.5714, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 3.2143, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.3214, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 4.1785, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 39.5356, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_CompoundMultipleNormalDiscountsServChargeServChargeTax()
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
    applyTaxToPriceInfo( item1, "Sales tax", 12);
    applyTaxToPriceInfo(item1,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item1, "Sales tax", 12, 1 );
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

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -673.8065, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 170.1290, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20.4154, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 69.0504, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1940.4696, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_CompoundMultipleMixedDiscountsServChargeServChargeTax_NormalPWD()
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
    applyTaxToPriceInfo( item1, "Sales tax", 12);
    applyTaxToPriceInfo(item1,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item1, "Sales tax", 12, 1 );
    applyServiceChargeToPriceInfo ( item1, 10, 12 );
    applyDiscountToPriceInfo( item1, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyPWDToPriceInfo( item1, 20);

    item2.TaxInclusive = false;
    item2.Value = 200;
    item2.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item2, "Sales tax", 12.00 );
    applyServiceChargeToPriceInfo ( item2, 10, 12 );
    applyDiscountToPriceInfo( item2, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyPWDToPriceInfo( item2, 20);

    item3.TaxInclusive = true;
    item3.Value = 350;
    item3.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item3, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item3, 10, 12 );
    applyDiscountToPriceInfo( item3, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyPWDToPriceInfo( item3, 20);

    item4.TaxInclusive = false;
    item4.Value = 750;
    item4.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item4, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item4, 10, 12 );
    applyDiscountToPriceInfo( item4, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyPWDToPriceInfo( item4, 20);

    item5.TaxInclusive = true;
    item5.Value = 175;
    item5.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item5, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item5, 10, 12 );
    applyDiscountToPriceInfo( item5, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyPWDToPriceInfo( item5, 20);

    item6.TaxInclusive = false;
    item6.Value = 525;
    item6.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item6, "Sales tax", 0.00 );
    applyServiceChargeToPriceInfo ( item6, 10, 12 );
    applyDiscountToPriceInfo( item6, "10perc", dtDiscount, dwPercentage, 10, 1 );
    applyPWDToPriceInfo( item6, 20);

    orders.push_back( item1 );
    orders.push_back( item2 );
    orders.push_back( item3 );
    orders.push_back( item4 );
    orders.push_back( item5 );
    orders.push_back( item6 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -665.9678, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 170.1290, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20.4154, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 69.0504, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1940.4695, finalPrice, 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_CompoundMultipleMixedDiscounts()
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
    item1.TaxInclusive      = false;
    item1.Value             = 450;
    item1.MultiDiscountType = mdtCompound;
    applyDiscountToPriceInfo( item1, "500 bucks", dtDiscount, dwMoney,       0, 2, 500 );
    applyDiscountToPriceInfo( item1, "10perc",    dtDiscount, dwPercentage, 10, 1 );

    item2.TaxInclusive      = false;
    item2.Value             = 450;
    item2.MultiDiscountType = mdtCompound;
    applyDiscountToPriceInfo( item2, "500 bucks", dtDiscount, dwMoney,       0, 2, 500 );
    applyDiscountToPriceInfo( item2, "10perc",    dtDiscount, dwPercentage, 10, 1 );
    orders.push_back( item1 );
    orders.push_back( item2 );
    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );
    CheckEquals( -900.00, discounts,  0.0001, ERR_Discounts );
    CheckEquals(    0.00, finalPrice, 0.0001, ERR_Total     );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_SimpleMultipleSurchargePercentage()
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
    item1.Value = 175.00;
    item1.MultiDiscountType = mdtCompound;
    item1.ServiceChargeInfo.Percentage = 14.00;
    item1.ServiceChargeBeforeDiscount = true;
    applyTaxToPriceInfo( item1, "Sales tax", 12.00 );
    applyDiscountToPriceInfo( item1, "37 bucks", dtSurcharge, dwMoney, 0, 1, 37 );

    item2.TaxInclusive = true;
    item2.Value = 175.00;
    item2.MultiDiscountType = mdtSimple;
    item2.ServiceChargeInfo.Percentage = 14.00;
    item2.ServiceChargeBeforeDiscount = true;
    applyTaxToPriceInfo( item2, "Sales tax", 12);
    applyTaxToPriceInfo(item2,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item2, "Sales tax", 12, 1 );
    applyDiscountToPriceInfo( item2, "37 bucks", dtSurcharge, dwMoney, 0, 1, 37 );
    orders.push_back( item1 );
    orders.push_back( item2 );

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals(  74.000, discounts,     0.0001, ERR_Discounts );
    CheckEquals(  44.2581, servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals(   0.000, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  46.3398, totalTax,      0.0001, ERR_Tax );
    CheckEquals( 476.7626, finalPrice,    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_EqualPriorityDiscountsOver100Perc()
{
    std::vector<TPriceInfo> orders;
    Currency discounts     = 0.00;
    Currency servCharge    = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax      = 0.00;
    Currency finalPrice    = 0.00;

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
    applyTaxToPriceInfo( item1, "Sales tax", 12);
    applyTaxToPriceInfo(item1,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item1, "Sales tax", 12, 1 );
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

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( -2406.4516, discounts,     0.0001, ERR_Discounts );
    CheckEquals( 0.0000,     servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000,     servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.0000,     totalTax,      0.0001, ERR_Tax );
    CheckEquals( 0.0000,     finalPrice,    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_HigherSurcharges()
{
    std::vector<TPriceInfo> orders;
    Currency discounts     = 0.00;
    Currency servCharge    = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax      = 0.00;
    Currency finalPrice    = 0.00;

    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );

    item1.Value = 10;
    item1.TaxInclusive = true;
    applyTaxToPriceInfo( item1, "Sales tax", 12);
    applyTaxToPriceInfo(item1,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item1, "Sales tax", 12, 1 );
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

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( 71.00, discounts,     0.0001, ERR_Discounts );
    CheckEquals( 0.00,     servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00,     servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 13.932,     totalTax,      0.0001, ERR_Tax );
    CheckEquals( 130.0322,     finalPrice,    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_HigherSurchargesWithDiscounts()
{
    std::vector<TPriceInfo> orders;
    Currency discounts     = 0.00;
    Currency servCharge    = 0.00;
    Currency servChargeTax = 0.00;
    Currency totalTax      = 0.00;
    Currency finalPrice    = 0.00;

    TPriceInfo item1;
    TPriceInfo item2;
    TPriceInfo item3;

    initializePriceInfoToTaxSettings( &item1 );
    initializePriceInfoToTaxSettings( &item2 );
    initializePriceInfoToTaxSettings( &item3 );

    item1.Value = 10;
    item1.TaxInclusive = true;
    item1.MultiDiscountType = mdtCompound;
    applyTaxToPriceInfo( item1, "Sales tax", 12);
    applyTaxToPriceInfo(item1,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item1, "Sales tax", 12, 1 );
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

    calculateValuesForItemSet( orders, discounts, servCharge, servChargeTax, totalTax, finalPrice );

    CheckEquals( 65.0968, discounts,     0.0001, ERR_Discounts );
    CheckEquals( 0.00,     servCharge,    0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00,     servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 13.2995,     totalTax,      0.0001, ERR_Tax );
    CheckEquals( 124.129,     finalPrice,    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_PartialPaymentNoDiscNoServCharge()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    execute();

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;
    partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );

    CheckEquals( 0.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 12.00, getTaxContentForCurrentResult("SalesTax1"), 0.0001, ERR_TaxContent );
    CheckEquals( 12.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 112.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, 56 ); // paying exactly half
    CheckEquals( 5.9999, partialPaymentOutput.PartialTotalTax(), 0.0001, ERR_Tax );
    CheckEquals( 0.00, partialPaymentOutput.PartialDiscount(), 0.0001, ERR_BasePrice );
    CheckEquals( 56.00,partialPaymentOutput.PartialValue(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_PartialPaymentDiscServChargeServChargeTax()
{
    priceInfoObj->Value = 124;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax" , 12 );
    applyTaxToCurrentPriceInfo("SalesTax1", 12);
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
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
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.80, getTaxContentForCurrentResult("SalesTax1"), 0.0001, ERR_TaxContent );
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

void __fastcall TTestTMMBillCalculator_India::Test_MultipleItems_PartialPaymentDiscServChargeServChargeTax()
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
    applyTaxToPriceInfo( item1, "Sales tax", 12);
    applyTaxToPriceInfo(item1,"Sales tax1", 12);
    applyRemovedTaxToPriceInfo(item1, "Sales tax", 12, 1 );
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

    CheckEquals( -673.8065, discounts, 0.0001, ERR_Discounts );
    CheckEquals( 170.129, servCharge, 0.0001, ERR_ServiceCharge );
    CheckEquals( 20.4154, servChargeTax, 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 69.0504, totalTax, 0.0001, ERR_Tax );
    CheckEquals( 1940.4696, finalPrice, 0.0001, ERR_Total );

    // then get the partial totals in the combination
    Currency completeTotal = finalPrice;
    discounts = 0.00;
    servCharge = 0.00;
    servChargeTax = 0.00;
    totalTax = 0.00;
    finalPrice = 0.00;
    calculatePartialValuesForItemSet( orders, completeTotal, 1000, discounts, servCharge, servChargeTax, totalTax, finalPrice );
    // check whether the partial total is matching what we expect
    CheckEquals( -347.2376, discounts, 0.0001, ERR_Discounts );         // todo: rounding issues expected: 353.5236 actual 353.5214
    CheckEquals( 87.6735, servCharge, 0.0001, ERR_ServiceCharge );        // todo: rounding issues expected: 87.6741 actual 87.6735
    CheckEquals( 10.5201, servChargeTax, 0.0001, ERR_ServiceChargeTax );// todo: rounding issues expected: 10.5201 actual 10.5204
    CheckEquals( 35.5832, totalTax, 0.0001, ERR_Tax );                   // todo: rounding issues expected: 35.5832 actual 35.5844
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SimpleMultiTaxes_NoDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 15.00,  getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 115.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.00,  getTaxContentForCurrentResult(tax_name_1), 0.0001,ERR_TaxContent );
    CheckEquals( 15.00,  getTaxContentForCurrentResult(tax_name_2), 0.0001,ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SimpleMultiTaxes_DiscNoSCNoSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(),     0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -11.50, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals(  13.50, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 103.50, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.50,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SimpleMultiTaxes_DiscSCSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -11.50, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 14.40, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 113.40, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.50,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SimpleMultiTaxes_PWDSCSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";

    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyPWDToCurrentPriceInfo( 10 );
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 14.40, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 113.40, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.50,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SimpleMultiTaxes_SCDSCSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->PriceTaxExempt = true;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
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
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SimpleMultiTaxes_MultiDisc_PWD_NoSCNoSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney,       0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyPWDToCurrentPriceInfo(10);
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -39.2402, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.7405, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 74.6774, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.7405,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 4, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyPWDToCurrentPriceInfo(10);
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -39.2402, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.4937, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6494, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 10.3899, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 81.8205, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.7405,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_CompoundMultiTaxes_NoDiscNoSCNoSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 15.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 115.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.00,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 15.00,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_CompoundMultiTaxes_DiscNoSCNoSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -11.5, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 13.5, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 103.5, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.5,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_CompoundMultiTaxes_DiscSCSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );       // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -11.5, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 14.4, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 113.4, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.5,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_CompoundMultiTaxes_PWDSCSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyPWDToCurrentPriceInfo( 10 );
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -10.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.90, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 14.4, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 113.4, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 13.5,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_CompoundMultiTaxes_MultiDisc_PWD_NoSCNoSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyPWDToCurrentPriceInfo( 10 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -39.2402, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 9.7405, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 74.6774, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.7405,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_CompoundMultiTaxes_MultiDisc_PWD_SCSCTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyPWDToCurrentPriceInfo( 10 );
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -39.2402, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.4937, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6494, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 10.3899, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 81.8205, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.7405,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxNoDiscNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  5.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 105.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxRegularDiscNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  2.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 52.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxPWDNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyPWDToCurrentPriceInfo( 50 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 2.50, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  2.50, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 52.50, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxSCDNoServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applySCDToCurrentPriceInfo( 50 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(),  0.0001, ERR_BasePrice );
    CheckEquals( -50.00, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 0.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals(  0.00, getTotalTaxForCurrentResult(),   0.0001, ERR_Tax );
    CheckEquals( 50.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxNoDiscServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyServiceChargeToCurrentPriceInfo( 10, 0 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals( 0.00, getTaxContentForCurrentResult("SalesTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 5.00, getTaxContentForCurrentResult("LocalTax"), 0.0001, ERR_TaxContent );
    CheckEquals( 10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 5.00, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );
    CheckEquals( 115.00, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxRegularDiscServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
    applyDiscountToCurrentPriceInfo( "50perc", dtDiscount, dwPercentage, 50 );
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
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxPWDServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
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
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxSCDServCharge()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->PriceTaxExempt = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
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
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxNoDiscServChargeSCTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
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

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxRegularDiscServChargeSCTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
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
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxPWDServChargeSCTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
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
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_LocalTaxSCDServChargeSCTax()
{
    priceInfoObj->Value = 112;
    priceInfoObj->TaxInclusive = true;
    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax" , 5 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, "SalesTax", 12, 1 );
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
}
//------------------------------------------------------------------------------

void _fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax()
{
    priceInfoObj->Value = 127;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 1 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax", 5.00 );
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyPWDToCurrentPriceInfo(10);
    applyServiceChargeToCurrentPriceInfo( 10.00, 10.00 );
    execute();
    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -39.2402, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.4937, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6494, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 13.6367, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                           // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 85.0673, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );
    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.7405,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 3.2468,
                 getTaxContentForCurrentResult("LocalTax"),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator_India::Test_TaxIncItem_CompoundMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax()
{
    priceInfoObj->Value = 128.8;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->MultiTaxType = mdtCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;
    UnicodeString tax_name_1 = "SalesTax_12";
    UnicodeString tax_name_2 = "SalesTax_15";
    applyTaxToCurrentPriceInfo( tax_name_1 , 12.00, 1 );
    applyTaxToCurrentPriceInfo( tax_name_2 , 15.00, 2 );
    applyRemovedTaxToPriceInfo(*priceInfoObj, tax_name_1, 12, 1 );
    applyLocalTaxToCurrentPriceInfo( "LocalTax", 5.00 );
    applyServiceChargeToCurrentPriceInfo( 10, 10 );
    applyDiscountToCurrentPriceInfo( "10 perc", dtDiscount, dwPercentage, 10, 1, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "15 perc", dtDiscount, dwPercentage, 15, 2, 0, false );        // name, discType, mode, percentage, priority, value, isTaxExclusive
    applyDiscountToCurrentPriceInfo( "5 bucks", dtDiscount, dwMoney, 0, 3, 5, false );              // name, discType, mode, percentage, priority, value, isTaxExclusive

    applyPWDToCurrentPriceInfo( 10 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );                   // base price is calculated at the beginning
    CheckEquals( -39.2402, getTotalDiscountForCurrentResult(), 0.0001, ERR_Discounts );
    CheckEquals( 6.4937, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.6494, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 13.6367, getTotalTaxForCurrentResult(), 0.0001, ERR_Tax );                         // tax calculated after discounts or before discounts depending on the settings
    CheckEquals( 85.0673, getFinalPriceForCurrentResult(), 0.0001, ERR_Total );

    CheckEquals( 0.0,
                 getTaxContentForCurrentResult(tax_name_1),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 9.7405,
                 getTaxContentForCurrentResult(tax_name_2),
                 0.0001,
                 ERR_TaxContent );
    CheckEquals( 3.2468,
                 getTaxContentForCurrentResult("LocalTax"),
                 0.0001,
                 ERR_TaxContent );
}
//------------------------------------------------------------------------------


void TTestTMMBillCalculator_India::initializePriceInfoToTaxSettings( TPriceInfo* priceInfoObj )
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
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_India::execute()
{
    billCalcResponseObj = billCalculator.Execute( priceInfoObj );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_India::execute( TPriceInfo &priceInfo )
{
    billCalcResponseObj = billCalculator.Execute( &priceInfo );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_India::applyTaxToCurrentPriceInfo( string name, Currency percentage, int priority )
{
    applyTaxToPriceInfo( *priceInfoObj, name, percentage, priority );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_India::applyRemovedTaxToPriceInfo( TPriceInfo &priceInfo1, string name, Currency percentage, int priority )
{
    TTaxInfo tax;
    tax.Name = name;
    tax.Percentage = percentage;
    tax.Priority = priority;
    tax.TaxType = ttSale;
    priceInfo1.RemovedTaxInfoList.push_back( tax );
}
//----------------------------------------------------

void TTestTMMBillCalculator_India::applyLocalTaxToCurrentPriceInfo( string name, Currency percentage )
{
    applyLocalTaxToPriceInfo( *priceInfoObj, name, percentage );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_India::applyServiceChargeToCurrentPriceInfo( Currency scPerc, Currency scTaxPerc )
{
    applyServiceChargeToPriceInfo( *priceInfoObj, scPerc, scTaxPerc );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_India::applyDiscountToCurrentPriceInfo( string name, TDiscountType discType, TDiscountWay mode, Currency percentage, int priority, Currency value, bool isTaxExclusive )
{
    applyDiscountToPriceInfo( *priceInfoObj, name, discType, mode, percentage, priority, value, isTaxExclusive );
}
//------------------------------------------------------------------------------
void TTestTMMBillCalculator_India::applySCDToCurrentPriceInfo( Currency percentage )
{
    applySCDToPriceInfo( *priceInfoObj, percentage );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_India::applyPWDToCurrentPriceInfo( Currency percentage )
{
    applyPWDToPriceInfo( *priceInfoObj, percentage );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_India::applyTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage, int priority )
{
    TTaxInfo tax;
    tax.Name = name;
    tax.Percentage = percentage;
    tax.Priority = priority;
    tax.TaxType = ttSale;
    priceInfo.TaxInfoList.push_back( tax );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_India::applyLocalTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage )
{
    TTaxInfo tax;
    tax.Name = name;
    tax.Percentage = percentage;
    tax.Priority = 1;
    tax.TaxType = ttLocal;

    priceInfo.TaxInfoList.push_back( tax );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_India::applyServiceChargeToPriceInfo( TPriceInfo &priceInfo, Currency scPerc, Currency scTaxPerc )
{
    priceInfo.ServiceChargeInfo.Percentage = scPerc;
    priceInfo.ServiceChargeInfo.TaxPercentage = scTaxPerc;
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_India::applyDiscountToPriceInfo( TPriceInfo &priceInfo, string name, TDiscountType discType, TDiscountWay mode, Currency percentage, int priority, Currency value, bool isTaxExclusive )
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

void TTestTMMBillCalculator_India::applySCDToPriceInfo( TPriceInfo &priceInfo, Currency percentage )
{
    TDiscountInfo discInfo;
    discInfo.Name = "SCD";
    discInfo.DiscountType = dtDiscount;
    discInfo.DiscountWay = dwPercentage;
    discInfo.Percentage = percentage;
    priceInfo.PriceTaxExempt = true;
    priceInfo.DiscountInfoList.push_back( discInfo );
}
//------------------------------------------------------------------------------

void TTestTMMBillCalculator_India::applyPWDToPriceInfo( TPriceInfo &priceInfo, Currency percentage )
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

void TTestTMMBillCalculator_India::calculateValuesForItemSet(
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
void TTestTMMBillCalculator_India::calculatePartialValuesForItemSet(
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
Currency TTestTMMBillCalculator_India::getTotalTaxForCurrentResult()
{
    return getTotalTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getDiscountXForCurrentResult( int X )
{
    return getDiscountX( billCalcResponseObj, X );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getTotalDiscountForCurrentResult()
{
    return getTotalDiscount( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getFinalPriceForCurrentResult()
{
    return getFinalPrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getBasePriceForCurrentResult()
{
    return getBasePrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getServiceChargeForCurrentResult()
{
    return getServiceCharge( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getServiceChargeTaxForCurrentResult()
{
    return getServiceChargeTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getTaxContentForCurrentResult( UnicodeString taxName )
{
    return getTaxContent( billCalcResponseObj, taxName );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getTotalTax( TBillCalcResponse* response )
{
    return response->Result.TotalTax;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getDiscountX( TBillCalcResponse* response, int X )
{
    return response->Result.Discount[X].Content;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getTotalDiscount( TBillCalcResponse* response )
{
    return response->Result.TotalDiscount;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getFinalPrice( TBillCalcResponse* response )
{
    return response->Result.FinalPrice;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getBasePrice( TBillCalcResponse* response )
{
    return response->Result.BasePrice;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getServiceCharge( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.Value;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getServiceChargeTax( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.TaxValue;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator_India::getTaxContent( TBillCalcResponse* response, UnicodeString taxName )
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
  TTestSuite* testSuite = new TTestSuite("Testing_MMBillCalculator_India");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(TTestTMMBillCalculator_India));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}

#pragma startup registerTests 5
