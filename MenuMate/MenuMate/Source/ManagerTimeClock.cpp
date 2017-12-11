//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerTimeClock.h"
#include "ManagerVariable.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

void TManagerTimeClock::SetDefaultDept(Database::TDBTransaction &DBTransaction)
{
   AddClockInDept(DBTransaction,"Bar",1);
   AddClockInDept(DBTransaction,"Restaurant",2);
   AddClockInDept(DBTransaction,"Cafe",3);
   AddClockInDept(DBTransaction,"Kitchen",4);
   AddClockInDept(DBTransaction,"Cleaner",5);
}
//---------------------------------------------------------------------------
void TManagerTimeClock::AddClockInDept(Database::TDBTransaction &DBTransaction,AnsiString DeptName,int DeptCode)
{

   if(GetClockInDept(DBTransaction,DeptName,DeptCode) == 0)
   {
      SetClockInDept(DBTransaction,DeptName,DeptCode);
   }
   else

   {
   AddDepInLoc(DBTransaction,DeptName,DeptCode);
   //AddDepInLoc(Database::TDBTransaction &DBTransaction, AnsiString DeptName,int DeptCode)
   }
}
//---------------------------------------------------------------------------
void TManagerTimeClock::SetClockInDept(Database::TDBTransaction &DBTransaction,AnsiString DeptName,int DeptCode)
{
   try
   {
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_TIMECLOCKLOCATIONS, 1) FROM RDB$DATABASE";
      IBInternalQuery->ExecQuery();
      int TimeClockKey = IBInternalQuery->Fields[0]->AsInteger;


		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"INSERT INTO TIMECLOCKLOCATIONS ("
				"TIMECLOCKLOCATIONS_KEY, "
				"DISPLAY_ORDER, "
				"NAME, "
				"CODE, "
				"RATE, "
                "DATE_CREATED, "
              "STATUS) "
			"VALUES ("
				":TIMECLOCKLOCATIONS_KEY, "
				":DISPLAY_ORDER, "
				":NAME, "
				":CODE, "
				":RATE,"
                ":DATE_CREATED,"
                ":STATUS);";
		IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = TimeClockKey;
		IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger 	      = TimeClockKey;
		IBInternalQuery->ParamByName("NAME")->AsString         		      = DeptName;
		IBInternalQuery->ParamByName("CODE")->AsInteger 			         = DeptCode;
        IBInternalQuery->ParamByName("RATE")->AsCurrency                  = 0;
        IBInternalQuery->ParamByName("STATUS")->AsString = "T";
        IBInternalQuery->ParamByName("DATE_CREATED")->AsDateTime = Now();
		IBInternalQuery->ExecQuery();
	}
	catch (Exception &E)
	{
		throw;
	}
}
//-------------------------------------------------------------------------
void TManagerTimeClock::AddDepInLoc(Database::TDBTransaction &DBTransaction, AnsiString DeptName,int DeptCode)
{
	try
	{
		if(DeptName != 0)
		{
            TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery1->Close();
			IBInternalQuery1->SQL->Text =
			"UPDATE "
			" TIMECLOCKLOCATIONS"
			" SET"
			" STATUS = 'T', "
            " DATE_CREATED = :DATE_CREATED "
			" WHERE NAME = :NAME AND CODE = :CODE;";
            IBInternalQuery1->ParamByName("DATE_CREATED")->AsDateTime = Now();
            IBInternalQuery1->ParamByName("NAME")->AsString = DeptName;
            IBInternalQuery1->ParamByName("CODE")->AsInteger = DeptCode;
            IBInternalQuery1->ExecQuery();
             }
             }
   catch(Exception &E)
	{
		throw;
	}
	//return Key;
}
//---------------------------------------------------------------------------
void TManagerTimeClock::UpdateClockInDept(Database::TDBTransaction &DBTransaction,AnsiString DeptName,int DeptCode,int DeptKey)
{
   try
   {
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" TIMECLOCKLOCATIONS"
		" SET"
		" NAME = :NAME, "
		" CODE = :CODE "
		" WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY";
		IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = DeptKey;
		IBInternalQuery->ParamByName("NAME")->AsString         		      = DeptName;
		IBInternalQuery->ParamByName("CODE")->AsInteger 			         = DeptCode;
		IBInternalQuery->ExecQuery();
	}
	catch (Exception &E)
	{
		throw;
	}
}
//---------------------------------------------------------------------------
int TManagerTimeClock::GetClockInDept(Database::TDBTransaction &DBTransaction,AnsiString DeptName,int DeptCode)
{
   int RetVal = 0;
   try
   {
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT TIMECLOCKLOCATIONS_KEY FROM TIMECLOCKLOCATIONS "
         " WHERE NAME = :NAME AND CODE = :CODE;";
		IBInternalQuery->ParamByName("NAME")->AsString       = DeptName;
		IBInternalQuery->ParamByName("CODE")->AsInteger      = DeptCode;
		IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
      {
			RetVal = IBInternalQuery->FieldByName("TIMECLOCKLOCATIONS_KEY")->AsInteger;
      }
	}
	catch (Exception &E)
	{
		throw;
	}
   return RetVal;
}
//---------------------------------------------------------------------------
AnsiString TManagerTimeClock::GetClockInDeptName(Database::TDBTransaction &DBTransaction,int DeptKey)
{
   AnsiString DeptName;
   try
   {
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT NAME FROM TIMECLOCKLOCATIONS "
         " WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY";
		IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = DeptKey;
		IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
      {
			DeptName = IBInternalQuery->FieldByName("NAME")->AsString;
      }
	}
	catch (Exception &E)
	{
		throw;
	}
   return DeptName;
}
//---------------------------------------------------------------------------
int TManagerTimeClock::GetClockInDeptCode(Database::TDBTransaction &DBTransaction,int DeptKey)
{
   int DeptCode = 0;
   try
   {
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT CODE FROM TIMECLOCKLOCATIONS "
         " WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY";
		IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = DeptKey;
		IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
      {
			DeptCode = IBInternalQuery->FieldByName("CODE")->AsInteger;
      }
	}
	catch (Exception &E)
	{
		throw;
	}
   return DeptCode;
}


//---------------------------------------------------------------------------
void TManagerTimeClock::GetClockInDeptList(Database::TDBTransaction &DBTransaction,TStringList *DeptList)
{
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      " SELECT *"
      " FROM"
      "  TIMECLOCKLOCATIONS"
       " WHERE STATUS = 'T'"
      " ORDER BY DISPLAY_ORDER ";
      IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount && DeptList != NULL)
      {
			for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				int Index = DeptList->Add(IBInternalQuery->FieldByName("NAME")->AsString + " (" + IBInternalQuery->FieldByName("CODE")->AsString + ")" );
				DeptList->Objects[Index] = (TObject *)(IBInternalQuery->FieldByName("TIMECLOCKLOCATIONS_KEY")->AsInteger);
			}
      }
	}
	catch (Exception &E)
	{
		throw;
	}
}
//---------------------------------------------------------------------------
void TManagerTimeClock::ClockIn(Database::TDBTransaction &DBTransaction,int inContactKey,int DeptKey,TDateTime LogInTime)
{
   try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CONTACTTIME, 1) FROM RDB$DATABASE";
      IBInternalQuery->ExecQuery();
      int ContactTimeKey = IBInternalQuery->Fields[0]->AsInteger;

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
	  "INSERT INTO CONTACTTIME ("
         "CONTACTTIME_KEY,"
         "CONTACTS_KEY,"
         "LOGIN_DATETIME,"
         "TIMECLOCKLOCATIONS_KEY,"
         "MODIFIED,"
         "EXPORTED,"
         "STATUS) "
      "VALUES ("
         ":CONTACTTIME_KEY,"
         ":CONTACTS_KEY,"
         ":LOGIN_DATETIME,"
         ":TIMECLOCKLOCATIONS_KEY,"
         ":MODIFIED,"
         ":EXPORTED,"
         ":STATUS);";
      IBInternalQuery->ParamByName("CONTACTTIME_KEY")->AsInteger = ContactTimeKey;
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
      IBInternalQuery->ParamByName("LOGIN_DATETIME")->AsDateTime = LogInTime;
      IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = DeptKey;
      IBInternalQuery->ParamByName("MODIFIED")->AsString = "F";
      IBInternalQuery->ParamByName("EXPORTED")->AsString = "F";
       IBInternalQuery->ParamByName("STATUS")->AsString = "T";
     IBInternalQuery->ExecQuery();
	}
   catch(Exception &E)
   {
		throw;
   }
}
//---------------------------------------------------------------------------
bool TManagerTimeClock::ClockedIn(Database::TDBTransaction &DBTransaction,int inContactKey)
{
   bool LoggedIn = false;
   try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select CONTACTTIME_KEY from CONTACTTIME where LOGIN_DATETIME is not null and LOGOUT_DATETIME is null and CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger	= inContactKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			LoggedIn = true;
		}
		IBInternalQuery->Close();
	}
	catch(Exception &E)
	{
		throw;
	}
	return LoggedIn;
}


//---------------------------------------------------------------------------
int TManagerTimeClock::ClockedCount(Database::TDBTransaction &DBTransaction,int inContactKey)
{
   int Count = 0;
   try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "Select COUNT(CONTACTTIME_KEY) from CONTACTTIME where CONTACTS_KEY = :CONTACTS_KEY";
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger	= inContactKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
      {
         Count = IBInternalQuery->FieldByName("COUNT")->AsInteger;
      }
      IBInternalQuery->Close();
	}
	catch(Exception &E)
	{
		throw;
   }
	return Count;
}


//---------------------------------------------------------------------------

TDateTime TManagerTimeClock::ClockedInDateTime(Database::TDBTransaction &DBTransaction,int inContactKey)
{
	TDateTime LoggedInAt = 0;
   try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select LOGIN_DATETIME from CONTACTTIME where LOGIN_DATETIME is not null and LOGOUT_DATETIME is null and CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger	= inContactKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			LoggedInAt = IBInternalQuery->FieldByName("LOGIN_DATETIME")->AsDateTime;
		}
		IBInternalQuery->Close();
	}
	catch(Exception &E)
	{
		throw;
	}
	return LoggedInAt;
}
//---------------------------------------------------------------------------

TTime TManagerTimeClock::GetUserBreak(Database::TDBTransaction &DBTransaction,int inContactKey, int &ContactTimeKey)
{
	TTime Breaks = 0;
   try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select BREAKS, CONTACTTIME_KEY from CONTACTTIME where LOGIN_DATETIME is not null and LOGOUT_DATETIME is null and CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger	= inContactKey;
		IBInternalQuery->ExecQuery();

			Breaks = IBInternalQuery->FieldByName("BREAKS")->AsTime;
			ContactTimeKey = IBInternalQuery->FieldByName("CONTACTTIME_KEY")->AsInteger;

		IBInternalQuery->Close();
	}
	catch(Exception &E)
	{
		throw;
	}
	return Breaks;
}
//---------------------------------------------------------------------------
int TManagerTimeClock::SetUserBreak(Database::TDBTransaction &DBTransaction, int &ContactTimeKey, TTime inBreak)
{
	try
	{
		if(ContactTimeKey != 0)
		{
			TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery1->Close();
			IBInternalQuery1->SQL->Text =
			"UPDATE "
			" CONTACTTIME"
			" SET"
			" BREAKS = :BREAKS "
			" WHERE CONTACTTIME_KEY = :CONTACTTIME_KEY";
			IBInternalQuery1->ParamByName("CONTACTTIME_KEY")->AsInteger = ContactTimeKey;
			IBInternalQuery1->ParamByName("BREAKS")->AsTime = inBreak;
			IBInternalQuery1->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		throw;
	}
	return ContactTimeKey;
}
//---------------------------------------------------------------------------
int TManagerTimeClock::ClockOut(Database::TDBTransaction &DBTransaction,int &inContactKey,TDateTime &LogInTime,TDateTime &LogOutTime)
{
	int ContactTimeKey = 0;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT  "
		" CONTACTTIME_KEY,LOGIN_DATETIME"
		" FROM CONTACTTIME"
		" WHERE LOGIN_DATETIME IS NOT NULL AND LOGOUT_DATETIME IS NULL"
		" AND CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		IBInternalQuery->ExecQuery();

		ContactTimeKey = IBInternalQuery->FieldByName("CONTACTTIME_KEY")->AsInteger;

		if(ContactTimeKey != 0)
		{
			LogInTime = IBInternalQuery->FieldByName("LOGIN_DATETIME")->AsDateTime;
			TTime breaks = GetUserBreak(DBTransaction,inContactKey, ContactTimeKey);
			if(double(LogOutTime - LogInTime - breaks) < 0)
				breaks = TTime(LogOutTime - LogInTime );

			TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery1->Close();
			IBInternalQuery1->SQL->Text =
			"UPDATE "
			" CONTACTTIME"
			" SET"
			" LOGOUT_DATETIME = :LOGOUT_DATETIME, "
			" BREAKS = :BREAKS "
			" WHERE CONTACTTIME_KEY = :CONTACTTIME_KEY";
			IBInternalQuery1->ParamByName("CONTACTTIME_KEY")->AsInteger = ContactTimeKey;
			IBInternalQuery1->ParamByName("LOGOUT_DATETIME")->AsDateTime = LogOutTime;
			IBInternalQuery1->ParamByName("BREAKS")->AsTime = breaks;
			IBInternalQuery1->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		throw;
	}
	return ContactTimeKey;
}
//---------------------------------------------------------------------------
void TManagerTimeClock::GetLoggedInDetails(Database::TDBTransaction &DBTransaction, TDateTime LogInTime, std::vector<TStaffHours> &inStaffHours,
                                           UnicodeString inDevicename)
{
   try
	{
        UnicodeString devicename = inDevicename;

		TDateTime PrevZedTime;
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT MAX(ZEDS.TIME_STAMP) TIME_STAMP, PROFILE.PROFILE_KEY "
			"FROM ZEDS Left JOIN PROFILE ON ZEDS.TERMINAL_NAME = PROFILE.NAME "
			"WHERE TIME_STAMP IS NOT NULL "

         //   "and TERMINAL_NAME = :devicename "
         " and  TIME_STAMP = (SELECT MAX(TIME_STAMP) FROM ZEDS where  STAFF_HOUR_ENABLE = 1)  "
			"GROUP BY TERMINAL_NAME, PROFILE_KEY "
			"ORDER BY TIME_STAMP DESC ";

		IBInternalQuery->ExecQuery();
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			int value;
			int Key = IBInternalQuery->FieldByName("PROFILE_KEY")->AsInteger;
			PrevZedTime = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
			TManagerVariable::Instance().GetProfileInt( DBTransaction, Key, vmEnableStaffHours, value);
			if(value)
            {
              break;
            }
		}
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
         "SELECT "
			"CONTACTTIME_KEY, "
			"ROUNDEDCONTACTTIME.CONTACTS_KEY, "
			"ROUNDED_LOGIN_DATETIME, "
			"ROUNDED_LOGOUT_DATETIME, "
			"TIMECLOCKLOCATIONS_KEY, "
			"CONTACTS.HOURLY_RATE, "
			"ROUNDEDCONTACTTIME.BREAKS, "
            "ROUNDEDCONTACTTIME.ZED_STATUS "
		"FROM "
			"ROUNDEDCONTACTTIME LEFT JOIN CONTACTS ON "
			"ROUNDEDCONTACTTIME.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
		"WHERE "
			"ROUNDED_LOGOUT_DATETIME > :LOGIN_DATETIME and CONTACTS.CONTACT_TYPE = :CONTACT_TYPE and ROUNDEDCONTACTTIME.ZED_STATUS is null "

         "union all "

        "SELECT "
            "CONTACTTIME_KEY , "
            "ROUNDEDCONTACTTIME.CONTACTS_KEY, "
            "ROUNDED_LOGIN_DATETIME,  "
            "ROUNDED_LOGOUT_DATETIME, "
            "TIMECLOCKLOCATIONS_KEY,  "
            "CONTACTS.HOURLY_RATE,  "
            "ROUNDEDCONTACTTIME.BREAKS,  "
            "ROUNDEDCONTACTTIME.ZED_STATUS "
        "FROM   "
            "ROUNDEDCONTACTTIME LEFT JOIN CONTACTS ON  "
            "ROUNDEDCONTACTTIME.CONTACTS_KEY = CONTACTS.CONTACTS_KEY  "
        "WHERE "
            "ROUNDED_LOGOUT_DATETIME is null  and CONTACTS.CONTACT_TYPE = :CONTACT_TYPE and ROUNDEDCONTACTTIME.ZED_STATUS is null " ;

		IBInternalQuery->ParamByName("LOGIN_DATETIME")->AsDateTime = PrevZedTime;
        IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = 0;

		IBInternalQuery->ExecQuery();
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			unsigned short th, tm, ts, tms;
			TTime onbreak = IBInternalQuery->FieldByName("BREAKS")->AsTime;
			onbreak.DecodeTime(&th, &tm, &ts, &tms);
			int temp = (th*60) + tm;

	        TStaffHours sh("",
				IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger,
                IBInternalQuery->FieldByName("ROUNDED_LOGIN_DATETIME")->AsDateTime,
                IBInternalQuery->FieldByName("ROUNDED_LOGOUT_DATETIME")->AsDateTime,
				IBInternalQuery->FieldByName("TIMECLOCKLOCATIONS_KEY")->AsInteger,
				NULL,
				IBInternalQuery->FieldByName("CONTACTTIME_KEY")->AsInteger,
				IBInternalQuery->FieldByName("HOURLY_RATE")->AsFloat,
				temp);

			if(IBInternalQuery->FieldByName("ROUNDED_LOGOUT_DATETIME")->AsString == "")
				sh.SetLogOutNull(true);

			inStaffHours.push_back(sh);
		}
	}
	catch(Exception &E)
	{
		throw;
	}
}
//---------------------------------------------------------------------------
TDateTime TManagerTimeClock::GetRoundedLoginTime(Database::TDBTransaction &DBTransaction, TDateTime loginTime, int contact_time_key)
{
   TDateTime RetVal = 0;
   try
   {
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
         "SELECT ROUNDED_LOGIN_DATETIME FROM ROUNDEDCONTACTTIME "
         " WHERE LOGIN_DATETIME = :LOGIN_DATETIME AND CONTACTTIME_KEY = :CONTACTTIME_KEY;";
		IBInternalQuery->ParamByName("LOGIN_DATETIME")->AsDateTime = loginTime;
		IBInternalQuery->ParamByName("CONTACTTIME_KEY")->AsInteger = contact_time_key;
		IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
      {
			RetVal = IBInternalQuery->FieldByName("ROUNDED_LOGIN_DATETIME")->AsDateTime;
      }
	}
	catch (Exception &E)
	{
		throw;
	}
   return RetVal;
}
//---------------------------------------------------------------------------
TDateTime TManagerTimeClock::GetRoundedLogOutTime(Database::TDBTransaction &DBTransaction, TDateTime loginTime, int contact_time_key)
{
   TDateTime RetVal = 0;
   try
   {
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
         "SELECT ROUNDED_LOGOUT_DATETIME FROM ROUNDEDCONTACTTIME "
         " WHERE LOGIN_DATETIME = :LOGIN_DATETIME AND CONTACTTIME_KEY = :CONTACTTIME_KEY;";
		IBInternalQuery->ParamByName("LOGIN_DATETIME")->AsDateTime = loginTime;
		IBInternalQuery->ParamByName("CONTACTTIME_KEY")->AsInteger = contact_time_key;
		IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
      {
			RetVal = IBInternalQuery->FieldByName("ROUNDED_LOGOUT_DATETIME")->AsDateTime;
      }
	}
	catch (Exception &E)
	{
		throw;
	}
   return RetVal;
}
//---------------------------------------------------------------------------
void TManagerTimeClock::SetLoggedInOutDetails(Database::TDBTransaction &DBTransaction, TStaffHoursContainer::iterator inStaffHours)
{
	try
	{
		int hrs = inStaffHours->second.GetBreak() / 60;
		int mins = inStaffHours->second.GetBreak() % 60;
		TTime temp = TTime(hrs, mins, 0, 0);
		(double)temp == NULL ? temp = 0: temp;			//Why was this (int)???

			if(inStaffHours->second.GetLogOutNull())
				return;

			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"UPDATE "
				" CONTACTTIME"
				" SET"
			    " LOGIN_DATETIME = :LOGIN_DATETIME, " // remove this to update login date time
				" LOGOUT_DATETIME = :LOGOUT_DATETIME," // remove this to update logout date time
				" TOTALHOURS = :TOTALHOURS, "
				" BREAKS = :BREAKS "
				" WHERE CONTACTTIME_KEY = :CONTACTTIME_KEY";
			IBInternalQuery->ParamByName("CONTACTTIME_KEY")->AsInteger = inStaffHours->second.GetContactTimeKey();
		    IBInternalQuery->ParamByName("LOGIN_DATETIME")->AsDateTime = inStaffHours->second.GetLoggedIn(); // remove this to update login date time
			IBInternalQuery->ParamByName("LOGOUT_DATETIME")->AsDateTime = inStaffHours->second.GetLoggedOut(); // remove this to update logout date time
			IBInternalQuery->ParamByName("TOTALHOURS")->AsFloat = inStaffHours->second.GetHoursWorked();
			IBInternalQuery->ParamByName("BREAKS")->AsTime = temp;
			IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		throw;
	}
}
//---------------------------------------------------------------------------
void TManagerTimeClock::MoveDisplayOrderUp(Database::TDBTransaction &DBTransaction,int Key)
{
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      " SELECT TIMECLOCKLOCATIONS_KEY"
      " FROM"
      "  TIMECLOCKLOCATIONS"
      " WHERE DISPLAY_ORDER < (SELECT DISPLAY_ORDER FROM TIMECLOCKLOCATIONS WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY) "
      " ORDER BY DISPLAY_ORDER DESC";
		IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = Key;
      IBInternalQuery->ExecQuery();
      int NextKey =  IBInternalQuery->FieldByName("TIMECLOCKLOCATIONS_KEY")->AsInteger;

      if(NextKey != 0)
      {
         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
         " SELECT DISPLAY_ORDER"
         " FROM"
         "  TIMECLOCKLOCATIONS"
         " WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY ";
         IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = NextKey;
         IBInternalQuery->ExecQuery();
         int DisplayOrder = IBInternalQuery->FieldByName("DISPLAY_ORDER")->AsInteger;

         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
         "UPDATE "
         " TIMECLOCKLOCATIONS"
         " SET"
         " DISPLAY_ORDER = (SELECT DISPLAY_ORDER FROM TIMECLOCKLOCATIONS WHERE TIMECLOCKLOCATIONS_KEY = :ORIGINAL_TIMECLOCKLOCATIONS_KEY)"
         " WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY ";
         IBInternalQuery->ParamByName("ORIGINAL_TIMECLOCKLOCATIONS_KEY")->AsInteger = Key;
         IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = NextKey;
         IBInternalQuery->ExecQuery();

         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
         "UPDATE "
         " TIMECLOCKLOCATIONS"
         " SET"
         " DISPLAY_ORDER = :DISPLAY_ORDER "
         " WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY ";
         IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = Key;
         IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = DisplayOrder;
         IBInternalQuery->ExecQuery();
      }
	}
   catch(Exception &E)
   {
      throw;
   }
}
//---------------------------------------------------------------------------
void TManagerTimeClock::MoveDisplayOrderDown(Database::TDBTransaction &DBTransaction,int Key)
{
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      " SELECT TIMECLOCKLOCATIONS_KEY"
      " FROM"
      "  TIMECLOCKLOCATIONS"
      " WHERE DISPLAY_ORDER > (SELECT DISPLAY_ORDER FROM TIMECLOCKLOCATIONS WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY) "
      " ORDER BY DISPLAY_ORDER ";
		IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = Key;
      IBInternalQuery->ExecQuery();
      int NextKey =  IBInternalQuery->FieldByName("TIMECLOCKLOCATIONS_KEY")->AsInteger;

      if(NextKey != 0)
      {
         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
         " SELECT DISPLAY_ORDER"
         " FROM"
         "  TIMECLOCKLOCATIONS"
         " WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY ";
         IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = NextKey;
         IBInternalQuery->ExecQuery();
         int DisplayOrder =  IBInternalQuery->FieldByName("DISPLAY_ORDER")->AsInteger;

         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
         "UPDATE "
         " TIMECLOCKLOCATIONS"
         " SET"
         " DISPLAY_ORDER = (SELECT DISPLAY_ORDER FROM TIMECLOCKLOCATIONS WHERE TIMECLOCKLOCATIONS_KEY = :ORIGINAL_TIMECLOCKLOCATIONS_KEY)"
         " WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY ";
         IBInternalQuery->ParamByName("ORIGINAL_TIMECLOCKLOCATIONS_KEY")->AsInteger = Key;
         IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = NextKey;
         IBInternalQuery->ExecQuery();

         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
         "UPDATE "
         " TIMECLOCKLOCATIONS"
         " SET"
         " DISPLAY_ORDER = :DISPLAY_ORDER "
         " WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY ";
         IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = Key;
         IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = DisplayOrder;
         IBInternalQuery->ExecQuery();
      }
	}
   catch(Exception &E)
   {
      throw;
   }
}
//---------------------------------------------------------------------------
void TManagerTimeClock::BuildXMLTotalsTimeClock(TPOS_XMLBase &Data,int SiteID, int ContactKey, TDateTime &Login, TDateTime &LogOut)
{
   Data.Doc.Clear();

	TiXmlDeclaration * decl = new TiXmlDeclaration( _T("1.0"), _T("UTF-8"), _T("") );
   Data.Doc.LinkEndChild( decl );

   // Insert DOCTYPE definiation here.
   TiXmlElement * List = new TiXmlElement( xmlEleListRoster );
	List->SetAttribute(xmlAttrID, AnsiString(Data.IntaMateID).c_str());
	List->SetAttribute(xmlAttrSiteID, SiteID);

	TiXmlElement *EleTimeClock = new TiXmlElement( xmlTimeClockInfo );
	EleTimeClock->SetAttribute(xmlAttrXmlID,        ContactKey);
	EleTimeClock->SetAttribute(xmlAttrTimeStart,    UnicodeString(Login.FormatString("YYYYMMDDHHMMSS")).t_str());
	EleTimeClock->SetAttribute(xmlAttrTimeEnd,      UnicodeString(LogOut.FormatString("YYYYMMDDHHMMSS")).t_str());
   List->LinkEndChild( EleTimeClock );
   Data.Doc.LinkEndChild( List );
}
//---------------------------------------------------------------------------
void TManagerTimeClock::UpdateClockInOut(Database::TDBTransaction &DBTransaction, int contact_time_key, int contact_key)
{
	try
	{
        DBTransaction.StartTransaction();

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT  "
		" CONTACTTIME_KEY, LOGIN_DATETIME"
		" FROM CONTACTTIME"
		" WHERE CONTACTTIME_KEY = :CONTACTTIME_KEY "
		" AND CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contact_key;
        IBInternalQuery->ParamByName("CONTACTTIME_KEY")->AsInteger = contact_time_key;
		IBInternalQuery->ExecQuery();

		if(contact_time_key != 0)
		{
			TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery1->Close();
			IBInternalQuery1->SQL->Text =
			"UPDATE "
			" CONTACTTIME"
			" SET"
            " LOGIN_DATETIME = :LOGIN_DATETIME, " // remove this to update login date time
            " LOGOUT_DATETIME = :LOGOUT_DATETIME " // remove this to update logout date time
			" WHERE CONTACTTIME_KEY = :CONTACTTIME_KEY ";
			IBInternalQuery1->ParamByName("CONTACTTIME_KEY")->AsInteger = contact_time_key;
            IBInternalQuery1->ParamByName("LOGIN_DATETIME")->AsDateTime = GetRoundedLoginTime(DBTransaction, IBInternalQuery->FieldByName("LOGIN_DATETIME")->AsDateTime, contact_time_key);
			IBInternalQuery1->ParamByName("LOGOUT_DATETIME")->AsDateTime = GetRoundedLogOutTime(DBTransaction, IBInternalQuery->FieldByName("LOGIN_DATETIME")->AsDateTime, contact_time_key);
			IBInternalQuery1->ExecQuery();
		}

        DBTransaction.Commit();
	}
	catch(Exception &E)
	{
		throw;
	}
}
//-----------------------------------------------------------------------------
bool TManagerTimeClock::ClockedInDep(Database::TDBTransaction &DBTransaction,int inContactKey)
{
   bool LoggedIn = false;
   try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select CONTACTTIME_KEY from CONTACTTIME where LOGIN_DATETIME is not null and LOGOUT_DATETIME is null and TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY";
		IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger	= inContactKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			LoggedIn = true;
		}
		IBInternalQuery->Close();
	}
	catch(Exception &E)
	{
		throw;
	}
	return LoggedIn;
}
//----------------------------------------------------------------------------------
bool TManagerTimeClock::CheckClockedIn(Database::TDBTransaction &DBTransaction,int inContactKey)
{
   bool LoggedIn = false;
   try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " Select CONTACTTIME_KEY from CONTACTTIME where TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY ";
		IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger	= inContactKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			LoggedIn = true;
		}
		IBInternalQuery->Close();
	}
	catch(Exception &E)
	{
		throw;
	}
	return LoggedIn;
}
//---------------------------------------------------------------------------
void TManagerTimeClock::DelClockInDept(Database::TDBTransaction &DBTransaction,int Key)
{
   try
   {
     /* TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "DELETE FROM CONTACTTIME WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY;";
      IBInternalQuery->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = Key;



      IBInternalQuery->ExecQuery();  */
      	if(Key != 0)
		{
			TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery1->Close();
			IBInternalQuery1->SQL->Text =
			"UPDATE "
			" CONTACTTIME"
			" SET"
			" STATUS = 'F' "
			" WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY";
			IBInternalQuery1->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = Key;
           // IBInternalQuery1->TimeStamp    = IBInternalQuery1->FieldByName("TIME_STAMP")->AsDateTime;
            IBInternalQuery1->ExecQuery();

           }
	}
	catch (Exception &E)
	{
	throw;
	}

}

//-------------------------------------------------------------------------
void TManagerTimeClock::DelClockInDeptFromLoc(Database::TDBTransaction &DBTransaction, int Key)
{
	try
	{
		if(Key != 0)
		{
            TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery1->Close();
			IBInternalQuery1->SQL->Text =
			"UPDATE "
			" TIMECLOCKLOCATIONS"
			" SET"
			" STATUS = 'F', "
            " DATE_CREATED = :DATE_CREATED "
			" WHERE TIMECLOCKLOCATIONS_KEY = :TIMECLOCKLOCATIONS_KEY";
			 IBInternalQuery1->ParamByName("TIMECLOCKLOCATIONS_KEY")->AsInteger = Key;
             IBInternalQuery1->ParamByName("DATE_CREATED")->AsDateTime = Now();
            //IBInternalQuery1->ParamByName("DATE_CREATED")->AsDateTime = LogOutTime;
            IBInternalQuery1->ExecQuery();
             }
             }

catch(Exception &E)
	{
		throw;
	}
	//return Key;
}
//----------------------------------------------------------------------


