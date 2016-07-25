//---------------------------------------------------------------------------

#ifndef MallExportSalesTypeAssignmentH
#define MallExportSalesTypeAssignmentH

#include "GenericGroupAssignment.h"
#include "MallExportSalesType.h"
//---------------------------------------------------------------------------

typedef std::map<int, TMallExportSalesType*> STMap;

class TfrmMallExportSalesTypeAssignment : public TfrmGenericGroupAssignment
{

public:
    void DisplayGroups();
    void DisplayTypes();
    void DisplayItemsInGroups(int GroupKey);
    void SaveAssignment();
    void AddToGroup(int selectedGroupKey, int TypeKey);
    void GroupSelected(TGridButton* GridButton);
    void TypeSelected(TGridButton* GridButton);
    void RemoveTypefromGroup(int TypeKey);
    void CleanUpMemory();
    void AssignAllItems();
    void RemoveAllItems();
    __fastcall TfrmMallExportSalesTypeAssignment(TComponent* Owner);

private:
    std::map<int, STMap> MallExportSalesTypeGroup;
    STMap MallExportSalesType;
    int selectedGroup;
    void RemoveFromGroup(int TypeKey, int selectedGroupKey);
    void disableSelectedTypesInGroup(int GroupKey);
    void displayItemsInGroups(int GroupKey);
    void storeSalesTypeGroupMapInMemory(int GroupKey, TList* TypesList);
    void clearSalesType();
    void clearSalesTypeGroup();
    void clearSalesType(STMap* SalesTypeMap);
};

//---------------------------------------------------------------------------
#endif
