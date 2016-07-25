//---------------------------------------------------------------------------

#ifndef MallExportSalesTypeManagerH
#define MallExportSalesTypeManagerH

#include "MallExportSalesTypeDriverInterface.h"
//---------------------------------------------------------------------------

class TMallExportSalesTypeManager
{
    public:
        TMallExportSalesTypeManager();
        void LoadAllSalesType(TList* types);

    private:
        std::auto_ptr<TMallExportSalesTypeDriverInterface> _driver;
};

class TMallExportSalesTypeGroupManager
{
    public:
        TMallExportSalesTypeGroupManager();
        void LoadAllSalesTypeGroup(TList* groups);
        void LoadAssignedItemsByGroup(int salestypeGroupKey, TList* assignedTypes);
        bool AssignItemsToSalesTypeGroup(int salestypeGroupKey, std::vector<int> typeKeys);
        void LoadAssignedSalesTypeGroupByItem(int salesTypeKey, TList* assignedGroups);

    private:
        std::auto_ptr<TMallExportSalesTypeGroupDriverInterface> _driver;
};
#endif
