//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerMallSetup.h"
#include "DeviceRealTerminal.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


std::vector<UnicodeString> TManagerMallSetup::LoadAllMalls(Database::TDBTransaction &dbTransaction)
{
    std::vector<UnicodeString> malls;
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = " SELECT MALL_NAME FROM MALLS ORDER BY MALLS.MALL_ID ASC ";
        ibInternalQuery->ExecQuery();

        for ( ; !ibInternalQuery->Eof; ibInternalQuery->Next())
        {
            malls.push_back(ibInternalQuery->FieldByName("MALL_NAME")->AsString);
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return malls;
}
////-------------------------------------------------------------
void TManagerMallSetup::UpdateActiveMall(Database::TDBTransaction &dbTransaction, int mallKey)
{
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = " UPDATE MALLS SET IS_ACTIVE = :IS_ACTIVE WHERE MALL_ID = :MALL_ID ";

        ibInternalQuery->ParamByName("MALL_ID")->AsInteger = mallKey;
        ibInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        ibInternalQuery->ExecQuery();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-------------------------------------------------------------------------------------------------
TMall TManagerMallSetup::LoadActiveMallSettings(Database::TDBTransaction &dbTransaction)
{
    TMall mallProperties;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Clear();
        ibInternalQuery->SQL->Text = " SELECT MALLS.MALL_ID, MALLS.MALL_NAME, MALLS.IS_ACTIVE FROM MALLS "
                                     " INNER JOIN MALLEXPORT_SETTINGS_VALUES ON MALLS.MALL_ID = MALLEXPORT_SETTINGS_VALUES.MALL_KEY "
                                     " WHERE IS_ACTIVE = :IS_ACTIVE AND DEVICE_KEY = :DEVICE_KEY "
                                     "GROUP BY 1,2,3 ";

        ibInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        ibInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
        ibInternalQuery->ExecQuery();

        ///if active mall exist and have setting according to corresponding device key
        if(ibInternalQuery->RecordCount)
        {
            mallProperties.MallId = ibInternalQuery->FieldByName("MALL_ID")->AsInteger;
            mallProperties.MallName = ibInternalQuery->FieldByName("MALL_NAME")->AsString;
            mallProperties.IsActive = ibInternalQuery->FieldByName("IS_ACTIVE")->AsString;
            mallProperties.DeviceKey = TDeviceRealTerminal::Instance().ID.ProfileKey;

            ibInternalQuery->Close();
            ibInternalQuery->SQL->Clear();
            ibInternalQuery->SQL->Text =
                                    "SELECT * FROM MALLEXPORT_SETTINGS a "
                                    "INNER JOIN MALLEXPORT_SETTINGS_MAPPING msp ON msp.MALLEXPORT_SETTING_ID = a.MALLEXPORT_SETTING_KEY "
                                    "LEFT JOIN MALLEXPORT_SETTINGS_VALUES msv    on msv.MALLEXPORTSETTING_ID  = a.MALLEXPORT_SETTING_KEY "
                                    " WHERE msp.MALL_ID = :MALL_ID  and msv.DEVICE_KEY = :DEVICE_KEY ";

            ibInternalQuery->ParamByName("MALL_ID")->AsInteger = mallProperties.MallId;
            ibInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = mallProperties.DeviceKey;
            ibInternalQuery->ExecQuery();
            for(; !ibInternalQuery->Eof; ibInternalQuery->Next())
            {
                TMallExportSettings settings;
                settings.MallExportSettingMappingId = ibInternalQuery->FieldByName("MALLEXPORT_SETTING_VALUE_KEY")->AsInteger;
                settings.MallExportSettingId =  ibInternalQuery->FieldByName("MALLEXPORTSETTING_ID")->AsInteger;
                settings.Name =  ibInternalQuery->FieldByName("NAME")->AsString;
                settings.ControlName = ibInternalQuery->FieldByName("CONTROL_NAME")->AsString;
                settings.Value =  ibInternalQuery->FieldByName("FIELD_VALUE")->AsString;
                settings.ValueType = ibInternalQuery->FieldByName("FIELD_TYPE")->AsString;
                mallProperties.MallSettings.push_back(settings);
            }
        }
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return mallProperties;
}
//------------------------------------------------------------------------------------------
void TManagerMallSetup::UpdateMallExportSettingValues(Database::TDBTransaction &dbTransaction, TMall &mallInfo)
{
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    TIBSQL *ibUpdateQuery = dbTransaction.Query(dbTransaction.AddQuery());
    ibInternalQuery->Close();

    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Clear();
        ibInternalQuery->SQL->Text = "SELECT * FROM MALLEXPORT_SETTINGS_VALUES a WHERE a.DEVICE_KEY = :DEVICE_KEY and a.MALL_KEY = :MALL_KEY ";
        ibInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = mallInfo.DeviceKey;
        ibInternalQuery->ParamByName("MALL_KEY")->AsString = mallInfo.MallId;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
           
            std::list<TMallExportSettings>::iterator it;
            for(it = mallInfo.MallSettings.begin(); it != mallInfo.MallSettings.end(); it++)
            {
                ibInternalQuery->Close();
                ibInternalQuery->SQL->Clear();
                ibInternalQuery->SQL->Text = " UPDATE MALLEXPORT_SETTINGS_VALUES SET FIELD_VALUE = :FIELD_VALUE "
                                                "WHERE MALLEXPORTSETTING_ID = :MALLEXPORTSETTING_ID  "
                                                "AND DEVICE_KEY = :DEVICE_KEY "
                                                "AND MALL_KEY = :MALL_KEY ";
                ibInternalQuery->ParamByName("MALLEXPORTSETTING_ID")->AsInteger = it->MallExportSettingId;
                ibInternalQuery->ParamByName("FIELD_VALUE")->AsString = it->Value;
                ibInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = mallInfo.DeviceKey;
                ibInternalQuery->ParamByName("MALL_KEY")->AsInteger = mallInfo.MallId;
                ibInternalQuery->ExecQuery();
            }
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------
int  TManagerMallSetup::CheckActiveMallExist(Database::TDBTransaction &dbTransaction)
{
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    ibInternalQuery->Close();
    int mallId = 0;

    try
	{
        ibInternalQuery->SQL->Text = " SELECT MALL_ID FROM MALLS "
                                     " INNER JOIN MALLEXPORT_SETTINGS_VALUES ON MALLS.MALL_ID = MALLEXPORT_SETTINGS_VALUES.MALL_KEY "
                                     " WHERE IS_ACTIVE = :IS_ACTIVE AND DEVICE_KEY = :DEVICE_KEY "
                                     " GROUP BY 1 ";

        ibInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        ibInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
            mallId =  ibInternalQuery->FieldByName("MALL_ID")->AsInteger;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return mallId;
}
//---------------------------------------------------------------------------------------------------------
void TManagerMallSetup::UpdateINActiveMall(Database::TDBTransaction &dbTransaction)
{
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = " UPDATE MALLS SET IS_ACTIVE = :IS_ACTIVE ";

        ibInternalQuery->ParamByName("IS_ACTIVE")->AsString = "F";
        ibInternalQuery->ExecQuery();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------------------------
void TManagerMallSetup::InsertInToMallExport_Settings_Values(int mallId)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    int deviceKey = TDeviceRealTerminal::Instance().ID.ProfileKey;

    try
    {
        switch(mallId)
        {
            case 1:
                InsertSettingValuesForEstancia(dbTransaction, deviceKey, mallId);
                break;
            case 2:
                InsertSettingValuesForDeanAndDeluca(dbTransaction, deviceKey, mallId);
                break;
            default:
                break;
        }

        //Commit The Transaction Since We Have done All Transaction.
        dbTransaction.Commit();
    }
    catch( Exception &E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        dbTransaction.Rollback();
    }
}
//-------------------------------------------------------------------------------------------------
void TManagerMallSetup::InsertSettingValuesForEstancia(Database::TDBTransaction &dbTransaction, int deviceKey, int mallId)
{
    try
    {
        bool isRecordExist = IsSettingExistInDB(dbTransaction, deviceKey, mallId);

        if(!isRecordExist)
        {
             const int numberOfFields = 77;
             UnicodeString fieldTypes[numberOfFields] =
             {
                "UnicodeString", "UnicodeString", "int", "UnicodeString", "UnicodeString", "int", "UnicodeString", "Currency", "UnicodeString", "UnicodeString",
                "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
                "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
                "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
                "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
                "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
                "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
                "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "bool", "bool",  "UnicodeString",
                "bool", "bool", "UnicodeString"
             };

             UnicodeString fieldValues[numberOfFields] =
             {
                "", "", "", ".txt", "5", "2", "8", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12",
                "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12",
                "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "Z", "false", "false",
                "SNNNNTTMMDDYYYY.B", "true", "true", ""
             };

             int settingID[numberOfFields] =
             {
                1, 2, 7, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18,
                19, 20, 21, 24, 25, 26
             };

            TIBSQL *insertQuery = dbTransaction.Query(dbTransaction.AddQuery());
            TIBSQL *incrementGenerator = dbTransaction.Query(dbTransaction.AddQuery());

            for(int index = 0; index < numberOfFields; index++)
            {
                incrementGenerator->Close();
                incrementGenerator->SQL->Text = "SELECT GEN_ID(GEN_MALL_SETT_VAL_KEY, 1) FROM RDB$DATABASE";
                incrementGenerator->ExecQuery();

                insertQuery->Close();
                insertQuery->SQL->Text =
                            "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (:SETTING_VALUE_KEY, :SETTING_KEY, :FIELD_VALUE, :FIELD_TYPE, :DEVICE_KEY, :MALL_KEY) ";
                insertQuery->ParamByName("SETTING_VALUE_KEY")->AsInteger = incrementGenerator->Fields[0]->AsInteger;
                insertQuery->ParamByName("SETTING_KEY")->AsInteger = settingID[index];
                insertQuery->ParamByName("FIELD_VALUE")->AsString = fieldValues[index];
                insertQuery->ParamByName("FIELD_TYPE")->AsString = fieldTypes[index];
                insertQuery->ParamByName("DEVICE_KEY")->AsInteger = deviceKey;
                insertQuery->ParamByName("MALL_KEY")->AsInteger = mallId;
                insertQuery->ExecQuery();
            }
        }
    }
    catch( Exception &E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
//-------------------------------------------------------------------------------------------------
void TManagerMallSetup::InsertSettingValuesForDeanAndDeluca(Database::TDBTransaction &dbTransaction, int deviceKey, int mallId)
{
    try
    {
        bool isRecordExist = IsSettingExistInDB(dbTransaction, deviceKey, mallId);

        if(!isRecordExist)
        {
            const int numberOfFields = 10;
             UnicodeString fieldTypes[numberOfFields] =
             {
                "UnicodeString", "UnicodeString", "int", "bool", "UnicodeString", "UnicodeString", "bool", "bool", "bool", "bool"
             };

             UnicodeString fieldValues[numberOfFields] =
             {
                "", "", "", "true", ".txt", "Z", "false", "false", "true", "false"
             };

             int settingID[numberOfFields] =
             {
                1, 2, 7, 9, 16, 18, 19, 20, 24, 25
             };

            TIBSQL *insertQuery        = dbTransaction.Query( dbTransaction.AddQuery() );
            TIBSQL *incrementGenerator = dbTransaction.Query(dbTransaction.AddQuery());

            for(int index = 0; index < numberOfFields; index++)
            {
                incrementGenerator->Close();
                incrementGenerator->SQL->Text = "SELECT GEN_ID(GEN_MALL_SETT_VAL_KEY, 1) FROM RDB$DATABASE";
                incrementGenerator->ExecQuery();

                insertQuery->Close();
                insertQuery->SQL->Text =
                            "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (:SETTING_VALUE_KEY, :SETTING_KEY, :FIELD_VALUE, :FIELD_TYPE, :DEVICE_KEY, :MALL_KEY) ";
                insertQuery->ParamByName("SETTING_VALUE_KEY")->AsInteger = index + 78;
                insertQuery->ParamByName("SETTING_KEY")->AsInteger = settingID[index];
                insertQuery->ParamByName("FIELD_VALUE")->AsString = fieldValues[index];
                insertQuery->ParamByName("FIELD_TYPE")->AsString = fieldTypes[index];
                insertQuery->ParamByName("DEVICE_KEY")->AsInteger = deviceKey;
                insertQuery->ParamByName("MALL_KEY")->AsInteger = mallId;
                insertQuery->ExecQuery();
            }
        }
    }
    catch( Exception &E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
//--------------------------------------------------------------------------------------------------------------
bool TManagerMallSetup::IsSettingExistInDB(Database::TDBTransaction &dbTransaction, int deviceKey, int mallID)
{
    bool isSettingExist = false;
    try
    {
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Clear();
        ibInternalQuery->SQL->Text = "SELECT * FROM MALLEXPORT_SETTINGS_VALUES a WHERE a.DEVICE_KEY = :DEVICE_KEY and a.MALL_KEY = :MALL_KEY ";
        ibInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = deviceKey;
        ibInternalQuery->ParamByName("MALL_KEY")->AsString = mallID;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            isSettingExist = true;
        }
    }

    catch( Exception &E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    return isSettingExist;
}
