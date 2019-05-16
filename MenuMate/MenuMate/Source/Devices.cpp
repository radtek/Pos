//---------------------------------------------------------------------------


#pragma hdrstop

#include "Devices.h"
#include "MMLogging.h"
#include <Memory>
#include <set>
//---------------------------------------------------------------------------

#pragma package(smart_init)


TManagerDevices::TManagerDevices()
{
	fEnabled = true;
}

TManagerDevices::~TManagerDevices()
{
}

void __fastcall TManagerDevices::SetEnabled(bool value)
{
   if (fEnabled != value)
	{
		fEnabled = value;
	}
}

bool __fastcall TManagerDevices::GetEnabled()
{
	return fEnabled;
}


AnsiString TManagerDevices::GetDeviceName(Database::TDBTransaction &DBTransaction,long DeviceKey)
{
	AnsiString RetVal = "";
	if(!Enabled) return RetVal;

	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
			"DEVICE_NAME "
		"FROM "
			"DEVICES "
		"WHERE "
			"DEVICE_KEY = :DEVICE_KEY";
		IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("DEVICE_NAME")->AsString;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

void TManagerDevices::GetDeviceList(Database::TDBTransaction &DBTransaction,int DeviceType,TStringList *Devices)
{
	if(!Enabled) return;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
			"DEVICE_KEY,DEVICE_NAME,PRODUCT "
		"FROM "
			"DEVICES "
		"WHERE "
			"DEVICE_TYPE IN (:DEVICE_TYPE, :APP_TYPE) ";
		IBInternalQuery->ParamByName("DEVICE_TYPE")->AsInteger = DeviceType;
        IBInternalQuery->ParamByName("APP_TYPE")->AsInteger = 7;
		IBInternalQuery->ExecQuery();

		for (;!IBInternalQuery->Eof ; IBInternalQuery->Next())
		{
			Devices->AddObject(IBInternalQuery->FieldByName("DEVICE_NAME")->AsString +
									 " [" + IBInternalQuery->FieldByName("PRODUCT")->AsString + "]",
									(System::TObject*)(IBInternalQuery->FieldByName("DEVICE_KEY")->AsInteger));
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}


void TManagerDevices::GetPrinterProfilesList(Database::TDBTransaction &DBTransaction,long DeviceKey, std::set<__int64> &DevicePrinterProfiles)
{
	if(!Enabled) return;

	try
	{
		DevicePrinterProfiles.clear();

		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
			"VIRTUALPRINTER_KEY "
		"FROM "
			"DEVICEVIRTUALPRINTER "
		"WHERE "
			"DEVICE_KEY = :DEVICE_KEY";
		IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
		IBInternalQuery->ExecQuery();

		for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
			DevicePrinterProfiles.insert(IBInternalQuery->FieldByName("VIRTUALPRINTER_KEY")->AsInteger);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TManagerDevices::GetChitProfilesList(Database::TDBTransaction &DBTransaction,long DeviceKey, typChitDefaultMap &DeviceChits)
{
	if(!Enabled) return;

	try
	{
		DeviceChits.clear();

		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
			" CHITNUMBER_KEY,DEFAULT_CHIT "
		"FROM "
			"DEVICECHITS "
		"WHERE "
			"DEVICE_KEY = :DEVICE_KEY";
		IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
		IBInternalQuery->ExecQuery();

		for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
			DeviceChits[IBInternalQuery->FieldByName("CHITNUMBER_KEY")->AsInteger] = ((IBInternalQuery->FieldByName("DEFAULT_CHIT")->AsInteger == 1) ? true : false);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TManagerDevices::SetChitProfilesList(Database::TDBTransaction &DBTransaction,long DeviceKey, const typChitDefaultMap &DeviceChitProfiles)
{
	if(!Enabled) return;

	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"DELETE FROM "
			"DEVICECHITS "
		"WHERE "
			"DEVICE_KEY = :DEVICE_KEY";
		IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
		IBInternalQuery->ExecQuery();

		for(typChitDefaultMap::const_iterator itProfile = DeviceChitProfiles.begin(); itProfile != DeviceChitProfiles.end(); advance(itProfile,1) )
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"INSERT INTO DEVICECHITS ("
					"DEVICECHITS_KEY,"
					"DEVICE_KEY,"
                    "CHITNUMBER_KEY,"
					"DEFAULT_CHIT) "
				"VALUES ("
					"(SELECT GEN_ID(GEN_DEVICECHITS, 1) FROM RDB$DATABASE),"
					":DEVICE_KEY,"
					":CHITNUMBER_KEY,"
                    ":DEFAULT_CHIT);";
			IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
			IBInternalQuery->ParamByName("CHITNUMBER_KEY")->AsInteger = itProfile->first;
			IBInternalQuery->ParamByName("DEFAULT_CHIT")->AsBoolean = itProfile->second;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TManagerDevices::SetPrinterProfilesList(Database::TDBTransaction &DBTransaction,long DeviceKey, const std::set<__int64> &DevicePrinterProfiles)
{
	if(!Enabled) return;

	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"DELETE FROM "
			"DEVICEVIRTUALPRINTER "
		"WHERE "
			"DEVICE_KEY = :DEVICE_KEY";
		IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
		IBInternalQuery->ExecQuery();

		for(std::set<__int64>::const_iterator itProfile = DevicePrinterProfiles.begin(); itProfile != DevicePrinterProfiles.end();	advance(itProfile,1) )
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"INSERT INTO DEVICEVIRTUALPRINTER ("
					"DEVICEVIRTUALPRINTER_KEY,"
					"VIRTUALPRINTER_KEY,"
					"DEVICE_KEY) "
				"VALUES ("
					"(SELECT GEN_ID(GEN_DEVICEVIRTUALPRINTER, 1) FROM RDB$DATABASE),"
					":VIRTUALPRINTER_KEY,"
					":DEVICE_KEY);";
			IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = *itProfile;
			IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TManagerDevices::GetDeviceNameList(Database::TDBTransaction &DBTransaction,int DeviceType,TStringList *Devices)
{
	if(!Enabled) return;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
			"DEVICE_KEY,DEVICE_NAME "
		"FROM "
			"DEVICES "
		"WHERE "
			"DEVICE_TYPE = :DEVICE_TYPE";
		IBInternalQuery->ParamByName("DEVICE_TYPE")->AsInteger = DeviceType;
		IBInternalQuery->ExecQuery();

		for (;!IBInternalQuery->Eof ; IBInternalQuery->Next())
		{
            UnicodeString name = IBInternalQuery->FieldByName("DEVICE_NAME")->AsString;
			Devices->AddObject(name, (System::TObject*)(IBInternalQuery->FieldByName("DEVICE_KEY")->AsInteger));
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

int TManagerDevices::GetDevicType(Database::TDBTransaction &DBTransaction,long DeviceKey)
{
	int RetVal = 0;
	if(!Enabled) return RetVal;

	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
			"DEVICE_TYPE "
		"FROM "
			"DEVICES "
		"WHERE "
			"DEVICE_KEY = :DEVICE_KEY";
		IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("DEVICE_TYPE")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}
