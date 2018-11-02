//---------------------------------------------------------------------------
#pragma hdrstop
#include "MewsDataProcessor.h"
#include "DeviceRealTerminal.h"
#include "GeneratorManager.h"
#pragma package(smart_init)
//--------------------------------------------------------------------------
TMewsDataProcessor::TMewsDataProcessor()
{
}
TMewsDataProcessor::~TMewsDataProcessor()
{
}
void TMewsDataProcessor::GetRevenueCodesDetails(TStringGrid * StringGrid,Database::TDBTransaction &DBTransaction)
{
    try
    {
       std::vector<TAccountingCategoriesMapping> accountingCategoriesMapping;
       accountingCategoriesMapping.clear();
       accountingCategoriesMapping = GetMewsCategoriesList(DBTransaction);
       if(accountingCategoriesMapping.size() < 2)
       {
           StringGrid->RowCount = 2;
       }
       else
       {
            StringGrid->RowCount = accountingCategoriesMapping.size() + 1;
       }
        std::vector<TAccountingCategoriesMapping>::iterator it =
                                        accountingCategoriesMapping.begin();
        int i = 0;
        int Index = 0;
        for(; it != accountingCategoriesMapping.end(); ++it)
        {
            Index = StringGrid->Cols[0]->Add(it->MMCategoryName);
            StringGrid->Cols[0]->Objects[Index] = (TObject*)it->CategoryMapId;
            Index = StringGrid->Cols[1]->Add(it->MewsCategoryCode);
            StringGrid->Cols[1]->Objects[Index] = (TObject*)it->CategoryMapId;
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
}
std::vector<TAccountingCategory> TMewsDataProcessor::GetCategoriesFromDB(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    std::vector<TAccountingCategory> categories;
    categories.clear();
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM REVENUECODEDETAILS";
        IBInternalQuery->ExecQuery();
        for( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           TAccountingCategory category;
           if(IBInternalQuery->FieldByName("ISACTIVE")->AsString == "T")
           {
               category.Code = IBInternalQuery->FieldByName("REVENUECODE_DESCRIPTION")->AsString;
               category.Id = IBInternalQuery->FieldByName("UNIQUEID")->AsString;
               categories.push_back(category);
           }
        }
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return categories;
}
std::vector<TAccountingCategoriesMapping> TMewsDataProcessor::GetMewsCategoriesList(Database::TDBTransaction &DBTransaction)
{
    std::vector<TAccountingCategoriesMapping> categoriesMMMews;
    categoriesMMMews.clear();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM PMSACCOUNTINGCATEGORIES WHERE REVENUE_CENTRE = :REVENUE_CENTRE";
        IBInternalQuery->ParamByName("REVENUE_CENTRE")->AsString =  TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        IBInternalQuery->ExecQuery();
        for( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TAccountingCategoriesMapping categoryMapping;
            categoryMapping.CategoryMapId = IBInternalQuery->FieldByName("CATEGORYMAPID")->AsInteger;
            categoryMapping.MMCategoryName = IBInternalQuery->FieldByName("MM_CATEGORYNAME")->AsString;
            categoryMapping.CategoryKey = IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
            categoryMapping.MewsCategoryId = IBInternalQuery->FieldByName("PMSACCOUNTINGID")->AsString;
            categoryMapping.MewsCategoryCode = IBInternalQuery->FieldByName("MEWS_CATEGORYNAME")->AsString;
            categoriesMMMews.push_back(categoryMapping);
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }
    return categoriesMMMews;
}
void TMewsDataProcessor::UpdateOutlets(std::vector<TOutlet> outlets,Database::TDBTransaction &DBTransaction)
{
    if(outlets.size() == 0 || outlets.size() < 0)
        return;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->ParamCheck = true;
    IBInternalQuery->SQL->Text =  "DELETE FROM OUTLETS";
    IBInternalQuery->ExecQuery();
    std::vector<TOutlet> ::iterator it = outlets.begin();
    for(;it != outlets.end();advance(it,1))
    {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
          "INSERT INTO OUTLETS "
          " ( UNIQUEID, NAME, ISACTIVE) "
          " VALUES "
          " (:UNIQUEID, :NAME, :ISACTIVE)";
      IBInternalQuery->ParamByName("UNIQUEID")->AsString = it->Id;
      IBInternalQuery->ParamByName("NAME")->AsString = it->Name.Trim();
      if(it->IsActive)
          IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
      else
          IBInternalQuery->ParamByName("ISACTIVE")->AsString = "F";
      IBInternalQuery->ExecQuery();
    }
}
void TMewsDataProcessor::UpdateServices(std::vector<TServiceMews> services,Database::TDBTransaction &DBTransaction)
{
    if(services.size() == 0 || services.size() < 0)
        return;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->ParamCheck = true;
    IBInternalQuery->SQL->Text =  "DELETE FROM SERVICES";
    IBInternalQuery->ExecQuery();
    std::vector<TServiceMews> ::iterator it = services.begin();
    for(;it != services.end();advance(it,1))
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "INSERT INTO SERVICES "
          " ( UNIQUEID, NAME, ISACTIVE, STARTTIME, ENDTIME) "
          " VALUES "
          " (:UNIQUEID, :NAME, :ISACTIVE, :STARTTIME, :ENDTIME)";
        IBInternalQuery->ParamByName("UNIQUEID")->AsString = it->Id;
        IBInternalQuery->ParamByName("NAME")->AsString = it->Name.Trim();
        if(it->IsActive)
          IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
        else
          IBInternalQuery->ParamByName("ISACTIVE")->AsString = "F";

        // to be changed to actual values
        IBInternalQuery->ParamByName("STARTTIME")->AsDateTime = Now();
        IBInternalQuery->ParamByName("ENDTIME")->AsString = Now();

        IBInternalQuery->ExecQuery();
    }
}
void TMewsDataProcessor::UpdateSpaces(TSpaceDetails spaces,Database::TDBTransaction &DBTransaction)
{
    if(spaces.Spaces.size() == 0 || spaces.Spaces.size() < 0)
        return;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->ParamCheck = true;
    IBInternalQuery->SQL->Text =  "DELETE FROM SPACES";
    IBInternalQuery->ExecQuery();
    std::vector<TSpace> ::iterator it = spaces.Spaces.begin();
    for(;it != spaces.Spaces.end();advance(it,1))
    {

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
          "INSERT INTO SPACES "
          " ( UNIQUEID, ISACTIVE, PARENTSPACEID, CATEGORYID, TYPE, NUMBER, FLOORNUMBER, BUILDINGNUMBER, STATE) "
          " VALUES "
          " (:UNIQUEID, :ISACTIVE, :PARENTSPACEID, :CATEGORYID, :TYPE, :NUMBER, :FLOORNUMBER, :BUILDINGNUMBER, :STATE)";
      IBInternalQuery->ParamByName("UNIQUEID")->AsString = it->Id;

      if(it->IsActive)
          IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
      else
          IBInternalQuery->ParamByName("ISACTIVE")->AsString = "F";

      // to be changed to actual values
      IBInternalQuery->ParamByName("PARENTSPACEID")->AsString = it->ParentSpaceId;
      IBInternalQuery->ParamByName("CATEGORYID")->AsString = it->CategoryId.Trim();
      IBInternalQuery->ParamByName("TYPE")->AsString = it->Type.Trim();
      IBInternalQuery->ParamByName("NUMBER")->AsString = it->Number.Trim();
      IBInternalQuery->ParamByName("FLOORNUMBER")->AsString = it->FloorNumber.Trim();
      IBInternalQuery->ParamByName("BUILDINGNUMBER")->AsString = it->BuildingNumber.Trim();
      IBInternalQuery->ParamByName("STATE")->AsString = it->State.Trim();

      IBInternalQuery->ExecQuery();
    }
}
void TMewsDataProcessor::UpdateCategories(std::vector<TAccountingCategory> categories,Database::TDBTransaction &DBTransaction)
{
    if(categories.size() == 0 || categories.size() < 0)
        return;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->ParamCheck = true;
    IBInternalQuery->SQL->Text =  "DELETE FROM REVENUECODEDETAILS";
    IBInternalQuery->ExecQuery();
    std::vector<TAccountingCategory> ::iterator it = categories.begin();
    int i = 1;
    for(;it != categories.end();advance(it,1))
    {
        if(it->IsActive)
        {
          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text =
              "INSERT INTO REVENUECODEDETAILS "
              " ( REVENUECODE, REVENUECODE_DESCRIPTION, ISDEFAULT_REVENUECODE, UNIQUEID, ISACTIVE, CODE, "
              " EXTERNALCODE, LEDGERACCOUNTCODE, POSTINGACCOUNTCODE, COSTCENTRECODE, CLASSIFICATION) "
              " VALUES "
              " ( :REVENUECODE, :REVENUECODE_DESCRIPTION, :ISDEFAULT_REVENUECODE, :UNIQUEID, :ISACTIVE, "
              " :CODE, :EXTERNALCODE, :LEDGERACCOUNTCODE, :POSTINGACCOUNTCODE, :COSTCENTRECODE, :CLASSIFICATION)";
          IBInternalQuery->ParamByName("UNIQUEID")->AsString = it->Id;

          IBInternalQuery->ParamByName("REVENUECODE")->AsInteger = i;
          IBInternalQuery->ParamByName("REVENUECODE_DESCRIPTION")->AsString = it->Name.Trim();
          IBInternalQuery->ParamByName("ISDEFAULT_REVENUECODE")->AsString = "F";
          IBInternalQuery->ParamByName("UNIQUEID")->AsString = it->Id;
          if(it->IsActive)
              IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
          else
              IBInternalQuery->ParamByName("ISACTIVE")->AsString = "F";
          IBInternalQuery->ParamByName("CODE")->AsString = it->Code;
          IBInternalQuery->ParamByName("EXTERNALCODE")->AsString = it->ExternalCode.Trim();
          IBInternalQuery->ParamByName("LEDGERACCOUNTCODE")->AsString = it->LedgerAccountCode.Trim();
          IBInternalQuery->ParamByName("POSTINGACCOUNTCODE")->AsString = it->PostingAccountCode.Trim();
          IBInternalQuery->ParamByName("CLASSIFICATION")->AsString = it->Classification.Trim();

          IBInternalQuery->ExecQuery();
          i++;
        }
    }
}
void TMewsDataProcessor::InitializeMewsCategories(Database::TDBTransaction &DBTransaction)
{
    std::map<int,UnicodeString> freshMenumateCategories;
    freshMenumateCategories.clear();
    freshMenumateCategories = GetFreshMenumateCategories(DBTransaction);
    if(freshMenumateCategories.size() > 0)
    {
        try
        {
            TIBSQL *InsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
            std::map<int,UnicodeString> :: iterator itMap = freshMenumateCategories.begin();
            for(;itMap !=  freshMenumateCategories.end(); advance(itMap,1))
            {
                InsertQuery->Close();
                InsertQuery->SQL->Text =
                           " INSERT INTO  PMSACCOUNTINGCATEGORIES ( CATEGORYMAPID, REVENUE_CENTRE, CATEGORY_KEY, MM_CATEGORYNAME, PMSACCOUNTINGID, MEWS_CATEGORYNAME )"
                           " VALUES ( :CATEGORYMAPID, :REVENUE_CENTRE, :CATEGORY_KEY, :MM_CATEGORYNAME, :PMSACCOUNTINGID, :MEWS_CATEGORYNAME )";
                InsertQuery->ParamByName("CATEGORYMAPID")->AsInteger = TGeneratorManager::GetNextGeneratorKey(DBTransaction,"GEN_PMSACCOUNTINGCATEGORIESID");
                InsertQuery->ParamByName("REVENUE_CENTRE")->AsString = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
                InsertQuery->ParamByName("CATEGORY_KEY")->AsInteger = itMap->first;
                InsertQuery->ParamByName("MM_CATEGORYNAME")->AsString = itMap->second;
                InsertQuery->ParamByName("PMSACCOUNTINGID")->AsString = "";
                InsertQuery->ParamByName("MEWS_CATEGORYNAME")->AsString = "";
                InsertQuery->ExecQuery();
            }
            DBTransaction.Commit();
            DBTransaction.StartTransaction();
        }
        catch(Exception &ex)
        {
            DBTransaction.Rollback();
            DBTransaction.StartTransaction();
            TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        }
    }
}
void TMewsDataProcessor::UpdateCategories(int categoryKey, UnicodeString name, bool isDeleted,
                                            Database::TDBTransaction &DBTransaction)
{
    try
    {
        TIBSQL *QueryInternal = DBTransaction.Query(DBTransaction.AddQuery());
        QueryInternal->Close();
        if(isDeleted)
        {
            QueryInternal->SQL->Text =
                       "DELETE FROM PMSACCOUNTINGCATEGORIES WHERE CATEGORY_KEY = :CATEGORY_KEY";
            QueryInternal->ParamByName("CATEGORY_KEY")->AsInteger = categoryKey;
            QueryInternal->ExecQuery();
        }
        else
        {
            QueryInternal->SQL->Text =
                       "UPDATE PMSACCOUNTINGCATEGORIES SET MM_CATEGORYNAME = :MM_CATEGORYNAME "
                       "WHERE CATEGORY_KEY = :CATEGORY_KEY";
            QueryInternal->ParamByName("CATEGORY_KEY")->AsInteger = categoryKey;
            QueryInternal->ParamByName("MM_CATEGORYNAME")->AsString = name;
            QueryInternal->ExecQuery();
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }
}
std::map<int,UnicodeString> TMewsDataProcessor::GetFreshMenumateCategories(Database::TDBTransaction &DBTransaction)
{
    std::map<int,UnicodeString> missingMenumateCategories;
    missingMenumateCategories.clear();
    try
    {
        TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text =
                   "SELECT CATEGORY_KEY,CATEGORY FROM ARCCATEGORIES WHERE CATEGORY_KEY NOT IN "
                   " ( SELECT CATEGORY_KEY FROM PMSACCOUNTINGCATEGORIES WHERE REVENUE_CENTRE = :REVENUE_CENTRE)"
                   " AND VISIBLE = 'T'";
        SelectQuery->ParamByName("REVENUE_CENTRE")->AsString = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        SelectQuery->ExecQuery();
        for( ; !SelectQuery->Eof; SelectQuery->Next())
        {
            UnicodeString mmCategory;
            mmCategory = SelectQuery->FieldByName("CATEGORY")->AsString;
            missingMenumateCategories[SelectQuery->FieldByName("CATEGORY_KEY")->AsInteger] =
                                                           mmCategory;
        }
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return missingMenumateCategories;
}
void TMewsDataProcessor::UpdateMewsMapToMMCategory(int key,UnicodeString code,UnicodeString id,Database::TDBTransaction &DBTransaction)
{
    try
    {
        TIBSQL *Query= DBTransaction.Query(DBTransaction.AddQuery());
        Query->Close();
        Query->SQL->Text = "UPDATE PMSACCOUNTINGCATEGORIES SET "
                           "PMSACCOUNTINGID = :PMSACCOUNTINGID,"
                           "MEWS_CATEGORYNAME = :MEWS_CATEGORYNAME "
                           "WHERE CATEGORYMAPID = :CATEGORYMAPID";
        Query->ParamByName("MEWS_CATEGORYNAME")->AsString = code;
        Query->ParamByName("PMSACCOUNTINGID")->AsString = id;
        Query->ParamByName("CATEGORYMAPID")->AsInteger = key;
        Query->ExecQuery();
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }
}
AnsiString TMewsDataProcessor::GetMewsName(Database::TDBTransaction &DBTransaction, UnicodeString code, int identifier)
{
    AnsiString name = "";
    AnsiString tableName = "";
    if(identifier == 1)
        tableName = "OUTLETS";
    else if(identifier == 2)
        tableName = "SERVICES";
    else if(identifier == 3)
        tableName = "REVENUECODEDETAILS";
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM " + tableName + " WHERE UNIQUEID = :UNIQUEID";
        IBInternalQuery->ParamByName("UNIQUEID")->AsString =  code;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount > 0)
        {
            if(identifier == 3)
                name = IBInternalQuery->FieldByName("REVENUECODE_DESCRIPTION")->AsString;
            else
                name = IBInternalQuery->FieldByName("NAME")->AsString;
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
        MessageBox(ex.Message,"Exception in GetMewsName",MB_OK);
    }
    return name;
}
std::map<AnsiString,AnsiString> TMewsDataProcessor::GetAllMewsDetailsFromDB(Database::TDBTransaction &DBTransaction,int identifier)
{
    AnsiString tableName = "";
    if(identifier == 1)
        tableName = "OUTLETS";
    else if(identifier == 2)
        tableName = "SERVICES";
    else if(identifier == 3)
        tableName = "REVENUECODEDETAILS";
    std::map<AnsiString,AnsiString> mapDetails;
    mapDetails.clear();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM " + tableName;
        IBInternalQuery->ExecQuery();
        for( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            AnsiString code = "";
            AnsiString name = "";
            if(identifier == 3)
                name = IBInternalQuery->FieldByName("REVENUECODE_DESCRIPTION")->AsString;
            else
                name = IBInternalQuery->FieldByName("NAME")->AsString;
            code = IBInternalQuery->FieldByName("UNIQUEID")->AsString;
            mapDetails[code] = name;
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
        MessageBox(ex.Message,"Exception in GetAllMewsDetailsFromDB",MB_OK);
    }
    return mapDetails;
}
TCustomerSearch TMewsDataProcessor::CreateInquiryForRoom(UnicodeString queryString,bool isSpace)
{
    TCustomerSearch customerSearch;
    if(isSpace)
        customerSearch.SpaceId = queryString;
    else
        customerSearch.Name = queryString;
    return customerSearch;
}
