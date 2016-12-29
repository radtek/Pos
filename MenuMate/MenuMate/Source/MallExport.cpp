//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExport.h"
#include "MMLogging.h"
#include "MallExportTextFile.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

bool TMallExport::PushToDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey)
{
    bool retVal = false;
    try
    {
        // Create TMallExportPrepareData object to store Preapared Data
        std::list<TMallExportSalesData> salesData;

        ///Prepare Data For Inserting Data Into DataBase
        salesData = PrepareDataForDatabase(paymentTransaction, arcBillKey);

        //Insert Prepared Data into DataBase
        retVal = InsertInToMallExport_Sales(paymentTransaction.DBTransaction, salesData);
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return retVal;
}
//-------------------------------------------------------------------------------
bool TMallExport::Export()
{
    try
    {
        TMallExportPrepareData preparedData;

        //Prepare Data For Exporting into File
        preparedData = PrepareDataForExport();
        bool transactionDoneBeforeZed =  CheckTransactionDoneBeforeZed();

        if(transactionDoneBeforeZed)
        {
            //Create Export Medium
            TMallExportTextFile* exporter = (TMallExportTextFile*)CreateExportMedium();
            exporter->WriteToFile(preparedData);
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return true;
}
//----------------------------------------------------------
bool TMallExport::InsertInToMallExport_Sales(Database::TDBTransaction &dbTransaction , std::list<TMallExportSalesData> mallExportSalesData)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dbTransaction.RegisterQuery(IBInternalQuery);
    bool isInserted = false;
    try
    {
        std::list<TMallExportSalesData>::iterator it;
        //Iterate mallExport Sales data for inserting into DB
        for(it = mallExportSalesData.begin(); it != mallExportSalesData.end(); it++)
        {
            // Inserting Each field of nall into Table
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "INSERT INTO MALLEXPORT_SALES ("
                    "MALLEXPORT_SALE_KEY, "
                    "MALL_KEY, "
                    "FIELD_INDEX, "
                    "FIELD, "
                    "FIELD_VALUE, "
                    "VALUE_TYPE, "
                    "DATE_CREATED, "
                    "CREATED_BY, "
                    "Z_KEY, "
                    "ARCBILL_KEY, "
                    "DEVICE_KEY "
                    " ) "
            "VALUES ("
                    ":MALLEXPORT_SALE_KEY, "
                    ":MALL_KEY, "
                    ":FIELD_INDEX, "
                    ":FIELD, "
                    ":FIELD_VALUE, "
                    ":VALUE_TYPE, "
                    ":DATE_CREATED, "
                    ":CREATED_BY, "
                    ":Z_KEY,"
                    ":ARCBILL_KEY, "
                    ":DEVICE_KEY "
                    " );";

            IBInternalQuery->ParamByName("MALLEXPORT_SALE_KEY")->AsInteger = it->MallExportSalesId;
            IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = it->MallKey;
            IBInternalQuery->ParamByName("FIELD_INDEX")->AsInteger = it->FieldIndex;
            IBInternalQuery->ParamByName("FIELD")->AsString = it->Field;
            IBInternalQuery->ParamByName("FIELD_VALUE")->AsString = it->DataValue;
            IBInternalQuery->ParamByName("VALUE_TYPE")->AsString = it->DataValueType;
            IBInternalQuery->ParamByName("DATE_CREATED")->AsDateTime = it->DateCreated;
            IBInternalQuery->ParamByName("CREATED_BY")->AsString = it->CreatedBy;
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = it->ZKey;
            IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = it->ArcBillKey;
            IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = it->DeviceKey;
            IBInternalQuery->ExecQuery();
        }
        isInserted = true;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return isInserted;
}
//---------------------------------------------------------------------------------------------------------------------------
bool TMallExport::CheckTransactionDoneBeforeZed()
{
    bool isZKeySame = false;
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(IBInternalQuery);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT MAX(Z_KEY) Z_KEY FROM ZEDS";
        IBInternalQuery->ExecQuery();
        int ZedKey = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT MAX(a.Z_KEY) Z_KEY FROM MALLEXPORT_SALES a";
        IBInternalQuery->ExecQuery();
        int mallZedKey = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;

        if(ZedKey == mallZedKey)
            isZKeySame = true;
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    dbTransaction.Commit();
    return isZKeySame;
}
//------------------------------------------------------------------------------------------------
void TMallExport::RegenerateMallReport(TDateTime sDate, TDateTime eDate)
{
     //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        TMallExportPrepareData preparedData;
        int zKey;
        bool isMasterTerminal = TGlobalSettings::Instance().EnableDepositBagNum;

        ///Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(IBInternalQuery);

        //Query for selecting data for invoice file
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =  "SELECT a.Z_KEY FROM MALLEXPORT_SALES a "
                                        "WHERE a.Z_KEY != :Z_KEY AND a.DATE_CREATED >= :START_TIME AND a.DATE_CREATED < :END_TIME ";

        if(!isMasterTerminal)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " AND a.DEVICE_KEY = :DEVICE_KEY ";
        }
        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                                                                "GROUP BY a.Z_KEY "
                                                                "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = 0;
        IBInternalQuery->ParamByName("START_TIME")->AsDateTime = sDate;
        IBInternalQuery->ParamByName("END_TIME")->AsDateTime = eDate;
        if(!isMasterTerminal)
        {
            IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
        }
        IBInternalQuery->ExecQuery();

        //Create Export Medium
        TMallExportTextFile* exporter =  new TMallExportTextFile();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
            //Fetch z-key
            zKey = IBInternalQuery->Fields[0]->AsInteger;
            
            //Prepare Data For Exporting into File
            preparedData = PrepareDataForExport(zKey);

           exporter->WriteToFile(preparedData);
       }
       delete exporter;

       //Display message showing status of file
       if(IBInternalQuery->RecordCount)
            MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );
       else
            MessageBox( "No Data For This Time Period", "Gernerating File", MB_OK );

         //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
