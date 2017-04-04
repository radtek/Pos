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

    //insert items into set which are assigned to any sales type ie; used items. Now it can't be assigned to others.
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
        std::map <int, std::map <int, TItemDetails> >::iterator outerit;
        std::map <int, TItemDetails>::iterator innerit;

        //finding selected sales type in assignedRemovedItemsBySalesType map
        outerit = assignedRemovedItemsBySalesType.find(SelectedSalesType);

        //if selected sales type exist..
        if(outerit != assignedRemovedItemsBySalesType.end())
        {
            innerit = outerit->second.find(GridButton->Tag);

            //If already exist then remove it because now item is in initial state..
            if(innerit != outerit->second.end())
            {
                outerit->second.erase(innerit);
            }
            else
            {
                ///insert all Items into assignedRemovedItemsBySalesType map..by adding status as removed..
                InsertIntoAssignedRemovedItemsBySalesTypeMap(GridButton->Tag, GridButton->Caption, eAssigned);
            }
        }
        else
        {
            ///insert all Items into assignedRemovedItemsBySalesType map..by adding status as removed..
            InsertIntoAssignedRemovedItemsBySalesTypeMap(GridButton->Tag, GridButton->Caption, eAssigned);
        }

        //insert to set because this item is now assigned..
        alreadyAssignedItems.insert(GridButton->Tag);

        //Display Sales Type According to selected sales type
        DisplayAssignedItemBySalesType();
    }
    else
    {
        MessageBox("Please Select a sales type to assign.", "Select a sales type", MB_ICONINFORMATION + MB_OK);
    }

}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnOkMouseClick(TObject *Sender)
{
    try
    {
        //Check whether any changed performed
        if(assignedRemovedItemsBySalesType.size())
            TDBSalesTypeAssignment::SaveItemRelationWithSalesType(assignedRemovedItemsBySalesType);

        Close();
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
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
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnEditSalesTypeMouseClick(TObject *Sender)
{
    if (SelectedSalesType)
	{
        //call same form of adding sales type by making editing variable as true. now it will be used as edit screen.
        std::auto_ptr <TfrmAddSalesType> frmAddSalesType(new TfrmAddSalesType(this));
        frmAddSalesType->Editing = true;
        frmAddSalesType->Caption = "Edit Sales Type";
        frmAddSalesType->SalesTypeId = SelectedSalesType;
        frmAddSalesType->ShowModal();
        DisplaySalesTypes();
        DisplayAssignedItemBySalesType();
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
        std::map <int, std::map <int, TItemDetails> >::iterator deletaIt;
        std::map <int, std::map <int, UnicodeString> >::iterator dbStateIt;

        TDBSalesTypeAssignment::DeleteSalesType(SelectedSalesType);
        DisplaySalesTypes();

        //find whether select sales type exist in assignedRemovedItemsBySalesType (delta) map.
        //if exist then erase it because sales type is being deleted.
        deletaIt = assignedRemovedItemsBySalesType.find(SelectedSalesType);
        if(deletaIt != assignedRemovedItemsBySalesType.end())
        {
            //read all items from map at selected sales type and remove them from set it means now they can be assigned to others sales type
            for(std::map <int, TItemDetails>::iterator innerit = deletaIt->second.begin(); innerit != deletaIt->second.end(); ++innerit)
            {
                alreadyAssignedItems.erase(innerit->first);
            }
            assignedRemovedItemsBySalesType.erase(deletaIt);
        }

        //find whether select sales type exist in assignedItemsDBState map.
        //if exist then erase it because sales type is being deleted.
        dbStateIt = assignedItemsDBState.find(SelectedSalesType);

        if(dbStateIt != assignedItemsDBState.end())
        {
            //read all items from map at selected sales type and remove them from set it means now they can be assigned to others sales type
            for(std::map <int, UnicodeString>::iterator innerit = dbStateIt->second.begin(); innerit != dbStateIt->second.end(); ++innerit)
            {
                alreadyAssignedItems.erase(innerit->first);
            }

           assignedItemsDBState.erase(dbStateIt);
        }

        //Remove from map which has DB state..
        DisplayAssignedItemBySalesType();
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
    assignedItemsBySalesTypeList->RowCount = 0;

    currentItemRelationsWithSalesType = assignedItemsDBState;
    std::map<int, std::map<int, UnicodeString> >::iterator currentOuterit;
    std::map <int, UnicodeString>::iterator currentInnerit;

    std::map <int, std::map <int, TItemDetails> >::iterator assignedouterit;
    std::map <int, TItemDetails>::iterator assignedinnerit;


    //find sales types in assignedRemovedItemsBySalesType maps.i.e. map2....
    assignedouterit = assignedRemovedItemsBySalesType.find(SelectedSalesType);

    // if sales types found in assignedRemovedItemsBySalesType maps
    if(assignedouterit != assignedRemovedItemsBySalesType.end())
    {
        //find sales types in currentItemRelationsWithSalesType maps ...
        currentOuterit = currentItemRelationsWithSalesType.find(SelectedSalesType);
        ///if sale stype found
        if(currentOuterit != currentItemRelationsWithSalesType.end())
        {
            //first loop to iterate items list from assignedRemovedItemsBySalesType i.e second map..
            for(assignedinnerit = assignedouterit->second.begin(); assignedinnerit != assignedouterit->second.end(); ++assignedinnerit)
            {
                 currentInnerit = currentOuterit->second.find(assignedinnerit->first);
                 if(currentInnerit != currentOuterit->second.end())
                 {
                     currentOuterit->second.erase(currentInnerit);
                 }
                 else
                 {
                     currentItemRelationsWithSalesType[SelectedSalesType].insert(std::make_pair(assignedinnerit->first, assignedinnerit->second.ItemName));
                 }
            }
       }
       else
       {
            // if sales types not found in currentItemRelationsWithSalesType maps then insert sales types and item details in currentItemRelationsWithSalesType maps
            for(assignedinnerit = assignedouterit->second.begin(); assignedinnerit != assignedouterit->second.end(); ++assignedinnerit)
            {
               currentItemRelationsWithSalesType[SelectedSalesType].insert(std::make_pair(assignedinnerit->first, assignedinnerit->second.ItemName));
            }
       }
    }

    // get all item details from currentItemRelationsWithSalesType details to show items details in assignedItemsBySalesTypeList list
    // assign all  item details from  currentItemRelationsWithSalesType map to  assignedItemsBySalesTypeList list
    currentOuterit = currentItemRelationsWithSalesType.find(SelectedSalesType);
    if(currentOuterit != currentItemRelationsWithSalesType.end())
    {
        assignedItemsBySalesTypeList->RowCount = currentItemRelationsWithSalesType[SelectedSalesType].size();
        for (currentInnerit = currentOuterit->second.begin(); currentInnerit != currentOuterit->second.end(); ++currentInnerit)
        {
                assignedItemsBySalesTypeList->Buttons[index][0]->Caption = currentInnerit->second;
                assignedItemsBySalesTypeList->Buttons[index][0]->Tag = currentInnerit->first;
                index++;
        }
    }
    // enable disable item list grid button..
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
         else
         {
             itemList->Buttons[i][0]->Enabled = true;
         }
    }
}
//---------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::AssignAllItems()
{
    if(SelectedSalesType)
    {
        std::map <int, std::map <int, TItemDetails> >::iterator outerit;
        std::map <int, TItemDetails>::iterator innerit;

        //finding selected sales type in assignedRemovedItemsBySalesType map
        outerit = assignedRemovedItemsBySalesType.find(SelectedSalesType);

        //if selected sales type exist..
        if(outerit != assignedRemovedItemsBySalesType.end())
        {
            for(int itemIndex =0; itemIndex < itemList->RowCount; ++itemIndex)
            {
                if(itemList->Buttons[itemIndex][0]->Enabled)
                {
                    //find Item at itemIndex position with tag key as item key in assignedRemovedItemsBySalesType because it may also possible that item with state
                    //inserted  already exist or not
                    innerit = outerit->second.find(itemList->Buttons[itemIndex][0]->Tag);

                    //If already exist then remove it because now item is in initial state..
                    if(innerit != outerit->second.end())
                    {
                        outerit->second.erase(innerit);
                        RemoveFromSet(itemList->Buttons[itemIndex][0]->Tag);
                    }
                    else
                    {
                       ///insert all Items into assignedRemovedItemsBySalesType map..by adding status as removed..
                        InsertIntoAssignedRemovedItemsBySalesTypeMap(itemList->Buttons[itemIndex][0]->Tag, itemList->Buttons[itemIndex][0]->Caption, eAssigned);
                        alreadyAssignedItems.insert(itemList->Buttons[itemIndex][0]->Tag);
                    }
                }
            }
        }
        else
        {
            for(int itemIndex = 0; itemIndex < itemList->RowCount; itemIndex++)
            {
                if(itemList->Buttons[itemIndex][0]->Enabled)
                {
                    ///insert all Items into assignedRemovedItemsBySalesType map..by adding status as removed..
                    InsertIntoAssignedRemovedItemsBySalesTypeMap(itemList->Buttons[itemIndex][0]->Tag,
                                                                        itemList->Buttons[itemIndex][0]->Caption, eAssigned);
                    alreadyAssignedItems.insert(itemList->Buttons[itemIndex][0]->Tag);
                }
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
    //Check wherther any sales type selected
    if(SelectedSalesType)
    {
        std::map <int, std::map <int, TItemDetails> >::iterator outerit;
        std::map <int, TItemDetails>::iterator innerit;

        //finding selected sales type in assignedRemovedItemsBySalesType map
        outerit = assignedRemovedItemsBySalesType.find(SelectedSalesType);

        //if selected sales type exist..
        if(outerit != assignedRemovedItemsBySalesType.end())
        {
            //Loop for all items which are being removed from assignedItemsBySalesTypeList
            for(int itemIndex = 0; itemIndex < assignedItemsBySalesTypeList->RowCount; ++itemIndex)
            {
                //find Item at itemIndex position with tag key as item key in assignedRemovedItemsBySalesType because it may also possible that item with state
                //inserted  already exist or not
                innerit = outerit->second.find(assignedItemsBySalesTypeList->Buttons[itemIndex][0]->Tag);

                //If already exist then remove it because now item is in initial state..
                if(innerit != outerit->second.end())
                {
                    outerit->second.erase(innerit);
                }
                else
                {
                    ///insert all Items into assignedRemovedItemsBySalesType map..by adding status as removed..
                    InsertIntoAssignedRemovedItemsBySalesTypeMap(assignedItemsBySalesTypeList->Buttons[itemIndex][0]->Tag,
                                                                    assignedItemsBySalesTypeList->Buttons[itemIndex][0]->Caption, eRemoved);
                }

                RemoveFromSet(assignedItemsBySalesTypeList->Buttons[itemIndex][0]->Tag);
            }
        }
        else
        {
            for(int itemIndex = 0; itemIndex < assignedItemsBySalesTypeList->RowCount; ++itemIndex)
            {
                ///insert all Items into assignedRemovedItemsBySalesType map..by adding status as removed..
                InsertIntoAssignedRemovedItemsBySalesTypeMap(assignedItemsBySalesTypeList->Buttons[itemIndex][0]->Tag,
                                                                    assignedItemsBySalesTypeList->Buttons[itemIndex][0]->Caption, eRemoved);

                RemoveFromSet(assignedItemsBySalesTypeList->Buttons[itemIndex][0]->Tag);
            }
        }

        ///Display all item's current state according to selected sales type.
        DisplayAssignedItemBySalesType();
    }
    else
    {
        MessageBox("Please Select a sales type to remove items.", "Select a sales type", MB_ICONINFORMATION + MB_OK);
    }
}
//-------------------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::assignedItemsBySalesTypeListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    if(SelectedSalesType)
    {
        std::map <int, std::map <int, TItemDetails> >::iterator outerit;
        std::map <int, TItemDetails>::iterator innerit;

        //finding selected sales type in assignedRemovedItemsBySalesType map
        outerit = assignedRemovedItemsBySalesType.find(SelectedSalesType);

        //if selected sales type exist..
        if(outerit != assignedRemovedItemsBySalesType.end())
        {
            innerit = outerit->second.find(GridButton->Tag);

            //If already exist then remove it because now item is in initial state..
            if(innerit != outerit->second.end())
            {
                outerit->second.erase(innerit);
            }
            else
            {
                ///insert all Items into assignedRemovedItemsBySalesType map..by adding status as removed..
                InsertIntoAssignedRemovedItemsBySalesTypeMap(GridButton->Tag, GridButton->Caption, eRemoved);
            }
        }
        else
        {
            ///insert all Items into assignedRemovedItemsBySalesType map..by adding status as removed..
            InsertIntoAssignedRemovedItemsBySalesTypeMap(GridButton->Tag, GridButton->Caption, eRemoved);
        }
        //Erase from set
        RemoveFromSet(GridButton->Tag);

        ///Display all item's current state according to selected sales type.
        DisplayAssignedItemBySalesType();
    }
}
//--------------------------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::InsertIntoAssignedRemovedItemsBySalesTypeMap(int itemKey, UnicodeString itemName, int itemStatus)
{
    TItemDetails itemDetails;
    itemDetails.ItemName = itemName;
    itemDetails.ItemStatus = itemStatus;
    assignedRemovedItemsBySalesType[SelectedSalesType].insert(std::make_pair(itemKey, itemDetails));
}
//--------------------------------------------------------------------------------------------------
void TfrmMallSalesTypeAssignment::RemoveFromSet(int itemId)
{
    std::set<int>::iterator it;

    //find and erase element from set because now it is freed..
    it = alreadyAssignedItems.find(itemId);
    if(it != alreadyAssignedItems.end())
    {
        alreadyAssignedItems.erase(it);
    }
}


