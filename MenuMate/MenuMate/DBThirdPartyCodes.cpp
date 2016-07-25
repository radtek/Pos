//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBThirdPartyCodes.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
UnicodeString TDBThirdPartyCodes::GetThirdPartyDescriptionByCode(Database::TDBTransaction &DBTransaction,UnicodeString ThirdPartyCode)
{
	UnicodeString RetVal = "";
	if(ThirdPartyCode == 0) return RetVal;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT DESCRIPTION FROM THIRDPARTYCODES "
									 "WHERE CODE = :CODE";
		IBInternalQuery->ParamByName("CODE")->AsString = ThirdPartyCode;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("DESCRIPTION")->AsString;
		}
	}
	catch(Exception &E)
	{
		#ifdef Logs
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		#endif
	}
	return RetVal;
}

UnicodeString TDBThirdPartyCodes::GetThirdPartyDescriptionByKey(Database::TDBTransaction &DBTransaction,int ThirdPartyKey)
{
	UnicodeString RetVal = "";
	if(ThirdPartyKey == 0) return RetVal;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT DESCRIPTION FROM THIRDPARTYCODES "
									 "WHERE THIRDPARTYCODES_KEY = :THIRDPARTYCODES_KEY";
		IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = ThirdPartyKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("DESCRIPTION")->AsString;
		}
	}
	catch(Exception &E)
	{
		#ifdef Logs
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		#endif
	}
	return RetVal;
}

TThirdPartyCodeType TDBThirdPartyCodes::GetThirdPartyTypeByKey(Database::TDBTransaction &DBTransaction,int ThirdPartyKey)
{
	TThirdPartyCodeType RetVal = tpItemSize;
	if(ThirdPartyKey == 0) return RetVal;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CODETYPE FROM THIRDPARTYCODES "
									 "WHERE THIRDPARTYCODES_KEY = :THIRDPARTYCODES_KEY";
		IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = ThirdPartyKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("CODETYPE")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		#ifdef Logs
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		#endif
	}
	return RetVal;
}

UnicodeString TDBThirdPartyCodes::GetThirdPartyCodeByKey(Database::TDBTransaction &DBTransaction,int ThirdPartyKey)
{
	UnicodeString RetVal = "";
	if(ThirdPartyKey == 0) return RetVal;

	try
	{
   	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	   DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CODE FROM THIRDPARTYCODES "
									 "WHERE THIRDPARTYCODES_KEY = :THIRDPARTYCODES_KEY";
		IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = ThirdPartyKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("CODE")->AsString;
		}
	}
	catch(Exception &E)
	{
		#ifdef Logs
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		#endif
	}
	return RetVal;
}

int TDBThirdPartyCodes::GetThirdPartyKeyByCode(Database::TDBTransaction &DBTransaction,UnicodeString ThirdPartyCode,TThirdPartyCodeType Type)
{
	int RetVal = 0;
	if(ThirdPartyCode == "") return RetVal;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT THIRDPARTYCODES_KEY FROM THIRDPARTYCODES "
									 "WHERE CODE = :CODE AND CODETYPE = :CODETYPE";
		IBInternalQuery->ParamByName("CODE")->AsString = ThirdPartyCode;
		IBInternalQuery->ParamByName("CODETYPE")->AsInteger = Type;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		#ifdef Logs
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		#endif
	}
	return RetVal;
}

int TDBThirdPartyCodes::SetThirdPartyCode(Database::TDBTransaction &DBTransaction,UnicodeString ThridPartyCode, UnicodeString Description,TThirdPartyCodeType Type)
{
	int ThridPartyCodeKey = 0;
	if(ThridPartyCode == "") return 0;
	try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		ThridPartyCodeKey = GetThirdPartyKeyByCode(DBTransaction,ThridPartyCode,Type);
		if(ThridPartyCodeKey == 0)
      {
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_THIRDPARTYCODES, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			ThridPartyCodeKey = IBInternalQuery->Fields[0]->AsInteger;

         IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"INSERT INTO THIRDPARTYCODES ("
					"THIRDPARTYCODES_KEY,"
					"CODE,"
					"CODETYPE,"
					"VISIBLE,"
					"DESCRIPTION) "
            "VALUES ("
					":THIRDPARTYCODES_KEY,"
					":CODE,"
					":CODETYPE,"
					":VISIBLE,"
					":DESCRIPTION);";
			IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = ThridPartyCodeKey;
			IBInternalQuery->ParamByName("CODE")->AsString = ThridPartyCode;
			IBInternalQuery->ParamByName("CODETYPE")->AsInteger = Type;
			IBInternalQuery->ParamByName("VISIBLE")->AsString = "T";
			IBInternalQuery->ParamByName("DESCRIPTION")->AsString = Description;
         IBInternalQuery->ExecQuery();
      }
		else
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"UPDATE "
				"THIRDPARTYCODES "
			"SET "
				"DESCRIPTION = :DESCRIPTION "
			"WHERE "
				"THIRDPARTYCODES_KEY = :THIRDPARTYCODES_KEY";
			IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = ThridPartyCodeKey;
			IBInternalQuery->ParamByName("DESCRIPTION")->AsString = Description;
			IBInternalQuery->ExecQuery();
		}
	}
   catch(Exception &E)
   {
		#ifdef Logs
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		#endif
   }
   return ThridPartyCodeKey;
}

int TDBThirdPartyCodes::SetThirdPartyCodeByKey(Database::TDBTransaction &DBTransaction,int ThridPartyCodeKey,UnicodeString ThridPartyCode, UnicodeString Description,TThirdPartyCodeType Type)
{
	if(ThridPartyCode == "") return 0;
	try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "UPDATE "
         "THIRDPARTYCODES "
      "SET "
			"CODE = :CODE, "
         "CODETYPE = :CODETYPE, "
         "DESCRIPTION = :DESCRIPTION "
      "WHERE "
         "THIRDPARTYCODES_KEY = :THIRDPARTYCODES_KEY";
      IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = ThridPartyCodeKey;
      IBInternalQuery->ParamByName("CODE")->AsString = ThridPartyCode;
      IBInternalQuery->ParamByName("CODETYPE")->AsInteger = Type;
      IBInternalQuery->ParamByName("DESCRIPTION")->AsString = Description;
      IBInternalQuery->ExecQuery();
	}
   catch(Exception &E)
   {
		#ifdef Logs
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		#endif
   }
   return ThridPartyCodeKey;
}
