//---------------------------------------------------------------------------

#ifndef PaymentTypeGroupsDBDriverH
#define PaymentTypeGroupsDBDriverH

#include "PaymentTypeGroupsDriverInterface.h"
#include "PaymentType.h"
#include "PaymentTypeGroup.h"
#include "MM_DBCore.cpp"

class TPaymentTypeGroupsDBDriver : public TPaymentTypeGroupsDriverInterface
{
    public:
        TPaymentTypeGroupsDBDriver();
        bool AddPaymentTypeGroup(TPaymentTypeGroup* group);
        bool UpdatePaymentTypeGroup(TPaymentTypeGroup* group);
        bool DeletePaymentTypeGroup(int groupKey);
        void LoadAssignedGroupsByType(int paymentTypeKey, std::vector<TPaymentTypeGroup> &assignedGroups);
        void LoadAllPaymentTypeGroups(TList* groups);
        //bool AssignGroupsToPaymentType(int paymentTypeKey, std::vector<int> groupKeys);
        void LoadAssignedTypesByGroup(int paymentGroupKey, TList* assignedTypes);
        bool AssignTypesToPaymentGroup(int paymentGroupKey, std::vector<int> typeKeys);
        TPaymentTypeGroup GetPaymentGroupByName(UnicodeString name);

    private:
        bool _addPaymentTypeGroup(Database::TDBTransaction &transaction, TPaymentTypeGroup* group);
        bool _updatePaymentTypeGroup(Database::TDBTransaction &transaction, TPaymentTypeGroup* group);
        bool _deletePaymentTypeGroup(Database::TDBTransaction &transaction, int groupKey);
        void _loadAssignedGroupsByType(Database::TDBTransaction &transaction, int paymentTypeKey, std::vector<TPaymentTypeGroup> &assignedGroups);
        void _loadAllPaymentTypeGroups(Database::TDBTransaction &transaction, TList* groups);
        bool _assignPaymentTypeToGroup(Database::TDBTransaction &transaction, int paymentTypeKey, int groupKey);
        void _removeAllAssignedGroupsFromType(Database::TDBTransaction &transaction, int paymentTypeKey);
        void _removeAllAssignedTypesFromGroup(Database::TDBTransaction &transaction, int paymentGroupKey);
        int _getNextPaymentTypeGroupKey(Database::TDBTransaction &transaction);
        int _getNextPaymentTypePaymentGroupRelationKey(Database::TDBTransaction &transaction);
        void _loadAssignedTypesByGroup(Database::TDBTransaction &transaction, int paymentGroupKey, TList* assignedTypes);
        TPaymentTypeGroup _getPaymentGroupByName( Database::TDBTransaction &transaction, UnicodeString name );
        void _loadPaymentTypeFromQuery(TIBSQL* query, TPaymentType* type);
        void _loadPaymentGroupFromQuery(TIBSQL* query, TPaymentTypeGroup* group);
        Database::TDBControl* DBControl;
};
#endif
