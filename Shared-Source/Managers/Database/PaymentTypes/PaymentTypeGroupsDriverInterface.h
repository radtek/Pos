//---------------------------------------------------------------------------

#ifndef PaymentTypeGroupsDriverInterfaceH
#define PaymentTypeGroupsDriverInterfaceH
//---------------------------------------------------------------------------

#include "DeviceRealTerminal.h"
#include "PaymentTypeGroup.h"

class TPaymentTypeGroupsDriverInterface
{
public:
    virtual bool AddPaymentTypeGroup(TPaymentTypeGroup* group) = 0;
    virtual bool UpdatePaymentTypeGroup(TPaymentTypeGroup* group) = 0;
    virtual bool DeletePaymentTypeGroup(int groupKey) = 0;
    virtual void LoadAllPaymentTypeGroups(TList* groups) = 0;
    virtual void LoadAssignedTypesByGroup(int paymentGroupKey, TList* assignedTypes) = 0;
    virtual void LoadAssignedGroupsByType(int paymentTypeKey, std::vector<TPaymentTypeGroup> &assignedGroups) = 0;
    virtual bool AssignTypesToPaymentGroup(int paymentGroupKey, std::vector<int> typeKeys) = 0;
    virtual TPaymentTypeGroup GetPaymentGroupByName(UnicodeString name) = 0;
};
#endif
