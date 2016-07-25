//---------------------------------------------------------------------------


#pragma hdrstop

#include "Enum.h"
#include "DeviceDB.h"
#include "MMLogging.h"
#include "MMMessageBox.h"
#include "ManagerVariable.h"
#include "MMRegistry.h"
#include "OpSys.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TDeviceDB::TDeviceDB()
{
    //VM = Vars.get();

	char CompName[MAX_COMPUTERNAME_LENGTH];
	DWORD Size = MAX_COMPUTERNAME_LENGTH + 1;
	if (!GetComputerName(CompName,&Size))
	{
		TManagerLogs::Instance().AddLastError(ERRORLOG);
	}
	ID.ComputerName = CompName;

	ID.Name = "";
	RegistryRead(MMBaseKey ,"TerminalName",ID.Name);
	if(ID.Name == "")
	{
		ID.Name = ID.ComputerName;
   }
   RegistryWrite(MMBaseKey ,"TerminalName",ID.Name);
   #ifdef  PalmMate
		ID.Product = "PalmMate";
   #endif
	#ifdef MenuMate
		ID.Product = "MenuMate";
   #endif
	#ifdef Kiosk
		ID.Product = "Kiosk";
   #endif
}

//---------------------------------------------------------------------------
void TDeviceDB::Initialise(Database::TDBTransaction &DBTransaction)
{
	TDeviceImage::Initialise();
}

__fastcall TDeviceDB::~TDeviceDB()
{
//   delete PaymentSystem;
}


//---------------------------------------------------------------------------




int TDeviceDB::Locate(Database::TDBTransaction &DBTransaction)
{
	int  DeviceKey = 0;
   try
   {

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "SELECT "
         "DEVICE_KEY "
      "FROM "
         "DEVICES "
      "WHERE "
         "DEVICE_NAME = :DEVICE_NAME AND "
         "PRODUCT = :PRODUCT";
      IBInternalQuery->ParamByName("DEVICE_NAME")->AsString = this->ID.Name;
      IBInternalQuery->ParamByName("PRODUCT")->AsString = this->ID.Product;
      IBInternalQuery->ExecQuery();

      if(IBInternalQuery->RecordCount)
      {
			ID.DeviceKey = IBInternalQuery->FieldByName("DEVICE_KEY")->AsInteger;
			DeviceKey = ID.DeviceKey;
		}
		else
		{
			Status.Error = Err_DeviceKey_Not_Found;
		}
	}
   catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		DeviceKey = 0;
   }
   return DeviceKey;
}

//---------------------------------------------------------------------------

int TDeviceDB::NameToKey(Database::TDBTransaction &DBTransaction,UnicodeString DeviceName)
{
	int  DeviceKey = 0;
   try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
			"DEVICE_KEY "
		"FROM "
			"DEVICES "
		"WHERE "
			"DEVICE_NAME = :DEVICE_NAME";
		IBInternalQuery->ParamByName("DEVICE_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			this->ID.DeviceKey = IBInternalQuery->FieldByName("DEVICE_KEY")->AsInteger;
			DeviceKey = this->ID.DeviceKey;
		}
		else
		{
			this->Status.Error = Err_DeviceKey_Not_Found;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		DeviceKey = 0;
	}
   return DeviceKey;
}

//---------------------------------------------------------------------------

int TDeviceDB::IPToKey(Database::TDBTransaction &DBTransaction,UnicodeString DeviceIP)
{
	int  DeviceKey = 0;
   try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "SELECT "
         "DEVICE_KEY "
      "FROM "
         "DEVICES "
      "WHERE "
         "IP = :IP";
      IBInternalQuery->ParamByName("IP")->AsString = DeviceIP;
      IBInternalQuery->ExecQuery();

      if(IBInternalQuery->RecordCount)
      {
         this->ID.DeviceKey = IBInternalQuery->FieldByName("DEVICE_KEY")->AsInteger;
			DeviceKey = this->ID.DeviceKey;
      }
      else
      {
		   this->Status.Error = Err_DeviceKey_Not_Found;
		}
	}
	catch(Exception &E)
	{
		DeviceKey = 0;
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
	return DeviceKey;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TDeviceDB::Create(Database::TDBTransaction &DBTransaction)
{
	try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
         "UPDATE DEVICES "
			"SET "
            "DEVICE_NAME = :DEVICE_NAME, "
            "PRODUCT = :PRODUCT, "
            "DEVICE_ID = :DEVICE_ID, "
				"DEVICE_TYPE = :DEVICE_TYPE, "
				"LOCATION_KEY = :LOCATION_KEY, "
            "PROFILE_KEY = :PROFILE_KEY, "
            "IP = :IP "
         "WHERE "
            "DEVICE_KEY	= :DEVICE_KEY";
      IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = this->ID.DeviceKey;
      IBInternalQuery->ParamByName("DEVICE_NAME")->AsString = this->ID.Name;
      IBInternalQuery->ParamByName("PRODUCT")->AsString = this->ID.Product;
      IBInternalQuery->ParamByName("DEVICE_ID")->AsInteger = 0;
      IBInternalQuery->ParamByName("DEVICE_TYPE")->AsInteger = this->ID.Type;
      IBInternalQuery->ParamByName("LOCATION_KEY")->AsInteger = this->ID.LocationKey;
      IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = this->ID.ProfileKey;
		IBInternalQuery->ParamByName("IP")->AsString = this->ID.IP;
		IBInternalQuery->ExecQuery();

      if(IBInternalQuery->RowsAffected <= 0)
      {
         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DEVICES, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			ID.DeviceKey = IBInternalQuery->Fields[0]->AsInteger;

         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
			"INSERT INTO DEVICES ("
            "DEVICE_KEY,"
            "DEVICE_NAME,"
            "PRODUCT,"
            "DEVICE_ID,"
            "DEVICE_TYPE,"
            "LOCATION_KEY,"
            "PROFILE_KEY,"
            "IP) "
         "VALUES ("
            ":DEVICE_KEY,"
            ":DEVICE_NAME,"
				":PRODUCT,"
            ":DEVICE_ID,"
				":DEVICE_TYPE,"
            ":LOCATION_KEY,"
            ":PROFILE_KEY,"
            ":IP);";
         IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = this->ID.DeviceKey;
			IBInternalQuery->ParamByName("DEVICE_NAME")->AsString = this->ID.Name;
         IBInternalQuery->ParamByName("PRODUCT")->AsString = this->ID.Product;
         IBInternalQuery->ParamByName("DEVICE_ID")->AsInteger = 0;
         IBInternalQuery->ParamByName("DEVICE_TYPE")->AsInteger = this->ID.Type;
			IBInternalQuery->ParamByName("LOCATION_KEY")->AsInteger = this->ID.LocationKey;
			IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = this->ID.ProfileKey;
			IBInternalQuery->ParamByName("IP")->AsString = this->ID.IP;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
		throw;
	}
}

void TDeviceDB::Destroy(Database::TDBTransaction &DBTransaction)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"DELETE FROM DEVICES "
			"WHERE "
				"DEVICE_KEY = :DEVICE_KEY";
		IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = this->ID.DeviceKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
		throw;
   }
}

void TDeviceDB::Load(Database::TDBTransaction &DBTransaction)
{

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
		"SELECT "
         "DEVICE_KEY,IP,DEVICE_ID,DEVICE_TYPE,TRANSNO,DEVICE_NAME,"
         "LOCATIONS.LOCATION_KEY,LOCATIONS.NAME,DEVICES.PROFILE_KEY "
      "FROM "
	      "DEVICES LEFT JOIN LOCATIONS ON DEVICES.LOCATION_KEY = LOCATIONS.LOCATION_KEY "
      "WHERE "
         "DEVICE_KEY = :DEVICE_KEY";
      IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = ID.DeviceKey;
      IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
      {
         this->ID.IP   = IBInternalQuery->FieldByName("IP")->AsString;
			this->ID.Type = (DeviceType)IBInternalQuery->FieldByName("DEVICE_TYPE")->AsInteger;
         this->ID.LocationKey = IBInternalQuery->FieldByName("LOCATION_KEY")->AsInteger;
         this->Status.TransactionNumber = IBInternalQuery->FieldByName("TRANSNO")->AsInteger;
         this->ID.Location = IBInternalQuery->FieldByName("NAME")->AsString;
         this->ID.LocationKey = IBInternalQuery->FieldByName("LOCATION_KEY")->AsInteger;
			this->ID.ProfileKey = IBInternalQuery->FieldByName("PROFILE_KEY")->AsInteger;
			this->ID.Name = IBInternalQuery->FieldByName("DEVICE_NAME")->AsString;
         this->Status.Error = proA_Ok;
      }
      else
      {
		   this->Status.Error = Err_DeviceKey_Not_Found;
      }
	}
   catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

//---------------------------------------------------------------------------
void TDeviceDB::CreateProfile(Database::TDBTransaction &DBTransaction)
{
	// Save off System Config Settings.
	TManagerVariable::Instance().DeviceProfileKey = ID.ProfileKey;
	if(TManagerVariable::Instance().DeviceProfileKey == 0)
	{
		TManagerVariable::Instance().DeviceProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eTerminalProfiles,ID.Name);
		SetProfileKey(DBTransaction,TManagerVariable::Instance().DeviceProfileKey);
	}
}


void TDeviceDB::CreateDevice(Database::TDBTransaction &DBTransaction)
{
	try
   {
      if(Locate(DBTransaction))
      {
         Load(DBTransaction);
         CheckLocation(DBTransaction);
         Create(DBTransaction);
      }
      else
      {
         CheckLocation(DBTransaction);
         Create(DBTransaction);
      }
      TManagerVariable::Instance().LocationProfileKey = ID.LocationKey;
      TManagerVariable::Instance().DeviceProfileKey = ID.ProfileKey;
	}
   catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

void TDeviceDB::Copy(TDeviceDB *Image)
{
	TDeviceImage::Copy(Image);
   Status.Active = Image->Status.Active;
   Status.Error = Image->Status.Error;
   Status.ErrorMsg = Image->Status.ErrorMsg;
}

void TDeviceDB::SetProfileKey(Database::TDBTransaction &DBTransaction,int ProfileKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"UPDATE DEVICES "
			"SET "
				"PROFILE_KEY = :PROFILE_KEY "
         "WHERE "
				"DEVICE_KEY	= :DEVICE_KEY";
      IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = this->ID.DeviceKey;
		IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
		IBInternalQuery->ExecQuery();
		ID.ProfileKey = ProfileKey;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
		throw;
	}
}
