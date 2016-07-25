//---------------------------------------------------------------------------

#ifndef MallExportSalesTypeDriverInterfaceH
#define MallExportSalesTypeDriverInterfaceH

#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

class TMallExportSalesTypeDriverInterface
{
    public:
        virtual void LoadAllSalesType(TList* types) = 0;
};

class TMallExportSalesTypeGroupDriverInterface
{
public:
    virtual void LoadAllSalesTypeGroup(TList* groups) = 0;
    virtual void LoadAssignedItemsByGroup(int salestypeGroupKey, TList* assignedTypes) = 0;
    virtual bool AssignItemsToSalesTypeGroup(int salestypeGroupKey, std::vector<int> typeKeys) = 0;
    virtual void LoadAssignedSalesTypeGroupByItem(int salesTypeKey, TList* assignedGroups) = 0;
};
#endif
