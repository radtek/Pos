//---------------------------------------------------------------------------
#ifndef LoadMenuH
#define LoadMenuH

#include <Classes.hpp>
#include "MM_Menu.h"

//---------------------------------------------------------------------------

class TiXmlDocument;
class TiXmlElement;

//::::::::::::::::::::::::::::::::::::::::::::

enum TLoadMenuResult
{
   lmSuccessful, lmInvalidMenuVersion, lmElementNotFound
};

class ExceptionLoadMenu : public Exception
{
public:
   TLoadMenuResult Code;

   ExceptionLoadMenu( AnsiString inMsg, TLoadMenuResult inCode ) : Exception( inMsg ) { Code = inCode; }
};

//::::::::::::::::::::::::::::::::::::::::::::

class TLoadMenu
{
protected:
    TiXmlDocument *_xmlMenuDoc;

    AnsiString FFileName;
   __int32 FLastGeneratedID;

    TiXmlElement *_rootElem;
    TiXmlElement *_sizesElem;
    TiXmlElement *_categoryGroupsElem;
    TiXmlElement *_allTaxProfilesElem;
    TiXmlElement *_servingCoursesElem;
    TiXmlElement *_coursesElem;
    TiXmlElement *_TPCsElem;   // Third Party Codes

    AnsiString GetMenuVersion();
    void SetMenuVersion( AnsiString inVersion );

    __int32    GetMenuKey();
    AnsiString GetMenuName();
    Menu::TMenuType GetMenuType();
    bool       GetAvailableOnPalm();
    __int32    GetLastGeneratedID() const;

public:
    TLoadMenu( AnsiString inFileName );
    ~TLoadMenu();

    __property  AnsiString MenuVersion    = { read = GetMenuVersion };
    __property  AnsiString FileName       = { read = FFileName };

    __property  __int32         MenuKey         = { read = GetMenuKey };
    __property  AnsiString      MenuName        = { read = GetMenuName };
    __property  Menu::TMenuType MenuType        = { read = GetMenuType };
    __property  bool            AvailableOnPalm = { read = GetAvailableOnPalm };
    __property __int32          LastGeneratedID = { read = GetLastGeneratedID };

    //::::::::::::::::::::::::::::::::::::::::::::::::::::

    __int32 SizeCount();
    __int32 SizeAtIndex( __int32 inIndex, __int32& outKey, WideString& outDescription, WideString& outKitchenName,
                         WideString& outHandheldName, WideString& outReceiptName, bool& outWeighed,
                         __int32& outSizeID, __int32& outPalmID );

    __int32 CategoryGroupCount();
    __int32 CategoryGroupAtIndex( __int32 inIndex, __int32& outKey, WideString& outDescription, bool& outGetDeleted );

    __int32 CategoryCount(   __int32 inCategoryGroupHandle );
    bool    CategoryAtIndex( __int32 inIndex, __int32 inCategoryGroupHandle, __int32& outKey, WideString& outDescription, WideString& outGLCode, bool& outGetDeleted );

    __int32 AllTaxProfilesCount();
    __int32 TaxProfileAtIndex( __int32 inIndex,  __int32& outKey, WideString& outName, Currency& outRate,
                               __int32& outType, __int32& outPriority );

    __int32 ServingCourseCount();
    __int32 ServingCourseAtIndex( __int32 inIndex, __int32& outKey, WideString& outLongDescription, WideString& outKitchenName,
                                  bool& outEnabled, bool& outDeleted, bool& outSelectable, TColor& outColor,__int32& displayOrder );

    __int32 ThirdPartyCodeCount();
    __int32 ThirdPartyCodeAtIndex( __int32 inIndex, __int32& outKey, WideString& outCode, WideString& outDescription, bool& outVisible,
                                   __int32& outCodeType );

     __int32 CourseCount();
     __int32 CourseAtIndex( __int32 inIndex, __int32& outKey, WideString& outDescription, WideString& outKitchenName,
                            WideString& outHandheldName, WideString& outReceiptName, __int32& outLocation, bool& outEnabled,
                            __int32& outServingCourseKey, WideString& outServiceCourse, bool& outNoDefaultServingCourse );

    __int32 ItemCount(   __int32 inCourseHandle );
    __int32 ItemAtIndex( __int32 inIndex, __int32 inCourseHandle, __int32& outKey, WideString& outLongDescription,
                         WideString& outKitchenName, WideString& outHandheldName, WideString& outReceiptName,
                         TColor& outColor, bool& outDisplaySizes, bool& outEnabled, bool& outItemOnlySide,
                         bool& outPrintUnderlined,  bool& outPrintBold, TColor& outPrintColor, __int32& outPrintFont,
                         bool& outPrintDoubleWidth, bool& outPrintDoubleHeight );

    void DisassembleOptionMask( const __int32 inFlags,
                                int& outGroupNumber,
                                int& outMaxSelect,
                                int& outPlusOption,
                                int& outMinusOption,
                                int& outOptionsGroupSkip ) const;

   __int32 OptionCount(   __int32 inCourseID );
	__int32 OptionAtIndex( __int32 inIndex, __int32 inCourseID, __int32& outKey, WideString& outLongDescription,
                           WideString& outKitchenName, WideString& outHandheldName, WideString& outReceiptName,
                           __int32& outCondimentsMask, __int32& outFlags, bool& outEnabled, bool& outPrintUnderlined,
                           bool& outPrintBold, __int32& outPrintColor, __int32& outPrintFont,
                           bool& outPrintDoubleWidth, bool& outPrintDoubleHeight );

    __int32 ForcedSideCount(   __int32 inItemHandle );
    __int32 ForcedSideAtIndex( __int32 inIndex, __int32 inItemHandle, __int32& outKey, __int32& outItemKey,
                               WideString& outLongDescription,
                               __int32& outGroupNumber, __int32& outMaxSelect, bool& outSideGroupSkip );
	   // outKey:      Key of the MasterItem and the Item's relationship.
	   // outItemFKey: Key of the Item as a Side.  The key of the MasterItem is in the ItemAtIndex function

    __int32 ForcedOptionCount(   __int32 inItemHandle );
	 __int32 ForcedOptionAtIndex( __int32 inIndex, __int32 inItemHandle, __int32& outKey,
                                 __int32& outOptionFKey, WideString& outDescription,
                                 __int32& outGroupNumber );
	   // outKey:        Key of the MasterItem and the Option's relationship.
	   // outOptionFKey: Key of the Option.  The key of the MasterItem is in the ItemAtIndex function

    __int32 ItemSizeCount(   __int32 inItemID );
    __int32 ItemSizeAtIndex( __int32 inIndex, __int32 inItemHandle, __int32& outKey, __int32& outSizeFKey, WideString& outLongDescription,
									WideString& outSizeKitchenName, bool& outFree, Currency& outPrice, Currency& outMaxRetailPrice, Currency& outSpecialPrice,
									double& outGST, Currency& outCost, double& outCostGST, double& outPointsPercent, __int32& outMemberPurchaseCount,
									double& outMemberPurchaseDiscount, __int32& outLocationPurchaseCount, double& outLocationPurchaseDiscount, bool& outNoRecipe,
									bool& outIsTaxExempt, bool& outHasServiceCharge, __int32& outSetMenuMask, bool& outAvailableAsStandard, WideString& outBarcode,
									bool& outEnabled, __int32& outCategoryKey, WideString& outCategory, __int32& outThirdPartyCodeKey, double& outTareWeight,
									__int32& outPLU, double &outAvailableQuantity, double &outDefaultQuantity, double &outWarningQuantity,
									bool &outDisableWhenCountReachesZero, bool &outCanBePaidForUsingPoints, int &outDefaultPatronCount,
                                    Currency& outPriceForPoints,int &revenueCode,AnsiString &revenueCodedescription);  // add price for points 

    __int32 BCategoryCount(   __int32 inItemSizeHandle );
    __int32 BCategoryAtIndex( __int32 inIndex, __int32 inItemSizeHandle, __int32& outKey, WideString& outDescription );

    __int32 ItemSizeReceipeCount(   __int32 inItemSizeHandle );
    __int32 ItemSizeReceipeAtIndex( __int32 inIndex, __int32 inItemSizeHandle, __int32& outKey, WideString& outStockCode,
                       double& outQuantity, WideString& outLocation );

    __int32 TaxProfileCount( __int32 inItemSizeHandle );
	 __int32 ItemSizeTaxProfileAtIndex( __int32 inIndex, __int32 inItemSizeHandle, __int32& outKey );

     __int32 PriceLevelCount( __int32 inItemSizeHandle );
	 __int32 ItemSizePriceLevelAtIndex( __int32 inIndex, __int32 inItemSizeHandle, __int32& outKey,Currency& Price,AnsiString& Name );


private:
    TiXmlDocument* createXMLMenuDoc( AnsiString inFileName );

    void loadMainElements();
    TiXmlElement* loadRootElement(  TiXmlDocument* inXMLMenuDoc );
    TiXmlElement* loadSizesElement( TiXmlElement* inRootElement );
    TiXmlElement* loadCategoryGroupsElement( TiXmlElement* inRootElement );
    TiXmlElement* loadAllTaxProfilesElement( TiXmlElement* inRootElement );
    TiXmlElement* loadServiceCoursesElement( TiXmlElement* inRootElement );
    TiXmlElement* loadCoursesElement( TiXmlElement* inRootElement );
    TiXmlElement* loadTPCsElement(    TiXmlElement* inRootElement );
    TiXmlElement* loadElement( AnsiString inElemName, TiXmlElement* inParentElement );

    __int32 childCount( TiXmlElement* inElem );
    WideString convertUTF8CharToWideString( const char* pchars );
    WideString UTF8ToWideString(AnsiString inString);
    //AnsiString WideStringToUTF8(WideString inString);
};

//---------------------------------------------------------------------------
#endif




