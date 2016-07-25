//---------------------------------------------------------------------------
#pragma hdrstop

#include "MM_DBCore.h"
//#ifdef USE_DBX
//	#include <sqlexpr.hpp>
//	#include <dbxdsnint.h>
//#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Database;
//---------------------------------------------------------------------------
TDBSettings::TDBSettings(
			AnsiString					iServer,
			AnsiString					iFileName,
			AnsiString					iUserName,
			AnsiString					iPassword,
			AnsiString					iCharacterSet,
			Ibservices::TProtocol	iProtocol,
			TDBType						iDBType,
			int							iPort) :
					Server			(iServer),
					FileName			(iFileName),
					UserName			(iUserName),
					Password			(iPassword),
					CharacterSet	(iCharacterSet),
					Protocol			(iProtocol),
					DBType			(iDBType),
					Port				(iPort)
{
}
//---------------------------------------------------------------------------
TDBSettings::TDBSettings(
			bool							UseFullConnectionString, // Dummy to force this constructor
			AnsiString					iConnectionString,
			AnsiString					iUserName,
			AnsiString					iPassword,
			AnsiString					iCharacterSet,
			Ibservices::TProtocol	iProtocol,
			TDBType						iDBType,
			int							iPort) :
					UserName			(iUserName),
					Password			(iPassword),
					CharacterSet	(iCharacterSet),
					Protocol			(iProtocol),
					DBType			(iDBType),
					Port				(iPort)
{
	int Pos = iConnectionString.AnsiPos(":\\");
	if (Pos == 0 || Pos == 2)
	{
		Server	= "";
		FileName	= iConnectionString;
	}
	else
	{
		AnsiString File;
		int ServerPos	= iConnectionString.AnsiPos(":");

		Server			= iConnectionString.SubString(1, ServerPos-1);
		FileName			= iConnectionString.SubString(ServerPos+1, iConnectionString.Length() - (ServerPos-1));

		int PortPos = Server.AnsiPos("/");
		if (PortPos)
		{
			Port			= StrToIntDef(Server.SubString(PortPos+1, Server.Length() - (PortPos-1)), 3050);
			Server		= Server.SubString(1, PortPos-1);
			DBType		= dtFirebird;
		}
	}
}
//---------------------------------------------------------------------------
AnsiString TDBSettings::ConnectionString()
{
	if (DBType == dtInterbase)
	{
		return Server + ":" + FileName;
	}
	else
	{
		return Server + "/" + IntToStr(Port) + ":" + FileName;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TDBTransaction::TDBTransaction(TIBDatabase *IBDatabase) : fIBTransaction(new TIBTransaction(NULL)),
			sqlGetGeneratorVal(new TIBSQL(NULL))
{
//#ifdef USE_DBX
//	RegisterDbXpressLib(::getSQLDriverINTERBASE);
//	RegisterMidasLib(::DllGetDataSnapClassObject);
//#endif
	fCreate(IBDatabase);
}
//---------------------------------------------------------------------------
TDBTransaction::TDBTransaction(TDBControl &DBControl) : fIBTransaction(new TIBTransaction(NULL)),
			sqlGetGeneratorVal(new TIBSQL(NULL))
{
	fCreate(DBControl);
/*	fIBDatabases.push_back(DBControl);
	fIBTransaction->DefaultDatabase	= fIBDatabases[0];
	fIBTransaction->Params->Add("read_committed");
	fIBTransaction->Params->Add("rec_version");
	fIBTransaction->Params->Add("nowait");

	sqlGetGeneratorVal->Database		= fIBDatabases[0];
	sqlGetGeneratorVal->Transaction	= fIBTransaction.get();*/
}
//---------------------------------------------------------------------------
void TDBTransaction::fCreate(TIBDatabase *IBDatabase)
{
	fIBDatabases.push_back(IBDatabase);
	fIBTransaction->DefaultDatabase	= fIBDatabases[0];
	fIBTransaction->Params->Add("read_committed");
	fIBTransaction->Params->Add("rec_version");
	fIBTransaction->Params->Add("nowait");

	sqlGetGeneratorVal->Database		= fIBDatabases[0];
	sqlGetGeneratorVal->Transaction	= fIBTransaction.get();
}
//---------------------------------------------------------------------------
TDBTransaction::~TDBTransaction()
{
	try
	{
		if (fIBTransaction->InTransaction)
		{
			fIBTransaction->Rollback();
		}
	}
	catch (...)
	{
	}
	for (std::vector<TIBSQL *>::iterator iQ = fQueries.begin(); iQ != fQueries.end(); ++iQ)
	{
		delete (*iQ);
	}
}
//---------------------------------------------------------------------------
int TDBTransaction::AddDatabase(TIBDatabase *IBDatabase)
{
	return fAddDatabase(IBDatabase);
}
//---------------------------------------------------------------------------
int TDBTransaction::AddDatabase(TDBControl &DBControl)
{
	return fAddDatabase(DBControl);
}
//---------------------------------------------------------------------------
int TDBTransaction::fAddDatabase(TIBDatabase *IBDatabase)
{
	fIBDatabases.push_back(IBDatabase);
	fIBTransaction->AddDatabase(IBDatabase);

	return fIBDatabases.size() - 1;
}
//---------------------------------------------------------------------------
TQueryHandle TDBTransaction::AddQuery(int DatabaseIndex)
{
	if (DatabaseIndex > static_cast<int>(fIBDatabases.size()))
	{
		throw Exception("Invalid Database Index: " + IntToStr(DatabaseIndex) + " (Max: " + IntToStr(fIBDatabases.size()) + ")");
	}
	TIBSQL *Query			= new TIBSQL(NULL);
	Query->Database		= fIBDatabases[DatabaseIndex];
	Query->Transaction	= fIBTransaction.get();
	
	fQueries.push_back(Query);
	return fQueries.size() - 1;
}
//---------------------------------------------------------------------------
TIBSQL *TDBTransaction::Query(TQueryHandle Index)
{
	return fQueries[Index];
}
//---------------------------------------------------------------------------
void TDBTransaction::RegisterQuery(TcpIBSQL &Query, int DatabaseIndex)
{
	Query->Database		= fIBDatabases[DatabaseIndex];
	Query->Transaction	= fIBTransaction.get();
}
//---------------------------------------------------------------------------
bool TDBTransaction::Start()
{
	if (!fIBTransaction->InTransaction)
	{
		fIBTransaction->StartTransaction();
	}
	return true;
}
//---------------------------------------------------------------------------
bool TDBTransaction::Commit()
{
	if (fIBTransaction->InTransaction)
	{
		fIBTransaction->Commit();
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool TDBTransaction::Undo()
{
	try
	{
		if (fIBTransaction->InTransaction)
		{
			fIBTransaction->Rollback();
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (Exception &E)
	{
		return false;
	}
}
//---------------------------------------------------------------------------
int TDBTransaction::GetGeneratorVal(AnsiString GeneratorName, int Increment)
{
	bool CommitOnCompetion = (!fIBTransaction->InTransaction);
	StartTransaction();

	sqlGetGeneratorVal->Close();
	if (sqlGetGeneratorVal->SQL->Text != "Select gen_id(" + GeneratorName + "," + Increment + ") From rdb$database")
	{
		// Don't force a 'Prepare' if not necessary.
		sqlGetGeneratorVal->SQL->Text = "Select gen_id(" + GeneratorName + "," + Increment + ") From rdb$database";
	}
	sqlGetGeneratorVal->ExecQuery();
	int Val = sqlGetGeneratorVal->Fields[0]->AsInteger;
	sqlGetGeneratorVal->Close();

	if (CommitOnCompetion)
	{
		CommitTransaction();
	}
	return Val;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TDBControl::TDBControl(TIBDatabase *IBDatabase, TDBSettings const &DBSettings) :
		fOwnDB(false),
		fIBDatabase(IBDatabase)
{
	fDBSettings					= DBSettings;
}
//---------------------------------------------------------------------------
TDBControl::TDBControl(TDBSettings const &DBSettings) :
		fOwnDB(true)
{
	fIBDatabase = new TIBDatabase(NULL);
	try
	{
		fIBDatabase->LoginPrompt			= false;
		fDBSettings								= DBSettings;
	}
	catch (Exception &E)
	{
		delete fIBDatabase;
		fIBDatabase = NULL;
		throw;
	}
}
//---------------------------------------------------------------------------
TDBControl::~TDBControl()
{
	if (fOwnDB)
	{
		try
		{
			Disconnect();
		}
		__finally
		{
			delete fIBDatabase;
			fIBDatabase = NULL;
		}
	}
}
//---------------------------------------------------------------------------
AnsiString TDBControl::ConnectionString()
{
	return fDBSettings.ConnectionString();
}
//---------------------------------------------------------------------------
AnsiString TDBControl::ConnectionServer()
{
	return fDBSettings.Server;
}
//---------------------------------------------------------------------------
AnsiString TDBControl::ConnectionPath()
{
	return ExtractFilePath(fDBSettings.FileName);
}
//---------------------------------------------------------------------------
AnsiString TDBControl::ConnectionFile()
{
	return ExtractFileName(fDBSettings.FileName);
}
//---------------------------------------------------------------------------
void TDBControl::Connect()
{
	if (fIBDatabase->Connected)
	{
		Disconnect();
	}
	SetIBAffinity();
	
	fIBDatabase->Params->Clear();
	fIBDatabase->Params->Add("user_name=" + fDBSettings.UserName);
	fIBDatabase->Params->Add("password=" + fDBSettings.Password);
	if (fDBSettings.CharacterSet != "")
	{
		fIBDatabase->Params->Add("lc_ctype=" + fDBSettings.CharacterSet);
	}
	fIBDatabase->DatabaseName = fDBSettings.ConnectionString();

	fIBDatabase->Connected = true;
}
//---------------------------------------------------------------------------
void TDBControl::Disconnect()
{
	try
	{
		fIBDatabase->Connected = false;
	}
	catch (EIBError &E)
	{
		fIBDatabase->ForceClose();
//		throw;
	}
}
//---------------------------------------------------------------------------
bool TDBControl::WaitForIBServer(unsigned int Timeout)
{
	bool Found = false;
	DWORD CurrentTime = GetTickCount();
	do
	{
		if ((Found = FindWindow("IB_Server", "InterBase Server")) == false)
		{
			Sleep(0);
		}
	}
	while (Timeout != 0 && CurrentTime + Timeout < GetTickCount() && !Found);

	return Found;
}
//---------------------------------------------------------------------------
bool TDBControl::SetIBAffinity()
{
	int Mask = 0;
	if (GetIBAffinity(Mask))
	{
		if (Mask != 1 && Mask != 2 && Mask != 4 && Mask != 8)
		{
			SetIBAffinity(1);
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
HANDLE TDBControl::InitIBAffinity()
{
	if (Win32Platform == VER_PLATFORM_WIN32_NT)
	{
		HANDLE p_hwnd = FindWindow("IB_Server", "InterBase Server");
		if (p_hwnd)
		{
			DWORD p_processid = 0;
			if (GetWindowThreadProcessId(p_hwnd, &p_processid))
			{
				HANDLE own_token = 0;
				if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &own_token))
				{
					if (EnablePrivilege(own_token, "SeDebugPrivilege", true))
					{
						int i = PROCESS_QUERY_INFORMATION | PROCESS_SET_INFORMATION;
						return OpenProcess(i, false, p_processid);
					}
				}
			}
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
bool TDBControl::SetIBAffinity(int Mask)
{
	bool Success = false;

	HANDLE p_processh = InitIBAffinity();
	if (p_processh)
	{
		try
		{
			DWORD	sys_affmask = 0;
			DWORD	process_affmask = 0;

			if (GetProcessAffinityMask(p_processh, &process_affmask, &sys_affmask))
			{
				if (SetProcessAffinityMask(p_processh, Mask & sys_affmask))
				{
					Success = true;
				}
			}
		}
		__finally
		{
			CloseHandle(p_processh);
		}
	}
	return Success;
}
//---------------------------------------------------------------------------
bool TDBControl::GetIBAffinity(int &Mask)
{
	bool Success = false;

	HANDLE p_processh = InitIBAffinity();
	if (p_processh)
	{
		try
		{
			DWORD	sys_affmask = 0;
			DWORD	process_affmask = 0;

			if (GetProcessAffinityMask(p_processh, &process_affmask, &sys_affmask))
			{
				Mask = process_affmask;
				Success = true;
			}
		}
		__finally
		{
			CloseHandle(p_processh);
		}
	}
	return Success;
}
//---------------------------------------------------------------------------
bool TDBControl::EnablePrivilege(HANDLE h, AnsiString AName, bool AState)
{
	TOKEN_PRIVILEGES tp;

	if (!LookupPrivilegeValue(NULL, AName.c_str(), &tp.Privileges[0].Luid)) return false;

	tp.PrivilegeCount = 1;
	if (AState)
	{
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else
	{
		tp.Privileges[0].Attributes = 0;
	}
	return AdjustTokenPrivileges(h, false, &tp, sizeof(tp), NULL, NULL);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TDBUtilities::TDBUtilities(TDBControl &DBControl) :
		fDBControl(DBControl),
		fDBTransaction(DBControl),
		fDatabaseInfo(new TIBDatabaseInfo(NULL)),
		fIBBackupService(new TIBBackupService(NULL)),
		fIBRestoreService(new TIBRestoreService(NULL)),
		fIBValidationService(new TIBValidationService(NULL)),
		fServiceThread(NULL),
		sqlIndexList(new TIBSQL(NULL)),
		sqlIndexType(new TIBSQL(NULL)),
		sqlIndexFields(new TIBSQL(NULL)),
		sqlFieldType(new TIBSQL(NULL)),
		sqlConstraintList(new TIBSQL(NULL)),
		sqlConstraintType(new TIBSQL(NULL))
{
	fDatabaseInfo->Database	= fDBControl;
	OnServiceComplete			= NULL;
	OnServiceStatus			= NULL;
	fUsingServiceThread		= false;

	fDBTransaction.RegisterQuery(sqlIndexList);
	fDBTransaction.RegisterQuery(sqlIndexType);
	fDBTransaction.RegisterQuery(sqlIndexFields);
	fDBTransaction.RegisterQuery(sqlFieldType);
	fDBTransaction.RegisterQuery(sqlConstraintList);
	fDBTransaction.RegisterQuery(sqlConstraintType);

	sqlIndexList->SQL->Text =
		"Select "
			"rdb$index_name "
		"From "
			"rdb$indices "
		"Where "
			"rdb$relation_name = :TableName "
			"And rdb$index_name Not Starting 'rdb$'";

	sqlIndexFields->SQL->Text =
		"Select "
			"Rdb$Index_Segments.Rdb$Field_Name "
		"From "
			"Rdb$Indices Left Join Rdb$Index_Segments on "
				"Rdb$Indices.Rdb$Index_Name = Rdb$Index_Segments.Rdb$Index_Name "
		"Where "
			"Rdb$Indices.Rdb$Index_Name = :IndexName";

	sqlIndexType->SQL->Text =
		"Select "
			"i.Rdb$Relation_Name,"
			"i.Rdb$Unique_Flag,"
			"i.Rdb$Foreign_Key, "
			"j.Rdb$Relation_Name Foreign_Relation_Name "
		"From "
			"Rdb$Indices i Left Join Rdb$Indices j On "
				"i.Rdb$Foreign_Key = j.Rdb$Index_Name "
		"Where "
			"i.Rdb$Index_Name = :IndexName";

	sqlFieldType->SQL->Text =
		"Select "
			"Rdb$Fields.Rdb$Field_Type,"
			"Rdb$Fields.Rdb$Field_Length,"
			"Rdb$Fields.Rdb$Field_Scale,"
			"Rdb$Fields.Rdb$Field_Precision "
		"From "
			"Rdb$Relation_Fields Left Join Rdb$Fields On "
			"Rdb$Relation_Fields.Rdb$Field_Source = Rdb$Fields.Rdb$Field_Name "
		"Where "
			"Rdb$Relation_Fields.Rdb$Relation_Name = :TableName And "
			"Rdb$Relation_Fields.rdb$Field_Name = :FieldName";

	sqlConstraintList->SQL->Text =
		"Select "
			"Rdb$Constraint_Name "
		"From "
			"Rdb$Relation_Constraints "
		"Where "
			"Rdb$Relation_Name = :TableName";

	sqlConstraintType->SQL->Text =
/*		"Select "
			"Rdb$Constraint_Type,"
			"Rdb$Relation_Name,"
			"Rdb$Index_Name "
		"From "
			"Rdb$Relation_Constraints "
		"Where "
			"Rdb$Constraint_Name = :ConstraintName";*/
		"Select "
			"Rdb$Relation_Constraints.Rdb$Constraint_Type,"
			"Rdb$Relation_Constraints.Rdb$Relation_Name,"
			"Rdb$Check_Constraints.Rdb$Trigger_Name,"
			"Rdb$Relation_Constraints.Rdb$Index_Name,"
			"j.Rdb$Relation_Name Foreign_Relation_Name "
		"From "
			"Rdb$Relation_Constraints Left Join Rdb$Indices i On "
				"Rdb$Relation_Constraints.Rdb$Index_Name = i.Rdb$Index_Name "
			"Left Join Rdb$Indices j On "
				"i.Rdb$Foreign_Key = j.Rdb$Index_Name "
			"Left Join Rdb$Check_Constraints On "
				"Rdb$Check_Constraints.Rdb$Constraint_Name = Rdb$Relation_Constraints.Rdb$Constraint_Name "
		"Where "
			"Rdb$Relation_Constraints.Rdb$Constraint_Name = :ConstraintName";
}
//---------------------------------------------------------------------------

TDBUtilities::~TDBUtilities()
{
}
//---------------------------------------------------------------------------
void TDBUtilities::GetTableNames(TTableNames &TableNames)
{
	TableNames.clear();
	std::auto_ptr<TStringList> Strings(new TStringList);
	fDBControl.fIBDatabase->GetTableNames(Strings.get(), false);
	for (int i=0; i<Strings->Count; i++) TableNames.push_back(Strings->Strings[i]);
}
//---------------------------------------------------------------------------
void TDBUtilities::GetFieldNames(AnsiString TableName, TFieldNames &FieldNames)
{
	FieldNames.clear();
	std::auto_ptr<TStringList> Strings(new TStringList);
	fDBControl.fIBDatabase->GetFieldNames(TableName, Strings.get());
	for (int i=0; i<Strings->Count; i++) FieldNames.push_back(Strings->Strings[i]);
}
//---------------------------------------------------------------------------
void TDBUtilities::GetFieldDefinition(AnsiString TableName, AnsiString FieldName, TFieldDefinition &FieldDefinition)
{
	fDBTransaction.StartTransaction();
	try
	{
		sqlFieldType->Close();
		sqlFieldType->ParamByName("FieldName")->AsString = FieldName;
		sqlFieldType->ParamByName("TableName")->AsString = TableName;
		sqlFieldType->ExecQuery();

		FieldDefinition.FieldType	= static_cast<TIBFieldType>(sqlFieldType->FieldByName("Rdb$Field_Type")->AsInteger);
		FieldDefinition.Length		= sqlFieldType->FieldByName("Rdb$Field_Length")->AsInteger;
		FieldDefinition.Scale		= sqlFieldType->FieldByName("Rdb$Field_Scale")->AsInteger;
		FieldDefinition.Precision	= sqlFieldType->FieldByName("Rdb$Field_Precision")->AsInteger;

		sqlFieldType->Close();
	}
	__finally
	{
		fDBTransaction.CommitTransaction();
	}
}
//---------------------------------------------------------------------------
void TDBUtilities::GetIndexNames(AnsiString TableName, TIndexNames &IndexNames)
{
	IndexNames.clear();
	fDBTransaction.StartTransaction();
	try
	{
		sqlIndexList->Close();
		sqlIndexList->ParamByName("TableName")->AsString = TableName;
		for (sqlIndexList->ExecQuery(); !sqlIndexList->Eof; sqlIndexList->Next())
		{
			IndexNames.insert(sqlIndexList->Fields[0]->AsString);
		}
	}
	__finally
	{
		fDBTransaction.CommitTransaction();
	}
}
//---------------------------------------------------------------------------
void TDBUtilities::GetIndexDefinition(AnsiString IndexName, TIndexDefinition &IndexDefinition)
{
	fDBTransaction.StartTransaction();
	try
	{
		sqlIndexType->Close();
		sqlIndexType->ParamByName("IndexName")->AsString = IndexName;
		sqlIndexType->ExecQuery();

		IndexDefinition.TableName		= sqlIndexType->FieldByName("Rdb$Relation_Name")->AsString;
		IndexDefinition.Unique			= (sqlIndexType->FieldByName("Rdb$Unique_Flag")->AsInteger == 1);
		IndexDefinition.ForeignKey		= sqlIndexType->FieldByName("Rdb$Foreign_Key")->AsString;
		IndexDefinition.ForeignTable	= sqlIndexType->FieldByName("Foreign_Relation_Name")->AsString;

		sqlIndexType->Close();
	}
	__finally
	{
		fDBTransaction.CommitTransaction();
	}
}
//---------------------------------------------------------------------------
void TDBUtilities::GetIndexFields(AnsiString IndexName, TIndexFields &IndexFields)
{
	IndexFields.clear();
	fDBTransaction.StartTransaction();
	try
	{
		sqlIndexFields->Close();
		sqlIndexFields->ParamByName("IndexName")->AsString = IndexName;
		for (sqlIndexFields->ExecQuery(); !sqlIndexFields->Eof; sqlIndexFields->Next())
		{
			IndexFields.insert(sqlIndexFields->Fields[0]->AsString);
		}
	}
	__finally
	{
		fDBTransaction.CommitTransaction();
	}
}
//---------------------------------------------------------------------------
void TDBUtilities::GetConstraintNames(AnsiString TableName, TConstraintNames &ConstraintNames)
{
	ConstraintNames.clear();
	fDBTransaction.StartTransaction();
	try
	{
		sqlConstraintList->Close();
		sqlConstraintList->ParamByName("TableName")->AsString = TableName;
		for (sqlConstraintList->ExecQuery(); !sqlConstraintList->Eof; sqlConstraintList->Next())
		{
			ConstraintNames.insert(sqlConstraintList->Fields[0]->AsString);
		}
	}
	__finally
	{
		fDBTransaction.CommitTransaction();
	}
}
//---------------------------------------------------------------------------
void TDBUtilities::GetConstraintDefinition(AnsiString ConstraintName, TConstraintDefinition &ConstraintDefinition)
{
	fDBTransaction.StartTransaction();
	try
	{
		sqlConstraintType->Close();
		sqlConstraintType->ParamByName("ConstraintName")->AsString = ConstraintName;
		sqlConstraintType->ExecQuery();

		ConstraintDefinition.TableName		= sqlConstraintType->FieldByName("Rdb$Relation_Name")->AsString;
		ConstraintDefinition.ConstraintType	= sqlConstraintType->FieldByName("Rdb$Constraint_Type")->AsString;
		ConstraintDefinition.FieldName		= sqlConstraintType->FieldByName("Rdb$Trigger_Name")->AsString;
		ConstraintDefinition.IndexName		= sqlConstraintType->FieldByName("Rdb$Index_Name")->AsString;
		ConstraintDefinition.ForeignTable	= sqlConstraintType->FieldByName("Foreign_Relation_Name")->AsString;

		sqlConstraintType->Close();
	}
	__finally
	{
		fDBTransaction.CommitTransaction();
	}
}
//---------------------------------------------------------------------------
bool TDBUtilities::CreateDatabase(int PageSize)
{
	fDBControl.fIBDatabase->Connected = false;
	fDBControl.fIBDatabase->DatabaseName = fDBControl.fDBSettings.ConnectionString();
	fDBControl.fIBDatabase->Params->Clear();
	fDBControl.fIBDatabase->Params->Append("USER '" + fDBControl.fDBSettings.UserName + "'");
	fDBControl.fIBDatabase->Params->Append("PASSWORD '" + fDBControl.fDBSettings.Password + "'");
	fDBControl.fIBDatabase->Params->Append("PAGE_SIZE " + IntToStr(PageSize));
	if (fDBControl.fDBSettings.CharacterSet != "")
	{
		fDBControl.fIBDatabase->Params->Append("DEFAULT CHARACTER SET \"" + fDBControl.fDBSettings.CharacterSet + "\"");
	}
	fDBControl.fIBDatabase->LoginPrompt = false;

	fDBControl.fIBDatabase->CreateDatabase();

	return true;
}
//---------------------------------------------------------------------------
bool TDBUtilities::SetForcedWrites()
{
	if (!fDBControl.fIBDatabase->Connected)
	{
		fDBControl.Connect();
	}
	if (!fDatabaseInfo->ForcedWrites)
	{
		std::auto_ptr<TIBConfigService> IBConfigService(new TIBConfigService(NULL));

		IBConfigService->Params->Add("user_name=" + fDBControl.fDBSettings.UserName);
		IBConfigService->Params->Add("password=" + fDBControl.fDBSettings.Password);
		IBConfigService->ServerName	= fDatabaseInfo->DBSiteName;
		IBConfigService->DatabaseName	= fDatabaseInfo->DBFileName;
		IBConfigService->LoginPrompt	= false;
		IBConfigService->Protocol		= fDBControl.fDBSettings.Protocol;

		if (fDatabaseInfo->UserNames->Count == 1)
		{
			fDBControl.Disconnect();

			IBConfigService->Attach();

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
			IBConfigService->Detach();
		}
	}
	return true;
}
//---------------------------------------------------------------------------
bool TDBUtilities::StartSweep()
{
	if (!fUsingServiceThread)
	{
		if (!fDBControl.fIBDatabase->Connected)
		{
			fDBControl.Connect();
		}
		fIBValidationService->Protocol		= fDBControl.fDBSettings.Protocol;
		fIBValidationService->ServerName		= fDatabaseInfo->DBSiteName;
		fIBValidationService->DatabaseName	= fDatabaseInfo->DBFileName;
		fIBValidationService->Options			= TValidateOptions() << SweepDB;
		fIBValidationService->LoginPrompt	= false;
		fIBValidationService->Params->Add("user_name=" + fDBControl.fDBSettings.UserName);
		fIBValidationService->Params->Add("password=" + fDBControl.fDBSettings.Password);

		StartServiceThread(fIBValidationService.get(), stSweep);

		return true;
	}
	else
	{
		throw Exception("Service is busy");
	}
}
//---------------------------------------------------------------------------
bool TDBUtilities::StartRepairCheck()
{
	if (!fUsingServiceThread)
	{
		if (fDBControl.fIBDatabase->Connected)
		{
			fDBControl.Disconnect();
		}
		fIBValidationService->Protocol		= fDBControl.fDBSettings.Protocol;
		fIBValidationService->ServerName		= fDBControl.ConnectionServer();
		fIBValidationService->DatabaseName	= IncludeTrailingBackslash(fDBControl.ConnectionPath()) + fDBControl.ConnectionFile();
		fIBValidationService->Options			= TValidateOptions() << IgnoreChecksum << CheckDB << ValidateFull << ValidateDB;
		fIBValidationService->LoginPrompt	= false;
		fIBValidationService->Params->Add("user_name=" + fDBControl.fDBSettings.UserName);
		fIBValidationService->Params->Add("password=" + fDBControl.fDBSettings.Password);

		StartServiceThread(fIBValidationService.get(), stRepairCheck);

		return true;
	}
	else
	{
		throw Exception("Service is busy");
	}
}
//---------------------------------------------------------------------------
bool TDBUtilities::StartRepair()
{
	if (!fUsingServiceThread)
	{
		if (fDBControl.fIBDatabase->Connected)
		{
			fDBControl.Disconnect();
		}
		return StartRepair(fDBControl.ConnectionServer(), IncludeTrailingBackslash(fDBControl.ConnectionPath()) + fDBControl.ConnectionFile());
	}
	else
	{
		throw Exception("Service is busy");
	}
}
//---------------------------------------------------------------------------
bool TDBUtilities::StartRepair(AnsiString ServerName, AnsiString FileName)
{
	if (!fUsingServiceThread)
	{
		fIBValidationService->Protocol		= fDBControl.fDBSettings.Protocol;
		fIBValidationService->ServerName		= ServerName;
		fIBValidationService->DatabaseName	= FileName;
		fIBValidationService->Options			= TValidateOptions() << IgnoreChecksum << MendDB;// << ValidateFull << ValidateDB;
		fIBValidationService->LoginPrompt	= false;
		fIBValidationService->Params->Add("user_name=" + fDBControl.fDBSettings.UserName);
		fIBValidationService->Params->Add("password=" + fDBControl.fDBSettings.Password);

		StartServiceThread(fIBValidationService.get(), stRepair);

		return true;
	}
	else
	{
		throw Exception("Service is busy");
	}
}
//---------------------------------------------------------------------------
bool TDBUtilities::StartBackup(AnsiString ServerName, AnsiString SourceFile, AnsiString DestFile, bool Verbose, TBackupOptions const &BackupOptions)
{
	if (!fUsingServiceThread)
	{
		fDBControl.Disconnect();

		fIBBackupService->Active					= false;
		fIBBackupService->ServerName				= ServerName;
		fIBBackupService->Protocol					= fDBControl.fDBSettings.Protocol;
		fIBBackupService->LoginPrompt				= false;
		fIBBackupService->Params->Clear();
		fIBBackupService->Params->Add("user_name=" + fDBControl.fDBSettings.UserName);
		fIBBackupService->Params->Add("password=" + fDBControl.fDBSettings.Password);

		fIBBackupService->Options					= BackupOptions;
		fIBBackupService->Verbose					= Verbose;
		fIBBackupService->DatabaseName			= SourceFile;
		fIBBackupService->BackupFile->Text		= DestFile;;

		StartServiceThread(fIBBackupService.get(), stBackup);

		return true;
	}
	else
	{
		throw Exception("Service is busy");
	}
}
//---------------------------------------------------------------------------
bool TDBUtilities::StartBackup(AnsiString DestFilePath, AnsiString DestFileName, bool Verbose, TBackupOptions const &BackupOptions)
{
	return StartBackup(
			fDBControl.ConnectionServer(),
			IncludeTrailingBackslash(fDBControl.ConnectionPath()) + fDBControl.ConnectionFile(),
			IncludeTrailingBackslash(DestFilePath) + DestFileName,
			Verbose);
}
//---------------------------------------------------------------------------
bool TDBUtilities::StartBackup(AnsiString DestFileName, bool Verbose, TBackupOptions const &BackupOptions)
{
	return StartBackup(
			fDBControl.ConnectionServer(),
			IncludeTrailingBackslash(fDBControl.ConnectionPath()) + fDBControl.ConnectionFile(),
			IncludeTrailingBackslash(fDBControl.ConnectionPath()) + DestFileName,
			Verbose);
}
//---------------------------------------------------------------------------
bool TDBUtilities::StartRestore(AnsiString ServerName, AnsiString SourceFile, AnsiString DestFile, bool Verbose, bool ReplaceDB)
{
	if (!fUsingServiceThread)
	{
		TRestoreOptions RestoreOptions = TRestoreOptions() << CreateNewDB;
		if (ReplaceDB)
		{
			RestoreOptions = RestoreOptions << Replace;
		}
		return StartRestore(ServerName, SourceFile, DestFile, Verbose, RestoreOptions);
/*


		fDBControl.Disconnect();

		fIBRestoreService->Active					= false;
		fIBRestoreService->ServerName				= ServerName;
		fIBRestoreService->Protocol				= fDBControl.fDBSettings.Protocol;
		fIBRestoreService->Verbose					= Verbose;
		fIBRestoreService->DatabaseName->Text	= DestFile;
		fIBRestoreService->BackupFile->Text		= SourceFile;
		fIBRestoreService->LoginPrompt			= false;
		fIBRestoreService->Params->Clear();
		fIBRestoreService->Params->Add("user_name=" + fDBControl.fDBSettings.UserName);
		fIBRestoreService->Params->Add("password=" + fDBControl.fDBSettings.Password);
		fIBRestoreService->Options.Clear();
		if (ReplaceDB)
		{
			fIBRestoreService->Options = RestoreOptions() << Replace << CreateNewDB;
		}
		else
		{
			fIBRestoreService->Options = TRestoreOptions() << CreateNewDB;
		}
		StartServiceThread(fIBRestoreService.get(), stRestore);

		return true;*/
	}
	else
	{
		throw Exception("Service is busy");
	}
}
//---------------------------------------------------------------------------
bool TDBUtilities::StartRestore(AnsiString ServerName, AnsiString SourceFile, AnsiString DestFile, bool Verbose,
			TRestoreOptions const &RestoreOptions)
{
	if (!fUsingServiceThread)
	{
		fDBControl.Disconnect();

		fIBRestoreService->Active					= false;
		fIBRestoreService->ServerName				= ServerName;
		fIBRestoreService->Protocol				= fDBControl.fDBSettings.Protocol;
		fIBRestoreService->LoginPrompt			= false;
		fIBRestoreService->Params->Clear();
		fIBRestoreService->Params->Add("user_name=" + fDBControl.fDBSettings.UserName);
		fIBRestoreService->Params->Add("password=" + fDBControl.fDBSettings.Password);

		fIBRestoreService->Options					= RestoreOptions;
		fIBRestoreService->Verbose					= Verbose;
		fIBRestoreService->DatabaseName->Text	= DestFile;
		fIBRestoreService->BackupFile->Text		= SourceFile;

		StartServiceThread(fIBRestoreService.get(), stRestore);

		return true;
	}
	else
	{
		throw Exception("Service is busy");
	}
}
//---------------------------------------------------------------------------
void TDBUtilities::StartServiceThread(TIBControlAndQueryService *IBService, TIBServiceType ServiceType)
{
	fServiceThread							= new TServiceThread(IBService, OnServiceStatus, OnServiceComplete, ServiceType);
	fUsingServiceThread					= true;
	fServiceThread->FreeOnTerminate	= true;
	fServiceThread->OnTerminate		= fServiceTerminate;
	fServiceThread->Priority			= tpLowest;
	fServiceThread->Resume();
}
//---------------------------------------------------------------------------
void __fastcall TDBUtilities::fServiceTerminate(TObject *Sender)
{
/*	if (OnServiceComplete)
	{
		OnServiceComplete(fServiceThread->ServiceSuccess(), fServiceThread->ErrorMsg(), fServiceThread->ServiceType());
	}*/
	fUsingServiceThread = false;
}
//---------------------------------------------------------------------------
void TDBUtilities::CancelService()
{
	if (fUsingServiceThread && fServiceThread)
	{
		fServiceThread->Cancel();
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TDBUtilities::TServiceThread::TServiceThread(TIBControlAndQueryService *IBService,
											TServiceGetStatus OnGetStatus,
											TServiceCompleteEvent OnServiceComplete,
											TIBServiceType ServiceType)
	: TThread(true)
{
	fServiceSuccess		= false;
	fIBService				= IBService;
	fOnServiceStatus		= OnGetStatus;
	fOnServiceComplete	= OnServiceComplete;
	fServiceType			= ServiceType;
}
//---------------------------------------------------------------------------
void TDBUtilities::TServiceThread::Cancel()
{
	Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TDBUtilities::TServiceThread::Execute()
{
	try
	{
		fServiceSuccess = false;
		try
		{
			fIBService->Attach();
			fIBService->ServiceStart();
			Sleep(100);
			do
			{
				Sleep(0);
//				if (!Terminated) Synchronize(GetComplete);
//				if (!Terminated && !fComplete) Synchronize(SetStatusText);
//				if (!Terminated) GetComplete();
				if (!Terminated && !fIBService->Eof)
				{
					fStatusText = fIBService->GetNextLine();
					if (!fIBService->Eof)
					{
						Synchronize(SetStatusText);
					}
				}
			}
			while (!fIBService->Eof && !Terminated);

			while (fIBService->IsServiceRunning && !Terminated)
			{
				Sleep(100);
			}
		}
		__finally
		{
			if (fIBService->Active)
			{
				fIBService->Detach();
			}
		}
		fServiceSuccess = true;
	}
	catch (Exception &E)
	{
		fErrorMsg = E.Message;
	}
	try
	{
		Synchronize(ServiceComplete);
	}
	catch (...)
	{
	}
}
//---------------------------------------------------------------------------
void __fastcall TDBUtilities::TServiceThread::SetStatusText()
{
/*	AnsiString Text = fIBService->GetNextLine();
	if (!fIBService->Eof)
	{
		if (fOnServiceStatus)// && fIBService->Verbose)
		{
			fOnServiceStatus(Text, fServiceType);
		}
	}*/
	if (fOnServiceStatus)// && fIBService->Verbose)
	{
		fOnServiceStatus(fStatusText, fServiceType);
	}
}
//---------------------------------------------------------------------------
/*void __fastcall TDBUtilities::TServiceThread::GetComplete()
{
	fComplete = fIBService->Eof;
} */
//---------------------------------------------------------------------------
void __fastcall TDBUtilities::TServiceThread::ServiceComplete()
{
	if (fOnServiceComplete)
	{
		fOnServiceComplete(fServiceSuccess, fErrorMsg, fServiceType);
	}
}
//---------------------------------------------------------------------------

