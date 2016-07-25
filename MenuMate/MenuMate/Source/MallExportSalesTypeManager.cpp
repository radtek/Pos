//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportSalesTypeManager.h"
#include "MallExportSalesTypeDBDriver.h"
//---------------------------------------------------------------------------

TMallExportSalesTypeManager::TMallExportSalesTypeManager()
{
    _driver.reset(new TMallExportSalesTypeDBDriver());
}
//---------------------------------------------------------------------------
void TMallExportSalesTypeManager::LoadAllSalesType(TList* types)
{
    _driver->LoadAllSalesType(types);
}
//---------------------------------------------------------------------------

TMallExportSalesTypeGroupManager::TMallExportSalesTypeGroupManager()
{
    _driver.reset(new TMallExportSalesTypeGroupDBDriver());
}
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupManager::LoadAllSalesTypeGroup(TList* groups)
{
    return _driver->LoadAllSalesTypeGroup(groups);
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupManager::LoadAssignedItemsByGroup(int salesTypeKey, TList* assignedTypes)
{
    return _driver->LoadAssignedItemsByGroup(salesTypeKey, assignedTypes);
};
//---------------------------------------------------------------------------

bool TMallExportSalesTypeGroupManager::AssignItemsToSalesTypeGroup(int salestypeGroupKey, std::vector<int> typeKeys)
{
    return _driver->AssignItemsToSalesTypeGroup(salestypeGroupKey, typeKeys);
};
//---------------------------------------------------------------------------

void TMallExportSalesTypeGroupManager::LoadAssignedSalesTypeGroupByItem(int salesTypeKey, TList* assignedGroups)
{
    return _driver->LoadAssignedSalesTypeGroupByItem(salesTypeKey, assignedGroups);
};
//---------------------------------------------------------------------------
#pragma package(smart_init)
