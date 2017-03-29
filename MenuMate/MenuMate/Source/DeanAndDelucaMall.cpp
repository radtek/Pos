//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeanAndDelucaMall.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TDeanAndDelucaMall::TDeanAndDelucaMall()
{
    terminalCondition = " AND a.DEVICE_KEY = :DEVICE_KEY ";
    isMasterTerminal = TGlobalSettings::Instance().EnableDepositBagNum;
    deviceKey = TDeviceRealTerminal::Instance().ID.ProfileKey;
}
//--------------------------------------------------------------------------------------------
std::list<TMallExportSalesData> TDeanAndDelucaMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey)
{
    std::list<TMallExportSalesData> mallExportSalesData;
    try
    {
        //initilize each field of TDeanAndDelucaMallField 's class by iterating value from payment transaction
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return mallExportSalesData;
}
//-----------------------------------------------------------------------------------------------------------
void TDeanAndDelucaMall::InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData,
                                TDeanAndDelucaMallField fieldData, int arcBillKey)
{
    ///Call Push Field into List Method for pushing every field into DB
}
//-----------------------------------------------------------------------------------------------------------
long TDeanAndDelucaMall::GenerateSaleKey(Database::TDBTransaction &dbTransaction)
{
    //call Generator for generating sales key
}
//---------------------------------------------------------------------------------
int TDeanAndDelucaMall::GetPatronCount(TPaymentTransaction &paymentTransaction)
{
    //Get customer count
}
//---------------------------------------------------------------------------------
double TDeanAndDelucaMall::GetOldAccumulatedSales(Database::TDBTransaction &dbTransaction, int fieldIndex)
{
    //Get Old Accumulated Sales
}
void TDeanAndDelucaMall::PushFieldsInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, UnicodeString field, UnicodeString dataType, UnicodeString fieldValue, int fieldIndex, int arcBillKey)
{
    try
    {
        //Push field into MallExportSalesData
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//--------------------------------------------------------------------------------------------------------
TMallExportPrepareData TDeanAndDelucaMall::PrepareDataForExport(int zKey)
{
    //Create TMallExportPrepareData  for returning prepared data
    TMallExportPrepareData preparedData;

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        //todo call Methods which will Prepare date for daily, hourly, discount files

        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return preparedData;
}
//---------------------------------------------------------------------------------------------------------------------------------
void TDeanAndDelucaMall::PrepareDataForDiscountFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, int indexKey2,
                                                    TMallExportPrepareData &prepareDataForInvoice, int index, int zKey)
{
    //Create List Of SalesData for invoice file
    std::list<TMallExportSalesData> salesDataForISF;
    try
    {
        ///Prepare Data For Invoice Sales file
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------
void TDeanAndDelucaMall::PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, std::set<int> indexKeys2, int indexKey3,
                                                    TMallExportPrepareData &prepareDataForHSF, int index, int zKey)
{
    //Create List Of SalesData for hourly file
    std::list<TMallExportSalesData> prepareListForHSF;
    try
    {
        // Prepare Data For Hourly Sales File
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------------------
void TDeanAndDelucaMall::PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, std::set<int> indexKeys2,
                                                   TMallExportPrepareData &prepareDataForDSF, int index, int zKey)
{
    //Create List Of SalesData for hourly file
    std::list<TMallExportSalesData> prepareListForDSF;
    try
    {
       //Prepare Data For Daily Sales File

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------------------
void TDeanAndDelucaMall::LoadMallSettingsForFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareData, std::set<int> keysToSelect,
                                                int index, int zKey)
{
    try
    {
        //Load Mall Setting For File....
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------------------------------------------
UnicodeString TDeanAndDelucaMall::GetFileName(Database::TDBTransaction &dBTransaction, std::set<int> keysToSelect, int zKey)
{
    UnicodeString fileName = "";
    try
    {
        //Get File Name According to file type
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return fileName;
}
//--------------------------------------------------------------------------------------------------------------------------------------------
UnicodeString TDeanAndDelucaMall::GetExportType()
{
    UnicodeString typeOfFile = "";
    try
    {
        //Get Export Type
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return typeOfFile;
}
//-----------------------------------------------------------------------------------------------------------
std::set<int> TDeanAndDelucaMall::InsertInToSet(int arr[], int size)
{
    //function for inserting array into set.
    std::set<int> keyToCheck;
    for(int index = 0; index < size; index++)
            keyToCheck.insert(arr[index]);

    return keyToCheck;
}
//----------------------------------------------------------------------------------------------------------------
UnicodeString TDeanAndDelucaMall::GetFieldIndexList(std::set<int> indexKeys)
{
    //Seperate keys with commas and store in the form of string and return
}
//----------------------------------------------------------------------------------------------------------------
IExporterInterface* TDeanAndDelucaMall::CreateExportMedium()
{
    //return file export type
}
//----------------------------------------------------------------------------------------------------------------

