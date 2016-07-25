//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmCfg.h"
#include "main.h"
#include "Queries.h"
#include "SystemConfigLite.h"
#include "MMRegistry.h"
#include "frmCfg.h"
#include "enum.h"
#include "Version.h"
#include "MMMessageBox.h"
#include "ManagerVariable.h"
#include <DateUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmConfig::TfrmConfig(TComponent* Owner,Database::TDBControl &inDBControl,TManagerVariable &inVM)
	: TForm(Owner),DBControl(inDBControl),Vars(inVM)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfig::FormCreate(TObject *Sender)
{
	MyMonitorNum = Monitor->MonitorNum;
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::tbConfigureFontSizeClick(TObject *Sender)
{
	frmMain->Fonts->Execute();

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
   	Vars.DeviceProfileKey = Vars.SetProfile(DBTransaction,eTerminalProfiles,"ChefMate");
    Vars.SetProfileStr(DBTransaction,Vars.DeviceProfileKey,vmGraphicFontName,frmMain->Fonts->Font->Name);
    Vars.SetProfileInt(DBTransaction,Vars.DeviceProfileKey,vmGraphicFontSize,frmMain->Fonts->Font->Size);
	DBTransaction.Commit();

	frmMain->Kitchen->SetFontSize(frmMain->Fonts);
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfig::tbExitClick(TObject *Sender)
{
	frmMain->Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::FormShow(TObject *Sender)
{
	FormResize(Sender);

   btnMon->Caption = "Display Next Screen";

   if(Screen->MonitorCount > 1)
	{
		btnMon->Enabled = true;
   }
   else
   {
		btnMon->Enabled = false;
	}
	lbeVersion->Caption = "Version " + GetFileVersion();
	lbeVersion->Left = (tbExit->Left + tbExit->Width) - lbeVersion->Width;
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::FormResize(TObject *Sender)
{
	if (Tag != Monitor->Width)
   {
      int Temp = Tag;
		Tag = Monitor->Width;
		ScaleBy(Monitor->Width, Temp);
	}
	Left = Monitor->Left + ((Monitor->Width - Width) / 2);
	Top = (Monitor->Height - Height) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::btnMonClick(TObject *Sender)
{
	MainsMonitorNum = frmMain->Monitor->MonitorNum;
	if(MainsMonitorNum < (Screen->MonitorCount -1))
	{
		MainsMonitorNum++;
	}
	else if(MainsMonitorNum == (Screen->MonitorCount -1) && MainsMonitorNum > 0)
	{
		MainsMonitorNum = 0;
	}
	frmMain->Hide();

	frmMain->Align = alNone;
	frmMain->Left = Screen->Monitors[MainsMonitorNum]->Left;
	frmMain->Width = Screen->Monitors[MainsMonitorNum]->Width;
	frmMain->Top = Screen->Monitors[MainsMonitorNum]->Top;
	frmMain->Height = Screen->Monitors[MainsMonitorNum]->Height;
	frmMain->MakeFullyVisible(Screen->Monitors[MainsMonitorNum]);
	frmMain->Show();
	MyMonitorNum = MainsMonitorNum;
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TfrmConfig::TouchButton4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::tbReconfigureClick(TObject *Sender)
{
	std::auto_ptr<TfrmSystemConfig> frmSystemConfig(new TfrmSystemConfig(NULL));
	frmSystemConfig->ShowModal();
	if(frmSystemConfig->ModalResult != mrAbort)
	{
		frmMain->CloseDB();
		bool OpenOk = false;
		do
		{
			OpenOk = frmMain->OpenDB();
		}while(!OpenOk);

		int PortNumber = -1;
		RegistryRead(MMBaseKey "Comms","SerialBarcodePort",PortNumber);
		if(PortNumber != -1 && !frmMain->BarCodeReader->Open(PortNumber))
		{
			MessageBox("Unable to Open Barcode Reader Comm port.\r" + frmMain->BarCodeReader->LastException, "Barcode Reader Error", MB_OK + MB_ICONWARNING);
		}

		String KeyName = MMBaseKey "Configuration";
		String KickKey = "AutoCloseWhenBumped";
		frmMain->Kitchen->AutoCloseWhenBumped = false;
		RegistryRead(KeyName,KickKey,frmMain->Kitchen->AutoCloseWhenBumped);

      KeyName = MMBaseKey "Configuration";
      KickKey = "RemovedWhenScanned";
		frmMain->Kitchen->RemovedWhenScanned = false;
      RegistryRead(KeyName,KickKey,frmMain->Kitchen->RemovedWhenScanned);

      KeyName = MMBaseKey "Configuration";
      KickKey = "RemovedWhenClosed";
		frmMain->Kitchen->RemovedWhenScanned = false;
      RegistryRead(KeyName,KickKey,frmMain->Kitchen->RemovedWhenClosed);

      KeyName = MMBaseKey "Configuration";
      KickKey = "DisplayDynamicOrders";
		frmMain->Kitchen->DisplayDynamicOrders = false;
      RegistryRead(KeyName,KickKey,frmMain->Kitchen->DisplayDynamicOrders);

      KeyName = MMBaseKey "Configuration";
      KickKey = "ConfirmBeforeBumping";
      frmMain->ConfirmBeforeBumping = false;
      RegistryRead(KeyName,KickKey,frmMain->ConfirmBeforeBumping);

      TDateTime inWarningTime = 0;
      inWarningTime = IncMinute(inWarningTime,frmSystemConfig->MinutesWarning);
      frmMain->Kitchen->SetMinutesWarning(inWarningTime);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfig::CardSwipe(TMessage& Message)
{
	enum TCardType {eUnknown,eKitchenDocket,eStaffCard,eMemberCard,eTabCard};
	TCardType DataType = eUnknown;
	int TimeKey = 0;
	UnicodeString Data = (char *)Message.WParam;
	if(Data.SubString(0,3) == "MMK") // Kitchen Docket.
	{
      DataType = eKitchenDocket;
		TimeKey = StrToInt("0x" + Data.SubString(4,Data.Length() - 3));
   }
   else if(Data.SubString(0,5) == "{AMMK") // Kitchen Docket.
   {
		DataType = eKitchenDocket;
		TimeKey = StrToInt("0x" + Data.SubString(6,Data.Length() - 5));
   }

   if(DataType == eKitchenDocket) // Kitchen Docket.
   {
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		int RecordsUpdated = CloseMakeStartTime(DBTransaction,TimeKey);
		DBTransaction.Commit();
		if(RecordsUpdated > 0)
		{
			frmMain->tbtnConfig->ButtonColor = clGreen;
		}
		else
		{
			frmMain->tbtnConfig->ButtonColor = clRed;
		}
   }
   else if(DataType == eStaffCard)
   {
	}
	else if(DataType == eTabCard)
	{
	}
	else if(DataType == eMemberCard)
	{
	}
	else
	{
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::TouchBtn2MouseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


