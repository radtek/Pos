  //---------------------------------------------------------------------------


#pragma hdrstop

#include "Eviamall.h"
#include "MallExportData.h"
#include "GlobalSettings.h"
#include "IBillCalculator.h"
#include "Comms.h"
#include "DeviceRealTerminal.h"
#include "MMMessageBox.h"
#include <Math.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

TEviaMall::TEviaMall()
{
    terminalCondition = "AND a.DEVICE_KEY = :DEVICE_KEY ";
    isMasterTerminal = TGlobalSettings::Instance().EnableDepositBagNum;
    deviceKey = TDeviceRealTerminal::Instance().ID.ProfileKey;
}
//---------------------------------------------------------------------------
TEviaMallField::TEviaMallField()
{
    RecordId = 0;
    StallCode = "";
    SalesDate = Now();
    SalesTime = "";
    GrossSales = 0.00;
    TotalVat = 0.00;
    TotalDiscount = 0.00;
    TotalServiceCharge = 0.00;
    NoOfTransactions = 0;
    SalesDept = "";
    NoOfRefund = 0;
    TotalRefund = 0.00;
    NoOfcancelledTransaction = 0;
    TotalcancelledAmount = 0.00;
    NonVatableGrossSales = 0.00;
    PosNumber = 0;
    OldGrandTotal = 0.00;
    NewGrandTotal = 0.00;
    ZKey =0;

}

//---------------------------------------------------------------------------

void TEviaMallField::SetRecordID(UnicodeString recordId)
{
    _recordId = recordId;
}
//---------------------------------------------------------------------------
void TEviaMallField::SetStallCode(UnicodeString stallCode)
{
    _stallCode = stallCode;
}
//---------------------------------------------------------------------------
void TEviaMallField::SetSalesDate(TDateTime salesDate)
{
    _salesDate = salesDate;
}
//---------------------------------------------------------------------------
void TEviaMallField::SetSalesTime(UnicodeString salesTime)
{
    _salesTime = salesTime;
}
//---------------------------------------------------------------------------
void TEviaMallField::SetGrossSales(double grossSales)
{
    _grossSales = grossSales;
}
//---------------------------------------------------------------------------
void TEviaMallField::SetTotalVat(double totalvat)
{
    _totalvat = totalvat;
}
//---------------------------------------------------------------------------
void TEviaMallField::SetTotalDiscount(double totaldiscount)
{
    _totaldiscount = totaldiscount;
}
//---------------------------------------------------------------------------
void TEviaMallField::SetTotalServiceCharge(double totalserviceCahrge)
{
    _totalserviceCahrge = totalserviceCahrge;
}
//---------------------------------------------------------------------------
void TEviaMallField::SetNoOfTransactions(int noOfTrans)
{
    _noOfTrans = noOfTrans;
}
//---------------------------------------------------------------------------
void TEviaMallField::SetSalesDept(UnicodeString salesDept)
{
    _salesDept = salesDept;
}
//---------------------------------------------------------------------------
void TEviaMallField::SetNoOfRefund(int noOfRefund)
{
    _noOfRefund = noOfRefund;
}

void TEviaMallField::SetTotalRefund(double totalrefund)
{
    _totalrefund = totalrefund;
}

void TEviaMallField::SetNoOfcancelledTransaction(int noOfcancelledTransaction)
{
    _noOfcancelledTransaction = noOfcancelledTransaction;
}

void TEviaMallField::SetTotalcancelledAmount(double totalcancelledAmount)
{
    _totalcancelledAmount = totalcancelledAmount;
}

void TEviaMallField::SetNonVatableGrossSales(double nonVatableGrossSales)
{
    _nonVatableGrossSales = nonVatableGrossSales;
}

void TEviaMallField::SetPosNumber(int posNumber)
{
    _posNumber = posNumber;
}

void TEviaMallField::SetOldGrandTotal(double oldGrandTotal)
{
    _oldGrandTotal = oldGrandTotal;
}

void TEviaMallField::SetNewGrandTotal(double newGrandTotal)
{
    _newGrandTotal = newGrandTotal;
}

void TEviaMallField::SetZKey(int zKey)
{
    _zkey = zKey;
}
void TEviaMallField::SetSalesBySalesType(std::map<int, double> salesBySalestype)
{
    _salesBysalesType = salesBySalestype;
}


//----------------------------------------------------------------------------

//---------------------------------------------------------------------------


void TEviaMallField::SetTotaltaxWithoutVat(double totaltaxwithoutvat)
{
    _totaltaxwithoutvat = totaltaxwithoutvat;
}

//---------------------------------------------------------------------------

TMallExportSalesWrapper TEviaMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime)
{

    std::list<TMallExportSettings>::iterator it;
    TMallExportSalesWrapper salesWrapper;
    std::list<TMallExportSalesData> mallExportSalesData;

    try
    {
       TEviaMallField *fieldData = new  TEviaMallField();
       UnicodeString saletime =  Now().FormatString("hh:00");
       fieldData->SalesTime  = saletime ;

       for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
       {
            if(it->Value != "" )
            {
               if(it->ControlName == "edMallTenantNo")
                {
                    UnicodeString tenantcode =  it->Value;

                    fieldData->StallCode =  "\"" + tenantcode + "\""   ;

                }
                else if(it->ControlName == "edMallTerminalNo" )
                {
                   fieldData->PosNumber = StrToInt(it->Value) ;

                }

            }
        }

        for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
        {
                TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

                PrepareDataByItem(paymentTransaction.DBTransaction, Order, *fieldData,paymentTransaction);

                for (int i = 0; i < Order->SubOrders->Count; i++)
				{
					TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
                    PrepareDataByItem(paymentTransaction.DBTransaction, CurrentSubOrder, *fieldData,paymentTransaction);
                }
        }

        fieldData->TotalRefund = paymentTransaction.Money.FinalPrice > 0 ? 0 : fabs(paymentTransaction.Money.FinalPrice);
        if(fieldData->TotalRefund >0)
        {
          fieldData->NoOfRefund =  1;
          fieldData->GrossSales = 0;
        }

        fieldData->OldGrandTotal = 0.00;
        int mallid = TGlobalSettings::Instance().mallInfo.MallId ;
        fieldData->OldGrandTotal = GetOldAccumulatedSales(paymentTransaction.DBTransaction, 18,mallid);

        fieldData->NewGrandTotal = (fieldData->GrossSales + fieldData->NonVatableGrossSales + fieldData->OldGrandTotal) - fieldData->TotalRefund ;


        fieldData->NoOfTransactions =  1;
        fieldData->RecordId = "01";

        InsertFieldInToList(paymentTransaction.DBTransaction, mallExportSalesData, *fieldData, arcBillKey);

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

//---------------------------------------------------------------------------
void TEviaMall::PrepareDataByItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *order, TEviaMallField &fieldData,TPaymentTransaction &paymentTransaction)
{

    double grosssaleamountwithvat = 0;
    double grosssaleamountwithoutvat = 0;
    double salesBySalesType = 0;
    bool isVatable = IsItemVatable(order, fieldData);
    for (std::vector <TDiscount> ::const_iterator ptrDiscounts = order->Discounts.begin(); ptrDiscounts != order->Discounts.end();std::advance(ptrDiscounts, 1))
    {
        fieldData.TotalDiscount += (double)order->DiscountValue_BillCalc(ptrDiscounts);
    }

    if(isVatable)
    {
           grosssaleamountwithvat = (double)(order->PriceEach_BillCalc()*order->GetQty()) ;
           fieldData.GrossSales +=grosssaleamountwithvat;
    }
    else
    {
             grosssaleamountwithoutvat = (double)(order->PriceEach_BillCalc()*order->GetQty()) ;
             fieldData.NonVatableGrossSales +=grosssaleamountwithoutvat;
    }


     TItemComplete *cancelOrder =   (TItemComplete*)order;
     fieldData.TotalcancelledAmount += (double)(cancelOrder->TabContainerName != "" && order->BillCalcResult.BasePrice == 0.00 ?
                                                                                    (order->GetQty()*order->PriceLevel0) : 0);
     if(fieldData.TotalcancelledAmount >0)
     fieldData.NoOfcancelledTransaction =  1;
     salesBySalesType =  (double)(order->PriceEach_BillCalc()*order->GetQty());
     int salesTypeId = GetItemSalesId(dbTransaction, order->ItemKey);
    if(salesTypeId)
    {

       UnicodeString salesdeptformat = GetSaleDeptName(dbTransaction, order->ItemKey,salesTypeId ) ;
        fieldData.SalesDept =  "\"" + salesdeptformat + "\"";
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
//---------------------------------------------------------------------------
TMallExportPrepareData TEviaMall::PrepareDataForExport(int zKey)
{

    TMallExportPrepareData preparedData;
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        int index = 1;
        std::vector<int> devicekey;
        Getdevicekey(dbTransaction,zKey,devicekey);
        int devicekeyvalue  = 0;
        std::list<TMallExportSalesData> preparelist;
        std::list<TMallExportSalesData>::iterator iter;

        for(int i=0; i<devicekey.size(); i++)
        {
            devicekeyvalue = devicekey[i];
            std::set<int> keyToCheck1;
            std::set<int> keyToCheck2;
            std::set<int> keyToCheck3;

            int hourSalekeys1[3] = {9,11,13};
            int hourSalekeys2[7] = {5,6,7,8,12,14,15};
            int hourSalekeys3[5]  ={1,2,3,4,10} ;
            keyToCheck1 = InsertInToSet(hourSalekeys1, 3);
            keyToCheck2 = InsertInToSet(hourSalekeys2, 7);
            keyToCheck3 = InsertInToSet(hourSalekeys3, 5);


            PrepareDataForHourlySalesFile(dbTransaction, keyToCheck1, keyToCheck2,keyToCheck3, preparedData,preparelist, 1,devicekeyvalue, zKey);
            keyToCheck1.clear();
            keyToCheck2.clear();
            keyToCheck3.clear();

            int dailySalesKeys1[9] = {1,2,3,4,10,16,17,18,19} ;
            int dailySalesKeys2[2] = {2,3} ;
            keyToCheck1 = InsertInToSet(dailySalesKeys1, 9);
            keyToCheck2 = InsertInToSet(dailySalesKeys2, 2);



            PrepareDataForDailySalesPerDeptFile(dbTransaction,keyToCheck1,keyToCheck2,10,1,preparedData,preparelist,1,devicekeyvalue,zKey) ;

            keyToCheck1.clear();
            keyToCheck2.clear();

            int grandtotalkeys[4] ={1,2,3,19};
            keyToCheck1 = InsertInToSet(grandtotalkeys, 4);
            PrepareDataForGrandTotalsFile(dbTransaction,keyToCheck1,18,preparedData,preparelist,1,devicekeyvalue,zKey);


        }
         preparedData.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(1, preparelist ));

        preparedData.SalesData.size();

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
//---------------------------------------------------------------------------
IExporterInterface* TEviaMall::CreateExportMedium()
{
    int mallid = TGlobalSettings::Instance().mallInfo.MallId ;
    UnicodeString exportType = GetExportType(mallid)  ;
    if(exportType == ".sal")
    {

        return new TMallExportTextFile;
    }
    else
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Export Type not found");
		throw;
    }

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TEviaMall::InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData,
                                TEviaMallField &fieldData, int arcBillKey)
{
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Record ID ", "UnicodeString", fieldData.RecordId, 1, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Stall Code", "UnicodeString", fieldData.StallCode, 2, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Date (mm/dd/yyyy)", "TDateTime", Now().FormatString("mm/dd/yyyy"), 3, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Time (hh:mm)", "UnicodeString",fieldData.SalesTime , 4, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Gross Sales Amount", "double", fieldData.GrossSales, 5, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Vat Amount", "double", fieldData.TotalVat, 6, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Discount", "double", fieldData.TotalDiscount, 7, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Total ServiceCharge", "double", fieldData.TotalServiceCharge, 8, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "No of SalesTransactions", "int", fieldData.NoOfTransactions, 9, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "SalesDept Code", "UnicodeString", fieldData.SalesDept, 10, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "NoOfRefund", "int", fieldData.NoOfRefund, 11, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Refund Amount", "double", fieldData.TotalRefund, 12, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "No of Cancelled Transaction", "int", fieldData.NoOfcancelledTransaction, 13, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Cancelled Amount", "double", fieldData.TotalcancelledAmount, 14, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Total NonVatableGrossSales", "double", fieldData.NonVatableGrossSales, 15, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Terminal Number", "int", fieldData.PosNumber, 16, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Old Grand Total", "double", fieldData.OldGrandTotal, 17, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "New Grand Total", "double", fieldData.NewGrandTotal, 18, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Reset Counter", "int", fieldData.ZKey, 19, arcBillKey);

}
//---------------------------------------------------------------------------
int TEviaMall::GetItemSalesId(Database::TDBTransaction &dbTransaction, int itemKey)
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
//---------------------------------------------------------------------------
std::set<int> TEviaMall::InsertInToSet(int arr[], int size)
{
    std::set<int> keyToCheck;
    for(int index = 0; index < size; index++)
            keyToCheck.insert(arr[index]);

    return keyToCheck;
}

//---------------------------------------------------------------------------
UnicodeString TEviaMall::GetSaleDeptName(Database::TDBTransaction &dbTransaction, int itemKey, int saletypeId)
{

    UnicodeString salesDeptname = "";
    try
    {
        Database::TcpIBSQL ibInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(ibInternalQuery);

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =  "SELECT upper(b.SALES_TYPE_NAME) SALES_TYPE_NAME "
                                    "FROM MALL_SALES_TYPE_ITEMS_RELATION a "
                                    "left join MALL_SALES_TYPE b on a.SALES_TYPE_ID = b.SALES_TYPE_ID "
                                    "where a.STI_ID = :STI_ID "
                                    "GROUP BY 1";

        ibInternalQuery->ParamByName("STI_ID")->AsInteger = saletypeId;

        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
            salesDeptname = ibInternalQuery->Fields[0]->AsString;


    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return salesDeptname;
}


UnicodeString TEviaMall::GetFieldIndexList(std::set<int> indexKeys)
{
    std::set<int>::iterator indexKeysIt = indexKeys.begin();
    UnicodeString indexKeyList = IntToStr(*indexKeysIt);
    indexKeysIt++;
    for(; indexKeysIt != indexKeys.end(); indexKeysIt++)
    {
        indexKeyList += ", " + IntToStr(*indexKeysIt);
    }
    return indexKeyList;
}

UnicodeString TEviaMall::GetFileName(Database::TDBTransaction &dBTransaction, std::set<int> keysToSelect, int zKey)
{
     UnicodeString fileName = "";
    try
    {
        UnicodeString indexKeysList = GetFieldIndexList(keysToSelect);
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

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

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 3;
        if(zKey)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;
        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            if(IBInternalQuery->Fields[0]->AsInteger == 3)
            {
                 TDateTime date = IBInternalQuery->Fields[5]->AsDate;

                 UnicodeString dateformat= date.FormatString("mmddyy");
                 int resetcount = IBInternalQuery->Fields[4]->AsInteger;
                 fileName =  fileName + dateformat + resetcount;
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

int TEviaMall::GetMaxZedKey(Database::TDBTransaction &dbTransaction, int zKey)
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

        selectQuery->ParamByName("MALL_KEY")->AsInteger = 3;

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

bool TEviaMall:: CheckSingleOrMultiplePos(Database::TDBTransaction &dbTransaction, int zKey)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dbTransaction.RegisterQuery(IBInternalQuery);
    bool isrecordPresent = false;
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.DEVICE_KEY"
                                     " FROM MALLEXPORT_SALES a"
                                     " WHERE a.MALL_KEY = :MALL_KEY ";


        if(zKey)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) OR a.Z_KEY = :Z_KEY) ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY 1";

        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 3 ;
        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

        while(!IBInternalQuery->Eof)
            IBInternalQuery->Next();


        if(IBInternalQuery->RecordCount > 1)
        {
         	isrecordPresent  = true;
        }


    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
	}

    return isrecordPresent;

}

void TEviaMall::PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys1,std::set<int> indexKeys2, std::set<int> indexKeys3,
                                                   TMallExportPrepareData &prepareDataForHSF, std::list<TMallExportSalesData> &prepareListForHSF, int index,int terminalkey , int zKey)
{


   UnicodeString fileName;
   std::list<TMallExportSettings> mallSettings;
   bool IsmultipledevicekeyExist = false;
   try
   {
      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
      dBTransaction.RegisterQuery(IBInternalQuery);
      std::set<int>keysToSelect;
      IsmultipledevicekeyExist =  CheckSingleOrMultiplePos(dBTransaction,zKey) ;
     
      int  fileNameKeys[1] = {3};

      keysToSelect = InsertInToSet(fileNameKeys, 1);
      fileName = GetFileName(dBTransaction, keysToSelect, zKey)  ;
      prepareDataForHSF.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

      keysToSelect.clear();


      UnicodeString indexKeysList1 = GetFieldIndexList(indexKeys1);
      UnicodeString indexKeysList2 = GetFieldIndexList(indexKeys2);
      UnicodeString indexKeysList3 = GetFieldIndexList(indexKeys3);

        IBInternalQuery->Close();


           IBInternalQuery->SQL->Text =

            "SELECT HOURLYDATA.FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, HOURLYDATA.FIELD_VALUE , HOURLYDATA.VALUE_TYPE, HOURLYDATA.Hour_code "
            "FROM"

            "(SELECT  HOURLYDATA.FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, CAST(SUM(HOURLYDATA.FIELD_VALUE) AS INT ) FIELD_VALUE , HOURLYDATA.VALUE_TYPE, HOURLYDATA.Hour_code "
                            "FROM "
                                    "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS INT) FIELD_VALUE, a.VALUE_TYPE,  "
                                            "Extract (Hour From a.DATE_CREATED) Hour_code "
                                     "FROM MALLEXPORT_SALES a "
                                    " WHERE a.FIELD_INDEX IN(" + indexKeysList1 + ") AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY ";


        if(zKey)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES)) ";
        }


        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.FIELD_INDEX ASC )HOURLYDATA "
        "GROUP BY 1,2,4 ,5 "




       "UNION ALL "

        "SELECT  HOURLYDATA.FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, CAST(SUM(HOURLYDATA.FIELD_VALUE) AS NUMERIC(17,2) ) FIELD_VALUE , HOURLYDATA.VALUE_TYPE, HOURLYDATA.Hour_code "
                            "FROM "
                                    "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE,  "
                                            "Extract (Hour From a.DATE_CREATED) Hour_code "
                                     "FROM MALLEXPORT_SALES a "
                                    " WHERE a.FIELD_INDEX IN(" + indexKeysList2 + ") AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY ";

        if(zKey)
        {

            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";

        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES)) ";
        }


        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.FIELD_INDEX ASC )HOURLYDATA "
        "GROUP BY 1,2,4 ,5 "




        "UNION ALL "

       "SELECT HOURLYDATA.FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, HOURLYDATA.FIELD_VALUE , HOURLYDATA.VALUE_TYPE, HOURLYDATA.Hour_code "
                            "FROM "
                                    "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, case when (a.FIELD_INDEX =1 ) then cast('01' as varchar(2)) else (a.FIELD_VALUE) end FIELD_VALUE, a.VALUE_TYPE,  "
                                            "Extract (Hour From a.DATE_CREATED) Hour_code "
                                     "FROM MALLEXPORT_SALES a "
                                    " WHERE a.FIELD_INDEX IN(" + indexKeysList3 + ") AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY ";

        if(zKey)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES)) ";
        }


        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.FIELD_INDEX ASC )HOURLYDATA "
        "GROUP BY 1,2,3,4 ,5 ";

        if(IsmultipledevicekeyExist)
        {

          IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +

          "UNION ALL "

                                        "SELECT HOURLYDATA.FIELD_INDEX, "
                                        "HOURLYDATA.FIELD, HOURLYDATA.FIELD_VALUE , HOURLYDATA.VALUE_TYPE, HOURLYDATA.Hour_code "


                                        "FROM "
                                        "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, case when (a.FIELD_INDEX =16 ) then CAST((a.FIELD_VALUE) AS int) end FIELD_VALUE, a.VALUE_TYPE, "
                                        "Extract (Hour From a.DATE_CREATED) Hour_code "
                                        "FROM MALLEXPORT_SALES a "
                                        "WHERE a.FIELD_INDEX  = 16 AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY ";

               if(zKey)
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
               }
               else
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES)) ";
               }


                IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.FIELD_INDEX ASC )HOURLYDATA "
                "GROUP BY 1,2,3,4 ,5 ";

          }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + ")HOURLYDATA "

         "ORDER BY 5 ASC , 1 ASC ";



        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 3 ;
        if(zKey)
        {
        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;
        }
        IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = terminalkey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TMallExportSalesData salesData;

            salesData.DataValue = IBInternalQuery->Fields[2]->AsString;
            salesData.FieldIndex = IBInternalQuery->Fields[0]->AsInteger;

            if(salesData.FieldIndex == 1)
            {
                 if(prepareListForHSF.size())
                 {
                     salesData.DataValue ="\r\n" +  salesData.DataValue;
                 }

            }

           salesData.DataValue = salesData.DataValue + " ";

           prepareListForHSF.push_back(salesData);

        }


    }

   catch(Exception &E)
   {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
   }

}

void TEviaMall::PrepareDataForDailySalesPerDeptFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys1,std::set<int> indexKeys2, int index1,int index2,
                                                   TMallExportPrepareData &prepareDataForDSFPD, std::list<TMallExportSalesData> &prepareListForDSF, int index,int terminalkeys, int zKey)
{
    bool IsmultipledevicekeyExist = false;
    try
    {
      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
      dBTransaction.RegisterQuery(IBInternalQuery);

      std::set<int>keysToSelect;
      IsmultipledevicekeyExist =  CheckSingleOrMultiplePos(dBTransaction,zKey) ;

      UnicodeString indexKeysList1 = GetFieldIndexList(indexKeys1);
      UnicodeString indexKeysList2 = GetFieldIndexList(indexKeys2);


        IBInternalQuery->Close();

        IBInternalQuery->SQL->Text =
          " Select DailySales.FIELD_INDEX,DailySales.FIELD,DailySales.FIELD_VALUE,DailySales.VALUE_TYPE,DailySales.Z_KEY,DailySales.DEVICE_KEY"
          " FROM"
          "(SELECT DAILYDATA.FIELD_INDEX,DAILYDATA.FIELD,"
          " CAST( case when (DAILYDATA.FIELD_INDEX = 9)  then SUM(DAILYDATA.FIELD_VALUE)  else SUM(DAILYDATA.FIELD_VALUE)  end AS  NUMERIC(17,2)) FIELD_VALUE,"
          " DAILYDATA.VALUE_TYPE ,DAILYDATA.Z_KEY,DAILYDATA.DEVICE_KEY"
          " FROM"
          " (SELECT a.ARCBILL_KEY,a.MALLEXPORT_SALE_KEY, a.FIELD,(case when (a.FIELD_INDEX IN (5,6,7,8,9,11,12,13,14,15)) then (a.FIELD_INDEX - 1)"
          " end)FIELD_INDEX,"
          " CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, a.Z_KEY,a.DEVICE_KEY"
          " FROM MALLEXPORT_SALES a "
          " WHERE a.FIELD_INDEX NOT IN(" + indexKeysList1 + ") AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY ";

               if(zKey)
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
               }
               else
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES)) ";
               }



            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY a.ARCBILL_KEY,a.MALLEXPORT_SALE_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE,a.Z_KEY,a.DEVICE_KEY"
            " ORDER BY A.ARCBILL_KEY ASC )DAILYDATA"
            " GROUP BY 1,2,4,5,6"


                                 " UNION ALL"
            " SELECT (case when (a.FIELD_INDEX=10) then 9 end) FIELD_INDEX, a.FIELD, CAST((a.FIELD_VALUE) AS varchar(20)) FIELD_VALUE,a.VALUE_TYPE,"
            " a.Z_KEY,a.DEVICE_KEY"
            " FROM MALLEXPORT_SALES a"
            " WHERE a.FIELD_INDEX = :INDEX_KEY1  AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY ";
               if(zKey)
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
               }
               else
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES)) ";
               }

            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY a.FIELD, a.FIELD_INDEX, a.VALUE_TYPE, a.FIELD_VALUE ,a.Z_KEY,a.DEVICE_KEY"


                                 " UNION ALL"
            " SELECT  a.FIELD_INDEX, a.FIELD ,CAST((a.FIELD_VALUE) AS varchar(20)) FIELD_VALUE, a.VALUE_TYPE, a.Z_KEY,a.DEVICE_KEY"
            " FROM MALLEXPORT_SALES a"
            " WHERE a.FIELD_INDEX  IN ( " + indexKeysList2 + " ) AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY ";
               if(zKey)
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
               }
               else
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES)) ";
               }

             IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +"GROUP BY a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE, a.Z_KEY,a.DEVICE_KEY"

                                " UNION ALL"
            " SELECT  a.FIELD_INDEX, a.FIELD, cast('99' as varchar(10))  FIELD_VALUE, a.VALUE_TYPE, a.Z_KEY,a.DEVICE_KEY"
            " FROM MALLEXPORT_SALES a"
            " WHERE a.FIELD_INDEX = :INDEX_KEY2 AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY ";

               if(zKey)
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
               }
               else
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES)) ";
               }


            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE ,a.Z_KEY,a.DEVICE_KEY";


            if(IsmultipledevicekeyExist)
            {
              IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +


                                 " UNION ALL"
            " SELECT (case when (a.FIELD_INDEX=16) then 15 end) FIELD_INDEX, a.FIELD, CAST((a.FIELD_VALUE) AS Numeric) FIELD_VALUE,a.VALUE_TYPE,"
            " a.Z_KEY,a.DEVICE_KEY"
            " FROM MALLEXPORT_SALES a"
            " WHERE a.FIELD_INDEX = 16  AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY ";
               if(zKey)
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
               }
               else
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES)) ";
               }

            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY a.FIELD, a.FIELD_INDEX, a.VALUE_TYPE, a.FIELD_VALUE ,a.Z_KEY,a.DEVICE_KEY" ;



            }

             IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " )DailySales "
              "ORDER BY DailySales.DEVICE_KEY,DailySales.FIELD_INDEX";


            IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 3;
            if(zKey)
            {
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;
            }

            IBInternalQuery->ParamByName("INDEX_KEY1")->AsInteger = index1;

            IBInternalQuery->ParamByName("INDEX_KEY2")->AsInteger = index2;
            IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = terminalkeys;

            IBInternalQuery->ExecQuery();

            for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
            {
                TMallExportSalesData salesData;

                salesData.DataValue = IBInternalQuery->Fields[2]->AsString;
                salesData.FieldIndex = IBInternalQuery->Fields[0]->AsInteger;

                if(salesData.FieldIndex == 1)
                {
                    if(prepareListForDSF.size())
                    {
                        salesData.DataValue ="\r\n" +  salesData.DataValue;

                    }

                }

               salesData.DataValue = salesData.DataValue + " ";
                prepareListForDSF.push_back(salesData);

            }

    }

    catch(Exception &E)
   {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
   }

}

void TEviaMall::Getdevicekey(Database::TDBTransaction &dbTransaction,int zkey , std::vector <int> &devicekeys)
{

    try
    {
        Database::TcpIBSQL ibInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(ibInternalQuery);

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =

        " SELECT a.DEVICE_KEY"
            " FROM MALLEXPORT_SALES a"
            " WHERE a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";

        if(!isMasterTerminal)
        {
            ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + terminalCondition;
        }

        ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + "GROUP BY 1";

        if(!isMasterTerminal)
        {
            ibInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
        }

        ibInternalQuery->ExecQuery();
        for ( ; !ibInternalQuery->Eof; ibInternalQuery->Next())
        {
           int key = ibInternalQuery->Fields[0]->AsInteger;
           devicekeys.push_back(key) ;
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

void TEviaMall::PrepareDataForGrandTotalsFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys1, int index1,
                                        TMallExportPrepareData &prepareDataForDGT,std::list<TMallExportSalesData> &prepareListForDGT, int index,int terminalkey, int zKey )
{
    try
    {
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);
        UnicodeString indexKeysList1 = GetFieldIndexList(indexKeys1);
        int maxZedKey;
        bool IsmultipledevicekeyExist = false;
        IsmultipledevicekeyExist =  CheckSingleOrMultiplePos(dBTransaction,zKey) ;
        int IndexForinitialOldGrand =17;
        if(!zKey)
            maxZedKey = GetMaxZedKey(dBTransaction);
        else
            maxZedKey = zKey;

        int maxZedKey2 = GetMaxZedKey(dBTransaction, maxZedKey);




        IBInternalQuery->Close();


          IBInternalQuery->SQL->Text =

           " SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD,DAILYDATA.FIELD_VALUE,"
           " DAILYDATA.VALUE_TYPE,DAILYDATA.Z_KEY,DAILYDATA.DEVICE_KEY"
           " From"


         " (SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD,CAST((DAILYDATA.FIELD_VALUE) AS NUMERIC(17,2))FIELD_VALUE,"
         " DAILYDATA.VALUE_TYPE,DAILYDATA.Z_KEY,DAILYDATA.DEVICE_KEY"

         " FROM"
         " (Select (case when (a.FIELD_INDEX = 18) then 5 end)FIELD_INDEX,a.FIELD,CAST((a.FIELD_VALUE) AS NUMERIC(17,2))FIELD_VALUE, a.VALUE_TYPE,a.Z_KEY,a.DEVICE_KEY"
         " FROM MALLEXPORT_SALES A"
         " where a.DEVICE_KEY = :DEVICE_KEY AND a.FIELD_INDEX = 18 AND a.MALL_KEY = :MALL_KEY AND a.MALLEXPORT_SALE_KEY = (SELECT MAX(MALLEXPORT_SALE_KEY) FROM MALLEXPORT_SALES A"
         " where a.FIELD_INDEX = 18 AND a.DEVICE_KEY = :DEVICE_KEY)";
            if(zKey)
            {
                IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " AND a.Z_KEY = :Z_KEY";
            }
            else
            {
                IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " AND (a.Z_KEY = :Max_Z_KEY)";
            }
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " )DAILYDATA";



           IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +



        " UNION all";
         if(maxZedKey2)
         {

             IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +

         " SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD,CAST((DAILYDATA.FIELD_VALUE) AS NUMERIC(17,2))FIELD_VALUE,"
         " DAILYDATA.VALUE_TYPE,DAILYDATA.Z_KEY,DAILYDATA.DEVICE_KEY"

         " FROM"
         " (Select (case when (a.FIELD_INDEX = 17) then 4 end)FIELD_INDEX,a.FIELD,CAST((a.FIELD_VALUE) AS NUMERIC(17,2))FIELD_VALUE, a.VALUE_TYPE,a.Z_KEY,a.DEVICE_KEY"
         " FROM MALLEXPORT_SALES A"
         " where a.DEVICE_KEY = :DEVICE_KEY AND a.FIELD_INDEX = 17 AND a.MALL_KEY = :MALL_KEY AND a.MALLEXPORT_SALE_KEY = (SELECT MAX(MALLEXPORT_SALE_KEY) FROM MALLEXPORT_SALES A"
         " where a.FIELD_INDEX = 17 AND a.DEVICE_KEY = :DEVICE_KEY)";
            if(zKey)
            {
                IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " AND a.Z_KEY = :Z_KEY";
            }
            else
            {
                IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " AND (a.Z_KEY = :Max_Z_KEY )";
            }


         IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " )DAILYDATA";


        }
        else
        {
             IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                            " SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, DAILYDATA.FIELD_VALUE,"
                                    " DAILYDATA.VALUE_TYPE,DAILYDATA.Z_KEY,DAILYDATA.DEVICE_KEY"
                              " FROM"
                                    " (SELECT a.ARCBILL_KEY,MALLEXPORT_SALE_KEY,a.FIELD,case when (a.FIELD_INDEX = 17) then 4 end FIELD_INDEX, CAST(0.00  AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, a.Z_KEY,a.DEVICE_KEY"
                                     " FROM MALLEXPORT_SALES a"
                                     " WHERE a.FIELD_INDEX  = 17"
                                     " AND a.MALL_KEY = :MALL_KEY  AND a.DEVICE_KEY = :DEVICE_KEY";
                                        if(zKey)
                                        {
                                            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " AND a.Z_KEY = :Z_KEY";
                                        }
                                        else
                                        {
                                            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " AND (a.Z_KEY = :Max_Z_KEY)";
                                        }

      IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " GROUP BY a.ARCBILL_KEY,a.MALLEXPORT_SALE_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, A.Z_KEY,a.DEVICE_KEY"
                                      " ORDER BY A.ARCBILL_KEY ASC )DAILYDATA"
                                      " GROUP BY 1,2,4,3,5,6";


        }
        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +

       " UNION all"

       " SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD,DAILYDATA.FIELD_VALUE , DAILYDATA.VALUE_TYPE,DAILYDATA.Z_KEY,DAILYDATA.DEVICE_KEY"
       " FROM"

       " (SELECT a.ARCBILL_KEY, a.FIELD, case when (a.FIELD_INDEX= 19) then 6 else a.FIELD_INDEX end FIELD_INDEX, case when (a.FIELD_INDEX =1 ) then cast('95' as varchar(2)) else (a.FIELD_VALUE) end FIELD_VALUE,"
       " a.VALUE_TYPE ,a.Z_KEY,a.DEVICE_KEY"
       " FROM MALLEXPORT_SALES a"
       " WHERE a.FIELD_INDEX  IN( " + indexKeysList1 + "   ) AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY";
       if(zKey)
       {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " AND a.Z_KEY = :Z_KEY";
       }
       else
       {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " AND (a.Z_KEY = :Max_Z_KEY)";
       }
       IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " ORDER BY A.MALLEXPORT_SALE_KEY ASC )DAILYDATA"
       " GROUP BY 1,2,4,3,5,6";

      if(IsmultipledevicekeyExist)
      {
        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +


                                 " UNION ALL"
            " SELECT (case when (a.FIELD_INDEX=16) then 7 end) FIELD_INDEX, a.FIELD, CAST((a.FIELD_VALUE) AS Numeric) FIELD_VALUE,a.VALUE_TYPE,"
            " a.Z_KEY,a.DEVICE_KEY"
            " FROM MALLEXPORT_SALES a"
            " WHERE a.FIELD_INDEX = 16  AND a.MALL_KEY = :MALL_KEY AND a.DEVICE_KEY = :DEVICE_KEY ";
               if(zKey)
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
               }
               else
               {
                    IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND (a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES)) ";
               }

            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY a.FIELD, a.FIELD_INDEX, a.VALUE_TYPE, a.FIELD_VALUE ,a.Z_KEY,a.DEVICE_KEY" ;

        }



             IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " )DAILYDATA"
             " order by 1 asc " ;



       IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 3;

       if(!zKey)
       {
       IBInternalQuery->ParamByName("Max_Z_KEY")->AsInteger = maxZedKey;
       }
       if(zKey)
       {
         IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;
       }
       IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = terminalkey;
       IBInternalQuery->ExecQuery();
       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
            TMallExportSalesData salesData;

            salesData.DataValue = IBInternalQuery->Fields[2]->AsString;
            salesData.FieldIndex = IBInternalQuery->Fields[0]->AsInteger;
            UnicodeString fieldindex = IBInternalQuery->Fields[0]->AsString;
            if(salesData.FieldIndex == 1)
                {
                    if(prepareListForDGT.size()>0)
                    {
                        salesData.DataValue ="\r\n" +  salesData.DataValue;

                    }

                }

            salesData.DataValue = salesData.DataValue + " ";
            prepareListForDGT.push_back(salesData);

       }

    }
   catch(Exception &E)
   {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
   }



}
bool TEviaMall::IsItemVatable(TItemMinorComplete *order, TEviaMallField &fieldData)
{
    std::vector<BillCalculator::TTaxResult> taxInfomation = order->BillCalcResult.Tax;
    bool isVatable = false;
    bool isVatAppliedOnServiceCharge = false;

    for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
    {
        switch( itTaxes->TaxType )
        {
            case TTaxType::ttSale:
                 fieldData.TotalVat += (double)itTaxes->Value;
                 isVatable = true;
                 break;
            case TTaxType::ttLocal:
                 isVatable = false;
                 break;
        }
    }
    if (order->BillCalcResult.ServiceCharge.Percentage != 0)
    {
        fieldData.TotalServiceCharge += (double)order->BillCalcResult.ServiceCharge.Value;
        if (order->BillCalcResult.ServiceCharge.TaxPercentage != 0)
        {
           isVatable = true;
           isVatAppliedOnServiceCharge = true;
        }
    }
    if( fieldData.TotalVat == 0.00 && !isVatAppliedOnServiceCharge )
        isVatable = false;
    return isVatable;
}






