//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportSalesTypeDBDriver.h"
#include "MallExportSalesType.h"
#include "GeneratorManager.h"
//---------------------------------------------------------------------------

void TMallExportSalesTypeDBDriver::LoadAllSalesType(TList* types)
{

    Database::TDBTransaction transaction(TDeviceRealTerminal::Instance().DBControl);
    transaction.StartTransaction();
    _loadAllSalesType(transaction, types);
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeDBDriver::_loadAllSalesType(Database::TDBTransaction &transaction, TList* types)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "SELECT ITEM_KEY, ITEM_NAME FROM ITEM";
    query->ExecQuery();

    while(!query->Eof)
    {
        TMallExportSalesType* type = new TMallExportSalesType(
                                         query->FieldByName("ITEM_KEY")->AsInteger,
                                         query->FieldByName("ITEM_NAME")->AsString);
        types->Add(type);
        query->Next();
    }
};
//---------------------------------------------------------------------------

TMallExportSalesTypeGroupDBDriver::TMallExportSalesTypeGroupDBDriver()
{
    DBControl = &TDeviceRealTerminal::Instance().DBControl;
}
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupDBDriver::LoadAllSalesTypeGroup(TList* groups)
{
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _loadAllSalesTypeGroup(transaction, groups);
        transaction.Commit();
    }
    catch(Exception &ex)
    {
        //todo: log exception
        #ifdef _DEBUG
            throw;
        #endif
    }
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupDBDriver::LoadAssignedItemsByGroup(int salestypeGroupKey, TList* assignedTypes)
{
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _loadAssignedItemsByGroup(transaction, salestypeGroupKey, assignedTypes);
        transaction.Commit();
    }
    catch(Exception &ex)
    {
        //todo: log exception
        #ifdef _DEBUG
            throw;
        #endif
    }
};
//---------------------------------------------------------------------------

bool TMallExportSalesTypeGroupDBDriver::AssignItemsToSalesTypeGroup(int salestypeGroupKey, std::vector<int> typeKeys)
{
    bool status = false;
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _removeAllAssignedItemsFromGroup(transaction, salestypeGroupKey);
        std::vector<int>::iterator it = typeKeys.begin();

        while(it != typeKeys.end())
        {
            status = _assignItemsToSalesTypeGroup(transaction, *it, salestypeGroupKey);

            if(!status)
                throw("failed to add type to discount group");

            it++;
        }
        transaction.Commit();
    }
    catch(Exception &ex)
    {
        //todo: log exception
        #ifdef _DEBUG
            throw;
        #endif
    }
    return status;
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupDBDriver::LoadAssignedSalesTypeGroupByItem(int salesTypeKey, TList* assignedGroups)
{
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _loadAssignedSalesTypeGroupByItem(transaction, salesTypeKey, assignedGroups);
        transaction.Commit();
    }
    catch(Exception &ex)
    {
        //todo: log exception
        #ifdef _DEBUG
            throw;
        #endif
    }
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupDBDriver::_loadAllSalesTypeGroup(Database::TDBTransaction &transaction, TList* groups)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =  "SELECT ST.SALES_TYPE_KEY, ST.SALES_TYPE_NAME, ST.DEFAULT_SALES_TYPE "
                        "FROM SALES_TYPE ST ";
    query->ExecQuery();

    while(!query->Eof)
    {
        TMallExportSalesTypeGroup* group = new TMallExportSalesTypeGroup(query->FieldByName("SALES_TYPE_KEY")->AsInteger);
        _loadSalesTypeGroupFromQuery(query, group);
        groups->Add(group);
        query->Next();
    }
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupDBDriver::_loadAssignedItemsByGroup(Database::TDBTransaction &transaction, int salestypeGroupKey, TList* assignedTypes)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =  "SELECT I.ITEM_KEY, I.ITEM_NAME "
                        "FROM SALES_TYPE_ITEMS_RELATION STIR JOIN ITEM I "
                        "ON STIR.ITEM_KEY = I.ITEM_KEY "
                        "WHERE STIR.SALES_TYPE_KEY = :SALES_TYPE_KEY";
    query->ParamByName("SALES_TYPE_KEY")->AsInteger = salestypeGroupKey;
    query->ExecQuery();

    while(!query->Eof)
    {
        TMallExportSalesType* type = new TMallExportSalesType(query->FieldByName("ITEM_KEY")->AsInteger);
        _loadSalesTypeFromQuery(query, type);
        assignedTypes->Add(type);
        query->Next();
    }
};
//---------------------------------------------------------------------------

bool TMallExportSalesTypeGroupDBDriver::_assignItemsToSalesTypeGroup(Database::TDBTransaction &transaction, int salesTypeKey, int groupKey)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "INSERT INTO SALES_TYPE_ITEMS_RELATION(STI_KEY, SALES_TYPE_KEY, ITEM_KEY) VALUES(:STI_KEY, :SALES_TYPE_KEY, :ITEM_KEY)";
    query->ParamByName("STI_KEY")->AsInteger = _getNextSalesTypeItemsRelationKey(transaction);
    query->ParamByName("SALES_TYPE_KEY")->AsInteger = groupKey;
    query->ParamByName("ITEM_KEY")->AsInteger = salesTypeKey;
    query->ExecQuery();
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupDBDriver::_loadAssignedSalesTypeGroupByItem(Database::TDBTransaction &transaction, int salesTypeKey, TList* assignedGroups)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =  "SELECT ST.SALES_TYPE_KEY, ST.SALES_TYPE_NAME, ST.DEFAULT_SALES_TYPE "
                        "FROM SALES_TYPE_ITEMS_RELATION STIR JOIN SALES_TYPE ST "
                        "ON STIR.SALES_TYPE_KEY = ST.SALES_TYPE_KEY "
                        "WHERE STIR.ITEM_KEY = :ITEM_KEY";
    query->ParamByName("ITEM_KEY")->AsInteger = salesTypeKey;
    query->ExecQuery();

    while(!query->Eof)
    {
        TMallExportSalesTypeGroup* group = new TMallExportSalesTypeGroup(query->FieldByName("SALES_TYPE_KEY")->AsInteger);
        _loadSalesTypeGroupFromQuery(query, group);
        assignedGroups->Add(group);
        query->Next();
    }
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupDBDriver::_removeAllAssignedSalesTypeGroupFromItem(Database::TDBTransaction &transaction, int salesTypeKey)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "DELETE FROM SALES_TYPE_ITEMS_RELATION WHERE ITEM_KEY=:ITEM_KEY";
    query->ParamByName("ITEM_KEY")->AsInteger = salesTypeKey;
    query->ExecQuery();
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupDBDriver::_removeAllAssignedItemsFromGroup(Database::TDBTransaction &transaction, int salestypeGroupKey)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "DELETE FROM SALES_TYPE_ITEMS_RELATION WHERE SALES_TYPE_KEY=:SALES_TYPE_KEY";
    query->ParamByName("SALES_TYPE_KEY")->AsInteger = salestypeGroupKey;
    query->ExecQuery();
};
//---------------------------------------------------------------------------

int TMallExportSalesTypeGroupDBDriver::_getNextSalesTypeItemsRelationKey(Database::TDBTransaction &transaction)
{
    return TGeneratorManager::GetNextGeneratorKey(transaction,"GEN_SALES_TYPE_ITEMS_RELATION");
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupDBDriver::_loadSalesTypeGroupFromQuery(TIBSQL* query, TMallExportSalesTypeGroup* group)
{
    group->Name = query->FieldByName("SALES_TYPE_NAME")->AsString;
    group->IsDefault = query->FieldByName("DEFAULT_SALES_TYPE")->AsString == "T" ? true : false;
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupDBDriver::_loadSalesTypeFromQuery(TIBSQL* query, TMallExportSalesType* type)
{
    type->Name = query->FieldByName("ITEM_NAME")->AsString;
};
//---------------------------------------------------------------------------
#pragma package(smart_init)
