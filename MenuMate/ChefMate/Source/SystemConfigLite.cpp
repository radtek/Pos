//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SystemConfigLite.h"
#include "MMTouchKeyboard.h"
#include "MMRegistry.h"
#include "enum.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmSystemConfig::TfrmSystemConfig(TComponent* Owner)
   : TForm(Owner)
{
   RequestClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::tbInterbaseClick(TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
   frmTouchKeyboard->MaxLength = 15;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   UnicodeString InterbaseIP = "";
   RegistryRead(MMBaseKey "Database","InterbaseIP",InterbaseIP);

	frmTouchKeyboard->KeyboardText = InterbaseIP;
	frmTouchKeyboard->Caption = "IP Address of Interbase Server";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		InterbaseIP = frmTouchKeyboard->KeyboardText;
		tbInterbase->Caption = "Interbase Server IP Address\r" + InterbaseIP;
		RegistryWrite(MMBaseKey "Database","InterbaseIP",InterbaseIP);
	}

	RegistryRead(MMBaseKey "Configuration","RedisplayWarningMinutes",MinutesWarning);
	tbUnsSannedWarning->Caption = IntToStr(MinutesWarning);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::tbDBLocationClick(TObject *Sender)
{
   UnicodeString DatabasePath = "";
   RegistryRead(MMBaseKey "Database","DatabasePath",DatabasePath);

  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 250;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = DatabasePath;

   if(frmTouchKeyboard->KeyboardText == "")
   {
      if(FileExists(ExtractFilePath(Application->ExeName) + "MenuMate.gdb"))
      {
         frmTouchKeyboard->KeyboardText = ExtractFilePath(Application->ExeName) + "MenuMate.gdb";
      }
      else
      {
         frmTouchKeyboard->KeyboardText = "C:\\Program Files\\MenuMate\\MenuMate.gdb";
      }
   }
   frmTouchKeyboard->Caption = "Path of MenuMate.gdb File.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
      DatabasePath = frmTouchKeyboard->KeyboardText;
      tbDBLocation->Caption = "Database Location\r" + DatabasePath;
		RegistryWrite(MMBaseKey "Database","DatabasePath",DatabasePath);
   }
}
//---------------------------------------------------------------------------


void __fastcall TfrmSystemConfig::tbtnExitClick(TObject *Sender)
{
   RequestClose = true;
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

	int PortNumber = -1;
	String KeyName = MMBaseKey "Comms";
	String KickKey = "SerialBarcodePort";
	RegistryRead(KeyName,KickKey,PortNumber);

	switch(PortNumber)
	{
		case -1 :
		case 0 :
			cbBarCodePort->ItemIndex = 0;
		default :
			cbBarCodePort->ItemIndex = PortNumber;
	}

	KeyName = MMBaseKey "Configuration";
	KickKey = "AutoCloseWhenBumped";
	bool AutoCloseWhenBumped = false;
	RegistryRead(KeyName,KickKey,AutoCloseWhenBumped);
	cbAutoCloseWhenBumped->Checked = AutoCloseWhenBumped;

	KeyName = MMBaseKey "Configuration";
	KickKey = "RemovedWhenScanned";
	bool RemovedWhenScanned = false;
	RegistryRead(KeyName,KickKey,RemovedWhenScanned);
	cbRemovedWhenScanned->Checked = RemovedWhenScanned;

	KeyName = MMBaseKey "Configuration";
	KickKey = "RemovedWhenClosed";
	bool RemovedWhenClosed = false;
	RegistryRead(KeyName,KickKey,RemovedWhenClosed);
	cbRemovedWhenClosed->Checked = RemovedWhenClosed;

	KeyName = MMBaseKey "Configuration";
	KickKey = "DisplayDynamicOrders";
	bool DisplayDynamicOrders = false;
	RegistryRead(KeyName,KickKey,DisplayDynamicOrders);
	cbDisplayDynamicOrders->Checked = DisplayDynamicOrders;

	KeyName = MMBaseKey "Configuration";
	KickKey = "ConfirmBeforeBumping";
	bool ConfirmBeforeBumping = false;
	RegistryWrite(KeyName,KickKey,ConfirmBeforeBumping);
   cbConfirmBeforeBumping->Checked = ConfirmBeforeBumping;

	RegistryRead(MMBaseKey "Configuration","RedisplayWarningMinutes",MinutesWarning);
   tbUnsSannedWarning->Caption = IntToStr(MinutesWarning);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSystemConfig::cbBarCodePortChange(TObject *Sender)
{
	int PortNumber = -1;
	if(cbBarCodePort->ItemIndex != -1)
   {
		if(cbBarCodePort->ItemIndex != 0)
		{
			PortNumber = cbBarCodePort->ItemIndex;
		}
      else
		{
			PortNumber = -1;
      }
		// Update Serial Draw Kick Value in Reg.
		const String KeyName = MMBaseKey "Comms";
		const String KickKey = "SerialBarcodePort";
		RegistryWrite(KeyName,KickKey,PortNumber);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSystemConfig::cbAutoCloseWhenBumpedClick(TObject *Sender)
{
	const String KeyName = MMBaseKey "Configuration";
	const String KickKey = "AutoCloseWhenBumped";
	bool AutoCloseWhenBumped = cbAutoCloseWhenBumped->Checked;
	RegistryWrite(KeyName,KickKey,AutoCloseWhenBumped);
}
//---------------------------------------------------------------------------


void __fastcall TfrmSystemConfig::tbUnsSannedWarningClick(TObject *Sender)
{
	RegistryRead(MMBaseKey "Configuration","RedisplayWarningMinutes",MinutesWarning);
   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter the Number of Minutes";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = MinutesWarning;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		const String KeyName = MMBaseKey "Configuration";
		const String KickKey = "RedisplayWarningMinutes";
		MinutesWarning = frmTouchNumpad->INTResult;
		RegistryWrite(KeyName,KickKey,MinutesWarning);
		tbUnsSannedWarning->Caption = IntToStr(MinutesWarning);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSystemConfig::cbRemovedWhenScannedClick(
      TObject *Sender)
{
	const String KeyName = MMBaseKey "Configuration";
	const String KickKey = "RemovedWhenScanned";
	bool RemovedWhenScanned = cbRemovedWhenScanned->Checked;
	RegistryWrite(KeyName,KickKey,RemovedWhenScanned);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::cbDisplayDynamicOrdersClick(TObject *Sender)
{
	const String KeyName = MMBaseKey "Configuration";
	const String KickKey = "DisplayDynamicOrders";
	bool DisplayDynamicOrders = cbDisplayDynamicOrders->Checked;
	RegistryWrite(KeyName,KickKey,DisplayDynamicOrders);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSystemConfig::cbConfirmBeforeBumpingClick(
      TObject *Sender)
{
	const String KeyName = MMBaseKey "Configuration";
	const String KickKey = "ConfirmBeforeBumping";
	bool ConfirmBeforeBumping = cbConfirmBeforeBumping->Checked;
	RegistryWrite(KeyName,KickKey,ConfirmBeforeBumping);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSystemConfig::cbRemovedWhenClosedClick(TObject *Sender)
{
	const String KeyName = MMBaseKey "Configuration";
	const String KickKey = "RemovedWhenClosed";
	bool RemovedWhenClosed = cbRemovedWhenClosed->Checked;
	RegistryWrite(KeyName,KickKey,RemovedWhenClosed);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSystemConfig::TouchBtn1MouseClick(TObject *Sender)
{
	String KeyName = MMBaseKey "Configuration";
	String KickKey = "Splitter1Left";
	RegistryWrite(KeyName,KickKey,100);
	KickKey = "Splitter2Left";
	RegistryWrite(KeyName,KickKey,200);
	KickKey = "Splitter3Left";
	RegistryWrite(KeyName,KickKey,300);
	KickKey = "SplitterAged1Left";
	RegistryWrite(KeyName,KickKey,100);
	KickKey = "SplitterAged2Left";
	RegistryWrite(KeyName,KickKey,200);
	KickKey = "SplitterAged3Left";
	RegistryWrite(KeyName,KickKey,300);
}
//---------------------------------------------------------------------------

