//---------------------------------------------------------------------------


#pragma hdrstop

#include "MenuItemListManager.h"

//---------------------------------------------------------------------------

MenuItemListManager::MenuItemListManager()
{
    item_list.clear();
}
//------------------------------------------------------------------------------

void MenuItemListManager::SetItems( std::vector<MenuItem> items )
{
    item_list.clear();
    item_list.assign( items.begin(), items.end() );

    executeItemList();
}
//------------------------------------------------------------------------------

void MenuItemListManager::DoPartialPayment( Currency partialPay )
{
    clearMap();

    //..........................................................

    partial_pay = partialPay;

    //..........................................................

    calcTotalPrice( item_list, total_menu_price, total_final_price );

    total_remaining         = calcTotalRemaining( partial_pay, total_final_price );
    total_remaining_percent = calcRemainingPercentage( total_remaining, total_final_price );

    //......................................................

    CURRENCY_VECTOR remainingFPList;
    CURRENCY_VECTOR newMPList;
    CURRENCY_VECTOR calculatedFPList;

    calcEachRemainingFP( total_remaining, total_remaining_percent, item_list, remainingFPList );

    total_new_menu_price = calcTotalNewMenuPrice( total_remaining_percent, total_menu_price );

    calcEachNewMP( total_new_menu_price, remainingFPList, item_list, newMPList, calculatedFPList );

    //......................................................

    result_map["remainingFP"]  = remainingFPList;
    result_map["newMP"]        = newMPList;
    result_map["calculatedMP"] = calculatedFPList;
}
//------------------------------------------------------------------------------

std::map<AnsiString, CURRENCY_VECTOR> MenuItemListManager::GetResultsMap()
{
    return result_map;
}
//------------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE
//::::::::::::::::::::::::::::::::::::::::::::::::

void MenuItemListManager::executeItemList()
{
    std::vector<MenuItem>::iterator it = item_list.begin();
    for( ; it != item_list.end(); it++ )
    {
        it->CalcResult();
    }
}

void MenuItemListManager::clearMap()
{
    try
    {
       result_map["remainingFP"].clear();
    }
    catch( ... )
    {
    }

    try
    {
       result_map["newMP"].clear();
    }
    catch( ... )
    {
    }
}
//------------------------------------------------------------------------------

void MenuItemListManager::calcTotalPrice( ITEM_VECTOR inItemList, Currency& outTotalMenuPrice, Currency& outTotalFinalPrice )
{
    outTotalMenuPrice  = 0;
    outTotalFinalPrice = 0;

    //............................................................

    ITEM_VECTOR::iterator it = inItemList.begin();

    for( ; it != inItemList.end(); it++ )
    {
        outTotalMenuPrice  += it->PriceInfo.Value;
        outTotalFinalPrice += it->BillCalcResult.FinalPrice;
    }
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::calcTotalRemaining( Currency inPartialPay, Currency inTotalFinalPrice )
{
    Currency result = inTotalFinalPrice - inPartialPay;

    return result;
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::calcRemainingPercentage( Currency inTotalRemaining, Currency inTotalFinalPrice )
{
    Currency result = ( inTotalRemaining * Currency( 100.0 ) ) / inTotalFinalPrice ;
    //Currency result = ( inTotalRemaining / inTotalFinalPrice ) * 100.00;

    return result;
}
//------------------------------------------------------------------------------

void MenuItemListManager::calcEachRemainingFP( Currency inTotalRemaining, Currency inTotalRemainingPercent, ITEM_VECTOR inItemList, CURRENCY_VECTOR& outRemainingFPList )
{
    outRemainingFPList.clear();

    Currency value = 0;
    Currency tmp   = Currency(0);

    for( int i = 0; i < ( inItemList.size() - 1 ); i++  )
    {
        value = ( inItemList[i].BillCalcResult.FinalPrice  * inTotalRemainingPercent ) / Currency( 100.0 );
        outRemainingFPList.push_back( value );

        tmp += value;
    }

    value = inTotalRemaining - tmp;
    outRemainingFPList.push_back( value );
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::calcTotalNewMenuPrice( Currency inTotalRemainingPercent, Currency inTotalMenuPrice )
{
    Currency result = ( inTotalMenuPrice  / Currency( 100.0 ) ) * inTotalRemainingPercent;
    //Currency result = ( inTotalMenuPrice * inTotalRemainingPercent ) / Currency( 100.0 );

    return result;
}
//------------------------------------------------------------------------------

void MenuItemListManager::calcEachNewMP( Currency total_new_menu_price, CURRENCY_VECTOR inRemainingFPList, ITEM_VECTOR inItemList, CURRENCY_VECTOR& outNewMPList, CURRENCY_VECTOR& outCalculatedFPList )
{
    Currency menuPrice = 0;
    Currency finalPrice = 0;
    Currency tmp = Currency( 0.0 );

    int i;

    for( i = 0; i < ( inItemList.size() - 1 ); i++  )
    {
        menuPrice = reverseCalcMenuPrice( inRemainingFPList[i], inItemList[i].PriceInfo, inItemList[i].BillCalcResult.FinalPrice );
        outNewMPList.push_back( menuPrice );

        finalPrice = calculateFinalPrice( inItemList[i].PriceInfo, menuPrice );
        outCalculatedFPList.push_back( finalPrice );

        tmp += menuPrice;
    }

    menuPrice = total_new_menu_price - tmp;
    outNewMPList.push_back( menuPrice );

    finalPrice = calculateFinalPrice( inItemList[i].PriceInfo, menuPrice );
    outCalculatedFPList.push_back( finalPrice );
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::reverseCalcMenuPrice( Currency inRemainingFP, TPriceInfo inPriceInfo, Currency inFinalPrice )
{
    Currency value = 0;

    if( inPriceInfo.TaxInclusive )
    {
        value = calcTaxInclusiveMenuPrice( inRemainingFP, inPriceInfo, inFinalPrice );
    }
    else
    {
        value = calcNonTaxInclusiveMenuPrice( inRemainingFP, inPriceInfo, inFinalPrice );
    }

    return value;
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::calcTaxInclusiveMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice )
{
    if( inPriceInfo.MultiTaxType == mttSimple && inPriceInfo.MultiDiscountType == mdtSimple )
    {
        return calcTaxInclusiveSimpleMenuPrice( inRemainingFP, inPriceInfo, inFinalPrice );
    }

    if( inPriceInfo.MultiTaxType == mttCompound && inPriceInfo.MultiDiscountType == mdtCompound )
    {
        return calcTaxInclusiveCompoundMenuPrice( inRemainingFP, inPriceInfo, inFinalPrice );
    }
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::calcNonTaxInclusiveMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice )
{
    if( inPriceInfo.MultiTaxType == mttSimple && inPriceInfo.MultiDiscountType == mdtSimple )
    {
        return calcNonTaxInclusiveSimpleMenuPrice( inRemainingFP, inPriceInfo, inFinalPrice );
    }

    if( inPriceInfo.MultiTaxType == mttCompound && inPriceInfo.MultiDiscountType == mdtCompound )
    {
        return calcNonTaxInclusiveCompoundMenuPrice( inRemainingFP, inPriceInfo, inFinalPrice );
    }
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::calcTaxInclusiveSimpleMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice  )
{
    Currency value = 0;

    Currency discountPercentage         = getTotalDiscountPercentage( inPriceInfo );
    Currency taxPercentage              = getTotalTaxPercentage( inPriceInfo );
    Currency scPercentage               = inPriceInfo.ServiceChargeInfo.Percentage;
    Currency scTaxPercentage            = inPriceInfo.ServiceChargeInfo.TaxPercentage;

    //.....................................................

    Currency _100 = Currency( 100.0 );
    Currency _10000 = Currency( 10000.0 );

    Currency upper = ( ( _10000 * inRemainingFP ) * ( _100 + taxPercentage ) ) / ( _10000 + ( _100 * taxPercentage ) + ( _100 * scPercentage ) + ( scPercentage * scTaxPercentage ) );
    Currency lower = 1 - ( discountPercentage / _100 );

    //Currency upper = ( ( _10000 * inRemainingFP ) * ( _100 + taxPercentage ) ) / ( _10000 + ( _100 * taxPercentage ) + ( _100 * scPercentage ) + ( scPercentage * scTaxPercentage ) );
    //Currency lower = ( _100 - discountPercentage ) / _100;

    value = upper / lower;

    //.....................................................

    return value;
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::calcTaxInclusiveCompoundMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice )
{
    return 0;
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::calcNonTaxInclusiveSimpleMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice )
{
    Currency value = 0;

    Currency discountPercentage         = getTotalDiscountPercentage( inPriceInfo );
    Currency taxPercentage              = getTotalTaxPercentage( inPriceInfo );
    Currency scPercentage               = inPriceInfo.ServiceChargeInfo.Percentage;
    Currency scTaxPercentage            = inPriceInfo.ServiceChargeInfo.TaxPercentage;

    //.....................................................

    Currency _100   = Currency( 100.0 );
    Currency _10000 = Currency( 10000.0 );

    Currency upper = ( _10000 * inRemainingFP ) / ( _10000 + ( _100 * taxPercentage ) + ( _100 * scPercentage ) + ( scPercentage * scTaxPercentage ) );
    Currency lower = 1 - ( discountPercentage / _100 );

    //Currency upper = ( ( _10000 * inRemainingFP ) * ( _100 + taxPercentage ) ) / ( _10000 + ( _100 * taxPercentage ) + ( _100 * scPercentage ) + ( scPercentage * scTaxPercentage ) );
    //Currency lower = ( _100 - discountPercentage ) / _100;

    value = upper / lower;

    //.....................................................

    return value;
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::calcNonTaxInclusiveCompoundMenuPrice( Currency inRemainingFP, TPriceInfo &inPriceInfo, Currency inFinalPrice )
{
    return 0;
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::getTotalDiscountPercentage( TPriceInfo &inPriceInfo )
{
    Currency value = 0;

    DISCOUNT_INFO_LIST::iterator it = inPriceInfo.DiscountInfoList.begin();
    for( ; it != inPriceInfo.DiscountInfoList.end(); it++ )
    {
        value += it->Percentage;
    }

    return value;
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::getTotalTaxPercentage( TPriceInfo &inPriceInfo )
{
    Currency value = 0;

    TAX_INFO_LIST::iterator it = inPriceInfo.TaxInfoList.begin();
    for( ; it != inPriceInfo.TaxInfoList.end(); it++ )
    {
        value += it->Percentage;
    }

    return value;
}
//------------------------------------------------------------------------------

Currency MenuItemListManager::calculateFinalPrice( TPriceInfo inPriceInfo, Currency inMenuPrice )
{
    TMMBillCalculator billCalc;

    TPriceInfo *calculatedPI = billCalc.CreatePriceInfoWithPriceInfo( &inPriceInfo );

    calculatedPI->Value = inMenuPrice;

    TBillCalcResponse* response = billCalc.Execute( calculatedPI );

    if( response->Succesful )
    {
        return response->Result.FinalPrice;
    }
    else
    {
        return Currency( 0.0 );
    }
}
//------------------------------------------------------------------------------

#pragma package(smart_init)
