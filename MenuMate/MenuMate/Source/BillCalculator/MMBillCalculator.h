 //---------------------------------------------------------------------------

#ifndef MMBillCalculatorH
#define MMBillCalculatorH

//---------------------------------------------------------------------------

#include "IBillCalculator.h"

#include <vector>

//---------------------------------------------------------------------------

namespace BillCalculator
{

const Currency Curr0   = Currency(0.00);
const Currency Curr1   = Currency(1.00);
const Currency Curr100 = Currency(100.00);

struct TServiceChargeSettings
{
    bool ServiceChargeInclusive;
    bool ServiceChargeTaxInclusive;

    Currency ServiceChargePercentage;
    Currency ServiceChargeTaxPercentage;
};

class TMMBillCalculator : public IBillCalculator
{
public:
             TMMBillCalculator();
    virtual ~TMMBillCalculator();

    virtual TBillCalcResponse* Execute( const TPriceInfo* const inPriceInfo );

    virtual TCalcContentOutput CalculatePartialPaymentContent( const TCalcContentInput* const inInputInfo, Currency partialPayment );

    virtual TPriceInfo* CreatePriceInfo();
    virtual TPriceInfo* CreatePriceInfoWithPriceInfo( const TPriceInfo* const inPriceInfo );

protected:

    void ExecuteWithoutTaxRemoval( const TPriceInfo* const inPriceInfo, TAX_INFO_LIST& endTaxList, DISCOUNT_INFO_LIST& endDiscountList, TBillCalcResponse* response );
    void ExecuteWithTaxRemoval( const TPriceInfo* const inPriceInfo, TAX_INFO_LIST& endTaxList, DISCOUNT_INFO_LIST& endDiscountList, TBillCalcResponse* response );

	void resetResponse( TBillCalcResponse* const outResponse );
	void resetResult(   TBillCalcResult*   const outResult );

	// Gets price without the tax portion
	Currency getTaxExemptPrice( const TPriceInfo* const inPriceInfo );

	// Gets price without tax portion, using the settings from price info
	Currency getTaxExemptPrice(
					const Currency inSalePrice,
					const TPriceInfo* inPriceInfo );

    // Gets base price from price info
	Currency getBasePrice( const TPriceInfo* const inPriceInfo );

    // Gets base price for a value, using settings from given price info
    Currency getBasePrice(
                const Currency inSalePrice,
                const TPriceInfo* inPriceInfo );

    // Gets base price for a value, using settings
    Currency getBasePrice(
                Currency      inSalePrice,
                TAX_INFO_LIST inTaxInfoList,
                TMultiTaxType inMultiTaxType,
                bool          inTaxInclusive,
                bool          inServiceChargeIncl,
                bool          inServiceChargeTxIncl,
				TServiceChargeInfo ServiceChargeInfo );
    //.............................................

    Currency getCombinedTaxRate( TAX_INFO_LIST &inTaxInfoList, TMultiTaxType inMultiTaxType );
    Currency getCombinedTaxRateSimple(   TAX_INFO_LIST &inTaxInfoList );
    Currency getCombinedTaxRateCompound( TAX_INFO_LIST &inTaxInfoList );

    Currency getTaxInclusiveBasePrice(
                Currency      inSalePrice,
                TAX_INFO_LIST inTaxInfoList,
                TMultiTaxType inMultiTaxType );

    TPriceInfo* createPriceInfoWithPriceInfo( const TPriceInfo* const inPriceInfo );
    void reviewResult( Currency inSalePrice, TBillCalcResult* inResult );

    //Remove Taxes
    void removeTaxes(TPriceInfo* const inPriceInfo);

    // Remove service charges
    void removeServiceCharges( TPriceInfo* const inPriceInfo );

    //.........................................................................
    // DISCOUNT
    void calcDiscount(
                   TPriceInfo*      const inPriceInfo,
                   TBillCalcResult* const outResult,
                   bool  inIgnoreSpecialDiscounts = true );
    //.........................................................................
    void seperateEndDiscountsFromRegularDiscounts( TPriceInfo* inPriceInfo, DISCOUNT_INFO_LIST& endDiscountList );
    //.........................................................................
    void populateEndDiscountList(
             DISCOUNT_INFO_LIST& inDiscountInfoList,
             DISCOUNT_INFO_LIST& outEndDiscountList );
    //.........................................................................
    void groupDiscountsByPriority(
                DISCOUNT_INFO_LIST  inDiscountInfoList,
                TMultiDiscountType  inMultiDiscountType,
                DISCOUNT_GROUP_MAP& outGroupMap );
   //.........................................................................
    // Simple Multiple Discount
    void calcSimpleDiscountWithMap(
                    DISCOUNT_GROUP_MAP     inGroupMap,
              const TPriceInfo*      const inPriceInfo,
                    TBillCalcResult* const outResult );
   //.........................................................................
    // Compound Multiple Discount
    void calcCompoundDiscountWithMap(
                   DISCOUNT_GROUP_MAP     inGroupMap,
             const TPriceInfo*      const inPriceInfo,
                   TBillCalcResult* const outResult );

    //.........................................................................
    void calcDiscountWithList(
			const Currency inBasePrice,
			const Currency inTaxExemptPrice,
			const Currency inSalePrice,
            const DISCOUNT_INFO_LIST inDiscInfoList,
			const TPriceInfo* const inPriceInfo,
            TBillCalcResult*  const outResult );
    //.........................................................................
    bool calcEachDiscount(
             const TMultiDiscountType inMultDiscountType,
             const TDiscountInfo* const inDiscountInfo,
			 const Currency inBasePrice,
			 const Currency	inTaxExemptPrice,
			 const Currency inSalePrice,
			 const bool     inTaxInclusivePrice,
			 const bool     inPriceTaxExempt,
                   TBillCalcResult* const outResult );
    //.........................................................................
    // Calculate the Discount for a Tax Inclusive Price
    bool calcEachTIPDiscount(
             const TMultiDiscountType inMultDiscountType,
             const TDiscountInfo* const inDiscountInfo,
			 const Currency inBasePrice,
			 const Currency	inTaxExemptPrice,
			 const Currency inSalePrice,
			 const bool     inPriceTaxExempt,
                   TBillCalcResult* const outResult );
    //.........................................................................
    // Calculate the Discount for a Not Tax Inclusive Price
    bool calcEachNTIPDiscount(
             const TMultiDiscountType inMultDiscountType,
             const TDiscountInfo*   const inDiscountInfo,
			 const Currency               inBasePrice,
			 const Currency				  inTaxExemptPrice,
             const Currency               inSalePrice,
                   TBillCalcResult* const outResult );
    //.........................................................................
    bool calcEachActualDiscount(
             const Currency               inActualPrice,
             const TMultiDiscountType     inMultDiscountType,
             const TDiscountInfo*   const inDiscountInfo,
                   TBillCalcResult* const outResult );
    //.........................................................................
    Currency calcActualDiscountContent(
                    const Currency             inActualPrice,
                    const TMultiDiscountType   inMultDiscountType,
                    const TDiscountInfo* const inDiscountInfo,
                    const Currency             inCumulativeDiscount );
    //.........................................................................
    Currency calcActualDiscountPercentage(
                Currency inActualPrice,
				Currency inActualDiscount );
    //.........................................................................
    // TAX CONTENT
    void calcTax(
            TPriceInfo* const inPriceInfo,
            TBillCalcResult* const outResult );
	//.........................................................................
	void removeSalesTaxesForExemptPrice( TPriceInfo* inPriceInfo );
	//.........................................................................
	void seperateEndTaxesFromRegularTaxes(TPriceInfo* inPriceInfo, TAX_INFO_LIST& endTaxList);
    //.........................................................................
    void populateEndTaxList(
             TAX_INFO_LIST& inTaxInfoList,
             TAX_INFO_LIST& outEndTaxList );
    //.........................................................................
    void groupTaxesByPriority(
                TAX_INFO_LIST  inTaxInfoList,
                TMultiTaxType  inMultiTaxType,
                TAX_GROUP_MAP& outGroupMap );
    //.........................................................................
    // Simple Miltiple Tax
    void calcSimpleTaxWithMap(
                  TAX_GROUP_MAP          inGroupMap,
            const TPriceInfo*      const inPriceInfo,
                  TBillCalcResult* const outResult );
    //.........................................................................
    // Compund Miltiple Tax
    void calcCompoundTaxWithMap(
                  TAX_GROUP_MAP          inGroupMap,
            const TPriceInfo*      const inPriceInfo,
                  TBillCalcResult* const outResult );
    //.........................................................................

    void calcNTIPCompoundTaxContent(
                            TAX_GROUP_MAP          inGroupMap,
                            const TPriceInfo*      const inPriceInfo,
                            Currency basePrice,
                            Currency salePrice,
                            bool taxInclusivePrice,
                            bool taxExemptPrice,
                            bool taxBeforeDiscount,
                            TBillCalcResult* const outResult,
                            TServiceChargeSettings &scSettings);
    //.........................................................................

    void calcTIPCompoundTaxContent(
                            TAX_GROUP_MAP          inGroupMap,
                            const TPriceInfo*      const inPriceInfo,
                            Currency basePrice,
                            Currency salePrice,
                            bool taxInclusivePrice,
                            bool taxExemptPrice,
                            bool taxBeforeDiscount,
                            TBillCalcResult* const outResult,
                            TServiceChargeSettings &scSettings);
    //.........................................................................
    void calcTaxWithList(
                  TAX_INFO_LIST inInfoList,
            const Currency inBasePrice,
            const Currency inSalePrice,
            const bool     inTaxInclusivePrice,
            const bool     inTaxExemptPrice,
            const bool     inTaxBeforeDiscount,
            const TServiceChargeSettings inSCSettings,
                  TBillCalcResult* const outResult );
    //.........................................................................
    void calcEachTax(
           const TTaxInfo* const inTaxInfo,
           const Currency inBasePrice,
           const Currency inSalePrice,
           const bool     inTaxInclusivePrice,
           const bool     inTaxExemptPrice,
           const bool     inTaxBeforeDiscount,
           const TServiceChargeSettings inSCSettings,
                 TBillCalcResult* const outResult );
    //.........................................................................
    // Calculate the Tax Content for a Tax Inclusive Price
    void calcEachTIPTax(
           const TTaxInfo* const  inTaxInfo,
           const Currency         inBasePrice,
           const Currency         inSalePrice,
           const bool             inTaxExemptPrice,
           const bool             inTaxBeforeDiscount,
           const TServiceChargeSettings inSCSettings,
           TBillCalcResult* const outResult );
    //.........................................................................
    // Calculate the Tax Content for a Not Tax Inclusive Price
    void calcEachNTIPTax(
           const TTaxInfo*  const inTaxInfo,
           const Currency         inBasePrice,
           const Currency         inSalePrice,
           const bool             inTaxExemptPrice,
           const bool             inTaxBeforeDiscount,
           const TServiceChargeSettings inSCSettings,
           TBillCalcResult* const outResult );
    //.........................................................................
    void calcEachActualTaxContent(
           Currency         inActualPrice,
           bool             inTaxExemptPrice,
           bool             inTaxInclusivePrice,
           const TTaxInfo*  const inTaxInfo,
           TBillCalcResult* const outResult );
	//.........................................................................
	// SERVICE CHARGE
	void calcServiceCharge(
			const TPriceInfo* 	   const inPriceInfo,
				  TBillCalcResult* const outResult );
    //.........................................................................
    // Calculate the Service Charge Tax Content for a Tax Inclusive Price
    void calcTIPServiceCharge(
             const TPriceInfo*      const inPriceInfo,
                   TBillCalcResult* const outResult );
    //.........................................................................
    // Calculate the Service Charge Tax Content for a Not Tax Inclusive Price
    void calcNTIPServiceCharge(
             const TPriceInfo*      const inPriceInfo,
                   TBillCalcResult* const outResult );
    //.........................................................................
    void calcActualServiceCharge(
			       Currency               inPrice,
             const TPriceInfo*      const inPriceInfo,
                   TBillCalcResult* const outResult );

    //.........................................................................
    Currency getServiceChargeContentInPrice(
                                    Currency salePrice,
                                    const TServiceChargeSettings    &inSCSettings,
                                    const TPriceInfo*      const inPriceInfo);
	//.........................................................................
	// FINAL TAX AND DISCOUNTS
	void doFinalDiscount( TPriceInfo* const inPriceInfo,  TBillCalcResult* const outResult, DISCOUNT_INFO_LIST& endDiscountList );
	//.........................................................................
	void doFinalTax(
            TPriceInfo*      const inPriceInfo,
            TBillCalcResult* const outResult,
            TAX_INFO_LIST& endTaxList,
            DISCOUNT_INFO_LIST& endDiscountList);
    //.........................................................................

	Currency calcContent( Currency inTotal, Currency inPercentage );
    //.........................................................................

    Currency calcTaxInclusiveContent(
                Currency inInclusiveTaxPrice,
                Currency inTaxPercentage );
	//.........................................................................
	Currency calcInclusiveContent(
				Currency inInclusivePrice,
				Currency inInclusiveContentPercentage );
	//........................................................................
	Currency getTotalTaxPercentage(
			  TMultiTaxType inMultiTaxType,
			  TAX_INFO_LIST inTaxInfo );
    //.........................................................................
	Currency calcPriceWithTax(
				Currency      inBasePrice,
				TAX_INFO_LIST inTaxInfoList );
    //.........................................................................

    Currency calcGrossPrice(
                 const TPriceInfo* const inPriceInfo,
                 TBillCalcResult* const inCalcResult );
    //.........................................................................

    Currency calcPartialTaxInclusivePrice(
                Currency      inPartialBasePrice,
                Currency      inPartialDiscount,
                TAX_INFO_LIST inTaxInfoList );
    //.........................................................................

	Currency calcFinalPrice(
				const TPriceInfo* 	   const inPriceInfo,
					  TBillCalcResult* const inCalcResult );
    //.........................................................................
	Currency calcTIPFinalPrice(
				const TPriceInfo* 	   const inPriceInfo,
					  TBillCalcResult* const inCalcResult );
    //.........................................................................
	Currency calcNTIPFinalPrice(
                const TPriceInfo* const inPriceInfo,
                TBillCalcResult* const inCalcResult );

    //.........................................................................
    Currency calcFinalPriceWithFinalDiscount(
                    TPriceInfo* const inPriceInfo,
                    TBillCalcResult*  const inCalcResult,
                    const DISCOUNT_INFO_LIST inEndDiscountList );
    //.........................................................................
    void putFinalDiscountInTheResult(
                    const TDiscountInfo* finalDiscountInfo,
                    Currency finalDiscountContent,
                    TBillCalcResult* outCalcResult );
    //.........................................................................
    void putFinalTaxInTheResult(
                    const TTaxInfo* finalTaxInfo,
                    Currency finalTaxContent,
                    TBillCalcResult* outCalcResult );
    //.........................................................................
    Currency totalTaxContent(
                 Currency      inPrice,
                 TMultiTaxType inMultiTaxType,
                 const TAX_INFO_LIST inTaxInfoList );
    //.........................................................................

    Currency recalcTotalTax(
                  bool  inFinalPriceTaxExempt,
                  const TBillCalcResult* const inCalcResult );
    //.........................................................................

    void splitTotalTaxContent(
                    const TPriceInfo* const inPriceInfo,
                    TBillCalcResult*        outResult );

    //.........................................................................
    void reCalculateTaxContentsOnBasePrice(
                    Currency inBasePrice,
                    const TAX_INFO_LIST* const inInfoList,
					TMultiTaxType multiTaxType,
					TBillCalcResult*        outResult );
	//.........................................................................
	void setDiscountGroupListResult(
			    const TPriceInfo* const inPriceInfo,
				TBillCalcResult*        outResult );
	//.........................................................................
	void calculateTaxesForDiscounts(
					const TPriceInfo*      const inPriceInfo,
						  TBillCalcResponse*       outResponse );

    //.........................................................................
    // Utility methods
    //.........................................................................

	Currency getPriceWithoutServiceChargeContents(
								   Currency	salePrice,
								   TServiceChargeSettings inSCSettings,
								   const TPriceInfo* const inPriceInfo);
	//.........................................................................
	TServiceChargeSettings getServiceChargeSettingsFromPriceInfo( const TPriceInfo* const inPriceInfo );

     void CalcPriceIncl( const TPriceInfo* const inPriceInfo, TAX_INFO_LIST& endTaxList, DISCOUNT_INFO_LIST& endDiscountList, TBillCalcResponse* response );

};
//---------------------------------------------------------------------------

} // namespace BillCalculator

#endif
