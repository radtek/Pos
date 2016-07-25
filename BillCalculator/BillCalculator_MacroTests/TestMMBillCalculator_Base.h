//---------------------------------------------------------------------------

#ifndef TestMMBillCalculator_BaseH
#define TestMMBillCalculator_BaseH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "MMBillCalculator.h"

using namespace BillCalculator;

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

class TTestMMBillCalculator_Base : public TTestCase
{
    public:

        __fastcall virtual TTestMMBillCalculator_Base(System::String name);
        __fastcall virtual ~TTestMMBillCalculator_Base();

        virtual void __fastcall SetUp();
        virtual void __fastcall TearDown();

        // initialize tax settings for billCalculator, country specific
        virtual void initializePriceInfoToTaxSettings( TPriceInfo* priceInfoObj );

    protected:

        TMMBillCalculator billCalculator;
        TPriceInfo* priceInfoObj;
        TBillCalcResponse* billCalcResponseObj;

        void execute();
        void execute( TPriceInfo &priceInfo ); //overload

        void applyTaxToCurrentPriceInfo( string name, Currency percentage, int priority = 0);
        void applyLocalTaxToCurrentPriceInfo( string name, Currency percentage );
        void applyServiceChargeToCurrentPriceInfo( Currency scPerc, Currency scTaxPerc );
        void applyDiscountToCurrentPriceInfo( string name, TDiscountType discType, TDiscountWay mode, Currency percentage = 0.0, int priority = 0, Currency value = 0.00, bool isTaxExclusive = false );
        void applySCDToCurrentPriceInfo( Currency percentage );
        void applyPWDToCurrentPriceInfo( Currency percentage );

        void applyTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage, int priority = 0 );
        void applyRemovedTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage, int priority );
        void applyLocalTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage );
        void applyServiceChargeToPriceInfo( TPriceInfo &priceInfo, Currency scPerc, Currency scTaxPerc );
        void applyRemovedServiceChargeToPriceInfo( TPriceInfo &priceInfo, Currency scPerc, Currency scTaxPerc );
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
