//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MallSalesTypeAssignment.h"
#include "MMMessageBox.h"
#include "AddSalesType.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"


//---------------------------------------------------------------------------
__fastcall TfrmMallSalesTypeAssignment::TfrmMallSalesTypeAssignment(TComponent* Owner) : TZForm(Owner)
{
    Panel1->Caption = "Items";
    Panel2->Caption = "Sales Types Items";
    Panel12->Caption = "Sales Types";
}
//---------------------------------------------------------------------------
__fastcall TfrmMallSalesTypeAssignment::~TfrmMallSalesTypeAssignment()
{
   //
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::FormShow(TObject *Sender)
{
    FormResize(NULL);
    DisplaySalesTypes();
    DisplayItems();
    assignedItems = TDBSalesTypeAssignment::LoadAssignedItemsBySalesType();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::GroupListMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{
    SelectedGroup = GridButton->Tag;
    DisplayAssignedItemBySalesType(SelectedGroup);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::MembersGridMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    if(SelectedGroup !=0)
    {
        assignedItems[SelectedGroup].insert(std::make_pair(GridButton->Tag, GridButton->Caption));
    }
    DisplayAssignedItemBySalesType(SelectedGroup);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::GroupMembersMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    RemoveTypefromGroup(GridButton->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnCloseMouseClick(TObject *Sender)
{
    try
    {
        TDBSalesTypeAssignment::SaveAssignedItemsToSalesTYpeGroup(assignedItems);
        Close();
    }
    catch(Exception &E)
    {
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btinGGA_CancelMouseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::FormResize(TObject *Sender)
{
    if (Tag != Screen->Width)
    {
        int Temp = Tag;
        Tag = Screen->Width;
		if ((double)Screen->Width / Screen->Height < 1.4)
		{
			ScaleBy(Screen->Width, Temp);
		}
    }
    Left = (Screen->Width - Width) / 2;
    Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnAssignAllMouseClick(TObject *Sender)
{
   AssignAllItems();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnRemoveAllMouseClick(TObject *Sender)
{
    RemoveAllItems();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnAddSalesTypeMouseClick(TObject *Sender)
{
    std::auto_ptr <TfrmAddSalesType> frmAddSalesType(new TfrmAddSalesType(this));
    frmAddSalesType->Editing = false;
    frmAddSalesType->Caption = "Add Sales Type";
    frmAddSalesType->ShowModal();
    DisplaySalesTypes();
    DisplayAssignedItemBySalesType(SelectedGroup);
    DisableSelectedTypesInGroup();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnEditSalesTypeMouseClick(TObject *Sender)
{
    if (SelectedGroup != 0)
	{
        std::auto_ptr <TfrmAddSalesType> frmAddSalesType(new TfrmAddSalesType(this));
        frmAddSalesType->Editing = true;
        frmAddSalesType->Caption = "Edit Sales Type";
        frmAddSalesType->SalesTypeId = SelectedGroup;
        frmAddSalesType->ShowModal();
        DisplaySalesTypes();
        DisplayAssignedItemBySalesType(SelectedGroup);
        DisableSelectedTypesInGroup();
    }
    else
    {
        MessageBox("Please Select a sales type to edit.", "Select a sales type", MB_ICONINFORMATION + MB_OK);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnDeleteSalesTypeMouseClick(TObject *Sender)
{
    if(SelectedGroup != 0)
    {
        std::map <int, std::map <int, UnicodeString> >::iterator outerit;

        TDBSalesTypeAssignment::DeleteSalesType(SelectedGroup);
        DisplaySalesTypes();
        outerit = assignedItems.find(SelectedGroup);
        if(outerit != assignedItems.end())
        {
           assignedItems.erase(outerit);
        }
        DisplayAssignedItemBySalesType(SelectedGroup);
        DisableSelectedTypesInGroup();
    }
    else
    {
        MessageBox("Please Select a sales type to delete.", "Select a sales type", MB_ICONINFORMATION + MB_OK);
    }
}
//---------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::DisplayItems()
{
    int index = 0;
    std::map<int, UnicodeString> loadedItems = TDBSalesTypeAssignment::LoadAllItems();
    MembersGrid->RowCount = loadedItems.size();

    std::map<int, UnicodeString>::iterator itItems;
    for (itItems = loadedItems.begin(); itItems != loadedItems.end(); ++itItems)
    {
        MembersGrid->Buttons[index][0]->Caption = itItems->second;
        MembersGrid->Buttons[index][0]->Tag = itItems->first;
        index++;
    }
}
//----------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::DisplaySalesTypes()
{
    int index = 0;
    std::map<int, UnicodeString> salesTypes = TDBSalesTypeAssignment::LoadAllSalesTypes();
    std::map<int, UnicodeString>::iterator itSalesTypes;
    GroupList->RowCount= salesTypes.size();
    for (itSalesTypes = salesTypes.begin(); itSalesTypes != salesTypes.end(); ++itSalesTypes)
    {
        GroupList->Buttons[index][0]->Caption = itSalesTypes->second;
        GroupList->Buttons[index][0]->Tag = itSalesTypes->first;
        index++;
    }
}

//----------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::DisplayAssignedItemBySalesType(int salesType)
{
    int index = 0;
    std::map <int, std::map <int, UnicodeString> >::iterator outerit;
    std::map <int, UnicodeString>::iterator innerit;
    GroupMembers->RowCount = 0;
    outerit = assignedItems.find(salesType);
    if(outerit != assignedItems.end())
    {
        GroupMembers->RowCount = assignedItems[salesType].size();
        for (innerit = outerit->second.begin(); innerit != outerit->second.end(); ++innerit){
                GroupMembers->Buttons[index][0]->Caption = innerit->second;
                GroupMembers->Buttons[index][0]->Tag = innerit->first;
                index++;
            }
    }
    DisableSelectedTypesInGroup();
}
void TfrmMallSalesTypeAssignment::DisableSelectedTypesInGroup()
{
   std::map <int, std::map <int, UnicodeString> >::iterator outerit;
   std::map <int, UnicodeString>::iterator innerit;

    for(int i=0; i < MembersGrid->RowCount; i++)
    {
        for(int j = 0; j <  GroupList->RowCount; j++)
        {
            outerit = assignedItems.find(GroupList->Buttons[j][0]->Tag);
            if(outerit != assignedItems.end())
            {
                innerit = outerit->second.find(MembersGrid->Buttons[i][0]->Tag);
                if(innerit != outerit->second.end())
                {
                    MembersGrid->Buttons[i][0]->Enabled = false;
                    break;
                }
                else
                {
                    MembersGrid->Buttons[i][0]->Enabled = true;
                }
            }
        }
    }
}

void TfrmMallSalesTypeAssignment::RemoveTypefromGroup(int itemKey)
{
    std::map <int, std::map <int, UnicodeString> >::iterator outerit;
    std::map <int, UnicodeString>::iterator innerit;
    outerit = assignedItems.find(SelectedGroup);
    if(outerit != assignedItems.end())
    {
        innerit = outerit->second.find(itemKey);
        if(innerit != outerit->second.end())
        {
            outerit->second.erase(innerit);
        }
    }
    DisplayAssignedItemBySalesType(SelectedGroup);
    DisableSelectedTypesInGroup();
}

void TfrmMallSalesTypeAssignment::AssignAllItems()
{
    if(SelectedGroup != 0)
    {
        for(int i=0; i < MembersGrid->RowCount; i++)
        {
            if(!CheckItemAlreadyExists(MembersGrid->Buttons[i][0]->Tag))
            {
                assignedItems[SelectedGroup].insert(std::make_pair(MembersGrid->Buttons[i][0]->Tag, MembersGrid->Buttons[i][0]->Caption));
            }
        }
        DisplayAssignedItemBySalesType(SelectedGroup);
        DisableSelectedTypesInGroup();
    }
    else
    {
        MessageBox("Please Select a sales type to assign items.", "Select a sales type", MB_ICONINFORMATION + MB_OK);
    }
}
void TfrmMallSalesTypeAssignment::RemoveAllItems()
{
    std::map <int, std::map <int, UnicodeString> >::iterator outerit;
    std::map <int, UnicodeString>::iterator innerit;

    if(SelectedGroup != 0)
    {
        outerit = assignedItems.find(SelectedGroup);
        if(outerit != assignedItems.end())
        {
            for(int i=0; i < MembersGrid->RowCount; i++)
            {
                innerit = outerit->second.find(MembersGrid->Buttons[i][0]->Tag);
                if(innerit != outerit->second.end())
                {
                    outerit->second.erase(innerit);
                }
            }
        }
        DisplayAssignedItemBySalesType(SelectedGroup);
        DisableSelectedTypesInGroup();
    }
    else
    {
        MessageBox("Please Select a sales type to remove items.", "Select a sales type", MB_ICONINFORMATION + MB_OK);
    }
}

bool TfrmMallSalesTypeAssignment::CheckItemAlreadyExists(int itemKey)
{
    bool itemFound = false;

    std::map <int, std::map <int, UnicodeString> >::iterator outerit;
    std::map <int, UnicodeString>::iterator innerit;
    for(int j = 0; j <  GroupList->RowCount; j++)
    {
        outerit = assignedItems.find(GroupList->Buttons[j][0]->Tag);
        if(outerit != assignedItems.end())
        {
            innerit = outerit->second.find(itemKey);
            if(innerit != outerit->second.end())
            {
               itemFound = true;
            }
        }
    }
    return itemFound;
}

