//---------------------------------------------------------------------------


#pragma hdrstop

#include "DiscountGroupsManager.h"
#include "DiscountGroupsDBDriver.h"

//---------------------------------------------------------------------------

TDiscountGroupsManager::TDiscountGroupsManager()
{
    _driver.reset(new TDiscountGroupsDBDriver());
}
//---------------------------------------------------------------------------

bool TDiscountGroupsManager::AddDiscountGroups(TDiscountGroup* group)
{
    return _driver->AddDiscountGroups(group);
};
//---------------------------------------------------------------------------

bool TDiscountGroupsManager::UpdateDiscountGroups(TDiscountGroup* group)
{
    return _driver->UpdateDiscountGroups(group);
};
//---------------------------------------------------------------------------

bool TDiscountGroupsManager::DeleteDiscountGroups(int groupKey)
{
    return _driver->DeleteDiscountGroups(groupKey);
};
//---------------------------------------------------------------------------

void TDiscountGroupsManager::LoadAssignedGroupsByType(int discountTypeKey, TList* assignedGroups)
{
    return _driver->LoadAssignedGroupsByType(discountTypeKey, assignedGroups);
};
//---------------------------------------------------------------------------

void TDiscountGroupsManager::LoadAllDiscountGroups(TList* groups)
{
    return _driver->LoadAllDiscountGroups(groups);
};
//---------------------------------------------------------------------------

//bool TDiscountGroupsManager::AssignGroupsToDiscountType(int discountTypeKey, std::vector<int> groupKeys)
//{
//    return _driver.AssignGroupsToDiscountType(discountTypeKey, groupKeys);
//};
//---------------------------------------------------------------------------

void TDiscountGroupsManager::LoadAssignedTypesByGroups(int discountTypeKey, TList* assignedTypes)
{
    return _driver->LoadAssignedTypesByGroups(discountTypeKey, assignedTypes);
};
//---------------------------------------------------------------------------

bool TDiscountGroupsManager::AssignTypesToDiscountGroups(int discountGroupKey, std::vector<int> typeKeys)
{
    return _driver->AssignTypesToDiscountGroups(discountGroupKey, typeKeys);
};
//---------------------------------------------------------------------------

#pragma package(smart_init)
