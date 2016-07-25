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

    #ifdef _DEBUG
    //AllocConsole();
    #endif

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
        if(discount.Mode == DiscModePoints)
          continue;

        if( IsRefundedOrder( inOrderItem ))
        {
         discount.Amount  =-1*discount.Amount;
        }
        TDiscountInfo discountInfo = ConvertDiscountToDiscountInfo( &discount );

        priceInfo.DiscountInfoList.push_back( discountInfo );
        priceInfo.PriceTaxExempt = (discountInfo.DiscountGroupList.size() == 1 && discountInfo.DiscountGroupList[0] == SCD_DISCOUNT_GROUP) // discountInfo->; TODO: Look to see if this discount is in the Senior Citizans Discount group
                                    || (discountInfo.DiscountGroupList.size() == 1 && discountInfo.DiscountGroupList[0] == COMPLIMENTARY_GROUP)
                                    || (discountInfo.DiscountGroupList.size() == 1 && discountInfo.DiscountGroupList[0] == NONCHARGABLE_GROUP) ;
        priceInfo.CalcDWTOnBasePrice =  priceInfo.PriceTaxExempt|| discountInfo.RecalcPriceWithTaxAfterDiscount;
    }

    #ifdef _DEBUG
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    AnsiString priceInfoString =
        "Price Info\n"
        "Value: " + priceInfo.Value + "\n"
        "TaxInclusive: " + (priceInfo.TaxInclusive ? "True" : "False") + "\n"
        "TaxExclusiveServiceCharge: " + (priceInfo.TaxExclusiveServiceCharge ? "True" : "False") + "\n"
        "TaxBeforeDiscount: " + (priceInfo.TaxBeforeDiscount ? "True" : "False") + "\n"
        "ServiceChargeBeforeDiscount: " + (priceInfo.ServiceChargeBeforeDiscount ? "True" : "False") + "\n"
        "MultiDiscountType: " + priceInfo.MultiDiscountType + "\n"
        "MultiTaxType: " + priceInfo.MultiTaxType + "\n"
        "PriceTaxExempt: " + (priceInfo.PriceTaxExempt ? "True" : "False") + "\n\n";
    WriteConsole( handle, priceInfoString.c_str(), priceInfoString.Length(), 0, 0 );
    FreeConsole();
    #endif

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

    //*
    #ifdef _DEBUG
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    AnsiString taxInfoString =
        "Tax Info\n"
        "Name: " + taxInfo.Name + "\n"
        "Percentage: " + CurrToStrF( taxInfo.Percentage, ffCurrency, 2 ) + "\n\n";
    WriteConsole( handle, taxInfoString.c_str(), taxInfoString.Length(), 0, 0 );
    #endif //*/

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
    discountInfo.RecalcPriceWithTaxAfterDiscount = inDiscount->IsPersonWithDisabilityDiscount();

    setDiscountGroupListFromList( inDiscount->DiscountGroupList, discountInfo );

    switch ( inDiscount->Mode )
    {
        case DiscModeItem:
        case DiscModeCurrency:
            discountInfo.Value        = ( inDiscount->Amount < 0 ) ? ( inDiscount->Amount * -1 ) : inDiscount->Amount;
            discountInfo.DiscountType = ( inDiscount->Amount < 0 ) ? dtSurcharge : dtDiscount;
            discountInfo.DiscountWay  = dwMoney;
            break;
        case DiscModePercent:
            discountInfo.DiscountType = ( inDiscount->PercentAmount < 0 ) ? dtSurcharge : dtDiscount;
            discountInfo.DiscountWay  = dwPercentage;
            break;
        case DiscModeSetPrice: // combo,deals and set prices works the same way
        case DiscModeCombo:
        case DiscModeDeal:
            discountInfo.Value        = inDiscount->Amount;
            discountInfo.DiscountType = dtDiscount;
            discountInfo.DiscountWay  = dwSetPrice;
            break;
        default:
            throw Exception("Discount Type Not Implemented in Bill Calculator");
    }

    #ifdef _DEBUG
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    AnsiString discountInfoString =
        "Discount Info\n"
        "Name: " + discountInfo.Name + "\n"
        "Value: " + CurrToStr(discountInfo.Value) + "\n"
        "Percentage: " + FloatToStr(discountInfo.Percentage) + "\n"
        "Priority: " + discountInfo.Priority + "\n"
        "DiscountType: " + discountInfo.DiscountType + "\n"
        "DiscountWay: " + discountInfo.DiscountWay + "\n\n";
    WriteConsole( handle, discountInfoString.c_str(), discountInfoString.Length(), 0, 0 );
    #endif //*/

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

    #ifdef _DEBUG
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    AnsiString serviceChargeInfoString =
        "Service Charge Info\n"
        "Percentage: " + FloatToStr(serviceChargeInfo.Percentage) + "\n"
        "TaxPercentage: " + FloatToStr(serviceChargeInfo.TaxPercentage) + "\n\n";
    WriteConsole( handle, serviceChargeInfoString.c_str(), serviceChargeInfoString.Length(), 0, 0 );
    #endif //*/

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


