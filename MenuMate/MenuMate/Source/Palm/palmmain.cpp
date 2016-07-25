// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Palm.h"
#include "enum.h"
#include "Comms.h"
#include "Printing.h"
#include "palmmain.h"
#include "crctable.h"
#include "MMRegistry.h"
#include "Processing.h"
#include "MMTouchKeyboard.h"
#include "SelectActiveMenus.h"
#include "PalmRegistration.h"
#include "PalmSetup.h"
#include "Secure.h"
#include "Clipbrd.hpp"
#include "DbMod.h"
#include "SystemConfig.h"
#include "Contact.h"
#include "MMMessageBox.h"
#include "ManagerStock.h"
#include "ManagerVariable.h"
#include "ManagerPatron.h"
#include "ManagerDiscount.h"
#include "Locations.h"
#include "Invoice.h"
#include "Version.h"
#include "ManagerNet.h"
#include "TableManager.h"
#include "VirtualPrinterManager.h"
#include "ManagerPhysicalPrinter.h"
#include "DocketManager.h"
#include "ContactStaff.h"
#include "MM_DBCore.h"
#include "DBTab.h"
#include "DBTables.h"

#ifdef _DEBUG
#include "DebugManager.h"
#include "Debugfrm.h"
#endif
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CPort"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

TfrmMain *frmMain;

// ---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner)
{
   // TODO 5 -o Michael  -c  Check : Palm Names On receipt.
   RequestClose = false;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
   ManagerDockets = new TManagerDockets(TDeviceRealPalms::Instance().DBControl);
   Invoice = new TInvoice;
   ManagerVirtualPrinter = new TManagerVirtualPrinter();
   ManagerDiscount = new TManagerDiscount();
#ifdef _DEBUG
   frmDebug = new TfrmDebug(NULL);
   ManagerDebug = new TManagerDebug;
#endif
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
   delete Invoice;
   Invoice = NULL;

   delete ManagerVirtualPrinter;
   ManagerVirtualPrinter = NULL;

	delete ManagerDiscount;

   delete ManagerDockets;
   ManagerDockets = NULL;

#ifdef _DEBUG
   delete frmDebug;
   delete ManagerDebug;
   ManagerDebug = NULL;
#endif
}
// ---------------------------------------------------------------------------

int TfrmMain::SomeStrings(TStrings *Strings)
{
   int retval = 0;
   for (int i = 1; i <= Strings->Text.Length(); i++)
   {
	  retval += (int)(Strings->Text[i]);
   }
   return retval;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void TfrmMain::Init()
{
   frmDBMod->Show();
   frmDBMod->SetCaption("Loading PalmMate...");

   frmDBMod->SetCaption("Opening Database...");
   if (!TDeviceRealPalms::Instance().OpenDatabases())
   {
	  OnCloseQuery = NULL;
	  Close();
	  return;
   }

   try
   {

	  lbeVersion->Caption = "Version " + GetFileVersion() + "  ";
	  lbeVersion->Left = Panel3->Width - lbeVersion->Width - 2;
	  lbeVersion->Top = Panel3->Height - lbeVersion->Height - 2;

	  if (!TDeviceRealPalms::Instance().BlueTooth->CommPort->Connected)
	  {
		 MessageBox("Bluetooth com port not selected.\r\rCheck Settings in 'Setup'.", "Bluetooth not started", MB_ICONWARNING + MB_OK);
	  }

	  Database::TDBTransaction DBBootTransaction(TDeviceRealPalms::Instance().DBControl);
	  DBBootTransaction.StartTransaction();

	  Kitchen->Initialise(DBBootTransaction);
	  Receipt->Initialise(DBBootTransaction);

	  bool Registered = false;
	  UnicodeString pRegisteredName = "";
	  TDeviceRealPalms::Instance().Registered(&Registered, &pRegisteredName);
	  if (!Registered)
	  {
		 frmDBMod->SetRegCaption("Unregistered");
	  }
	  else
	  {
		 TManagerLogs::Instance().Add("NA", REGISTRATIONLOG, "Application Registered");
		 frmDBMod->SetRegCaption(pRegisteredName);
	  }

	  // Boot Locations
	  frmDBMod->SetCaption("Loading Virtual Terminal...");
	  // Make sure this terminal is updated in DB.
	  // Also aligns TConnect Table.

	  // Set up Header and Footer Info.
	  frmDBMod->SetCaption("Loading Users...");

	  Invoice->Initialise();
	  TComms::Instance().Initialise(DBBootTransaction);
	  ManagerDiscount->Initialise();

	  TManagerPhysicalPrinter ManagerPhysicalPrinter;
	  ManagerPhysicalPrinter.Initialise(DBBootTransaction, TDeviceRealPalms::Instance().ID.DeviceKey, TDeviceRealPalms::Instance().ID.ComputerName);

	  ManagerVirtualPrinter->Initialise();
	  ManagerVirtualPrinter->InitialisePrinters(DBBootTransaction, TDeviceRealPalms::Instance().ID.DeviceKey, TDeviceRealPalms::Instance().ID.ComputerName);
	  ManagerDockets->Initialise(TDeviceRealPalms::Instance().ID.Name, TDeviceRealPalms::Instance().ID.DeviceKey);

	  bool KitchenRegistered = false;
	  TDeviceRealPalms::Instance().RegisteredModule(&KitchenRegistered, eRegKitchenScreen);
	  if (KitchenRegistered)
	  {
		 TDeviceRealPalms::Instance().KitchenMod->Enabled = true;
		 TManagerLogs::Instance().Add("NA", REGISTRATIONLOG, "Kitchen Mod Registered");
	  }

	  frmDBMod->SetCaption("Loading Headers & Footers...");
	  TDeviceRealPalms::Instance().LoadHdrFtr();
	  frmDBMod->SetCaption("Loading Menus...");

	  TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
	  Request->Broadcast = false;
	  TDeviceRealPalms::Instance().Menus->GetCurrentMenus(DBBootTransaction, Request);
	  TDeviceRealPalms::Instance().Menus->UpdateMenuChanged(TDeviceRealPalms::Instance().DBControl, Request);
	  // TDeviceRealPalms::Instance().Menus->MenuChanged(Request);
	  delete Request;
	  // Swap in the Menus.
	  TDeviceRealPalms::Instance().Menus->ToggleMenus(DBBootTransaction);
	  if (TDeviceRealPalms::Instance().Menus->MenuFood == NULL)
	  {
		 btnChangeFood->Caption = "None";
	  }
	  else
	  {
		 btnChangeFood->Caption = TDeviceRealPalms::Instance().Menus->MenuFood->MenuName;
	  }

	  if (TDeviceRealPalms::Instance().Menus->MenuDrinks == NULL)
	  {
		 btnChangeDrinks->Caption = "None";
	  }
	  else
	  {
		 btnChangeDrinks->Caption = TDeviceRealPalms::Instance().Menus->MenuDrinks->MenuName;
	  }

	  TDeviceRealPalms::Instance().LoadPalms(DBBootTransaction);

	  DBBootTransaction.Commit();
	  TManagerLogs::Instance().Enabled = true;
	  tiMenuUpdateRequired->Enabled = true;
   }
   __finally
   {
	  frmDBMod->Close();
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
  TManagerLogs::Instance().Enabled = false;
  Application->ProcessMessages();
  TManagerLogs::Instance().ClearAll();

}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::btnChangeFoodClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
   DBTransaction.StartTransaction();
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT * FROM MENU WHERE MENU_TYPE = :MENU_TYPE AND PALMABLE = :PALMABLE";
   IBInternalQuery->ParamByName("MENU_TYPE")->AsInteger = int(eFoodMenu);
   IBInternalQuery->ParamByName("PALMABLE")->AsString = "T";
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->RecordCount == 0)
   {
	  MessageBox("There are no Food menus Available! \rDid you set the 'Available on Palm' check box in office?", "Error",
		 MB_ICONERROR + MB_OK);
   }
   else
   {
	  std::auto_ptr <TfrmSelectActiveMenus> frmSelectActiveMenus(new TfrmSelectActiveMenus(TDeviceRealPalms::Instance().Menus, DBTransaction, eFoodMenu));
	  if (frmSelectActiveMenus->ShowModal() == mrOk)
	  {
		 std::auto_ptr <TfrmProcessing> Processing(TfrmProcessing::Create <TfrmProcessing> (this));
		 try
		 {
			Processing->Message = "Updating Menus";
			Processing->Show();
			TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
			try
			{
			   if (TDeviceRealPalms::Instance().Menus->MenuFood)
			   {
				  Request->Menu_Names[TDeviceRealPalms::Instance().Menus->MenuFood->MenuName] = eMenuRemove;
			   }
			   Request->Menu_Names[frmSelectActiveMenus->MenuName] = eMenuAddReplace;
			   TDeviceRealPalms::Instance().Menus->UpdateMenuChanged(TDeviceRealPalms::Instance().DBControl, Request);

			   TDeviceRealPalms::Instance().Menus->ToggleMenus(DBTransaction);

			   if (TDeviceRealPalms::Instance().Menus->MenuDrinks == NULL)
			   {
				  btnChangeDrinks->Caption = "None";
			   }
			   else
			   {
				  btnChangeDrinks->Caption = TDeviceRealPalms::Instance().Menus->MenuDrinks->MenuName;
			   }
			   if (TDeviceRealPalms::Instance().Menus->MenuFood == NULL)
			   {
				  btnChangeFood->Caption = "None";
			   }
			   else
			   {
				  btnChangeFood->Caption = TDeviceRealPalms::Instance().Menus->MenuFood->MenuName;
			   }
			}
			__finally
			{
			   delete Request;
			}
		 }
		 __finally
		 {
			Processing->Close();
		 }
	  }
   }
   DBTransaction.Commit();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnChangeDrinksClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
   DBTransaction.StartTransaction();
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT * FROM MENU WHERE MENU_TYPE = :MENU_TYPE AND PALMABLE = :PALMABLE";
   IBInternalQuery->ParamByName("MENU_TYPE")->AsInteger = int(eDrinksMenu);
   IBInternalQuery->ParamByName("PALMABLE")->AsString = "T";
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->RecordCount == 0)
   {
	  MessageBox("There are no Beverage menus Available! \rDid you set the 'Available on Palm' check box in office?", "Error",
		 MB_ICONERROR + MB_OK);
   }
   else
   {
	  std::auto_ptr <TfrmSelectActiveMenus> frmSelectActiveMenus(new TfrmSelectActiveMenus(TDeviceRealPalms::Instance().Menus, DBTransaction,
			eDrinksMenu));
	  if (frmSelectActiveMenus->ShowModal() == mrOk)
	  {
		 std::auto_ptr <TfrmProcessing> Processing(TfrmProcessing::Create <TfrmProcessing> (this));
		 try
		 {
			Processing->Message = "Updating Menus";
			Processing->Show();
			TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
			try
			{
			   if (TDeviceRealPalms::Instance().Menus->MenuDrinks)
			   {
				  Request->Menu_Names[TDeviceRealPalms::Instance().Menus->MenuDrinks->MenuName] = eMenuRemove;
			   }
			   Request->Menu_Names[frmSelectActiveMenus->MenuName] = eMenuAddReplace;
			   TDeviceRealPalms::Instance().Menus->UpdateMenuChanged(TDeviceRealPalms::Instance().DBControl, Request);

			   TDeviceRealPalms::Instance().Menus->ToggleMenus(DBTransaction);

			   if (TDeviceRealPalms::Instance().Menus->MenuDrinks == NULL)
			   {
				  btnChangeDrinks->Caption = "None";
			   }
			   else
			   {
				  btnChangeDrinks->Caption = TDeviceRealPalms::Instance().Menus->MenuDrinks->MenuName;
			   }
			   if (TDeviceRealPalms::Instance().Menus->MenuFood == NULL)
			   {
				  btnChangeFood->Caption = "None";
			   }
			   else
			   {
				  btnChangeFood->Caption = TDeviceRealPalms::Instance().Menus->MenuFood->MenuName;
			   }
			}
			__finally
			{
			   delete Request;
			}
		 }
		 __finally
		 {
			Processing->Close();
		 }
	  }
   }
   DBTransaction.Commit();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
   FormResize(NULL);
   Init();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::tmLogsTimer(TObject *Sender)
{
	TManagerLogs::Instance().Process();

   if (TDeviceRealPalms::Instance().Menus->MenuDrinks == NULL)
   {
	  btnChangeDrinks->Caption = "None";
   }
   else
   {
	  btnChangeDrinks->Caption = TDeviceRealPalms::Instance().Menus->MenuDrinks->MenuName;
   }
   if (TDeviceRealPalms::Instance().Menus->MenuFood == NULL)
   {
	  btnChangeFood->Caption = "None";
   }
   else
   {
	  btnChangeFood->Caption = TDeviceRealPalms::Instance().Menus->MenuFood->MenuName;
   }

   // Happy Hour
   if (TGlobalSettings::Instance().ForceHappyHour && !TGlobalSettings::Instance().InHappyHour)
   {
	  TGlobalSettings::Instance().InHappyHour = true;
	  btnHappyHour->Color = clGreen;
   }
   else
   {
	  if (!TGlobalSettings::Instance().HappyHourExempt && !TGlobalSettings::Instance().ForceHappyHour)
	  {
		 TDateTime CurrentTime = Time();
		 bool InTimeSpan;
		 int CheckNextDay = 0; // Happy hour goes past midnight, therefore the next day should be checked.
		 // Subtracted from current date so either 0 or 1.
		 if (TGlobalSettings::Instance().HappyHourStart < TGlobalSettings::Instance().HappyHourFinish)
		 {
			InTimeSpan = (CurrentTime >= TGlobalSettings::Instance().HappyHourStart && CurrentTime < TGlobalSettings::Instance().HappyHourFinish);
		 }
		 else
		 {
			if (CurrentTime >= TGlobalSettings::Instance().HappyHourStart)
			{
			   InTimeSpan = true;
			}
			else if (CurrentTime <= TGlobalSettings::Instance().HappyHourFinish)
			{
			   InTimeSpan = true;
			   CheckNextDay = 1;
			}
			else
			{
			   InTimeSpan = false;
			}
		 }

		 if (TGlobalSettings::Instance().HappyHourDays[(DayOfWeek(Date() - CheckNextDay)) - 1])
		 {
			if (InTimeSpan)
			{
			   TGlobalSettings::Instance().InHappyHour = true;
			   btnHappyHour->Color = clGreen;
			}
			else
			{
			   TGlobalSettings::Instance().InHappyHour = false;
			   btnHappyHour->Color = clPurple;
			}
		 }
		 else
		 {
			TGlobalSettings::Instance().InHappyHour = false;
			btnHappyHour->Color = clPurple;
		 }
	  }
   }

   // Refresh Reg Palm List
   if (frmPalmRegistration != NULL)
   {
	  frmPalmRegistration->UpdateDisplay();
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnRegisterPalmsClick(TObject *Sender)
{
   TMMContactInfo TempUserInfo;
   Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
   TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckSetup);
   DBTransaction.Commit();
   if (Result == lsAccepted)
   {
	  if (frmPalmRegistration == NULL)
	  {
		 Application->CreateForm(__classid(TfrmPalmRegistration), &frmPalmRegistration);
	  }
	  frmPalmRegistration->ShowModal();
	  delete frmPalmRegistration;
	  frmPalmRegistration = NULL;
   }
   else if (Result == lsDenied)
   {
	  MessageBox("You do not have access to Palm Registration.", "Error", MB_OK + MB_ICONERROR);
   }
   else if (Result == lsPINIncorrect)
   {
	  MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSetupClick(TObject *Sender)
{
   TMMContactInfo TempUserInfo;
   Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
   TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckSetup);
   DBTransaction.Commit();
   if (Result == lsAccepted)
   {
	  try
	  {
		 if (frmPalmSetup == NULL)
		 {
			Application->CreateForm(__classid(TfrmPalmSetup), &frmPalmSetup);
		 }
		 frmPalmSetup->ShowModal();
	  }
	  __finally
	  {
		 delete frmPalmSetup;
		 frmPalmSetup = NULL;
	  }
   }
   else if (Result == lsDenied)
   {
	  MessageBox("You do not have access to System Setup.", "Error", MB_OK + MB_ICONERROR);
   }
   else if (Result == lsPINIncorrect)
   {
	  MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnQuitClick(TObject *Sender)
{
   TMMContactInfo TempUserInfo;
   Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
   TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckQuit);
   DBTransaction.Commit();
   if (Result == lsAccepted)
   {
	  Close();
   }
   else if (Result == lsDenied)
   {
	  MessageBox("You do not have access to Quit.", "Error", MB_OK + MB_ICONERROR);
   }
   else if (Result == lsPINIncorrect)
   {
	  MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   CanClose = (MessageBox("This will stop Palms from working! Do you wish to continue?", "Warning", MB_ICONWARNING + MB_OKCANCEL) == mrOk);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
   if (Tag != Screen->Width)
   {
	  int Temp = Tag;
	  Tag = Screen->Width;

	  ScaleBy(Screen->Width, Temp);
   }
   lbeVersion->Left = Panel3->Width - lbeVersion->Width - 2;
   lbeVersion->Top = Panel3->Height - lbeVersion->Height - 2;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::WMDisplayChange(TWMDisplayChange& Message)
{
   FormResize(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnMinimiseClick(TObject *Sender)
{
   Application->Minimize();
}
// ---------------------------------------------------------------------------

__fastcall TfrmMain::~TfrmMain()
{
   ::Sleep(5000); // Stops Win 98 closeing to fast and causeing an Access Violation.
   // Dont ask me how or why this seems to fix it.
}

// ---------------------------------------------------------------------------

void __fastcall TfrmMain::lbeVersionClick(TObject *Sender)
{
#ifdef _DEBUG
   frmDebug->Show();
#endif
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::btnHappyHourClick(TObject *Sender)
{
   if (TGlobalSettings::Instance().InHappyHour)
   {
	  if (MessageBox("This will force Palms out of Happy Hour Pricing! Do you wish to continue?", "Warning",
			MB_ICONWARNING + MB_OKCANCEL) == mrOk)
	  {
		 TGlobalSettings::Instance().ForceHappyHour = false;
		 TGlobalSettings::Instance().InHappyHour = false;
		 btnHappyHour->Color = clPurple;
	  }
   }
   else
   {
	  if (MessageBox("This will force Palms into Happy Hour Pricing! Do you wish to continue?", "Warning",
			MB_ICONWARNING + MB_OKCANCEL) == mrOk)
	  {
		 TGlobalSettings::Instance().ForceHappyHour = true;
		 TGlobalSettings::Instance().InHappyHour = true;
		 btnHappyHour->Color = clGreen;
	  }
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::tiMenuUpdateRequiredTimer(TObject *Sender)
{
   // --------------------------------- Check All Menus are Current.
   try
   {
	  tiMenuUpdateRequired->Enabled = false;

	  Database::TDBTransaction DBBootTransaction(TDeviceRealPalms::Instance().DBControl);
	  DBBootTransaction.StartTransaction();

	  TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
	  Request->Broadcast = false;
	  TDeviceRealPalms::Instance().Menus->GetCurrentMenus(DBBootTransaction, Request);
	  TDeviceRealPalms::Instance().Menus->UpdateMenuChanged(TDeviceRealPalms::Instance().DBControl, Request);
	  TDeviceRealPalms::Instance().Menus->ToggleMenus(DBBootTransaction);
	  delete Request;
	  // Swap in the Menus.
	  DBBootTransaction.Commit();
	  tiMenuUpdateRequired->Enabled = true;
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  MessageBox("Unable to check Menus are up to date.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);

   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmMain::tchbtn1MouseClick(TObject *Sender)
{
//   TMMContactInfo TempUserInfo;
	TMMContactInfo *TempUserInfo = new TMMContactInfo();
    delete TempUserInfo;
//   std::auto_ptr <TMMContactInfo> Staff(new TMMContactInfo());

}
//---------------------------------------------------------------------------

