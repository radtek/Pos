//---------------------------------------------------------------------------

#include <vcl.h>
#include <DateUtils.hpp>
#pragma hdrstop

#include "main.h"
#include "MMRegistry.h"
#include <DateUtils.hpp>
#include "Version.h"
#include <Memory>
#include "ManagerTimeClock.h"
#include "process.h"
//---------------------------------------------------------------------------
#define CHANGE   	"Change"
#define SEARCHSTRING "CashOut"
#define MMBaseKey "\\Software\\IQWorks\\MenuMate\\"
#pragma package(smart_init)
#pragma link "TouchButton"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	frmMain->Caption = "MenuMate Updater";
	Memo->Lines->Add("MenuMate Updater Version " + GetFileVersion());
	CreateRequired = false;
	GetDBLocation();
	Log("Path to Apply Updates To is " + DBPath);
	Log("Click 'Begin Update' To Continue...");
}
//---------------------------------------------------------------------------
void TfrmMain::GetDBLocation()
{
	DBControl.Disconnect();
	try
	{
		DBPath = "";
		RegistryRead(MMBaseKey "Database","DatabasePath",DBPath);
		ServerName = "";
		RegistryRead(MMBaseKey "Database","InterbaseIP",ServerName);
      if(ExtractFileExt(DBPath).UpperCase() == ".GDB")
      {
   		Log("== Database Name will need to be changed to *.IB in MenuMate ==");
         DBPath = ChangeFileExt(DBPath, ".IB");
      }

		if(DBPath != "" && ServerName != "")
		{
			try
			{
				DBControl.Disconnect();
				DBControl.Init(Database::TDBSettings(ServerName,DBPath,false));
				DBControl.Connect();
				Application->ProcessMessages();
			}
			catch(Exception &E)
			{
				Log("Unable to connect to database at location " + DBPath);
				DBPath = "Invalid";
			}
		}
		else if(FileExists("C:\\Program Files\\MenuMate\\MenuMate.ib"))
		{
			ServerName = "localhost";
         DBPath = "C:\\Program Files\\MenuMate\\MenuMate.ib";
			try
			{
				Log("No Registery Settings but found DB at " + DBPath);
				DBControl.Disconnect();
				DBControl.Init(Database::TDBSettings(ServerName,DBPath,false));
				DBControl.Connect();
				Application->ProcessMessages();
			}
			catch(Exception &E)
			{
				Log("Unable to connect to database at location " + DBPath);
				DBPath = "Invalid";
			}
		}
		else
		{
			DBPath = "C:\\Program Files\\MenuMate\\MenuMate.ib";
         ServerName = "Localhost";
			if(!FileExists(DBPath))
			{
				Log("Will Create a new Database.");
				CreateRequired = true;
			}
			else
			{
				Log("Unable to create a new database a MenuMate.ib already exists.");
				DBPath = "Invalid";
			}
		}
	}
	catch(Exception &E)
	{
		Log("Unable to open Database " + E.Message);
	}
}

bool TfrmMain::DB1Connect()
{
	bool Retval = true;
	try
	{
		if(CreateRequired)
		{
			if(!FileExists(DBPath))
			{
				Log("Unable to locate DB");
			}
			else
			{
				Log("Unable to create a database at location " + DBPath +" a MenuMate.IB already exists.");
			}
		}
		else
		{
         DBControl.Disconnect();
         DBControl.Init(Database::TDBSettings(ServerName,DBPath,false));
         DBControl.Connect();
         Application->ProcessMessages();
		}
	}
	catch(Exception &E)
	{
		Log("Unable to connect to database at location " + DBPath);
		Log(E.Message);
		Retval = false;
	}
	return Retval;
}

void TfrmMain::DB6Disconnect()
{
   DBControl.Disconnect();
}

void TfrmMain::Log(AnsiString Log)
{
   Memo->Lines->Add(Log);
}

void __fastcall TfrmMain::TouchBtn1MouseClick(TObject *Sender)
{
	if(DBPath == "Invalid")
	{
		Log("Unable to connect to Invalid DB path. Checking paths again");
		CreateRequired = false;
		GetDBLocation();
		Log("Path to Apply Updates To is " + DBPath);
		Log("Click 'Begin Update' To Continue...");
	}
	else
	{
		if(DB1Connect())
		{

         Database::TDBTransaction DBTransaction(DBControl);
         DBTransaction.StartTransaction();
         TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());

			IBQuery->Close();
			IBQuery->SQL->Text =
			"select version_number from versionhistory order by time_stamp desc";
			IBQuery->ExecQuery();
			if(IBQuery->RecordCount > 0)
			{
				Log("Last Version Applied Was " + IBQuery->FieldByName("version_number")->AsString);
			}

         DBTransaction.Commit();

         AnsiString ErrorMessage = "Do you wish to do a database backup? (Recommended)";
         if(Application->MessageBox(ErrorMessage.c_str(),"Information",
            MB_YESNO + MB_DEFBUTTON2 + MB_ICONINFORMATION) == IDYES)
         {
            if(FileExists(ExtractFilePath(Application->ExeName) + "IBTools.exe"))
            {
               HWND PrevWnd;
               PrevWnd = FindWindow("TIBTools", "IBTools");
               spawnl(
                  P_WAIT,
                  (ExtractFilePath(Application->ExeName) + "IBTools.exe").c_str(),
                  (ExtractFilePath(Application->ExeName) + "IBTools.exe").c_str(),
                  AnsiString("\"")+ DBPath.c_str() + AnsiString("\""),
                  NULL);

               ::Sleep(1000);
               PrevWnd = FindWindow("TIBTools", "IBTools");
               if (PrevWnd != NULL)
               {
                  PrevWnd = GetWindow(PrevWnd, GW_OWNER);
                  if (PrevWnd != NULL)
                  {
                     if (IsIconic(PrevWnd))
                     {
                        ShowWindow(PrevWnd, SW_RESTORE);
                     }
                     else
                     {
                        SetForegroundWindow(PrevWnd);
                     }
                  }
               }
            }
            else
            {
               Application->MessageBox("IBTools not found.","Information", MB_OK + MB_ICONERROR);
            }
         }

			RemoveDemoMenuMateDB();
			UpDateAll();
			DB6Disconnect();
			Log("Upgrade Complete");
			Log("Click 'Close'");
		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void TfrmMain::UpDateAll()
{
	TDateTime BeginTime = Now();
   Log("Begin Time : " + BeginTime.FormatString("dd/mm/yyyy hh:nn:ss"));
   Patch();
   Application->ProcessMessages();
	TDateTime EndTime = Now();
	Log("End Time : " + EndTime.FormatString("dd/mm/yyyy hh:nn:ss"));
	Log("Elasped Time " + IntToStr(HoursBetween(EndTime, BeginTime)) +":"+ IntToStr(MinutesBetween(EndTime, BeginTime)) +":"+ IntToStr(SecondsBetween(EndTime, BeginTime)));
}

void TfrmMain::RemoveDemoMenuMateDB()
{
	try
	{
		if(FileExists("C:\\MMDemoData.ib"))
      {
         DeleteFile("C:\\MMDemoData.ib");
         Log("Removed demo database.");
      }
      if(FileExists("C:\\MMDemoData.ibk"))
      {
         DeleteFile("C:\\MMDemoData.ibk");
         Log("Removed demo database backup.");
      }
   }
   catch(Exception &E)
   {
      Log("Unable to remove C:\\MMDemoData.ib. Please delete this file before entering demo mode on any terminal.");
      Log(E.Message);
   }
}


void TfrmMain::Patch()
{
   DB6Disconnect();
   DB1Connect();

	AnsiString UpdateFromVersion = "4.12";
	AnsiString UpdateVersion = "Patch-19905";
	try
	{
      Database::TDBTransaction DBTransaction(DBControl);
      TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
      TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
      TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
      DBTransaction.StartTransaction();

		IBQuery->Close();

		IBQuery->SQL->Text =
		"SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if(IBQuery->RecordCount == 0)
		{
			Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{

			IBQuery->Close();
			IBQuery->SQL->Text =
			"SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if(IBQuery->RecordCount == 0)
			{
				Log("Updating Database To Version Number " + UpdateVersion);

				DBTransaction.StartTransaction();

            // Add Security to the current orders. -----------------------------------
            DBTransaction.StartTransaction();
            IBQuery2->Close();
            IBQuery2->SQL->Text = "SELECT COUNT(ORDER_KEY) FROM ORDERS WHERE SECURITY_REF = 0";
            IBQuery2->ExecQuery();
            int RecordCount = IBQuery2->Fields[0]->AsInteger;

            lbeProgress->Caption = "Processing " +IntToStr(RecordCount) + " order records";
            Application->ProcessMessages();
            barProgress->Position = 0;
            barProgress->Min = 0;
            barProgress->Max = RecordCount;

            int CurrentCount = 0;

            IBQuery->Close();
            IBQuery->SQL->Text =
            "SELECT TAB_KEY,ORDER_KEY,SERVER_NAME,TERMINAL_NAME,TIME_STAMP "
            "FROM ORDERS WHERE SECURITY_REF = 0";
            IBQuery->ExecQuery();
            if(!IBQuery->RecordCount == 0)
            {
               unsigned int Time;
               Time = GetTickCount() + 1000;
               for (; !IBQuery->Eof; IBQuery->Next())
               {
                  if(Time < GetTickCount() )
                  {
                     Application->ProcessMessages();
                     Time = GetTickCount() + 1000;
                  }

                  AnsiString UserName;
                  int ContactKey = GetUserFromTabKey(DBTransaction,IBQuery->FieldByName("TAB_KEY")->AsInteger,UserName);

                  AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
                  int OrderKey = IBQuery->FieldByName("ORDER_KEY")->AsInteger;
                  TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;

                  if(ContactKey != 0)
                  {
                     int CurrentSecurityRef;
                     IBQuery1->Close();
                     IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_REF, 1) FROM RDB$DATABASE";
                     IBQuery1->ExecQuery();
                     CurrentSecurityRef = IBQuery1->Fields[0]->AsInteger;

                     int CurrentSecurityKey;
                     IBQuery1->Close();
                     IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE";
                     IBQuery1->ExecQuery();
                     CurrentSecurityKey = IBQuery1->Fields[0]->AsInteger;

                     // Add new Reocrd.
                     IBQuery1->Close();
                     IBQuery1->SQL->Clear();
                     IBQuery1->SQL->Text =
                        "INSERT INTO SECURITY ("
                           "SECURITY_KEY, "
                           "SECURITY_REF, "
                           "SECURITY_EVENT, "
                           "FROM_VAL, "
                           "TO_VAL, "
                           "TERMINAL_NAME, "
                           "USER_KEY, "
                           "TIME_STAMP) "
                        "VALUES ("
                           ":SECURITY_KEY, "
                           ":SECURITY_REF, "
                           ":SECURITY_EVENT, "
                           ":FROM_VAL, "
                           ":TO_VAL, "
                           ":TERMINAL_NAME, "
                           ":USER_KEY, "
                           ":TIME_STAMP);";
                     IBQuery1->ParamByName("SECURITY_KEY")->AsInteger	= CurrentSecurityKey;
                     IBQuery1->ParamByName("SECURITY_REF")->AsInteger	= CurrentSecurityRef;
                     IBQuery1->ParamByName("USER_KEY")->AsInteger	      = ContactKey;
                     IBQuery1->ParamByName("SECURITY_EVENT")->AsString	= SecurityTypes[secOrderedBy];
                     IBQuery1->ParamByName("FROM_VAL")->AsString	      = UserName;
                     IBQuery1->ParamByName("TO_VAL")->AsString			   = "Created by Auto-updater";
                     IBQuery1->ParamByName("TIME_STAMP")->AsDateTime	   = TimeStamp;
                     IBQuery1->ParamByName("TERMINAL_NAME")->AsString	= TerminalName;
                     IBQuery1->ExecQuery();

                     IBQuery1->Close();
                     IBQuery1->SQL->Text =
                     "UPDATE ORDERS"
                     " SET SECURITY_REF = :SECURITY_REF "
                     "WHERE ORDER_KEY = :ORDER_KEY "
                     "AND SECURITY_REF = 0";
                     IBQuery1->ParamByName("ORDER_KEY")->AsInteger = OrderKey;
                     IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
                     IBQuery1->ExecQuery();
                  }
                   CurrentCount++;
                   barProgress->Position = CurrentCount;
               }
            }
            DBTransaction.Commit();

            // Add Security to the Day Archives orders. -----------------------------------
            DBTransaction.StartTransaction();
            IBQuery2->SQL->Text = "SELECT COUNT(ARCHIVE_KEY) FROM DAYARCHIVE WHERE SECURITY_REF = 0";
            IBQuery2->ExecQuery();
            RecordCount = IBQuery2->Fields[0]->AsInteger;

            lbeProgress->Caption = "Processing " +IntToStr(RecordCount) + " day archive records";
            Application->ProcessMessages();
            barProgress->Position = 0;
            barProgress->Min = 0;
            barProgress->Max = RecordCount;

            CurrentCount = 0;

            IBQuery->Close();
            IBQuery->SQL->Text =
            "SELECT ARCBILL_KEY,ARCHIVE_KEY,SERVER_NAME,TERMINAL_NAME,TIME_STAMP "
            "FROM DAYARCHIVE WHERE SECURITY_REF = 0";
            IBQuery->ExecQuery();
            if(!IBQuery->RecordCount == 0)
            {
               unsigned int Time;
               Time = GetTickCount() + 1000;
               for (; !IBQuery->Eof; IBQuery->Next())
               {
                  if(Time < GetTickCount() )
                  {
                     Application->ProcessMessages();
                     Time = GetTickCount() + 1000;
                  }

                  AnsiString UserName;
                  int ContactKey = GetUserFromDayArcBillKey(DBTransaction,IBQuery->FieldByName("ARCBILL_KEY")->AsInteger,UserName);

                  AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
                  int Key = IBQuery->FieldByName("ARCHIVE_KEY")->AsInteger;
                  TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;

                  if(ContactKey != 0)
                  {

                     int CurrentSecurityRef;
                     IBQuery1->Close();
                     IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_REF, 1) FROM RDB$DATABASE";
                     IBQuery1->ExecQuery();
                     CurrentSecurityRef = IBQuery1->Fields[0]->AsInteger;

                     int CurrentSecurityKey;
                     IBQuery1->Close();
                     IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE";
                     IBQuery1->ExecQuery();
                     CurrentSecurityKey = IBQuery1->Fields[0]->AsInteger;


                     // Add new Reocrd.
                     IBQuery1->Close();
                     IBQuery1->ParamCheck = true;
                     IBQuery1->SQL->Clear();
                     IBQuery1->SQL->Text =
                        "INSERT INTO SECURITY ("
                           "SECURITY_KEY, "
                           "SECURITY_REF, "
                           "SECURITY_EVENT, "
                           "FROM_VAL, "
                           "TO_VAL, "
                           "TERMINAL_NAME, "
                           "USER_KEY, "
                           "TIME_STAMP) "
                        "VALUES ("
                           ":SECURITY_KEY, "
                           ":SECURITY_REF, "
                           ":SECURITY_EVENT, "
                           ":FROM_VAL, "
                           ":TO_VAL, "
                           ":TERMINAL_NAME, "
                           ":USER_KEY, "
                           ":TIME_STAMP);";
                     IBQuery1->ParamByName("SECURITY_KEY")->AsInteger	= CurrentSecurityKey;
                     IBQuery1->ParamByName("SECURITY_REF")->AsInteger	= CurrentSecurityRef;
                     IBQuery1->ParamByName("USER_KEY")->AsInteger	   = ContactKey;
                     IBQuery1->ParamByName("SECURITY_EVENT")->AsString			   = SecurityTypes[secOrderedBy];
                     IBQuery1->ParamByName("FROM_VAL")->AsString	      = UserName;
                     IBQuery1->ParamByName("TO_VAL")->AsString			= "Created by Auto-updater";
                     IBQuery1->ParamByName("TIME_STAMP")->AsDateTime	= TimeStamp;
                     IBQuery1->ParamByName("TERMINAL_NAME")->AsString	= TerminalName;
                     IBQuery1->ExecQuery();

                     IBQuery1->Close();
                     IBQuery1->SQL->Text =
                     "UPDATE DAYARCHIVE"
                     " SET SECURITY_REF = :SECURITY_REF "
                     "WHERE ARCHIVE_KEY = :ARCHIVE_KEY "
                     "AND SECURITY_REF = 0";
                     IBQuery1->ParamByName("ARCHIVE_KEY")->AsInteger = Key;
                     IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
                     IBQuery1->ExecQuery();
                  }
                   CurrentCount++;
                   barProgress->Position = CurrentCount;
               }
            }
            DBTransaction.Commit();

            // Add Security to the Archives orders. -----------------------------------
            DBTransaction.StartTransaction();
            IBQuery2->SQL->Text = "SELECT COUNT(ARCHIVE_KEY) FROM ARCHIVE WHERE SECURITY_REF = 0";
            IBQuery2->ExecQuery();
            RecordCount = IBQuery2->Fields[0]->AsInteger;

            lbeProgress->Caption = "Processing " +IntToStr(RecordCount) + " archive records";
            Application->ProcessMessages();
            barProgress->Position = 0;
            barProgress->Min = 0;
            barProgress->Max = RecordCount;

            CurrentCount = 0;

            IBQuery->Close();
            IBQuery->SQL->Text =
            "SELECT ARCBILL_KEY,ARCHIVE_KEY,SERVER_NAME,TERMINAL_NAME,TIME_STAMP "
            "FROM ARCHIVE "
            "WHERE SECURITY_REF = 0";
            IBQuery->ExecQuery();
            if(!IBQuery->RecordCount == 0)
            {
               unsigned int Time;
               Time = GetTickCount() + 1000;
               for (; !IBQuery->Eof; IBQuery->Next())
               {
                  if(Time < GetTickCount() )
                  {
                     Application->ProcessMessages();
                     Time = GetTickCount() + 1000;
                  }

                  AnsiString UserName;
                  int ContactKey = GetUserFromArcBillKey(DBTransaction,IBQuery->FieldByName("ARCBILL_KEY")->AsInteger,UserName);

                  AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
                  TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
                  int Key = IBQuery->FieldByName("ARCHIVE_KEY")->AsInteger;
                  if(ContactKey != 0)
                  {

                     int CurrentSecurityRef;
                     IBQuery1->Close();
                     IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_REF, 1) FROM RDB$DATABASE";
                     IBQuery1->ExecQuery();
                     CurrentSecurityRef = IBQuery1->Fields[0]->AsInteger;

                     int CurrentSecurityKey;
                     IBQuery1->Close();
                     IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE";
                     IBQuery1->ExecQuery();
                     CurrentSecurityKey = IBQuery1->Fields[0]->AsInteger;


                     // Add new Reocrd.
                     IBQuery1->Close();
                     IBQuery1->ParamCheck = true;
                     IBQuery1->SQL->Clear();
                     IBQuery1->SQL->Text =
                        "INSERT INTO SECURITY ("
                           "SECURITY_KEY, "
                           "SECURITY_REF, "
                           "SECURITY_EVENT, "
                           "FROM_VAL, "
                           "TO_VAL, "
                           "TERMINAL_NAME, "
                           "USER_KEY, "
                           "TIME_STAMP) "
                        "VALUES ("
                           ":SECURITY_KEY, "
                           ":SECURITY_REF, "
                           ":SECURITY_EVENT, "
                           ":FROM_VAL, "
                           ":TO_VAL, "
                           ":TERMINAL_NAME, "
                           ":USER_KEY, "
                           ":TIME_STAMP);";
                     IBQuery1->ParamByName("SECURITY_KEY")->AsInteger	= CurrentSecurityKey;
                     IBQuery1->ParamByName("SECURITY_REF")->AsInteger	= CurrentSecurityRef;
                     IBQuery1->ParamByName("USER_KEY")->AsInteger	   = ContactKey;
                     IBQuery1->ParamByName("SECURITY_EVENT")->AsString			   = SecurityTypes[secOrderedBy];
                     IBQuery1->ParamByName("FROM_VAL")->AsString	      = UserName;
                     IBQuery1->ParamByName("TO_VAL")->AsString			= "Created by Auto-updater";
                     IBQuery1->ParamByName("TIME_STAMP")->AsDateTime	= TimeStamp;
                     IBQuery1->ParamByName("TERMINAL_NAME")->AsString	= TerminalName;
                     IBQuery1->ExecQuery();

                     IBQuery1->Close();
                     IBQuery1->SQL->Text =
                     "UPDATE ARCHIVE"
                     " SET SECURITY_REF = :SECURITY_REF "
                     "WHERE ARCHIVE_KEY = :ARCHIVE_KEY "
                     "AND SECURITY_REF = 0";
                     IBQuery1->ParamByName("ARCHIVE_KEY")->AsInteger = Key;
                     IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
                     IBQuery1->ExecQuery();
                  }
                   CurrentCount++;
                   barProgress->Position = CurrentCount;
               }
            }
            DBTransaction.Commit();
            DBTransaction.StartTransaction();
            
				Sleep(1000);
				IBQuery1->Close();
				IBQuery1->ParamCheck = true;
				IBQuery1->SQL->Text =
				"INSERT INTO VERSIONHISTORY ("
				" VERSION_NUMBER,"
				" TIME_STAMP,"
				" COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Patch 19905');";
				IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
				IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
				IBQuery1->ExecQuery();

				DBTransaction.Commit();
				Log("Patch 19905 " + UpdateVersion + " Successfull");
					// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception &E)
	{
		ShowMessage("The upgrade failed. Please restore from backup and contact IQWorks with the following error message : " + E.Message);
		Log(E.Message);
	}
}


int TfrmMain::GetUserFromTabKey(Database::TDBTransaction &DBTransaction,int TabKey, AnsiString &UserName)
{
   int ContactKey = 0;
	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	DBTransaction.RegisterQuery(IBInternalQuery);

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT SECURITY_REF FROM ORDERS WHERE TAB_KEY = :TAB_KEY AND SECURITY_REF != 0";
   IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger != 0)
   {
      int SecRef = IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "SELECT USER_KEY FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = :SECURITY_EVENT";
      IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString	  = SecurityTypes[secOrderedBy];
      IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger	  = SecRef;
      IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount != 0)
      {
         ContactKey = IBInternalQuery->FieldByName("USER_KEY")->AsInteger;
      }
   }
   return ContactKey;
}

int TfrmMain::GetUserFromDayArcBillKey(Database::TDBTransaction &DBTransaction,int Key, AnsiString &UserName)
{
   int ContactKey = 0;
	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	DBTransaction.RegisterQuery(IBInternalQuery);

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT SECURITY_REF FROM DAYARCHIVE WHERE ARCBILL_KEY = :ARCBILL_KEY AND SECURITY_REF != 0";
   IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = Key;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger != 0)
   {
      int SecRef = IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "SELECT USER_KEY FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = :SECURITY_EVENT";
      IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString	  = SecurityTypes[secOrderedBy];
      IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger	  = SecRef;
      IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount != 0)
      {
         ContactKey = IBInternalQuery->FieldByName("USER_KEY")->AsInteger;
      }
   }
   return ContactKey;
}

int TfrmMain::GetUserFromArcBillKey(Database::TDBTransaction &DBTransaction,int Key, AnsiString &UserName)
{
   int ContactKey = 0;
	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	DBTransaction.RegisterQuery(IBInternalQuery);

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT SECURITY_REF FROM ARCHIVE WHERE ARCBILL_KEY = :ARCBILL_KEY AND SECURITY_REF != 0";
   IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = Key;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger != 0)
   {
      int SecRef = IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "SELECT USER_KEY FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = :SECURITY_EVENT";
      IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString	  = SecurityTypes[secOrderedBy];
      IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger	  = SecRef;
      IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount != 0)
      {
         ContactKey = IBInternalQuery->FieldByName("USER_KEY")->AsInteger;
      }
   }
   return ContactKey;
}
