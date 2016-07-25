//---------------------------------------------------------------------------
#pragma hdrstop

#include "PaymentTypeGroupAssignment.h"
#include "PaymentTypesManager.h"
#include "PaymentTypeGroupsManager.h"

//---------------------------------------------------------------------------
__fastcall TfrmPaymentTypeGroupAssignment::TfrmPaymentTypeGroupAssignment(TComponent* Owner) : TfrmGenericGroupAssignment(Owner)
{
    selectedGroup = 0;
    Panel1->Caption = "Payment Type";
    Panel2->Caption = "Payment Group Types";
    Panel12->Caption = "Payment Groups";
}
//---------------------------------------------------------------------------

    void TfrmPaymentTypeGroupAssignment::DisplayGroups(){
        TPaymentTypeGroupsManager PTManager;
        std::auto_ptr<TList> PTGList(new TList());
        PTManager.LoadAllPaymentTypeGroups(PTGList.get());
        GroupList->RowCount=PTGList->Count;
        for (int i = 0; i < PTGList->Count; i++)
	    {
        	GroupList->Buttons[i][0]->Caption = ((TPaymentTypeGroup*)PTGList->Items[i])->Name;
            GroupList->Buttons[i][0]->Tag = ((TPaymentTypeGroup*)PTGList->Items[i])->DBKey;
	    }
    };

    void TfrmPaymentTypeGroupAssignment::DisplayTypes(){
        TPaymentTypesManager PTManager;
        std::auto_ptr<TList> PTList(new TList());
        PTManager.LoadAllPaymentTypes(PTList.get());

        MembersGrid->RowCount=PTList->Count;
        TPaymentType* PaymentType;
        clearPaymentTypes();
        for (int i = 0; i < PTList->Count; i++)
	    {
            PaymentType = ((TPaymentType*)PTList->Items[i]);
        	MembersGrid->Buttons[i][0]->Caption = PaymentType->Name;
            MembersGrid->Buttons[i][0]->Tag = PaymentType->DBKey;
            PaymentTypes[PaymentType->DBKey] = PaymentType;
	    }
    };

    void TfrmPaymentTypeGroupAssignment::DisplayItemsInGroups(int GroupKey){
        TPaymentTypeGroupsManager PTManager;
        std::auto_ptr<TList> ATBGList(new TList());
        if(PaymentGroups.find(GroupKey) == PaymentGroups.end())
        {
            PTManager.LoadAssignedTypesByGroup(GroupKey, ATBGList.get());
            storePaymentTypeGroupMapInMemory(GroupKey, ATBGList.get());
        }
        displayItemsInGroups(GroupKey);
        disableSelectedTypesInGroup(GroupKey);
    };

    void  TfrmPaymentTypeGroupAssignment::displayItemsInGroups(int GroupKey)
    {
        PTMap PaymentTypeMap = PaymentGroups[GroupKey];
        GroupMembers->RowCount=PaymentTypeMap.size();

        PTMap::iterator it = PaymentTypeMap.begin();
        int i = 0;
        for (; it != PaymentTypeMap.end(); it++)
	    {
             GroupMembers->Buttons[i][0]->Caption = (it->second)->Name;
             GroupMembers->Buttons[i][0]->Tag = (it->second)->DBKey;
             i++;
        }
    };

    void TfrmPaymentTypeGroupAssignment::disableSelectedTypesInGroup(int GroupKey){
        for(int i=0; i < MembersGrid->RowCount; i++){
            int checkTypeInGroup = MembersGrid->Buttons[i][0]->Tag;
            if(PaymentGroups[GroupKey].find(checkTypeInGroup) != PaymentGroups[GroupKey].end()){
                MembersGrid->Buttons[i][0]->Enabled = false;
            }else{
                MembersGrid->Buttons[i][0]->Enabled = true;
            }
        }
    }

    void TfrmPaymentTypeGroupAssignment::storePaymentTypeGroupMapInMemory(int GroupKey, TList* TypesList)
    {
        TPaymentType* PT;
        for(int i=0; i<TypesList->Count; i++)
        {
            PT = (TPaymentType*) TypesList->Items[i];
            PaymentGroups[GroupKey].insert( std::pair<int,TPaymentType*>(PT->DBKey,PT) );
        }
        int test = (PaymentGroups[GroupKey]).size();
    }

    void TfrmPaymentTypeGroupAssignment::SaveAssignment(){
        std::map<int, PTMap>::iterator it = PaymentGroups.begin();
        PTMap::iterator itPT;
        std::vector<int> GroupPaymentTypes;
        TPaymentTypeGroupsManager PTManager;
        for(; it != PaymentGroups.end(); it++){
            itPT = (it->second).begin();
            GroupPaymentTypes.clear();
            for(; itPT != (it->second).end(); itPT++){
                GroupPaymentTypes.push_back(itPT->first);
            }
            PTManager.AssignTypesToPaymentGroup(it->first, GroupPaymentTypes);
        }
    };

    void TfrmPaymentTypeGroupAssignment::AddToGroup(int selectedGroupKey, int TypeKey){
        PaymentGroups[selectedGroupKey].insert( std::pair<int,TPaymentType*>(TypeKey, PaymentTypes[TypeKey]) );
    };

    void TfrmPaymentTypeGroupAssignment::RemoveFromGroup(int TypeKey, int GroupKey){
        PaymentGroups[GroupKey].erase(TypeKey);
    };

    void TfrmPaymentTypeGroupAssignment::GroupSelected(TGridButton *GridButton){
        selectedGroup = GridButton->Tag;
        DisplayItemsInGroups(selectedGroup);
    }

    void TfrmPaymentTypeGroupAssignment::TypeSelected(TGridButton *GridButton){
        if(selectedGroup != 0){
            AddToGroup(selectedGroup, GridButton->Tag);
            DisplayItemsInGroups(selectedGroup);
        }
    }

    void TfrmPaymentTypeGroupAssignment::RemoveTypefromGroup(int TypeKey){
        RemoveFromGroup(TypeKey, selectedGroup);
        DisplayItemsInGroups(selectedGroup);
    }
//---------------------------------------------------------------------------
    void TfrmPaymentTypeGroupAssignment::CleanUpMemory(){
        clearPaymentTypes();
        clearPaymentGroups();
    }
//---------------------------------------------------------------------------
    void TfrmPaymentTypeGroupAssignment::clearPaymentTypes(){
        clearPaymentTypes(&PaymentTypes);
    }
//---------------------------------------------------------------------------
    void TfrmPaymentTypeGroupAssignment::clearPaymentGroups(){
        std::map<int, PTMap>::iterator itPG = PaymentGroups.begin();
        for (; itPG != PaymentGroups.end(); itPG++)
	    {
            clearPaymentTypes(&(itPG->second));
        }
        PaymentGroups.clear();
    }
//---------------------------------------------------------------------------
    void TfrmPaymentTypeGroupAssignment::clearPaymentTypes(PTMap* PaymentTypeMap){
        PTMap::iterator it = PaymentTypeMap->begin();
        for (; it != PaymentTypeMap->end(); it++)
	    {
            delete it->second;
        }
        PaymentTypeMap->clear();
    }
//---------------------------------------------------------------------------


#pragma }package(smart_init)
