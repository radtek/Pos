//---------------------------------------------------------------------------

#ifndef TestMMBillCalculator_PhillipinesH
#define TestMMBillCalculator_PhillipinesH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "MMBillCalculator.h"

using namespace BillCalculator;

class TTestTMMBillCalculator_Phillipines : public TTestCase
{
public:

    __fastcall virtual TTestTMMBillCalculator_Phillipines(System::String name);
    __fastcall virtual ~TTestTMMBillCalculator_Phillipines();

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
    void __fastcall Test_TaxIncItem_SCDNoServCharge();
    void __fastcall Test_TaxIncItem_SCDServCharge();
    void __fastcall Test_TaxIncItem_PWDDiscNoServCharge();
    void __fastcall Test_TaxIncItem_PWDDiscServCharge();
    void __fastcall Test_TaxIncItem_ServChargeServChargeTax();
    void __fastcall Test_TaxIncItem_DiscServChargeServChargeTax();
    void __fastcall Test_TaxIncItem_PWDDiscServChargeServChargeTax();
    void __fastcall Test_TaxIncItem_SCDServChargeServChargeTax();

    void __fastcall Test_TaxExcItem_NoDiscNoServCharge();
    void __fastcall Test_TaxExcItem_NoDiscServCharge();
    void __fastcall Test_TaxExcItem_DiscNoServCharge();
    void __fastcall Test_TaxExcItem_SetPriceDiscNoServCharge();
    void __fastcall Test_TaxExcItem_DiscServCharge();
    void __fastcall Test_TaxExcItem_SetPriceDiscServCharge();
    void __fastcall Test_TaxExcItem_SurchargeServCharge();
    void __fastcall Test_TaxExcItem_DollarSurchargeServCharge();
    void __fastcall Test_TaxExcItem_DollarDiscountServCharge();
    void __fastcall Test_TaxExcItem_SCDNoServCharge();
    void __fastcall Test_TaxExcItem_SCDServCharge();
    void __fastcall Test_TaxExcItem_PWDDiscNoServCharge();
    void __fastcall Test_TaxExcItem_PWDDiscServCharge();
    void __fastcall Test_TaxExcItem_ServChargeServChargeTax();
    void __fastcall Test_TaxExcItem_DiscServChargeServChargeTax();
    void __fastcall Test_TaxExcItem_PWDDiscServChargeServChargeTax();
    void __fastcall Test_TaxExcItem_SCDServChargeServChargeTax();

    void __fastcall Test_TaxExemptItem_NoDiscNoServCharge();
    void __fastcall Test_TaxExemptItem_NoDiscServCharge();
    void __fastcall Test_TaxExemptItem_DiscNoServCharge();
    void __fastcall Test_TaxExemptItem_SetPriceDiscNoServCharge();
    void __fastcall Test_TaxExemptItem_DiscServCharge();
    void __fastcall Test_TaxExemptItem_SetPriceDiscServCharge();
    void __fastcall Test_TaxExemptItem_SurchargeServCharge();
    void __fastcall Test_TaxExemptItem_DollarSurchargeServCharge();
    void __fastcall Test_TaxExemptItem_DollarDiscountServCharge();
    void __fastcall Test_TaxExemptItem_SCDNoServCharge();
    void __fastcall Test_TaxExemptItem_SCDServCharge();
    void __fastcall Test_TaxExemptItem_PWDDiscNoServCharge();
    void __fastcall Test_TaxExemptItem_PWDDiscServCharge();
    void __fastcall Test_TaxExemptItem_ServChargeServChargeTax();
    void __fastcall Test_TaxExemptItem_DiscServChargeServChargeTax();
    void __fastcall Test_TaxExemptItem_PWDDiscServChargeServChargeTax();
    void __fastcall Test_TaxExemptItem_SCDServChargeServChargeTax();

    void __fastcall Test_MultipleItems_NoDiscNoServCharge();
    void __fastcall Test_MultipleItems_DiscServChargeServChargeTax();
    void __fastcall Test_MultipleItems_PWDDiscServChargeServChargeTax();
    void __fastcall Test_MultipleItems_SCDServChargeServChargeTax();

    // multiple discounts

    void __fastcall Test_TaxIncItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax();
    void __fastcall Test_TaxIncItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax();
    void __fastcall Test_TaxExcItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax();
    void __fastcall Test_TaxExcItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax();
    void __fastcall Test_TaxExemptItem_MultiDisc_Normal100PercThenSetPrice_ServChargeServChargeTax();
    void __fastcall Test_TaxExemptItem_MultiDisc_SetPriceThenNormal100Perc_ServChargeServChargeTax();
    void __fastcall Test_TaxIncItem_MultipleDiscounts_NormalPWDSetPriceDollarDisc_ServChargeServChargeTax();
    void __fastcall Test_MultipleItems_CompoundMultipleNormalDiscountsServChargeServChargeTax();
    void __fastcall Test_MultipleItems_CompoundMultipleMixedDiscountsServChargeServChargeTax_NormalPWD();
    void __fastcall Test_MultipleItems_CompoundMultipleMixedDiscounts();

    void __fastcall Test_MultipleItems_SimpleMultipleSurchargePercentage();

    void __fastcall Test_MultipleItems_EqualPriorityDiscountsOver100Perc();
    void __fastcall Test_MultipleItems_HigherSurcharges();
    void __fastcall Test_MultipleItems_HigherSurchargesWithDiscounts();

    // partial payments
    void __fastcall Test_TaxIncItem_PartialPaymentNoDiscNoServCharge();
    void __fastcall Test_TaxIncItem_PartialPaymentDiscServChargeServChargeTax();

    void __fastcall Test_TaxExcItem_PartialPaymentNoDiscNoServCharge();
    void __fastcall Test_TaxExcItem_PartialPaymentDiscServChargeServChargeTax();

    void __fastcall Test_TaxExemptItem_PartialPaymentNoDiscNoServCharge();
    void __fastcall Test_TaxExemptItem_PartialPaymentDiscServChargeServChargeTax();

    void __fastcall Test_MultipleItems_PartialPaymentDiscServChargeServChargeTax();

    // multiple taxes
    void __fastcall Test_TaxIncItem_SimpleMultiTaxes_NoDiscNoSCNoSCTax();
    void __fastcall Test_TaxIncItem_SimpleMultiTaxes_DiscNoSCNoSCTax();
    void __fastcall Test_TaxIncItem_SimpleMultiTaxes_DiscSCSCTax();
    void __fastcall Test_TaxIncItem_SimpleMultiTaxes_PWDSCSCTax();
    void __fastcall Test_TaxIncItem_SimpleMultiTaxes_SCDSCSCTax();
    void __fastcall Test_TaxIncItem_SimpleMultiTaxes_MultiDisc_PWD_NoSCNoSCTax();
    void __fastcall Test_TaxIncItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax();

    void __fastcall Test_TaxIncItem_CompoundMultiTaxes_NoDiscNoSCNoSCTax();
    void __fastcall Test_TaxIncItem_CompoundMultiTaxes_DiscNoSCNoSCTax();
    void __fastcall Test_TaxIncItem_CompoundMultiTaxes_DiscSCSCTax();
    void __fastcall Test_TaxIncItem_CompoundMultiTaxes_PWDSCSCTax();
    void __fastcall Test_TaxIncItem_CompoundMultiTaxes_MultiDisc_PWD_NoSCNoSCTax();
    void __fastcall Test_TaxIncItem_CompoundMultiTaxes_MultiDisc_PWD_SCSCTax();

    void __fastcall Test_TaxExcItem_SimpleMultiTaxes_NoDiscNoSCNoSCTax();
    void __fastcall Test_TaxExcItem_SimpleMultiTaxes_DiscNoSCNoSCTax();
    void __fastcall Test_TaxExcItem_SimpleMultiTaxes_DiscSCSCTax();
    void __fastcall Test_TaxExcItem_SimpleMultiTaxes_PWDSCSCTax();
    void __fastcall Test_TaxExcItem_SimpleMultiTaxes_MultiDiscNoSCNoSCTax();
    void __fastcall Test_TaxExcItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax();

    void __fastcall Test_TaxExcItem_CompoundMultiTaxes_NoDiscNoSCNoSCTax();
    void __fastcall Test_TaxExcItem_CompoundMultiTaxes_DiscNoSCNoSCTax();
    void __fastcall Test_TaxExcItem_CompoundMultiTaxes_DiscSCSCTax();
    void __fastcall Test_TaxExcItem_CompoundMultiTaxes_PWDSCSCTax();
    void __fastcall Test_TaxExcItem_CompoundMultiTaxes_MultiDiscNoSCNoSCTax();
    void __fastcall Test_TaxExcItem_CompoundMultiTaxes_MultiDiscSCSCTax();

    void __fastcall Test_TaxExemptItem_SimpleMultiTaxes_NoDiscNoSCNoSCTax();
    void __fastcall Test_TaxExemptItem_SimpleMultiTaxes_DiscNoSCNoSCTax();
    void __fastcall Test_TaxExemptItem_SimpleMultiTaxes_DiscSCSCTax();
//    void __fastcall Test_TaxExemptItem_SimpleMultiTaxes_PWDSCSCTax();
    void __fastcall Test_TaxExemptItem_SimpleMultiTaxes_MultiDiscNoSCNoSCTax();
    void __fastcall Test_TaxExemptItem_SimpleMultiTaxes_MultiDiscSCSCTax();

    void __fastcall Test_TaxExemptItem_CompoundMultiTaxes_NoDiscNoSCNoSCTax();
    void __fastcall Test_TaxExemptItem_CompoundMultiTaxes_DiscNoSCNoSCTax();
    void __fastcall Test_TaxExemptItem_CompoundMultiTaxes_DiscSCSCTax();
//    void __fastcall Test_TaxExemptItem_CompoundMultiTaxes_PWDSCSCTax();
    void __fastcall Test_TaxExemptItem_CompoundMultiTaxes_MultiDiscNoSCNoSCTax();
    void __fastcall Test_TaxExemptItem_CompoundMultiTaxes_MultiDiscSCSCTax();

    // local taxes
    void __fastcall Test_TaxIncItem_LocalTaxNoDiscNoServCharge();
    void __fastcall Test_TaxExcItem_LocalTaxNoDiscNoServCharge();
    void __fastcall Test_TaxExemptItem_LocalTaxNoDiscNoServCharge();

    void __fastcall Test_TaxIncItem_LocalTaxRegularDiscNoServCharge();
    void __fastcall Test_TaxExcItem_LocalTaxRegularDiscNoServCharge();
    void __fastcall Test_TaxExemptItem_LocalTaxRegularDiscNoServCharge();

    void __fastcall Test_TaxIncItem_LocalTaxPWDNoServCharge();
    void __fastcall Test_TaxExcItem_LocalTaxPWDNoServCharge();
    void __fastcall Test_TaxExemptItem_LocalTaxPWDNoServCharge();

    void __fastcall Test_TaxIncItem_LocalTaxSCDNoServCharge();
    void __fastcall Test_TaxExcItem_LocalTaxSCDNoServCharge();
    void __fastcall Test_TaxExemptItem_LocalTaxSCDNoServCharge();

    void __fastcall Test_TaxIncItem_LocalTaxNoDiscServCharge();
    void __fastcall Test_TaxExcItem_LocalTaxNoDiscServCharge();
    void __fastcall Test_TaxExemptItem_LocalTaxNoDiscServCharge();

    void __fastcall Test_TaxIncItem_LocalTaxRegularDiscServCharge();
    void __fastcall Test_TaxExcItem_LocalTaxRegularDiscServCharge();
    void __fastcall Test_TaxExemptItem_LocalTaxRegularDiscServCharge();

    void __fastcall Test_TaxIncItem_LocalTaxPWDServCharge();
    void __fastcall Test_TaxExcItem_LocalTaxPWDServCharge();
    void __fastcall Test_TaxExemptItem_LocalTaxPWDServCharge();

    void __fastcall Test_TaxIncItem_LocalTaxSCDServCharge();
    void __fastcall Test_TaxExcItem_LocalTaxSCDServCharge();
    void __fastcall Test_TaxExemptItem_LocalTaxSCDServCharge();

    void __fastcall Test_TaxIncItem_LocalTaxNoDiscServChargeSCTax();
    void __fastcall Test_TaxExcItem_LocalTaxNoDiscServChargeSCTax();
    void __fastcall Test_TaxExemptItem_LocalTaxNoDiscServChargeSCTax();

    void __fastcall Test_TaxIncItem_LocalTaxRegularDiscServChargeSCTax();
    void __fastcall Test_TaxExcItem_LocalTaxRegularDiscServChargeSCTax();
    void __fastcall Test_TaxExemptItem_LocalTaxRegularDiscServChargeSCTax();

    void __fastcall Test_TaxIncItem_LocalTaxPWDServChargeSCTax();
    void __fastcall Test_TaxExcItem_LocalTaxPWDServChargeSCTax();
    void __fastcall Test_TaxExemptItem_LocalTaxPWDServChargeSCTax();

    void __fastcall Test_TaxIncItem_LocalTaxSCDServChargeSCTax();
    void __fastcall Test_TaxExcItem_LocalTaxSCDServChargeSCTax();
    void __fastcall Test_TaxExemptItem_LocalTaxSCDServChargeSCTax();

    void _fastcall Test_TaxIncItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax();
    void __fastcall Test_TaxExcItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax();
    void __fastcall Test_TaxExemptItem_SimpleMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax();

    void __fastcall Test_TaxIncItem_CompoundMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax();
    void __fastcall Test_TaxExcItem_CompoundMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax();
    void __fastcall Test_TaxExemptItem_CompoundMultiTaxes_MultiDisc_PWD_SCSCTax_LocalTax();

private:

    TMMBillCalculator billCalculator;
    TPriceInfo* priceInfoObj;
    TBillCalcResponse* billCalcResponseObj;

    void initializePriceInfoToTaxSettings( TPriceInfo* priceInfoObj );  // initialize tax settings for billCalculator, country specific

    void execute();
    void execute( TPriceInfo &priceInfo ); //overload

    void applyTaxToCurrentPriceInfo( string name, Currency percentage, int priority = 0);
    void applyLocalTaxToCurrentPriceInfo( string name, Currency percentage );
    void applyServiceChargeToCurrentPriceInfo( Currency scPerc, Currency scTaxPerc );
    void applyDiscountToCurrentPriceInfo( string name, TDiscountType discType, TDiscountWay mode, Currency percentage = 0.0, int priority = 0, Currency value = 0.00, bool isTaxExclusive = false );
    void applySCDToCurrentPriceInfo( Currency percentage );
    void applyPWDToCurrentPriceInfo( Currency percentage );

    void applyTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage, int priority = 0 );
    void applyLocalTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage );
    void applyServiceChargeToPriceInfo( TPriceInfo &priceInfo, Currency scPerc, Currency scTaxPerc );
    void applyDiscountToPriceInfo( TPriceInfo &priceInfo, string name, TDiscountType discType, TDiscountWay mode, Currency percentage = 0.0, int priority = 0, Currency value = 0.00, bool isTaxExclusive = false );
    void applySCDToPriceInfo( TPriceInfo &priceInfo, Currency percentage );
    void applyPWDToPriceInfo( TPriceInfo &priceInfo, Currency percentage );

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
                        Currency &finalPrice,
                        Currency &discountWithOutTax,
                        Currency &taxOnDiscount);

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
    Currency getDiscountWithoutTax( TBillCalcResponse* response );
    Currency getTaxOnDiscount( TBillCalcResponse* response );
    Currency getDiscountWithoutTaxForCurrent( );
    Currency getTaxOnDiscountForCurrent(  );
};
#endif
