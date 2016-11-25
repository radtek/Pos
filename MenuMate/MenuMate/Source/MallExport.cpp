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
                    "ARCBILL_KEY "
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
                    ":ARCBILL_KEY "
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
