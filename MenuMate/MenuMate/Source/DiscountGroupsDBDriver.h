//---------------------------------------------------------------------------

#ifndef DiscountGroupsDBDriverH
#define DiscountGroupsDBDriverH

#include "DiscountGroupsDriverInterface.h"
#include "DiscountTypes.h"
#include "DiscountGroup.h"
#include "MM_DBCore.cpp"
//---------------------------------------------------------------------------

class TDiscountGroupsDBDriver : public TDiscountGroupsDriverInterface
{
    public:
        TDiscountGroupsDBDriver();
        bool AddDiscountGroups(TDiscountGroup* group);
        bool UpdateDiscountGroups(TDiscountGroup* group);
        bool DeleteDiscountGroups(int groupKey);
        void LoadAssignedGroupsByType(int discountTypeKey, TList* assignedGroups);
        void LoadAllDiscountGroups(TList* groups);
        //bool AssignGroupsToDiscountType(int discountTypeKey, std::vector<int> groupKeys);
        void LoadAssignedTypesByGroups(int discountGroupKey, TList* assignedTypes);
        bool AssignTypesToDiscountGroups(int discountGroupKey, std::vector<int> typeKeys);

    private:
        bool _addDiscountGroups(Database::TDBTransaction &transaction, TDiscountGroup* group);
        bool _updateDiscountGroups(Database::TDBTransaction &transaction, TDiscountGroup* group);
        bool _deleteDiscountGroups(Database::TDBTransaction &transaction, int groupKey);
        void _loadAssignedGroupsByType(Database::TDBTransaction &transaction, int discountTypeKey, TList* assignedGroups);
        void _loadAllDiscountGroups(Database::TDBTransaction &transaction, TList* groups);
        bool _assignDiscountTypeToGroups(Database::TDBTransaction &transaction, int discountTypeKey, int groupKey);
        void _removeAllAssignedGroupsFromType(Database::TDBTransaction &transaction, int discountTypeKey);
        void _removeAllAssignedTypesFromGroups(Database::TDBTransaction &transaction, int discountGroupKey);
        int _getNextDiscountGroupsKey(Database::TDBTransaction &transaction);
        int _getNextDiscountTypeDiscountGroupsRelationKey(Database::TDBTransaction &transaction);
        void _loadDiscountGroupsFromQuery(TIBSQL* query, TDiscountGroup* group);
        void _loadAssignedTypesByGroup(Database::TDBTransaction &transaction, int discountGroupKey, TList* assignedTypes);
        void _loadDiscountTypeFromQuery(TIBSQL* query, TDiscountTypes* type);
        Database::TDBControl* DBControl;
};
#endif
