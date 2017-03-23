//---------------------------------------------------------------------------


#pragma hdrstop

#include "DiscountGroupsDBDriver.h"
#include "DeviceRealTerminal.h"
#include "GeneratorManager.h"
//---------------------------------------------------------------------------

TDiscountGroupsDBDriver::TDiscountGroupsDBDriver()
{
    DBControl = &TDeviceRealTerminal::Instance().DBControl;
}
//---------------------------------------------------------------------------

bool TDiscountGroupsDBDriver::AddDiscountGroups(TDiscountGroup* group)
{
    bool status = false;
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        status = _addDiscountGroups(transaction, group);
        transaction.Commit();
    }
    catch(Exception &ex)
    {
    }
    return status;
};
//---------------------------------------------------------------------------

bool TDiscountGroupsDBDriver::UpdateDiscountGroups(TDiscountGroup* group)
{
    bool status = false;
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        status = _updateDiscountGroups(transaction, group);
        transaction.Commit();
    }
    catch(Exception &ex)
    {
    }
    return status;
};
//---------------------------------------------------------------------------

bool TDiscountGroupsDBDriver::DeleteDiscountGroups(int groupKey)
{
    bool status = false;
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        status = _deleteDiscountGroups(transaction, groupKey);
        transaction.Commit();
    }
    catch(Exception &ex)
    {
    }
    return status;
};
//---------------------------------------------------------------------------

void TDiscountGroupsDBDriver::LoadAssignedGroupsByType(int discountTypeKey, TList* assignedGroups)
{
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _loadAssignedGroupsByType(transaction, discountTypeKey, assignedGroups);
        transaction.Commit();
    }
    catch(Exception &ex)
    {
    }
};
//---------------------------------------------------------------------------

void TDiscountGroupsDBDriver::LoadAllDiscountGroups(TList* groups)
{
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _loadAllDiscountGroups(transaction, groups);
        transaction.Commit();
    }
    catch(Exception &ex)
    {

    }
};
//---------------------------------------------------------------------------

//bool TDiscountGroupsDBDriver::AssignGroupsToDiscountType(int discountTypeKey, std::vector<int> groupKeys)
//{;
//    bool status = false;
//    try
//    {
//        Database::TDBTransaction transaction(*DBControl);
//        transaction.StartTransaction();
//        _removeAllAssignedGroupsFromType(transaction, discountTypeKey);
//        std::vector<int>::iterator it = groupKeys.begin();
//
//        while(it != groupKeys.end())
//        {
//            status = _assignDiscountTypeToGroups(transaction, discountTypeKey, *it);
//
//            if(!status)
//                throw("failed to add group to discount type");
//
//            it++;
//        }
//
//        transaction.Commit();
//    }
//    catch(Exception &ex)
//    {
//        //todo: log exception
//        #ifdef _DEBUG
//            throw;
//        #endif
//    }
//    return status;
//};
//---------------------------------------------------------------------------

bool TDiscountGroupsDBDriver::AssignTypesToDiscountGroups(int discountGroupKey, std::vector<int> typeKeys)
{
    bool status = false;
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _removeAllAssignedTypesFromGroups(transaction, discountGroupKey);
//      _removeAllAssignedGroupsFromTypes(transaction, discountTypeKey);
        std::vector<int>::iterator it = typeKeys.begin();

        while(it != typeKeys.end())
        {
            status = _assignDiscountTypeToGroups(transaction, *it, discountGroupKey);

            if(!status)
                throw("failed to add type to discount group");

            it++;
        }
        transaction.Commit();
    }
    catch(Exception &ex)
    {

    }
    return status;
};
//---------------------------------------------------------------------------

bool TDiscountGroupsDBDriver::_addDiscountGroups(Database::TDBTransaction &transaction, TDiscountGroup* group)
{
    TIBSQL* insertQuery = transaction.Query(transaction.AddQuery());
    int groupKey = _getNextDiscountGroupsKey(transaction);
    insertQuery->SQL->Text = "INSERT INTO DISCOUNT_GROUPS(DISCOUNTGROUPS_KEY,DISCOUNTGROUP_NAME,DEFAULT_GROUP,DISCOUNTGROUP_TYPE,TAX_EXEMPT) "
                             "values(:GROUP_KEY,:NAME,:IS_DEFAULT,:DISCOUNTGROUP_TYPE,:TAX_EXEMPT)";
    insertQuery->ParamByName("GROUP_KEY")->AsInteger = groupKey;
    insertQuery->ParamByName("NAME")->AsString = group->Name;
    insertQuery->ParamByName("IS_DEFAULT")->AsString = 'F';  // default groups will only be created by parser
    insertQuery->ParamByName("DISCOUNTGROUP_TYPE")->AsInteger = 0;
    insertQuery->ParamByName("TAX_EXEMPT")->AsString = 'F';
    insertQuery->ExecQuery();

    return insertQuery->RowsAffected > 0;
};
//---------------------------------------------------------------------------

bool TDiscountGroupsDBDriver::_updateDiscountGroups(Database::TDBTransaction &transaction, TDiscountGroup* group)
{
    TIBSQL* updateQuery = transaction.Query(transaction.AddQuery());
    updateQuery->SQL->Text = "UPDATE DISCOUNT_GROUPS SET DISCOUNTGROUP_NAME=:NAME WHERE DISCOUNTGROUPS_KEY=:GROUP_KEY";
    updateQuery->ParamByName("NAME")->AsString = group->Name;
    updateQuery->ParamByName("GROUP_KEY")->AsInteger = group->DBKey;
    updateQuery->ExecQuery();

    return updateQuery->RowsAffected > 0;
};
//---------------------------------------------------------------------------

bool TDiscountGroupsDBDriver::_deleteDiscountGroups(Database::TDBTransaction &transaction, int groupKey)
{
    TIBSQL* deleteQuery = transaction.Query(transaction.AddQuery());
    deleteQuery->SQL->Text = "DELETE FROM DISCOUNT_GROUPS WHERE DISCOUNTGROUPS_KEY=:GROUP_KEY";
    deleteQuery->ParamByName("GROUP_KEY")->AsInteger = groupKey;
    deleteQuery->ExecQuery();

    return deleteQuery->RowsAffected > 0;
};
//---------------------------------------------------------------------------

void TDiscountGroupsDBDriver::_loadAllDiscountGroups(Database::TDBTransaction &transaction, TList* groups)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =  "SELECT DG.DISCOUNTGROUPS_KEY, DG.DISCOUNTGROUP_NAME, DG.DEFAULT_GROUP, DG.DISCOUNTGROUP_TYPE, DG.TAX_EXEMPT "
                        "FROM DISCOUNT_GROUPS DG ";
    query->ExecQuery();

    while(!query->Eof)
    {
        TDiscountGroup* group = new TDiscountGroup(query->FieldByName("DISCOUNTGROUPS_KEY")->AsInteger);
        _loadDiscountGroupsFromQuery(query, group);
        groups->Add(group);
        query->Next();
    }
};
//---------------------------------------------------------------------------

void TDiscountGroupsDBDriver::_removeAllAssignedGroupsFromType(Database::TDBTransaction &transaction, int discountTypeKey)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "DELETE FROM DISCOUNTGROUPS_DISCOUNTTYPES WHERE DISCOUNT_KEY=:DISCOUNTTYPE_KEY";
    query->ParamByName("DISCOUNTTYPE_KEY")->AsInteger = discountTypeKey;
    query->ExecQuery();
};
//---------------------------------------------------------------------------

void TDiscountGroupsDBDriver::_removeAllAssignedTypesFromGroups(Database::TDBTransaction &transaction, int discountGroupKey)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "DELETE FROM DISCOUNTGROUPS_DISCOUNTTYPES WHERE DISCOUNTGROUPS_KEY=:DISCOUNTGROUP_KEY";
    query->ParamByName("DISCOUNTGROUP_KEY")->AsInteger = discountGroupKey;
    query->ExecQuery();
};
//---------------------------------------------------------------------------

bool TDiscountGroupsDBDriver::_assignDiscountTypeToGroups(Database::TDBTransaction &transaction, int discountTypeKey, int groupKey)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "INSERT INTO DISCOUNTGROUPS_DISCOUNTTYPES(DGDT_KEY, DISCOUNTGROUPS_KEY, DISCOUNTTYPE_KEY) VALUES(:DGDT_KEY, :DISCOUNTGROUPS_KEY, :DISCOUNTTYPE_KEY)";
    query->ParamByName("DGDT_KEY")->AsInteger = _getNextDiscountTypeDiscountGroupsRelationKey(transaction);
    query->ParamByName("DISCOUNTGROUPS_KEY")->AsInteger = groupKey;
    query->ParamByName("DISCOUNTTYPE_KEY")->AsInteger = discountTypeKey;
    query->ExecQuery();
};
//---------------------------------------------------------------------------

int TDiscountGroupsDBDriver::_getNextDiscountGroupsKey(Database::TDBTransaction &transaction)
{
    return TGeneratorManager::GetNextGeneratorKey(transaction,"GEN_DISCOUNT_GROUPS");
};
//---------------------------------------------------------------------------

int TDiscountGroupsDBDriver::_getNextDiscountTypeDiscountGroupsRelationKey(Database::TDBTransaction &transaction)
{
    return TGeneratorManager::GetNextGeneratorKey(transaction,"GEN_DISCGROUPS_DISCTYPES");
};
//---------------------------------------------------------------------------

void TDiscountGroupsDBDriver::_loadDiscountGroupsFromQuery(TIBSQL* query, TDiscountGroup* group)
{
    group->Name = query->FieldByName("DISCOUNTGROUP_NAME")->AsString;
    group->DiscountGroupType =  query->FieldByName("DISCOUNTGROUP_TYPE")->AsInteger;
    group->TaxExempt = query->FieldByName("TAX_EXEMPT")->AsString == "T" ? true : false;
    group->IsDefault = query->FieldByName("DEFAULT_GROUP")->AsString == "T" ? true : false;
};
//---------------------------------------------------------------------------

void TDiscountGroupsDBDriver::LoadAssignedTypesByGroups(int discountGroupKey,TList* assignedTypes)
{
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _loadAssignedTypesByGroup(transaction, discountGroupKey, assignedTypes);
        transaction.Commit();
    }
    catch(Exception &ex)
    {

    }
};
//---------------------------------------------------------------------------

void TDiscountGroupsDBDriver::_loadAssignedTypesByGroup(Database::TDBTransaction &transaction,int discountGroupKey,TList* assignedTypes)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =  "SELECT DT.DISCOUNT_KEY, DT.NAME "
                        "FROM DISCOUNTGROUPS_DISCOUNTTYPES DGTR JOIN DISCOUNTS DT "
                        "ON DGTR.DISCOUNTTYPE_KEY = DT.DISCOUNT_KEY "
                        "WHERE DGTR.DISCOUNTGROUPS_KEY = :DISCOUNTGROUP_KEY";
    query->ParamByName("DISCOUNTGROUP_KEY")->AsInteger = discountGroupKey;
    query->ExecQuery();

    while(!query->Eof)
    {
        TDiscountTypes* type = new TDiscountTypes(query->FieldByName("DISCOUNT_KEY")->AsInteger);
        _loadDiscountTypeFromQuery(query, type);
        assignedTypes->Add(type);
        query->Next();
    }
};
//---------------------------------------------------------------------------

void TDiscountGroupsDBDriver::_loadAssignedGroupsByType(Database::TDBTransaction &transaction, int discountTypeKey,
                                                        TList* assignedGroups)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =  "SELECT DG.DISCOUNTGROUPS_KEY, DG.DISCOUNTGROUP_NAME, DG.DEFAULT_GROUP, DG.DISCOUNTGROUP_TYPE, DG.TAX_EXEMPT "
                        "FROM DISCOUNTGROUPS_DISCOUNTTYPES DGTR JOIN DISCOUNT_GROUPS DG "
                        "ON DGTR.DISCOUNTGROUPS_KEY = DG.DISCOUNTGROUPS_KEY "
                        "WHERE DGTR.DISCOUNTTYPE_KEY = :DISCOUNTTYPE_KEY";
    query->ParamByName("DISCOUNTTYPE_KEY")->AsInteger = discountTypeKey;
    query->ExecQuery();

    while(!query->Eof)
    {
        TDiscountGroup* group = new TDiscountGroup(query->FieldByName("DISCOUNTGROUPS_KEY")->AsInteger);
        _loadDiscountGroupsFromQuery(query, group);
        assignedGroups->Add(group);
        query->Next();
    }
};
//---------------------------------------------------------------------------

void TDiscountGroupsDBDriver::_loadDiscountTypeFromQuery(TIBSQL* query, TDiscountTypes* type)
{
    type->Name = query->FieldByName("NAME")->AsString;
//    group->IsDefault = query->FieldByName("DEFAULT_GROUP")->AsString == "T" ? true : false;
};
//---------------------------------------------------------------------------

#pragma package(smart_init)
