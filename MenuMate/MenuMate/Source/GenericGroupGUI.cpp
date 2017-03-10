//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GenericGroupGUI.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmGenericGroupGUI::TfrmGenericGroupGUI(TComponent* Owner)
    : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
__fastcall TfrmGenericGroupGUI::~TfrmGenericGroupGUI()
{
   //
}
//---------------------------------------------------------------------------
void __fastcall TfrmGenericGroupGUI::btnCloseMouseClick(TObject *Sender)
{
	ModalResult = mrOk;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmGenericGroupGUI::btnAddMouseClick(TObject *Sender)
{
    AddGroup();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGenericGroupGUI::btnEditMouseClick(TObject *Sender)
{
    EditGroup();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGenericGroupGUI::btnDeleteMouseClick(TObject *Sender)
{
    DeleteGroup();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGenericGroupGUI::btnAssignMouseClick(TObject *Sender)
{
    AssignGroups();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGenericGroupGUI::FormShow(TObject *Sender)
{
    FormResize(NULL);
    DisplayGroups();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGenericGroupGUI::FormResize(TObject *Sender)
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
void __fastcall TfrmGenericGroupGUI::tgridGroupsClick(TObject *Sender)
{
    ItemSelected();
}
//---------------------------------------------------------------------------
void TfrmGenericGroupGUI::ButtonStatus(bool Status)
{
    btnEdit->Enabled = Status;
    btnDelete->Enabled = Status;
}
//---------------------------------------------------------------------------


