//---------------------------------------------------------------------------


#pragma hdrstop

#include "MewsDataProcessor.h"
#include "DeviceRealTerminal.h"
#include <SysUtils.hpp>
#include "GeneratorManager.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TMewsDataProcessor::TMewsDataProcessor()
{
}
//---------------------------------------------------------------------------
TMewsDataProcessor::~TMewsDataProcessor()
{
}
//---------------------------------------------------------------------------
void TMewsDataProcessor::UpdateOutlets(std::vector<TOutlet> outlets)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
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
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception in OUTLETS",MB_OK);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TMewsDataProcessor::UpdateServices(std::vector<TServiceMews> services)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
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
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception in SERVICES",MB_OK);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TMewsDataProcessor::UpdateSpaces(TSpaceDetails spaces)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
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
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception in SPACES",MB_OK);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TMewsDataProcessor::UpdateCategories(std::vector<TAccountingCategory> categories)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
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
              IBInternalQuery->ParamByName("REVENUECODE_DESCRIPTION")->AsString = it->Code.Trim();
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
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception in UpdateCategories",MB_OK);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
std::vector<TOutlet> TMewsDataProcessor::GetOutlets()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    std::vector<TOutlet> outlets;
    outlets.clear();
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM OUTLETS";
        IBInternalQuery->ExecQuery();
        //if(IBInternalQuery->RecordCount)
        for( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           TOutlet outlet;
           outlet.Id = IBInternalQuery->FieldByName("UNIQUEID")->AsString;
           if(IBInternalQuery->FieldByName("ISACTIVE")->AsString == "T")
               outlet.IsActive = true;
           else
               outlet.IsActive = false;
           outlet.Name = IBInternalQuery->FieldByName("NAME")->AsString;
           if(outlet.IsActive)
               outlets.push_back(outlet);
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
    }
    return outlets;
}
//---------------------------------------------------------------------------
std::vector<TServiceMews> TMewsDataProcessor::GetServices()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    std::vector<TServiceMews> services;
    services.clear();
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM SERVICES";
        IBInternalQuery->ExecQuery();
        //if(IBInternalQuery->RecordCount)

        for( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           TServiceMews service;
           service.Id = IBInternalQuery->FieldByName("UNIQUEID")->AsString;
           if(IBInternalQuery->FieldByName("ISACTIVE")->AsString == "T")
               service.IsActive = true;
           else
               service.IsActive = false;
           service.Name = IBInternalQuery->FieldByName("NAME")->AsString;
           if(service.IsActive)
               services.push_back(service);
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
    }
    return services;
}
//--------------------------------------------------------------------------
std::vector<TAccountingCategory> TMewsDataProcessor::GetCategoriesFromDB()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    std::vector<TAccountingCategory> categories;
    categories.clear();
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM REVENUECODEDETAILS";
        IBInternalQuery->ExecQuery();
        //if(IBInternalQuery->RecordCount)
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
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
    }
    return categories;
}
//---------------------------------------------------------------------------
TCustomerSearch TMewsDataProcessor::CreateInquiryForRoomBySpace(UnicodeString queryString)
{
    TCustomerSearch customerSearch;
    customerSearch.SpaceId = queryString;
    return customerSearch;
}
//---------------------------------------------------------------------------
TCustomerSearch TMewsDataProcessor::CreateInquiryForRoomByName(UnicodeString queryString)
{
    TCustomerSearch customerSearch;
    customerSearch.Name = queryString;
    return customerSearch;
}
//---------------------------------------------------------------------------
std::vector<TItemMews> TMewsDataProcessor::GetMewsOrder(TPaymentTransaction &paymentTransaction, double portion, int paymentIndex, double tipPortion, std::map<int,TAccountingCategory> accountingMap)
{
    std::vector<TItemMews> itemsList;
    itemsList.clear();
    try
    {
        for(int i = 0; i < paymentTransaction.Orders->Count; i++)
        {
            TItemComplete* itemComplete = ((TItemComplete*)paymentTransaction.Orders->Items[i]);
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = itemComplete->Item;
            itemMews.UnitCount = RoundTo((double)itemComplete->GetQty() * portion, -2);
            itemMews.UnitCost = GetUnitCost(itemComplete, portion);
            itemMews.Category.Code = "";
            for(std::map<int,TAccountingCategory>::iterator itAcc = accountingMap.begin();
                itAcc!= accountingMap.end(); advance(itAcc,1))
            {
                if(itAcc->first == itemComplete->RevenueCode)
                {
                   itemMews.Category.Code = itAcc->second.Code;
                   break;
                }
            }
            itemsList.push_back(itemMews);
            // add service charge as an item if present
//            if(itemComplete->BillCalcResult.ServiceCharge.Value != 0)
//            {
//                TItemMews itemMews;
//                itemMews.Type = "Revenue";
//                itemMews.Name = "Service Charge";
//                itemMews.UnitCount = 1;
//                itemMews.UnitCost.Amount = itemComplete->BillCalcResult.ServiceCharge.Value;
//                itemMews.UnitCost.Currency = CurrencyString;
//                if(TGlobalSettings::Instance().ApplyServiceChargeTax)
//                    itemMews.UnitCost.Tax = TGlobalSettings::Instance().ServiceChargeTaxRate;
//                else
//                    itemMews.UnitCost.Tax = 0;
//                itemMews.Category.Code = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
//                // get code for items
//                itemsList.push_back(itemMews);
//            }
        }
        if(paymentTransaction.Money.ServiceCharge != 0)
        {
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = "Service Charge";
            itemMews.UnitCount = 1;
            itemMews.UnitCost.Amount = paymentTransaction.Money.ServiceCharge;
            itemMews.UnitCost.Currency = CurrencyString;
            if(TGlobalSettings::Instance().ApplyServiceChargeTax)
                itemMews.UnitCost.Tax = TGlobalSettings::Instance().ServiceChargeTaxRate;
            else
                itemMews.UnitCost.Tax = 0;
            itemMews.Category.Code = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
            // get code for items
            itemsList.push_back(itemMews);
        }
        if(tipPortion != 0)
        {
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = "Tips";
            itemMews.UnitCount = 1;
            itemMews.UnitCost.Amount = tipPortion;
            itemMews.UnitCost.Currency = CurrencyString;
            itemMews.UnitCost.Tax = 0;
            itemMews.Category.Code = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
            // get code for items
            itemsList.push_back(itemMews);
        }
        TPayment *payment = paymentTransaction.PaymentGet(paymentIndex);
        double surcharge = 0;
        if(paymentTransaction.CreditTransaction)
            surcharge = payment->GetDiscount();
        else
            surcharge = payment->GetSurcharge();
        if(surcharge != 0)
        {
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = "Surcharge";
            itemMews.UnitCount = 1;
            itemMews.UnitCost.Amount = surcharge;
            itemMews.UnitCost.Currency = CurrencyString;
            itemMews.UnitCost.Tax = 0;
            itemMews.Category.Code = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
            // get code for items
            itemsList.push_back(itemMews);
        }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return itemsList;
}
//---------------------------------------------------------------------------
std::vector<TBill> TMewsDataProcessor::GetMewsBill(TPaymentTransaction &paymentTransaction, double portion, int index, double tipPortion, std::map<int,TAccountingCategory> accountingMap)
{
    std::vector<TBill> billList;
    billList.clear();
    try
    {
//        order.Items.clear();
//        for(int i = 0; i < paymentTransaction.Orders->Count; i++)
//        {
//            TItemComplete* itemComplete = ((TItemComplete*)paymentTransaction.Orders->Items[i]);
//            TItemMews itemMews;
//            itemMews.Type = "Revenue";
//            itemMews.Name = "";
//            itemMews.UnitCount = itemComplete->GetQty();
//            itemMews.UnitCost = GetUnitCost(itemComplete);
//            itemMews.Category.Code = "";// get code for items
//            order.Items.push_back(itemMews);
//        }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return billList;
}
//----------------------------------------------------------------------------
TUnitCost TMewsDataProcessor::GetUnitCost(TItemComplete* itemComplete, double portion)
{
    TUnitCost unitCost;
    try
    {
        unitCost.Amount = RoundTo((double)itemComplete->BillCalcResult.FinalPrice * portion, -2);
        unitCost.Amount -= RoundTo((double)itemComplete->BillCalcResult.ServiceCharge.Value,-2);
        unitCost.Amount = fabs(unitCost.Amount);
        unitCost.Currency = CurrencyString;
        unitCost.Tax = 0;
        for(int i = 0; i < itemComplete->BillCalcResult.Tax.size();i++)
        {
//            if(itemComplete->BillCalcResult.Tax[i].TaxType == TTaxType.ttSale)
            unitCost.Tax += fabs((double)itemComplete->BillCalcResult.Tax[i].Percentage);
        }
        unitCost.Tax = unitCost.Tax/100;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return unitCost;
}
//----------------------------------------------------------------------------
std::vector<TAccountingCategoriesMapping> TMewsDataProcessor::GetMewsCategoriesList()
{
    std::vector<TAccountingCategoriesMapping> categoriesMMMews;
    categoriesMMMews.clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM PMSACCOUNTINGCATEGORIES";
        IBInternalQuery->ExecQuery();
        //if(IBInternalQuery->RecordCount)
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
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        MessageBox("Exception in GetMewsCategoriesMap()","",MB_OK);
        DBTransaction.Rollback();
    }
    return categoriesMMMews;
}
//----------------------------------------------------------------------------
std::map<int,TAccountingCategory> TMewsDataProcessor::GetMewsCategoriesMap()
{
    std::map<int,TAccountingCategory> categoriesMMMews;
    categoriesMMMews.clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM REVENUECODEDETAILS";
        IBInternalQuery->ExecQuery();
        //if(IBInternalQuery->RecordCount)
        for( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TAccountingCategory accCategory;
            accCategory.Code = IBInternalQuery->FieldByName("REVENUECODE_DESCRIPTION")->AsString;
            categoriesMMMews[IBInternalQuery->FieldByName("REVENUECODE")->AsInteger] =
                                                           accCategory;
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        MessageBox("Exception in GetMewsCategoriesMap()","",MB_OK);
        DBTransaction.Rollback();
    }
    return categoriesMMMews;
}
//----------------------------------------------------------------------------
void TMewsDataProcessor::InitializeMewsCategories()
{
    std::map<int,UnicodeString> freshMenumateCategories;
    freshMenumateCategories.clear();
    freshMenumateCategories = GetFreshMenumateCategories();
    if(freshMenumateCategories.size() > 0)
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
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
                InsertQuery->ParamByName("REVENUE_CENTRE")->AsString = TDeviceRealTerminal::Instance().BasePMS->SelectedMewsOutlet;
                InsertQuery->ParamByName("CATEGORY_KEY")->AsInteger = itMap->first;
                InsertQuery->ParamByName("MM_CATEGORYNAME")->AsString = itMap->second;
                InsertQuery->ParamByName("PMSACCOUNTINGID")->AsString = "";
                InsertQuery->ParamByName("MEWS_CATEGORYNAME")->AsString = "";
                InsertQuery->ExecQuery();
            }
            DBTransaction.Commit();
        }
        catch(Exception &ex)
        {
            DBTransaction.Rollback();
            MessageBox("Exception in InitializeMewsCategories",ex.Message,MB_OK);
        }
    }
}
//---------------------------------------------------------------------------
std::map<int,UnicodeString> TMewsDataProcessor::GetFreshMenumateCategories()
{
    std::map<int,UnicodeString> missingMenumateCategories;
    missingMenumateCategories.clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text =
                   "SELECT CATEGORY_KEY,CATEGORY FROM ARCCATEGORIES WHERE CATEGORY_KEY NOT IN "
                   " ( SELECT CATEGORY_KEY FROM PMSACCOUNTINGCATEGORIES WHERE REVENUE_CENTRE = :REVENUE_CENTRE)";
        SelectQuery->ParamByName("REVENUE_CENTRE")->AsString = TDeviceRealTerminal::Instance().BasePMS->SelectedMewsOutlet;
        SelectQuery->ExecQuery();
        for( ; !SelectQuery->Eof; SelectQuery->Next())
        {
            UnicodeString mmCategory;
            mmCategory = SelectQuery->FieldByName("CATEGORY")->AsString;
            missingMenumateCategories[SelectQuery->FieldByName("CATEGORY_KEY")->AsInteger] =
                                                           mmCategory;
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
        MessageBox("Exception in GetFreshMenumateCategories",ex.Message,MB_OK);
    }
    return missingMenumateCategories;
}
//----------------------------------------------------------------------------
void TMewsDataProcessor::UpdateCategories(int categoryKey, UnicodeString name, bool isDeleted)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *QueryInternal = DBTransaction.Query(DBTransaction.AddQuery());
        QueryInternal->Close();
        if(isDeleted)
        {
            QueryInternal->SQL->Text =
                       "DELETE FROM ARCCATEGORIES WHERE CATEGORY_KEY = :CATEGORY_KEY";
            QueryInternal->ParamByName("CATEGORY_KEY")->AsInteger = categoryKey;
            QueryInternal->ExecQuery();
        }
        else
        {
            QueryInternal->SQL->Text =
                       "UPDATE ARCCATEGORIES SET CATEGORY = :CATEGORY WHERE CATEGORY_KEY = :CATEGORY_KEY";
            QueryInternal->ParamByName("CATEGORY_KEY")->AsInteger = categoryKey;
            QueryInternal->ParamByName("CATEGORY")->AsString = name;
            QueryInternal->ExecQuery();
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
        MessageBox("Exception in UpdateCategories",ex.Message,MB_OK);
    }
}
//----------------------------------------------------------------------------
