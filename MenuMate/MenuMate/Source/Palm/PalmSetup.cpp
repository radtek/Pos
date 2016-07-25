//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Palm.h"
#include "PalmSetup.h"
#include "Comms.h"
#include "MMLogging.h"
#include "enum.h"
#include "MMRegistry.h"
//#include "PrinterConfig.h"
#include "ManagerVariable.h"
#include "HappyHourSettings.h"
#include "MMMessageBox.h"
#include "MMTouchKeyboard.h"
#include "TableManager.h"
#include "ContactStaff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CPortCtl"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmPalmSetup *frmPalmSetup;
//---------------------------------------------------------------------------
__fastcall TfrmPalmSetup::TfrmPalmSetup(TComponent* Owner)
	: TForm(Owner)
{
#ifdef _DEBUG
	chkLogGetOrders->Visible = true;
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::FormShow(TObject *Sender)
{
	FormResize(NULL);
	cbReceiptPrinter->Clear();
	// Loop though Receipt printers
	TManagerPhysicalPrinter ManagerPhysicalPrinter;
	cbReceiptPrinter->Items->AddObject("None",static_cast<TObject *>(0));

	Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
	DBTransaction.StartTransaction();
	ManagerPhysicalPrinter.DBGetPrinterList(DBTransaction,cbReceiptPrinter->Items);
	DBTransaction.Commit();

	cbReceiptPrinter->ItemIndex = 0;
	if(TComms::Instance().ReceiptPrinter.PhysicalPrinterKey != 0)
	{
		int PrinterIndex = 0;
		for(int i=0; i< cbReceiptPrinter->Items->Count; i++)
		{
			if((int)(cbReceiptPrinter->Items->Objects[i]) == TComms::Instance().ReceiptPrinter.PhysicalPrinterKey)
			{
					PrinterIndex = i;
			}
		}
		cbReceiptPrinter->ItemIndex = PrinterIndex;
	}

	ComcbBlueTooth->ComPort = TDeviceRealPalms::Instance().BlueTooth->CommPort;
	if(TDeviceRealPalms::Instance().BlueTooth->CommPort->Connected)
   {
      btnBlueToothOn->OnClick = NULL;
      btnBlueToothOn->Checked = true;
      btnBlueToothOn->OnClick = btnBlueToothOnClick;
   }
	else
	{
		btnBlueToothOn->OnClick = NULL;
		btnBlueToothOn->Checked = false;
		btnBlueToothOn->OnClick = btnBlueToothOnClick;
	}

	cbOnLineLogging->Checked = TManagerLogs::Instance().OnLineLogging;
	cbRememberLastServingCourse->Checked = TGlobalSettings::Instance().RememberLastServingCourse;

	if(TDeviceRealPalms::Instance().KitchenMod->Enabled)
   {
		tbChefMateIP->Color = TGlobalSettings::Instance().KitchenColour;
	}
	else
	{
		tbChefMateIP->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::cbReceiptPrinterChange(TObject *Sender)
{
	if(cbReceiptPrinter->ItemIndex != -1)
   {
		Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
		DBTransaction.StartTransaction();
		if(cbReceiptPrinter->ItemIndex == 0)
		{
			TPrinterPhysical Blank;
			TComms::Instance().ReceiptPrinter = Blank;
			TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmReceiptPrinter,0);
		}
		else
		{
			TComboBox *pCB = (TComboBox *)Sender;
         TManagerPhysicalPrinter ManagerPhysicalPrinter;
         TComms::Instance().ReceiptPrinter = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,(int)cbReceiptPrinter->Items->Objects[cbReceiptPrinter->ItemIndex]);
         TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmReceiptPrinter,TComms::Instance().ReceiptPrinter.PhysicalPrinterKey);
		}
		DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::cbReceiptPrinterDropDown(TObject *Sender)
{
	cbReceiptPrinter->Clear();
	TManagerPhysicalPrinter ManagerPhysicalPrinter;
	cbReceiptPrinter->Items->AddObject("None",static_cast<TObject *>(0));
	Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
	DBTransaction.StartTransaction();
	ManagerPhysicalPrinter.DBGetPrinterList(DBTransaction,cbReceiptPrinter->Items);
	DBTransaction.Commit();
	cbReceiptPrinter->ItemIndex = 0;
	if(TComms::Instance().ReceiptPrinter.PhysicalPrinterKey != 0)
	{
		int PrinterIndex = 0;
		for(int i=0; i< cbReceiptPrinter->Items->Count; i++)
		{
			if((int)(cbReceiptPrinter->Items->Objects[i]) == TComms::Instance().ReceiptPrinter.PhysicalPrinterKey)
			{
					PrinterIndex = i;
			}
		}
		cbReceiptPrinter->ItemIndex = PrinterIndex;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
		int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------


void __fastcall TfrmPalmSetup::tbtnChangeServer(TObject *Sender)
{
   if(MessageBox(_T("This action requires this PalmMate to be restarted. Are you sure you wish to continue?"),_T("Confirmation"),
		MB_OKCANCEL + MB_DEFBUTTON2 + MB_ICONINFORMATION) == IDOK)
   {
		Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().Delete(DBTransaction,vmAutoMateIP);
		DBTransaction.Commit();
		RegistryRemoveValue(MMBaseKey "Database","DatabasePath");
		RegistryRemoveValue(MMBaseKey "Database","InterbaseIP");
      	TDeviceRealPalms::Instance().Reboot = true;
		Application->Terminate();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPalmSetup::btnBlueToothOnClick(TObject *Sender)
{
   if(!btnBlueToothOn->Checked)
   {
      if(TDeviceRealPalms::Instance().BlueTooth->Close())
      {
			Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
			DBTransaction.StartTransaction();
			TManagerVariable::Instance().Delete(DBTransaction,vmBluetoothPort);
			DBTransaction.Commit();
		}
	}
	else
	{
		if(!TDeviceRealPalms::Instance().BlueTooth->Open())
		{
			btnBlueToothOn->Checked = false;
		}
		else
		{
			Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
			DBTransaction.StartTransaction();
			TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmBluetoothPort,TDeviceRealPalms::Instance().BlueTooth->CommPort->Port);
			DBTransaction.Commit();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPalmSetup::comcbBlueToothChange(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmBluetoothPort,TDeviceRealPalms::Instance().BlueTooth->CommPort->Port);
	DBTransaction.Commit();
	TDeviceRealPalms::Instance().BlueTooth->Close();
	btnBlueToothOn->Checked = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::cbOnLineLoggingClick(TObject *Sender)
{
	TManagerLogs::Instance().OnLineLogging = cbOnLineLogging->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::ComcbBlueToothClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmBluetoothPort,TDeviceRealPalms::Instance().BlueTooth->CommPort->Port);
	DBTransaction.Commit();
	TDeviceRealPalms::Instance().BlueTooth->Close();
	btnBlueToothOn->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPalmSetup::btnHappyHourClick(TObject *Sender)
{
	TMMContactInfo UserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,UserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		TfrmHappyHourSettings *frmHappyHourSettings = new TfrmHappyHourSettings(this);
		try
		{
			frmHappyHourSettings->ShowModal();
		}
		__finally
		{
			delete frmHappyHourSettings;
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the Happy Hour settings." , "Error" , MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPalmSetup::tbChefMateColourClick(TObject *Sender)
{
	MessageBox("Please select a color for this terminal.", "Select Color", MB_OK + MB_ICONQUESTION);
	ColorDialog->Execute();
	TGlobalSettings::Instance().KitchenColour = ColorDialog->Color;
	tbChefMateIP->Color = TGlobalSettings::Instance().KitchenColour;
	Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmChefMateColour,TGlobalSettings::Instance().KitchenColour);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::tbtnSeatName(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
	DBTransaction.StartTransaction();

  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));

	frmTouchKeyboard->MaxLength				= 8;
	frmTouchKeyboard->AllowCarriageReturn	= false;
	frmTouchKeyboard->StartWithShiftDown	= false;
	frmTouchKeyboard->Caption					= "Enter Seat Label";
	frmTouchKeyboard->KeyboardText			= TManagerVariable::Instance().GetStr(DBTransaction,vmSeatLabel, "");

	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		if (frmTouchKeyboard->KeyboardText == "")
		{
			MessageBox("You must enter a name.", "Default Seat Name", MB_OK + MB_ICONERROR);
		}
		else
		{
			TGlobalSettings::Instance().SeatLabel = frmTouchKeyboard->KeyboardText;
			TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmSeatLabel, TGlobalSettings::Instance().SeatLabel);
			TNetMessageInfoSync Request;
			TDeviceRealPalms::Instance().UpdateInfoSync(&Request);
		}
	}
	DBTransaction.Commit();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::cbRememberLastServingCourseClick(TObject *Sender)
{
	TGlobalSettings::Instance().RememberLastServingCourse = cbRememberLastServingCourse->Checked;
	Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmRememberLastServingCourse, cbRememberLastServingCourse->Checked);
	DBTransaction.Commit();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::chkLogGetOrdersClick(TObject *Sender)
{
	LogGetOrders = chkLogGetOrders->Checked;
}
//---------------------------------------------------------------------------

