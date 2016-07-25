//---------------------------------------------------------------------------

#ifndef DiscountGroupsAssignmentH
#define DiscountGroupsAssignmentH

#include "GenericGroupAssignment.h"
#include "DiscountTypes.h"
//---------------------------------------------------------------------------

typedef std::map<int, TDiscountTypes*> DTMap;

class TfrmDiscountGroupsAssignment : public TfrmGenericGroupAssignment
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
    __fastcall TfrmDiscountGroupsAssignment(TComponent* Owner);

private:
    std::map<int, DTMap> DiscountGroups;
    DTMap DiscountTypes;
    int selectedGroup;
    void RemoveFromGroup(int TypeKey, int selectedGroupKey);
    void disableSelectedTypesInGroup(int GroupKey);
    void displayItemsInGroups(int GroupKey);
    void storeDiscountGroupsMapInMemory(int GroupKey, TList* TypesList);
    void clearDiscountTypes();
    void clearDiscountGroups();
    void clearDiscountTypes(DTMap* DiscountTypeMap);
};

#endif
