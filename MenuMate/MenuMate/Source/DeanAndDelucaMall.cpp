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
 void TDeanAndDelucaMallField::SetSCDDiscount(double totalSCDAmount)
{
   _scdAmount = totalSCDAmount;
}
//------------------------------------------------------------------------------------------
 void TDeanAndDelucaMallField::SetPWDDiscount(double totalPWDAmount)
{
   _pwdAmount = totalPWDAmount;
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
 void TDeanAndDelucaMallField::SetNonTaxableServiceCharge(double totalServiceCharge)
{
    _totalNonTaxableSC = _totalNonTaxableSC;
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
TMallExportSalesWrapper TDeanAndDelucaMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime)
{
    TMallExportSalesWrapper salesWrapper;
    std::list<TMallExportSalesData> mallExportSalesData;
    try
    {
        TDeanAndDelucaMallField *fieldData = new TDeanAndDelucaMallField();
        int terminalNumber;
        UnicodeString tenantCode;
        Currency taxRate = 0.00;
        double amount = 0;
        billedTime = currentTime;
        std::list<TMallExportSettings>::iterator it;

        for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
        {
            if(it->Value != "" )
            {
                if(it->ControlName == "edMallTenantNo")
                {
                    fieldData->TenantCode = it->Value;
                }
                else if(it->ControlName == "edMallTerminalNo" )
                {
                    fieldData->TerminalNumber = StrToInt(it->Value);
                }
            }
        }

        for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
        {
                TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

                //prepare all fields per items
                PrepareDataByItem(paymentTransaction.DBTransaction, Order, *fieldData);

                //For SubOrder
                for (int i = 0; i < Order->SubOrders->Count; i++)
				{
					TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
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
                amount = (double)(SubPayment->GetPayTendered() - SubPayment->GetChange() - paymentTransaction.Membership.Member.Points.getCurrentPointsPurchased());

                if(SubPayment->GetPaymentAttribute(ePayTypeElectronicTransaction))
                {
                     fieldData->TotalChargedSales += amount;
                }
                else if (SubPayment->GetPaymentAttribute(ePayTypeGetVoucherDetails))
                {
                    fieldData->TotalGCSales += amount;
                }
                else
                {
                    fieldData->TotalCashSales +=  amount;
                }
            }
        }

        fieldData->TotalNetSaleAmount =  fieldData->TotalGCSales + fieldData->TotalChargedSales + fieldData->TotalCashSales;
        fieldData->OldAccSalesTotal = 0;//GetOldAccumulatedSales(paymentTransaction.DBTransaction, 5);
        fieldData->NewAccSalesTotal = fieldData->OldAccSalesTotal + fieldData->TotalNetSaleAmount;

        fieldData->GrossSaleAmount =  fieldData->TotalSCDAndPWDAmount + fieldData->TotalOtherDiscount + fieldData->TotalCashSales +
                                        fieldData->TotalChargedSales + fieldData->TotalGCSales;
         
        fieldData->TotalRefundAmount = paymentTransaction.Money.FinalPrice > 0 ? 0 : fabs(paymentTransaction.Money.FinalPrice);

        fieldData->ZKey = 0;
        fieldData->SalesCount =  1;

        if(fieldData->TotalVoidAmount > 0 && fieldData->TotalNetSaleAmount == 0)
        {
            fieldData->CustomerCount = 0;
            fieldData->SalesCount = 0;
        }
        else
        {
            fieldData->CustomerCount = GetPatronCount(paymentTransaction);
        }

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
    int hourCode;
    if(HourOf(Now()))
    {
        hourCode= HourOf(Now());
    }
    else
    {
        hourCode = 24;
    }
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Tenant Code", "UnicodeString", fieldData.TenantCode, 1, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "POS Terminal Number", "int", fieldData.TerminalNumber, 2, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Date (mmddyyyy)", "TDateTime", Now().FormatString("mmddyyyy"), 3, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Old Accumulated Total", "double", fabs(fieldData.OldAccSalesTotal), 4, arcBillKey);
    PushFieldsInToList(dbTransaction, mallExportSalesData, "New Accumulated Total", "double", fieldData.NewAccSalesTotal, 5, arcBillKey);
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
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Hour Code", "int", IntToStr(hourCode), 21, arcBillKey);
}
//-----------------------------------------------------------------------------------------------------------
int TDeanAndDelucaMall::GetPatronCount(TPaymentTransaction &paymentTransaction)
{
    int totalPatronCount = 0;
    std::vector <TPatronType> ::iterator ptrPatronTypes;
    for (ptrPatronTypes = paymentTransaction.Patrons.begin(); ptrPatronTypes != paymentTransaction.Patrons.end(); ptrPatronTypes++)
    {
        totalPatronCount += ptrPatronTypes->Count;
    }
    return totalPatronCount != 0 ? totalPatronCount : 1;
}
//---------------------------------------------------------------------------------
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
        int dailySalekeys[7] = {1, 2, 3,4, 5, 19, 21};
        int dailySalekeys2 = 19;

        //insert these indexes into set.
        keyToCheck = InsertInToSet(dailySalekeys, 7);

        //Prepare Data For Daily Sales File
        PrepareDataForDailySalesFile(dbTransaction, keyToCheck, dailySalekeys2, preparedData, 1, zKey);

       //indexes for selecting total Net sale, patron count, etc
        int  hourIndexkeys[2] = {18,20};
        int hourIndexKeys2[3] = {13,20,18};

        //Clear the map because same map is used for many time insertion
        keyToCheck.clear();
        keyToCheck2.clear();

        //insert these indexes into set.
        keyToCheck = InsertInToSet(hourIndexkeys, 2);
        keyToCheck2 = InsertInToSet(hourIndexKeys2, 3);

        //Prepare Data For Hourly File
        PrepareDataForHourlySalesFile(dbTransaction, keyToCheck, keyToCheck2, 13, preparedData, 2, zKey);

        //Prepare Data For Discount File
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
        UnicodeString fileName = "D";

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

         //Get Max Z key;
        int maxZedKey;
        if(!zKey)
            maxZedKey = GetMaxZedKey(dBTransaction);
        else
            maxZedKey = zKey;

        //Query for selecting data for hourly file
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                "SELECT DISCOUNT_BREAKUP.DISCOUNT_ID, DISCOUNT_BREAKUP.NAME, "
                "CAST (SUM((DISCOUNT_BREAKUP.DISCOUNTED_VALUE*-1)) AS NUMERIC(17,2)) DISC_AMOUNT "
                "FROM "
                    "(SELECT A.ARCBILL_KEY, CASE WHEN AOD.NAME = 'Member Reward' THEN 'LP' "
                                                "WHEN AOD.NAME = 'Location Reward' THEN 'PC' "
                                                "ELSE  DISCOUNTS.DISCOUNT_ID END DISCOUNT_ID, "
                           " AOD.NAME, AOD.DISCOUNTED_VALUE, ARCHIVE.ARCHIVE_KEY "
                    "FROM MALLEXPORT_SALES a "
                    "INNER JOIN ARCHIVE ON ARCHIVE.ARCBILL_KEY = A.ARCBILL_KEY "
                    "INNER JOIN ARCORDERDISCOUNTS AOD ON ARCHIVE.ARCHIVE_KEY = AOD.ARCHIVE_KEY "
                    "LEFT JOIN DISCOUNTS ON DISCOUNTS.DISCOUNT_KEY = AOD.DISCOUNT_KEY "
                    "WHERE A.MALL_KEY = :MALL_KEY AND AOD.DISCOUNT_GROUPNAME <> 'Complimentary' AND AOD.DISCOUNT_GROUPNAME <> 'Non-Chargeable' "
                    "AND ARCHIVE.TIME_STAMP_BILLED >=  (SELECT MIN(A.DATE_CREATED) DATE_CREATED FROM MALLEXPORT_SALES a WHERE a.Z_KEY = :MAX_ZKEY ) "
                    "AND ARCHIVE.TIME_STAMP_BILLED <=  (SELECT MAX(A.DATE_CREATED) DATE_CREATED FROM MALLEXPORT_SALES a WHERE a.Z_KEY = :MAX_ZKEY ) "
                    "AND a.Z_KEY = :MAX_ZKEY "
                    "GROUP BY A.ARCBILL_KEY, AOD.NAME, DISCOUNTS.DISCOUNT_ID, AOD.DISCOUNTED_VALUE, ARCHIVE.ARCHIVE_KEY "

                "UNION "
                            "SELECT A.ARCBILL_KEY, CASE WHEN AOD.NAME = 'Member Reward' THEN 'LP' "
                                                "WHEN AOD.NAME = 'Location Reward' THEN 'PC' "
                                                "ELSE  DISCOUNTS.DISCOUNT_ID END DISCOUNT_ID, "
                            "AOD.NAME, AOD.DISCOUNTED_VALUE, DAYARCHIVE.ARCHIVE_KEY "
                    "FROM MALLEXPORT_SALES a "
                    "INNER JOIN DAYARCHIVE ON DAYARCHIVE.ARCBILL_KEY = A.ARCBILL_KEY "
                    "INNER JOIN DAYARCORDERDISCOUNTS AOD ON DAYARCHIVE.ARCHIVE_KEY = AOD.ARCHIVE_KEY "
                    "LEFT JOIN DISCOUNTS ON DISCOUNTS.DISCOUNT_KEY = AOD.DISCOUNT_KEY "
                    "WHERE A.MALL_KEY = 2 AND AOD.DISCOUNT_GROUPNAME <> 'Complimentary' AND AOD.DISCOUNT_GROUPNAME <> 'Non-Chargeable' "
                    "AND DAYARCHIVE.TIME_STAMP_BILLED >=  (SELECT MIN(A.DATE_CREATED) DATE_CREATED FROM MALLEXPORT_SALES a WHERE a.Z_KEY = :MAX_ZKEY ) "
                    "AND DAYARCHIVE.TIME_STAMP_BILLED <=  (SELECT MAX(A.DATE_CREATED) DATE_CREATED FROM MALLEXPORT_SALES a WHERE a.Z_KEY = :MAX_ZKEY ) "
                    "AND a.Z_KEY = :MAX_ZKEY "
                    "GROUP BY A.ARCBILL_KEY, AOD.NAME, DISCOUNTS.DISCOUNT_ID, AOD.DISCOUNTED_VALUE, DAYARCHIVE.ARCHIVE_KEY "
                        ") DISCOUNT_BREAKUP "
                "GROUP BY 1,2 ";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;
        IBInternalQuery->ParamByName("MAX_ZKEY")->AsInteger = maxZedKey;

        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          TMallExportSalesData salesData;
          salesData.FieldIndex  = 0;
          salesData.Field = IBInternalQuery->Fields[0]->AsString;

          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "," + IBInternalQuery->Fields[1]->AsString + "," + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = "UnicodeString";
          salesData.MallExportSalesId = 0;
          salesDataForDISF.push_back(salesData);
        }

        if(salesDataForDISF.size())
        {
            //insert filename into map according to index and file type
             prepareDataForDiscount.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

             //insert list into TMallExportPrepareData's map
             prepareDataForDiscount.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, salesDataForDISF ));
        }
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
        int hourIndexKeys[2] = {1, 2};

        //clear the map
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = InsertInToSet(hourIndexKeys, 2);

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
                                 "WHERE a.FIELD_INDEX IN(" + indexKeysList + ")  AND a.MALL_KEY = :MALL_KEY AND A.FIELD_VALUE <> 0  ";
        if(zKey)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) OR a.Z_KEY = :Z_KEY) ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +

        "UNION ALL "
                        "SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS int) FIELD_VALUE, a.VALUE_TYPE, "
                                                        "Extract (Hour From a.DATE_CREATED) Hour_code "
                                 "FROM MALLEXPORT_SALES a "
                                 "WHERE a.FIELD_INDEX = 21 AND a.MALL_KEY = :MALL_KEY  ";
        if(zKey)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) OR a.Z_KEY = :Z_KEY) ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY 1 ASC )HOURLYDATA  "
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
        if(zKey)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) OR a.Z_KEY = :Z_KEY) ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA "
                            "GROUP BY 1,2,4 ,5 "
                            "ORDER BY 5 ASC, 1 ASC ";

        IBInternalQuery->ParamByName("FIELD_INDEX")->AsInteger = indexKey3;
        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;
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
        if(zKey)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) OR a.Z_KEY = :Z_KEY) ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA "
                                "GROUP BY 1,2,4 "
                                "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;
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
void TDeanAndDelucaMall::PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, int zIndex,
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

        //Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        Database::TcpIBSQL SelectTime(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(SelectTime);

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
        int dailyIndexKeys[2] = {1, 2};

        //clear the map
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = InsertInToSet(dailyIndexKeys, 2);

        ///Load MallSetting For writing into file
        LoadMallSettingsForFile(dBTransaction, prepareDataForDSF, keysToSelect, index, zKey);

        //Get Max Z key;
        int maxZedKey;
        if(!zKey)
            maxZedKey = GetMaxZedKey(dBTransaction);
        else
            maxZedKey = zKey;

        //Get Second maximum key;
        int maxZedKey2 = GetMaxZedKey(dBTransaction, maxZedKey);

        //Query for fetching data for writing into daily sales file.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, "
                                             " CAST( case when (DAILYDATA.FIELD_INDEX = 18 OR DAILYDATA.FIELD_INDEX = 20)  then SUM(DAILYDATA.FIELD_VALUE) else SUM(DAILYDATA.FIELD_VALUE)*100  end AS BIGINT) FIELD_VALUE, "
                                             "DAILYDATA.VALUE_TYPE "
                                      "FROM "
                                            "(SELECT a.ARCBILL_KEY,a.MALLEXPORT_SALE_KEY, a.FIELD, LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, MAX(A.Z_KEY) Z_KEY "
                                             "FROM MALLEXPORT_SALES a "
                                             "WHERE a.FIELD_INDEX NOT IN(" + indexKeysList + ")  "
                                             "AND a.MALL_KEY = :MALL_KEY AND (a.Z_KEY = :MAX_ZKEY ";
        if(!zKey)
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " OR a.Z_KEY = :MIN_ZKEY ";

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                                              " ) GROUP BY a.ARCBILL_KEY,a.MALLEXPORT_SALE_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE  "
                                             "ORDER BY A.ARCBILL_KEY ASC )DAILYDATA "
                                    "GROUP BY 1,2,4 "

         "UNION ALL "

            "SELECT  LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, a.FIELD, CAST((a.FIELD_VALUE) AS BIGINT) FIELD_VALUE, "
                     "a.VALUE_TYPE "
             "FROM MALLEXPORT_SALES a "
             "WHERE a.FIELD_INDEX  = 19  AND a.MALL_KEY = :MALL_KEY "
                     "AND (a.Z_KEY = :MAX_ZKEY ";

        if(!zKey)
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " OR a.Z_KEY = :MIN_ZKEY ";

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
              ") GROUP BY a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE "

        "UNION ALL "

            "SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, "
                      "CAST( case when (DAILYDATA.FIELD_INDEX = 5)  then SUM(DAILYDATA.FIELD_VALUE)*100 end AS BIGINT) FIELD_VALUE, "
                     "DAILYDATA.VALUE_TYPE "
            "FROM "
                    "(SELECT a.ARCBILL_KEY, a.FIELD, LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, "
                                    "a.VALUE_TYPE, A.Z_KEY "
                     "FROM MALLEXPORT_SALES a "
                     "WHERE a.FIELD_INDEX  = 5  "
                     "AND a.MALL_KEY = :MALL_KEY AND a.MALLEXPORT_SALE_KEY = "
                            "(SELECT MAX(MALLEXPORT_SALE_KEY) FROM MALLEXPORT_SALES A  "
                                 "WHERE a.FIELD_INDEX  = 5 and (a.Z_KEY = :MAX_ZKEY ";

        if(!zKey)
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " OR a.Z_KEY = :MIN_ZKEY ";

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " ))  GROUP BY a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE, A.Z_KEY "
                                                                    "ORDER BY A.ARCBILL_KEY ASC )DAILYDATA  "
            "GROUP BY 1,2,4 "

        "UNION ALL ";

        if(maxZedKey2)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                            "SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, "
                                      "CAST( case when (DAILYDATA.FIELD_INDEX = 4)  then SUM(DAILYDATA.FIELD_VALUE)*100 end AS BIGINT) FIELD_VALUE, "
                                     "DAILYDATA.VALUE_TYPE "
                              "FROM "
                                    "(SELECT a.ARCBILL_KEY, CAST('Old Accumulated Total' as varchar(25)) FIELD, LPAD(4,2,0) FIELD_INDEX, "
                                            "CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, A.Z_KEY "
                                     "FROM MALLEXPORT_SALES a  "
                                     "WHERE a.FIELD_INDEX  = 5 "
                                     "AND a.MALL_KEY = :MALL_KEY AND a.MALLEXPORT_SALE_KEY = "
                                                        " (SELECT MAX(MALLEXPORT_SALE_KEY) FROM MALLEXPORT_SALES A  WHERE a.Z_KEY = :MAX_ZKEY2 and a.FIELD_INDEX = 5)  "
                                      "GROUP BY a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE, A.Z_KEY "
                                                                    "ORDER BY A.ARCBILL_KEY ASC )DAILYDATA "
                                                                    "GROUP BY 1,2,4";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                            "SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, DAILYDATA.FIELD_VALUE, "
                                    "DAILYDATA.VALUE_TYPE "
                              "FROM "
                                    "(SELECT a.FIELD, LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, CAST(0  AS BIGINT ) FIELD_VALUE, a.VALUE_TYPE, A.Z_KEY "
                                     "FROM MALLEXPORT_SALES a "
                                     "WHERE a.FIELD_INDEX  = 4 "
                                     "AND a.MALL_KEY = :MALL_KEY AND a.Z_KEY = :MAX_ZKEY "
                                      "GROUP BY a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE,  A.Z_KEY )DAILYDATA ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY 1 ASC  ";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;
        IBInternalQuery->ParamByName("MAX_ZKEY")->AsInteger = maxZedKey;
        if(maxZedKey2)
            IBInternalQuery->ParamByName("MAX_ZKEY2")->AsInteger = maxZedKey2;

        if(!zKey)
            IBInternalQuery->ParamByName("MIN_ZKEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
           ///prepare sales data
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;

          if(salesData.FieldIndex != 19)
          {
                salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          }
          else
          {
                salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "1";
          }
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;
          salesData.ZKey = 0;

          //insert prepared data into list
          prepareListForDSF.push_back(salesData);
       }

       TDateTime sDate,eDate;

       //SELECT MAX TIME FOR THAT ZED KEY.
       SelectTime->Close();
       SelectTime->SQL->Text = "SELECT MAX(A.DATE_CREATED) DATE_CREATED  "
                                "FROM MALLEXPORT_SALES a "
                                "WHERE a.Z_KEY = :MAX_ZKEY ";

        SelectTime->ParamByName("MAX_ZKEY")->AsInteger = maxZedKey;
        SelectTime->ExecQuery();

        if(SelectTime->RecordCount)
            eDate = SelectTime->Fields[0]->AsDateTime;

       //Select minimum time for that zed key.
       SelectTime->Close();
       SelectTime->SQL->Text = "SELECT MIN(A.DATE_CREATED) DATE_CREATED  "
                                "FROM MALLEXPORT_SALES a "
                                "WHERE a.Z_KEY = :MAX_ZKEY ";

        SelectTime->ParamByName("MAX_ZKEY")->AsInteger = maxZedKey;
        SelectTime->ExecQuery();

        if(SelectTime->RecordCount)
            sDate = SelectTime->Fields[0]->AsDateTime;

       // Now Fetch data Sales by Sales Type
       IBInternalQuery->Close();
       IBInternalQuery->SQL->Text = "SELECT  SALES_TYPE_REL.SALES_TYPE_CODE, SUM(SALES_TYPE_REL.FIELD_VALUE)FIELD_VALUE  "
                                    "FROM "
                                        "(SELECT a.ARCBILL_KEY, CAST((MST.SUBTOTAL)*100 AS BIGINT) FIELD_VALUE, "
                                                    "A.Z_KEY, MS.SALES_TYPE_CODE, MST.SALES_ID "
                                         "FROM MALLEXPORT_SALES a "
                                         "INNER JOIN MALL_SALES_BY_SALES_TYPE MST ON A.ARCBILL_KEY = MST.ARCBILL_KEY "
                                         "LEFT JOIN MALL_SALES_TYPE_ITEMS_RELATION MSIT ON MST.SALES_TYPE_ID = MSIT.STI_ID "
                                         "LEFT JOIN MALL_SALES_TYPE MS ON MSIT.SALES_TYPE_ID = MS.SALES_TYPE_ID "
                                         "WHERE   a.MALL_KEY = :MALL_KEY AND (a.Z_KEY = :MAX_ZKEY ";

        if(!zKey)
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " OR a.Z_KEY = :MIN_ZKEY ";

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                                         ") and MST.DATE_CREATED >= :START_TIME and MST.DATE_CREATED <= :END_TIME "
                                         "GROUP BY a.ARCBILL_KEY,  FIELD_VALUE, A.Z_KEY , MS.SALES_TYPE_CODE, MST.SALES_ID "
                                         "ORDER BY A.ARCBILL_KEY ) SALES_TYPE_REL "
                                    "GROUP BY SALES_TYPE_REL.SALES_TYPE_CODE ";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;
        IBInternalQuery->ParamByName("START_TIME")->AsDateTime = sDate;
        IBInternalQuery->ParamByName("END_TIME")->AsDateTime = eDate;
        IBInternalQuery->ParamByName("MAX_ZKEY")->AsInteger = maxZedKey;
        if(!zKey)
            IBInternalQuery->ParamByName("MIN_ZKEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
           ///prepare sales data
          TMallExportSalesData salesData;
          salesData.FieldIndex  = 21;
          salesData.Field = "Sales Type ";
          salesData.DataValue = "21" + IBInternalQuery->Fields[0]->AsString;
          salesData.DataValueType = "String";
          salesData.ZKey = 0;
          prepareListForDSF.push_back(salesData);

          salesData.FieldIndex  = 22;
          salesData.Field = "Net Sale Amount(Per Sales Type) ";
          salesData.DataValue = "22" + IBInternalQuery->Fields[1]->AsCurrency;
          salesData.DataValueType = "double";
          salesData.ZKey = 0;
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
                                                "MAX(a.FIELD_VALUE) FIELD_VALUE, a.VALUE_TYPE "
                                      "FROM MALLEXPORT_SALES a "
                                      "WHERE a.FIELD_INDEX IN(" + indexKeysList + ") "
                                      "AND a.MALL_KEY = :MALL_KEY ";
        if(zKey)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }

		IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY 1,2,4 "

        "UNION ALL "
        "SELECT LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, a.FIELD, "
                                                "MIN(a.FIELD_VALUE) FIELD_VALUE, a.VALUE_TYPE "
                                      "FROM MALLEXPORT_SALES a "
                                      "WHERE a.FIELD_INDEX = 3 "
                                      "AND a.MALL_KEY = :MALL_KEY ";
        if(zKey)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }

		IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY 1,2,4 ";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;

        if(zKey)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

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
        IBInternalQuery->SQL->Text = "SELECT a.FIELD_INDEX, a.FIELD, a.FIELD_VALUE, a.VALUE_TYPE , a.Z_KEY, MIN(a.DATE_CREATED)   "
                                    "FROM MALLEXPORT_SALES a "
                                    "WHERE a.FIELD_INDEX IN(" + indexKeysList + " ) AND a.MALL_KEY = :MALL_KEY  ";
        if(zKey)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY 1,2,3,4,5 "
                                                                "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;

        if(zKey)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            if(IBInternalQuery->Fields[0]->AsInteger == 19)
            {
                fileName = fileName + "1" ;
                TDateTime date = IBInternalQuery->Fields[5]->AsDateTime;
                UnicodeString month = GetMonthCode(MonthOf(date));
                int day = DayOf(date);
                fileName = fileName + "." + month + IntToStr(day);
            }
            else if(IBInternalQuery->Fields[0]->AsInteger == 1)
            {
                fileName = fileName + "" + (IBInternalQuery->Fields[2]->AsString).SubString(0,4);
            }
            else
            {
                fileName = fileName + "" + IBInternalQuery->Fields[2]->AsString;
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

    double grossAmount = 0;
    double salesBySalesType = 0;
    if(order->GetQty() > 0 )
        grossAmount = (double)(order->PriceEach_BillCalc()*order->GetQty()) ;

    salesBySalesType =  (double)(order->PriceEach_BillCalc()*order->GetQty());
    discounts.scdDiscount = order->GetQty() > 0.00 ? fabs(discounts.scdDiscount) : discounts.scdDiscount*-1;
    discounts.pwdDiscount = order->GetQty() > 0.00 ? fabs(discounts.pwdDiscount) : discounts.pwdDiscount*-1;
    fieldData.TotalSCDAndPWDAmount += discounts.scdDiscount + discounts.pwdDiscount;
    fieldData.TotalSCDAmount += discounts.scdDiscount;
    fieldData.TotalPWDAmount += discounts.pwdDiscount;
    fieldData.TotalOtherDiscount += order->GetQty() > 0.00 ? fabs(discounts.otherDiscount) : discounts.otherDiscount*-1;
    fieldData.TotalTax += taxes.salesTax + taxes.localTax + taxes.serviceChargeTax;
    fieldData.TotalServiceCharge += taxes.serviceCharge;

    //For Cancel Items..
    TItemComplete *cancelOrder =   (TItemComplete*)order;
    fieldData.TotalVoidAmount += (double)(cancelOrder->TabContainerName != "" && order->BillCalcResult.BasePrice == 0.00 ?
                                            (order->GetQty()*order->PriceLevel0) : 0);

    if(isVatable)
    {
        fieldData.GrossSaleAmount += discounts.scdDiscount + discounts.pwdDiscount + fieldData.TotalOtherDiscount + grossAmount;
    }
    else
    {
       fieldData.NonTaxableSaleAmount += (salesBySalesType + discounts.scdDiscount + discounts.pwdDiscount - taxes.serviceCharge);
       fieldData.TotalNonTaxableSC += taxes.serviceCharge;
    }

    //Get Salestype Code. if item is assigned to any sales type then it will return code else "";
    int salesTypeId = GetItemSalesId(dbTransaction, order->ItemKey);

    //insert sales by salesType into map id already exist then update amount..
    if(salesTypeId)
    {
        std::map <int, double> ::iterator isSumBySalesType = fieldData.SalesBySalesType.find(salesTypeId);

        if(isSumBySalesType != fieldData.SalesBySalesType.end())
        {
            isSumBySalesType->second += salesBySalesType;
        }
        else
        {
            fieldData.SalesBySalesType.insert(std::pair<int, double >(salesTypeId, salesBySalesType));
        }
    }

}
//------------------------------------------------------------------------------------------------------------------
TDeanAndDelucaDiscount TDeanAndDelucaMall::PrepareDiscounts(Database::TDBTransaction &dbTransaction, TItemMinorComplete *Order)
{
    TDeanAndDelucaDiscount discounts;
    discounts.scdDiscount = 0;
    discounts.pwdDiscount = 0;
    discounts.otherDiscount = 0;

    for (std::vector <TDiscount> ::const_iterator ptrDiscounts = Order->Discounts.begin(); ptrDiscounts != Order->Discounts.end();std::advance(ptrDiscounts, 1))
    {
        if(Order->DiscountValue_BillCalc(ptrDiscounts) == 0)
            continue;

        if(ptrDiscounts->DiscountGroupList.size())
        {
            if(ptrDiscounts->DiscountGroupList[0].Name == "Senior Citizen" )
            {
                discounts.scdDiscount += (double)Order->DiscountValue_BillCalc(ptrDiscounts);
            }
            else if(ptrDiscounts->DiscountGroupList[0].Name == "Person with Disability")
            {
                discounts.pwdDiscount += (double)Order->DiscountValue_BillCalc(ptrDiscounts);
            }
            else if(ptrDiscounts->DiscountGroupList[0].Name != "Complimentary" || ptrDiscounts->DiscountGroupList[0].Name != "Non-Chargeable")
            {
                discounts.otherDiscount +=  (double)Order->DiscountValue_BillCalc(ptrDiscounts);
            }
        }
        else
        {
            discounts.otherDiscount +=  (double)Order->DiscountValue_BillCalc(ptrDiscounts);
        }
    }
    return discounts;
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
                 isVatable = true;
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
    if(taxes.salesTax == 0.00 && taxes.localTax == 0.00)
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
//-----------------------------------------------------------------------------------------------------------
int TDeanAndDelucaMall::GetMaxZedKey(Database::TDBTransaction &dbTransaction, int zKey)
{
    Database::TcpIBSQL selectQuery(new TIBSQL(NULL));
    dbTransaction.RegisterQuery(selectQuery);
    int maxZedKey = 0;

    try
    {
        selectQuery->Close();
        selectQuery->SQL->Text = "SELECT MAX(a.Z_KEY) Z_KEY FROM MALLEXPORT_SALES a "
                                 "WHERE a.MALL_KEY = :MALL_KEY ";

        if(zKey)
            selectQuery->SQL->Text = selectQuery->SQL->Text + "AND a.Z_KEY < :Z_KEY ";

        selectQuery->ParamByName("MALL_KEY")->AsInteger = 2;

        if(zKey)
            selectQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        selectQuery->ExecQuery();

        if(selectQuery->RecordCount)
                maxZedKey = selectQuery->Fields[0]->AsInteger;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
	}

    return maxZedKey;
}


