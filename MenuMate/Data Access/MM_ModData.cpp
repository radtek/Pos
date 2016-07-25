//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MM_ModData.h"
#include "MMRegistry.h"
#include "enum.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TmodData *modData;
//---------------------------------------------------------------------------
__fastcall TmodData::TmodData(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------

bool TmodData::OpenPrimaryDB()
{
   try
   {
      // Retrive MenuMate DB Settings.
      if(DatabasePath == "" && InterbaseIP == "")
      {
         DatabasePath = "";
         RegistryRead(MMBaseKey "Database","DatabasePath",DatabasePath);

         InterbaseIP = "";
         RegistryRead(MMBaseKey "Database","InterbaseIP",InterbaseIP);
      }

      if(DatabasePath != "" && InterbaseIP != "")
      {
         SQLConnection->Connected		= false;
         SQLConnection->ConnectionName = "IBConnection";
         SQLConnection->Params->Values["Database"] = InterbaseIP + ":" + DatabasePath;
         SQLConnection->Connected		= true;

         if (SQLConnection->Connected)
         {
            return true;
         }
      }
   }
   catch(Exception &E)
   {
   	ShowMessage(E.Message);
   }
   return false;
}

bool TmodData::OpenSecondaryDB()
{
   try
   {
      // Retrive MenuMate DB Settings.
/*      DatabasePath1 = "";
      RegistryRead(MMBaseKey "Database","DatabasePath1",DatabasePath1);

      InterbaseIP1 = "";
      RegistryRead(MMBaseKey "Database","InterbaseIP1",InterbaseIP1);*/
      
      if(DatabasePath1 != "" && InterbaseIP1 != "")
      {
         SQLConnection1->Connected		= false;
         SQLConnection1->ConnectionName = "IBConnection";
         SQLConnection1->Params->Values["Database"] = InterbaseIP1 + ":" + DatabasePath1;
         SQLConnection1->Connected		= true;

         if (SQLConnection->Connected)
         {
            return true;
         }
      }
   }
   catch(Exception &E)
   {
   	ShowMessage(E.Message);
   }
   return false;
}

void TmodData::CloseSecondaryDB()
{
   try
   {
      SQLConnection1->Connected = false;
   }
   catch(Exception &E)
   {
   	ShowMessage(E.Message);
   }
}

void TmodData::ClosePrimaryDB()
{
   try
   {
      SQLConnection->Connected = false;
   }
   catch(Exception &E)
   {
   	ShowMessage(E.Message);
   }
}


void TmodData::MakeDBImage(AnsiString BackupPath,TLabel *Display)
{
   IBCopy->ServerName = InterbaseIP;
   IBCopy->LoginPrompt = false;
   IBCopy->Params->Add("user_name=SYSDBA");
   IBCopy->Params->Add("password=masterkey");
   IBCopy->Active = true;
   try
   {
      IBCopy->Verbose = false;
      TBackupOptions opts;
      opts << IgnoreLimbo << NoGarbageCollection;
      IBCopy->Options = opts;
      IBCopy->Verbose = true;

      IBCopy->DatabaseName = DatabasePath;
      IBCopy->BackupFile->Clear();
      IBCopy->BackupFile->Add(BackupPath);
      IBCopy->ServiceStart();
      while (!(IBCopy->Eof))
      {
         if(Display != NULL)
         {
            Display->Caption = IBCopy->GetNextLine();
            Display->Repaint();
         }
         else
         {
            IBCopy->GetNextLine();
         }
      }
   }
   __finally
   {
      IBCopy->Active = false;
   }

   IBRestore->ServerName = InterbaseIP;
   IBRestore->LoginPrompt = false;
   IBRestore->Params->Add("user_name=SYSDBA");
   IBRestore->Params->Add("password=masterkey");
   IBRestore->Active = true;
   try
   {
      IBRestore->Verbose = false;
      TRestoreOptions opts;
      opts << Replace << UseAllSpace;
      IBRestore->Options = opts;
      IBRestore->PageBuffers = 3000;
      IBRestore->PageSize = 4096;
      IBRestore->Verbose = true;
		IBRestore->DatabaseName->Clear();
      IBRestore->DatabaseName->Add( ExtractFilePath(BackupPath) + "LogMate.gdb" );
      IBRestore->BackupFile->Clear();
      IBRestore->BackupFile->Add( BackupPath);
      IBRestore->ServiceStart();
      while (!(IBRestore->Eof))
      {
         if(Display != NULL)
         {
            Display->Caption = IBRestore->GetNextLine();
            Display->Repaint();            
         }
         else
         {
            IBRestore->GetNextLine();
         }
      }
   }
   __finally
   {
      IBRestore->Active = false;
   }
}

AnsiString TmodData::CompressDB(AnsiString BackupPath,AnsiString PickUpPath)
{
   IBCopy->ServerName = InterbaseIP;
   IBCopy->LoginPrompt = false;
   IBCopy->Params->Add("user_name=SYSDBA");
   IBCopy->Params->Add("password=masterkey");
   IBCopy->Active = true;
   try
   {
      IBCopy->Verbose = false;
      TBackupOptions opts;
      opts << IgnoreLimbo << NoGarbageCollection;
      IBCopy->Options = opts;

      IBCopy->DatabaseName = ExtractFilePath(BackupPath) + "LogMate.gdb";
      IBCopy->BackupFile->Clear();
      IBCopy->BackupFile->Add(ExtractFilePath(BackupPath) + "LogMateToSend.gbk" );
      IBCopy->ServiceStart();
      while (!(IBCopy->Eof))
      {
         IBCopy->GetNextLine();
      }
   }
   __finally
   {
      IBCopy->Active = false;
   }

   AnsiString FileToZip = IncludeTrailingBackslash(ExtractFilePath(PickUpPath)) + "LogMateToSend.gbk";
   AnsiString ZipFile = IncludeTrailingBackslash(ExtractFilePath(PickUpPath)) + "LogMateToSend.zip";
	//   Zip this file here.
	try
   {
   	try
      {
			Zip->DLLDirectory = ExtractFilePath(Application->ExeName);
         Zip->AddOptions = AddOpts() << AddDirNames;
         Zip->TempDir = ExtractFilePath(Application->ExeName);
         Zip->ZipFileName = ZipFile;
         Zip->FSpecArgs->Clear();
         Zip->FSpecArgs->Add(FileToZip);
         Zip->Add();
      }
      catch (Exception &E)
      {
      	Application->ShowException(&E);
   	}
   }
   __finally
   {
   	//delete Zip;
   }
   return ZipFile;
}


void TmodData::FormatDB(TDateTime StartTime, TDateTime EndTime)
{
   if(SQLConnection->Connected == false || SQLConnection1->Connected == false)
   {
   	return;
   }

      TTransactionDesc TD;
      TD.TransactionID = 1;
      TD.IsolationLevel = xilREADCOMMITTED;
      SQLConnection1->StartTransaction(TD);

      SQLQuery1->SQL->Clear();
      SQLQuery1->Params->Clear();
      SQLQuery1->ParamCheck = true;
      SQLQuery1->SQL->Text = "delete from ARCBILL where "
                             " TIME_STAMP < :StartDate"
                             " and TIME_STAMP > :EndDate";
      SQLQuery1->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQuery1->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQuery1->ExecSQL();

      SQLQuery1->SQL->Clear();
      SQLQuery1->Params->Clear();
      SQLQuery1->ParamCheck = true;
      SQLQuery1->SQL->Text = "delete from SECURITY where "
                             " TIME_STAMP < :StartDate"
                             " and TIME_STAMP > :EndDate";
      SQLQuery1->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQuery1->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQuery1->ExecSQL();

      SQLQuery1->SQL->Clear();
      SQLQuery1->Params->Clear();
      SQLQuery1->ParamCheck = true;
      SQLQuery1->SQL->Text = "delete from LOGS where "
                             " TIME_STAMP < :StartDate"
                             " and TIME_STAMP > :EndDate";
      SQLQuery1->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQuery1->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQuery1->ExecSQL();

      SQLConnection1->Commit(TD);      
}
/*
void TmodData::CopyDB(TDateTime StartTime, TDateTime EndTime)
{
   if(SQLConnection->Connected == false || SQLConnection1->Connected == false)
   {
   	return;
   }

      SQLQuery1->SQL->Clear();
      SQLQuery1->Params->Clear();
      SQLQuery1->ParamCheck = true;
      SQLQuery1->SQL->Text = "delete from rdb$relation_constraints where "
      							  "rdb$constraint_type != 'PRIMARY KEY' and "
      							  "rdb$constraint_type != 'NOT NULL'";
      SQLQuery1->ExecSQL();

      SQLQuery1->SQL->Clear();
      SQLQuery1->Params->Clear();
      SQLQuery1->ParamCheck = true;
      SQLQuery1->SQL->Text = "  delete from rdb$relation_fields";
      SQLQuery1->ExecSQL();

      SQLConnection1->Connected = false;
      SQLConnection1->Connected = true;

   // Copy Logs
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from LOGS where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate";
      SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("LOGS",SQLQueryA);

   // Copy Version History
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from VERSIONHISTORY";
      SQLQueryA->Open();
	   CopyTable("VERSIONHISTORY",SQLQueryA);

	// Copy ArcCategories
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ARCCATEGORIES";
      SQLQueryA->Open();
	   CopyTable("ARCCATEGORIES",SQLQueryA);

  	// Copy Categories
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from CATEGORIES";
      SQLQueryA->Open();
	   CopyTable("CATEGORIES",SQLQueryA);


   // Copy Registration.
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from REGISTRATION";
      SQLQueryA->Open();
	   CopyTable("REGISTRATION",SQLQueryA);

   // Copy Payment Types.
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from PAYMENTTYPES";
      SQLQueryA->Open();
	   CopyTable("PAYMENTTYPES",SQLQueryA);

   // Copy Zeds.
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ZEDS where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate";
      SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("ZEDS",SQLQueryA);

   // Copy Companies
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from COMPANIES";
      SQLQueryA->Open();
	   CopyTable("COMPANIES",SQLQueryA);

   // Copy Contacts
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from CONTACTS";
      SQLQueryA->Open();
	   CopyTable("CONTACTS",SQLQueryA);
   	// Copy Contact Rollover.
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from CONTACTROLLOVER";
      SQLQueryA->Open();
	   CopyTable("CONTACTROLLOVER",SQLQueryA);
      // Copy CcontactTime
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from CONTACTTIME";
      SQLQueryA->Open();
	   CopyTable("CONTACTTIME",SQLQueryA);
      // Copy Invoices
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from INVOICES";
      SQLQueryA->Open();
	   CopyTable("INVOICES",SQLQueryA);

  	// Copy Security
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from SECURITY where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate";
      SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("SECURITY",SQLQueryA);

	// Copy Arcbill.
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ARCBILL where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate";
      SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("ARCBILL",SQLQueryA);
      	// Copy Security
   	// ArcbillPay
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ARCBILLPAY where ARCBILL_KEY in "
									  "(select ARCBILL_KEY from ARCBILL where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate)";
   	SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("ARCBILLPAY",SQLQueryA);
      	// Copy Security
      // ArcSurcharge
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ARCSURCHARGE where ARCBILL_KEY in "
									  "(select ARCBILL_KEY from ARCBILL where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate)";
   	SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("ARCSURCHARGE",SQLQueryA);
      	// Copy Security
      // Archive
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ARCHIVE where ARCBILL_KEY in "
									  "(select ARCBILL_KEY from ARCBILL where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate)";
   	SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("ARCHIVE",SQLQueryA);
      	// Copy Security

	// Copy DayArcbill.
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from DAYARCBILL where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate";
      SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("DAYARCBILL",SQLQueryA);
      	// Copy Security
   	// DayArcbillPay
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from DAYARCBILLPAY where ARCBILL_KEY in "
									  "(select ARCBILL_KEY from DAYARCBILL where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate)";
   	SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("DAYARCBILLPAY",SQLQueryA);
      	// Copy Security
      // DayArcSurcharge
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from DAYARCSURCHARGE where ARCBILL_KEY in "
									  "(select ARCBILL_KEY from DAYARCBILL where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate)";
   	SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("DAYARCSURCHARGE",SQLQueryA);
      	// Copy Security
      // DayArchive
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from DAYARCHIVE where ARCBILL_KEY in "
									  "(select ARCBILL_KEY from DAYARCBILL where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate)";
   	SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("DAYARCHIVE",SQLQueryA);
      	// Copy Security

      	// Copy ArcCatergory <<<< YUMMY NESTED SELECTS >>>>
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ARCCATEGORY where ARCHIVE_KEY in"
									  "(select ARCHIVE_KEY from ARCHIVE where ARCBILL_KEY in "
									  "(select ARCBILL_KEY from ARCBILL where "
                             " TIME_STAMP > :StartDate"
                             " and TIME_STAMP < :EndDate))";
   	SQLQueryA->ParamByName("StartDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(StartTime);
      SQLQueryA->ParamByName("EndDate")->AsSQLTimeStamp = DateTimeToSQLTimeStamp(EndTime);
      SQLQueryA->Open();
	   CopyTable("ARCCATEGORY",SQLQueryA);

   // Copy Menus
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from MENU";
      SQLQueryA->Open();
	   CopyTable("MENU",SQLQueryA);
      // Copy Sizes
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from SIZES";
      SQLQueryA->Open();
	   CopyTable("SIZES",SQLQueryA);
   	// Copy Courses
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from COURSE";
      SQLQueryA->Open();
	   CopyTable("COURSE",SQLQueryA);
      	// Items
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ITEM";
      SQLQueryA->Open();
	   CopyTable("ITEM",SQLQueryA);
         	// Items Sizes
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ITEMSIZE";
      SQLQueryA->Open();
	   CopyTable("ITEMSIZE",SQLQueryA);
            	// ItemSizeCategory
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ITEMSIZECATEGORY";
      SQLQueryA->Open();
	   CopyTable("ITEMSIZECATEGORY",SQLQueryA);
               // Receipe
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from RECIPE";
      SQLQueryA->Open();
	   CopyTable("RECIPE",SQLQueryA);

   // Copy Locations
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from LOCATIONS";
      SQLQueryA->Open();
	   CopyTable("LOCATIONS",SQLQueryA);

   // Copy Devices
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from DEVICES";
      SQLQueryA->Open();
	   CopyTable("DEVICES",SQLQueryA);
	  	// Devices Menus
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from DEVICESMENUS";
      SQLQueryA->Open();
	   CopyTable("DEVICESMENUS",SQLQueryA);
   	// Conenctions
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from CONNECTIONS";
      SQLQueryA->Open();
	   CopyTable("CONNECTIONS",SQLQueryA);
      	// PrnOrder
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from PRNORDER";
      SQLQueryA->Open();
	   CopyTable("PRNORDER",SQLQueryA);

   // Copy Tabs
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from TAB";
      SQLQueryA->Open();
	   CopyTable("TAB",SQLQueryA);
   	// Copy Orders
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ORDERS";
      SQLQueryA->Open();
	   CopyTable("ORDERS",SQLQueryA);
      	// Copy Order Cat
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ORDERCATEGORY";
      SQLQueryA->Open();
	   CopyTable("ORDERCATEGORY",SQLQueryA);
			// Copy Order Options
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ORDEROPTION";
      SQLQueryA->Open();
	   CopyTable("ORDEROPTION",SQLQueryA);
         // Copy Order Receipe
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ORDERRECIPE";
      SQLQueryA->Open();
	   CopyTable("ORDERRECIPE",SQLQueryA);
      	// Copy Order Mix
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ORDERMIX";
      SQLQueryA->Open();
	   CopyTable("ORDERMIX",SQLQueryA);
	         // Copy Order Mix Cat
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ORDERMIXCATEGORY";
      SQLQueryA->Open();
	   CopyTable("ORDERMIXCATEGORY",SQLQueryA);

	// Copy Stock Costs
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from STOCKCOSTS";
      SQLQueryA->Open();
	   CopyTable("STOCKCOSTS",SQLQueryA);
   // Copy Stock Track
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from STOCKTRACK";
      SQLQueryA->Open();
	   CopyTable("STOCKTRACK",SQLQueryA);

   // Copy Tables
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from TABLES";
      SQLQueryA->Open();
	   CopyTable("TABLES",SQLQueryA);
   	//Copy Seats
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from SEAT";
      SQLQueryA->Open();
	   CopyTable("SEAT",SQLQueryA);

   // Copy Rooms
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from ROOMS";
      SQLQueryA->Open();
	   CopyTable("ROOMS",SQLQueryA);

   // Copy Turnaround Times.
      SQLQueryA->SQL->Clear();
      SQLQueryA->Params->Clear();
      SQLQueryA->ParamCheck = true;
      SQLQueryA->SQL->Text = "select * from TURNAROUNDTIMES";
      SQLQueryA->Open();
	   CopyTable("TURNAROUNDTIMES",SQLQueryA);

}


void TmodData::CopyTable(AnsiString TableName,TSQLQuery *Query)
{
	SQLQuery1->Params->Clear();
	SQLQuery1->ParamCheck = true;
   AnsiString SQL = "INSERT INTO " + TableName +" (";
   for (int i=0; i < Query->FieldCount -1; i++)
   {
      AnsiString FieldName = Query->Fields->Fields[i]->FieldName;
      AnsiString SetSQL = FieldName + ",";
      SQL += SetSQL;
   }
   AnsiString FieldName = Query->Fields->Fields[Query->FieldCount-1]->FieldName;
   AnsiString SetSQL = FieldName + ") VALUES (";
	SQL += SetSQL;
   for (int i=0; i < Query->FieldCount -1; i++)
   {
      AnsiString FieldName = Query->Fields->Fields[i]->FieldName;
      AnsiString SetSQL = ":" +FieldName + ",";
      SQL += SetSQL;
   }
   FieldName = Query->Fields->Fields[Query->FieldCount-1]->FieldName;
   SetSQL = ":" + FieldName + ");";
	SQL += SetSQL;
   SQLQuery1->SQL->Text = SQL;
   TMemoryStream *BlobStream = new TMemoryStream;
   try
   {
      for (Query->First(); !Query->Eof  ; Query->Next())
      {
         SQLQuery1->Close();
         for (int i=0; i < Query->FieldCount; i++)
         {
            AnsiString FieldName = Query->Fields->Fields[i]->FieldName;
            if(Query->Fields->Fields[i]->DataType == ftBlob	|| Query->Fields->Fields[i]->DataType == ftMemo)
            {
               BlobStream->Clear();
               TBlobField *BlobField = (TBlobField *)Query->FieldByName(FieldName);
		         BlobField->SaveToStream(BlobStream);
         		BlobStream->Position = 0;
			      SQLQuery1->ParamByName(FieldName)->LoadFromStream(BlobStream,ftBlob);
            }
            if(Query->FieldByName(FieldName)->IsNull)
            {
               if(Query->Fields->Fields[i]->DataType == ftTimeStamp)
               {
                  SQLQuery1->ParamByName(FieldName)->AsSQLTimeStamp =
                     Query->FieldByName(FieldName)->AsSQLTimeStamp;
               }
               else if(Query->Fields->Fields[i]->DataType == ftString)
               {
               	if(Query->Fields->Fields[i]->Size == 1)
                  {
                     SQLQuery1->Params->Items[i]->AsString = "F";
                     SQLQuery1->Params->Items[i]->Bound = true;
                  }
                  else
                  {
                     SQLQuery1->Params->Items[i]->AsString = "";
                     SQLQuery1->Params->Items[i]->Bound = true;
                  }
               }
               else
               {
                  SQLQuery1->Params->Items[i]->Value = NULL;
                  SQLQuery1->Params->Items[i]->Bound = true;
               }
            }
            else
            {
               if(Query->Fields->Fields[i]->DataType == ftTimeStamp)
               {
                  SQLQuery1->ParamByName(FieldName)->AsSQLTimeStamp =
                     Query->FieldByName(FieldName)->AsSQLTimeStamp;
               }
               else if(Query->Fields->Fields[i]->DataType == ftString)
               {
                  SQLQuery1->ParamByName(FieldName)->AsString =
                     Query->FieldByName(FieldName)->AsString;
               }
               else
               {
                  SQLQuery1->ParamByName(FieldName)->Value =
                     Query->FieldByName(FieldName)->AsVariant;
               }
            }
         }
         SQLQuery1->ExecSQL();
      }
   }
   __finally
   {
   	delete BlobStream;
   }
}

*/
