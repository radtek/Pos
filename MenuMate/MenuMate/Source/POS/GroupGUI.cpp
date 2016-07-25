// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GroupGUI.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "MMTouchNumpad.h"
#include "MMTouchKeyboard.h"
#include "DBGroups.h"
#include "MMMessageBox.h"
#include "Users.h"
#include "DBContacts.h"

#include "DeviceRealTerminal.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
__fastcall TfrmGroupGUI::TfrmGroupGUI(TComponent* Owner, Database::TDBControl &inDBControl) : TZForm(Owner), DBControl(inDBControl)
{
	DisplayGroups();
}

__fastcall TfrmGroupGUI::~TfrmGroupGUI()
{
}

void __fastcall TfrmGroupGUI::FormShow(TObject *Sender)
{
	FormResize(this);
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TfrmGroupGUI::FormResize(TObject *Sender)
{
	if (Screen->Width != Tag)
	{
		int temp = Tag;
		Tag = Screen->Width;
		if ((double)Screen->Width / Screen->Height < 1.4)
		{
			ScaleBy(Screen->Width, temp);
		}
	}
	pnlMajor->Width = pnlMinor->Left -this->BorderWidth;
}
// ---------------------------------------------------------------------------

void TfrmGroupGUI::DisplayGroups(void)
{

	tgridGroups->Cols[GROUPNAME]->Clear();
	tgridGroups->Cols[GROUPNUMBER]->Clear();
	// tgridGroups->ColCount = 1;
	tgridGroups->RowCount = 99;
	// tgridGroups->DefaultColWidth = tgridGroups->Width;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDBGroups DBGroups;
	std::auto_ptr <TStringList> DataGroup(new TStringList);
	std::auto_ptr <TStringList> DataType(new TStringList);

	// DBGroups.SortGroups(DBTransaction);
	// std::auto_ptr<int> DataType(new int[tgridGroups->RowCount]);

	DBGroups.GetGroupsList(DBTransaction, DataGroup.get(), DataType.get());

	tgridGroups->RowCount = DataGroup->Count;
	DBTransaction.Commit();
	for (int i = 0; i < DataGroup->Count; i++)
	{
		tgridGroups->Cols[GROUPNAME]->AddObject(UnicodeString(DataGroup->Strings[i]), DataGroup->Objects[i]);
		tgridGroups->Cols[GROUPNUMBER]->AddObject(UnicodeString(DataType->Strings[i]), DataType->Objects[i]);
		// List->Cells[0][i] = UnicodeString(Data->Strings[i]);
		// List->Objects[0][i] = Data->Objects[i];
	}

}

// ---------------------------------------------------------------------------

void __fastcall TfrmGroupGUI::btnAddMouseClick(TObject *Sender)
{
	std::auto_ptr <TfrmTouchKeyboard> TouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	UnicodeString ButtonTitle = TouchKeyboard->KeyboardText;
	TouchKeyboard->MaxLength = 25;
	TouchKeyboard->AllowCarriageReturn = false;
	TouchKeyboard->StartWithShiftDown = true;
	TouchKeyboard->KeyboardText = "";
	TouchKeyboard->Caption = "Enter Group Name e.g Sport, Movies";

	if (TouchKeyboard->ShowModal() == mrOk)
	{
		if (TouchKeyboard->KeyboardText != "")
		{
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TDBGroups DBGroups;
			if (!TDBGroups::GroupExists(DBTransaction, TouchKeyboard->KeyboardText))
			{
				ContactGroup Group;
				int GroupNumber = DisplayNumpad();
				Group.Type = GroupNumber;
				Group.Name = TouchKeyboard->KeyboardText;

				DBGroups.CreateGroup(DBTransaction, Group);

			}
			else
			{
				MessageBox("Group already exists.", "Warning", MB_ICONWARNING + MB_OK);
			}
			DBTransaction.Commit();
		}
		else
		{
			MessageBox("A blank group is not allowed.", "Warning", MB_ICONWARNING + MB_OK);
		}
	}
	DisplayGroups();
}
// --------------------------------------------------------------------------

void __fastcall TfrmGroupGUI::btnEditMouseClick(TObject *Sender)
{
	ContactGroup editData;
	TDBContacts DBContacts;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDBGroups DBGroups;
	UnicodeString GroupName;




	editData.Key = (int)tgridGroups->Objects[GROUPNAME][tgridGroups->Row];
	DBGroups.GetGroup(DBTransaction, editData);
	// editData.Type = DBGroups.GetGroup(DBTransaction,(int)tgridGroups->Objects[GROUPNUMBER][tgridGroups->Row]);

	if (!tgridGroups->Col)
	{
		std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
		frmTouchKeyboard->MaxLength = 25;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = true;
		frmTouchKeyboard->KeyboardText = editData.Name;
		frmTouchKeyboard->Caption = "Enter this Group";


		if (frmTouchKeyboard->ShowModal() == mrOk)
		{

   //		GroupName = frmTouchKeyboard->KeyboardText;
			if (!DBGroups.GroupExists(DBTransaction, GroupName = frmTouchKeyboard->KeyboardText) || editData.Name == frmTouchKeyboard->KeyboardText)
			{
				editData.Name = frmTouchKeyboard->KeyboardText;
				DBGroups.UpdateGroup(DBTransaction, editData);
			}
			else
			{
				MessageBox("Group already exists.", "Warning", MB_ICONWARNING + MB_OK);
			}

		}

	}
	else
	{
		std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));

		frmTouchNumpad->Mode = pmNumber;
		frmTouchNumpad->Caption = "Enter the new group Number";
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->btnSurcharge->Visible = false;
		frmTouchNumpad->btnOk->Visible = true;
		frmTouchNumpad->INTInitial = editData.Type;
		std::vector<ContactGroup> ContactsGroupsKeys;

		DBGroups.GetContacts_ContactsKeys(DBTransaction, editData.Key, ContactsGroupsKeys);

		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			int Type = frmTouchNumpad->INTResult;
			editData.Type = Type;

			for(int i = 0; i < ContactsGroupsKeys.size(); i++)
			{
				std::vector<ContactGroup> Group;
				DBGroups.GetContacts_GroupsKeys(DBTransaction, ContactsGroupsKeys.at(i).Key, Group);


				for(int j = 0; j < Group.size(); j++)
				{
					DBGroups.GetGroup(DBTransaction, Group.at(j));
					if(Type == Group.at(j).Type)
					{
						MessageBox(" Cannot assign this group exclusivity number ", "error", 0);
						return;
					}

				}
			}
			DBGroups.UpdateGroup(DBTransaction, editData);
		}

	}

	DBTransaction.Commit();
	DisplayGroups();

}

// ---------------------------------------------------------------------------
int TfrmGroupGUI::DisplayNumpad(void)
{

	std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->Caption = "Enter the group Number";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = false;
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->ShowModal();

	return frmTouchNumpad->INTResult;

}

/*

void TfrmLocations::RefreshList()
{
List->Cols[0]->Clear();
List->ColCount = 1;
List->RowCount = 99;
List->DefaultColWidth = List->Width;
Database::TDBTransaction DBTransaction(IBDatabase);
DBTransaction.StartTransaction();
TManagerLocations ManagerLocations;
std::auto_ptr<TStringList> Data(new TStringList);
ManagerLocations.GetLocationList(DBTransaction,Data.get());
List->RowCount = Data->Count+1;
DBTransaction.Commit();
for (int i = 0; i < Data->Count ; i++)
{
List->Cols[0]->AddObject(UnicodeString(Data->Strings[i]),Data->Objects[i]);
//		List->Cells[0][i] = UnicodeString(Data->Strings[i]);
//		List->Objects[0][i] = Data->Objects[i];
}


}

 */

void __fastcall TfrmGroupGUI::btnUsersMouseClick(TObject *Sender)
{

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr <TfrmUsers> (Users)(TfrmUsers::Create <TfrmUsers> (this, TDeviceRealTerminal::Instance().DBControl));
	Users->ShowModal();

	DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGroupGUI::btnDeleteMouseClick(TObject *Sender)
{
	// Check location assigned.
	long Group_Key_To_Remove = (int)tgridGroups->Objects[0][tgridGroups->Row];

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	/* TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT * FROM GROUPS WHERE GROUPS_KEY = :GROUPS_KEY";
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = Group_Key_To_Remove;
	IBInternalQuery->ExecQuery();
	 */

	if (MessageBox("Are you sure wish to delete this group?", "Warning", MB_ICONWARNING + MB_YESNO) == ID_YES)
	{
		TDBGroups DBGroups;
		DBGroups.DeleteGroup(DBTransaction, Group_Key_To_Remove);
	}
	// }
	DBTransaction.Commit();
	DisplayGroups();

	/*
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	if(MessageBox("Are you sure you want to delete this Group","", MB_YESNO + MB_ICONQUESTION == IDYES))
	{
	int temp = 9;
	}
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TfrmGroupGUI::btnCloseMouseClick(TObject *Sender)
{
	ModalResult = mrOk;
}
