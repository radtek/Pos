//---------------------------------------------------------------------------

#ifndef ImportMenuH
#define ImportMenuH

#include <IBDatabase.hpp>
#include <map>

#include "LoadMenu.h"
#include "ManagerMenus.h"

//---------------------------------------------------------------------------

// temporary xml key, atual DB key.
typedef std::map<__int32, __int32> DB_KEY_MAP;

// Element Name, DB_KEY_MAP.
// e.g Course Name, Size Name, Item Name etc.
typedef std::map<AnsiString, DB_KEY_MAP> DB_ELEMENT_KEY_MAP;

// simple vector to hold collection of DB keys
typedef std::vector<__int32> DB_KEY_VECTOR;

//---------------------------------------------------------------------------

// stores information about serving course's print order related to a menu
class TServingCoursePrintOrder
{
    public:
        __int32 PRNOrderKey;
        __int32 MenuKey;
        __int32 CourseKey;
        UnicodeString CourseName;
        __int32 VirtualPrinterKey;
        __int32 RedirectFrom;
};

//---------------------------------------------------------------------------

class TImportMenu
{
	public:
		 TImportMenu( TIBDatabase *inIBDatabase );
		~TImportMenu();


		bool ImportFromXMLFilesInDir( AnsiString inDirName  );
		bool ImportFromXMLFile(       AnsiString inFileName );

		void       ResetErrorLog();
		__int32    FailedToLoadAMenu();
        void       AddErrorLog( AnsiString inLog );
		AnsiString ErrorLog();

	protected:
		TIBDatabase              *ibDatabase;
        Database::TDBTransaction *dbTransaction;

        AnsiString   textLog;
		TStringList *errorLog;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Key Map: It maps new keys from the XML file with actual DB keys.
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		DB_ELEMENT_KEY_MAP _keyMap;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Item Key Map: It maps item DB keys (itemKey) with item XML file Handles (itemXMLHandle).
        // It's used to read the Forced Sides from the XML file.
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		DB_KEY_MAP _itemMap;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Existing item keys set. this is used to see whether a generated id is already exists in the database.
        // This is a rare situation when dealing with older menumate databases
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        std::set<__int32> _existingItemKeys;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // A map to hold newly created course key and its name. This is used later
        // when inserting course print orders back to the database
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        std::map<UnicodeString, __int32> _courseNameKeyMap;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // This map will contain item keys that is being used in the current orders table
        // We can use this to re-use item keys so it will not break the relationship between orders and the items tables.
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        std::map<__int32,__int32> _ordersItemsKeyMap;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // This map will contain item size keys that maches to item keys, which are being used in the current orders table
        // We can use this to re-use item size keys so it will not break the relationship between orders, items and the item_size tables.
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        std::map<__int32, DB_KEY_VECTOR> _ordersItemSizesKeyMap;

        __int32 _maxItemSizePLU;

		__int32 _newSAO;
		__int32 _newDisplayOrder;
		__int32 _newCAO;
		__int32 _newOptionOrder;
		__int32 _newIAO;
		__int32 _newIOO;
		__int32 _newISAO;
		__int32 _newCOO;

		//::::::::::::::::::::::::::::
		//  Protected.
		//::::::::::::::::::::::::::::

        bool menuExists( AnsiString inMenuName );
        bool deleteExistingMenu( AnsiString inMenuName );

		//::::::::::::::::::::::::::::

		bool ImportNewMenuFromXMLFile( TLoadMenu* inMenu );
		bool ImportExistingMenuFromXMLFile( TLoadMenu* inMenu );

		//::::::::::::::::::::::::::::

		bool CreateMenuInDB( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		bool SaveMenuInDB(   TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );

		//::::::::::::::::::::::::::::

		void SaveSizesFromMenu( __int32 inMenuKey, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveCategoryGroupsFromMenu( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveCategoriesFromMenu( __int32 inCGXMLHandle, __int32 inCGroupKey, TLoadMenu* inMenu,
									 Database::TDBTransaction *inDBTransaction );
		void SaveTaxProfilesFromMenu( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveServingCoursesFromMenu( __int32 inMenuKey, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveThirdPartyCodesFromMenu( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveCoursesFromMenu( __int32 inMenuKey, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveOptionsFromMenu( __int32 inCourseID, __int32 inCourseKey, TLoadMenu* inMenu,
								 Database::TDBTransaction *inDBTransaction );
		void SaveItemsFromMenu( __int32 inCourseID, __int32 inCourseKey, TLoadMenu* inMenu,
							   Database::TDBTransaction *inDBTransaction );
		void SaveForcedSidesFromMenu( __int32 inItemID, __int32 inMasterItemKey, TLoadMenu* inMenu,
									 Database::TDBTransaction *inDBTransaction );
		void SaveItemSizesFromMenu( __int32 inItemID, __int32 inMasterItemKey, TLoadMenu* inMenu,
								   Database::TDBTransaction *inDBTransaction );
		void SaveBCategoriesFromMenu( __int32 inItemSizeID, __int32 inItemSizeKey, TLoadMenu* inMenu,
									 Database::TDBTransaction *inDBTransaction );
		void SaveItemSizeReceipesFromMenu( __int32 inItemSizeID, __int32 inItemSizeKey, TLoadMenu* inMenu,
		                                  Database::TDBTransaction *inDBTransaction );
		void SaveItemSizeTaxProfilesFromMenu( __int32 inItemSizeID, __int32 inItemSizeKey, TLoadMenu* inMenu,
		                                     Database::TDBTransaction *inDBTransaction );

       	void SaveItemSizePriceLevelsFromMenu( __int32 inItemSizeID, __int32 inItemSizeKey, TLoadMenu* inMenu,
		                                     Database::TDBTransaction *inDBTransaction );

		//::::::::::::::::::::::::::::

		void SaveSizeFromMenu( __int32 inIndex, __int32 inMenuKey, TLoadMenu* inMenu,
							  Database::TDBTransaction *inDBTransaction );
		void SaveCategoryGroupFromMenu( __int32 inIndex, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveCategoryFromMenu( __int32 inIndex, __int32 inCGXMLHandle, __int32 inCGroupKey,
		                           TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveTaxProfileFromMenu( __int32 inIndex, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveServingCourseFromMenu( __int32 inIndex, __int32 inMenuKey, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveThirdPartyCodeFromMenu( __int32 inIndex, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveCourseFromMenu( __int32 inIndex, __int32 inMenuKey, TLoadMenu* inMenu,
		                                     Database::TDBTransaction *inDBTransaction );
		void SaveOptionFromMenu( __int32 inIndex, __int32 inCourseID, __int32 inCourseKey, TLoadMenu* inMenu,
								Database::TDBTransaction *inDBTransaction );
		void SaveItemFromMenu( __int32 inIndex, __int32 inCourseID, __int32 inCourseKey, TLoadMenu* inMenu,
							  Database::TDBTransaction *inDBTransaction );
		void SaveAllForcedSidesFromMenu( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveForcedSideFromMenu( __int32 inIndex, __int32 inItemHandle, __int32 inMasterItemKey,
									TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveForcedOptionFromMenu( __int32 inIndex, __int32 inItemHandle, __int32 inMasterItemKey,
									  TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveItemSizeFromMenu( __int32 inIndex, __int32 inItemID, __int32 inMasterItemKey,
								  TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveBCategoryFromMenu( __int32 inIndex, __int32 inItemSizeID, __int32 inItemSizeKey,
								   TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveItemSizeReceipeFromMenu( __int32 inIndex, __int32 inItemSizeID, __int32 inItemSizeKey,
		                                 TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
		void SaveItemSizeTaxProfileFromMenu( __int32 inIndex, __int32 inItemSizeID, __int32 inItemSizeKey,
		                                    TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );
        void SaveItemSizePriceLevelFromMenu( __int32 inIndex, __int32 inItemSizeXMLHandle, __int32 inItemSizeKey,
											     TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );


		//::::::::::::::::::::::::::::

        bool menuKeyExists(            __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool sizeKeyExists(            __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool categoryGroupKeyExists(   __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool categoryKeyExists(        __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool servingCourseKeyExists(   __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool thirdPartyCodeKeyExists(  __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool courseKeyExists(          __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool optionKeyExists(          __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool itemKeyExists(            __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool forcedSideKeyExists(      __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool forcedOptionKeyExists(    __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool itemSizeKeyExists(        __int32 inKey, Database::TDBTransaction *inDBTransaction );
        bool itemSizeReceipeKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction );

        bool keyExists(
                   __int32 inKey,
                AnsiString inKeyName,
                AnsiString inTableName,
                Database::TDBTransaction *inDBTransaction );

        //::::::::::::::::::::::::::::

        bool menuExistsInDB(
                AnsiString inMenuName,
                Database::TDBTransaction *inDBTransaction );

        bool deleteExistingMenuInDB(
                AnsiString inMenuName,
                Database::TDBTransaction *inDBTransaction );

        void getServingCourseKeyIfExists(
                AnsiString inServingCourseName,
                __int32 *outServingCourseKey,
                Database::TDBTransaction *inDBTransaction);

        void getCategoryGroupKeyIfExists(
                AnsiString inCategoryGroupName,
                __int32 *outCategoryGroupKey,
                Database::TDBTransaction *inDBTransaction);

        void getCategoryKeyIfExists(
                AnsiString inCategoryName,
                __int32 *outCategoryKey,
                Database::TDBTransaction *inDBTransaction);

        //::::::::::::::::::::::::::::

        //::::::::::::::::::::::::::::::::::::::::::::::
        // Insert_InDB functions insert new objects in th DB.
        // Insert_InDBWithKey functions are not used so far!!!!.
        //::::::::::::::::::::::::::::::::::::::::::::::
		__int32 InsertMenuInDB( WideString inMenuName, Menu::TMenuType inMenuType,
								bool inAvailableOnPalm, Database::TDBTransaction *inDBTransaction );
		   void InsertMenuInDBWithKey( __int32 inKey, WideString inMenuName, Menu::TMenuType inMenuType,
								bool inAvailableOnPalm, Database::TDBTransaction *inDBTransaction );

		__int32 InsertSizeInDB( __int32 inMenuKey, WideString inSizeName, __int32 inSizeID,
								__int32 inPalmSizeID, __int32 inSAO, WideString inKitchenName,
								bool inWeighed, WideString inHandheldName, WideString inReceiptName,
								Database::TDBTransaction *inDBTransaction );
 		   void InsertSizeInDBWithKey( __int32 inKey, __int32 inMenuKey, WideString inSizeName, __int32 inSizeID,
                                       __int32 inPalmSizeID, __int32 inSAO, WideString inKitchenName,
                                       bool inWeighed, WideString inHandheldName, WideString inReceiptName,
                                       Database::TDBTransaction *inDBTransaction );

		__int32 InsertCategoryGroupInDB( WideString inDescription, bool inDeleted,
										Database::TDBTransaction *inDBTransaction );
		   void InsertCategoryGroupInDBWithKey( __int32 inKey, WideString inDescription, bool inDeleted,
										Database::TDBTransaction *inDBTransaction );

		__int32 InsertCategoryInDB( WideString inCategory,WideString inGLCode, bool inVisible, __int32 inCGroupKey,
		                            __int32 inCOO, Database::TDBTransaction *inDBTransaction );
		   void InsertCategoryInDBWithKey( __int32 inKey, WideString inCategory,WideString inGLCode, bool inVisible, __int32 inCGroupKey,
		                            __int32 inCOO, Database::TDBTransaction *inDBTransaction );

		__int32 InsertServingCourseInDB( WideString inDescription, WideString inKitchenName,
										bool inSCDeleted, bool inSelectable, TColor inColor,
										__int32 inDisplayOrder, Database::TDBTransaction *inDBTransaction );
		   void InsertServingCourseInDBWithKey( __int32 inKey, WideString inDescription, WideString inKitchenName,
										bool inSCDeleted, bool inSelectable, TColor inColor,
										__int32 inDisplayOrder, Database::TDBTransaction *inDBTransaction );

		__int32 InsertMenu_SCInDB( __int32 inMenuKey, __int32 inKey, Database::TDBTransaction *inDBTransaction );

		__int32 InsertThirdPartyCodeInDB( WideString inCode, WideString inDescription,
										bool inVisible, __int32 inCodeType,
										Database::TDBTransaction *inDBTransaction );
        void UpdateThirdPartyCodeInDBWithKey(
                                __int32    inKey,
                                WideString inCode,
                                WideString inDescription,
                                bool       inVisible,
                                __int32    inCodeType,
                                Database::TDBTransaction *inDBTransaction );
       void InsertThirdPartyCodeInDBWithKey( __int32 inKey, WideString inCode, WideString inDescription,
                                    bool inVisible, __int32 inCodeType,
                                    Database::TDBTransaction *inDBTransaction );

		__int32 InsertCourseInDB( __int32 inMenuKey, WideString inDescription, WideString inKitchenName,
								  WideString inHandheldName, WideString inReceiptName, __int32 inLocation,
								  bool inEnabled, __int32 inServingCourseKey, WideString inServiceCourse,
								  bool inNoDefaultServingCourse, __int32 inCAO,
								  Database::TDBTransaction *inDBTransaction );
		   void InsertCourseInDBWithKey( __int32 inKey, __int32 inMenuKey, WideString inDescription, WideString inKitchenName,
								  WideString inHandheldName, WideString inReceiptName, __int32 inLocation,
								  bool inEnabled, __int32 inServingCourseKey, WideString inServiceCourse,
								  bool inNoDefaultServingCourse, __int32 inCAO,
								  Database::TDBTransaction *inDBTransaction );

		__int32 InsertOptionInDB( __int32 inCourseKey, WideString inDescription, WideString inKitchenName,
								  WideString inHandheldName, WideString inReceiptName,
								  __int32 inCondimentsMask, __int32 inFlags, bool inEnabled, bool inPrintUnderlined,
								  bool inPrintBold, __int32 inPrintColor, __int32 inPrintFont,
								  bool inPrintDoubleWidth, bool inPrintDoubleHeight, __int32 inOptionOrder,
								  Database::TDBTransaction *inDBTransaction );
		   void InsertOptionInDBWithKey( __int32 inKey, __int32 inCourseKey, WideString inDescription, WideString inKitchenName,
								  WideString inHandheldName, WideString inReceiptName,
								  __int32 inCondimentsMask, __int32 inFlags, bool inEnabled, bool inPrintUnderlined,
								  bool inPrintBold, __int32 inPrintColor, __int32 inPrintFont,
								  bool inPrintDoubleWidth, bool inPrintDoubleHeight, __int32 inOptionOrder,
								  Database::TDBTransaction *inDBTransaction );

		__int32 InsertItemInDB( __int32 inCourseKey, WideString inDescription,
								WideString inKitchenName, TColor inButtonColor, bool inDisplaySizes,
								bool inEnabled, bool inItemOnlySide,
								bool inPrintUnderlined, bool inPrintBold, TColor inPrintColor,
								__int32 inPrintFont, bool inPrintDoubleWidth, bool inPrintDoubleHeight,
								__int32 inIAO,
								Database::TDBTransaction *inDBTransaction, WideString inHandheldName );
		   void InsertItemInDBWithKey( __int32 inKey, __int32 inCourseKey, WideString inDescription,
								WideString inKitchenName, TColor inButtonColor, bool inDisplaySizes,
								bool inEnabled, bool inItemOnlySide,
								bool inPrintUnderlined, bool inPrintBold, TColor inPrintColor,
								__int32 inPrintFont, bool inPrintDoubleWidth, bool inPrintDoubleHeight,
								__int32 inIAO,
								Database::TDBTransaction *inDBTransaction, WideString inHandheldName );

		__int32 InsertForcedSideInDB( __int32 inMasterItemKey, __int32 inItemKey, __int32 inIOO,
									  __int32 inGroupNumber, __int32 inMaxSelect, bool inSideGroupSkip,
									  Database::TDBTransaction *inDBTransaction );
		   void InsertForcedSideInDBWithKey( __int32 inKey, __int32 inMasterItemKey, __int32 inItemKey, __int32 inIOO,
									  __int32 inGroupNumber, __int32 inMaxSelect, bool inSideGroupSkip,
									  Database::TDBTransaction *inDBTransaction );

		__int32 InsertForcedOptionInDB( __int32 inMasterItemKey, __int32 itemKey,
										Database::TDBTransaction *inDBTransaction );
		   void InsertForcedOptionInDBWithKey( __int32 inKey, __int32 inMasterItemKey, __int32 itemKey,
										Database::TDBTransaction *inDBTransaction );

		__int32 InsertItemSizeInDB(
						__int32    inMasterItemKey,
						__int32    inSizeKey,
						__int32    inItemID,
						__int32    inSizeID,
						__int32    inPalmItemID,
						__int32    inPalmSizeID,
						WideString inSizeName,
						Currency   inPrice,
                        Currency   inMaxRetailPrice,
						Currency   inSpecialPrice,
						Currency   inCost,
						__int32    inISAO,
						bool       inFree,
						bool       inAvailableAsStandard,
						bool       inNoRecipe,
						WideString inBarcode,
						__int32    inSetMenuMask,
						bool       inEnabled,
						double     inGSTPercent,
						double     inCostGSTPercent,
						double     inPointsPercent,
						WideString inSizeKitchenName,
						__int32    inThirdPartyCodeKey,
						double     inMemberPurchaseDiscount,
						double     inLocationPurchaseDiscount,
						__int32    inMemberPurchaseCount,
						__int32    inLocationPurchaseCount,
						__int32    inCategoryKey,
						double     inTareWeight,
						WideString inHandheldName,
						WideString inReceiptName,
						__int32    inPLU,
						Currency   inAvailableQuantity,
						Currency   inDefaultQuantity,
						Currency   inWarningQuantity,
						bool       inDisableWhenCountReachesZero,
                        bool       inCanBePaidUsingPoints,
                        int        inDefaultPatronCount,
                        Currency   inPriceForPoints,
						Database::TDBTransaction *inDBTransaction );

		   void InsertItemSizeInDBWithKey(
                        __int32    inKey,
                        __int32    inMasterItemKey,
						__int32    inSizeKey,
						__int32    inItemID,
						__int32    inSizeID,
						__int32    inPalmItemID,
						__int32    inPalmSizeID,
						WideString inSizeName,
						Currency   inPrice,
                        Currency   inMaxRetailPrice,
						Currency   inSpecialPrice,
						Currency   inCost,
						__int32    inISAO,
						bool       inFree,
						bool       inAvailableAsStandard,
						bool       inNoRecipe,
						WideString inBarcode,
						__int32    inSetMenuMask,
						bool       inEnabled,
						double     inGSTPercent,
						double     inCostGSTPercent,
						double     inPointsPercent,
						WideString inSizeKitchenName,
						__int32    inThirdPartyCodeKey,
						double     inMemberPurchaseDiscount,
						double     inLocationPurchaseDiscount,
						__int32    inMemberPurchaseCount,
						__int32    inLocationPurchaseCount,
						__int32    inCategoryKey,
						double     inTareWeight,
						WideString inHandheldName,
						WideString inReceiptName,
						__int32    inPLU,
						Currency   inAvailableQuantity,
						Currency   inDefaultQuantity,
						Currency   inWarningQuantity,
						bool       inDisableWhenCountReachesZero,
                        bool       inCanBePaidUsingPoints,
                        int        inDefaultPatronCount,
                        Currency   inPriceForPoints,
						Database::TDBTransaction *inDBTransaction );

		__int32 InsertBCategoryInDB( __int32 inItemSizeKey, __int32 inCategoryKey,
									Database::TDBTransaction *inDBTransaction );

		__int32 InsertItemSizeReceipeInDB( __int32 inItemSizeKey, WideString inStockCode,
										   double inQuantity, WideString inLocation,
										   Database::TDBTransaction *inDBTransaction  );
		   void InsertItemSizeReceipeInDBWithKey( __int32 inKey, __int32 inItemSizeKey, WideString inStockCode,
										   double inQuantity, WideString inLocation,
										   Database::TDBTransaction *inDBTransaction  );

		__int32 InsertItemSizeTaxProfileInDB( __int32 inItemSizeKey, __int32 inTaxProfileKey,
										      Database::TDBTransaction *inDBTransaction  );
		   void InsertItemSizeTaxProfileInDBWithKey( __int32 inKey, __int32 inItemSizeKey, __int32 inTaxProfileKey,
										      Database::TDBTransaction *inDBTransaction  );
       void InsertItemSizePriceLevelInDB(
                         __int32 inItemSizeKey, __int32  PriceLevelKey,Currency Price,AnsiString Name,
                                      Database::TDBTransaction *inDBTransaction  );


		//::::::::::::::::::::::::::::

		// Update_InDB functions update objects in the DB.
		bool UpdateMenuInDB( __int32 inKey, AnsiString inMenuName, Menu::TMenuType inMenuType,
							bool inAvailableOnPalm, Database::TDBTransaction *inDBTransaction );
		bool UpdateSizeInDB( __int32 inKey, __int32 inMenuKey, WideString inDescription,
							 WideString inKitchenName, WideString inHandheldName,
							 WideString inReceiptName, bool inWeighted, __int32 inSizeID,
							 __int32 inPalmID, Database::TDBTransaction* inDBTransaction );
		bool UpdateCategoryGroupInDB( __int32 inKey, WideString inDescription,
									  bool inCGDeleted, Database::TDBTransaction *inDBTransaction );
		__int32 UpdateCategoryInDB( __int32 inKey, WideString inDescription,WideString inGLCode, bool inDeleted,
									__int32 inCGroupKey, Database::TDBTransaction *inDBTransaction );
		__int32 UpdateServingCourseInDB( __int32 inKey, WideString inDescription, WideString inKitchenName,
										 bool inSCDeleted, bool inSelectable,
										 TColor inColor, __int32 inDisplayOrder, Database::TDBTransaction *inDBTransaction );

		__int32 UpdateThirdPartyCodeInDB( WideString inCode, WideString inDescription,
										  bool inVisible, __int32 inCodeType,
										  Database::TDBTransaction* inDBTransaction );
		__int32 UpdateCourseInDB( __int32 inCourseKey, __int32 inMenuKey, WideString inDescription,
								  WideString inKitchenName, WideString inHandheldName, WideString inReceiptName,
								  __int32 inLocation, bool inEnabled, __int32 inServingCourseKey,
								  bool inNoDefaultServingCourse, Database::TDBTransaction* inDBTransaction );
		__int32 UpdateOptionInDB( __int32 inOptionKey, __int32 inCourseKey, WideString inDescription,
								  WideString inKitchenName, WideString inHandheldName, WideString inReceiptName,
								  __int32 inCondimentsMask, bool inFlags, bool inEnabled, bool inPrintUnderlined,
								  bool inPrintBold, __int32 inPrintColor, __int32 inPrintFont,
								  bool inPrintDoubleWidth, bool inPrintDoubleHeight,
								  Database::TDBTransaction* inDBTransaction );
		__int32 UpdateItemInDB( __int32 inItemKey, __int32 inCourseKey, WideString inDescription,
								WideString inKitchenName, __int32 inButtonColor, bool inDisplaySizes,
								bool inEnabled, bool inItemOnlySide, bool inPrintUnderlined,
								bool inPrintBold, __int32 inPrintColor, __int32 inPrintFont,
								bool inPrintDoubleWidth, bool inPrintDoubleHeight,
								Database::TDBTransaction* inDBTransaction );
		__int32 UpdateForcedSideInDB( __int32 inForcedSideKey, __int32 inItemFKey,
									  __int32 inSideFKey, __int32 inGroupNumber,
									  __int32 inMaxSelect, bool inSideGroupSkip,
									  Database::TDBTransaction* inDBTransaction );
		__int32 UpdateForcedOptionInDB( __int32 inForcedOptionKey, __int32 inMasterItemKey,
										__int32 inOptionKey, Database::TDBTransaction* inDBTransaction );

		__int32 UpdateItemSizeInDB(
						__int32    inItemSizeKey,
						__int32    inMasterItemKey,
						__int32    inSizeKey,
						__int32    inItemID,
						__int32    inSizeID,
						__int32    inPalmItemID,
						__int32    inPalmSizeID,
						WideString inSizeName,
						Currency   inPrice,
						Currency   inSpecialPrice,
						Currency   inCost,
						bool       inFree,
						bool       inAvailableAsStandard,
						bool       inNoRecipe,
						WideString inBarcode,
						__int32    inSetMenuMask,
						bool       inEnabled,
						double     inGSTPercent,
						double     inCostGSTPercent,
						double     inPointsPercent,
						WideString inSizeKitchenName,
						__int32    inThirdPartyCodeKey,
						double     inMemberPurchaseDiscount,
						double     inLocationPurchaseDiscount,
						__int32    inMemberPurchaseCount,
						__int32    inLocationPurchaseCount,
						__int32    inCategoryKey,
						double     inTareWeight,
						WideString inHandheldName,
						WideString inReceiptName,
						__int32    inPLU,
						/* ASEAN */ /* bool inTaxExempt, */
						/* ASEAN */ /* bool inHasServiceCharge, */
						Currency inAvailableQuantity,
						Currency inDefaultQuantity,
						Currency inWarningQuantity,
						bool inDisableWhenCountReachesZero,
                        bool inCanBePaidUsingPoints,
                        Currency   inPriceForPoints,
						Database::TDBTransaction *inDBTransaction );

		__int32 UpdateItemSizeReceipeInDB( __int32 inReceipeKey, __int32 inItemSizeKey,
										   WideString inStockCode, double inQuantity, WideString inLocation,
										   Database::TDBTransaction* inDBTransaction );

		//::::::::::::::::::::::::::::

		//Tool functions used when saving menus in DB from an xml file
		bool resetNextNewSAO( __int32 inMenuKey, Database::TDBTransaction *inDBTransaction );
		__int32 getNextNewSAO();

		bool resetNextNewDisplayOrder( Database::TDBTransaction *inDBTransaction );
		__int32 getNextNewDisplayOrder();

		bool resetNextNewCAO( __int32 inMenuKey, Database::TDBTransaction *inDBTransaction );
		__int32 getNextNewCAO();

		bool resetNextNewOptionOrder( __int32 inCourseKey, Database::TDBTransaction *inDBTransaction );
		__int32 getNextNewOptionOrder();

		bool resetNextNewIAO( __int32 inCourseKey, Database::TDBTransaction *inDBTransaction );
		__int32 getNextNewIAO();

		bool resetNextNewIOO( __int32 inMasterItemKey, Database::TDBTransaction *inDBTransaction);
		__int32 getNextNewIOO();

		bool resetNextNewISAO( __int32 inItemKey, Database::TDBTransaction *inDBTransaction );
		__int32 getNextNewISAO();

		bool resetNextNewCOO( __int32 inCGroupKey, Database::TDBTransaction *inDBTransaction );
		__int32 getNextNewCOO();

		void MoveMenuFile( AnsiString inFileName, AnsiString inDestPath );

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Key Map: It maps new keys from the XML file with actual DB keys.
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		void    ClearKeyMap();
		void    AddKeyMap(
                    AnsiString inMapName,
                    __int32 inTmpKey,
                    __int32 inActualKey );
		__int32 GetKeyMap(
                     AnsiString inMapName,
                     __int32 inTmpKey );

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Item Key Map: It maps item DB keys (itemKey) with item XML file Handles (itemXMLHandle).
        // It's used to read the Forced Sides from the XML file.
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        void    ClearItemMap();
		void    AddItemMap(
                    __int32 inItemKey,
                    __int32 inItemXMLHandle );
		__int32 GetItemMap( __int32 inItemKey );

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Existing item keys set. this is used to see whether a generated id is already exists in the database.
        // This is a rare situation when dealing with older menumate databases
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        void    ClearExistingItemKeys();
		void    LoadExistingItemKeys( __int32 latestGeneratedItemID, Database::TDBTransaction *inDBTransaction );
		bool    IsItemKeyExists( __int32 inItemKey );

        // gets the item id from generator. The call to database refactored into this function because it is needed in several places
        __int32 GetLatestItemIDFromGenerator(Database::TDBTransaction *inDBTransaction);


        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // A map to hold newly created course key and its name. This is used later when inserting course print orders back to the database
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        void    ClearCourseNameKeyMap();
		void    AddCourseNameKeyMap(
                    UnicodeString inCourseName,
                    __int32 inCourseKey );
		__int32 GetCourseNameKeyMap( UnicodeString inCourseName );

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Existing serving course print orders related to a menu. We need to populate this structure before deleting the menu so depending on the
        // serving course name we can insert records back into the database which fixes the issue at case #26135
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        std::vector<TServingCoursePrintOrder *> ExistingServingCoursePrintOrders;
        void    ClearExistingServingCoursePrintOrders();
        void    PopulateExistingServingCoursePrintOrders( AnsiString menuName );
        void    ReInsertServingCoursePrintOrdersInDB( Database::TDBTransaction *inDBTransaction, __int32 inMenuKey );
        __int32 GetExistingMenuKeyByName(Database::TDBTransaction *inDBTransaction, AnsiString menuName );


        // common function to get the next id from specified generator
        __int32 GetLatestIDFromGenerator(Database::TDBTransaction *inDBTransaction, AnsiString generator_name, __int32 incrementor);

        bool ThirdPartyCodeExists(
                Database::TDBTransaction *inDBTransaction,
                WideString inCode );
        __int32 GetTPCKey(
                    Database::TDBTransaction *inDBTransaction,
                    WideString inCode );

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Removes existing thirdparty codes related to items in the current menu
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        void RemoveExistingThirdPartyCodes( AnsiString inMenuName );

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Methods that deals with populating, retrieving currently used item keys from the orders table
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        void ClearOrdersItemsKeyMap();
		bool IsItemKeyAlreadyBeingUsedForOrders( __int32 inItemKey );
        void PopulateOrdersItemsKeyMap(
                    Database::TDBTransaction *inDBTransaction,
                    AnsiString inMenuName);

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Allocates new plu numbers for item sizes
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        void ResetMaxItemSizePLU(
                    Database::TDBTransaction *inDBTransaction);
        int GetMaxItemSizePLU(
                    Database::TDBTransaction *inDBTransaction);
        int GetNextItemSizePLU();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Methods that deals with populating, retrieving currently used item size keys from the orders table
        /*
         * Re-using item_size_keys was implemented to get around the below issue with PCD.(Product Count Down)
         * When a menu is loaded in to the pos, PCD will load them into memory.
         * We then ring up an item and create a parked/ saved sale.
         * Without restarting POS, we commit the same menu again into POS
         * Since we have parked sales, POS will import this menu only into DB, but not to the memory, and the selectDish page's controls will still refer to old item_size_keys
         * Upon clicking on an item size, it will use the item_size_key from the control( button tag ) and passes it to PCD to get some information
         * Since PCD has the new item_size_keys, it will throw an error
         * By re-using the item_size_keys if we have saved/parked sales at the menu import time, we can get around this issue
         * Refer to ISSUE MM-230
         */
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        void ClearOrdersItemSizesKeyMap();
        bool IsItemSizeKeyAlreadyBeingUsedForOrders(
                                        __int32 parentItemKey,
                                        __int32 inItemSizeKey );
        void PopulateOrdersItemSizeKeysFromItemKeys( Database::TDBTransaction *inDBTransaction);
        void PopulateOrdersItemSizesKeyMap(
                    Database::TDBTransaction *inDBTransaction,
                    int parentItemKey);

        Currency PricelevelZero;
        void SaveAllForcedOptionsFromMenu( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction );

};

//---------------------------------------------------------------------------

#endif

