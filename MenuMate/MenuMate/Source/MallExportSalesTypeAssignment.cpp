//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportSalesTypeAssignment.h"
#include "MallExportSalesTypeManager.h"

//---------------------------------------------------------------------------

__fastcall TfrmMallExportSalesTypeAssignment::TfrmMallExportSalesTypeAssignment(TComponent* Owner) : TfrmGenericGroupAssignment(Owner)
{
    selectedGroup = 0;
    Panel1->Caption = "Items";
    Panel2->Caption = "Sales Types Items";
    Panel12->Caption = "Sales Types";
    btnAssignAll->Visible = true;
    btnRemoveAll->Visible = true;
}
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::DisplayTypes()
{
    TMallExportSalesTypeManager STManager;
    std::auto_ptr<TList> STList(new TList());
    STManager.LoadAllSalesType(STList.get());

    MembersGrid->RowCount=STList->Count;
    TMallExportSalesType* SalesType;
    clearSalesType();
    for (int i = 0; i < STList->Count; i++)
    {
        SalesType = ((TMallExportSalesType*)STList->Items[i]);
        MembersGrid->Buttons[i][0]->Caption = SalesType->Name;
        MembersGrid->Buttons[i][0]->Tag = SalesType->DBKey;
        MallExportSalesType[SalesType->DBKey] = SalesType;
    }
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::clearSalesType()
{
    clearSalesType(&MallExportSalesType);
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::clearSalesType(STMap* SalesTypeMap)
{
    STMap::iterator it = SalesTypeMap->begin();
    for (; it != SalesTypeMap->end(); it++)
    {
        delete it->second;
    }
    SalesTypeMap->clear();
}
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::DisplayGroups()
{
    TMallExportSalesTypeGroupManager STGManager;
    std::auto_ptr<TList> STGList(new TList());
    STGManager.LoadAllSalesTypeGroup(STGList.get());
    GroupList->RowCount=STGList->Count;
    for (int i = 0; i < STGList->Count; i++)
    {
        GroupList->Buttons[i][0]->Caption = ((TMallExportSalesTypeGroup*)STGList->Items[i])->Name;
        GroupList->Buttons[i][0]->Tag = ((TMallExportSalesTypeGroup*)STGList->Items[i])->DBKey;
    }
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::GroupSelected(TGridButton *GridButton)
{
    selectedGroup = GridButton->Tag;
    DisplayItemsInGroups(selectedGroup);
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::TypeSelected(TGridButton *GridButton)
{
    if(selectedGroup != 0)
    {
        AddToGroup(selectedGroup, GridButton->Tag);
        DisplayItemsInGroups(selectedGroup);
    }
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::DisplayItemsInGroups(int GroupKey)
{
    TMallExportSalesTypeGroupManager STGManager;
    std::auto_ptr<TList> ADBGList(new TList());
    if(MallExportSalesTypeGroup.find(GroupKey) == MallExportSalesTypeGroup.end())
    {
        STGManager.LoadAssignedItemsByGroup(GroupKey, ADBGList.get());
        storeSalesTypeGroupMapInMemory(GroupKey, ADBGList.get());
    }
    displayItemsInGroups(GroupKey);
    disableSelectedTypesInGroup(GroupKey);
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::storeSalesTypeGroupMapInMemory(int GroupKey, TList* TypeList)
{
    TMallExportSalesType* ST;
    for(int i=0; i<TypeList->Count; i++)
    {
        ST = (TMallExportSalesType*) TypeList->Items[i];
        MallExportSalesTypeGroup[GroupKey].insert( std::pair<int,TMallExportSalesType*>(ST->DBKey,ST));
    }
    int test = (MallExportSalesTypeGroup[GroupKey]).size();
};
//---------------------------------------------------------------------------
void TfrmMallExportSalesTypeAssignment::AddToGroup(int selectedGroupKey, int TypeKey)
{
    MallExportSalesTypeGroup[selectedGroupKey].insert( std::pair<int,TMallExportSalesType*>(TypeKey, MallExportSalesType[TypeKey]));
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::displayItemsInGroups(int GroupKey)
{
    STMap SalesTypeGroupMap = MallExportSalesTypeGroup[GroupKey];
    GroupMembers->RowCount=SalesTypeGroupMap.size();

    STMap::iterator it = SalesTypeGroupMap.begin();
    int i = 0;
    for (; it != SalesTypeGroupMap.end(); it++)
    {
        GroupMembers->Buttons[i][0]->Caption = (it->second)->Name;
        GroupMembers->Buttons[i][0]->Tag = (it->second)->DBKey;
        i++;
    }
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::disableSelectedTypesInGroup(int GroupKey)
{
    for(int i=0; i < MembersGrid->RowCount; i++)
    {
        int checkItemsInGroup = MembersGrid->Buttons[i][0]->Tag;
        if(MallExportSalesTypeGroup[GroupKey].find(checkItemsInGroup) != MallExportSalesTypeGroup[GroupKey].end())
        {
            MembersGrid->Buttons[i][0]->Enabled = false;
        } else {
            MembersGrid->Buttons[i][0]->Enabled = true;
        }

    }
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::SaveAssignment()
{
    std::map<int, STMap>::iterator it = MallExportSalesTypeGroup.begin();
    STMap::iterator itST;
    std::vector<int> GroupSalesType;
    TMallExportSalesTypeGroupManager STGManager;
    for(; it != MallExportSalesTypeGroup.end(); it++)
    {
        itST = (it->second).begin();
        GroupSalesType.clear();
        for(; itST != (it->second).end(); itST++)
        {
            GroupSalesType.push_back(itST->first);
        }
        STGManager.AssignItemsToSalesTypeGroup(it->first, GroupSalesType);
    }
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::RemoveTypefromGroup(int TypeKey)
{
    RemoveFromGroup(TypeKey, selectedGroup);
    DisplayItemsInGroups(selectedGroup);
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::RemoveFromGroup(int TypeKey, int GroupKey)
{
    MallExportSalesTypeGroup[GroupKey].erase(TypeKey);
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::clearSalesTypeGroup()
{
    std::map<int, STMap>::iterator itSDG = MallExportSalesTypeGroup.begin();
    for (; itSDG != MallExportSalesTypeGroup.end(); itSDG++)
    {
        clearSalesType(&(itSDG->second));
    }
    MallExportSalesTypeGroup.clear();
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::CleanUpMemory()
{
    clearSalesType();
    clearSalesTypeGroup();
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::AssignAllItems()
{
    if(selectedGroup != 0)
    {
        TMallExportSalesTypeManager STManager;
        std::auto_ptr<TList> STList(new TList());
        STManager.LoadAllSalesType(STList.get());
        TMallExportSalesType* SalesType;

        for (int i = 0; i < STList->Count; i++)
        {
            SalesType = ((TMallExportSalesType*)STList->Items[i]);
            MallExportSalesType[SalesType->DBKey] = SalesType;
            MallExportSalesTypeGroup[selectedGroup].insert( std::pair<int,TMallExportSalesType*>(SalesType->DBKey, MallExportSalesType[SalesType->DBKey]));
        }
        DisplayItemsInGroups(selectedGroup);
    }
};
//---------------------------------------------------------------------------

void TfrmMallExportSalesTypeAssignment::RemoveAllItems()
{
    if(selectedGroup != 0)
    {
        TMallExportSalesTypeManager STManager;
        std::auto_ptr<TList> STList(new TList());
        STManager.LoadAllSalesType(STList.get());
        TMallExportSalesType* SalesType;

        for (int i = 0; i < STList->Count; i++)
        {
            SalesType = ((TMallExportSalesType*)STList->Items[i]);
            MallExportSalesTypeGroup[selectedGroup].erase(SalesType->DBKey);
        }
        DisplayItemsInGroups(selectedGroup);
    }
};
//---------------------------------------------------------------------------
#pragma package(smart_init)
