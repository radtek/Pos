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
    panel1->Caption = "Items";
    panel2->Caption = "Sales Types Items";
    panel12->Caption = "Sales Types";
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
    assignedItemsDBState = TDBSalesTypeAssignment::LoadAssignedItemsBySalesType();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::salesTypeListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    SelectedSalesType = GridButton->Tag;
    DisplayAssignedItemBySalesType();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::itemListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    if(SelectedSalesType)
    {
        assignedItemsDBState[SelectedSalesType].insert(std::make_pair(GridButton->Tag, GridButton->Caption));
    }
    DisplayAssignedItemBySalesType();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnOkMouseClick(TObject *Sender)
{
    try
    {
        TDBSalesTypeAssignment::SaveItemRelationWithSalesType(assignedRemovedItemsBySalesType);
        Close();
    }
    catch(Exception &E)
    {
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnCancelMouseClick(TObject *Sender)
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
    DisplayAssignedItemBySalesType();
    DisableSelectedTypesInGroup();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnEditSalesTypeMouseClick(TObject *Sender)
{
    if (SelectedSalesType)
	{
        std::auto_ptr <TfrmAddSalesType> frmAddSalesType(new TfrmAddSalesType(this));
        frmAddSalesType->Editing = true;
        frmAddSalesType->Caption = "Edit Sales Type";
        frmAddSalesType->SalesTypeId = SelectedSalesType;
        frmAddSalesType->ShowModal();
        DisplaySalesTypes();
        DisplayAssignedItemBySalesType();
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
    if(SelectedSalesType)
    {
        std::map <int, std::map <int, UnicodeString> >::iterator outerit;

        TDBSalesTypeAssignment::DeleteSalesType(SelectedSalesType);
        DisplaySalesTypes();
        outerit = assignedItemsDBState.find(SelectedSalesType);
        if(outerit != assignedItemsDBState.end())
        {
           assignedItemsDBState.erase(outerit);
        }
        DisplayAssignedItemBySalesType();
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
    itemList->RowCount = loadedItems.size();

    std::map<int, UnicodeString>::iterator itItems;
    for (itItems = loadedItems.begin(); itItems != loadedItems.end(); ++itItems)
    {
        itemList->Buttons[index][0]->Caption = itItems->second;
        itemList->Buttons[index][0]->Tag = itItems->first;
        index++;
    }
}
//----------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::DisplaySalesTypes()
{
    int index = 0;
    std::map<int, UnicodeString> salesTypes = TDBSalesTypeAssignment::LoadAllSalesTypes();
    std::map<int, UnicodeString>::iterator itSalesTypes;
    salesTypeList->RowCount= salesTypes.size();
    for (itSalesTypes = salesTypes.begin(); itSalesTypes != salesTypes.end(); ++itSalesTypes)
    {
        salesTypeList->Buttons[index][0]->Caption = itSalesTypes->second;
        salesTypeList->Buttons[index][0]->Tag = itSalesTypes->first;
        index++;
    }
}
//----------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::DisplayAssignedItemBySalesType()
{
    int index = 0;
    std::map <int, std::map <int, UnicodeString> >::iterator outerit;
    std::map <int, UnicodeString>::iterator innerit;
    assignedItemsBySalesTypeList->RowCount = 0;
    outerit = assignedItemsDBState.find(SelectedSalesType);
    if(outerit != assignedItemsDBState.end())
    {
        assignedItemsBySalesTypeList->RowCount = assignedItemsDBState[SelectedSalesType].size();
        for (innerit = outerit->second.begin(); innerit != outerit->second.end(); ++innerit){
                assignedItemsBySalesTypeList->Buttons[index][0]->Caption = innerit->second;
                assignedItemsBySalesTypeList->Buttons[index][0]->Tag = innerit->first;
                index++;
            }
    }
    DisableSelectedTypesInGroup();
}
//---------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::DisableSelectedTypesInGroup()
{
   std::map <int, std::map <int, UnicodeString> >::iterator outerit;
   std::map <int, UnicodeString>::iterator innerit;

    for(int i=0; i < itemList->RowCount; i++)
    {
        for(int j = 0; j <  salesTypeList->RowCount; j++)
        {
            outerit = assignedItemsDBState.find(salesTypeList->Buttons[j][0]->Tag);
            if(outerit != assignedItemsDBState.end())
            {
                innerit = outerit->second.find(itemList->Buttons[i][0]->Tag);
                if(innerit != outerit->second.end())
                {
                    itemList->Buttons[i][0]->Enabled = false;
                    break;
                }
                else
                {
                    itemList->Buttons[i][0]->Enabled = true;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::RemoveItemsFromAssignedItemsBySalesTypeList(int itemKey)
{
    std::map <int, std::map <int, UnicodeString> >::iterator outerit;
    std::map <int, UnicodeString>::iterator innerit;
    outerit = assignedItemsDBState.find(SelectedSalesType);
    if(outerit != assignedItemsDBState.end())
    {
        innerit = outerit->second.find(itemKey);
        if(innerit != outerit->second.end())
        {
            outerit->second.erase(innerit);
        }
    }
    DisplayAssignedItemBySalesType();
    DisableSelectedTypesInGroup();
}
//---------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::AssignAllItems()
{
    if(SelectedSalesType)
    {
        for(int i=0; i < itemList->RowCount; i++)
        {
//            if(!CheckItemAlreadyExists(itemList->Buttons[i][0]->Tag))
//            {
//                assignedItems[SelectedSalesType].insert(std::make_pair(itemList->Buttons[i][0]->Tag, itemList->Buttons[i][0]->Caption));
//            }
        }
        DisplayAssignedItemBySalesType();
        DisableSelectedTypesInGroup();
    }
    else
    {
        MessageBox("Please Select a sales type to assign items.", "Select a sales type", MB_ICONINFORMATION + MB_OK);
    }
}
//-------------------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::RemoveAllItems()
{
    std::map <int, std::map <int, UnicodeString> >::iterator outerit;
    std::map <int, UnicodeString>::iterator innerit;

    if(SelectedSalesType)
    {
        outerit = assignedItemsDBState.find(SelectedSalesType);
        if(outerit != assignedItemsDBState.end())
        {
            for(int i=0; i < itemList->RowCount; i++)
            {
                innerit = outerit->second.find(itemList->Buttons[i][0]->Tag);
                if(innerit != outerit->second.end())
                {
                    outerit->second.erase(innerit);
                }
            }
        }
        DisplayAssignedItemBySalesType();
        DisableSelectedTypesInGroup();
    }
    else
    {
        MessageBox("Please Select a sales type to remove items.", "Select a sales type", MB_ICONINFORMATION + MB_OK);
    }
}
//-------------------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::assignedItemsBySalesTypeListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    //todo according to new design
}

