//---------------------------------------------------------------------------


#pragma hdrstop

#include "DiscountGroupsAssignment.h"
#include "DiscountTypesManager.h"
#include "DiscountGroupsManager.h"

//---------------------------------------------------------------------------
__fastcall TfrmDiscountGroupsAssignment::TfrmDiscountGroupsAssignment(TComponent* Owner) : TfrmGenericGroupAssignment(Owner)
{
    selectedGroup = 0;
    Panel1->Caption = "Discount Types";
    Panel2->Caption = "Discount Group Types";
    Panel12->Caption = "Discount Groups";
}
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::DisplayGroups()
{
    TDiscountGroupsManager DGManager;
    std::auto_ptr<TList> DGGList(new TList());
    DGManager.LoadAllDiscountGroups(DGGList.get());
    GroupList->RowCount=DGGList->Count;
    for (int i = 0; i < DGGList->Count; i++)
    {
        GroupList->Buttons[i][0]->Caption = ((TDiscountGroup*)DGGList->Items[i])->Name;
        GroupList->Buttons[i][0]->Tag = ((TDiscountGroup*)DGGList->Items[i])->DBKey;
    }
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::DisplayTypes()
{
    TDiscountTypesManager DTManager;
    std::auto_ptr<TList> DTList(new TList());
    DTManager.LoadAllDiscountTypes(DTList.get());

    MembersGrid->RowCount=DTList->Count;
    TDiscountTypes* DiscountType;
    clearDiscountTypes();
    for (int i = 0; i < DTList->Count; i++)
    {
        DiscountType = ((TDiscountTypes*)DTList->Items[i]);
        MembersGrid->Buttons[i][0]->Caption = DiscountType->Name;
        MembersGrid->Buttons[i][0]->Tag = DiscountType->DBKey;
        DiscountTypes[DiscountType->DBKey] = DiscountType;
    }
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::DisplayItemsInGroups(int GroupKey)
{
    TDiscountGroupsManager DGManager;
    std::auto_ptr<TList> ADBGList(new TList());
    if(DiscountGroups.find(GroupKey) == DiscountGroups.end())
    {
        DGManager.LoadAssignedTypesByGroups(GroupKey, ADBGList.get());
        storeDiscountGroupsMapInMemory(GroupKey, ADBGList.get());
    }
    displayItemsInGroups(GroupKey);
    disableSelectedTypesInGroup(GroupKey);
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::displayItemsInGroups(int GroupKey)
{
    DTMap DiscountGroupsMap = DiscountGroups[GroupKey];
    GroupMembers->RowCount=DiscountGroupsMap.size();

    DTMap::iterator it = DiscountGroupsMap.begin();
    int i = 0;
    for (; it != DiscountGroupsMap.end(); it++)
    {
        GroupMembers->Buttons[i][0]->Caption = (it->second)->Name;
        GroupMembers->Buttons[i][0]->Tag = (it->second)->DBKey;
        i++;
    }
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::disableSelectedTypesInGroup(int GroupKey)
{
    for(int i=0; i < MembersGrid->RowCount; i++)
    {
        int checkDiscountInGroup = MembersGrid->Buttons[i][0]->Tag;
        if(DiscountGroups[GroupKey].find(checkDiscountInGroup) != DiscountGroups[GroupKey].end())
        {
            MembersGrid->Buttons[i][0]->Enabled = false;
        } else {
            MembersGrid->Buttons[i][0]->Enabled = true;
        }

    }
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::storeDiscountGroupsMapInMemory(int GroupKey, TList* TypeList)
{
    TDiscountTypes* DT;
    for(int i=0; i<TypeList->Count; i++)
    {
        DT = (TDiscountTypes*) TypeList->Items[i];
        DiscountGroups[GroupKey].insert( std::pair<int,TDiscountTypes*>(DT->DBKey,DT));
    }
    int test = (DiscountGroups[GroupKey]).size();
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::SaveAssignment()
{
    std::map<int, DTMap>::iterator it = DiscountGroups.begin();
    DTMap::iterator itDT;
    std::vector<int> GroupsDiscountTypes;
    TDiscountGroupsManager DGManager;
    for(; it != DiscountGroups.end(); it++)
    {
        itDT = (it->second).begin();
        GroupsDiscountTypes.clear();
        for(; itDT != (it->second).end(); itDT++)
        {
            GroupsDiscountTypes.push_back(itDT->first);
        }
        DGManager.AssignTypesToDiscountGroups(it->first, GroupsDiscountTypes);
    }
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::AddToGroup(int selectedGroupKey, int TypeKey)
{
    DiscountGroups[selectedGroupKey].insert( std::pair<int,TDiscountTypes*>(TypeKey, DiscountTypes[TypeKey]));
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::RemoveFromGroup(int TypeKey, int GroupKey)
{
    DiscountGroups[GroupKey].erase(TypeKey);
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::GroupSelected(TGridButton *GridButton)
{
    selectedGroup = GridButton->Tag;
    DisplayItemsInGroups(selectedGroup);
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::TypeSelected(TGridButton *GridButton)
{
    if(selectedGroup != 0)
    {
        AddToGroup(selectedGroup, GridButton->Tag);
        DisplayItemsInGroups(selectedGroup);
    }
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::RemoveTypefromGroup(int TypeKey)
{
    RemoveFromGroup(TypeKey, selectedGroup);
    DisplayItemsInGroups(selectedGroup);
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::CleanUpMemory()
{
    clearDiscountTypes();
    clearDiscountGroups();
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::clearDiscountTypes()
{
    clearDiscountTypes(&DiscountTypes);
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::clearDiscountGroups()
{
    std::map<int, DTMap>::iterator itDG = DiscountGroups.begin();
    for (; itDG != DiscountGroups.end(); itDG++)
    {
        clearDiscountTypes(&(itDG->second));
    }
    DiscountGroups.clear();
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsAssignment::clearDiscountTypes(DTMap* DiscountTypeMap)
{
    DTMap::iterator it = DiscountTypeMap->begin();
    for (; it != DiscountTypeMap->end(); it++)
    {
        delete it->second;
    }
    DiscountTypeMap->clear();
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
