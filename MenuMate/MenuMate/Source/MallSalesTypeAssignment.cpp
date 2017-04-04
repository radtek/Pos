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
    for(std::map <int, std::map <int, UnicodeString> >::iterator outerit = assignedItemsDBState.begin(); outerit != assignedItemsDBState.end(); ++outerit)
    {
        for(std::map <int, UnicodeString>::iterator innerit = outerit->second.begin(); innerit != outerit->second.end(); ++innerit)
        {
            alreadyAssignedItems.insert(innerit->first);
        }
    }
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
        TItemDetails itemDetails;
        itemDetails.ItemStatus = eAssigned;
        itemDetails.ItemName = GridButton->Caption;
        assignedRemovedItemsBySalesType[SelectedSalesType].insert(std::make_pair(GridButton->Tag, itemDetails));
        alreadyAssignedItems.insert(GridButton->Tag);
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

    currentItemRelationsWithSalesType = assignedItemsDBState;
    std::map<int, std::map<int, UnicodeString> >::iterator currentOuterit;
    std::map <int, UnicodeString>::iterator currentInnerit;

    int index = 0;

    std::map <int, std::map <int, TItemDetails> >::iterator outerit;
    std::map <int, TItemDetails>::iterator innerit;
    assignedItemsBySalesTypeList->RowCount = 0;

    outerit = assignedRemovedItemsBySalesType.find(SelectedSalesType);


    if(outerit != assignedRemovedItemsBySalesType.end())
    {
        //first loop to iterate items list from assignedRemovedItemsBySalesType i.e second map..
        for(innerit = outerit->second.begin(); innerit != outerit->second.end(); ++innerit)
        {
            currentOuterit = currentItemRelationsWithSalesType.find(SelectedSalesType);
            if(currentOuterit != currentItemRelationsWithSalesType.end())
            {
                 //second loop to iterate items list from currentItemRelationsWithSalesType to check items already existed in curreent map or not..
                for(currentInnerit = currentOuterit->second.begin(); currentInnerit != currentOuterit->second.end(); ++currentInnerit)
                {
                     if(innerit->first == currentInnerit->first)
                     {
                         currentOuterit->second.erase(currentInnerit);
                     }
                     else
                     {
                         // if value is not present in current map then insert items details in current map to display item details
                         if(innerit->second.ItemStatus != eRemoved)
                         {
                            currentItemRelationsWithSalesType[SelectedSalesType].insert(std::make_pair(innerit->first, innerit->second.ItemName));
                         }
                     }
                }
            }
        }
    }

    // get all item details from currentItemRelationsWithSalesType details to show items details in assignedItemsBySalesTypeList list
    // assign all  item details from  currentItemRelationsWithSalesType map to  assignedItemsBySalesTypeList list
    currentOuterit = currentItemRelationsWithSalesType.find(SelectedSalesType);
    assignedItemsBySalesTypeList->RowCount = currentItemRelationsWithSalesType[SelectedSalesType].size();
    for (currentInnerit = currentOuterit->second.begin(); currentInnerit != currentOuterit->second.end(); ++currentInnerit){
            assignedItemsBySalesTypeList->Buttons[index][0]->Caption = currentInnerit->second;
            assignedItemsBySalesTypeList->Buttons[index][0]->Tag = currentInnerit->first;
            index++;
        }
    EnableDisableItemList();
}
//---------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::EnableDisableItemList()
{

    std::set <int>::iterator itemKeys;
    for(int i=0; i < itemList->RowCount; i++)
    {
         // if item keys find in the collections of items then disable item button from itemlist
         itemKeys = alreadyAssignedItems.find(itemList->Buttons[i][0]->Tag);
         if(itemKeys != alreadyAssignedItems.end())
         {
             itemList->Buttons[i][0]->Enabled = false;
         }
    }
}
//---------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::RemoveItemsFromAssignedItemsBySalesTypeList(int itemKey)
{
    std::map <int, std::map <int, TItemDetails> >::iterator outerit;
    std::map <int, TItemDetails>::iterator innerit;

    std::set <int>::iterator collectionOfItemKeys;
    outerit = assignedRemovedItemsBySalesType.find(SelectedSalesType);
    if(outerit != assignedRemovedItemsBySalesType.end())
    {
        // if sales type find in assignedRemovedItemsBySalesType map then search for item
        innerit = outerit->second.find(itemKey);
        if(innerit != outerit->second.end())
        {
            // if item found in the list of items then erase that item from the list
            outerit->second.erase(innerit);
        }
        else
        {
            /// if item not found in the list then insert item
            TItemDetails itemDetails;
            itemDetails.ItemStatus = eRemoved;
            assignedRemovedItemsBySalesType[SelectedSalesType].insert(std::make_pair(itemKey, itemDetails));
        }
        collectionOfItemKeys = alreadyAssignedItems.find(itemKey);
        if(collectionOfItemKeys != alreadyAssignedItems.end())
        {
             alreadyAssignedItems.erase(collectionOfItemKeys);
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
            if(itemList->Buttons[i][0]->Enabled)
            {
                TItemDetails itemDetails;
                itemDetails.ItemStatus = eAssigned;
                itemDetails.ItemName = itemList->Buttons[i][0]->Caption;
                assignedRemovedItemsBySalesType[SelectedSalesType].insert(std::make_pair(itemList->Buttons[i][0]->Tag, itemDetails));
            }
        }
        DisplayAssignedItemBySalesType();
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
        //DisableSelectedTypesInGroup();
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

