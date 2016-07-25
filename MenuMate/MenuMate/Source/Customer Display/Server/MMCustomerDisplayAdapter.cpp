//---------------------------------------------------------------------------

#pragma hdrstop
#include "MMCustomerDisplayAdapter.h"
#include <Classes.hpp>
#include <math.h>
#include "SeatOrders.h"
#include "ItemComplete.h"
#include "ItemCompleteSub.h"
#include "ContainerOrders.h"
#include "rounding.h"
#include "GlobalSettings.h"
//#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------


#pragma package(smart_init)

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TMMCustomerDisplayAdapter
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//:::::::::::::
//  PUBLIC
//:::::::::::::

TMMCustomerDisplayAdapter::TMMCustomerDisplayAdapter()
{
}
//...........................................................................
TMMCustomerDisplayAdapter::~TMMCustomerDisplayAdapter()
{
}
//...........................................................................

TiXmlDocument* TMMCustomerDisplayAdapter::CreateOrderXML(
                   std::vector <TSeatOrders*> inPOSOrder,
                  TCustomerDisp inChangeStr )
{
    xml_order_doc = createXMLMenuDoc();

    addMainElements();

    setNodeAttr( root_elem, "handle", createOrderHandle( inPOSOrder ) );
    if(TGlobalSettings::Instance().ShowCurrencySymbol) // MM-2827
    {
      setNodeAttr( currencysym_elem, "symbol", "true");
    }
    else
    {
       setNodeAttr( currencysym_elem, "symbol", "false" );
    }
    if(TGlobalSettings::Instance().SetTextFontSizeOnCustomerDisplay)
    {
       setNodeAttr( textFontSize, "fontsize", getCustomerFontSize());
    }
    else
    {
       setNodeAttr( textFontSize, "fontsize", "");
    }
    populateOrderXmlDoc( inPOSOrder, inChangeStr );

    //:::::::::::::::::::::::::::::::::::::::::::::::

    return xml_order_doc;
}
//...........................................................................

TiXmlDocument* TMMCustomerDisplayAdapter::CreateOrderPaidXML(
                   Currency inLastTotal,
                   Currency inLastChange,
                   Currency inTotalServiceCharge,
                   Currency inTotalTax )
{
    xml_order_doc = createXMLMenuDoc();

    addMainElements();

    setNodeAttr( root_elem, "handle", createDefaultOrderHandle() );

    try
    {
        populateOrderPaidXmlDoc(
            inLastTotal,
            inLastChange,
            inTotalServiceCharge,
            inTotalTax );
    }
    catch( ... )
    {
    }
    //:::::::::::::::::::::::::::::::::::::::::::::::

    return xml_order_doc;
}
//...........................................................................

//:::::::::::::
//  PRIVATE
//:::::::::::::

TiXmlDocument* TMMCustomerDisplayAdapter::createXMLMenuDoc()
{
    TiXmlDocument* result = new TiXmlDocument();

    //::::::::::::::::::::::::::::::

    // add declaration
	TiXmlDeclaration *decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	result->LinkEndChild( decl );

    //::::::::::::::::::::::::::::::

    return result;
}
//...........................................................................

UnicodeString TMMCustomerDisplayAdapter::createDefaultOrderHandle()
{
    return "";
}
//...........................................................................

UnicodeString TMMCustomerDisplayAdapter::createOrderHandle( std::vector <TSeatOrders*> inPOSOrder )
{
    return "";
}
//...........................................................................

void TMMCustomerDisplayAdapter::addMainElements()
{
    addElement( ( TiXmlElement* )xml_order_doc, "POSOrder",  root_elem );
    addElement( root_elem, "CurrencySymbol", currencysym_elem );
    addElement( root_elem, "TextFontSize", textFontSize );
    addElement( root_elem, "ServingCourses", scourses_elem );
    addElement( root_elem, "Totals",         totals_elem );
    addElement( root_elem, "LoyaltyUser",         loyalty_elem );
}
//---------------------------------------------------------------------------

void TMMCustomerDisplayAdapter::populateOrderXmlDoc(
                                   std::vector <TSeatOrders*> inPOSOrder,
                                    TCustomerDisp  inChangeStr )
{
    total_scharge  = Currency( 0 );

        total_discount = Currency( 0 );
        total          = Currency( 0 );
        total_scharge  = Currency( 0 );
        total_tax      = Currency( 0 );

    for( UINT iSeat = 0; iSeat < inPOSOrder.size(); iSeat++ )
    {
        for( int i = 0; i < inPOSOrder[iSeat]->Orders->Count; i++ )
        {
            TItemComplete *orderItem = inPOSOrder[iSeat]->Orders->Items[i];

            TiXmlElement *scourseElem = getSCourseElement( orderItem );
            addItemElement( scourseElem, orderItem );

            total_discount += orderItem->BillCalcResult.TotalDiscount;
                     total += orderItem->BillCalcResult.FinalPrice;
             total_scharge += orderItem->BillCalcResult.ServiceCharge.Value;
                 total_tax += orderItem->BillCalcResult.TotalTax;

            if( !TGlobalSettings::Instance().IncludeServiceChargeTaxInTaxValue )
            {
                    total_tax -= orderItem->BillCalcResult.ServiceCharge.TaxValue;
                total_scharge += orderItem->BillCalcResult.ServiceCharge.TaxValue;
            }
        }
    }

    TiXmlElement *totalElem;

    addElement( totals_elem, "TotalDiscount", totalElem );
    setNodeAttr( totalElem, "name",  "Total Discount" );
    setNodeAttr( totalElem, "value", currencyRoundStr( total_discount ) );

    addElement( totals_elem, "Total", totalElem );
    setNodeAttr( totalElem, "name",  "Total" );
    setNodeAttr( totalElem, "value", currencyRoundStr( total ) );

    addElement( totals_elem, "Change", totalElem );
    setNodeAttr( totalElem, "name",  "Change" );
    setNodeAttr( totalElem, "value", inChangeStr.changeStr );

    //...........................................................

	UnicodeString showSCharge = TGlobalSettings::Instance().EnableDisplayServiceCharge ?
                                 "true" : "false";

    addElement( totals_elem, "ServiceCharge", totalElem );
    setNodeAttr( totalElem, "name",    "Service Charge" );
    setNodeAttr( totalElem, "value",   currencyRoundStr( total_scharge ) );
    setNodeAttr( totalElem, "visible", showSCharge );

    //...........................................................

    UnicodeString showTax = TGlobalSettings::Instance().EnableDisplayTax ?
                                 "true" : "false";
     UnicodeString  BirthDayBonus=  inChangeStr.HappyBirthDay ?
                                 "true" : "false";
    UnicodeString  FirstVisitBonus    =  inChangeStr.FirstVisit ?
                                 "true" : "false";

    addElement( totals_elem, "TotalTax", totalElem );
    setNodeAttr( totalElem, "name",    "Tax" );
    setNodeAttr( totalElem, "value",   currencyRoundStr( total_tax ) );
    setNodeAttr( totalElem, "visible", showTax );
    TiXmlElement *loyaltyelem;

    addElement( loyalty_elem, "FirstVisitBonus", loyaltyelem );
    setNodeAttr( loyaltyelem, "name",  "FirstVisit" );
    setNodeAttr( loyaltyelem, "value", FirstVisitBonus );
    // PreUserInfo =frmBillGroup->TempUserInfo;
    addElement( loyalty_elem, "BirthDayBonus", loyaltyelem );
    setNodeAttr( loyaltyelem, "name",  "BirthDay" );
    setNodeAttr( loyaltyelem, "value", BirthDayBonus );

       addElement( loyalty_elem, "TierIncrease", loyaltyelem );
    setNodeAttr( loyaltyelem, "name",  "Tier" );
    setNodeAttr( loyaltyelem, "value",inChangeStr.TierLevel  );
}
//---------------------------------------------------------------------------

void TMMCustomerDisplayAdapter::populateOrderPaidXmlDoc(
                                   Currency inLastTotal,
                                   Currency inLastChange,
                                   Currency inTotalServiceCharge,
                                   Currency inTotalTax )
{
    total_discount = 0;
    total          = inLastTotal;
    total_change   = inLastChange;
    total_scharge  = inTotalServiceCharge;
    total_tax      = inTotalTax;

    TiXmlElement *totalElem;

    addElement( totals_elem, "TotalDiscount", totalElem );
    setNodeAttr( totalElem, "name",  "Total Discount" );
    setNodeAttr( totalElem, "value", currencyRoundStr( total_discount ) );

    addElement( totals_elem, "Total", totalElem );
    setNodeAttr( totalElem, "name",  "Total" );
    setNodeAttr( totalElem, "value", currencyRoundStr( total ) );

    addElement( totals_elem, "Change", totalElem );
    setNodeAttr( totalElem, "name",  "Change" );
    setNodeAttr( totalElem, "value", currencyRoundStr( total_change ) );

    //...........................................................

    UnicodeString showSCharge = TGlobalSettings::Instance().EnableDisplayServiceCharge ?
                                 "true" : "false";

    addElement( totals_elem, "ServiceCharge", totalElem );
    setNodeAttr( totalElem, "name",    "Service Charge" );
    setNodeAttr( totalElem, "value",   currencyRoundStr( total_scharge ) );
    setNodeAttr( totalElem, "visible", showSCharge );

    //...........................................................

    UnicodeString showTax = TGlobalSettings::Instance().EnableDisplayTax ?
                                 "true" : "false";

    addElement( totals_elem, "TotalTax", totalElem );
    setNodeAttr( totalElem, "name",    "Tax" );
    setNodeAttr( totalElem, "value",   currencyRoundStr( total_tax ) );
    setNodeAttr( totalElem, "visible", showTax );

    TiXmlElement *loyaltyelem;

     addElement( loyalty_elem, "TierIncrease", loyaltyelem );
    setNodeAttr( loyaltyelem, "name",  "Tier" );
    setNodeAttr( loyaltyelem, "value",TGlobalSettings::Instance().TierLevelChange  );


   //  addElement( totals_elem, "TierIncrease", totalElem );
   // setNodeAttr( totalElem, "name",  "Tier" );
   // setNodeAttr( totalElem, "value", TGlobalSettings::Instance().TierLevelChange  );
    TGlobalSettings::Instance().TierLevelChange=0;
}
//---------------------------------------------------------------------------

UnicodeString TMMCustomerDisplayAdapter::currencyRoundStr( Currency inValue )
{
    return UnicodeString::CurrToStrF( currencyRound( inValue ), UnicodeString::sffNumber, 2 ) ;
}
//---------------------------------------------------------------------------

Currency TMMCustomerDisplayAdapter::currencyRound( Currency inValue )
{
    return RoundToNearest( inValue, 0.01, TGlobalSettings::Instance().MidPointRoundsDown );
}
//---------------------------------------------------------------------------

TiXmlElement* TMMCustomerDisplayAdapter::getSCourseElement( TItemComplete* inOrderItem )
{
   TiXmlElement* result;

   //::::::::::::::::::::::::::::::::::::::::::::::

   UnicodeString scName = inOrderItem->ServingCourse.Name;

   try
   {
       result = findChildElementByAttr( scourses_elem, "name", scName );
   }
   catch( ... )
   {
       // Add new ServingCourse element
       addElement(  scourses_elem, "ServingCourse", result );
       setNodeAttr( result, "name", scName );

       TiXmlElement *itemsElem;
       addElement( result, "Items", itemsElem );
   }

   //::::::::::::::::::::::::::::::::::::::::::::::

   return result;
}
//---------------------------------------------------------------------------
void TMMCustomerDisplayAdapter::addItemElement(
                                    TiXmlElement*  inSCourseElem,
                                    TItemComplete* inOrderItem )
{
    TiXmlElement *itemsElem = findChildElement( inSCourseElem, "Items" );

    bool isOriginalPrice = inOrderItem->PriceLevelCustom == 0;
    bool discounted      = inOrderItem->BillCalcResult.TotalDiscount != 0;
    Currency  price      = inOrderItem->PriceEach() * fabs( inOrderItem->GetQty() );
    price += inOrderItem->BillCalcResult.TotalDiscount;

	UnicodeString itemDisplayName = getItemDisplayName( inOrderItem, inOrderItem->GetQty() );
    UnicodeString itemName        = getItemName( inOrderItem );
    UnicodeString qty             = CurrToStrF( inOrderItem->GetQty(), ffNumber, CurrencyDecimals );
    UnicodeString priceStr        = CurrToStrF( price, ffNumber, CurrencyDecimals );
    UnicodeString discountedStr   = discounted ? "true" : "false";

    int optionCount = inOrderItem->OptionsSelected->Count;
	int sideCount   = inOrderItem->SubOrders->Count;

	bool isUsingNmiStandard  = TGlobalSettings::Instance().EnableNmiDisplay;
	bool isSeperateItemOnNMI = inOrderItem->WeightedPrice.WeightedItem
								|| inOrderItem->IsPriceBarcodedItem;

    // Find an existing item with no Options/Sides/Discounts
    TiXmlElement *itemElem = findItemElement( itemName, itemsElem );

    UnicodeString isLastAddedItem = inOrderItem->LastAddedItem ? "True" : "False";  //check last added item in select dish..
    

	if( isOriginalPrice
		&& ( optionCount == 0 )
		&& ( sideCount == 0 )
		&& ( itemElem != NULL )
		&& !( isUsingNmiStandard && isSeperateItemOnNMI ))	// specific items are seperated when using nmi standard
    {
        // Update existing item
        Currency existingQty = StrToCurr( itemElem->Attribute( "qty" ) );

        existingQty = existingQty + inOrderItem->GetQty();
        price = inOrderItem->PriceEach() * fabs( existingQty );
        price += inOrderItem->BillCalcResult.TotalDiscount;

        priceStr = CurrToStrF( price, ffNumber, CurrencyDecimals );

        setNodeAttr( itemElem, "qty",         CurrToStrF( existingQty, ffNumber, CurrencyDecimals ) );
//        setNodeAttr( itemElem, "displayName", updateItemDisplayName( existingQty, itemElem->Attribute( "name" ) ) );
        setNodeAttr( itemElem, "displayName", getItemDisplayName( inOrderItem, existingQty ) );
        setNodeAttr( itemElem, "price",       priceStr        );
        setNodeAttr( itemElem, "lastAddedItem",   isSameAddedItem    );
        //isSameAddedItem = "False";
    }
    else
    {
        // Add new item
        addElement( itemsElem, "Item",        itemElem        );
        setNodeAttr( itemElem, "displayName", itemDisplayName );
        setNodeAttr( itemElem, "name",        itemName        );
        setNodeAttr( itemElem, "qty",         qty             );
        setNodeAttr( itemElem, "price",       priceStr        );
        setNodeAttr( itemElem, "discounted",  discountedStr   );
        setNodeAttr( itemElem, "lastAddedItem", isLastAddedItem );
        isSameAddedItem = isLastAddedItem;
        addOptions( itemElem, inOrderItem );
        addSides(   itemElem, inOrderItem );
    }
}
//---------------------------------------------------------------------------
TiXmlElement* TMMCustomerDisplayAdapter::findItemElement( UnicodeString inItemName, TiXmlElement *itemsElem )
{
   TiXmlElement* result;

   //::::::::::::::::::::::::::::::::::::::::::::::

   try
   {
       result = findChildElementByAttr( itemsElem, "name", inItemName );

       bool discounted = UnicodeString( result->Attribute( "discounted" )  ).UpperCase().Trim() == "TRUE";
       if( discounted )
       {
           result = NULL;
       }
       else
       {
           TiXmlElement *options = findChildElement( result, "Options" );
           TiXmlElement *sides   = findChildElement( result, "Sides" );

           if( ( options->FirstChild() != NULL ) || ( sides->FirstChild() != NULL ) )
           {
              result = NULL;
           }
       }
   }
   catch( ... )
   {
       result = NULL;
   }

   //::::::::::::::::::::::::::::::::::::::::::::::

   return result;
}
//---------------------------------------------------------------------------
UnicodeString TMMCustomerDisplayAdapter::getItemDisplayName( TItemComplete* inOrderItem, Currency inQty )
{
	UnicodeString result = "";

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	if( TGlobalSettings::Instance().EnableNmiDisplay)
	{
		result = updateItemDisplayNameNMIFormat( inOrderItem, inQty );
	}
	else
	{
		result = updateItemDisplayName( inQty, getItemName( inOrderItem ) );
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
UnicodeString TMMCustomerDisplayAdapter::updateItemDisplayNameNMIFormat( TItemComplete* inOrderItem, Currency inQty )
{
	UnicodeString formattedOutput = "";

	Currency Qty = inQty;

	if( inOrderItem->WeightedPrice.WeightedItem )
	{
		formattedOutput = inOrderItem->Item
							+ "\n"
							+ UnicodeString(FormatFloat("0.000", Qty))
							+ " "
							+ inOrderItem->WeightedPrice.Weight.GetUnitAsString();

		bool hasTareWeight = inOrderItem->WeightedPrice.TareWeight.Valid()
								&& inOrderItem->WeightedPrice.TareWeight.ToDouble() > 0.00;

		// if the weighted item has tare weight, format it accordingly
		formattedOutput += hasTareWeight ? UnicodeString(" N") : UnicodeString("");

		formattedOutput += " @ "
							+ getCurrencySymbol()
							+ CurrToStrF(inOrderItem->PriceLevel0, ffNumber, CurrencyDecimals)
							+ "/"
                            + inOrderItem->WeightedPrice.Weight.GetUnitAsString();
	}
	else
	{
		if (inOrderItem->IsPriceBarcodedItem)
		{
			// price barcoded items are displayed same as weighted items
			formattedOutput = inOrderItem->Item
								+ "\n"
								+ FormatFloat("0.###", Qty)
								+ " kg @ "
								+ getCurrencySymbol()
								+ CurrToStrF(inOrderItem->PriceEach(), ffNumber, CurrencyDecimals)
								+ "/kg" ;
		}
		else
		{
			UnicodeString SizeString = inOrderItem->ItemOrderedFrom->Sizes->Count == 1
										|| inOrderItem->Size.UpperCase() == "DEFAULT" ? UnicodeString("") : inOrderItem->Size + " ";

			// items with qty are displayed in an special way
			formattedOutput = SizeString
								+ inOrderItem->Item
								+ "\n"
								+ "qty "
								+ FormatFloat("0.###", Qty)
								+ " @ "
								+ getCurrencySymbol()
								+ inOrderItem->PriceEach()
								+ " each";
		}
	}

	return formattedOutput;
}
//---------------------------------------------------------------------------
UnicodeString TMMCustomerDisplayAdapter::updateItemDisplayName( Currency inQty, UnicodeString inItemName )
{
    UnicodeString result = "";

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    double intPart, fractPart;

    fractPart = modf( inQty, &intPart );
    if( fractPart == 0 )
    {
        int intQty = inQty;

        result = IntToStr( intQty ) + "x " + inItemName;
    }
    else
	{
        double intQty = inQty;

        result = CurrToStrF( intQty, ffNumber, CurrencyDecimals ) + "x " + inItemName;
    }


    //:::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
UnicodeString TMMCustomerDisplayAdapter::getItemName( TItemComplete* inOrderItem )
{
    UnicodeString result = "";

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    UnicodeString sizeStr  = inOrderItem->Size.UpperCase().Trim();
    UnicodeString itemSize = sizeStr == UnicodeString( "DEFAULT" ) ?
                             UnicodeString( "" ) :  inOrderItem->Size + UnicodeString( " " );

    result = itemSize + inOrderItem->ItemKitchenName;

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

void TMMCustomerDisplayAdapter::addOptions(
                                   TiXmlElement*  inItemElem,
                                   TItemComplete* inOrderItem )
{
    TiXmlElement *optionsElem;
    addElement( inItemElem, "Options", optionsElem );

	for( int i = 0; i < inOrderItem->OptionsSelected->Count; i++ )
	{
		TItemOption *option = inOrderItem->OptionsSelected->OptionGet( i );

        TiXmlElement *optionElem;
        UnicodeString optionName = option->Name;
        UnicodeString added      = option->IsPlus ? "true" : "false";

        addElement( optionsElem, "Option", optionElem );
        setNodeAttr( optionElem, "name",   optionName );
        setNodeAttr( optionElem, "added",  added );
	}
}
//---------------------------------------------------------------------------
void TMMCustomerDisplayAdapter::addSides(
                                   TiXmlElement*  inItemElem,
                                   TItemComplete* inOrderItem )
{
    TiXmlElement *sidesElem;
    addElement( inItemElem, "Sides", sidesElem );

    for( int i = 0; i < inOrderItem->SubOrders->Count; i++ )
	{
		TItemCompleteSub *side = inOrderItem->SubOrders->SubOrderGet( i );

        TiXmlElement *sideElem;
        UnicodeString sideName   = side->Item;
        UnicodeString qty        = "1.00";
        UnicodeString price      = CurrToStrF( side->BillCalcResult.FinalPrice, ffNumber, CurrencyDecimals );
        UnicodeString discounted = side->BillCalcResult.TotalDiscount > 0 ? "true" : "false";

        addElement(  sidesElem, "Side",       sideElem   );
        setNodeAttr( sideElem,  "name",       sideName   );
        setNodeAttr( sideElem,  "qty",        qty        );
        setNodeAttr( sideElem,  "price",      price      );
        setNodeAttr( sideElem,  "discounted", discounted );

        total_discount += side->BillCalcResult.TotalDiscount;
                total  += side->BillCalcResult.FinalPrice;
         total_scharge += side->BillCalcResult.ServiceCharge.Value;
             total_tax += side->BillCalcResult.TotalTax;

        if( !TGlobalSettings::Instance().IncludeServiceChargeTaxInTaxValue )
        {
                total_tax -= side->BillCalcResult.ServiceCharge.TaxValue;
            total_scharge += side->BillCalcResult.ServiceCharge.TaxValue;
        }
	}
}
//---------------------------------------------------------------------------

void TMMCustomerDisplayAdapter::addElement( TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem )
{
	inElem = new TiXmlElement(  inName.c_str() );
	inParentElem->LinkEndChild( inElem );
}
//----------------------------------------------------------------------------
void TMMCustomerDisplayAdapter::setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue )
{
    inElem->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------
TiXmlElement* TMMCustomerDisplayAdapter::findChildElementByAttr(
                                           TiXmlElement* inParentElem,
                                           AnsiString inChildAttrName,
                                           AnsiString inChildAttrValue )
{
    AnsiString elementNotFound = inChildAttrName;
    try
    {
       TiXmlElement *childElement = inParentElem->FirstChildElement();

       while( childElement != NULL )
       {
         if( AnsiString( childElement->Attribute( inChildAttrName.c_str() ) ) == inChildAttrValue )
         {
            return childElement;
         }
         childElement = childElement->NextSiblingElement();
       }
    }
    catch( ... )
    {
       elementNotFound = "root";
    }

    throw Exception( "Element not found: " + elementNotFound );
}
//----------------------------------------------------------------------------
TiXmlElement* TMMCustomerDisplayAdapter::findChildElement( TiXmlElement* inParentElem, AnsiString inChildElementName )
{
    AnsiString elementNotFound = inChildElementName;
    try
    {
       TiXmlElement *childElement = inParentElem->FirstChildElement();

       while( childElement != NULL )
       {
         if( AnsiString( childElement->Value() ) == inChildElementName )
         {
            return childElement;
         }
         childElement = childElement->NextSiblingElement();
       }
    }
    catch( ... )
    {
       elementNotFound = "root";
    }

    throw Exception( "Element not found: " + elementNotFound );
}
//----------------------------------------------------------------------------
UnicodeString TMMCustomerDisplayAdapter::getCurrencySymbol()
{
	return CurrencyString;
}
//---------------------------------------------------------------------------
//----------------------------------------------------------------------------
UnicodeString TMMCustomerDisplayAdapter::getCustomerFontSize()
{
    return "9.5";
}
//---------------------------------------------------------------------------
