//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MMMessageBox.h"
#include "enum.h"
#include "Comms.h"
#include "Printing.h"
#include "palmmain.h"
#include "Palm.h"
#include "crctable.h"
#include "Data.h"
#include "MMRegistry.h"
#include "DataLink.h"
#include "Processing.h"
#include "SelectReceipt.h"
#include "TouchKeyboard.h"
#include "SelectActiveMenus.h"
#include "PalmRegistration.h"
#include "PalmSetup.h"
#include "Secure.h"
#include "Clipbrd.hpp"
#include "DbMod.h"
#include "SystemConfig.h"
#include "Version.h"
#include "StockManager.h"
#include "ManagerVariable.h"
#include "Locations.h"
#include "Orders.h"
#include "Invoice.h"
#include "Tabs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchButton"
#pragma link "TouchButton"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
   : TForm(Owner)
{
// TODO 5 -o Michael  -c  Check : Palm Names On receipt.
	RequestClose = false;
}
//---------------------------------------------------------------------------
int TfrmMain::SomeStrings(TStrings *Strings)
{
	int retval = 0;
	for (int i = 1; i <= Strings->Text.Length() ; i ++)
	{
		retval += (int)(Strings->Text[i]);
	}
	return retval;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
   Staff = new TContactStaff();
	ManagerStock = new TManagerStock;
	OrderManager = new TManagerOrder;
   VM = new TManagerVariable;
   Invoice = new TInvoice;
   Tabs = new TTabs();
   if(!CheckRegistered())
	{
      TStringList *Lines = new TStringList;
      try
      {
         Lines->Clear();
         Lines->Add("MenuMate By IQWorks Ltd");
         Lines->Add("Pre-Install Version");
         Lines->Add("NOT TO BE USED IN A COMMERCIAL CONTEXT");
         Lines->Add("PLEASE CONTACT IQWORKS LTD");
         Lines->Add("Phone : 03 343 1202");
         Lines->Add("EMail : Sales@IQWorks.co.nz");
         if(SomeStrings(Lines) != 11963)
         {
            ShowMessage("Exe Checksum Failure");
         }
         Receipt->SetHeaderFooter(Lines,Lines,Lines);
         Kitchen->SetHeaderFooter(Lines,Lines);
      }
      __finally
      {
      	delete Lines;
      }
   }
   else
   {
      TStringList *Receipthdr = new TStringList;
      TStringList *ReceiptPhdr = new TStringList;
      TStringList *Receiptftr = new TStringList;
		try
      {
         if(FileExists(ExtractFilePath(Application->ExeName) + "Receipt.hdr"))
         {
            Receipthdr->LoadFromFile(ExtractFilePath(Application->ExeName) + "Receipt.hdr");
         }
         if(FileExists(ExtractFilePath(Application->ExeName) + "ReceiptP.hdr"))
         {
            ReceiptPhdr->LoadFromFile(ExtractFilePath(Application->ExeName) + "ReceiptP.hdr");
         }
         if(FileExists(ExtractFilePath(Application->ExeName) + "Receipt.ftr"))
         {
            Receiptftr->LoadFromFile(ExtractFilePath(Application->ExeName) + "Receipt.ftr");
         }
         Kitchen->SetHeaderFooter(Receipthdr,Receiptftr);
         Receipt->SetHeaderFooter(Receipthdr,ReceiptPhdr,Receiptftr);
      }
      __finally
      {
         delete Receipthdr;
         delete ReceiptPhdr;
         delete Receiptftr;
      }
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
   delete Staff;
   Staff = NULL;
	delete ManagerStock;
	ManagerStock = NULL;

   delete OrderManager;
   OrderManager = NULL;

   delete VM;
   VM = NULL;

   delete Invoice;
   Invoice = NULL;

   delete Tabs;
   Tabs = NULL;
}
//---------------------------------------------------------------------------

bool TfrmMain::CheckRegistered()
{
   AnsiString Path = ExtractFilePath(Application->ExeName) + "MenuMate.exe";
   if(!FileExists(Path))
   {
      return false;
   }
   // Retrive the CRC.
   unsigned short CurrentCRC = 0;
   unsigned short CRC = 0;
   unsigned char Tild;
   bool retval = false;
   TFileStream *File = new TFileStream(Path,fmOpenRead | fmShareDenyNone);
   try
   {
      long StartPos;
      long EndPos;
      File->Seek(-sizeof(Tild),soFromEnd);
      File->Read(&Tild,sizeof(Tild));
      if(Tild == '~')
      {
         File->Seek(-(sizeof(Tild) + sizeof(EndPos))  ,soFromEnd);
         File->Read(&EndPos,sizeof(EndPos));

         File->Seek(-(sizeof(Tild) + sizeof(EndPos) + sizeof(StartPos)),soFromEnd);
         File->Read(&StartPos,sizeof(StartPos));

         File->Seek(-(sizeof(Tild) + sizeof(EndPos) + sizeof(StartPos) + sizeof(CurrentCRC)),soFromEnd);
         File->Read(&CurrentCRC,sizeof(CurrentCRC));

         unsigned char filebyte = 0;
         File->Seek(StartPos,soFromBeginning);
         File->Read(&filebyte,sizeof(filebyte)); // already read first byte i = 1.
         CRC = (unsigned short) StartCRC(filebyte);
         while(File->Position < EndPos)
         {
            File->Read(&filebyte,sizeof(filebyte));
            CRC = (unsigned short)UpdateCRC(CRC,filebyte);
         }
         if(CurrentCRC == CRC)
         {
            retval = true;
         }
      }
	}
   __finally
   {
   	delete File;
   }
   return retval;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TfrmMain::Init()
{
   frmDBMod->Show();
   frmDBMod->SetCaption("Loading PalmMate...");
   try
   {
     // Boot DB
      bool DBOk = false;
      do
      {
         try
         {
            TDeviceRealTerminal::Instance().OpenDB();
            frmDBMod->SetCaption("Opening Database...");
            DBOk = true;
         }
         catch(Exception &Err)
         {
            TDeviceRealTerminal::Instance().CloseDB();
            DBOk = false;
         }
         if(DBOk == false)
         {
            frmSystemConfig->ShowModal();
            if(frmSystemConfig->ModalResult == mrAbort)
            {
               frmMain->OnCloseQuery = NULL;
               Close();
               return;
            }
         }
      }while(DBOk == false);

		TDeviceRealTerminal::Instance().OS.Initialise();

      if(TDeviceRealTerminal::Instance().OS.MMSoftwareVersion != TDeviceRealTerminal::Instance().OS.MMDBVersion)
      {
	     	MessageBox("This MenuMate version does not match the Database version.\rYou may have to run Parser.Exe", "Database Mismatch", MB_ICONINFORMATION + MB_OK);
         frmMain->OnCloseQuery = NULL;
         Close();
         return;
      }
      lbeVersion->Caption = "Version " + GetFileVersion() + "  ";
      lbeVersion->Left = Panel3->Width - lbeVersion->Width - 2;
      lbeVersion->Top = Panel3->Height - lbeVersion->Height - 2;

      VM->Initialise(TDeviceRealTerminal::Instance().IBDatabase,NULL);

      if(TDeviceRealTerminal::Instance().Locate())
      {
         TDeviceRealTerminal::Instance().Load();
      }
      else
      {
         if(TDeviceRealTerminal::Instance().ID.Location == "")
         {
            MessageBox("Please select a location for this terminal.", "Location Request", MB_ICONINFORMATION + MB_OK);
            do
            {
               frmLocations->ShowModal();
               if(frmLocations->SelectedLocation == "")
               {
                  MessageBox("All termianls must have a location.", "Error", MB_OK + MB_ICONWARNING);
               }
            }while(frmLocations->SelectedLocation == "");
            TDeviceRealTerminal::Instance().ID.Location = frmLocations->SelectedLocation;
            TDeviceRealTerminal::Instance().ID.LocationKey = frmLocations->SelectedLocationKey;
         }
	      TDeviceRealTerminal::Instance().Create();
		}

      // Save off System Config Settings.
		VM->DeviceProfileKey = TDeviceRealTerminal::Instance().ID.ProfileKey;
		if(VM->DeviceProfileKey == 0)
		{
			VM->DeviceProfileKey = VM->SetProfile(eTerminalProfiles,TDeviceRealTerminal::Instance().ID.Name + " PalmMate");
			TDeviceRealTerminal::Instance().SetProfileKey(VM->DeviceProfileKey);
		}

      VM->SetDeviceStr(vmBroadCastIP,TDeviceRealTerminal::Instance().System.BroadcastIP);
      VM->SetDeviceStr(vmStockInterbaseIP,TDeviceRealTerminal::Instance().System.StockInterbaseIP);
      VM->SetDeviceStr(vmStockDatabasePath,TDeviceRealTerminal::Instance().System.StockDatabasePath);
      VM->SetDeviceStr(vmSntpIP,TDeviceRealTerminal::Instance().System.SntpIP);
      TDeviceRealTerminal::Instance().Initialise();
      Kitchen->Initialise();
      Receipt->Initialise();


      bool Registered = false;
      AnsiString pRegisteredName = "";
      TDeviceRealTerminal::Instance().Registered(&Registered,&pRegisteredName);
      if(!Registered)
      {
         frmDBMod->Label2->Visible = false;
      }
      else
      {
         frmDBMod->Label2->Visible = true;
         frmDBMod->lbRegInfo->Caption = pRegisteredName;
      }


      // Boot Locations
      frmDBMod->SetCaption("Loading Virtual Terminal...");
      // Make sure this terminal is updated in DB.
      // Also aligns TConnect Table.
      frmData->Initialise();

      // Set up Header and Footer Info.
      frmDBMod->SetCaption("Loading Users...");
      Staff->Initialise(frmData->IBDatabase,frmData->IBTransaction);
      ManagerStock->Initialise(frmData->IBDatabase,frmData->IBTransaction);
      OrderManager->Initialise(frmData->IBDatabase,frmData->IBTransaction);
      Invoice->Initialise(frmData->IBDatabase,frmData->IBTransaction);
      TDBTab::Initialise(frmData->IBDatabase,frmData->IBTransaction);
      
      frmDBMod->SetCaption("Loading Headers & Footers...");
		TDeviceRealTerminal::Instance().LoadHdrFtr();
      frmDBMod->SetCaption("Loading Menus...");

		TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
		Request->Broadcast = false;
		TDeviceRealTerminal::Instance().Menus.GetCurrentMenus(Request);
		TDeviceRealTerminal::Instance().Menus.MenuChanged(Request);
		delete Request;
		// Swap in the Menus.
		TDeviceRealTerminal::Instance().Menus.ToggleMenus();
		if(TDeviceRealTerminal::Instance().Menus.MenuFood == NULL)
      {
         btnChangeFood->Caption = "None";
      }
      else
      {
         btnChangeFood->Caption = TDeviceRealTerminal::Instance().Menus.MenuFood->MenuName;
      }

      if(TDeviceRealTerminal::Instance().Menus.MenuDrinks == NULL)
      {
         btnChangeDrinks->Caption = "None";
      }
      else
      {
         btnChangeDrinks->Caption = TDeviceRealTerminal::Instance().Menus.MenuDrinks->MenuName;
      }

      TDeviceRealTerminal::Instance().LoadPalms();
      TComms::Instance().Start();
      tmTManagerLogs::Instance().Enabled = true;
   }
   __finally
   {
      frmDBMod->Close();
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
   tmTManagerLogs::Instance().Enabled = false;
   Application->ProcessMessages();
   if(Logs)
	{
		TManagerLogs::Instance().ClearAll();
	}
	if(Comms != NULL)
	{
		if(TComms::Instance().Sync != NULL)
		{
			TComms::Instance().Stop();
		}
	}
	if (frmData->IBTransaction->InTransaction) { frmData->IBTransaction->Commit(); }   
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnChangeFoodClick(TObject *Sender)
{
   if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); }
	frmData->IBQuery->Close();
	frmData->IBQuery->SQL->Text = "SELECT * FROM MENU WHERE MENU_TYPE = :MENU_TYPE";
	frmData->IBQuery->ParamByName("MENU_TYPE")->AsInteger = int(eFoodMenu);
	frmData->IBQuery->Open();
	frmData->IBQuery->FetchAll();
	if(frmData->IBQuery->RecordCount == 0)
	{
		MessageBox("There are no food menus loaded!", "Error", MB_ICONERROR + MB_OK);
	}
	else
	{
		try
      {
         if(frmSelectActiveMenus == NULL)
         {
            Application->CreateForm(__classid(TfrmSelectActiveMenus), &frmSelectActiveMenus);
         }
         if (frmSelectActiveMenus->ShowModal() == mrOk)
         {
            try
            {
               frmProcessing->Message = "Updating Menus";
               frmProcessing->Show();
               TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
               try
               {
                  if(TDeviceRealTerminal::Instance().Menus.MenuFood)
                  {
                     Request->Menu_Names[TDeviceRealTerminal::Instance().Menus.MenuFood->MenuName] = eMenuRemove;
                  }
                  Request->Menu_Names[frmSelectActiveMenus->MenuName] = eMenuAddReplace;
                  TDeviceRealTerminal::Instance().Menus.UpdateMenuChanged(Request);

                  TDeviceRealTerminal::Instance().Menus.SwapInNewMenus();
                  TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eDrinksMenu);
                  TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eFoodMenu);

                  if(TDeviceRealTerminal::Instance().Menus.MenuDrinks == NULL)
                  {
                     btnChangeDrinks->Caption = "None";
                  }
                  else
                  {
                     btnChangeDrinks->Caption = TDeviceRealTerminal::Instance().Menus.MenuDrinks->MenuName;
                  }
                  if(TDeviceRealTerminal::Instance().Menus.MenuFood == NULL)
                  {
                     btnChangeFood->Caption = "None";
                  }
                  else
                  {
                     btnChangeFood->Caption = TDeviceRealTerminal::Instance().Menus.MenuFood->MenuName;
                  }
               }
               __finally
               {
                  delete Request;
               }
            }
            __finally
            {
               frmProcessing->Close();
            }
         }
      }
      __finally
      {
         delete frmSelectActiveMenus;
         frmSelectActiveMenus = NULL;
      }
	}
   if (frmData->IBTransaction->InTransaction) { frmData->IBTransaction->Commit(); }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnChangeDrinksClick(TObject *Sender)
{
	if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); }

	frmData->IBQuery->Close();
	frmData->IBQuery->SQL->Text = "SELECT * FROM MENU WHERE MENU_TYPE = :MENU_TYPE";
	frmData->IBQuery->ParamByName("MENU_TYPE")->AsInteger = int(eDrinksMenu);
	frmData->IBQuery->Open();
	frmData->IBQuery->FetchAll();
	if(frmData->IBQuery->RecordCount == 0)
	{
		MessageBox("There are no drink menus loaded!", "Error", MB_ICONERROR + MB_OK);
	}
	else
	{
		try
      {
         if(frmSelectActiveMenus == NULL)
         {
            Application->CreateForm(__classid(TfrmSelectActiveMenus), &frmSelectActiveMenus);
         }
         if (frmSelectActiveMenus->ShowModal() == mrOk)
         {
            try
            {
               frmProcessing->Message = "Updating Menus";
               frmProcessing->Show();
               TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
               try
               {
                  if(TDeviceRealTerminal::Instance().Menus.MenuDrinks)
                  {
                     Request->Menu_Names[TDeviceRealTerminal::Instance().Menus.MenuDrinks->MenuName] = eMenuRemove;
                  }
                  Request->Menu_Names[frmSelectActiveMenus->MenuName] = eMenuAddReplace;
                  TDeviceRealTerminal::Instance().Menus.UpdateMenuChanged(Request);

                  TDeviceRealTerminal::Instance().Menus.SwapInNewMenus();
                  TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eDrinksMenu);
                  TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eFoodMenu);

                  if(TDeviceRealTerminal::Instance().Menus.MenuDrinks == NULL)
                  {
                     btnChangeDrinks->Caption = "None";
                  }
                  else
                  {
                     btnChangeDrinks->Caption = TDeviceRealTerminal::Instance().Menus.MenuDrinks->MenuName;
                  }
                  if(TDeviceRealTerminal::Instance().Menus.MenuFood == NULL)
                  {
                     btnChangeFood->Caption = "None";
                  }
                  else
                  {
                     btnChangeFood->Caption = TDeviceRealTerminal::Instance().Menus.MenuFood->MenuName;
                  }
               }
               __finally
               {
                  delete Request;
               }
            }
            __finally
            {
               frmProcessing->Close();
            }
         }
      }
      __finally
      {
         delete frmSelectActiveMenus;
         frmSelectActiveMenus = NULL;
      }
	}
	if (frmData->IBTransaction->InTransaction) { frmData->IBTransaction->Commit(); }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	FormResize(NULL);
   Init();
	if (frmMain->RequestClose)
	{
		Close();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tmLogsTimer(TObject *Sender)
{
	if(Logs)
	{
		TManagerLogs::Instance().Process();
	}

   if(TDeviceRealTerminal::Instance().Menus.MenuDrinks == NULL)
   {
      btnChangeDrinks->Caption = "None";
   }
   else
   {
      btnChangeDrinks->Caption = TDeviceRealTerminal::Instance().Menus.MenuDrinks->MenuName;
   }
   if(TDeviceRealTerminal::Instance().Menus.MenuFood == NULL)
   {
      btnChangeFood->Caption = "None";
   }
   else
   {
      btnChangeFood->Caption = TDeviceRealTerminal::Instance().Menus.MenuFood->MenuName;
   }

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnRegisterPalmsClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	TLoginSuccess Result = Staff->Login(TempUserInfo, CheckSetup);
	if (Result == lsAccepted)
	{
      if(frmPalmRegistration == NULL)
      {
         Application->CreateForm(__classid(TfrmPalmRegistration), &frmPalmRegistration);
      }
      frmPalmRegistration->ShowModal();
      delete frmPalmRegistration;
      frmPalmRegistration = NULL;
		frmSecurity->LogOut();
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
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSetupClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	TLoginSuccess Result = Staff->Login(TempUserInfo, CheckSetup);
	if (Result == lsAccepted)
	{
      try
      {
         if(frmPalmSetup == NULL)
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
		frmSecurity->LogOut();
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
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnQuitClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	TLoginSuccess Result = Staff->Login(TempUserInfo, CheckQuit);
	if (Result == lsAccepted)
	{
		Close();
		frmSecurity->LogOut();
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
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	CanClose = (MessageBox("This will stop Palms from working! Do you wish to continue?", "Warning",
			MB_ICONWARNING + MB_OKCANCEL) == mrOk);
}
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
void __fastcall TfrmMain::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnMinimiseClick(TObject *Sender)
{
	Application->Minimize();
}
//---------------------------------------------------------------------------


__fastcall TfrmMain::~TfrmMain()
{
	::Sleep(5000);  // Stops Win 98 closeing to fast and causeing an Access Violation.
   // Dont ask me how or why this seems to fix it.
}

//---------------------------------------------------------------------------


