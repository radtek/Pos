// ---------------------------------------------------------------------------

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
#include "MMMessageBox.h"
#include "DB_5_2.h"
#include "DB_5_3.h"
#include "DB_5_4.h"
#include "DB_5_5.h"
#include "DB_5_6.h"
#include "DB_5_7.h"
#include "DB_5_8.h"
#include "DB_5_9.h"
#include "DB_5_10.h"
#include "DB_5_11.h"
#include "DB_5_12.h"
#include "DB_5_12_0_0_ChitList.h"
#include "DB_5_13_0_3_Hotfix.h"
#include "DB_5_15.h"
#include "DB_5_16.h"
#include "DB_5_17.h"

// ---------------------------------------------------------------------------
#define CHANGE   	"Change"
#define SEARCHSTRING "CashOut"
#define MMBaseKey "\\Software\\Posabl\\Posabl\\"
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmMain *frmMain;

// ---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner)
{
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
   SetProgressLabel( "" );
   SetStartTimeLabel(  "" );
   SetFinishTimeLabel( "" );

   frmMain->Caption = "Posabl Updater";
   Memo->Lines->Add("Posabl Updater Version " + GetFileVersion());
   CreateRequired = false;
   BackupRequired = true;
   GetDBLocation();
   Log("Path to Apply Updates To is " + DBPath);
   Log("Click 'Begin' To Continue...");

   SetDBPathLabel( DBPath );
}
// ---------------------------------------------------------------------------
void TfrmMain::GetDBLocation()
{
   DBControl.Disconnect();
   try
   {
	  DBPath = "";
	  RegistryRead(MMBaseKey "Database", "DatabasePath", DBPath);
	  ServerName = "";
	  RegistryRead(MMBaseKey "Database", "InterbaseIP", ServerName);
	  if (ExtractFileExt(DBPath).UpperCase() == ".GDB" || ExtractFileExt(DBPath).UpperCase() == ".IB" )
	  {
		 Log("== Database Name will need to be changed to *.FDB in Posabl ==");
		 DBPath = ChangeFileExt(DBPath, ".FDB");
	  }

      //..........................................................

      //ServerName = "localhost";
      //DBPath     = "C:\\Code\\databases\\FP GP\\MENUMATE.FDB";
      //DBPath     = "C:\\Code\\databases\\FP GP\\MENUMATE518122.FDB";

      //..........................................................

	  if (DBPath != "" && ServerName != "")
	  {
		 try
		 {
			DBControl.Disconnect();
			DBControl.Init(Database::TDBSettings(ServerName, DBPath, false));
			DBControl.Connect();
			Application->ProcessMessages();
		 }
		 catch(Exception & E)
		 {
			Log("Unable to connect to database at location " + DBPath);
			UnicodeString ErrorMessage = "Do you wish to CREATE a new Database?";
			if (Application->MessageBox(ErrorMessage.w_str(), L"Information", MB_YESNO + MB_DEFBUTTON2 + MB_ICONINFORMATION) == IDYES)
			{
			   CreateRequired = true;
			   BackupRequired = false;
			}
		 }
	  }
	  else if (FileExists("C:\\Program Files\\Posabl\\Posabl.fdb"))
	  {
		 ServerName = "localhost";
		 DBPath = "C:\\Program Files\\Posabl\\Posabl.fdb";
		 try
		 {
			Log("No Registery Settings but found DB at " + DBPath);
			DBControl.Disconnect();
			DBControl.Init(Database::TDBSettings(ServerName, DBPath, false));
			DBControl.Connect();
			Application->ProcessMessages();
		 }
		 catch(Exception & E)
		 {
			Log("Unable to connect to database at location " + DBPath);
			DBPath = "Invalid";
		 }
	  }
	  else
	  {
		 DBPath = "C:\\Program Files\\Posabl\\Posabl.fdb";
		 ServerName = "Localhost";
		 if (!FileExists(DBPath))
		 {
			Log("Will Create a new Database.");
			CreateRequired = true;
			BackupRequired = false;
		 }
		 else
		 {
			Log("Unable to create a new database a Posabl.fdb already exists.");
			DBPath = "Invalid";
		 }
	  }
   }
   catch(Exception & E)
   {
	  Log("Unable to open Database " + E.Message);
   }
}

bool TfrmMain::DB1Connect()
{
   bool Retval = true;
   try
   {
	  if (CreateRequired)
	  {
		 if (!FileExists(DBPath))
		 {
			Log("Creating Database at Location " + DBPath);
			DBControl.Disconnect();
			DBControl.Init(Database::TDBSettings(ServerName, DBPath, false));
			UpdateVersion2_1_18_X();
			CreateRequired = false;
			BackupRequired = false;
		 }
		 else
		 {
			Log("Unable to create a database at location " + DBPath + " a Posabl.IB already exists.");
		 }
	  }
	  else
	  {
		 DBControl.Disconnect();
		 DBControl.Init(Database::TDBSettings(ServerName, DBPath, false));
		 DBControl.Connect();
		 Application->ProcessMessages();
	  }
   }
   catch(Exception & E)
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
   //Memo->Lines->Add(Log);

   TListItem *item = lvLog->Items->Insert( 0 );

   item->Caption = DateTimeToStr( Now() );
   item->SubItems->Add( Log );

    SetHeaderLabel( Log );
}

void __fastcall TfrmMain::TouchBtn1MouseClick(TObject *Sender)
{
   if (DBPath == "Invalid")
   {
	  Log("Unable to connect to Invalid DB path. Checking paths again");
	  CreateRequired = false;
	  GetDBLocation();
	  Log("Path to Apply Updates To is " + DBPath);
	  Log("Click 'Begin Update' To Continue...");
   }
   else
   {
	  if (DB1Connect())
	  {

		 Database::TDBTransaction DBTransaction(DBControl);
		 DBTransaction.StartTransaction();
		 TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT MAX(TIME_STAMP) "
                              "FROM VERSIONHISTORY";
		 IBQuery->ExecQuery();
         TDateTime MaxTimeStamp = IBQuery->FieldByName("MAX")->AsDateTime;
         IBQuery->Close();
         IBQuery->SQL->Text = "SELECT VERSION_NUMBER "
                              "FROM VERSIONHISTORY "
                              "WHERE TIME_STAMP = :TimeStamp";
         IBQuery->ParamByName("TimeStamp")->AsDateTime = MaxTimeStamp;
         IBQuery->ExecQuery();
		 if (IBQuery->RecordCount > 0)
		 {
			Log("Last Version Applied Was " + IBQuery->FieldByName("VERSION_NUMBER")->AsString);
		 }

		 DBTransaction.Commit();

		 UnicodeString ErrorMessage = "Do you wish to do a database backup? (Recommended)";
		 if (BackupRequired && Application->MessageBox(ErrorMessage.w_str(), L"Information", MB_YESNO + MB_DEFBUTTON2 + MB_ICONINFORMATION)
			== IDYES)
		 {
			if (FileExists(ExtractFilePath(Application->ExeName) + "IBTools.exe"))
			{
			   HWND PrevWnd;
			   PrevWnd = FindWindow(_T("TIBTools"), _T("IBTools"));
			   _wspawnl(P_WAIT, (ExtractFilePath(Application->ExeName) + "IBTools.exe").w_str(),
				  (ExtractFilePath(Application->ExeName) + "IBTools.exe").w_str(), AnsiString("\"") + DBPath.c_str() + AnsiString("\""),
				  NULL);

			   ::Sleep(1000);
			   PrevWnd = FindWindow(_T("TIBTools"), _T("IBTools"));
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
			   MessageBox("IBTools not found.", "Information", MB_OK + MB_ICONERROR);
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
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::btnCloseClick(TObject *Sender)
{
   Close();
}
// ---------------------------------------------------------------------------

void TfrmMain::UpDateAll()
{
    SetProgressLabel( "Progress ... " );
    SetStartTimeLabel(  DateTimeToStr( Now() ) );
    SetProgressBarMax( GetVersionCount() );

    TDateTime BeginTime = Now();
    Log("Begin Time : " + BeginTime.FormatString("dd/mm/yyyy hh:nn:ss"));

    UpdateVersion2_1_19_X();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion2_2_00_X();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion2_3_00_X();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion2_4_00_X();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion2_5_00_X();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion2_6_00_X();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion2_7_00_X();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_2_8();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_2_9();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_0();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_1();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_2();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_21();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_22();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_23();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_231();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_232();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_24();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_3();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_31();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_32();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_33();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_34();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_3_35();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_0();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_1();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_2();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_3();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_4();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_5();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_6();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_7();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_8();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_9();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_10();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_11();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_12();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_13();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_4_14();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_5_0();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_5_1();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_5_2();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_5_3();
    StepProgressBar( 1 );
    Application->ProcessMessages();
    UpdateVersion_5_4();
    StepProgressBar( 1 );
    Application->ProcessMessages();
	UpdateVersion_5_5();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_6();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_7();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_8();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_9();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_10();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_11();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_12();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_13();
    StepProgressBar( 1 );
	Application->ProcessMessages();
    UpdateVersion_5_13_0_3();
    StepProgressBar( 1 );
	Application->ProcessMessages();
    UpdateVersion_5_14();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_15();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_16();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersion_5_17();
    StepProgressBar( 1 );
	Application->ProcessMessages();
	UpdateVersionNew();
	//Application->ProcessMessages();

	TDateTime EndTime = Now();
	Log("End Time : " + EndTime.FormatString("dd/mm/yyyy hh:nn:ss"));
    Log("Elasped Time " + IntToStr(HoursBetween(EndTime, BeginTime)) + ":" + IntToStr(MinutesBetween(EndTime, BeginTime)) + ":" + IntToStr
	  (SecondsBetween(EndTime, BeginTime)));

    SetFinishTimeLabel( DateTimeToStr( Now() ) );
    SetProgressLabel( "Done" );
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion2_1_18_X()
{
   AnsiString UpdateFromVersion = "";
   AnsiString UpdateVersion = "2.1.18";

   Database::TDBUtilities *MyUtil = new Database::TDBUtilities(DBControl);
   MyUtil->CreateDatabase();
   DBControl.Connect();

   Database::TDBTransaction DBTransaction(DBControl);
   TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());

   AnsiString RawSQL = memMMCreateSQL->Lines->Text;
   int StringBegin = 1;
   int StringEnd = RawSQL.Pos(";");
   while (StringEnd != 0)
   {
	  AnsiString CurrentSQL = RawSQL.SubString(StringBegin, StringEnd);
	  RawSQL = RawSQL.SubString(StringEnd + 1, RawSQL.Length() - StringEnd);
	  StringEnd = RawSQL.Pos(";");

	  DBTransaction.StartTransaction();
	  IBQuery1->Close();
	  IBQuery1->SQL->Text = CurrentSQL;
	  IBQuery1->ExecQuery();
	  DBTransaction.Commit();
   }

   DBTransaction.StartTransaction();

   IBQuery1->Close();
   IBQuery1->SQL->Text = "CREATE TRIGGER MENUDELETE FOR MENU AFTER DELETE AS BEGIN POST_EVENT 'EVENT_MENUDELETE'; END";
   IBQuery1->ExecQuery();

   IBQuery1->Close();
   IBQuery1->SQL->Text = "CREATE TRIGGER MENUINSERT FOR MENU AFTER INSERT AS BEGIN POST_EVENT 'EVENT_MENUINSERT'; END";
   IBQuery1->ExecQuery();

   IBQuery1->Close();
   IBQuery1->SQL->Text = "CREATE TRIGGER TABCHANGED FOR TAB AFTER DELETE AS BEGIN POST_EVENT 'EVENT_TABCHANGED'; END";
   IBQuery1->ExecQuery();

   DBTransaction.Commit();

}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion2_1_19_X()
{
   AnsiString UpdateFromVersion = "2.1.18.0";
   AnsiString UpdateVersion = "2.1.19.%";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE PAYMENTTYPES (" " PAYMENT_KEY INTEGER NOT NULL PRIMARY KEY,"
				" PAYMENT_NAME VARCHAR(30)," " PROPERTIES NUMERIC(15,4)," " EXCHANGE_RATE NUMERIC(15,4),"
				" COLOUR INTEGER," " DISPLAY_ORDER INTEGER);";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILLPAY ADD VOUCHER_NUMBER VARCHAR(50);";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILLPAY ADD VOUCHER_NUMBER VARCHAR(50);";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL ADD PATRON_COUNT INTEGER;";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL ADD PATRON_COUNT INTEGER;";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILLPAY ADD CASH_OUT T_TRUEFALSE;";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILLPAY ADD CASH_OUT T_TRUEFALSE;";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL ADD RECEIPT BLOB SUB_TYPE 1;";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL ADD RECEIPT BLOB SUB_TYPE 1;";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_PAYMENTTYPES;";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_PAYMENTTYPES TO 0;";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCBILLPAY" " SET CASH_OUT = :CASH_OUT " "WHERE CASH_OUT IS NULL";
			IBQuery1->ParamByName("CASH_OUT")->AsString = "F";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DAYARCBILLPAY" " SET CASH_OUT = :CASH_OUT " "WHERE CASH_OUT IS NULL";
			IBQuery1->ParamByName("CASH_OUT")->AsString = "F";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			AnsiString SearchString = SEARCHSTRING;

			DBTransaction.StartTransaction();
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM ARCBILLPAY WHERE PAY_TYPE LIKE '%CashOut'";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   for (; !IBQuery->Eof; IBQuery->Next())
			   {

				  AnsiString PayType = IBQuery->FieldByName("PAY_TYPE")->AsString;
				  int Length = PayType.Length() - SearchString.Length();
				  PayType = PayType.SubString(1, Length);
				  // Change the CashOut from - to +;
				  // Change the CashOut from XXXXCashOut to XXXXX.
				  // Change the CASH_OUT field from F to T;
				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE ARCBILLPAY" " SET SUBTOTAL = :SUBTOTAL, " " CASH_OUT = :CASH_OUT, "
					  " PAY_TYPE = :PAY_TYPE " "WHERE ARCBILLPAY_KEY = :ARCBILLPAY_KEY";
				  IBQuery1->ParamByName("SUBTOTAL")->AsFloat = -IBQuery->FieldByName("SUBTOTAL")->AsFloat;
				  IBQuery1->ParamByName("CASH_OUT")->AsString = "T";
				  IBQuery1->ParamByName("PAY_TYPE")->AsString = PayType;
				  IBQuery1->ParamByName("ARCBILLPAY_KEY")->AsInteger = IBQuery->FieldByName("ARCBILLPAY_KEY")->AsInteger;
				  IBQuery1->ExecQuery();

				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCBILLPAY, 1) FROM RDB$DATABASE";
				  IBInternalQuery->ExecQuery();
				  int PaymentKey = IBInternalQuery->Fields[0]->AsInteger;

				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text =
					  "INSERT INTO ARCBILLPAY (" "ARCBILLPAY_KEY, " "ARCBILL_KEY, " "PAY_TYPE, " "VOUCHER_NUMBER, "
					  "SUBTOTAL, " "ROUNDING, " "CASH_OUT, " "TAX_FREE, " "NOTE, " "PROPERTIES, " "GROUP_NUMBER) " "VALUES ("
					  ":ARCBILLPAY_KEY, " ":ARCBILL_KEY, " ":PAY_TYPE, " ":VOUCHER_NUMBER, " ":SUBTOTAL, " ":ROUNDING, " ":CASH_OUT, "
					  ":TAX_FREE, " ":NOTE, " ":PROPERTIES, " ":GROUP_NUMBER) ";

				  IBInternalQuery->ParamByName("ARCBILLPAY_KEY")->AsInteger = PaymentKey;
				  IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = IBQuery->FieldByName("ARCBILL_KEY")->AsInteger;
				  IBInternalQuery->FieldByName("PAY_TYPE")->AsString = CHANGE;
				  IBInternalQuery->FieldByName("SUBTOTAL")->AsFloat = IBQuery->FieldByName("SUBTOTAL")->AsFloat;
				  IBInternalQuery->FieldByName("VOUCHER_NUMBER")->AsString = "";
				  IBInternalQuery->FieldByName("CASH_OUT")->AsString = "F";
				  IBInternalQuery->ExecQuery();
			   }
			}
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM DAYARCBILLPAY WHERE PAY_TYPE LIKE '%CashOut'";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   for (; !IBQuery->Eof; IBQuery->Next())
			   {
				  AnsiString PayType = IBQuery->FieldByName("PAY_TYPE")->AsString;
				  int Length = PayType.Length() - SearchString.Length();
				  PayType = PayType.SubString(1, Length);
				  // Change the CashOut from - to +;
				  // Change the CashOut from XXXXCashOut to XXXXX.
				  // Change the CASH_OUT field from F to T;
				  IBQuery1->Close();
				  IBQuery1->SQL->Text =
					  "UPDATE DAYARCBILLPAY" " SET SUBTOTAL = :SUBTOTAL, " " CASH_OUT = :CASH_OUT, "
					  " PAY_TYPE = :PAY_TYPE " "WHERE DAYARCBILLPAY_KEY = :DAYARCBILLPAY_KEY";
				  IBQuery1->ParamByName("SUBTOTAL")->AsFloat = -IBQuery->FieldByName("SUBTOTAL")->AsFloat;
				  IBQuery1->ParamByName("CASH_OUT")->AsString = "T";
				  IBQuery1->ParamByName("PAY_TYPE")->AsString = PayType;
				  IBQuery1->ParamByName("DAYARCBILLPAY_KEY")->AsInteger = IBQuery->FieldByName("DAYARCBILLPAY_KEY")->AsInteger;
				  IBQuery1->ExecQuery();

				  // Get New Key
				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCBILLPAY, 1) FROM RDB$DATABASE";
				  IBInternalQuery->ExecQuery();
				  int PaymentKey = IBInternalQuery->Fields[0]->AsInteger;

				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text =
					  "INSERT INTO DAYARCBILLPAY (" "DAYARCBILLPAY_KEY, " "ARCBILL_KEY, " "PAY_TYPE, " "VOUCHER_NUMBER, "
					  "SUBTOTAL, " "ROUNDING, " "CASH_OUT, " "TAX_FREE, " "NOTE, " "PROPERTIES, " "GROUP_NUMBER) " "VALUES ("
					  ":DAYARCBILLPAY_KEY, " ":ARCBILL_KEY, " ":PAY_TYPE, " ":VOUCHER_NUMBER, " ":SUBTOTAL, " ":ROUNDING, " ":CASH_OUT, "
					  ":TAX_FREE, " ":NOTE, " ":PROPERTIES, " ":GROUP_NUMBER) ";

				  IBInternalQuery->ParamByName("DAYARCBILLPAY_KEY")->AsInteger = PaymentKey;
				  IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = IBQuery->FieldByName("ARCBILL_KEY")->AsInteger;
				  IBInternalQuery->FieldByName("PAY_TYPE")->AsString = CHANGE;
				  IBInternalQuery->FieldByName("SUBTOTAL")->AsFloat = IBQuery->FieldByName("SUBTOTAL")->AsFloat;
				  IBInternalQuery->FieldByName("VOUCHER_NUMBER")->AsString = "";
				  IBInternalQuery->FieldByName("CASH_OUT")->AsString = "F";
				  IBInternalQuery->ExecQuery();
			   }
			}
			DBTransaction.Commit();

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCBILLPAY" " SET PAY_TYPE = :PAY_TYPE " "WHERE PAY_TYPE = 'EFTPOS'";
			IBQuery1->ParamByName("PAY_TYPE")->AsString = "Eftpos";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DAYARCBILLPAY" " SET PAY_TYPE = :PAY_TYPE " "WHERE PAY_TYPE = 'EFTPOS'";
			IBQuery1->ParamByName("PAY_TYPE")->AsString = "Eftpos";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			Log("Update " + UpdateVersion + " Successfull");
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion2_2_00_X()
{
   bool RunAlterartions = false;
   AnsiString UpdateFromVersion = "2.1.19.%";
   AnsiString UpdateVersion = "2.2";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			RunAlterartions = true;
			// ZEDS --------------------------------------
			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE ZEDS (" " Z_KEY INTEGER NOT NULL PRIMARY KEY," " INITIAL_FLOAT NUMERIC(15,4),"
				" ZED_CID NUMERIC(15,4)," // Zed Total cash in drawer.
			" SKIMS_TOTAL NUMERIC(15,4)," // Skimming and refloats.
			" CASH_COUNT NUMERIC(15,4)," // Count by the bar staff.
			" OFFICE_COUNT NUMERIC(15,4)," // Count by office staff
			" ADJUSTMENTS NUMERIC(15,4)," // Adjustment (out by) as counted by office staff.
			" REPORT BLOB SUB_TYPE 1," " TIME_STAMP TIMESTAMP," // Is applied when Till is Zedded.
			" TERMINAL_NAME VARCHAR(22)," " SECURITY_REF INTEGER );";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_ZED;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_ZED TO 0;";
			IBQuery1->ExecQuery();

			// Locations --------------------------------------
			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE LOCATIONS (" " LOCATION_KEY INTEGER NOT NULL PRIMARY KEY," " NAME VARCHAR(25) );";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DEVICES ADD LOCATION_KEY INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_LOCATION;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE DEVICES ADD FOREIGN KEY (LOCATION_KEY) REFERENCES LOCATIONS (LOCATION_KEY) ON UPDATE CASCADE ON DELETE SET NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_LOCATION TO 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD LOCATION VARCHAR(25)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD LOCATION VARCHAR(25)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD LOCATION VARCHAR(25)";
			IBQuery1->ExecQuery();

			// Security --------------------------------------

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE SECURITY (" " SECURITY_KEY INTEGER NOT NULL PRIMARY KEY," " SECURITY_REF INTEGER NOT NULL,"
				" SECURITY_EVENT VARCHAR(50)," " FROM_VAL VARCHAR(50)," " TO_VAL VARCHAR(50)," " NOTE VARCHAR(50),"
				" TERMINAL_NAME VARCHAR(22)," " USER_KEY INTEGER NOT NULL,"
				" FOREIGN KEY (USER_KEY) REFERENCES USERS (USER_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " TIME_STAMP TIMESTAMP);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_SECURITY_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_SECURITY_REF;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_SECURITY_REF TO 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL ADD SECURITY_REF INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL ADD SECURITY_REF INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD SECURITY_REF INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD SECURITY_REF INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD SECURITY_REF INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERMIX ADD SECURITY_REF INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX SEC_REF_INDEX ON SECURITY SECURITY_REF;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX SEC_EVENT_INDEX ON SECURITY SECURITY_EVENT;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX SEC_TIMESTAMP_INDEX ON SECURITY TIME_STAMP;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX SEC_TERMINAL_NAME_INDEX ON SECURITY TERMINAL_NAME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ARC_SECURITY_REF_INDEX ON ARCHIVE SECURITY_REF;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ARC_TAB_NAME_INDEX ON ARCHIVE TAB_NAME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ARC_MENU_NAME_INDEX ON ARCHIVE MENU_NAME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ARC_SIZE_NAME_INDEX ON ARCHIVE SIZE_NAME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ARCBILL_SECURITY_REF_INDEX ON ARCBILL SECURITY_REF;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ARCBP_PAY_TYPE_INDEX ON ARCBILLPAY PAY_TYPE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP INDEX ARCHIVEINDEX;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP INDEX ARCHIVEINDEX2;";
			IBQuery1->ExecQuery();

			// DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();
			// DBTransaction.Commit();
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
	  DBTransaction.Commit();

	  if (RunAlterartions)
	  {
		 AnsiString SearchString = SEARCHSTRING;

		 // Add All Previous Users back into the Users table-----------------------

		 DBTransaction.StartTransaction();
		 IBQuery2->Close();
		 IBQuery2->SQL->Text = "SELECT COUNT(DISTINCT(SERVER_NAME)) FROM ARCHIVE WHERE SECURITY_REF IS NULL";
		 IBQuery2->ExecQuery();
		 int RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " user names";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 int CurrentCount = 0;

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT DISTINCT(SERVER_NAME) FROM ARCHIVE WHERE SECURITY_REF IS NULL";
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			unsigned int Time;
			Time = GetTickCount() + 1000;
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   if (Time < GetTickCount())
			   {
				  Application->ProcessMessages();
				  Time = GetTickCount() + 1000;
			   }
			   AnsiString UserName = IBQuery->FieldByName("SERVER_NAME")->AsString;

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "SELECT USER_KEY FROM USERS " "WHERE NAME = :NAME";
			   IBQuery1->ParamByName("NAME")->AsString = UserName;
			   IBQuery1->ExecQuery();
			   int UserKey = IBQuery1->FieldByName("USER_KEY")->AsInteger;
			   if (UserKey == 0)
			   {
				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_USERS, 1) FROM RDB$DATABASE";
				  IBQuery1->ExecQuery();
				  UserKey = IBQuery1->Fields[0]->AsInteger;

				  // Add new Reocrd.
				  IBQuery1->Close();
				  IBQuery1->SQL->Clear();
				  IBQuery1->SQL->Text =
					  "INSERT INTO USERS (" "USER_KEY," "NAME, " "USER_TYPE) " "VALUES (" ":USER_KEY," ":NAME, " ":USER_TYPE);";
				  IBQuery1->ParamByName("USER_KEY")->AsInteger = UserKey;
				  IBQuery1->ParamByName("NAME")->AsString = UserName;
				  IBQuery1->ParamByName("USER_TYPE")->AsInteger = eDeletedStaff;
				  IBQuery1->ExecQuery();
			   }
			   CurrentCount++;
			   barProgress->Position = CurrentCount;
			}
		 }
		 DBTransaction.Commit();

		 // Add Security to the current orders. -----------------------------------
		 DBTransaction.StartTransaction();
		 IBQuery2->Close();
		 IBQuery2->SQL->Text = "SELECT COUNT(ORDER_KEY) FROM ORDERS WHERE SECURITY_REF IS NULL";
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " order records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 CurrentCount = 0;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT ORDER_KEY,SERVER_NAME,TERMINAL_NAME,TIME_STAMP,USERS.USER_KEY " "FROM ORDERS INNER JOIN USERS ON USERS.NAME = ORDERS.SERVER_NAME " "WHERE SECURITY_REF IS NULL";
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			unsigned int Time;
			Time = GetTickCount() + 1000;
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   if (Time < GetTickCount())
			   {
				  Application->ProcessMessages();
				  Time = GetTickCount() + 1000;
			   }
			   AnsiString UserName = IBQuery->FieldByName("SERVER_NAME")->AsString;
			   AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
			   int OrderKey = IBQuery->FieldByName("ORDER_KEY")->AsInteger;
			   TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
			   int UserKey = IBQuery->FieldByName("USER_KEY")->AsInteger;
			   if (UserKey != 0)
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
					  "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, "
					  "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, " ":SECURITY_EVENT, "
					  ":FROM_VAL, " ":TO_VAL, " ":TERMINAL_NAME, " ":USER_KEY, " ":TIME_STAMP);";
				  IBQuery1->ParamByName("SECURITY_KEY")->AsInteger = CurrentSecurityKey;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ParamByName("USER_KEY")->AsInteger = UserKey;
				  IBQuery1->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secOrderedBy];
				  IBQuery1->ParamByName("FROM_VAL")->AsString = UserName;
				  IBQuery1->ParamByName("TO_VAL")->AsString = "Created by Auto-updater";
				  IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = TimeStamp;
				  IBQuery1->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
				  IBQuery1->ExecQuery();

				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE ORDERS" " SET SECURITY_REF = :SECURITY_REF " "WHERE ORDER_KEY = :ORDER_KEY "
					  "AND SECURITY_REF IS NULL";
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
		 IBQuery2->SQL->Text = "SELECT COUNT(ARCHIVE_KEY) FROM DAYARCHIVE WHERE SECURITY_REF IS NULL";
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " day archive records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 CurrentCount = 0;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT ARCHIVE_KEY,SERVER_NAME,TERMINAL_NAME,TIME_STAMP,USERS.USER_KEY " "FROM DAYARCHIVE INNER JOIN USERS ON USERS.NAME = DAYARCHIVE.SERVER_NAME " "WHERE SECURITY_REF IS NULL";
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			unsigned int Time;
			Time = GetTickCount() + 1000;
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   if (Time < GetTickCount())
			   {
				  Application->ProcessMessages();
				  Time = GetTickCount() + 1000;
			   }
			   AnsiString UserName = IBQuery->FieldByName("SERVER_NAME")->AsString;
			   AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
			   int Key = IBQuery->FieldByName("ARCHIVE_KEY")->AsInteger;
			   TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
			   int UserKey = IBQuery->FieldByName("USER_KEY")->AsInteger;

			   if (UserKey != 0)
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
					  "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, "
					  "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, " ":SECURITY_EVENT, "
					  ":FROM_VAL, " ":TO_VAL, " ":TERMINAL_NAME, " ":USER_KEY, " ":TIME_STAMP);";
				  IBQuery1->ParamByName("SECURITY_KEY")->AsInteger = CurrentSecurityKey;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ParamByName("USER_KEY")->AsInteger = UserKey;
				  IBQuery1->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secOrderedBy];
				  IBQuery1->ParamByName("FROM_VAL")->AsString = UserName;
				  IBQuery1->ParamByName("TO_VAL")->AsString = "Created by Auto-updater";
				  IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = TimeStamp;
				  IBQuery1->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
				  IBQuery1->ExecQuery();

				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE DAYARCHIVE" " SET SECURITY_REF = :SECURITY_REF " "WHERE ARCHIVE_KEY = :ARCHIVE_KEY "
					  "AND SECURITY_REF IS NULL";
				  IBQuery1->ParamByName("ARCHIVE_KEY")->AsInteger = Key;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ExecQuery();
			   }
			   CurrentCount++;
			   barProgress->Position = CurrentCount;
			}
		 }
		 DBTransaction.Commit();

		 // Add Billed by Security to the DayArcBill. -----------------------------------

		 DBTransaction.StartTransaction();
		 IBQuery2->Close();
		 IBQuery2->SQL->Text = "SELECT COUNT(ARCBILL_KEY) FROM DAYARCBILL WHERE SECURITY_REF IS NULL";
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " day billing records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 CurrentCount = 0;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT ARCBILL_KEY,STAFF_NAME,TERMINAL_NAME,TIME_STAMP,USERS.USER_KEY " "FROM DAYARCBILL INNER JOIN USERS ON USERS.NAME = DAYARCBILL.STAFF_NAME " "WHERE SECURITY_REF IS NULL";
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			unsigned int Time;
			Time = GetTickCount() + 1000;
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   if (Time < GetTickCount())
			   {
				  Application->ProcessMessages();
				  Time = GetTickCount() + 1000;
			   }
			   AnsiString UserName = IBQuery->FieldByName("STAFF_NAME")->AsString;
			   AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
			   TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
			   int Key = IBQuery->FieldByName("ARCBILL_KEY")->AsInteger;
			   int UserKey = IBQuery->FieldByName("USER_KEY")->AsInteger;

			   if (UserKey != 0)
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
					  "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, "
					  "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, " ":SECURITY_EVENT, "
					  ":FROM_VAL, " ":TO_VAL, " ":TERMINAL_NAME, " ":USER_KEY, " ":TIME_STAMP);";
				  IBQuery1->ParamByName("SECURITY_KEY")->AsInteger = CurrentSecurityKey;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ParamByName("USER_KEY")->AsInteger = UserKey;
				  IBQuery1->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secBilledBy];
				  IBQuery1->ParamByName("FROM_VAL")->AsString = UserName;
				  IBQuery1->ParamByName("TO_VAL")->AsString = "Created by Auto-updater";
				  IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = TimeStamp;
				  IBQuery1->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
				  IBQuery1->ExecQuery();

				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE DAYARCBILL" " SET SECURITY_REF = :SECURITY_REF " "WHERE ARCBILL_KEY = :ARCBILL_KEY "
					  "AND SECURITY_REF IS NULL";
				  IBQuery1->ParamByName("ARCBILL_KEY")->AsInteger = Key;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ExecQuery();
			   }
			   CurrentCount++;
			   barProgress->Position = CurrentCount;
			}
		 }
		 DBTransaction.Commit();

		 // Add Billed By Security to the ArcBill. ------------------------------------------

		 DBTransaction.StartTransaction();

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT COUNT(ARCBILL_KEY) FROM ARCBILL WHERE SECURITY_REF IS NULL";
		 IBQuery->ExecQuery();
		 RecordCount = IBQuery->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " billing records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 CurrentCount = 0;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT ARCBILL_KEY,STAFF_NAME,TERMINAL_NAME,TIME_STAMP,USERS.USER_KEY " "FROM ARCBILL INNER JOIN USERS ON USERS.NAME = ARCBILL.STAFF_NAME " "WHERE SECURITY_REF IS NULL";
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			unsigned int Time;
			Time = GetTickCount() + 1000;
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   if (Time < GetTickCount())
			   {
				  Application->ProcessMessages();
				  Time = GetTickCount() + 1000;
			   }
			   AnsiString UserName = IBQuery->FieldByName("STAFF_NAME")->AsString;
			   AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
			   TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
			   int Key = IBQuery->FieldByName("ARCBILL_KEY")->AsInteger;
			   int UserKey = IBQuery->FieldByName("USER_KEY")->AsInteger;

			   if (UserKey != 0)
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
					  "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, "
					  "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, " ":SECURITY_EVENT, "
					  ":FROM_VAL, " ":TO_VAL, " ":TERMINAL_NAME, " ":USER_KEY, " ":TIME_STAMP);";
				  IBQuery1->ParamByName("SECURITY_KEY")->AsInteger = CurrentSecurityKey;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ParamByName("USER_KEY")->AsInteger = UserKey;
				  IBQuery1->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secBilledBy];
				  IBQuery1->ParamByName("FROM_VAL")->AsString = UserName;
				  IBQuery1->ParamByName("TO_VAL")->AsString = "Created by Auto-updater";
				  IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = TimeStamp;
				  IBQuery1->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
				  IBQuery1->ExecQuery();

				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE ARCBILL" " SET SECURITY_REF = :SECURITY_REF " "WHERE ARCBILL_KEY = :ARCBILL_KEY "
					  "AND SECURITY_REF IS NULL";
				  IBQuery1->ParamByName("ARCBILL_KEY")->AsInteger = Key;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ExecQuery();
			   }
			   CurrentCount++;
			   barProgress->Position = CurrentCount;
			}
		 }
		 DBTransaction.Commit();

		 // Add Discount Security to the ArcBills -------------------------------

		 DBTransaction.StartTransaction();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text = "SELECT COUNT(ARCBILL_KEY) FROM ARCBILL WHERE DISCOUNT != 0 " "AND SECURITY_REF NOT IN "
			 "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secDiscountedBy];
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " discount records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT ARCBILL_KEY,STAFF_NAME,DISCOUNT,TERMINAL_NAME,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM ARCBILL INNER JOIN USERS ON USERS.NAME = ARCBILL.STAFF_NAME " "WHERE DISCOUNT != 0 AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secDiscountedBy];
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			unsigned int Time;
			Time = GetTickCount() + 1000;
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   if (Time < GetTickCount())
			   {
				  Application->ProcessMessages();
				  Time = GetTickCount() + 1000;
			   }
			   AnsiString UserName = IBQuery->FieldByName("STAFF_NAME")->AsString;
			   AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
			   TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
			   int Key = IBQuery->FieldByName("ARCBILL_KEY")->AsInteger;
			   int UserKey = IBQuery->FieldByName("USER_KEY")->AsInteger;
			   int CurrentSecurityRef = IBQuery->FieldByName("SECURITY_REF")->AsInteger;
			   Currency Discount = IBQuery->FieldByName("DISCOUNT")->AsFloat;

			   if (UserKey != 0)
			   {
				  if (CurrentSecurityRef == 0)
				  {
					 IBQuery1->Close();
					 IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_REF, 1) FROM RDB$DATABASE";
					 IBQuery1->ExecQuery();
					 CurrentSecurityRef = IBQuery1->Fields[0]->AsInteger;
				  }

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
					  "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, "
					  "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, " ":SECURITY_EVENT, "
					  ":FROM_VAL, " ":TO_VAL, " ":TERMINAL_NAME, " ":USER_KEY, " ":TIME_STAMP);";
				  IBQuery1->ParamByName("SECURITY_KEY")->AsInteger = CurrentSecurityKey;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ParamByName("USER_KEY")->AsInteger = UserKey;
				  IBQuery1->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secDiscountedBy];
				  IBQuery1->ParamByName("FROM_VAL")->AsString = "0";
				  IBQuery1->ParamByName("TO_VAL")->AsString = CurrToStrF(Discount, ffCurrency, 2);
				  IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = TimeStamp;
				  IBQuery1->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
				  IBQuery1->ExecQuery();

				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE ARCBILL" " SET SECURITY_REF = :SECURITY_REF " "WHERE ARCBILL_KEY = :ARCBILL_KEY";
				  IBQuery1->ParamByName("ARCBILL_KEY")->AsInteger = Key;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ExecQuery();
			   }
			   CurrentCount++;
			   barProgress->Position = CurrentCount;
			}
		 }
		 DBTransaction.Commit();

		 // Add Discount Security to the DayArcBill -------------------------------

		 DBTransaction.StartTransaction();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text = "SELECT COUNT(ARCBILL_KEY) FROM DAYARCBILL WHERE DISCOUNT != 0 " "AND SECURITY_REF NOT IN "
			 "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secDiscountedBy];
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " daily discounted records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT ARCBILL_KEY,STAFF_NAME,DISCOUNT,TERMINAL_NAME,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM DAYARCBILL INNER JOIN USERS ON USERS.NAME = DAYARCBILL.STAFF_NAME " "WHERE DISCOUNT != 0 AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secDiscountedBy];
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			unsigned int Time;
			Time = GetTickCount() + 1000;
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   if (Time < GetTickCount())
			   {
				  Application->ProcessMessages();
				  Time = GetTickCount() + 1000;
			   }
			   AnsiString UserName = IBQuery->FieldByName("STAFF_NAME")->AsString;
			   AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
			   TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
			   int Key = IBQuery->FieldByName("ARCBILL_KEY")->AsInteger;
			   int UserKey = IBQuery->FieldByName("USER_KEY")->AsInteger;
			   int CurrentSecurityRef = IBQuery->FieldByName("SECURITY_REF")->AsInteger;
			   Currency Discount = IBQuery->FieldByName("DISCOUNT")->AsFloat;

			   if (UserKey != 0)
			   {
				  if (CurrentSecurityRef == 0)
				  {
					 IBQuery1->Close();
					 IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_REF, 1) FROM RDB$DATABASE";
					 IBQuery1->ExecQuery();
					 CurrentSecurityRef = IBQuery1->Fields[0]->AsInteger;
				  }

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
					  "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, "
					  "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, " ":SECURITY_EVENT, "
					  ":FROM_VAL, " ":TO_VAL, " ":TERMINAL_NAME, " ":USER_KEY, " ":TIME_STAMP);";
				  IBQuery1->ParamByName("SECURITY_KEY")->AsInteger = CurrentSecurityKey;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ParamByName("USER_KEY")->AsInteger = UserKey;
				  IBQuery1->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secDiscountedBy];
				  IBQuery1->ParamByName("FROM_VAL")->AsString = "0";
				  IBQuery1->ParamByName("TO_VAL")->AsString = CurrToStrF(Discount, ffCurrency, 2); ;
				  IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = TimeStamp;
				  IBQuery1->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
				  IBQuery1->ExecQuery();

				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE DAYARCBILL" " SET SECURITY_REF = :SECURITY_REF " "WHERE ARCBILL_KEY = :ARCBILL_KEY";
				  IBQuery1->ParamByName("ARCBILL_KEY")->AsInteger = Key;
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
		 IBQuery2->SQL->Text = "SELECT COUNT(ARCHIVE_KEY) FROM ARCHIVE WHERE SECURITY_REF IS NULL";
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " archive records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 CurrentCount = 0;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT ARCHIVE_KEY,SERVER_NAME,TERMINAL_NAME,TIME_STAMP,USERS.USER_KEY " "FROM ARCHIVE INNER JOIN USERS ON USERS.NAME = ARCHIVE.SERVER_NAME " "WHERE SECURITY_REF IS NULL";
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			unsigned int Time;
			Time = GetTickCount() + 1000;
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   if (Time < GetTickCount())
			   {
				  Application->ProcessMessages();
				  Time = GetTickCount() + 1000;
			   }
			   AnsiString UserName = IBQuery->FieldByName("SERVER_NAME")->AsString;
			   AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
			   TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
			   int Key = IBQuery->FieldByName("ARCHIVE_KEY")->AsInteger;
			   int UserKey = IBQuery->FieldByName("USER_KEY")->AsInteger;
			   if (UserKey != 0)
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
					  "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, "
					  "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, " ":SECURITY_EVENT, "
					  ":FROM_VAL, " ":TO_VAL, " ":TERMINAL_NAME, " ":USER_KEY, " ":TIME_STAMP);";
				  IBQuery1->ParamByName("SECURITY_KEY")->AsInteger = CurrentSecurityKey;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ParamByName("USER_KEY")->AsInteger = UserKey;
				  IBQuery1->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secOrderedBy];
				  IBQuery1->ParamByName("FROM_VAL")->AsString = UserName;
				  IBQuery1->ParamByName("TO_VAL")->AsString = "Created by Auto-updater";
				  IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = TimeStamp;
				  IBQuery1->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
				  IBQuery1->ExecQuery();

				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE ARCHIVE" " SET SECURITY_REF = :SECURITY_REF " "WHERE ARCHIVE_KEY = :ARCHIVE_KEY "
					  "AND SECURITY_REF IS NULL";
				  IBQuery1->ParamByName("ARCHIVE_KEY")->AsInteger = Key;
				  IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
				  IBQuery1->ExecQuery();
			   }
			   CurrentCount++;
			   barProgress->Position = CurrentCount;
			}
		 }
		 DBTransaction.Commit();

		 // Add Cancels Security ------------------------------------------
		 // Add Cancels Security ------------------------------------------
		 // Add Cancels Security ------------------------------------------
		 // Add Cancels Security ------------------------------------------
		 DBTransaction.StartTransaction();
		 IBQuery2->Close();
		 IBQuery2->SQL->Text =
			 "SELECT COUNT(ORDER_KEY) FROM ORDERS WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCancel];
		 IBQuery2->ParamByName("ORDER_TYPE")->AsInteger = CreditOrder;
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " current cancel records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT SERVER_NAME,TERMINAL_NAME, PRICE, PRICE_ADJUST,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM ORDERS INNER JOIN USERS ON USERS.NAME = ORDERS.SERVER_NAME " "WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCancel];
		 IBQuery->ParamByName("ORDER_TYPE")->AsInteger = CreditOrder;
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			Ver2_1_19_X_UpdateOrderTypeSecurity(DBTransaction, IBQuery, secCancel);
		 }
		 DBTransaction.Commit();
		 // Add Cancel Security to the Day Archives--------------------------------

		 DBTransaction.StartTransaction();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text =
			 "SELECT COUNT(ARCHIVE_KEY) FROM DAYARCHIVE WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCancel];
		 IBQuery2->ParamByName("ORDER_TYPE")->AsInteger = CreditOrder;
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " daily cancel records";
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT SERVER_NAME,TERMINAL_NAME, PRICE, PRICE_ADJUST,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM DAYARCHIVE INNER JOIN USERS ON USERS.NAME = DAYARCHIVE.SERVER_NAME " "WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCancel];
		 IBQuery->ParamByName("ORDER_TYPE")->AsInteger = CreditOrder;
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			Ver2_1_19_X_UpdateOrderTypeSecurity(DBTransaction, IBQuery, secCancel);
		 }
		 DBTransaction.Commit();
		 // Add Cancel Security to the Archives -----------------------------------

		 DBTransaction.StartTransaction();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text =
			 "SELECT COUNT(ARCHIVE_KEY) FROM ARCHIVE WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCancel];
		 IBQuery2->ParamByName("ORDER_TYPE")->AsInteger = CreditOrder;
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " archive cancel records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT SERVER_NAME,TERMINAL_NAME, PRICE, PRICE_ADJUST,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM ARCHIVE INNER JOIN USERS ON USERS.NAME = ARCHIVE.SERVER_NAME " "WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCancel];
		 IBQuery->ParamByName("ORDER_TYPE")->AsInteger = CreditOrder;
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			Ver2_1_19_X_UpdateOrderTypeSecurity(DBTransaction, IBQuery, secCancel);
		 }
		 DBTransaction.Commit();
		 // Add Credits Security ------------------------------------------
		 // Add Credits Security ------------------------------------------
		 // Add Credits Security ------------------------------------------

		 DBTransaction.StartTransaction();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text =
			 "SELECT COUNT(ORDER_KEY) FROM ORDERS WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCredit];
		 IBQuery2->ParamByName("ORDER_TYPE")->AsInteger = CreditNonExistingOrder;
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " current credit records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT SERVER_NAME,TERMINAL_NAME, PRICE, PRICE_ADJUST,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM ORDERS INNER JOIN USERS ON USERS.NAME = ORDERS.SERVER_NAME " "WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCredit];
		 IBQuery->ParamByName("ORDER_TYPE")->AsInteger = CreditNonExistingOrder;
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			Ver2_1_19_X_UpdateOrderTypeSecurity(DBTransaction, IBQuery, secCredit);
		 }
		 DBTransaction.Commit();
		 // Add Credits Security to the Day Archives ------------------------------

		 DBTransaction.StartTransaction();
		 IBQuery2->Close();
		 IBQuery2->SQL->Text =
			 "SELECT COUNT(ARCHIVE_KEY) FROM DAYARCHIVE WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCredit];
		 IBQuery2->ParamByName("ORDER_TYPE")->AsInteger = CreditNonExistingOrder;
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " daily credit records";
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT SERVER_NAME,TERMINAL_NAME, PRICE, PRICE_ADJUST,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM DAYARCHIVE INNER JOIN USERS ON USERS.NAME = DAYARCHIVE.SERVER_NAME " "WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCredit];
		 IBQuery->ParamByName("ORDER_TYPE")->AsInteger = CreditNonExistingOrder;
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			Ver2_1_19_X_UpdateOrderTypeSecurity(DBTransaction, IBQuery, secCredit);
		 }
		 DBTransaction.Commit();

		 // Add Credits Security to the Archives ----------------------------------

		 DBTransaction.StartTransaction();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text =
			 "SELECT COUNT(ARCHIVE_KEY) FROM ARCHIVE WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCredit];
		 IBQuery2->ParamByName("ORDER_TYPE")->AsInteger = CreditNonExistingOrder;
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " archive credit records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT SERVER_NAME,TERMINAL_NAME, PRICE, PRICE_ADJUST,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM ARCHIVE INNER JOIN USERS ON USERS.NAME = ARCHIVE.SERVER_NAME " "WHERE ORDER_TYPE = :ORDER_TYPE AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCredit];
		 IBQuery->ParamByName("ORDER_TYPE")->AsInteger = CreditNonExistingOrder;
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			Ver2_1_19_X_UpdateOrderTypeSecurity(DBTransaction, IBQuery, secCredit);
		 }
		 DBTransaction.Commit();

		 // Add Price Adjusts Security ------------------------------------------
		 // Add Price Adjusts Security ------------------------------------------
		 // Add Price Adjusts Security ------------------------------------------

		 DBTransaction.StartTransaction();
		 IBQuery2->Close();
		 IBQuery2->SQL->Text =
			 "SELECT COUNT(ORDER_KEY) FROM ORDERS WHERE ORDER_TYPE = :ORDER_TYPE "
			 "AND PRICE_ADJUST != 0 AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secPriceAdjust];
		 IBQuery2->ParamByName("ORDER_TYPE")->AsInteger = NormalOrder;
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " current price adjusted records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT SERVER_NAME,TERMINAL_NAME, PRICE, PRICE_ADJUST,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM ORDERS INNER JOIN USERS ON USERS.NAME = ORDERS.SERVER_NAME " "WHERE ORDER_TYPE = :ORDER_TYPE " "AND PRICE_ADJUST != 0 AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secPriceAdjust];
		 IBQuery->ParamByName("ORDER_TYPE")->AsInteger = NormalOrder;
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			Ver2_1_19_X_UpdateOrderTypeSecurity(DBTransaction, IBQuery, secPriceAdjust);
		 }
		 DBTransaction.Commit();
		 // Add Price Adjusts Security to the Day Archives ------------------------------

		 DBTransaction.StartTransaction();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text =
			 "SELECT COUNT(ARCHIVE_KEY) FROM DAYARCHIVE WHERE ORDER_TYPE = :ORDER_TYPE "
			 "AND PRICE_ADJUST != 0 AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secPriceAdjust];
		 IBQuery2->ParamByName("ORDER_TYPE")->AsInteger = NormalOrder;
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " daily price adjusted records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT SERVER_NAME,TERMINAL_NAME, PRICE, PRICE_ADJUST,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM DAYARCHIVE INNER JOIN USERS ON USERS.NAME = DAYARCHIVE.SERVER_NAME " "WHERE ORDER_TYPE = :ORDER_TYPE " "AND PRICE_ADJUST != 0 AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secPriceAdjust];
		 IBQuery->ParamByName("ORDER_TYPE")->AsInteger = NormalOrder;
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			Ver2_1_19_X_UpdateOrderTypeSecurity(DBTransaction, IBQuery, secPriceAdjust);
		 }
		 DBTransaction.Commit();
		 // Add Price Adjusts Security to the Archives ----------------------------------

		 DBTransaction.StartTransaction();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text =
			 "SELECT COUNT(ARCHIVE_KEY) FROM ARCHIVE WHERE ORDER_TYPE = :ORDER_TYPE "
			 "AND PRICE_ADJUST != 0 AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery2->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secPriceAdjust];
		 IBQuery2->ParamByName("ORDER_TYPE")->AsInteger = NormalOrder;
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " archived price adjusted records";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text =
			 "SELECT SERVER_NAME,TERMINAL_NAME, PRICE, PRICE_ADJUST,TIME_STAMP,SECURITY_REF,USERS.USER_KEY " "FROM ARCHIVE INNER JOIN USERS ON USERS.NAME = ARCHIVE.SERVER_NAME " "WHERE ORDER_TYPE = :ORDER_TYPE " "AND PRICE_ADJUST != 0 AND SECURITY_REF NOT IN " "(SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = :SECURITY_EVENT );";
		 IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secPriceAdjust];
		 IBQuery->ParamByName("ORDER_TYPE")->AsInteger = NormalOrder;
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			Ver2_1_19_X_UpdateOrderTypeSecurity(DBTransaction, IBQuery, secPriceAdjust);
		 }
		 DBTransaction.Commit();
		 Log("Update " + UpdateVersion + " Successfull");
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::Ver2_1_19_X_UpdateOrderTypeSecurity(Database::TDBTransaction &DBTransaction, TIBSQL *IBQuery, TSecurityTypes secType)
{
   enum OrderStatus
   {
	  NormalOrder, DeleteOrder, CreditOrder, CreditNonExistingOrder
   };

   int CurrentCount = 0;
   unsigned int Time;
   Time = GetTickCount() + 1000;
   for (; !IBQuery->Eof; IBQuery->Next())
   {
	  if (Time < GetTickCount())
	  {
		 Application->ProcessMessages();
		 Time = GetTickCount() + 1000;
	  }
	  AnsiString UserName = IBQuery->FieldByName("SERVER_NAME")->AsString;
	  AnsiString TerminalName = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
	  Currency Price = IBQuery->FieldByName("PRICE")->AsFloat;
	  Currency PriceAdj = IBQuery->FieldByName("PRICE_ADJUST")->AsFloat;
	  TDateTime TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
	  int CurrentSecurityRef = IBQuery->FieldByName("SECURITY_REF")->AsInteger;
	  int UserKey = IBQuery->FieldByName("USER_KEY")->AsInteger;

	  if (UserKey != 0)
	  {
		 Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		 DBTransaction.RegisterQuery(IBQuery1);

		 if (CurrentSecurityRef == 0)
		 {
			IBQuery1->Close();
			IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_REF, 1) FROM RDB$DATABASE";
			IBQuery1->ExecQuery();
			CurrentSecurityRef = IBQuery1->Fields[0]->AsInteger;
		 }

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
			 "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, "
			 "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, "
			 ":SECURITY_EVENT, " ":FROM_VAL, " ":TO_VAL, " ":TERMINAL_NAME, " ":USER_KEY, " ":TIME_STAMP);";
		 IBQuery1->ParamByName("SECURITY_KEY")->AsInteger = CurrentSecurityKey;
		 IBQuery1->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
		 IBQuery1->ParamByName("USER_KEY")->AsInteger = UserKey;
		 IBQuery1->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secType];
		 if (secType == secCancel || secType == secPriceAdjust)
		 {
			IBQuery1->ParamByName("FROM_VAL")->AsString = CurrToStrF(Price, ffCurrency, 2);
			IBQuery1->ParamByName("TO_VAL")->AsString = CurrToStrF(Price + PriceAdj, ffCurrency, 2);
		 }
		 else if (secType == secCredit)
		 {
			IBQuery1->ParamByName("FROM_VAL")->AsString = CurrToStrF(-Price, ffCurrency, 2);
			IBQuery1->ParamByName("TO_VAL")->AsString = CurrToStrF(Price, ffCurrency, 2);
		 }

		 IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = TimeStamp;
		 IBQuery1->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
		 IBQuery1->ExecQuery();
	  }
	  CurrentCount++;
	  barProgress->Position = CurrentCount;
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::RemoveDemoMenuMateDB()
{
   try
   {
	  if (FileExists("C:\\MMDemoData.ib"))
	  {
		 DeleteFile("C:\\MMDemoData.ib");
		 Log("Removed demo database.");
	  }
	  if (FileExists("C:\\MMDemoData.ibk"))
	  {
		 DeleteFile("C:\\MMDemoData.ibk");
		 Log("Removed demo database backup.");
	  }
   }
   catch(Exception & E)
   {
	  Log("Unable to remove C:\\MMDemoData.ib. Please delete this file before entering demo mode on any terminal.");
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion2_3_00_X()
{
   bool RunAlterartions = false;
   AnsiString UpdateFromVersion = "2.2";
   AnsiString UpdateVersion = "2.3";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			RunAlterartions = true;
			// ZEDS --------------------------------------
			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE CATEGORIES (" " CATEGORY_KEY INTEGER NOT NULL PRIMARY KEY,"
				" CATEGORY VARCHAR(30) CHARACTER SET ISO8859_1 );";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE ARCCATEGORIES (" " CATEGORY_KEY INTEGER NOT NULL PRIMARY KEY,"
				" CATEGORY VARCHAR(30) CHARACTER SET ISO8859_1 );";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX CAT_CATEGORY_INDEX ON CATEGORIES CATEGORY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ARCCAT_CATEGORY_INDEX ON ARCCATEGORIES CATEGORY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE ITEMSIZECATEGORY (" " ITEMSIZE_KEY INTEGER, "
				" FOREIGN KEY (ITEMSIZE_KEY) REFERENCES ITEMSIZE (ITEMSIZE_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
				" CATEGORY_KEY INTEGER, "
				" FOREIGN KEY (CATEGORY_KEY) REFERENCES CATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE ON DELETE CASCADE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ITEMSIZECATEGORY_INDEX ON ITEMSIZECATEGORY ITEMSIZE_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE ORDERCATEGORY (" " ORDER_KEY INTEGER, "
				" FOREIGN KEY (ORDER_KEY) REFERENCES ORDERS (ORDER_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " CATEGORY_KEY INTEGER, "
				" FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE ON DELETE CASCADE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ORDERCATEGORY_INDEX ON ORDERCATEGORY ORDER_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE ORDERMIXCATEGORY (" " MIX_KEY INTEGER, "
				" FOREIGN KEY (MIX_KEY) REFERENCES ORDERMIX (MIX_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " CATEGORY_KEY INTEGER, "
				" FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE ON DELETE CASCADE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ORDERMIXCATEGORY_INDEX ON ORDERMIXCATEGORY MIX_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE DAYARCCATEGORY (" " ARCHIVE_KEY INTEGER, "
				" FOREIGN KEY (ARCHIVE_KEY) REFERENCES DAYARCHIVE (ARCHIVE_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
				" CATEGORY_KEY INTEGER, "
				" FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE ON DELETE CASCADE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX DAYARCCATEGORY_INDEX ON DAYARCCATEGORY ARCHIVE_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE REGISTRATION ("
				" MACHINEKEY VARCHAR(32) NOT NULL PRIMARY KEY," " TERMINAL_NAME VARCHAR(40),"
				" POS_LICENCES INTEGER, " " PALM_LICENCES INTEGER, " " PRODUCT VARCHAR(25),"
				" COMPANY VARCHAR(80)," " REGKEY VARCHAR(32));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE ARCCATEGORY (" " ARCHIVE_KEY INTEGER, "
				" FOREIGN KEY (ARCHIVE_KEY) REFERENCES ARCHIVE (ARCHIVE_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
				" CATEGORY_KEY INTEGER, " " FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE ON DELETE CASCADE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ARCCATEGORY_INDEX ON ARCCATEGORY ARCHIVE_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ORD_SECURITY_REF_INDEX ON ORDERS(SECURITY_REF);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ORD_MENU_INDEX ON ORDERS(MENU_NAME);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP INDEX CREDITINDEX;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP INDEX TRANSINDEX;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP INDEX ORDER_PRINTING_INDEX;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ORD_COURSE_INDEX ON ORDERS(COURSE_NAME);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ORD_ITEM_INDEX ON ORDERS(ITEM_NAME);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ORD_SIZE_INDEX ON ORDERS(SIZE_NAME);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ORD_TABLE_INDEX ON ORDERS(TABLE_NUMBER);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX DAYARC_SECURITY_REF_INDEX ON DAYARCHIVE(SECURITY_REF);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_CATEGORIES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_ARCCATEGORIES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE OPTIONS ADD FLAGS INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE OPTIONS ADD FORCED_MASK INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDEROPTION ADD PLUS T_TRUEFALSE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE STOCKCOSTS";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE STOCKCOSTS (" " STOCK_CODE VARCHAR(25) NOT NULL," " LOCATION VARCHAR(25) NOT NULL,"
				" UNIT CHAR(16)," " STOCK_DESC VARCHAR(50)," " COST NUMERIC(15,4));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX STOCKCOSTS_INDEX ON STOCKCOSTS(STOCK_CODE,LOCATION);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILLPAY ADD NOTE VARCHAR(50) CHARACTER SET ISO8859_1";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILLPAY ADD NOTE VARCHAR(50) CHARACTER SET ISO8859_1";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILLPAY ADD TAX_FREE T_TRUEFALSE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILLPAY ADD TAX_FREE T_TRUEFALSE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILLPAY ADD USER_KEY INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ARCBILLPAY ADD FOREIGN KEY (USER_KEY) REFERENCES USERS (USER_KEY) ON UPDATE CASCADE ON DELETE CASCADE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILLPAY ADD USER_KEY INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE DAYARCBILLPAY ADD FOREIGN KEY (USER_KEY) REFERENCES USERS (USER_KEY) ON UPDATE CASCADE ON DELETE CASCADE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ALTER ITEM_NAME TYPE VARCHAR(50) CHARACTER SET ISO8859_1";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_INVOICENUMBER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL ADD INVOICE_NUMBER INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL ADD INVOICE_NUMBER INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL ADD SALES_TYPE INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL ADD SALES_TYPE INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
	  DBTransaction.Commit();

	  if (RunAlterartions)
	  {

		 // Set all options with no options flag to 0x20.
		 DBTransaction.StartTransaction();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text = "SELECT COUNT(OPTIONS_KEY) FROM OPTIONS WHERE FLAGS IS NULL";
		 IBQuery2->ExecQuery();
		 int RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " Option flags";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text = "UPDATE OPTIONS" " SET FLAGS = :FLAGS" " WHERE FLAGS IS NULL";
		 IBQuery->ParamByName("FLAGS")->AsInteger = 0x20;
		 IBQuery->ExecQuery();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text = "SELECT COUNT(ARCBILL_KEY) FROM ARCBILL WHERE INVOICE_NUMBER IS NULL";
		 IBQuery2->ExecQuery();
		 RecordCount = IBQuery2->Fields[0]->AsInteger;

		 lbeProgress->Caption = "Processing " + IntToStr(RecordCount) + " Invoice Numbers";
		 Application->ProcessMessages();
		 barProgress->Position = 0;
		 barProgress->Min = 0;
		 barProgress->Max = RecordCount;

		 IBQuery->Close();
		 IBQuery->SQL->Text = "UPDATE ARCBILL" " SET INVOICE_NUMBER = ARCBILL_KEY" " WHERE INVOICE_NUMBER IS NULL";
		 IBQuery->ExecQuery();

		 IBQuery2->Close();
		 IBQuery2->SQL->Text = "SELECT MAX(INVOICE_NUMBER) FROM ARCBILL";
		 IBQuery2->ExecQuery();
		 int MaxInvoiceNumbner = IBQuery2->Fields[0]->AsInteger;

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SET GENERATOR GEN_INVOICENUMBER TO " + IntToStr(MaxInvoiceNumbner);
		 IBQuery->ExecQuery();

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM DAYARCBILL WHERE INVOICE_NUMBER IS NULL";
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			for (; !IBQuery->Eof; IBQuery->Next())
			{

			   int InvoiceNumber;
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 1) FROM RDB$DATABASE";
			   IBQuery1->ExecQuery();
			   InvoiceNumber = IBQuery1->Fields[0]->AsInteger;

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "UPDATE DAYARCBILL" " SET INVOICE_NUMBER = :INVOICE_NUMBER" " WHERE ARCBILL_KEY = :ARCBILL_KEY";
			   IBQuery1->ParamByName("ARCBILL_KEY")->AsInteger = IBQuery->FieldByName("ARCBILL_KEY")->AsInteger;
			   IBQuery1->ParamByName("INVOICE_NUMBER")->AsInteger = InvoiceNumber;
			   IBQuery1->ExecQuery();
			}
		 }

		 enum eTransactionType
		 {
			eTab, eTableSeat, eCash, eAccount, eRoom
		 };

		 IBQuery1->Close();
		 IBQuery1->SQL->Text = "UPDATE DAYARCBILL" " SET SALES_TYPE = :SALES_TYPE" " WHERE ARCBILL_KEY IN ( "
			 " SELECT DISTINCT(DAYARCHIVE.ARCBILL_KEY) FROM DAYARCHIVE"
			 " WHERE TAB_NAME != 'Tender Sale' AND TAB_NAME != 'Cash Sale' AND TABLE_NUMBER = 0 )";
		 IBQuery1->ParamByName("SALES_TYPE")->AsInteger = eTab;
		 IBQuery1->ExecQuery();

		 IBQuery1->Close();
		 IBQuery1->SQL->Text = "UPDATE DAYARCBILL" " SET SALES_TYPE = :SALES_TYPE" " WHERE ARCBILL_KEY IN ( "
			 " SELECT DISTINCT(DAYARCHIVE.ARCBILL_KEY) FROM DAYARCHIVE" " WHERE TABLE_NUMBER != 0 )";
		 IBQuery1->ParamByName("SALES_TYPE")->AsInteger = eTableSeat;
		 IBQuery1->ExecQuery();

		 IBQuery1->Close();
		 IBQuery1->SQL->Text = "UPDATE DAYARCBILL" " SET SALES_TYPE = :SALES_TYPE" " WHERE ARCBILL_KEY IN ( "
			 " SELECT DISTINCT(DAYARCHIVE.ARCBILL_KEY) FROM DAYARCHIVE" " WHERE TAB_NAME = 'Tender Sale' OR TAB_NAME = 'Cash Sale' )";
		 IBQuery1->ParamByName("SALES_TYPE")->AsInteger = eCash;
		 IBQuery1->ExecQuery();

		 IBQuery1->Close();
		 IBQuery1->SQL->Text = "UPDATE ARCBILL" " SET SALES_TYPE = :SALES_TYPE" " WHERE ARCBILL_KEY IN ( "
			 " SELECT DISTINCT(ARCHIVE.ARCBILL_KEY) FROM ARCHIVE" " WHERE TABLE_NUMBER != 0 )";
		 IBQuery1->ParamByName("SALES_TYPE")->AsInteger = eTableSeat;
		 IBQuery1->ExecQuery();

		 IBQuery1->Close();
		 IBQuery1->SQL->Text = "UPDATE ARCBILL" " SET SALES_TYPE = :SALES_TYPE" " WHERE ARCBILL_KEY IN ( "
			 " SELECT DISTINCT(ARCHIVE.ARCBILL_KEY) FROM ARCHIVE"
			 " WHERE TAB_NAME != 'Tender Sale' AND TAB_NAME != 'Cash Sale' AND TABLE_NUMBER = 0 )";
		 IBQuery1->ParamByName("SALES_TYPE")->AsInteger = eTab;
		 IBQuery1->ExecQuery();

		 IBQuery1->Close();
		 IBQuery1->SQL->Text = "UPDATE ARCBILL" " SET SALES_TYPE = :SALES_TYPE" " WHERE ARCBILL_KEY IN ( "
			 " SELECT DISTINCT(ARCHIVE.ARCBILL_KEY) FROM ARCHIVE" " WHERE TAB_NAME = 'Tender Sale' OR TAB_NAME = 'Cash Sale' )";
		 IBQuery1->ParamByName("SALES_TYPE")->AsInteger = eCash;
		 IBQuery1->ExecQuery();
		 DBTransaction.Commit();
		 Log("Update " + UpdateVersion + " Successfull");
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion2_4_00_X()
{
   bool RunAlterartions = false;
   AnsiString UpdateFromVersion = "2.3";
   AnsiString UpdateVersion = "2.4";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE RECIPE ALTER STOCK_LOCATION TYPE VARCHAR(25)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERRECIPE ALTER STOCK_LOCATION TYPE VARCHAR(25)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			Log("Update " + UpdateVersion + " Successfull");
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
	  DBTransaction.Commit();

	  if (RunAlterartions)
	  {
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion2_5_00_X()
{
   bool RunAlterartions = false;
   AnsiString UpdateFromVersion = "2.4";
   AnsiString UpdateVersion = "2.5";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			RunAlterartions = true;

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES ADD PERCENT_ADJUST NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES ADD AMOUNT_ADJUST NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES ADD ADJUST_REASON VARCHAR(30)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
	  DBTransaction.Commit();
	  int const ePayTypeCash = 0x1000;

	  if (RunAlterartions)
	  {
		 DBTransaction.StartTransaction();
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM PAYMENTTYPES WHERE UPPER(PAYMENT_NAME) = 'CASH'";
		 IBQuery->ExecQuery();
		 if (!IBQuery->RecordCount == 0)
		 {
			long Properties = IBQuery->FieldByName("PROPERTIES")->AsInteger;
			Properties |= ePayTypeCash;
			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE PAYMENTTYPES" " SET PROPERTIES = :PROPERTIES" " WHERE UPPER(PAYMENT_NAME) = 'CASH'";
			IBQuery1->ParamByName("PROPERTIES")->AsInteger = Properties;
			IBQuery1->ExecQuery();
		 }
		 DBTransaction.Commit();
		 Log("Update " + UpdateVersion + " Successfull");
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion2_6_00_X()
{
   bool RunAlterartions = false;
   AnsiString UpdateFromVersion = "2.5";
   AnsiString UpdateVersion = "2.6";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE ROOMS (" " ROOM_KEY INTEGER NOT NULL PRIMARY KEY," " TAB_KEY INTEGER,"
				" ROOM_NUMBER INTEGER," " ROOM_NAME VARCHAR(25)," " STATUS INTEGER," " COLOUR INTEGER)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_ROOMS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();
			Log("Update " + UpdateVersion + " Successfull");
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
	  DBTransaction.Commit();

	  if (RunAlterartions)
	  {
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion2_7_00_X()
{
   AnsiString UpdateFromVersion = "2.6";
   AnsiString UpdateVersion = "2.7";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			IBQuery1->Close();
			IBQuery1->SQL->Text = "SELECT rdb$constraint_name Name " "FROM rdb$relation_constraints "
				"WHERE rdb$relation_name = 'REGISTRATION' " "AND rdb$constraint_type = 'PRIMARY KEY'";
			IBQuery1->ExecQuery();
			AnsiString ConstraintName = IBQuery1->FieldByName("Name")->AsString;
			ConstraintName = ConstraintName.Trim();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE REGISTRATION DROP CONSTRAINT " + ConstraintName;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE REGISTRATION ADD MM_MODULES INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE REGISTRATION ADD OFFICE_MODULES INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE TURNAROUNDTIMES (" " TIME_KEY INTEGER NOT NULL PRIMARY KEY,"
				" MAKE_START_TIME TIMESTAMP," " MAKE_END_TIME TIMESTAMP," " SALE_START_TIME TIMESTAMP,"
				" SALE_END_TIME TIMESTAMP)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_TURN_AROUND_TIMES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD TIME_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD TIME_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD TIME_KEY INTEGER;";
			IBQuery1->ExecQuery();

			// ----------------------------- New GST Stuff ------------------------
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD COST_GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD COST_GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERMIX ADD GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERMIX ADD COST_GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD COST_GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD COST_GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE STOCKCOSTS ADD COST_GST_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			// ----------------------------- New TABS Stuff -----------------------

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE TAB ADD LOCKED_REASON VARCHAR(128);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE TAB ADD PIN VARCHAR(128);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE TAB ADD CREDIT_LIMIT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE TAB ADD TAB_PERMANENT T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			// Applying alterations here for speed.
			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ITEMSIZE SET GST_PERCENT = 12.5,COST_GST_PERCENT = 12.5";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ORDERS SET GST_PERCENT = 12.5,COST_GST_PERCENT = 12.5";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ORDERMIX SET GST_PERCENT = 12.5,COST_GST_PERCENT = 12.5";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE STOCKCOSTS SET COST_GST_PERCENT = 12.5";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCHIVE SET GST_PERCENT = 12.5,COST_GST_PERCENT = 12.5";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DAYARCHIVE SET GST_PERCENT = 12.5,COST_GST_PERCENT = 12.5";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE TAB SET CREDIT_LIMIT = -1";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_2_8()
{
   bool RunAlterartions = false;
   AnsiString UpdateFromVersion = "2.7";
   AnsiString UpdateVersion = "2.8";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			RunAlterartions = true;

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE STOCKTRACK ADD DEVICE_NAME VARCHAR(25);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVESTOCKTRACK ADD DEVICE_NAME VARCHAR(25);";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }

	  if (RunAlterartions)
	  {

	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_2_9()
{
   bool RunAlterartions = false;
   AnsiString UpdateFromVersion = "2.8";
   AnsiString UpdateVersion = "2.9";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			RunAlterartions = true;

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DEVICES ADD PRODUCT VARCHAR(25);";
			IBQuery1->ExecQuery();

			// REMOVE PALMS THEY WILL NEED TO BE RE-REGISTERED IN ORDER TO FILL IN THE PRODUCT.
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DELETE FROM DEVICES WHERE DEVICE_TYPE = 2;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE STOCKTRACK ADD PRODUCT VARCHAR(25);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP INDEX DEV_INDEX;";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }

	  if (RunAlterartions)
	  {

	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_0()
{
   AnsiString UpdateFromVersion = "2.9";
   AnsiString UpdateVersion = "3.0";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery3 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE INVOICES (" " INVOICE_KEY INTEGER NOT NULL PRIMARY KEY," " CONTACTS_KEY INTEGER NOT NULL,"
				" INVOICE_NUMBER INTEGER," " CREATION_DATE TIMESTAMP," " TOTAL_INC NUMERIC(15,4)," " TOTAL_EXCL NUMERIC(15,4),"
				" CLOSED T_TRUEFALSE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE ITEMSIDES (" " ITEMSIDES_KEY INTEGER NOT NULL PRIMARY KEY,"
				" MASTER_ITEM_KEY INTEGER NOT NULL,"
				" FOREIGN KEY (MASTER_ITEM_KEY) REFERENCES ITEM (ITEM_KEY) ON UPDATE CASCADE ON DELETE CASCADE, "
				" ITEM_KEY INTEGER NOT NULL," " FOREIGN KEY (ITEM_KEY) REFERENCES ITEM (ITEM_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
				" IOO INTEGER," " MAX_SELECT INTEGER," " GROUP_NO INTEGER);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_ITEMSIDES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE CONTACTROLLOVER ("
				" CONTACTROLLOVER_KEY INTEGER NOT NULL PRIMARY KEY,"
				" CONTACTS_KEY INTEGER NOT NULL," " TIME_STAMP TIMESTAMP NOT NULL);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_CONTACTROLLOVER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_INVOICES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE TAB ADD INVOICE_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE TAB ADD CARD VARCHAR(80);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE TAB ADD CONTACTS_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD SIDE_ORDER_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD QTY NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD QTY NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD QTY NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES ADD GROUP_NUMBER INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILLPAY ADD GROUP_NUMBER INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILLPAY ADD GROUP_NUMBER INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE COMPANIES ( " "COMPANY_KEY INTEGER NOT NULL, " "COMPANY_TYPE INTEGER, "
				"COMPANY_NAME VARCHAR(50) CHARACTER SET ISO8859_1 NOT NULL COLLATE EN_UK, "
				"PHONE VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK, " "FAX VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"MOBILE VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK, " "EMAIL VARCHAR(50) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"LOCATION_ADDRESS VARCHAR(250) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"MAILING_ADDRESS VARCHAR(250) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"BILLING_NAME VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK, " "BAD_DEBTOR T_TRUEFALSE, "
				"NOTE VARCHAR(250) CHARACTER SET ISO8859_1 COLLATE EN_UK, " "CONSTRAINT COMPANY_PK PRIMARY KEY (COMPANY_KEY));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE UNIQUE INDEX COMPANY_NAME_INDEX ON COMPANIES COMPANY_NAME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_COMPANIES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE CONTACTS (" "CONTACTS_KEY INTEGER NOT NULL, " "COMPANY_KEY INTEGER, "
				"CONTACT_ID INTEGER, " "CONTACT_TYPE INTEGER NOT NULL, "
				"NAME VARCHAR(50) CHARACTER SET ISO8859_1 NOT NULL COLLATE EN_UK, "
				"TITLE VARCHAR(10) CHARACTER SET ISO8859_1 COLLATE EN_UK," "SEX VARCHAR(10) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"FIRST_NAME VARCHAR(20) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"MIDDLE_NAME VARCHAR(20) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"LAST_NAME VARCHAR(20) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"INITIALS VARCHAR(5) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"LOCATION_ADDRESS VARCHAR(250) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"MAILING_ADDRESS VARCHAR(250) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"PHONE VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"PHONE_EXTENSION VARCHAR(8) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"MOBILE VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK, " "FAX VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"EMAIL VARCHAR(50) CHARACTER SET ISO8859_1 COLLATE EN_UK, " "TOTAL_SPENT NUMERIC(15, 4), "
				"PIN VARCHAR(6) CHARACTER SET ISO8859_1 NOT NULL COLLATE EN_UK, "
				"SWIPE_CARD VARCHAR(128) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"PROX_CARD VARCHAR(128) CHARACTER SET ISO8859_1 COLLATE EN_UK, " "ACCESS_LEVEL INTEGER, " "PALM_AUTO_OPTIONS T_TRUEFALSE, "
				"PALM_LARGE_FONT T_TRUEFALSE, " "PALM_ADVANCED_USER T_TRUEFALSE, " "PALM_TIMEOUT INTEGER, " "TAB_ENALBED T_TRUEFALSE, "
				"PAYROLL_ID VARCHAR(15), " "START_TIME1 TIMESTAMP, " "START_TIME2 TIMESTAMP, " "START_TIME3 TIMESTAMP, "
				"START_TIME4 TIMESTAMP, " "START_TIME5 TIMESTAMP, " "START_TIME6 TIMESTAMP, " "DATEOFBIRTH TIMESTAMP, "
				"NEXT_ORDER_NUMBER INTEGER, " "ORDER_PREFIX VARCHAR(10) CHARACTER SET ISO8859_1 COLLATE EN_UK, "
				"ORDER_SUFFIX VARCHAR(10) CHARACTER SET ISO8859_1 COLLATE EN_UK, " "CONSTRAINT CONTACTS_PK PRIMARY KEY (CONTACTS_KEY));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE UNIQUE INDEX CONTACT_INDEX ON CONTACTS (NAME,CONTACT_TYPE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_CONTACTS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE CONTACTTIME " "( " "CONTACTTIME_KEY INTEGER NOT NULL PRIMARY KEY, "
				"CONTACTS_KEY INTEGER NOT NULL, "
				"FOREIGN KEY (CONTACTS_KEY) REFERENCES CONTACTS (CONTACTS_KEY) ON UPDATE CASCADE ON DELETE CASCADE, "
				"LOGIN_DATETIME TIMESTAMP, " "LOGOUT_DATETIME TIMESTAMP, " "ZONE	INTEGER, " "MODIFIED T_TRUEFALSE, "
				"EXPORTED T_TRUEFALSE " ");";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_CONTACTTIME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE DISCOUNTS " "( " "DISCOUNT_KEY INTEGER NOT NULL PRIMARY KEY, " "NAME VARCHAR(15), "
				"DESCRIPTION VARCHAR(128), " "MENU VARCHAR(15), " "\"PERCENT\" NUMERIC(15, 4), " "AMOUNT NUMERIC(15, 4) " ");";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_DISCOUNTS;";
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->ParamCheck = false;

			IBQuery->SQL->Text = "Select * from ORDERS";
			IBQuery->ExecQuery();

			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   IBQuery1->Close();
			   IBQuery1->ParamCheck = true;
			   IBQuery1->SQL->Text = "Select * from ORDERMIX where ORDER_KEY = :ORDER_KEY";
			   IBQuery1->ParamByName("ORDER_KEY")->AsInteger = IBQuery->FieldByName("ORDER_KEY")->AsInteger;
			   IBQuery1->ExecQuery();

			   for (; !IBQuery1->Eof; IBQuery1->Next())
			   {
				  IBQuery2->Close();
				  IBQuery2->SQL->Text = "SELECT GEN_ID(GEN_ORDERS, 1) FROM RDB$DATABASE";
				  IBQuery2->ExecQuery();
				  int KeyVal = IBQuery2->Fields[0]->AsInteger;

				  IBQuery2->Close();
				  IBQuery2->SQL->Text =
					  "INSERT INTO ORDERS (" "ORDER_KEY," "SIDE_ORDER_KEY," "COURSE_NAME," "ITEM_NAME," "ITEM_ID," "SIZE_NAME,"
					  "PRICE," "COST," "PRICE_ADJUST," "SECURITY_REF," "GST_PERCENT," "TAB_KEY," "COST_GST_PERCENT) " "VALUES ("
					  ":ORDER_KEY," ":SIDE_ORDER_KEY," ":COURSE_NAME," ":ITEM_NAME," ":ITEM_ID," ":SIZE_NAME," ":PRICE," ":COST,"
					  ":PRICE_ADJUST," ":SECURITY_REF," ":GST_PERCENT," ":TAB_KEY," ":COST_GST_PERCENT);";
				  IBQuery2->ParamByName("ORDER_KEY")->AsInteger = KeyVal;
				  IBQuery2->ParamByName("SIDE_ORDER_KEY")->AsInteger = IBQuery1->FieldByName("ORDER_KEY")->AsInteger;
				  IBQuery2->ParamByName("COURSE_NAME")->AsString = IBQuery1->FieldByName("COURSE_NAME")->AsString;
				  IBQuery2->ParamByName("ITEM_NAME")->AsString = IBQuery1->FieldByName("ITEM_NAME")->AsString;
				  IBQuery2->ParamByName("ITEM_ID")->AsInteger = IBQuery1->FieldByName("ITEM_ID")->AsInteger;
				  IBQuery2->ParamByName("SIZE_NAME")->AsString = IBQuery1->FieldByName("SIZE_NAME")->AsString;
				  IBQuery2->ParamByName("PRICE")->AsFloat = IBQuery1->FieldByName("PRICE")->AsFloat;
				  IBQuery2->ParamByName("COST")->AsFloat = IBQuery1->FieldByName("COST")->AsFloat;
				  IBQuery2->ParamByName("PRICE_ADJUST")->AsFloat = IBQuery1->FieldByName("PRICE_ADJUST")->AsFloat;
				  IBQuery2->ParamByName("SECURITY_REF")->AsInteger = IBQuery1->FieldByName("SECURITY_REF")->AsInteger;
				  IBQuery2->ParamByName("TAB_KEY")->AsInteger = IBQuery->FieldByName("TAB_KEY")->AsInteger;
				  IBQuery2->ParamByName("GST_PERCENT")->AsFloat = IBQuery1->FieldByName("GST_PERCENT")->AsFloat;
				  IBQuery2->ParamByName("COST_GST_PERCENT")->AsFloat = IBQuery1->FieldByName("COST_GST_PERCENT")->AsFloat;
				  IBQuery2->ParamByName("COST_GST_PERCENT")->AsFloat = IBQuery1->FieldByName("COST_GST_PERCENT")->AsFloat;
				  IBQuery2->ExecQuery();

				  IBQuery3->Close();
				  IBQuery3->SQL->Text = "Select * from ORDERMIXCATEGORY where MIX_KEY = :MIX_KEY";
				  IBQuery3->ParamByName("MIX_KEY")->AsInteger = IBQuery1->FieldByName("MIX_KEY")->AsInteger;
				  IBQuery3->ExecQuery();

				  for (; !IBQuery3->Eof; IBQuery3->Next())
				  {
					 IBQuery2->Close();
					 IBQuery2->SQL->Text =
						 "INSERT INTO ORDERCATEGORY (" "ORDER_KEY," "CATEGORY_KEY) " "VALUES (" ":ORDER_KEY,"
						 ":CATEGORY_KEY);";
					 IBQuery2->ParamByName("ORDER_KEY")->AsInteger = KeyVal;
					 IBQuery2->ParamByName("CATEGORY_KEY")->AsInteger = IBQuery3->FieldByName("CATEGORY_KEY")->AsInteger;
					 IBQuery2->ExecQuery();
				  }
			   }
			}

			// ------------------------------------------------------------------------------
			DBTransaction.Commit();
			DB6Disconnect();
			DB1Connect();
			DBTransaction.StartTransaction();
			// ------------------------------------------------------------------------------

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE ORDERMIXCATEGORY";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE ORDERMIX";
			IBQuery1->ExecQuery();

			// remove all user table constraints.
			AnsiString PrimaryIndexName = "";

			IBQuery1->Close();
			IBQuery1->SQL->Text = "Select RDB$CONSTRAINT_NAME from rdb$relation_constraints "
				"where RDB$RELATION_NAME = 'USERS' and RDB$CONSTRAINT_TYPE = 'PRIMARY KEY';";
			IBQuery1->ExecQuery();
			PrimaryIndexName = IBQuery1->FieldByName("RDB$CONSTRAINT_NAME")->AsString;

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"select RDB$REF_CONSTRAINTS.RDB$CONSTRAINT_NAME,RDB$REF_CONSTRAINTS.RDB$CONST_NAME_UQ," "RDB$RELATION_CONSTRAINTS.RDB$RELATION_NAME "
				"from RDB$REF_CONSTRAINTS "
				"inner join RDB$RELATION_CONSTRAINTS on RDB$REF_CONSTRAINTS.RDB$CONSTRAINT_NAME = RDB$RELATION_CONSTRAINTS.RDB$CONSTRAINT_NAME " "where RDB$REF_CONSTRAINTS.RDB$CONST_NAME_UQ = :RDB$CONST_NAME_UQ";
			IBQuery1->ParamByName("RDB$CONST_NAME_UQ")->AsString = PrimaryIndexName;
			IBQuery1->ExecQuery();

			for (; !IBQuery1->Eof; IBQuery1->Next())
			{
			   IBQuery->Close();
			   IBQuery->SQL->Text = "ALTER TABLE " + IBQuery1->FieldByName("RDB$RELATION_NAME")->AsString.Trim()
				   + " DROP CONSTRAINT " + IBQuery1->FieldByName("RDB$CONSTRAINT_NAME")->AsString.Trim();
			   IBQuery->ExecQuery();
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			// Updater needs to remove old faulty logouts
			IBQuery->Close();
			IBQuery->SQL->Text =
				"select max(LOGIN_DATETIME),USER_KEY,LOGOUT_DATETIME FROM USERSTIME GROUP BY USER_KEY,LOGOUT_DATETIME HAVING LOGOUT_DATETIME IS NULL";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   for (; !IBQuery->Eof; IBQuery->Next())
			   {
				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "DELETE FROM USERSTIME " "WHERE USER_KEY = :USER_KEY AND "
					  "LOGOUT_DATETIME IS NULL AND LOGIN_DATETIME != :LOGIN_DATETIME";
				  IBQuery1->ParamByName("USER_KEY")->AsInteger = IBQuery->FieldByName("USER_KEY")->AsInteger;
				  IBQuery1->ParamByName("LOGIN_DATETIME")->AsDateTime = IBQuery->FieldByName("MAX")->AsDateTime;
				  IBQuery1->ExecQuery();
			   }
			}

			// Transfer Users->Contacts.
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM USERS";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   for (; !IBQuery->Eof; IBQuery->Next())
			   {
				  int UserType = IBQuery->FieldByName("USER_TYPE")->AsInteger;
				  // TranslateAccess Types
				  switch(UserType)
				  {
				  case 0:
					 UserType = 0;
					 break;
				  case 1:
					 UserType = 2;
					 break;
				  case 2:
					 UserType = 1;
					 break;
				  case 3:
					 UserType = 3;
					 break;
				  }

				  IBQuery1->Close();
				  IBQuery1->SQL->Text =
					  "INSERT INTO CONTACTS (" "CONTACTS_KEY," "CONTACT_TYPE," "NAME," "INITIALS," "PHONE," "MOBILE,"
					  "ACCESS_LEVEL," "PALM_AUTO_OPTIONS," "PALM_LARGE_FONT," "PALM_ADVANCED_USER," "PALM_TIMEOUT,"
					  "PAYROLL_ID," "START_TIME1," "START_TIME2," "START_TIME3," "START_TIME4," "START_TIME5," "START_TIME6," "PROX_CARD,"
					  "TOTAL_SPENT," "CONTACT_ID," "PIN) " "VALUES (" ":CONTACTS_KEY," ":CONTACT_TYPE," ":NAME," ":INITIALS," ":PHONE,"
					  ":MOBILE," ":ACCESS_LEVEL," ":PALM_AUTO_OPTIONS," ":PALM_LARGE_FONT," ":PALM_ADVANCED_USER," ":PALM_TIMEOUT,"
					  ":PAYROLL_ID," ":START_TIME1," ":START_TIME2," ":START_TIME3," ":START_TIME4," ":START_TIME5," ":START_TIME6,"
					  ":PROX_CARD," ":TOTAL_SPENT," ":CONTACT_ID," ":PIN);";
				  IBQuery1->ParamByName("CONTACTS_KEY")->AsInteger = IBQuery->FieldByName("USER_KEY")->AsInteger;
				  IBQuery1->ParamByName("CONTACT_TYPE")->AsInteger = UserType;
				  IBQuery1->ParamByName("NAME")->AsString = IBQuery->FieldByName("NAME")->AsString.SubString(1, 45);
				  IBQuery1->ParamByName("INITIALS")->AsString = IBQuery->FieldByName("INITIALS")->AsString;
				  IBQuery1->ParamByName("PHONE")->AsString = IBQuery->FieldByName("PHONE")->AsString.SubString(1, 25);
				  IBQuery1->ParamByName("MOBILE")->AsString = "";
				  IBQuery1->ParamByName("ACCESS_LEVEL")->AsInteger = IBQuery->FieldByName("ACCESS_LEVEL")->AsInteger;
				  IBQuery1->ParamByName("PALM_AUTO_OPTIONS")->AsString = IBQuery->FieldByName("AUTO_OPTIONS")->AsString;
				  IBQuery1->ParamByName("PALM_LARGE_FONT")->AsString = IBQuery->FieldByName("LARGE_FONT")->AsString;
				  IBQuery1->ParamByName("PALM_ADVANCED_USER")->AsString = IBQuery->FieldByName("ADVANCED_USER")->AsString;
				  IBQuery1->ParamByName("PALM_TIMEOUT")->AsInteger = IBQuery->FieldByName("TIMEOUT")->AsInteger;
				  IBQuery1->ParamByName("PAYROLL_ID")->AsString = IBQuery->FieldByName("PAYROLL_ID")->AsString;
				  IBQuery1->ParamByName("START_TIME1")->AsDateTime = IBQuery->FieldByName("START_TIME1")->AsDateTime;
				  IBQuery1->ParamByName("START_TIME2")->AsDateTime = IBQuery->FieldByName("START_TIME2")->AsDateTime;
				  IBQuery1->ParamByName("START_TIME3")->AsDateTime = IBQuery->FieldByName("START_TIME3")->AsDateTime;
				  IBQuery1->ParamByName("START_TIME4")->AsDateTime = IBQuery->FieldByName("START_TIME4")->AsDateTime;
				  IBQuery1->ParamByName("START_TIME5")->AsDateTime = IBQuery->FieldByName("START_TIME5")->AsDateTime;
				  IBQuery1->ParamByName("START_TIME6")->AsDateTime = IBQuery->FieldByName("START_TIME6")->AsDateTime;
				  IBQuery1->ParamByName("PROX_CARD")->AsString = "";
				  IBQuery1->ParamByName("TOTAL_SPENT")->AsFloat = IBQuery->FieldByName("TOTAL_SPENT")->AsFloat;
				  IBQuery1->ParamByName("CONTACT_ID")->AsInteger = IBQuery->FieldByName("USER_ID")->AsInteger;

				  if (IBQuery->FieldByName("PIN")->AsInteger == 0)
				  {
					 IBQuery1->ParamByName("PIN")->AsString = "";
				  }
				  else
				  {
					 IBQuery1->ParamByName("PIN")->AsString = IntToStr(IBQuery->FieldByName("PIN")->AsInteger);
				  }
				  IBQuery1->ExecQuery();

				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_CONTACTROLLOVER, 1) FROM RDB$DATABASE";
				  IBQuery1->ExecQuery();
				  int KeyVal = IBQuery1->Fields[0]->AsInteger;

				  IBQuery1->Close();
				  IBQuery1->SQL->Text =
					  "INSERT INTO CONTACTROLLOVER (" "CONTACTROLLOVER_KEY," "CONTACTS_KEY," "TIME_STAMP)" "VALUES ("
					  ":CONTACTROLLOVER_KEY," ":CONTACTS_KEY," ":TIME_STAMP);";
				  IBQuery1->ParamByName("CONTACTROLLOVER_KEY")->AsInteger = KeyVal;
				  IBQuery1->ParamByName("CONTACTS_KEY")->AsInteger = IBQuery->FieldByName("USER_KEY")->AsInteger;
				  IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = Now();
				  IBQuery1->ExecQuery();
			   }
			}

			// Transfer UsersTime->ContactsTime.
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM USERSTIME";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   for (; !IBQuery->Eof; IBQuery->Next())
			   {
				  IBQuery1->Close();
				  IBQuery1->SQL->Text =
					  "INSERT INTO CONTACTTIME (" "CONTACTTIME_KEY," "CONTACTS_KEY," "LOGIN_DATETIME," "LOGOUT_DATETIME,"
					  "ZONE," "MODIFIED," "EXPORTED) " "VALUES (" ":CONTACTTIME_KEY," ":CONTACTS_KEY," ":LOGIN_DATETIME,"
					  ":LOGOUT_DATETIME," ":ZONE," ":MODIFIED," ":EXPORTED);";
				  IBQuery1->ParamByName("CONTACTTIME_KEY")->AsInteger = IBQuery->FieldByName("USERTIME_KEY")->AsInteger;
				  IBQuery1->ParamByName("CONTACTS_KEY")->AsInteger = IBQuery->FieldByName("USER_KEY")->AsInteger;
				  IBQuery1->ParamByName("LOGIN_DATETIME")->AsDateTime = IBQuery->FieldByName("LOGIN_DATETIME")->AsDateTime;
				  IBQuery1->ParamByName("LOGOUT_DATETIME")->AsDateTime = IBQuery->FieldByName("LOGOUT_DATETIME")->AsDateTime;
				  IBQuery1->ParamByName("ZONE")->AsInteger = IBQuery->FieldByName("ZONE")->AsInteger;
				  if (IBQuery->FieldByName("MODIFIED")->AsString == "")
				  {
					 IBQuery1->ParamByName("MODIFIED")->AsString = "F";
				  }
				  else
				  {
					 IBQuery1->ParamByName("MODIFIED")->AsString = IBQuery->FieldByName("MODIFIED")->AsString;
				  }
				  if (IBQuery->FieldByName("EXPORTED")->AsString == "")
				  {
					 IBQuery1->ParamByName("EXPORTED")->AsString = "F";
				  }
				  else
				  {
					 IBQuery1->ParamByName("EXPORTED")->AsString = IBQuery->FieldByName("EXPORTED")->AsString;
				  }
				  IBQuery1->ExecQuery();
			   }
			}

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE SECURITY ADD CONSTRAINT SECURITY2CONTACTS FOREIGN KEY (USER_KEY) REFERENCES CONTACTS (CONTACTS_KEY) ON UPDATE CASCADE ON DELETE CASCADE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SELECT MAX(CONTACTS_KEY) FROM CONTACTS";
			IBQuery1->ExecQuery();
			int MaxKey = IBQuery1->FieldByName("MAX")->AsInteger;

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_CONTACTS TO " + IntToStr(MaxKey + 1);
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SELECT MAX(CONTACTTIME_KEY) FROM CONTACTTIME";
			IBQuery1->ExecQuery();
			MaxKey = IBQuery1->FieldByName("MAX")->AsInteger;

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_CONTACTTIME TO " + IntToStr(MaxKey + 1);
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DB6Disconnect();
			DB1Connect();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE USERSTIME";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE USERS";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILLPAY DROP USER_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILLPAY DROP USER_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILLPAY ADD PROPERTIES NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILLPAY ADD PAY_TYPE_DETAILS VARCHAR(50);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILLPAY ADD PROPERTIES NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILLPAY ADD PAY_TYPE_DETAILS VARCHAR(50);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE DAYARCSURCHARGE ( " " SURCHARGE_KEY INTEGER NOT NULL PRIMARY KEY , "
				" ARCBILL_KEY INTEGER NOT NULL, "
				" FOREIGN KEY (ARCBILL_KEY) REFERENCES DAYARCBILL (ARCBILL_KEY) ON UPDATE CASCADE ON DELETE CASCADE, " " PAY_TYPE VARCHAR(50), " " PAY_TYPE_DETAILS VARCHAR(50), " " SUBTOTAL NUMERIC(15,4), " " NOTE VARCHAR(50) CHARACTER SET ISO8859_1, " " TAX_FREE T_TRUEFALSE, " " PROPERTIES INTEGER, " " GROUP_NUMBER INTEGER ); ";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX DAYARCSURCHARGEFKINDEX ON DAYARCSURCHARGE ARCBILL_KEY";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_DAYARCSURCHARGE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE ARCSURCHARGE ( " " SURCHARGE_KEY INTEGER NOT NULL PRIMARY KEY , " " ARCBILL_KEY INTEGER, "
				" FOREIGN KEY (ARCBILL_KEY) REFERENCES ARCBILL (ARCBILL_KEY) ON UPDATE CASCADE ON DELETE CASCADE, "
				" PAY_TYPE VARCHAR(50), " " PAY_TYPE_DETAILS VARCHAR(50), " " SUBTOTAL NUMERIC(15,4), "
				" NOTE VARCHAR(50) CHARACTER SET ISO8859_1, " " TAX_FREE T_TRUEFALSE, " " PROPERTIES INTEGER, " " GROUP_NUMBER INTEGER ); ";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ARCSURCHARGEFKINDEX ON ARCSURCHARGE ARCBILL_KEY";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_ARCSURCHARGE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ROOMS ADD BOOKING_ID INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE LOCATIONS ADD LOCATION_TYPE INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL ADD BILLED_LOCATION VARCHAR(25);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL ADD BILLED_LOCATION VARCHAR(25);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE DROP LOCATION;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE DROP LOCATION;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS DROP LOCATION;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD DISCOUNT NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD DISCOUNT NUMERIC(15,4) DEFAULT 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD DISCOUNT NUMERIC(15,4) DEFAULT 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD REDEEMED NUMERIC(15,4) DEFAULT 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD REDEEMED NUMERIC(15,4) DEFAULT 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD REDEEMED NUMERIC(15,4) DEFAULT 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE REGISTRATION ADD OFFICE_LICENCES INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE REGISTRATION DROP OFFICE_MODULES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE COURSE ALTER ORDER_LOCATION TO VIEW_LOCATION;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONNECTIONS DROP ORDER_ACTIVATE_BUZZER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONNECTIONS DROP PC_ACTIVATE_CASHDRAW;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONNECTIONS DROP PALM_ACTIVATE_CASHDRAW;";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ZEDS" " SET INITIAL_FLOAT = 0"
				" WHERE ZEDS.SECURITY_REF NOT IN (SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_EVENT = 'Float Set');";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE TAB" " SET TAB_TYPE = 0," " TAB_PERMANENT = 'F'" " WHERE TAB_TYPE = 1 OR TAB_TYPE = 2";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE LOCATIONS" " SET LOCATION_TYPE = :LOCATION_TYPE;";
			IBQuery1->ParamByName("LOCATION_TYPE")->AsInteger = 2;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SELECT LOCATION_KEY FROM DEVICES WHERE DEVICE_NAME LIKE '%PalmMate%'";
			IBQuery1->ExecQuery();
			int LocKey = IBQuery1->FieldByName("LOCATION_KEY")->AsInteger;

			if (LocKey != 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "UPDATE DEVICES" " SET LOCATION_KEY = :LOCATION_KEY" " WHERE LOCATION_KEY IS NULL";
			   IBQuery1->ParamByName("LOCATION_KEY")->AsInteger = LocKey;
			   IBQuery1->ExecQuery();
			}

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DAYARCBILL" " SET PATRON_COUNT = 1" " WHERE PATRON_COUNT = 0";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCBILL" " SET PATRON_COUNT = 1" " WHERE PATRON_COUNT = 0";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ORDERS SET QTY = 1,REDEEMED = 0,DISCOUNT = 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DAYARCHIVE SET QTY = 1,REDEEMED = 0,DISCOUNT = 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCHIVE SET QTY = 1,REDEEMED = 0,DISCOUNT = 0;";
			IBQuery1->ExecQuery();

			/* IBQuery1->Close();
			IBQuery1->SQL->Text =
			"UPDATE DAYARCBILLPAY"
			" SET TAX_FREE = 'F'"
			" WHERE TAX_FREE IS NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
			"UPDATE ARCBILLPAY"
			" SET TAX_FREE = 'F'"
			" WHERE TAX_FREE IS NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
			"UPDATE DAYARCBILLPAY"
			" SET CASH_OUT = 'F'"
			" WHERE CASH_OUT IS NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
			"UPDATE ARCBILLPAY"
			" SET CASH_OUT = 'F'"
			" WHERE CASH_OUT IS NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
			"UPDATE ITEM"
			" SET FREE = 'F'"
			" WHERE FREE IS NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
			"UPDATE CONNECTIONS"
			" SET DISABLED = 'F'"
			" WHERE DISABLED IS NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
			"UPDATE TAB"
			" SET TAB_PERMANENT = 'F'"
			" WHERE TAB_PERMANENT IS NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
			"UPDATE TABLES"
			" SET CIRCLE = 'F'"
			" WHERE CIRCLE IS NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
			"UPDATE TABLES"
			" SET TEMPORARY = 'F'"
			" WHERE TEMPORARY IS NULL";
			IBQuery1->ExecQuery(); */

			IBQuery1->Close();
			IBQuery1->ParamCheck = false;
			IBQuery1->SQL->Text =
				"Create Procedure UpdateBillPayProperties As "
				"Declare Variable vProperty NUMERIC(15, 4); " "Declare Variable vName VARCHAR(30); " "Begin " "For " "Select "
				"PAYMENTTYPES.PROPERTIES, " "PAYMENTTYPES.PAYMENT_NAME " "From " "PAYMENTTYPES " "Into "
				":vProperty,:vName " "Do " "Begin " "Update " "ARCBILLPAY " "Set " "ARCBILLPAY.PROPERTIES = :vProperty " "Where "
				"ARCBILLPAY.PAY_TYPE = :vName; " "Update " "DAYARCBILLPAY " "Set " "PROPERTIES = :vProperty " "Where "
				"DAYARCBILLPAY.PAY_TYPE = :vName; " "End " "End ";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->ParamCheck = false;
			IBQuery1->SQL->Text = "Execute Procedure UpdateBillPayProperties";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "Drop Procedure UpdateBillPayProperties";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_1()
{
   AnsiString UpdateFromVersion = "3.0";
   AnsiString UpdateVersion = "3.1";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DEVICES ADD IP VARCHAR(20);";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_2()
{
   AnsiString UpdateFromVersion = "3.1";
   AnsiString UpdateVersion = "3.2";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE RUNPROGRAMS ( "
				" RUNPROGRAMS_KEY INTEGER NOT NULL PRIMARY KEY , " " APPEARANCE_ORDER INTEGER, "
				" NAME VARCHAR(15)," " RUN_LINE VARCHAR(255) ); ";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_RUNPROGRAMS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_ORDERRECIPE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD POINTS_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEM ADD EXCLUSIVELY_AS_SIDE T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEM ADD ITEM_KITCHEN_NAME VARCHAR(200);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE SIZES ADD SIZE_KITCHEN_NAME VARCHAR(200);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD SIZE_KITCHEN_NAME VARCHAR(200);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE COURSE ADD COURSE_KITCHEN_NAME VARCHAR(200);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE OPTIONS ADD OPTION_KITCHEN_NAME VARCHAR(200);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDEROPTION ADD OPTION_KITCHEN_NAME VARCHAR(200);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD ITEM_KITCHEN_NAME VARCHAR(200);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD SIZE_KITCHEN_NAME VARCHAR(200);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD COURSE_KITCHEN_NAME VARCHAR(200);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEM DROP ITEM_SHORT_NAME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDEROPTION DROP OPTION_SHORT_NAME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE OPTIONS DROP OPTION_SHORT_NAME;";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_21()
{
   AnsiString UpdateFromVersion = "3.2";
   AnsiString UpdateVersion = "3.21";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE INVOICES ALTER COLUMN INVOICE_NUMBER TYPE VARCHAR(50);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL ALTER COLUMN INVOICE_NUMBER TYPE VARCHAR(50);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL ALTER COLUMN INVOICE_NUMBER TYPE VARCHAR(50);";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE INVOICES ADD INVOICE_NUM VARCHAR(50);";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE INVOICES SET INVOICE_NUM = INVOICE_NUMBER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE INVOICES DROP INVOICE_NUMBER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE INVOICES ALTER COLUMN INVOICE_NUM TO INVOICE_NUMBER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL ADD INVOICE_NUM VARCHAR(50);";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCBILL SET INVOICE_NUM = INVOICE_NUMBER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL DROP INVOICE_NUMBER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL ALTER COLUMN INVOICE_NUM TO INVOICE_NUMBER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL ADD INVOICE_NUM VARCHAR(50);";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DAYARCBILL SET INVOICE_NUM = INVOICE_NUMBER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL DROP INVOICE_NUMBER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL ALTER COLUMN INVOICE_NUM TO INVOICE_NUMBER;";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");

			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_22()
{
   AnsiString UpdateFromVersion = "3.21";
   AnsiString UpdateVersion = "3.22";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD POINTS_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD POINTS_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD POINTS_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD POINTS_EARNED NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD POINTS_EARNED NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD LOYALTY_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD LOYALTY_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP INDEX INVERSEKEYINDEX;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE TABHISTORY ( "
            " TABHISTORY_KEY INTEGER NOT NULL PRIMARY KEY, "
				" TAB_KEY INTEGER NOT NULL, "
            " CONSTRAINT TABHISTORY_TAB_KEY FOREIGN KEY (TAB_KEY) REFERENCES TAB (TAB_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
            " INVOICE_NUMBER VARCHAR(50)); ";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE DESCENDING INDEX INVERSEKEYINDEX ON TAB(TAB_KEY);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_TABHISTORY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_TABHISTORY TO 0;";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");

			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_23()
{
   AnsiString UpdateFromVersion = "3.22";
   AnsiString UpdateVersion = "3.23";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ALTER SIZE_NAME TYPE VARCHAR(30);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ALTER MASTER_CONTAINER TYPE VARCHAR(30);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ALTER SIZE_NAME TYPE VARCHAR(30);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ALTER SIZE_NAME TYPE VARCHAR(30);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ALTER ITEM_NAME TYPE VARCHAR(50);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE INVOICES ADD CLOSED_TIME_STAMP TIMESTAMP;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD CONTACTS_3RDPARTY_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "ARCBILL_INVOICE_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX ARCBILL_INVOICE_INDEX ON ARCBILL(INVOICE_NUMBER);";
			   IBQuery1->ExecQuery();
			}

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "DAYARCBILL_INVOICE_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX DAYARCBILL_INVOICE_INDEX ON DAYARCBILL(INVOICE_NUMBER);";
			   IBQuery1->ExecQuery();
			}

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "ARCBILL_DESC_KEY_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE DESCENDING INDEX ARCBILL_DESC_KEY_INDEX ON ARCBILL(ARCBILL_KEY);";
			   IBQuery1->ExecQuery();
			}

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "DAYARCBILL_DESC_KEY_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE DESCENDING INDEX DAYARCBILL_DESC_KEY_INDEX ON DAYARCBILL(ARCBILL_KEY);";
			   IBQuery1->ExecQuery();
			}

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "INVOICES_INVOICE_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX INVOICES_INVOICE_INDEX ON INVOICES(INVOICE_NUMBER)";
			   IBQuery1->ExecQuery();
			}

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "TABHISTORY_INVOICE_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX TABHISTORY_INVOICE_INDEX ON TABHISTORY(INVOICE_NUMBER)";
			   IBQuery1->ExecQuery();
			}

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_UPDATE_STOCK_ON_ZED;";
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT " " SECURITY.SECURITY_REF," " SECURITY.SECURITY_EVENT," " SECURITY.TERMINAL_NAME,"
				" SECURITY.USER_KEY," " SECURITY.TIME_STAMP," " CONTACTS.NAME," " CONTACTS.INITIALS" " FROM SECURITY "
				" LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY" " WHERE SECURITY_EVENT = :SECURITY_EVENT";
			IBQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secCredit];
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   int Time;
			   Time = GetTickCount() + 1000;
			   for (; !IBQuery->Eof; IBQuery->Next())
			   {
				  if (Time < GetTickCount())
				  {
					 Application->ProcessMessages();
					 Time = GetTickCount() + 1000;
				  }

				  IBQuery1->Close();
				  IBQuery1->SQL->Text =
					  "SELECT SECURITY_KEY "
					  "FROM SECURITY " "WHERE SECURITY_EVENT = :SECURITY_EVENT AND " "SECURITY_REF = :SECURITY_REF";
				  IBQuery1->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secOrderedBy];
				  IBQuery1->ParamByName("SECURITY_REF")->AsString = IBQuery->FieldByName("SECURITY_REF")->AsInteger;
				  IBQuery1->ExecQuery();

				  if (IBQuery1->RecordCount == 0)
				  {
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
						 "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, "
						 "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, " ":SECURITY_EVENT, "
						 ":FROM_VAL, " ":TO_VAL, " ":TERMINAL_NAME, " ":USER_KEY, " ":TIME_STAMP);";
					 IBQuery1->ParamByName("SECURITY_KEY")->AsInteger = CurrentSecurityKey;
					 IBQuery1->ParamByName("SECURITY_REF")->AsInteger = IBQuery->FieldByName("SECURITY_REF")->AsInteger;
					 IBQuery1->ParamByName("USER_KEY")->AsInteger = IBQuery->FieldByName("USER_KEY")->AsInteger;
					 IBQuery1->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secOrderedBy];
					 IBQuery1->ParamByName("FROM_VAL")->AsString = IBQuery->FieldByName("NAME")->AsString;
					 IBQuery1->ParamByName("TO_VAL")->AsString = "Created by Auto-updater";
					 IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
					 IBQuery1->ParamByName("TERMINAL_NAME")->AsString = IBQuery->FieldByName("TERMINAL_NAME")->AsString;
					 IBQuery1->ExecQuery();
				  }
			   }
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE INVOICES SET CLOSED_TIME_STAMP = :TIME_STAMP WHERE CLOSED = 'T';";
			IBQuery1->ParamByName("TIME_STAMP")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");

			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_231()
{
   AnsiString UpdateFromVersion = "3.23";
   AnsiString UpdateVersion = "3.231";
   try
   {
	  DB6Disconnect(); // Required so initail update of CONTACTS_3RDPARTY_KEY works.
	  DB1Connect();

	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTS SET CONTACTS_3RDPARTY_KEY = 0 WHERE CONTACTS_3RDPARTY_KEY IS NULL";
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->SQL->Text =
				"SELECT CONTACTS.CONTACTS_KEY,TAB_KEY FROM CONTACTS " "LEFT JOIN TAB ON TAB.CONTACTS_KEY = CONTACTS.CONTACTS_KEY " "WHERE CONTACTS.NAME = ''";
			IBQuery->ExecQuery();
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = " SELECT ORDER_KEY" " FROM " "  ORDERS " " WHERE " "  TAB_KEY = :TAB_KEY";
			   IBQuery1->ParamByName("TAB_KEY")->AsInteger = IBQuery->FieldByName("TAB_KEY")->AsInteger;
			   IBQuery1->ExecQuery();
			   if (IBQuery1->RecordCount == 0)
			   {
				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "DELETE FROM CONTACTS " "WHERE CONTACTS.CONTACTS_KEY = :CONTACTS_KEY";
				  IBQuery1->ParamByName("CONTACTS_KEY")->AsInteger = IBQuery->FieldByName("CONTACTS_KEY")->AsInteger;
				  IBQuery1->ExecQuery();

				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "DELETE FROM TAB " "WHERE TAB_KEY = :TAB_KEY";
				  IBQuery1->ParamByName("TAB_KEY")->AsInteger = IBQuery->FieldByName("TAB_KEY")->AsInteger;
				  IBQuery1->ExecQuery();
			   }
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS " "WHERE CONTACTS.NAME = ''";
			IBQuery->ExecQuery();

			if (!IBQuery->RecordCount == 0)
			{
			   Log("Unable to proceed with update there are contacts with no name that have orders. Contact Posabl or ERS to proceed.");
			}
			else
			{

			   IBQuery->Close();
			   IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			   IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "CONTACT_INDEX";
			   IBQuery->ExecQuery();
			   if (!IBQuery->RecordCount == 0)
			   {
				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "DROP INDEX CONTACT_INDEX;";
				  IBQuery1->ExecQuery();
			   }

			   DBTransaction.Commit();
			   DB6Disconnect();
			   DB1Connect();
			   DBTransaction.StartTransaction();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE UNIQUE INDEX CONTACT_INDEX ON CONTACTS(NAME,CONTACT_TYPE,CONTACTS_3RDPARTY_KEY)";
			   IBQuery1->ExecQuery();

			   // Insert this version before 3.24 if it exists.
			   TDateTime TimeStamp = Now();
			   IBQuery->Close();

			   IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			   IBQuery->ParamByName("VERSION_NUMBER")->AsString = "3.24";
			   IBQuery->ExecQuery();
			   if (!IBQuery->RecordCount == 0)
			   {
				  TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
				  TimeStamp = IncSecond(TimeStamp, -2);
			   }
			   else
			   {
				  Sleep(1000);
			   }

			   IBQuery1->Close();
			   IBQuery1->ParamCheck = true;
			   IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				   " VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			   IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			   IBQuery1->ParamByName("DateTime")->AsDateTime = TimeStamp;
			   IBQuery1->ExecQuery();

			   DBTransaction.Commit();
			   Log("Update " + UpdateVersion + " Successfull");
			}
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_232()
{
   AnsiString UpdateFromVersion = "3.231";
   AnsiString UpdateVersion = "3.232";
   try
   {
	  DB6Disconnect(); // Required so initail update of CONTACTS_3RDPARTY_KEY works.
	  DB1Connect();

	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "ARCBILL_TIMESTAMP_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX ARCBILL_TIMESTAMP_INDEX ON ARCBILL(TIME_STAMP)";
			   IBQuery1->ExecQuery();
			}

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "DAYARCBILL_TIMESTAMP_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX DAYARCBILL_TIMESTAMP_INDEX ON ARCBILL(TIME_STAMP)";
			   IBQuery1->ExecQuery();
			}

			// Insert this version before 3.24 if it exists.
			TDateTime TimeStamp = Now();
			IBQuery->Close();

			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = "3.24";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   TimeStamp = IBQuery->FieldByName("TIME_STAMP")->AsDateTime;
			   TimeStamp = IncSecond(TimeStamp, -1);
			}
			else
			{
			   Sleep(1000);
			}

			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = TimeStamp;
			IBQuery1->ExecQuery();

			DBTransaction.Commit();

			Log("Update " + UpdateVersion + " Successfull");
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_24()
{
   AnsiString UpdateFromVersion = "3.231";
   AnsiString UpdateVersion = "3.24";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE THIRDPARTYCODES ( "
				" THIRDPARTYCODES_KEY INTEGER NOT NULL PRIMARY KEY, " " CODE VARCHAR(25) NOT NULL, "
				" CODETYPE INTEGER, " " VISIBLE T_TRUEFALSE, " " DESCRIPTION VARCHAR(25)); ";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD THIRDPARTYCODES_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD THIRDPARTYCODES_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD THIRDPARTYCODES_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD THIRDPARTYCODES_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_THIRDPARTYCODES;";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_3()
{
   AnsiString UpdateFromVersion = "3.22";
   AnsiString UpdateVersion = "3.3";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			DBTransaction.Commit();
			DB6Disconnect();
			DB1Connect();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SELECT rdb$constraint_name Name " "FROM rdb$relation_constraints "
				"WHERE rdb$relation_name = 'ITEMSIZECATEGORY' " "AND rdb$constraint_type = 'FOREIGN KEY'";
			IBQuery1->ExecQuery();
			for (; !IBQuery1->Eof; IBQuery1->Next())
			{
			   AnsiString ConstraintName = IBQuery1->FieldByName("Name")->AsString;
			   ConstraintName = ConstraintName.Trim();

			   IBQuery->Close();
			   IBQuery->SQL->Text = "ALTER TABLE ITEMSIZECATEGORY DROP CONSTRAINT " + ConstraintName;
			   IBQuery->ExecQuery();
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			// Multiple Cards per user

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE CONTACTCARDS ( " " CONTACTCARDS_KEY INTEGER NOT NULL PRIMARY KEY , "
				" CONTACTS_KEY INTEGER NOT NULL, " " CONSTRAINT CONTACTCARDS_CONTACTS_KEY FOREIGN KEY (CONTACTS_KEY) REFERENCES CONTACTS (CONTACTS_KEY) ON UPDATE CASCADE ON DELETE CASCADE, " " SWIPE_CARD VARCHAR(128) ); ";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_CONTACTCARDS;";
			IBQuery1->ExecQuery();

			// ------------------------------------------------------------------------------
			// Free Drinks.

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE CONTACTFREEBIE ( " " CONTACTFREEBIE_KEY INTEGER NOT NULL PRIMARY KEY , "
				" CONTACTS_KEY INTEGER NOT NULL, " " CONSTRAINT CONTACTFREEBIE_CONTACTS_KEY FOREIGN KEY (CONTACTS_KEY) REFERENCES CONTACTS (CONTACTS_KEY) ON UPDATE CASCADE ON DELETE CASCADE, " " ITEM_NAME	VARCHAR(50) NOT NULL, " " SIZE_NAME VARCHAR(30) NOT NULL, " " ITEM_COUNT INTEGER NOT NULL); ";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_CONTACTFREEBIE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE CATEGORYGROUPS (" " CATEGORYGROUPS_KEY INTEGER NOT NULL PRIMARY KEY,"
				" NAME VARCHAR(25) CHARACTER SET ISO8859_1," " VISIBLE T_TRUEFALSE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_CATEGORYGROUPS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_CATEGORYGROUPS, 1) FROM RDB$DATABASE";
			IBQuery1->ExecQuery();
			int CatGroupKey = IBQuery1->Fields[0]->AsInteger;

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD MEM_DISCOUNT_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD LOC_DISCOUNT_PERCENT NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD MEM_SALE_COUNT INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD LOC_SALE_COUNT INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIDES ADD ALLOW_SKIP T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE MENU ADD PALMABLE T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			// ------------------------------------------------------------------------------
			// Categories.

			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO CATEGORYGROUPS (" " CATEGORYGROUPS_KEY," " NAME," " VISIBLE)"
				" VALUES ( :CATEGORYGROUPS_KEY,'MM_ALL','F');";
			IBQuery1->ParamByName("CATEGORYGROUPS_KEY")->AsInteger = CatGroupKey;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCCATEGORIES ADD CATEGORYGROUPS_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCCATEGORIES ADD COO INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCCATEGORIES ADD VISIBLE T_TRUEFALSE";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT DISTINCT " "CATEGORIES.CATEGORY_KEY CAT_CATEGORY_KEY, " "ARCCATEGORIES.CATEGORY_KEY ARC_CAT_KEY, "
				"ARCCATEGORIES.CATEGORY ARC_CAT, " "CATEGORIES.CATEGORY CAT_CATEGORY " "FROM "
				"ITEMSIZECATEGORY LEFT JOIN CATEGORIES ON ITEMSIZECATEGORY.CATEGORY_KEY = CATEGORIES.CATEGORY_KEY "
				"LEFT JOIN ARCCATEGORIES ON CATEGORIES.CATEGORY = ARCCATEGORIES.CATEGORY";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   for (; !IBQuery->Eof; IBQuery->Next())
			   {
				  if (IBQuery->FieldByName("ARC_CAT_KEY")->AsInteger == 0)
				  { // Matching Category is not in the Archives Add it.
					 IBQuery1->Close();
					 IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_ARCCATEGORIES, 1) FROM RDB$DATABASE";
					 IBQuery1->ExecQuery();
					 int key = IBQuery1->Fields[0]->AsInteger;

					 IBQuery1->Close();
					 IBQuery1->SQL->Text =
						 "INSERT INTO ARCCATEGORIES (" "CATEGORY_KEY," "CATEGORY," "VISIBLE) " "VALUES ("
						 ":CATEGORY_KEY," ":CATEGORY," "'T');";
					 IBQuery1->ParamByName("CATEGORY_KEY")->AsInteger = key;
					 IBQuery1->ParamByName("CATEGORY")->AsString = IBQuery->FieldByName("CAT_CATEGORY")->AsString;
					 IBQuery1->ExecQuery();

					 IBQuery1->Close();
					 IBQuery1->SQL->Text =
						 "UPDATE ITEMSIZECATEGORY"
						 " SET CATEGORY_KEY = :CATEGORY_KEY" " WHERE CATEGORY_KEY = :OLDCATEGORY_KEY";
					 IBQuery1->ParamByName("CATEGORY_KEY")->AsInteger = key;
					 IBQuery1->ParamByName("OLDCATEGORY_KEY")->AsInteger = IBQuery->FieldByName("CAT_CATEGORY_KEY")->AsInteger;
					 IBQuery1->ExecQuery();
				  }
				  else
				  {
					 IBQuery1->Close();
					 IBQuery1->SQL->Text =
						 "UPDATE ITEMSIZECATEGORY"
						 " SET CATEGORY_KEY = :CATEGORY_KEY" " WHERE CATEGORY_KEY = :OLDCATEGORY_KEY";
					 IBQuery1->ParamByName("CATEGORY_KEY")->AsInteger = IBQuery->FieldByName("ARC_CAT_KEY")->AsInteger;
					 IBQuery1->ParamByName("OLDCATEGORY_KEY")->AsInteger = IBQuery->FieldByName("CAT_CATEGORY_KEY")->AsInteger;
					 IBQuery1->ExecQuery();
				  }
			   }
			}

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_ARCCATEGORIES, 1) FROM RDB$DATABASE";
			IBQuery1->ExecQuery();
			int PriArcCatKey = IBQuery1->Fields[0]->AsInteger;

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCCATEGORIES" " SET VISIBLE = 'T'" " WHERE VISIBLE IS NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "INSERT INTO ARCCATEGORIES (" " CATEGORY_KEY," " CATEGORY," " CATEGORYGROUPS_KEY," " COO," " VISIBLE)"
				" VALUES ( :CATEGORY_KEY,'MM_TOTAL',:CATEGORYGROUPS_KEY,1,'F');";
			IBQuery1->ParamByName("CATEGORY_KEY")->AsInteger = PriArcCatKey;
			IBQuery1->ParamByName("CATEGORYGROUPS_KEY")->AsInteger = CatGroupKey;
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DB6Disconnect();
			DB1Connect();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE CATEGORIES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD CATEGORY_KEY INTEGER NOT NULL;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD CATEGORY_KEY INTEGER NOT NULL;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD CATEGORY_KEY INTEGER NOT NULL;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD CATEGORY_KEY INTEGER NOT NULL;";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCCATEGORIES" " SET CATEGORYGROUPS_KEY = :CATEGORYGROUPS_KEY";
			IBQuery1->ParamByName("CATEGORYGROUPS_KEY")->AsInteger = CatGroupKey;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCHIVE" " SET CATEGORY_KEY = :CATEGORY_KEY";
			IBQuery1->ParamByName("CATEGORY_KEY")->AsInteger = PriArcCatKey;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DAYARCHIVE" " SET CATEGORY_KEY = :CATEGORY_KEY";
			IBQuery1->ParamByName("CATEGORY_KEY")->AsInteger = PriArcCatKey;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ORDERS" " SET CATEGORY_KEY = :CATEGORY_KEY";
			IBQuery1->ParamByName("CATEGORY_KEY")->AsInteger = PriArcCatKey;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ITEMSIZE" " SET CATEGORY_KEY = :CATEGORY_KEY";
			IBQuery1->ParamByName("CATEGORY_KEY")->AsInteger = PriArcCatKey;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE SECURITY" " SET SECURITY_EVENT = :SECURITY_EVENT " " WHERE SECURITY_EVENT = :OLDSECURITY_EVENT";
			IBQuery1->ParamByName("SECURITY_EVENT")->AsString = "Transferred";
			IBQuery1->ParamByName("OLDSECURITY_EVENT")->AsString = "Transfered";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ITEMSIZECATEGORY ADD CONSTRAINT ITEMSIZECATEGORY_CATEGORY_KEY FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ITEMSIZECATEGORY ADD CONSTRAINT ITEMSIZECATEGORY_ITEMSIZE_KEY FOREIGN KEY (ITEMSIZE_KEY) REFERENCES ITEMSIZE (ITEMSIZE_KEY) ON UPDATE CASCADE ON DELETE CASCADE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ARCCATEGORIES ADD CONSTRAINT ARCCAT_CATEGORYGROUPS_KEY FOREIGN KEY (CATEGORYGROUPS_KEY) REFERENCES CATEGORYGROUPS (CATEGORYGROUPS_KEY) ON UPDATE CASCADE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ARCHIVE ADD CONSTRAINT ARCHIVE_CATEGORY_KEY FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE DAYARCHIVE ADD CONSTRAINT DAYARCHIVE_CATEGORY_KEY FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ORDERS ADD CONSTRAINT ORDERS_CATEGORY_KEY FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE ON DELETE CASCADE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ITEMSIZE ADD CONSTRAINT ITEMSIZE_CATEGORY_KEY FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DEVICES ADD PROFILE_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONNECTIONS ADD PROFILE_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE LOCATIONS ADD PROFILE_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE REGISTRATION ADD EXPIRY TIMESTAMP;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILL ADD INVOICE_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILL ADD INVOICE_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERRECIPE ALTER UNIT TYPE CHAR(16);";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE LOCATIONSFREEBIE ( " " LOCATIONSFREEBIE_KEY 			INTEGER NOT NULL PRIMARY KEY , "
				" LOCATION_KEY 					INTEGER NOT NULL, " " CONSTRAINT LOCATIONSFREEBIE_LOCATION_KEY FOREIGN KEY (LOCATION_KEY) REFERENCES LOCATIONS (LOCATION_KEY) ON UPDATE CASCADE ON DELETE CASCADE, " " ITEM_NAME							VARCHAR(50) NOT NULL, " " SIZE_NAME 						VARCHAR(30) NOT NULL, " " ITEM_COUNT 						INTEGER NOT NULL); ";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_LOCATIONSFREEBIE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE PROFILE ("
				" PROFILE_KEY INTEGER NOT NULL PRIMARY KEY," " PROFILE_TYPE INTEGER NOT NULL," " NAME VARCHAR(255));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_PROFILE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_PROFILE TO 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE VARIABLES (" " VARIABLES_KEY INTEGER NOT NULL PRIMARY KEY," " VARIABLE_NAME VARCHAR(75),"
				" VARIABLE_DESCRIPTION VARCHAR(255)," " VARIABLE_GROUP INTEGER," " VARCHAR_VAL VARCHAR(128)," " INTEGER_VAL INTEGER,"
				" NUMERIC_VAL NUMERIC(15,4)," " TIMESTAMP_VAL TIMESTAMP);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_VARIABLES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_VARIABLES TO 10000;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE VARIABLESOPTIONS ( " " VARIABLESOPTIONS_KEY INTEGER NOT NULL PRIMARY KEY,"
				" VARIABLES_KEY INTEGER NOT NULL," " CONSTRAINT VARIABLESOPTIONS_VARIABLES_KEY FOREIGN KEY (VARIABLES_KEY) REFERENCES VARIABLES (VARIABLES_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " OPTION_DESCRIPTION VARCHAR(45)," " VARCHAR_VAL VARCHAR(128)," " INTEGER_VAL INTEGER," " NUMERIC_VAL NUMERIC(15,4)," " TIMESTAMP_VAL TIMESTAMP);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_VARIABLESOPTIONS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_VARIABLESOPTIONS TO 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE VARSPROFILE (" " VARSPROFILE_KEY INTEGER NOT NULL PRIMARY KEY,"
				" VARIABLES_KEY INTEGER NOT NULL,"
				" CONSTRAINT VARSPROFILE_VARIABLES_KEY FOREIGN KEY (VARIABLES_KEY) REFERENCES VARIABLES (VARIABLES_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
				" PROFILE_KEY INTEGER NOT NULL," " CONSTRAINT VARSPROFILE_PROFILE_KEY FOREIGN KEY (PROFILE_KEY) REFERENCES PROFILE (PROFILE_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " VARCHAR_VAL VARCHAR(128)," " INTEGER_VAL INTEGER," " NUMERIC_VAL NUMERIC(15,4)," " TIMESTAMP_VAL TIMESTAMP);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_VARSPROFILE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "SET GENERATOR GEN_VARSPROFILE TO 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE STOCKTRACK ADD ADJUSTMENT_TYPE INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVESTOCKTRACK ADD ADJUSTMENT_TYPE INTEGER;";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ITEMSIDES" " SET ALLOW_SKIP = 'T'";
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT CONTACTS_KEY,SWIPE_CARD FROM CONTACTS WHERE SWIPE_CARD IS NOT NULL";
			IBQuery->ExecQuery();

			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   int ContactCardKey = 0;
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_CONTACTCARDS, 1) FROM RDB$DATABASE";
			   IBQuery1->ExecQuery();
			   ContactCardKey = IBQuery1->Fields[0]->AsInteger;

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "INSERT INTO CONTACTCARDS (" " CONTACTCARDS_KEY," " CONTACTS_KEY," " SWIPE_CARD)"
				   " VALUES ( :CONTACTCARDS_KEY,:CONTACTS_KEY,:SWIPE_CARD);";
			   IBQuery1->ParamByName("CONTACTCARDS_KEY")->AsInteger = ContactCardKey;
			   IBQuery1->ParamByName("CONTACTS_KEY")->AsInteger = IBQuery->FieldByName("CONTACTS_KEY")->AsInteger;
			   IBQuery1->ParamByName("SWIPE_CARD")->AsString = IBQuery->FieldByName("SWIPE_CARD")->AsString;
			   IBQuery1->ExecQuery();
			}

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS DROP SWIPE_CARD;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ALTER ORDER_LOCATION TYPE VARCHAR(25);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ALTER ORDER_LOCATION TYPE VARCHAR(25);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ALTER ORDER_LOCATION TYPE VARCHAR(25);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS DROP LOYALTY_NAME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TRIGGER MENUDELETE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TRIGGER MENUINSERT;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TRIGGER TABCHANGED;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX DAYARCHIVE_LOYALTY_KEY_INDEX ON DAYARCHIVE LOYALTY_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ARCHIVE_LOYALTY_KEY_INDEX ON ARCHIVE LOYALTY_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE INDEX ORDERS_LOYALTY_KEY_INDEX ON ORDERS LOYALTY_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE MENU" " SET PALMABLE = :PALMABLE ";
			IBQuery1->ParamByName("PALMABLE")->AsString = "T";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_31()
{
   AnsiString UpdateFromVersion = "3.3";
   AnsiString UpdateVersion = "3.31";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();
	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES ADD THIRDPARTYCODES_KEY INTEGER;";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_32()
{
   AnsiString UpdateFromVersion = "3.31";
   AnsiString UpdateVersion = "3.32";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			DBTransaction.StartTransaction();

			// Archive Table.
			IBQuery1->Close();
			IBQuery1->ParamCheck = false;
			IBQuery1->SQL->Text =
				"Create Procedure UpdateCategories As"
				" Declare Variable vCategory_Key Integer;"
				" Declare Variable vARCHIVE_Key Integer;" " Begin" " For" "  Select" "	ItemSize.Category_Key,"
				"	ARCHIVE.ARCHIVE_Key" "  From" "	ARCHIVE left join menu on ARCHIVE.menu_name = menu.menu_name"
				"	left join course on (ARCHIVE.course_name = course.course_name and course.menu_key = menu.menu_key)"
				"	left join item on (ARCHIVE.Item_Name = item.Item_Name and item.course_key = course.course_key)"
				"	left join itemsize on (ARCHIVE.size_name = itemsize.size_name and itemsize.item_key = item.item_key)" "  Where"
				"	ARCHIVE.Category_Key in (" "	 Select" "	  ArcCategories.Category_Key" "	 from" "	  ArcCategories" "	 where"
				"	  ArcCategories.Category = '')" "  Into" "	:vCategory_Key," "	:vARCHIVE_Key" " Do" " Begin" "  Update" "	ARCHIVE"
				"  Set" "	Category_Key = :vCategory_Key" "  Where" "	ARCHIVE_Key = :vARCHIVE_Key;" " End" " End";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "Execute Procedure UpdateCategories;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "Drop Procedure UpdateCategories;";
			IBQuery1->ExecQuery();

			// Dayarchive Table.
			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"Create Procedure UpdateCategories1 As"
				" Declare Variable vCategory_Key Integer;"
				" Declare Variable vARCHIVE_Key Integer;" " Begin" " For" "  Select" "	ItemSize.Category_Key,"
				"	DAYARCHIVE.ARCHIVE_Key" "  From" "	DAYARCHIVE left join menu on DAYARCHIVE.menu_name = menu.menu_name"
				"	left join course on (DAYARCHIVE.course_name = course.course_name and course.menu_key = menu.menu_key)"
				"	left join item on (DAYARCHIVE.Item_Name = item.Item_Name and item.course_key = course.course_key)"
				"	left join itemsize on (DAYARCHIVE.size_name = itemsize.size_name and itemsize.item_key = item.item_key)" "  Where"
				"	DAYARCHIVE.Category_Key in (" "	 Select" "	  ArcCategories.Category_Key" "	 from" "	  ArcCategories" "	 where"
				"	  ArcCategories.Category = '')" "  Into" "	:vCategory_Key," "	:vARCHIVE_Key" " Do" " Begin" "  Update" "	DAYARCHIVE"
				"  Set" "	Category_Key = :vCategory_Key" "  Where" "	ARCHIVE_Key = :vARCHIVE_Key;" " End" " End;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "Execute Procedure UpdateCategories1;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "Drop Procedure UpdateCategories1;";
			IBQuery1->ExecQuery();

			// Orders Table.
			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"Create Procedure UpdateCategories2 As"
				" Declare Variable vCategory_Key Integer;" " Declare Variable vORDER_Key Integer;"
				" Begin" " For" "  Select" "	ItemSize.Category_Key," "	ORDERS.ORDER_Key" "  From"
				"	ORDERS left join menu on ORDERS.menu_name = menu.menu_name"
				"	left join course on (ORDERS.course_name = course.course_name and course.menu_key = menu.menu_key)"
				"	left join item on (ORDERS.Item_Name = item.Item_Name and item.course_key = course.course_key)"
				"	left join itemsize on (ORDERS.size_name = itemsize.size_name and itemsize.item_key = item.item_key)" "  Where"
				"	ORDERS.Category_Key in (" "	 Select" "	  ArcCategories.Category_Key" "	 from" "	  ArcCategories" "	 where"
				"	  ArcCategories.Category = '')" "  Into" "	:vCategory_Key," "	:vORDER_Key" " Do" " Begin" "  Update" "	ORDERS"
				"  Set" "	Category_Key = :vCategory_Key" "  Where" "	ORDER_Key = :vORDER_Key;" " End" " End;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "Execute Procedure UpdateCategories2;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "Drop Procedure UpdateCategories2;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "Delete from ARCCATEGORIES where CATEGORY = '';";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "Delete from CATEGORYGROUPS where NAME = '';";
			IBQuery1->ExecQuery();

			IBQuery1->ParamCheck = true;

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_33()
{
   AnsiString UpdateFromVersion = "3.32";
   AnsiString UpdateVersion = "3.33";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD MEMBER_FREE_SALE_COUNT INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD MEMBER_FREE_SALE_DISCOUNT NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD LOCATION_FREE_SALE_COUNT INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD LOCATION_FREE_SALE_DISCOUNT NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD IS_FREE T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_34()
{
   AnsiString UpdateFromVersion = "3.33";
   AnsiString UpdateVersion = "3.34";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DISCOUNTS ADD DISCOUNT_TYPE INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD DISCOUNT_REASON VARCHAR(40);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD DISCOUNT_REASON VARCHAR(40);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD DISCOUNT_REASON VARCHAR(40);";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_3_35()
{
   AnsiString UpdateFromVersion = "3.34";
   AnsiString UpdateVersion = "3.35";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE KITCHENDOCKETS ("
				" DOCKETS_KEY INTEGER NOT NULL PRIMARY KEY," " DOCKETS_NUMBER INTEGER NOT NULL,"
				" NAME VARCHAR(20)," " BARCODE VARCHAR(20)," " CHITNUMBER VARCHAR(20)," " TERMINAL_NAME VARCHAR(22),"
				" TIME_KEY INTEGER," " DOCKET BLOB SUB_TYPE 1," " TIME_STAMP TIMESTAMP);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_KITCHENDOCKETS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_DOCKETSNUMBER;";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_0()
{

   AnsiString UpdateFromVersion = "3.35";
   AnsiString UpdateVersion = "4.0";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();
	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			DBTransaction.StartTransaction();
			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE SERVINGCOURSES ("
				" SERVINGCOURSES_KEY INTEGER NOT NULL PRIMARY KEY,"
				" SERVINGCOURSE_NAME VARCHAR(45)," " SERVINGCOURSE_KITCHEN_NAME VARCHAR(45),"
				" DELETED T_TRUEFALSE," " SELECTABLE T_TRUEFALSE," " DISPLAY_ORDER INTEGER," " COLOUR INTEGER);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_SERVINGCOURSES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE MENUSERVINGCOURSES (" " MENUSERVINGCOURSES_KEY INTEGER NOT NULL PRIMARY KEY,"
				" MENU_KEY INTEGER NOT NULL," " CONSTRAINT MSC_MENU_KEY FOREIGN KEY (MENU_KEY) REFERENCES MENU (MENU_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
				" SERVINGCOURSES_KEY INTEGER NOT NULL," " CONSTRAINT MSC_SERVINGCOURSES_KEY FOREIGN KEY (SERVINGCOURSES_KEY) REFERENCES SERVINGCOURSES (SERVINGCOURSES_KEY) ON UPDATE CASCADE ON DELETE CASCADE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_MENUSERVINGCOURSES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE PHYSICALPRINTER ("
				" PHYSICALPRINTER_KEY INTEGER NOT NULL PRIMARY KEY," " PRINTER_TYPE INTEGER,"
				" SHARE_NAME VARCHAR(80)," " COMPUTER_NAME VARCHAR(45)," " PROFILE_KEY INTEGER);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_PHYSICALPRINTER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE VIRTUALPRINTER (" " VIRTUALPRINTER_KEY INTEGER NOT NULL PRIMARY KEY," " NAME VARCHAR(80),"
				" PHYSICALPRINTER_KEY INTEGER,"
				" CONSTRAINT VP_PHYSICALPRINTER_KEY FOREIGN KEY (PHYSICALPRINTER_KEY) REFERENCES PHYSICALPRINTER (PHYSICALPRINTER_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " NEVER_PRINTS T_TRUEFALSE," " PROFILE_KEY INTEGER);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_VIRTUALPRINTER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE DEVICEVIRTUALPRINTER (" " DEVICEVIRTUALPRINTER_KEY INTEGER NOT NULL PRIMARY KEY,"
				" VIRTUALPRINTER_KEY INTEGER NOT NULL,"
				" CONSTRAINT DVP_VIRTUALPRINTER_KEY FOREIGN KEY (VIRTUALPRINTER_KEY) REFERENCES VIRTUALPRINTER (VIRTUALPRINTER_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
				" DEVICE_KEY INTEGER," " CONSTRAINT DDK_DEVICE_KEY FOREIGN KEY (DEVICE_KEY) REFERENCES DEVICES (DEVICE_KEY) ON UPDATE CASCADE ON DELETE CASCADE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_DEVICEVIRTUALPRINTER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE KITCHENPRINTING (" " KITCHENPRINTING_KEY INTEGER NOT NULL PRIMARY KEY,"
				" VIRTUALPRINTER_KEY INTEGER NOT NULL,"
				" CONSTRAINT KP_VIRTUALPRINTER_KEY FOREIGN KEY (VIRTUALPRINTER_KEY) REFERENCES VIRTUALPRINTER (VIRTUALPRINTER_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " DISPLAY_ORDER INTEGER," " INSTRUCTION INTEGER," " CAPTION VARCHAR(45)," " LINE_ABOVE T_TRUEFALSE," " LINE_BELOW T_TRUEFALSE," " DRAW_LINE_ABOVE T_TRUEFALSE," " DRAW_LINE_BELOW T_TRUEFALSE," " CUT T_TRUEFALSE," " VISIBLE T_TRUEFALSE," " PRINT_UNDERLINED T_TRUEFALSE," " PRINT_BOLD T_TRUEFALSE," " PRINT_DOUBLE_WIDTH T_TRUEFALSE," " PRINT_DOUBLE_HEIGHT T_TRUEFALSE," " PRINT_COLOUR INTEGER," " PRINT_FONT INTEGER);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_KITCHENPRINTING;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE DISCOUNTCARDS (" " DISCOUNTCARDS_KEY INTEGER NOT NULL PRIMARY KEY," " DISCOUNT_KEY INTEGER," " CONSTRAINT DISCOUNTCARDS_DISCOUNT_KEY FOREIGN KEY (DISCOUNT_KEY) REFERENCES DISCOUNTS (DISCOUNT_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " CARD VARCHAR(128));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_DISCOUNTCARDS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE PRNSERVINGCOURSEORDER (" " PRNSERVINGCOURSEORDER_KEY INTEGER NOT NULL PRIMARY KEY,"
				" VIRTUALPRINTER_KEY INTEGER," " CONSTRAINT PSCO_VIRTUALPRINTER_KEY FOREIGN KEY (VIRTUALPRINTER_KEY) REFERENCES VIRTUALPRINTER (VIRTUALPRINTER_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
				" SERVINGCOURSES_KEY INTEGER," " CONSTRAINT PSCO_SERVINGCOURSES_KEY FOREIGN KEY (SERVINGCOURSES_KEY) REFERENCES SERVINGCOURSES (SERVINGCOURSES_KEY) ON UPDATE CASCADE ON DELETE CASCADE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_PRNSERVINGCOURSEORDER;";
			IBQuery1->ExecQuery();

			IBQuery->Close();

			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "ARCBILL_DESC_KEY_INDEX";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "DROP INDEX ARCBILL_DESC_KEY_INDEX;";
			   IBQuery1->ExecQuery();
			}

			IBQuery->Close();

			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "DAYARCBILL_DESC_KEY_INDEX";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "DROP INDEX DAYARCBILL_DESC_KEY_INDEX;";
			   IBQuery1->ExecQuery();
			}

			DBTransaction.Commit();
			DB6Disconnect();
			DB1Connect();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE PATRONCOUNT (" " PATRONCOUNT_KEY INTEGER NOT NULL PRIMARY KEY," " ARCBILL_KEY INTEGER,"
				" CONSTRAINT PATRONCOUNT_ARCBILL_KEY FOREIGN KEY (ARCBILL_KEY) REFERENCES ARCBILL (ARCBILL_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " PATRON_TYPE VARCHAR(40)," " PATRON_COUNT INTEGER);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_PATRONCOUNT;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE DAYPATRONCOUNT (" " PATRONCOUNT_KEY INTEGER NOT NULL PRIMARY KEY," " ARCBILL_KEY INTEGER,"
				" CONSTRAINT DAYPATRONCOUNT_ARCBILL_KEY FOREIGN KEY (ARCBILL_KEY) REFERENCES DAYARCBILL (ARCBILL_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " PATRON_TYPE VARCHAR(40)," " PATRON_COUNT INTEGER);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_DAYPATRONCOUNT;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE PATRONTYPES (" " PATRONTYPES_KEY INTEGER NOT NULL PRIMARY KEY," " PATRON_TYPE VARCHAR(40),"
				" IS_DEFAULT T_TRUEFALSE);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_PATRONTYPES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE REFS (" " REFS_KEY INTEGER NOT NULL PRIMARY KEY," " REFTYPE INTEGER," " NAME VARCHAR(20),"
				" CODE VARCHAR(20));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_REFS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_PHOENIX_REF;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE DAYARCREF (" " DAYARCREF_KEY INTEGER NOT NULL PRIMARY KEY," " ARCBILL_KEY INTEGER,"
				" CONSTRAINT DAYARCREF_ARCBILL_KEY FOREIGN KEY (ARCBILL_KEY) REFERENCES DAYARCBILL (ARCBILL_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
				" REFS_KEY INTEGER," " CONSTRAINT DAYARCREF_REFERENCE_KEY FOREIGN KEY (REFS_KEY) REFERENCES REFS (REFS_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " REFERENCE VARCHAR(20));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_DAYARCREF;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE ARCREF (" " ARCREF_KEY INTEGER NOT NULL PRIMARY KEY," " ARCBILL_KEY INTEGER,"
				" CONSTRAINT RCREF_ARCBILL_KEY FOREIGN KEY (ARCBILL_KEY) REFERENCES ARCBILL (ARCBILL_KEY) ON UPDATE CASCADE ON DELETE CASCADE,"
				" REFS_KEY INTEGER," " CONSTRAINT ARCREF_REFERENCE_KEY FOREIGN KEY (REFS_KEY) REFERENCES REFS (REFS_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " REFERENCE VARCHAR(20));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_ARCREF;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE DESCENDING INDEX DAYARCBILL_DESC_KEY_INDEX ON DAYARCBILL(ARCBILL_KEY);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE DESCENDING INDEX ARCBILL_DESC_KEY_INDEX ON ARCBILL(ARCBILL_KEY);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONNECTIONS ADD PHYSICALPRINTER_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE CONNECTIONS ADD CONSTRAINT CON_PHYSICALPRINTER_KEY FOREIGN KEY (PHYSICALPRINTER_KEY) REFERENCES PHYSICALPRINTER (PHYSICALPRINTER_KEY) ON UPDATE CASCADE ON DELETE SET NULL;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DELETE FROM PRNORDER;";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DB6Disconnect();
			DB1Connect();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP INDEX COURSEINDEX1;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP INDEX COURSEIDINDEX1;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PRNORDER DROP CONNECT_KEY;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD PRICE_LEVEL0 NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD PRICE_LEVEL0 NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD PRICE_LEVEL0 NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD PRICE_LEVEL1 NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD PRICE_LEVEL1 NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD PRICE_LEVEL1 NUMERIC(15,4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PRNORDER ADD VIRTUALPRINTER_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PRNORDER ADD REDIRECT_FROM INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE PRNORDER ADD CONSTRAINT PRNORD_VIRTUALPRINTER_KEY FOREIGN KEY (VIRTUALPRINTER_KEY) REFERENCES VIRTUALPRINTER (VIRTUALPRINTER_KEY) ON UPDATE CASCADE ON DELETE CASCADE;";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD SERVINGCOURSES_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ORDERS ADD CONSTRAINT ORDERS_SERVINGCOURSES_KEY FOREIGN KEY (SERVINGCOURSES_KEY) REFERENCES SERVINGCOURSES (SERVINGCOURSES_KEY) ON UPDATE CASCADE ON DELETE SET NULL;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE ADD SERVINGCOURSES_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE DAYARCHIVE ADD CONSTRAINT DAYARCHIVE_SERVINGCOURSES_KEY FOREIGN KEY (SERVINGCOURSES_KEY) REFERENCES SERVINGCOURSES (SERVINGCOURSES_KEY) ON UPDATE CASCADE ON DELETE SET NULL;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCHIVE ADD SERVINGCOURSES_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ARCHIVE ADD CONSTRAINT ARCHIVE_SERVINGCOURSES_KEY FOREIGN KEY (SERVINGCOURSES_KEY) REFERENCES SERVINGCOURSES (SERVINGCOURSES_KEY) ON UPDATE CASCADE ON DELETE SET NULL;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEM ADD PRINT_UNDERLINED T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEM ADD PRINT_BOLD T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEM ADD PRINT_COLOUR INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEM ADD PRINT_FONT INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEM ADD PRINT_DOUBLE_WIDTH T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEM ADD PRINT_DOUBLE_HEIGHT T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE OPTIONS ADD PRINT_UNDERLINED T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE OPTIONS ADD PRINT_BOLD T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE OPTIONS ADD PRINT_COLOUR INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE OPTIONS ADD PRINT_FONT INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE OPTIONS ADD PRINT_DOUBLE_WIDTH T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE OPTIONS ADD PRINT_DOUBLE_HEIGHT T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE MENU ADD DELETED T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE COURSE ADD SERVINGCOURSES_KEY INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE COURSE ADD NO_DEFAULT_SERVING_COURSE T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD PALM_SCREEN_LOCK T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD PALM_CLASSIC_MODE T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD PALM_SOUND_LEVEL INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD PALM_GET_ALL_TABLES T_TRUEFALSE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE COURSE ADD CONSTRAINT COURSE_SERVINGCOURSES_KEY FOREIGN KEY (SERVINGCOURSES_KEY) REFERENCES SERVINGCOURSES (SERVINGCOURSES_KEY) ON UPDATE CASCADE ON DELETE SET NULL;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ALTER COLUMN PIN TYPE VARCHAR(10);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE VARSPROFILE ADD VARS_ORDER INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP INDEX DESCRIPTIONINDEX1;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE STOCKTRACK DROP STOCK_DESC;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE STOCKTRACK DROP UNIT;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE STOCKTRACK DROP DEFAULT_LOCATION;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE STOCKTRACK DROP DOLLAR_VALUE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE STOCKTRACK DROP COST;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DISCOUNTS ADD DISCOUNT_MODE INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DISCOUNTS ALTER \"PERCENT\" TO PERCENTAGE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERRECIPE ALTER STOCK_CODE TYPE VARCHAR(25);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE RECIPE ALTER STOCK_CODE TYPE VARCHAR(25)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTFREEBIE ADD MAX_COUNT INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE LOCATIONSFREEBIE ADD MAX_COUNT INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES ADD TAX_RATE NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			// System References.

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT GEN_ID(GEN_REFS, 1) FROM RDB$DATABASE";
			IBQuery->ExecQuery();
			int REFKey = IBQuery->Fields[0]->AsInteger;

			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO REFS (" " REFS_KEY," " REFTYPE," " NAME," " CODE)"
				" VALUES ( :REFS_KEY,:REFTYPE,:NAME,:CODE);";
			IBQuery1->ParamByName("REFS_KEY")->AsInteger = REFKey;
			IBQuery1->ParamByName("REFTYPE")->AsInteger = 1;
			IBQuery1->ParamByName("NAME")->AsString = "EFTPOS Ref";
			IBQuery1->ParamByName("CODE")->AsString = "EFTREF";
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->ExecQuery();
			REFKey = IBQuery->Fields[0]->AsInteger;

			IBQuery1->Close();
			IBQuery1->ParamByName("REFS_KEY")->AsInteger = REFKey;
			IBQuery1->ParamByName("REFTYPE")->AsInteger = 2;
			IBQuery1->ParamByName("NAME")->AsString = "PMS Ref";
			IBQuery1->ParamByName("CODE")->AsString = "PMSREF";
			IBQuery1->ExecQuery();

			// Serving Courses.

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT GEN_ID(GEN_SERVINGCOURSES, 1) FROM RDB$DATABASE";
			IBQuery->ExecQuery();
			int NoneServingKey = IBQuery->Fields[0]->AsInteger;

			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Clear();
			IBQuery1->SQL->Text =
				"INSERT INTO SERVINGCOURSES (" "SERVINGCOURSES_KEY," "SERVINGCOURSE_NAME, "
				"SERVINGCOURSE_KITCHEN_NAME, " "DELETED, " "SELECTABLE, " "DISPLAY_ORDER, " "COLOUR) " "VALUES ("
				":SERVINGCOURSES_KEY," ":SERVINGCOURSE_NAME, " ":SERVINGCOURSE_KITCHEN_NAME, " ":DELETED, " ":SELECTABLE, "
				":DISPLAY_ORDER, " ":COLOUR);";
			IBQuery1->ParamByName("SERVINGCOURSES_KEY")->AsInteger = NoneServingKey;
			IBQuery1->ParamByName("SERVINGCOURSE_NAME")->AsString = "No Serving Course";
			IBQuery1->ParamByName("SERVINGCOURSE_KITCHEN_NAME")->AsString = "No Serving Course";
			IBQuery1->ParamByName("DELETED")->AsString = "F";
			IBQuery1->ParamByName("SELECTABLE")->AsString = "F";
			IBQuery1->ParamByName("DISPLAY_ORDER")->AsInteger = 4;
			IBQuery1->ParamByName("COLOUR")->AsInteger = (int)clOlive;
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT GEN_ID(GEN_SERVINGCOURSES, 1) FROM RDB$DATABASE";
			IBQuery->ExecQuery();
			int EntreeServingKey = IBQuery->Fields[0]->AsInteger;

			// Add new Reocrd.
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Clear();
			IBQuery1->SQL->Text =
				"INSERT INTO SERVINGCOURSES (" "SERVINGCOURSES_KEY," "SERVINGCOURSE_NAME, "
				"SERVINGCOURSE_KITCHEN_NAME, " "DELETED, " "SELECTABLE, " "DISPLAY_ORDER, " "COLOUR) " "VALUES ("
				":SERVINGCOURSES_KEY," ":SERVINGCOURSE_NAME, " ":SERVINGCOURSE_KITCHEN_NAME, " ":DELETED, " ":SELECTABLE, "
				":DISPLAY_ORDER, " ":COLOUR);";
			IBQuery1->ParamByName("SERVINGCOURSES_KEY")->AsInteger = EntreeServingKey;
			IBQuery1->ParamByName("SERVINGCOURSE_NAME")->AsString = "Entrees";
			IBQuery1->ParamByName("SERVINGCOURSE_KITCHEN_NAME")->AsString = "Entrees";
			IBQuery1->ParamByName("DELETED")->AsString = "F";
			IBQuery1->ParamByName("SELECTABLE")->AsString = "T";
			IBQuery1->ParamByName("DISPLAY_ORDER")->AsInteger = 1;
			IBQuery1->ParamByName("COLOUR")->AsInteger = (int)RGB(0, 200, 0);
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT GEN_ID(GEN_SERVINGCOURSES, 1) FROM RDB$DATABASE";
			IBQuery->ExecQuery();
			int MainsServingKey = IBQuery->Fields[0]->AsInteger;

			// Add new Reocrd.
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Clear();
			IBQuery1->SQL->Text =
				"INSERT INTO SERVINGCOURSES (" "SERVINGCOURSES_KEY," "SERVINGCOURSE_NAME, "
				"SERVINGCOURSE_KITCHEN_NAME, " "DELETED, " "SELECTABLE, " "DISPLAY_ORDER, " "COLOUR) " "VALUES ("
				":SERVINGCOURSES_KEY," ":SERVINGCOURSE_NAME, " ":SERVINGCOURSE_KITCHEN_NAME, " ":DELETED, " ":SELECTABLE, "
				":DISPLAY_ORDER, " ":COLOUR);";
			IBQuery1->ParamByName("SERVINGCOURSES_KEY")->AsInteger = MainsServingKey;
			IBQuery1->ParamByName("SERVINGCOURSE_NAME")->AsString = "Mains";
			IBQuery1->ParamByName("SERVINGCOURSE_KITCHEN_NAME")->AsString = "Mains";
			IBQuery1->ParamByName("DELETED")->AsString = "F";
			IBQuery1->ParamByName("SELECTABLE")->AsString = "T";
			IBQuery1->ParamByName("DISPLAY_ORDER")->AsInteger = 2;
			IBQuery1->ParamByName("COLOUR")->AsInteger = (int)clRed;
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT GEN_ID(GEN_SERVINGCOURSES, 1) FROM RDB$DATABASE";
			IBQuery->ExecQuery();
			int DessertServingKey = IBQuery->Fields[0]->AsInteger;

			// Add new Reocrd.
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Clear();
			IBQuery1->SQL->Text =
				"INSERT INTO SERVINGCOURSES (" "SERVINGCOURSES_KEY," "SERVINGCOURSE_NAME, "
				"SERVINGCOURSE_KITCHEN_NAME, " "DELETED, " "SELECTABLE, " "DISPLAY_ORDER, " "COLOUR) " "VALUES ("
				":SERVINGCOURSES_KEY," ":SERVINGCOURSE_NAME, " ":SERVINGCOURSE_KITCHEN_NAME, " ":DELETED, " ":SELECTABLE, "
				":DISPLAY_ORDER, " ":COLOUR);";
			IBQuery1->ParamByName("SERVINGCOURSES_KEY")->AsInteger = DessertServingKey;
			IBQuery1->ParamByName("SERVINGCOURSE_NAME")->AsString = "Desserts";
			IBQuery1->ParamByName("SERVINGCOURSE_KITCHEN_NAME")->AsString = "Desserts";
			IBQuery1->ParamByName("DELETED")->AsString = "F";
			IBQuery1->ParamByName("SELECTABLE")->AsString = "T";
			IBQuery1->ParamByName("DISPLAY_ORDER")->AsInteger = 3;
			IBQuery1->ParamByName("COLOUR")->AsInteger = (int)RGB(220, 0, 220);
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT GEN_ID(GEN_SERVINGCOURSES, 1) FROM RDB$DATABASE";
			IBQuery->ExecQuery();
			int BeverageServingKey = IBQuery->Fields[0]->AsInteger;

			// Add new Reocrd.
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Clear();
			IBQuery1->SQL->Text =
				"INSERT INTO SERVINGCOURSES (" "SERVINGCOURSES_KEY," "SERVINGCOURSE_NAME, "
				"SERVINGCOURSE_KITCHEN_NAME, " "DELETED, " "SELECTABLE, " "DISPLAY_ORDER, " "COLOUR) " "VALUES ("
				":SERVINGCOURSES_KEY," ":SERVINGCOURSE_NAME, " ":SERVINGCOURSE_KITCHEN_NAME, " ":DELETED, " ":SELECTABLE, "
				":DISPLAY_ORDER, " ":COLOUR);";
			IBQuery1->ParamByName("SERVINGCOURSES_KEY")->AsInteger = BeverageServingKey;
			IBQuery1->ParamByName("SERVINGCOURSE_NAME")->AsString = "Beverages";
			IBQuery1->ParamByName("SERVINGCOURSE_KITCHEN_NAME")->AsString = "Beverages";
			IBQuery1->ParamByName("DELETED")->AsString = "F";
			IBQuery1->ParamByName("SELECTABLE")->AsString = "T";
			IBQuery1->ParamByName("DISPLAY_ORDER")->AsInteger = 1;
			IBQuery1->ParamByName("COLOUR")->AsInteger = (int)clBlue;
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->ParamCheck = false;

			IBQuery->SQL->Text = "Select * from MENU";
			IBQuery->ExecQuery();
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   if (IBQuery->FieldByName("MENU_TYPE")->AsInteger == 0)
			   {
				  IBQuery2->Close();
				  IBQuery2->SQL->Text = "SELECT GEN_ID(GEN_MENUSERVINGCOURSES, 1) FROM RDB$DATABASE";
				  IBQuery2->ExecQuery();
				  int KeyVal = IBQuery2->Fields[0]->AsInteger;

				  IBQuery2->Close();
				  IBQuery2->SQL->Text =
					  "INSERT INTO MENUSERVINGCOURSES (" "MENUSERVINGCOURSES_KEY," "MENU_KEY," "SERVINGCOURSES_KEY) "
					  "VALUES (" ":MENUSERVINGCOURSES_KEY," ":MENU_KEY," ":SERVINGCOURSES_KEY);";
				  IBQuery2->ParamByName("MENUSERVINGCOURSES_KEY")->AsInteger = KeyVal;
				  IBQuery2->ParamByName("MENU_KEY")->AsInteger = IBQuery->FieldByName("MENU_KEY")->AsInteger;
				  IBQuery2->ParamByName("SERVINGCOURSES_KEY")->AsInteger = NoneServingKey;
				  IBQuery2->ExecQuery();

				  IBQuery2->Close();
				  IBQuery2->SQL->Text = "SELECT GEN_ID(GEN_MENUSERVINGCOURSES, 1) FROM RDB$DATABASE";
				  IBQuery2->ExecQuery();
				  KeyVal = IBQuery2->Fields[0]->AsInteger;

				  IBQuery2->Close();
				  IBQuery2->SQL->Text =
					  "INSERT INTO MENUSERVINGCOURSES (" "MENUSERVINGCOURSES_KEY," "MENU_KEY," "SERVINGCOURSES_KEY) "
					  "VALUES (" ":MENUSERVINGCOURSES_KEY," ":MENU_KEY," ":SERVINGCOURSES_KEY);";
				  IBQuery2->ParamByName("MENUSERVINGCOURSES_KEY")->AsInteger = KeyVal;
				  IBQuery2->ParamByName("MENU_KEY")->AsInteger = IBQuery->FieldByName("MENU_KEY")->AsInteger;
				  IBQuery2->ParamByName("SERVINGCOURSES_KEY")->AsInteger = EntreeServingKey;
				  IBQuery2->ExecQuery();

				  IBQuery2->Close();
				  IBQuery2->SQL->Text = "SELECT GEN_ID(GEN_MENUSERVINGCOURSES, 1) FROM RDB$DATABASE";
				  IBQuery2->ExecQuery();
				  KeyVal = IBQuery2->Fields[0]->AsInteger;

				  IBQuery2->Close();
				  IBQuery2->SQL->Text =
					  "INSERT INTO MENUSERVINGCOURSES (" "MENUSERVINGCOURSES_KEY," "MENU_KEY," "SERVINGCOURSES_KEY) "
					  "VALUES (" ":MENUSERVINGCOURSES_KEY," ":MENU_KEY," ":SERVINGCOURSES_KEY);";
				  IBQuery2->ParamByName("MENUSERVINGCOURSES_KEY")->AsInteger = KeyVal;
				  IBQuery2->ParamByName("MENU_KEY")->AsInteger = IBQuery->FieldByName("MENU_KEY")->AsInteger;
				  IBQuery2->ParamByName("SERVINGCOURSES_KEY")->AsInteger = MainsServingKey;
				  IBQuery2->ExecQuery();

				  IBQuery2->Close();
				  IBQuery2->SQL->Text = "SELECT GEN_ID(GEN_MENUSERVINGCOURSES, 1) FROM RDB$DATABASE";
				  IBQuery2->ExecQuery();
				  KeyVal = IBQuery2->Fields[0]->AsInteger;

				  IBQuery2->Close();
				  IBQuery2->SQL->Text =
					  "INSERT INTO MENUSERVINGCOURSES (" "MENUSERVINGCOURSES_KEY," "MENU_KEY," "SERVINGCOURSES_KEY) "
					  "VALUES (" ":MENUSERVINGCOURSES_KEY," ":MENU_KEY," ":SERVINGCOURSES_KEY);";
				  IBQuery2->ParamByName("MENUSERVINGCOURSES_KEY")->AsInteger = KeyVal;
				  IBQuery2->ParamByName("MENU_KEY")->AsInteger = IBQuery->FieldByName("MENU_KEY")->AsInteger;
				  IBQuery2->ParamByName("SERVINGCOURSES_KEY")->AsInteger = DessertServingKey;
				  IBQuery2->ExecQuery();
			   }
			   else if (IBQuery->FieldByName("MENU_TYPE")->AsInteger == 1)
			   {
				  IBQuery2->Close();
				  IBQuery2->SQL->Text = "SELECT GEN_ID(GEN_MENUSERVINGCOURSES, 1) FROM RDB$DATABASE";
				  IBQuery2->ExecQuery();
				  int KeyVal = IBQuery2->Fields[0]->AsInteger;

				  IBQuery2->Close();
				  IBQuery2->SQL->Text =
					  "INSERT INTO MENUSERVINGCOURSES (" "MENUSERVINGCOURSES_KEY," "MENU_KEY," "SERVINGCOURSES_KEY) "
					  "VALUES (" ":MENUSERVINGCOURSES_KEY," ":MENU_KEY," ":SERVINGCOURSES_KEY);";
				  IBQuery2->ParamByName("MENUSERVINGCOURSES_KEY")->AsInteger = KeyVal;
				  IBQuery2->ParamByName("MENU_KEY")->AsInteger = IBQuery->FieldByName("MENU_KEY")->AsInteger;
				  IBQuery2->ParamByName("SERVINGCOURSES_KEY")->AsInteger = NoneServingKey;
				  IBQuery2->ExecQuery();

				  IBQuery2->Close();
				  IBQuery2->SQL->Text = "SELECT GEN_ID(GEN_MENUSERVINGCOURSES, 1) FROM RDB$DATABASE";
				  IBQuery2->ExecQuery();
				  KeyVal = IBQuery2->Fields[0]->AsInteger;

				  IBQuery2->Close();
				  IBQuery2->SQL->Text =
					  "INSERT INTO MENUSERVINGCOURSES (" "MENUSERVINGCOURSES_KEY," "MENU_KEY," "SERVINGCOURSES_KEY) "
					  "VALUES (" ":MENUSERVINGCOURSES_KEY," ":MENU_KEY," ":SERVINGCOURSES_KEY);";
				  IBQuery2->ParamByName("MENUSERVINGCOURSES_KEY")->AsInteger = KeyVal;
				  IBQuery2->ParamByName("MENU_KEY")->AsInteger = IBQuery->FieldByName("MENU_KEY")->AsInteger;
				  IBQuery2->ParamByName("SERVINGCOURSES_KEY")->AsInteger = BeverageServingKey;
				  IBQuery2->ExecQuery();
			   }
			}

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE MENU SET DELETED = 'F';";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTS SET PALM_SCREEN_LOCK = 'F';";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTS SET PALM_CLASSIC_MODE = 'F';";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE COURSE SET NO_DEFAULT_SERVING_COURSE = 'T';";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE COURSE" " SET " " NO_DEFAULT_SERVING_COURSE = 'F'," " SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY"
				" WHERE MENU_KEY IN" " (SELECT MENU_KEY" "	FROM" " MENU" " WHERE" " MENU_TYPE = :MENU_TYPE)";
			IBQuery1->ParamByName("SERVINGCOURSES_KEY")->AsInteger = BeverageServingKey;
			IBQuery1->ParamByName("MENU_TYPE")->AsInteger = 1;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ORDERS" " SET " " SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY" " WHERE MENU_NAME IN"
				" (SELECT MENU_NAME" "	FROM" " MENU" " WHERE" " MENU_TYPE = :MENU_TYPE)";
			IBQuery1->ParamByName("SERVINGCOURSES_KEY")->AsInteger = BeverageServingKey;
			IBQuery1->ParamByName("MENU_TYPE")->AsInteger = 1;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ORDERS" " SET " " SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY" " WHERE MENU_NAME IN"
				" (SELECT MENU_NAME" "	FROM" " MENU" " WHERE" " MENU_TYPE = :MENU_TYPE)";
			IBQuery1->ParamByName("SERVINGCOURSES_KEY")->AsInteger = NoneServingKey;
			IBQuery1->ParamByName("MENU_TYPE")->AsInteger = 0;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DAYARCHIVE" " SET " " SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
			IBQuery1->ParamByName("SERVINGCOURSES_KEY")->AsInteger = NoneServingKey;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCHIVE" " SET " " SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
			IBQuery1->ParamByName("SERVINGCOURSES_KEY")->AsInteger = NoneServingKey;
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTS SET PALM_SOUND_LEVEL = 2;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTS SET PALM_GET_ALL_TABLES = 'T';";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE PRNORDER SET REDIRECT_FROM = 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ITEM " " SET PRINT_UNDERLINED = 'F'," " PRINT_BOLD = 'F'," " PRINT_COLOUR = 0,"
				" PRINT_FONT = 0," " PRINT_DOUBLE_WIDTH = 'F'," " PRINT_DOUBLE_HEIGHT = 'F';";
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->ParamCheck = true;
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDICES.RDB$INDEX_NAME = :INDEXNAME ";
			IBQuery->ParamByName("INDEXNAME")->AsString = "ARCBILL_DESC_KEY_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE DESCENDING INDEX ARCBILL_DESC_KEY_INDEX ON ARCBILL(ARCBILL_KEY);";
			   IBQuery1->ExecQuery();
			}

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :INDEXNAME";
			IBQuery->ParamByName("INDEXNAME")->AsString = "DAYARCBILL_DESC_KEY_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE DESCENDING INDEX DAYARCBILL_DESC_KEY_INDEX ON DAYARCBILL(ARCBILL_KEY);";
			   IBQuery1->ExecQuery();
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM DAYARCBILL WHERE SALES_TYPE = 6";
			IBQuery->ExecQuery();
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   Currency CreditValue = IBQuery->FieldByName("TOTAL")->AsCurrency;
			   int ArcBillKey = IBQuery->FieldByName("ARCBILL_KEY")->AsCurrency;

			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCBILL, 1) FROM RDB$DATABASE";
			   IBInternalQuery->ExecQuery();
			   int DAYARCBILLPAY_KEY = IBInternalQuery->Fields[0]->AsInteger;
			   IBInternalQuery->Close();

			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Text =
				   "INSERT INTO DAYARCBILLPAY (" "DAYARCBILLPAY_KEY, " "ARCBILL_KEY, " "PAY_TYPE, " "VOUCHER_NUMBER, "
				   "SUBTOTAL, " "CASH_OUT, " "TAX_FREE, " "NOTE, " "PROPERTIES, " "GROUP_NUMBER) " "VALUES (" ":DAYARCBILLPAY_KEY, "
				   ":ARCBILL_KEY, " ":PAY_TYPE, " ":VOUCHER_NUMBER, " ":SUBTOTAL, " ":CASH_OUT, " ":TAX_FREE, " ":NOTE, " ":PROPERTIES, "
				   ":GROUP_NUMBER) ";

			   IBInternalQuery->Close();
			   IBInternalQuery->ParamByName("DAYARCBILLPAY_KEY")->AsInteger = DAYARCBILLPAY_KEY;
			   IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
			   IBInternalQuery->ParamByName("PAY_TYPE")->AsString = "Credit";
			   IBInternalQuery->ParamByName("VOUCHER_NUMBER")->AsString = "";
			   IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = -CreditValue;
			   IBInternalQuery->ParamByName("CASH_OUT")->AsString = "F";
			   IBInternalQuery->ParamByName("TAX_FREE")->AsString = "F";
			   IBInternalQuery->ParamByName("NOTE")->AsString = "System Generated Correction";
			   IBInternalQuery->ParamByName("PROPERTIES")->AsInteger |= 0x20000;
			   IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = 0;
			   IBInternalQuery->ExecQuery();

			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Text = "UPDATE DAYARCBILL" " SET TOTAL = :TOTAL " "WHERE ARCBILL_KEY = :ARCBILL_KEY";
			   IBInternalQuery->ParamByName("TOTAL")->AsCurrency = 0;
			   IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
			   IBInternalQuery->ExecQuery();
			}

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM ARCBILL WHERE SALES_TYPE = 6";
			IBQuery->ExecQuery();
			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   Currency CreditValue = IBQuery->FieldByName("TOTAL")->AsCurrency;
			   int ArcBillKey = IBQuery->FieldByName("ARCBILL_KEY")->AsCurrency;

			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCBILLPAY, 1) FROM RDB$DATABASE";
			   IBInternalQuery->ExecQuery();
			   int ARCBILLPAY_KEY = IBInternalQuery->Fields[0]->AsInteger;
			   IBInternalQuery->Close();

			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Text =
				   "INSERT INTO ARCBILLPAY (" "ARCBILLPAY_KEY, " "ARCBILL_KEY, " "PAY_TYPE, " "VOUCHER_NUMBER, "
				   "SUBTOTAL, " "CASH_OUT, " "TAX_FREE, " "NOTE, " "PROPERTIES, " "GROUP_NUMBER) " "VALUES ("
				   ":ARCBILLPAY_KEY, " ":ARCBILL_KEY, " ":PAY_TYPE, " ":VOUCHER_NUMBER, " ":SUBTOTAL, " ":CASH_OUT, " ":TAX_FREE, "
				   ":NOTE, " ":PROPERTIES, " ":GROUP_NUMBER) ";

			   IBInternalQuery->Close();
			   IBInternalQuery->ParamByName("ARCBILLPAY_KEY")->AsInteger = ARCBILLPAY_KEY;
			   IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
			   IBInternalQuery->ParamByName("PAY_TYPE")->AsString = "Credit";
			   IBInternalQuery->ParamByName("VOUCHER_NUMBER")->AsString = "";
			   IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = -CreditValue;
			   IBInternalQuery->ParamByName("CASH_OUT")->AsString = "F";
			   IBInternalQuery->ParamByName("TAX_FREE")->AsString = "F";
			   IBInternalQuery->ParamByName("NOTE")->AsString = "System Generated Correction";
			   IBInternalQuery->ParamByName("PROPERTIES")->AsInteger |= 0x20000;
			   IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = 0;
			   IBInternalQuery->ExecQuery();

			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Text = "UPDATE ARCBILL" " SET TOTAL = :TOTAL " "WHERE ARCBILL_KEY = :ARCBILL_KEY";
			   IBInternalQuery->ParamByName("TOTAL")->AsCurrency = 0;
			   IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
			   IBInternalQuery->ExecQuery();
			}

			// Unregister MenuMate.
			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE REGISTRATION SET REGKEY = '';";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DISCOUNTS SET DISCOUNT_MODE = 0;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DISCOUNTS SET DISCOUNT_MODE = 1 WHERE PERCENTAGE != 0 AND PERCENTAGE IS NOT NULL;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DISCOUNTS SET DISCOUNT_TYPE = 0 WHERE DISCOUNT_TYPE IS NULL;";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_1()
{
   AnsiString UpdateFromVersion = "4.0";
   AnsiString UpdateVersion = "4.1";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCBILLPAY ADD ROUNDING NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCSURCHARGE ADD ROUNDING NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCBILLPAY ADD ROUNDING NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCSURCHARGE ADD ROUNDING NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES ADD ROUNDTO NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			try
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX ARCBILL_INVOICE_KEY_INDEX ON ARCBILL INVOICE_KEY;";
			   IBQuery1->ExecQuery();
			}
			catch(...)
			{
			}

			try
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX DAYARCBILL_INVOICE_KEY_INDEX ON DAYARCBILL INVOICE_KEY;";
			   IBQuery1->ExecQuery();
			}
			catch(...)
			{
			}

			try
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX TAB_INVOICE_KEY_INDEX ON TAB INVOICE_KEY;";
			   IBQuery1->ExecQuery();
			}
			catch(...)
			{
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"select ARCHIVE_KEY from archive where gst_percent = 10 "
				"union " "select ARCHIVE_KEY from dayarchive where gst_percent = 10 ";
			IBQuery1->ExecQuery();
			if (IBQuery1->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text =
				   "select ARCHIVE_KEY from archive where gst_percent = 12.5 "
				   "union " "select ARCHIVE_KEY from dayarchive where gst_percent = 12.5 ";
			   IBQuery1->ExecQuery();
			   if (!IBQuery1->RecordCount == 0)
			   {
				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE ARCHIVE SET GST_PERCENT = 12.5 WHERE GST_PERCENT = 0;";
				  IBQuery1->ExecQuery();

				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE DAYARCHIVE SET GST_PERCENT = 12.5 WHERE GST_PERCENT = 0;";
				  IBQuery1->ExecQuery();
			   }
			}

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE PAYMENTTYPES SET ROUNDTO = 0.01;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE PAYMENTTYPES SET ROUNDTO = 0.05 WHERE UPPER(PAYMENT_NAME) = 'CASH';";
			IBQuery1->ExecQuery();

			// Remove Duplicate breakdown Categories from the Items in the DB.
			// Fix for bug #1088.
			IBQuery->Close();
			IBQuery->SQL->Text =
				"select arccategory.archive_key, arccategory.category_key,count(arccategory.category_key) " "from arccategory " "group by arccategory.archive_key, arccategory.category_key " "having count(arccategory.category_key) > 1 ";
			IBQuery->ExecQuery();

			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   int ArchiveKey = IBQuery->FieldByName("ARCHIVE_KEY")->AsInteger;
			   int CategoryKey = IBQuery->FieldByName("CATEGORY_KEY")->AsInteger;

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "DELETE FROM ARCCATEGORY WHERE ARCHIVE_KEY = :ARCHIVE_KEY AND CATEGORY_KEY = :CATEGORY_KEY;";
			   IBQuery1->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
			   IBQuery1->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
			   IBQuery1->ExecQuery();

			   IBQuery2->Close();
			   IBQuery2->SQL->Text =
				   "INSERT INTO ARCCATEGORY (" "ARCHIVE_KEY, " "CATEGORY_KEY) " "VALUES (" ":ARCHIVE_KEY, "
				   ":CATEGORY_KEY) ";
			   IBQuery2->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
			   IBQuery2->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
			   IBQuery2->ExecQuery();
			}

			IBQuery->Close();
			IBQuery->SQL->Text =
				"select dayarccategory.archive_key, dayarccategory.category_key,count(dayarccategory.category_key) " "from dayarccategory " "group by dayarccategory.archive_key, dayarccategory.category_key " "having count(dayarccategory.category_key) > 1 ";
			IBQuery->ExecQuery();

			for (; !IBQuery->Eof; IBQuery->Next())
			{
			   int ArchiveKey = IBQuery->FieldByName("ARCHIVE_KEY")->AsInteger;
			   int CategoryKey = IBQuery->FieldByName("CATEGORY_KEY")->AsInteger;

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "DELETE FROM DAYARCCATEGORY WHERE ARCHIVE_KEY = :ARCHIVE_KEY AND CATEGORY_KEY = :CATEGORY_KEY;";
			   IBQuery1->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
			   IBQuery1->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
			   IBQuery1->ExecQuery();

			   IBQuery2->Close();
			   IBQuery2->SQL->Text =
				   "INSERT INTO DAYARCCATEGORY (" "ARCHIVE_KEY, " "CATEGORY_KEY) " "VALUES ("
				   ":ARCHIVE_KEY, " ":CATEGORY_KEY) ";
			   IBQuery2->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
			   IBQuery2->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
			   IBQuery2->ExecQuery();
			}

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_2()
{
   AnsiString UpdateFromVersion = "4.1";
   AnsiString UpdateVersion = "4.2";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DISCOUNTS ADD ROUNDING NUMERIC(15, 4);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DISCOUNTS ADD CATEGORY_KEY INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE DISCOUNTS ADD FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE ON DELETE SET NULL";
			IBQuery1->ExecQuery();

			try
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX MAKE_START_TIME_INDEX ON TURNAROUNDTIMES MAKE_START_TIME;";
			   IBQuery1->ExecQuery();
			}
			catch(...)
			{
			}

			try
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX MAKE_END_TIME_INDEX ON TURNAROUNDTIMES MAKE_END_TIME;";
			   IBQuery1->ExecQuery();
			}
			catch(...)
			{
			}

			try
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX SALE_START_TIME_INDEX ON TURNAROUNDTIMES SALE_START_TIME;";
			   IBQuery1->ExecQuery();
			}
			catch(...)
			{
			}

			try
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX SALE_END_TIME_INDEX ON TURNAROUNDTIMES SALE_END_TIME;";
			   IBQuery1->ExecQuery();
			}
			catch(...)
			{
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DISCOUNTS SET ROUNDING = 0.01;";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_3()
{
   AnsiString UpdateFromVersion = "4.2";
   AnsiString UpdateVersion = "4.3";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "alter table contacts add Member_Number VARCHAR(10) CHARACTER SET ISO8859_1 NOT NULL COLLATE EN_UK";
			IBQuery1->ExecQuery();

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "CONTACT_INDEX";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "DROP INDEX CONTACT_INDEX;";
			   IBQuery1->ExecQuery();
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTS SET MEMBER_NUMBER = CONTACTS_KEY WHERE CONTACT_TYPE != 4";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTS SET MEMBER_NUMBER = CONTACTS_3RDPARTY_KEY WHERE CONTACT_TYPE = 4";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD SITE_ID INTEGER NOT NULL;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD CARD_CREATION_DATE TIMESTAMP;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD KNOWN_AS VARCHAR(50);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD LAST_MODIFIED TIMESTAMP;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD NOTE VARCHAR(50);";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTS SET SITE_ID = -1";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE UNIQUE INDEX CONTACT_INDEX ON CONTACTS(NAME,CONTACTS_3RDPARTY_KEY,SITE_ID,MEMBER_NUMBER)";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {

	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_4()
{
   AnsiString UpdateFromVersion = "4.3";
   AnsiString UpdateVersion = "4.4";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			// Account

			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE TIMECLOCKLOCATIONS ("
				" TIMECLOCKLOCATIONS_KEY INTEGER NOT NULL PRIMARY KEY," " DISPLAY_ORDER INTEGER,"
				" NAME VARCHAR(25)," " CODE INTEGER," " RATE NUMERIC(15,4), DATE_CREATED TIMESTAMP, STATUS CHAR(1) DEFAULT 'T' NOT NULL );";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE AUTO_DISCOUNTS ("
				" AUTO_DISCOUNTS_KEY INTEGER NOT NULL PRIMARY KEY,"
				" CONTACTS_KEY INTEGER NOT NULL," " DISCOUNT_KEY INTEGER NOT NULL);";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();

			// Invoice Bill Pay Table
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_TIMECLOCKLOCATIONS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE AUTO_DISCOUNTS ADD FOREIGN KEY (CONTACTS_KEY) REFERENCES CONTACTS (CONTACTS_KEY) ON UPDATE CASCADE ON DELETE CASCADE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE AUTO_DISCOUNTS ADD FOREIGN KEY (DISCOUNT_KEY) REFERENCES DISCOUNTS (DISCOUNT_KEY) ON UPDATE CASCADE ON DELETE CASCADE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD CANCEL_STOCK_TYPE INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTTIME ADD NAME VARCHAR(25)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTTIME ALTER ZONE TO CODE;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery1->ParamByName("RDB$INDEX_NAME")->AsString = "DAYARCBILL_SECURITY_REF_INDEX";
			IBQuery1->ExecQuery();
			if (IBQuery1->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX DAYARCBILL_SECURITY_REF_INDEX ON DAYARCBILL(SECURITY_REF)";
			   IBQuery1->ExecQuery();
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ORDERS SET CANCEL_STOCK_TYPE = 0";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTTIME SET CODE = 5 WHERE CODE = 4";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTTIME SET CODE = 4 WHERE CODE = 3";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"UPDATE CONTACTTIME SET NAME = (SELECT NAME FROM TIMECLOCKLOCATIONS WHERE TIMECLOCKLOCATIONS.CODE = CONTACTTIME.CODE)";
			IBQuery1->ExecQuery();

			std::auto_ptr <TManagerTimeClock> ManagerTimeClock(new TManagerTimeClock);
			ManagerTimeClock->SetDefaultDept(DBTransaction);

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_5()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.4";
   AnsiString UpdateVersion = "4.5";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			// Account

			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTTIME ADD TIMECLOCKLOCATIONS_KEY INTEGER NOT NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DISCOUNTS ADD PROPERTIES NUMERIC(15, 4)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DISCOUNTS ADD PRIORITY INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE DISCOUNTSTIME ("
				" DISCOUNTSTIME_KEY INTEGER NOT NULL PRIMARY KEY," " DISCOUNT_KEY INTEGER NOT NULL,"
				" TIME_STAMP_START TIMESTAMP," " TIME_STAMP_END TIMESTAMP," " PRIORITY_OVERRIDE INTEGER,"
				" PROPERTIES INTEGER);"; // E.G AUTOMATICALLY APPLY
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_DISCOUNTSTIME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE DISCOUNTSTIME ADD FOREIGN KEY (DISCOUNT_KEY) REFERENCES DISCOUNTS (DISCOUNT_KEY) ON UPDATE CASCADE ON DELETE CASCADE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE AUTO_DISCOUNTS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE CONTACTDISCOUNTS ("
				" CONTACTDISCOUNTS_KEY INTEGER NOT NULL PRIMARY KEY,"
				" CONTACTS_KEY INTEGER NOT NULL," " DISCOUNT_KEY INTEGER NOT NULL);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_CONTACTDISCOUNTS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"UPDATE CONTACTTIME SET CODE = (select min(CODE) FROM TIMECLOCKLOCATIONS) WHERE CODE NOT IN (select CODE FROM TIMECLOCKLOCATIONS);";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DB6Disconnect();
			DB1Connect();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery1->ParamByName("RDB$INDEX_NAME")->AsString = "DAYARCBILL_SECURITY_REF_INDEX";
			IBQuery1->ExecQuery();
			if (IBQuery1->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX DAYARCBILL_SECURITY_REF_INDEX ON DAYARCBILL(SECURITY_REF)";
			   IBQuery1->ExecQuery();
			}

			DBTransaction.Commit();
			DB6Disconnect();
			DB1Connect();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE CONTACTDISCOUNTS ADD FOREIGN KEY (CONTACTS_KEY) REFERENCES CONTACTS (CONTACTS_KEY) ON UPDATE CASCADE ON DELETE CASCADE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE CONTACTDISCOUNTS ADD FOREIGN KEY (DISCOUNT_KEY) REFERENCES DISCOUNTS (DISCOUNT_KEY) ON UPDATE CASCADE ON DELETE CASCADE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"UPDATE CONTACTTIME SET TIMECLOCKLOCATIONS_KEY = (SELECT TIMECLOCKLOCATIONS_KEY FROM TIMECLOCKLOCATIONS WHERE TIMECLOCKLOCATIONS.CODE = CONTACTTIME.CODE)";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE CONTACTTIME ADD FOREIGN KEY (TIMECLOCKLOCATIONS_KEY) REFERENCES TIMECLOCKLOCATIONS (TIMECLOCKLOCATIONS_KEY) ON UPDATE CASCADE ON DELETE SET NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTTIME DROP NAME;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTTIME DROP CODE;";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_6()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.5";
   AnsiString UpdateVersion = "4.6";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			// Account

			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE SMARTCARDS ("
				" SMARTCARDS_KEY INTEGER NOT NULL PRIMARY KEY," " CONTACTS_KEY INTEGER NOT NULL,"
				" CARD_STATUS INTEGER," " BACKUP_TIME_STAMP TIMESTAMP," " CARD_CREATION_DATE TIMESTAMP,"
				" BACKUP BLOB SUB_TYPE 0," " ATR VARCHAR(74));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_SMARTCARDS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_SMARTCARD_NUMBER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE SMARTCARDS ADD FOREIGN KEY (CONTACTS_KEY) REFERENCES CONTACTS (CONTACTS_KEY) ON UPDATE CASCADE ON DELETE SET NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTS SET MEMBER_NUMBER = 0 WHERE (CONTACT_TYPE = 0 OR CONTACT_TYPE = 1)";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_7()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.6";
   AnsiString UpdateVersion = "4.7";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			// Account

			DBTransaction.StartTransaction();

			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "CONTACT_INDEX";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "DROP INDEX CONTACT_INDEX;";
			   IBQuery1->ExecQuery();
			}

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE POINTSTRANSACTIONS ("
				" POINTSTRANSACTIONS_KEY INTEGER NOT NULL PRIMARY KEY," " CONTACTS_KEY INTEGER NOT NULL,"
				" TIME_STAMP TIMESTAMP," " ADJUSTMENT_TYPE INTEGER NOT NULL,"
				" ADJUSTMENT_SUBTYPE INTEGER NOT NULL," " ADJUSTMENT NUMERIC(15,4)," " EXPORTED_STATUS INTEGER NOT NULL,"
				" TIME_STAMP_EXPORTED TIMESTAMP," " MISC BLOB SUB_TYPE 0," " INVOICE_NUMBER VARCHAR(50));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_POINTSTRANSACTIONS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE POINTSTRANSACTIONS ADD FOREIGN KEY (CONTACTS_KEY) REFERENCES CONTACTS (CONTACTS_KEY) ON UPDATE CASCADE ON DELETE SET NULL";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE UNIQUE INDEX CONTACT_INDEX ON CONTACTS(NAME,CONTACTS_3RDPARTY_KEY,SITE_ID,MEMBER_NUMBER)";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_8()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.7";
   AnsiString UpdateVersion = "4.8";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			// Account

			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE SIZES ADD WEIGHTED_SIZE T_TRUEFALSE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD TARE_WEIGHT NUMERIC(15, 4)";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE SIZES SET WEIGHTED_SIZE = 'F'";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ITEMSIZE SET TARE_WEIGHT = 0";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_9()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.8";
   AnsiString UpdateVersion = "4.9";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			// Account

			DBTransaction.StartTransaction();

			// AA Rewards storage.
			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE REWARDS (" " REWARDS_KEY INTEGER NOT NULL PRIMARY KEY,"
				" ARCBILL_KEY INTEGER NOT NULL," " CARD VARCHAR(50)," " REFERENCE VARCHAR(50),"
				" AMOUNT NUMERIC(15,4));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_REWARDS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES ADD DEST_IP VARCHAR(16);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES ADD DEST_PORT INTEGER;";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_10()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.9";
   AnsiString UpdateVersion = "4.10";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			// Account

			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE SYNDCODES (" " SYNDCODES_KEY INTEGER NOT NULL PRIMARY KEY,"
				" NAME VARCHAR(25) NOT NULL," " SYNDCODE VARCHAR(50)," " ENCRYPT_CODE T_TRUEFALSE,"
				" ENABLED T_TRUEFALSE," " VALID_FROM TIMESTAMP," " VALID_TO TIMESTAMP," " REPLACEMENTCODE VARCHAR(50));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE ORDERDISCOUNTS " "( "
				"ORDERDISCOUNTS_KEY INTEGER NOT NULL PRIMARY KEY, " "ORDER_KEY INTEGER NOT NULL, "
				"DISCOUNT_KEY INTEGER, " "NAME VARCHAR(15), " "DESCRIPTION VARCHAR(128), " "MENU VARCHAR(15), "
				"PERCENTAGE NUMERIC(15, 4), " "AMOUNT NUMERIC(15, 4), " "DISCOUNTED_VALUE NUMERIC(15, 4), " "DISCOUNT_TYPE	INTEGER, "
				"DISCOUNT_MODE	INTEGER, " "SOURCE INTEGER, " "ROUNDING NUMERIC(15, 4), " "CATEGORY_KEY INTEGER, "
				"PROPERTIES NUMERIC(15, 4), " "PRIORITY INTEGER, " "DISCOUNT_ID INTEGER," "APPEARANCE_ORDER	INTEGER " ");";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_SYNDCODES;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_ORDERDISCOUNTS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DISCOUNTS ADD DISCOUNT_ID INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DISCOUNTS ADD APPEARANCE_ORDER INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ORDERDISCOUNTS ADD FOREIGN KEY (ORDER_KEY) REFERENCES ORDERS (ORDER_KEY) ON UPDATE CASCADE ON DELETE CASCADE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD POINTS_RULES INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE SMARTCARDS ADD SYNDCODES_KEY INTEGER";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE SMARTCARDS ADD FOREIGN KEY (SYNDCODES_KEY) REFERENCES SYNDCODES (SYNDCODES_KEY) ON UPDATE CASCADE ON DELETE SET NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE ARCORDERDISCOUNTS " "( "
				"ARCORDERDISCOUNTS_KEY INTEGER NOT NULL PRIMARY KEY, " "ARCHIVE_KEY INTEGER NOT NULL, "
				"DISCOUNT_KEY INTEGER, " "NAME VARCHAR(15), " "DESCRIPTION VARCHAR(128), " "MENU VARCHAR(15), "
				"PERCENTAGE NUMERIC(15, 4), " "AMOUNT NUMERIC(15, 4), " "DISCOUNTED_VALUE NUMERIC(15, 4), " "DISCOUNT_TYPE	INTEGER, "
				"DISCOUNT_MODE	INTEGER, " "SOURCE INTEGER, " "ROUNDING NUMERIC(15, 4), " "CATEGORY_KEY INTEGER, "
				"PROPERTIES NUMERIC(15, 4), " "PRIORITY INTEGER, " "DISCOUNT_ID INTEGER," "APPEARANCE_ORDER	INTEGER " ");";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_ARCORDERDISCOUNTS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE ARCORDERDISCOUNTS ADD FOREIGN KEY (ARCHIVE_KEY) REFERENCES ARCHIVE (ARCHIVE_KEY) ON UPDATE CASCADE ON DELETE CASCADE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE DAYARCORDERDISCOUNTS "
				"( " "ARCORDERDISCOUNTS_KEY INTEGER NOT NULL PRIMARY KEY, "
				"ARCHIVE_KEY INTEGER NOT NULL, " "DISCOUNT_KEY INTEGER, " "NAME VARCHAR(15), "
				"DESCRIPTION VARCHAR(128), " "MENU VARCHAR(15), " "PERCENTAGE NUMERIC(15, 4), " "AMOUNT NUMERIC(15, 4), "
				"DISCOUNTED_VALUE NUMERIC(15, 4), " "DISCOUNT_TYPE	INTEGER, " "DISCOUNT_MODE	INTEGER, " "SOURCE INTEGER, "
				"ROUNDING NUMERIC(15, 4), " "CATEGORY_KEY INTEGER, " "PROPERTIES NUMERIC(15, 4), " "PRIORITY INTEGER, "
				"DISCOUNT_ID INTEGER," "APPEARANCE_ORDER	INTEGER " ");";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_DAYARCORDERDISCOUNTS;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE DAYARCORDERDISCOUNTS ADD FOREIGN KEY (ARCHIVE_KEY) REFERENCES DAYARCHIVE (ARCHIVE_KEY) ON UPDATE CASCADE ON DELETE CASCADE";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DB6Disconnect();
			DB1Connect();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DISCOUNTS SET APPEARANCE_ORDER = DISCOUNT_KEY,PRIORITY = DISCOUNT_KEY ";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_11()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.10";
   AnsiString UpdateVersion = "4.11";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			// Account

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE INVOICES ADD EXPORTED_TIME_STAMP TIMESTAMP";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD ACCOUNT_NAME VARCHAR(50)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD ACCOUNT_ID VARCHAR(25)";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_12()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.11";
   AnsiString UpdateVersion = "4.12";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE RECEIPTPRINTING (" " RECEIPTPRINTING_KEY INTEGER NOT NULL PRIMARY KEY,"
				" PHYSICALPRINTER_KEY INTEGER NOT NULL,"
				" CONSTRAINT RP_PHYSICALPRINTER_KEY FOREIGN KEY (PHYSICALPRINTER_KEY) REFERENCES PHYSICALPRINTER (PHYSICALPRINTER_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " DISPLAY_ORDER INTEGER," " INSTRUCTION INTEGER," " CAPTION VARCHAR(45)," " LINE_ABOVE T_TRUEFALSE," " LINE_BELOW T_TRUEFALSE," " DRAW_LINE_ABOVE T_TRUEFALSE," " DRAW_LINE_BELOW T_TRUEFALSE," " CUT T_TRUEFALSE," " VISIBLE T_TRUEFALSE," " PRINT_UNDERLINED T_TRUEFALSE," " PRINT_BOLD T_TRUEFALSE," " PRINT_DOUBLE_WIDTH T_TRUEFALSE," " PRINT_DOUBLE_HEIGHT T_TRUEFALSE," " PRINT_COLOUR INTEGER," " PRINT_FONT INTEGER);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_RECEIPTPRINTING;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE CHITNUMBER (" " CHITNUMBER_KEY INTEGER NOT NULL PRIMARY KEY," " NAME VARCHAR(45),"
				" MASK VARCHAR(50)," " CHITTYPE INTEGER," " FORMAT INTEGER," " APPEARANCE_ORDER INTEGER,"
				" IS_DEFAULT T_TRUEFALSE," " SELECTION_TRIGGER INTEGER," " SELECTION_NAME VARCHAR(50));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_CHITNUMBER_KEY";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD ITEM_TYPE INTEGER;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "alter table RECEIPTPRINTING add GROUP_NUMBER Integer;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "alter table KITCHENPRINTING add GROUP_NUMBER Integer;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE TABLE BLINDBALANCE (" " BLINDBALANCE_KEY INTEGER NOT NULL PRIMARY KEY,"
				" Z_KEY INTEGER NOT NULL,"
				" CONSTRAINT BB_Z_KEY FOREIGN KEY (Z_KEY) REFERENCES ZEDS (Z_KEY) ON UPDATE CASCADE ON DELETE CASCADE," " PAYMENT VARCHAR(30)," " PAYMENT_GROUP INTEGER," " PAYMENT_TRANS_QTY INTEGER," " BLIND_BALANCE NUMERIC(15,4)," " SYSTEM_BALANCE NUMERIC(15,4)," " OFFICE_BALANCE NUMERIC(15,4));";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_BLINDBALANCE_KEY";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_13()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.12";
   AnsiString UpdateVersion = "4.13";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text = "alter table PAYMENTTYPES add PRE_VOUCHER_CODE Integer;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "alter table PAYMENTTYPES add VOUCHER_MERCHANT_ID Integer;";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCREF ALTER REFERENCE TYPE VARCHAR(40)";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ARCREF ALTER REFERENCE TYPE VARCHAR(40)";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_4_14()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.13";
   AnsiString UpdateVersion = "4.14";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 IBQuery->Close();

		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			Log("Updating Database To Version Number " + UpdateVersion);

			IBQuery1->Close();
			IBQuery1->SQL->Text = "alter table PAYMENTTYPES alter PRE_VOUCHER_CODE TYPE VARCHAR(42);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "alter table PAYMENTTYPES alter VOUCHER_MERCHANT_ID TYPE VARCHAR(42);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD LAST_VISIT TIMESTAMP;";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DB6Disconnect();
			DB1Connect();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE CONTACTS SET LAST_VISIT = :LAST_VISIT;";
			IBQuery1->ParamByName("LAST_VISIT")->AsDateTime = 0;
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery->Close();
			IBQuery->SQL->Text =
				"select max(time_stamp) LAST_VISIT,loyalty_key from ARCHIVE " "where LOYALTY_KEY != 0 and LOYALTY_KEY is not null " "group by LOYALTY_KEY";
			IBQuery->ExecQuery();
			if (!IBQuery->RecordCount == 0)
			{
			   for (; !IBQuery->Eof; IBQuery->Next())
			   {
				  IBQuery1->Close();
				  IBQuery1->SQL->Text = "UPDATE CONTACTS SET LAST_VISIT = :LAST_VISIT WHERE CONTACTS_KEY = :CONTACTS_KEY";
				  IBQuery1->ParamByName("LAST_VISIT")->AsDateTime = IBQuery->FieldByName("LAST_VISIT")->AsDateTime;
				  IBQuery1->ParamByName("CONTACTS_KEY")->AsString = IBQuery->FieldByName("LOYALTY_KEY")->AsString;
				  IBQuery1->ExecQuery();
			   }
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_5_0()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "4.14";
   AnsiString UpdateVersion = "5.0";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 Log("Updating Database To Version Number " + UpdateVersion);

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			// Membership & Staff log in speed improvments.
			IBQuery->Close();
			IBQuery->SQL->Text = "select * from RDB$INDICES where RDB$INDEX_NAME = :RDB$INDEX_NAME";
			IBQuery->ParamByName("RDB$INDEX_NAME")->AsString = "CONTACT_PROX_INDEX";
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ADD PROX_CARD1 VARCHAR(128)";
			   IBQuery1->ExecQuery();

			   DBTransaction.Commit();
			   DBTransaction.StartTransaction();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "UPDATE CONTACTS SET PROX_CARD1 = PROX_CARD WHERE PROX_CARD is not null and PROX_CARD != '';";
			   IBQuery1->ExecQuery();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ALTER PROX_CARD TO PROX_CARD_OLD";
			   IBQuery1->ExecQuery();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "ALTER TABLE CONTACTS ALTER PROX_CARD1 TO PROX_CARD";
			   IBQuery1->ExecQuery();

			   DBTransaction.Commit();
			   DBTransaction.StartTransaction();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX CONTACT_PROX_INDEX ON CONTACTS(PROX_CARD)";
			   IBQuery1->ExecQuery();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX CONTACT_TYPE_INDEX ON CONTACTS(CONTACT_TYPE)";
			   IBQuery1->ExecQuery();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX CONTACTCARDS_CARD_INDEX ON CONTACTCARDS(SWIPE_CARD)";
			   IBQuery1->ExecQuery();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX CONTACTCARDS_CONTACTKEY_INDEX ON CONTACTCARDS(CONTACTS_KEY)";
			   IBQuery1->ExecQuery();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "ALTER TABLE BLINDBALANCE ALTER PAYMENT TYPE VARCHAR(50)";
			   IBQuery1->ExecQuery();

			   DBTransaction.Commit();
			   DBTransaction.StartTransaction();

			   /* IBQuery1->Close();
			   IBQuery1->SQL->Text = "ALTER TABLE MEMBERS ADD SWIPE_CARD1 VARCHAR(128)";
			   IBQuery1->ExecQuery();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "UPDATE MEMBERS SET SWIPE_CARD1 = SWIPE_CARD";
			   IBQuery1->ExecQuery();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "ALTER TABLE MEMBERS ALTER SWIPE_CARD TO SWIPE_CARD_OLD";
			   IBQuery1->ExecQuery();

			   IBQuery1->Close();
			   IBQuery1->SQL->Text = "CREATE INDEX MEMBERS_SWIPECARD ON MEMBERS(SWIPE_CARD)";
			   IBQuery1->ExecQuery(); */
			}

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ORDERS SET PRICE = PRICE + PRICE_ADJUST";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ORDERS SET PRICE_ADJUST = 0";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCHIVE SET PRICE_LEVEL0 = PRICE WHERE PRICE_LEVEL0 IS NULL";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCHIVE SET PRICE = PRICE + PRICE_ADJUST";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE ARCHIVE SET PRICE_ADJUST = 0";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DAYARCHIVE SET PRICE = PRICE + PRICE_ADJUST";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "UPDATE DAYARCHIVE SET PRICE_ADJUST = 0";
			IBQuery1->ExecQuery();

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY (" " VERSION_NUMBER," " TIME_STAMP," " COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void TfrmMain::UpdateVersion_5_1()
{
   DB6Disconnect();
   DB1Connect();

   AnsiString UpdateFromVersion = "5.0";
   AnsiString UpdateVersion = "5.1";
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
	  TIBSQL *IBQuery3 = DBTransaction.Query(DBTransaction.AddQuery());
	  DBTransaction.StartTransaction();

	  IBQuery->Close();

	  IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
	  IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
	  IBQuery->ExecQuery();
	  if (IBQuery->RecordCount == 0)
	  {
		 Log("Version Number " + UpdateFromVersion + " Required");
	  }
	  else
	  {
		 Log("Updating Database To Version Number " + UpdateVersion);

		 IBQuery->Close();
		 IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		 IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
		 IBQuery->ExecQuery();
		 if (IBQuery->RecordCount == 0)
		 {
			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE TABLE DEVICECHITS ( DEVICECHITS_KEY INTEGER NOT NULL PRIMARY KEY, "
                							"DEVICE_KEY INTEGER, "
                                            "CHITNUMBER_KEY INTEGER, "
         									"DEFAULT_CHIT INTEGER);";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE DEVICECHITS ADD FOREIGN KEY (DEVICE_KEY) REFERENCES DEVICES (DEVICE_KEY) ON UPDATE CASCADE ON DELETE CASCADE";
			IBQuery1->ExecQuery();

			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"ALTER TABLE DEVICECHITS ADD FOREIGN KEY (CHITNUMBER_KEY) REFERENCES CHITNUMBER (CHITNUMBER_KEY) ON UPDATE CASCADE ON DELETE CASCADE";
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			IBQuery1->Close();
			IBQuery1->SQL->Text = "CREATE GENERATOR GEN_DEVICECHITS;";
			IBQuery1->ExecQuery();
			DBTransaction.Commit();

			DBTransaction.Commit();
			DBTransaction.StartTransaction();

			// Hook every device to every chit to maintain backwards compatablility.
			IBQuery1->Close();
			IBQuery1->SQL->Text = "SELECT DEVICE_KEY FROM DEVICES ";
			IBQuery1->ExecQuery();

			IBQuery2->Close();
			IBQuery2->SQL->Text = "SELECT CHITNUMBER_KEY FROM CHITNUMBER ";
			IBQuery2->ExecQuery();

			for (; !IBQuery1->Eof; IBQuery1->Next())
			{
			   for (; !IBQuery2->Eof; IBQuery2->Next())
			   {
				  IBQuery3->Close();
				  IBQuery3->SQL->Text = "INSERT INTO DEVICECHITS (DEVICECHITS_KEY, CHITNUMBER_KEY, DEVICE_KEY) VALUES ("
					  "(SELECT GEN_ID(GEN_DEVICECHITS, 1) FROM RDB$DATABASE), :CHITNUMBER_KEY, :DEVICE_KEY);";
				  IBQuery3->ParamByName("CHITNUMBER_KEY")->AsInteger = IBQuery2->FieldByName("CHITNUMBER_KEY")->AsInteger;
				  IBQuery3->ParamByName("DEVICE_KEY")->AsInteger = IBQuery1->FieldByName("DEVICE_KEY")->AsInteger;
				  IBQuery3->ExecQuery();
			   }
			}

			Sleep(1000);
			IBQuery1->Close();
			IBQuery1->ParamCheck = true;
			IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY ( VERSION_NUMBER, TIME_STAMP, COMMENT)"
				" VALUES ( :VersionInfo,:DateTime,'Upgrade');";
			IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
			IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
			IBQuery1->ExecQuery();

			DBTransaction.Commit();
			Log("Update " + UpdateVersion + " Successfull");
			// Applying alterations here for speed.
		 }
		 else
		 {
			Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
		 }
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
   }
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_2()
{
	AnsiString UpdateFromVersion = "5.1";
	AnsiString UpdateVersion = "5.2";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
         Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_2::Up_5_2(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
     DB_5_2::Down_5_2(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_3()
{
	AnsiString UpdateFromVersion = "5.2";
	AnsiString UpdateVersion = "5.3";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
         Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_3::Up_5_3(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
	 DB_5_3::Down_5_3(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_4()
{
	AnsiString UpdateFromVersion = "5.3";
	AnsiString UpdateVersion = "5.4";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
         Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_4::Up_5_4(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
	  DB_5_4::Down_5_4(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_5()
{
	AnsiString UpdateFromVersion = "5.4";
	AnsiString UpdateVersion = "5.5";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
         Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_5::Up_5_5(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
	  DB_5_5::Down_5_5(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_6()
{
	AnsiString UpdateFromVersion = "5.5";
	AnsiString UpdateVersion = "5.6";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
			Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_6::Up_5_6(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
	  DB_5_6::Down_5_6(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_7()
{
	AnsiString UpdateFromVersion = "5.6";
	AnsiString UpdateVersion = "5.7";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
			Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_7::Up_5_7(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
	  DB_5_7::Down_5_7(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact IQWorks with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_8()
{
	AnsiString UpdateFromVersion = "5.7%";
	AnsiString UpdateVersion = "5.8";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
			Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_8::Up_5_8(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
	  DB_5_8::Down_5_8(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_9()
{
	AnsiString UpdateFromVersion = "5.8";
	AnsiString UpdateVersion = "5.9";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
			Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_9::Up_5_9(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
	  DB_5_9::Down_5_9(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_10()
{
	AnsiString UpdateFromVersion = "5.9";
	AnsiString UpdateVersion = "5.10";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
			Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_10::Up_5_10(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
	  DB_5_10::Down_5_10(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_11()
{
	AnsiString UpdateFromVersion = "5.10";
	AnsiString UpdateVersion = "5.11";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
			Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_11::Up_5_11(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
	  DB_5_11::Down_5_11(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
void  TfrmMain::UpdateVersion_5_12()
{
	AnsiString UpdateFromVersion = "5.11";
	AnsiString UpdateVersion = "5.12";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
			Log("Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{
				DBTransaction.Commit();

				Log("Updating Database To Version Number " + UpdateVersion);
				DB_5_12::Up_5_12(DBControl);
				Log("Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}
			else
			{
				Log("Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}
	catch(Exception & E)
	{
	  DB_5_12::Down_5_12(DBControl);
	  ShowMessage("The upgrade failed. Please restore from backup and contact Posabl with the following error message : " + E.Message);
	  Log(E.Message);
	}
}
// ---------------------------------------------------------------------------
bool
TfrmMain::HasBeenUpgradedTo(const UnicodeString &version)
{
    Database::TDBTransaction t(DBControl);
    TIBSQL *q = t.Query(t.AddQuery());
    q->SQL->Text = "select version_number "
                   "       from versionhistory "
                   "       where version_number = :version_number;";
    t.StartTransaction();
    q->ParamByName("version_number")->AsString = version;
    try {
        const bool present = (q->ExecQuery(), q->RecordCount != 0);
        t.Commit();
        return present;
    } catch (...) {
        t.Rollback();
        throw;
    }
}
// ---------------------------------------------------------------------------
void
TfrmMain::UpdateVersion_5_13_0_3()
{
    if (HasBeenUpgradedTo("5.13.0.3")) {
        Log("Version Number 5.13.0.3 Updates Already Applied Successfully");
    } else {
        Database::TDBTransaction t(DBControl);
        TIBSQL *q = t.Query(t.AddQuery());

        Log("Updating Database To Version Number 5.13.0.3 ...");
        DB_5_13_0_3_Hotfix::Apply(DBControl);

        t.StartTransaction();
            q->SQL->Text =
              "insert into versionhistory "
              "       values ('5.13.0.3', current_timestamp, 'Upgrade');";
            q->ExecQuery();
        t.Commit();
    }
}
// ---------------------------------------------------------------------------
void
TfrmMain::UpdateVersion_5_13()
{
	if (HasBeenUpgradedTo("5.13")) {
		Log("Version Number 5.13 Updates Already Applied Successfully");
	} else {
		Database::TDBTransaction t(DBControl);
		TIBSQL *q = t.Query(t.AddQuery());

		Log("Updating Database To Version Number 5.13 ...");
		DB_5_12_0_0_ChitList::Up(DBControl);

		t.StartTransaction();
			q->SQL->Text =
			  "insert into versionhistory "
			  "       values ('5.13', current_timestamp, 'Upgrade');";
			q->ExecQuery();
		t.Commit();
	}
}
// ---------------------------------------------------------------------------
void
TfrmMain::UpdateVersion_5_14()
{
    if (HasBeenUpgradedTo("5.14")) {
        Log("Version Number 5.14 Updates Already Applied Successfully");
    } else {
        Database::TDBTransaction t(DBControl);
        TIBSQL *q = t.Query(t.AddQuery());

        Log("Updating Database To Version Number 5.14 ...");
        q->SQL->Text = "insert into versionhistory "
                       "       values ('5.14', current_timestamp, 'Upgrade');";
        t.StartTransaction();
            q->ExecQuery();
        t.Commit();
    }
}
// ---------------------------------------------------------------------------
void
TfrmMain::UpdateVersion_5_15()
{
	if (HasBeenUpgradedTo("5.15")) {
		Log("Version Number 5.15 Updates Already Applied Successfully");
	} else if (!HasBeenUpgradedTo("5.14")) {
		Log("Version Number 5.14 required to install 5.15 update.");
	} else {
		Database::TDBTransaction t(DBControl);
		TIBSQL *q = t.Query(t.AddQuery());

		Log("Updating Database To Version Number 5.15 ...");
		DB_5_15::Apply(DBControl);

		q->SQL->Text = "insert into versionhistory "
					   "       values ('5.15', current_timestamp, 'Upgrade');";
		t.StartTransaction();
			q->ExecQuery();
		t.Commit();
	}
}
// ---------------------------------------------------------------------------
void
TfrmMain::UpdateVersion_5_16()
{
	if (HasBeenUpgradedTo("5.16")) {
		Log("Version Number 5.16 Updates Already Applied Successfully");
	} else if (!HasBeenUpgradedTo("5.15")) {
		Log("Version Number 5.15 required to install 5.16 update.");
	} else {
		Database::TDBTransaction t(DBControl);
		TIBSQL *q = t.Query(t.AddQuery());

		Log("Updating Database To Version Number 5.16 ...");
		DB_5_16::Apply(DBControl);

		q->SQL->Text = "insert into versionhistory "
					   "       values ('5.16', current_timestamp, 'Upgrade');";
		t.StartTransaction();
			q->ExecQuery();
		t.Commit();
	}
}
// ---------------------------------------------------------------------------
void
TfrmMain::UpdateVersion_5_17()
{
	if (HasBeenUpgradedTo("5.17")) {
		Log("Version Number 5.17 Updates Already Applied Successfully");
	} else if (!HasBeenUpgradedTo("5.16")) {
		Log("Version Number 5.16 required to install 5.16 update.");
	} else {
		Database::TDBTransaction t(DBControl);
		TIBSQL *q = t.Query(t.AddQuery());

		Log("Updating Database To Version Number 5.17 ...");
		DB_5_17::Apply(DBControl);

		q->SQL->Text = "insert into versionhistory "
					   "       values ('5.17', current_timestamp, 'Upgrade');";
		t.StartTransaction();
			q->ExecQuery();
		t.Commit();
	}
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// NEW PARSER FROM VERSION 5.18.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void onParserInProgress(
		std::string inVersion,
		std::string inMsg );
void onParserSucceed(
		std::string inVersion,
		std::string inMsg );
void onParserFailed(
		std::string inVersion,
		std::string inErrorMsg );

void TfrmMain::UpdateVersionNew()
{
	TVersionParser::Instance()->SetDBControl( &DBControl );
	TVersionParser::Instance()->SetParser(    "posabl" );

    TVersionParser::Instance()->SetVersionParserInProgressEvent( ( VERSION_PARSER_EVENT )onParserInProgress );
	TVersionParser::Instance()->SetVersionParserSucceedEvent(    ( VERSION_PARSER_EVENT )onParserSucceed    );
	TVersionParser::Instance()->SetVersionParserFailedEvent(     ( VERSION_PARSER_EVENT )onParserFailed     );

	PARSER_ERROR error = TVersionParser::Instance()->Execute();
}
// ---------------------------------------------------------------------------
void onParserInProgress(
		std::string inVersion,
		std::string inMsg )
{
	frmMain->Log( "Version Number " + UnicodeString( inVersion.c_str() ) + " " +
                  UnicodeString( inMsg.c_str() ) );

    Application->ProcessMessages();
}
// ---------------------------------------------------------------------------
void onParserSucceed(
		std::string inVersion,
		std::string inMsg )
{
    frmMain->StepProgressBar( 1 );

	frmMain->Log( "Version Number " + UnicodeString( inVersion.c_str() ) + " " +
                  UnicodeString( inMsg.c_str() ) );

    Application->ProcessMessages();
}
// ---------------------------------------------------------------------------
void onParserFailed(
		std::string inVersion,
		std::string inErrorMsg )
{
    frmMain->StepProgressBar( 1 );

	frmMain->Log( "Version Number " + UnicodeString( inVersion.c_str()  ) + ". Updates FAILED: " +
							          UnicodeString( inErrorMsg.c_str() ) );

    Application->ProcessMessages();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmMain::lvLogCustomDrawItem(TCustomListView *Sender, TListItem *Item,
          TCustomDrawState State, bool &DefaultDraw)
{
    Sender->Canvas->Font->Color = 0xFFAA00;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::lvLogCustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
          int SubItem, TCustomDrawState State, bool &DefaultDraw)
{
    Sender->Canvas->Font->Color = clTeal;
}
//---------------------------------------------------------------------------

void TfrmMain::SetProgressLabel( UnicodeString inText )
{
    lbeProgress->Caption = inText;
}
//---------------------------------------------------------------------------

void TfrmMain::SetHeaderLabel( UnicodeString inText )
{
    lbeHeader->Caption = inText;
}
//---------------------------------------------------------------------------

void TfrmMain::SetDBPathLabel( UnicodeString inText )
{
    lbeDBPath->Caption = inText;
}
//---------------------------------------------------------------------------

void TfrmMain::SetStartTimeLabel(  UnicodeString inText )
{
    lbeStartTime->Caption = inText;
}
//---------------------------------------------------------------------------

void TfrmMain::SetFinishTimeLabel( UnicodeString inText )
{
    lbeFinishTime->Caption = inText;
}
//---------------------------------------------------------------------------

void TfrmMain::SetProgressBarMax( int inMax )
{
    barProgress->Min      = 0;
    barProgress->Max      = inMax;
    barProgress->Position = barProgress->Min;
}
//---------------------------------------------------------------------------

void TfrmMain::StepProgressBar( int inStep )
{
    barProgress->Position += inStep;
}
//---------------------------------------------------------------------------

int TfrmMain::GetVersionCount()
{
    // 57 old upgrades
    return 57 + TVersionParser::Instance()->VersionCount();
}
//---------------------------------------------------------------------------

