// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "MMRegistry.h"
#include "Registration.h"
#include "Printers.hpp"
#include "EnterKey.h"
#include "MMMessageBox.h"

#define MMBaseKey "\\Software\\IQWorks\\MenuMate\\"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;

// ---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
   // Ask Window$ nicely for some pretty icons for our tree.
   SHFILEINFO sfi;
   TIcon *Icon = new TIcon;
   try
   {
	  // Get the closed folder icon. The path is fictitious but shows it is a folder that is required.
	  if (SUCCEEDED(SHGetFileInfo("C:\\DIRECTORY", FILE_ATTRIBUTE_DIRECTORY, &sfi, sizeof(sfi),
			   SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON)))
	  {
		 Icon->Handle = sfi.hIcon;
		 imlTreeImages->AddIcon(Icon);
	  }
	  // Get the open folder icon.
	  if (SUCCEEDED(SHGetFileInfo("C:\\DIRECTORY", FILE_ATTRIBUTE_DIRECTORY, &sfi, sizeof(sfi),
			   SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON)))
	  {
		 Icon->Handle = sfi.hIcon;
		 imlTreeImages->AddIcon(Icon);
	  }

	  // Get the icon for the favorites folder.
	  LPMALLOC lpMalloc = NULL;
	  if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
	  {
		 LPITEMIDLIST lpidl;
		 if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_FAVORITES, &lpidl)))
		 {
			if (SUCCEEDED(SHGetFileInfo((LPCTSTR)lpidl, 0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_ICON | SHGFI_SMALLICON)))
			{
			   Icon->Handle = sfi.hIcon;
			   imlTreeImages->AddIcon(Icon);
			}
			lpMalloc->Free(lpidl);
		 }
		 else
		 {
			// Use the normal folder as a fallback.
			imlTreeImages->AddIcon(Icon);
		 }
		 lpMalloc->Release();
	  }
	  // Get the icon for the rubbish folder.
	  lpMalloc = NULL;
	  if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
	  {
		 LPITEMIDLIST lpidl;
		 if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_BITBUCKET, &lpidl)))
		 {
			if (SUCCEEDED(SHGetFileInfo((LPCTSTR)lpidl, 0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_ICON | SHGFI_SMALLICON)))
			{
			   Icon->Handle = sfi.hIcon;
			   imlTreeImages->AddIcon(Icon);
			}
			lpMalloc->Free(lpidl);
		 }
		 else
		 {
			// Use the normal folder as a fallback.
			imlTreeImages->AddIcon(Icon);
		 }
		 lpMalloc->Release();
	  }
   }
   __finally
   {
	  delete Icon;
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
   pcRegistration->ActivePage = tsConnection;
   pcRegistrationChange(NULL);
   chbKitchenModule->Checked = false;
   MenuMateModules = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::cbCustomerNameChange(TObject *Sender)
{
   CompanyName = ((TComboBox*)Sender)->Text;
}

// ---------------------------------------------------------------------------
bool TfrmMain::OpenDatabase(AnsiString DBName)
{
   bool Opened = false;
   Screen->Cursor = crHourGlass;
   try
   {
	  try
	  {
		 DBControl.Disconnect();
		 DBControl.Init(Database::TDBSettings("", DBName, true));
		 DBControl.Connect();
		 Opened = true;
	  }
	  catch(EIBError & E)
	  {
		 Application->ShowException(&E);
	  }
   }
   __finally
   {
	  Screen->Cursor = crDefault;
   }
   return Opened;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::edPOSLicencesChange(TObject *Sender)
{
   if (((TEdit*)Sender)->Text != "")
   {
	  POSLicences = StrToInt(((TEdit*)Sender)->Text);
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::edPalmLicencesChange(TObject *Sender)
{
   if (((TEdit*)Sender)->Text != "")
   {
	  PalmLicences = StrToInt(((TEdit*)Sender)->Text);
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::chbKitchenModuleClick(TObject *Sender)
{
   if (chbKitchenModule->Checked)
   {
	  MenuMateModules |= eRegKitchenScreen;
   }
   else
   {
	  MenuMateModules &= ~eRegKitchenScreen;
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnUnregisterClick(TObject *Sender)
{
   TRegInfo RegInfo(DBControl);
   if (tvLicences->Selected && tvLicences->Selected->Level == 2)
   {
	  TMachineInfo *MachineInfo = (TMachineInfo*)tvLicences->Selected->Data;
	  if (MachineInfo)
	  {
		 RegInfo.Unregister(MachineInfo); // ->HardwareKey);
	  }
   }
   pcRegistrationChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnRemoveKeyClick(TObject *Sender)
{
   TRegInfo RegInfo(DBControl);
   if (tvLicences->Selected && tvLicences->Selected->Level == 2)
   {
	  TMachineInfo *MachineInfo = (TMachineInfo*)tvLicences->Selected->Data;
	  if (MachineInfo)
	  {
		 RegInfo.Delete(MachineInfo);
	  }
   }
   pcRegistrationChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnPrintClick(TObject *Sender)
{
   UnicodeString Message = "'" + cbCustomerName->Text + "' will appear in the header of customer receipts.\r" "Do you wish to continue?";
   if (MessageBox(Message.c_str(), UnicodeString("Continue?").c_str(), MB_OKCANCEL + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDOK)
   {
	  if (PrintDialog->Execute())
	  {
		 TStringList *StringList = new TStringList;
		 try
		 {
			StringList->Add(Now().FormatString("dddd, d mmmm yyyy HH:nn:ss"));
			StringList->Add("");
			StringList->Add("Company Name: " + cbCustomerName->Text);
			StringList->Add("POS Licences: " + edPOSLicences->Text);
			StringList->Add("Palm Licences: " + edPalmLicences->Text);
			StringList->Add("Office Licences: " + edOffice->Text);
			StringList->Add("ChefMate Modules: " + AnsiString(chbKitchenModule->Checked ? "Y" : "N"));
			StringList->Add("Time Tracking Modules: " + AnsiString(chbTimeTracking->Checked ? "Y" : "N"));
			StringList->Add("Rooms Modules: " + AnsiString(chbRoomsModule->Checked ? "Y" : "N"));
			StringList->Add("Membership Modules: " + AnsiString(cbMembership->Checked ? "Y" : "N"));
			StringList->Add("MYOB Modules: " + AnsiString(cbMYOB->Checked ? "Y" : "N"));
			StringList->Add("MenuMate IntaMate: " + AnsiString(cbIntaMate->Checked ? "Y" : "N"));
			StringList->Add("");

			Printer()->Canvas->Font->Name = "MS Sans Serif";
			Printer()->Canvas->Font->Size = 10;
			int MarginLeft = Printer()->Canvas->Font->Size * 4;
			int MarginTop = MarginLeft;
			int Max = Printer()->PageHeight - (MarginTop * 2);

			Printer()->BeginDoc();
			try
			{
			   int Y = MarginTop;
			   for (int i = 0; i < StringList->Count; i++)
			   {
				  Printer()->Canvas->TextOut(MarginLeft, Y, StringList->Strings[i]);
				  Y = Y + Printer()->Canvas->TextHeight("A");
				  if (Y >= Max)
				  {
					 if (i < StringList->Count - 1)
					 {
						Printer()->NewPage();
					 }
					 Y = MarginTop;
				  }
				  Application->ProcessMessages(); // for Printer.Abort();
			   }
			   StringList->Clear();
			   GetKeys(StringList, true);
			   Printer()->Canvas->Font->Name = "Courier New";
			   Printer()->Canvas->Font->Size = 12;
			   for (int i = 0; i < StringList->Count; i++)
			   {
				  Printer()->Canvas->TextOut(MarginLeft, Y, StringList->Strings[i]);
				  Y = Y + Printer()->Canvas->TextHeight("A");
				  if (Y >= Max)
				  {
					 if (i < StringList->Count - 1)
					 {
						Printer()->NewPage();
					 }
					 Y = MarginTop;
				  }
				  Application->ProcessMessages(); // for Printer.Abort();
			   }
			}
			__finally
			{
			   Printer()->EndDoc();
			}
		 }
		 __finally
		 {
			delete StringList;
		 }
	  }
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSaveClick(TObject *Sender)
{
   if (SaveDialog->Execute())
   {
	  TStringList *StringList = new TStringList;
	  try
	  {
		 StringList->Add(Now().FormatString("dddd, d mmmm yyyy HH:nn:ss"));
		 StringList->Add("");
		 StringList->Add("Company Name: " + cbCustomerName->Text);
		 StringList->Add("POS Licences: " + edPOSLicences->Text);
		 StringList->Add("Palm Licences: " + edPalmLicences->Text);
		 StringList->Add("Office Licences: " + edOffice->Text);
		 StringList->Add("Time Trial : N" /*+ AnsiString(cbExpiry->Checked ? "Y" : "N")*/);
		 StringList->Add("ChefMate Modules: " + AnsiString(chbKitchenModule->Checked ? "Y" : "N"));
		 StringList->Add("Time Tracking Modules: " + AnsiString(chbTimeTracking->Checked ? "Y" : "N"));
		 StringList->Add("Rooms Modules: " + AnsiString(chbRoomsModule->Checked ? "Y" : "N"));
		 StringList->Add("Phoenix Modules: " + AnsiString(cbPhoenixHS->Checked ? "Y" : "N"));
		 StringList->Add("Membership Modules: " + AnsiString(cbMembership->Checked ? "Y" : "N"));
		 StringList->Add("Intergrated Eftpos: " + AnsiString(cbIntergratedEftpos->Checked ? "Y" : "N"));
		 StringList->Add("MYOB Modules: " + AnsiString(cbMYOB->Checked ? "Y" : "N"));
		 StringList->Add("MenuMate Direct: " + AnsiString(cbIntaMate->Checked ? "Y" : "N"));
		 StringList->Add("");
		 GetKeys(StringList);
		 StringList->SaveToFile(SaveDialog->FileName);
	  }
	  __finally
	  {
		 delete StringList;
	  }
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::rbUseMenuMateSettingsClick(TObject *Sender)
{
   UnicodeString DatabasePath = "";
   RegistryRead(MMBaseKey + UnicodeString("Database"), UnicodeString("DatabasePath"), DatabasePath);

   UnicodeString InterbaseIP = "";
   RegistryRead(MMBaseKey + UnicodeString("Database"), UnicodeString("InterbaseIP"), InterbaseIP);

   edConnectionName->Text = InterbaseIP + ":" + DatabasePath;

   cbOfficeConnections->Items->Clear();
   cbOfficeConnections->Enabled = false;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::rbUseOfficeSettingsClick(TObject *Sender)
{
   cbOfficeConnections->Items->Clear();
   cbOfficeConnections->Enabled = true;
   RegistryGetKeys(MMBaseKey "Office", cbOfficeConnections->Items);
   if (cbOfficeConnections->Items->Count == 0)
   {
	  edConnectionName->Text = "";
	  rbUseMenuMateSettings->Checked = true;
	  cbOfficeConnections->Enabled = false;
   }
   else if (cbOfficeConnections->Items->Count == 1)
   {
	  cbOfficeConnections->Enabled = false;
	  cbOfficeConnections->ItemIndex = 0;
	  cbOfficeConnectionsChange(NULL);
   }
   else if (cbOfficeConnections->Items->Count > 1)
   {
	  cbOfficeConnections->Enabled = true;
	  cbOfficeConnections->ItemIndex = 0;
	  cbOfficeConnectionsChange(NULL);
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::cbOfficeConnectionsChange(TObject *Sender)
{
   UnicodeString DBName;
   RegistryRead(MMBaseKey + UnicodeString("Office\\" + cbOfficeConnections->Text), UnicodeString("MMDataFile"), DBName);
   edConnectionName->Text = DBName;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::pcRegistrationChange(TObject *Sender)
{
   if (pcRegistration->ActivePage == tsConnection)
   {
	  tsIndividualLicences->TabVisible = false;
	  tsMultiLicences->TabVisible = false;
	  if (rbUseMenuMateSettings->Checked)
	  {
		 rbUseMenuMateSettingsClick(NULL);
	  }
	  else if (rbUseOfficeSettings->Checked)
	  {
		 rbUseOfficeSettingsClick(NULL);
	  }
   }
   else if (pcRegistration->ActivePage == tsInformation)
   {
	  tsIndividualLicences->TabVisible = true;
	  tsMultiLicences->TabVisible = true;
   }
   else if (pcRegistration->ActivePage == tsIndividualLicences)
   {
	  tvLicences->Items->Clear();

	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "Select " "MachineKey," "Product," "Terminal_Name," "Company, " "RegKey," "POS_Licences,"
		  "Palm_Licences, " "Office_Licences, " "Expiry " "From " "Registration " "Order By " "Product," "Terminal_Name," "MachineKey";

	  AnsiString LastProduct = "";
	  AnsiString LastTerminalName = "";
	  TTreeNode *ProductNode;
	  TTreeNode *TerminalNode;
	  TTreeNode *CustomerKeyNode;
	  TRegInfo RegInfo(DBControl);
	  int Index = 0;

	  MenuMateCount = 0;
	  OfficeCount = 0;
	  int PalmCount = 0;
	  int *CurrentLicence = NULL;

	  for (IBInternalQuery->ExecQuery(); !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 TMachineInfo *MachineInfo = new TMachineInfo;
		 MachineInfo->CompanyName = CompanyName;
		 MachineInfo->TerminalName = IBInternalQuery->FieldByName("Terminal_Name")->AsString;
		 MachineInfo->HardwareKey = IBInternalQuery->FieldByName("MachineKey")->AsString;
		 MachineInfo->RegistrationKey = IBInternalQuery->FieldByName("RegKey")->AsString;
		 MachineInfo->ProductName = IBInternalQuery->FieldByName("Product")->AsString;
		 MachineInfo->Expiry = 0;/*dtpExpiry->Date;*/
		 MachineInfo->POSLicences = POSLicences;
		 MachineInfo->PalmLicences = PalmLicences;
		 MachineInfo->OfficeLicences = OfficeLicences;
		 MachineInfo->MenuMateModules = MenuMateModules;
		 MachineInfo->Index = Index++;

		 if (MachineInfo->ProductName != LastProduct || LastProduct == "")
		 {
			LastProduct = MachineInfo->ProductName;
			ProductNode = tvLicences->Items->Add(NULL, LastProduct);
			ProductNode->ImageIndex = 3;
			ProductNode->SelectedIndex = 4;
			ProductNode->Data = NULL;
			LastTerminalName = "";

			if (LastProduct == "MenuMate")
			{
			   CurrentLicence = &MenuMateCount;
			}
			else if (LastProduct == "Office")
			{
			   CurrentLicence = &OfficeCount;
			}
			else if (LastProduct == "PalmMate")
			{
			   CurrentLicence = &PalmCount;
			}
			else
			{
			   MessageBox("The licences table is corrupt.", "Error", MB_ICONERROR);
			   break;
			}
		 }
		 if (MachineInfo->TerminalName != LastTerminalName || LastTerminalName == "")
		 {
			LastTerminalName = MachineInfo->TerminalName;
			TerminalNode = tvLicences->Items->AddChild(ProductNode, LastTerminalName);
			TerminalNode->ImageIndex = 0;
			TerminalNode->SelectedIndex = 0;
			TerminalNode->Data = NULL;
		 }
		 // AnsiString CustomerKey					= RegInfo.GenerateCustomerKey(MachineInfo, IBQuery->FieldByName("MachineKey")->AsString);
		 MachineInfo->CustomerKey = RegInfo.GenerateCustomerKey(MachineInfo);
		 *CurrentLicence = *CurrentLicence + 1;
		 /* if (MachineInfo->ProductName == "MenuMate")
		 {
		 MachineInfo->ProductID = prMenuMate;
		 }
		 else if (MachineInfo->ProductName == "PalmMate")
		 {
		 MachineInfo->ProductID = prPalmMate;
		 }
		 else if (MachineInfo->ProductName == "Office")
		 {
		 MachineInfo->ProductID = prOffice;
		 } */
		 AnsiString FriendlyCustomerKey = KeyToFriendlyKey(MachineInfo->CustomerKey, MachineInfo->Index);
		 CustomerKeyNode = tvLicences->Items->AddChild(TerminalNode, FriendlyCustomerKey);
		 // KeyToFriendlyKey(MachineInfo->CustomerKey, MachineInfo->ProductID, MachineInfo->Index));
		 // if(RegInfo.Register(MachineInfo, MachineInfo->HardwareKey))// Doesn't change DB if incorrect
		 if (RegInfo.Register(MachineInfo)) // Doesn't change DB if incorrect
		 {
			CustomerKeyNode->ImageIndex = 1;
			CustomerKeyNode->SelectedIndex = 1;
		 }
		 else
		 {
			CustomerKeyNode->ImageIndex = 2;
			CustomerKeyNode->SelectedIndex = 2;
			CustomerKeyNode->MakeVisible();
		 }
		 CustomerKeyNode->Data = MachineInfo;
	  }
	  DBTransaction.Commit();
   }
   else if (pcRegistration->ActivePage == tsMultiLicences)
   {
	  memCustomerKeys->Lines->Clear();
	  memRegKeys->Lines->Clear();
	  if (POSLicences < MenuMateCount)
	  {
		 MessageBox("There are too many MenuMate keys. Please delete those that are not needed or purchase more keys.", "Error",
			MB_ICONERROR);
	  }
	  else if (OfficeLicences < OfficeCount)
	  {
		 MessageBox("There are too many Office keys. Please delete those that are not needed or purchase more keys.", "Error",
			MB_ICONERROR);
	  }
	  else
	  {
		 GetKeys(memCustomerKeys->Lines);
	  }
   }
}

// ---------------------------------------------------------------------------}
void __fastcall TfrmMain::btnEnterKeyClick(TObject *Sender)
{
   tvLicencesDblClick(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::tvLicencesDblClick(TObject *Sender)
{
   if (tvLicences->Selected && tvLicences->Selected->Level == 2)
   {
	  if (tvLicences->Selected->Parent->Parent->Text == "MenuMate")
	  {
		 if (POSLicences < MenuMateCount)
		 {
			MessageBox("There are too many MenuMate keys. Please delete those that are not needed or purchase more keys.", "Error",
			   MB_ICONERROR);
			return;
		 }
	  }
	  else if (tvLicences->Selected->Parent->Parent->Text == "Office")
	  {
		 if (OfficeLicences < OfficeCount)
		 {
			MessageBox("There are too many Office keys. Please delete those that are not needed or purchase more keys.", "Error",
			   MB_ICONERROR);
			return;
		 }
	  }
	  else if (tvLicences->Selected->Parent->Parent->Text != "PalmMate")
	  {
		 MessageBox("The licences table is corrupt.", "Error", MB_ICONERROR);
		 return;
	  }
	  TMachineInfo *MachineInfo = (TMachineInfo*)tvLicences->Selected->Data;
	  frmEnterKey->CustomerKey = MachineInfo->CustomerKey;
	  // frmEnterKey->ProductID		= MachineInfo->ProductID;
	  frmEnterKey->Index = MachineInfo->Index;
	  bool KeyCorrect = false;
	  while (frmEnterKey->ShowModal() == mrOk && KeyCorrect == false)
	  {
		 TRegInfo RegInfo(DBControl);
		 if (MachineInfo)
		 {
			MachineInfo->RegistrationKey = frmEnterKey->RegistrationKey.LowerCase();

			if (RegInfo.Register(MachineInfo))
			{
			   tvLicences->Selected->ImageIndex = 1;
			   tvLicences->Selected->SelectedIndex = 1;
			   KeyCorrect = true;
			}
			else
			{
			   tvLicences->Selected->ImageIndex = 2;
			   tvLicences->Selected->SelectedIndex = 2;
			   MessageBox(
				  "The key you entered is incorrect. Check that you have entered the key correctly and that the number of licences is correct."
				  , "Error", MB_ICONERROR);
			}
		 }
	  }
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::pcRegistrationChanging(TObject *Sender, bool &AllowChange)
{
   if (pcRegistration->ActivePage == tsConnection)
   {
	  if (OpenDatabase(edConnectionName->Text))
	  {
		 cbCustomerName->Items->Clear();
		 if (rbUseOfficeSettings->Checked)
		 {
			cbCustomerName->Items->Add(cbOfficeConnections->Text);
		 }
		 pcRegistration->ActivePage = tsInformation;

		 Database::TDBTransaction DBTransaction(DBControl);
		 DBTransaction.StartTransaction();

		 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "Select Distinct " "Company " "From " "Registration " "Order By " "Company";

		 for (IBInternalQuery->ExecQuery(); !IBInternalQuery->Eof; IBInternalQuery->Next())
		 {
			if (IBInternalQuery->FieldByName("Company")->AsString != "")
			{
			   if (cbCustomerName->Items->IndexOf(IBInternalQuery->FieldByName("Company")->AsString) == -1)
			   {
				  cbCustomerName->Items->Add(IBInternalQuery->FieldByName("Company")->AsString);
			   }
			}
		 }
		 if (cbCustomerName->Items->Count > 0)
		 {
			cbCustomerName->ItemIndex = 0;
			cbCustomerNameChange(cbCustomerName);
		 }

		 DBTransaction.Commit();
	  }
	  else
	  {
		 AllowChange = false;
	  }
   }
   else
   {
	  if (CompanyName == "")
	  {
		 AllowChange = false;
		 MessageBox("Please enter the company name.\This is the name that will appear on receipts.", "Error", MB_ICONERROR);
		 cbCustomerName->SetFocus();
	  }
	  else if (edPOSLicences->Text == "")
	  {
		 AllowChange = false;
		 MessageBox("Please enter the number of POS licences required.", "Error", MB_ICONERROR);
		 edPOSLicences->SetFocus();
	  }
	  else if (edPalmLicences->Text == "")
	  {
		 AllowChange = false;
		 MessageBox("Please enter the number of Palm licences required.", "Error", MB_ICONERROR);
		 edPalmLicences->SetFocus();
	  }
	  else if (edOffice->Text == "")
	  {
		 AllowChange = false;
		 MessageBox("Please enter the number of Office licences required.", "Error", MB_ICONERROR);
		 edOffice->SetFocus();
	  }
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::tvLicencesDeletion(TObject *Sender, TTreeNode *Node)
{
   TMachineInfo *MachineInfo = (TMachineInfo*)Node->Data;
   if (MachineInfo)
   {
	  delete MachineInfo;
   }
}

// ---------------------------------------------------------------------------
void TfrmMain::GetKeys(TStrings *Strings, bool Friendly)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();

   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "Select " "MachineKey," "Product," "Terminal_Name " "From " "Registration " "Order By " "Product,"
	   "Terminal_Name," "MachineKey";

   TRegInfo RegInfo(DBControl);
   int Index = 0;
   for (IBInternalQuery->ExecQuery(); !IBInternalQuery->Eof; IBInternalQuery->Next())
   {
	  TMachineInfo MachineInfo;
	  MachineInfo.CompanyName = CompanyName;
	  MachineInfo.ProductName = IBInternalQuery->FieldByName("Product")->AsString;
	  MachineInfo.TerminalName = IBInternalQuery->FieldByName("Terminal_Name")->AsString;
	  MachineInfo.HardwareKey = IBInternalQuery->FieldByName("MachineKey")->AsString;

	  AnsiString CustomerKey = RegInfo.GenerateCustomerKey(&MachineInfo); // , IBQuery->FieldByName("MachineKey")->AsString);
	  /* int ProductID;
	  if (MachineInfo.ProductName == "MenuMate")
	  {
	  ProductID = prMenuMate;
	  }
	  else if (MachineInfo.ProductName == "PalmMate")
	  {
	  ProductID = prPalmMate;
	  }
	  else if (MachineInfo.ProductName == "Office")
	  {
	  ProductID = prOffice;
	  } */
	  if (Friendly)
	  {
		 Strings->Add(KeyToFriendlyKey(CustomerKey, Index)); // ProductID, Index));
	  }
	  else
	  {
		 // CustomerKey += IntToHex(ProductID, 1) + IntToHex(Index, 2);
		 CustomerKey += IntToHex(Index, 3);
		 Strings->Add(CustomerKey.UpperCase());
	  }
	  Index++;
   }

   DBTransaction.Commit();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnRefreshClick(TObject *Sender)
{
   memCustomerKeys->Lines->Clear();
   GetKeys(memCustomerKeys->Lines);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::btnApplyClick(TObject *Sender)
{
   bool Unsuccessful = false;
   for (int i = 0; i < memRegKeys->Lines->Count; i++)
   {
	  TMachineInfo MachineInfo;
	  AnsiString RegistrationKey = memRegKeys->Lines->Strings[i];
	  if (RegistrationKey.Length() != 35)
	  {
		 Unsuccessful = true;
	  }
	  else if (RegistrationKey != "")
	  {
		 int DBIndex = 0;
		 HexToBin((AnsiString("0") + RegistrationKey.SubString(33, 1)).LowerCase().c_str(), (char*) & DBIndex + 1, 1);
		 HexToBin(RegistrationKey.SubString(34, 2).LowerCase().c_str(), (char*) & DBIndex, 1);

		 Database::TDBTransaction DBTransaction(DBControl);
		 DBTransaction.StartTransaction();

		 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "Select " "MachineKey," "Terminal_Name," "Product " "From " "Registration " "Order By " "Product,"
			 "Terminal_Name," "MachineKey";
		 IBInternalQuery->ExecQuery();
		 for (int i = 0; i < DBIndex; i++)
		 {
			IBInternalQuery->Next();
		 }

		 MachineInfo.RegistrationKey = RegistrationKey.SubString(1, 32).LowerCase();
		 MachineInfo.POSLicences = POSLicences;
		 MachineInfo.PalmLicences = PalmLicences;
		 MachineInfo.MenuMateModules = MenuMateModules;
		 MachineInfo.OfficeLicences = OfficeLicences;
		 MachineInfo.CompanyName = CompanyName;

		 MachineInfo.HardwareKey = IBInternalQuery->FieldByName("MachineKey")->AsString;
		 MachineInfo.TerminalName = IBInternalQuery->FieldByName("Terminal_Name")->AsString;
		 MachineInfo.ProductName = IBInternalQuery->FieldByName("Product")->AsString;
		 MachineInfo.Expiry = 0;/*dtpExpiry->Date;*/

		 TRegInfo RegInfo(DBControl);
		 if (!RegInfo.Register(&MachineInfo))
		 {
			Unsuccessful = true;
		 }
		 DBTransaction.Commit();
	  }
   }
   if (Unsuccessful)
   {
	  MessageBox("At least 1 registration key was incorrect.", "Error", MB_ICONERROR);
   }
   pcRegistration->ActivePage = tsIndividualLicences;
   pcRegistrationChange(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::cbRoomsClick(TObject *Sender)
{
   if (chbRoomsModule->Checked)
   {
	  MenuMateModules |= eRegRooms;
   }
   else
   {
	  MenuMateModules &= ~eRegRooms;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::chbTimeTrackingClick(TObject *Sender)
{
   if (chbTimeTracking->Checked)
   {
	  MenuMateModules |= eRegSaleTurnAround;
   }
   else
   {
	  MenuMateModules &= ~eRegSaleTurnAround;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::cbMYOBClick(TObject *Sender)
{
   if (cbMYOB->Checked)
   {
	  MenuMateModules |= eOfficeMYOB;
   }
   else
   {
	  MenuMateModules &= ~eOfficeMYOB;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::cbMembershipClick(TObject *Sender)
{
   if (cbMembership->Checked)
   {
	  MenuMateModules |= eRegMembers;
   }
   else
   {
	  MenuMateModules &= ~eRegMembers;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::edOfficeChange(TObject *Sender)
{
   if (((TEdit*)Sender)->Text != "")
   {
	  OfficeLicences = StrToInt(((TEdit*)Sender)->Text);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::cbIntaMateClick(TObject *Sender)
{
   if (cbIntaMate->Checked)
   {
	  MenuMateModules |= eIntaMate;
   }
   else
   {
	  MenuMateModules &= ~eIntaMate;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::cbIntergratedEftposClick(TObject *Sender)
{
   if (cbIntergratedEftpos->Checked)
   {
	  MenuMateModules |= eEFTPOS;
   }
   else
   {
	  MenuMateModules &= ~eEFTPOS;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::cbExpiryClick(TObject *Sender)
{
/*   if (cbExpiry->Checked)
   {
	  MenuMateModules |= eExpires;
   }
   else
   {
	  MenuMateModules &= ~eExpires;
   }*/
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::cbPhoenixHSClick(TObject *Sender)
{
   if (cbPhoenixHS->Checked)
   {
	  MenuMateModules |= ePhoenixHotelSystem;
   }
   else
   {
	  MenuMateModules &= ~ePhoenixHotelSystem;
   }
}
// ---------------------------------------------------------------------------
