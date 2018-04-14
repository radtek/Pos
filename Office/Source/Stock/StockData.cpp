#include <vcl.h>
#pragma hdrstop

#include "StockData.h"
#include "..\Data\DBCreate.h"

#include <memory>

using std::auto_ptr;

#include <cassert>

#define STOCK_DB_VERSION "6.24.0"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmStockData *dmStockData;
//---------------------------------------------------------------------------
__fastcall TdmStockData::TdmStockData(TComponent* Owner)
	: TDataModule(Owner)
{
	dbStock->Connected = false;
}
//---------------------------------------------------------------------------
bool TdmStockData::Connect(TLabel *Label)
{
	dbStock->Connected = false;
        DefaultLocation = CurrentConnection.DefaultLocation;
	if (CurrentConnection.StockDB.RemoteConnection)
	{
		if (!CurrentConnection.StockDB.Connected)
		{
			if (Application->MessageBox(AnsiString("Do you wish to dial " + CurrentConnection.StockDB.RASEntry + " to connect to Stock now?").c_str(),
												 "Dial?", MB_ICONQUESTION + MB_YESNO) == ID_YES)
			{
				Screen->Cursor = crHourGlass;
				try
				{
					bool Retry = true;
					while (Retry)
					{
						Retry = false;
						if (Label)
						{
							Label->Caption = "Dialing...";
							Label->Update();
						}
						AnsiString ErrorMessage;
						if (!CurrentConnection.StockDB.Dial(ErrorMessage))
						{
							Retry = (Application->MessageBox((ErrorMessage + "\rDo you wish to try again?").c_str(),
															  "Dial Error", MB_ICONERROR + MB_YESNO) == ID_YES);
						}
					}
					if (!CurrentConnection.StockDB.Connected)
					{
						return false;
					}
				}
				__finally
				{
					Screen->Cursor = crDefault;
				}
			}
			else return false;
		}
	}
	if (CurrentConnection.StockDB.DBName == "")
	{
		return false;
	}
	else
	{
		try
		{
			if (Label)
			{
				Label->Caption = "Opening database...";
				Label->Update();
			}
			dbStock->DatabaseName = CurrentConnection.StockDB.DBName;
			dbStock->Connected = true;
			// Make sure forced writes is on.
			TIBDatabaseInfo *IBDatabaseInfo						= new TIBDatabaseInfo(this);
			try
			{
				IBDatabaseInfo->Database							= dbStock;
				if (!IBDatabaseInfo->ForcedWrites)
				{
					TIBConfigService *IBConfigService			= new TIBConfigService(this);
					try
					{
						try
						{
							AnsiString ServerName	= CurrentConnection.StockDB.DBName.SubString(1, CurrentConnection.StockDB.DBName.Pos(":"));
							AnsiString DatabaseName	= CurrentConnection.StockDB.DBName.SubString(CurrentConnection.StockDB.DBName.Pos(":")+1,
																														CurrentConnection.StockDB.DBName.Length() -
																														CurrentConnection.StockDB.DBName.Pos(":") + 1);

							IBDatabaseInfo->Database->Connected	= false;
							IBConfigService->ServerName			= ServerName;
							IBConfigService->DatabaseName			= DatabaseName;
							IBConfigService->LoginPrompt			= false;
							IBConfigService->Protocol				= TCP;
							IBConfigService->Params->Add("user_name=SYSDBA");
							IBConfigService->Params->Add("password=masterkey");
							IBConfigService->Active					= true;

							IBConfigService->SetReadOnly(false);
							while (IBConfigService->IsServiceRunning)
							{
								Sleep(5);
							}
							IBConfigService->SetAsyncMode(false);
							while (IBConfigService->IsServiceRunning)
							{
								Sleep(5);
							}
						}
						catch (EIBError &E)
						{
							// Someone else may be using the db. No biggie
						}
					}
					__finally
					{
						IBConfigService->Active = false;
						delete IBConfigService;
					}
				}
			}
			__finally
			{
				delete IBDatabaseInfo;
			}
			dbStock->Connected = true;
			return true;
		}
		catch (Exception &E)
		{
			Application->MessageBox(("Could not connect to Database.\r" + E.Message).c_str(), "Error", MB_OK + MB_ICONERROR);
			if (CurrentConnection.StockDB.CreateDB)
			{
				if (Application->MessageBox("Do you wish to create a new database?", "Create new database?", MB_YESNO + MB_ICONQUESTION) == ID_YES)
				{
					return CreateDB(CurrentConnection.StockDB.DBName);
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
}
//---------------------------------------------------------------------------
bool TdmStockData::CreateDB(AnsiString DatabaseName)
{
	dbStock->Connected = false;
	dbStock->DatabaseName = DatabaseName;
	dbStock->Params->Clear();
	dbStock->Params->Append("USER 'SYSDBA'");
	dbStock->Params->Append("PASSWORD 'masterkey'");
	dbStock->Params->Append("PAGE_SIZE 4096");
	dbStock->LoginPrompt = false;
	try
	{
		dbStock->CreateDatabase();
	}
	catch(Exception &E)
	{
		Application->MessageBox(AnsiString("Could not create file.\r" + E.Message).c_str(), "Error", MB_OK + MB_ICONERROR);
		return false;
	}
	dbStock->Connected = false;
	dbStock->Params->Clear();
	dbStock->Params->Add("user_name=SYSDBA");
	dbStock->Params->Add("password=masterkey");
	dbStock->Params->Add("lc_ctype=ISO8859_1");
	dbStock->LoginPrompt = false;
	dbStock->Connected   = true;

	Query->Transaction->StartTransaction();
	int i = 0;
	try
	{
		do
		{
			Query->SQL->Text = CreateSQL[i++];
			Query->ExecQuery();
		}
		while (CreateSQL[i] != "!EOF!");
		Query->Close();
		Query->SQL->Text = "Select Gen_id(Gen_Version_Key, 1) From rdb$database";
		Query->ExecQuery();
		int VersionKey = Query->Fields[0]->AsInteger;
		Query->Close();
		Query->Transaction->Commit();
		Query->Transaction->StartTransaction();
		TVarRec v[] = { STOCK_DB_VERSION };
		Query->SQL->Text =
			Format("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values (:Version_Key, '%s', Current_TimeStamp)",v, ARRAYSIZE(v) - 1 );
		Query->ParamByName("Version_Key")->AsInteger = VersionKey;
		Query->ExecQuery();

		Query->Transaction->Commit();
        dmStockData->Update6_18_0();
		return true;
	}
	catch (Exception &E)
	{
		ShowMessage(E.Message + "\rLine " + IntToStr(i) + "\r\r" + Query->SQL->Text);
		Application->ShowException(&E);
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		return false;
	}
}
//---------------------------------------------------------------------------
void TdmStockData::Sweep(AnsiString DatabaseName)
{
	AnsiString ServerName = DatabaseName.SubString(1, DatabaseName.Pos(":"));
	AnsiString FileName = DatabaseName.SubString(DatabaseName.Pos(":")+1, DatabaseName.Length() - DatabaseName.Pos(":") + 1);

	ValidationService->ServerName		= ServerName;
	ValidationService->DatabaseName	= FileName;
	ValidationService->Active			= true;
	try
	{
		Screen->Cursor = crHourGlass;
		ValidationService->Options		= TValidateOptions() << SweepDB;
		ValidationService->ServiceStart();
		while (ValidationService->IsServiceRunning)
		{
			Application->ProcessMessages();
			Screen->Cursor = crHourGlass;
		}
	}
	__finally
	{
		Screen->Cursor = crDefault;
		ValidationService->Active		= false;
	}
	Application->MessageBox("Sweep Complete", "Complete", MB_OK + MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void TdmStockData::CloseDB()
{
	try
	{
		dmStockData->dbStock->Connected = false;
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		dmStockData->dbStock->ForceClose();
	}
}
//---------------------------------------------------------------------------
void TdmStockData::Disconnect()
{
	if (CurrentConnection.StockDB.RemoteConnection)
	{
		if (CurrentConnection.StockDB.Connected)
		{
			if (Application->MessageBox(("You are connected to " + CurrentConnection.StockDB.RASEntry +
												  ". Do you wish to disconnect?").c_str(),
												  "Question",
												  MB_ICONQUESTION + MB_YESNO) == ID_YES)
			{
				Screen->Cursor = crHourGlass;
				try
				{
					CurrentConnection.StockDB.HangUp();
				}
				__finally
				{
					Screen->Cursor = crDefault;
				}
			}
		}
	}
}

bool TdmStockData::RequiresUpdateTo(DBVersion::DBVersions version)
{
	switch (version)
	{
		case DBVersion::V2_4:
			return !HasDBVersion("2.4") && RequiresUpdateTo(DBVersion::V2_7);
		case DBVersion::V2_7:
			return !HasDBVersion("2.7") && RequiresUpdateTo(DBVersion::V3_0);
		case DBVersion::V3_0:
			return !HasDBVersion("3.0") && RequiresUpdateTo(DBVersion::V3_3);
		case DBVersion::V3_3:
			return !HasDBVersion("3.3") && RequiresUpdateTo(DBVersion::V3_31);
		case DBVersion::V3_31:
			return !HasDBVersion("3.31") && RequiresUpdateTo(DBVersion::V3_4);
		case DBVersion::V3_4:
			return !HasDBVersion("3.4") && RequiresUpdateTo(DBVersion::V4_0);
		case DBVersion::V4_0:
			return !HasDBVersion("4.0") && RequiresUpdateTo(DBVersion::V5_0);
		case DBVersion::V5_0:
			return !HasDBVersion("5.0") && RequiresUpdateTo(DBVersion::V5_5);
      case DBVersion::V5_5:
         return !HasDBVersion("5.5") && RequiresUpdateTo(DBVersion::V5_6);
      case DBVersion::V5_6:
         return !HasDBVersion("5.6") && RequiresUpdateTo(DBVersion::V5_7);
      case DBVersion::V5_7:
         return !HasDBVersion("5.7") && RequiresUpdateTo(DBVersion::V6_0);
    case DBVersion::V6_0:
         return !HasDBVersion("6.0") && RequiresUpdateTo(DBVersion::V6_1);
    case DBVersion::V6_1:
         return !HasDBVersion("6.1") && RequiresUpdateTo(DBVersion::V6_2);
    case DBVersion::V6_2:
         return !HasDBVersion("6.2") && RequiresUpdateTo(DBVersion::V6_2_1);
           case DBVersion::V6_2_1:
    return !HasDBVersion("6.2.1") && RequiresUpdateTo(DBVersion::V6_2_2);
    case DBVersion::V6_2_2:
    return !HasDBVersion("6.2.2") && RequiresUpdateTo(DBVersion::V6_2_3);
    case DBVersion::V6_2_3:
      return !HasDBVersion("6.2.3")  && RequiresUpdateTo(DBVersion::V6_18_0);
    case DBVersion::V6_18_0:
      return !HasDBVersion("6.18.0") && RequiresUpdateTo(DBVersion::V6_22_0);
    case DBVersion::V6_22_0:
      return !HasDBVersion("6.22.0");
    case DBVersion::V6_23_0:
      return !HasDBVersion("6.23.0");
    case DBVersion::V6_24_0:
      return !HasDBVersion("6.24.0");
      default:
         assert(0);
	};
}
//---------------------------------------------------------------------------
bool TdmStockData::UpdateDB(TLabel *Label)
{
	if (dmStockData->dbStock->Connected)
	{
		if (dmStockData->HasDBVersion(STOCK_DB_VERSION))
		{
			return true;
		}
		else if (dmStockData->HasDBVersion("")) // Check if there is no version
		{
			Application->MessageBox("This version of the database is too old and must be updated.", "Error", MB_ICONERROR + MB_OK);
			return false;
		}
		else
		{
			if (Application->MessageBox("The database requires updating. Do you wish to proceed?", "Question", MB_ICONQUESTION + MB_YESNO) != ID_YES)
			{
				return false;
			}
			else
			{
				if (!dmStockData->WaitForSingleUser())
				{
					return false;
				}
				Label->Caption = "Backing Up Database...";
				Label->Update();
				AnsiString BackupFileName = AnsiString("Stock Update V") + STOCK_DB_VERSION + " - " + Now().FormatString("dd_mm_yyyy HH_nn_ss") + ".gbk";
				if (dmStockData->BackupDB(BackupFileName, Label))
				{
					if (!dmStockData->WaitForSingleUser())
					{
						return false;
					}
					Label->Caption = "Updating Database...";
					Label->Update();

					bool SuccessfulUpdate = dmStockData->Update2_4();
					if (SuccessfulUpdate)
					{
						SuccessfulUpdate = dmStockData->Update2_7();
					}
					if (SuccessfulUpdate)
					{
						SuccessfulUpdate = dmStockData->Update3_0();
					}
					if (SuccessfulUpdate)
					{
						SuccessfulUpdate = dmStockData->Update3_31();
					}
					if (SuccessfulUpdate)
					{
						SuccessfulUpdate = dmStockData->Update4_0();
					}
					if (SuccessfulUpdate)
					{
						SuccessfulUpdate = dmStockData->Update5_0();
					}
               if (SuccessfulUpdate)
               {
                  SuccessfulUpdate = dmStockData->Update5_5();
               }
               if (SuccessfulUpdate)
               {
                  SuccessfulUpdate = dmStockData->Update5_6();
               }
              if (SuccessfulUpdate)
               {
                  SuccessfulUpdate = dmStockData->Update5_7();
               }
               if(SuccessfulUpdate)
               {
                 SuccessfulUpdate = dmStockData->Update6_0();
               }
                if(SuccessfulUpdate)
               {
                 SuccessfulUpdate = dmStockData->Update6_1();
               }
               if(SuccessfulUpdate)
               {
                 SuccessfulUpdate = dmStockData->Update6_2();
               }
                 if(SuccessfulUpdate)
               {
                  SuccessfulUpdate = dmStockData->Update6_2_1();

               }
                if(SuccessfulUpdate)
               {
                  SuccessfulUpdate = dmStockData->Update6_2_2();

               }
               if(SuccessfulUpdate)
               {
                  SuccessfulUpdate = dmStockData->Update6_2_3();

               }
               if(SuccessfulUpdate)
               {
                  SuccessfulUpdate = dmStockData->Update6_18_0();

               }
               if(SuccessfulUpdate)
               {
                   SuccessfulUpdate = dmStockData->Update6_22_0();
               }

               if(SuccessfulUpdate)
               {
                  SuccessfulUpdate =  dmStockData->Update6_23_0();
               }

              if(SuccessfulUpdate)
               {
                SuccessfulUpdate =  dmStockData->Update6_24_0();
               }
                    if (!SuccessfulUpdate)
                    {
                        Application->MessageBox("The upgrade was unsuccessful. Restoring original version.\r\rIf this is the first time you have seen this, please try once more.", "Error", MB_ICONERROR + MB_OK);
                        if (!dmStockData->WaitForSingleUser())
                        {
                            return false;
                        }
                        Label->Caption = "Restoring Database...";
                        Label->Update();
                        if (!dmStockData->RestoreDB(BackupFileName, Label, true))
                        {
                            Application->MessageBox(AnsiString("Could not restore previous version (" + BackupFileName + ").").c_str(), "Error", MB_ICONERROR + MB_OK);
                        }
                        return false;
                    }
                    else
                    {
                        return true;
                    }
				}
				else
				{
				  Application->MessageBox(AnsiString("Could not create backup file (" + BackupFileName + ").\r Aborting upgrade.").c_str(), "Error", MB_ICONERROR + MB_OK);
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}
}

bool
TdmStockData::Update5_6()
{
    /* Are we /sure/ we need to update? */
    if (RequiresUpdateTo(DBVersion::V5_6))
        try {
            auto_ptr<TIBQuery> q(new TIBQuery(this));

            q->Database    = dbStock;
            q->Transaction = trStock;

            trStock->StartTransaction();
            q->SQL->Text =
              "alter table contact add weborder_target_uri varchar(32);";
            q->ExecSQL();
            q->Close();

		    q->SQL->Text =
              "insert into DBVersion (Version_Key, "
              "                       Version_Number, "
              "                       Time_Stamp) "
              "            values (gen_id(gen_version_key, 1), "
              "                    '5.6', "
              "                    Current_TimeStamp)";
            q->ExecSQL();

            trStock->Commit();
        } catch (Exception &e) {
            trStock->Rollback();
            return false;
        }

    return true;
}

bool
TdmStockData::Update5_7()
{
   /* Are we /sure/ we need to update? */
   if (RequiresUpdateTo(DBVersion::V5_7))
   {
      try
      {
         auto_ptr<TIBQuery> q(new TIBQuery(this));

         q->Database    = dbStock;
         q->Transaction = trStock;

         trStock->StartTransaction();

         createGenerators5_7(q.get());
         createTables5_7(q.get());
         setDBVersion(q.get(),"5.7");

         trStock->Commit();

      }
      catch (Exception &e)
      {
         trStock->Rollback();
         return false;
      }
   }

   return true;
}

void
TdmStockData::createGenerators5_7(TIBQuery* query)
{
   query->Close();
   query->SQL->Text = "CREATE GENERATOR GEN_TAXPROFILES";
   query->ExecSQL();
}

void
TdmStockData::createTables5_7(TIBQuery* query)
{
   query->Close();
   query->SQL->Text =
      "CREATE TABLE TAXPROFILES "
      "( "
      "  PROFILE_KEY int primary key, "
      "  NAME varchar(50), "
      "  RATE numeric(15,4), "
      "  TYPE int, "
      "  PRIORITY int, "
      "  SELECTABLE char(1) "
      ")";
   query->ExecSQL();
}

void
TdmStockData::setDBVersion(TIBQuery* query, AnsiString version)
{
   query->Close();
   query->SQL->Text =
     "insert into DBVersion (Version_Key, "
     "                       Version_Number, "
     "                       Time_Stamp) "
     "            values (gen_id(gen_version_key, 1), '" + version + "',"
     "                    Current_TimeStamp)";
   query->ExecSQL();
}

//---------------------------------------------------------------------------
/*TDateTime TdmStockData::Now()
{
	TDateTime Now = 0;
	if (!sqlNow->Transaction->InTransaction)
	{
		sqlNow->Transaction->StartTransaction();
	}
	sqlNow->Close();
	sqlNow->ExecQuery();
	Now = dmStockData->sqlNow->Fields[0]->AsDateTime;
	sqlNow->Transaction->Commit();

	return Now;
}
//---------------------------------------------------------------------------
TDateTime TdmStockData::Date()
{
	return this->Now();
}
//---------------------------------------------------------------------------
TDateTime TdmStockData::Time()
{
	return this->Now();
} */
//---------------------------------------------------------------------------
bool TdmStockData::BackupDB(AnsiString FileName, TLabel *Label)
{
	try
	{
		IBBackupService1->Active = false;
		IBBackupService1->ServerName = dmStockData->dbinfStock->DBSiteName; // Needs to resolve to IP
		IBBackupService1->Protocol = TCP;
		IBBackupService1->Attach();

		try
		{
			IBBackupService1->Options.Clear();
			IBBackupService1->Options << NoGarbageCollection << IgnoreLimbo;
			IBBackupService1->Verbose = true;
			IBBackupService1->DatabaseName = dbinfStock->DBFileName;
			IBBackupService1->BackupFile->Clear();
			IBBackupService1->BackupFile->Add(CurrentConnection.StockDB.DBFilePath + FileName);

			IBBackupService1->ServiceStart();
			while (IBBackupService1->IsServiceRunning)
			{
				while (!IBBackupService1->Eof)
				{
					Label->Caption = IBBackupService1->GetNextLine();
					Label->Update();
					Sleep(0);
					Application->ProcessMessages();
				}
				Sleep(0);
				Application->ProcessMessages();
			}
		}
		__finally
		{
			if (IBBackupService1->Active)
			{
				IBBackupService1->Detach();
			}
		}
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TdmStockData::RestoreDB(AnsiString FileName, TLabel *Label, bool ReplaceDB)
{
	try
	{
		IBRestoreService1->Active = false;
		IBRestoreService1->ServerName = dmStockData->dbinfStock->DBSiteName;
		IBRestoreService1->DatabaseName->Clear();
		IBRestoreService1->DatabaseName->Add(dbinfStock->DBFileName);
		IBRestoreService1->BackupFile->Clear();
		IBRestoreService1->BackupFile->Add(CurrentConnection.StockDB.DBFilePath + FileName);

		dbStock->Connected = false;
		IBRestoreService1->Attach();
		try
		{
			IBRestoreService1->Options.Clear();
			if (ReplaceDB) IBRestoreService1->Options << Replace;
			IBRestoreService1->Verbose = true;

			IBRestoreService1->ServiceStart();
			while (IBRestoreService1->IsServiceRunning)
			{
				while (!IBRestoreService1->Eof)
				{
					Label->Caption = IBRestoreService1->GetNextLine();
					Label->Update();
					Sleep(0);
					Application->ProcessMessages();
				}
				Sleep(0);
				Application->ProcessMessages();
			}
		}
		__finally
		{
			if (IBRestoreService1->Active)
			{
				IBRestoreService1->Detach();
			}
		}
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TdmStockData::HasDBVersion(AnsiString Version)
{
	bool Exists = false;
	try
	{
		TStringList *TablesList = new TStringList();
		try
		{
			GetTableNames(TablesList);
			if (TablesList->IndexOf("VERSION") > -1)
			{
				if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

				if (Version == "") // Check for no version
				{
					Query->SQL->Text = "Select Version_Number From Version";
					Query->ExecQuery();
					Exists = (Query->Eof);
				}
				else
				{
					Query->SQL->Text = "Select Version_Number From Version Where Version_Number = :VersionNumber";
					Query->ParamByName("VersionNumber")->AsString = Version;
					Query->ExecQuery();
					Exists = (Version == Query->FieldByName("Version_Number")->AsString);
				}

				if (Query->Transaction->InTransaction) Query->Transaction->Commit();
			}
			else if (TablesList->IndexOf("DBVERSION") > -1)
			{
				if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

				if (Version == "") // Check for no version
				{
					return false; // Can't be no version.
				}
				else
				{
					Query->SQL->Text = "Select Version_Number From DBVersion Where Version_Number = :VersionNumber";
					Query->ParamByName("VersionNumber")->AsString = Version;
					Query->ExecQuery();
					Exists = (Version == Query->FieldByName("Version_Number")->AsString);
				}

				if (Query->Transaction->InTransaction) Query->Transaction->Commit();
			}
			else
			{
				Exists = false;
			}
		}
		__finally
		{
			delete TablesList;
		}
	}
	catch (Exception &E)
	{
		Exists = true; // Don't want them trying to update a db if there was a problem getting the version!
		Application->ShowException(&E);
	}
	return Exists;
}
//---------------------------------------------------------------------------
bool TdmStockData::WaitForSingleUser()
{
	bool Retry = (dbinfStock->UserNames->Count > 1);
	while (Retry)
	{
		if (Application->MessageBox("There must be no other computers or applications connected to the database to proceed.", "Error", MB_ICONERROR + MB_RETRYCANCEL) == ID_CANCEL)
		{
			return false;
		}
		if (dmStockData->dbinfStock->UserNames->Count == 1)
		{
			Retry = false;
		}
	}
	return true;
}
//---------------------------------------------------------------------------
void TdmStockData::GetTableNames(TStrings *Fields)
{
	Fields->Clear();
	if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

	Query->Close();
	Query->SQL->Text =
		"Select distinct "
			"rdb$relation_name TableName "
		"From "
			"rdb$relations "
		"Where "
			"rdb$relation_name not like 'RDB$%'";
	for (Query->ExecQuery(); !Query->Eof; Query->Next())
	{
		Fields->Add(Query->FieldByName("TableName")->AsString.Trim());
	}
	if (Query->Transaction->InTransaction) Query->Transaction->Commit();
}
//---------------------------------------------------------------------------
void TdmStockData::GetFieldNames(AnsiString TableName, TStrings *Fields)
{
	Fields->Clear();
	if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

	Query->Close();
	Query->SQL->Text =
		"Select distinct "
			"Rel.rdb$field_name FieldName,"
			"Fld.rdb$field_type FieldType,"
			"Fld.rdb$field_sub_type FieldSubType "
		"From "
			"rdb$relation_fields Rel Left Join rdb$fields Fld on "
				"Fld.rdb$field_name = Rel.rdb$field_source "
		"Where "
			"Rel.rdb$relation_name not like 'RDB$%' and "
			"Rel.rdb$relation_name = :TableName";
	for (Query->ExecQuery(); !Query->Eof; Query->Next())
	{
		Fields->Add(Query->FieldByName("FieldName")->AsString);
	}
	if (Query->Transaction->InTransaction) Query->Transaction->Commit();
}
//---------------------------------------------------------------------------
bool TdmStockData::Update2_4()
{
	try
	{
		if (this->RequiresUpdateTo(DBVersion::V2_4) == false)
			return true;

		//if (HasDBVersion("2.7") || HasDBVersion("3.0") || HasDBVersion("3.3") || HasDBVersion("3.31") || HasDBVersion("3.4") || HasDBVersion("4.0") )
		//{
		//	return true; // Already updated
		//}
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->SQL->Text = "alter table stock add GST_PERCENT Numeric(15,4)";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "alter table stocktrans add GST_PERCENT Numeric(15,4)";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "alter table stocktrans add SALE_GST_PERCENT Numeric(15,4)";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "alter table stock alter GST_PERCENT position 8";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "alter table stocktrans alter GST_PERCENT position 16";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "alter table stocktrans alter SALE_GST_PERCENT position 18";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text =
				"Create table DBVersion"
				"("
					"Version_Number		Varchar(10) Character Set ISO8859_1 not null collate en_uk,"
					"Time_Stamp				Timestamp,"

					"Constraint				DBVersion_PK Primary Key (Version_Number)"
				")";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();

		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->SQL->Text = "update stock set GST_PERCENT = 12.5";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "update stocktrans set GST_PERCENT = 12.5, SALE_GST_PERCENT = 12.5";
		Query->ExecQuery();

		IBQuery1->Close();
		IBQuery1->SQL->Text = "Insert into DBVersion (Version_Number, Time_Stamp) Values (:Version_Number, :Time_Stamp)";

		Query->Close();
		Query->SQL->Text = "Select * from Version order by Time_Stamp";
		for (Query->ExecQuery(); !Query->Eof; Query->Next())
		{
			IBQuery1->ParamByName("Version_Number")->AsString	= Query->FieldByName("Version_Number")->AsString;
			IBQuery1->ParamByName("Time_Stamp")->AsDateTime		= Query->FieldByName("Time_Stamp")->AsDateTime;
			IBQuery1->ExecSQL();
		}

		Query->Close();
		Query->SQL->Text = "Insert into DBVersion (Version_Number, Time_Stamp) Values ('2.7', Current_TimeStamp)";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();

		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();
		Query->Close();
		Query->SQL->Text = "Drop Table Version";
		Query->ExecQuery();
		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
	}
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TdmStockData::Update2_7()
{
	try
	{
		if (this->RequiresUpdateTo(DBVersion::V2_7) == false)
			return true;

		//if (HasDBVersion("3.0") || HasDBVersion("3.3") || HasDBVersion("3.31") || HasDBVersion("3.4") || HasDBVersion("4.0"))
		//{
		// 	return true; // Already updated
		//}
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->ParamCheck = true;
		Query->SQL->Text = "Alter Table Stock Add GL_Code Varchar(12) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Stock Alter GL_Code Position 3";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Stock Add Deleted T_Boolean Default 'F'";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table StockCategory Add GL_Code Varchar(12) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table StockGroup Add GL_Code Varchar(12) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockGroup Alter GL_Code Position 3";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Add GL_Code Varchar(12) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Add Stock_Category Varchar(25) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Add Stock_Group Varchar(25) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Add Supplier_Key Integer";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter Stock_Category Position 3";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter Stock_Group Position 4";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter GL_Code Position 5";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Add Supplier_Unit_Size Numeric(15, 4)";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Alter Order_Unit To Supplier_Unit";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Alter Unit_Cost To Supplier_Unit_Cost";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Alter Order_Qty To Supplier_Unit_Qty";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseOrder drop Delivery_Name";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseOrder drop Delivery_ID";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseOrder Drop Delivery_Phone";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseOrder Drop Job_Number";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseOrder Drop Supplier_Name";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Stock_Key";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Drop Index Stock_Order_Number_Index";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Order_Number";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Description";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Supplier_Name";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Unit_Quoted_Cost";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Total_Cost";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Quote_Reference";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Posted";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Stock_Unit";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text =
			"Create Table ContactGroup"
			"("
				"Contact_Group_Key	Integer Not Null,"
				"Contact_Group			VarChar(25) Character Set ISO8859_1 Not Null Collate En_UK,"
				"Sort_Order				Integer,"

				"Constraint				ContactGroup_PK Primary Key (Contact_Group_Key)"
			")";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Create Generator Gen_Contact_Group_Key";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Contact Add Contact_Group_Key Integer Not Null";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Contact Alter Contact_Group_Key Position 1";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Contact Add Deleted T_Boolean Default 'F'";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text =
			"Delete From "
				"PurchaseStock "
			"Where "
				"PurchaseStock.Purchase_Stock_LK in "
			"(Select "
				"PurchaseStock.Purchase_Stock_LK "
			"From "

				"PurchaseStock Left Join PurchaseOrder On "
					"PurchaseStock.Order_FK = PurchaseOrder.Order_LK "
			"Where "
				"PurchaseOrder.Order_LK is Null)";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseOrder Add Received T_Boolean";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();

		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		IBQuery1->Close();
		IBQuery1->SQL->Text =
			"Select "
				"Order_FK,"
				"Max(Supplier_Unit_Qty - (Qty_Received + Qty_Not_Received)) Received "
			"From "
				"PurchaseStock "
			"Group By "
				"Order_FK";
		Query->Close();
		Query->SQL->Text = "Update PurchaseOrder Set Received = :Received Where Order_LK = :OrderKey";
		for (IBQuery1->Open(); !IBQuery1->Eof; IBQuery1->Next())
		{
			Query->ParamByName("OrderKey")->AsInteger = IBQuery1->FieldByName("Order_FK")->AsInteger;
			Query->ParamByName("Received")->AsString = (IBQuery1->FieldByName("Received")->AsFloat == 0)?"T":"F";
			Query->ExecQuery();
		}
		IBQuery1->Close();
		Query->Close();
		Query->SQL->Text = "Update PurchaseOrder Set Received = 'F' Where Received Is Null";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();

		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->ParamCheck = false;
		Query->SQL->Text =
			"Create Procedure UpdateStockTrans As "

				"Declare Variable vTransNumber Integer; "
				"Declare Variable vStockGroup Varchar(25); "
				"Declare Variable vStockCategory Varchar(25); "
				"Declare Variable vSupplierKey Integer; "

			"Begin "
				"For "
					"Select "
						"StockTrans.Trans_Number,"
						"StockGroup.Stock_Group,"
						"StockCategory.Stock_Category,"
						"Contact.Contact_LK "
					"From "
						"StockTrans Left Join Stock On "
							"StockTrans.Code = Stock.Code "
						"Left Join StockGroup On "
							"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
						"Left Join StockCategory On "
							"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
						"Left Join Contact On "
							"StockTrans.Supplier_Name = Contact.Company_Name "
					"Into "
						":vTransNumber,"
						":vStockGroup,"
						":vStockCategory,"
						"vSupplierKey "
				"Do "
				"Begin "
					"Update "
						"StockTrans "
					"Set "
						"Stock_Group = :vStockGroup, "
						"Stock_Category = :vStockCategory, "
						"Supplier_Key = :vSupplierKey "
					"Where "
						"StockTrans.Trans_Number = :vTransNumber; "
				"End "
			"End ";
		Query->ExecQuery();
		Query->Close();
		Query->ParamCheck = true;
		Query->Close();
		Query->SQL->Text = "Execute Procedure UpdateStockTrans";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Drop Procedure UpdateStockTrans";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Create Index Trans_Category_Index On StockTrans(Stock_Category)";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Create Index Trans_Group_Index On StockTrans(Stock_Group)";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Create Index Trans_Supplier_Key_Index On StockTrans(Supplier_Key)";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseOrder Add Supplier_Fax Varchar(25) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseOrder Alter Supplier_Fax Position 4";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseOrder Add Instructions Varchar(250) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseOrder Alter Instructions Position 6";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Add Constraint PurchaseStock2Order Foreign Key (Order_FK) References PurchaseOrder (Order_LK) On Update Cascade On Delete Cascade";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Update PurchaseStock Set Supplier_Unit_Size = Stock_Qty / Supplier_Unit_Qty Where Stock_Qty <> 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Stock_Qty";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table Contact Add MYOB1 Varchar(25) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Contact Add MYOB2 Varchar(25) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Contact Add MYOB3 Varchar(25) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Select Gen_id(Gen_Contact_Group_Key, 1) From rdb$database";
		Query->ExecQuery();
		int ContactGroupKey = Query->Fields[0]->AsInteger;
		Query->Close();
		Query->SQL->Text = "Insert Into ContactGroup (Contact_Group_Key, Contact_Group, Sort_Order) Values (:Contact_Group_Key, 'All', 0)";
		Query->ParamByName("Contact_Group_Key")->AsInteger = ContactGroupKey;
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Update Contact Set Contact_Group_Key = 1";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Contact Add Constraint Contact2ContactGroup Foreign Key (Contact_Group_Key) References ContactGroup (Contact_Group_Key) On Update Cascade On Delete Cascade";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Update Stock Set Deleted = 'F'";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Update Contact Set Deleted = 'F'";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter Transaction_Type Position 1";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter Created Position 2";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter GST_Percent Position 9";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter Unit Position 10";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter Qty Position 11";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter Unit_Cost Position 12";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter Supplier_Key Position 15";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Update StockTrans Set GST_Percent = 12.5 Where GST_Percent Is Null";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Insert into DBVersion (Version_Number, Time_Stamp) Values ('3.0', Current_TimeStamp)";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
	}
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TdmStockData::Update3_0()
{
	try
	{
		if (this->RequiresUpdateTo(DBVersion::V3_0) == false)
			return true;

		//if (HasDBVersion("3.3") || HasDBVersion("3.31") || HasDBVersion("3.4") || HasDBVersion("4.0") )
		//{
		//	return true; // Already updated
		//}
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->ParamCheck = true;
		Query->SQL->Text = "Alter Table StockLocation Add Sales_Pending Numeric(15, 4) Default 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockLocation Add Writeoffs_Pending Numeric(15, 4) Default 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockLocation Add Opening Numeric(15, 4) Default 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockLocation Add Last_Stocktake Timestamp";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockLocation Add Inwards Numeric(15, 4) Default 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockLocation Add Writeoff Numeric(15, 4) Default 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockLocation Add Transfer Numeric(15, 4) Default 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockLocation Add Sales Numeric(15, 4) Default 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockLocation Add Default_Location T_Boolean Default 'F'";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Stock Add Assessed_Value Numeric(15, 4) Default 0 Not Null";
		Query->ExecQuery();
		//Query->Close();
		//Query->SQL->Text = "Update Stock Set Assessed_Value = 0";
		//Query->ExecQuery();
		Query->Close();
		Query->SQL->Text =
			"CREATE TABLE STOCKTAKEHISTORY"
			"("
				"STOCKTAKEHISTORY_KEY	INTEGER NOT NULL,"
				"BATCH_KEY					INTEGER NOT NULL,"
				"CREATED						TIMESTAMP,"
				"USER_ID						VARCHAR(12) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"USER_NAME					VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"CODE							VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"DESCRIPTION				VARCHAR(50) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"STOCK_CATEGORY			VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"STOCK_GROUP				VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"LOCATION					VARCHAR(25) CHARACTER SET ISO8859_1,"
				"STOCKTAKE_UNIT			VARCHAR(16) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"UNIT_COST					NUMERIC(15, 4),"
				"LAST_STOCKTAKE			TIMESTAMP,"
				"OPENING						NUMERIC(15, 4) DEFAULT 0,"
				"INWARDS						NUMERIC(15, 4) DEFAULT 0,"
				"WRITEOFF					NUMERIC(15, 4) DEFAULT 0,"
				"TRANSFER					NUMERIC(15, 4) DEFAULT 0,"
				"SALES						NUMERIC(15, 4) DEFAULT 0,"
				"CLOSING						NUMERIC(15, 4) DEFAULT 0"
			")";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Create Generator Gen_StocktakeHistory_Key";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text =
			"CREATE TABLE TransactionBatch"
			"("
				"BATCH_KEY				INTEGER NOT NULL,"
				"CREATED					TIMESTAMP,"
				"TRANSACTION_TYPE		VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"USER_ID					VARCHAR(12) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"USER_NAME				VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"REFERENCE				VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"SUPPLIER_KEY			INTEGER,"
				"SUPPLIER_NAME			VARCHAR(50) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"SUPPLIER_ID			VARCHAR(50) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"EXPORT_1				VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"EXPORT_2				VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"EXPORT_3				VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"ORDER_NUMBER			VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK,"
				"TOTAL_COST				NUMERIC(15, 4),"
				"TOTAL_GST				NUMERIC(15, 4),"
				"CLOSED					T_BOOLEAN DEFAULT 'F',"
				"EXPORTED				T_BOOLEAN DEFAULT 'F',"
                "ISCOMMITTED_PACKINGSLIP				T_BOOLEAN DEFAULT 'F',"

				"CONSTRAINT				TRANSACTIONBATCH_PK PRIMARY KEY (BATCH_KEY)"
			")";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Create Index Batch_Created_Index On TransactionBatch(Created)";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Create Index Batch_Type_Index On TransactionBatch(Transaction_Type)";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Create Index Batch_Reference_Index On TransactionBatch(Reference)";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Create Generator Gen_Stocktrans_Batch_Key";
		Query->ExecQuery();
		Query->Close();

		Query->SQL->Text =
			"Alter Table StocktakeHistory		Add Constraint StocktakeHistory2Batch Foreign Key (Batch_Key) "
														"References TransactionBatch (Batch_Key) On Update Cascade On Delete Cascade";
		Query->ExecQuery();
		Query->Close();

		Query->SQL->Text = "Create Generator Gen_Version_Key";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table DBVersion Add Version_Key Integer Not Null";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table DBVersion Alter Version_Key Position 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Contact Add Supplier_ID Varchar(50) Character Set ISO8859_1 collate en_uk";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Contact Alter Supplier_ID Position 18";
		Query->ExecQuery();
		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->SQL->Text = "Alter Table Stocktrans Add Total_GST Numeric(15,4)";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table StockCategory Add Deleted T_Boolean Default 'F'";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockGroup Add Deleted T_Boolean Default 'F'";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Drop Table ImportTrack";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();
		// cww
		Query->Close();
		Query->SQL->Text = "Update Stock Set Assessed_Value = 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table Stock Alter Assessed_Value Position 9";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Update StockCategory Set Deleted = 'F'";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Update StockGroup Set Deleted = 'F'";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Update Contact Set Supplier_ID = Bill_Attn Where Supplier_ID Is Null";
		Query->ExecQuery();

		IBQuery1->Close();
		IBQuery1->SQL->Text = "Select Time_Stamp From DBVersion Order By Time_Stamp";
		IBQuery2->Close();
		IBQuery2->SQL->Text = "Select Gen_id(Gen_Version_Key, 1) From rdb$database";

		Query->Close();
		Query->SQL->Text = "Update DBVersion Set Version_Key = :Version_Key Where Time_Stamp = :Time_Stamp";
		for (IBQuery1->Open(); !IBQuery1->Eof; IBQuery1->Next())
		{
			IBQuery2->Open();
			Query->ParamByName("Version_Key")->AsInteger	= IBQuery2->Fields->Fields[0]->AsInteger;
			Query->ParamByName("Time_Stamp")->AsDateTime	= IBQuery1->FieldByName("Time_Stamp")->AsDateTime;
			Query->ExecQuery();
			IBQuery2->Close();
		}
		IBQuery2->Open();
		Query->Close();
		Query->SQL->Text = "Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values (:Version_Key, '3.3', Current_TimeStamp)";
		Query->ParamByName("Version_Key")->AsInteger	= IBQuery2->Fields->Fields[0]->AsInteger;
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Alter Supplier_Unit Type Varchar(25)";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Add Qty_On_Order Numeric(15, 4) Default 0";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table SupplierStock Add Min_Order_Qty Numeric(15, 4) Default 1";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table DBVersion Drop Constraint DBVersion_PK";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table DBVersion Add Constraint DBVersion_PK Primary Key (Version_Key)";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Create Unique Index Version_Number_Index On DBVersion(Version_Number)";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text =
			"Update "
				"StockLocation "
			"Set "
				"Default_Location		= 'F',"
				"Sales_Pending			= 0,"
				"Writeoffs_Pending	= 0,"
				"Opening					= 0,"
				"Inwards					= 0,"
				"Writeoff				= 0,"
				"Transfer				= 0,"
				"Sales					= 0";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->SQL->Text =
			"Update "
				"PurchaseStock "
			"Set "
				"Qty_On_Order = Supplier_Unit_Qty - (Qty_Received + Qty_Not_Received) "
			"Where "
				"Qty_On_Order is Null";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text =
			"Update "
				"PurchaseStock "
			"Set "
				"Qty_On_Order = 0 "
			"Where "
				"Qty_On_Order Is Null Or "
				"PurchaseStock.Purchase_Stock_LK In ("
					"Select "
						"PurchaseStock.Purchase_Stock_LK "
					"From "
						"PurchaseOrder Left Join PurchaseStock On "
							"PurchaseOrder.Order_LK = PurchaseStock.Order_FK "
					"Where "
						"PurchaseOrder.Received = 'T' Or "
						"PurchaseOrder.Posted = 'F' Or PurchaseOrder.Posted Is Null)";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Alter Table StockLocation Drop On_Order";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text =
			"Update "
				"SupplierStock "
			"Set "
				"Min_Order_Qty = 1 "
			"Where "
				"Min_Order_Qty is Null";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text =
			"Update "
				"StockTrans "
			"Set "
				"Location = :Location,"
				"Purchaser_Name = :Purchaser_Name "
			"Where "
				"Trans_Number = :Trans_Number";
		IBQuery1->Close();
		IBQuery1->SQL->Text =
			"Select "
				"Trans_Number,"
				"Location,"
				"Purchaser_Name "
			"From "
				"StockTrans "
			"Where "
				"Transaction_Type = 'Transfer' And "
				"Qty > 0";
		for (IBQuery1->Open(); !IBQuery1->Eof; IBQuery1->Next())
		{
			Query->Close();
			Query->ParamByName("Trans_Number")->AsInteger	= IBQuery1->FieldByName("Trans_Number")->AsInteger;
			Query->ParamByName("Location")->AsString			= IBQuery1->FieldByName("Purchaser_Name")->AsString;
			Query->ParamByName("Purchaser_Name")->AsString	= IBQuery1->FieldByName("Location")->AsString;
			Query->ExecQuery();
		}

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->SQL->Text = "Alter Table PurchaseStock Drop Qty_Not_Received";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text =
			"Insert Into StockTrans ("
				"Trans_Number, "
				"Code, "
				"Description, "
				"Stock_Category, "
				"Stock_Group, "
				"GL_Code, "
				"Location, "
				"User_ID, "
				"User_Name, "
				"Created, "
				"Unit, "
				"Qty, "
				"Unit_Cost, "
				"GST_Percent, "
				"Purchaser_Name, "
				"Transaction_Type) "
			"Values ("
				":Trans_Number, "
				":Code, "
				":Description, "
				":Stock_Category, "
				":Stock_Group, "
				":GL_Code, "
				":Location, "
				":User_ID, "
				":User_Name, "
				":Created, "
				":Unit, "
				":Qty, "
				":Unit_Cost, "
				"15, "
				":Purchaser_Name, "
				"'Transfer')";
		IBQuery2->Close();
		IBQuery2->SQL->Text = "Select Gen_id(Gen_StockTrans, 1) From rdb$database";
		IBQuery1->Close();
		IBQuery1->SQL->Text = "Select * From StockTrans Where Transaction_Type = 'Transfer' And Qty >= 0";
        double unitCost = 0.00;
		for (IBQuery1->Open(); !IBQuery1->Eof; IBQuery1->Next())
		{
			IBQuery2->Open();

			Query->Close();
			Query->ParamByName("Trans_Number")->AsInteger	= IBQuery2->Fields->Fields[0]->AsInteger;
			Query->ParamByName("Code")->AsString				= IBQuery1->FieldByName("Code")->AsString;
			Query->ParamByName("Description")->AsString		= IBQuery1->FieldByName("Description")->AsString;
			Query->ParamByName("Stock_Category")->AsString	= IBQuery1->FieldByName("Stock_Category")->AsString;
			Query->ParamByName("Stock_Group")->AsString		= IBQuery1->FieldByName("Stock_Group")->AsString;
			Query->ParamByName("GL_Code")->AsString			= IBQuery1->FieldByName("GL_Code")->AsString;
			Query->ParamByName("Location")->AsString			= IBQuery1->FieldByName("Purchaser_Name")->AsString;
			Query->ParamByName("User_ID")->AsString			= IBQuery1->FieldByName("User_ID")->AsString;
			Query->ParamByName("User_Name")->AsString			= IBQuery1->FieldByName("User_Name")->AsString;
			Query->ParamByName("Created")->AsDateTime			= IBQuery1->FieldByName("Created")->AsDateTime;
			Query->ParamByName("Unit")->AsString				= IBQuery1->FieldByName("Unit")->AsString;
			Query->ParamByName("Qty")->AsDouble					= -IBQuery1->FieldByName("Qty")->AsFloat;
            unitCost = IBQuery1->FieldByName("Unit_Cost")->AsFloat;
			Query->ParamByName("Unit_Cost")->AsDouble			=  unitCost > -1000000 ? double(unitCost) : 0;
			Query->ParamByName("Purchaser_Name")->AsString	= IBQuery1->FieldByName("Location")->AsString;
			Query->ExecQuery();

			IBQuery2->Close();
		}
		IBQuery1->Close();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();
		//Fix missing categories/groups. Forgot to fix in stockinterface.dll
		Query->Close();
		Query->ParamCheck = false;
		Query->SQL->Text =
			"Create Procedure UpdateStockTrans As "

				"Declare Variable vTransNumber Integer; "
				"Declare Variable vStockGroup Varchar(25); "
				"Declare Variable vStockCategory Varchar(25); "

			"Begin "
				"For "
					"Select "
						"StockTrans.Trans_Number,"
						"StockGroup.Stock_Group,"
						"StockCategory.Stock_Category "
					"From "
						"StockTrans Left Join Stock On "
							"StockTrans.Code = Stock.Code "
						"Left Join StockGroup On "
							"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
						"Left Join StockCategory On "
							"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
					"Where "
						"StockTrans.Stock_Group is null or "
						"StockTrans.Stock_Category is null "
					"Into "
						":vTransNumber,"
						":vStockGroup,"
						":vStockCategory "
				"Do "
				"Begin "
					"Update "
						"StockTrans "
					"Set "
						"Stock_Group = :vStockGroup, "
						"Stock_Category = :vStockCategory "
					"Where "
						"StockTrans.Trans_Number = :vTransNumber; "
				"End "
			"End ";
		Query->ExecQuery();
		Query->Close();

		Query->ParamCheck = true;
		Query->Close();
		Query->SQL->Text = "Execute Procedure UpdateStockTrans";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Drop Procedure UpdateStockTrans";
		Query->ExecQuery();

		Query->Close();
		Query->SQL->Text = "Create Unique Index Unique_Stock_Location_Index On StockLocation(Stock_Key, Location)";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->SQL->Text =
			"Select "
				"StockTrans.Created,"
				"StockTrans.Transaction_Type,"
				"StockTrans.Reference,"
				"StockTrans.Order_Number,"
				"StockTrans.User_ID,"
				"StockTrans.User_Name,"
				"Contact.Contact_LK Supplier_Key,"
				"Contact.Company_Name Supplier_Name,"
				"Contact.Supplier_ID,"
				"Contact.MYOB1,"
				"Contact.MYOB2,"
				"Contact.MYOB3,"
				// trouble hot spot - had to cast them all as Double Precision to avoid integer overflow
				"Cast(Sum( cast(StockTrans.Unit_Cost as Double Precision) * cast(StockTrans.Qty) as Double Precision ) ) as Numeric(15, 4)) Total_Cost,"
				"cast(Sum( cast(StockTrans.Unit_Cost as Double Precision) * cast(StockTrans.Qty) as Double Precision) * cast(StockTrans.GST_Percent as Double Precision) / 100 ) as numeric(15,4) ) Total_GST "
			"From "
				"StockTrans Left Join Contact On "
					"StockTrans.Supplier_Key = Contact.Contact_LK "
			"Group By "
				"StockTrans.Created,"
				"StockTrans.Transaction_Type,"
				"StockTrans.Reference,"
				"StockTrans.Order_Number,"
				"StockTrans.User_ID,"
				"StockTrans.User_Name,"
				"Contact.Contact_LK,"
				"Contact.Company_Name,"
				"Contact.Supplier_ID,"
				"Contact.MYOB1,"
				"Contact.MYOB2,"
				"Contact.MYOB3 "
			"Order By "
				"StockTrans.Created";
		IBQuery1->Close();
		IBQuery1->SQL->Text =
			"Insert Into TransactionBatch ("
				"Batch_Key,"
				"Created,"
				"Transaction_Type,"
				"Reference,"
				"Order_Number,"
				"User_ID,"
				"User_Name,"
				"Supplier_Key,"
				"Supplier_Name,"
				"Supplier_ID,"
				"Export_1,"
				"Export_2,"
				"Export_3,"
				"Total_Cost,"
				"Total_GST,"
				"Closed,"
				"Exported, "
                "ISCOMMITTED_PACKINGSLIP)"
			"Values ("
				":Batch_Key,"
				":Created,"
				":Transaction_Type,"
				":Reference,"
				":Order_Number,"
				":User_ID,"
				":User_Name,"
				":Supplier_Key,"
				":Supplier_Name,"
				":Supplier_ID,"
				":Export_1,"
				":Export_2,"
				":Export_3,"
				":Total_Cost,"
				":Total_GST,"
				"'T',"
				"'F', "
                "'F')";
		IBQuery2->Close();
		IBQuery2->SQL->Text = "Select Gen_id(Gen_Stocktrans_Batch_Key, 1) From rdb$database";
		try
		{
			for (Query->ExecQuery(); !Query->Eof; Query->Next())
			{
				IBQuery2->Close();
				IBQuery2->Open();
				TDateTime Temp = Query->FieldByName("Created")->AsDateTime;
				IBQuery1->Close();
				IBQuery1->ParamByName("Batch_Key")->AsInteger			= IBQuery2->Fields->Fields[0]->AsInteger;
				IBQuery1->ParamByName("Created")->AsDateTime				= Temp;
				IBQuery1->ParamByName("Transaction_Type")->AsString	= Query->FieldByName("Transaction_Type")->AsString;
				IBQuery1->ParamByName("Reference")->AsString				= Query->FieldByName("Reference")->AsString;
				IBQuery1->ParamByName("Order_Number")->AsString			= Query->FieldByName("Order_Number")->AsString;
				IBQuery1->ParamByName("User_ID")->AsString				= Query->FieldByName("User_ID")->AsString;
				IBQuery1->ParamByName("User_Name")->AsString				= Query->FieldByName("User_Name")->AsString;
				IBQuery1->ParamByName("Supplier_Key")->AsInteger		= Query->FieldByName("Supplier_Key")->AsInteger;
				IBQuery1->ParamByName("Supplier_Name")->AsString		= Query->FieldByName("Supplier_Name")->AsString;
				IBQuery1->ParamByName("Supplier_ID")->AsString			= Query->FieldByName("Supplier_ID")->AsString;
				IBQuery1->ParamByName("Export_1")->AsString				= Query->FieldByName("MYOB1")->AsString;
				IBQuery1->ParamByName("Export_2")->AsString				= Query->FieldByName("MYOB2")->AsString;
				IBQuery1->ParamByName("Export_3")->AsString				= Query->FieldByName("MYOB3")->AsString;
				IBQuery1->ParamByName("Total_Cost")->AsString 			= Query->FieldByName("Total_Cost")->AsString;
				IBQuery1->ParamByName("Total_GST")->AsString				= Query->FieldByName("Total_GST")->AsString;
				IBQuery1->ExecSQL();
			}
		}
		catch (Exception &E)
		{
			// E.Message = Integer Overflow the result of an integer operation caused the most significant bit of the resul to carry
			ShowMessage("Total Cost is too large to fit into the TransactionBatch table. \r"
							"It has been calculated from Unit_Cost * Qty in the StockTrans table" );
			throw;
		}
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Add Batch_Key Integer Not Null";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter Batch_Key Position 1";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Alter Table SupplierStock Alter Min_Order_Qty Position 9";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		dbStock->Connected = false;
		dbStock->Connected = true;
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		//Add batches to transactions
		Query->Close();
		Query->ParamCheck = false;
		Query->SQL->Text =
			"Create Procedure UpdateStockTrans As "

				"Declare Variable vBatchKey Integer; "
				"Declare Variable vCreated TimeStamp; "
				"Declare Variable vTransaction_Type Varchar(25); "

			"Begin "
				"For "
					"Select "
						"Batch_Key,"
						"Created,"
						"Transaction_Type "
					"From "
						"TransactionBatch "
					"Into "
						":vBatchKey,"
						":vCreated,"
						":vTransaction_Type "
				"Do "
				"Begin "
					"Update "
						"StockTrans "
					"Set "
						"Batch_Key = :vBatchKey "
					"Where "
						"StockTrans.Transaction_Type = :vTransaction_Type And "
						"StockTrans.Created = :vCreated; "
				"End "
			"End ";
		Query->ExecQuery();
		Query->Close();
		Query->ParamCheck = true;
		Query->Close();
		Query->SQL->Text = "Execute Procedure UpdateStockTrans";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Drop Procedure UpdateStockTrans";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->SQL->Text = "Alter Table StockTrans Alter Total_GST Position 11";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "Update StockTrans Set Total_GST = Qty * Unit_Cost * GST_Percent / 100 Where Total_GST Is Null";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text =
			"Alter Table StockTrans	Add Constraint StockTrans2Batch Foreign Key (Batch_Key) References TransactionBatch (Batch_Key) "
				"On Update Cascade On Delete Cascade";
		Query->ExecQuery();

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
	}
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TdmStockData::Update3_31()
{
	try
	{
		if (this->RequiresUpdateTo(DBVersion::V3_31) == false)
			return true;

		//if ( HasDBVersion("3.31") || HasDBVersion("3.4") || HasDBVersion("4.0") )
		//{
		//	return true; // Already updated
		//}
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->SQL->Text = "alter table stocklocation add ASSESSED_VALUE 	Numeric(15, 4) Default 0";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "alter table stocklocation alter ASSESSED_VALUE position 8";
		Query->ExecQuery();
		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();
		Query->Close(); // pull out the figures into stocklocation from stock
		Query->SQL->Text = "update stocklocation sl set sl.assessed_value = "
								 "(select s.assessed_value from stock s where sl.stock_key = s.stock_key)";
		Query->ExecQuery();
		Query->Close();
		Query->SQL->Text = "alter table stock drop ASSESSED_VALUE";
		Query->ExecQuery();
		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();
		Query->Close();
		Query->SQL->Text = "Select max(VERSION_KEY) as maxVersionKey from DBVersion";
		Query->ExecQuery();
		int VersionKey = Query->FieldByName("maxVersionKey")->AsInteger + 1;
		Query->Close();
		Query->SQL->Text = "Insert into DBVersion (Version_Key,Version_Number, Time_Stamp) Values (" + IntToStr(VersionKey) + ",'3.31', Current_TimeStamp)";
		Query->ExecQuery();
		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
	}
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TdmStockData::Update4_0()
{
	const AnsiString THIS_VER = "4.0";
	try
	{
		if (this->RequiresUpdateTo(DBVersion::V4_0) == false)
			return true;

		//if (HasDBVersion("3.4") || HasDBVersion("4.0") )
		//{
		//	return true; // Already updated
		//}
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();
		// Delete lost order stock.
		RunSQL(
			"Delete From PurchaseStock Where "
				"Purchase_Stock_LK In ("
					"Select "
						"PurchaseStock.Purchase_Stock_LK "
					"From "
						"PurchaseStock Left Join PurchaseOrder On "
							"PurchaseStock.Order_FK = PurchaseOrder.Order_LK "
					"Where "
						"Order_LK Is Null)");

		// Add a stocktake entity. Stock items are pointed to this table when initialised.
		// Stocktake history slaves off this.
		RunSQL(
			"Create Table Stocktake "
			"("
				"Stocktake_Key				Integer Not NULL,"
				"Batch_Key					Integer,"
				"Name							Varchar(25) Character Set ISO8859_1,"
				"Location					Varchar(25) Character Set ISO8859_1,"

				"Initialised_Time			TimeStamp,"
				"Initialised_User_ID	   Varchar(12) Character Set ISO8859_1 COLLATE EN_UK,"
				"Initialised_User_Name	Varchar(25) Character Set ISO8859_1 COLLATE EN_UK,"

				"Committed_Time			TimeStamp,"
				"Committed_User_ID	   Varchar(12) Character Set ISO8859_1 COLLATE EN_UK,"
				"Committed_User_Name	   Varchar(25) Character Set ISO8859_1 COLLATE EN_UK,"

				"Constraint					Stocktake_PK Primary Key (Stocktake_Key)"
			")");
		RunSQL("Create Index Stocktake_Initialised_Index On Stocktake(Initialised_Time)");
		RunSQL("Create Index Stocktake_Committed_Index On Stocktake(Committed_Time)");

		RunSQL("Alter Table StocktakeHistory Add Stocktake_Key Integer Not NULL");
		RunSQL("Alter Table StocktakeHistory Add Sales_Pending Numeric(15, 4) Default 0");
		RunSQL("Alter Table StocktakeHistory Add Writeoffs_Pending Numeric(15, 4) Default 0");

		RunSQL("Alter Table StocktakeHistory Add Average_Unit_Cost Numeric(15, 4) Default 0");
		RunSQL("Alter Table StocktakeHistory Add Latest_Unit_Cost Numeric(15, 4) Default 0");

		RunSQL("Alter Table StocktakeHistory Alter Stocktake_Key Position 2");
		RunSQL("Alter Table StocktakeHistory Alter Average_Unit_Cost Position 12");
		RunSQL("Alter Table StocktakeHistory Alter Latest_Unit_Cost Position 13");
		RunSQL("Alter Table StockLocation Add Stocktake_Key Integer");
		RunSQL("Alter Table StockLocation Alter Stocktake_Key Position 9");
		RunSQL("Alter Table StockGroup Drop Stocktake_Visible");
		RunSQL("Create Generator Gen_Stocktake_Key");

		RestartTransaction();

		// Create stocktake entities for all previous stocktakes.
		RunSQL(
			"Select "
				"Created,"
				"Batch_Key,"
				"User_ID,"
				"User_Name,"
				"Location "
			"From "
				"StocktakeHistory "
			"Group By "
				"Created,"
				"Batch_Key,"
				"User_ID,"
				"User_Name,"
				"Location "
			"Order By "
				"Created ");

		IBQuery1->Close();
		IBQuery1->SQL->Text =
			"Insert Into Stocktake ("
				"Stocktake_Key,"
				"Batch_Key,"
				"Location,"
				"Name,"
				"Initialised_Time,"
				"Initialised_User_ID,"
				"Initialised_User_Name,"
				"Committed_Time,"
				"Committed_User_ID,"
				"Committed_User_Name)"
			"Values ("
				"(Select Gen_id(Gen_Stocktake_Key, 1) From rdb$database),"
				":Batch_Key,"
				":Location,"
				":Name,"
				":Initialised_Time,"
				":Initialised_User_ID,"
				":Initialised_User_Name,"
				":Committed_Time,"
				":Committed_User_ID,"
				":Committed_User_Name)";
		for (; !Query->Eof; Query->Next())
		{
			IBQuery1->Close();
			IBQuery1->ParamByName("Batch_Key")->AsInteger				= Query->FieldByName("Batch_Key")->AsInteger;
			IBQuery1->ParamByName("Location")->AsString					= Query->FieldByName("Location")->AsString;
			IBQuery1->ParamByName("Name")->AsString						= Query->FieldByName("Location")->AsString;

			IBQuery1->ParamByName("Initialised_Time")->AsDateTime		= Query->FieldByName("Created")->AsDateTime;
			IBQuery1->ParamByName("Initialised_User_ID")->AsString	= Query->FieldByName("User_ID")->AsString;
			IBQuery1->ParamByName("Initialised_User_Name")->AsString	= Query->FieldByName("User_Name")->AsString;

			IBQuery1->ParamByName("Committed_Time")->AsDateTime		= Query->FieldByName("Created")->AsDateTime;
			IBQuery1->ParamByName("Committed_User_ID")->AsString		= Query->FieldByName("User_ID")->AsString;
			IBQuery1->ParamByName("Committed_User_Name")->AsString	= Query->FieldByName("User_Name")->AsString;
			IBQuery1->ExecSQL();
		}
		Query->ParamCheck = false;
		RunSQL(
			"Create Procedure UpdateStocktakeHistory As "

				"Declare Variable vStocktakeKey Integer; "
				"Declare Variable vBatchKey Integer; "
				"Declare Variable vInitialisedTime TimeStamp; "

			"Begin "
				"For "
					"Select "
						"Stocktake_Key,"
						"Batch_Key,"
						"Initialised_Time "
					"From "
						"Stocktake "
					"Into "
						":vStocktakeKey,"
						":vBatchKey,"
						":vInitialisedTime "
				"Do "
				"Begin "
					"Update "
						"StocktakeHistory "
					"Set "
						"Stocktake_Key = :vStocktakeKey "
					"Where "
						"StocktakeHistory.Batch_Key = :vBatchKey And "
						"StocktakeHistory.Created = :vInitialisedTime; "
				"End "
			"End ");
		Query->ParamCheck = true;
		RunSQL("Execute Procedure UpdateStocktakeHistory");
		RunSQL("Drop Procedure UpdateStocktakeHistory");

		RestartTransaction();

		RunSQL("Update StocktakeHistory Set Sales_Pending = 0 Where Sales_Pending Is Null");
		RunSQL("Update StocktakeHistory Set Writeoffs_Pending = 0 Where Writeoffs_Pending Is Null");
		RunSQL("Update StocktakeHistory Set Average_Unit_Cost = Unit_Cost Where Average_Unit_Cost Is Null");
		RunSQL("Update StocktakeHistory Set Latest_Unit_Cost = Unit_Cost Where Latest_Unit_Cost Is Null");

		RestartTransaction();

		RunSQL("Alter Table StocktakeHistory "
					 "Drop Created,"
					 "Drop User_ID,"
					 "Drop User_Name,"
					 "Drop Unit_Cost;");
		RunSQL("Alter Table StocktakeHistory Add Constraint Stocktake2History Foreign Key (Stocktake_Key) References Stocktake (Stocktake_Key) On Update Cascade On Delete Cascade");

		RestartTransaction();

		// Create stocktake entities for all previous stocktakes.
		RunSQL(
			"Select "
				"Location "
			"From "
				"StockLocation "
			"Where "
				"Initialised = 'T' "
			"Group By "
				"Location");

		IBQuery1->Close();
		IBQuery1->SQL->Text =
			"Insert Into Stocktake ("
				"Stocktake_Key,"
				"Location,"
				"Name,"
				"Initialised_Time,"
				"Initialised_User_ID,"
				"Initialised_User_Name) "
			"Values ("
				"(Select Gen_id(Gen_Stocktake_Key, 1) From rdb$database),"
				":Location,"
				":Name,"
				":Initialised_Time,"
				"'Unknown',"
				"'Unknown')";
		for (; !Query->Eof; Query->Next())
		{
			IBQuery1->Close();
			IBQuery1->ParamByName("Location")->AsString					= Query->FieldByName("Location")->AsString;
			IBQuery1->ParamByName("Name")->AsString						= Query->FieldByName("Location")->AsString;
			IBQuery1->ParamByName("Initialised_Time")->AsDateTime		= Now();

			IBQuery1->ExecSQL();
		}
		Query->ParamCheck = false;
		RunSQL(
			"Create Procedure UpdateStockLocation As "

				"Declare Variable vStocktakeKey Integer; "
				"Declare Variable vLocation Varchar(25); "

			"Begin "
				"For "
					"Select "
						"Stocktake_Key,"
						"Location "
					"From "
						"Stocktake "
					"Where "
						"Committed_Time Is Null "
					"Into "
						":vStocktakeKey,"
						":vLocation "
				"Do "
				"Begin "
					"Update "
						"StockLocation "
					"Set "
						"Stocktake_Key = :vStocktakeKey "
					"Where "
						"StockLocation.Location = :vLocation And "
						"StockLocation.Initialised = 'T'; "
				"End "
			"End ");
		Query->ParamCheck = true;
		RunSQL("Execute Procedure UpdateStockLocation");
		RunSQL("Drop Procedure UpdateStockLocation");

		try
		{
			// Make sure it doesn't already exist.
			RunSQL("ALTER TABLE PURCHASESTOCK DROP CONSTRAINT PURCHASESTOCK2ORDER");
		}
		catch (Exception &E)
		{
		}
		RunSQL("ALTER TABLE PURCHASESTOCK ADD CONSTRAINT PURCHASESTOCK2ORDER FOREIGN KEY (ORDER_FK) "
					"REFERENCES PURCHASEORDER (ORDER_LK)");

		// Receive Invoice Rounding issues
		RunSQL("alter table stocktrans add TOTAL_COST Numeric(15, 4) Default 0");
		RunSQL("alter table stocktrans alter TOTAL_COST position 15");

		RestartTransaction();

		// Cost rounding errors in Receive Invoice
		RunSQL("update stocktrans set TOTAL_COST = QTY * UNIT_COST");

		// Multiple Barcode stuff
		RunSQL("CREATE TABLE BARCODE "
					"("
						"STOCK_KEY	INTEGER NOT NULL,"
						"SUPPLIER_STOCK_KEY	INTEGER NOT NULL,"
						"BARCODE VARCHAR(25) CHARACTER SET ISO8859_1 COLLATE EN_UK "
					")");

		RestartTransaction();

		// Check for duplicate barcodes in Stock
		IBQuery1->Close();
		IBQuery1->SQL->Text = "Select Count(barcode),barcode from Stock group by barcode having count(barcode) > 1";
		for (IBQuery1->Open(); !IBQuery1->Eof; IBQuery1->Next())
		{
			// check for Null Barcodes
			if (IBQuery1->FieldByName("BARCODE")->AsString != "")
			{
				// set barcode to null for each duplicate entry
				RunSQL("Update Stock set Barcode = NULL where barcode = '" + IBQuery1->FieldByName("BARCODE")->AsString + "'");
			}	
		}
		// Insert into Barcode
		RunSQL("Insert into Barcode select Stock_Key,-1,Barcode from Stock where Barcode is not null");
		RunSQL("ALTER TABLE STOCK DROP BARCODE");
		RunSQL("ALTER TABLE SUPPLIERSTOCK DROP BARCODE");

		RunSQL("Select Gen_id(Gen_Version_Key, 1) From rdb$database");	// Increment key first as MAX was used in 3.31
		RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
	}
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TdmStockData::Update5_0()
{
	const AnsiString THIS_VER = "5.0";
	try
	{
		if (this->RequiresUpdateTo(DBVersion::V5_0) == false)
			return true;

		//if (HasDBVersion(THIS_VER))
		//{
		//	return true; // Already updated
		//}
		if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		// Add a stocktake entity. Stock items are pointed to this table when initialised.
		// Stocktake history slaves off this.
		RunSQL(
			"CREATE TABLE SECURITY_ROLES "
			"( "
				"ROLE_ID		INTEGER NOT NULL,"
				"ROLE_NAME	VARCHAR(32) CHARACTER SET ISO8859_1 NOT NULL COLLATE EN_UK,"

				"CONSTRAINT	PK_ROLE_ID PRIMARY KEY (ROLE_ID)"
			")");

		RunSQL(
			"CREATE TABLE SECURITY_PERMISSIONS "
			"( "
				"PERMISSION_ID		INTEGER NOT NULL,"
				"PERMISSION_KEY	INTEGER NOT NULL,"
				"PERMISSION_NAME	VARCHAR(32) CHARACTER SET ISO8859_1 NOT NULL COLLATE EN_UK,"

				"CONSTRAINT	PK_PERMISSION_ID PRIMARY KEY (PERMISSION_ID)"
			")");

		RunSQL(
			"CREATE TABLE SECURITY_USER_ROLES "
			"( "
				"USER_ROLE_ID	INTEGER NOT NULL,"
				"ROLE_ID			INTEGER NOT NULL,"
				"USER_ID			INTEGER NOT NULL,"

				"CONSTRAINT	PK_USER_ROLE_ID PRIMARY KEY (USER_ROLE_ID)"
			")");

		RunSQL(
			"CREATE TABLE SECURITY_ROLE_PERMISSIONS "
			"( "
				"ROLE_PERMISSIONS_ID	INTEGER NOT NULL,"
				"ROLE_ID					INTEGER NOT NULL,"
				"PERMISSION_ID			INTEGER NOT NULL,"

				"CONSTRAINT	PK_ROLE_PERMISSIONS_ID PRIMARY KEY (ROLE_PERMISSIONS_ID)"
			")");

		RunSQL("CREATE UNIQUE INDEX PERMISSION_KEY_INDEX ON SECURITY_PERMISSIONS (PERMISSION_KEY)");
		RunSQL("CREATE UNIQUE INDEX ROLE_NAME_INDEX ON SECURITY_ROLES (ROLE_NAME)");

		RunSQL("ALTER TABLE USERS DROP CONSTRAINT USERS_PK");
		RunSQL("DROP INDEX LOGIN_INDEX");
		RunSQL("CREATE GENERATOR GEN_USER_KEY");
		RunSQL("CREATE GENERATOR GEN_SECURITY_KEY");

		RunSQL("ALTER TABLE USERS ADD USER_KEY INTEGER NOT NULL");
		RunSQL("ALTER TABLE USERS ADD PASSWRD VARCHAR(32) CHARACTER SET ISO8859_1 DEFAULT '' NOT NULL COLLATE EN_UK");

		RestartTransaction();

		RunSQL("UPDATE USERS SET USER_KEY = (SELECT GEN_ID(GEN_USER_KEY, 1) FROM RDB$DATABASE)");
		RunSQL("CREATE UNIQUE INDEX USER_ID_INDEX ON USERS (USER_ID)");
		RunSQL("UPDATE USERS SET PASSWRD = PIN WHERE PIN IS NOT NULL");

		RestartTransaction();

		RunSQL("ALTER TABLE USERS ADD CONSTRAINT USER_KEY_PK PRIMARY KEY (USER_KEY)");

		RestartTransaction();
		RunSQL("ALTER TABLE USERS ALTER PASSWRD POSITION 11");
		RunSQL("ALTER TABLE USERS ALTER USER_KEY POSITION 1");
		RestartTransaction();

		RunSQL("ALTER TABLE SECURITY_USER_ROLES ADD CONSTRAINT USER_ROLE_TO_ROLE_FK FOREIGN KEY (ROLE_ID) REFERENCES SECURITY_ROLES (ROLE_ID) ON UPDATE CASCADE ON DELETE CASCADE");
		RunSQL("ALTER TABLE SECURITY_USER_ROLES ADD CONSTRAINT USER_ROLE_TO_USER_FK FOREIGN KEY (USER_ID) REFERENCES USERS (USER_KEY) ON UPDATE CASCADE ON DELETE CASCADE");

		RunSQL("ALTER TABLE SECURITY_ROLE_PERMISSIONS ADD CONSTRAINT PERMISSION_PERMISSION_FK FOREIGN KEY (PERMISSION_ID) REFERENCES SECURITY_PERMISSIONS (PERMISSION_ID) ON UPDATE CASCADE ON DELETE CASCADE");
		RunSQL("ALTER TABLE SECURITY_ROLE_PERMISSIONS ADD CONSTRAINT PERMISSION_ROLE_FK FOREIGN KEY (ROLE_ID) REFERENCES SECURITY_ROLES (ROLE_ID) ON UPDATE CASCADE ON DELETE CASCADE");

		RunSQL("CREATE UNIQUE INDEX UNIQUE_USER_ROLES ON SECURITY_USER_ROLES (USER_ID, ROLE_ID)");
		RunSQL("CREATE UNIQUE INDEX UNIQUE_ROLE_PERMISSIONS ON SECURITY_ROLE_PERMISSIONS (PERMISSION_ID, ROLE_ID)");

		RestartTransaction();

		RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");

		if (Query->Transaction->InTransaction) Query->Transaction->Commit();
	}
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TdmStockData::Update5_5()
{
	const AnsiString THIS_VER = "5.5";
	try
	{
		if (this->RequiresUpdateTo(DBVersion::V5_5) == false)
			return true;

		TStringList *TablesList = new TStringList();
		GetTableNames(TablesList);

        /*
         * Because the internal 5.4 builds were installed on
         * several sites the RECIPES table will be present.
         *
         * This table was meant to be present only with the
         * 5.5 release. We need to ensure that we don't try
         * to create the if it's already present.
         *
         * We do, however, need to update the version number.
         */
        bool recipes_table_found = false;
        for (int i = 0; !recipes_table_found
                        && i < TablesList->Count; i++) {
            recipes_table_found =
              TablesList->Strings[i] == "RECIPES";
        }

		if (!Query->Transaction->InTransaction)
            Query->Transaction->StartTransaction();


        if (!recipes_table_found) {
            // Add a stocktake entity. Stock items are pointed to this table when initialised.
            // Stocktake history slaves off this.
            RunSQL(
                "CREATE TABLE RECIPES "
                "( "
                    "RECIPES_ID             INTEGER NOT NULL,"
                    "RECIPE                 VARCHAR(30),"
                    "FINAL_STOCK            VARCHAR(50),"
                    "FINAL_STOCK_CODE       VARCHAR(25),"
                    "DESCRIPTION            VARCHAR(200),"
                    "FINAL_STOCK_LOCATION   VARCHAR(25),"
                    "TIME_STAMP             TIMESTAMP,"
                    "REQUIRED_STOCK         VARCHAR(50),"
                    "STOCK_CODE             VARCHAR(25),"
                    "STOCK_QTY              NUMERIC(15,4),"
                    "STOCK_LOCATION         VARCHAR(25),"
                    "STOCK_UNIT             VARCHAR(16),"
                    "STOCK_AVERAGE_COST     NUMERIC(15,4),"
                    "STOCK_LATEST_COST      NUMERIC(15,4),"
                    "DELETED                T_Boolean,"
                    "EDITTED                T_Boolean"

                ")");

            RunSQL("CREATE GENERATOR GEN_RECIPES_KEY");
            RestartTransaction();
        }

		RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");

		if (Query->Transaction->InTransaction)
            Query->Transaction->Commit();
	}
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		return false;
	}
	return true;

}

//---------------------------------------------------------------------------
inline void TdmStockData::RunSQL( AnsiString s )
{
	Query->Close();
	Query->SQL->Text = s;
	Query->ExecQuery();
}
//---------------------------------------------------------------------------
inline void TdmStockData::RestartTransaction()
{
	if (Query->Transaction->InTransaction) Query->Transaction->Commit();
	if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();
}

bool TdmStockData::Update6_0()
{
   const AnsiString THIS_VER = "6.0";
   if (RequiresUpdateTo(DBVersion::V6_0))
     {
        try
          {
             if (!Query->Transaction->InTransaction)
                  Query->Transaction->StartTransaction();
                CreateTables6_0();
                CreateGenerators6_0();
                RestartTransaction();
                CreateTrigger6_0();
                UpdateTables6_0();
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");
                if (Query->Transaction->InTransaction)
                    Query->Transaction->Commit();
           }
        catch (Exception &E)
         {
           if (Query->Transaction->InTransaction)
                Query->Transaction->Rollback();
	        	  Application->ShowException(&E);
		        return false;
         }
     }
  return true;
}


void TdmStockData::CreateTables6_0()
{
    if(!TableExists("STOCKREQUEST"))
     {
       RunSQL(
               "  CREATE TABLE STOCKREQUEST ( "
               "  REQUEST_NUMBER Integer NOT NULL, "
               "  LOCATION Varchar(25) CHARACTER SET ISO8859_1, "
               "  REQUEST_TYPE Varchar(16) CHARACTER SET ISO8859_1, "
               "  DATE_GENERATED Timestamp, "
               "  DATE_REQUIRED Timestamp, "
               "  ISCOMMITTED Char(1) CHARACTER SET ISO8859_1, "
               "  ISDELETED Char(1) CHARACTER SET ISO8859_1, "
               "  CONSTRAINT PK_STOCKREQUEST PRIMARY KEY (REQUEST_NUMBER)) ");
     }
    if(!TableExists("STOCKREQUEST_STOCK"))
     {
       RunSQL(
               " CREATE TABLE STOCKREQUEST_STOCK ( "
               "  STOCK_REQUEST_STOCKKEY Integer NOT NULL, "
               "  REQUEST_NUMBER Integer, "
               "  STOCK_KEY Integer, "
               "  SUPPLIER_KEY Integer, "
               "  QTY_REQUIRED Numeric(15,4), "
               "  STOCK_REQUEST_UNIT Varchar(16) CHARACTER SET ISO8859_1, "
               "  STOCK_REQUEST_UNIT_COST Numeric(15,4), "
               "  ISPROCESSED Char(1) CHARACTER SET ISO8859_1, "
               "  ISREMOVED Char(1) CHARACTER SET ISO8859_1, "
               "  DATE_ENTERED Timestamp, "
               "  DATE_UPDATED Timestamp, "
                " SUPPLIER_STOCK_KEY INTEGER, "
               "  SUPPLIER_CODE VARCHAR(25) CHARACTER SET ISO8859_1, " 
               "  CONSTRAINT PK_STOCKREQUEST_STOCK PRIMARY KEY (STOCK_REQUEST_STOCKKEY)) ");
    }
    if(!TableExists("STOCK_SUBSTITUTES"))
     {
       RunSQL(
               "CREATE TABLE STOCK_SUBSTITUTES( "
               "SUBSTITUTE_KEY Integer NOT NULL, "
               "STOCK_KEY Integer, "
               "SUBSTITUTE_STOCK_KEY Integer )");
     }
   if(!TableExists("STOCK_SUBSTITUTE_PERMISSIONS"))
     {
       RunSQL(
               "CREATE TABLE STOCK_SUBSTITUTE_PERMISSIONS ( "
               "SUBSTITUTE_KEY Integer, "
               "SUBSTITUTE_COND1 Char(1), "
               "SUBSTITUTE_COND2 Char(1), "
               "SUBSTITUTE_COND3 Varchar(4), "
               "TIME_STAMP Timestamp, "
               "SELECTABLE Char(1))");
     }
}

void TdmStockData::UpdateTables6_0()
{
 if(!ConstrantExists("FK_STOCKREQUEST_STOCK"))
  {
    RunSQL("ALTER TABLE STOCKREQUEST_STOCK ADD CONSTRAINT FK_STOCKREQUEST_STOCK FOREIGN KEY (REQUEST_NUMBER) REFERENCES STOCKREQUEST (REQUEST_NUMBER) ON UPDATE CASCADE ON DELETE CASCADE");
  }
 if(!ConstrantExists("FK_STOCKREQUEST_STOCK_STOCK"))
  {
    RunSQL("ALTER TABLE STOCKREQUEST_STOCK ADD CONSTRAINT FK_STOCKREQUEST_STOCK_STOCK FOREIGN KEY (STOCK_KEY) REFERENCES STOCK (STOCK_KEY) ON UPDATE CASCADE ON DELETE CASCADE");
  }
 if(!ConstrantExists("FK_STOCKREQUEST_STOCK_SUPPLIER"))
  {
    RunSQL("ALTER TABLE STOCKREQUEST_STOCK ADD CONSTRAINT FK_STOCKREQUEST_STOCK_SUPPLIER FOREIGN KEY (SUPPLIER_KEY) REFERENCES CONTACT (CONTACT_LK) ON UPDATE CASCADE ON DELETE CASCADE");
  }
 if(!FieldExists("STOCK","REDUCTION_UNIT"))
  {
    RunSQL("ALTER TABLE STOCK ADD REDUCTION_UNIT Varchar(16) CHARACTER SET ISO8859_1 COLLATE EN_UK");
  }
 if(!FieldExists("STOCK","CONVERSION_FACTOR"))
  {
    RunSQL("ALTER TABLE STOCK ADD CONVERSION_FACTOR Numeric(15,4)");
  }
 if(!FieldExists("STOCK","STOCK_SUB_GROUP"))
  {
    RunSQL("ALTER TABLE STOCK ADD STOCK_SUB_GROUP Varchar(25) CHARACTER SET ISO8859_1 COLLATE EN_UK");
  }
 if(!FieldExists("STOCK","REVERT_TO_ORIGINAL"))
  {
    RunSQL("ALTER TABLE STOCK ADD REVERT_TO_ORIGINAL T_BOOLEAN DEFAULT 'F'");
  }
 if(!FieldExists("PURCHASESTOCK","SUPPLIER_UNIT_CHANGED"))
  {
    RunSQL("ALTER TABLE PURCHASESTOCK ADD SUPPLIER_UNIT_CHANGED T_BOOLEAN DEFAULT 'F'");
  }
}

void TdmStockData::CreateGenerators6_0()
{
  if(!GeneratorExists("GEN_STOCK_SUBSTITUTE_KEY"))
   {
     RunSQL("CREATE GENERATOR GEN_STOCK_SUBSTITUTE_KEY");
   }
  if(!GeneratorExists("GEN_STOCKREQUEST_ID"))
   {
     RunSQL("CREATE GENERATOR GEN_STOCKREQUEST_ID");
   }
}

void TdmStockData::CreateTrigger6_0()
{
  if(!TriggerExists("STOCK_SUBSTITUTES_BI"))
  {
    RunSQL("CREATE TRIGGER STOCK_SUBSTITUTES_BI FOR STOCK_SUBSTITUTES ACTIVE  "
           "BEFORE INSERT POSITION 0  "
           "AS "
           "BEGIN "
           "if (NEW.SUBSTITUTE_KEY is NULL) then NEW.SUBSTITUTE_KEY= GEN_ID(gen_STOCK_SUBSTITUTE_KEY, 1); "
           "END ");
  }
}

  void TdmStockData::UpdateTables6_1()
{
    RunSQL("update stock set STOCK.REDUCTION_UNIT=STOCK.STOCKTAKE_UNIT where STOCK.REDUCTION_UNIT is null or STOCK.REDUCTION_UNIT='' ");
    RunSQL("update stock set STOCK.CONVERSION_FACTOR= 1 where STOCK.CONVERSION_FACTOR is null or STOCK.CONVERSION_FACTOR=0 ");
}

bool TdmStockData::FieldExists(AnsiString tableName, AnsiString fieldName)
{
  	Query->Close();
	Query->SQL->Text = "SELECT * FROM RDB$RELATION_FIELDS WHERE RDB$RELATION_NAME = :Table_Name AND RDB$FIELD_NAME = :Field_Name;";
   Query->ParamByName("Table_Name")->AsString = tableName;
   Query->ParamByName("Field_Name")->AsString = fieldName;
	Query->ExecQuery();
   return  Query->RecordCount > 0;
}

bool TdmStockData::TableExists(AnsiString tableName)
{
	Query->Close();
	Query->SQL->Text = "SELECT * FROM RDB$RELATIONS WHERE RDB$RELATION_NAME = :Table_Name;";
   Query->ParamByName("Table_Name")->AsString = tableName;
	Query->ExecQuery();
   return Query->RecordCount > 0;
}

bool TdmStockData::GeneratorExists(AnsiString generatorName)
{
	Query->Close();
	Query->SQL->Text = "SELECT * FROM RDB$GENERATORS WHERE RDB$GENERATOR_NAME = :Generator_Name;";
   Query->ParamByName("Generator_Name")->AsString = generatorName;
	Query->ExecQuery();
   return Query->RecordCount > 0;
}

bool TdmStockData::TriggerExists(AnsiString triggerName)
{
	Query->Close();
	Query->SQL->Text = "SELECT * FROM RDB$TRIGGERS WHERE RDB$TRIGGER_NAME = :Trigger_Name;";
   Query->ParamByName("Trigger_Name")->AsString = triggerName;
	Query->ExecQuery();
   return Query->RecordCount > 0;
}

bool TdmStockData::ConstrantExists(AnsiString constrantName)
{
	Query->Close();
	Query->SQL->Text = "SELECT * FROM RDB$RELATION_CONSTRAINTS WHERE RDB$CONSTRAINT_NAME = :Constrant_Name;";
   Query->ParamByName("Constrant_Name")->AsString = constrantName;
	Query->ExecQuery();
   return Query->RecordCount > 0;
}



bool TdmStockData::Update6_1()
{
   const AnsiString THIS_VER = "6.1";
   if (RequiresUpdateTo(DBVersion::V6_1))
     {
        try
          {
             if (!Query->Transaction->InTransaction)
                  Query->Transaction->StartTransaction();
                UpdateTables6_1();
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");
                if (Query->Transaction->InTransaction)
                    Query->Transaction->Commit();
           }
        catch (Exception &E)
         {
           if (Query->Transaction->InTransaction)
                Query->Transaction->Rollback();
	        	  Application->ShowException(&E);
		        return false;
         }
     }
  return true;
}




bool TdmStockData::Update6_2()
{
   const AnsiString THIS_VER = "6.2";
   if (RequiresUpdateTo(DBVersion::V6_2))
     {
        try
          {
             if (!Query->Transaction->InTransaction)
                  Query->Transaction->StartTransaction();
              //  UpdateTables6_2();

                      RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");

          if (Query->Transaction->InTransaction)
                    Query->Transaction->Commit();
           }
        catch (Exception &E)
         {
           if (Query->Transaction->InTransaction)
                Query->Transaction->Rollback();
	        	  Application->ShowException(&E);
		        return false;
         }
     }
  return true;
}
bool TdmStockData::Update6_2_1()
{
   const AnsiString THIS_VER = "6.2.1";
   if (RequiresUpdateTo(DBVersion::V6_2_1))
     {
        try
          {
             if (!Query->Transaction->InTransaction)
                  Query->Transaction->StartTransaction();
                    if(!FieldExists("STOCKREQUEST_STOCK","SUPPLIER_STOCK_KEY"))
            {
        	RunSQL("alter table STOCKREQUEST_STOCK add SUPPLIER_STOCK_KEY INTEGER");
            }
                 if(!FieldExists("STOCKREQUEST_STOCK","SUPPLIER_CODE"))
            {
            	RunSQL("alter table STOCKREQUEST_STOCK add SUPPLIER_CODE 	VARCHAR(25) ");
            }


            if(!FieldExists("PURCHASESTOCK","SUPPLIER_UNIT_CHANGED"))
            {
            RunSQL("ALTER TABLE PURCHASESTOCK ADD SUPPLIER_UNIT_CHANGED T_BOOLEAN DEFAULT 'F'");
            }
                 RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");

          if (Query->Transaction->InTransaction)
                    Query->Transaction->Commit();
           }
        catch (Exception &E)
         {
           if (Query->Transaction->InTransaction)
                Query->Transaction->Rollback();
	        	  Application->ShowException(&E);
		        return false;
         }
     }
  return true;
}



bool TdmStockData::Update6_2_2()
{
   const AnsiString THIS_VER = "6.2.2";
   if (RequiresUpdateTo(DBVersion::V6_2_2))
     {
        try
          {
             if (!Query->Transaction->InTransaction)
                  Query->Transaction->StartTransaction();
                UpdateTables6_2_2();
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");
                if (Query->Transaction->InTransaction)
                    Query->Transaction->Commit();
           }
        catch (Exception &E)
         {
           if (Query->Transaction->InTransaction)
                Query->Transaction->Rollback();
	        	  Application->ShowException(&E);
		        return false;
         }
     }
  return true;
}



 void TdmStockData::UpdateTables6_2_2()
{
 /*   RunSQL("UPDATE STOCKREQUEST_STOCK "
" SET STOCKREQUEST_STOCK.SUPPLIER_STOCK_KEY=(SELECT SUPPLIERSTOCK.SUPPLIER_STOCK_KEY FROM SUPPLIERSTOCK "
" WHERE STOCKREQUEST_STOCK.SUPPLIER_KEY=SUPPLIERSTOCK.SUPPLIER_KEY and  SUPPLIERSTOCK.STOCK_KEY=STOCKREQUEST_STOCK.STOCK_KEY)  ");   */


if (!Query->Transaction->InTransaction) Query->Transaction->StartTransaction();

		Query->Close();
		Query->SQL->Text = "UPDATE STOCKREQUEST_STOCK "
" SET STOCKREQUEST_STOCK.SUPPLIER_STOCK_KEY=(SELECT SUPPLIERSTOCK.SUPPLIER_STOCK_KEY FROM SUPPLIERSTOCK "
" WHERE STOCKREQUEST_STOCK.SUPPLIER_KEY=SUPPLIERSTOCK.SUPPLIER_KEY and  SUPPLIERSTOCK.STOCK_KEY=STOCKREQUEST_STOCK.STOCK_KEY) ";
		Query->ExecQuery();
          //	if (Query->Transaction->InTransaction) Query->Transaction->Commit();

 }


bool TdmStockData::Update6_2_3()
{
   const AnsiString THIS_VER = "6.2.3";
   if (RequiresUpdateTo(DBVersion::V6_2_3))
     {
        try
          {
             if (!Query->Transaction->InTransaction)
                  Query->Transaction->StartTransaction();
                CreateGenerators6_2_3();
                UpdateTables6_2_3();
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");
                if (Query->Transaction->InTransaction)
                    Query->Transaction->Commit();
           }
        catch (Exception &E)
         {
           if (Query->Transaction->InTransaction)
                Query->Transaction->Rollback();
	        	  Application->ShowException(&E);
		        return false;
         }
     }
  return true;
}

bool TdmStockData::Update6_18_0()
{
   const AnsiString THIS_VER_24 = "2.4";
   const AnsiString THIS_VER_27 = "2.7";
   const AnsiString THIS_VER_30 = "3.0";
   const AnsiString THIS_VER_33 = "3.3";
   const AnsiString THIS_VER_331 = "3.31";
   const AnsiString THIS_VER_34 = "3.4";
   const AnsiString THIS_VER_40 = "4.0";
   const AnsiString THIS_VER_50 = "5.0";
   const AnsiString THIS_VER_55 = "5.5";
   const AnsiString THIS_VER_56 = "5.6";
   const AnsiString THIS_VER_6180 = "6.18.0";
   const AnsiString THIS_VER_6220 = "6.22.0";

   if (!RequiresUpdateTo(DBVersion::V6_22_0)||RequiresUpdateTo(DBVersion::V6_22_0))
     {
        try
          {
             if (!Query->Transaction->InTransaction)
                  Query->Transaction->StartTransaction();
                if(!HasDBVersionExist("2.4"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_24 + "', Current_TimeStamp)");
                }
                if(!HasDBVersionExist("2.7"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_27 + "', Current_TimeStamp)");
                }
                if(!HasDBVersionExist("3.0"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_30 + "', Current_TimeStamp)");
                }
                if(!HasDBVersionExist("3.3"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_33 + "', Current_TimeStamp)");
                }
                if(!HasDBVersionExist("3.31"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_331 + "', Current_TimeStamp)");
                }
                if(!HasDBVersionExist("3.4"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_34 + "', Current_TimeStamp)");
                }
                if(!HasDBVersionExist("4.0"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_40 + "', Current_TimeStamp)");
                }
                if(!HasDBVersionExist("5.0"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_50 + "', Current_TimeStamp)");
                }
                if(!HasDBVersionExist("5.5"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_55 + "', Current_TimeStamp)");
                }
                if(!HasDBVersionExist("5.6"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_56 + "', Current_TimeStamp)");
                }
                if(!HasDBVersionExist("6.18.0"))
                {
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_6180 + "', Current_TimeStamp)");
                }
                if (Query->Transaction->InTransaction)
                    Query->Transaction->Commit();
           }
        catch (Exception &E)
         {
           if (Query->Transaction->InTransaction)
                Query->Transaction->Rollback();
	        	  Application->ShowException(&E);
		        return false;
         }
     }
  return true;
}

void TdmStockData::CreateGenerators6_2_3()
{
  if(!GeneratorExists("GEN_TRANSFER_NUMBER"))
   {

       RunSQL("CREATE GENERATOR GEN_TRANSFER_NUMBER");

   }
}

void TdmStockData::UpdateTables6_2_3()
{
    if(!FieldExists("STOCKTRANS","TRANSFER_ID"))
    {
        RunSQL("Alter Table STOCKTRANS Add TRANSFER_ID INTEGER");
          
    }

    if(!FieldExists("STOCKTRANS","STOCK_REQUEST_NO"))
    {
      RunSQL("Alter Table STOCKTRANS Add STOCK_REQUEST_NO INTEGER");

    }

    if(!FieldExists("STOCKTRANS","STOCK_REQUEST_STOCK_KEY_VAL"))
    {
        RunSQL("Alter Table STOCKTRANS Add STOCK_REQUEST_STOCK_KEY_VAL INTEGER");
    }

    if(!FieldExists("STOCKTRANS","REQUESTED_QTY_ORIGINAL_VAL"))
    {
       RunSQL("Alter Table STOCKTRANS Add REQUESTED_QTY_ORIGINAL_VAL Numeric(15,4)");

    }

}

bool TdmStockData::HasDBVersionExist(AnsiString Version)
{        Query->Close();
        Query->SQL->Text = "Select Version_Number From DBVersion Where Version_Number = :VersionNumber";
        Query->ParamByName("VersionNumber")->AsString = Version;
        Query->ExecQuery();
        bool versionNumber= (Version == Query->FieldByName("Version_Number")->AsString);
        return  versionNumber;
}
//---------------------------------------------------------------------------------------------------------------
void TdmStockData::UpdateTables6_22_0()
{
    if(!FieldExists("TRANSACTIONBATCH","ISCOMMITTED_PACKINGSLIP"))
    {
        RunSQL("Alter Table TRANSACTIONBATCH Add ISCOMMITTED_PACKINGSLIP T_Boolean");
          
    }
}
//--------------------------------------------------------------------------------------------------------------

bool TdmStockData::Update6_22_0()
{
   const AnsiString THIS_VER = "6.22.0";
   if (RequiresUpdateTo(DBVersion::V6_22_0))
     {
        try
          {
             if (!Query->Transaction->InTransaction)
                 Query->Transaction->StartTransaction();
                UpdateTables6_22_0();
                RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");
                if (Query->Transaction->InTransaction)
                    Query->Transaction->Commit();
           }
        catch (Exception &E)
         {
           if (Query->Transaction->InTransaction)
                Query->Transaction->Rollback();
	        	  Application->ShowException(&E);
		        return false;
         }
     }
  return true;
}
//---------------------------------------------------------------------------------------------------------------
void TdmStockData::UpdateTables6_23_0()
{
    const AnsiString THIS_VER_6230 = "6.23.0";

    if(!HasDBVersionExist("6.23.0"))
    {
        RunSQL(" Update STOCKLOCATION a set a.AVERAGE_COST = 0 , a.LATEST_COST = 0 where a.AVERAGE_COST < 0 or a.LATEST_COST < 0  ");

        RunSQL(" Update STOCKTAKEHISTORY a set a.AVERAGE_UNIT_COST = 0 , a.LATEST_UNIT_COST = 0 where a.AVERAGE_UNIT_COST < 0 or a.LATEST_UNIT_COST < 0  ");

        RunSQL(" Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER_6230 + "', Current_TimeStamp) ");
    }

}
//--------------------------------------------------------------------------------------------------------------
bool TdmStockData::Update6_23_0()
{
    try
      {
         if (!Query->Transaction->InTransaction)
             Query->Transaction->StartTransaction();
            UpdateTables6_23_0();
            if (Query->Transaction->InTransaction)
                Query->Transaction->Commit();
    }
    catch (Exception &E)
     {
       if (Query->Transaction->InTransaction)
            Query->Transaction->Rollback();
              Application->ShowException(&E);
            return false;
     }

  return true;
}
//-------------------------------------------------------------------------------------------------------------------------
 void TdmStockData::UpdateTables6_24_0()
{
    const AnsiString THIS_VER_6240 = "6.24.0";

    if(!HasDBVersionExist("6.24.0"))
    {
        IndexingForStocktakehistoryTable();
        RunSQL("Alter Table StocktakeHistory Add Prev_Average_Unit_Cost Numeric(15, 4) Default 0");
    }

}
//-------------------------------------------------------------------------------------------------------------------------
bool TdmStockData::Update6_24_0()
{
  const AnsiString THIS_VER = "6.24.0";
  if (RequiresUpdateTo(DBVersion::V6_24_0))
  {
    try
      {
         if (!Query->Transaction->InTransaction)
             Query->Transaction->StartTransaction();
            UpdateTables6_24_0();
            RunSQL("Insert into DBVersion (Version_Key, Version_Number, Time_Stamp) Values ((Select Gen_id(Gen_Version_Key, 1) From rdb$database), '" + THIS_VER + "', Current_TimeStamp)");
            if (Query->Transaction->InTransaction)
                Query->Transaction->Commit();
              // IndexingForStocktakehistoryTable();

            }

    catch (Exception &E)
     {
       if (Query->Transaction->InTransaction)
            Query->Transaction->Rollback();
              Application->ShowException(&E);
            return false;
     }
      }
  return true;
}      
  //--------------------------------------------------------------------------------------------------------------------

void TdmStockData::IndexingForStocktakehistoryTable()
{
     Query->Close();
     Query->SQL->Text = "CREATE INDEX IDX_STOCKTAKEHISTORY1 ON STOCKTAKEHISTORY (DESCRIPTION,STOCK_CATEGORY,STOCK_GROUP,LOCATION) ";
     Query->ExecQuery();

     Query->Close();
     Query->SQL->Text = "CREATE INDEX IDX_STOCKTAKEHISTORY2 ON STOCKTAKEHISTORY (STOCKTAKEHISTORY_KEY) ";
     Query->ExecQuery();
}

//---------------------------------------------------------------------------------------------------------------------------------

