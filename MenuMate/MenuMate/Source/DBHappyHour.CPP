#include "DBHappyHour.h"
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
#include "GlobalSettings.h"
void THappyHour::SetStartTime(TDateTime startTime)
{
    _startTime = startTime;
}
//---------------------------------------------------------------------------

void THappyHour::SetEndTime(TDateTime endTime)
{
    _endTime = endTime;
}
//---------------------------------------------------------------------------
void THappyHour::SetDateTime(TDateTime dateTime)
{
    _dateTime = dateTime;
}

//---------------------------------------------------------------------------

void THappyHour::SetProfileName(AnsiString profileName)
{
    _profileName = profileName;
}

//---------------------------------------------------------------------------

void THappyHour::SetProfileKey(int profileKey)
{
    _profileKey = profileKey;
}
//---------------------------------------------------------------------------

void THappyHour::SetEditedProfile(bool isEditedProfile)
{
    _isEditedProfile = isEditedProfile;
}
//---------------------------------------------------------------------------

void THappyHour::SetHappyHourProfileDays(std::vector<int> happyHourProfileDays)
{
    _happyHourProfileDays = happyHourProfileDays;
}
//---------------------------------------------------------------------------

void THappyHour::SetHappyHourProfileSettings(std::vector<int> happyHourProfileSettings)
{
    _happyHourProfileSettings = happyHourProfileSettings;
}
//---------------------------------------------------------------------------

void THappyHour::SetProfileTerminalKeys(std::vector<int> profileTerminalsKeys)
{
    _profileTerminalsKeys = profileTerminalsKeys;
}
///-----------------------------------------------------------------
void THappyHour::SetProfilePricelevel(int profilePricelevel)
{
    _profilePricelevel = profilePricelevel;
}
//---------------------------------------------------------------------------
void THappyHour::SetProfilePriority(int Priority)
{
    _priority = Priority;
}

 void THappyHour::SetProfileTerminalKey(int TerminalKey)
{
    _terminalKey = TerminalKey;
}

int TDBHappyHour::CreateProfileKey(Database::TDBTransaction &dBTransaction)
{
    int profileKey;
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dBTransaction.RegisterQuery(IBInternalQuery);
    try
    {
        // Generate new profile key using auto generator.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_HAPPYHOURPROFILES_KEY, 1) FROM RDB$DATABASE ;";
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
             profileKey = IBInternalQuery->Fields[0]->AsInteger;

        return  profileKey;

    }
    catch(Exception &Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}

void TDBHappyHour::SaveProfile(Database::TDBTransaction &dBTransaction,THappyHour* happyHourProfiles, int profileKey)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dBTransaction.RegisterQuery(IBInternalQuery);
    try
    {
        // Inserting new profile into HAPPYHOURPROFILES Table.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "INSERT INTO HAPPYHOURPROFILES ("
        "HAPPYHOURPROFILES_KEY, "
        "HAPPYHOURPROFILE_NAME, "
        "ISACTIVE, "
        "HAPPYHOURDAY_STARTTIME, "
        "HAPPYHOURDAY_ENDTIME, "
        "HAPPYHOURDAY_PROFILEDATE, "
        "PRICELEVEL_KEY "
        " ) "
        "VALUES ("
        ":HAPPYHOURPROFILES_KEY, "
        ":HAPPYHOURPROFILE_NAME, "
        ":ISACTIVE, "
        ":HAPPYHOURDAY_STARTTIME, "
        ":HAPPYHOURDAY_ENDTIME, "
        ":HAPPYHOURDAY_PROFILEDATE, "
        ":PRICELEVEL_KEY "
        " );";

        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = profileKey;
        IBInternalQuery->ParamByName("HAPPYHOURPROFILE_NAME")->AsString = happyHourProfiles->ProfileName;
        IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
        IBInternalQuery->ParamByName("HAPPYHOURDAY_STARTTIME")->AsDateTime = happyHourProfiles->StartTime;
        IBInternalQuery->ParamByName("HAPPYHOURDAY_ENDTIME")->AsDateTime = happyHourProfiles->EndTime;
        IBInternalQuery->ParamByName("HAPPYHOURDAY_PROFILEDATE")->AsDateTime = happyHourProfiles->DateTime;
        IBInternalQuery->ParamByName("PRICELEVEL_KEY")->AsInteger = happyHourProfiles->Pricelevel;

        IBInternalQuery->ExecQuery();

    }
    catch(Exception &Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}
//---------------------------------------------------------------------------

void TDBHappyHour::SaveProfileDays(Database::TDBTransaction &dBTransaction,THappyHour* happyHourProfiles, int profileKey)
{
     Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
     dBTransaction.RegisterQuery(IBInternalQuery);

     try
     {
        // Inserting days info into HAPPYHOURDAYS Table.
        for (std::vector<Integer>::iterator iter = happyHourProfiles->HappyHourProfileDays.begin() ; iter != happyHourProfiles->HappyHourProfileDays.end() ; iter++)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "INSERT INTO HAPPYHOURDAYS ("
            "HAPPYHOURPROFILES_KEY, "
            "HAPPYHOURDAYS_KEY "
            " ) "
            "VALUES ("
            ":HAPPYHOURPROFILES_KEY, "
            ":HAPPYHOURDAYS_KEY "
            " ); ";

            IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = profileKey;
            IBInternalQuery->ParamByName("HAPPYHOURDAYS_KEY")->AsInteger = *iter;
            IBInternalQuery->ExecQuery();
        }
    }

    catch(Exception &Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}
//---------------------------------------------------------------------------

void TDBHappyHour::SaveProfileSettings(Database::TDBTransaction &dBTransaction,THappyHour* happyHourProfiles, int profileKey)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);
    try
    {
        // Inserting setting info into HAPPYHOURSETTINGS Table.
        for (std::vector<Integer>::iterator iter = happyHourProfiles->HappyHourProfileSettings.begin() ; iter != happyHourProfiles->HappyHourProfileSettings.end() ; iter++)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "INSERT INTO HAPPYHOURSETTINGS ("
            "HAPPYHOURPROFILES_KEY, "
            "HAPPYHOURSETTINGS_KEY "
            " ) "
            " VALUES ( "
            ":HAPPYHOURPROFILES_KEY, "
            ":HAPPYHOURSETTINGS_KEY "
            " );";

            IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = profileKey;
            IBInternalQuery->ParamByName("HAPPYHOURSETTINGS_KEY")->AsInteger = *iter;
            IBInternalQuery->ExecQuery();
        }
    }

    catch(Exception &Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}
//---------------------------------------------------------------------------

void TDBHappyHour::SaveTerminalMapping(Database::TDBTransaction &dBTransaction,THappyHour* happyHourProfiles, int profileKey)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);

    try
    {
        // Inserting terminal info into TERMINALHAPPYHOURPROFILES Table.
        for (std::vector<Integer>::iterator iter = happyHourProfiles->ProfileTerminalsKeys.begin() ; iter != happyHourProfiles->ProfileTerminalsKeys.end() ; iter++)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "INSERT INTO TERMINALHAPPYHOURPROFILES ("
            "HAPPYHOURPROFILES_KEY, "
            "TERMINALS_KEY "
            " ) "
            " VALUES ( "
            ":HAPPYHOURPROFILES_KEY, "
            ":TERMINALS_KEY "
            " );";

            IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = profileKey;
            IBInternalQuery->ParamByName("TERMINALS_KEY")->AsInteger = *iter;
            IBInternalQuery->ExecQuery();
        }
    }
    catch(Exception &Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}
//---------------------------------------------------------------------------

std::vector<THappyHour> TDBHappyHour::GetAvailableHappyHourProfile(Database::TDBTransaction &dBTransaction,bool allTerminalsHHProfile)
{
    std::vector<THappyHour> happyHourProfile = std::vector<THappyHour>();
    THappyHour hhProfile;

    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);
    Database::TcpIBSQL SelectQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(SelectQuery);

    try
    {

      if (allTerminalsHHProfile)
      {
           // load hapy hour profiles of all terminals

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
             "SELECT "
                "hhp.HAPPYHOURPROFILES_KEY, "
                "hhp.HAPPYHOURPROFILE_NAME, "
                "hhp.HAPPYHOURDAY_STARTTIME, "
                "hhp.HAPPYHOURDAY_ENDTIME, "
                "hhp.HAPPYHOURDAY_PROFILEDATE, "
                "hhp.PRICELEVEL_KEY, "
                "coalesce(hprior.HAPPYHOUR_PRIORITY_VAL,0) HAPPYHOUR_PRIORITY_VAL, "
                "Thp.TERMINALS_KEY "
            "FROM HAPPYHOURPROFILES hhp inner join TERMINALHAPPYHOURPROFILES Thp on  hhp.HAPPYHOURPROFILES_KEY=thp.HAPPYHOURPROFILES_KEY "
                "left join  HAPPYHOUR_PRIORITY hprior on hprior.HAPPYHOURPROFILES_KEY =hhp.HAPPYHOURPROFILES_KEY "
            "WHERE "
                "Thp.TERMINALS_KEY <> :TERMINALS_KEY AND "
                "hhp.ISACTIVE  = :ISACTIVE; ";
            IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
            IBInternalQuery->ParamByName("TERMINALS_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.DeviceKey;

             IBInternalQuery->ExecQuery();



      }else
      {
            // Load  HAPPYHOURPROFILES for this terminal only.
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
             "SELECT "
                "hhp.HAPPYHOURPROFILES_KEY, "
                "hhp.HAPPYHOURPROFILE_NAME, "
                "hhp.HAPPYHOURDAY_STARTTIME, "
                "hhp.HAPPYHOURDAY_ENDTIME, "
                "hhp.HAPPYHOURDAY_PROFILEDATE, "
                "hhp.PRICELEVEL_KEY, "
                "coalesce(hprior.HAPPYHOUR_PRIORITY_VAL,0) HAPPYHOUR_PRIORITY_VAL, "
                "Thp.TERMINALS_KEY "
             "FROM HAPPYHOURPROFILES hhp inner join TERMINALHAPPYHOURPROFILES Thp on  hhp.HAPPYHOURPROFILES_KEY=thp.HAPPYHOURPROFILES_KEY "
                "left join  HAPPYHOUR_PRIORITY hprior on hprior.HAPPYHOURPROFILES_KEY =hhp.HAPPYHOURPROFILES_KEY "
            "WHERE "
                "Thp.TERMINALS_KEY = :TERMINALS_KEY AND "
                "hhp.ISACTIVE  = :ISACTIVE; ";
            IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
             IBInternalQuery->ParamByName("TERMINALS_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.DeviceKey;
             IBInternalQuery->ExecQuery();


         }
        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            hhProfile.ProfileKey = IBInternalQuery->FieldByName("HAPPYHOURPROFILES_KEY")->AsInteger;
            hhProfile.ProfileName = IBInternalQuery->FieldByName("HAPPYHOURPROFILE_NAME")->AsString;
            hhProfile.StartTime = IBInternalQuery->FieldByName("HAPPYHOURDAY_STARTTIME")->AsDateTime;
            hhProfile.EndTime = IBInternalQuery->FieldByName("HAPPYHOURDAY_ENDTIME")->AsDateTime;
            hhProfile.DateTime = IBInternalQuery->FieldByName("HAPPYHOURDAY_PROFILEDATE")->AsDateTime;
            hhProfile.Pricelevel = IBInternalQuery->FieldByName("PRICELEVEL_KEY")->AsInteger;
            hhProfile.Priority = IBInternalQuery->FieldByName("HAPPYHOUR_PRIORITY_VAL")->AsInteger;
            hhProfile.TerminalKey = IBInternalQuery->FieldByName("TERMINALS_KEY")->AsInteger;

            hhProfile.IsEditedProfile=false;

            // Load info from HAPPYHOURDAYS table for selected profile.
            SelectQuery->Close();
            SelectQuery->SQL->Text =
            "SELECT  HAPPYHOURDAYS_KEY "
            "FROM HAPPYHOURDAYS "
            "WHERE "
            "HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY "
            "ORDER BY HAPPYHOURDAYS_KEY ASC;" ;

            SelectQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = hhProfile.ProfileKey;
            SelectQuery->ExecQuery();

            hhProfile.HappyHourProfileDays.clear();
            for ( ; !SelectQuery->Eof; SelectQuery->Next())
            {
                hhProfile.HappyHourProfileDays.push_back(SelectQuery->FieldByName("HAPPYHOURDAYS_KEY")->AsInteger);
            }

            // Load info from HAPPYHOURSETTINGS table for selected profile.
            SelectQuery->Close();
            SelectQuery->SQL->Text =
            "SELECT  HAPPYHOURSETTINGS_KEY "
            "FROM HAPPYHOURSETTINGS "
            "WHERE "
            "HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY "
            "ORDER BY HAPPYHOURSETTINGS_KEY ASC ;";

            SelectQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = hhProfile.ProfileKey;
            SelectQuery->ExecQuery();

            hhProfile.HappyHourProfileSettings.clear();
            for ( ; !SelectQuery->Eof; SelectQuery->Next())
            {
                hhProfile.HappyHourProfileSettings.push_back(SelectQuery->FieldByName("HAPPYHOURSETTINGS_KEY")->AsInteger);
            }

            // Load info from TERMINALHAPPYHOURPROFILES table for selected profile.
            SelectQuery->Close();
            SelectQuery->SQL->Text =
            "SELECT  TERMINALS_KEY "
            "FROM TERMINALHAPPYHOURPROFILES "
            "WHERE "
            "HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY "
            "ORDER BY TERMINALS_KEY ASC;" ;

            SelectQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = hhProfile.ProfileKey;
            SelectQuery->ExecQuery();

            hhProfile.ProfileTerminalsKeys.clear();
            for ( ; !SelectQuery->Eof; SelectQuery->Next())
            {
                hhProfile.ProfileTerminalsKeys.push_back(SelectQuery->FieldByName("TERMINALS_KEY")->AsInteger);
            }

            happyHourProfile.push_back(hhProfile);
        }
    }
    catch(Exception &Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }

    return happyHourProfile;
}
//---------------------------------------------------------------------------

void TDBHappyHour::UpdateProfile(Database::TDBTransaction &DBTransaction,THappyHour* happyHourProfiles)
{
    // update the HAPPY hour profile
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    DBTransaction.RegisterQuery(IBInternalQuery);

    try
    {
        // Update profile info into HAPPYHOURPROFILES table.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
   	    "UPDATE HAPPYHOURPROFILES "
		"SET "
		"HAPPYHOURPROFILE_NAME = :HAPPYHOURPROFILE_NAME,"
		"HAPPYHOURDAY_STARTTIME = :HAPPYHOURDAY_STARTTIME, "
       	"HAPPYHOURDAY_ENDTIME = :HAPPYHOURDAY_ENDTIME, "
        "HappyHourDay_ProfileDate = :HappyHourDay_ProfileDate, "
        "PRICELEVEL_KEY = :PRICELEVEL_KEY "
        "WHERE "
		"HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY ;";

        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = happyHourProfiles->ProfileKey;
        IBInternalQuery->ParamByName("HAPPYHOURPROFILE_NAME")->AsString = happyHourProfiles->ProfileName;
        IBInternalQuery->ParamByName("HAPPYHOURDAY_STARTTIME")->AsDateTime = happyHourProfiles->StartTime;
        IBInternalQuery->ParamByName("HAPPYHOURDAY_ENDTIME")->AsDateTime = happyHourProfiles->EndTime;
        IBInternalQuery->ParamByName("HappyHourDay_ProfileDate")->AsDateTime = happyHourProfiles->DateTime;
        IBInternalQuery->ParamByName("PRICELEVEL_KEY")->AsInteger = happyHourProfiles->Pricelevel;

        IBInternalQuery->ExecQuery();
    }

    catch(Exception & Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}
//---------------------------------------------------------------------------

void TDBHappyHour::UpdateProfileDays(Database::TDBTransaction &DBTransaction,THappyHour* happyHourProfiles)
{
    // update the HAPPY hour profile
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    DBTransaction.RegisterQuery(IBInternalQuery);

    try
    {
        // Update days info into HAPPYHOURDAYS table.
        // First delete existing info then add new info for current profile key.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
   	    "DELETE FROM HAPPYHOURDAYS "
		"WHERE "
		"HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY ;";
        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = happyHourProfiles->ProfileKey;
        IBInternalQuery->ExecQuery();

        for (std::vector<Integer>::iterator iter = happyHourProfiles->HappyHourProfileDays.begin() ; iter != happyHourProfiles->HappyHourProfileDays.end() ; iter++)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "INSERT INTO HAPPYHOURDAYS ("
            "HAPPYHOURPROFILES_KEY, "
            "HAPPYHOURDAYS_KEY "
            " ) "
            "VALUES ("
            ":HAPPYHOURPROFILES_KEY, "
            ":HAPPYHOURDAYS_KEY "
            " ); ";

            IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = happyHourProfiles->ProfileKey;
            IBInternalQuery->ParamByName("HAPPYHOURDAYS_KEY")->AsInteger = *iter;
            IBInternalQuery->ExecQuery();
        }

    }
    catch(Exception & Err)
    {

        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}
//---------------------------------------------------------------------------

void TDBHappyHour::UpdateProfileSettings(Database::TDBTransaction &DBTransaction,THappyHour* happyHourProfiles)
{
    // update the HAPPY hour profile
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    DBTransaction.RegisterQuery(IBInternalQuery);

    try
    {
        // Update settings info into HAPPYHOURSETTINGS table.
        // First delete existing info then add new info for current profile key.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
   	    "DELETE FROM HAPPYHOURSETTINGS "
		"WHERE "
		"HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY ;";
        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = happyHourProfiles->ProfileKey;
        IBInternalQuery->ExecQuery();

        for (std::vector<Integer>::iterator iter = happyHourProfiles->HappyHourProfileSettings.begin() ; iter != happyHourProfiles->HappyHourProfileSettings.end() ; iter++)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "INSERT INTO HAPPYHOURSETTINGS ("
            "HAPPYHOURPROFILES_KEY, "
            "HAPPYHOURSETTINGS_KEY "
            " ) "
            " VALUES ( "
            ":HAPPYHOURPROFILES_KEY, "
            ":HAPPYHOURSETTINGS_KEY "
            " );";

            IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = happyHourProfiles->ProfileKey;
            IBInternalQuery->ParamByName("HAPPYHOURSETTINGS_KEY")->AsInteger = *iter;
            IBInternalQuery->ExecQuery();
        }
    }
    catch(Exception & Err)
    {

        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}


void TDBHappyHour::UpdateTerminalMapping(Database::TDBTransaction &DBTransaction,THappyHour* happyHourProfiles)
{
    // update the HAPPY hour profile
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    DBTransaction.RegisterQuery(IBInternalQuery);

    try
    {
        // Update terminal info into TERMINALHAPPYHOURPROFILES table.
        // First delete existing info then add new info for current profile key.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
   	    "DELETE FROM TERMINALHAPPYHOURPROFILES "
		"WHERE "
		"HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY ;";
        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = happyHourProfiles->ProfileKey;
        IBInternalQuery->ExecQuery();

        for (std::vector<Integer>::iterator iter = happyHourProfiles->ProfileTerminalsKeys.begin() ; iter != happyHourProfiles->ProfileTerminalsKeys.end() ; iter++)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "INSERT INTO TERMINALHAPPYHOURPROFILES ("
            "HAPPYHOURPROFILES_KEY, "
            "TERMINALS_KEY "
            " ) "
            " VALUES ( "
            ":HAPPYHOURPROFILES_KEY, "
            ":TERMINALS_KEY "
            " );";

            IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = happyHourProfiles->ProfileKey;

            IBInternalQuery->ParamByName("TERMINALS_KEY")->AsInteger = *iter;
            IBInternalQuery->ExecQuery();
        }
    }
    catch(Exception & Err)
    {

        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }

}
//---------------------------------------------------------------------------

void TDBHappyHour::DeleteProfile(Database::TDBTransaction &dBTransaction, int profileKey)
{
   // update the isdeleted  field in  database by matching profile key
    // update the HAPPY hour profile

    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);
    try
    {
        // Update the ISACTIVE field in HAPPYHOURPROFILES table for matching profile key.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
   	    "UPDATE HAPPYHOURPROFILES "
        "SET "
		"ISACTIVE = :ISACTIVE "
        "WHERE "
		"HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY ;";

        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = profileKey;
        IBInternalQuery->ParamByName("ISACTIVE")->AsString  = "F";
        IBInternalQuery->ExecQuery();
    }
    catch(Exception & Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}
//---------------------------------------------------------------------------

void TDBHappyHour::DeleteProfileSettings(Database::TDBTransaction &dBTransaction, int profileKey)
{
   // update the isdeleted  field in  database by matching profile key
    // update the HAPPY hour profile

    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
   	    "DELETE FROM HAPPYHOURSETTINGS "
		"WHERE "
		"HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY ;";
        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = profileKey;
        IBInternalQuery->ExecQuery();
    }
     catch(Exception & Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}
//---------------------------------------------------------------------------

void TDBHappyHour::DeleteProfileDays(Database::TDBTransaction &DBTransaction, int profileKey)
{
   // update the isdeleted  field in  database by matching profile key
    // update the HAPPY hour profile
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    DBTransaction.RegisterQuery(IBInternalQuery);
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
   	    "DELETE FROM HAPPYHOURDAYS "
		"WHERE "
		"HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY ;";
        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = profileKey;
        IBInternalQuery->ExecQuery();
    }
    catch(Exception & Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}
//---------------------------------------------------------------------------

void TDBHappyHour::DeleteTerminalMapping(Database::TDBTransaction &dBTransaction, int profileKey)
{
   // update the isdeleted  field in  database by matching profile key
    // update the HAPPY hour profile

    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
   	    "DELETE FROM TERMINALHAPPYHOURPROFILES "
		"WHERE "
		"HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY ;";
        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = profileKey;
        IBInternalQuery->ExecQuery();
    }
    catch(Exception & Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }

 }
//---------------------------------------------------------------------------

 bool TDBHappyHour::IsHHProfilePresent(Database::TDBTransaction &dBTransaction, int profileKey)
{
    bool profilePresent = false;

    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);

    try
    {
        // Check profile is active or not.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT count(*) from  HAPPYHOURPROFILES "
           "WHERE HAPPYHOURPROFILES_KEY =:HAPPYHOURPROFILES_KEY  ;";
        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger =  profileKey;
        IBInternalQuery->ExecQuery();

        if( IBInternalQuery->Fields[0]->AsInteger > 0)
        {
            profilePresent=true;
        }

        return profilePresent;

    }
    catch(Exception & Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }

}

//---------------------------------------------------------------------------

std::vector<THappyHour *> TDBHappyHour::GetHappyHourTime(Database::TDBTransaction &dBTransaction, int terminalKey)
{

    bool isProfileDay=false;
    std::vector<THappyHour *>happyHourProfile = std::vector<THappyHour *>();

    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);
    Database::TcpIBSQL SelectQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(SelectQuery);
    TDateTime currenttimestamp=Now();


    try
    {
        // Load profiles from TERMINALHAPPYHOURPROFILES table for selected terminal.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "SELECT  HAPPYHOURPROFILES_KEY "
        "FROM TERMINALHAPPYHOURPROFILES "
        "WHERE TERMINALS_KEY = :TERMINALS_KEY "
        "ORDER BY HAPPYHOURPROFILES_KEY ASC;" ;

        IBInternalQuery->ParamByName("TERMINALS_KEY")->AsInteger = terminalKey;
        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            THappyHour* hhProfile = new THappyHour();
            isProfileDay = false;

            // Load info from HAPPYHOURPROFILES table.
            SelectQuery->Close();
            SelectQuery->SQL->Text =
            "SELECT a.HAPPYHOURPROFILES_KEY, "
                    "a.HAPPYHOURPROFILE_NAME, a.HAPPYHOURDAY_STARTTIME, "
                    "a.HAPPYHOURDAY_ENDTIME, a.HAPPYHOURDAY_PROFILEDATE,a.PRICELEVEL_KEY, "
                    "coalesce(hprior.HAPPYHOUR_PRIORITY_VAL,0) HAPPYHOUR_PRIORITY_VAL  "
            "FROM HAPPYHOURPROFILES a "
            "left join  HAPPYHOUR_PRIORITY hprior on hprior.HAPPYHOURPROFILES_KEY =a.HAPPYHOURPROFILES_KEY  "
            "WHERE a.HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY and a.ISACTIVE= :ISACTIVE ;";

            SelectQuery->ParamByName("ISACTIVE")->AsString = "T";
            SelectQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = IBInternalQuery->FieldByName("HAPPYHOURPROFILES_KEY")->AsInteger;
            SelectQuery->ExecQuery();

            if(!SelectQuery->Eof)
            {

                        hhProfile->ProfileKey = SelectQuery->FieldByName("HAPPYHOURPROFILES_KEY")->AsInteger;
                        hhProfile->ProfileName = SelectQuery->FieldByName("HAPPYHOURPROFILE_NAME")->AsString;
                        hhProfile->StartTime = SelectQuery->FieldByName("HAPPYHOURDAY_STARTTIME")->AsDateTime;
                        hhProfile->EndTime = SelectQuery->FieldByName("HAPPYHOURDAY_ENDTIME")->AsDateTime;
                        hhProfile->DateTime = SelectQuery->FieldByName("HAPPYHOURDAY_PROFILEDATE")->AsDateTime;
                        hhProfile->Pricelevel = SelectQuery->FieldByName("PRICELEVEL_KEY")->AsInteger;
                        hhProfile->Priority = SelectQuery->FieldByName("HAPPYHOUR_PRIORITY_VAL")->AsInteger;


                          // Load info from HAPPYHOURDAYS table for selected profile.
                        SelectQuery->Close();
                        SelectQuery->SQL->Text =
                        "SELECT  HAPPYHOURDAYS_KEY "
                        "FROM HAPPYHOURDAYS "
                        "WHERE "
                        "HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY "
                        "ORDER BY HAPPYHOURDAYS_KEY ASC;" ;

                        SelectQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = hhProfile->ProfileKey;
                        SelectQuery->ExecQuery();

                        hhProfile->HappyHourProfileDays.clear();
                        for ( ; !SelectQuery->Eof; SelectQuery->Next())
                        {
                            hhProfile->HappyHourProfileDays.push_back(SelectQuery->FieldByName("HAPPYHOURDAYS_KEY")->AsInteger);
                        }
                      //  System::TDateTime currentTime = Now();

                      TDateTime currentTime = Dateutils::EncodeDateTime(YearOf(Now()),MonthOf(Now()),DayOf(Now()),
                                                    HourOfTheDay(Now()),MinuteOfTheHour(Now()),SecondOfTheMinute(Now()),000); //in this second and millisecond change to 0 because
                       // before this we were passing seconds of now so HH time ends some seconds late (in db second were stored from Now())


                        bool inTime = false;
                        bool isValidProfile = false;

                        //check whether any profile fall under the current time stamp

                       if ( YearOf(hhProfile->DateTime)!= 1899 )   // if date is set
                       {

                          if((!(Dateutils::CompareDateTime(hhProfile->StartTime,Now()) == GreaterThanValue)) && (!(Dateutils::CompareDateTime(hhProfile->EndTime,Now()) == LessThanValue)))
                            {
                                 isValidProfile = true;
                            }
                       }
                        else   //  if No Date is Set
                       {
                           //check if current day is present  in profile day settings
                            for (int day=0; day<hhProfile->HappyHourProfileDays.size();   day++)
                            {

                                 if(hhProfile->HappyHourProfileDays[day]==0)
                                     hhProfile->HappyHourProfileDays[day]=7;

                                 if( DayOfTheWeek(currentTime)== hhProfile->HappyHourProfileDays[day] )
                                 {
                                    isProfileDay=true;
                                 }
                            }

                          if (isProfileDay)  //if current day is profile day then check the time
                          {
                               if (SecondOfTheDay(currentTime) >= SecondOfTheDay(hhProfile->StartTime) && SecondOfTheDay(currentTime) <= SecondOfTheDay(hhProfile->EndTime) )
                               {
                                     isValidProfile = true;
                               }
                         }
                       }

                            if (isValidProfile)
                            {

                             // Load info from HAPPYHOURSETTINGS table for selected profile.
                                SelectQuery->Close();
                                SelectQuery->SQL->Text =
                                "SELECT  HAPPYHOURSETTINGS_KEY "
                                "FROM HAPPYHOURSETTINGS "
                                "WHERE "
                                "HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY "
                                "ORDER BY HAPPYHOURSETTINGS_KEY ASC ;";

                                SelectQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = hhProfile->ProfileKey;
                                SelectQuery->ExecQuery();

                                hhProfile->HappyHourProfileSettings.clear();
                                for ( ; !SelectQuery->Eof; SelectQuery->Next())
                                {
                                    hhProfile->HappyHourProfileSettings.push_back(SelectQuery->FieldByName("HAPPYHOURSETTINGS_KEY")->AsInteger);
                                }


                                // Load info from TERMINALHAPPYHOURPROFILES table for selected profile.
                                SelectQuery->Close();
                                SelectQuery->SQL->Text =
                                "SELECT  TERMINALS_KEY "
                                "FROM TERMINALHAPPYHOURPROFILES "
                                "WHERE "
                                "HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY "
                                "ORDER BY TERMINALS_KEY ASC;" ;

                                SelectQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = hhProfile->ProfileKey;
                                SelectQuery->ExecQuery();

                                hhProfile->ProfileTerminalsKeys.clear();
                                for ( ; !SelectQuery->Eof; SelectQuery->Next())
                                {
                                    hhProfile->ProfileTerminalsKeys.push_back(SelectQuery->FieldByName("TERMINALS_KEY")->AsInteger);
                                }

                                happyHourProfile.push_back(hhProfile);
                            }
                 }

       //   delete hhProfile;
         }

    }

    catch(Exception &Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
     return happyHourProfile;
}

//get alll terminals keys
std::vector<int> TDBHappyHour::GetDevicesKeys(Database::TDBTransaction &dBTransaction)
{

   std::vector<int> deviceKeys;

    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text =
    "SELECT  DEVICE_KEY FROM DEVICES a where a.DEVICE_ID>=0 ;";

    IBInternalQuery->ExecQuery();

    for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
    {
        deviceKeys.push_back(IBInternalQuery->FieldByName("DEVICE_KEY")->AsInteger);
    }
    return  deviceKeys;

}



void TDBHappyHour::DeleteTerminalMapping(Database::TDBTransaction &dBTransaction,int profileKey, int terminalKey)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);

    //Delete the profiles mapping with terminals
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text =
     "DELETE FROM TERMINALHAPPYHOURPROFILES "
       "WHERE "
    "HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY And "
    "TERMINALS_KEY = :TERMINALS_KEY ;" ;
    IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger =profileKey;
    IBInternalQuery->ParamByName("TERMINALS_KEY")->AsInteger = terminalKey;
    IBInternalQuery->ExecQuery();
}


int TDBHappyHour::MaxProfilesNo(Database::TDBTransaction &dBTransaction)
{
    int profileNo=0;
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT coalesce(max( HAPPYHOURPROFILES_KEY),0) FROM HAPPYHOURPROFILES";
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
       profileNo = IBInternalQuery->Fields[0]->AsInteger;

    return  profileNo;

}


 std::map<AnsiString,int>  TDBHappyHour:: GetAllPriceLevels(Database::TDBTransaction &dBTransaction)
 {
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);

     std::map<AnsiString,int> ProfilePriceLevels;
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT a.PRICELEVEL_KEY, a.PRICELEVEL_NAME, a.ISENABLED  "
                                "FROM PRICELEVELS a where a.ISENABLED = :ISENABLED  ";
    IBInternalQuery->ParamByName("ISENABLED")->AsString = "T";
    IBInternalQuery->ExecQuery();

   for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
    {

      int key=  IBInternalQuery->Fields[0]->AsInteger;
      AnsiString priceLevelName=IBInternalQuery->Fields[1]->AsString;

       ProfilePriceLevels.insert ( std::pair<AnsiString,int>(priceLevelName,key) );

    }

    return  ProfilePriceLevels;

 }
 ////----------------------------------------------------------------------------

 void  TDBHappyHour:: UpdateProfilePriority(Database::TDBTransaction &DBTransaction, THappyHour* happyHourProfiles)
 {
     // update the HAPPY hour profile
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    DBTransaction.RegisterQuery(IBInternalQuery);

    try
    {

        // First delete existing info then add new info for current profile key.

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
   	    "DELETE FROM HAPPYHOUR_PRIORITY "
		"WHERE "
		"HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY ;";
        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = happyHourProfiles->ProfileKey;
        IBInternalQuery->ExecQuery();


        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
            "INSERT INTO HAPPYHOUR_PRIORITY (HAPPYHOURPROFILES_KEY, HAPPYHOUR_PRIORITY_VAL )  "
            "VALUES (    "
            ":HAPPYHOURPROFILES_KEY,  "
            ":HAPPYHOUR_PRIORITY_VAL  ) ";

        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = happyHourProfiles->ProfileKey;
        IBInternalQuery->ParamByName("HAPPYHOUR_PRIORITY_VAL")->AsInteger = happyHourProfiles->Priority;
        IBInternalQuery->ExecQuery();




    }
    catch(Exception & Err)
    {

        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }

 }


void TDBHappyHour::SaveProfilePriority(Database::TDBTransaction &DBTransaction,THappyHour* happyHourProfiles, int profileKey)
{

     // update the HAPPY hour profile
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    DBTransaction.RegisterQuery(IBInternalQuery);

    try
    {
        // Update terminal info into TERMINALHAPPYHOURPROFILES table.
        // First delete existing info then add new info for current profile key.


        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
            "INSERT INTO HAPPYHOUR_PRIORITY (HAPPYHOURPROFILES_KEY, HAPPYHOUR_PRIORITY_VAL )  "
            "VALUES (    "
            ":HAPPYHOURPROFILES_KEY,  "
            ":HAPPYHOUR_PRIORITY_VAL  ) ";

        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = profileKey;
        IBInternalQuery->ParamByName("HAPPYHOUR_PRIORITY_VAL")->AsInteger = happyHourProfiles->Priority;
        IBInternalQuery->ExecQuery();

    }
    catch(Exception & Err)
    {

        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}

 void TDBHappyHour::DeleteprofilePriority(Database::TDBTransaction &dBTransaction, int profile_key)
 {
    // update the isdeleted  field in  database by matching profile key
    // update the HAPPY hour profile

    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dBTransaction.RegisterQuery(IBInternalQuery);
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
   	    "DELETE FROM HAPPYHOUR_PRIORITY "
		"WHERE "
		"HAPPYHOURPROFILES_KEY = :HAPPYHOURPROFILES_KEY ;";
        IBInternalQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger = profile_key;
        IBInternalQuery->ExecQuery();
    }
    catch(Exception & Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }

 }

 






