//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeanAndDelucaMall.h"
#include "MallExportData.h"
#include "GlobalSettings.h"
#include "IBillCalculator.h"
#include "Comms.h"
#include "DeviceRealTerminal.h"
#include <Math.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)


void TDeanAndDelucaMallField::SetTerminalNumber(int terminalNumber)
{
    _terminalNumber = terminalNumber;
}
//----------------------------------------------------------------------------------------
void TDeanAndDelucaMallField::SetTenantCode(UnicodeString tenantCode)
{
    _tenantCode = tenantCode;
}
//----------------------------------------------------------------------------------------
void TDeanAndDelucaMallField::SetOldAccSalesTotal(double oldAccSalesTotal)
{
    _oldAccSalesTotal = oldAccSalesTotal;
}
//----------------------------------------------------------------------------------------
void TDeanAndDelucaMallField::SetNewAccSalesTotal(double newAccSalesTotal)
{
    _newAccSalesTotal = newAccSalesTotal;
}
//----------------------------------------------------------------------------------------
void TDeanAndDelucaMallField::SetGrossSaleAmount(double grossSaleAmount)
{
    _grossSaleAmount = grossSaleAmount;
}
//----------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetNoNTaxableSaleAmount(double nonTaxableSaleAmount)
{
    _nonTaxableSaleAmount = nonTaxableSaleAmount;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetSCDAndPWDDiscount(double totalSCDAndPWDAmount)
{
   _totalSCDAndPWDAmount = totalSCDAndPWDAmount;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetOtherDiscount(double totalOtherDiscount)
{
    _totalOtherDiscount = totalOtherDiscount;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetRefundAmount(double totalRefundAmount)
{
    _totalRefundAmount = totalRefundAmount;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetTax(double totalTax)
{
    _totalTax = totalTax;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetServiceCharge(double totalServiceCharge)
{
    _totalServiceCharge = totalServiceCharge;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetNetSaleAmount(double totalNetSaleAmount)
{
    _totalNetSaleAmount = totalNetSaleAmount;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetCashSales(double totalCashSales)
{
    _totalCashSales = totalCashSales;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetTotalChargedSales(double totalChargedSales)
{
    _totalChargedSales = totalChargedSales;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetTotalGCSales(double totalGCSales)
{
    _totalGCSales = totalGCSales;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetVoidAmount(double totalVoidAmount)
{
    _totalVoidAmount = totalVoidAmount;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetCustomerCount(int customerCount)
{
    _customerCount = customerCount;
}
//------------------------------------------------------------------------------------------
void TDeanAndDelucaMallField::SetSalesCount(int salesCount)
{
    _salesCount = salesCount;
}
//------------------------------------------------------------------------------------------
void TDeanAndDelucaMallField::SetHourCode(int hourCode)
{
    _hourCode = hourCode;
}
//------------------------------------------------------------------------------------------
void TDeanAndDelucaMallField::SetZKey(int zKey)
{
    _zkey = zKey;
}
//------------------------------------------------------------------------------------------
void TDeanAndDelucaMallField::SetSalesBySalesType(std::map<int, double> salesBySalestype)
{
    _salesBysalesType = salesBySalestype;
}
//---------------------------------------------------------------------------------------------
TDeanAndDelucaMallField::TDeanAndDelucaMallField()
{
   TerminalNumber = 0;
   TenantCode = "";
   OldAccSalesTotal = 0.00;
   NewAccSalesTotal = 0.00;
   GrossSaleAmount = 0.00;
   NonTaxableSaleAmount = 0.00;
   TotalSCDAndPWDAmount = 0.00;
   TotalOtherDiscount = 0.00;
   TotalRefundAmount = 0.00;
   TotalTax = 0.00;
   TotalServiceCharge = 0.00;
   TotalNetSaleAmount = 0.00;
   TotalCashSales = 0.00;
   TotalChargedSales = 0.00;
   TotalGCSales = 0.00;
   TotalVoidAmount = 0.00;
   CustomerCount = 0;
   SalesCount = 0;
}

//-----------------------------------------------------------------------------------------------------------------
TDeanAndDelucaMall::TDeanAndDelucaMall()
{
    terminalCondition = " AND a.DEVICE_KEY = :DEVICE_KEY ";
    isMasterTerminal = TGlobalSettings::Instance().EnableDepositBagNum;
    deviceKey = TDeviceRealTerminal::Instance().ID.ProfileKey;
}
//--------------------------------------------------------------------------------------------
TMallExportSalesWrapper TDeanAndDelucaMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey)
{
    TMallExportSalesWrapper salesWrapper;
    std::list<TMallExportSalesData> mallExportSalesData;
    try
    {
        TDeanAndDelucaMallField *fieldData = new TDeanAndDelucaMallField();
        int terminalNumber;
        UnicodeString tenantCode;
        Currency taxRate = 0.00;
        std::list<TMallExportSettings>::iterator it;

        for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
        {
            if(it->ControlName == "edMallTenantNo")
            {
                fieldData->TenantCode = it->Value;
            }
            else if(it->ControlName == "edMallTerminalNo")
            {
                fieldData->TerminalNumber = StrToInt(it->Value);
            }
        }

        for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
        {
                TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

                //todo
                PrepareDataByItem(paymentTransaction.DBTransaction, Order, *fieldData);

                //For SubOrder
                for (int i = 0; i < Order->SubOrders->Count; i++)
				{
					TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];

                    //todo
                    PrepareDataByItem(paymentTransaction.DBTransaction, CurrentSubOrder, *fieldData);
                }
        }

        fieldData->TotalGCSales = 0;
        fieldData->TotalCashSales = 0;
        fieldData->TotalChargedSales = 0;

        for (int i = 0; i < paymentTransaction.PaymentsCount(); i++)
		{
			TPayment *SubPayment = paymentTransaction.PaymentGet(i);
			if (SubPayment->GetPay() != 0)
			{
                 AnsiString payTypeName = "";
                AnsiString cardType = SubPayment->CardType;
                if( cardType != "" && cardType != NULL)
                {
                  payTypeName =  cardType;
                }
                else
                {
                    if (SubPayment->SysNameOveride != "")
                    {
                        payTypeName = SubPayment->SysNameOveride;
                    }
                    else
                    {
                        if (SubPayment->NameOveride == "")
                        {
                            payTypeName = SubPayment->Name;
                        }
                        else
                        {
                            payTypeName = SubPayment->NameOveride;
                        }
                    }
                }

                if (SubPayment->Properties & ePayTypeGetVoucherDetails)
                {
                    fieldData->TotalGCSales += (double)SubPayment->GetPayTendered();
                }
                else if(payTypeName == cardType)
                {
                     fieldData->TotalChargedSales += (double)SubPayment->GetPayTendered();
                }
                else
                {
                    fieldData->TotalCashSales +=  (double)SubPayment->GetPayTendered();
                }
            }
        }

        fieldData->TotalNetSaleAmount =  fieldData->TotalGCSales + fieldData->TotalChargedSales + fieldData->TotalCashSales;
        fieldData->OldAccSalesTotal = GetOldAccumulatedSales(paymentTransaction.DBTransaction, 5);
        fieldData->NewAccSalesTotal = fieldData->OldAccSalesTotal + fieldData->TotalNetSaleAmount;
        fieldData->GrossSaleAmount = fieldData->TotalSCDAndPWDAmount + fieldData->TotalOtherDiscount + fieldData->TotalCashSales + fieldData->TotalChargedSales
                                        + fieldData->TotalGCSales;
        //fieldData->NonTaxableSaleAmount todo
        fieldData->TotalRefundAmount = paymentTransaction.Money.FinalPrice > 0 ? 0 : paymentTransaction.Money.FinalPrice;
       /// fieldData->TotalTax = //todo;
        fieldData->ZKey = 0;
        fieldData->SalesCount = (fieldData->TotalRefundAmount > 0 ? 0 : 1);


        //fieldData->SalesType = 1; //todo



        fieldData->CustomerCount = GetPatronCount(paymentTransaction);

        //call For inserting into list
        InsertFieldInToList(paymentTransaction.DBTransaction, mallExportSalesData, *fieldData, arcBillKey);

        //Assign sales list to wrapper class
        salesWrapper.SalesData = mallExportSalesData;
        salesWrapper.SaleBySalsType = fieldData->SalesBySalesType;
		delete fieldData;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return salesWrapper;
}
//-----------------------------------------------------------------------------------------------------------
void TDeanAndDelucaMall::InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData,
                                TDeanAndDelucaMallField &fieldData, int arcBillKey)
{
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Tenant Code", "UnicodeString", fieldData.TenantCode, 1, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "POS Terminal Number", "int", fieldData.TerminalNumber, 2, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Date (mmddyyyy)", "TDateTime", Now().FormatString("mmddyyyy"), 3, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Old Accumulated Total", "double", fabs(fieldData.OldAccSalesTotal), 4, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "New Accumulated Total", "double", fabs(fieldData.NewAccSalesTotal), 5, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Gross Sales Amount", "double", fieldData.GrossSaleAmount, 6, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Non-Taxable Sales Amount", "double", fieldData.NonTaxableSaleAmount, 7, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Senior Citizen Discount", "double", fieldData.TotalSCDAndPWDAmount, 8, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Other Discount", "double", fieldData.TotalOtherDiscount, 9, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Refund Amount", "double", fieldData.TotalRefundAmount, 10, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total TAX/VAT Amount", "double", fieldData.TotalTax, 11, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total ServiceCharge Amount", "double", fieldData.TotalServiceCharge, 12, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Net Sales Amount", "double", fieldData.TotalNetSaleAmount, 13, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Cash Sales", "double", fieldData.TotalCashSales, 14, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Charge Sales", "double", fieldData.TotalChargedSales, 15, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total GC Sales", "double", fieldData.TotalGCSales, 16, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Void Amount", "double", fieldData.TotalVoidAmount, 17, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Customer Count", "int", fieldData.CustomerCount, 18, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Control Number", "int", fieldData.ZKey, 19, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Number Of Sales Transaction", "int", fieldData.SalesCount, 20, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Hour Code", "int", HourOf(Now()), 21, arcBillKey);
}
//-----------------------------------------------------------------------------------------------------------
void TDeanAndDelucaMall::PushFieldsInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, UnicodeString field, UnicodeString dataType, UnicodeString fieldValue, int fieldIndex, int arcBillKey)
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
        salesData.DateCreated = Now();
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
//--------------------------------------------------------------------------------------------------------
long TDeanAndDelucaMall::GenerateSaleKey(Database::TDBTransaction &dbTransaction)
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
//---------------------------------------------------------------------------------
int TDeanAndDelucaMall::GetPatronCount(TPaymentTransaction &paymentTransaction)
{
    int totalPatronCount = 0;
    std::vector <TPatronType> ::iterator ptrPatronTypes;
    for (ptrPatronTypes = paymentTransaction.Patrons.begin(); ptrPatronTypes != paymentTransaction.Patrons.end(); ptrPatronTypes++)
    {
        totalPatronCount += ptrPatronTypes->Count;
    }
    return totalPatronCount;
}
//---------------------------------------------------------------------------------
double TDeanAndDelucaMall::GetOldAccumulatedSales(Database::TDBTransaction &dbTransaction, int fieldIndex)
{
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
//----------------------------------------------------------------------------------------------------------------
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
       //Set for inserting index. these indexes will be used for fetching data
        std::set<int> keyToCheck;
        std::set<int> keyToCheck2;

        //Indexes for which data will not selected
        int dailySalekeys[12] = {1, 2, 3,4, 5, 19, 21};
        int dailySalekeys2[2] = {19,0};   ///todo

        //insert these indexes into set.
        keyToCheck = InsertInToSet(dailySalekeys, 12);
        keyToCheck2 = InsertInToSet(dailySalekeys2, 2);

        //Prepare Data For Daily Sales File
        PrepareDataForDailySalesFile(dbTransaction, keyToCheck, keyToCheck2, preparedData, 1, zKey);

       //indexes for selecting total Net sale, patron count, etc
        int  hourIndexkeys[3] = {18,20,21};
        int hourIndexKeys2[3] = {13,20,18};

        //Clear the map because same map is used for many time insertion
        keyToCheck.clear();
        keyToCheck2.clear();

        //insert these indexes into set.
        keyToCheck = InsertInToSet(hourIndexkeys, 3);
        keyToCheck2 = InsertInToSet(hourIndexKeys2, 3);

        //Prepare Data For Hourly File
        PrepareDataForHourlySalesFile(dbTransaction, keyToCheck, keyToCheck2, 13, preparedData, 2, zKey);

        //Prepare Data For DiscountFile
        PrepareDataForDiscountFile(dbTransaction, preparedData, 3, zKey);

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
void TDeanAndDelucaMall::PrepareDataForDiscountFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareDataForDiscount,
                                int index, int zKey)
{
    //Create List Of SalesData for invoice file
    std::list<TMallExportSalesData> salesDataForDISF;
    try
    {
        ///Store First Letter of file name
        UnicodeString fileName = "H";

        ///Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Declare Set For storing index
        std::set<int>keysToSelect;

        //Create array for storing index by which file name will be prepared
        int  fileNameKeys[3] = {1, 2, 19};

        //Store keys into set
        keysToSelect = InsertInToSet(fileNameKeys, 3);

        //Get file name according to field index.
        fileName = fileName + "" + GetFileName(dBTransaction, keysToSelect, zKey);

        //insert filename into map according to index and file type
        prepareDataForDiscount.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

         //insert indexes into array for fetching tenant code, date , terminal number
        int hourIndexKeys[3] = {1, 2, 3};

        //clear the map
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = InsertInToSet(hourIndexKeys, 3);

        ///Load MallSetting For writing into file
        LoadMallSettingsForFile(dBTransaction, prepareDataForDiscount, keysToSelect, index, zKey);

        //Query for selecting data for hourly file
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                "SELECT DISCOUNT_BREAKUP.ARCBILL_KEY, DISCOUNT_BREAKUP.DISCOUNT_ID, DISCOUNT_BREAKUP.NAME, "
                "CAST (SUM(DISCOUNT_BREAKUP.DISCOUNTED_VALUE) AS NUMERIC(17,2)) DISC_AMOUNT "
                "FROM "
                    "(SELECT A.ARCBILL_KEY, DISCOUNTS.DISCOUNT_ID, AOD.NAME, AOD.DISCOUNTED_VALUE "
                    "FROM MALLEXPORT_SALES a "
                    "INNER JOIN ARCHIVE ON ARCHIVE.ARCBILL_KEY = A.ARCBILL_KEY "
                    "INNER JOIN ARCORDERDISCOUNTS AOD ON ARCHIVE.ARCHIVE_KEY = AOD.ARCHIVE_KEY "
                    "INNER JOIN DISCOUNTS ON DISCOUNTS.DISCOUNT_KEY = AOD.DISCOUNT_KEY "
                    "WHERE A.MALL_KEY = :MALL_KEY ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                        "GROUP BY A.ARCBILL_KEY, AOD.NAME, DISCOUNTS.DISCOUNT_ID, AOD.DISCOUNTED_VALUE ) DISCOUNT_BREAKUP "
                "GROUP BY 1,2,3 ";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;

          salesData.DataValue = IBInternalQuery->Fields[1]->AsString + "," + IBInternalQuery->Fields[2]->AsString + "," + IBInternalQuery->Fields[3]->AsCurrency;
          salesData.DataValueType = "UnicodeString";
          salesData.MallExportSalesId = IBInternalQuery->Fields[4]->AsInteger;
          salesDataForDISF.push_back(salesData);
        }
         //insert list into TMallExportPrepareData's map
        prepareDataForDiscount.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, salesDataForDISF ));
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
        ///Store First Letter of file name
        UnicodeString fileName = "H";

        //Seperate key with commas in the form of string.
        UnicodeString indexKeysList = GetFieldIndexList(indexKeys);
        UnicodeString indexKeysList2 = GetFieldIndexList(indexKeys2);

        ///Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Declare Set For storing index
        std::set<int>keysToSelect;

        //Create array for storing index by which file name will be prepared
        int  fileNameKeys[3] = {1, 2, 19};

        //Store keys into set
        keysToSelect = InsertInToSet(fileNameKeys, 3);

        //Get file name according to field index.
        fileName = fileName + "" + GetFileName(dBTransaction, keysToSelect, zKey);

        //insert filename into map according to index and file type
        prepareDataForHSF.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

         //insert indexes into array for fetching tenant code, date , terminal number
        int hourIndexKeys[3] = {1, 2, 3};

        //clear the map
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = InsertInToSet(hourIndexKeys, 3);

        ///Load MallSetting For writing into file
        LoadMallSettingsForFile(dBTransaction, prepareDataForHSF, keysToSelect, index, zKey);

        //Query for selecting data for hourly file
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                             "SELECT LPAD((CASE WHEN (HOURLYDATA.FIELD_INDEX = 18) THEN 7  "
                                            "WHEN (HOURLYDATA.FIELD_INDEX = 20) THEN 6 "
                                            "WHEN (HOURLYDATA.FIELD_INDEX = 21) THEN 4 "
                                            "ELSE (HOURLYDATA.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, "
                                    "case when (HOURLYDATA.FIELD_INDEX = 21) then LPAD(HOURLYDATA.FIELD_VALUE,2,0) else SUM(HOURLYDATA.FIELD_VALUE) end FIELD_VALUE , "
                                    "HOURLYDATA.VALUE_TYPE , "
                                    "HOURLYDATA.Hour_code  "
                            "FROM "
                                "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS int) FIELD_VALUE, a.VALUE_TYPE, "
                                                        "Extract (Hour From a.DATE_CREATED) Hour_code "
                                 "FROM MALLEXPORT_SALES a "
                                 "WHERE a.FIELD_INDEX IN(" + indexKeysList + ")  AND a.MALL_KEY = :MALL_KEY ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA  "
                            "GROUP BY HOURLYDATA.FIELD_INDEX,HOURLYDATA.FIELD,HOURLYDATA.VALUE_TYPE , "
                                    " HOURLYDATA.Hour_code, HOURLYDATA.FIELD_VALUE "

       "UNION ALL "

                            "SELECT LPAD((CASE WHEN (HOURLYDATA.FIELD_INDEX = 13) THEN 5 ELSE (HOURLYDATA.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, CAST(SUM(HOURLYDATA.FIELD_VALUE)*100 AS INT ) FIELD_VALUE , HOURLYDATA.VALUE_TYPE, HOURLYDATA.Hour_code "
                            "FROM "
                                    "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE,  "
                                            "Extract (Hour From a.DATE_CREATED) Hour_code "
                                     "FROM MALLEXPORT_SALES a "
                                    " WHERE a.FIELD_INDEX = :FIELD_INDEX AND a.MALL_KEY = :MALL_KEY ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA "
                            "GROUP BY 1,2,4 ,5 "
                            "ORDER BY 5 ASC, 1 ASC ";

        IBInternalQuery->ParamByName("FIELD_INDEX")->AsInteger = indexKey3;
        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          if(salesData.FieldIndex == 4)
          {
                salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsString;
          }
          else
          {
                 salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          }
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;
          salesData.MallExportSalesId = IBInternalQuery->Fields[4]->AsInteger;
          prepareListForHSF.push_back(salesData);
        }

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                                "SELECT LPAD((CASE WHEN (HOURLYDATA.FIELD_INDEX = 20) THEN 9  "
                                                  "WHEN (HOURLYDATA.FIELD_INDEX = 13) THEN 8 "
                                                  "WHEN (HOURLYDATA.FIELD_INDEX = 18) THEN 10 "
                                                  "ELSE (HOURLYDATA.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                        "HOURLYDATA.FIELD, "
                                        "CAST(SUM(CASE WHEN HOURLYDATA.FIELD_INDEX = 13 THEN (HOURLYDATA.FIELD_VALUE*100) ELSE (HOURLYDATA.FIELD_VALUE) END )  AS INT ) FIELD_VALUE, "
                                         "HOURLYDATA.VALUE_TYPE "
                                "FROM "
                                "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE "
                                "FROM MALLEXPORT_SALES a  "
                                "WHERE a.FIELD_INDEX IN(" + indexKeysList2 + ")  AND a.MALL_KEY = :MALL_KEY  ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA "
                                "GROUP BY 1,2,4 "
                                "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           ///prepare sales data
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;

          //insert prepared data into list
          prepareListForHSF.push_back(salesData);
        }

        //insert list into TMallExportPrepareData's map
        prepareDataForHSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForHSF ));
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
        ///Store First Letter of file name ie; file type
        UnicodeString fileName = "S";

        //Seperate key with commas in the form of string.
        UnicodeString indexKeysList = GetFieldIndexList(indexKeys);
        UnicodeString indexKeysList2 = GetFieldIndexList(indexKeys2);

        //Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        Database::TcpIBSQL selectQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(selectQuery);

        //Declare Set For storing index
        std::set<int>keysToSelect;

        //Create array for storing index by which file name will be prepared
        int  fileNameKeys[3] = {1, 2, 19};

         //Store keys into set
        keysToSelect = InsertInToSet(fileNameKeys, 3);

        //Get file name according to field index.
        fileName = fileName + "" + GetFileName(dBTransaction, keysToSelect, zKey);

        //insert filename into map according to index and file type
        prepareDataForDSF.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

         //insert indexes into array for fetching tenant code, date , terminal number
        int dailyIndexKeys[3] = {1, 2, 3};

        //clear the map
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = InsertInToSet(dailyIndexKeys, 3);

        ///Load MallSetting For writing into file
        LoadMallSettingsForFile(dBTransaction, prepareDataForDSF, keysToSelect, index, zKey);

        selectQuery->Close();
        selectQuery->SQL->Text = "SELECT Z_KEY FROM MALLEXPORT_SALES a "
                                 "WHERE a.MALL_KEY = :MALL_KEY AND a.Z_KEY != :Z_KEY "
                                  "GROUP BY 1";

        selectQuery->ParamByName("MALL_KEY")->AsInteger = 2;
        selectQuery->ParamByName("Z_KEY")->AsInteger = 0;
        selectQuery->ExecQuery();

        bool  recordPresent = false;
        while(!selectQuery->Eof)
            selectQuery->Next();

        if(selectQuery->RecordCount > 1)
           recordPresent = true;

        //Query for fetching data for writing into daily sales file.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, "
                                             " CAST( case when (DAILYDATA.FIELD_INDEX = 18 OR DAILYDATA.FIELD_INDEX = 20)  then SUM(DAILYDATA.FIELD_VALUE) else SUM(DAILYDATA.FIELD_VALUE)*100  end AS INT) FIELD_VALUE, "
                                             "DAILYDATA.VALUE_TYPE, DAILYDATA.Z_KEY "
                                      "FROM "
                                            "(SELECT a.ARCBILL_KEY, a.FIELD, LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, MAX(A.Z_KEY) Z_KEY "
                                             "FROM MALLEXPORT_SALES a "
                                             "WHERE a.FIELD_INDEX NOT IN(" + indexKeysList + ")  "
                                             "AND a.MALL_KEY = :MALL_KEY ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE  "
                                             "ORDER BY A.ARCBILL_KEY ASC )DAILYDATA "
                                    "GROUP BY 1,2,4,5 "

        "UNION ALL "

            "SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, "
                      "CAST( case when (DAILYDATA.FIELD_INDEX = 5)  then SUM(DAILYDATA.FIELD_VALUE)*100 end AS INT) FIELD_VALUE, "
                     "DAILYDATA.VALUE_TYPE, DAILYDATA.Z_KEY "
            "FROM "
                    "(SELECT a.ARCBILL_KEY, a.FIELD, LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, "
                                    "a.VALUE_TYPE, A.Z_KEY "
                     "FROM MALLEXPORT_SALES a "
                     "WHERE a.FIELD_INDEX  = 5  "
                     "AND a.MALL_KEY = :MALL_KEY AND a.ARCBILL_KEY = "
                            "(SELECT MAX(ARCBILL_KEY)FROM MALLEXPORT_SALES A ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "WHERE a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) )";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "WHERE a.Z_KEY = :Z_KEY ) ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " GROUP BY a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE, A.Z_KEY "
                                                                    "ORDER BY A.ARCBILL_KEY ASC )DAILYDATA  "
                                                                    "GROUP BY 1,2,4,5 "
        "UNION ALL "

        "SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, "
                  "CAST( case when (DAILYDATA.FIELD_INDEX = 4)  then SUM(DAILYDATA.FIELD_VALUE)*100 end AS INT) FIELD_VALUE, "
                 "DAILYDATA.VALUE_TYPE, DAILYDATA.Z_KEY "
          "FROM "
                "(SELECT a.ARCBILL_KEY, a.FIELD, LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, "
                    "a.VALUE_TYPE, A.Z_KEY "
                 "FROM MALLEXPORT_SALES a  "
                 "WHERE a.FIELD_INDEX  = 4 "
                 "AND a.MALL_KEY = :MALL_KEY AND a.ARCBILL_KEY = (SELECT MAX(ARCBILL_KEY)FROM MALLEXPORT_SALES A ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "WHERE A.Z_KEY = (SELECT MAX(A.Z_KEY) FROM MALLEXPORT_SALES A ) - 1) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "WHERE a.Z_KEY = :Z_KEY - 1 ";
        }
        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE, A.Z_KEY "
                                                                    "ORDER BY A.ARCBILL_KEY ASC )DAILYDATA "
                                                                    "GROUP BY 1,2,4,5 "

        "ORDER BY 1 ASC  ";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
           ///prepare sales data
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;
          salesData.ZKey = IBInternalQuery->Fields[4]->AsInteger;

          //insert prepared data into list
          prepareListForDSF.push_back(salesData);
       }

       //insert list into TMallExportPrepareData's map
       prepareDataForDSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForDSF ));
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
        //Create List Of mallExportSetting
        std::list<TMallExportSettings> mallSettings;

        //Seperate key with commas in the form of string.
        UnicodeString indexKeysList = GetFieldIndexList(keysToSelect);

        //Register Query.
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Query for fetching setting for files according to file type and index keys.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, a.FIELD, "
                                                "CASE WHEN(a.FIELD_INDEX = 1) THEN LPAD(a.FIELD_VALUE,5,0) "
                                                "WHEN(a.FIELD_INDEX = 2) THEN LPAD(a.FIELD_VALUE,2,0) "
                                                "ELSE (a.FIELD_VALUE) END FIELD_VALUE, a.VALUE_TYPE "
                                      "FROM MALLEXPORT_SALES a "
                                      "INNER JOIN MALLEXPORT_HEADER MEH ON A.FIELD_INDEX = MEH.MALLEXPORT_HEADER_ID "
                                      "WHERE a.FIELD_INDEX IN(" + indexKeysList + ") AND meh.IS_ACTIVE = :IS_ACTIVE "
                                      "AND a.MALL_KEY = :MALL_KEY ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

		IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + terminalCondition + "GROUP BY 1,2,3,4 ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = deviceKey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          //Prepare Setting data
          TMallExportSettings settings;
          settings.Name =   IBInternalQuery->Fields[1]->AsString;
          settings.Value  = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsString;
          settings.ValueType = IBInternalQuery->Fields[3]->AsString;

          //Insert setting data into list.
          mallSettings.push_back(settings);
        }

        ////insert list into TMallExportPrepareData's setting map
        prepareData.MallSettings.insert( std::pair<int,list<TMallExportSettings> >(index, mallSettings));
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
        //create list of mallexportsetting
        std::list<TMallExportSettings> mallSettings;

        //seperate keys with comma in the form of string
        UnicodeString indexKeysList = GetFieldIndexList(keysToSelect);

        //Register Query.
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Query for fetching file name.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.FIELD_INDEX, a.FIELD, a.FIELD_VALUE, a.VALUE_TYPE , a.Z_KEY, a.DATE_CREATED  "
                                    "FROM MALLEXPORT_SALES a "
                                    "WHERE a.FIELD_INDEX IN(" + indexKeysList + " ) AND a.MALL_KEY = :MALL_KEY  ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY 1,2,3,4,5,6 "
                                                                "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            fileName = fileName + "" + IBInternalQuery->Fields[2]->AsString;

            if(IBInternalQuery->Fields[0]->AsInteger == 19)
            {
                TDateTime date = IBInternalQuery->Fields[5]->AsDateTime;
                UnicodeString month = GetMonthCode(MonthOf(date));
                int day = DayOf(date);
                fileName = fileName + "." + month + IntToStr(day);
            }
        }
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
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        ///Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(IBInternalQuery);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT MES.NAME, MES.MALLEXPORT_SETTING_KEY, MSP.MALL_ID, MSV.FIELD_VALUE  "
                                     "FROM MALLEXPORT_SETTINGS MES "
                                     "INNER JOIN MALLEXPORT_SETTINGS_MAPPING MSP ON MES.MALLEXPORT_SETTING_KEY = MSP.MALLEXPORT_SETTING_ID "
                                     "INNER JOIN MALLEXPORT_SETTINGS_VALUES MSV ON MES.MALLEXPORT_SETTING_KEY = MSV.MALLEXPORTSETTING_ID "
                                     "WHERE MES.NAME = :NAME AND MSP.MALL_ID = :MALL_ID";

        IBInternalQuery->ParamByName("NAME")->AsString = "TYPE_OF_FILE";
        IBInternalQuery->ParamByName("MALL_ID")->AsInteger = 2;
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
    std::set<int>::iterator indexKeysIt = indexKeys.begin();
    UnicodeString indexKeyList = IntToStr(*indexKeysIt);
    indexKeysIt++;
    for(; indexKeysIt != indexKeys.end(); indexKeysIt++)
    {
        indexKeyList += ", " + IntToStr(*indexKeysIt);
    }
    return indexKeyList;
}
//----------------------------------------------------------------------------------------------------------------
IExporterInterface* TDeanAndDelucaMall::CreateExportMedium()
{
    UnicodeString exportType = GetExportType();
    if(exportType == ".txt")
    {
        return new TMallExportTextFile;
    }
    else
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Export Type not found");
		throw;
    }
}
//----------------------------------------------------------------------------------------------------------------
void TDeanAndDelucaMall::PrepareDataByItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *order, TDeanAndDelucaMallField &fieldData)
{
    //Create Taxes Object to collect all taxes details
    TDeanAndDelucaTaxes taxes;
    taxes.salesTax = 0;
    taxes.serviceCharge = 0;
    taxes.serviceChargeTax = 0;
    taxes.localTax = 0;

    //Get all Taxes stored them in TEstanciaTaxes type structure
    bool isVatable = IsItemVatable(order, taxes);

    TDeanAndDelucaDiscount discounts = PrepareDiscounts(dbTransaction, order);

    double grossAmount = order->GetQty() > 0.00 ? ((order->PriceEach_BillCalc()*order->GetQty()) + fabs(order->TotalAdjustment())) : 0;

    fieldData.TotalSCDAndPWDAmount += discounts.scdDiscount + discounts.pwdDiscount;
    fieldData.TotalOtherDiscount += discounts.otherDiscount;
    fieldData.TotalTax += taxes.salesTax + taxes.localTax + taxes.serviceChargeTax;
    fieldData.TotalServiceCharge += taxes.serviceCharge;

    if(isVatable)
    {
        fieldData.GrossSaleAmount += discounts.scdDiscount + discounts.pwdDiscount + fieldData.TotalOtherDiscount + grossAmount;
    }
    else
    {
       fieldData.GrossSaleAmount += (grossAmount - taxes.serviceCharge);
    }

    //Get Salestype Code. if item is assigned to any sales type then it will return code else "";
    int salesTypeId = GetItemSalesId(dbTransaction, order->ItemKey);

    //insert sales by salesType into map id already exist then update amount..
    if(salesTypeId)
    {
        std::map <int, double> ::iterator isSumBySalesType = fieldData.SalesBySalesType.find(salesTypeId);

        if(isSumBySalesType != fieldData.SalesBySalesType.end())
        {
            isSumBySalesType->second += grossAmount;
        }
        else
        {
            fieldData.SalesBySalesType.insert(std::pair<int, double >(salesTypeId, grossAmount));
        }
    }

}
//------------------------------------------------------------------------------------------------------------------
TDeanAndDelucaDiscount TDeanAndDelucaMall::PrepareDiscounts(Database::TDBTransaction &dbTransaction, TItemMinorComplete *Order)
{
    TDeanAndDelucaDiscount discount;
    discount.scdDiscount = 0;
    discount.pwdDiscount = 0;
    discount.otherDiscount = 0;

    for (std::vector <TDiscount> ::const_iterator ptrDiscounts = Order->Discounts.begin(); ptrDiscounts != Order->Discounts.end();std::advance(ptrDiscounts, 1))
    {
        if(Order->DiscountValue_BillCalc(ptrDiscounts) == 0)
            continue;

        if(ptrDiscounts->DiscountGroupList.size())
        {
            if(ptrDiscounts->DiscountGroupList[0].Name == "Senior Citizen" )
            {
                discount.scdDiscount += (double)Order->DiscountValue_BillCalc(ptrDiscounts);
            }
            else if(ptrDiscounts->DiscountGroupList[0].Name == "Person with Disability")
            {
                discount.pwdDiscount += (double)Order->DiscountValue_BillCalc(ptrDiscounts);
            }
            else if(ptrDiscounts->DiscountGroupList[0].Name != "Complimentary" || ptrDiscounts->DiscountGroupList[0].Name != "Non-Chargeable")
            {
                discount.otherDiscount +=  (double)Order->DiscountValue_BillCalc(ptrDiscounts);
            }
        }
        else
        {
            discount.otherDiscount +=  (double)Order->DiscountValue_BillCalc(ptrDiscounts);
        }
    }
    return discount;
}
//--------------------------------------------------------------------------------------------------------------------------
bool TDeanAndDelucaMall::IsItemVatable(TItemMinorComplete *order, TDeanAndDelucaTaxes &taxes)
{
    std::vector<BillCalculator::TTaxResult> taxInfomation = order->BillCalcResult.Tax;
    bool isVatable = false;

    ///load all taxes value seperate
    for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
    {
        switch( itTaxes->TaxType )
        {
            case TTaxType::ttSale:
                isVatable = true;
                taxes.salesTax += (double)itTaxes->Value;
                break;
            case TTaxType::ttLocal:
                 taxes.localTax += (double)itTaxes->Value;
                 break;
        }
    }
    if (order->BillCalcResult.ServiceCharge.Percentage != 0)
    {
        taxes.serviceCharge += (double)order->BillCalcResult.ServiceCharge.Value;
        if (order->BillCalcResult.ServiceCharge.TaxPercentage != 0)
        {
            taxes.serviceChargeTax += (double)order->BillCalcResult.ServiceCharge.TaxValue;
        }
    }
    if(taxes.salesTax == 0.00)
        isVatable = false;

    return isVatable;
}
//-----------------------------------------------------------------------------------------------------------
int TDeanAndDelucaMall::GetItemSalesId(Database::TDBTransaction &dbTransaction, int itemKey)
{
    int salesTypeId = 0;
    try
    {
        Database::TcpIBSQL ibInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(ibInternalQuery);

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "SELECT a.STI_ID FROM MALL_SALES_TYPE_ITEMS_RELATION a "
                                     "WHERE A.ITEM_ID = :ITEM_ID ";
        ibInternalQuery->ParamByName("ITEM_ID")->AsInteger = itemKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
            salesTypeId = ibInternalQuery->Fields[0]->AsInteger;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return salesTypeId;
}
//--------------------------------------------------------------------------------------------------------
UnicodeString TDeanAndDelucaMall::GetMonthCode(int month)
{
    UnicodeString code = "";

    switch(month)
    {
        case 10:
            code = "A";
            break;
        case 11:
            code = "B";
            break;
        case 12:
            code = "C";
            break;
        default:
            code = IntToStr(month);
    }
    return code;
}


