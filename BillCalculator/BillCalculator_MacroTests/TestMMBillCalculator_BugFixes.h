//---------------------------------------------------------------------------

#ifndef TestMMBillCalculator_BugFixesH
#define TestMMBillCalculator_BugFixesH
//---------------------------------------------------------------------------

/*
 * The purpose of this test fixture is to provide test methods for various bug fixes.
 * As some bug fixes uses tax settings which are different from the country specific settings,
 * those methods are needed to be put in a different place, hence in this class
 *
 * Abbreviations and meaning for tax settings
 *
 * TBD - Tax Before Discount
 * NTBD - Not Tax Before Discount
 * SCAD - Service Charge After Discount
 * SCBD - Service charge before discount
 * SCTEP - Service charge tax exclusive price
 * SCTIP - Service charge tax inclusive price
 * DISC - Discounts present
 * ATTRS - Misc attributes ( tax excl, scd, pwd etc )
 *
 */

#include <TestFramework.hpp>
#include "MMBillCalculator.h"

using namespace BillCalculator;

class TTestTMMBillCalculator_BugFixes : public TTestCase
{
public:

    __fastcall virtual TTestTMMBillCalculator_BugFixes(System::String name);
    __fastcall virtual ~TTestTMMBillCalculator_BugFixes();

    virtual void __fastcall SetUp();
    virtual void __fastcall TearDown();

__published:
    void __fastcall Test_TaxIncItem_TBD_SCAD_SCTEP_DISC();
    void __fastcall Test_TaxExcItem_TBD_SCAD_SCTEP_DISC();

private:
    TMMBillCalculator billCalculator;
    TBillCalcResponse* billCalcResponseObj;

    void resetPriceInfo( TPriceInfo* priceInfoObj );

    void execute( TPriceInfo &priceInfo );

    void applyTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage, int priority = 0 );
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
