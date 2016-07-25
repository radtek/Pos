//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GenericGroupAssignment.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"


//---------------------------------------------------------------------------
__fastcall TfrmGenericGroupAssignment::TfrmGenericGroupAssignment(TComponent* Owner) : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmGenericGroupAssignment::FormShow(TObject *Sender)
{
    FormResize(NULL);
    DisplayGroups();
    DisplayTypes();
}

//---------------------------------------------------------------------------

void __fastcall TfrmGenericGroupAssignment::GroupListMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{
    GroupSelected(GridButton);
}
//---------------------------------------------------------------------------


void __fastcall TfrmGenericGroupAssignment::MembersGridMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    TypeSelected(GridButton);
}
//---------------------------------------------------------------------------


void __fastcall TfrmGenericGroupAssignment::GroupMembersMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    RemoveTypefromGroup(GridButton->Tag);
}
//---------------------------------------------------------------------------

void __fastcall TfrmGenericGroupAssignment::btnCloseMouseClick(TObject *Sender)
{
    SaveAssignment();
    CleanUpMemory();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGenericGroupAssignment::btinGGA_CancelMouseClick(TObject *Sender)
{
    CleanUpMemory();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGenericGroupAssignment::FormResize(TObject *Sender)
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

void __fastcall TfrmGenericGroupAssignment::btnAssignAllMouseClick(TObject *Sender)

{
    AssignAllItems();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGenericGroupAssignment::btnRemoveAllMouseClick(TObject *Sender)

{
    RemoveAllItems();
}
//---------------------------------------------------------------------------

