//---------------------------------------------------------------------------

#pragma hdrstop

#include "SaveMenu.h"

#include "tinyxml.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

TSaveMenu::TSaveMenu( __int32 inKey, AnsiString inMenuVersion, AnsiString inFileName, AnsiString inBackupFileName, const __int32 inLastGeneratedID )
{
    FFileName       = inFileName;
    FBackupFileName = inBackupFileName;

    _xmlMenuDoc = createXMLMenuDoc( inKey, inMenuVersion );

    addMainElements();

    setNodeAttr( _rootElem, "key",     IntToStr( inKey ) );
    MenuVersion = inMenuVersion;
    LastGeneratedID = inLastGeneratedID;
}
//---------------------------------------------------------------------------
TSaveMenu::~TSaveMenu()
{
    delete _xmlMenuDoc;
}
//---------------------------------------------------------------------------
__int32 TSaveMenu::SaveCategoryGroup( __int32 inKey, AnsiString inDescription, bool inGetDeleted )
{
    TiXmlElement *groupElem;

    //::::::::::::::::::::::::::::

    addElement( _categoryGroupsElem, "CategoryGroup", groupElem );
    setNodeAttr( groupElem, "key",         IntToStr( inKey ) );
    setNodeAttr( groupElem, "description", convertUTF8CharToWideString(inDescription) );
    setNodeAttr( groupElem, "deleted",     inGetDeleted ? "true" : "false" );

    //::::::::::::::::::::::::::::

    return ( __int32 )groupElem;
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveCategory( __int32 inCategoryGroupHandle, __int32 inKey, AnsiString inDescription,AnsiString inGLCode, bool inGetDeleted )
{
    try
    {
      TiXmlElement *groupElem = ( TiXmlElement* )inCategoryGroupHandle;
      TiXmlElement *categoryElem;
      addElement( groupElem,     "Category", categoryElem );
      setNodeAttr( categoryElem, "key",         IntToStr( inKey ) );
	  setNodeAttr( categoryElem, "description", convertUTF8CharToWideString(inDescription) );
	  setNodeAttr( categoryElem, "glcode",      convertUTF8CharToWideString(inGLCode) );
	  setNodeAttr( categoryElem, "deleted",     inGetDeleted ? "true" : "false" );
    }
    catch( ... )
    {
    }
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveSize( __int32 inKey, AnsiString inDescription, AnsiString inKitchenName, AnsiString inHandheldName,
                          AnsiString inReceiptName, bool inWeighed, __int32 inSizeID, __int32 inPalmID )
{
    TiXmlElement *sizeElem;

    addElement( _sizesElem, "Size",        sizeElem );
    setNodeAttr( sizeElem, "key",          IntToStr( inKey ) );
    setNodeAttr( sizeElem, "description",  convertUTF8CharToWideString(inDescription) );
    setNodeAttr( sizeElem, "kitchenName",  convertUTF8CharToWideString(inKitchenName) );
    setNodeAttr( sizeElem, "handheldName", convertUTF8CharToWideString(inHandheldName) );
    setNodeAttr( sizeElem, "kitchenName",  convertUTF8CharToWideString(inKitchenName) );
    setNodeAttr( sizeElem, "weighed",      inWeighed ? "true" : "false" );
    setNodeAttr( sizeElem, "sizeID",       IntToStr( inSizeID ) );
    setNodeAttr( sizeElem, "palmID",       IntToStr( inPalmID ) );
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveServingCourse( __int32 inKey, AnsiString inLongDescription, AnsiString inKitchenName, bool inEnabled,
                                   bool inDeleted, bool inSelectable, TColor inColor, __int32 displayOrder )
{
    TiXmlElement *serviceCourseElem;

    addElement( _serviceCoursesElem, "ServiceCourse", serviceCourseElem );
    setNodeAttr( serviceCourseElem, "key",            IntToStr( inKey ) );
    setNodeAttr( serviceCourseElem, "description",    convertUTF8CharToWideString(inLongDescription) );
    setNodeAttr( serviceCourseElem, "kitchenName",    convertUTF8CharToWideString(inKitchenName) );
    setNodeAttr( serviceCourseElem, "enabled",        inEnabled    ? "true" : "false" );
    setNodeAttr( serviceCourseElem, "deleted",        inDeleted    ? "true" : "false" );
    setNodeAttr( serviceCourseElem, "selectable",     inSelectable ? "true" : "false" );
    setNodeAttr( serviceCourseElem, "color",          IntToStr( inColor ) );
    setNodeAttr( serviceCourseElem, "displayOrder",            IntToStr( displayOrder ) );
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveThirdPartyCode( __int32 inKey, AnsiString inCode, AnsiString inDescription, bool inVisible, __int32 inCodeType )
{
    TiXmlElement *tpcElem;

    addElement( _TPCsElem, "ThirdPartyCode", tpcElem );
    setNodeAttr( tpcElem, "key",         IntToStr( inKey ) );
    setNodeAttr( tpcElem, "code",        inCode );
    setNodeAttr( tpcElem, "codeType",    inCodeType );
    setNodeAttr( tpcElem, "visible",     ( int )inVisible );
    setNodeAttr( tpcElem, "description", inDescription );
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveRevenueCodes(__int32 code, AnsiString description)
{
    TiXmlElement *tpcElem;

    addElement( _revenueElem, "RevenueCode", tpcElem );
    setNodeAttr( tpcElem, "code",          IntToStr( code ) );
    setNodeAttr( tpcElem, "description",   description );
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveTaxProfile( __int32 inKey, AnsiString inName, Currency inTaxRate, __int32 inTaxType, __int32 inPriority)
                        //-- to be incorpotated in future--//
                        //, __int32 inTaxCode )
{
    TiXmlElement *taxProfileElem;

    addElement( _taxProfilesElem, "TaxProfile", taxProfileElem );
    setNodeAttr( taxProfileElem, "key",      IntToStr( inKey ) );
    setNodeAttr( taxProfileElem, "name",     convertUTF8CharToWideString(inName) );
    setNodeAttr( taxProfileElem, "rate",     CurrToStr( inTaxRate ) );
    setNodeAttr( taxProfileElem, "type",     IntToStr( inTaxType ) );
    setNodeAttr( taxProfileElem, "priority", IntToStr( inPriority ) );
    //-- to be incorpotated in future--//
    //setNodeAttr( taxProfileElem, "taxcode",  IntToStr( inTaxCode ) );
}
//---------------------------------------------------------------------------
__int32 TSaveMenu::SaveMenuCourse( __int32 inKey, AnsiString inLongDescription, AnsiString inKitchenName,
                                   AnsiString inHandheldName, AnsiString inReceiptName, __int32 inLocation,
                                   bool inEnabled, __int32 inServingCourseKey, AnsiString inServingCourse,
                                   bool inNoDefaultServingCourse )
{
    try
    {
        TiXmlElement *courseElem;

        addElement(  _coursesElem, "Course",                 courseElem );
        setNodeAttr(  courseElem,  "key",                    IntToStr( inKey ) );
        setNodeAttr(  courseElem,  "description",            convertUTF8CharToWideString(inLongDescription) );
        setNodeAttr(  courseElem,  "kitchenName",            convertUTF8CharToWideString(inKitchenName) );
        setNodeAttr(  courseElem,  "handheldName",           convertUTF8CharToWideString(inHandheldName) );
        setNodeAttr(  courseElem,  "receiptName",            convertUTF8CharToWideString(inReceiptName) );
        setNodeAttr(  courseElem,  "location",               IntToStr( inLocation ) );
        setNodeAttr(  courseElem,  "enabled",                inEnabled ? "true" : "false" );
        setNodeAttr(  courseElem,  "noDefaultServingCourse", inNoDefaultServingCourse ? "true" : "false" );
        setNodeAttr(  courseElem,  "servingCourseKey",       IntToStr( inServingCourseKey ) );
        setNodeAttr(  courseElem,  "servingCourseName",      convertUTF8CharToWideString(inServingCourse) );

        //::::::::::::::::::::::::::

        TiXmlElement *newElem;

        addElement( courseElem, "Items",   newElem );
        addElement( courseElem, "Options", newElem );

        //::::::::::::::::::::::::::::

        return ( __int32 )courseElem;
    }
    catch( ... )
    {
        return ( __int32 )NULL;
    }
}
//---------------------------------------------------------------------------
//changes to save items...
__int32 TSaveMenu::SaveItem( __int32 inCourseHandle, __int32 inKey, AnsiString inLongDescription,
                             AnsiString inKitchenName, AnsiString inHandheldName, AnsiString inReceiptName,
                             TColor inColor, bool inDisplaySizes, bool inEnabled, bool inItemOnlySide,
                             bool  inPrintUnderlined, bool inPrintBold, TColor inPrintColor,
                             __int32 inPrintFont, bool inPrintDoubleWidth, bool inPrintDoubleHeight )
{
    try
    {
      TiXmlElement *courseElem = ( TiXmlElement* )inCourseHandle;
      TiXmlElement *itemsElem  = ( TiXmlElement* )courseElem->FirstChild();
      TiXmlElement *itemElem;

      //:::::::::::::::::::::::::::::::::

      addElement(  itemsElem, "Item",              itemElem );
	  setNodeAttr( itemElem,  "key",               IntToStr( inKey ) );
	  setNodeAttr( itemElem,  "description",       convertUTF8CharToWideString(inLongDescription));
	  setNodeAttr( itemElem,  "kitchenName",       convertUTF8CharToWideString(inKitchenName));
	  setNodeAttr( itemElem,  "handheldName",      convertUTF8CharToWideString(inHandheldName));
	  setNodeAttr( itemElem,  "receiptName",       convertUTF8CharToWideString(inReceiptName));
	  setNodeAttr( itemElem,  "color",             IntToStr( inColor ) );
	  setNodeAttr( itemElem,  "displaySizes",      inDisplaySizes ? "true" : "false" );
	  setNodeAttr( itemElem,  "enabled",           inEnabled ? "true" : "false" );
	  setNodeAttr( itemElem,  "itemOnlySide",      inItemOnlySide ? "true" : "false" );
	  setNodeAttr( itemElem,  "printUnderlined",   inPrintUnderlined ? "true" : "false" );
	  setNodeAttr( itemElem,  "printBold",         inPrintBold ? "true" : "false" );
	  setNodeAttr( itemElem,  "printColor",        IntToStr( inPrintColor ) );
	  setNodeAttr( itemElem,  "printFont",         inPrintFont );
      setNodeAttr( itemElem,  "printDoubleWidth",  inPrintDoubleWidth ? "true" : "false" );
      setNodeAttr( itemElem,  "printDoubleHeight", inPrintDoubleHeight ? "true" : "false" );

      //:::::::::::::::::::::::::::::::::

      TiXmlElement *newElem;

      addElement( itemElem, "ForcedSides",   newElem );
      addElement( itemElem, "ForcedOptions", newElem );
      addElement( itemElem, "ItemSizes",     newElem );
      //:::::::::::::::::::::::::::::::::

      return ( __int32 )itemElem;
    }
    catch( ... )
    {
        return ( __int32 )NULL;
    }
}
//---------------------------------------------------------------------------
__int32 TSaveMenu::AssembleOptionMask(const int inGroupNumber,
                                      const int inMaxSelect,
                                      const bool inPlusOption,
                                      const bool inMinusOption,
                                      const bool inOptionsGroupSkip)
const
{
   /*
   __int32 flags = inGroupNumber \
                   | inMaxSelect << 5 \
                   | inPlusOption << 10 \
                   | inMinusOption << 11 \
                   | inOptionsGroupSkip << 12;
   */

   __int32 flags = inGroupNumber
                   + (inMaxSelect ? 0x10 : 0)
                   + (inPlusOption ? 0x20 : 0)
                   + (inMinusOption ? 0x40 : 0)
                   + (inOptionsGroupSkip ? 0x80 : 0);

   return flags;
}

void TSaveMenu::SaveOption( __int32 inCourseHandle, __int32 inKey, AnsiString inLongDescription,
                           AnsiString inKitchenName, AnsiString inHandheldName, AnsiString inReceiptName,
                           __int32 inCondimentsMask, __int32 inFlags, bool inEnabled, bool inPrintUnderlined,
                           bool inPrintBold, __int32 inPrintColor, __int32 inPrintFont,
                           bool inPrintDoubleWidth, bool inPrintDoubleHeight )
{
    try
    {
      TiXmlElement *courseElem  = ( TiXmlElement* )inCourseHandle;
      TiXmlElement *optionsElem = ( TiXmlElement * )courseElem->FirstChild();
      optionsElem = optionsElem->NextSiblingElement();
      TiXmlElement *optionElem;

      //:::::::::::::::::::::::::::::::::

      addElement(  optionsElem, "Option",            optionElem );
      setNodeAttr( optionElem,  "key",               IntToStr( inKey ) );
      setNodeAttr( optionElem,  "description",       convertUTF8CharToWideString(inLongDescription));
      setNodeAttr( optionElem,  "kitchenName",       convertUTF8CharToWideString(inKitchenName));
      setNodeAttr( optionElem,  "handheldName",      convertUTF8CharToWideString(inHandheldName));
      setNodeAttr( optionElem,  "receiptName",       convertUTF8CharToWideString(inReceiptName));
      setNodeAttr( optionElem,  "condimentsMask",    inCondimentsMask );
      setNodeAttr( optionElem,  "flags",             inFlags );
      setNodeAttr( optionElem,  "enabled",           inEnabled ? "true" : "false");
      setNodeAttr( optionElem,  "printUnderlined",   inPrintUnderlined ? "true" : "false" );
      setNodeAttr( optionElem,  "printBold",         inPrintBold ? "true" : "false" );
      setNodeAttr( optionElem,  "printColor",        IntToStr( inPrintColor ) );
      setNodeAttr( optionElem,  "printFont",         IntToStr( inPrintFont ) );
      setNodeAttr( optionElem,  "printDoubleWidth",  inPrintDoubleWidth ? "true" : "false" );
      setNodeAttr( optionElem,  "printDoubleHeight", inPrintDoubleHeight ? "true" : "false" );
      //optionElem->Clear();
    }
    catch( ... )
    {
    }
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveForcedSide( __int32 inItemHandle, __int32 inKey, __int32 inItemFKey,
                                AnsiString inLongDescription,
                                __int32 inGroupNumber, __int32 inMaxSelect, bool inSideGroupSkip )
{
    try
    {
      TiXmlElement *itemElem  = ( TiXmlElement* )inItemHandle;
      TiXmlElement *sidesElem = ( TiXmlElement* )itemElem->FirstChild();
      TiXmlElement *sideElem;

      //:::::::::::::::::::::::::::::::::

      addElement(  sidesElem, "ForcedSide",    sideElem );
      setNodeAttr( sideElem, "key",           IntToStr( inKey ) );
      setNodeAttr( sideElem, "itemFKey",      IntToStr( inItemFKey ) );
      setNodeAttr( sideElem, "description",   convertUTF8CharToWideString(inLongDescription));
      setNodeAttr( sideElem, "groupNumber",   inGroupNumber );
      setNodeAttr( sideElem, "maxSelect",     inMaxSelect );
      setNodeAttr( sideElem, "sideGroupSkip", inSideGroupSkip ? "true" : "false" );
      //sideElem->Clear();
    }
    catch( ... )
    {
    }
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveMenuItemForcedOption( __int32 inItemHandle, __int32 inKey, __int32 inOptionFKey,
                                          AnsiString inDescription, __int32 inGroupNumber )
{
    try
    {
        TiXmlElement *itemElem       = ( TiXmlElement* )inItemHandle;
        TiXmlElement *fisOptionsElem = ( TiXmlElement* )itemElem->FirstChild()->NextSiblingElement();
        TiXmlElement *fisOptionElem;

        addElement( fisOptionsElem, "ForcedOption", fisOptionElem );
        setNodeAttr( fisOptionElem, "key",         IntToStr( inKey ) );
        setNodeAttr( fisOptionElem, "optionFKey",  IntToStr( inOptionFKey ) );
        setNodeAttr( fisOptionElem, "description", convertUTF8CharToWideString(inDescription));
        setNodeAttr( fisOptionElem, "groupNumber", inGroupNumber );
        //fisOptionElem->Clear();
    }
    catch( ... )
    {
    }
}
//---------------------------------------------------------------------------
__int32 TSaveMenu::SaveItemSize( __int32 inItemHandle, __int32 inKey, __int32 inSizeFKey, AnsiString inLongDescription, /*MM1351*/AnsiString inKitchenName,
								 bool inFree, Currency inPrice, Currency inMaxRetailPrice, Currency inSpecialPrice,
                                 double inGST, Currency inCost, Currency inCostGST,
                                 double inPointsPercent, double inMemberPurchaseCount,
                                 double inMemberPurchaseDiscount, double inLocationPurchaseCount,
                                 double inLocationPurchaseDiscount, bool inNoRecipe,
                                 bool inIsTaxExempt, bool inHasServiceCharge,
                                 __int32 inSetMenuMask, bool inAvailableAsStandard,
                                 AnsiString inBarcode, bool inEnabled,
                                 __int32 inCategoryKey, AnsiString inCategory,
                                 __int32 inThirdPartyCodeKey, double inTareWeight,
                                 __int32 inPLU, double inAvailableQuantity, double inDefaultQuantity, double inWarningQuantity,
                                 bool inDisableWhenCountReachesZero,
                                 bool inCanBePaidForUsingPoints,
                                 const int inDefaultPatronCount,
                                 Currency inPriceForPoints, int revenueCode)
{
    try
    {
        const char * const true_or_false[2] = {"false", "true"};

        TiXmlElement *itemElem  = ( TiXmlElement* )inItemHandle;
        TiXmlElement *sizesElem = itemElem->FirstChild()->NextSiblingElement()->NextSiblingElement();
        TiXmlElement *sizeElem;

        //:::::::::::::::::::::::::::::::::
        //sizeElem->Clear();

        addElement(  sizesElem, "ItemSize",                 sizeElem );
        setNodeAttr( sizeElem,  "key",                      IntToStr( inKey ) );
        setNodeAttr( sizeElem,  "sizeFKey",                 IntToStr( inSizeFKey ) );
        setNodeAttr( sizeElem,  "description",              convertUTF8CharToWideString(inLongDescription));
        //MM1351
        setNodeAttr( sizeElem,  "kitchenName",              convertUTF8CharToWideString(inKitchenName));
        setNodeAttr( sizeElem,  "free",                     inFree ? "true" : "false" );
		setNodeAttr( sizeElem,  "price",                    CurrToStr( inPrice ) );
		setNodeAttr( sizeElem,  "maxRetailPrice",           CurrToStr( inMaxRetailPrice ) );
        setNodeAttr( sizeElem,  "specialPrice",             CurrToStr( inSpecialPrice ) );
        setNodeAttr( sizeElem,  "gst",                      FloatToStr( inGST ) );
        setNodeAttr( sizeElem,  "cost",                     CurrToStr( inCost ) );
        setNodeAttr( sizeElem,  "costGST",                  CurrToStr( inCostGST ) );
        setNodeAttr( sizeElem,  "pointsPercent",            FloatToStr( inPointsPercent ) );
        setNodeAttr( sizeElem,  "memberPurchaseCount",      FloatToStr( inMemberPurchaseCount ) );
        setNodeAttr( sizeElem,  "memberPurchaseDiscount",   FloatToStr( inMemberPurchaseDiscount ) );
        setNodeAttr( sizeElem,  "locationPurchaseCount",    FloatToStr( inLocationPurchaseCount ) );
        setNodeAttr( sizeElem,  "locationPurchaseDiscount", FloatToStr( inLocationPurchaseDiscount ) );
        setNodeAttr( sizeElem,  "noRecipe",                 inNoRecipe ? "true" : "false" );
        setNodeAttr( sizeElem,  "setMenuMask",              IntToStr( inSetMenuMask ) );
        setNodeAttr( sizeElem,  "availableAsStandard",      inAvailableAsStandard ? "true" : "false" );
        setNodeAttr( sizeElem,  "barcode",                  inBarcode );
        setNodeAttr( sizeElem,  "enabled",                  inEnabled ? "true" : "false" );
        setNodeAttr( sizeElem,  "categoryKey",              inCategoryKey );
        setNodeAttr( sizeElem,  "category",                 convertUTF8CharToWideString(inCategory));
        setNodeAttr( sizeElem,  "thirdPartyCodeKey",        IntToStr( inThirdPartyCodeKey ) );
        setNodeAttr( sizeElem,  "tareWeight",               inTareWeight );
        setNodeAttr( sizeElem,  "PLU",                      IntToStr( inPLU ) );

        //  ASEAN++
        //setNodeAttr( sizeElem,  "isTaxExempt",                 inIsTaxExempt ? "true" : "false" );
        //setNodeAttr( sizeElem,  "hasServiceCharge",            inHasServiceCharge ? "true" : "false" );

        setNodeAttr( sizeElem,  "availableQuantity",           FloatToStr( inAvailableQuantity ) );
        setNodeAttr( sizeElem,  "defaultQuantity",             FloatToStr( inDefaultQuantity ) );
        setNodeAttr( sizeElem,  "warningQuantity",             FloatToStr( inWarningQuantity ) );
        setNodeAttr( sizeElem,  "disableWhenCountReachesZero", inDisableWhenCountReachesZero ? "true" : "false" );

        setNodeAttr(sizeElem,
                    "canBePaidForUsingPoints",
                    true_or_false[inCanBePaidForUsingPoints]);

        setNodeAttr(sizeElem,
                    "defaultPatronCount",
                    IntToStr(inDefaultPatronCount));
        setNodeAttr( sizeElem,  "priceforpoints",    CurrToStr( inPriceForPoints ) ); // add price for points..
        setNodeAttr( sizeElem,  "revenueCode", IntToStr(revenueCode));
        //setNodeAttr( sizeElem,  "revenueCodeDescription", IntToStr(""));

        //:::::::::::::::::::::::::::::::::

        TiXmlElement *newElem;

        addElement( sizeElem, "BreakdownCategories", newElem );
        addElement( sizeElem, "Receipes",            newElem );
        addElement( sizeElem, "TaxProfiles",         newElem );
        addElement( sizeElem, "PriceLevels",         newElem );

        //:::::::::::::::::::::::::::::::::

        return ( __int32 )sizeElem;
    }
    catch( ... )
    {
        return ( __int32 )NULL;
    }
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveBCategory( __int32 inItemSizeHandle, __int32 inKey, AnsiString inName )
{
    try
    {
        TiXmlElement *itemSizeElem    = ( TiXmlElement* )inItemSizeHandle;
        TiXmlElement *bCategoriesElem = ( TiXmlElement* )itemSizeElem->FirstChild();
        TiXmlElement *bCategoryElem;

        addElement( bCategoriesElem, "BreakdownCategory", bCategoryElem );
        setNodeAttr( bCategoryElem,  "key",         IntToStr( inKey ) );
        setNodeAttr( bCategoryElem,  "description", convertUTF8CharToWideString(inName));

    }
    catch( ... )
    {
    }
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveItemSizeRecipe( __int32 inItemSizeHandle, __int32 inKey, AnsiString inStockCode,
                                    double  inQuantity, AnsiString inLocation )
{
    try
    {
        TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;
        TiXmlElement *receipesElem = itemSizeElem->FirstChild()->NextSiblingElement();
        TiXmlElement *receipeElem;

        addElement( receipesElem, "Receipe",   receipeElem );
        setNodeAttr( receipeElem, "key",       IntToStr( inKey ) );
        setNodeAttr( receipeElem, "stockCode", convertUTF8CharToWideString(inStockCode));
        setNodeAttr( receipeElem, "quantity",  inQuantity );
        setNodeAttr( receipeElem, "location",  convertUTF8CharToWideString(inLocation));
    }
    catch( ... )
    {
    }
}
//---------------------------------------------------------------------------
void TSaveMenu::SaveItemSizeTaxProfile( __int32 inItemSizeHandle, __int32 inKey, AnsiString inName )
{
    try
    {
        TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;
        TiXmlElement *taxProfilesElem = itemSizeElem->FirstChild()->NextSiblingElement()->NextSiblingElement();
        TiXmlElement *taxProfileElem;

        addElement( taxProfilesElem, "TaxProfile", taxProfileElem );
        setNodeAttr( taxProfileElem, "key",  IntToStr( inKey ) );
        setNodeAttr( taxProfileElem, "name", convertUTF8CharToWideString(inName));
    }
    catch( ... )
    {
    }
}
//---------------------------------------------------------------------------
bool TSaveMenu::Commit()
{
	bool result = true;

	//:::::::::::::::::::::::::::::::::::::::::

	try
	{
		result = _xmlMenuDoc->SaveFile( FFileName.c_str() );
	}
	catch( Exception &exc )
	{
		result = false;
	}

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//----------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::
// Private
//::::::::::::::::::::::::::::::::
TiXmlDocument* TSaveMenu::createXMLMenuDoc( __int32 inKey, AnsiString inMenuVersion )
{
    TiXmlDocument* result = new TiXmlDocument();
    //::::::::::::::::::::::::::::::

    // add declaration
	TiXmlDeclaration *decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	result->LinkEndChild( decl );
    //::::::::::::::::::::::::::::::  
    return result;
}
//---------------------------------------------------------------------------
void TSaveMenu::addMainElements()
{
    addElement( ( TiXmlElement* )_xmlMenuDoc, "Menu",  _rootElem );
    addElement( _rootElem,   "Sizes",           _sizesElem );
    addElement( _rootElem,   "CategoryGroups",  _categoryGroupsElem ); 
    addElement( _rootElem,   "AllTaxProfiles",  _taxProfilesElem );
    addElement( _rootElem,   "ServiceCourses",  _serviceCoursesElem );
    addElement( _rootElem,   "Courses",         _coursesElem );
    addElement( _rootElem,   "ThirdPartyCodes", _TPCsElem );
    addElement( _rootElem,   "RevenueCodes", _revenueElem );
}
//---------------------------------------------------------------------------
void TSaveMenu::addElement( TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem )
{
	inElem = new TiXmlElement(  inName.c_str() );
	inParentElem->LinkEndChild( inElem );
}
//----------------------------------------------------------------------------
void TSaveMenu::setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue )
{
    inElem->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------
TiXmlElement* findChildElement( TiXmlElement* inParentElem, AnsiString inChildElementName )
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
//::::::::::::::::::::::::::::::::
// Protected
//::::::::::::::::::::::::::::::::
AnsiString TSaveMenu::GetMenuVersion()
{
    try
    {
        AnsiString result( _rootElem->Attribute( "version" ) );
        return result;
    }
    catch( ... )
    {
        return "";
    }
}
//----------------------------------------------------------------------------
void TSaveMenu::SetMenuVersion( AnsiString inVersion )
{
    setNodeAttr( _rootElem, "version", inVersion.c_str() );
}
//----------------------------------------------------------------------------
AnsiString TSaveMenu::GetMenuName()
{
    try
    {
        AnsiString result( _rootElem->Attribute( "name" ) );
        return result;
    }
    catch( ... )
    {
        return "";
    }
}
//----------------------------------------------------------------------------

void TSaveMenu::SetMenuName( AnsiString inName )
{
    WideString menu_name = inName + "    ";
    setNodeAttr( _rootElem, "name", menu_name ); //inName.c_str() );
}
//----------------------------------------------------------------------------

AnsiString TSaveMenu::GetMenuType()
{
    try
    {
        AnsiString result( _rootElem->Attribute( "type" ) );
        return result;
    }
    catch( ... )
    {
        return "";
    }
}
//----------------------------------------------------------------------------

void TSaveMenu::SetMenuType( Menu::TMenuType inType )
{
    setNodeAttr( _rootElem, "type", IntToStr( ( int )inType ) );
}
//----------------------------------------------------------------------------

bool TSaveMenu::GetAvailableOnPalm()
{
    try
    {
        AnsiString result( _rootElem->Attribute( "availableOnPalm" ) );
        return result.UpperCase() == "TRUE";
    }
    catch( ... )
    {
        return false;            
    }
}
//----------------------------------------------------------------------------

void TSaveMenu::SetAvailableOnPalm( bool inAvailable )
{
    setNodeAttr( _rootElem, "availableOnPalm", inAvailable ? "true" : "false" );
}
//----------------------------------------------------------------------------

__int32 TSaveMenu::GetLastGeneratedID()
const
{
   return FLastGeneratedID;
}
//----------------------------------------------------------------------------

void TSaveMenu::SetLastGeneratedID( const __int32 inLastGeneratedID )
{
   setNodeAttr( _rootElem, \
                "nextAvailableKey", \
                FLastGeneratedID = inLastGeneratedID );
}
//----------------------------------------------------------------------------

void TSaveMenu::SaveItemSizePriceLevels( __int32 inItemSizeHandle, __int32 inKey, AnsiString inName,Currency Price )
{
    try
    {
        TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;
        TiXmlElement *PriceLevels = itemSizeElem->FirstChild()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement();
        TiXmlElement *PriceLevel;

        addElement( PriceLevels,"PriceLevels", PriceLevel );
        setNodeAttr( PriceLevel, "PriceLevelKey",  IntToStr( inKey ) );
        setNodeAttr( PriceLevel, "Price", CurrToStrF(Price,ffFixed ,2) );
        setNodeAttr( PriceLevel, "Name", inName );

    }
    catch( ... )
    {
    }
}
////----------------------------------------------------------
WideString TSaveMenu::convertUTF8CharToWideString( AnsiString pchars )
{
	WideString result = "";
#ifdef MenuMate
//	result = UTF8ToWideString( pchars );
    result = UnicodeString( pchars );
#endif

#ifdef Office
	result = UTF8ToWideString( AnsiString(pchars) );
#endif
	return result;
}
//---------------------------------------------------------------------------
WideString TSaveMenu::UTF8ToWideString(AnsiString inString)
{
	int BufferSize   = MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), NULL, 0);
	wchar_t *dest    = new wchar_t[BufferSize];
	MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), dest, BufferSize);
	WideString RetVal(dest, BufferSize);
	delete [] dest;
    if(inString.Length() > 0)
    {
    	return RetVal + "    ";
    }
    else
    {
       return RetVal;
    }

}
