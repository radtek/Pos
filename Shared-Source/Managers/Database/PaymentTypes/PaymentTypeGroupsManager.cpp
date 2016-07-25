//---------------------------------------------------------------------------


#pragma hdrstop

#include "PaymentTypeGroupsManager.h"
#include "PaymentTypeGroupsDBDriver.h"

//---------------------------------------------------------------------------
TPaymentTypeGroupsManager::TPaymentTypeGroupsManager()
{
    _driver.reset(new TPaymentTypeGroupsDBDriver());
}

//---------------------------------------------------------------------------
bool TPaymentTypeGroupsManager::AddPaymentTypeGroup(TPaymentTypeGroup* group)
{
    return _driver->AddPaymentTypeGroup(group);
}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsManager::UpdatePaymentTypeGroup(TPaymentTypeGroup* group)
{
    return _driver->UpdatePaymentTypeGroup(group);
}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsManager::DeletePaymentTypeGroup(int groupKey)
{
    return _driver->DeletePaymentTypeGroup(groupKey);
}
//---------------------------------------------------------------------------
void TPaymentTypeGroupsManager::LoadAssignedGroupsByType(int paymentTypeKey, std::vector<TPaymentTypeGroup> &assignedGroups)
{
    _driver->LoadAssignedGroupsByType( paymentTypeKey, assignedGroups );
}

//---------------------------------------------------------------------------
void TPaymentTypeGroupsManager::LoadAllPaymentTypeGroups(TList* groups)
{
    return _driver->LoadAllPaymentTypeGroups(groups);
}
//---------------------------------------------------------------------------
//bool TPaymentTypeGroupsManager::AssignGroupsToPaymentType(int paymentTypeKey, std::vector<int> groupKeys)
//{
//    return _driver.AssignGroupsToPaymentType(paymentTypeKey, groupKeys);
//}
//---------------------------------------------------------------------------
void TPaymentTypeGroupsManager::LoadAssignedTypesByGroup(int paymentTypeKey, TList* assignedTypes)
{
    return _driver->LoadAssignedTypesByGroup(paymentTypeKey, assignedTypes);
}
//---------------------------------------------------------------------------
bool TPaymentTypeGroupsManager::AssignTypesToPaymentGroup(int paymentGroupKey, std::vector<int> typeKeys)
{
    return _driver->AssignTypesToPaymentGroup(paymentGroupKey, typeKeys);
}
//---------------------------------------------------------------------------
TPaymentTypeGroup TPaymentTypeGroupsManager::GetPaymentGroupByName(UnicodeString name)
{
    return _driver->GetPaymentGroupByName(name);
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
