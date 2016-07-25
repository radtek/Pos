#include <vcl.h>
#pragma hdrstop

#include "TestPartialPayments.h"

using namespace BillCalculator;

void __fastcall TestPartialPayments::SetUp()
{
    menuItemList.clear();
}

void __fastcall TestPartialPayments::TearDown()
{
}

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive()
{
    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       15.30,     // Value = Price
       false,     // Tax Inclusive Price
       false,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       NULL,      // Tax Info List
       NULL,      // Discount Info List
       NULL,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       27.38,     // Value = Price
       false,     // Tax Inclusive Price
       false,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       NULL,      // Tax Info List
       NULL,      // Discount Info List
       NULL,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );

    doPartialPayment( 14.50 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTax()
{
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleDiscount()
{
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscount()
{
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_A()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       13.50,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       27.45,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       48.10,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 50.50 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_B()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       14.95,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       10.83,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       57.62,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 40.35 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_C()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       17.35,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       33.45,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       63.55,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 47.20 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_D()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       24.46,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       70.15,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       33.35,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 74.78 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_E()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       13.35,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       27.45,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       31.60,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 57.15 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_F()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       21.75,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       68.65,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       19.83,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 71.00 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_G()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       17.15,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       53.85,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       41.49,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 81.00 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_H()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       83.87,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       43.99,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       39.70,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 29.99 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_I()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       104.27,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       250.85,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       50.78,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 150.25 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_J()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       46.65,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       83.12,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       15.46,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 64.53 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_K()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       34.47,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       12.99,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       43.58,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 39.90 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_L()
{

    TAX_INFO_LIST taxes;
    DISCOUNT_INFO_LIST discounts;
    TServiceChargeInfo serviceCharge;

    TTaxInfo tax;
    tax.TaxType = ttSale;
    tax.Name = "10 perc";
    tax.Percentage = 10;

    taxes.push_back( tax );

    TDiscountInfo discount;
    discount.Name = "10 perc";
	discount.Percentage = 10;
	discount.DiscountType = dtDiscount;
	discount.DiscountWay = dwPercentage;

    discounts.push_back( discount );

    serviceCharge.Percentage = 10;
    serviceCharge.TaxPercentage = 5;

    TPriceInfo priceInfo_1;
    //................................................................

    setPriceInfo(
       &priceInfo_1,
       11.99,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_1( priceInfo_1 );

    TPriceInfo priceInfo_2;
    //................................................................

    setPriceInfo(
       &priceInfo_2,
       46.49,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_2( priceInfo_2 );

    TPriceInfo priceInfo_3;
    //................................................................

    setPriceInfo(
       &priceInfo_3,
       53.69,     // Value = Price
       false,     // Tax Inclusive Price
       true,     // Tax Exclusive Service Charge
       false,     // Tax Before Discount
       false,     // Service Charge Before Discount
       mdtSimple, // Multi Discount Type
       mttSimple, // MultiTaxType
       &taxes,      // Tax Info List
       &discounts,      // Discount Info List
       &serviceCharge,      // Service Charge Info
       false      // PriceTaxExempt
    );

    //................................................................

    MenuItem menuItem_3( priceInfo_3);

    menuItemList.push_back( menuItem_1 );
    menuItemList.push_back( menuItem_2 );
    menuItemList.push_back( menuItem_3 );

    doPartialPayment( 5.00 );
    validateResults();
}
//------------------------------------------------------------------------------

void __fastcall TestPartialPayments::TestPartialPayments_TaxExclusive_MultiTaxMultiDiscounts()
{
}
//------------------------------------------------------------------------------

void TestPartialPayments::doPartialPayment( Currency partialPayment )
{
    menuItemListManager.SetItems( menuItemList );
    menuItemListManager.DoPartialPayment( partialPayment );
}
//------------------------------------------------------------------------------

void TestPartialPayments::validateResults()
{
    std::map<AnsiString, CURRENCY_VECTOR> resultsMap = menuItemListManager.GetResultsMap();
    validate( resultsMap );
}
//------------------------------------------------------------------------------

void TestPartialPayments::validate( std::map<AnsiString, CURRENCY_VECTOR> &results )
{
    int i = 0;
    int j = results["remainingFP"].size();

    Currency remainingPrice = 0;
    Currency newMenuPrice = 0;
    Currency calculatedPrice = 0;

    AnsiString errorMessage = "Prices does not match. Variance: ";
    AnsiString itemIdentifier = " , Item # : ";

    for( ; i < j ; i++)
    {
        remainingPrice  = results["remainingFP"][i];
        newMenuPrice    = results["newMP"][i];
        calculatedPrice = results["calculatedMP"][i];

        //errorMessage += fabs( newMenuPrice - calculatedPrice );
        //CheckEquals( newMenuPrice, calculatedPrice, 0.0001, errorMessage );

        CheckEquals( remainingPrice, calculatedPrice, 0.0001, errorMessage + fabs( remainingPrice - calculatedPrice ) + itemIdentifier + i );
    }
}

void TestPartialPayments::setPriceInfo(
                               TPriceInfo*   const inPriceInfo,
                               Currency            inValue,
                               bool                inTaxInclusivePrice,
                               bool                inTaxExclusiveServiceCharge,
                               bool                inTaxBeforeDiscount,
                               bool                inServiceChargeBeforeDiscount,
                               TMultiDiscountType  inMultiDiscountType,
                               TMultiTaxType       inMultiTaxType,
                               TAX_INFO_LIST*      inTaxInfoList,
                               DISCOUNT_INFO_LIST* inDiscountInfoList,
                               TServiceChargeInfo* inServiceChargeInfo,
                               bool                inPriceTaxExempt
                            )
{
    inPriceInfo->Value                       = inValue;
    inPriceInfo->TaxInclusive                = inTaxInclusivePrice;
    inPriceInfo->TaxExclusiveServiceCharge   = inTaxExclusiveServiceCharge;
    inPriceInfo->TaxBeforeDiscount           = inTaxBeforeDiscount;
    inPriceInfo->ServiceChargeBeforeDiscount = inServiceChargeBeforeDiscount;

    inPriceInfo->MultiDiscountType           = inMultiDiscountType;
    inPriceInfo->MultiTaxType                = inMultiTaxType;

    inPriceInfo->PriceTaxExempt              = inPriceTaxExempt;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Tax
    TAX_INFO_LIST::iterator taxInfoIT;

     inPriceInfo->TaxInfoList.clear();

    if( inTaxInfoList != NULL )
    {
        for( taxInfoIT  = inTaxInfoList->begin();
             taxInfoIT != inTaxInfoList->end(); taxInfoIT++ )
        {
            TTaxInfo taxInfo;

            taxInfo.TaxType    = taxInfoIT->TaxType;
            taxInfo.Name       = taxInfoIT->Name;
            taxInfo.Percentage = taxInfoIT->Percentage;

            taxInfo.Priority   = taxInfoIT->Priority;

            inPriceInfo->TaxInfoList.push_back( taxInfo );
        }
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Discount
    DISCOUNT_INFO_LIST::const_iterator discountInfoIT;

    inPriceInfo->DiscountInfoList.clear();

    if( inDiscountInfoList != NULL )
    {
        for( discountInfoIT  = inDiscountInfoList->begin();
             discountInfoIT != inDiscountInfoList->end(); discountInfoIT++ )
        {
            TDiscountInfo discountInfo;

            discountInfo.DiscountKey            = discountInfoIT->DiscountKey;
            discountInfo.Name                   = discountInfoIT->Name;
            discountInfo.Description            = discountInfoIT->Description;
            discountInfo.Percentage             = discountInfoIT->Percentage;
            discountInfo.Value                  = discountInfoIT->Value;
            discountInfo.Priority               = discountInfoIT->Priority;
            discountInfo.TaxExclusiveDiscount   = discountInfoIT->TaxExclusiveDiscount;
            discountInfo.DiscountType           = discountInfoIT->DiscountType;
            discountInfo.DiscountWay            = discountInfoIT->DiscountWay;
            discountInfo.RecalcPriceWithTaxAfterDiscount = discountInfoIT->RecalcPriceWithTaxAfterDiscount;
            discountInfo.DiscountGroupList      = discountInfoIT->DiscountGroupList;

            inPriceInfo->DiscountInfoList.push_back( discountInfo );
        }
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Service Charge
    if( inServiceChargeInfo != NULL )
    {
        inPriceInfo->ServiceChargeInfo.Percentage    = inServiceChargeInfo->Percentage;
        inPriceInfo->ServiceChargeInfo.TaxPercentage = inServiceChargeInfo->TaxPercentage;
    }
    else
    {
        inPriceInfo->ServiceChargeInfo.Percentage    = Currency( 0.0 );
        inPriceInfo->ServiceChargeInfo.TaxPercentage = Currency( 0.0 );
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}


static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("TestPartialPayments");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(TestPartialPayments));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 33


/* [For debug purposes only - To be removed soon!!]
GenerateHeaderComment=true
DefaultExtension=.cpp
FileName=C:\Code\Git\MenumateGit-LocalMirror\MicroTests\PartialPayments\TestPartialPayments.cpp
TestFramework=DUnit / C++ Win32
OutputPersonality=CPlusPlusBuilder.Personality
TestProject=C:\Code\Git\MenumateGit-LocalMirror\MicroTests\PartialPayments\MMPartialPaymentsTest.cbproj
UnitUnderTest=C:\Code\Git\MenumateGit-LocalMirror\MenuMate\MenuMate\Source\BillCalculator\MMBillCalculatorAdapter.h
NameOfUnitUnderTest=MMBillCalculatorAdapter.h
TestCaseBaseClass=TTestCase
TestCasePrefix=Test
UnitName=TestPartialPayments
Namespace=TestPartialPayments
TestClasses=
  <0>
    Name=TMMBillCalculatorAdapter
    Methods=
      <0>
        Name=CalculateOrderItemPrices
        Signature=TBillCalcResponse* CalculateOrderItemPrices(TItemMinorComplete* inOrderItem)
TestClass=
Method=
*/
