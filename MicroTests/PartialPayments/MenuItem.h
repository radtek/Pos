//---------------------------------------------------------------------------

#ifndef MenuItemH
#define MenuItemH
//---------------------------------------------------------------------------

#include "MMBillCalculator.h"

using namespace BillCalculator;

class MenuItem
{
    public:
        Currency Qty;

        TPriceInfo PriceInfo;
        TBillCalcResult BillCalcResult;

        MenuItem( TPriceInfo info );

        void AddTax(TTaxInfo taxInfo);
        void AddDiscount(TDiscountInfo discountInfo);
        void AddServiceCharge( TServiceChargeInfo serviceChargeInfo );

        void CalcResult();
};

#endif
