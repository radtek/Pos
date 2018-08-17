//---------------------------------------------------------------------------


#pragma hdrstop

#include <vcl.h>
#include <system.hpp>

#include "Enum.h"
#include "ImportMenu.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "TaxProfile.h"
#include "TaxProfileDBAccessManager_MM.h"

//#pragma link "IBDatabase"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TImportMenu::TImportMenu( TIBDatabase *inIBDatabase )
{
	ibDatabase    = inIBDatabase;
    dbTransaction = new Database::TDBTransaction( ibDatabase );
	errorLog      = new TStringList();
}
//...........................................................................
TImportMenu::~TImportMenu()
{
   delete errorLog;
   delete dbTransaction;
}
//...........................................................................
bool TImportMenu::ImportFromXMLFilesInDir( AnsiString inDirName )
{
	// Returns  TRUE if at least one menu has been loaded
	// Returns FALSE if no menu has been loaded.

	bool result = false;

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	UnicodeString path = ExtractFilePath( Application->ExeName ) + "Menu Import\\";

	TSearchRec sr;
	int iAttributes = faAnyFile;

	ClearKeyMap();
    ClearItemMap();
    ClearExistingItemKeys();

	if( FindFirst( path + "*.xml", iAttributes, sr ) == 0 )
	{
		if (sr.Attr & iAttributes)
		{
			result = ImportFromXMLFile( path + sr.Name);
		}
		while( FindNext(sr) == 0 )
		{
		   if( sr.Attr & iAttributes )
		   {
			  result = result || ImportFromXMLFile( path + sr.Name );
		   }
		}
	}
	FindClose(sr);
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................
bool TImportMenu::ImportFromXMLFile( UnicodeString inFileName )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    bool transactionStarted = false;

    try
    {
	    std::auto_ptr<TLoadMenu> loadMenu( new TLoadMenu( inFileName ) );

        if( loadMenu->MenuVersion == MENU_IMPORT_VERSION )
        {
            // Start Transaction is only called at this point
            // so we can rollback in a event of an error
            dbTransaction->StartTransaction();
            transactionStarted = true;
            //...................................................

            ClearExistingServingCoursePrintOrders();
            ClearOrdersItemsKeyMap();
            ClearOrdersItemSizesKeyMap();
            ResetMaxItemSizePLU(dbTransaction);

            // All menus are imported as if they are new.
            if( menuExists( loadMenu->MenuName ) )
            {
                PopulateExistingServingCoursePrintOrders ( loadMenu->MenuName );

                // We remove thirdparty codes related to items in here because
                // there is no foreign key between the itemsize and thirdparty
                // codes nor between menu and thirdparty codes. We can add a
                // foreign key to thirdparty codes table so it contain the menu
                // key but at this point I'm not very sure whether thats a good
                // idea so will proceed with deletion within the code itself
                // NOT LONGER USED !!!
                // RemoveExistingThirdPartyCodes( loadMenu->MenuName );

                deleteExistingMenu( loadMenu->MenuName );

                PopulateOrdersItemsKeyMap(
                                    dbTransaction,
                                    loadMenu->MenuName);

                // loads up all item size keys associated with item keys in _ordersItemsKeyMap
                PopulateOrdersItemSizeKeysFromItemKeys( dbTransaction );
            }

            // KeyMap: Maps the element's keys from the XML file with the actual keys
            // in the database.
            ClearKeyMap();
            ClearItemMap();
            ClearCourseNameKeyMap();

            result = ImportNewMenuFromXMLFile( loadMenu.get() );
        }
        else
        {
            textLog = "The file format is invalid: " + inFileName + ".";
            TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
            AddErrorLog( textLog );
        }
    }
    catch( Exception& exc )
    {
        textLog = inFileName +  ". The menu load process failed: " + exc.Message;
        TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
        AddErrorLog( textLog );
    }

    //.....................................................................

	if( result )
	{
        // Commit Transaction is only called at this point
        // so we can rollback in a event of an error
        dbTransaction->Commit();
        //...................................................
	    MoveMenuFile( inFileName, ExtractFilePath( Application->ExeName ) + "Menu Import\\Old Menus\\" );
	}
	else
	{
        // Rollback transaction .............................
        if( transactionStarted )
        {
            dbTransaction->Rollback();
        }
        //...................................................
	   MoveMenuFile( inFileName, ExtractFilePath( Application->ExeName ) + "Menu Import\\Failed\\" );
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................
void TImportMenu::ResetErrorLog()
{
   errorLog->Clear();
}
//...........................................................................
__int32 TImportMenu::FailedToLoadAMenu()
{
   return errorLog->Count > 0;
}
//...........................................................................
void TImportMenu::AddErrorLog( AnsiString inLog )
{
    errorLog->Add( inLog );
}
AnsiString TImportMenu::ErrorLog()
{
   return errorLog->Text;
}
//...........................................................................

//::::::::::::::::::::::::::::
//  Protected.
//::::::::::::::::::::::::::::

bool TImportMenu::menuExists( AnsiString inMenuName )
{
   bool result = false;

   //:::::::::::::::::::::::::::::::::::::::::::::::::

	result = menuExistsInDB( inMenuName.Trim(), dbTransaction );

   //:::::::::::::::::::::::::::::::::::::::::::::::::

   return result;
}
//...........................................................................
bool TImportMenu::deleteExistingMenu( AnsiString inMenuName )
{
   bool result = false;

   //:::::::::::::::::::::::::::::::::::::::::::::::::

   result = deleteExistingMenuInDB( inMenuName.Trim(), dbTransaction );

   //:::::::::::::::::::::::::::::::::::::::::::::::::

   return result;
}
//...........................................................................
bool TImportMenu::ImportNewMenuFromXMLFile( TLoadMenu* inMenu )
{
    bool result = false;

    //:::::::::::::::::::::::::::::::::::::::::::::::::

	result = CreateMenuInDB( inMenu, dbTransaction );

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
// ---------------------------------------------------------------------------
// It must not be used as all the menus are inserted in the DB as if they are new.
// Existing menus are first deleted.
bool TImportMenu::ImportExistingMenuFromXMLFile( TLoadMenu* inMenu )
{
    bool result = false;

    //:::::::::::::::::::::::::::::::::::::::::::::::::

	result = SaveMenuInDB( inMenu, dbTransaction );

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
// ---------------------------------------------------------------------------
bool TImportMenu::CreateMenuInDB( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
   bool result = false;

   //:::::::::::::::::::::::::::::::::::::::::::::::::

   try
   {
	   __int32 menuKey = InsertMenuInDB( inMenu->MenuName, inMenu->MenuType, inMenu->AvailableOnPalm, inDBTransaction );

	   SaveSizesFromMenu(           menuKey, inMenu, inDBTransaction );
	   SaveCategoryGroupsFromMenu(           inMenu, inDBTransaction );
	   SaveTaxProfilesFromMenu(			 	 inMenu, inDBTransaction );
	   SaveServingCoursesFromMenu(  menuKey, inMenu, inDBTransaction );
	   SaveThirdPartyCodesFromMenu(          inMenu, inDBTransaction );
	   SaveCoursesFromMenu(         menuKey, inMenu, inDBTransaction );
       SaveRevenueCodes(                     inMenu, inDBTransaction );

	   SaveAllForcedSidesFromMenu( inMenu, inDBTransaction );
       SaveAllForcedOptionsFromMenu(inMenu, inDBTransaction);

       ReInsertServingCoursePrintOrdersInDB( inDBTransaction, menuKey );

       result = true;
   }
   catch( Exception& exc )
   {
       textLog = "Failed to create a new menu in the DB: " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
   }

   //:::::::::::::::::::::::::::::::::::::::::::::::::

   return result;
}
// ---------------------------------------------------------------------------
// This function must not be used since all the menus are inserted in the DB as if they were new.
// Existing menus are first deleted.
bool TImportMenu::SaveMenuInDB( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
   bool result = false;

   //:::::::::::::::::::::::::::::::::::::::::::::::::

   try
   {
       if( !menuKeyExists( inMenu->MenuKey, inDBTransaction ) )
       {
            InsertMenuInDBWithKey(
                inMenu->MenuKey, inMenu->MenuName, inMenu->MenuType, inMenu->AvailableOnPalm, inDBTransaction );
       }

	   SaveSizesFromMenu(           inMenu->MenuKey, inMenu, inDBTransaction );
	   SaveCategoryGroupsFromMenu(           inMenu, inDBTransaction );
	   SaveTaxProfilesFromMenu(			 	 inMenu, inDBTransaction );
	   SaveServingCoursesFromMenu(  inMenu->MenuKey, inMenu, inDBTransaction );
	   SaveThirdPartyCodesFromMenu(          inMenu, inDBTransaction );
	   SaveCoursesFromMenu(         inMenu->MenuKey, inMenu, inDBTransaction );
       SaveRevenueCodes(            inMenu, inDBTransaction );

	   SaveAllForcedSidesFromMenu( inMenu, inDBTransaction );
       SaveAllForcedOptionsFromMenu(inMenu, inDBTransaction);

       result = true;
   }
   catch( Exception& exc )
   {
       textLog = "Failed to save a menu in the DB: " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
   }

   //:::::::::::::::::::::::::::::::::::::::::::::::::

   return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveSizesFromMenu( __int32 inMenuKey, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32 sizeCount = inMenu->SizeCount();

	resetNextNewSAO( inMenuKey, inDBTransaction );

	for( int i = 0; i < sizeCount; i++ )
	{
		SaveSizeFromMenu( i, inMenuKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveCategoryGroupsFromMenu( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32 cgCount = inMenu->CategoryGroupCount();

	for( int i = 0; i < cgCount; i++ )
	{
		SaveCategoryGroupFromMenu( i, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveCategoriesFromMenu( __int32 inCGXMLHandle, __int32 inCGroupKey,
										  TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32 categoryCount = inMenu->CategoryCount( inCGXMLHandle );

	resetNextNewCOO( inCGroupKey, inDBTransaction );

	for( int i = 0; i < categoryCount; i++ )
	{
		SaveCategoryFromMenu( i, inCGXMLHandle, inCGroupKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveTaxProfilesFromMenu( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32 taxProfileCount = inMenu->AllTaxProfilesCount();

	for( int i = 0; i < taxProfileCount; i++ )
	{
    	SaveTaxProfileFromMenu( i, inMenu, inDBTransaction );
    }
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveServingCoursesFromMenu( __int32 inMenuKey, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32 scCount = inMenu->ServingCourseCount();

	resetNextNewDisplayOrder( inDBTransaction );

	for( int i = 0; i < scCount; i++ )
	{
		SaveServingCourseFromMenu( i, inMenuKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveThirdPartyCodesFromMenu( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32 tpcCount = inMenu->ThirdPartyCodeCount();

	for( int i = 0; i < tpcCount; i++ )
	{
		SaveThirdPartyCodeFromMenu( i, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveRevenueCodes( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32 tpcCount = inMenu->RevenueCodesCount();

	for( int i = 0; i < tpcCount; i++ )
	{
		InsertRevenueCodesInDB( i,inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveCoursesFromMenu( __int32 inMenuKey, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32 courseCount = inMenu->CourseCount();

	resetNextNewCAO( inMenuKey, inDBTransaction );

    if(courseCount > 0)
    {
        __int32 latestGeneratedItemID = GetLatestItemIDFromGenerator (inDBTransaction);
        LoadExistingItemKeys( latestGeneratedItemID, inDBTransaction );
    }

	for( int i = 0; i < courseCount; i++ )
	{
		SaveCourseFromMenu( i, inMenuKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveOptionsFromMenu( __int32 inCourseXMLHandle, __int32 inCourseKey, TLoadMenu* inMenu,
									   Database::TDBTransaction *inDBTransaction )
{
	__int32 optionCount = inMenu->OptionCount( inCourseXMLHandle );

	resetNextNewOptionOrder( inCourseKey, inDBTransaction );

	for( int i = 0; i < optionCount; i++ )
	{
		SaveOptionFromMenu( i, inCourseXMLHandle, inCourseKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveItemsFromMenu( __int32 inCourseXMLHandle, __int32 inCourseKey, TLoadMenu* inMenu,
									 Database::TDBTransaction *inDBTransaction )
{
	__int32 itemCount = inMenu->ItemCount( inCourseXMLHandle );

	resetNextNewIAO( inCourseKey, inDBTransaction );
    //ClearItemMap();

	for( int i = 0; i < itemCount; i++ )
	{
		SaveItemFromMenu( i, inCourseXMLHandle, inCourseKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveAllForcedSidesFromMenu( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	std::map<__int32, __int32>::const_iterator imIT;

	for( imIT = _itemMap.begin(); imIT != _itemMap.end(); imIT++ )
	{
		__int32 masterItemKey = imIT->first;
		__int32 itemXMLHandle = imIT->second;

		__int32 fsCount = inMenu->ForcedSideCount( itemXMLHandle );

		resetNextNewIOO( masterItemKey, inDBTransaction );

		for( int i = 0; i < fsCount; i++ )
		{
			SaveForcedSideFromMenu( i, itemXMLHandle, masterItemKey, inMenu, inDBTransaction );
		}
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveForcedSidesFromMenu( __int32 inItemXMLHandle, __int32 inMasterItemKey, TLoadMenu* inMenu,
										   Database::TDBTransaction *inDBTransaction )
{
	__int32 fsCount = inMenu->ForcedOptionCount( inItemXMLHandle );

	resetNextNewIOO( inMasterItemKey, inDBTransaction );

	for( int i = 0; i < fsCount; i++ )
	{
		SaveForcedSideFromMenu( i, inItemXMLHandle, inMasterItemKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void TImportMenu::SaveItemSizesFromMenu( __int32 inItemXMLHandle, __int32 inMasterItemKey, TLoadMenu* inMenu,
											 Database::TDBTransaction *inDBTransaction )
{
	__int32 isCount = inMenu->ItemSizeCount( inItemXMLHandle );

	resetNextNewISAO( inMasterItemKey, inDBTransaction );

	for( int i = 0; i < isCount; i++ )
	{
		SaveItemSizeFromMenu( i, inItemXMLHandle, inMasterItemKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveBCategoriesFromMenu( __int32 inItemSizeXMLHandle, __int32 inItemSizeKey, TLoadMenu* inMenu,
											   Database::TDBTransaction *inDBTransaction )
{
	__int32 bcCount = inMenu->BCategoryCount( inItemSizeXMLHandle );

	for( int i = 0; i < bcCount; i++ )
	{
		SaveBCategoryFromMenu( i, inItemSizeXMLHandle, inItemSizeKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveItemSizeReceipesFromMenu( __int32 inItemSizeXMLHandle, __int32 inItemSizeKey, TLoadMenu* inMenu,
													Database::TDBTransaction *inDBTransaction )
{
	__int32 bcCount = inMenu->ItemSizeReceipeCount( inItemSizeXMLHandle );

	for( int i = 0; i < bcCount; i++ )
	{
		SaveItemSizeReceipeFromMenu( i, inItemSizeXMLHandle, inItemSizeKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveItemSizeTaxProfilesFromMenu( __int32 inItemSizeXMLHandle, __int32 inItemSizeKey, TLoadMenu* inMenu,
													   Database::TDBTransaction *inDBTransaction )
{
	__int32 taxProfileCount = inMenu->TaxProfileCount( inItemSizeXMLHandle );

	for( int i = 0; i < taxProfileCount; i++ )
	{
		SaveItemSizeTaxProfileFromMenu( i, inItemSizeXMLHandle, inItemSizeKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------

void TImportMenu::SaveSizeFromMenu( __int32 inIndex, __int32 inMenuKey, TLoadMenu* inMenu,
									Database::TDBTransaction *inDBTransaction )
{
	if( inMenuKey > 0 )
	{
		__int32    sizeKey;
		WideString description;
		__int32    sizeID;
		__int32    palmSizeID;
		WideString kitchenName;
		bool       weighted;
		WideString handheldName;
		WideString receiptName;
		__int32    sao;


	    //::::::::::::::::::::::::::::::::::::::::::::::::

		__int32 menuXMLHandle = inMenu->SizeAtIndex( inIndex, sizeKey, description, kitchenName, handheldName,
													 receiptName, weighted, sizeID, palmSizeID );

	    //::::::::::::::::::::::::::::::::::::::::::::::::

        // Sizes are allways inserted as new.
		if( menuXMLHandle > 0 )
		{
            __int32 xmlKey  = sizeKey;
                    sizeKey = InsertSizeInDB(
                                inMenuKey, description.Trim(), sizeID, palmSizeID, sao, kitchenName.Trim(),
                                weighted, handheldName.Trim(), receiptName.Trim(), inDBTransaction );

            // This Key Map will be used in the ItemSize section
            AddKeyMap( "sizes", xmlKey, sizeKey );
        }
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveCategoryGroupFromMenu( __int32 inIndex, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32    cgKey;
	WideString description;
	bool       getDeleted;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	__int32 cgXMLHandle = inMenu->CategoryGroupAtIndex( inIndex, cgKey, description, getDeleted );

	//::::::::::::::::::::::::::::::::::::::::::::::::

    // Category Groups are allways inserted as new.
	if( cgXMLHandle > 0 )
	{
		if( description != "" )
		{
			cgKey = InsertCategoryGroupInDB( description.Trim(), getDeleted, inDBTransaction );

			SaveCategoriesFromMenu( cgXMLHandle, cgKey, inMenu, inDBTransaction );
		}
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveCategoryFromMenu( __int32 inIndex, __int32 inCGXMLHandle, __int32 inCGroupKey,
										TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32    categoryKey;
	WideString description;
    WideString glcode;
	bool       cDeleted;
	__int32    coo;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	__int32 categoryXMLHandle = inMenu->CategoryAtIndex( inIndex, inCGXMLHandle, categoryKey, description,glcode, cDeleted );

	//::::::::::::::::::::::::::::::::::::::::::::::::


    if( categoryXMLHandle > 0 )
    {
        if( description != "" )
        {
			__int32 xmlKey     = categoryKey;
			__int32 tempCategoryKey = -1;
            getCategoryKeyIfExists(description.Trim(), &tempCategoryKey, inDBTransaction);

			if( tempCategoryKey < 0 )
			// New Category.
            {
                coo = getNextNewCOO();

                categoryKey = InsertCategoryInDB(
                                 description.Trim(),glcode.Trim(), !cDeleted, inCGroupKey, coo, inDBTransaction );

                // This Key Map will be used in the ItemSize section.
                AddKeyMap( "categories", xmlKey, categoryKey );
            }
            else
            // Existing Category.
            {
                    UpdateCategoryInDB(
                        tempCategoryKey, description.Trim(), glcode.Trim(), cDeleted, inCGroupKey, inDBTransaction );
//                    // This Key Map will be used in the ItemSize section.
					AddKeyMap( "categories", xmlKey, tempCategoryKey );
            }
        }
    }
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveTaxProfileFromMenu(__int32 inIndex, TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction)
{
	__int32    taxProfileKey;
	WideString taxProfileName;
	Currency   taxRate;
	__int32    taxType;
    __int32    priority;
    //__int32    taxCode;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	__int32 taxProfileXMLHandle = inMenu->TaxProfileAtIndex(inIndex, taxProfileKey, taxProfileName, taxRate, taxType, priority);//, taxCode);

	//::::::::::::::::::::::::::::::::::::::::::::::::

    if( taxProfileXMLHandle > 0 )
    {
        __int32 taxProfileXMLKey = taxProfileKey;

        TTaxProfileDBAccessManager_MM taxProfileDBManager;

        taxProfileKey = taxProfileDBManager.GetTaxProfileKey(*inDBTransaction, taxProfileName, taxRate, taxType);

        if( taxProfileKey == 0 )
        {
            if( taxProfileDBManager.CheckTaxProfileNameTypePair(*inDBTransaction, taxProfileName, taxType) )
            {
                taxProfileName += " " + DateTimeToStr(Now());
            }

            TaxType taxTypeEnum = TaxProfile::Convert(taxType);

            TaxProfile taxProfile(taxProfileName.Trim(), taxRate, taxTypeEnum, priority);//,taxCode);
            taxProfileDBManager.InsertTaxProfile(*inDBTransaction, taxProfile, taxProfileKey);
        }

        AddKeyMap("taxProfiles", taxProfileXMLKey, taxProfileKey);
    }
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveServingCourseFromMenu(
                                    __int32    inIndex,
                                    __int32    inMenuKey,
                                    TLoadMenu *inMenu,
                     Database::TDBTransaction *inDBTransaction )
{
	if( inMenuKey > 0 )
	{
		__int32    scKey;
		WideString description;
		WideString kitchenName;
		bool       enabled;
		bool       scDeleted;
		bool       selectable;
		TColor     color;
		__int32    displayOrder;

	    //::::::::::::::::::::::::::::::::::::::::::::::::

		__int32 scXMLHandle = inMenu->ServingCourseAtIndex(
                                        inIndex, scKey, description, kitchenName,
                                        enabled, scDeleted, selectable, color, displayOrder );

		//::::::::::::::::::::::::::::::::::::::::::::::::

        // Serving Courses are allways inserted as new.
		if( scXMLHandle > 0 )
		{
            __int32 xmlKey = scKey;
                    scKey  = InsertServingCourseInDB(
                                description.Trim(), kitchenName.Trim(), scDeleted,
                                selectable, color, displayOrder,
                                inDBTransaction );

            if( enabled )
            {
                InsertMenu_SCInDB( inMenuKey, scKey, inDBTransaction );

                // This Key Map will be used in the Course section
                AddKeyMap( "servingCourses", xmlKey, scKey );
            }
		}
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveThirdPartyCodeFromMenu( __int32    inIndex,
                                              TLoadMenu* inMenu,
                                              Database::TDBTransaction *inDBTransaction )
{
	__int32    tpcKey;
	WideString code;
	WideString description;
	bool       visible;
	__int32    codeType;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	__int32 tpcXMLHandle = inMenu->ThirdPartyCodeAtIndex(
                                        inIndex,
                                        tpcKey,
                                        code,
                                        description,
                                        visible,
                                        codeType );

	//::::::::::::::::::::::::::::::::::::::::::::::::

    // Third Party Codes are allways inserted as new.
	if( tpcXMLHandle > 0 )
	{
		if( code != "" )
		{
            __int32 xmlKey = tpcKey;

            if( ThirdPartyCodeExists( inDBTransaction, code ) )
            {
                tpcKey = UpdateThirdPartyCodeInDB(
                            code.Trim(), description.Trim(), visible, codeType, inDBTransaction );
            }
            else
            {
                tpcKey = InsertThirdPartyCodeInDB(
                            code, description, visible, codeType, inDBTransaction );
            }

            // This Key Map will be used in the ItemSize section
            AddKeyMap( "tpCodes", xmlKey, tpcKey );
        }
    }
}
//----------------------------------------------------------------------------
void TImportMenu::InsertRevenueCodesInDB(int index, TLoadMenu* inMenu,
                        Database::TDBTransaction *inDBTransaction )
{
	__int32    code;
	WideString description;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	__int32 tpcXMLHandle = inMenu->RevenueCodeAtIndex(
                                        index,
                                        code,
                                        description);

	//:::::::::::::::::::::::::::::::::::::
	try
	{
        if( tpcXMLHandle > 0 )
        {
            if( code != 0 )
            {
                if( !RevenueCodeExists( inDBTransaction, code ) && description.Trim().Length() != 0)
                {
                    TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
                    qr->SQL->Text = "INSERT INTO REVENUECODEDETAILS ( REVENUECODE, REVENUECODE_DESCRIPTION ) "
                                    "VALUES ( :REVENUECODE, :REVENUECODE_DESCRIPTION ); ";

                    qr->ParamByName("REVENUECODE"        )->AsInteger = code;
                    qr->ParamByName("REVENUECODE_DESCRIPTION"       )->AsString  = description;

                    qr->ExecQuery();
                }
            }
        }
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert REVENUECODE with key in the DB: "
                 "REVENUECODE: " + IntToStr( code ) + "  " +
                 "REVENUECODE_DESCRIPTION: " + description            + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveCourseFromMenu( __int32 inIndex, __int32 inMenuKey, TLoadMenu* inMenu,
									  Database::TDBTransaction *inDBTransaction )
{
	if( inMenuKey > 0 )
	{
		__int32     courseKey;
		WideString  description;
		WideString  kitchenName;
		WideString  handheldName;
		WideString  receiptName;
		__int32     location;
		bool        enabled;
		__int32     servingCourseKey;
		WideString  serviceCourse;
		bool        noDefaultServingCourse;
		__int32     cao;

		//::::::::::::::::::::::::::::::::::::::::::::::::

		__int32 courseXMLHandle = inMenu->CourseAtIndex( inIndex, courseKey, description, kitchenName,
														 handheldName, receiptName, location, enabled,
														 servingCourseKey, serviceCourse, noDefaultServingCourse );

		//::::::::::::::::::::::::::::::::::::::::::::::::

        // Courses are allways inserted as new.
		if( courseXMLHandle > 0 )
		{
			__int32 scKeyMapped = GetKeyMap( "servingCourses", servingCourseKey );

            cao = getNextNewCAO();

            courseKey = InsertCourseInDB(
                            inMenuKey, description.Trim(), kitchenName.Trim(), handheldName.Trim(), receiptName.Trim(), location,
                            enabled, scKeyMapped, serviceCourse, noDefaultServingCourse, cao,
                            inDBTransaction );

			SaveOptionsFromMenu( courseXMLHandle, courseKey, inMenu, inDBTransaction );
			SaveItemsFromMenu(   courseXMLHandle, courseKey, inMenu, inDBTransaction );
		}
    }
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveOptionFromMenu( __int32 inIndex, __int32 inCourseXMLHandle, __int32 inCourseKey,
									  TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32    optionKey;
	WideString description;
	WideString kitchenName;
	WideString handheldName;
	WideString receiptName;
	__int32    condimentsMask;
	__int32    flags;
	bool       enabled;
	bool       printUnderlined;
	bool       printBold;
	__int32    printColor;
	__int32    printFont;
	bool       printDoubleWidth;
	bool       printDoubleHeight;
	__int32    optionOrder;

	//::::::::::::::::::::::::::::::::::::::::

	__int32 optionXMLHandle = inMenu->OptionAtIndex( inIndex, inCourseXMLHandle, optionKey, description, kitchenName,
													 handheldName, receiptName, condimentsMask, flags, enabled,
													 printUnderlined, printBold, printColor, printFont,
											         printDoubleWidth, printDoubleHeight );

	//::::::::::::::::::::::::::::::::::::::::

    // Options are allways inserted as new.
	if( optionXMLHandle > 0 )
	{
        __int32 xmlKey = optionKey;

        optionOrder = getNextNewOptionOrder();

        optionKey = InsertOptionInDB(
                        inCourseKey, description.Trim(), kitchenName.Trim(), handheldName.Trim(), receiptName.Trim(),
                        condimentsMask, flags, enabled, printUnderlined, printBold, printColor,
                        printFont, printDoubleWidth, printDoubleHeight, optionOrder, inDBTransaction );

        // This Key Map will be used in the Forced Options section
        AddKeyMap( "options", xmlKey, optionKey );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveItemFromMenu( __int32 inIndex, __int32 inCourseXMLHandle, __int32 inCourseKey, TLoadMenu* inMenu,
                                    Database::TDBTransaction *inDBTransaction )
{
	__int32    itemKey;
	WideString description;
	WideString kitchenName;
	WideString handheldName;
	WideString receiptName;
	TColor     buttonColor;
	bool       displaySizes;
	bool       enabled;
	bool       itemOnlySide;
	bool       printUnderlined;
	bool       printBold;
	TColor     printColor;
	__int32    printFont;
	bool       printDoubleWidth;
	bool       printDoubleHeight;
	__int32    iao;
    int    itemIdentifier;

	//::::::::::::::::::::::::::::::::::::::::

	__int32 itemXMLHandle = inMenu->ItemAtIndex(
                                        inIndex, inCourseXMLHandle, itemKey, description, kitchenName,
                                        handheldName, receiptName, buttonColor, displaySizes, enabled,
                                        itemOnlySide, printUnderlined, printBold, printColor, printFont,
                                        printDoubleWidth, printDoubleHeight, itemIdentifier );

	//::::::::::::::::::::::::::::::::::::::::

    // Items are allways inserted as new.
	if( itemXMLHandle > 0 )
	{
        __int32 xmlKey = itemKey;

        iao = getNextNewIAO();

        // check whether this item is being used in the orders table. if so we need to preserve the item key as it is
        if(IsItemKeyAlreadyBeingUsedForOrders(itemKey))
        {
            InsertItemInDBWithKey(
                    itemKey,
                    inCourseKey, description.Trim(), kitchenName.Trim(),
                    buttonColor, displaySizes, enabled,
                    itemOnlySide, printUnderlined, printBold, printColor, printFont,
                    printDoubleWidth, printDoubleHeight, iao, inDBTransaction, handheldName.Trim(), itemIdentifier );
        }
        else
        {
            itemKey = InsertItemInDB(
                    inCourseKey, description.Trim(), kitchenName.Trim(),
                    buttonColor, displaySizes, enabled,
                    itemOnlySide, printUnderlined, printBold, printColor, printFont,
                    printDoubleWidth, printDoubleHeight, iao, inDBTransaction, handheldName.Trim(), itemIdentifier );
        }

		// This Item Map will be used in the Forced Sides section
		AddItemMap( itemKey, itemXMLHandle );

		// This Item Map will be used in the Item Sizes section
        AddKeyMap( "items", xmlKey, itemKey );


		SaveItemSizesFromMenu(     itemXMLHandle, itemKey, inMenu, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveForcedSideFromMenu( __int32 inIndex, __int32 inItemXMLHandle, __int32 inMasterItemKey,
										  TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32    forcedSideKey;
	__int32    itemFKey;  // Item as a Side.
	WideString description;
	__int32    groupNumber;
	__int32    maxSelect;
	bool       sideGroupSkip;
	__int32    ioo;

	//::::::::::::::::::::::::::::::::::::::::

	__int32 forcedSideXMLHandle = inMenu->ForcedSideAtIndex( inIndex, inItemXMLHandle, forcedSideKey, itemFKey, description,
															 groupNumber, maxSelect, sideGroupSkip );

	//::::::::::::::::::::::::::::::::::::::::

    // Forced Sides are allways inserted as new.
	if( forcedSideXMLHandle > 0 )
	{
		__int32 itemFKeyMapped = GetKeyMap( "items", itemFKey );

        ioo = getNextNewIOO();

        forcedSideKey = InsertForcedSideInDB(
                            inMasterItemKey, itemFKeyMapped, ioo,
                            groupNumber, maxSelect, sideGroupSkip, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveForcedOptionFromMenu( __int32 inIndex, __int32 inItemXMLHandle, __int32 inMasterItemKey,
										   TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32    forcedOptionKey;
	__int32    optionFKey;
	WideString description;
	__int32	   groupNumber;

	//::::::::::::::::::::::::::::::::::::::::

	__int32 forcedOptionXMLHandle = inMenu->ForcedOptionAtIndex( inIndex, inItemXMLHandle,
																 forcedOptionKey, optionFKey,
																 description, groupNumber );

	//::::::::::::::::::::::::::::::::::::::::

    // Forced Options are allways inserted as new.
	if( forcedOptionXMLHandle > 0 )
	{
		__int32 optionFKeyMapped = GetKeyMap( "options", optionFKey );

        forcedOptionKey = InsertForcedOptionInDB(
                                inMasterItemKey, optionFKeyMapped, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveItemSizeFromMenu( __int32 inIndex, __int32 inItemXMLHandle, __int32 inMasterItemKey,
                                        TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32    itemSizeKey;
	__int32    sizeFKey;
	__int32    itemID;
	__int32    sizeID;
	__int32    palmItemID;
	__int32    palmSizeID;
	WideString sizeName;
	Currency   price;
    Currency maxRetailPrice;
	Currency   specialPrice;
	Currency   cost;
	__int32    isao;
	bool       free;
	bool       availableAsStandard;
	bool       noRecipe;
	WideString barcode;
	__int32    setMenuMask;
	bool       enabled;
	double     gstPercent;
	double     costGSTPercent;
	double     pointsPercent;
	WideString sizeKitchenName;
	__int32    thirdPartyCodeFKey;
	double     memberPurchaseDiscount;
	double     locationPurchaseDiscount;
	__int32    memberPurchaseCount;
	__int32    locationPurchaseCount;
	__int32    categoryFKey;
	WideString category;
	double     tareWeight;
	AnsiString handheldName;
	AnsiString receiptName;
	__int32    plu;
	bool       taxExempt;
	bool       hasServiceCharge;
	double     availableQuantity;
	double     defaultQuantity;
	double     warningQuantity;
	bool       disableWhenCountReachesZero;
    bool       canBePaidForUsingPoints;
    int        default_patron_count;
    __int32    taxProfileFKey;
   Currency    priceForPoints;
   int         revenueCode;
   int itemSizeIdentifier;

	//::::::::::::::::::::::::::::::::::::::::

	__int32 itemSizeXMLHandle = inMenu->ItemSizeAtIndex( inIndex, inItemXMLHandle, itemSizeKey, sizeFKey, sizeName,sizeKitchenName,
														 free, price, maxRetailPrice, specialPrice,
														 gstPercent, cost, costGSTPercent, pointsPercent, memberPurchaseCount,
														 memberPurchaseDiscount, locationPurchaseCount,
														 locationPurchaseDiscount, noRecipe, taxExempt, hasServiceCharge,
														 setMenuMask, availableAsStandard, barcode, enabled, categoryFKey,
														 category, thirdPartyCodeFKey, tareWeight, plu,
														 availableQuantity, defaultQuantity, warningQuantity,
														 disableWhenCountReachesZero, canBePaidForUsingPoints, default_patron_count,
                                                         priceForPoints,revenueCode, itemSizeIdentifier);
    // Item Sizes are allways inserted as new.
    itemID = itemSizeKey;
	sizeID = sizeFKey;

	palmItemID = itemSizeKey;
	palmSizeID = sizeFKey;

	if( itemSizeXMLHandle > 0 )
	{
		__int32 sizeFKeyMapped       = GetKeyMap( "sizes", sizeFKey );
		__int32 categoryFKeyMapped   = GetKeyMap( "categories", categoryFKey );
		__int32 tpcFKeyMapped        = GetKeyMap( "tpCodes", thirdPartyCodeFKey );

        isao = getNextNewISAO();

        // if the itemsizekey from menu is an existing one, we need to see whether to re-use those itemsize keys
        if( itemSizeKey > 0
            && IsItemSizeKeyAlreadyBeingUsedForOrders(inMasterItemKey,itemSizeKey))
        {
            InsertItemSizeInDBWithKey(
                            itemSizeKey, inMasterItemKey, sizeFKeyMapped, itemID, sizeID, palmItemID, palmSizeID,
                            sizeName.Trim(), price, maxRetailPrice, specialPrice, cost, isao, free, availableAsStandard,
                            noRecipe, barcode.Trim(), setMenuMask, enabled, gstPercent, costGSTPercent,
                            pointsPercent, sizeKitchenName.Trim(), tpcFKeyMapped, memberPurchaseDiscount,
                            locationPurchaseDiscount, memberPurchaseCount, locationPurchaseCount,
                            categoryFKeyMapped, tareWeight, handheldName, receiptName, plu,
                            availableQuantity, defaultQuantity, warningQuantity,
                            disableWhenCountReachesZero,
                            canBePaidForUsingPoints, default_patron_count, priceForPoints,revenueCode, itemSizeIdentifier, inDBTransaction );
        }
        else
        {
        itemSizeKey = InsertItemSizeInDB(
                        inMasterItemKey, sizeFKeyMapped, itemID, sizeID, palmItemID, palmSizeID,
                        sizeName.Trim(), price, maxRetailPrice, specialPrice, cost, isao, free, availableAsStandard,
                        noRecipe, barcode.Trim(), setMenuMask, enabled, gstPercent, costGSTPercent,
                        pointsPercent, sizeKitchenName.Trim(), tpcFKeyMapped, memberPurchaseDiscount,
                        locationPurchaseDiscount, memberPurchaseCount, locationPurchaseCount,
                        categoryFKeyMapped, tareWeight, handheldName, receiptName, plu,
                        availableQuantity, defaultQuantity, warningQuantity,
                        disableWhenCountReachesZero, canBePaidForUsingPoints,
                        default_patron_count, priceForPoints,revenueCode, itemSizeIdentifier, inDBTransaction );
        }

//			// ASEAN ++
//			itemSizeKey = InsertItemSizeInDB( inMasterItemKey, sizeFKeyMapped, itemID, sizeID, palmItemID, palmSizeID,
//											  sizeName, price, specialPrice, cost, isao, free, availableAsStandard,
//											  noRecipe, barcode, setMenuMask, enabled, gstPercent, costGSTPercent,
//											  pointsPercent, sizeKitchenName, tpcFKeyMapped, memberPurchaseDiscount,
//											  locationPurchaseDiscount, memberPurchaseCount, locationPurchaseCount,
//											  categoryFKeyMapped, tareWeight, handheldName, receiptName, plu, taxExempt, hasServiceCharge,
//											  availableQuantity, defaultQuantity, warningQuantity, disableWhenCountReachesZero,
//											  inDBTransaction );

		SaveBCategoriesFromMenu(         itemSizeXMLHandle, itemSizeKey, inMenu, inDBTransaction );
		SaveItemSizeReceipesFromMenu(    itemSizeXMLHandle, itemSizeKey, inMenu, inDBTransaction );
        SaveItemSizeTaxProfilesFromMenu( itemSizeXMLHandle, itemSizeKey, inMenu, inDBTransaction );

        SaveItemSizePriceLevelsFromMenu( itemSizeXMLHandle, itemSizeKey, inMenu, inDBTransaction );

	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveBCategoryFromMenu( __int32 inIndex, __int32 inItemSizeXMLHandle, __int32 inItemSizeKey,
											 TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32    bCategoryKey;
	WideString description;

	//::::::::::::::::::::::::::::::::::::::::

	__int32 bCategoryXMLHandle = inMenu->BCategoryAtIndex( inIndex, inItemSizeXMLHandle, bCategoryKey, description );

    bCategoryKey = GetKeyMap( "categories", bCategoryKey );

	//::::::::::::::::::::::::::::::::::::::::

    // Breakdown Categoriess are allways inserted as new.
	if( bCategoryXMLHandle > 0 )
	{
		bCategoryKey = InsertBCategoryInDB( inItemSizeKey, bCategoryKey, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveItemSizeReceipeFromMenu( __int32 inIndex, __int32 inItemSizeXMLHandle, __int32 inItemSizeKey,
											  TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	__int32    isReceipeKey;
	WideString stockCode;
	double     quantity;
	WideString location;

	//::::::::::::::::::::::::::::::::::::::::

	__int32 isReceipeXMLHandle = inMenu->ItemSizeReceipeAtIndex( inIndex, inItemSizeXMLHandle, isReceipeKey, stockCode,
														         quantity, location );

	//::::::::::::::::::::::::::::::::::::::::

    // Item Size Receipes are allways inserted as new.
	if( isReceipeXMLHandle > 0 )
	{
		isReceipeKey = InsertItemSizeReceipeInDB( inItemSizeKey, stockCode.Trim(), quantity, location.Trim(), inDBTransaction );
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::SaveItemSizeTaxProfileFromMenu( __int32 inIndex, __int32 inItemSizeXMLHandle, __int32 inItemSizeKey,
											     TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
    __int32 taxProfileXMLKey;

	//::::::::::::::::::::::::::::::::::::::::

	__int32 taxProfileXMLHandle = inMenu->ItemSizeTaxProfileAtIndex( inIndex, inItemSizeXMLHandle, taxProfileXMLKey );

	//::::::::::::::::::::::::::::::::::::::::

    // Item Size Receipes are allways inserted as new.
	if( taxProfileXMLHandle > 0 )
	{
        __int32 taxProfileKey = GetKeyMap( "taxProfiles", taxProfileXMLKey );
		InsertItemSizeTaxProfileInDB( inItemSizeKey, taxProfileKey, inDBTransaction );
	}
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::
// NOT USED SO FAR!.
//::::::::::::::::::::::::::::::::::::::::::::::
bool TImportMenu::menuKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "MENU_KEY", "MENU", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::sizeKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "SIZES_KEY", "SIZES", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::categoryGroupKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "CATEGORYGROUPS_KEY", "CATEGORYGROUPS", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::categoryKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "CATEGORY_KEY", "ARCCATEGORIES", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::servingCourseKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "SERVINGCOURSES_KEY", "SERVINGCOURSES", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::thirdPartyCodeKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "THIRDPARTYCODES_KEY", "THIRDPARTYCODES", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::courseKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "COURSE_KEY", "COURSE", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::optionKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "OPTIONS_KEY", "OPTIONS", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::itemKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "ITEM_KEY", "ITEM", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::forcedSideKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "ITEMSIDES_KEY", "ITEMSIDES", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::forcedOptionKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "ID", "FORCEDOPTIONS", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::itemSizeKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "ITEMSIZE_KEY", "ITEMSIZE", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::itemSizeReceipeKeyExists( __int32 inKey, Database::TDBTransaction *inDBTransaction )
{
    return keyExists( inKey, "RECIPE_KEY", "RECIPE", inDBTransaction );
}
// ---------------------------------------------------------------------------
bool TImportMenu::keyExists(
                       __int32 inKey,
                    AnsiString inKeyName,
                    AnsiString inTableName,
                    Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "SELECT COUNT(" + inKeyName + ") "
                        "FROM  " + inTableName + " "
                        "WHERE " + inKeyName + " = :key;";

		qr->ParamByName("key")->AsInteger = inKey;

		qr->ExecQuery();

        __int32 rCount = qr->FieldByName( "count" )->AsInteger;

		result = rCount > 0;
	}
    catch( Exception& exc )
    {
       textLog = "Failed to read key from the DB: "
                 "key: "        + IntToStr( inKey ) + " " +
                 "key name: "   + inKeyName +  " " +
                 "table name: " + inTableName + ": " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
    }

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
//::::::::::::::::::::::::::::::::::::::::::::::

bool TImportMenu::menuExistsInDB(
                    AnsiString inMenuName,
                    Database::TDBTransaction *inDBTransaction )
{
   bool result = false;

   //:::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "SELECT COUNT(Menu.Menu_Name) FROM Menu WHERE Menu.Menu_Name = :menuName";

		qr->ParamByName("menuName")->AsString = inMenuName.Trim();

		qr->ExecQuery();

        int menuCount = qr->FieldByName("count")->AsInteger;

        result = menuCount > 0;
	}
    catch( Exception& exc )
    {
       textLog = "Failed to read menu name from the DB: "
                 "menu name: " + inMenuName + ": " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
    }

   //:::::::::::::::::::::::::::::::::::::::::::::::::

   return result;
}
//...........................................................................
bool TImportMenu::deleteExistingMenuInDB(
                    AnsiString inMenuName,
                    Database::TDBTransaction *inDBTransaction )
{
   bool result = false;

   //:::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "DELETE FROM Menu WHERE Menu.Menu_Name = :menuName";

		qr->ParamByName("menuName")->AsString = inMenuName;

		qr->ExecQuery();

        result = true;
	}
    catch( Exception& exc )
    {
       textLog = "Failed to delete menu from the DB: "
                 "menu name: " + inMenuName + ": " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
    }

   //:::::::::::::::::::::::::::::::::::::::::::::::::

   return result;
}
//...........................................................................
void TImportMenu::getServingCourseKeyIfExists(AnsiString inServingCourseName, __int32 *outServingCourseKey, Database::TDBTransaction *inDBTransaction)
{
    TIBSQL *qrGetServingCourseKey = inDBTransaction->Query( inDBTransaction->AddQuery() );
    qrGetServingCourseKey->SQL->Text = "Select SC.SERVINGCOURSES_Key from SERVINGCOURSES SC where SC.SERVINGCOURSE_NAME = :SCName";
    qrGetServingCourseKey->ParamByName("SCName")->AsString = inServingCourseName;
    qrGetServingCourseKey->ExecQuery();

    if(qrGetServingCourseKey->Eof)
        *outServingCourseKey = -1;
    else
    {
        *outServingCourseKey = qrGetServingCourseKey->Fields[0]->AsInteger;
    }
}
//...........................................................................
void TImportMenu::getCategoryGroupKeyIfExists(AnsiString inCategoryGroupName, __int32 *outCategoryGroupKey, Database::TDBTransaction *inDBTransaction)
{
    TIBSQL *qrGetCategoryGroupKey = inDBTransaction->Query( inDBTransaction->AddQuery() );
    qrGetCategoryGroupKey->SQL->Text = "Select CG.CATEGORYGROUPS_KEY from CATEGORYGROUPS CG where CG.NAME = :CGName";
    qrGetCategoryGroupKey->ParamByName("CGName")->AsString = inCategoryGroupName;
    qrGetCategoryGroupKey->ExecQuery();

    if(qrGetCategoryGroupKey->Eof)
        *outCategoryGroupKey = -1;
    else
    {
        *outCategoryGroupKey = qrGetCategoryGroupKey->Fields[0]->AsInteger;
    }
}
//...........................................................................
void TImportMenu::getCategoryKeyIfExists(AnsiString inCategoryName, __int32 *outCategoryKey, Database::TDBTransaction *inDBTransaction)
{
    TIBSQL *qrGetCategoryKey = inDBTransaction->Query( inDBTransaction->AddQuery() );
    qrGetCategoryKey->SQL->Text = "Select C.CATEGORY_KEY from ARCCATEGORIES C where C.CATEGORY = :Category";
    qrGetCategoryKey->ParamByName("Category")->AsString = inCategoryName;
    qrGetCategoryKey->ExecQuery();

    if(qrGetCategoryKey->Eof)
        *outCategoryKey = -1;
    else
    {
        *outCategoryKey = qrGetCategoryKey->Fields[0]->AsInteger;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::
// Insert_InDB functions insert new objects in th DB.
// Insert_InDBWithKey functions are not used so far!!!!.
//::::::::::::::::::::::::::::::::::::::::::::::
__int32 TImportMenu::InsertMenuInDB(
                        WideString inMenuName, Menu::TMenuType inMenuType,
                        bool inAvailableOnPalm, Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

    TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

    qrGenID->SQL->Text = "SELECT GEN_ID(GEN_MENU, 1) FROM RDB$DATABASE";
    qrGenID->ExecQuery();

    __int32 key = qrGenID->Fields[0]->AsInteger;

    //:::::::::::::::::::::::::::::::::::::::

    InsertMenuInDBWithKey(
        key, inMenuName.Trim(), inMenuType, inAvailableOnPalm, inDBTransaction );

    result = key;

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertMenuInDBWithKey (
                        __int32 inKey, WideString inMenuName, Menu::TMenuType inMenuType,
                        bool inAvailableOnPalm, Database::TDBTransaction *inDBTransaction )
{
    TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
    qr->SQL->Text = "INSERT INTO MENU ( Menu_Key, Menu_Name, Menu_Type, Description, Palmable, Deleted ) "
                    "VALUES ( :key, :name, :type, :description, :palmable, :deleted ); ";

    qr->ParamByName("key"        )->AsInteger = inKey;
    qr->ParamByName("name"       )->AsString  = inMenuName;
    qr->ParamByName("type"       )->AsInteger = inMenuType;
    qr->ParamByName("description")->AsString  = "";
    qr->ParamByName("palmable"   )->AsString  = inAvailableOnPalm ? "T" : "F";
    qr->ParamByName("deleted"    )->AsString  = "F";

    qr->ExecQuery();
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertSizeInDB( __int32 inMenuKey, WideString inSizeName, __int32 inSizeID,
									  __int32 inPalmSizeID, __int32 inSAO, WideString inKitchenName,
									  bool inWeighted, WideString inHandheldName, WideString inReceiptName,
									  Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_SIZE, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

		InsertSizeInDBWithKey(
            key, inMenuKey, inSizeName, inSizeID, inPalmSizeID, inSAO, inKitchenName,
			inWeighted, inHandheldName, inReceiptName, inDBTransaction );

		result = key;
	}
    catch( Exception& exc )
    {
       textLog = "Failed to insert size in the DB: "
                 "size name: " + inSizeName + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
    }

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertSizeInDBWithKey(
                            __int32 inKey, __int32 inMenuKey, WideString inSizeName, __int32 inSizeID,
                            __int32 inPalmSizeID, __int32 inSAO, WideString inKitchenName,
                            bool inWeighted, WideString inHandheldName, WideString inReceiptName,
                            Database::TDBTransaction *inDBTransaction )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO SIZES ( Sizes_Key, Menu_Key, Size_Name, Size_ID, Palm_Size_ID, "
											"SAO, Size_Kitchen_Name, Weighted_Size, Handheld_Name, "
											"Receipt_Name ) "
						"VALUES ( :key, :menuFKey, :sizeName, :sizeID, :palmSizeID, :sao, "
								 ":sizeKitchenName, :weightedSize, :handheldName, :receiptName ); ";

		qr->ParamByName("key"            )->AsInteger = inKey;
		qr->ParamByName("menuFKey"       )->AsInteger = inMenuKey;
		qr->ParamByName("sizeName"       )->AsString  = inSizeName;
		qr->ParamByName("sizeID"         )->AsInteger = inSizeID;
		qr->ParamByName("palmSizeID"     )->AsInteger = inPalmSizeID;
		qr->ParamByName("sao"            )->AsInteger = inSAO;
		qr->ParamByName("sizeKitchenName")->AsString  = inKitchenName;
		qr->ParamByName("weightedSize"   )->AsString  = inWeighted ? "T" : "F";
		qr->ParamByName("handheldName"   )->AsString  = inHandheldName;
		qr->ParamByName("receiptName"    )->AsString  = inReceiptName;

		qr->ExecQuery();

	}
    catch( Exception& exc )
    {
       textLog = "Failed to insert size with key in the DB: "
                 "key: "       + IntToStr( inKey ) + "  " +
                 "size name: " + inSizeName        + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
    }
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertCategoryGroupInDB( WideString inDescription, bool inDeleted,
												   Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{

        __int32 key = -1;
        getCategoryGroupKeyIfExists(inDescription, &key, inDBTransaction);

        if( key == -1) // if category group does not exist already.. insert it
        {

		    TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		    qrGenID->SQL->Text = "SELECT GEN_ID(GEN_CATEGORYGROUPS, 1) FROM RDB$DATABASE";
		    qrGenID->ExecQuery();

            key = qrGenID->Fields[0]->AsInteger;

		    //:::::::::::::::::::::::::::::::::::::::

		    InsertCategoryGroupInDBWithKey(
                key, inDescription, inDeleted, inDBTransaction );
         }

         result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert category group in the DB: "
                 "description: " + inDescription + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertCategoryGroupInDBWithKey(
                        __int32 inKey, WideString inDescription, bool inDeleted,
                        Database::TDBTransaction *inDBTransaction )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO CATEGORYGROUPS ( CategoryGroups_Key, Name, Visible ) "
						"VALUES ( :key, :name, :visible ); ";

		qr->ParamByName("key"    )->AsInteger = inKey;
		qr->ParamByName("name"   )->AsString  = inDescription;
		qr->ParamByName("visible")->AsString  = !inDeleted ? "T" : "F"; //CHECK THIS OUT!!!!!! is it Visible or Deleted?

		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert category group with key in the DB: "
                 "key: "         + IntToStr( inKey ) + "  " +
                 "description: " + inDescription     + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertCategoryInDB( WideString inCategory,WideString inGLCode, bool inVisible, __int32 inCGroupKey,
										 __int32 inCOO, Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_ARCCATEGORIES, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

		InsertCategoryInDBWithKey(
            key, inCategory,inGLCode.Trim(), inVisible, inCGroupKey,
			inCOO, inDBTransaction );

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert category key in the DB: "
                 "description: " + inCategory + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertCategoryInDBWithKey(
                        __int32 inKey, WideString inCategory,WideString inGLCode, bool inVisible, __int32 inCGroupKey,
                        __int32 inCOO, Database::TDBTransaction *inDBTransaction )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO ARCCATEGORIES ( Category_Key, Category, CategoryGroups_Key, "
													"COO, Visible,GL_CODE ) "
						"VALUES ( :key, :category, :cgFKey, :coo, :visible,:GL_CODE ); ";

		qr->ParamByName("key"     )->AsInteger = inKey;
		qr->ParamByName("category")->AsString  = inCategory;
		qr->ParamByName("cgFKey  ")->AsInteger = inCGroupKey;
		qr->ParamByName("coo"     )->AsInteger = inCOO;
		qr->ParamByName("visible" )->AsString  = inVisible ? "T" : "F";
        qr->ParamByName("GL_CODE")->AsString  = inGLCode;
		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert size with key in the DB: "
                 "key: "       + IntToStr( inKey ) + "  ";
                 "description: " + inCategory      + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertServingCourseInDB(
                        WideString inDescription, WideString inKitchenName,
                        bool inSCDeleted, bool inSelectable, TColor inColor,
                        __int32 inDisplayOrder,
                        Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
        __int32 key = -1;
        getServingCourseKeyIfExists(inDescription, &key, inDBTransaction);

        if(key == -1)
        {
		    TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		    qrGenID->SQL->Text = "SELECT GEN_ID(GEN_SERVINGCOURSES, 1) FROM RDB$DATABASE";
		    qrGenID->ExecQuery();

		    key = qrGenID->Fields[0]->AsInteger;

 		    //:::::::::::::::::::::::::::::::::::::::

		    InsertServingCourseInDBWithKey(
                key, inDescription, inKitchenName, inSCDeleted, inSelectable, inColor,
                inDisplayOrder, inDBTransaction );
        }
        else
        {
            UpdateServingCourseInDB(
                key, inDescription, inKitchenName,
				inSCDeleted, inSelectable, inColor, inDisplayOrder,
                inDBTransaction );
        }

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert course with key in the DB: "
                 "description: " + inDescription + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertServingCourseInDBWithKey(
                        __int32 inKey, WideString inDescription, WideString inKitchenName,
                        bool inSCDeleted, bool inSelectable, TColor inColor,
                        __int32 inDisplayOrder, Database::TDBTransaction *inDBTransaction )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO SERVINGCOURSES ( ServingCourses_Key, ServingCourse_Name, ServingCourse_Kitchen_Name, "
										   "Deleted, Selectable, Display_Order, Colour ) "
						"VALUES ( :key, :servingCourseName, :servingCourseKitchenName, "
						"         :deleted, :selectable, :displayOrder, :colour ); ";

		qr->ParamByName("key"                     )->AsInteger = inKey;
		qr->ParamByName("servingCourseName"       )->AsString  = inDescription;
		qr->ParamByName("servingCourseKitchenName")->AsString  = inKitchenName;
		qr->ParamByName("deleted"                 )->AsString  = inSCDeleted  ? "T" : "F";
		qr->ParamByName("selectable"              )->AsString  = inSelectable ? "T" : "F";
		qr->ParamByName("displayOrder"            )->AsInteger = inDisplayOrder;
		qr->ParamByName("colour"                  )->AsInteger = (int)inColor;

		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert size with key in the DB: "
                 "key: "       + IntToStr( inKey ) + "  ";
                 "description: " + inDescription   + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertMenu_SCInDB( __int32 inMenuKey, __int32 inKey,
                                        Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_MENUSERVINGCOURSES, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO MENUSERVINGCOURSES ( MenuServingCourses_Key, Menu_Key, "
														 "ServingCourses_Key ) "
						"VALUES ( :key, :menuFKey, :scFKey ); ";

		qr->ParamByName("key"     )->AsInteger = key;
		qr->ParamByName("menuFKey")->AsInteger = inMenuKey;
		qr->ParamByName("scFKey"  )->AsInteger = inKey;

		qr->ExecQuery();

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert menu serving course with key in the DB: "
                 "menu key: " + IntToStr( inMenuKey ) + "  " +
                 "key: "      + IntToStr( inKey )  + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::UpdateThirdPartyCodeInDB(
                        WideString inCode, WideString inDescription,
                        bool inVisible, __int32 inCodeType,
                        Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		__int32 key = GetTPCKey( inDBTransaction, inCode ) ;

        if( key == -1)
        {
            throw new Exception( "Third Party Code's key not found in the database." );
        }

		//:::::::::::::::::::::::::::::::::::::::

		UpdateThirdPartyCodeInDBWithKey(
            key, inCode, inDescription, inVisible, inCodeType, inDBTransaction );

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update TPC in the DB: "
                 "TPC: " + inCode + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertThirdPartyCodeInDB(
                        WideString inCode, WideString inDescription,
                        bool inVisible, __int32 inCodeType,
                        Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_THIRDPARTYCODES, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

		InsertThirdPartyCodeInDBWithKey(
            key, inCode, inDescription, inVisible, inCodeType, inDBTransaction );

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert TPC in the DB: "
                 "TPC: " + inCode + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------

void TImportMenu::UpdateThirdPartyCodeInDBWithKey(
                        __int32    inKey,
                        WideString inCode,
                        WideString inDescription,
                        bool       inVisible,
                        __int32    inCodeType,
                        Database::TDBTransaction *inDBTransaction )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qr->SQL->Text = "UPDATE THIRDPARTYCODES "
                        "SET  "
                            "Code        = :code, "
                            "CodeType    = :codeType, "
                            "Visible     = :visible, "
                            "Description = :description "
						"WHERE ThirdPartyCodes_Key = :key; ";

		qr->ParamByName("key"        )->AsInteger = inKey;
		qr->ParamByName("code"       )->AsString  = inCode;
		qr->ParamByName("codeType"   )->AsInteger = inCodeType;
		qr->ParamByName("visible"    )->AsString  = inVisible ? "T" : "F";
		qr->ParamByName("description")->AsString  = inDescription;

		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update TPC with key in the DB: "
                 "key: " + IntToStr( inKey ) + "  " +
                 "TPC: " + inCode            + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertThirdPartyCodeInDBWithKey(
                        __int32 inKey, WideString inCode, WideString inDescription,
                        bool inVisible, __int32 inCodeType, Database::TDBTransaction *inDBTransaction )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO THIRDPARTYCODES ( ThirdPartyCodes_Key, Code, CodeType, "
										              "Visible, Description ) "
						"VALUES ( :key, :code, :codeType, :visible, :description ); ";

		qr->ParamByName("key"        )->AsInteger = inKey;
		qr->ParamByName("code"       )->AsString  = inCode;
		qr->ParamByName("codeType"   )->AsInteger = inCodeType;
		qr->ParamByName("visible"    )->AsString  = inVisible ? "T" : "F";
		qr->ParamByName("description")->AsString  = inDescription;

		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert TPC with key in the DB: "
                 "key: " + IntToStr( inKey ) + "  " +
                 "TPC: " + inCode            + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertCourseInDB( __int32 inMenuKey, WideString inDescription, WideString inKitchenName,
									   WideString inHandheldName, WideString inReceiptName, __int32 inLocation,
									   bool inEnabled, __int32 inServingCourseKey, WideString inServiceCourse,
									   bool inNoDefaultServingCourse, __int32 inCAO,
									   Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_COURSE, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

		InsertCourseInDBWithKey(
            key, inMenuKey, inDescription, inKitchenName,
            inHandheldName, inReceiptName, inLocation,
            inEnabled, inServingCourseKey, inServiceCourse,
            inNoDefaultServingCourse, inCAO, inDBTransaction );

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert course in the DB: "
                 "course: " + inDescription + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertCourseInDBWithKey(
                        __int32 inKey, __int32 inMenuKey, WideString inDescription, WideString inKitchenName,
                        WideString inHandheldName, WideString inReceiptName, __int32 inLocation,
                        bool inEnabled, __int32 inServingCourseKey, WideString inServiceCourse,
                        bool inNoDefaultServingCourse, __int32 inCAO,
                        Database::TDBTransaction *inDBTransaction )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO COURSE ( Course_Key, Menu_Key, Course_Name, Course_ID, "
											 "CAO, Enabled, View_Location, Course_Kitchen_Name, ServingCourses_Key, "
											 "No_Default_Serving_Course, Handheld_Name, Receipt_Name ) "
						"VALUES ( :key, :menuFKey, :courseName, :courseID, :cao, :enabled, "
								 ":viewLocation, :courseKitchenName, :servingCourseKey, :noDefaultServingCourse, "
								 ":handheldName, :receiptName ); ";

		qr->ParamByName("key"                   )->AsInteger = inKey;
		qr->ParamByName("menuFKey"              )->AsInteger = inMenuKey;
		qr->ParamByName("courseName"            )->AsString  = inDescription;
		qr->ParamByName("courseID"              )->AsInteger = inKey;
		qr->ParamByName("cao"                   )->AsInteger = inCAO;
		qr->ParamByName("enabled"               )->AsString  = inEnabled ? "T" : "F";
		qr->ParamByName("viewLocation"          )->AsInteger = inLocation;
		qr->ParamByName("courseKitchenName"     )->AsString  = inKitchenName;
        if ( inServingCourseKey == 0 )
        {
		    qr->ParamByName("servingCourseKey"  )->Clear();
        }
        else
        {
            qr->ParamByName("servingCourseKey"  )->AsInteger = inServingCourseKey;
        }
		qr->ParamByName("noDefaultServingCourse")->AsString  = inNoDefaultServingCourse ? "T" : "F";
		qr->ParamByName("handheldName"          )->AsString  = inHandheldName;
		qr->ParamByName("receiptName"           )->AsString  = inReceiptName;

		qr->ExecQuery();

        // add the kitchenName / Course name and key for later use when inserting course print orders back into the database
        // we check whether a kitchen name is present, if so add it, else add the course name
        // this is done because the printer profile screen always checkes for the kitchen name rather than the course name
        AddCourseNameKeyMap(inKitchenName.Length() > 0 ? UnicodeString(inKitchenName) : UnicodeString(inDescription), inKey);
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert course with key in the DB: "
                 "key: "    + IntToStr( inKey ) + "  " +
                 "course: " + inDescription     + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertOptionInDB(
                        __int32 inCourseKey, WideString inDescription, WideString inKitchenName,
                        WideString inHandheldName, WideString inReceiptName,
                        __int32 inCondimentsMask, __int32 inFlags, bool inEnabled, bool inPrintUnderlined,
                        bool inPrintBold, __int32 inPrintColor, __int32 inPrintFont,
                        bool inPrintDoubleWidth, bool inPrintDoubleHeight, __int32 inOptionOrder,
                        Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_OPTION, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

        InsertOptionInDBWithKey(
            key, inCourseKey, inDescription, inKitchenName, inHandheldName,
            inReceiptName, inCondimentsMask, inFlags, inEnabled, inPrintUnderlined,
            inPrintBold, inPrintColor, inPrintFont, inPrintDoubleWidth,
            inPrintDoubleHeight, inOptionOrder, inDBTransaction );

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert option in the DB: "
                 "course key: " + IntToStr( inCourseKey ) + "  " +
                 "option: "     + inDescription     + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertOptionInDBWithKey(
                        __int32 inKey, __int32 inCourseKey, WideString inDescription, WideString inKitchenName,
                        WideString inHandheldName, WideString inReceiptName,
                        __int32 inCondimentsMask, __int32 inFlags, bool inEnabled, bool inPrintUnderlined,
                        bool inPrintBold, __int32 inPrintColor, __int32 inPrintFont,
                        bool inPrintDoubleWidth, bool inPrintDoubleHeight, __int32 inOptionOrder,
                        Database::TDBTransaction *inDBTransaction )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO OPTIONS ( Options_Key, Course_Key, Palm_Option_ID, Option_ID, "
											  "Option_Name, Option_Order, Redir_ID, Enabled, Flags, "
											  "Forced_Mask, Option_Kitchen_Name, Print_Underlined, "
											  "Print_Bold, Print_Colour, Print_Font, Print_Double_Width, "
										      "Print_Double_Height, Handheld_Name, Receipt_Name ) "
						"VALUES ( :key, :courseFKey, :palmOptionID, :optionID, "
								 ":optionName, :optionOrder, :redirID, :enabled, :flags, "
								 ":forcedMask, :optionKitchenName, :printUnderlined, "
								 ":printBold,  :printColor, :printFont, :printDoubleWidth, "
								 ":printDoubleHeight, :handheldName, :receiptName ); ";

		qr->ParamByName("key"              )->AsInteger = inKey;
		qr->ParamByName("courseFKey"       )->AsInteger = inCourseKey;
		qr->ParamByName("optionID"         )->AsInteger = inKey;
		qr->ParamByName("optionName"       )->AsString  = inDescription;
		qr->ParamByName("optionOrder"      )->AsInteger = inOptionOrder;
		qr->ParamByName("redirID"          )->AsInteger = 0;
		qr->ParamByName("enabled"          )->AsString  = inEnabled ? "T" : "F";
		qr->ParamByName("flags"            )->AsInteger = inFlags;
		qr->ParamByName("optionKitchenName")->AsString  = inKitchenName;
		qr->ParamByName("printUnderlined"  )->AsString  = inPrintUnderlined ? "T" : "F";
		qr->ParamByName("printBold"        )->AsString  = inPrintBold ? "T" : "F";
		qr->ParamByName("printColor"       )->AsInteger = inPrintColor;
		qr->ParamByName("printFont"        )->AsInteger = inPrintFont;
		qr->ParamByName("printDoubleWidth" )->AsString  = inPrintDoubleWidth  ? "T" : "F";
		qr->ParamByName("printDoubleHeight")->AsString  = inPrintDoubleHeight ? "T" : "F";
		qr->ParamByName("handheldName"     )->AsString  = inHandheldName;
		qr->ParamByName("receiptName"      )->AsString  = inReceiptName;

		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert option with key in the DB: "
                 "course key: " + IntToStr( inCourseKey ) + "  " +
                 "key: "        + IntToStr( inKey ) + "  " +
                 "option: "     + inDescription     + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertItemInDB(
                        __int32 inCourseKey, WideString inDescription,
						WideString inKitchenName, TColor inButtonColor, bool inDisplaySizes,
						bool inEnabled, bool inItemOnlySide,
						bool inPrintUnderlined, bool inPrintBold, TColor inPrintColor,
						__int32 inPrintFont, bool inPrintDoubleWidth, bool inPrintDoubleHeight,
						__int32 inIAO, Database::TDBTransaction *inDBTransaction, WideString inHandheldName, int inItemIdentifier )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		__int32 key = GetLatestItemIDFromGenerator( inDBTransaction );

        // checks whether the item key is already exists in the database, if so get a new key from the generator
        while(IsItemKeyExists(key))
        {
            key = GetLatestItemIDFromGenerator( inDBTransaction );
        }

		//:::::::::::::::::::::::::::::::::::::::

		InsertItemInDBWithKey(
            key, inCourseKey, inDescription, inKitchenName, inButtonColor, inDisplaySizes,
            inEnabled, inItemOnlySide, inPrintUnderlined, inPrintBold, inPrintColor,
            inPrintFont, inPrintDoubleWidth, inPrintDoubleHeight, inIAO, inDBTransaction, inHandheldName, inItemIdentifier );

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert item in the DB: "
                 "course key: " + IntToStr( inCourseKey ) + "  " +
                 "item: "       + inDescription + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertItemInDBWithKey(
                        __int32 inKey, __int32 inCourseKey, WideString inDescription,
                        WideString inKitchenName, TColor inButtonColor, bool inDisplaySizes,
                        bool inEnabled, bool inItemOnlySide,
                        bool inPrintUnderlined, bool inPrintBold, TColor inPrintColor,
                        __int32 inPrintFont, bool inPrintDoubleWidth, bool inPrintDoubleHeight,
                        __int32 inIAO,
                        Database::TDBTransaction *inDBTransaction, WideString inHandheldName, int inItemIdentifier )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO ITEM ( ITEM_KEY, COURSE_KEY, ITEM_ID, ITEM_NAME, "
										   "DISPLAY_SIZES, ENABLED, IAO, BUTTON_COLOUR, PRINT_CHIT, "
										   "EXCLUSIVELY_AS_SIDE, ITEM_KITCHEN_NAME, PRINT_UNDERLINED, "
										   "PRINT_BOLD, PRINT_COLOUR, PRINT_FONT, PRINT_DOUBLE_WIDTH, "
										   "PRINT_DOUBLE_HEIGHT, HANDHELD_NAME, ITEM_IDENTIFIER) "
						"VALUES ( :key, :courseFKey, :itemID, :itemName, "
								 ":displaySizes, :enabled, :iao, :buttonColor, :printChit, "
								 ":onlySide,    :kitchenName, :printUnderlined, :printBold, "
								 ":printColor,   :printFont, :printDoubleWidth, :printDoubleHeight, :handheldName, :ITEM_IDENTIFIER ); ";

		qr->ParamByName("key"              )->AsInteger = inKey;
		qr->ParamByName("courseFKey"       )->AsInteger = inCourseKey;
		qr->ParamByName("itemID"           )->AsInteger = inKey;
		qr->ParamByName("itemName"         )->AsString  = inDescription;
		qr->ParamByName("displaySizes"     )->AsString  = inDisplaySizes ? "T" : "F";
		qr->ParamByName("enabled"          )->AsString  = inEnabled ? "T" : "F";
		qr->ParamByName("iao"              )->AsInteger = inIAO;
		qr->ParamByName("buttonColor"      )->AsInteger = (int)inButtonColor;
		qr->ParamByName("printChit"        )->AsString  = "T";
		qr->ParamByName("onlySide"         )->AsString  = inItemOnlySide ? "T" : "F";
		qr->ParamByName("kitchenName"      )->AsString  = inKitchenName;
		qr->ParamByName("printUnderlined"  )->AsString  = inPrintUnderlined ? "T" : "F";
		qr->ParamByName("printBold"        )->AsString  = inPrintBold ? "T" : "F";
		qr->ParamByName("printColor"       )->AsInteger = inPrintColor;
		qr->ParamByName("printFont"        )->AsInteger = inPrintFont;
		qr->ParamByName("printDoubleWidth" )->AsString  = inPrintDoubleWidth  ? "T" : "F";
		qr->ParamByName("printDoubleHeight")->AsString  = inPrintDoubleHeight ? "T" : "F";
        qr->ParamByName("handheldName"     )->AsString  = inHandheldName;
        qr->ParamByName("ITEM_IDENTIFIER"     )->AsString  = IntToStr(inItemIdentifier);

//PALM_ITEM_ID
//ITEM_CATEGORy

		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert size with key in the DB: "
                 "course key: " + IntToStr( inCourseKey ) + "  " +
                 "key: "        + IntToStr( inKey ) + "  " +
                 "item: "       + inDescription + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertForcedSideInDB( __int32 inItemFKey, __int32 inSideFKey, __int32 inIOO,
										   __int32 inGroupNumber, __int32 inMaxSelect, bool inSideGroupSkip,
										   Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_ITEMSIDES, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

		InsertForcedSideInDBWithKey(
            key, inItemFKey, inSideFKey, inIOO, inGroupNumber, inMaxSelect,
            inSideGroupSkip, inDBTransaction );

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert forced side in the DB: "
                 "itemFKey: " + IntToStr( inItemFKey ) + "  " +
                 "sideFKey: " + IntToStr( inSideFKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertForcedSideInDBWithKey(
                        __int32 inKey, __int32 inItemFKey, __int32 inSideFKey, __int32 inIOO,
                        __int32 inGroupNumber, __int32 inMaxSelect, bool inSideGroupSkip,
                        Database::TDBTransaction *inDBTransaction )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO ITEMSIDES ( ItemSides_Key, Master_Item_Key, Item_Key, "
												"IOO, Max_Select, Group_No, Allow_Skip ) "
						"VALUES ( :key, :itemFKey, :sideFKey, :ioo, :maxSelect, :groupNo, :allowSkip ); ";

		qr->ParamByName("key"      )->AsInteger = inKey;
		qr->ParamByName("itemFKey" )->AsInteger = inItemFKey;
		qr->ParamByName("sideFKey" )->AsInteger = inSideFKey;
		qr->ParamByName("ioo"      )->AsInteger = inIOO;
		qr->ParamByName("maxSelect")->AsInteger = inMaxSelect;
		qr->ParamByName("groupNo"  )->AsInteger = inGroupNumber;
		qr->ParamByName("allowSkip")->AsString  = inSideGroupSkip ? "T" : "F";

		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert forced side with key in the DB: "
                 "itemFKey: " + IntToStr( inItemFKey ) + "  " +
                 "sideFKey: " + IntToStr( inSideFKey ) + "  " +
                 "key: "      + IntToStr( inKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertForcedOptionInDB( __int32 inMasterItemKey, __int32 inItemKey,
                                             Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_FORCEDOPTIONS, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

        InsertForcedOptionInDBWithKey(
            key, inMasterItemKey, inItemKey, inDBTransaction );

		result = key;
	}
    catch( Exception& exc )
	{
       textLog = "Failed to insert forced option in the DB: "
                 "master FKey: " + IntToStr( inMasterItemKey ) + "  " +
                 "item FKey: "   + IntToStr( inItemKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertForcedOptionInDBWithKey(
                        __int32 inKey, __int32 inMasterItemKey, __int32 inItemKey,
                        Database::TDBTransaction *inDBTransaction )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO FORCEDOPTIONS ( ID, Option, Item ) "
						"VALUES ( :key, :optionFKey, :itemFKey ); ";

		qr->ParamByName("key"       )->AsInteger = inKey;
		qr->ParamByName("optionFKey")->AsInteger = inItemKey;
		qr->ParamByName("itemFKey"  )->AsInteger = inMasterItemKey;

		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert forced option with key in the DB: "
                 "key: "         + IntToStr( inKey ) + "  " +
                 "master FKey: " + IntToStr( inMasterItemKey ) + "  " +
                 "item FKey: "   + IntToStr( inItemKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertItemSizeInDB(
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
                        bool       inCanBePaidForUsingPoints,
                        int        inDefaultPatronCount,
                        Currency   inPriceForPoints,
                        int        inrevenueCode,
                        int        itemSizeIdentifier,
						Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_ITEMSIZE, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

        if( inPLU <= 0 )
            inPLU = GetNextItemSizePLU();

		//:::::::::::::::::::::::::::::::::::::::

        InsertItemSizeInDBWithKey(
            key, inMasterItemKey, inSizeKey, inItemID, inSizeID, inPalmItemID,
			inPalmSizeID, inSizeName, inPrice, inMaxRetailPrice, inSpecialPrice, inCost, inISAO,
			inFree, inAvailableAsStandard, inNoRecipe, inBarcode, inSetMenuMask,
			inEnabled, inGSTPercent, inCostGSTPercent, inPointsPercent, inSizeKitchenName,
			inThirdPartyCodeKey, inMemberPurchaseDiscount, inLocationPurchaseDiscount,
			inMemberPurchaseCount, inLocationPurchaseCount, inCategoryKey, inTareWeight,
			inHandheldName, inReceiptName, inPLU, inAvailableQuantity, inDefaultQuantity,
            inWarningQuantity, inDisableWhenCountReachesZero, inCanBePaidForUsingPoints,
            inDefaultPatronCount, inPriceForPoints,inrevenueCode, itemSizeIdentifier, inDBTransaction );

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert item size in the DB: "
                 "master item FKey: " + IntToStr( inMasterItemKey ) + "  " +
                 "sike FKey: "        + IntToStr( inSizeKey       ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertItemSizeInDBWithKey(
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
                        bool       inCanBePaidForUsingPoints,
                        int        inDefaultPatronCount,
                        Currency   inPriceForPoints,
                        int        inrevenueCode,
                        int        inItemSizeIdentifier,
						Database::TDBTransaction *inDBTransaction )
{
	try
	{
        if(inCost < -1000000 || inCost > 900000000)
            inCost = 0;

		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
        qr->Close();
		qr->SQL->Text = "INSERT INTO ITEMSIZE ( ItemSize_Key, Item_Key, Sizes_Key, Item_ID, Size_ID, Palm_Item_ID, Palm_Size_ID, "
											   "Size_Name, Price, MaxRetailPrice, Special_Price, Cost, ISAO, Free, Available_As_Standard, "
											   "No_Recipe, Barcode, SetMenu_Mask, Enabled, GST_Percent, Cost_GST_Percent, "
											   "Points_Percent, Size_Kitchen_Name, ThirdPartyCodes_Key, Mem_Discount_Percent, "
											   "Loc_Discount_Percent, Mem_Sale_Count, Loc_Sale_Count, Category_Key, "
											   "Tare_Weight, Handheld_Name, Receipt_Name, PLU, available_quantity, "
                                               "default_quantity, warning_quantity, disable_when_count_reaches_zero, "
											   "can_be_paid_for_using_points, default_patron_count, PRICE_FOR_POINTS,REVENUECODE, ITEMSIZE_IDENTIFIER) "
						"VALUES ( :key, :itemFKey, :sizeFKey, :itemID, :sizeID, :palmItemID, :palmSizeID, :sizeName, :price, :maxRetailPrice, "
								 ":specialPrice, :cost, :isao, :free, :availableAsStandard, :noRecipe, :barcode, :setMenuMask, "
								 ":enabled, :gstPercent, :costGSTPercent, :pointsPercent, :sizeKitchenName, :thirdPartyCodesKey, "
								 ":memDiscountPercent, :locDiscountPercent, :memSaleCount, :locSaleCount, :categoryKey, "
								 ":tareWeight, :handheldName, :receiptName, :plu, "
                                 ":available_quantity, :default_quantity, :warning_quantity, :disable_when_count_reaches_zero, "
								 ":can_be_paid_for_using_points, :default_patron_count, :PRICE_FOR_POINTS, :REVENUECODE, :ITEMSIZE_IDENTIFIER);";

		qr->ParamByName("key"                )->AsInteger = inKey;
		qr->ParamByName("itemFKey"           )->AsInteger = inMasterItemKey;
		qr->ParamByName("sizeFKey"           )->AsInteger = inSizeKey;
		qr->ParamByName("itemID"             )->AsInteger = inKey;
		qr->ParamByName("sizeID"             )->AsInteger = inSizeID;
		qr->ParamByName("palmItemID"         )->AsInteger = inPalmItemID;
		qr->ParamByName("palmSizeID"         )->AsInteger = inPalmSizeID;
		qr->ParamByName("sizeName"           )->AsString  = inSizeName;
		qr->ParamByName("price"              )->AsCurrency = inPrice;
        qr->ParamByName("maxRetailPrice"     )->AsCurrency = inMaxRetailPrice;
		qr->ParamByName("specialPrice"       )->AsCurrency = inSpecialPrice;
		qr->ParamByName("cost"               )->AsCurrency = inCost;
		qr->ParamByName("isao"               )->AsInteger = inISAO;
		qr->ParamByName("free"               )->AsString  = inFree ? "T" : "F";
		qr->ParamByName("availableAsStandard")->AsString  = inAvailableAsStandard ? "T" : "F";
		qr->ParamByName("noRecipe"           )->AsString  = inNoRecipe ? "T" : "F";
		qr->ParamByName("barcode"            )->AsString  = inBarcode;
		qr->ParamByName("setMenuMask"        )->AsInteger = inSetMenuMask;
		qr->ParamByName("enabled"            )->AsString  = inEnabled ? "T" : "F";
		qr->ParamByName("gstPercent"         )->AsFloat   = inGSTPercent;
		qr->ParamByName("costGSTPercent"     )->AsFloat   = inCostGSTPercent;
		qr->ParamByName("pointsPercent"      )->AsFloat   = inPointsPercent;
		qr->ParamByName("sizeKitchenName"    )->AsString  = inSizeKitchenName;
		qr->ParamByName("thirdPartyCodesKey" )->AsInteger = inThirdPartyCodeKey;
		qr->ParamByName("memDiscountPercent" )->AsFloat   = inMemberPurchaseDiscount;
		qr->ParamByName("locDiscountPercent" )->AsFloat   = inLocationPurchaseDiscount;
		qr->ParamByName("memSaleCount"       )->AsInteger = inMemberPurchaseCount;
		qr->ParamByName("locSaleCount"       )->AsInteger = inLocationPurchaseCount;
		qr->ParamByName("categoryKey"        )->AsInteger = inCategoryKey;
		qr->ParamByName("tareWeight"         )->AsCurrency = inTareWeight;
		qr->ParamByName("handheldName"       )->AsString  = inHandheldName;
		qr->ParamByName("receiptName"        )->AsString  = inReceiptName;
		qr->ParamByName("plu"                )->AsInteger = inPLU;

        qr->ParamByName("can_be_paid_for_using_points")->AsInteger = inCanBePaidForUsingPoints;
        qr->ParamByName("available_quantity")->AsCurrency = inAvailableQuantity;
        qr->ParamByName("default_quantity")->AsCurrency =  inDefaultQuantity;
        qr->ParamByName("warning_quantity")->AsCurrency = inWarningQuantity;
        qr->ParamByName("disable_when_count_reaches_zero")->AsInteger = inDisableWhenCountReachesZero;
        qr->ParamByName("default_patron_count")->AsInteger = inDefaultPatronCount;
        qr->ParamByName("PRICE_FOR_POINTS")->AsCurrency = inPriceForPoints;
        qr->ParamByName("REVENUECODE")->AsInteger = inrevenueCode;
        qr->ParamByName("ITEMSIZE_IDENTIFIER")->AsString = IntToStr(inItemSizeIdentifier);
		qr->ExecQuery();

	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert item size with key in the DB: "
                 "key: "              + IntToStr( inKey )           + "  " +
                 "master item FKey: " + IntToStr( inMasterItemKey ) + "  " +
                 "size FKey: "        + IntToStr( inSizeKey )       + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
// ASEAN ++
/*
__int32 TImportMenu::InsertItemSizeInDB(
						__int32    inMasterItemKey,
						__int32    inSizeKey,
						__int32    inItemID,
						__int32    inSizeID,
						__int32    inPalmItemID,
						__int32    inPalmSizeID,
						AnsiString inSizeName,
						Currency   inPrice,
						Currency   inSpecialPrice,
						Currency   inCost,
						__int32    inISAO,
						bool       inFree,
						bool       inAvailableAsStandard,
						bool       inNoRecipe,
						AnsiString inBarcode,
						__int32    inSetMenuMask,
						bool       inEnabled,
						double     inGSTPercent,
						double     inCostGSTPercent,
						double     inPointsPercent,
						AnsiString inSizeKitchenName,
						__int32    inThirdPartyCodeKey,
						double     inMemberPurchaseDiscount,
						double     inLocationPurchaseDiscount,
						__int32    inMemberPurchaseCount,
						__int32    inLocationPurchaseCount,
						__int32    inCategoryKey,
						double     inTareWeight,
						AnsiString inHandheldName,
						AnsiString inReceiptName,
						__int32    inPLU,
						__int16    inTaxExempt,
						__int16    inHasServiceCharge,
						double     inAvailableQuantity,
						double     inDefaultQuantity,
						double     inWarningQuantity,
						bool       inDisableWhenCountReachesZero,
						Database::TDBTransaction *inDBTransaction )

{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		inDBTransaction->StartTransaction();

		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = L"SELECT GEN_ID(GEN_ITEMSIZE, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO ITEMSIZE ( ItemSize_Key, Item_Key, Sizes_Key, Item_ID, Size_ID, Palm_Item_ID, Palm_Size_ID, "
											   "Size_Name, Price, Special_Price, Cost, ISAO, Free, Available_As_Standard, "
											   "No_Recipe, Barcode, SetMenu_Mask, Enabled, GST_Percent, Cost_GST_Percent, "
											   "Points_Percent, Size_Kitchen_Name, ThirdPartyCodes_Key, Mem_Discount_Percent, "
											   "Loc_Discount_Percent, Mem_Sale_Count, Loc_Sale_Count, Category_Key, "
											   "Tare_Weight, Handheld_Name, Receipt_Name, PLU, "
											   "has_service_charge, tax_exempt, "
											   "available_quantity, default_quantity, disable_when_count_reaches_zero ) "
						"VALUES ( :key, :itemFKey, :sizeFKey, :itemID, :sizeID, :palmItemID, :palmSizeID, :sizeName, :price, "
								 ":specialPrice, :cost, :isao, :free, :availableAsStandard, :noRecipe, :barcode, :setMenuMask, "
								 ":enabled, :gstPercent, :costGSTPercent, :pointsPercent, :sizeKitchenName, :thirdPartyCodesKey, "
								 ":memDiscountPercent, :locDiscountPercent, :memSaleCount, :locSaleCount, :categoryKey, "
								 ":tareWeight, :handheldName, :receiptName, :plu, :has_service_charge, :is_tax_exempt, "
								 ":available_quantity, :default_quantity, "
								 ":warning_quantity, :disable_when_count_reaches_zero); ";

		qr->ParamByName("key"                )->AsInteger = key;
		qr->ParamByName("itemFKey"           )->AsInteger = inMasterItemKey;
		qr->ParamByName("sizeFKey"           )->AsInteger = inSizeKey;
		qr->ParamByName("itemID"             )->AsInteger = inItemID;
		qr->ParamByName("sizeID"             )->AsInteger = inSizeID;
		qr->ParamByName("palmItemID"         )->AsInteger = inPalmItemID;
		qr->ParamByName("palmSizeID"         )->AsInteger = inPalmSizeID;
		qr->ParamByName("sizeName"           )->AsString  = inSizeName;
		qr->ParamByName("price"              )->AsCurrency = inPrice;
		qr->ParamByName("specialPrice"       )->AsCurrency = inSpecialPrice;
		qr->ParamByName("cost"               )->AsCurrency = inCost;
		qr->ParamByName("isao"               )->AsInteger = inISAO;
		qr->ParamByName("free"               )->AsString  = inFree ? "T" : "F";
		qr->ParamByName("availableAsStandard")->AsString  = inAvailableAsStandard ? "T" : "F";
		qr->ParamByName("noRecipe"           )->AsString  = inNoRecipe ? "T" : "F";
		qr->ParamByName("barcode"            )->AsString  = inBarcode;
		qr->ParamByName("setMenuMask"        )->AsInteger = inSetMenuMask;
		qr->ParamByName("enabled"            )->AsString  = inEnabled ? "T" : "F";
		qr->ParamByName("gstPercent"         )->AsFloat   = inGSTPercent;
		qr->ParamByName("costGSTPercent"     )->AsFloat   = inCostGSTPercent;
		qr->ParamByName("pointsPercent"      )->AsFloat   = inPointsPercent;
		qr->ParamByName("sizeKitchenName"    )->AsString  = inSizeKitchenName;
		qr->ParamByName("thirdPartyCodesKey" )->AsInteger = inThirdPartyCodeKey;
		qr->ParamByName("memDiscountPercent" )->AsFloat   = inMemberPurchaseDiscount;
		qr->ParamByName("locDiscountPercent" )->AsFloat   = inLocationPurchaseDiscount;
		qr->ParamByName("memSaleCount"       )->AsInteger = inMemberPurchaseCount;
		qr->ParamByName("locSaleCount"       )->AsInteger = inLocationPurchaseCount;
		qr->ParamByName("categoryKey"        )->AsInteger = inCategoryKey;
		qr->ParamByName("tareWeight"         )->AsCurrency = inTareWeight;
		qr->ParamByName("handheldName"       )->AsString  = inHandheldName;
		qr->ParamByName("receiptName"        )->AsString  = inReceiptName;
		qr->ParamByName("plu"                )->AsInteger = inPLU;

		qr->ParamByName("is_tax_exempt"      )->AsInteger = inTaxExempt;
		qr->ParamByName("has_service_charge" )->AsInteger = inHasServiceCharge;

		qr->ParamByName("available_quantity" )->AsFloat = inAvailableQuantity;
		qr->ParamByName("default_quantity"   )->AsFloat = inDefaultQuantity;
		qr->ParamByName("warning_quantity"   )->AsFloat = inWarningQuantity;
		qr->ParamByName("disable_when_count_reaches_zero")->AsInteger =
		  inDisableWhenCountReachesZero;

		qr->ExecQuery();

		inDBTransaction->Commit();

		result = key;
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
*/
__int32 TImportMenu::InsertBCategoryInDB( __int32 inItemSizeKey, __int32 inCategoryKey,
										  Database::TDBTransaction *inDBTransaction )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO ITEMSIZECATEGORY ( ItemSize_Key, Category_Key ) "
						"VALUES ( :itemSizeFKey, :categoryFKey ); ";

		qr->ParamByName("itemSizeFKey")->AsInteger = inItemSizeKey;
		qr->ParamByName("categoryFKey")->AsInteger = inCategoryKey;

		qr->ExecQuery();

		result = 1;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert breakdown category in the DB: "
                 "item size FKey: " + IntToStr( inItemSizeKey ) + "  " +
                 "category FKey: "  + IntToStr( inCategoryKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertItemSizeReceipeInDB( __int32 inItemSizeKey, WideString inStockCode,
												double inQuantity, WideString inLocation,
												Database::TDBTransaction *inDBTransaction  )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_RECIPE, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

        InsertItemSizeReceipeInDBWithKey(
            key, inItemSizeKey, inStockCode, inQuantity, inLocation,
            inDBTransaction  );

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert item size receipe with key in the DB: "
                 "item size FKey: " + IntToStr( inItemSizeKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertItemSizeReceipeInDBWithKey(
                        __int32 inKey, __int32 inItemSizeKey, WideString inStockCode,
						double inQuantity, WideString inLocation,
						Database::TDBTransaction *inDBTransaction  )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO RECIPE ( Recipe_Key, ItemSize_Key, Stock_Code, QTY, Stock_Location,ORGINAL_STOCK_CODE ) "
						"VALUES ( :key, :itemSizeFKey, :stockCode, :qty, :stockLocation,:ORGINAL_STOCK_CODE ); ";

		qr->ParamByName("key"          )->AsInteger = inKey;
		qr->ParamByName("itemSizeFKey" )->AsInteger = inItemSizeKey;
		qr->ParamByName("stockCode"    )->AsString  = inStockCode;
                qr->ParamByName("ORGINAL_STOCK_CODE"    )->AsString  = inStockCode;
		qr->ParamByName("qty"          )->AsFloat   = inQuantity;
		qr->ParamByName("stockLocation")->AsString  = inLocation;

		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert item size receipe with key in the DB: "
                 "key: "            + IntToStr( inKey ) + ": " + "";
                 "item size FKey: " + IntToStr( inItemSizeKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::InsertItemSizeTaxProfileInDB( __int32 inItemSizeKey, __int32 inTaxProfileKey,
										           Database::TDBTransaction *inDBTransaction  )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_TAXPROFILES_ITEMSIZE, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		__int32 key = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

        InsertItemSizeTaxProfileInDBWithKey( key, inItemSizeKey, inTaxProfileKey, inDBTransaction );

		result = key;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert item size tax profile with key in the DB: "
                 "item size FKey: " + IntToStr( inItemSizeKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TImportMenu::InsertItemSizeTaxProfileInDBWithKey(
                        __int32 inKey, __int32 inItemSizeKey, __int32 inTaxProfileKey,
                        Database::TDBTransaction *inDBTransaction  )
{
	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "INSERT INTO TAXPROFILES_ITEMSIZE ( TPIS_Key, Profile_Key, Itemsize_Key ) "
						"VALUES ( :key, :taxProfileFKey, :itemSizeFKey ); ";

		qr->ParamByName("key"            )->AsInteger = inKey;
		qr->ParamByName("itemSizeFKey"   )->AsInteger = inItemSizeKey;
		qr->ParamByName("taxProfileFKey" )->AsInteger = inTaxProfileKey;

		qr->ExecQuery();
	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert item size tax profile with key in the DB: "
                 "key: "            + IntToStr( inKey ) + ": " + "";
                 "item size FKey: " + IntToStr( inItemSizeKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
// ---------------------------------------------------------------------------
// Update_InDB functions update objects in the DB.
bool TImportMenu::UpdateSizeInDB( __int32 inKey, __int32 inMenuKey, WideString inSizeName,
								  WideString inKitchenName, WideString inHandheldName,
								  WideString inReceiptName, bool inWeighted, __int32 inSizeID,
								  __int32 inPalmSizeID, Database::TDBTransaction* inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE SIZES "
						"SET Menu_Key = :menuFKey, Size_Name = :sizeName, Size_ID = :sizeID, Palm_Size_ID = :palmSizeID, "
							"Size_Kitchen_Name = :sizeKitchenName, Weighted_Size = :weighted, Handheld_Name = :sizeHandheldName, "
							"Receipt_Name = :sizeReceiptName "
						"WHERE Sizes_Key = :key;";

		// Field SAO is not used

		qr->ParamByName("key"             )->AsInteger = inKey;
		qr->ParamByName("menuFKey"        )->AsInteger = inMenuKey;
		qr->ParamByName("sizeName"        )->AsString  = inSizeName;
		qr->ParamByName("sizeID"          )->AsInteger = inSizeID;
		qr->ParamByName("palmSizeID"      )->AsInteger = inPalmSizeID;
		qr->ParamByName("sizeKitchenName" )->AsString  = inKitchenName;
		qr->ParamByName("weighted"        )->AsString  = inWeighted ? "T" : "F";
		qr->ParamByName("sizeHandheldName")->AsString  = inHandheldName;
		qr->ParamByName("sizeReceiptName" )->AsString  = inReceiptName;

		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update size with key in the DB: "
                 "key: "       + IntToStr( inKey ) + "  " +
                 "size name: " + inSizeName        + ": " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
bool TImportMenu::UpdateCategoryGroupInDB( __int32 inKey, WideString inDescription,
										   bool inCGDeleted, Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE CATEGORYGROUPS "
						"SET Name = :name, Visible = :visible "
						"WHERE CategoryGroups_Key = :key;";

		qr->ParamByName("key"    )->AsInteger = inKey;
		qr->ParamByName("name"   )->AsString  = inDescription;
		qr->ParamByName("visible")->AsString  = !inCGDeleted ? "T" : "F";

		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update category group with key in the DB: "
                 "key: "            + IntToStr( inKey ) + "  " +
                 "category group: " + inDescription     + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::UpdateCategoryInDB( __int32 inKey, WideString inDescription, WideString inGLCode, bool inDeleted,
										 __int32 inCGroupKey, Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE ARCCATEGORIES "
						"SET Category = :category, CategoryGroups_Key = :categoryGroupFKey, Visible = :visible, GL_CODE = :GL_Code "
						"WHERE Category_Key = :key;";

		qr->ParamByName("key"              )->AsInteger = inKey;
		qr->ParamByName("category"         )->AsString  = inDescription;
		qr->ParamByName("categoryGroupFKey")->AsInteger = inCGroupKey;
		qr->ParamByName("visible"          )->AsString  = !inDeleted ? "T" : "F";
        qr->ParamByName("GL_Code"         )->AsString  = inGLCode;
		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update category in the DB: "
                 "key: "      + IntToStr( inKey ) + "  " +
                 "category: " + inDescription     + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::UpdateServingCourseInDB( __int32 inKey, WideString inDescription, WideString inKitchenName,
											  bool inSCDeleted, bool inSelectable,
											  TColor inColor, __int32 inDisplayOrder, Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE SERVINGCOURSES "
						"SET ServingCourse_Name = :servingCourseName, ServingCourse_Kitchen_Name = :servingCourseKitchenName, "
							"Deleted = :deleted, Selectable = :selectable, Colour = :colour, Display_ORDER = :order "
						"WHERE ServingCourses_Key = :key;";

		qr->ParamByName("key"                     )->AsInteger = inKey;
		qr->ParamByName("servingCourseName"       )->AsString  = inDescription;
		qr->ParamByName("servingCourseKitchenName")->AsString  = inKitchenName;
		qr->ParamByName("deleted"                 )->AsString  = inSCDeleted  ? "T" : "F";
		qr->ParamByName("selectable"              )->AsString  = inSelectable ? "T" : "F";
		qr->ParamByName("colour"                  )->AsInteger = (int)inColor;
        qr->ParamByName("order"                   )->AsInteger = inDisplayOrder;

		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update serving course in the DB: "
                 "key: "            + IntToStr( inKey ) + "  " +
                 "serving course: " + inDescription     + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------

__int32 TImportMenu::UpdateCourseInDB( __int32 inCourseKey, __int32 inMenuKey, WideString inDescription,
									   WideString inKitchenName, WideString inHandheldName, WideString inReceiptName,
									   __int32 inLocation, bool inEnabled, __int32 inServingCourseKey,
									   bool inNoDefaultServingCourse, Database::TDBTransaction* inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE COURSE "
						"SET Menu_Key = :menuFKey, Course_Name = :courseName, CAO = :cao, Enabled = :enabled, "
							"View_Location = :viewLocation, Course_Kitchen_Name = :courseKitchenName, "
							"ServingCourses_Key = :servingCourseKey, No_Default_Serving_Course = :noDefaultServingCourse, "
							"Handheld_Name = :handheldName, Receipt_Name = :receiptName "
						"WHERE Course_Key = :key;";

		qr->ParamByName("key"                   )->AsInteger = inCourseKey;
		qr->ParamByName("menuFKey"              )->AsInteger = inMenuKey;
		qr->ParamByName("courseName"            )->AsString  = inDescription;
		qr->ParamByName("enabled"               )->AsString  = inEnabled ? "T" : "F";
		qr->ParamByName("viewLocation"          )->AsInteger = inLocation;
		qr->ParamByName("courseKitchenName"     )->AsString  = inKitchenName;
		qr->ParamByName("servingCourseKey"      )->AsInteger = inServingCourseKey;
		qr->ParamByName("noDefaultServingCourse")->AsString  = inNoDefaultServingCourse ? "T" : "F";
		qr->ParamByName("handheldName"          )->AsString  = inHandheldName;
		qr->ParamByName("receiptName"           )->AsString  = inHandheldName;

		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update course in the DB: "
                 "key: "    + IntToStr( inCourseKey ) + "  " +
                 "course: " + inDescription     + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::UpdateOptionInDB( __int32 inOptionKey, __int32 inCourseKey, WideString inDescription,
									   WideString inKitchenName, WideString inHandheldName, WideString inReceiptName,
									   __int32 inCondimentsMask, bool inFlags, bool inEnabled, bool inPrintUnderlined,
									   bool inPrintBold, __int32 inPrintColor, __int32 inPrintFont,
									   bool inPrintDoubleWidth, bool inPrintDoubleHeight,
									   Database::TDBTransaction* inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE OPTIONS "
						"SET Course_Key = :courseFKey, Option_Name = :optionName, "
							"Enabled = :enabled, Flags = :flags, Option_Kitchen_Name = :optionKitchenName, "
							"Print_Underlined = :printUnderlined, Print_Bold = :printBold, Print_Colour = :printColor, "
							"Print_Font = :printFont, Print_Double_Width = :printDoubleWidth, "
							"Print_Double_Height = :printDoubleHeight, Handheld_Name = :handheldName, "
							"Receipt_Name = :receiptName "
						"WHERE Options_Key = :key;";

		qr->ParamByName("key"              )->AsInteger = inOptionKey;
		qr->ParamByName("courseFKey"       )->AsInteger = inCourseKey;
		qr->ParamByName("optionName"       )->AsString  = inDescription;
		qr->ParamByName("enabled"          )->AsString  = inEnabled ? "T" : "F";
		qr->ParamByName("flags"            )->AsInteger = inFlags;
		qr->ParamByName("optionKitchenName")->AsString  = inKitchenName;
		qr->ParamByName("printUnderlined"  )->AsString  = inPrintUnderlined ? "T" : "F";
		qr->ParamByName("printBold"        )->AsString  = inPrintBold ? "T" : "F";
		qr->ParamByName("printColor"       )->AsInteger = inPrintColor;
		qr->ParamByName("printFont"        )->AsInteger = inPrintFont;
		qr->ParamByName("printDoubleWidth" )->AsString  = inPrintDoubleWidth  ? "T" : "F";
		qr->ParamByName("printDoubleHeight")->AsString  = inPrintDoubleHeight ? "T" : "F";
		qr->ParamByName("handheldName"     )->AsString  = inHandheldName;
		qr->ParamByName("receiptName"      )->AsString  = inReceiptName;

		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update option in the DB: "
                 "course FKey: " + IntToStr( inCourseKey ) + "  " +
                 "key: "         + IntToStr( inOptionKey ) + "  " +
                 "option: "      + inDescription     + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::UpdateItemInDB( __int32 inItemKey, __int32 inCourseKey, WideString inDescription,
									 WideString inKitchenName, __int32 inButtonColor, bool inDisplaySizes,
									 bool inEnabled, bool inItemOnlySide, bool inPrintUnderlined,
									 bool inPrintBold, __int32 inPrintColor, __int32 inPrintFont,
									 bool inPrintDoubleWidth, bool inPrintDoubleHeight,
									 Database::TDBTransaction* inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE ITEM "
						"SET COURSE_KEY = :courseFKey, ITEM_NAME = :itemName, DISPLAY_SIZES = :displaySizes, "
							"ENABLED = :enabled, BUTTON_COLOUR = :buttonColor, PRINT_CHIT = :printChit, "
							"EXCLUSIVELY_AS_SIDE = :onlySide, ITEM_KITCHEN_NAME = :kitchenName, "
							"PRINT_UNDERLINED = :printUnderlined, PRINT_BOLD = :printBold, "
							"PRINT_COLOUR = :printColor, PRINT_FONT = :printFont, PRINT_DOUBLE_WIDTH = :printDoubleWidth, "
							"PRINT_DOUBLE_HEIGHT = :printDoubleHeight "
						"WHERE ITEM_KEY = :key;";

		qr->ParamByName("key"              )->AsInteger = inItemKey;
		qr->ParamByName("courseFKey"       )->AsInteger = inCourseKey;
		qr->ParamByName("itemName"         )->AsString  = inDescription;
		qr->ParamByName("displaySizes"     )->AsString  = inDisplaySizes ? "T" : "F";
		qr->ParamByName("enabled"          )->AsString  = inEnabled ? "T" : "F";
		qr->ParamByName("buttonColor"      )->AsInteger = (int)inButtonColor;
		qr->ParamByName("printChit"        )->AsString  = "T";
		qr->ParamByName("onlySide"         )->AsString  = inItemOnlySide ? "T" : "F";
		qr->ParamByName("kitchenName"      )->AsString  = inKitchenName;
		qr->ParamByName("printUnderlined"  )->AsString  = inPrintUnderlined ? "T" : "F";
		qr->ParamByName("printBold"        )->AsString  = inPrintBold ? "T" : "F";
		qr->ParamByName("printColor"       )->AsInteger = inPrintColor;
		qr->ParamByName("printFont"        )->AsInteger = inPrintFont;
		qr->ParamByName("printDoubleWidth" )->AsString  = inPrintDoubleWidth  ? "T" : "F";
		qr->ParamByName("printDoubleHeight")->AsString  = inPrintDoubleHeight ? "T" : "F";

		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update item in the DB: "
                 "course FKey: " + IntToStr( inCourseKey ) + "  " +
                 "key: "         + IntToStr( inItemKey   ) + "  " +
                 "option: "      + inDescription     + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::UpdateForcedSideInDB( __int32 inForcedSideKey, __int32 inItemFKey,
										   __int32 inSideFKey, __int32 inGroupNumber,
										   __int32 inMaxSelect, bool inSideGroupSkip,
										   Database::TDBTransaction* inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE ITEMSIDES "
						"SET Master_Item_Key = :itemFKey, Item_Key = :sideFKey, Max_Select = :maxSelect, "
							"Group_No = :groupNo, Allow_Skip = :allowSkip "
						"WHERE ItemSides_Key = :key;";

		qr->ParamByName("key"      )->AsInteger = inForcedSideKey;
		qr->ParamByName("itemFKey" )->AsInteger = inItemFKey;
		qr->ParamByName("sideFKey" )->AsInteger = inSideFKey;
		qr->ParamByName("maxSelect")->AsInteger = inMaxSelect;
		qr->ParamByName("groupNo"  )->AsInteger = inGroupNumber;
		qr->ParamByName("allowSkip")->AsString  = inSideGroupSkip ? "T" : "F";

		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update forced side in the DB: "
                 "forced side key: " + IntToStr( inForcedSideKey ) + "  " +
                 "item FKey: "       + IntToStr( inItemFKey   )    + "  " +
                 "side FKey: "       + IntToStr( inSideFKey   )    + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::UpdateForcedOptionInDB( __int32 inForcedOptionKey, __int32 inMasterItemKey,
											 __int32 inOptionKey, Database::TDBTransaction* inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE FORCEDOPTIONS "
						"SET Option = :optionFKey, Item = :itemFKey "
						"WHERE ID = :key;";

		qr->ParamByName("key"       )->AsInteger = inForcedOptionKey;
		qr->ParamByName("optionFKey")->AsInteger = inOptionKey;
		qr->ParamByName("itemFKey"  )->AsInteger = inMasterItemKey;

		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update forced option in the DB: "
                 "forced option key: " + IntToStr( inForcedOptionKey ) + "  " +
                 "master item FKey: "  + IntToStr( inMasterItemKey   ) + "  " +
                 "option FKey: "       + IntToStr( inOptionKey       ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::UpdateItemSizeInDB(
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
                                                Currency inAvailableQuantity,
                                                Currency inDefaultQuantity,
                                                Currency inWarningQuantity,
                                                bool inDisableWhenCountReachesZero,
						bool inCanBePaidForUsingPoints,
                        Currency   inPriceForPoints,
                        __int32    inRevenueCode,
						Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
        if(inCost < -1000000 || inCost > 900000000)
            inCost = 0;

		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE ITEMSIZE "
						"SET Item_Key = :itemFKey, Sizes_Key = :sizeFKey, "
						    //"Item_ID = :itemID, Size_ID = :sizeID, "
							"Palm_Item_ID = :palmItemID, Palm_Size_ID = :palmSizeID, Size_Name = :sizeName, "
							"Price = :price, Special_Price = :specialPrice, Cost = :cost, Free = :free, "
							"Available_As_Standard = :availableAsStandard, No_Recipe = :noRecipe, "
							"Barcode = :barcode, SetMenu_Mask = :setMenuMask, Enabled = :enabled, GST_Percent = :gstPercent, "
							"Cost_GST_Percent = :costGSTPercent, Points_Percent = :pointsPercent, Size_Kitchen_Name = :sizeKitchenName, "
							"ThirdPartyCodes_Key = :thirdPartyCodesKey, Mem_Discount_Percent = :memDiscountPercent, "
							"Loc_Discount_Percent = :locDiscountPercent, Mem_Sale_Count = :memSaleCount, "
							"Loc_Sale_Count = :locSaleCount, Category_Key = :categoryKey, Tare_Weight = :tareWeight, "
							"Handheld_Name = :handheldName, Receipt_Name = :receiptName, PLU = :plu, "
                                                        "available_quantity = :available_quantity, "
                                                        "default_quantity = :default_quantity, "
                                                        "warning_quantity = :warning_quantity, "
                                                        "disable_when_count_reaches_zero = :disable_when_count_reaches_zero, "
                                                        "can_be_paid_for_using_points = :can_be_paid_for_using_points, "
                                                        "PRICE_FOR_POINTS = :PRICE_FOR_POINTS, REVENUECODE =: REVENUECODE "
						"WHERE ItemSize_Key = :key;";

		qr->ParamByName("key"                )->AsInteger  = inItemSizeKey;
		qr->ParamByName("itemFKey"           )->AsInteger  = inMasterItemKey;
		qr->ParamByName("sizeFKey"           )->AsInteger  = inSizeKey;
		//qr->ParamByName("itemID"             )->AsInteger  = inItemID;
		//qr->ParamByName("sizeID"             )->AsInteger  = inSizeID;
		qr->ParamByName("palmItemID"         )->AsInteger  = inPalmItemID;
		qr->ParamByName("palmSizeID"         )->AsInteger  = inPalmSizeID;
		qr->ParamByName("sizeName"           )->AsString   = inSizeName;
		qr->ParamByName("price"              )->AsCurrency = inPrice;
		qr->ParamByName("specialPrice"       )->AsCurrency = inSpecialPrice;
		qr->ParamByName("cost"               )->AsCurrency = inCost;
		qr->ParamByName("free"               )->AsString   = inFree ? "T" : "F";
		qr->ParamByName("availableAsStandard")->AsString   = inAvailableAsStandard ? "T" : "F";
		qr->ParamByName("noRecipe"           )->AsString   = inNoRecipe ? "T" : "F";
		qr->ParamByName("barcode"            )->AsString   = inBarcode;
		qr->ParamByName("setMenuMask"        )->AsInteger  = inSetMenuMask;
		qr->ParamByName("enabled"            )->AsString   = inEnabled ? "T" : "F";
		qr->ParamByName("gstPercent"         )->AsFloat    = inGSTPercent;
		qr->ParamByName("costGSTPercent"     )->AsFloat    = inCostGSTPercent;
		qr->ParamByName("pointsPercent"      )->AsFloat    = inPointsPercent;
		qr->ParamByName("sizeKitchenName"    )->AsString   = inSizeKitchenName;
		qr->ParamByName("thirdPartyCodesKey" )->AsInteger  = inThirdPartyCodeKey;
		qr->ParamByName("memDiscountPercent" )->AsFloat    = inMemberPurchaseDiscount;
		qr->ParamByName("locDiscountPercent" )->AsFloat    = inLocationPurchaseDiscount;
		qr->ParamByName("memSaleCount"       )->AsInteger  = inMemberPurchaseCount;
		qr->ParamByName("locSaleCount"       )->AsInteger  = inLocationPurchaseCount;
		qr->ParamByName("categoryKey"        )->AsInteger  = inCategoryKey;
		qr->ParamByName("tareWeight"         )->AsCurrency  = inTareWeight;
		qr->ParamByName("handheldName"       )->AsString   = inHandheldName;
		qr->ParamByName("receiptName"        )->AsString   = inReceiptName;
		qr->ParamByName("plu"                )->AsInteger  = inPLU;

		qr->ParamByName("can_be_paid_for_using_points")->AsInteger =
		  inCanBePaidForUsingPoints;
                qr->ParamByName("available_quantity")->AsCurrency =
                  inAvailableQuantity;
                qr->ParamByName("default_quantity")->AsCurrency =
                  inDefaultQuantity;
                qr->ParamByName("warning_quantity")->AsCurrency =
                  inWarningQuantity;
                qr->ParamByName("disable_when_count_reaches_zero")->AsInteger =
                  inDisableWhenCountReachesZero;
                qr->ParamByName("PRICE_FOR_POINTS")->AsCurrency =
                  inPriceForPoints;
                qr->ParamByName("REVENUECODE")->AsInteger =
                  inRevenueCode;

		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update item size in the DB: "
                 "item size key: "    + IntToStr( inItemSizeKey )     + "  " +
                 "master item FKey: " + IntToStr( inMasterItemKey   ) + "  " +
                 "size FKey: "        + IntToStr( inSizeKey       )   + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}

__int32 TImportMenu::UpdateItemSizeReceipeInDB( __int32 inReceipeKey, __int32 inItemSizeKey,
												WideString inStockCode, double inQuantity, WideString inLocation,
												Database::TDBTransaction* inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "UPDATE RECIPE "
						"SET Stock_Code = :stockCode, QTY = :qty, Stock_Location = :stockLocation "
						"WHERE Recipe_Key = :recipeKey AND ItemSize_Key = :itemSizeKey;";

		qr->ParamByName("recipeKey"    )->AsInteger = inReceipeKey;
		qr->ParamByName("itemSizeKey"  )->AsInteger = inItemSizeKey;
		qr->ParamByName("stockCode"    )->AsString  = inStockCode;
		qr->ParamByName("qty"          )->AsFloat   = inQuantity;
		qr->ParamByName("stockLocation")->AsString  = inLocation;

		qr->ExecQuery();

		result = true;
	}
	catch( Exception& exc )
	{
       textLog = "Failed to update item size receipe in the DB: "
                 "receipe key: "    + IntToStr( inReceipeKey )  + "  " +
                 "item size FKey: " + IntToStr( inItemSizeKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Tool functions used when saving menus in DB from an xml file
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool TImportMenu::resetNextNewSAO( __int32 inMenuKey, Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "SELECT MAX( sao ) as maxSAO FROM SIZES WHERE Menu_Key=:menuKey;";

		qr->ParamByName( "menuKey" )->AsInteger = inMenuKey;

		qr->ExecQuery();

		_newSAO = qr->FieldByName( "maxSAO" )->AsInteger;

		result = true;
	}
	catch( Exception& exc )
	{
		_newSAO = 0;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::getNextNewSAO()
{
   return ++_newSAO;
}
// ---------------------------------------------------------------------------
bool TImportMenu::resetNextNewDisplayOrder( Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "SELECT MAX( display_order ) as maxDOrder FROM SERVINGCOURSES;";

		qr->ExecQuery();

		_newDisplayOrder = qr->FieldByName( "maxDOrder" )->AsInteger;

		result = true;
	}
	catch( ... )
	{
		_newDisplayOrder = 0;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::getNextNewDisplayOrder()
{
   return ++_newDisplayOrder;
}
// ---------------------------------------------------------------------------
bool TImportMenu::resetNextNewCAO( __int32 inMenuKey, Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "SELECT MAX( cao ) as maxCAO FROM COURSE WHERE Menu_Key=:menuKey;";

		qr->ParamByName( "menuKey" )->AsInteger = inMenuKey;

		qr->ExecQuery();

		_newCAO = qr->FieldByName( "maxCAO" )->AsInteger;

		result = true;
	}
	catch( ... )
	{
		_newCAO = 0;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::getNextNewCAO()
{
   return ++_newCAO;
}
// ---------------------------------------------------------------------------
bool TImportMenu::resetNextNewOptionOrder( __int32 inCourseKey, Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "SELECT MAX( option_order ) as maxOOrder FROM OPTIONS WHERE Course_Key=:courseKey;";

		qr->ParamByName( "courseKey" )->AsInteger = inCourseKey;

		qr->ExecQuery();

		_newOptionOrder = qr->FieldByName( "maxOOrder" )->AsInteger;

		result = true;
	}
	catch( ... )
	{
		_newOptionOrder = 0;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::getNextNewOptionOrder()
{
   return ++_newOptionOrder;
}
// ---------------------------------------------------------------------------
bool TImportMenu::resetNextNewIAO( __int32 inCourseKey, Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "SELECT MAX( iao ) as maxIAO FROM ITEM WHERE Course_Key=:courseKey;";

		qr->ParamByName( "courseKey" )->AsInteger = inCourseKey;

		qr->ExecQuery();

		_newIAO = qr->FieldByName( "maxIAO" )->AsInteger;

		result = true;
	}
	catch( ... )
	{
		_newIAO = 0;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::getNextNewIAO()
{
   return ++_newIAO;
}
// ---------------------------------------------------------------------------
bool TImportMenu::resetNextNewIOO( __int32 inMasterItemKey, Database::TDBTransaction *inDBTransaction)
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "SELECT MAX( ioo ) as maxIOO FROM ITEMSIDES WHERE Master_Item_Key = :masterItemKey;";

		qr->ParamByName( "masterItemKey" )->AsInteger = inMasterItemKey;

		qr->ExecQuery();

		_newIOO = qr->FieldByName( "maxIOO" )->AsInteger;

		result = true;
	}
	catch( ... )
	{
		_newIOO = 0;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::getNextNewIOO()
{
   return ++_newIOO;
}
// ---------------------------------------------------------------------------
bool TImportMenu::resetNextNewISAO( __int32 inItemKey, Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "SELECT MAX( isao ) as maxISAO FROM ITEMSIZE WHERE Item_Key=:itemKey;";

		qr->ParamByName( "itemKey" )->AsInteger = inItemKey;

		qr->ExecQuery();

		_newISAO = qr->FieldByName( "maxISAO" )->AsInteger;

		result = true;
	}
	catch( ... )
	{
		_newISAO = 0;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::getNextNewISAO()
{
   return ++_newISAO;
}
// ---------------------------------------------------------------------------
bool TImportMenu::resetNextNewCOO( __int32 inCGroupKey, Database::TDBTransaction *inDBTransaction )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr->SQL->Text = "SELECT MAX( coo ) as maxCOO FROM ARCCATEGORIES WHERE CategoryGroups_Key=:cgrpKey;";

		qr->ParamByName( "cgrpKey" )->AsInteger = inCGroupKey;

		qr->ExecQuery();

		_newCOO = qr->FieldByName( "maxCOO" )->AsInteger;

		result = true;
	}
	catch( ... )
	{
		_newCOO = 0;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::getNextNewCOO()
{
   return ++_newCOO;
}
// ---------------------------------------------------------------------------

void TImportMenu::MoveMenuFile( UnicodeString inFileName, UnicodeString inDestPath )
{
    UnicodeString destFileName;
	if( !DirectoryExists( inDestPath ) )
	{
		ForceDirectories( inDestPath );
	}

	destFileName = inDestPath + Now().FormatString("yyyy-mm-dd - hh-nn-ss ") +
							  ExtractFileName( inFileName );
	if( FileExists( destFileName ) )
	{
		DeleteFile( destFileName );
	}

	if( CopyFileW( inFileName.c_str(), destFileName.c_str(), false ) != 0 )
	{
		if( FileExists( inFileName ) )
		{
			DeleteFile( inFileName );
		}
	}

}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Key Map: It maps new keys from the XML file with actual DB keys.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TImportMenu::ClearKeyMap()
{
    DB_ELEMENT_KEY_MAP::iterator elemIT;

    for( elemIT = _keyMap.begin(); elemIT != _keyMap.end(); elemIT++ )
    {
        elemIT->second.clear();
    }

	_keyMap.clear();
}
// ---------------------------------------------------------------------------
void TImportMenu::AddKeyMap(
                    AnsiString inMapName,
                       __int32 inTmpKey,
                       __int32 inActualKey )
{
	_keyMap[inMapName][inTmpKey] = inActualKey;
}
// ---------------------------------------------------------------------------
__int32 TImportMenu::GetKeyMap(
                        AnsiString inMapName,
                           __int32 inTmpKey )
{
   try
   {
		DB_KEY_MAP::iterator kmIT = _keyMap[inMapName].find( inTmpKey );

        return kmIT == _keyMap[inMapName].end() ? inTmpKey : kmIT->second;
   }
   catch( ... )
   {
		return inTmpKey;
   }
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Item Key Map: It maps item DB keys (itemKey) with item XML file Handles (itemXMLHandle).
// It's used to read the Forced Sides from the XML file.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TImportMenu::ClearItemMap()
{
	_itemMap.clear();
}
// ---------------------------------------------------------------------------
void TImportMenu::AddItemMap( __int32 inItemKey, __int32 inItemXMLHandle )
{
	_itemMap[inItemKey] = inItemXMLHandle;
}
// ---------------------------------------------------------------------------
//  NOT USED SO FAR!!.
__int32 TImportMenu::GetItemMap( __int32 inItemKey )
{
   try
   {
		std::map<__int32, __int32>::iterator imIT = _itemMap.find( inItemKey );

		return imIT == _itemMap.end() ? 0 : imIT->second;
   }
   catch( ... )
   {
		return 0;
   }
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Existing item keys set. this is used to see whether a generated id is already exists in the database.
// This is a rare situation when dealing with older menumate databases
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void    TImportMenu::ClearExistingItemKeys()
{
    _existingItemKeys.clear();
}

// ---------------------------------------------------------------------------
void    TImportMenu::LoadExistingItemKeys( __int32 latestGeneratedItemID, Database::TDBTransaction *inDBTransaction )
{
    TIBSQL *qr = inDBTransaction->Query(inDBTransaction->AddQuery());

    qr->SQL->Text = "SELECT ITEM_KEY from ITEM where ITEM_KEY > :generatedID;";

    qr->ParamByName( "generatedID" )->AsInteger = latestGeneratedItemID;

    qr->ExecQuery();

    for (;!qr->Eof; qr->Next())
	{
		__int32 key = qr->FieldByName("ITEM_KEY")->AsInteger;
        _existingItemKeys.insert(key);
	}
}

// ---------------------------------------------------------------------------
bool    TImportMenu::IsItemKeyExists( __int32 inItemKey )
{
    return _existingItemKeys.find(inItemKey) != _existingItemKeys.end();
}
// ---------------------------------------------------------------------------

__int32 TImportMenu::GetLatestItemIDFromGenerator(Database::TDBTransaction *inDBTransaction)
{
	TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

	qrGenID->SQL->Text = "SELECT GEN_ID(GEN_ITEM, 1) FROM RDB$DATABASE";
	qrGenID->ExecQuery();

	__int32 key = qrGenID->Fields[0]->AsInteger;

    return key;
}

// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// A map to hold newly created course key and its name. This is used later when inserting course print orders back to the database
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TImportMenu::ClearCourseNameKeyMap()
{
    _courseNameKeyMap.clear();
}

void TImportMenu::AddCourseNameKeyMap( UnicodeString inCourseName,__int32 inCourseKey )
{
    _courseNameKeyMap[inCourseName] = inCourseKey;
}

__int32 TImportMenu::GetCourseNameKeyMap( UnicodeString inCourseName )
{
    __int32 result = -1;

    if( _courseNameKeyMap.find(inCourseName) != _courseNameKeyMap.end())
        result = _courseNameKeyMap[inCourseName];

    return result;
}

// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Existing serving course print orders related to a menu. We need to populate
// this structure before deleting the menu so depending on the
// serving course name we can insert records back into the database which fixes
// the issue at case #26135
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TImportMenu::ClearExistingServingCoursePrintOrders()
{
    ExistingServingCoursePrintOrders.clear();
}

// ---------------------------------------------------------------------------

void TImportMenu::PopulateExistingServingCoursePrintOrders( AnsiString menuName )
{
    //Database::TDBTransaction dbTransaction( ibDatabase );

    __int32 menuKey = GetExistingMenuKeyByName( dbTransaction, menuName.Trim() );

    if( menuKey > 0)
    {
        TIBSQL *qr = dbTransaction->Query(dbTransaction->AddQuery());
        qr->SQL->Text = "SELECT a.PRNORD_KEY, a.MENU_KEY, a.COURSE_KEY, a.COURSE_NAME, a.VIRTUALPRINTER_KEY, a.REDIRECT_FROM  "
                        "FROM PRNORDER a WHERE a.MENU_KEY = :menuKey";

        qr->ParamByName("menuKey")->AsInteger = menuKey;
        qr->ExecQuery();

        while(!qr->Eof)
        {
            TServingCoursePrintOrder *PrintOrder = new TServingCoursePrintOrder;
            PrintOrder->PRNOrderKey = qr->FieldByName("PRNORD_KEY")->AsInteger;
            PrintOrder->MenuKey = qr->FieldByName("MENU_KEY")->AsInteger;
            PrintOrder->CourseKey = qr->FieldByName("COURSE_KEY")->AsInteger;
            PrintOrder->CourseName = qr->FieldByName("COURSE_NAME")->AsString;
            PrintOrder->VirtualPrinterKey = qr->FieldByName("VIRTUALPRINTER_KEY")->AsInteger;
            PrintOrder->RedirectFrom = qr->FieldByName("REDIRECT_FROM")->AsInteger;

            ExistingServingCoursePrintOrders.push_back(PrintOrder);

            qr->Next();
        }
    }
}

// ---------------------------------------------------------------------------

void TImportMenu::ReInsertServingCoursePrintOrdersInDB( Database::TDBTransaction *inDBTransaction, __int32 inMenuKey )
{
    std::vector<TServingCoursePrintOrder *>::iterator it = ExistingServingCoursePrintOrders.begin();
    TIBSQL *qr = inDBTransaction->Query( inDBTransaction->AddQuery() );
    qr->SQL->Text = "INSERT INTO PRNORDER (PRNORD_KEY, MENU_KEY, COURSE_KEY, COURSE_NAME, VIRTUALPRINTER_KEY) "           //, REDIRECT_FROM
                    " VALUES (:PRNORD_KEY,:MENU_KEY,:COURSE_KEY,:COURSE_NAME, :VIRTUALPRINTER_KEY ) ";                    //, :REDIRECT_FROM

    __int32 prnOrderKey = 0;

    while(it != ExistingServingCoursePrintOrders.end())
    {
        if( GetCourseNameKeyMap((*it)->CourseName) > 0 )
        {
            prnOrderKey = GetLatestIDFromGenerator(inDBTransaction , "GEN_PRNORDER",1);

            qr->ParamByName("PRNORD_KEY")->AsInteger = prnOrderKey;
            qr->ParamByName("MENU_KEY")->AsInteger = inMenuKey;
            qr->ParamByName("COURSE_KEY")->AsInteger = GetCourseNameKeyMap((*it)->CourseName);
            qr->ParamByName("COURSE_NAME")->AsString = (*it)->CourseName;
            qr->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = (*it)->VirtualPrinterKey;
            //qr->ParamByName("REDIRECT_FROM")->AsInteger = "";

            qr->ExecQuery();
        }

        // delete the pointer from the vector.
        // This will also increment the iterator which helps the while loop to carry on
        ExistingServingCoursePrintOrders.erase(it);
    }
}
// ---------------------------------------------------------------------------

__int32 TImportMenu::GetExistingMenuKeyByName( Database::TDBTransaction *inDBTransaction,
                                               AnsiString menuName )
{
    __int32 result = -1;

    TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "Select Menu_Key FROM Menu WHERE Menu_Name = :menuName";
	qr->ParamByName("menuName")->AsString = menuName.Trim();
	qr->ExecQuery();

    if(!qr->Eof)
        result = qr->Fields[0]->AsInteger;

    return result;
}
// ---------------------------------------------------------------------------
bool TImportMenu::RevenueCodeExists(
                            Database::TDBTransaction *inDBTransaction,
                            __int32 inCode )
{
    bool result = false;

    TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "Select COUNT( REVENUECODE ) "
                    "FROM REVENUECODEDETAILS "
                    "WHERE REVENUECODE = :REVENUECODE;";
	qr->ParamByName("REVENUECODE")->AsString = inCode;
	qr->ExecQuery();

    if(!qr->Eof)
    {
        result = qr->Fields[0]->AsInteger > 0;
    }

    return result;
}
//----------------------------------------------------------------------------

bool TImportMenu::ThirdPartyCodeExists(
                            Database::TDBTransaction *inDBTransaction,
                            WideString inCode )
{
    bool result = false;

    TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "Select COUNT( code ) "
                    "FROM THIRDPARTYCODES "
                    "WHERE code = :code;";
	qr->ParamByName("code")->AsString = inCode;
	qr->ExecQuery();

    if(!qr->Eof)
    {
        result = qr->Fields[0]->AsInteger > 0;
    }

    return result;
}
// ---------------------------------------------------------------------------

__int32 TImportMenu::GetTPCKey(
                        Database::TDBTransaction *inDBTransaction,
                        WideString inCode )
{
    __int32 result = -1;

    TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "Select ThirdPartyCodes_Key "
                    "FROM THIRDPARTYCODES "
                    "WHERE code = :code;";
	qr->ParamByName("code")->AsString = inCode;
	qr->ExecQuery();

    if(!qr->Eof)
    {
        result = qr->Fields[0]->AsInteger;
    }

    return result;
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Removes existing thirdparty codes related to items in the current menu
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TImportMenu::RemoveExistingThirdPartyCodes( AnsiString inMenuName )
{
    __int32 menuKey = GetExistingMenuKeyByName( dbTransaction, inMenuName );

    if(menuKey > 0)
    {
        TIBSQL *qr = dbTransaction->Query(dbTransaction->AddQuery());
        qr->SQL->Text = "DELETE FROM THIRDPARTYCODES WHERE THIRDPARTYCODES_KEY in "
                        " ( SELECT S.THIRDPARTYCODES_KEY "
                        "   FROM "
                        "   COURSE C "
                        "   JOIN Item I on C.COURSE_KEY = I.COURSE_KEY and C.MENU_KEY=:MENU_KEY "
                        "   JOIN ITEMSIZE S on S.ITEM_KEY = I.ITEM_KEY ) " ;

        qr->ParamByName("MENU_KEY")->AsInteger = menuKey;
        qr->ExecQuery();
    }
}
//----------------------------------------------------------------------------

// common function to get the next id from specified generator
__int32 TImportMenu::GetLatestIDFromGenerator(Database::TDBTransaction *inDBTransaction,  AnsiString generator_name,  __int32 incrementor)
{
	TIBSQL *qrGenID    = inDBTransaction->Query( inDBTransaction->AddQuery() );

	qrGenID->SQL->Text = "SELECT GEN_ID( " + generator_name + "," + incrementor + ") FROM RDB$DATABASE";
	qrGenID->ExecQuery();

	__int32 key = qrGenID->Fields[0]->AsInteger;

    return key;
}
//----------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Methods that deals with populating, retrieving currently used item keys from the orders table
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TImportMenu::ClearOrdersItemsKeyMap()
{
    _ordersItemsKeyMap.clear();
}
//----------------------------------------------------------------------------

bool TImportMenu::IsItemKeyAlreadyBeingUsedForOrders( __int32 inItemKey )
{
    return _ordersItemsKeyMap.find(inItemKey) != _ordersItemsKeyMap.end();
}
//----------------------------------------------------------------------------

void TImportMenu::PopulateOrdersItemsKeyMap(
                Database::TDBTransaction *inDBTransaction,
                AnsiString inMenuName)
{
    TIBSQL *qr = inDBTransaction->Query(inDBTransaction->AddQuery());
    qr->SQL->Text = "SELECT DISTINCT(ITEM_ID) FROM ORDERS WHERE MENU_NAME = :MENU_NAME";

    qr->ParamByName("MENU_NAME")->AsString = inMenuName.Trim();
    qr->ExecQuery();

    int id = 0;
    while(!qr->Eof)
    {
        id = qr->Fields[0]->AsInteger;
        _ordersItemsKeyMap[id] = id;

        qr->Next();
    }
}
//----------------------------------------------------------------------------
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Allocates new plu numbers for item sizes
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TImportMenu::ResetMaxItemSizePLU(
            Database::TDBTransaction *inDBTransaction)
{
    _maxItemSizePLU = GetMaxItemSizePLU( inDBTransaction );
}
//----------------------------------------------------------------------------
int TImportMenu::GetMaxItemSizePLU(
            Database::TDBTransaction *inDBTransaction)
{
    int max_plu = 0;

    TIBSQL *qr = inDBTransaction->Query(inDBTransaction->AddQuery());
    qr->SQL->Text = "SELECT MAX(PLU) FROM ITEMSIZE";
    qr->ExecQuery();

    int id = 0;
    if(!qr->Eof)
        max_plu = qr->Fields[0]->AsInteger;

    return max_plu;
}
//----------------------------------------------------------------------------
int TImportMenu::GetNextItemSizePLU()
{
    return ++_maxItemSizePLU;
}
//----------------------------------------------------------------------------
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Methods that deals with populating, retrieving currently used item size keys from the orders table
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TImportMenu::ClearOrdersItemSizesKeyMap()
{
    _ordersItemSizesKeyMap.clear();
}
//----------------------------------------------------------------------------
bool TImportMenu::IsItemSizeKeyAlreadyBeingUsedForOrders(
                                    __int32 parentItemKey,
                                    __int32 inItemSizeKey )
{
    bool isItemKeyBeingUsed = false;        // whether we have the parentItemKey in saved sales
    bool isItemSizeKeyBeingUsed = false;    // whether we have the itemsize key in the ItemSize table

    isItemKeyBeingUsed = IsItemKeyAlreadyBeingUsedForOrders(parentItemKey);

    if(isItemKeyBeingUsed)
    {
        DB_KEY_VECTOR::iterator it = _ordersItemSizesKeyMap[parentItemKey].begin();
        for( ; it != _ordersItemSizesKeyMap[parentItemKey].end(); it++ )
        {
            isItemSizeKeyBeingUsed = inItemSizeKey == *it;
            if(isItemSizeKeyBeingUsed)
                break;
        }
    }

    return isItemKeyBeingUsed && isItemSizeKeyBeingUsed;
}
//----------------------------------------------------------------------------
void TImportMenu::PopulateOrdersItemSizeKeysFromItemKeys(
                        Database::TDBTransaction *inDBTransaction)
{
    std::map<__int32,__int32>::iterator it =  _ordersItemsKeyMap.begin();
    while(it != _ordersItemsKeyMap.end())
    {
       PopulateOrdersItemSizesKeyMap(inDBTransaction, it->first);
        it++;
    }
}
//----------------------------------------------------------------------------
void TImportMenu::PopulateOrdersItemSizesKeyMap(
            Database::TDBTransaction *inDBTransaction,
            int parentItemKey)
{
    TIBSQL *qr = inDBTransaction->Query(inDBTransaction->AddQuery());
    qr->SQL->Text = "SELECT DISTINCT(ITEMSIZE_KEY) FROM ITEMSIZE WHERE ITEM_KEY = :ITEM_KEY";

    qr->ParamByName("ITEM_KEY")->AsInteger = parentItemKey;
    qr->ExecQuery();

    int id = 0;
    while(!qr->Eof)
    {
        id = qr->Fields[0]->AsInteger;
        _ordersItemSizesKeyMap[parentItemKey].push_back(id);

        qr->Next();
    }
}
//----------------------------------------------------------------------------
void TImportMenu:: SaveItemSizePriceLevelsFromMenu( __int32 inItemSizeID, __int32 inItemSizeKey, TLoadMenu* inMenu,
		                                     Database::TDBTransaction *inDBTransaction )
{
        int noOfPriceLevelInDataBase;
        TIBSQL *IBInternalQuery2   = inDBTransaction->Query( inDBTransaction->AddQuery() );
        IBInternalQuery2->Close();
        IBInternalQuery2->SQL->Text =
                                     "SELECT "
                                         "INTEGER_VAL "
                                     "FROM "
                                          "VARIABLES "
                                    "WHERE "
                                          "VARIABLES_KEY = 5014; ";
       IBInternalQuery2->ExecQuery();
       noOfPriceLevelInDataBase= IBInternalQuery2->Fields[0]->AsInteger;

       if(noOfPriceLevelInDataBase==0)noOfPriceLevelInDataBase=2;

    __int32 PriceLevelCount = inMenu->PriceLevelCount( inItemSizeID );

	for( int priceLevel = 0; priceLevel < noOfPriceLevelInDataBase; priceLevel++ )
    {
      if (priceLevel < PriceLevelCount)
      {
         	SaveItemSizePriceLevelFromMenu( priceLevel, inItemSizeID, inItemSizeKey, inMenu, inDBTransaction );

      }else
      {
        	InsertItemSizePriceLevelInDB( inItemSizeKey, (priceLevel+1) ,PricelevelZero , "PriceLevel "+IntToStr(priceLevel+1) , inDBTransaction );
      }
    }

}


void TImportMenu::SaveItemSizePriceLevelFromMenu( __int32 inIndex, __int32 inItemSizeXMLHandle, __int32 inItemSizeKey,
											     TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
   __int32 priceLevelXMLKey;
   __int32  PriceLevelKey;
   Currency Price;
   AnsiString Name;

	//::::::::::::::::::::::::::::::::::::::::

	__int32 priceLevelXMLHandle = inMenu->ItemSizePriceLevelAtIndex(inIndex, inItemSizeXMLHandle, PriceLevelKey,Price,Name );

	//::::::::::::::::::::::::::::::::::::::::


	if( priceLevelXMLHandle > 0 )
	{

		InsertItemSizePriceLevelInDB( inItemSizeKey, PriceLevelKey,Price,Name, inDBTransaction );
        if(PriceLevelKey==1)
        {
         PricelevelZero= Price;
        }
	}

}


void TImportMenu::InsertItemSizePriceLevelInDB(
                         __int32 inItemSizeKey, __int32  PriceLevelKey,Currency Price,AnsiString Name,
                                      Database::TDBTransaction *inDBTransaction  )
{
   int key;
 	try
	{


        TIBSQL *qr1    = inDBTransaction->Query( inDBTransaction->AddQuery() );
		qr1->SQL->Text = "SELECT GEN_ID(GEN_PRICELEVELITEMSIZE_KEY, 1) FROM RDB$DATABASE";
		qr1->ExecQuery();
		key = qr1->Fields[0]->AsInteger;




		TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
        qr->Close();
	    qr->SQL->Text = "INSERT INTO PriceLevelItemSize "
	                                	"( "
	                                    	"   PriceLevelItemSize_Key, "
                                            "   ItemSize_Key, "
                                         	"   PRICELEVEL_KEY, "
                                            "   Price, "
                                            "   IsEnabled "
                                       	") "
	                                	"VALUES "
                                		"( "
                                    		"   :PriceLevelItemSize_Key, "
                                            "   :ItemSize_Key, "
                                    		"   :PRICELEVEL_KEY, "
                                            "   :Price, "
                                            "   :IsEnabled "
                                         ");";

        qr->ParamByName("PriceLevelItemSize_Key")->AsInteger =key;
        qr->ParamByName("ItemSize_Key")->AsInteger = inItemSizeKey;
        qr->ParamByName("PRICELEVEL_KEY")->AsInteger = PriceLevelKey;
        qr->ParamByName("Price")->AsCurrency = Price;
        qr->ParamByName("IsEnabled")->AsString = "T";
        qr->ExecQuery();

       qr->Close();
       qr->SQL->Text = "Update  PRICELEVELS a set a.PRICELEVEL_NAME = :PRICELEVEL_NAME "
                        "where a.PRICELEVEL_KEY = :PRICELEVEL_KEY ";
       qr->ParamByName("PRICELEVEL_KEY")->AsInteger = PriceLevelKey;
       qr->ParamByName("PRICELEVEL_NAME")->AsString = Name.SubString(1,50);
       qr->ExecQuery();

	}
	catch( Exception& exc )
	{
       textLog = "Failed to insert item size price level with key in the DB: "
                 "item size FKey: " + IntToStr( inItemSizeKey ) + " : " + exc.Message;
       TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, textLog );
       AddErrorLog( textLog );

       throw;
	}
}
///----------------------------------------------------
void TImportMenu::SaveAllForcedOptionsFromMenu( TLoadMenu* inMenu, Database::TDBTransaction *inDBTransaction )
{
	std::map<__int32, __int32>::const_iterator imIT;

	for( imIT = _itemMap.begin(); imIT != _itemMap.end(); imIT++ )
	{
		__int32 masterItemKey = imIT->first;
		__int32 itemXMLHandle = imIT->second;

		__int32 fsCount = inMenu->ForcedOptionCount( itemXMLHandle );

		resetNextNewIOO( masterItemKey, inDBTransaction );

		for( int i = 0; i < fsCount; i++ )
		{
            SaveForcedOptionFromMenu( i, itemXMLHandle, masterItemKey, inMenu, inDBTransaction );
		}
	}
}


