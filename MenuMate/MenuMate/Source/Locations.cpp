#include <vcl.h>
#pragma hdrstop

#include "Locations.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
#include "ManagerLocations.h"
#include "GUIScale.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmLocations::TfrmLocations(TComponent* Owner, Database::TDBControl &inIBDatabase)
	: TZForm(Owner) , IBDatabase(inIBDatabase)
{
	Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLocations::AddClick(TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	UnicodeString ButtonTitle = frmTouchKeyboard->KeyboardText;
	frmTouchKeyboard->MaxLength = 25;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = true;
	frmTouchKeyboard->KeyboardText = "";
	frmTouchKeyboard->Caption = "Enter Location Name e.g Bar,Restaurant";



	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		if(frmTouchKeyboard->KeyboardText != "")
		{
			Database::TDBTransaction DBTransaction(IBDatabase);
			DBTransaction.StartTransaction();
			TManagerLocations ManagerLocations;
			if(ManagerLocations.FindLocation(DBTransaction,frmTouchKeyboard->KeyboardText) == 0)
			{
				// Used to signal to SetLocation that we are, indeed, creating a new location.
				int Key = 0;
				ManagerLocations.SetLocation(DBTransaction,Key,frmTouchKeyboard->KeyboardText);
			}
			else
			{
				MessageBox("Location already exists.", "Warning", MB_ICONWARNING + MB_OK);
			}
			DBTransaction.Commit();
		}
		else
		{
			MessageBox("A Blank location is not allowed.", "Warning", MB_ICONWARNING + MB_OK);
		}
	}
 	RefreshList();

}
//---------------------------------------------------------------------------
void __fastcall TfrmLocations::EditLocClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
	TManagerLocations ManagerLocations;
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 25;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = true;
	frmTouchKeyboard->KeyboardText = ManagerLocations.GetLocation(DBTransaction,(int)List->Objects[0][List->Row]);
	frmTouchKeyboard->Caption = "Enter this Location";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		int Key = (int)List->Objects[0][List->Row];
		ManagerLocations.SetLocation(DBTransaction,Key,frmTouchKeyboard->KeyboardText);
	}
	DBTransaction.Commit();	
  	RefreshList();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLocations::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	SelectedLocation = List->Cells[0][List->Row];
	SelectedLocationKey = (int)List->Objects[0][List->Row];
}
//---------------------------------------------------------------------------
void __fastcall TfrmLocations::ExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TfrmLocations::FormShow(TObject *Sender)
{
	FormResize(this);
    	RefreshList();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLocations::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
                if(double(Screen->Width) / Screen->Height < 1.4)
                {
      			ScaleBy(Screen->Width, Temp);
                }
	}
        /*
        this->Height = Screen->Height - 40;

	ClientWidth = pnl2->Left + pnl2->Width + pnl1->Left;
 //	ClientHeight = pnl1->Height + (pnl1->Top * 2);
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
        if(Screen->Height > 600)
        {
        	GUIScale::ChildrenTop(pnl1, 0.9);
        }
        */
}

void __fastcall TfrmLocations::btnUpMouseClick(TObject *Sender)
{
	__int64 TickCount = ::GetTickCount();
	int ScrollCount = 0;
//	while (ScrollCount < List->VisibleRowCount-1)
	while (ScrollCount < (int(List->Height / List->DefaultRowHeight) -1))
   {
      if (::GetTickCount() > TickCount + 20)
		{
         TickCount = ::GetTickCount();
         SendMessage(List->Handle, WM_VSCROLL, SB_LINEUP, 0L);
         ScrollCount++;
         if (List->TopRow == 0)
         {
         	break;
         }
      }
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmLocations::btnDownMouseClick(TObject *Sender)
{
	__int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
	while (ScrollCount < (int(List->Height / List->DefaultRowHeight) -1))
	{
		if (::GetTickCount() > TickCount + 20)
      {
			TickCount = ::GetTickCount();
         SendMessage(List->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
			ScrollCount++;
         if (List->TopRow == List->RowCount - List->VisibleRowCount)
         {
         	break;
         }
      }
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmLocations::TbtnDeleteClick(TObject *Sender)
{
// Check location assigned.
	long Location_Key_To_Remove = (int)List->Objects[0][List->Row];

	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		"SELECT * FROM DEVICES WHERE LOCATION_KEY = :LOC_LOCATION_KEY";
	IBInternalQuery->ParamByName("LOC_LOCATION_KEY")->AsInteger = Location_Key_To_Remove;
	IBInternalQuery->ExecQuery();

	if (IBInternalQuery->RecordCount > 0)
	{
		UnicodeString Terminals = "";
		for (;!IBInternalQuery->Eof ;IBInternalQuery->Next())
		{
			Terminals += IBInternalQuery->FieldByName("DEVICE_NAME")->AsString + ", ";
		}
		MessageBox("The Following terminals are using this location you must reconfigure them first. " + Terminals, "Warning", MB_ICONWARNING + MB_OKCANCEL);
	}
	else
	{
		if(MessageBox("Are you sure wish to delete this Location?", "Warning",
			MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
		{
			TManagerLocations ManagerLocations;
			ManagerLocations.DeleteLocation(DBTransaction,Location_Key_To_Remove);
		}
	}
	DBTransaction.Commit();
	RefreshList();
}
//---------------------------------------------------------------------------


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


