//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallHelper.h"
#include "DeviceRealTerminal.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)

TMallHelper::TMallHelper()
{
}

UnicodeString TMallHelper::GetExportType(int mallid)
{
    UnicodeString typeOfFile = "";
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(IBInternalQuery);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT MES.NAME, MES.MALLEXPORT_SETTING_KEY, MSP.MALL_ID, MSV.FIELD_VALUE  "
                                     "FROM MALLEXPORT_SETTINGS MES "
                                     "INNER JOIN MALLEXPORT_SETTINGS_MAPPING MSP ON MES.MALLEXPORT_SETTING_KEY = MSP.MALLEXPORT_SETTING_ID "
                                     "INNER JOIN MALLEXPORT_SETTINGS_VALUES MSV ON MSP.MALLEXPORT_SETTING_ID = MSV.MALLEXPORTSETTING_ID AND MSP.MALL_ID = MSV.MALL_KEY "
                                      "WHERE MES.NAME = :NAME AND MSP.MALL_ID = :MALL_ID";

        IBInternalQuery->ParamByName("NAME")->AsString = "TYPE_OF_FILE";
        IBInternalQuery->ParamByName("MALL_ID")->AsInteger = mallid;
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            typeOfFile = IBInternalQuery->Fields[3]->AsString;
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return typeOfFile;
}


double TMallHelper::GetOldAccumulatedSales(Database::TDBTransaction &dbTransaction, int fieldIndex,int mallid)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dbTransaction.RegisterQuery(IBInternalQuery);
    double oldAccumulatedSales = 0.00;
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT Z_KEY FROM MALLEXPORT_SALES a WHERE a.MALL_KEY = :MALL_KEY ";

        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.DEVICE_KEY = :DEVICE_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY 1";
        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = mallid ;

        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
        }

        IBInternalQuery->ExecQuery();
        bool  recordPresent = false;

        if(IBInternalQuery->RecordCount )
           recordPresent = true;

        if(recordPresent)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
                                        "SELECT a.FIELD_INDEX, A.FIELD, A.FIELD_VALUE "
                                        "FROM MALLEXPORT_SALES a "
                                        "WHERE  a.MALLEXPORT_SALE_KEY = (SELECT MAX(A.MALLEXPORT_SALE_KEY) FROM MALLEXPORT_SALES a WHERE A.FIELD_INDEX  = :FIELD_INDEX ";
            if(!TGlobalSettings::Instance().EnableDepositBagNum)
            {
                IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.DEVICE_KEY = :DEVICE_KEY ";
            }
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + ")";

            IBInternalQuery->ParamByName("FIELD_INDEX")->AsString = fieldIndex;
            if(!TGlobalSettings::Instance().EnableDepositBagNum)
            {
                IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
            }
            IBInternalQuery->ExecQuery();

            if(IBInternalQuery->RecordCount)
                oldAccumulatedSales = IBInternalQuery->Fields[2]->AsCurrency;
        }
        else
        {
            oldAccumulatedSales = 0;
        }
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return oldAccumulatedSales;
}