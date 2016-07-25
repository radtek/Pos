//---------------------------------------------------------------------------

#ifndef MMBillCalculatorAdapterH
#define MMBillCalculatorAdapterH

//---------------------------------------------------------------------------

#include "IBillCalculator.h"
#include "ItemComplete.h"
#include "TaxProfile.h"

#include "DiscountGroup.h"

//---------------------------------------------------------------------------


namespace BillCalculator
{

class TMMBillCalculatorAdapter
{
public:
    TMMBillCalculatorAdapter();
   ~TMMBillCalculatorAdapter();

    void CalculateOrderItemPrices(TItemMinorComplete* inOrderItem);

private:
    TPriceInfo         ConvertItemCompleteToPriceInfo(       TItemMinorComplete* inOrderItem  );
    TTaxInfo           ConvertTaxProfileToTaxInfo(           TaxProfile*         inTaxProfile );
    TDiscountInfo      ConvertDiscountToDiscountInfo(        TDiscount*          inDiscount   );
    TServiceChargeInfo ConvertTaxProfileToServiceChargeInfo( TaxProfile*         inTaxProfile );

    void               ConvertBillCalcResultForRefundOrder(  TBillCalcResult &outResult );
    void               StretchBillCalcResultForQuantity( TBillCalcResult &outResult, Currency qty );
    bool               IsRefundedOrder( const TItemMinorComplete* order );

    void setDiscountGroupListFromList(
            std::vector<TDiscountGroup> inDiscountGroupList,
            TDiscountInfo&              outDiscountInfo );

};
//---------------------------------------------------------------------------

} // namespace BillCalculator


#endif
