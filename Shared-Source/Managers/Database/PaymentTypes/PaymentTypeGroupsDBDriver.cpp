//---------------------------------------------------------------------------


#pragma hdrstop

#include "PaymentTypeGroupsDBDriver.h"
#include "DeviceRealTerminal.h"
#include "GeneratorManager.h"

//---------------------------------------------------------------------------
TPaymentTypeGroupsDBDriver::TPaymentTypeGroupsDBDriver()
{
    DBControl = &TDeviceRealTerminal::Instance().DBControl;
}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsDBDriver::AddPaymentTypeGroup(TPaymentTypeGroup* group)
{
    bool status = false;
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        status = _addPaymentTypeGroup(transaction, group);
        transaction.Commit();
    }
    catch(Exception &ex)
    {
    }
    return status;
}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsDBDriver::UpdatePaymentTypeGroup(TPaymentTypeGroup* group)
{
    bool status = false;
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        status = _updatePaymentTypeGroup(transaction, group);
        transaction.Commit();
    }
    catch(Exception &ex)
    {

    }
    return status;
}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsDBDriver::DeletePaymentTypeGroup(int groupKey)
{
    bool status = false;
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        status = _deletePaymentTypeGroup(transaction, groupKey);
        transaction.Commit();
    }
    catch(Exception &ex)
    {

    }
    return status;
}
//---------------------------------------------------------------------------
void TPaymentTypeGroupsDBDriver::LoadAssignedGroupsByType(
                                                    int paymentTypeKey,
                                                    std::vector<TPaymentTypeGroup> &assignedGroups)
{
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _loadAssignedGroupsByType(transaction, paymentTypeKey, assignedGroups);
        transaction.Commit();
    }
    catch(Exception &ex)
    {

    }
}

//---------------------------------------------------------------------------
void TPaymentTypeGroupsDBDriver::LoadAllPaymentTypeGroups(TList* groups)
{
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _loadAllPaymentTypeGroups(transaction, groups);
        transaction.Commit();
    }
    catch(Exception &ex)
    {

    }
}
//---------------------------------------------------------------------------
//bool TPaymentTypeGroupsDBDriver::AssignGroupsToPaymentType(
//                                                    int paymentTypeKey,
//                                                    std::vector<int> groupKeys)
//{
//    bool status = false;
//    try
//    {
//        Database::TDBTransaction transaction(*DBControl);
//        transaction.StartTransaction();
//        _removeAllAssignedGroupsFromType(transaction, paymentTypeKey);
//        std::vector<int>::iterator it = groupKeys.begin();
//
//        while(it != groupKeys.end())
//        {
//            status = _assignPaymentTypeToGroup(transaction, paymentTypeKey, *it);
//
//            if(!status)
//                throw("failed to add group to payment type");
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
//}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsDBDriver::AssignTypesToPaymentGroup(
                                                    int paymentGroupKey,
                                                    std::vector<int> typeKeys)
{
    bool status = false;
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _removeAllAssignedTypesFromGroup(transaction, paymentGroupKey);
//      _removeAllAssignedGroupsFromType(transaction, paymentTypeKey);
        std::vector<int>::iterator it = typeKeys.begin();

        while(it != typeKeys.end())
        {
            status = _assignPaymentTypeToGroup(transaction, *it, paymentGroupKey);

            if(!status)
                throw("failed to add type to payment group");

            it++;
        }

        transaction.Commit();
    }
    catch(Exception &ex)
    {
    }
    return status;
}
//---------------------------------------------------------------------------
TPaymentTypeGroup TPaymentTypeGroupsDBDriver::GetPaymentGroupByName(UnicodeString name)
{
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();

        return _getPaymentGroupByName( transaction, name );

        transaction.Commit();
    }
    catch(Exception &ex)
    {

    }
}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsDBDriver::_addPaymentTypeGroup(
                                                    Database::TDBTransaction &transaction,
                                                    TPaymentTypeGroup* group)
{
    TIBSQL* insertQuery = transaction.Query(transaction.AddQuery());
    int groupKey = _getNextPaymentTypeGroupKey(transaction);
    insertQuery->SQL->Text = "INSERT INTO PAYMENTGROUPS(PAYMENTGROUPS_KEY,PAYMENTGROUP_NAME,DEFAULT_GROUP) values(:GROUP_KEY,:NAME,:IS_DEFAULT)";
    insertQuery->ParamByName("GROUP_KEY")->AsInteger = groupKey;
    insertQuery->ParamByName("NAME")->AsString = group->Name;
    insertQuery->ParamByName("IS_DEFAULT")->AsString = 'F';  // default groups will only be created by parser
    insertQuery->ExecQuery();

    return insertQuery->RowsAffected > 0;
}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsDBDriver::_updatePaymentTypeGroup(
                                                    Database::TDBTransaction &transaction,
                                                    TPaymentTypeGroup* group)
{
    TIBSQL* updateQuery = transaction.Query(transaction.AddQuery());
    updateQuery->SQL->Text = "UPDATE PAYMENTGROUPS SET PAYMENTGROUP_NAME=:NAME WHERE PAYMENTGROUPS_KEY=:GROUP_KEY";
    updateQuery->ParamByName("NAME")->AsString = group->Name;
    updateQuery->ParamByName("GROUP_KEY")->AsInteger = group->DBKey;
    updateQuery->ExecQuery();

    return updateQuery->RowsAffected > 0;
}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsDBDriver::_deletePaymentTypeGroup(
                                                    Database::TDBTransaction &transaction,
                                                    int groupKey)
{
    TIBSQL* deleteQuery = transaction.Query(transaction.AddQuery());
    deleteQuery->SQL->Text = "DELETE FROM PAYMENTGROUPS WHERE PAYMENTGROUPS_KEY=:GROUP_KEY";
    deleteQuery->ParamByName("GROUP_KEY")->AsInteger = groupKey;
    deleteQuery->ExecQuery();

    return deleteQuery->RowsAffected > 0;
}
//---------------------------------------------------------------------------
void TPaymentTypeGroupsDBDriver::_loadAssignedGroupsByType(
                                                    Database::TDBTransaction &transaction,
                                                    int paymentTypeKey,
                                                    std::vector<TPaymentTypeGroup> &assignedGroups)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =  "SELECT PG.PAYMENTGROUPS_KEY, PG.PAYMENTGROUP_NAME, PG.DEFAULT_GROUP "
                        "FROM PAYMENTGROUPS PG "
                        "JOIN PAYMENTGROUPS_PAYMENTTYPES PGTR ON PG.PAYMENTGROUPS_KEY = PGTR.PAYMENTGROUPS_KEY "
                        "WHERE PGTR.PAYMENT_KEY = :PAYTYPE_KEY "
                        "ORDER BY PGTR.PGPT_KEY ASC";
    query->ParamByName("PAYTYPE_KEY")->AsInteger = paymentTypeKey;
    query->ExecQuery();

    while(!query->Eof)
    {
        TPaymentTypeGroup group( query->FieldByName("PAYMENTGROUPS_KEY")->AsInteger );
        _loadPaymentGroupFromQuery(query, &group);
        assignedGroups.push_back(group);
        query->Next();
    }
}

//---------------------------------------------------------------------------
void TPaymentTypeGroupsDBDriver::_loadAllPaymentTypeGroups(
                                                    Database::TDBTransaction &transaction,
                                                    TList* groups)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =  "SELECT PG.PAYMENTGROUPS_KEY, PG.PAYMENTGROUP_NAME, PG.DEFAULT_GROUP "
                        "FROM PAYMENTGROUPS PG ";
    query->ExecQuery();

    while(!query->Eof)
    {
        TPaymentTypeGroup* group = new TPaymentTypeGroup(query->FieldByName("PAYMENTGROUPS_KEY")->AsInteger);
        _loadPaymentGroupFromQuery(query, group);
        groups->Add(group);
        query->Next();
    }
}
//---------------------------------------------------------------------------
void TPaymentTypeGroupsDBDriver::_removeAllAssignedGroupsFromType(
                                                    Database::TDBTransaction &transaction,
                                                    int paymentTypeKey)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "DELETE FROM PAYMENTGROUPS_PAYMENTTYPES WHERE PAYMENT_KEY=:PAYTYPE_KEY";
    query->ParamByName("PAYTYPE_KEY")->AsInteger = paymentTypeKey;
    query->ExecQuery();
}
//---------------------------------------------------------------------------
void TPaymentTypeGroupsDBDriver::_removeAllAssignedTypesFromGroup(
                                                    Database::TDBTransaction &transaction,
                                                    int paymentGroupKey)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "DELETE FROM PAYMENTGROUPS_PAYMENTTYPES WHERE PAYMENTGROUPS_KEY=:PAYGROUP_KEY";
    query->ParamByName("PAYGROUP_KEY")->AsInteger = paymentGroupKey;
    query->ExecQuery();
}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsDBDriver::_assignPaymentTypeToGroup(
                                                    Database::TDBTransaction &transaction,
                                                    int paymentTypeKey,
                                                    int groupKey)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "INSERT INTO PAYMENTGROUPS_PAYMENTTYPES(PGPT_KEY, PAYMENTGROUPS_KEY, PAYMENT_KEY) VALUES(:PGPT_KEY, :PAYMENTGROUPS_KEY, :PAYMENT_KEY)";
    query->ParamByName("PGPT_KEY")->AsInteger = _getNextPaymentTypePaymentGroupRelationKey(transaction);
    query->ParamByName("PAYMENTGROUPS_KEY")->AsInteger = groupKey;
    query->ParamByName("PAYMENT_KEY")->AsInteger = paymentTypeKey;
    query->ExecQuery();
}
//---------------------------------------------------------------------------
int TPaymentTypeGroupsDBDriver::_getNextPaymentTypeGroupKey(Database::TDBTransaction &transaction)
{
    return TGeneratorManager::GetNextGeneratorKey(transaction,"GEN_PAYMENTGROUPS");
}
//---------------------------------------------------------------------------
int TPaymentTypeGroupsDBDriver::_getNextPaymentTypePaymentGroupRelationKey(Database::TDBTransaction &transaction)
{
    return TGeneratorManager::GetNextGeneratorKey(transaction,"GEN_PAYMENTGROUPS_PAYMENTTYPES");
}
//---------------------------------------------------------------------------
void TPaymentTypeGroupsDBDriver::LoadAssignedTypesByGroup(int paymentGroupKey,TList* assignedTypes)
{
    try
    {
        Database::TDBTransaction transaction(*DBControl);
        transaction.StartTransaction();
        _loadAssignedTypesByGroup(transaction, paymentGroupKey, assignedTypes);
        transaction.Commit();
    }
    catch(Exception &ex)
    {
    }
}
//---------------------------------------------------------------------------
void TPaymentTypeGroupsDBDriver::_loadAssignedTypesByGroup(Database::TDBTransaction &transaction,int paymentGroupKey,TList* assignedTypes)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =  "SELECT PT.PAYMENT_KEY, PT.PAYMENT_NAME "
                        "FROM PAYMENTGROUPS_PAYMENTTYPES PGTR JOIN PAYMENTTYPES PT "
                        "ON PGTR.PAYMENT_KEY = PT.PAYMENT_KEY "
                        "WHERE PGTR.PAYMENTGROUPS_KEY = :PAYGROUP_KEY"
                        "";
    query->ParamByName("PAYGROUP_KEY")->AsInteger = paymentGroupKey;
    query->ExecQuery();

    while(!query->Eof)
    {
        TPaymentType* type = new TPaymentType(query->FieldByName("PAYMENT_KEY")->AsInteger);
        _loadPaymentTypeFromQuery(query, type);
        assignedTypes->Add(type);
        query->Next();
    }
}
//---------------------------------------------------------------------------
TPaymentTypeGroup TPaymentTypeGroupsDBDriver::_getPaymentGroupByName( Database::TDBTransaction &transaction, UnicodeString name )
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =  "SELECT a.PAYMENTGROUPS_KEY, a.PAYMENTGROUP_NAME, a.DEFAULT_GROUP "
                        "FROM PAYMENTGROUPS a "
                        "WHERE a.PAYMENTGROUP_NAME = :PAYMENTGROUP_NAME";

    query->ParamByName("PAYMENTGROUP_NAME")->AsString = name;
    query->ExecQuery();

    if(!query->Eof)
    {
        TPaymentTypeGroup group(query->FieldByName("PAYMENTGROUPS_KEY")->AsInteger);
        _loadPaymentGroupFromQuery(query, &group);
        return group;
    }
    else
    {
        TPaymentTypeGroup group(-1);
        return group;
    }
}
//---------------------------------------------------------------------------
void TPaymentTypeGroupsDBDriver::_loadPaymentTypeFromQuery(TIBSQL* query, TPaymentType* type)
{
    type->Name = query->FieldByName("PAYMENT_NAME")->AsString;
    //group->IsDefault = query->FieldByName("DEFAULT_GROUP")->AsString == "T" ? true : false;
}
//---------------------------------------------------------------------------
void TPaymentTypeGroupsDBDriver::_loadPaymentGroupFromQuery(TIBSQL* query, TPaymentTypeGroup* group)
{
    group->Name = query->FieldByName("PAYMENTGROUP_NAME")->AsString;
    group->IsDefault = query->FieldByName("DEFAULT_GROUP")->AsString == "T" ? true : false;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
