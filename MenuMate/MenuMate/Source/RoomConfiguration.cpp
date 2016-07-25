//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RoomConfiguration.h"
#include "Rooms.h"
#include "MMMessageBox.h"
#include "MMRegistry.h"

// Fomr ThisTerminal
#include "enum.h"
#include "MMTouchKeyboard.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchbutton"
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmRoomConfiguration *frmRoomConfiguration;
//---------------------------------------------------------------------------
__fastcall TfrmRoomConfiguration::TfrmRoomConfiguration(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmRoomConfiguration::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
   }
	ClientWidth = Panel2->Left + Panel2->Width + Panel24->Left;
	ClientHeight = Panel24->Height + (Panel24->Top * 2);
   Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRoomConfiguration::FormShow(TObject *Sender)
{
	FormResize(NULL);
	tbGuestMaster->Caption = "Room Interface Path\r" + TGlobalSettings::Instance().GuestMasterPath;
	cbImportRoomFiles->Checked = TRooms::Instance().ImportFiles;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRoomConfiguration::tbGuestMasterClick(TObject *Sender)
{
   if(!TRooms::Instance().Enabled)
   {
	   MessageBox("You must have the Rooms Module in order to use GuestMaster .", "Error", MB_OK);
   }
   else
   {
      frmTouchKeyboard->MaxLength = 255;
      frmTouchKeyboard->AllowCarriageReturn = false;
      frmTouchKeyboard->StartWithShiftDown = false;
      frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().GuestMasterPath;
      frmTouchKeyboard->Caption = "Enter the Path to save and read GM files from.";
      if (frmTouchKeyboard->ShowModal() == mrOk)
      {
         TGlobalSettings::Instance().GuestMasterPath = frmTouchKeyboard->KeyboardText;
         TRooms::Instance().SetGuestMasterPath(TGlobalSettings::Instance().GuestMasterPath);
         tbGuestMaster->Caption = "Room Interface Path\r" + TGlobalSettings::Instance().GuestMasterPath;
         RegistryWrite(MMBaseKey "Database","GuestMasterPath",TGlobalSettings::Instance().GuestMasterPath);
		   MessageBox("Dont forget to create a GuestMaster payment type.", "Info", MB_OK);
      }
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmRoomConfiguration::cbImportRoomFilesClick(
      TObject *Sender)
{
	TRooms::Instance().ImportFiles = cbImportRoomFiles->Checked;
	RegistryWrite(MMBaseKey "Rooms", "ImportFiles", TRooms::Instance().ImportFiles);
}
//---------------------------------------------------------------------------
void __fastcall TfrmRoomConfiguration::btnOkClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
