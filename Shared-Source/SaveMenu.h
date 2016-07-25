//---------------------------------------------------------------------------
#ifndef SaveMenuH
#define SaveMenuH

#include "MM_Menu.h"

class TiXmlDocument;
class TiXmlElement;

class i_generator;

class TSaveMenu
{
protected:
    TiXmlDocument *_xmlMenuDoc;

    AnsiString FVersion;
    AnsiString FFileName;
    AnsiString FBackupFileName;
    __int32 FLastGeneratedID;

    TiXmlElement *_rootElem;
    TiXmlElement *_sizesElem;
    TiXmlElement *_categoryGroupsElem;
    TiXmlElement *_serviceCoursesElem;
    TiXmlElement *_coursesElem;
    TiXmlElement *_TPCsElem;   // Third Party Codes
    TiXmlElement *_taxProfilesElem;

    AnsiString GetMenuVersion();
    void SetMenuVersion( AnsiString inVersion );

    AnsiString GetMenuName();
    void SetMenuName( AnsiString inName );

    AnsiString GetMenuType();
    void SetMenuType( Menu::TMenuType inType );

    bool GetAvailableOnPalm();
    void SetAvailableOnPalm( bool inAvailable );

   __int32 GetLastGeneratedID() const;
   void SetLastGeneratedID( __int32 inLastGeneratedID );

public:
    TSaveMenu( __int32 inKey, AnsiString inMenuVersion,  AnsiString inFileName,
               AnsiString inBackupFileName, const __int32 inLastGeneratedID );
    ~TSaveMenu();

    __property  AnsiString MenuVersion    = { read = GetMenuVersion, write = SetMenuVersion };
    __property  AnsiString FileName       = { read = FFileName };
    __property  AnsiString BackupFileName = { read = FBackupFileName };

    __property  AnsiString MenuName        = { read = GetMenuName,        write = SetMenuName };
    __property  Menu::TMenuType MenuType   = { read = GetMenuType,        write = SetMenuType };
    __property  bool       AvailableOnPalm = { read = GetAvailableOnPalm, write = SetAvailableOnPalm };
   __property __int32 LastGeneratedID = { read = GetLastGeneratedID, write = SetLastGeneratedID };

    //::::::::::::::::::::::::::::::::::::::::::::::::::::

    __int32 SaveCategoryGroup( __int32 inKey, AnsiString inDescription, bool inGetDeleted );
    void    SaveCategory(      __int32 inCategoryGroupHandle, __int32 inKey, AnsiString inDescription,AnsiString inGLCode, bool inGetDeleted );

    void SaveSize( __int32 inKey, AnsiString inDescription, AnsiString inKitchenName, AnsiString inHandheldName,
                   AnsiString inReceiptName, bool inWeighed, __int32 inSizeID, __int32 inPalmID );

    void SaveServingCourse( __int32 inKey, AnsiString inLongDescription, AnsiString inKitchenName, bool inEnabled,
                             bool inDeleted, bool inSelectable, TColor inColor, __int32 displayOrder );

    void SaveThirdPartyCode( __int32 inKey, AnsiString inCode, AnsiString inDescription, bool inVisible, __int32 inCodeType );

    void SaveTaxProfile( __int32 inKey, AnsiString inName, Currency inTaxRate, __int32 inTaxType, __int32 inPriority );

    __int32 SaveMenuCourse( __int32 inKey, AnsiString inLongDescription, AnsiString inKitchenName,
                            AnsiString inHandheldName, AnsiString inReceiptName, __int32 inLocation,
                            bool inEnabled, __int32 inServingCourseKey, AnsiString inServiceCourse,
                            bool inNoDefaultServingCourse );

    __int32 SaveItem( __int32 inCourseHandle, __int32 inKey, AnsiString inLongDescription,
                      AnsiString inKitchenName, AnsiString inHandheldName, AnsiString inReceiptName,
                      TColor inColor, bool inDisplaySizes, bool inEnabled, bool inItemOnlySide,
                      bool  inPrintUnderlined, bool inPrintBold, TColor inPrintColor,
                      __int32 inPrintFont, bool inPrintDoubleWidth, bool inPrintDoubleHeight );

   __int32 AssembleOptionMask(const int inGroupNumber,
                              const int inMaxSelect,
                              const bool inPlusOption,
                              const bool inMinusOption,
                              const bool inOptionsGroupSkip) const;
 
    void SaveOption( __int32 inCourseHandle, __int32 inKey, AnsiString inLongDescription,
                     AnsiString inKitchenName, AnsiString inHandheldName, AnsiString inReceiptName,
                     __int32 inCondimentsMask, __int32 inFlags, bool inEnabled, bool inPrintUnderlined,
                     bool inPrintBold, __int32 inPrintColor, __int32 inPrintFont,
                     bool inPrintDoubleWidth, bool inPrintDoubleHeight );

    // inKey: Key of the MasterItem and the Item relationship. inItemKey: Key of the Item.  The key of the MasterItem is in the SaveItem function
    void SaveForcedSide( __int32 inItemHandle, __int32 inKey, __int32 inItemFKey,
                         AnsiString inLongDescription,
                         __int32 inGroupNumber, __int32 inMaxSelect, bool inSideGroupSkip );
    // inKey:      Key of the MasterItem and the Item's relationship. 
	// inItemFKey: Key of the Item as a Side.  The key of the MasterItem is in the SaveItem function

     void SaveMenuItemForcedOption( __int32 inItemHandle, __int32 inKey, __int32 inOptionFKey,
                                    AnsiString inDescription, __int32 inGroupNumber );

    // inItemSizeKey: Key of the MasterItem and the Size's relationship.
	// inSizeFKey: Key of the Size. The key of the MasterItem is in the SaveItem function
    __int32 SaveItemSize( __int32 inItemHandle, __int32 inItemSizeKey, __int32 inSizeFKey, AnsiString inLongDescription, /*MM1351*/AnsiString inKitchenName,
                          bool inFree, Currency inPrice, Currency inMaxRetailPrice, Currency inSpecialPrice,
                          double inGST, Currency inCost, Currency inCostGST,
                          double inPointsPercent,  double inMemberPurchaseCount,
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
                          Currency inPriceForPoints); // add price for points.

     void SaveBCategory( __int32 inItemSizeHandle, __int32 inKey, AnsiString inName );

     void SaveItemSizeRecipe( __int32 itemSizeHandle, __int32 inKey, AnsiString inStockCode,
                              double  inQuantity, AnsiString inLocation );

     void SaveItemSizeTaxProfile( __int32 inItemSizeHandle, __int32 inKey, AnsiString inName );
     void SaveItemSizePriceLevels( __int32 inItemSizeHandle, __int32 inKey, AnsiString inName,Currency Price );

     bool Commit();

private:
    TiXmlDocument* createXMLMenuDoc( __int32 inKey, AnsiString inMenuVersion );

    void addMainElements();
    void addElement(  TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem );
    void setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue );

    TiXmlElement* findChildElement( TiXmlElement* inParentElem, AnsiString inChildElementName );

};

//---------------------------------------------------------------------------
#endif
