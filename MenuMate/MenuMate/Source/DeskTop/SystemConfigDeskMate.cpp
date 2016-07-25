//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SystemConfigDeskMate.h"
#include "MMTouchKeyboard.h"
#include "MMRegistry.h"
#include "enum.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"
#include <memory>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmSystemConfig::TfrmSystemConfig(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::tbInterbaseClick(TObject *Sender)
{
	std::auto_ptr<TfrmTouchKeyboard> TouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));

	TouchKeyboard->MaxLength = 15;
   TouchKeyboard->AllowCarriageReturn = false;
   TouchKeyboard->StartWithShiftDown = false;
	UnicodeString InterbaseIP = "";
	RegistryRead(MMBaseKey "Database","InterbaseIP",InterbaseIP);

	TouchKeyboard->KeyboardText = InterbaseIP;
	TouchKeyboard->Caption = "IP Address of Interbase Server";
	if (TouchKeyboard->ShowModal() == mrOk)
	{
		InterbaseIP = TouchKeyboard->KeyboardText;
		tbInterbase->Caption = "Interbase Server IP Address\r" + InterbaseIP;
		RegistryWrite(MMBaseKey "Database","InterbaseIP",InterbaseIP);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::tbDBLocationClick(TObject *Sender)
{
	UnicodeString DatabasePath = "";
   RegistryRead(MMBaseKey "Database","DatabasePath",DatabasePath);
	std::auto_ptr<TfrmTouchKeyboard> TouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));

	TouchKeyboard->MaxLength = 250;
   TouchKeyboard->AllowCarriageReturn = false;
   TouchKeyboard->StartWithShiftDown = false;
   TouchKeyboard->KeyboardText = DatabasePath;

   if(TouchKeyboard->KeyboardText == "")
   {
      if(FileExists(ExtractFilePath(Application->ExeName) + "MenuMate.gdb"))
      {
         TouchKeyboard->KeyboardText = ExtractFilePath(Application->ExeName) + "MenuMate.gdb";
      }
      else
      {
         TouchKeyboard->KeyboardText = "C:\\Program Files\\MenuMate\\MenuMate.gdb";
      }
   }
   TouchKeyboard->Caption = "Path of MenuMate.gdb File.";
   if (TouchKeyboard->ShowModal() == mrOk)
   {
      DatabasePath = TouchKeyboard->KeyboardText;
      tbDBLocation->Caption = "Database Location\r" + DatabasePath;
		RegistryWrite(MMBaseKey "Database","DatabasePath",DatabasePath);
   }
}
//---------------------------------------------------------------------------


void __fastcall TfrmSystemConfig::TouchButton2Click(TObject *Sender)
{
   ModalResult = mrAbort;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::tbOkClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
   }
//   ClientWidth = pButtons->Width;
//   ClientHeight = pButtons->Height;
   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;

}
//---------------------------------------------------------------------------

void __fastcall TfrmSystemConfig::FormShow(TObject *Sender)
{
	FormResize(Sender);
	UnicodeString DatabasePath = "";
	RegistryRead(MMBaseKey "Database","DatabasePath",DatabasePath);
	UnicodeString InterbaseIP = "";
	RegistryRead(MMBaseKey "Database","InterbaseIP",InterbaseIP);

	tbInterbase->Caption = "Interbase Server IP Address\r" + InterbaseIP;
	tbDBLocation->Caption = "Database Location\r" + DatabasePath;
}
//---------------------------------------------------------------------------
