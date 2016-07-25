//---------------------------------------------------------------------------


#pragma hdrstop

#include "PriceInclServiceCharge_Generic.h"

//---------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_Generic::PriceInclServiceCharge_Generic(System::String name)
        : TTestMMBillCalculator_Base(name)
{
}
//------------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_Generic::~PriceInclServiceCharge_Generic()
{
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TestCases with only service charge inclusive

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Basic scenarios
void __fastcall PriceInclServiceCharge_Generic::Test_InclSimpleTax_InclSC()
{
    priceInfoObj->Value = 110;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  8.0292, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  21.6788, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 110.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_Generic::Test_InclCompoundTax_InclSC()
{
    priceInfoObj->Value = 200;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;

    UnicodeString tax12Perc = "Tax_12";
    UnicodeString tax15Perc = "Tax_15";

    applyTaxToCurrentPriceInfo( tax12Perc, 12, 1 );
    applyTaxToCurrentPriceInfo( tax15Perc, 15, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  14.4092, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  41.4986, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );

    CheckEquals( 17.2911, getTaxContentForCurrentResult(tax12Perc), 0.0001, ERR_TaxContent );
    CheckEquals( 24.2075, getTaxContentForCurrentResult(tax15Perc), 0.0001, ERR_TaxContent );

    CheckEquals( 200.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_Generic::Test_ExclSimpleTax_InclSC()
{
    priceInfoObj->Value = 110;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals(  10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  27.00, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 137.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_Generic::Test_ExclCompoundTax_InclSC()
{
    priceInfoObj->Value = 110;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;

    applyTaxToCurrentPriceInfo( "SalesTax", 12, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals(  10.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  28.80, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 138.80, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );

}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_Generic::Test_ExemptTax_InclSC()
{
    priceInfoObj->Value = 55.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0.0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  0.00, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 55.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Simple regular discount scenarios
void __fastcall PriceInclServiceCharge_Generic::Test_InclSimpleTax_InclSC_SimpleRegDiscounts()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiDiscountType = mdtSimple;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax", 12.0, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15.0, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  10.9489, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  29.5620, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -50.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 150.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_Generic::Test_InclCompoundTax_InclSC_SimpleRegDiscounts()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->MultiDiscountType = mdtSimple;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.0, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.0, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  10.8069, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 12.9683, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 18.1556, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  31.1239, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -50.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 150.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_Generic::Test_ExclSimpleTax_InclSC_SimpleRegDiscounts()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiDiscountType = mdtSimple;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax", 12.0, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15.0, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  7.5000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  20.2500, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -27.5000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 102.7500, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_Generic::Test_ExclCompoundTax_InclSC_SimpleRegDiscounts()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->MultiDiscountType = mdtSimple;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.0, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.0, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  7.5000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 9.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 12.6000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  21.6000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -27.5000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 104.1000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_Generic::Test_ExemptTax_InclSC_SimpleRegDiscounts()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->MultiDiscountType = mdtSimple;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 0.00, 1 );   // exempt taxes
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 0.00, 2 );   // exempt taxes
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  7.5000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -27.5000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 82.5000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Compound regular discount scenarios
void __fastcall PriceInclServiceCharge_Generic::Test_InclSimpleTax_InclSC_CompoundRegDiscounts()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  11.1679, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 13.4015, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 16.7518, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 30.1533, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -47.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 153.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_Generic::Test_InclCompoundTax_InclSC_CompoundRegDiscounts()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  11.0231, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 13.2277, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 18.5187, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 31.7464, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -47.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 153.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_Generic::Test_ExclSimpleTax_InclSC_CompoundRegDiscounts()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.0, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.0, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  7.6500, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 9.1800, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 11.4750, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  20.6550, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -25.8500, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 104.8050, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_Generic::Test_ExclCompoundTax_InclSC_CompoundRegDiscounts()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.0, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.0, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  7.6500, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 9.1800, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 12.8520, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  22.0320, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -25.8500, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 106.182, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_Generic::Test_ExemptTax_InclSC_CompoundRegDiscounts()
{
    priceInfoObj->Value = 110.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 0.00, 1 );   // exempt tax
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 0.00, 2 );   // exempt tax
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    execute();

    CheckEquals(  7.6500, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -25.8500, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 84.1500, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TestCases with service charge and service charge tax inclusive

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Basic scenarios
void __fastcall PriceInclServiceCharge_Generic::Test_InclSimpleTax_InclSC_InclSCT()
{
    priceInfoObj->Value = 138.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    CheckEquals(  10.000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 1.0000, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  28.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 138.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_Generic::Test_InclCompoundTax_InclSC_InclSCT()
{
    priceInfoObj->Value = 200;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;

    UnicodeString tax12Perc = "Tax_12";
    UnicodeString tax15Perc = "Tax_15";

    applyTaxToCurrentPriceInfo( tax12Perc, 12, 1 );
    applyTaxToCurrentPriceInfo( tax15Perc, 15, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    CheckEquals(  14.3062, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  1.4306, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  42.6323, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );

    CheckEquals( 17.1674, getTaxContentForCurrentResult(tax12Perc), 0.0001, ERR_TaxContent );
    CheckEquals( 24.0343, getTaxContentForCurrentResult(tax15Perc), 0.0001, ERR_TaxContent );

    CheckEquals( 200.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_Generic::Test_ExemptTax_InclSC_InclSCT()
{
    priceInfoObj->Value = 55.50;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0.0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    CheckEquals(  5.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  0.50, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax);
    CheckEquals(  0.50, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 55.50, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Simple regular discount scenarios
void __fastcall PriceInclServiceCharge_Generic::Test_InclSimpleTax_InclSC_InclSCT_SimpleRegDiscounts()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->MultiDiscountType = mdtSimple;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax", 12.0, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15.0, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    CheckEquals(  10.8696, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  1.0870, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  30.4348, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -50.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 150.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_Generic::Test_InclCompoundTax_InclSC_InclSCT_SimpleRegDiscounts()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->MultiDiscountType = mdtSimple;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.0, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.0, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    CheckEquals(  10.7296, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  1.0730, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 12.8755, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 18.0257, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  31.9742, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -50.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 150.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Compound regular discount scenarios
void __fastcall PriceInclServiceCharge_Generic::Test_InclSimpleTax_InclSC_InclSCT_CompoundRegDiscounts()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->MultiTaxType = mttSimple;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_12Perc", 12.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    CheckEquals(  11.0870, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  1.1087, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 13.3044, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 16.6304, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 31.0435, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -47.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 153.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
void __fastcall PriceInclServiceCharge_Generic::Test_InclCompoundTax_InclSC_InclSCT_CompoundRegDiscounts()
{
    priceInfoObj->Value = 200.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->MultiDiscountType = mdtCompound;

    applyDiscountToCurrentPriceInfo( "Disc_10Perc", dtDiscount, dwPercentage, 10.00, 1 );
    applyDiscountToCurrentPriceInfo( "Disc_15Perc", dtDiscount, dwPercentage, 15.00, 2 );
    applyTaxToCurrentPriceInfo( "SalesTax_10Perc", 10.00, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax_15Perc", 15.00, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    execute();

    CheckEquals(  11.1273, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(   1.1127, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  11.1273, getTaxContentForCurrentResult("SalesTax_10Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  18.36, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  30.6, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -47.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 153.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("PriceInclusiveServiceCharge_Generic");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(PriceInclServiceCharge_Generic));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 6
