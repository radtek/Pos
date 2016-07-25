//---------------------------------------------------------------------------
#ifndef PaymentTypeGroupAssignmentH
#define PaymentTypeGroupAssignmentH
//---------------------------------------------------------------------------

#include "GenericGroupAssignment.h"
#include "PaymentType.h"

typedef std::map<int, TPaymentType*> PTMap;

class TfrmPaymentTypeGroupAssignment : public TfrmGenericGroupAssignment
{
public:
    void DisplayGroups();
    void DisplayTypes();
    void DisplayItemsInGroups(int GroupKey);
    void SaveAssignment();
    void AddToGroup(int selectedGroupKey, int TypeKey);
    __fastcall TfrmPaymentTypeGroupAssignment(TComponent* Owner);
    void GroupSelected(TGridButton* GridButton);
    void TypeSelected(TGridButton* GridButton);
    void RemoveTypefromGroup(int TypeKey);
    void CleanUpMemory();
private:
    std::map<int, PTMap> PaymentGroups;
    PTMap PaymentTypes;
    int selectedGroup;
    void RemoveFromGroup(int TypeKey, int selectedGroupKey);
    void disableSelectedTypesInGroup(int GroupKey);
    void displayItemsInGroups(int GroupKey);
    void storePaymentTypeGroupMapInMemory(int GroupKey, TList* TypesList);
    void clearPaymentTypes();
    void clearPaymentGroups();
    void clearPaymentTypes(PTMap* PaymentTypeMap);
};

//---------------------------------------------------------------------------
#endif
