//---------------------------------------------------------------------------

#pragma hdrstop

#pragma package(smart_init)

#include "MMBillCalculatorAdapter.h"
#include "MMBillCalculator.h"
#include "GlobalSettings.h"
#include "Discount.h"
#include "math.h"
#include <string>

//---------------------------------------------------------------------------

namespace BillCalculator
{


TMMBillCalculatorAdapter::TMMBillCalculatorAdapter()
{
}
//---------------------------------------------------------------------------
TMMBillCalculatorAdapter::~TMMBillCalculatorAdapter()
{
}
//---------------------------------------------------------------------------

void TMMBillCalculatorAdapter::CalculateOrderItemPrices( TItemMinorComplete* inOrderItem )
{
    IBillCalculator*   billCalc  = new TMMBillCalculator();
    TPriceInfo         priceInfo = ConvertItemCompleteToPriceInfo( inOrderItem );
    TBillCalcResponse* response  = billCalc->Execute( &priceInfo );

    if ( response->Succesful )
    {
        StretchBillCalcResultForQuantity( response->Result, fabs(inOrderItem->GetQty()) );

        if( IsRefundedOrder( inOrderItem ))
        {
            // converts the result to be suitable for refunded order
            ConvertBillCalcResultForRefundOrder( response->Result );

        }

        inOrderItem->BillCalcResult = response->Result;
    }
    delete response;
    delete billCalc;
    //return response;
}
//---------------------------------------------------------------------------

TPriceInfo TMMBillCalculatorAdapter::ConvertItemCompleteToPriceInfo( TItemMinorComplete* inOrderItem  )
{

    TPriceInfo priceInfo;

    priceInfo.Value                       = inOrderItem->PriceEach(); // always use the positive prices for billCalculator as it does not support negative calculations
    priceInfo.MaxRetailPrice              = inOrderItem->MaxRetailPrice;
    priceInfo.TaxInclusive                = TGlobalSettings::Instance().ItemPriceIncludeTax;
	priceInfo.TaxExclusiveServiceCharge   = true; //TODO: Make this an actual setting for outside of the Philippines
	priceInfo.ServiceChargeInclusive	  = TGlobalSettings::Instance().ItemPriceIncludeServiceCharge;
	priceInfo.ServiceChargeTaxInclusive	  = priceInfo.TaxInclusive
                                                && priceInfo.ServiceChargeInclusive;    // item price must be both tax and service charge inclusive for this to be true
    priceInfo.TaxBeforeDiscount           = !TGlobalSettings::Instance().ReCalculateTaxPostDiscount;
    priceInfo.ServiceChargeBeforeDiscount = !TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount;
    priceInfo.MultiDiscountType           = mdtCompound;
    priceInfo.MultiTaxType                = mttSimple;
    for( int i = 0; i < inOrderItem->TaxProfiles.size(); i++)
    {
        TaxProfile taxProfile = inOrderItem->TaxProfiles[i];
        /*
            MM-3663
            In case of profit tax we only need to take the tax value with us
            We really dont care about other taxes, but we need to take into account the service charge
            The service charge can be inclusive or exclusive...
        */
        if(taxProfile.taxProfileType == ProfitTax)
        {
            TTaxInfo taxInfo = ConvertTaxProfileToTaxInfo( &taxProfile );
            priceInfo.TaxInfoList.push_back( taxInfo );
        }
        else if( taxProfile.taxProfileType == SalesTax || taxProfile.taxProfileType == LocalTax)
        {
            TTaxInfo taxInfo = ConvertTaxProfileToTaxInfo( &taxProfile );
            if(inOrderItem->RemovedTaxes->IndexOf(taxInfo.Name)  >= 0)
            {
                if(TGlobalSettings::Instance().ItemPriceIncludeTax)
                    priceInfo.RemovedTaxInfoList.push_back( taxInfo );
                else
                    taxInfo.Percentage = 0;
            }
            priceInfo.TaxInfoList.push_back( taxInfo );
        }
        else if( taxProfile.taxProfileType == ServiceCharge )
        {
            priceInfo.ServiceChargeInfo = ConvertTaxProfileToServiceChargeInfo( &taxProfile );
            if(inOrderItem->RemovedTaxes->IndexOf(taxProfile.taxProfileName)  >= 0)
            {
                /*
                    when service charge is removed through the tax removal feature,
                    we keep a copy of existing serviceChargeInfo, so that we can remove it within billCalculator
                */
                priceInfo.RemovedServiceChargeInfo = priceInfo.ServiceChargeInfo;
            }
        }
    }

    for( int i = 0; i < inOrderItem->Discounts.size(); i++ )
    {
        TDiscount discount = inOrderItem->Discounts[i];
        if( IsRefundedOrder( inOrderItem ))
        {
         discount.Amount  =-1*discount.Amount;
        }
        TDiscountInfo discountInfo = ConvertDiscountToDiscountInfo( &discount );

        priceInfo.DiscountInfoList.push_back( discountInfo );
        priceInfo.PriceTaxExempt = (discountInfo.DiscountGroupList.size() == 1 && discountInfo.DiscountGroupList[0] == SCD_DISCOUNT_GROUP) // discountInfo->; TODO: Look to see if this discount is in the Senior Citizans Discount group
                                    || (discountInfo.DiscountGroupList.size() == 1 && discountInfo.DiscountGroupList[0] == PWD_DISCOUNT_GROUP)
                                    || (discountInfo.DiscountGroupList.size() == 1 && discountInfo.DiscountGroupList[0] == COMPLIMENTARY_GROUP)
                                    || (discountInfo.DiscountGroupList.size() == 1 && discountInfo.DiscountGroupList[0] == NONCHARGABLE_GROUP) ;
        priceInfo.CalcDWTOnBasePrice =  priceInfo.PriceTaxExempt|| discountInfo.RecalcPriceWithTaxAfterDiscount;
    }

    return priceInfo;
}
//---------------------------------------------------------------------------

TTaxInfo TMMBillCalculatorAdapter::ConvertTaxProfileToTaxInfo( TaxProfile* inTaxProfile )
{
    TTaxInfo taxInfo;

    taxInfo.Name       = inTaxProfile->taxProfileName;
    taxInfo.Percentage = inTaxProfile->taxPercentage;
    taxInfo.Priority   = inTaxProfile->taxPriority;
    switch( inTaxProfile->taxProfileType )
    {
        case SalesTax:
            taxInfo.TaxType = ttSale;
            break;
        case LocalTax:
            taxInfo.TaxType = ttLocal;
            break;
        case ProfitTax:
            taxInfo.TaxType = ttProfit;
            break;
        default:
            break;
    }

    return taxInfo;
}
//---------------------------------------------------------------------------

TDiscountInfo TMMBillCalculatorAdapter::ConvertDiscountToDiscountInfo( TDiscount* inDiscount )
{
    TDiscountInfo discountInfo;

    discountInfo.DiscountKey          = inDiscount->DiscountKey;
    discountInfo.Name                 = inDiscount->Name;
    discountInfo.Description          = inDiscount->GetDescription();
    discountInfo.Percentage           = fabs(inDiscount->PercentAmount);
    discountInfo.Priority             = inDiscount->Priority;
    discountInfo.RecalcPriceWithTaxAfterDiscount = false;//inDiscount->IsPersonWithDisabilityDiscount();   Changed on account of change in behaviour of PWD discount
    discountInfo.DiscountCode = inDiscount->DiscountCode;
    discountInfo.IsCloudDiscount = inDiscount->IsCloudDiscount;
    discountInfo.DailyUsageAllowedPerMember = inDiscount->DailyUsageAllowedPerMember;
    discountInfo.MembersOnly = inDiscount->MembersOnly;
    setDiscountGroupListFromList( inDiscount->DiscountGroupList, discountInfo );

    switch ( inDiscount->AppliedMode )
    {
        case DiscModePoints:
            discountInfo.Value        = 0;
            discountInfo.DiscountType = dtDiscount;
            discountInfo.DiscountWay  = dwMoney;
        case DiscModeItem:
        case DiscModeCurrency:
        case DiscModeCombo:
            discountInfo.Value        = ( inDiscount->Amount < 0 ) ? ( inDiscount->Amount * -1 ) : inDiscount->Amount;
            discountInfo.DiscountType = ( inDiscount->Amount < 0 ) ? dtSurcharge : dtDiscount;
            discountInfo.DiscountWay  = dwMoney;
            break;
        case DiscModePercent:
            discountInfo.DiscountType = ( inDiscount->PercentAmount < 0 ) ? dtSurcharge : dtDiscount;
            discountInfo.DiscountWay  = dwPercentage;
            break;
        case DiscModeSetPrice: // combo,deals and set prices works the same way
        case DiscModeDeal:
            discountInfo.Value        = inDiscount->Amount;
            discountInfo.DiscountType = dtDiscount;
            discountInfo.DiscountWay  = dwSetPrice;
            break;
        default:
            throw Exception("Discount Type Not Implemented in Bill Calculator");
    }

    return discountInfo;
}
//---------------------------------------------------------------------------

TServiceChargeInfo TMMBillCalculatorAdapter::ConvertTaxProfileToServiceChargeInfo( TaxProfile* inTaxProfile )
{
    TServiceChargeInfo serviceChargeInfo;

    serviceChargeInfo.Percentage = inTaxProfile->taxPercentage;
    if( TGlobalSettings::Instance().ApplyServiceChargeTax )
    {
        serviceChargeInfo.TaxPercentage = TGlobalSettings::Instance().ServiceChargeTaxRate;
    }
    else
    {
        serviceChargeInfo.TaxPercentage = 0;
    }

    return serviceChargeInfo;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TMMBillCalculatorAdapter::setDiscountGroupListFromList(
            std::vector<TDiscountGroup> inDiscountGroupList,
            TDiscountInfo&              outDiscountInfo )
{
    outDiscountInfo.DiscountGroupList.clear();

    std::vector<TDiscountGroup>::iterator dgIT;

    for( dgIT  = inDiscountGroupList.begin();
         dgIT != inDiscountGroupList.end(); dgIT++ )
    {
        UnicodeString gname = dgIT->Name;

        outDiscountInfo.DiscountGroupList.push_back( gname );
    }
}
//---------------------------------------------------------------------------

void TMMBillCalculatorAdapter::StretchBillCalcResultForQuantity(
                                            TBillCalcResult &outResult,
                                            Currency qty )
{
    outResult.BasePrice = outResult.BasePrice;// * qty;
    outResult.TotalTax = outResult.TotalTax * qty;
    outResult.TotalDiscount = outResult.TotalDiscount * qty;
    outResult.FinalPrice = outResult.FinalPrice * qty;
    outResult.GrossPrice = outResult.GrossPrice * qty;
    outResult.ServiceCharge.Value = outResult.ServiceCharge.Value * qty;
    outResult.ServiceCharge.TaxValue = outResult.ServiceCharge.TaxValue * qty;
    outResult.DiscountWithoutTax =  outResult.DiscountWithoutTax * qty;
    outResult.TaxOnDiscount =  outResult.TaxOnDiscount * qty;
    DISCOUNT_RESULT_LIST::iterator it_discount = outResult.Discount.begin();
    for( ; it_discount != outResult.Discount.end(); it_discount++ )
    {
      (*it_discount).Content = (*it_discount).Content * qty;
      (*it_discount).Value   = (*it_discount).Value * qty;
    }

    TAX_RESULT_LIST::iterator it_tax = outResult.Tax.begin();
    for( ; it_tax != outResult.Tax.end(); it_tax++ )
    {
        (*it_tax).Value = (*it_tax).Value * qty;
    }
}

void TMMBillCalculatorAdapter::ConvertBillCalcResultForRefundOrder(
                                            TBillCalcResult &outResult )
{
    outResult.BasePrice = outResult.BasePrice * -1;
    outResult.TotalTax = outResult.TotalTax * -1;
    outResult.TotalDiscount = outResult.TotalDiscount *  -1;
    outResult.FinalPrice = outResult.FinalPrice * -1;
    outResult.GrossPrice = outResult.GrossPrice * -1;

    outResult.ServiceCharge.Value = outResult.ServiceCharge.Value * -1;
    outResult.ServiceCharge.TaxValue = outResult.ServiceCharge.TaxValue * -1;

    DISCOUNT_RESULT_LIST::iterator it_discount = outResult.Discount.begin();
    for( ; it_discount != outResult.Discount.end(); it_discount++ )
    {
        (*it_discount).Content = (*it_discount).Content *  -1;
        (*it_discount).Value   = (*it_discount).Value *  -1;
    }

    TAX_RESULT_LIST::iterator it_tax = outResult.Tax.begin();
    for( ; it_tax != outResult.Tax.end(); it_tax++ )
    {
        (*it_tax).Value = (*it_tax).Value * -1;
    }
}
//---------------------------------------------------------------------------

bool TMMBillCalculatorAdapter::IsRefundedOrder( const TItemMinorComplete* order )
{
    return order->OrderType == CreditNonExistingOrder && order->GetQty() < 0;
}
//---------------------------------------------------------------------------

}


