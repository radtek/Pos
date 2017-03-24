//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MallSalesTypeAssignment.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"


//---------------------------------------------------------------------------
__fastcall TfrmMallSalesTypeAssignment::TfrmMallSalesTypeAssignment(TComponent* Owner) : TZForm(Owner)
{
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
   // DisplayGroups();
   // DisplayTypes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::GroupListMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{
    //GroupSelected(GridButton);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::MembersGridMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    //TypeSelected(GridButton);
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
  //  RemoveAllItems();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnEditSalesTypeMouseClick(TObject *Sender)
{
   // RemoveAllItems();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallSalesTypeAssignment::btnDeleteSalesTypeMouseClick(TObject *Sender)
{
    //RemoveAllItems();
}
//---------------------------------------------------------------------------

