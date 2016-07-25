//---------------------------------------------------------------------------

#ifndef MenuItemListManagerH
#define MenuItemListManagerH
//---------------------------------------------------------------------------

#include "MenuItem.h"

typedef std::vector<MenuItem> ITEM_VECTOR;
typedef std::vector<Currency> CURRENCY_VECTOR;

class MenuItemListManager
{
    public:
        MenuItemListManager();

        void SetItems( std::vector<MenuItem> items );
        void DoPartialPayment( Currency partialPay );
        std::map<AnsiString, CURRENCY_VECTOR> GetResultsMap();

    private:
        ITEM_VECTOR item_list;

        Currency partial_pay;
        Currency total_menu_price;
        Currency total_final_price;
        Currency total_remaining;
        Currency total_remaining_percent;
        Currency total_new_menu_price;

        std::map<AnsiString, CURRENCY_VECTOR> result_map;

        void executeItemList();
        void clearMap();

        void calcTotalPrice( ITEM_VECTOR inItemList, Currency& outTotalMenuPrice, Currency& outTotalFinalPrice );
        Currency calcTotalRemaining( Currency inPartialPay, Currency inTotalFinalPrice );
        Currency calcRemainingPercentage( Currency inTotalRemaining, Currency inTotalFinalPrice );
        void calcEachRemainingFP( Currency inTotalRemaining, Currency inTotalRemainingPercent, ITEM_VECTOR inItemList, CURRENCY_VECTOR& outRemainingFPList );
        Currency calcTotalNewMenuPrice( Currency inTotalRemainingPercent, Currency inTotalMenuPrice );
        void calcEachNewMP( Currency total_new_menu_price, CURRENCY_VECTOR inRemainingFPList, ITEM_VECTOR inItemList, CURRENCY_VECTOR& outNewMPList, CURRENCY_VECTOR& outCalculatedFPList );

        Currency reverseCalcMenuPrice( Currency inRemainingFP, TPriceInfo inPriceInfo, Currency inFinalPrice );
        Currency calcTaxInclusiveMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice );
        Currency calcNonTaxInclusiveMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice );
        Currency calcTaxInclusiveSimpleMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice );
        Currency calcTaxInclusiveCompoundMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice );
        Currency calcNonTaxInclusiveSimpleMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice );
        Currency calcNonTaxInclusiveCompoundMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice );

        Currency getTotalDiscountPercentage( TPriceInfo &inPriceInfo );
        Currency getTotalTaxPercentage( TPriceInfo &inPriceInfo );

        Currency calculateFinalPrice( TPriceInfo inPriceInfo, Currency inMenuPrice );
};

#endif
