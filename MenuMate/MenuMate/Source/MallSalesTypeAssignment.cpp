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
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::GroupListMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{
    SelectedGroup = GridButton->Tag;
    //GroupSelected(GridButton);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::MembersGridMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    if(SelectedGroup !=0)
    {
        std::vector<int> itemsKeys;
        itemsKeys.push_back(GridButton->Tag);
//todo
        //assignedItems.insert( std::map<int, std::vector<int> >(SelectedGroup, itemsKeys));

    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::GroupMembersMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    //RemoveTypefromGroup(GridButton->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnCloseMouseClick(TObject *Sender)
{
    try
    {
       // SaveAssignment();
       // CleanUpMemory();
        Close();
    }
    catch(Exception &E)
    {
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btinGGA_CancelMouseClick(TObject *Sender)
{
   // CleanUpMemory();
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
   // AssignAllItems();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnRemoveAllMouseClick(TObject *Sender)
{
   // RemoveAllItems();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnAddSalesTypeMouseClick(TObject *Sender)
{
    std::auto_ptr <TfrmAddSalesType> frmAddSalesType(new TfrmAddSalesType(this));
    frmAddSalesType->Editing = false;
    frmAddSalesType->Caption = "Add Sales Type";
    frmAddSalesType->ShowModal();
    DisplaySalesTypes();
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
        TDBSalesTypeAssignment::DeleteSalesType(SelectedGroup);
        DisplaySalesTypes();
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

