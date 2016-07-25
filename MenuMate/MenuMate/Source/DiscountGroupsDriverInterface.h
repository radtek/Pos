//---------------------------------------------------------------------------

#ifndef DiscountGroupsDriverInterfaceH
#define DiscountGroupsDriverInterfaceH

#include "DeviceRealTerminal.h"
#include "DiscountGroup.h"
//---------------------------------------------------------------------------

class TDiscountGroupsDriverInterface
{
public:
    virtual bool AddDiscountGroups(TDiscountGroup* group) = 0;
    virtual bool UpdateDiscountGroups(TDiscountGroup* group) = 0;
    virtual bool DeleteDiscountGroups(int groupKey) = 0;
    virtual void LoadAllDiscountGroups(TList* groups) = 0;
    virtual void LoadAssignedTypesByGroups(int discountGroupKey, TList* assignedTypes) = 0;
    virtual bool AssignTypesToDiscountGroups(int discountGroupKey, std::vector<int> typeKeys) = 0;
    virtual void LoadAssignedGroupsByType(int discountTypeKey, TList* assignedGroups) = 0;
};
#endif
