

//---------------------------------------------------------------------------

#pragma hdrstop

#include "MMBillCalculator.h"
#include <math.h>
#include "Math.hpp"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

namespace BillCalculator
{
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// BillCalculator namespace methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Currency calculatePercentageValue( Currency LHS, Currency RHS )
{
    Currency percentage = (RHS * Curr100) / LHS;
    return percentage;
}

Currency calculateValueFromPercentage( Currency baseValue, Currency percentage)
{
    Currency value = (baseValue * percentage) / Curr100;
    return value;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TTaxInfo
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC SECTION
TTaxInfo::TTaxInfo()
{
    TaxType    = ttSale;
    Name       = "";
    Percentage = 0.0;
    TaxCode    = 0;
    Priority   = 0;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TDiscountInfo
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC SECTION
TDiscountInfo::TDiscountInfo()
{
	Name = "";
	Percentage = 0.0;
	Value = 0.0;

	Priority = 0;

    // Only apply if the Price is Tax Inclusive
	TaxExclusiveDiscount = false;

    // Set to TRUE is Person With Disability (PWD) discount is applied
    RecalcPriceWithTaxAfterDiscount = false;

	DiscountType = dtDiscount;
	DiscountWay  = dwPercentage;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TServiceChargeInfo
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC SECTION
TServiceChargeInfo::TServiceChargeInfo()
{
    Percentage    = 0.0;
    TaxPercentage = 0.0;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TPriceInfo
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC SECTION
TPriceInfo::TPriceInfo()
{
    Value = 0;
    MaxRetailPrice = 0;
    TaxInclusive = true;
    ServiceChargeInclusive = false;
    ServiceChargeTaxInclusive = false;

    // Only apply if the Price is Tax Inclusive
    TaxExclusiveServiceCharge = true; //Phillipines

    TaxBeforeDiscount = false;
    ServiceChargeBeforeDiscount = false;  //Phillipines

    MultiDiscountType = mdtSimple;
    MultiTaxType      = mttSimple;

    TaxInfoList.clear();
    DiscountInfoList.clear();
    RemovedTaxInfoList.clear();
    ServiceChargeInfo.Percentage  = 0;
    ServiceChargeInfo.TaxPercentage = 0;
    RemovedServiceChargeInfo.Percentage = 0;
    RemovedServiceChargeInfo.TaxPercentage=0;
    PriceTaxExempt = false;

    TaxExemptPriceExclusiveOfServiceCharge  = true;
    AllowServiceChargesOnTaxExemptPrice     = true;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TCalcContentInput
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC SECTION
TCalcContentInput::TCalcContentInput()
{
    Reset();
}

void TCalcContentInput::Reset()
{
    Value = 0;
    TaxPercentages.clear();
    DiscountPercentages.clear();
    ServiceChargePercentage = 0;
    ServiceChargeTaxPercentage = 0;
}

void TCalcContentInput::InitializeFromBillCalcResult( const TBillCalcResult* const billCalcResult )
{
    // start from scratch
    Reset();

    // final price from result
    Value = billCalcResult->FinalPrice;

    // taxes from result
    TAX_RESULT_LIST::const_iterator it_taxes = billCalcResult->Tax.begin();

    string taxContentName = "";
    Currency taxPercentage = 0;

    for( ; it_taxes != billCalcResult->Tax.end() ; it_taxes++ )
    {
        taxContentName = it_taxes->Name;
        taxPercentage = calculatePercentageValue( Value, it_taxes->Value );

        TaxPercentages[taxContentName] = taxPercentage;
    }

    // discounts from result
    DISCOUNT_RESULT_LIST::const_iterator it_discounts = billCalcResult->Discount.begin();

    string discountContentName = "";
    Currency discountPercentage = 0;

    for( ; it_discounts != billCalcResult->Discount.end() ; it_discounts++ )
    {
        discountContentName = it_discounts->Name;
        discountPercentage = calculatePercentageValue( Value, it_discounts->Content );
        DiscountPercentages[discountContentName] = discountPercentage;
    }

    // service charge from result
    ServiceChargePercentage = calculatePercentageValue( Value, billCalcResult->ServiceCharge.Value );

    // service charge taxe from result
    ServiceChargeTaxPercentage = calculatePercentageValue( Value, billCalcResult->ServiceCharge.TaxValue );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TCalcContentOutput
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC SECTION
Currency TCalcContentOutput::PartialTax()
{
    Currency total = 0;

    TAX_CONTENTS_LIST::iterator it_tax = TaxContents.begin();

    for( ; it_tax != TaxContents.end() ; it_tax++ )
    {
        total += it_tax->second;
    }

    return total;
}

Currency TCalcContentOutput::PartialTotalTax()
{
    Currency total = 0;

    total = PartialTax() + PartialServiceChargeTax();

    return total;
}

Currency TCalcContentOutput::PartialDiscount()
{
    Currency total = 0;

    DISCOUNT_CONTENTS_LIST::iterator it_disc = DiscountContents.begin();

    for( ; it_disc != DiscountContents.end() ; it_disc++ )
    {
        total += it_disc->second;
    }

    return -total;

}

Currency TCalcContentOutput::PartialValue()
{
    return ValueContent;
}

Currency TCalcContentOutput::PartialServiceCharge()
{
    return ServiceChargeContent;
}

Currency TCalcContentOutput::PartialServiceChargeTax()
{
    return ServiceChargeTaxContent;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TMMBillCalculator
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC SECTION
TMMBillCalculator::TMMBillCalculator()
{
}
//...........................................................................
TMMBillCalculator::~TMMBillCalculator()
{
}
//...........................................................................
TBillCalcResponse* TMMBillCalculator::Execute( const TPriceInfo* const inPriceInfo )
{
    Math::SetRoundMode( rmNearest );
    TBillCalcResponse* response = new TBillCalcResponse();

    DISCOUNT_INFO_LIST endDiscountList;
    TAX_INFO_LIST endTaxList;

    //calculate price incl
    CalcPriceIncl(inPriceInfo, endTaxList, endDiscountList, response);

    Currency priceIncl = response->Result.FinalPrice;


    if(inPriceInfo->RemovedTaxInfoList.size() > 0                   // one or more taxes have been removed
        || inPriceInfo->RemovedServiceChargeInfo.Percentage != 0 )  // service charge has been removed
    {
        ExecuteWithTaxRemoval(inPriceInfo, endTaxList, endDiscountList, response);
    }
    else
    {
        ExecuteWithoutTaxRemoval(inPriceInfo, endTaxList, endDiscountList, response);
    }

    Math::SetRoundMode( rmNearest );

    response->Result.PriceIncl = priceIncl;

    return response;
}
//...........................................................................

void TMMBillCalculator::ExecuteWithoutTaxRemoval( const TPriceInfo* const inPriceInfo,
                                                                TAX_INFO_LIST& endTaxList,
                                                                DISCOUNT_INFO_LIST& endDiscountList,
                                                                TBillCalcResponse* response)
{
    TPriceInfo *priceInfo = CreatePriceInfoWithPriceInfo( inPriceInfo );
    resetResponse( response );

    // seperating out final discounts and taxes from the regular ones
    seperateEndDiscountsFromRegularDiscounts( priceInfo, endDiscountList);
    seperateEndTaxesFromRegularTaxes( priceInfo, endTaxList);

	response->Result.BasePrice 		= getBasePrice( priceInfo );
	response->Result.TaxExemptPrice = getTaxExemptPrice( priceInfo );

    calcDiscount( priceInfo, &response->Result );

    if( priceInfo->TaxInfoList.size() > 0 )
        calcTax( priceInfo, &response->Result );

    calcServiceCharge( priceInfo, &response->Result);

    // Gross Price only includes the Tax content, not the Service Charge Tax Content
    response->Result.GrossPrice = calcGrossPrice(
                                    priceInfo,
                                    &response->Result );

    // Recalc TotalTax taking into account the Service Charge Tax
    response->Result.TotalTax   = recalcTotalTax( priceInfo->PriceTaxExempt,
                                                 &response->Result );

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    doFinalDiscount( priceInfo, &response->Result, endDiscountList );
    doFinalTax(      priceInfo, &response->Result, endTaxList, endDiscountList );

	response->Result.PriceTaxExempt = priceInfo->PriceTaxExempt;
	setDiscountGroupListResult( priceInfo, &response->Result );
    calculateTaxesForDiscounts( priceInfo, response );

    delete priceInfo;

}

void TMMBillCalculator::ExecuteWithTaxRemoval( const TPriceInfo* const inPriceInfo,
                                                             TAX_INFO_LIST& endTaxList,
                                                             DISCOUNT_INFO_LIST& endDiscountList,
                                                             TBillCalcResponse* response )
{
    TPriceInfo *priceInfo = CreatePriceInfoWithPriceInfo( inPriceInfo );
    resetResponse(response);

    // seperating out final discounts and taxes from the regular ones
    seperateEndDiscountsFromRegularDiscounts( priceInfo, endDiscountList );
    seperateEndTaxesFromRegularTaxes( priceInfo, endTaxList);

    //Calculate Base price
    response->Result.BasePrice = getBasePrice( priceInfo );

    //Remove taxes
    removeTaxes(priceInfo);

    //Remove service charges
    removeServiceCharges(priceInfo);

    //Calculate Taxes and service charge on the base price
    priceInfo->Value                        = response->Result.BasePrice;
    priceInfo->TaxInclusive                 = false;
    priceInfo->PriceTaxExempt               = false;
    priceInfo->ServiceChargeInclusive       = false;
    priceInfo->ServiceChargeTaxInclusive    = false;

    calcTax(priceInfo, &response->Result );
    calcActualServiceCharge(
                        priceInfo->Value,
                        priceInfo,
                        &response->Result );

    // Re calculate the value for the object to be re-structured
    Currency restructuredPriceValue = priceInfo->Value;

    restructuredPriceValue          += inPriceInfo->TaxInclusive
                                        ? response->Result.TotalTax
                                        : Curr0;

    // if the original item is service charge inclusive, then add the service charge value
    restructuredPriceValue          += inPriceInfo->ServiceChargeInclusive
                                        ? response->Result.ServiceCharge.Value
                                        : Curr0;

    // if the original item is service charge tax inclusive, then add the service charge value
    restructuredPriceValue          += inPriceInfo->ServiceChargeTaxInclusive
                                        ? response->Result.ServiceCharge.TaxValue
                                        : Curr0;

    // Re structure the price info object after tax removal
    TPriceInfo *taxRemovedPriceInfo                 = CreatePriceInfoWithPriceInfo( priceInfo );
    taxRemovedPriceInfo->Value                      = restructuredPriceValue;
    taxRemovedPriceInfo->TaxInclusive               = inPriceInfo->TaxInclusive;
    taxRemovedPriceInfo->ServiceChargeInclusive     = inPriceInfo->ServiceChargeInclusive;
    taxRemovedPriceInfo->ServiceChargeTaxInclusive  = inPriceInfo->ServiceChargeTaxInclusive;
    taxRemovedPriceInfo->PriceTaxExempt             = inPriceInfo->PriceTaxExempt;
    taxRemovedPriceInfo->CalcDWTOnBasePrice         = inPriceInfo->CalcDWTOnBasePrice;

    // Add discounts
    taxRemovedPriceInfo->DiscountInfoList.clear();

    DISCOUNT_INFO_LIST::const_iterator discountInfoIT;

    for( discountInfoIT  = inPriceInfo->DiscountInfoList.begin();
         discountInfoIT != inPriceInfo->DiscountInfoList.end(); discountInfoIT++ )
    {
        TDiscountInfo discountInfo;
        discountInfo.DiscountKey            = discountInfoIT->DiscountKey;
        discountInfo.Name                   = discountInfoIT->Name;
        discountInfo.Description            = discountInfoIT->Description;
        discountInfo.Percentage             = discountInfoIT->Percentage;
        discountInfo.Value                  = discountInfoIT->Value;
        discountInfo.Priority               = discountInfoIT->Priority;
        discountInfo.TaxExclusiveDiscount   = discountInfoIT->TaxExclusiveDiscount;
        discountInfo.DiscountType           = discountInfoIT->DiscountType;
        discountInfo.DiscountWay            = discountInfoIT->DiscountWay;
        discountInfo.RecalcPriceWithTaxAfterDiscount = discountInfoIT->RecalcPriceWithTaxAfterDiscount;
        discountInfo.DiscountGroupList      = discountInfoIT->DiscountGroupList;
        taxRemovedPriceInfo->DiscountInfoList.push_back( discountInfo );
    }
    TAX_INFO_LIST::const_iterator taxInfoIT;

    for( taxInfoIT  = inPriceInfo->TaxInfoList.begin();
         taxInfoIT != inPriceInfo->TaxInfoList.end(); taxInfoIT++ )
    {
       if(taxInfoIT->TaxType  == ttLocal)
        {
            TTaxInfo taxInfo;
            taxInfo.TaxType    = taxInfoIT->TaxType;
            taxInfo.Name       = taxInfoIT->Name;
            taxInfo.Percentage = taxInfoIT->Percentage;
            taxInfo.TaxCode    = taxInfoIT->TaxCode;
            taxInfo.Priority   = taxInfoIT->Priority;
            taxRemovedPriceInfo->TaxInfoList.push_back( taxInfo );
        }
    }

    resetResponse(response);
    ExecuteWithoutTaxRemoval(taxRemovedPriceInfo, endTaxList, endDiscountList,response);

    delete priceInfo;
    delete taxRemovedPriceInfo;

}


// Arpit
//Remove Taxes
//Story:1492
void TMMBillCalculator::removeTaxes(TPriceInfo* const inPriceInfo)
{
     if(inPriceInfo->TaxInclusive)
      {
            TAX_INFO_LIST::iterator taxInfo;
            TAX_INFO_LIST::iterator removedTaxInfo;
            for(removedTaxInfo = inPriceInfo->RemovedTaxInfoList.begin();
                removedTaxInfo != inPriceInfo->RemovedTaxInfoList.end();
                 removedTaxInfo++ )
            {
                for(taxInfo = inPriceInfo->TaxInfoList.begin();
                    taxInfo != inPriceInfo->TaxInfoList.end();
                    taxInfo++ )
                 {
                     if(removedTaxInfo->Name == taxInfo->Name)
                     {
                         taxInfo->Percentage = 0;
                     }
                 }
            }
       }
}

// set service charge percentages to zero, if they have been removed
void TMMBillCalculator::removeServiceCharges( TPriceInfo* const inPriceInfo )
{
    if( inPriceInfo->RemovedServiceChargeInfo.Percentage != 0 )
    {
        // when service charge is removed, service charge tax is also removed
        inPriceInfo->ServiceChargeInfo.Percentage = 0;
        inPriceInfo->ServiceChargeInfo.TaxPercentage = 0;
    }
}

//...........................................................................
TCalcContentOutput TMMBillCalculator::CalculatePartialPaymentContent( const TCalcContentInput* const inInputInfo, Currency partialPayment )
{
    TCalcContentOutput result;

    if( inInputInfo->Value > partialPayment )
    {
        result.ValueContent = partialPayment;

        // calculate taxes according to percentages
        TAX_CONTENTS_LIST::const_iterator it_tax = inInputInfo->TaxPercentages.begin();

        for( ; it_tax != inInputInfo->TaxPercentages.end() ; it_tax++ )
        {
            result.TaxContents[ it_tax->first ] = calculateValueFromPercentage( result.ValueContent, it_tax->second );
        }

        // calculate discounts according to percentages
        DISCOUNT_CONTENTS_LIST::const_iterator it_disc = inInputInfo->DiscountPercentages.begin();

        for( ; it_disc != inInputInfo->DiscountPercentages.end() ; it_disc++ )
        {
            result.DiscountContents[ it_disc->first ] = calculateValueFromPercentage( result.ValueContent, it_disc->second );
        }

        // service charge content according to percentage
        result.ServiceChargeContent = calculateValueFromPercentage( result.ValueContent, inInputInfo->ServiceChargePercentage );

        // service charge tax content according to percentage
        result.ServiceChargeTaxContent = calculateValueFromPercentage( result.ValueContent, inInputInfo->ServiceChargeTaxPercentage );
    }

    return result;
}
//...........................................................................

TPriceInfo* TMMBillCalculator::CreatePriceInfo()
{
    TPriceInfo *result = new TPriceInfo();

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    result->Value = 0.0;
    result->MaxRetailPrice = 0.0;
    result->TaxInclusive              = false;
    result->ServiceChargeInclusive    = false;
    result->ServiceChargeTaxInclusive = false;;

    result->TaxExclusiveServiceCharge   = true;
    result->TaxBeforeDiscount           = false;
    result->ServiceChargeBeforeDiscount = false;

    result->MultiDiscountType = mdtSimple;
    result->MultiTaxType      = mttSimple;

    result->PriceTaxExempt     = false;
    result->CalcDWTOnBasePrice = false;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Tax
    result->TaxInfoList.clear();

    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Discount
    result->DiscountInfoList.clear();

    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Service Charge
    result->ServiceChargeInfo.Percentage    = 0.0;
    result->ServiceChargeInfo.TaxPercentage = 0.0;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Special
    result->TaxExemptPriceExclusiveOfServiceCharge  = true;
    result->AllowServiceChargesOnTaxExemptPrice     = true;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//...........................................................................

TPriceInfo* TMMBillCalculator::CreatePriceInfoWithPriceInfo( const TPriceInfo* const inPriceInfo )
{
    TPriceInfo *result = new TPriceInfo();

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    result->Value       = inPriceInfo->Value;
    result->MaxRetailPrice              = inPriceInfo->MaxRetailPrice;
    result->TaxInclusive                = inPriceInfo->TaxInclusive;
    result->TaxExclusiveServiceCharge   = inPriceInfo->TaxExclusiveServiceCharge;
    result->TaxBeforeDiscount           = inPriceInfo->TaxBeforeDiscount;
    result->ServiceChargeBeforeDiscount = inPriceInfo->ServiceChargeBeforeDiscount;

    result->ServiceChargeInclusive      = inPriceInfo->ServiceChargeInclusive;
    result->ServiceChargeTaxInclusive   = inPriceInfo->ServiceChargeTaxInclusive;

    result->MultiDiscountType = inPriceInfo->MultiDiscountType;
    result->MultiTaxType      = inPriceInfo->MultiTaxType;

    result->PriceTaxExempt = inPriceInfo->PriceTaxExempt;
    result->CalcDWTOnBasePrice =    inPriceInfo->CalcDWTOnBasePrice;
    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Tax
    TAX_INFO_LIST::const_iterator taxInfoIT;

    for( taxInfoIT  = inPriceInfo->TaxInfoList.begin();
         taxInfoIT != inPriceInfo->TaxInfoList.end(); taxInfoIT++ )
    {
        TTaxInfo taxInfo;

        taxInfo.TaxType    = taxInfoIT->TaxType;
        taxInfo.Name       = taxInfoIT->Name;
        taxInfo.Percentage = taxInfoIT->Percentage;
        taxInfo.TaxCode    = taxInfoIT->TaxCode;
        taxInfo.Priority   = taxInfoIT->Priority;

        result->TaxInfoList.push_back( taxInfo );
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Removed Tax
    TAX_INFO_LIST::const_iterator removedTaxInfoIT;

    for( removedTaxInfoIT  = inPriceInfo->RemovedTaxInfoList.begin();
         removedTaxInfoIT != inPriceInfo->RemovedTaxInfoList.end(); removedTaxInfoIT++ )
    {
        TTaxInfo taxInfo;

        taxInfo.TaxType    = removedTaxInfoIT->TaxType;
        taxInfo.Name       = removedTaxInfoIT->Name;
        taxInfo.Percentage = removedTaxInfoIT->Percentage;
        taxInfo.TaxCode    = taxInfoIT->TaxCode;
        taxInfo.Priority   = removedTaxInfoIT->Priority;

        result->RemovedTaxInfoList.push_back( taxInfo );
    }


    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Discount
    DISCOUNT_INFO_LIST::const_iterator discountInfoIT;

    for( discountInfoIT  = inPriceInfo->DiscountInfoList.begin();
         discountInfoIT != inPriceInfo->DiscountInfoList.end(); discountInfoIT++ )
    {
        TDiscountInfo discountInfo;

        discountInfo.DiscountKey            = discountInfoIT->DiscountKey;
        discountInfo.Name                   = discountInfoIT->Name;
        discountInfo.Description            = discountInfoIT->Description;
        discountInfo.Percentage             = discountInfoIT->Percentage;
        discountInfo.Value                  = discountInfoIT->Value;
        discountInfo.Priority               = discountInfoIT->Priority;
        discountInfo.TaxExclusiveDiscount   = discountInfoIT->TaxExclusiveDiscount;
        discountInfo.DiscountType           = discountInfoIT->DiscountType;
        discountInfo.DiscountWay            = discountInfoIT->DiscountWay;
        discountInfo.RecalcPriceWithTaxAfterDiscount = discountInfoIT->RecalcPriceWithTaxAfterDiscount;
        discountInfo.DiscountGroupList      = discountInfoIT->DiscountGroupList;
        discountInfo.DiscountCode = discountInfoIT->DiscountCode;
        discountInfo.IsCloudDiscount = discountInfoIT->IsCloudDiscount;
        discountInfo.DailyUsageAllowedPerMember = discountInfoIT->DailyUsageAllowedPerMember;
        discountInfo.MembersOnly = discountInfoIT->MembersOnly;
        result->DiscountInfoList.push_back( discountInfo );
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Service Charge
    result->ServiceChargeInfo.Percentage    = inPriceInfo->ServiceChargeInfo.Percentage;
    result->ServiceChargeInfo.TaxPercentage = inPriceInfo->ServiceChargeInfo.TaxPercentage;
    result->RemovedServiceChargeInfo.Percentage    = inPriceInfo->RemovedServiceChargeInfo.Percentage;
    result->RemovedServiceChargeInfo.TaxPercentage = inPriceInfo->RemovedServiceChargeInfo.TaxPercentage;
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //::::::::::::::::::::::::::::::::::::::::::::::::::::
    // Special
    result->TaxExemptPriceExclusiveOfServiceCharge  = inPriceInfo->TaxExemptPriceExclusiveOfServiceCharge;
    result->AllowServiceChargesOnTaxExemptPrice     = inPriceInfo->AllowServiceChargesOnTaxExemptPrice;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    return result;
}
//...........................................................................

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED SECTION
void TMMBillCalculator::resetResponse( TBillCalcResponse* const outResponse )
{
    outResponse->Succesful   = true;
    outResponse->ErrorCode   = bcr_NO_ERROR;
    outResponse->Message     = "";
    outResponse->Description = "";

    resetResult( &outResponse->Result );
}
//...........................................................................
void TMMBillCalculator::resetResult( TBillCalcResult* const outResult )
{
	outResult->BasePrice        = Curr0;
	outResult->TaxExemptPrice   = Curr0;
    outResult->FinalPrice       = Curr0;
    outResult->TotalTax         = Curr0;
    outResult->TotalDiscount    = Curr0;
    outResult->DiscountWithoutTax   = Curr0;
    outResult->TaxOnDiscount        = Curr0;
    outResult->Discount.clear();
    outResult->Tax.clear();

    outResult->ServiceCharge.Percentage    = Curr0;
    outResult->ServiceCharge.Value         = Curr0;
    outResult->ServiceCharge.TaxPercentage = Curr0;
    outResult->ServiceCharge.TaxValue      = Curr0;
    outResult->PriceIncl            = Curr0;
}
//...........................................................................

Currency TMMBillCalculator::getTaxExemptPrice( const TPriceInfo* const inPriceInfo )
{
	return getTaxExemptPrice(
						inPriceInfo->Value,
						inPriceInfo );
}
//...........................................................................
Currency TMMBillCalculator::getTaxExemptPrice(
										const Currency inSalePrice,
										const TPriceInfo* inPriceInfo )
{
	TPriceInfo* priceInfo = CreatePriceInfoWithPriceInfo( inPriceInfo );
	priceInfo->Value 	  = inSalePrice;

	Currency taxExemptPrice = Curr0;
	Currency basePrice = Curr0;
	Currency serviceCharge = Curr0;
	Currency serviceChargeTax = Curr0;

	// calculate base price : will remove all components incl ( tax, sc, sctax )
	basePrice = getBasePrice( priceInfo );

	// calculate service charge, if its included in menu price
	if( inPriceInfo->ServiceChargeInclusive )
	{
		serviceCharge = calcContent(
								basePrice,
								priceInfo->ServiceChargeInfo.Percentage );
	}

	// add the components together to get tax exempt price
	taxExemptPrice = basePrice + serviceCharge + serviceChargeTax;

	delete priceInfo;
	return taxExemptPrice;
}
//...........................................................................

Currency TMMBillCalculator::getBasePrice( const TPriceInfo* const inPriceInfo )
{
    return getBasePrice(
              inPriceInfo->Value,
              inPriceInfo->TaxInfoList,
              inPriceInfo->MultiTaxType,
              inPriceInfo->TaxInclusive,
              inPriceInfo->ServiceChargeInclusive,
              inPriceInfo->ServiceChargeTaxInclusive,
              inPriceInfo->ServiceChargeInfo );
}
//.........................................................................
Currency TMMBillCalculator::getBasePrice(
                            const Currency inSalePrice,
                            const TPriceInfo* inPriceInfo )
{
    return getBasePrice(
              inSalePrice,
              inPriceInfo->TaxInfoList,
              inPriceInfo->MultiTaxType,
              inPriceInfo->TaxInclusive,
              inPriceInfo->ServiceChargeInclusive,
              inPriceInfo->ServiceChargeTaxInclusive,
              inPriceInfo->ServiceChargeInfo );
}
//.........................................................................
Currency TMMBillCalculator::getBasePrice(
                                Currency      inSalePrice,
                                TAX_INFO_LIST inTaxInfoList,
                                TMultiTaxType inMultiTaxType,
                                bool          inTaxInclusive,
                                bool          inServiceChargeIncl,
                                bool          inServiceChargeTxIncl,
                                TServiceChargeInfo inServiceChargeInfo )
{
    Currency basePrice = Curr0;

    // findout all the required variables
    Currency scRate   = inServiceChargeIncl   ? inServiceChargeInfo.Percentage / Curr100 : Curr0;
    Currency scTxRate = inServiceChargeTxIncl ? inServiceChargeInfo.TaxPercentage / Curr100 : Curr0;
    Currency txRate   = inTaxInclusive
                        ? getCombinedTaxRate(
                                    inTaxInfoList,
                                    inMultiTaxType)
                        : Curr0;
    Currency combinedScSctRate = scRate * ( Curr1 + scTxRate );

    /*
        use formula to calculate the base price
        Base = SalePrice / 1 + SC( 1 + SCT ) + TXRate
    */
    basePrice = inSalePrice/ ( Curr1 + combinedScSctRate + txRate );

    return basePrice;
}
//.........................................................................

Currency TMMBillCalculator::getCombinedTaxRate( TAX_INFO_LIST &inTaxInfoList, TMultiTaxType inMultiTaxType )
{
    if( inMultiTaxType == mttSimple )
    {
        return getCombinedTaxRateSimple( inTaxInfoList );
    }
    else
    {
        return getCombinedTaxRateCompound( inTaxInfoList );
    }
}
//.........................................................................

Currency TMMBillCalculator::getCombinedTaxRateSimple( TAX_INFO_LIST &inTaxInfoList )
{
    // create a price info obj with a value of 1 and a billcalc result
    TPriceInfo *priceInfoObj   = CreatePriceInfo();
    priceInfoObj->Value        = 1;
    priceInfoObj->TaxInfoList  = inTaxInfoList;
    priceInfoObj->TaxInclusive = false;

    TBillCalcResult result;
    resetResult( &result );

    result.BasePrice = priceInfoObj->Value;

    // use the existing calcTax method to calculate the tax amount.
    calcTax( priceInfoObj, &result );

    delete priceInfoObj;

    return result.TotalTax;
}
//.........................................................................

Currency TMMBillCalculator::getCombinedTaxRateCompound( TAX_INFO_LIST &inTaxInfoList )
{
    // create a price info obj with a value of 1 and a billcalc result
    TPriceInfo *priceInfoObj   = CreatePriceInfo();
    priceInfoObj->Value        = 1;
    priceInfoObj->MultiTaxType = mttCompound;
    priceInfoObj->TaxInfoList  = inTaxInfoList;
    priceInfoObj->TaxInclusive = false;

    TBillCalcResult result;
    resetResult( &result );

    result.BasePrice = priceInfoObj->Value;

    // use the existing calcTax method to calculate the tax amount.
    calcTax( priceInfoObj, &result );

    delete priceInfoObj;

    return result.TotalTax;
}
//.........................................................................

Currency TMMBillCalculator::getTaxInclusiveBasePrice(
                                Currency      inSalePrice,
                                TAX_INFO_LIST inTaxInfoList,
                                TMultiTaxType inMultiTaxType )
{
    Currency result = 0;

    //::::::::::::::::::::::::::::::::::::::::::::::::

    try
    {
        Currency price = inSalePrice;

        if( inMultiTaxType ==  mttSimple )
        {
            Currency taxPercentage = getTotalTaxPercentage(
                                            inMultiTaxType,
                                            inTaxInfoList );
            Currency tax = taxPercentage / Curr100;

            // Base Price = Sale Price / ( 1 + Tax )
            // Tax = Tax% / 100
            result = price / ( Curr1 + tax );
        }
        else // mttCompound
        {
            TAX_INFO_LIST::reverse_iterator tiIT;

            for( tiIT = inTaxInfoList.rbegin(); tiIT != inTaxInfoList.rend(); tiIT++ )
            {
				// We are removing the taxes from the price by using the division.
                Currency taxP       = tiIT->Percentage / Curr100;
                price               = price / (Curr1 + taxP) ;
            }

            result = price;
        }
    }
    catch( ... )
    {
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//.........................................................................
void TMMBillCalculator::reviewResult( Currency inSalePrice, TBillCalcResult* inResult )
{
    if( fabs( inResult->TotalDiscount ) > inSalePrice )
    {
        inResult->TotalDiscount = -inSalePrice;
        inResult->FinalPrice    = 0;
        inResult->TotalTax      = 0;
    }
}
//.........................................................................
// DISCOUNT
void TMMBillCalculator::calcDiscount(
							      TPriceInfo*      const inPriceInfo,
								  TBillCalcResult* const outResult,
                                  bool  inIgnoreSpecialDiscounts )
{
	DISCOUNT_GROUP_MAP discountGroupMap;

	groupDiscountsByPriority(inPriceInfo->DiscountInfoList, inPriceInfo->MultiDiscountType, discountGroupMap );

    // if all discounts have same priority, we calculate them as simple
    inPriceInfo->MultiDiscountType = ( discountGroupMap.size() == 1 ) ? mdtSimple : mdtCompound;

    // Dsp%[i]: Same priority discount percentage
    // Dp[j]:   Total discount by priority
    // D:       Total discount
    // Ps:      Sale Price. Tax Inclusive or Not Tax Inclusive
    // i = 0 -> N-1
    // j = 0 -> M-1
    // N = amount of discounts with the same priority
    // M = amount of priorities
	if( inPriceInfo->MultiDiscountType == mdtSimple ) // Simple Multiple Discount
	{
        // Simple Multiple Discount
        // Dp[j] = Ps * SUM( Dsp%[i] ) = SUM( Ps * Dspi%[i] )
        // D     = SUM( Dp[j] )
		calcSimpleDiscountWithMap( discountGroupMap, inPriceInfo, outResult );
	}
	else
	{
        // Compound Multiple Discount
        // Ppt: Partial price (same priority )
        // Dpt: Partial discount (same priority )
        // Ppt[0] = Ps
        // Dpt[j] = Sum(i)( Ppt[j] * Dsp%[i] )
        // Ppt[j+1] =  Ppt[j] - Dpt[j]
        // D = ( ( ( Ppt[j] - Dpt[j] ) - Dpt[j+1] ) - Dpt[j+2] ) - Dpt[j+M-1]
        calcCompoundDiscountWithMap( discountGroupMap, inPriceInfo, outResult );
	}
}
//.........................................................................
void TMMBillCalculator::seperateEndDiscountsFromRegularDiscounts( TPriceInfo* inPriceInfo, DISCOUNT_INFO_LIST& endDiscountList )
{
    populateEndDiscountList( inPriceInfo->DiscountInfoList, endDiscountList );
}
//.........................................................................
void TMMBillCalculator::populateEndDiscountList(
                            DISCOUNT_INFO_LIST& inDiscountInfoList,
                            DISCOUNT_INFO_LIST& outEndDiscountList )
{
    outEndDiscountList.clear();

    DISCOUNT_INFO_LIST::iterator discountIT;
    for( discountIT  = inDiscountInfoList.begin();
         discountIT != inDiscountInfoList.end(); discountIT++ )
    {
        if( discountIT->RecalcPriceWithTaxAfterDiscount )
        {
            outEndDiscountList.push_back( *discountIT );
            inDiscountInfoList.erase( discountIT );
            break;
        }
    }
}
//.........................................................................
void TMMBillCalculator::groupDiscountsByPriority(
							DISCOUNT_INFO_LIST  inDiscountInfoList,
                            TMultiDiscountType  inMultiDiscountType,
							DISCOUNT_GROUP_MAP& outGroupMap )
{
	DISCOUNT_INFO_LIST::const_iterator discountIT;
    DISCOUNT_INFO_LIST discountInfoList;

    if( inMultiDiscountType == mdtSimple ) // As if all discounts with the same priority
    {
        for( discountIT  = inDiscountInfoList.begin();
             discountIT != inDiscountInfoList.end(); discountIT++ )
        {
            discountInfoList.push_back( *discountIT );
        }

        outGroupMap[1] = discountInfoList;
    }
    else // mdtCompound
    {
        for( discountIT  = inDiscountInfoList.begin();
             discountIT != inDiscountInfoList.end(); discountIT++ )
        {
            TDiscountInfo discountInfo = *discountIT;

            if( outGroupMap.find( discountInfo.Priority ) == outGroupMap.end() )
            {
                discountInfoList.clear();
                discountInfoList.push_back( discountInfo );

                outGroupMap[discountInfo.Priority] = discountInfoList;
            }
            else
            {
                outGroupMap[discountInfo.Priority].push_back( discountInfo );
            }
        }
    }
}
//.........................................................................
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Simple Multiple Discount
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Dsp%[i]: Same priority discount percentage
// Dp[j]:   Total discount by priority
// D:       Total discount
// Ps:      Sale Price. Tax Inclusive or Not Tax Inclusive
// -
// i = 0 -> N-1
// j = 0 -> M-1
// N = amount of discounts with the same priority
// M = amount of priorities
// -
// Dp[j] = Ps * SUM( Dsp%[i] ) = SUM( Ps * Dspi%[i] )
// D     = SUM( Dp[j] )
// -
// D = outResult.TotalDiscount
void TMMBillCalculator::calcSimpleDiscountWithMap(
								  DISCOUNT_GROUP_MAP     inGroupMap,
							const TPriceInfo*      const inPriceInfo,
								  TBillCalcResult* const outResult )
{
    DISCOUNT_GROUP_MAP::const_iterator mapIT;

	Currency basePrice         = outResult->BasePrice;
	Currency taxExemptPrice	   = outResult->TaxExemptPrice;
    Currency salePrice         = inPriceInfo->Value; // Sale Price = menumate's Menu Price
    bool     taxInclusivePrice = inPriceInfo->TaxInclusive;
	bool     priceTaxExempt    = inPriceInfo->PriceTaxExempt;

    for( mapIT = inGroupMap.begin(); mapIT != inGroupMap.end(); mapIT++ )
    {
		DISCOUNT_INFO_LIST discountInfoList = mapIT->second;

        calcDiscountWithList(basePrice,	taxExemptPrice,	salePrice, discountInfoList, inPriceInfo, outResult );
    }
}
//.........................................................................
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Compound Multiple Discount
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Dsp%[i]: Same priority discount percentage
// Dp[j]:   Total discount by priority
// D:       Total discount
// Ps:      Sale Price. Tax Inclusive or Not Tax Inclusive
// -
// i = 0 -> N-1
// j = 0 -> M-1
// N = amount of discounts with the same priority
// M = amount of priorities
// Ppt: Partial price (same priority )
// Dpt: Partial discount (same priority )
// Pbpt: Partial base price (same priority )
// -
// Ppt[0] = Ps
// Dpt[j] = Sum(i)( Ppt[j] * Dsp%[i] )
// Ppt[j+1] =  Ppt[j] - Dpt[j]
// D = Dpt
//
void TMMBillCalculator::calcCompoundDiscountWithMap(
								  DISCOUNT_GROUP_MAP     inGroupMap,
							const TPriceInfo*      const inPriceInfo,
								  TBillCalcResult* const outResult )
{
    DISCOUNT_GROUP_MAP::const_iterator mapIT;

    //:::::::::::::::::::::::::::::::::::::::::::::::::::

	Currency basePrice         = outResult->BasePrice;
	Currency taxExemptPrice	   = outResult->TaxExemptPrice;
    Currency salePrice         = inPriceInfo->Value; // Sale Price = menumate's Menu Price
    bool     taxInclusivePrice = inPriceInfo->TaxInclusive;
    bool     priceTaxExempt    = inPriceInfo->PriceTaxExempt;

    //:::::::::::::::::::::::::::::::::::::::::::::::::::

    Currency Dpt  = 0;
    Currency Pbpt = basePrice;
	Currency Ppt  = salePrice;
	Currency Tep  = taxExemptPrice;

    //:::::::::::::::::::::::::::::::::::::::::::::::::::

    for( mapIT = inGroupMap.begin(); mapIT != inGroupMap.end(); mapIT++ )
    {
		DISCOUNT_INFO_LIST discountInfoList = mapIT->second;

        calcDiscountWithList(
			Pbpt,
			Tep,
            Ppt,
            discountInfoList,
            inPriceInfo,
            outResult );

        Dpt  = outResult->TotalDiscount; // positve: surcharge | negative: discount

        Ppt  = inPriceInfo->Value + Dpt;

		Pbpt = getBasePrice( Ppt, inPriceInfo );
		Tep	 = getTaxExemptPrice( Ppt, inPriceInfo );
    }
}
//.........................................................................
void TMMBillCalculator::calcDiscountWithList(
							const Currency inBasePrice,
							const Currency inTaxExemptPrice,
                            const Currency inSalePrice,
                            const DISCOUNT_INFO_LIST inDiscInfoList,
							const TPriceInfo*      const inPriceInfo,
                                  TBillCalcResult* const outResult )
{
    DISCOUNT_INFO_LIST::const_iterator discountIT;

    for( discountIT  = inDiscInfoList.begin();
         discountIT != inDiscInfoList.end(); discountIT++ )
    {
        TDiscountInfo discountInfo = *discountIT;

        calcEachDiscount(
                 inPriceInfo->MultiDiscountType,
                 &discountInfo,
				 inBasePrice,
				 inTaxExemptPrice,
                 inSalePrice,
                 inPriceInfo->TaxInclusive,
				 inPriceInfo->PriceTaxExempt,
                 outResult );
    }
}
//.........................................................................
bool TMMBillCalculator::calcEachDiscount(
                           const TMultiDiscountType   inMultDiscountType,
						   const TDiscountInfo* const inDiscountInfo,
						   const Currency inBasePrice,
						   const Currency inTaxExemptPrice,
                           const Currency inSalePrice,
                           const bool     inTaxInclusivePrice,
						   const bool     inPriceTaxExempt,
                                 TBillCalcResult* const outResult )
{
	if( inTaxInclusivePrice )
	{
		// Tax Inclusive Price
		return calcEachTIPDiscount(
                    inMultDiscountType,
					inDiscountInfo,
					inBasePrice,
					inTaxExemptPrice,
					inSalePrice,
					inPriceTaxExempt,
                    outResult );
	}
	else
	{
		// NOT Tax Inclusive Price
		return calcEachNTIPDiscount(
                    inMultDiscountType,
                    inDiscountInfo,
					inBasePrice,
					inTaxExemptPrice,
					inSalePrice,
                    outResult );
	}
}
//.........................................................................
// Calculate the Discount for a Tax Inclusive Price
bool TMMBillCalculator::calcEachTIPDiscount(
                           const TMultiDiscountType   inMultDiscountType,
						   const TDiscountInfo* const inDiscountInfo,
						   const Currency inBasePrice,
						   const Currency inTaxExemptPrice,
                           const Currency inSalePrice,
						   const bool     inPriceTaxExempt,
							     TBillCalcResult* const outResult )
{
	/*
		the below statements uses taxExemptPrice when price is tax exempt.
		however, this needs be confirmed. as the taxExempt price may or may not have service charge in it.
		when applying scd - we usually remove taxes. since service charge is not a tax we dont remove it from
		the price. This affects only when the price is inclusive service charge.
	*/

	Currency price = ( inDiscountInfo->TaxExclusiveDiscount || inPriceTaxExempt )
					? inTaxExemptPrice
					: inSalePrice;

	return calcEachActualDiscount(
                price,
                inMultDiscountType,
                inDiscountInfo,
                outResult );
}
//.........................................................................
// Calculate the Discount for a Not Tax Inclusive Price
bool TMMBillCalculator::calcEachNTIPDiscount(
                           const TMultiDiscountType     inMultDiscountType,
						   const TDiscountInfo* const   inDiscountInfo,
						   const Currency               inBasePrice,
						   const Currency				inTaxExemptPrice,
						   const Currency               inSalePrice,
								 TBillCalcResult* const outResult )
{
	/*
		the below statements uses taxExemptPrice when price is tax exempt.
		however, this needs be confirmed. as the taxExempt price may or may not have service charge in it.
		when applying scd - we usually remove taxes. since service charge is not a tax we dont remove it from
		the price. This affects only when the price is inclusive service charge.
	*/

	Currency price = inTaxExemptPrice;

	return calcEachActualDiscount(
                price,
                inMultDiscountType,
                inDiscountInfo,
                outResult );
}
//.........................................................................
bool TMMBillCalculator::calcEachActualDiscount(
                            const Currency               inActualPrice,
                            const TMultiDiscountType     inMultDiscountType,
						    const TDiscountInfo*   const inDiscountInfo,
                                  TBillCalcResult* const outResult )
{
    bool result = true;

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    Currency actualContent = calcActualDiscountContent(
                                inActualPrice,
                                inMultDiscountType,
                                inDiscountInfo,
                                outResult->TotalDiscount );

    if( inDiscountInfo->DiscountType == dtDiscount )
    {
		// Discount
        outResult->TotalDiscount -= actualContent;
    }
	else
    {
        // Surcharge
		outResult->TotalDiscount += actualContent;
    }

    Currency actualPercentage = calcActualDiscountPercentage( inActualPrice, actualContent );

    //:::::::::::::::::::::::::::::::::::::::::::::::::

	TDiscountResult discountResult;

	discountResult.DiscountKey            = inDiscountInfo->DiscountKey;
	discountResult.Name                   = inDiscountInfo->Name;
    discountResult.Description            = inDiscountInfo->Description;
	discountResult.Percentage             = actualPercentage;
	discountResult.Content                = actualContent;
	discountResult.Value                  = actualContent;
	discountResult.Priority               = inDiscountInfo->Priority;
    discountResult.TaxExclusiveDiscount   = inDiscountInfo->TaxExclusiveDiscount;
	discountResult.DiscountType           = inDiscountInfo->DiscountType;
	discountResult.DiscountWay            = inDiscountInfo->DiscountWay;
    discountResult.RecalcPriceWithTaxAfterDiscount = inDiscountInfo->RecalcPriceWithTaxAfterDiscount;
    discountResult.DiscountCode = inDiscountInfo->DiscountCode;
    discountResult.IsCloudDiscount = inDiscountInfo->IsCloudDiscount;
    discountResult.DailyUsageAllowedPerMember = inDiscountInfo->DailyUsageAllowedPerMember;
    discountResult.MembersOnly = inDiscountInfo->MembersOnly;
	outResult->Discount.push_back( discountResult );

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//.........................................................................
Currency TMMBillCalculator::calcActualDiscountContent(
                const Currency             inActualPrice,
                const TMultiDiscountType   inMultDiscountType,
                const TDiscountInfo* const inDiscountInfo,
                const Currency             inCumulativeDiscount )
{
    Currency result = 0;

    //::::::::::::::::::::::::::::::::::::::::::::::::

    // Negative inCumulativeDiscount => Discount
    // Positive inCumulativeDiscount => Surcharge
    Currency remainingPrice = ( inMultDiscountType == mdtCompound ) ?
                               inActualPrice :
                               inActualPrice + inCumulativeDiscount;

    Currency discountValue = inDiscountInfo->Value;

    if( inDiscountInfo->DiscountWay == dwSetPrice )
    {
        // If result is negative => the discount is going to be added as a surcharge
        result = remainingPrice - discountValue;
    }
    else
    {
        result = ( inDiscountInfo->DiscountWay == dwPercentage ) ?
                    calcContent( inActualPrice, inDiscountInfo->Percentage ) :
                    discountValue;

        // check whether we have encountered a discount which is higher than the remaining price,
        // if so make them equal so the final result will not be negative
        if( result > remainingPrice
            && inDiscountInfo->DiscountType != dtSurcharge )
        {
            result = remainingPrice;
        }
    }

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//.........................................................................
Currency TMMBillCalculator::calcActualDiscountPercentage(
            Currency inActualPrice,
            Currency inActualDiscount )
{
    Currency result = 0;

    //::::::::::::::::::::::::::::::::::::::::::::::::

    result = ( inActualPrice == 0 ) ?
                0 : ( fabs( inActualDiscount ) / inActualPrice ) * Curr100;

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//.........................................................................

// TAX CONTENT
void TMMBillCalculator::calcTax(
                            TPriceInfo* const inPriceInfo,
                            TBillCalcResult* const outResult )
{
	TAX_GROUP_MAP taxGroupMap;

	groupTaxesByPriority(
         inPriceInfo->TaxInfoList,
         inPriceInfo->MultiTaxType,
         taxGroupMap );

    // Tsp%[i]: Same priority tax percentage
    // Tp[j]:   Total tax by priority
    // T:       Total tax
    // Ps:      Sale Price. Tax Inclusive or Not Tax Inclusive
    // i = 0 -> N-1
    // j = 0 -> M-1
    // N = amount of discounts with the same priority
    // M = amount of priorities
	if( ( taxGroupMap.size() == 1 ) ||               // All discounts with the same priority
        ( inPriceInfo->MultiTaxType == mttSimple ) ) // Simple Multiple Discount
	{
        // Simple Multiple Tax
        // Tp[j] = Ps * SUM( Tsp%[i] ) = SUM( Ps * Tspi%[i] )
        // T     = SUM( Tp[j] )
		calcSimpleTaxWithMap( taxGroupMap, inPriceInfo, outResult );
	}
	else
	{
        // Compound Multiple Tax
        // Ppt: Partial price (same priority )
        // Tpt: Partial discount (same priority )
        // Ppt[0] = Ps
        // Tpt[j] = Sum(i)( Ppt[j] * Tsp%[i] )
        // Ppt[j+1] =  Ppt[j] - Tpt[j]
        // T = ( ( ( Ppt[j] - Tpt[j] ) - Tpt[j+1] ) - Tpt[j+2] ) - Tpt[j+M-1]
        calcCompoundTaxWithMap( taxGroupMap, inPriceInfo, outResult );
	}
}
//.........................................................................
void TMMBillCalculator::removeSalesTaxesForExemptPrice( TPriceInfo* inPriceInfo )
{
	if( inPriceInfo->PriceTaxExempt )
	{
		TAX_INFO_LIST::iterator taxIT;
		for( taxIT  = inPriceInfo->TaxInfoList.begin();
			 taxIT != inPriceInfo->TaxInfoList.end(); taxIT++ )
		{
			if( taxIT->TaxType == ttSale )
			{
				taxIT->Percentage = 0.0;
			}
		}

        if( inPriceInfo->ServiceChargeTaxInclusive )
        {
            inPriceInfo->ServiceChargeInfo.TaxPercentage = 0.0;
        }
	}
}
//.........................................................................
void TMMBillCalculator::seperateEndTaxesFromRegularTaxes( TPriceInfo* inPriceInfo, TAX_INFO_LIST& endTaxList )
{
    populateEndTaxList( inPriceInfo->TaxInfoList, endTaxList );
}
//.........................................................................
void TMMBillCalculator::populateEndTaxList(TAX_INFO_LIST& inTaxInfoList, TAX_INFO_LIST& outEndTaxList )
{
    outEndTaxList.clear();

	TAX_INFO_LIST::iterator taxIT;
	for( taxIT  = inTaxInfoList.begin();
		 taxIT != inTaxInfoList.end(); taxIT++ )
	{
		if( taxIT->TaxType == ttLocal || taxIT->TaxType == ttProfit)
		{
			outEndTaxList.push_back(*taxIT);
			inTaxInfoList.erase(taxIT);
			break;
		}
	}
}
//.........................................................................
void TMMBillCalculator::groupTaxesByPriority(
							TAX_INFO_LIST  inTaxInfoList,
                            TMultiTaxType  inMultiTaxType,
							TAX_GROUP_MAP& outGroupMap )
{
	TAX_INFO_LIST::const_iterator taxIT;
    TAX_INFO_LIST taxInfoList;

    if( inMultiTaxType == mttSimple ) // As if all taxes with the same priority
    {
        for( taxIT  = inTaxInfoList.begin();
             taxIT != inTaxInfoList.end(); taxIT++ )
        {
             taxInfoList.push_back( *taxIT );
        }

        outGroupMap[1] = taxInfoList;
    }
    else //  mttCompound
    {
        for( taxIT  = inTaxInfoList.begin();
             taxIT != inTaxInfoList.end(); taxIT++ )
        {
            TTaxInfo taxInfo = *taxIT;

            if( outGroupMap.find( taxInfo.Priority ) == outGroupMap.end() )
            {
                taxInfoList.clear();
                taxInfoList.push_back( taxInfo );

                outGroupMap[taxInfo.Priority] = taxInfoList;
            }
            else
            {
                outGroupMap[taxInfo.Priority].push_back( taxInfo );
            }
        }
    }
}
//.........................................................................
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Simple Multiple Tax
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Tsp%[i]: Same priority tax percentage
// Tp[j]:   Total tax by priority
// T:       Total tax
// Ps:      Sale Price. Tax Inclusive or Not Tax Inclusive
// -
// i = 0 -> N-1
// j = 0 -> M-1
// N = amount of discounts with the same priority
// M = amount of priorities
// -
// Tp[j] = Ps * SUM( Tsp%[i] ) = SUM( Ps * Tspi%[i] )
// T     = SUM( Tp[j] )
// -
// T = outResult.TotalTax
void TMMBillCalculator::calcSimpleTaxWithMap(
								  TAX_GROUP_MAP          inGroupMap,
							const TPriceInfo*      const inPriceInfo,
								  TBillCalcResult* const outResult )
{
    TAX_GROUP_MAP::const_iterator mapIT;

    Currency basePrice         = outResult->BasePrice;
    Currency salePrice         = inPriceInfo->Value; // Sale Price = menumate's Menu Price
    Currency maxRetailPrice = inPriceInfo->MaxRetailPrice;//MM-3663 Profit Tax..
    Currency totalDiscount = outResult->TotalDiscount;
    bool     taxInclusivePrice = inPriceInfo->TaxInclusive;
    bool     taxExemptPrice    = inPriceInfo->PriceTaxExempt;
    bool     taxBeforeDiscount = inPriceInfo->TaxBeforeDiscount;

    TServiceChargeSettings scSettings = getServiceChargeSettingsFromPriceInfo( inPriceInfo );

	if( !taxExemptPrice )
	{
		salePrice = taxBeforeDiscount ?
					salePrice : salePrice + outResult->TotalDiscount;

		// remove service charge or service charge tax, if included in price,
		salePrice = getPriceWithoutServiceChargeContents(
													salePrice,
													scSettings,
													inPriceInfo);
	}

    for( mapIT = inGroupMap.begin(); mapIT != inGroupMap.end(); mapIT++ )
    {
		TAX_INFO_LIST infoList = mapIT->second;

        calcTaxWithList(
            infoList,
            basePrice,
            salePrice,
            taxInclusivePrice,
            taxExemptPrice,
            taxBeforeDiscount,
            scSettings, // MM-788: Service Charge Inclusive
            outResult );
    }
}
//.........................................................................
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Compound Multiple Tax
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Tsp%[i]: Same priority tax percentage
// Tp[j]:   Total tax by priority
// T:       Total tax
// Ps:      Sale Price. Tax Inclusive or Not Tax Inclusive
// -
// i = 0 -> N-1
// j = 0 -> M-1
// N = amount of discounts with the same priority
// M = amount of priorities
// Ppt: Partial price (same priority )
// Tpt: Partial tax (same priority )
// Pbpt: Partial base price (same priority )
// -
// Ppt[0] = Ps
// Tpt[j] = Sum(i)( Ppt[j] * Tsp%[i] )
// Ppt[j+1] =  Ppt[j] - Tpt[j]
// T = Tpt
//
void TMMBillCalculator::calcCompoundTaxWithMap(
								  TAX_GROUP_MAP          inGroupMap,
							const TPriceInfo*      const inPriceInfo,
								  TBillCalcResult* const outResult )
{
    Currency basePrice         = outResult->BasePrice;
    Currency salePrice         = inPriceInfo->Value; // Sale Price = menumate's Menu Price
    bool     taxInclusivePrice = inPriceInfo->TaxInclusive;
    bool     taxExemptPrice    = inPriceInfo->PriceTaxExempt;
    bool     taxBeforeDiscount = inPriceInfo->TaxBeforeDiscount;

    TServiceChargeSettings scSettings = getServiceChargeSettingsFromPriceInfo( inPriceInfo );

	if( !taxExemptPrice )
	{
		salePrice = taxBeforeDiscount ?
					salePrice : salePrice + outResult->TotalDiscount;

		// remove service charge or service charge tax, if included in price,
		salePrice = getPriceWithoutServiceChargeContents(
													salePrice,
													scSettings,
													inPriceInfo);
	}


	if( !inPriceInfo->TaxInclusive )
    {
        calcNTIPCompoundTaxContent(
                            inGroupMap,
                            inPriceInfo,
                            basePrice,
                            salePrice,
                            taxInclusivePrice,
                            taxExemptPrice,
                            taxBeforeDiscount,
                            outResult,
                            scSettings);
    }
    else
    {
        calcTIPCompoundTaxContent(
                            inGroupMap,
                            inPriceInfo,
                            basePrice,
                            salePrice,
                            taxInclusivePrice,
                            taxExemptPrice,
                            taxBeforeDiscount,
                            outResult,
                            scSettings);
    }
}
//.........................................................................

void TMMBillCalculator::calcNTIPCompoundTaxContent(
                                    TAX_GROUP_MAP          inGroupMap,
							        const TPriceInfo*      const inPriceInfo,
                                    Currency basePrice,
                                    Currency salePrice,
                                    bool taxInclusivePrice,
                                    bool taxExemptPrice,
                                    bool taxBeforeDiscount,
								    TBillCalcResult* const outResult,
                                    TServiceChargeSettings &scSettings)
{
    Currency Tpt  = 0;
    Currency Pbpt = basePrice;
    Currency Ppt  = salePrice;
    Currency maxRetailPrice = inPriceInfo->MaxRetailPrice;
    Currency totalDiscount = outResult->TotalDiscount;

    TAX_GROUP_MAP::const_iterator mapIT;

    for( mapIT = inGroupMap.begin(); mapIT != inGroupMap.end(); mapIT++ )
    {
        TAX_INFO_LIST infoList = mapIT->second;

        calcTaxWithList(
            infoList,
            Pbpt,
            Ppt,
            taxInclusivePrice,
            taxExemptPrice,
            taxBeforeDiscount,
            scSettings, // MM-788: Service Charge Inclusive
            outResult );

        Tpt  = outResult->TotalTax;
        Ppt  = salePrice + Tpt;

        Pbpt = getBasePrice(
                  Ppt,
                  inPriceInfo->TaxInfoList,
                  inPriceInfo->MultiTaxType,
                  inPriceInfo->TaxInclusive,
                  inPriceInfo->ServiceChargeInclusive,
                  inPriceInfo->ServiceChargeTaxInclusive,
                  inPriceInfo->ServiceChargeInfo );
    }
}
//.........................................................................

void TMMBillCalculator::calcTIPCompoundTaxContent(
                                    TAX_GROUP_MAP          inGroupMap,
							        const TPriceInfo*      const inPriceInfo,
                                    Currency basePrice,
                                    Currency salePrice,
                                    bool taxInclusivePrice,
                                    bool taxExemptPrice,
                                    bool taxBeforeDiscount,
								    TBillCalcResult* const outResult,
                                    TServiceChargeSettings &scSettings)
{
    Currency Tpt  = 0;
    Currency Pbpt = basePrice;
    Currency Ppt  = salePrice;
    Currency maxRetailPrice = inPriceInfo->MaxRetailPrice;
    Currency totalDiscount = outResult->TotalDiscount;

    TAX_GROUP_MAP::const_reverse_iterator rmapIT;

    for( rmapIT = inGroupMap.rbegin(); rmapIT != inGroupMap.rend(); rmapIT++ )
    {
        TAX_INFO_LIST infoList = rmapIT->second;

        calcTaxWithList(
            infoList,
            Pbpt,
            Ppt,
            taxInclusivePrice,
            taxExemptPrice,
            taxBeforeDiscount,
            scSettings, // MM-788: Service Charge Inclusive
            outResult );

        Tpt  = outResult->TotalTax;
        Ppt  = salePrice - Tpt;

        Pbpt = getBasePrice(
                  Ppt,
                  inPriceInfo->TaxInfoList,
                  inPriceInfo->MultiTaxType,
                  inPriceInfo->TaxInclusive,
                  inPriceInfo->ServiceChargeInclusive,
                  inPriceInfo->ServiceChargeTaxInclusive,
                  inPriceInfo->ServiceChargeInfo );
    }
}

//.........................................................................

void TMMBillCalculator::calcTaxWithList(
								  TAX_INFO_LIST inInfoList,
							const Currency inBasePrice,
                            const Currency inSalePrice,
							const bool     inTaxInclusivePrice,
                            const bool     inTaxExemptPrice,
                            const bool     inTaxBeforeDiscount,
                            const TServiceChargeSettings inSCSettings,
                            TBillCalcResult* const outResult )
{
    TAX_INFO_LIST::const_iterator taxIT;

    TTaxInfo taxInfo;
    Currency totalTaxPercentage = 0;

    for( taxIT  = inInfoList.begin();
         taxIT != inInfoList.end(); taxIT++ )
    {
        taxInfo = *taxIT;

        totalTaxPercentage += taxInfo.Percentage;
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    taxInfo.Percentage = totalTaxPercentage;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    calcEachTax(
            &taxInfo,
            inBasePrice,
            inSalePrice,
            inTaxInclusivePrice,
            inTaxExemptPrice,
            inTaxBeforeDiscount,
            inSCSettings,
            outResult );
}
//.........................................................................
void TMMBillCalculator::calcEachTax(
						   const TTaxInfo* const inTaxInfo,
                           const Currency inBasePrice,
                           const Currency inSalePrice,
                           const bool     inTaxInclusivePrice,
                           const bool     inTaxExemptPrice,
                           const bool     inTaxBeforeDiscount,
                           const TServiceChargeSettings inSCSettings,
                                 TBillCalcResult* const outResult )
{
    if( inTaxInclusivePrice )
	{
		// Tax Inclusive Price
		calcEachTIPTax(
                    inTaxInfo,
                    inBasePrice,
                    inSalePrice,
                    inTaxExemptPrice,
                    inTaxBeforeDiscount,
                    inSCSettings,
                    outResult );
	}
	else
	{
		// NOT Tax Inclusive Price
		calcEachNTIPTax(
                    inTaxInfo,
                    inBasePrice,
                    inSalePrice,
                    inTaxExemptPrice,
                    inTaxBeforeDiscount,
                    inSCSettings,
                    outResult );
	}
}
//.........................................................................
// Calculate the Tax Content for a Tax Inclusive Price
void TMMBillCalculator::calcEachTIPTax(
						   const TTaxInfo* const  inTaxInfo,
						   const Currency         inBasePrice,
                           const Currency         inSalePrice,
                           const bool             inTaxExemptPrice,
                           const bool             inTaxBeforeDiscount,
                           const TServiceChargeSettings inSCSettings,
                           TBillCalcResult* const outResult )
{
    Currency price = inSalePrice;

    bool taxInclusivePrice = true;
	calcEachActualTaxContent( price, inTaxExemptPrice, taxInclusivePrice, inTaxInfo, outResult );
}
//.........................................................................
// Calculate the Tax Content for a Not Tax Inclusive Price
void TMMBillCalculator::calcEachNTIPTax(
						   const TTaxInfo*  const inTaxInfo,
                           const Currency         inBasePrice,
                           const Currency         inSalePrice,
                           const bool             inTaxExemptPrice,
                           const bool             inTaxBeforeDiscount,
                           const TServiceChargeSettings inSCSettings,
                           TBillCalcResult* const outResult )
{
    Currency price = inSalePrice;

    bool taxInclusivePrice = false;
	calcEachActualTaxContent( price, inTaxExemptPrice, taxInclusivePrice, inTaxInfo, outResult );
}
//.........................................................................
void TMMBillCalculator::calcEachActualTaxContent(
                           Currency         inActualPrice,
                           bool             inTaxExemptPrice,
                           bool             inTaxInclusivePrice,
						   const TTaxInfo*  const inTaxInfo,
						   TBillCalcResult* const outResult )
{
	TTaxResult taxResult;

    taxResult.TaxType    = inTaxInfo->TaxType;
    taxResult.Name       = inTaxInfo->Name;
    taxResult.Percentage = inTaxInfo->Percentage;
    taxResult.TaxCode    = inTaxInfo->TaxCode;
    taxResult.Priority   = inTaxInfo->Priority;

    if( inTaxExemptPrice )
    {
        taxResult.Value  = 0;
    }
    else
    {
        taxResult.Value  = inTaxInclusivePrice ?
                             calcTaxInclusiveContent( inActualPrice, inTaxInfo->Percentage ) :
                             calcContent(             inActualPrice, inTaxInfo->Percentage );
    }

    outResult->Tax.push_back( taxResult );
    outResult->TotalTax += taxResult.Value;
}

//.........................................................................
// SERVICE CHARGE
void TMMBillCalculator::calcServiceCharge(
                            const TPriceInfo* const inPriceInfo,
                             TBillCalcResult* const outResult )
{
	if( inPriceInfo->TaxInclusive )
	{
		// Tax Inclusive Price
		calcTIPServiceCharge( inPriceInfo, outResult );
	}
	else
	{
		// NOT Tax Inclusive Price
		calcNTIPServiceCharge( inPriceInfo, outResult );
	}
}
//.........................................................................
// Calculate the Service Charge Tax Content for a Tax Inclusive Price
void TMMBillCalculator::calcTIPServiceCharge(
                           const TPriceInfo* const inPriceInfo,
                            TBillCalcResult* const outResult )
{
    Currency price;

        // e.g. Senior Citizen Discount
    if( inPriceInfo->PriceTaxExempt )
	{
		// price = either Base Price or ( Base Price - Discount )
		price = inPriceInfo->ServiceChargeBeforeDiscount ?
					outResult->TaxExemptPrice :
					outResult->TaxExemptPrice + outResult->TotalDiscount;

		// if the price is service charge inclusive, then we need to get to a base price,
		// before calculating the service charge
		if( inPriceInfo->ServiceChargeInclusive )
		{
			TPriceInfo* priceInfo = CreatePriceInfoWithPriceInfo( inPriceInfo );
			priceInfo->Value	  = price;

			// though the price is tax exempt flaged, we still have tax percentages.
			// when execution reaches this point, the price only contains service charge.
			// before calling, getBasePrice, we need to remove any sales taxes so it will provide the correct figures.
			removeSalesTaxesForExemptPrice( priceInfo );

			price = getBasePrice(priceInfo);
		}
    }
    else
    {
         price = inPriceInfo->ServiceChargeBeforeDiscount ?
                    inPriceInfo->Value :
                    inPriceInfo->Value + outResult->TotalDiscount;


        if( inPriceInfo->TaxExclusiveServiceCharge )
        {
           // At this point the Total Tax ( without Service Charge Tax )
           // must be already calculated
           //      Currency taxContent = outResult->TotalTax ;
           //      price = price - taxContent;

           // we cannot use TotalTax here. Instead we need to calculate the content accordingly
           // this was done to prevent an error thrown when the TaxBeforeDiscount is set to true and ServiceCharge after discount it true
           // we need to remove the tax portion of the price that needs to be servicec charged

           price = getBasePrice(
                            price,
                            inPriceInfo);
        }
    }

    calcActualServiceCharge( price, inPriceInfo, outResult );
}
//.........................................................................
// Calculate the Service Charge Tax Content for a Not Tax Inclusive Price
void TMMBillCalculator::calcNTIPServiceCharge(
                           const TPriceInfo* const inPriceInfo,
                            TBillCalcResult* const outResult )
{
    Currency price = inPriceInfo->ServiceChargeBeforeDiscount ?
                            inPriceInfo->Value : inPriceInfo->Value + outResult->TotalDiscount;

    TPriceInfo* priceInfo   = CreatePriceInfoWithPriceInfo( inPriceInfo );
    priceInfo->Value        = price;

    Currency basePrice      = getBasePrice( priceInfo );

	price = inPriceInfo->TaxExclusiveServiceCharge
                ? basePrice
				: basePrice + outResult->TotalTax;

	calcActualServiceCharge( price, inPriceInfo, outResult );

	delete priceInfo;
}
//.........................................................................
void TMMBillCalculator::calcActualServiceCharge(
								 Currency               inPrice,
						   const TPriceInfo*      const inPriceInfo,
								 TBillCalcResult* const outResult )
{
    outResult->ServiceCharge.Value = calcContent(
                                        inPrice,
                                        inPriceInfo->ServiceChargeInfo.Percentage );

    outResult->ServiceCharge.Percentage = inPriceInfo->ServiceChargeInfo.Percentage;

    //.................................................

    outResult->ServiceCharge.TaxPercentage = inPriceInfo->ServiceChargeInfo.TaxPercentage;
    outResult->ServiceCharge.TaxValue      = calcContent(
                                                outResult->ServiceCharge.Value,
                                                inPriceInfo->ServiceChargeInfo.TaxPercentage );
}
//.........................................................................
Currency TMMBillCalculator::getServiceChargeContentInPrice(
                                                    Currency salePrice,
                                                    const TServiceChargeSettings    &inSCSettings,
                                                    const TPriceInfo*      const inPriceInfo)
{
    Currency basePrice = 0;

    basePrice = getBasePrice(
                        salePrice,
                        inPriceInfo->TaxInfoList,
                        inPriceInfo->MultiTaxType,  // Multi Tax Type
                        inPriceInfo->TaxInclusive,       // Tax Inclusive
                        inSCSettings.ServiceChargeInclusive,    // ServiceChargeIncl,
                        inSCSettings.ServiceChargeTaxInclusive, // ServiceChargeTxIncl,
                        inPriceInfo->ServiceChargeInfo );

    Currency serviceCharge = calcContent(
                                        basePrice,
                                        inSCSettings.ServiceChargePercentage );

    return serviceCharge;
}
//.........................................................................

Currency TMMBillCalculator::calcContent( Currency inTotal, Currency inPercentage )
{
    Currency content = inTotal * (inPercentage / Curr100);
    return content;
}
//.........................................................................
Currency TMMBillCalculator::calcTaxInclusiveContent(
                                Currency inInclusiveTaxPrice,
                                Currency inTaxPercentage )
{
	return calcInclusiveContent(
						inInclusiveTaxPrice,
						inTaxPercentage);

}
//.........................................................................
Currency TMMBillCalculator::calcInclusiveContent(
								Currency inInclusivePrice,
								Currency inInclusiveContentPercentage )
{
	Currency contentPercentage = inInclusiveContentPercentage / Curr100;
	Currency content = Curr0;

	content = ( inInclusivePrice / ( Curr1 + contentPercentage )) * contentPercentage ; // multiplying at the end seems to fix the rounding issue
	return content;
}
//.........................................................................
Currency TMMBillCalculator::getTotalTaxPercentage(
							 TMultiTaxType inMultiTaxType,
                             TAX_INFO_LIST inTaxInfo )
{
	Currency result = 0;

	//::::::::::::::::::::::::::::::::::::::::::

	switch( inMultiTaxType )
	{
		case mttSimple:
		{
			TAX_INFO_LIST::iterator listIT;

			for( listIT = inTaxInfo.begin(); listIT != inTaxInfo.end(); listIT++ )
			{
				result += listIT->Percentage;
			}
		} break;
	}

	//::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//.........................................................................
Currency TMMBillCalculator::calcPriceWithTax(
                                Currency      inBasePrice,
                                TAX_INFO_LIST inTaxInfoList )
{
	Currency   taxPercentage = 0;
	Currency taxContent = 0;

    TAX_INFO_LIST::iterator taxInfoIT;

    for( taxInfoIT  = inTaxInfoList.begin();
         taxInfoIT != inTaxInfoList.end();
         taxInfoIT++ )
    {
		taxContent += calcContent( inBasePrice, taxInfoIT->Percentage );
    }

    return inBasePrice + taxContent;
}
//.........................................................................
Currency TMMBillCalculator::calcGrossPrice(
                               const TPriceInfo* const inPriceInfo,
                                TBillCalcResult* const inCalcResult )
{
    if( inPriceInfo->TaxInclusive &&  !inPriceInfo->PriceTaxExempt )
    {
        return inPriceInfo->Value;  // Menu Price
    }
	else
    {
		return inPriceInfo->Value + inCalcResult->TotalTax; // Menu Price + Total Tax
    }
}
//.........................................................................

Currency TMMBillCalculator::calcFinalPrice(
                               const TPriceInfo* const inPriceInfo,
                                TBillCalcResult* const inCalcResult )
{
    inCalcResult->DiscountedPrice = inPriceInfo->Value + inCalcResult->TotalDiscount;

    if( inPriceInfo->TaxInclusive && !inPriceInfo->PriceTaxExempt )
    {
        return calcTIPFinalPrice( inPriceInfo, inCalcResult );
    }
	else
    {
		return calcNTIPFinalPrice( inPriceInfo, inCalcResult );
    }
}
//.........................................................................
Currency TMMBillCalculator::calcTIPFinalPrice(
                               const TPriceInfo* const inPriceInfo,
                                TBillCalcResult* const inCalcResult )
{
/*   Currency value =  inPriceInfo->TaxBeforeDiscount ?
                      inPriceInfo->Value
                     :inCalcResult->TaxExemptPrice + inCalcResult->TotalTax;


    Currency result =  value + */
    Currency result =  inPriceInfo->Value +
                       inCalcResult->TotalDiscount +
                       inCalcResult->ServiceCharge.Value +
                       inCalcResult->ServiceCharge.TaxValue;

    if( inPriceInfo->ServiceChargeInclusive)
    {
        result -= inCalcResult->ServiceCharge.Value;
    }

    if( inPriceInfo->ServiceChargeTaxInclusive)
    {
        result -= inCalcResult->ServiceCharge.TaxValue;
    }

    return result;
}
//.........................................................................
Currency TMMBillCalculator::calcNTIPFinalPrice( const TPriceInfo* const inPriceInfo, TBillCalcResult* const inCalcResult )
{
    Currency result = inCalcResult->TaxExemptPrice +
                      inCalcResult->TotalTax +
                      inCalcResult->TotalDiscount +
                      inCalcResult->ServiceCharge.Value;

    //todo: refactor to method
    if( inPriceInfo->ServiceChargeInclusive )
    {
        result -= inCalcResult->ServiceCharge.Value;
    }

    return result;
}
//.........................................................................

void TMMBillCalculator::doFinalDiscount(
                             TPriceInfo* const inPriceInfo,
                             TBillCalcResult* const outResult,
                             DISCOUNT_INFO_LIST& endDiscountList )
{
    // To apply PWD
    if( endDiscountList.size() > 0  )
    {
        outResult->FinalPrice = calcFinalPriceWithFinalDiscount(
                                         inPriceInfo,
                                         outResult,
                                         endDiscountList );
    }
    else // Normal Discounts
    {
        outResult->FinalPrice = calcFinalPrice(
                                   inPriceInfo,
                                   outResult );

        // Spliting Total Tax Content into all the partial Tax Contents
        splitTotalTaxContent( inPriceInfo, outResult );
    }
}
//.........................................................................
void TMMBillCalculator::doFinalTax(
                             TPriceInfo* const inPriceInfo,
                             TBillCalcResult* const outCalcResult,
                             TAX_INFO_LIST& endTaxList,
                             DISCOUNT_INFO_LIST& endDiscountList )
{
    if( endTaxList.size() > 0 )
    {
        TPriceInfo priceInfo;

        priceInfo.TaxInclusive      = inPriceInfo->TaxInclusive;
        priceInfo.MultiDiscountType = inPriceInfo->MultiDiscountType;
        priceInfo.MultiTaxType      = inPriceInfo->MultiTaxType;
        priceInfo.ServiceChargeInfo = inPriceInfo->ServiceChargeInfo;
        priceInfo.MaxRetailPrice = inPriceInfo->MaxRetailPrice;

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        priceInfo.Value = outCalcResult->DiscountedPrice;
        Currency basePrice = outCalcResult->BasePrice;
        Currency profitTaxPrice = (basePrice + outCalcResult->TotalDiscount) - priceInfo.MaxRetailPrice;

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        Currency newBasePrice = getBasePrice(
                                  priceInfo.Value,
                                  inPriceInfo->TaxInfoList,
                                  priceInfo.MultiTaxType,
                                  priceInfo.TaxInclusive || endDiscountList.size() > 0,
                                  priceInfo.ServiceChargeInclusive,
                                  priceInfo.ServiceChargeTaxInclusive,
                                  priceInfo.ServiceChargeInfo );

        //TODO: We need to loop around the tax info since now there can be multiple end taxes...
        Currency lastTaxPercentage = inPriceInfo->PriceTaxExempt ? 0.00 : endTaxList.begin()->Percentage;
        Currency lastTaxContent    = endTaxList.begin()->TaxType == ttProfit ?
                                                 ((profitTaxPrice < 0.00) ? 0.00 :
                                                 calcContent(profitTaxPrice, lastTaxPercentage)):
                                                 calcContent(newBasePrice, lastTaxPercentage);

        outCalcResult->ProfitTax = lastTaxContent;

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        if( endDiscountList.size() == 0)  // NO End discounts applied e.g. PWD Discounts
        {
            outCalcResult->FinalPrice  = calcFinalPrice( inPriceInfo, outCalcResult );
            outCalcResult->FinalPrice += lastTaxContent;
        }
        else  // End discounts applied e.g. PWD Discounts
        {
            outCalcResult->FinalPrice += lastTaxContent;
        }

        outCalcResult->TotalTax += lastTaxContent;

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        // place final tax in the outResult
        const TTaxInfo taxInfo = *endTaxList.begin();

        putFinalTaxInTheResult(&taxInfo, lastTaxContent, outCalcResult );
    }
}
//.........................................................................

Currency TMMBillCalculator::calcFinalPriceWithFinalDiscount(
                                        TPriceInfo* const inPriceInfo,
                                        TBillCalcResult*  const inCalcResult,
                                        const DISCOUNT_INFO_LIST inEndDiscountList )
{
	if( inEndDiscountList.size() > 0 )
	{
		// price after appllying all discounts exept the final
		Currency discountedPrice = inPriceInfo->Value + inCalcResult->TotalDiscount;

		TPriceInfo* priceInfo  	 = CreatePriceInfoWithPriceInfo(inPriceInfo);
		priceInfo->Value         = discountedPrice;

		// pwd is always applied to tax exempt price
		Currency taxExemptPrice = getTaxExemptPrice( priceInfo );

		// calculate pwd discount content
        Currency lastDiscountPercentage 	= inEndDiscountList.begin()->Percentage;
		Currency lastDiscountContent    	= calcContent(
												taxExemptPrice,
												lastDiscountPercentage );

		// different prices, after applying pwd
		Currency discountedTaxExemptPrice	= taxExemptPrice - lastDiscountContent;
		Currency discountedInclServiceCharge	= priceInfo->ServiceChargeInclusive
												? calcInclusiveContent(
														discountedTaxExemptPrice,
														priceInfo->ServiceChargeInfo.Percentage )
												: 0.0;
		Currency discountedBasePrice		= discountedTaxExemptPrice - discountedInclServiceCharge;

		// calculate the tax amount after applying pwd
		Currency finalTaxContent = totalTaxContent(
                                      discountedBasePrice,
                                      priceInfo->MultiTaxType,
                                      priceInfo->TaxInfoList );

        inCalcResult->TotalDiscount -= lastDiscountContent;
        inCalcResult->TotalTax       = finalTaxContent;

		// discounted price without service charge, will be used to calculate local taxes
        inCalcResult->DiscountedPrice = discountedBasePrice + finalTaxContent;

        // re-calculate the service charge on the new price after the end discount
		priceInfo->Value = inCalcResult->DiscountedPrice;
		priceInfo->DiscountInfoList.clear(); 				// no discounts needed here as we already applied the discounts in above step
		priceInfo->ServiceChargeBeforeDiscount = true;
		priceInfo->TaxInclusive 			   = true; 		// since we have put the tax back on the discounted price
		priceInfo->ServiceChargeInclusive 	   = false; 	// we are trying to calculate service charge on a price inclusive of tax
		priceInfo->ServiceChargeTaxInclusive   = false;

        calcTIPServiceCharge( priceInfo, inCalcResult ); 	// this will overwrite the service charge

		// adding the service charge tax to total tax amount
        inCalcResult->TotalTax = recalcTotalTax( priceInfo->PriceTaxExempt, inCalcResult );

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        // since we applied the final discount, re- calculate the tax contents
        reCalculateTaxContentsOnBasePrice(
                                    discountedBasePrice,
                                    &priceInfo->TaxInfoList,
									priceInfo->MultiTaxType,
                                    inCalcResult );

        // place final discount in the outResult
        const TDiscountInfo pwdDiscountInfo = *inEndDiscountList.begin();
        putFinalDiscountInTheResult(
                            &pwdDiscountInfo,
                            lastDiscountContent,
                            inCalcResult);

        // calculations with final discount is finished. so put the discount back to where it was
        inPriceInfo->DiscountInfoList.push_back( *inEndDiscountList.begin() );

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        return inCalcResult->DiscountedPrice
                + inCalcResult->ServiceCharge.Value
                + inCalcResult->ServiceCharge.TaxValue;
    }
    else return 0.0;
}
//.........................................................................

void TMMBillCalculator::putFinalDiscountInTheResult(
                                const TDiscountInfo* finalDiscountInfo,
                                Currency finalDiscountContent,
                                TBillCalcResult* outCalcResult )
{
    TDiscountResult finalDiscResult;
    finalDiscResult.DiscountKey   = finalDiscountInfo->DiscountKey;
    finalDiscResult.Name          = finalDiscountInfo->Name;
    finalDiscResult.Description   = finalDiscountInfo->Description;
    finalDiscResult.Percentage    = finalDiscountInfo->Percentage;
    finalDiscResult.Value         = finalDiscountContent;
    finalDiscResult.Content       = finalDiscountContent;
    finalDiscResult.Priority      = finalDiscountInfo->Priority;
    finalDiscResult.TaxExclusiveDiscount              = finalDiscountInfo->TaxExclusiveDiscount;
    finalDiscResult.RecalcPriceWithTaxAfterDiscount   = finalDiscountInfo->RecalcPriceWithTaxAfterDiscount;
    finalDiscResult.DiscountType  = finalDiscountInfo->DiscountType;
    finalDiscResult.DiscountWay   = finalDiscountInfo->DiscountWay;
    finalDiscResult.DiscountCode = finalDiscountInfo->DiscountCode;
    finalDiscResult.IsCloudDiscount = finalDiscountInfo->IsCloudDiscount;
    finalDiscResult.DailyUsageAllowedPerMember = finalDiscountInfo->DailyUsageAllowedPerMember;
    finalDiscResult.MembersOnly = finalDiscountInfo->MembersOnly;
    outCalcResult->Discount.push_back( finalDiscResult );
}
//.........................................................................
void TMMBillCalculator::putFinalTaxInTheResult(
                                const TTaxInfo* finalTaxInfo,
                                Currency finalTaxContent,
                                TBillCalcResult* outCalcResult )
{
    TTaxResult finalTaxResult;

    finalTaxResult.TaxType    = finalTaxInfo->TaxType;
    finalTaxResult.Name       = finalTaxInfo->Name;
    finalTaxResult.Percentage = finalTaxInfo->Percentage;
    finalTaxResult.Priority   = finalTaxInfo->Priority;
    finalTaxResult.TaxCode    = finalTaxInfo->TaxCode;
    finalTaxResult.Value      = finalTaxContent;

    outCalcResult->Tax.push_back( finalTaxResult );
}
//.........................................................................

Currency TMMBillCalculator::totalTaxContent(
                                 Currency      inPrice,
                                 TMultiTaxType inMultiTaxType,
                                 const TAX_INFO_LIST inTaxInfoList )
{
    Currency result = 0;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::

    TAX_INFO_LIST::const_iterator tiIT;

    if( inMultiTaxType ==  mttSimple )
    {
        Currency totalPercentage = 0.0;

        for( tiIT = inTaxInfoList.begin(); tiIT != inTaxInfoList.end(); tiIT++ )
        {
            totalPercentage += tiIT->Percentage;
        }

        result = inPrice * ( totalPercentage / Curr100 );
    }
    else // mttCompound
    {
       Currency totalContent = 0.0;
       Currency price        = inPrice;
       Currency currentTaxContent = 0.0;

        for( tiIT = inTaxInfoList.begin(); tiIT != inTaxInfoList.end(); tiIT++ )
        {
            currentTaxContent = price  * ( tiIT->Percentage / Curr100 );

            totalContent += currentTaxContent;
            price += currentTaxContent;
        }

        result = totalContent;
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}

//.........................................................................

Currency TMMBillCalculator::recalcTotalTax(
                              bool  inFinalPriceTaxExempt,
                              const TBillCalcResult* const inCalcResult )
{
    Currency result = inFinalPriceTaxExempt ?
                        inCalcResult->ServiceCharge.TaxValue :
                        inCalcResult->ServiceCharge.TaxValue + inCalcResult->TotalTax;
                        
    return result;
}
//.........................................................................

void TMMBillCalculator::splitTotalTaxContent(
                            const TPriceInfo* const inPriceInfo,
                            TBillCalcResult*        outResult )
{
    Currency price              = 0.0;
    Currency inSalePrice        = inPriceInfo->Value;
    Currency basePrice          = 0.0;
    TMultiTaxType multiTaxType  = inPriceInfo->MultiTaxType;
    TAX_INFO_LIST inInfoList    = inPriceInfo->TaxInfoList;
    bool inTaxBeforeDiscount    = inPriceInfo->TaxBeforeDiscount;
    bool inTaxInclusivePrice    = inPriceInfo->TaxInclusive;
    TAX_INFO_LIST::const_iterator taxIT;
    TTaxInfo taxInfo;

    TTaxResult taxResult;

    if( inInfoList.size() > 1  && !inPriceInfo->PriceTaxExempt )    // ignoring SCD discounted items, as the taxes were set to zero in prior point
    {
        outResult->Tax.clear();

        price = inTaxBeforeDiscount
                    ? inSalePrice
                    : outResult->DiscountedPrice;

        // calculating the base price, considering service charge is inclusive
        TPriceInfo* priceInfo = CreatePriceInfoWithPriceInfo( inPriceInfo );
        priceInfo->Value = price;

        basePrice = getBasePrice(priceInfo);

        reCalculateTaxContentsOnBasePrice(  basePrice,
                                            &inPriceInfo->TaxInfoList,
                                            inPriceInfo->MultiTaxType,
											outResult );

		delete priceInfo;
    }
}

//.........................................................................
void TMMBillCalculator::reCalculateTaxContentsOnBasePrice(
                            Currency inBasePrice,
                            const TAX_INFO_LIST* const inInfoList,
                            TMultiTaxType multiTaxType,
                            TBillCalcResult*        outResult )
{
    TAX_INFO_LIST::const_iterator taxIT;
    TTaxInfo taxInfo;

    TTaxResult taxResult;

    outResult->Tax.clear();

    Currency taxContent = 0;

    for( taxIT  = inInfoList->begin();
         taxIT != inInfoList->end(); taxIT++ )
    {
        taxInfo = *taxIT;

        if( multiTaxType == mttSimple )
            taxContent = calcContent( inBasePrice, taxInfo.Percentage );
        else
            taxContent = calcContent( inBasePrice + taxContent, taxInfo.Percentage );

        TTaxResult taxResult;

        taxResult.Name       = taxInfo.Name;
        taxResult.Percentage = taxInfo.Percentage;
        taxResult.Value      = taxContent;
        taxResult.Priority   = taxInfo.Priority;
        taxResult.TaxCode    = taxInfo.TaxCode;
        taxResult.TaxType    = ttSale;

        outResult->Tax.push_back( taxResult );
    }
}

//.........................................................................
void TMMBillCalculator::setDiscountGroupListResult(
							const TPriceInfo*      const inPriceInfo,
								  TBillCalcResult*       outResult )
{
    for( int i = 0; i < inPriceInfo->DiscountInfoList.size(); i++ )
    {
        outResult->Discount[i].DiscountGroupList.clear();

        for( int j = 0; j < inPriceInfo->DiscountInfoList[i].DiscountGroupList.size(); j++ )
        {
            outResult->Discount[i].DiscountGroupList.push_back( inPriceInfo->DiscountInfoList[i].DiscountGroupList[j] );
        }
    }
}
//.........................................................................

void TMMBillCalculator::calculateTaxesForDiscounts(
							const TPriceInfo*      const inPriceInfo,
								  TBillCalcResponse*       outResponse )
{
	if(inPriceInfo->TaxBeforeDiscount || inPriceInfo->CalcDWTOnBasePrice )
	{
	  outResponse->Result.DiscountWithoutTax = outResponse->Result.TotalDiscount;

	}
	else if(inPriceInfo->Value > 0)
	{
	   outResponse->Result.DiscountWithoutTax = (outResponse->Result.TotalDiscount *outResponse->Result.BasePrice)/inPriceInfo->Value;
	}
	outResponse->Result.TaxOnDiscount =  outResponse->Result.TotalDiscount - outResponse->Result.DiscountWithoutTax;
}
//.........................................................................

//.........................................................................
// Utility methods
//.........................................................................

Currency TMMBillCalculator::getPriceWithoutServiceChargeContents(
													   Currency	salePrice,
													   TServiceChargeSettings inSCSettings,
													   const TPriceInfo* const inPriceInfo)
{
	Currency result 			= salePrice;
	Currency inclSCContent 		= 0.0;
	Currency inclSCTaxContent 	= 0.0;

	if( inSCSettings.ServiceChargeInclusive)
	{
		inclSCContent = getServiceChargeContentInPrice(
													result,
													inSCSettings,
													inPriceInfo);

		result 		  = result - inclSCContent;
	}

	if( inSCSettings.ServiceChargeTaxInclusive )
	{
		inclSCTaxContent = calcContent( inclSCContent, inSCSettings.ServiceChargeTaxPercentage );
		result			 = result - inclSCTaxContent;
	}

	return result;
}

TServiceChargeSettings TMMBillCalculator::getServiceChargeSettingsFromPriceInfo( const TPriceInfo* const inPriceInfo )
{
    TServiceChargeSettings scSettings;
    scSettings.ServiceChargeInclusive     = inPriceInfo->ServiceChargeInclusive;
    scSettings.ServiceChargeTaxInclusive  = inPriceInfo->ServiceChargeTaxInclusive;
    scSettings.ServiceChargePercentage    = inPriceInfo->ServiceChargeInfo.Percentage;
    scSettings.ServiceChargeTaxPercentage = inPriceInfo->ServiceChargeInfo.TaxPercentage;

    return scSettings;
}

//-----------------------------------------------------------------------------------------------------------
void TMMBillCalculator::CalcPriceIncl( const TPriceInfo* const inPriceInfo,
                                                             TAX_INFO_LIST& endTaxList,
                                                             DISCOUNT_INFO_LIST& endDiscountList,
                                                             TBillCalcResponse* response )
{
    TPriceInfo *priceInfo                 = CreatePriceInfoWithPriceInfo( inPriceInfo );
    // clear discounts
    priceInfo->DiscountInfoList.clear();
    resetResponse(response);
    ExecuteWithoutTaxRemoval(priceInfo, endTaxList, endDiscountList,response);
    delete priceInfo;
}

} // namespace BillCalculator