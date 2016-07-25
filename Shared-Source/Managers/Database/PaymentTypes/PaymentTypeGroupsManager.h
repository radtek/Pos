//---------------------------------------------------------------------------

#ifndef PaymentTypeGroupsManagerH
#define PaymentTypeGroupsManagerH

#include "PaymentTypeGroupsDriverInterface.h"

//---------------------------------------------------------------------------
class TPaymentTypeGroupsManager
{
    public:
        TPaymentTypeGroupsManager();
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
        std::auto_ptr<TPaymentTypeGroupsDriverInterface> _driver;
};
#endif
