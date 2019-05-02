//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBOnlineOrdeing.h"
#include "MMLogging.h"
#include "GlobalSettings.h"
#include "MMMessageBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TSiteMenuInfo TDBOnlineOrdering::GetMenuInfo(Database::TDBTransaction &dbTransaction)
{
    TSiteMenuInfo siteMenuInfo;
    try
    {
        std::list<TMenuConsumableInfo> menuList;
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "SELECT a.MENU_KEY, a.MENU_NAME, a.MENU_TYPE, a.DESCRIPTION, a.PALMABLE  "
                                     "FROM MENU a "
                                     "WHERE a.DELETED = :DELETED AND a.PALMABLE = :PALMABLE ";
        ibInternalQuery->ParamByName("DELETED")->AsString = "F";
        ibInternalQuery->ParamByName("PALMABLE")->AsString = "T";
        ibInternalQuery->ExecQuery();
        while (!ibInternalQuery->Eof)
        {
            TMenuConsumableInfo menuInfo;
            menuInfo.MenuId = ibInternalQuery->FieldByName("MENU_KEY")->AsInteger;
            menuInfo.Name = ibInternalQuery->FieldByName("MENU_NAME")->AsString;
            menuInfo.Description = menuInfo.Name;
            menuInfo.MenuType = ibInternalQuery->FieldByName("MENU_TYPE")->AsInteger;
            menuInfo.IsPalmable = ibInternalQuery->FieldByName("PALMABLE")->AsString == "T" ? true : false;
            menuInfo.SiteCourses = GetCourseInfo(dbTransaction, ibInternalQuery->FieldByName("MENU_KEY")->AsInteger);
            menuList.push_back(menuInfo);
            ibInternalQuery->Next();
        }
        siteMenuInfo.CompanyId = 0;
        siteMenuInfo.SiteId = TGlobalSettings::Instance().SiteID;
        siteMenuInfo.MenuConsumables = menuList;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return siteMenuInfo;
}
//-------------------------------------------------------------------------------
std::list<TCourseInfo> TDBOnlineOrdering::GetCourseInfo(Database::TDBTransaction &dbTransaction, int menuKey)
{
    std::list<TCourseInfo> courseList;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "SELECT c.COURSE_KEY, c.COURSE_ID, c.COURSE_NAME,sc.SERVINGCOURSE_NAME "
                                        "FROM  COURSE c "
                                        "INNER JOIN SERVINGCOURSES sc on c.SERVINGCOURSES_KEY = sc.SERVINGCOURSES_KEY "
                                        "WHERE c.MENU_KEY = :MENU_KEY "
                                        "GROUP BY c.COURSE_KEY, c.COURSE_ID, c.COURSE_NAME,sc.SERVINGCOURSE_NAME ";
        ibInternalQuery->ParamByName("MENU_KEY")->AsInteger = menuKey;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TCourseInfo courseInfo;
            courseInfo.CourseId = ibInternalQuery->FieldByName("COURSE_KEY")->AsInteger;
            courseInfo.Name = ibInternalQuery->FieldByName("COURSE_NAME")->AsString;
            courseInfo.Description = courseInfo.Name;
            courseInfo.ServingCourseName = ibInternalQuery->FieldByName("SERVINGCOURSE_NAME")->AsString;
            courseInfo.ServingCourseDescription = courseInfo.ServingCourseName;
            courseInfo.Items = GetItemInfo(dbTransaction, ibInternalQuery->FieldByName("COURSE_KEY")->AsInteger);
            courseList.push_back(courseInfo);
            ibInternalQuery->Next();
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return courseList;
}
//------------------------------------------------------------------------------
std::list<TSiteItemInfo> TDBOnlineOrdering::GetItemInfo(Database::TDBTransaction &dbTransaction, int courseId)
{
    std::list<TSiteItemInfo> siteItemsList;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "SELECT a.ITEM_KEY, a.ITEM_ID, a.ITEM_NAME, a.ITEM_IDENTIFIER, A.EXCLUSIVELY_AS_SIDE "
                                        "FROM ITEM a "
                                        "WHERE a.ENABLED = :ENABLED AND a.COURSE_KEY = :COURSE_KEY ";
        ibInternalQuery->ParamByName("ENABLED")->AsString = "T";
        ibInternalQuery->ParamByName("COURSE_KEY")->AsInteger = courseId;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TSiteItemInfo itemInfo;
            itemInfo.SiteItemId = 0;
            itemInfo.Name = ibInternalQuery->FieldByName("ITEM_NAME")->AsString;
            itemInfo.Description = itemInfo.Name;
            itemInfo.ItemUniqueId = ibInternalQuery->FieldByName("ITEM_IDENTIFIER")->AsInteger;
            itemInfo.OnlyAsSide = ibInternalQuery->FieldByName("EXCLUSIVELY_AS_SIDE")->AsString == "T" ? true : false;
            itemInfo.CompanyId = 0;
            itemInfo.ItemSizes = GetItemSizeInfo(dbTransaction, ibInternalQuery->FieldByName("ITEM_KEY")->AsInteger);
            itemInfo.SideGroup = GetSideGroupInfo(dbTransaction, ibInternalQuery->FieldByName("ITEM_KEY")->AsInteger);
            siteItemsList.push_back(itemInfo);
            ibInternalQuery->Next();
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return siteItemsList;
}
//------------------------------------------------------------------------------
std::list<TItemSizeInf> TDBOnlineOrdering::GetItemSizeInfo(Database::TDBTransaction &dbTransaction, int itemId)
{
    std::list<TItemSizeInf> ItemSizeInfo;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "SELECT a.ITEMSIZE_KEY, a.ITEM_ID, a.SIZE_NAME, a.SIZE_KITCHEN_NAME, a.PRICE, a.FREE, a.POINTS_PERCENT, "
                                        "a.CAN_BE_PAID_FOR_USING_POINTS, a.ITEMSIZE_IDENTIFIER, a.THIRDPARTYCODES_KEY, a.DEFAULT_PATRON_COUNT, "
                                        "a.PRICE_FOR_POINTS, a.ITEMSIZE_IDENTIFIER "
                                     "FROM ITEMSIZE a "
                                     "WHERE a.ENABLED = :ENABLED AND a.ITEM_KEY = :ITEM_KEY ";
        ibInternalQuery->ParamByName("ENABLED")->AsString = "T";
        ibInternalQuery->ParamByName("ITEM_KEY")->AsInteger = itemId;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TItemSizeInf itemSizeInfo;
            itemSizeInfo.ItemSizeId			= ibInternalQuery->FieldByName("ITEMSIZE_IDENTIFIER")->AsInteger;;
            itemSizeInfo.Name               = ibInternalQuery->FieldByName("SIZE_NAME")->AsString;
            itemSizeInfo.Description        = itemSizeInfo.Name;
            itemSizeInfo.IsWeighted         = false;
            itemSizeInfo.Price              = ibInternalQuery->FieldByName("PRICE")->AsDouble;
            itemSizeInfo.IsFree             = ibInternalQuery->FieldByName("FREE")->AsString == "T" ? true : false;
            itemSizeInfo.PointsPercentage   = ibInternalQuery->FieldByName("POINTS_PERCENT")->AsDouble;
            itemSizeInfo.PointsPrice        = ibInternalQuery->FieldByName("PRICE_FOR_POINTS")->AsDouble;
            itemSizeInfo.CanBePaidUsingPoints = ibInternalQuery->FieldByName("CAN_BE_PAID_FOR_USING_POINTS")->AsString == "T" ? true : false;
            itemSizeInfo.OrderingUniqueId   = ibInternalQuery->FieldByName("ITEMSIZE_IDENTIFIER")->AsInteger;
            itemSizeInfo.ThirdPartyId       = ibInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsInteger;
            itemSizeInfo.DefaultPatronCount = ibInternalQuery->FieldByName("DEFAULT_PATRON_COUNT")->AsInteger;
            itemSizeInfo.ItemSizeTaxProfiles = GetItemSizeTaxProfileInfo(dbTransaction, ibInternalQuery->FieldByName("ITEMSIZE_KEY")->AsInteger);
            ItemSizeInfo.push_back(itemSizeInfo);
            ibInternalQuery->Next();
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return ItemSizeInfo;
}
//------------------------------------------------------------------------------
std::list<TItemSizeTaxProfileInfo> TDBOnlineOrdering::GetItemSizeTaxProfileInfo(Database::TDBTransaction &dbTransaction, int itemSizeKey)
{
    std::list<TItemSizeTaxProfileInfo> ItemSizeTaxProfiles;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "SELECT a.PROFILE_KEY, a.ITEMSIZE_KEY, TP.NAME, TP.RATE, TP.PRIORITY, TP.TYPE "
                                        "FROM TAXPROFILES_ITEMSIZE a "
                                        "INNER JOIN TAXPROFILES TP ON A.PROFILE_KEY = TP.PROFILE_KEY "
                                        "WHERE a.ITEMSIZE_KEY = :ITEMSIZE_KEY ";
        ibInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = itemSizeKey;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TItemSizeTaxProfileInfo itemSizeTaxProfileInfo;
            itemSizeTaxProfileInfo.ItemSizeTaxProfileId = ibInternalQuery->FieldByName("PROFILE_KEY")->AsInteger;
            itemSizeTaxProfileInfo.Name = ibInternalQuery->FieldByName("NAME")->AsString;
            itemSizeTaxProfileInfo.Description = itemSizeTaxProfileInfo.Name;
            itemSizeTaxProfileInfo.Rate = ibInternalQuery->FieldByName("RATE")->AsDouble;
            itemSizeTaxProfileInfo.TaxProfileType = ibInternalQuery->FieldByName("TYPE")->AsInteger;
            itemSizeTaxProfileInfo.Priority = ibInternalQuery->FieldByName("PRIORITY")->AsInteger;
            ItemSizeTaxProfiles.push_back(itemSizeTaxProfileInfo);
            ibInternalQuery->Next();
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return ItemSizeTaxProfiles;
}
//------------------------------------------------------------------------------
std::list<TSideGroupInfo> TDBOnlineOrdering::GetSideGroupInfo(Database::TDBTransaction &dbTransaction, int masterItemKey)
{
    std::list<TSideGroupInfo> sideGroupList;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "SELECT CAST ('Group' || a.GROUP_NO AS VARCHAR(20))GROUP_INFO, a.GROUP_NO, a.ALLOW_SKIP, a.MAX_SELECT "
                                        "FROM ITEMSIDES a "
                                        "WHERE a.MASTER_ITEM_KEY = :MASTER_ITEM_KEY "
                                        "GROUP BY 1,2,3,4 ";
        ibInternalQuery->ParamByName("MASTER_ITEM_KEY")->AsInteger = masterItemKey;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TSideGroupInfo sideGroupInfo;
            sideGroupInfo.SideGroupId = ibInternalQuery->FieldByName("GROUP_NO")->AsInteger;
            sideGroupInfo.Name = ibInternalQuery->FieldByName("GROUP_INFO")->AsString;
            sideGroupInfo.Description = sideGroupInfo.Name;
            sideGroupInfo.MaxSelect = ibInternalQuery->FieldByName("MAX_SELECT")->AsInteger;
            sideGroupInfo.AllowSkip = ibInternalQuery->FieldByName("ALLOW_SKIP")->AsString == "T" ? true : false;
            sideGroupInfo.ItemSides = GetItemSideInfo(dbTransaction, masterItemKey, ibInternalQuery->FieldByName("GROUP_NO")->AsInteger);
            sideGroupList.push_back(sideGroupInfo);
            ibInternalQuery->Next();
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return sideGroupList;
}
//------------------------------------------------------------------------------
std::list<TItemSideInfo> TDBOnlineOrdering::GetItemSideInfo(Database::TDBTransaction &dbTransaction, int masterItemKey, int groupNo)
{
    std::list<TItemSideInfo> itemSideList;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "SELECT a.ITEM_KEY, a.GROUP_NO, a.ALLOW_SKIP, i.ITEM_NAME, i.ITEM_IDENTIFIER, i.EXCLUSIVELY_AS_SIDE "
                                        "FROM ITEMSIDES a "
                                        "INNER JOIN  ITEM i ON a.ITEM_KEY = i.ITEM_KEY "
                                        "WHERE a.MASTER_ITEM_KEY = :MASTER_ITEM_KEY AND i.ENABLED = :ENABLED AND a.GROUP_NO = :GROUP_NO ";
        ibInternalQuery->ParamByName("MASTER_ITEM_KEY")->AsInteger = masterItemKey;
        ibInternalQuery->ParamByName("GROUP_NO")->AsInteger = groupNo;
        ibInternalQuery->ParamByName("ENABLED")->AsString = "T";
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TItemSideInfo itemSideInfo;
            itemSideInfo.SiteItemId = ibInternalQuery->FieldByName("ITEM_KEY")->AsInteger;
            itemSideInfo.SideGroupId = ibInternalQuery->FieldByName("GROUP_NO")->AsInteger;
            itemSideInfo.Name = ibInternalQuery->FieldByName("ITEM_NAME")->AsString;
            itemSideInfo.Description = itemSideInfo.Name;
            itemSideInfo.ItemUniqueId = ibInternalQuery->FieldByName("ITEM_IDENTIFIER")->AsInteger;
            itemSideInfo.OnlyAsSide = ibInternalQuery->FieldByName("EXCLUSIVELY_AS_SIDE")->AsString == "T" ? true : false;
            itemSideInfo.CompanyId = 0;
            itemSideInfo.ItemSizes = GetItemSizeInfo(dbTransaction, ibInternalQuery->FieldByName("ITEM_KEY")->AsInteger);
            itemSideList.push_back(itemSideInfo);
            ibInternalQuery->Next();
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return itemSideList;
}
//------------------------------------------------------------------------------
TSiteTaxSettingsInfo TDBOnlineOrdering::GetTaxSettings(Database::TDBTransaction &dbTransaction)
{
    TSiteTaxSettingsInfo siteTaxSettingsInfo;
    try
    {
        std::list<TTaxSettingsInfo> SiteTaxSettings;
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "SELECT a.VARIABLES_KEY, COALESCE(a.INTEGER_VAL,0) INTEGER_VAL, a.NUMERIC_VAL "
                                        "FROM VARSPROFILE a "
                                        "WHERE a.VARIABLES_KEY IN (8000,8001,8002,8003, 8005,8007) "//8004,for service charge
                                        "ORDER BY a.VARIABLES_KEY ";
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TTaxSettingsInfo taxSettingInfo;
            taxSettingInfo.Value = IntToStr(ibInternalQuery->FieldByName("INTEGER_VAL")->AsInteger);
            switch(ibInternalQuery->FieldByName("VARIABLES_KEY")->AsInteger)
            {
                 case 8000:
                     taxSettingInfo.SettingType = ItemPriceIncludeTax;
                    break;
                 case 8001:
                     taxSettingInfo.SettingType = ItemPriceIncludeServiceCharge;
                    break;
                 case 8002:
                     taxSettingInfo.SettingType = ibInternalQuery->FieldByName("INTEGER_VAL")->AsInteger == 0 ?
                                                        CalculateTaxBeforeDiscount : CalculateTaxAfterDiscount;
                    taxSettingInfo.Value = "1";
                    break;
                 case 8003:
                     taxSettingInfo.SettingType = ibInternalQuery->FieldByName("INTEGER_VAL")->AsInteger == 0 ? CalculateScPreDiscountedPrice
                                                         : ReCalculateScAfterDiscount;
                     taxSettingInfo.Value = "1";
                     break;
                 case 8005:
                     taxSettingInfo.SettingType = ApplyServiceChargeTax;
                    break;
                 case 8007:
                     taxSettingInfo.SettingType = ServiceChargeTaxRate;
                     taxSettingInfo.Value = CurrToStrF(ibInternalQuery->FieldByName("NUMERIC_VAL")->AsCurrency, ffGeneral, 2);
                    break;
                 default:
                    break;
            }
            SiteTaxSettings.push_back(taxSettingInfo);
            ibInternalQuery->Next();
        }
        for(int index = 3; index <= 10; ++index )
        {
            bool indexPresent = false;
            for(std::list<TTaxSettingsInfo>::iterator it = SiteTaxSettings.begin(); it != SiteTaxSettings.end(); it++)
            {
                if(it->SettingType == index)
                {
                    indexPresent = true;
                    break;
                }
            }

            if(!indexPresent)
            {
                TTaxSettingsInfo taxSettingInfo;
                taxSettingInfo.SettingType = index;
                taxSettingInfo.Value = "0";
                SiteTaxSettings.push_back(taxSettingInfo);
            }
        }

        siteTaxSettingsInfo.SiteTaxSettings = SiteTaxSettings;
        siteTaxSettingsInfo.SiteId = TGlobalSettings::Instance().SiteID;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return siteTaxSettingsInfo;
}
//----------------------------------------------------------------------------------------------------
void TDBOnlineOrdering::GetOrdersByOnlineOrderGUID(Database::TDBTransaction &DBTransaction,TList *Orders, UnicodeString orderUniqueId)
{
	if(orderUniqueId.Trim() == "")
	{
		return;
	}

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		/* Exclud the Sides here it will be loaded in as part of the order */
		IBInternalQuery->SQL->Text =
		"SELECT ORDER_KEY "
		"FROM ORDERS "
		"WHERE ORDER_GUID =  :ORDER_GUID "
		        "AND (SIDE_ORDER_KEY IS NULL OR SIDE_ORDER_KEY = 0) "
		"ORDER BY TAB_KEY,ORDER_KEY";

        IBInternalQuery->ParamByName("ORDER_GUID")->AsString = orderUniqueId;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			std::set<__int64> OrderKeys;
			for(; !IBInternalQuery->Eof;	IBInternalQuery->Next())
			{
				OrderKeys.insert(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger);
			}
			TDBOrder::GetOrdersIncludingSidesFromOrderKeys(DBTransaction,Orders,OrderKeys, true);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//----------------------------------------------------------------------------
UnicodeString TDBOnlineOrdering::GetOnlineOrderGUID(Database::TDBTransaction &dbTransaction)
{
    UnicodeString onlineOrderGUID = "";
    try
    {
        std::list<TTaxSettingsInfo> SiteTaxSettings;
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "SELECT  FIRST 1 A.ORDER_GUID "
                                        "FROM ORDERS a "
                                        "WHERE a.IS_DOCKET_PRINTED = :IS_DOCKET_PRINTED "
                                        "ORDER BY a.ORDER_KEY ASC ";
        ibInternalQuery->ParamByName("IS_DOCKET_PRINTED")->AsString = "F";
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
            onlineOrderGUID = ibInternalQuery->FieldByName("ORDER_GUID")->AsString;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return onlineOrderGUID;
}
//----------------------------------------------------------------------------
void TDBOnlineOrdering::SetOnlineOrderStatus(Database::TDBTransaction &dbTransaction, UnicodeString orderUniqueId)
{
    try
    {
        std::list<TTaxSettingsInfo> SiteTaxSettings;
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "UPDATE ORDERS a SET a.IS_DOCKET_PRINTED = :IS_DOCKET_PRINTED "
                                        "WHERE a.ORDER_GUID = :ORDER_GUID ";
        ibInternalQuery->ParamByName("ORDER_GUID")->AsString = orderUniqueId;
        ibInternalQuery->ParamByName("IS_DOCKET_PRINTED")->AsString = "T";
        ibInternalQuery->ExecQuery();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//----------------------------------------------------------------------------
int TDBOnlineOrdering::GetMemberKey(Database::TDBTransaction &dbTransaction, int orderKey)
{
    int memberKey = 0;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = " SELECT a.CONTACTS_KEY FROM ORDERS a "
                                      "WHERE a.ORDER_KEY = :ORDER_KEY ";
        ibInternalQuery->ParamByName("ORDER_KEY")->AsString = orderKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
            memberKey = ibInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return memberKey;
}
//----------------------------------------------------------------------------
int TDBOnlineOrdering::GetItemSizeKey(Database::TDBTransaction &dbTransaction, int itemKey, UnicodeString sizeName)
{
    int itemSizeKey = 0;
    try
    {
        int itemId = GetItemIdByItemKey(dbTransaction, itemKey);
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = " SELECT a.ITEMSIZE_KEY FROM ITEMSIZE a "
                                     " WHERE a.ITEM_KEY = :ITEM_KEY AND a.SIZE_NAME = :SIZE_NAME ";
        ibInternalQuery->ParamByName("ITEM_KEY")->AsInteger = itemId;
        ibInternalQuery->ParamByName("SIZE_NAME")->AsString = sizeName;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
            itemSizeKey = ibInternalQuery->FieldByName("ITEMSIZE_KEY")->AsInteger;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return itemSizeKey;
}
//----------------------------------------------------------------------------
int TDBOnlineOrdering::GetItemIdByItemKey(Database::TDBTransaction &dbTransaction, int itemKey)
{
    int itemId = 0;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = " SELECT a.ITEM_ID FROM ITEM a "
                                     " WHERE a.ITEM_KEY = :ITEM_KEY ";
        ibInternalQuery->ParamByName("ITEM_KEY")->AsInteger = itemKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
            itemId = ibInternalQuery->FieldByName("ITEM_ID")->AsInteger;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return itemId;
}
//----------------------------------------------------------------------------
Currency TDBOnlineOrdering::GetHappyHourPrice(Database::TDBTransaction &dbTransaction, int itemsizeKey, int priceLevelKey)
{
    Currency happyHourPrice = 0.0;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = " SELECT a.PRICE FROM PRICELEVELITEMSIZE a "
                                     " WHERE a.ITEMSIZE_KEY = :ITEMSIZE_KEY AND a.PRICELEVEL_KEY = :PRICELEVEL_KEY ";
        ibInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = itemsizeKey;
        ibInternalQuery->ParamByName("PRICELEVEL_KEY")->AsInteger = priceLevelKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
            happyHourPrice = ibInternalQuery->FieldByName("PRICE")->AsCurrency;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return happyHourPrice;
}
//----------------------------------------------------------------------------
void TDBOnlineOrdering::UpdateHappyHourPriceForItem(Database::TDBTransaction &dbTransaction, int itemKey, UnicodeString sizeName,
                            UnicodeString onlineOrderId, Currency hhPrice, Currency basePrice)
{        
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "UPDATE ORDERS a SET a.PRICE_LEVEL1 = :PRICE_LEVEL1 , a.HAPPYHOUR = :HAPPYHOUR, a.PRICE = :PRICE, "
                                        "a.BASE_PRICE = :BASE_PRICE "
                                     " WHERE a.ITEM_ID = :ITEM_ID AND a.SIZE_NAME = :SIZE_NAME AND a.ORDER_GUID = :ORDER_GUID ";
        ibInternalQuery->ParamByName("ITEM_ID")->AsInteger = itemKey;
        ibInternalQuery->ParamByName("SIZE_NAME")->AsString = sizeName;
        ibInternalQuery->ParamByName("ORDER_GUID")->AsString = onlineOrderId;
        ibInternalQuery->ParamByName("PRICE_LEVEL1")->AsCurrency = hhPrice;
        ibInternalQuery->ParamByName("PRICE")->AsCurrency = hhPrice;
        ibInternalQuery->ParamByName("BASE_PRICE")->AsCurrency = basePrice;
        ibInternalQuery->ParamByName("HAPPYHOUR")->AsString = "T";
        ibInternalQuery->ExecQuery();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
UnicodeString TDBOnlineOrdering::GetOnlineOrderGUIDForWaiterApp(Database::TDBTransaction &dbTransaction, bool IsDayArcBill)
{
    UnicodeString onlineOrderGUIDForWaiterApp = "";
    UnicodeString tableName = "";
    try
    {
        tableName =  IsDayArcBill?" DAYARCBILL a ":" ARCBILL a ";
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text  =   "SELECT  FIRST 1 A.ORDER_GUID FROM ";
        ibInternalQuery->SQL->Text  =   ibInternalQuery->SQL->Text + tableName;
        ibInternalQuery->SQL->Text  =   ibInternalQuery->SQL->Text + "WHERE a.IS_PRINT_REQUIRED = :IS_PRINT_REQUIRED "
                                        "ORDER BY a.ARCBILL_KEY ASC ";

        ibInternalQuery->ParamByName("IS_PRINT_REQUIRED")->AsString = "T";
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
            onlineOrderGUIDForWaiterApp = ibInternalQuery->FieldByName("ORDER_GUID")->AsString;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return onlineOrderGUIDForWaiterApp;
}
//------------------------------------------------------------------------------
void TDBOnlineOrdering::SetStatusAndSaveReceiptForWApp(Database::TDBTransaction &dbTransaction, UnicodeString orderUniqueIdForWaiterApp, TMemoryStream *ReceiptToArchive, bool IsDayArcBill)
{
    UnicodeString tableName = "";
    try
    {
        tableName =  IsDayArcBill?" DAYARCBILL a ":" ARCBILL a ";
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text  =    "UPDATE" + tableName;
        ibInternalQuery->SQL->Text  =    ibInternalQuery->SQL->Text + " SET a.IS_PRINT_REQUIRED = :IS_PRINT_REQUIRED, a.RECEIPT = :RECEIPT "
                                         " WHERE a.ORDER_GUID = :ORDER_GUID ";

        ibInternalQuery->ParamByName("ORDER_GUID")->AsString = orderUniqueIdForWaiterApp;
        ibInternalQuery->ParamByName("IS_PRINT_REQUIRED")->AsString = "F";
        ibInternalQuery->ParamByName("RECEIPT")->LoadFromStream(ReceiptToArchive);
        ibInternalQuery->ExecQuery();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
std::list<TItemInfoKey> TDBOnlineOrdering::GetItemInfoKeyListFromOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID)
{
    std::list<TItemInfoKey> itemInfoKeyList;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "SELECT a.ITEMSIZE_KEY, a.ITEM_KEY, b.ITEMSIZE_IDENTIFIER "
                                        "FROM DAYARCHIVE b "
                                        "INNER JOIN ITEMSIZE a on b.ITEMSIZE_IDENTIFIER = a.ITEMSIZE_IDENTIFIER "
                                        "WHERE b.ORDER_GUID =:ORDER_GUID "
                                        "UNION ALL "
                                        "SELECT a.ITEMSIZE_KEY, a.ITEM_KEY, b.ITEMSIZE_IDENTIFIER "
                                        "FROM ARCHIVE b "
                                        "INNER JOIN ITEMSIZE a on b.ITEMSIZE_IDENTIFIER = a.ITEMSIZE_IDENTIFIER "
                                        "WHERE b.ORDER_GUID =:ORDER_GUID ";

        ibInternalQuery->ParamByName("ORDER_GUID")->AsString = orderGUID;
        ibInternalQuery->ExecQuery();

        for(;!ibInternalQuery->Eof;ibInternalQuery->Next())
        {
            TItemInfoKey itemInfoKey;
            itemInfoKey.itemId                = ibInternalQuery->FieldByName("ITEM_KEY")->AsInteger;
            itemInfoKey.itemSizeKey           = ibInternalQuery->FieldByName("ITEMSIZE_KEY")->AsInteger;
            itemInfoKey.itemSizeIdentifierKey = ibInternalQuery->FieldByName("ITEMSIZE_IDENTIFIER")->AsInteger;
            itemInfoKeyList.push_back(itemInfoKey);
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return itemInfoKeyList;
}
//------------------------------------------------------------------------------
TMemoryStream* TDBOnlineOrdering::GetEFTPOSReceiptForOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID)
{
    TMemoryStream *EFTPOSReceipt = new TMemoryStream;;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "SELECT a.EFTPOS_RECEIPT "
                                        "FROM DAYARCBILL b "
                                        "INNER JOIN ONLINEORDERS a on b.INVOICE_NUMBER = a.INVOICE_NUMBER "
                                        "WHERE b.ORDER_GUID =:ORDER_GUID "
                                        "UNION ALL "
                                        "SELECT a.EFTPOS_RECEIPT "
                                        "FROM ARCBILL b "
                                        "INNER JOIN ONLINEORDERS a on b.INVOICE_NUMBER = a.INVOICE_NUMBER "
                                        "WHERE b.ORDER_GUID =:ORDER_GUID ";

        ibInternalQuery->ParamByName("ORDER_GUID")->AsString = orderGUID;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            EFTPOSReceipt->Clear();
            ibInternalQuery->FieldByName("EFTPOS_RECEIPT")->SaveToStream(EFTPOSReceipt);
            EFTPOSReceipt->Position = 0;
        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return EFTPOSReceipt;
}
//------------------------------------------------------------------------------
std::list<TPaymentInfo> TDBOnlineOrdering::GetPaymentInfoForOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID)
{
    std::list<TPaymentInfo> paymentInfoList;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    "SELECT a.PAY_TYPE, a.SUBTOTAL "
                                        "FROM DAYARCBILL b "
                                        "INNER JOIN DAYARCBILLPAY a on b.ARCBILL_KEY = a.ARCBILL_KEY "
                                        "WHERE b.ORDER_GUID =:ORDER_GUID AND a.NOTE !=:NOTE "
                                        "UNION ALL "
                                        "SELECT a.PAY_TYPE, a.SUBTOTAL "
                                        "FROM ARCBILL b "
                                        "INNER JOIN ARCBILLPAY a on b.ARCBILL_KEY = a.ARCBILL_KEY "
                                        "WHERE b.ORDER_GUID =:ORDER_GUID AND a.NOTE !=:NOTE ";

        ibInternalQuery->ParamByName("ORDER_GUID")->AsString = orderGUID;
        ibInternalQuery->ParamByName("NOTE")->AsString = "Total Change.";
        ibInternalQuery->ExecQuery();

        for(;!ibInternalQuery->Eof;ibInternalQuery->Next())
        {
            if(ibInternalQuery->FieldByName("PAY_TYPE")->AsString.Trim() != "")
            {
                TPaymentInfo paymentInfo;
                paymentInfo.payType = ibInternalQuery->FieldByName("PAY_TYPE")->AsString;
                paymentInfo.amount  = ibInternalQuery->FieldByName("SUBTOTAL")->AsDouble;
                paymentInfoList.push_back(paymentInfo);
            }
        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return paymentInfoList;
}
//------------------------------------------------------------------------------
void TDBOnlineOrdering::LoadItemCompleteInfoForOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID,
                            TItemComplete *itemComplete, int itemSizeIdentifier)
{
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =    " SELECT "
                                        " a.TABLE_NUMBER, "
                                        " a.SEAT_NUMBER, "
                                        " a.TIME_STAMP, "
                                        " a.GST_PERCENT, "
                                        " a.PRICE_LEVEL0, "
                                        " a.PRICE_LEVEL1, "
                                        " a.COST, "
                                        " a.COST_GST_PERCENT, "
                                        " a.NOTE, "
                                        " a.QTY, "
                                        " a.PLU, "
                                        " a.TIME_KEY "
                                        " FROM DAYARCHIVE a "
                                        " WHERE a.ORDER_GUID =:ORDER_GUID AND a.ITEMSIZE_IDENTIFIER =:ITEMSIZE_IDENTIFIER "
                                        " UNION ALL"
                                        " SELECT "
                                        " a.TABLE_NUMBER, "
                                        " a.SEAT_NUMBER, "
                                        " a.TIME_STAMP, "
                                        " a.GST_PERCENT, "
                                        " a.PRICE_LEVEL0, "
                                        " a.PRICE_LEVEL1, "
                                        " a.COST, "
                                        " a.COST_GST_PERCENT, "
                                        " a.NOTE, "
                                        " a.QTY, "
                                        " a.PLU, "
                                        " a.TIME_KEY "
                                        " FROM ARCHIVE a "
                                        " WHERE a.ORDER_GUID =:ORDER_GUID AND a.ITEMSIZE_IDENTIFIER =:ITEMSIZE_IDENTIFIER ";

        ibInternalQuery->ParamByName("ORDER_GUID")->AsString = orderGUID;
        ibInternalQuery->ParamByName("ITEMSIZE_IDENTIFIER")->AsInteger = itemSizeIdentifier;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            itemComplete->TableNo        = ibInternalQuery->FieldByName("TABLE_NUMBER")->AsInteger;
            itemComplete->SeatNo         = ibInternalQuery->FieldByName("SEAT_NUMBER")->AsInteger;
            itemComplete->TimeStamp      = ibInternalQuery->FieldByName("TIME_STAMP")->AsDate;
            itemComplete->GSTPercent     = ibInternalQuery->FieldByName("GST_PERCENT")->AsDouble;
            itemComplete->PriceLevel0    = ibInternalQuery->FieldByName("PRICE_LEVEL0")->AsDouble;
            itemComplete->PriceLevel1    = ibInternalQuery->FieldByName("PRICE_LEVEL1")->AsDouble;
            itemComplete->Cost           = ibInternalQuery->FieldByName("COST")->AsDouble;
            itemComplete->CostGSTPercent = ibInternalQuery->FieldByName("COST_GST_PERCENT")->AsDouble;
            itemComplete->Note           = ibInternalQuery->FieldByName("NOTE")->AsString;
            itemComplete->PLU            = ibInternalQuery->FieldByName("PLU")->AsInteger;
            itemComplete->ItemKey        = ibInternalQuery->FieldByName("TIME_KEY")->AsInteger;
            itemComplete->SetQty(ibInternalQuery->FieldByName("QTY")->AsInteger);
        }

    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
    }
}
//------------------------------------------------------------------------------
UnicodeString TDBOnlineOrdering::GetTerminalNameForOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID)
{
    UnicodeString terminalName = "";
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =   " SELECT "
                                       " a.TERMINAL_NAME "
                                       " FROM DAYARCBILL a "
                                       " WHERE a.ORDER_GUID =:ORDER_GUID "
                                       " UNION ALL "
                                       " SELECT "
                                       " a.TERMINAL_NAME "
                                       " FROM ARCBILL a "
                                       " WHERE a.ORDER_GUID =:ORDER_GUID ";

        ibInternalQuery->ParamByName("ORDER_GUID")->AsString = orderGUID;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            if(ibInternalQuery->FieldByName("TERMINAL_NAME")->AsString.Trim() != "")
                terminalName = ibInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
    }
    return terminalName;
}
//------------------------------------------------------------------------------
UnicodeString TDBOnlineOrdering::GetInvoiceNumberForOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID)
{
    UnicodeString invoiceNumber = "";
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =   " SELECT "
                                       " a.INVOICE_NUMBER "
                                       " FROM DAYARCBILL a "
                                       " WHERE a.ORDER_GUID =:ORDER_GUID "
                                       " UNION ALL "
                                       " SELECT "
                                       " a.INVOICE_NUMBER "
                                       " FROM ARCBILL a "
                                       " WHERE a.ORDER_GUID =:ORDER_GUID ";

        ibInternalQuery->ParamByName("ORDER_GUID")->AsString = orderGUID;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            if(ibInternalQuery->FieldByName("INVOICE_NUMBER")->AsString.Trim() != "")
                invoiceNumber = ibInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
    }
    return invoiceNumber;
}

