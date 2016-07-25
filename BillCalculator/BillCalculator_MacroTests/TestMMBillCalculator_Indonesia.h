//---------------------------------------------------------------------------

#ifndef TestMMBillCalculator_IndonesiaH
#define TestMMBillCalculator_IndonesiaH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "MMBillCalculator.h"

using namespace BillCalculator;

/*
 * as the TaxExclusive discount calculations has been changed for phillipines,
 * we cannot test those for indonesia yet.
 * hence all the test cases which uses tax exclusive discounts has been commented out
 */

class TTestTMMBillCalculator_Indonesia : public TTestCase
{
public:
    __fastcall virtual TTestTMMBillCalculator_Indonesia(System::String name);
    __fastcall virtual ~TTestTMMBillCalculator_Indonesia();

    virtual void __fastcall SetUp();
    virtual void __fastcall TearDown();

__published:

    void __fastcall Test_Execute_Empty();

    void __fastcall Test_TaxIncItem_NoDiscNoServCharge();
    void __fastcall Test_TaxIncItem_NoDiscServCharge();
    void __fastcall Test_TaxIncItem_DiscNoServCharge();
    void __fastcall Test_TaxIncItem_HigherDiscNoServCharge();
    void __fastcall Test_TaxIncItem_SetPriceDiscNoServCharge();
    void __fastcall Test_TaxIncItem_SurchargeNoServCharge();
    void __fastcall Test_TaxIncItem_DiscServCharge();
    void __fastcall Test_TaxIncItem_SetPriceDiscServCharge();
    void __fastcall Test_TaxIncItem_SurchargeServCharge();
    void __fastcall Test_TaxIncItem_DollarSurchargeServCharge();
    void __fastcall Test_TaxIncItem_DollarDiscountServCharge();
//    void __fastcall Test_TaxIncItem_TaxExclDiscNoServCharge();
//    void __fastcall Test_TaxIncItem_TaxExclDiscServCharge();
    void __fastcall Test_TaxIncItem_ServChargeServChargeTax();
    void __fastcall Test_TaxIncItem_DiscServChargeServChargeTax();
//    void __fastcall Test_TaxIncItem_TaxExclDiscServChargeServChargeTax();

    void __fastcall Test_TaxExcItem_NoDiscNoServCharge();
    void __fastcall Test_TaxExcItem_NoDiscServCharge();
    void __fastcall Test_TaxExcItem_DiscNoServCharge();
    void __fastcall Test_TaxExcItem_SetPriceDiscNoServCharge();
    void __fastcall Test_TaxExcItem_DiscServCharge();
    void __fastcall Test_TaxExcItem_SetPriceDiscServCharge();
    void __fastcall Test_TaxExcItem_SurchargeServCharge();
    void __fastcall Test_TaxExcItem_DollarSurchargeServCharge();
    void __fastcall Test_TaxExcItem_DollarDiscountServCharge();
//    void __fastcall Test_TaxExcItem_TaxExclDiscNoServCharge();
//    void __fastcall Test_TaxExcItem_TaxExclDiscServCharge();
    void __fastcall Test_TaxExcItem_ServChargeServChargeTax();
    void __fastcall Test_TaxExcItem_DiscServChargeServChargeTax();
//    void __fastcall Test_TaxExcItem_TaxExclDiscServChargeServChargeTax();

    void __fastcall Test_TaxExemptItem_NoDiscNoServCharge();
    void __fastcall Test_TaxExemptItem_NoDiscServCharge();
    void __fastcall Test_TaxExemptItem_DiscNoServCharge();
    void __fastcall Test_TaxExemptItem_SetPriceDiscNoServCharge();
    void __fastcall Test_TaxExemptItem_DiscServCharge();
    void __fastcall Test_TaxExemptItem_SetPriceDiscServCharge();
    void __fastcall Test_TaxExemptItem_SurchargeServCharge();
    void __fastcall Test_TaxExemptItem_DollarSurchargeServCharge();
    void __fastcall Test_TaxExemptItem_DollarDiscountServCharge();
//    void __fastcall Test_TaxExemptItem_TaxExclDiscNoServCharge();
//    void __fastcall Test_TaxExemptItem_TaxExclDiscServCharge();
    void __fastcall Test_TaxExemptItem_ServChargeServChargeTax();
    void __fastcall Test_TaxExemptItem_DiscServChargeServChargeTax();
//    void __fastcall Test_TaxExemptItem_TaxExclDiscServChargeServChargeTax();

    void __fastcall Test_MultipleItems_NoDiscNoServCharge();
    void __fastcall Test_MultipleItems_DiscServChargeServChargeTax();
//    void __fastcall Test_MultipleItems_TaxExclDiscServChargeServChargeTax();

    // multiple discounts

    void __fastcall Test_TaxIncItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax();
    void __fastcall Test_TaxIncItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax();
    void __fastcall Test_TaxExcItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax();
    void __fastcall Test_TaxExcItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax();
    void __fastcall Test_TaxExemptItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax();
    void __fastcall Test_TaxExemptItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax();
//    void __fastcall Test_TaxIncItem_MultipleDiscounts_NormalTaxExclSetPriceDollarDisc_ServChargeServChargeTax();
    void __fastcall Test_MultipleItems_CompoundMultipleNormalDiscountsServChargeServChargeTax();
//    void __fastcall Test_MultipleItems_CompoundMultipleMixedDiscountsServChargeServChargeTax_NormalTaxExcl();
    void __fastcall Test_MultipleItems_CompoundMultipleMixedDiscounts();

    void __fastcall Test_MultipleItems_SimpleMultipleSurchargePercentage();

    // partial payments
    void __fastcall Test_TaxIncItem_PartialPaymentNoDiscNoServCharge();
    void __fastcall Test_TaxIncItem_PartialPaymentDiscServChargeServChargeTax();

    void __fastcall Test_TaxExcItem_PartialPaymentNoDiscNoServCharge();
    void __fastcall Test_TaxExcItem_PartialPaymentDiscServChargeServChargeTax();

    void __fastcall Test_TaxExemptItem_PartialPaymentNoDiscNoServCharge();
    void __fastcall Test_TaxExemptItem_PartialPaymentDiscServChargeServChargeTax();

//    void __fastcall Test_MultipleItems_PartialPaymentDiscServChargeServChargeTax();

private:

    TMMBillCalculator billCalculator;
    TPriceInfo* priceInfoObj;
    TBillCalcResponse* billCalcResponseObj;

    void initializePriceInfoToTaxSettings( TPriceInfo* priceInfoObj );  // initialize tax settings for billCalculator, country specific

    void execute();
    void execute( TPriceInfo &priceInfo ); //overload

    void applyTaxToCurrentPriceInfo( string name, Currency percentage, int priority = 0);
    void applyServiceChargeToCurrentPriceInfo( Currency scPerc, Currency scTaxPerc );
    void applyDiscountToCurrentPriceInfo( string name, TDiscountType discType, TDiscountWay mode, Currency percentage = 0.0, int priority = 0, Currency value = 0.00, bool isTaxExclusive = false );
    void applySCDToCurrentPriceInfo( Currency percentage );

    void applyTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage, int priority = 0 );
    void applyServiceChargeToPriceInfo( TPriceInfo &priceInfo, Currency scPerc, Currency scTaxPerc );
    void applyDiscountToPriceInfo( TPriceInfo &priceInfo, string name, TDiscountType discType, TDiscountWay mode, Currency percentage = 0.0, int priority = 0, Currency value = 0.00, bool isTaxExclusive = false );
    void applySCDToPriceInfo( TPriceInfo &priceInfo, Currency percentage );

    void calculatePartialValuesForItemSet(
                        std::vector<TPriceInfo> &items,
                        Currency itemsTotal,
                        Currency partialPayment,
                        Currency &discounts,
                        Currency &servCharge,
                        Currency &servChargeTax,
                        Currency &totalTax,
                        Currency &finalPrice);

    void calculateValuesForItemSet(
                        std::vector<TPriceInfo> &items,
                        Currency &discounts,
                        Currency &servCharge,
                        Currency &servChargeTax,
                        Currency &totalTax,
                        Currency &finalPrice);

    Currency getTotalTaxForCurrentResult();
    Currency getDiscountXForCurrentResult( int X );
    Currency getTotalDiscountForCurrentResult();
    Currency getFinalPriceForCurrentResult();
    Currency getBasePriceForCurrentResult();
    Currency getServiceChargeForCurrentResult();
    Currency getServiceChargeTaxForCurrentResult();
    Currency getTaxContentForCurrentResult( UnicodeString taxName );

    Currency getTotalTax( TBillCalcResponse* response );
    Currency getDiscountX( TBillCalcResponse* response, int X );
    Currency getTotalDiscount( TBillCalcResponse* response );
    Currency getFinalPrice( TBillCalcResponse* response );
    Currency getBasePrice( TBillCalcResponse* response );
    Currency getServiceCharge( TBillCalcResponse* response );
    Currency getServiceChargeTax( TBillCalcResponse* response );
    Currency getTaxContent( TBillCalcResponse* response, UnicodeString taxName );
};
#endif