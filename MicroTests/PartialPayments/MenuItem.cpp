//---------------------------------------------------------------------------


#pragma hdrstop

#include "MenuItem.h"

//---------------------------------------------------------------------------

MenuItem::MenuItem( TPriceInfo info )
{
    PriceInfo = info;
}

void MenuItem::AddTax(TTaxInfo taxInfo)
{
    PriceInfo.TaxInfoList.push_back( taxInfo );
}

void MenuItem::AddDiscount(TDiscountInfo discountInfo)
{
    PriceInfo.DiscountInfoList.push_back( discountInfo );
}

void MenuItem::AddServiceCharge( TServiceChargeInfo serviceChargeInfo )
{
    PriceInfo.ServiceChargeInfo = serviceChargeInfo;
}

void MenuItem::CalcResult()
{
    TMMBillCalculator billCalc;
    TBillCalcResponse* response = billCalc.Execute( &PriceInfo );

    if(response->Succesful)
       BillCalcResult = response->Result;
}

#pragma package(smart_init)
