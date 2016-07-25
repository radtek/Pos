//---------------------------------------------------------------------------


#pragma hdrstop

#include "PriceInclServiceCharge_ServiceChargeRemoval.h"

//---------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_ServiceChargeRemoval::PriceInclServiceCharge_ServiceChargeRemoval(System::String name)
        : TTestMMBillCalculator_Base(name)
{
}
//------------------------------------------------------------------------------

__fastcall PriceInclServiceCharge_ServiceChargeRemoval::~PriceInclServiceCharge_ServiceChargeRemoval()
{
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TestCases with only service charge inclusive

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Basic scenarios
void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclSimpleTax_InclSC()
{
    priceInfoObj->Value = 110;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  21.6788, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 101.9708, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclCompoundTax_InclSC()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  41.4986, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );

    CheckEquals( 17.2911, getTaxContentForCurrentResult(tax12Perc), 0.0001, ERR_TaxContent );
    CheckEquals( 24.2075, getTaxContentForCurrentResult(tax15Perc), 0.0001, ERR_TaxContent );

    CheckEquals( 185.5908, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_ExclSimpleTax_InclSC()
{
    priceInfoObj->Value = 110;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals(  0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  27.00, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 127.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_ExclCompoundTax_InclSC()
{
    priceInfoObj->Value = 110;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->MultiTaxType = mttCompound;

    applyTaxToCurrentPriceInfo( "SalesTax", 12, 1 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15, 2 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals( 100.00, getBasePriceForCurrentResult(), 0.0001, ERR_BasePrice );
    CheckEquals(  00.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  28.80, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 128.80, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );

}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_ExemptTax_InclSC()
{
    priceInfoObj->Value = 55.00;
    priceInfoObj->TaxInclusive = false;
    priceInfoObj->ServiceChargeInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0.0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 0 );

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  0.00, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 50.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Simple regular discount scenarios
void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclSimpleTax_InclSC_SimpleRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  29.5620, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -46.3504, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 139.0511, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclCompoundTax_InclSC_SimpleRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 12.9683, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 18.1556, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  31.1239, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -46.3977, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 139.1931, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_ExclSimpleTax_InclSC_SimpleRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  20.2500, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -25.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 95.25, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_ExclCompoundTax_InclSC_SimpleRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 9.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 12.6000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  21.6000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -25.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 96.6000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_ExemptTax_InclSC_SimpleRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -25.0000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 75.0000, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Compound regular discount scenarios
void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclSimpleTax_InclSC_CompoundRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 13.4015, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 16.7518, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 30.1533, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -43.5694, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 141.8321, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclCompoundTax_InclSC_CompoundRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 13.2277, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 18.5187, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 31.7464, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -43.6139, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 141.9769, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_ExclSimpleTax_InclSC_CompoundRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 9.1800, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 11.4750, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  20.6550, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -23.5000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 97.155, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_ExclCompoundTax_InclSC_CompoundRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 9.1800, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 12.8520, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  22.0320, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -23.5000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 98.5320, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_ExemptTax_InclSC_CompoundRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 0 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 0.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -23.5000, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 76.5, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TestCases with service charge and service charge tax inclusive

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Basic scenarios
void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclSimpleTax_InclSC_InclSCT()
{
    priceInfoObj->Value = 138.00;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 12 );
    applyTaxToCurrentPriceInfo( "SalesTax", 15 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 10 );

    execute();

    CheckEquals(  00.000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 0.0000, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  27.0000, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 127.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclCompoundTax_InclSC_InclSCT()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 10 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  0.0000, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  41.2017, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );

    CheckEquals( 17.1674, getTaxContentForCurrentResult(tax12Perc), 0.0001, ERR_TaxContent );
    CheckEquals( 24.0343, getTaxContentForCurrentResult(tax15Perc), 0.0001, ERR_TaxContent );

    CheckEquals( 184.2632, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_ExemptTax_InclSC_InclSCT()
{
    priceInfoObj->Value = 55.50;
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->ServiceChargeInclusive = true;
    priceInfoObj->ServiceChargeTaxInclusive = true;

    applyTaxToCurrentPriceInfo( "SalesTax", 0.0 );
    applyServiceChargeToCurrentPriceInfo ( 10, 10 );

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 10 );

    execute();

    CheckEquals(  0.00, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  0.00, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax);
    CheckEquals(  0.00, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( 50.00, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Simple regular discount scenarios
void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclSimpleTax_InclSC_InclSCT_SimpleRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 10 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  0.0000, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals(  29.3478, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -46.0145, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 138.0434, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}

void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclCompoundTax_InclSC_InclSCT_SimpleRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 10 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  0.0000, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceChargeTax );
    CheckEquals( 12.8755, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 18.0257, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  30.9012, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals(  -46.0658, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 138.1974, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Compound regular discount scenarios
void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclSimpleTax_InclSC_InclSCT_CompoundRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 10 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  0.0000, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals( 13.3044, getTaxContentForCurrentResult("SalesTax_12Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 16.6304, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals( 29.9348, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -43.2536, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 140.8043, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------
void __fastcall PriceInclServiceCharge_ServiceChargeRemoval::Test_InclCompoundTax_InclSC_InclSCT_CompoundRegDiscounts()
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

    applyRemovedServiceChargeToPriceInfo( *priceInfoObj, 10, 10 );

    execute();

    CheckEquals(  0.0000, getServiceChargeForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  0.0000, getServiceChargeTaxForCurrentResult(), 0.0001, ERR_ServiceCharge );
    CheckEquals(  11.1273, getTaxContentForCurrentResult("SalesTax_10Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  18.36, getTaxContentForCurrentResult("SalesTax_15Perc"),0.0001, ERR_TaxContent );
    CheckEquals(  29.4873, getTotalTaxForCurrentResult(),      0.0001, ERR_Tax );
    CheckEquals( -43.2400, getTotalDiscountForCurrentResult(),      0.0001, ERR_Discounts );
    CheckEquals( 140.76, getFinalPriceForCurrentResult(),    0.0001, ERR_Total );
}
//------------------------------------------------------------------------------

static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("PriceInclServiceCharge_ServiceChargeRemoval");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(PriceInclServiceCharge_ServiceChargeRemoval));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 12
