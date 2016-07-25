//---------------------------------------------------------------------------

#ifndef DiscountGroupsManagerH
#define DiscountGroupsManagerH

#include "DiscountGroupsDriverInterface.h"
//---------------------------------------------------------------------------
class TDiscountGroupsManager
{
    public:
        TDiscountGroupsManager();
        bool AddDiscountGroups(TDiscountGroup* group);
        bool UpdateDiscountGroups(TDiscountGroup* group);
        bool DeleteDiscountGroups(int groupKey);
        void LoadAssignedGroupsByType(int discountTypeKey, TList* assignedGroups);
//        void LoadAssignedGroupsByType(int discountTypeKey, TList* assignedGroups);
        void LoadAllDiscountGroups(TList* groups);
        //bool AssignGroupsToDiscountType(int discountTypeKey, std::vector<int> groupKeys);
        void LoadAssignedTypesByGroups(int discountGroupKey, TList* assignedTypes);
        bool AssignTypesToDiscountGroups(int discountGroupKey, std::vector<int> typeKeys);

    private:
        std::auto_ptr<TDiscountGroupsDriverInterface> _driver;
};
#endif
