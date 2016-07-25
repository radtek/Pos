//---------------------------------------------------------------------------

#ifndef MallExportSalesTypeDBDriverH
#define MallExportSalesTypeDBDriverH

#include "MallExportSalesTypeDriverInterface.h"
#include "MallExportSalesType.h"
#include "MM_DBCore.cpp"
//---------------------------------------------------------------------------

class TMallExportSalesTypeDBDriver : public TMallExportSalesTypeDriverInterface
{
    public:
        void LoadAllSalesType(TList* types);
    private:
        void _loadAllSalesType(Database::TDBTransaction &transaction, TList* types);
};

class TMallExportSalesTypeGroupDBDriver : public TMallExportSalesTypeGroupDriverInterface
{
    public:
        TMallExportSalesTypeGroupDBDriver();
        void LoadAllSalesTypeGroup(TList* groups);
        void LoadAssignedItemsByGroup(int salestypeGroupKey, TList* assignedTypes);
        bool AssignItemsToSalesTypeGroup(int salestypeGroupKey, std::vector<int> typeKeys);
        void LoadAssignedSalesTypeGroupByItem(int salesTypeKey, TList* assignedGroups);

    private:
        void _loadAllSalesTypeGroup(Database::TDBTransaction &transaction, TList* groups);
        void _loadAssignedItemsByGroup(Database::TDBTransaction &transaction, int salestypeGroupKey, TList* assignedTypes);
        bool _assignItemsToSalesTypeGroup(Database::TDBTransaction &transaction, int salesTypeKey, int groupKey);
        void _loadAssignedSalesTypeGroupByItem(Database::TDBTransaction &transaction, int salesTypeKey, TList* assignedGroups);

        void _removeAllAssignedSalesTypeGroupFromItem(Database::TDBTransaction &transaction, int salesTypeKey);
        void _removeAllAssignedItemsFromGroup(Database::TDBTransaction &transaction, int salestypeGroupKey);
        int _getNextSalesTypeItemsRelationKey(Database::TDBTransaction &transaction);
        void _loadSalesTypeGroupFromQuery(TIBSQL* query, TMallExportSalesTypeGroup* group);
        void _loadSalesTypeFromQuery(TIBSQL* query, TMallExportSalesType* type);
        Database::TDBControl* DBControl;

//        void _removeAllAssignedGroupsFromType(Database::TDBTransaction &transaction, int discountTypeKey);
//        void _removeAllAssignedTypesFromGroups(Database::TDBTransaction &transaction, int discountGroupKey);
//        int _getNextDiscountTypeDiscountGroupsRelationKey(Database::TDBTransaction &transaction);
//        void _loadDiscountGroupsFromQuery(TIBSQL* query, TDiscountGroup* group);
//        void _loadDiscountTypeFromQuery(TIBSQL* query, TDiscountTypes* type);
};
#endif
