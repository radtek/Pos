//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBOnlineOrdeing.h"
#include "MMLogging.h"
#include "GlobalSettings.h"

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
            menuInfo.Description = ibInternalQuery->FieldByName("DESCRIPTION")->AsString;
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
                                        "WHERE a.ENABLED = :ENABLED ";
        ibInternalQuery->ParamByName("ENABLED")->AsString = "T";
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
std::list<TItemSizeInfo> TDBOnlineOrdering::GetItemSizeInfo(Database::TDBTransaction &dbTransaction, int itemId)
{
    std::list<TItemSizeInfo> ItemSizeInfo;
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
            TItemSizeInfo itemSizeInfo;
            itemSizeInfo.ItemSizeId			= ibInternalQuery->FieldByName("ITEMSIZE_IDENTIFIER")->AsInteger;;
            itemSizeInfo.Name               = ibInternalQuery->FieldByName("SIZE_NAME")->AsString;
            itemSizeInfo.Description        = itemSizeInfo.Name;
            itemSizeInfo.IsWeighted         = false;
            itemSizeInfo.Price              = ibInternalQuery->FieldByName("PRICE")->AsInteger;
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
        ibInternalQuery->SQL->Text =    "SELECT a.GROUP_NO, a.ALLOW_SKIP, a.MAX_SELECT "
                                        "FROM ITEMSIDES a "
                                        "WHERE a.MASTER_ITEM_KEY = :MASTER_ITEM_KEY "
                                        "GROUP BY 1,2,3 ";
        ibInternalQuery->ParamByName("MASTER_ITEM_KEY")->AsInteger = masterItemKey;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TSideGroupInfo sideGroupInfo;
            sideGroupInfo.SideGroupId = ibInternalQuery->FieldByName("GROUP_NO")->AsInteger;
            sideGroupInfo.Name = "Group " + ibInternalQuery->FieldByName("GROUP_NO")->AsInteger;
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
       // ibInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = menuKey;
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
                     taxSettingInfo.SettingType = ibInternalQuery->FieldByName("INTEGER_VAL")->AsInteger == 0 ? CalculateTaxBeforeDiscount
                                                        : CalculateTaxAfterDiscount;
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
                     taxSettingInfo.Value = CurrToStrF(ibInternalQuery->FieldByName("NUMERIC_VAL")->AsCurrency, ffCurrency, 2);
                    break;
                 default:
                    break;
            }
            SiteTaxSettings.push_back(taxSettingInfo);
            ibInternalQuery->Next();
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
