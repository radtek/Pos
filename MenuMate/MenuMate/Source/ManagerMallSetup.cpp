//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerMallSetup.h"
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
        ibInternalQuery->SQL->Text = " SELECT MALL_NAME FROM MALLS ORDER BY 1 ASC ";
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
        ibInternalQuery->SQL->Text = "SELECT * FROM MALLS a WHERE a.IS_ACTIVE = 'T'";
        ibInternalQuery->ExecQuery();
        mallProperties.MallId = ibInternalQuery->FieldByName("MALL_ID")->AsInteger;
        mallProperties.MallName = ibInternalQuery->FieldByName("MALL_NAME")->AsString;
        mallProperties.IsActive = ibInternalQuery->FieldByName("IS_ACTIVE")->AsString;

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Clear();
        ibInternalQuery->SQL->Text = "SELECT * FROM MALLEXPORT_SETTINGS_VALUES msv "
                                        "INNER JOIN MALLEXPORT_SETTINGS a   on a.MALLEXPORT_SETTING_KEY = msv.MALLEXPORTSETTING_ID "
                                        "INNER JOIN MALLEXPORT_SETTINGS_MAPPING msp ON msp.MALLEXPORT_SETTING_ID = msv.MALLEXPORTSETTING_ID "
                                    "WHERE msp.MALL_ID = :MALL_ID ";

        ibInternalQuery->ParamByName("MALL_ID")->AsInteger = mallProperties.MallId;
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
        std::list<TMallExportSettings>::iterator it;
        for(it = mallInfo.MallSettings.begin(); it != mallInfo.MallSettings.end(); it++)
        {
            ibInternalQuery->SQL->Text = " UPDATE MALLEXPORT_SETTINGS_VALUES SET FIELD_VALUE = :FIELD_VALUE WHERE MALLEXPORTSETTING_ID = :MALLEXPORTSETTING_ID  ";
            ibInternalQuery->ParamByName("MALLEXPORTSETTING_ID")->AsInteger = it->MallExportSettingId;
            ibInternalQuery->ParamByName("FIELD_VALUE")->AsString = it->Value;
            ibInternalQuery->ExecQuery();
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
        ibInternalQuery->SQL->Text = " SELECT MALL_ID FROM MALLS WHERE IS_ACTIVE = :IS_ACTIVE ";

        ibInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
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
