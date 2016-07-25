//---------------------------------------------------------------------------


#pragma hdrstop

#include "TestMMBillCalculator_Base.h"

//---------------------------------------------------------------------------

__fastcall TTestMMBillCalculator_Base::TTestMMBillCalculator_Base(System::String name)
        : TTestCase(name)
{
}
//------------------------------------------------------------------------------

__fastcall TTestMMBillCalculator_Base::~TTestMMBillCalculator_Base()
{
}
//------------------------------------------------------------------------------

void __fastcall TTestMMBillCalculator_Base::SetUp()
{
    priceInfoObj = new TPriceInfo();
    initializePriceInfoToTaxSettings( priceInfoObj );

    delete billCalcResponseObj;
}
//------------------------------------------------------------------------------

void __fastcall TTestMMBillCalculator_Base::TearDown()
{
    delete priceInfoObj;
    delete billCalcResponseObj;
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::initializePriceInfoToTaxSettings( TPriceInfo* priceInfoObj )
{
    priceInfoObj->TaxInclusive = true;
    priceInfoObj->TaxExclusiveServiceCharge = true;

    priceInfoObj->TaxBeforeDiscount = false;
    priceInfoObj->ServiceChargeBeforeDiscount = false;

    priceInfoObj->MultiDiscountType = mdtSimple;
    priceInfoObj->MultiTaxType      = mttSimple;

    priceInfoObj->TaxInfoList.clear();
    priceInfoObj->DiscountInfoList.clear();

    priceInfoObj->ServiceChargeInfo.Percentage  = Curr0;
    priceInfoObj->ServiceChargeInfo.TaxPercentage = Curr0;

    priceInfoObj->PriceTaxExempt = false;
    priceInfoObj->CalcDWTOnBasePrice = false;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TTestMMBillCalculator_Base::execute()
{
    billCalcResponseObj = billCalculator.Execute( priceInfoObj );
}
//------------------------------------------------------------------------------
void TTestMMBillCalculator_Base::execute( TPriceInfo &priceInfo )
{
    billCalcResponseObj = billCalculator.Execute( &priceInfo );
}
//------------------------------------------------------------------------------
void TTestMMBillCalculator_Base::applyTaxToCurrentPriceInfo( string name, Currency percentage, int priority )
{
    applyTaxToPriceInfo( *priceInfoObj, name, percentage, priority );
}
//------------------------------------------------------------------------------
void TTestMMBillCalculator_Base::applyLocalTaxToCurrentPriceInfo( string name, Currency percentage )
{
    applyLocalTaxToPriceInfo( *priceInfoObj, name, percentage );
}
//------------------------------------------------------------------------------
void TTestMMBillCalculator_Base::applyServiceChargeToCurrentPriceInfo( Currency scPerc, Currency scTaxPerc )
{
    applyServiceChargeToPriceInfo( *priceInfoObj, scPerc, scTaxPerc );
}
//------------------------------------------------------------------------------
void TTestMMBillCalculator_Base::applyDiscountToCurrentPriceInfo( string name, TDiscountType discType, TDiscountWay mode, Currency percentage, int priority, Currency value, bool isTaxExclusive )
{
    applyDiscountToPriceInfo( *priceInfoObj, name, discType, mode, percentage, priority, value, isTaxExclusive );
}
//------------------------------------------------------------------------------
void TTestMMBillCalculator_Base::applySCDToCurrentPriceInfo( Currency percentage )
{
    applySCDToPriceInfo( *priceInfoObj, percentage );
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::applyPWDToCurrentPriceInfo( Currency percentage )
{
    applyPWDToPriceInfo( *priceInfoObj, percentage );
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::applyTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage, int priority )
{
    TTaxInfo tax;
    tax.Name = name;
    tax.Percentage = percentage;
    tax.Priority = priority;
    tax.TaxType = ttSale;

    priceInfo.TaxInfoList.push_back( tax );
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::applyRemovedTaxToPriceInfo( TPriceInfo &priceInfo1, string name, Currency percentage, int priority )
{
    TTaxInfo tax;
    tax.Name = name;
    tax.Percentage = percentage;
    tax.Priority = priority;
    tax.TaxType = ttSale;
    priceInfo1.RemovedTaxInfoList.push_back( tax );
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::applyLocalTaxToPriceInfo( TPriceInfo &priceInfo, string name, Currency percentage )
{
    TTaxInfo tax;
    tax.Name = name;
    tax.Percentage = percentage;
    tax.Priority = 1;
    tax.TaxType = ttLocal;

    priceInfo.TaxInfoList.push_back( tax );
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::applyServiceChargeToPriceInfo( TPriceInfo &priceInfo, Currency scPerc, Currency scTaxPerc )
{
    priceInfo.ServiceChargeInfo.Percentage = scPerc;
    priceInfo.ServiceChargeInfo.TaxPercentage = scTaxPerc;
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::applyRemovedServiceChargeToPriceInfo( TPriceInfo &priceInfo, Currency scPerc, Currency scTaxPerc )
{
    priceInfo.RemovedServiceChargeInfo.Percentage = scPerc;
    priceInfo.RemovedServiceChargeInfo.TaxPercentage = scTaxPerc;
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::applyDiscountToPriceInfo( TPriceInfo &priceInfo, string name, TDiscountType discType, TDiscountWay mode, Currency percentage, int priority, Currency value, bool isTaxExclusive )
{
    TDiscountInfo discInfo;
    discInfo.Name = name;
    discInfo.DiscountType = discType;
    discInfo.DiscountWay = mode;
    discInfo.TaxExclusiveDiscount = isTaxExclusive;
    discInfo.Percentage = percentage;
    discInfo.Value = value;
    discInfo.Priority = priority;

    priceInfo.DiscountInfoList.push_back( discInfo );
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::applySCDToPriceInfo( TPriceInfo &priceInfo, Currency percentage )
{
    TDiscountInfo discInfo;
    discInfo.Name = "SCD";
    discInfo.DiscountType = dtDiscount;
    discInfo.DiscountWay = dwPercentage;
    discInfo.Percentage = percentage;

    priceInfo.PriceTaxExempt = true;
    priceInfo.CalcDWTOnBasePrice = true;
    priceInfo.DiscountInfoList.push_back( discInfo );
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::applyPWDToPriceInfo( TPriceInfo &priceInfo, Currency percentage )
{
    TDiscountInfo discInfo;
    discInfo.Name = "PWD";
    discInfo.DiscountType = dtDiscount;
    discInfo.DiscountWay = dwPercentage;
    discInfo.Percentage = percentage;
    discInfo.RecalcPriceWithTaxAfterDiscount = true;
    discInfo.TaxExclusiveDiscount = true;
    priceInfo.CalcDWTOnBasePrice = true;
    priceInfo.DiscountInfoList.push_back( discInfo );
}
//------------------------------------------------------------------------------

void TTestMMBillCalculator_Base::calculateValuesForItemSet(
                                                    std::vector<TPriceInfo> &items,
                                                    Currency &discounts,
                                                    Currency &servCharge,
                                                    Currency &servChargeTax,
                                                    Currency &totalTax,
                                                    Currency &finalPrice,
                                                    Currency &discountWithOutTax,
                                                    Currency &taxOnDiscount)
{
    std::vector<TPriceInfo>::iterator it = items.begin();

    for( ; it != items.end() ; it++ )
    {
        execute( *it );
        discounts += getTotalDiscountForCurrentResult();
        servCharge += getServiceChargeForCurrentResult();
        servChargeTax += getServiceChargeTaxForCurrentResult();
        totalTax += getTotalTaxForCurrentResult();
        finalPrice += getFinalPriceForCurrentResult();
        discountWithOutTax+=getDiscountWithoutTaxForCurrent();
        taxOnDiscount +=getTaxOnDiscountForCurrent();
    }
}
//------------------------------------------------------------------------------
void TTestMMBillCalculator_Base::calculatePartialValuesForItemSet(
                                                    std::vector<TPriceInfo> &items,
                                                    Currency itemsTotal,
                                                    Currency partialPayment,
                                                    Currency &discounts,
                                                    Currency &servCharge,
                                                    Currency &servChargeTax,
                                                    Currency &totalTax,
                                                    Currency &finalPrice)
{
    std::vector<TPriceInfo>::iterator it = items.begin();
    Currency partialPaymentPercentage = (partialPayment * 100.0000) / itemsTotal;
    Currency currentItemTotal = 0;
    Currency partialPaymentAmountPerItem = 0;

    // creating partial payment
    TCalcContentInput partialPaymentInput;
    TCalcContentOutput partialPaymentOutput;

    for( ; it != items.end() ; it++ )
    {
        execute( *it );
        currentItemTotal = getFinalPriceForCurrentResult();
        partialPaymentAmountPerItem = (partialPaymentPercentage * currentItemTotal) / 100.0000;

        partialPaymentInput.InitializeFromBillCalcResult( &billCalcResponseObj->Result );
        partialPaymentOutput = billCalculator.CalculatePartialPaymentContent( &partialPaymentInput, partialPaymentAmountPerItem );

        discounts += partialPaymentOutput.PartialDiscount();
        servCharge += partialPaymentOutput.PartialServiceCharge();
        servChargeTax += partialPaymentOutput.PartialServiceChargeTax();
        totalTax += partialPaymentOutput.PartialTotalTax();
    }
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getTotalTaxForCurrentResult()
{
    return getTotalTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getDiscountXForCurrentResult( int X )
{
    return getDiscountX( billCalcResponseObj, X );
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getTotalDiscountForCurrentResult()
{
    return getTotalDiscount( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getFinalPriceForCurrentResult()
{
    return getFinalPrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getBasePriceForCurrentResult()
{
    return getBasePrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getServiceChargeForCurrentResult()
{
    return getServiceCharge( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getServiceChargeTaxForCurrentResult()
{
    return getServiceChargeTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getTaxContentForCurrentResult( UnicodeString taxName )
{
    return getTaxContent( billCalcResponseObj, taxName );
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getTotalTax( TBillCalcResponse* response )
{
    return response->Result.TotalTax;
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getDiscountX( TBillCalcResponse* response, int X )
{
    return response->Result.Discount[X].Content;
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getTotalDiscount( TBillCalcResponse* response )
{
    return response->Result.TotalDiscount;
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getFinalPrice( TBillCalcResponse* response )
{
    return response->Result.FinalPrice;
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getBasePrice( TBillCalcResponse* response )
{
    return response->Result.BasePrice;
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getServiceCharge( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.Value;
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getServiceChargeTax( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.TaxValue;
}
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getTaxContent( TBillCalcResponse* response, UnicodeString taxName )
{
    Currency taxContent = Curr0;
    TAX_RESULT_LIST::iterator it = response->Result.Tax.begin();

    for( ; it != response->Result.Tax.end(); it++ )
    {
        if( it->Name == taxName )
        {
            taxContent = it->Value;
            break;
        }
    }

    return taxContent;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getDiscountWithoutTaxForCurrent( )
{
    return getDiscountWithoutTax(billCalcResponseObj);
}

//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getTaxOnDiscountForCurrent(  )
{
    return getTaxOnDiscount(billCalcResponseObj);
}

//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getDiscountWithoutTax( TBillCalcResponse* response )
{
    return response->Result.DiscountWithoutTax;
}

//------------------------------------------------------------------------------
Currency TTestMMBillCalculator_Base::getTaxOnDiscount( TBillCalcResponse* response )
{
    return response->Result.TaxOnDiscount;
}

#pragma package(smart_init)
