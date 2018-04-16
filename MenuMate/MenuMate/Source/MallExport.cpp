//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExport.h"
#include "MMLogging.h"
#include "MallExportTextFile.h"
#include "MallExportEviaSalFile.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

bool TMallExport::PushToDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime)
{
    bool retVal = false;
    try
    {
        BilledTimeStamp = currentTime;

        // Create TMallExportPrepareData object to store Preapared Data
        TMallExportSalesWrapper salesData;

        ///Prepare Data For Inserting Data Into DataBase
        salesData = PrepareDataForDatabase(paymentTransaction, arcBillKey, currentTime);

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
        //    TMallExportTextFile* exporter = (TMallExportTextFile*)CreateExportMedium();
            if(TGlobalSettings::Instance().mallInfo.MallId == 3)
            {
                TMallExportSalFile* exporter = (TMallExportSalFile*)CreateExportMedium();
                 exporter->WriteToFile(preparedData);
            }
            else
            {
               TMallExportTextFile* exporter = (TMallExportTextFile*)CreateExportMedium();
                exporter->WriteToFile(preparedData);
            }

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
bool TMallExport::InsertInToMallExport_Sales(Database::TDBTransaction &dbTransaction , TMallExportSalesWrapper mallExportSalesData)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dbTransaction.RegisterQuery(IBInternalQuery);

    Database::TcpIBSQL IBInternalQuery1(new TIBSQL(NULL));
	dbTransaction.RegisterQuery(IBInternalQuery1);

    bool isInserted = false;
    try
    {
        std::list<TMallExportSalesData>::iterator it;
        int arcBillKey;
        TDateTime billedTime;
        Currency oldAccumulatedSales;
        //Iterate mallExport Sales data for inserting into DB
        for(it = mallExportSalesData.SalesData.begin(); it != mallExportSalesData.SalesData.end(); it++)
        {
            oldAccumulatedSales = 0;

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

//            if((it->FieldIndex == 4 || it->FieldIndex == 5 ) && (it->MallKey == 2))
//            {
//                oldAccumulatedSales = GetOldAccumulatedSales(5);
//                oldAccumulatedSales += StrToCurr(it->DataValue);
//                IBInternalQuery->ParamByName("FIELD_VALUE")->AsString = CurrToStr(oldAccumulatedSales);
//            }
//            else
//            {
                IBInternalQuery->ParamByName("FIELD_VALUE")->AsString = it->DataValue;
      //      }

            IBInternalQuery->ParamByName("VALUE_TYPE")->AsString = it->DataValueType;
            IBInternalQuery->ParamByName("DATE_CREATED")->AsDateTime = it->DateCreated;
            IBInternalQuery->ParamByName("CREATED_BY")->AsString = it->CreatedBy;
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = it->ZKey;
            IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = it->ArcBillKey;
            IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = it->DeviceKey;
            arcBillKey = it->ArcBillKey;
            billedTime = it->DateCreated;
            IBInternalQuery->ExecQuery();
        }

        //Insert sales total amount according to sales type.
        if(mallExportSalesData.SaleBySalsType.size())
        {
            InsertInToMallSalesBySalesType(dbTransaction, mallExportSalesData.SaleBySalsType, arcBillKey, billedTime);
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

        if(!isMasterTerminal && TGlobalSettings::Instance().mallInfo.MallId !=2)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " AND a.DEVICE_KEY = :DEVICE_KEY ";
        }
        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                                                                "GROUP BY a.Z_KEY "
                                                                "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = 0;
        IBInternalQuery->ParamByName("START_TIME")->AsDateTime = sDate;
        IBInternalQuery->ParamByName("END_TIME")->AsDateTime = eDate;
        if(!isMasterTerminal && TGlobalSettings::Instance().mallInfo.MallId != 2)
        {
            IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
        }
        IBInternalQuery->ExecQuery();

        //Create Export Medium
         if(TGlobalSettings::Instance().mallInfo.MallId == 3)
         {
              TMallExportSalFile* exporter =  new TMallExportSalFile();
              for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
              {
                //Fetch z-key
                zKey = IBInternalQuery->Fields[0]->AsInteger;
                //Prepare Data For Exporting into File
                preparedData = PrepareDataForExport(zKey);
                exporter->WriteToFile(preparedData);
             }
               delete exporter;

         }
         else
         {
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
         }

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
//---------------------------------------------------------------------------------------------------------------------
void TMallExport::InsertInToMallSalesBySalesType(Database::TDBTransaction &dbTransaction , std::map<int, double> salesBySalesType, int arcBillKey,
                                                    TDateTime billedTime)
{
    TIBSQL *incrementGenerator = dbTransaction.Query(dbTransaction.AddQuery());
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

    try
    {
        std::map<int, double>::iterator itSalesBySalesTypes;
        for (itSalesBySalesTypes = salesBySalesType.begin(); itSalesBySalesTypes != salesBySalesType.end(); ++itSalesBySalesTypes)
        {
            incrementGenerator->Close();
            incrementGenerator->SQL->Text = "SELECT GEN_ID(GEN_MALL_SALES_BY_TYPE, 1) FROM RDB$DATABASE";
            incrementGenerator->ExecQuery();

            ibInternalQuery->Close();
            ibInternalQuery->SQL->Text =
            "INSERT INTO MALL_SALES_BY_SALES_TYPE ( "
                    "SALES_ID, "
                    "ARCBILL_KEY, "
                    "SALES_TYPE_ID, "
                    "SUBTOTAL, "
                    "DEVICE_KEY, "
                    "DATE_CREATED "
                     ") "
            "VALUES ( "
                    ":SALES_ID, "
                    ":ARCBILL_KEY, "
                    ":SALES_TYPE_ID, "
                    ":SUBTOTAL, "
                    ":DEVICE_KEY, "
                    ":DATE_CREATED "
                     ") ";

            ibInternalQuery->ParamByName("SALES_ID")->AsInteger = incrementGenerator->Fields[0]->AsInteger;
            ibInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
            ibInternalQuery->ParamByName("SALES_TYPE_ID")->AsInteger = itSalesBySalesTypes->first;
            ibInternalQuery->ParamByName("SUBTOTAL")->AsDouble = itSalesBySalesTypes->second;
            ibInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
            ibInternalQuery->ParamByName("DATE_CREATED")->AsDateTime = billedTime;
            ibInternalQuery->ExecQuery();
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//----------------------------------------------------------------------------------------------
double TMallExport::GetOldAccumulatedSales(int fieldIndex)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dbTransaction.RegisterQuery(IBInternalQuery);
    double oldAccumulatedSales = 0.00;
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT Z_KEY FROM MALLEXPORT_SALES a WHERE a.MALL_KEY = :MALL_KEY GROUP BY 1";
        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;

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
                                        "WHERE  a.MALLEXPORT_SALE_KEY = "
                                            "(SELECT MAX(A.MALLEXPORT_SALE_KEY) FROM MALLEXPORT_SALES a WHERE A.FIELD_INDEX  = :FIELD_INDEX ) ";
            IBInternalQuery->ParamByName("FIELD_INDEX")->AsString = fieldIndex;
            IBInternalQuery->ExecQuery();

            if(IBInternalQuery->RecordCount)
                oldAccumulatedSales = IBInternalQuery->Fields[2]->AsCurrency;
        }
        dbTransaction.Commit();
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return oldAccumulatedSales;
}
//----------------------------------------------------------------------------------------------------------------
void TMallExport::PushFieldsInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, UnicodeString field,
                    UnicodeString dataType, UnicodeString fieldValue, int fieldIndex, int arcBillKey)
{
    try
    {
        TMallExportSalesData salesData;
        salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
        salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
        salesData.DataValue = fieldValue;
        salesData.Field = field;
        salesData.DataValueType = dataType;
        salesData.FieldIndex = fieldIndex;
        salesData.DateCreated = BilledTimeStamp;
        salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
        salesData.ArcBillKey = arcBillKey;
        salesData.ZKey = 0;
        salesData.DeviceKey = TDeviceRealTerminal::Instance().ID.ProfileKey;
        mallExportSalesData.push_back(salesData);
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-------------------------------------------------------------------------------------------------------------------
long TMallExport::GenerateSaleKey(Database::TDBTransaction &dbTransaction)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dbTransaction.RegisterQuery(IBInternalQuery);
    long saleKey;
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_MALLEXPORT_SALE_KEY, 1) FROM RDB$DATABASE";
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            saleKey = IBInternalQuery->Fields[0]->AsInteger;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return saleKey;
}

