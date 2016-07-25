//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <memory>
#include "Users.h"
#include "AddUser.h"
#include "StockData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmUsers *frmUsers;
//---------------------------------------------------------------------------
__fastcall TfrmUsers::TfrmUsers(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmUsers::FormShow(TObject *Sender)
{
	FormResize(NULL);
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	dtUsers->Close();
	dtUsers->Open();
	dtUsersAfterScroll(NULL);
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmUsers::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmUsers::dtUsersAfterScroll(TDataSet *DataSet)
{
	lbeTitle->Caption	= dtUsers->FieldByName("USER_NAME")->AsString;
	Label2->Caption	= dtUsers->FieldByName("ORDER_PREFIX")->AsString +
							  dtUsers->FieldByName("NEXT_ORDER_NUMBER")->AsString +
							  dtUsers->FieldByName("ORDER_SUFFIX")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUsers::FormResize(TObject *Sender)
{
/*	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;

		ScaleBy(Screen->Width, Temp);
	}
	ClientHeight						= btnClose->Top + btnClose->Height + Panel1->Top;
	ClientWidth							= Panel1->Width + Panel1->Left + DBGrid1->Left;
	Left									= (Screen->Width - Width) / 2;
	Top									= (Screen->Height - Height) / 2;*/
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmUsers::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmUsers::ResizeGrids()
{
/*	static PercArray[2] = {75, 25};
	int WidthSoFar = 0;
	int PercLeft = 100;
	for (int i=0; i<DBGrid1->Columns->Count; i++)
	{
		DBGrid1->Columns->Items[i]->Width = (DBGrid1->ClientWidth - WidthSoFar) * PercArray[i] / PercLeft - 1;
		WidthSoFar += DBGrid1->Columns->Items[i]->Width + 1;
		PercLeft -= PercArray[i];
	}
	DBGrid1->Columns->Items[DBGrid1->Columns->Count-1]->Width++;*/
	DBGrid1->Columns->Items[0]->Width = DBGrid1->ClientWidth - 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUsers::btnAddClick(TObject *Sender)
{
	std::auto_ptr<TfrmAddUser>frmAddUser(new TfrmAddUser(NULL));
	frmAddUser->UserMode = umAdd;
	if (frmAddUser->ShowModal() == mrOk)
	{
		FormShow(NULL);
		dtUsers->Locate("USER_ID", frmAddUser->UserID, TLocateOptions());
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUsers::btnEditClick(TObject *Sender)
{
	std::auto_ptr<TfrmAddUser>frmAddUser(new TfrmAddUser(NULL));
	frmAddUser->UserMode = umEdit;
	frmAddUser->UserID = dtUsers->FieldByName("USER_ID")->AsString;
	if (frmAddUser->ShowModal() == mrOk)
	{
		FormShow(NULL);
		dtUsers->Locate("USER_ID", frmAddUser->UserID, TLocateOptions());
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUsers::btnDeleteClick(TObject *Sender)
{
	if (Application->MessageBox("Delete this user?", "Delete?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
	{
		dtUsers->Delete();
		if (Transaction->InTransaction) Transaction->Commit();
		FormShow(NULL);
	}
}
//-------------------------------------------------------------------------
void __fastcall TfrmUsers::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

