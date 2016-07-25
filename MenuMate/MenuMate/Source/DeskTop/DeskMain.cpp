//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DeskMain.h"
#include <process.h>
#include "OnlineManager.h"
#include "LanManager.h"
#include "MMRegistry.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "MMMessageBox.h"

#include "ReportDisplay.h"
#include "NetManagerClient.h"
#include "NetMessageKillProc.h"
#include "Version.h"

#define TechSupportIndex "Techsupport.xml"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner), StringList(new TStringList),ManagerNet(new TManagerNet())
{
	Application->OnIdle = IdleHandler;
   RetrieveNamesThread = new TRetrieveNamesThread(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
   RetrieveNamesThread->ComputerNames = StringList.get();
   RetrieveNamesThread->FreeOnTerminate = true;
   RetrieveNamesThread->OnTerminate = RetrieveNamesThreadTerminate;
   RetrieveNamesThread->Resume();

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::IdleHandler(TObject *Sender, bool &Done)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnStartClick(TObject *Sender)
{
   std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(Screen->ActiveForm));

   TVerticalSelection Item;
   Item.Title = "Back";
   Item.Properties["Color"] = clGreen;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   Item.Title = "Start";
   Item.Properties["Color"] = clNavy;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   Item.Title = "Task Manager";
   Item.Properties["Color"] = clSkyBlue;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   Item.Title = "OnScreen Keyboard";
   Item.Properties["Color"] = clOlive;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   Item.Title = "Windows Explorer";
   Item.Properties["Color"] = clPurple;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   Item.Title = "Reboot";
   Item.Properties["Color"] = clMaroon;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   Item.Title = "Shutdown";
   Item.Properties["Color"] = clRed;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   SelectionForm->ShowModal();

	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem))
	{
		if(SelectedItem.Title == "Start")
		{
        SendMessage(frmMain->Handle,WM_SYSCOMMAND,SC_TASKLIST,0);      
		}
		else if(SelectedItem.Title == "Task Manager")
		{
         char Path[MAX_PATH+1];
         ZeroMemory(Path,MAX_PATH+1);
         GetSystemDirectory(Path,MAX_PATH);
         AnsiString TaskPath(Path);
         TaskPath += "\\taskmgr.exe";
         ShellExecute(Handle,"open",TaskPath.c_str(),NULL,NULL,SW_SHOWNORMAL	);
		}
		else if(SelectedItem.Title == "OnScreen Keyboard")
		{
         char Path[MAX_PATH+1];
         ZeroMemory(Path,MAX_PATH+1);
         GetSystemDirectory(Path,MAX_PATH);
         AnsiString TaskPath(Path);
         TaskPath += "\\osk.exe";
         ShellExecute(Handle,"open",TaskPath.c_str(),NULL,NULL,SW_SHOWNORMAL	);
		}
		else if(SelectedItem.Title == "Windows Explorer")
		{
         AnsiString TaskPath = "C:\\Program Files\\MenuMate\\";
         ShellExecute(Handle,"explore",TaskPath.c_str(),NULL,NULL,SW_SHOWNORMAL	);
		}
		else if(SelectedItem.Title == "Shutdown")
		{
      	UINT uFlags = EWX_SHUTDOWN | EWX_FORCE;// | EWX_POWEROFF;
      	ShutDown(uFlags);

		}
		else if(SelectedItem.Title == "Reboot")
		{
      	UINT uFlags = EWX_SHUTDOWN | EWX_REBOOT | EWX_FORCE;// | EWX_FORCE | EWX_POWEROFF;
      	ShutDown(uFlags);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCloseClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

bool TfrmMain::ShutDown(UINT uFlags)
{
	if (Win32Platform == VER_PLATFORM_WIN32_WINDOWS)
	{
		if (!ExitWindowsEx(uFlags, NULL))
		{
			ShowLastErr();
			return false;
		}
		return true;
	}
	else if (Win32Platform == VER_PLATFORM_WIN32_NT)
	{
		HANDLE hToken;
		if (!OpenProcessToken(	GetCurrentProcess(),
										TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
										&hToken))
		{
			return false;
		}
		if (!SetPrivilege(hToken, SE_SHUTDOWN_NAME, true))
		{
			ShowLastErr();
			CloseHandle(hToken);
			return false;
		}
		bool Success = true;
		if (!ExitWindowsEx(uFlags, NULL))
		{
			ShowLastErr();
			Success = false;
		}
		SetPrivilege(hToken, SE_SHUTDOWN_NAME, false);
		CloseHandle(hToken);
		return Success;
	}
	else return false;
}
//---------------------------------------------------------------------------
bool TfrmMain::SetPrivilege(HANDLE hToken, LPCTSTR Privilege, bool bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	TOKEN_PRIVILEGES tpPrevious;
	DWORD cbPrevious = sizeof(TOKEN_PRIVILEGES);
	LUID luid;
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid ))
	{
		return false;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = 0;
	AdjustTokenPrivileges(	hToken,
									false,
									&tp,
									sizeof(TOKEN_PRIVILEGES),
									&tpPrevious,
									&cbPrevious);
	if (GetLastError() != ERROR_SUCCESS)
	{
		return false;
	}
	tpPrevious.PrivilegeCount = 1;
	tpPrevious.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
	{
		tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
	}
	else
	{
		tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED & tpPrevious.Privileges[0].Attributes);
	}
	AdjustTokenPrivileges(	hToken,
									false,
									&tpPrevious,
									cbPrevious,
									NULL,
									NULL);
	if (GetLastError() != ERROR_SUCCESS)
	{
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
void TfrmMain::ShowLastErr()
{
	LPTSTR lpMsgBuf;
	FormatMessage(  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						 NULL,
						 GetLastError(),
						 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						 (LPTSTR) &lpMsgBuf,
						 0,
						 NULL);
	MessageBox(lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION);
	LocalFree(lpMsgBuf);
}


void __fastcall TfrmMain::tbtnOnlineUpdateMouseClick(TObject *Sender)
{
   std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(Screen->ActiveForm));

   TVerticalSelection Item;
   Item.Title = "Back";
   Item.Properties["Color"] = clGreen;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   Item.Title = "Online Update";
   Item.Properties["Color"] = clRed;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   Item.Title = "LAN Update";
   Item.Properties["Color"] = clNavy;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   SelectionForm->ShowModal();

	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem))
	{
		if(SelectedItem.Title == "Online Update")
		{
         try
         {

            TLoginSuccess Result = lsAccepted;
            AnsiString PIN;
            std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
            frmTouchNumpad->Caption = "Enter Access PIN";
            frmTouchNumpad->btnSurcharge->Caption = "Ok";
            frmTouchNumpad->btnDiscount->Visible = false;
            frmTouchNumpad->btnSurcharge->Visible = true;
            frmTouchNumpad->Mode = pmPIN;
            if (frmTouchNumpad->ShowModal() == mrOk)
            {
               PIN = frmTouchNumpad->STRResult;
            }

            if (Result == lsAccepted)
            {
               std::auto_ptr<TOnlineManager> OnlineManager(new TOnlineManager);

					UnicodeString UpdateFile = OnlineManager->GetUpdateFile(PIN);

               if(UpdateFile != "")
               {
                  ShellExecute(
                  this->Handle,	// handle to parent window
                  "open",	// pointer to string that specifies operation to perform
						UpdateFile.t_str(),	// pointer to filename or folder name string
						UpdateFile.t_str(),	// pointer to string that specifies executable-file parameters
						ExtractFilePath(Application->ExeName).t_str(),	// pointer to string that specifies default directory
                  SW_MAXIMIZE 	// whether file is shown when opened
						);
               }
            }
            else if (Result == lsDenied)
            {
               MessageBox("You do not have access to Setup.", "Error", MB_OK + MB_ICONERROR);
            }
            else if (Result == lsPINIncorrect)
            {
               MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
            }
         }
         catch(Exception &E)
         {
            ShowMessage(E.Message);
         }
		}
		else if(SelectedItem.Title == "LAN Update")
		{
         try
         {
					UnicodeString CurrentDir;
					RegistryRead(MMBaseKey,"UpdateDir",CurrentDir);
               if(CurrentDir == "")
               {
                  std::auto_ptr<TfrmTouchKeyboard> TouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
                  TouchKeyboard->MaxLength = 255;
                  TouchKeyboard->AllowCarriageReturn = false;
                  TouchKeyboard->StartWithShiftDown = false;
                  TouchKeyboard->KeyboardText = "\\\\Pos1\\MenuMate\\Updates\\";
                  TouchKeyboard->Caption = "The UNC Path to the Updates directory on your server.";
                  if (TouchKeyboard->ShowModal() == mrOk)
                  {
                     CurrentDir = IncludeTrailingPathDelimiter(TouchKeyboard->KeyboardText);
                     RegistryWrite(MMBaseKey,"UpdateDir",CurrentDir);
                  }
               }

               std::auto_ptr<TLanManager> LanManager(new TLanManager(CurrentDir));
               AnsiString UpdateFile = LanManager->GetUpdateFile();
               if(UpdateFile != "")
               {
                  ShellExecute(
                  this->Handle,	// handle to parent window
                  "open",	// pointer to string that specifies operation to perform
                  UpdateFile.c_str(),	// pointer to filename or folder name string
                  UpdateFile.c_str(),	// pointer to string that specifies executable-file parameters
                  NULL,	// pointer to string that specifies default directory
                  SW_MAXIMIZE 	// whether file is shown when opened
                  );

/*                  spawnl(
                     P_NOWAIT,
                     UpdateFile.c_str(),
                     UpdateFile.c_str(),
                     NULL);*/
               }
            }
         catch(Exception &E)
         {
            ShowMessage(E.Message);
         }
		}
	}
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
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
   FormResize(Sender);

   std::auto_ptr<TfrmSystemConfig> frmSystemConfig(new TfrmSystemConfig(NULL));
   bool DBOk = false;
   bool Abort = false;

   if(Abort)
   {
     Application->Terminate();
   }

   AnsiString ComputerName = "DeskMate";
	char CompName[MAX_COMPUTERNAME_LENGTH];
	DWORD Size = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerName(CompName,&Size))
	{
	   ComputerName = CompName;
	}

	lbeVersion->Caption = GetFileVersion();
}

//---------------------------------------------------------------------------

void TfrmMain::OpenDB()
{
	UnicodeString DatabasePath = "";
	RegistryRead(MMBaseKey "Database","DatabasePath",DatabasePath);

	UnicodeString InterbaseIP = "";
   RegistryRead(MMBaseKey "Database","InterbaseIP",InterbaseIP);

	if (InterbaseIP == "" || DatabasePath == "")
	{
		throw Exception("Database not set up");
	}

	DBControl.Disconnect();
	DBControl.Init(Database::TDBSettings(InterbaseIP,DatabasePath,false));
	DBControl.Connect();
	Database::TDBUtilities Utils(DBControl);
	Utils.SetForcedWrites();
}

void TfrmMain::CloseDB()
{
	DBControl.Disconnect();
}

//---------------------------------------------------------------------------


void __fastcall TfrmMain::tbtnOnlineHelpMouseClick(TObject *Sender)
{
#ifndef _DEBUG
   std::auto_ptr<TIdHTTP> idHTTP(new TIdHTTP(NULL));
   std::auto_ptr<TMemoryStream> RespStream(new TMemoryStream);
   idHTTP->HandleRedirects = true;
   idHTTP->ProtocolVersion = pv1_1;
   idHTTP->Connect("vnc.menumate.com",8000);
   idHTTP->Request->Accept = "text/html, */*";
   idHTTP->Get("http://vnc.menumate.com/OnlineHelp/" TechSupportIndex,RespStream.get());

   RespStream->SaveToFile(TechSupportIndex);
   TiXmlDocument doc(TechSupportIndex);
#endif

#ifdef _DEBUG
   if(MessageBox("Download File", "Download Index File ", MB_YESNO + MB_ICONERROR) == IDYES)
   {
	  std::auto_ptr<TIdHTTP> idHTTP(new TIdHTTP(NULL));
      std::auto_ptr<TMemoryStream> RespStream(new TMemoryStream);
	  idHTTP->HandleRedirects = true;
      idHTTP->ProtocolVersion = pv1_1;
      idHTTP->Connect("vnc.menumate.com",80);
      idHTTP->Request->Accept = "text/html, */*";
      idHTTP->Get("http://vnc.menumate.com/OnlineHelp/" TechSupportIndex,RespStream.get());
      RespStream->SaveToFile(TechSupportIndex);
   }
   TiXmlDocument doc(TechSupportIndex);
#endif

	if (doc.LoadFile())
	{
      TiXmlHandle hDoc(&doc);
      XMLGUINav(hDoc);
	}
	else
	{
      MessageBox("Online Help is Unavailable", "Error", MB_OK + MB_ICONERROR);
	}
}

void TfrmMain::XMLGUINav(TiXmlHandle &doc)
{

   TiXmlElement* pElement;
   TiXmlHandle pButtonSet(0);
   TiXmlHandle pTopLevel(0);
   pElement = doc.FirstChildElement().Element();
   if (pElement)
   {
      // Save off the Curernt Root Handle.
      pTopLevel = TiXmlHandle(pElement);
      // Save off the Curernt Button Set.
      pButtonSet = TiXmlHandle(pElement);
      bool Exit = false;

      // Draw the Current Button Set.
      do
      {
         TiXmlElement* pButtonsNode = pButtonSet.FirstChild( "Button" ).Element();
         std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(Screen->ActiveForm));
         for(; pButtonsNode; pButtonsNode = pButtonsNode->NextSiblingElement())
         {
            TVerticalSelection Item;
            Item.Title = AnsiString(pButtonsNode->Attribute("Title"));
            if(Item.Title != "")
            {
            Item.Properties["Ref"] = AnsiString(pButtonsNode->Attribute("Ref"));
            Item.Properties["Params"] = AnsiString(pButtonsNode->Attribute("Params"));

            int Back = 0;
            pButtonsNode->QueryIntAttribute("Back",&Back);
            Item.Properties["Back"] = Back;

               int Exit = 1;
               pButtonsNode->QueryIntAttribute("Exit",&Exit);
               Item.Properties["Exit"] = Exit;

            int ShellOpen = 0;
            pButtonsNode->QueryIntAttribute("ShellOpen",&ShellOpen);
            Item.Properties["ShellOpen"] = ShellOpen;

            int Colour = clNavy;
               pButtonsNode->QueryIntAttribute("Color",&Colour);
            Item.Properties["Color"] = Colour;

            SelectionForm->Items.push_back(Item);
         }
         }

         SelectionForm->ShowModal();

         TVerticalSelection SelectedItem;
         if(SelectionForm->GetFirstSelectedItem(SelectedItem))
         {
            AnsiString Ref = SelectedItem.Properties["Ref"];
//            Ref = AnsiReplaceText(Ref,"[cr]","\\r");
            AnsiString Params = SelectedItem.Properties["Params"];
            if(Ref != "")
            {
               std::auto_ptr<TStringList> Commands(new TStringList);
               std::auto_ptr<TStringList> ParamsList(new TStringList);
               Commands->Delimiter = ';';
               Commands->DelimitedText = Ref;
               ParamsList->Delimiter = ';';
               ParamsList->DelimitedText = Params;

               for (int i = 0; i < Commands->Count ; i ++)
               {
						UnicodeString CurrentCommand = Commands->Strings[i];
						UnicodeString CurrentParam = "";
                  if(Commands->Count > 1 && ParamsList->Count >= i+1)
                  {
                     CurrentParam = ParamsList->Strings[i];
                  }
                  else
						{
                     CurrentParam = Params;
                  }

                  if(SelectedItem.Properties["ShellOpen"])
                  {
							ShellExecute(Handle,"open",CurrentCommand.t_str(),CurrentParam.t_str(),ExtractFilePath(Application->ExeName).t_str(),SW_SHOWNORMAL);
                     Exit = SelectedItem.Properties["Exit"];
                  }
                  else
                  {
                     std::auto_ptr<TfrmReportDisplay> frmReportDisplay(TfrmReportDisplay::Create<TfrmReportDisplay>(this));
                     WideString ReportLocation = "";
                     frmReportDisplay->NavigateToURL(CurrentCommand);
                     frmReportDisplay->ShowModal();
                     Exit = true;
                  }
                  ::Sleep(1000);
               }
            }

            if(SelectedItem.Properties["Back"])
            {
               pButtonSet = pButtonSet.Node()->Parent()->Parent();
               if(pButtonSet.FirstChild( "Button" ).Element() == NULL)
               {
                  Exit = true;
               }
            }
            else if(!Exit)
            {
               // Find the Item with this Title in the current Set.
               TiXmlElement* pButtonsNode = pButtonSet.FirstChild( "Button" ).Element();
               for(; pButtonsNode; pButtonsNode = pButtonsNode->NextSiblingElement())
               {
                  if(AnsiString(pButtonsNode->Attribute("Title")) == SelectedItem.Title)
                  {
                     pButtonSet = TiXmlHandle(pButtonsNode->FirstChild( "Buttons" ));
                  }
               }
               if(pButtonSet.FirstChild( "Button" ).Element() == NULL)
               {
                  Exit = true;
                  MessageBox("Futher online help is on this topic is unavailable", "Error", MB_OK + MB_ICONERROR);
               }
            }
         }
      }while(!Exit);
   }
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::tbtnDatabaseMouseClick(TObject *Sender)
{
	UnicodeString DatabasePath = "";
   RegistryRead(MMBaseKey "Database","DatabasePath",DatabasePath);

   std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(Screen->ActiveForm));

   TVerticalSelection Item;
   Item.Title = "Back";
   Item.Properties["Color"] = clGreen;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   Item.Title = "IBConsole";
   Item.Properties["Color"] = clOlive;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   if(FileExists(DatabasePath))
   {
      Item.Title = "IBTools";
      Item.Properties["Color"] = clPurple;
      Item.CloseSelection = true;
      SelectionForm->Items.push_back(Item);
   }

   SelectionForm->ShowModal();

	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem))
	{
		if(SelectedItem.Title == "IBConsole")
		{
         AnsiString TaskPath("C:\\Program Files\\borland\\interbase\\Bin\\IBConsole.exe");
         ShellExecute(Handle,"open",TaskPath.c_str(),NULL,NULL,SW_SHOWNORMAL	);
      }
		else if(SelectedItem.Title == "IBTools")
		{
         AnsiString TaskPath(DatabasePath);
         ShellExecute(Handle,"open",TaskPath.c_str(),NULL,NULL,SW_SHOWNORMAL);
		}
   }
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::tbtnOnlineHelpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(Shift.Contains(ssCtrl) && Shift.Contains(ssShift) && Shift.Contains(ssAlt))
   {
      TiXmlDocument doc;
      TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
      doc.LinkEndChild( decl );

      TiXmlElement * Buttons = new TiXmlElement( "Buttons" );
      doc.LinkEndChild( Buttons );

      TiXmlElement *L1Button = new TiXmlElement( "Button" );
      L1Button->SetAttribute("Title", "Back");
      L1Button->SetAttribute("Color", int(clMaroon));
      L1Button->SetAttribute("Back", 1);
      Buttons->LinkEndChild( L1Button );

      L1Button = new TiXmlElement( "Button" );
      L1Button->SetAttribute("Title", "End Remote Support");
      L1Button->SetAttribute("Ref", "\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
      L1Button->SetAttribute("Params", "-kill");
      L1Button->SetAttribute("Color", clRed);
      L1Button->SetAttribute("ShellOpen", 1);
      Buttons->LinkEndChild( L1Button );

      L1Button = new TiXmlElement( "Button" );
      L1Button->SetAttribute("Title", "MenuMate Support Brad");
      L1Button->SetAttribute("Ref", "\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
      L1Button->SetAttribute("Params", "-connect mm_bradm.myvnc.com::5550");
      L1Button->SetAttribute("Color", clGreen);
      L1Button->SetAttribute("ShellOpen", 1);
      Buttons->LinkEndChild( L1Button );

      L1Button = new TiXmlElement( "Button" );
      L1Button->SetAttribute("Title", "MenuMate Support Chris");
      L1Button->SetAttribute("Color", clGreen);
      L1Button->SetAttribute("Ref", "\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
      L1Button->SetAttribute("Params", "-connect vnc.menumate.com::5502");
      L1Button->SetAttribute("ShellOpen", 1);
      Buttons->LinkEndChild( L1Button );

      L1Button = new TiXmlElement( "Button" );
      L1Button->SetAttribute("Title", "MenuMate Support Michael");
      L1Button->SetAttribute("Color", clGreen);
      L1Button->SetAttribute("Ref", "\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
      L1Button->SetAttribute("Params", "-connect vnc.menumate.com::5500");
      L1Button->SetAttribute("ShellOpen", 1);
      Buttons->LinkEndChild( L1Button );

      L1Button = new TiXmlElement( "Button" );
      L1Button->SetAttribute("Title", "Scratchpad");
      L1Button->SetAttribute("Color", clBlue);
      L1Button->SetAttribute("Ref", "http://www.iqworks.co.nz/iqsite html/scratchpad");
      Buttons->LinkEndChild( L1Button );

      L1Button = new TiXmlElement( "VNC" );
      L1Button->SetAttribute("Title", "Config VNC");
      L1Button->SetAttribute("Color", clDkGray);
      Buttons->LinkEndChild( L1Button );

         TiXmlElement * L2Buttons = new TiXmlElement( "Buttons" );
         L1Button->LinkEndChild( L2Buttons );

         TiXmlElement *L2Button = new TiXmlElement( "Button" );
         L2Button->SetAttribute("Title", "Back");
         L2Button->SetAttribute("Color", int(clMaroon));
         L2Button->SetAttribute("Back", 1);
         L2Buttons->LinkEndChild( L2Button );

         L2Button = new TiXmlElement( "Button" );
         L2Button->SetAttribute("Title", "Tight VNC");
         L2Button->SetAttribute("Color", clNavy);
         L2Button->SetAttribute("Ref", "\"Cmd.exe\"");
         L2Button->SetAttribute("Params", " /c copy \"C:\\Program Files\\MenuMate\\help\\WinVNC.Tight.exe\" \"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\" ");
         L2Button->SetAttribute("ShellOpen", 1);
         L2Button->SetAttribute("Exit", 0);
         L2Buttons->LinkEndChild( L2Button );

            TiXmlElement * L3Buttons = new TiXmlElement( "Buttons" );
            L2Button->LinkEndChild( L3Buttons );

            L1Button = new TiXmlElement( "Button" );
            L1Button->SetAttribute("Title", "Back");
            L1Button->SetAttribute("Color", int(clMaroon));
            L1Button->SetAttribute("Back", 1);
            L3Buttons->LinkEndChild( L1Button );

            L1Button = new TiXmlElement( "Button" );
            L1Button->SetAttribute("Title", "MenuMate Support Brad");
            L1Button->SetAttribute("Ref", "\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\";\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
            L1Button->SetAttribute("Params", "\"-run\";\"-autoreconnect -connect mm_bradm.myvnc.com::5550\"");
            L1Button->SetAttribute("Color", clGreen);
            L1Button->SetAttribute("ShellOpen", 1);
            L3Buttons->LinkEndChild( L1Button );

            L1Button = new TiXmlElement( "Button" );
            L1Button->SetAttribute("Title", "MenuMate Support Chris");
            L1Button->SetAttribute("Color", clGreen);
            L1Button->SetAttribute("Ref", "\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\";\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
            L1Button->SetAttribute("Params", "\"-run\";\"-autoreconnect -connect vnc.menumate.com::5502\"");
            L1Button->SetAttribute("ShellOpen", 1);
            L3Buttons->LinkEndChild( L1Button );

            L1Button = new TiXmlElement( "Button" );
            L1Button->SetAttribute("Title", "MenuMate Support Michael");
            L1Button->SetAttribute("Color", clGreen);
            L1Button->SetAttribute("Ref", "\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\";\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
            L1Button->SetAttribute("Params", "\"-run\";\"-autoreconnect -connect vnc.menumate.com::5500\"");
            L1Button->SetAttribute("ShellOpen", 1);
            L3Buttons->LinkEndChild( L1Button );

         L2Button = new TiXmlElement( "Button" );
         L2Button->SetAttribute("Title", "Real VNC");
         L2Button->SetAttribute("Color", clNavy);
         L2Button->SetAttribute("Ref", "\"Cmd.exe\"");
         L2Button->SetAttribute("Params", "/c copy \"C:\\Program Files\\MenuMate\\help\\WinVNC.Real.exe\" \"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
         L2Button->SetAttribute("ShellOpen", 1);
         L2Button->SetAttribute("Exit", 0);
         L2Buttons->LinkEndChild( L2Button );

            L3Buttons = new TiXmlElement( "Buttons" );
            L2Button->LinkEndChild( L3Buttons );

            L1Button = new TiXmlElement( "Button" );
            L1Button->SetAttribute("Title", "Back");
            L1Button->SetAttribute("Color", int(clMaroon));
            L1Button->SetAttribute("Back", 1);
            L3Buttons->LinkEndChild( L1Button );

            L1Button = new TiXmlElement( "Button" );
            L1Button->SetAttribute("Title", "MenuMate Support Brad");
            L1Button->SetAttribute("Ref", "\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
            L1Button->SetAttribute("Params", "-connect mm_bradm.myvnc.com::5550");
            L1Button->SetAttribute("Color", clGreen);
            L1Button->SetAttribute("ShellOpen", 1);
            L3Buttons->LinkEndChild( L1Button );

      L1Button = new TiXmlElement( "Button" );
            L1Button->SetAttribute("Title", "MenuMate Support Chris");
            L1Button->SetAttribute("Color", clGreen);
            L1Button->SetAttribute("Ref", "\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
            L1Button->SetAttribute("Params", "-connect vnc.menumate.com::5502");
            L1Button->SetAttribute("ShellOpen", 1);
            L3Buttons->LinkEndChild( L1Button );

      L1Button = new TiXmlElement( "Button" );
            L1Button->SetAttribute("Title", "MenuMate Support Michael");
            L1Button->SetAttribute("Color", int(clGreen));
            L1Button->SetAttribute("Ref", "\"C:\\Program Files\\MenuMate\\help\\WinVNC.exe\"");
            L1Button->SetAttribute("Params", "-connect vnc.menumate.com::5500");
      L1Button->SetAttribute("ShellOpen", 1);
            L3Buttons->LinkEndChild( L1Button );

         doc.SaveFile( TechSupportIndex );
   }

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TRetrieveNamesThread::TRetrieveNamesThread(bool CreateSuspended)
: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TRetrieveNamesThread::Execute()
{
   EnumNetRes(NULL, ComputerNames);
}
//---------------------------------------------------------------------------
void __fastcall TRetrieveNamesThread::EnumNetRes(NETRESOURCE* nr, TStrings *ComputerNames)
{
   try
   {
      HANDLE hEnum;
      if (WNetOpenEnum( RESOURCE_GLOBALNET,
      RESOURCETYPE_ANY,
      RESOURCEUSAGE_CONTAINER | RESOURCEUSAGE_CONNECTABLE,
      nr,
      &hEnum) != NO_ERROR)
      {
         return;
      }
      DWORD Count=-1;
      DWORD BufferSize=0;
      DWORD ReturnVal=0;
      NETRESOURCE *NetRes = (NETRESOURCE*)new char[1024];
      ZeroMemory(NetRes, sizeof(NETRESOURCE));
      for(;;)
      {
         ReturnVal = WNetEnumResource(hEnum, &Count, NetRes, &BufferSize);
         if(ReturnVal == ERROR_MORE_DATA) //we set the size of NetRes
         {
            //we set the size of NetRes if there is more data
            Count=-1;
            delete[] NetRes;
            NetRes = (NETRESOURCE*)new char[BufferSize];
            ZeroMemory(NetRes,sizeof(NETRESOURCE));
            ReturnVal = WNetEnumResource(hEnum, &Count, NetRes, &BufferSize);
         }
         if(ReturnVal != NO_ERROR) break;
         // Add the resource to the ListBox
         for (unsigned int i=0; i<Count && !Terminated; i++)
         {
            // Application->ProcessMessages();
            if((NetRes[i].dwUsage & RESOURCEUSAGE_CONTAINER) == RESOURCEUSAGE_CONTAINER)
            {
               if(NetRes[i].dwDisplayType == RESOURCEDISPLAYTYPE_SERVER)
               {
                  ComputerName = NetRes[i].lpRemoteName;
                  ComputerName = ComputerName.SubString(3, ComputerName.Length()-2);
                  Synchronize(AddName);
               }
               EnumNetRes(&NetRes[i], ComputerNames);
            }
         }
         if(ReturnVal == ERROR_NO_MORE_ITEMS)
         {
            delete[] NetRes;
            WNetCloseEnum(hEnum);
            //RaiseLastWin32Error();
         }
         if (Terminated)
         {
            break;
         }
      }
      delete[] NetRes;
      WNetCloseEnum(hEnum);
   }
   catch (Exception &E)
   {
      ShowMessage(E.Message);
   }
}
//---------------------------------------------------------------------------
void __fastcall TRetrieveNamesThread::AddName()
{
   ComputerNames->Add(ComputerName);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


void __fastcall TfrmMain::RetrieveNamesThreadTerminate(TObject *Sender)
{
   RetrieveNamesThread = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tbtnTillsMouseClick(TObject *Sender)
{
   std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(Screen->ActiveForm));

   TVerticalSelection Item;
   Item.Title = "Back";
   Item.Properties["Color"] = clGreen;
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   for (int i = 0 ; i < StringList->Count ; i++)
   {
      Item.Title = StringList->Strings[i];
      Item.Properties["Color"] = clMaroon;
      Item.CloseSelection = true;
      SelectionForm->Items.push_back(Item);
   }

   SelectionForm->ShowModal();

	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem))
	{
      if(SelectedItem.Title != "Back")
      {
         std::auto_ptr<TfrmVerticalSelect> AppSelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(Screen->ActiveForm));

         TVerticalSelection AppItem;
         AppItem.Title = "Back";
         AppItem.Properties["Color"] = clGreen;
         AppItem.CloseSelection = true;
         AppSelectionForm->Items.push_back(AppItem);

         AppItem.Title = "MenuMate";
         AppItem.Properties["Color"] = clNavy;
         AppItem.Properties["Port"] = MENUMATEPORT;
         AppItem.CloseSelection = true;
         AppSelectionForm->Items.push_back(AppItem);

         AppItem.Title = "PalmMate";
         AppItem.Properties["Color"] = clMaroon;
         AppItem.Properties["Port"] = PALMMATEPORT;
         AppItem.CloseSelection = true;
         AppSelectionForm->Items.push_back(AppItem);

         AppSelectionForm->ShowModal();

         TVerticalSelection AppSelectedItem;
         if(AppSelectionForm->GetFirstSelectedItem(AppSelectedItem))
         {
            if(AppSelectedItem.Title != "Back")
            {
               TerminateMenuMate(SelectedItem.Title,AppSelectedItem.Properties["Port"]);
            }
         }
      }
   }
}

//---------------------------------------------------------------------------

void TfrmMain::TerminateMenuMate(AnsiString ComputerName,int Port)
{
   AnsiString AppName = "MenuMate";
   if(Port == PALMMATEPORT)
   {
      AppName = "PalmMate";
   }

   if(MessageBox("Are you sure you wish to shutdown " + AppName + " on " + ComputerName, "ShutDown MenuMate on " + ComputerName, MB_YESNO + MB_ICONERROR) == IDYES)
   {
      std::auto_ptr<TNetMessageKillProc> Request(new TNetMessageKillProc);
     	std::auto_ptr<TManagerNetClient> udp(new TManagerNetClient);
      udp->SendTo(Request.get(),ComputerName,Port);
   }
}

