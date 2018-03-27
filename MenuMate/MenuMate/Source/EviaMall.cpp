  //---------------------------------------------------------------------------


#pragma hdrstop

#include "Eviamall.h"
#include "MallExportData.h"
#include "GlobalSettings.h"
#include "IBillCalculator.h"
#include "Comms.h"
#include "DeviceRealTerminal.h"
#include <Math.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

TEviaMall::TEviaMall()
{
     terminalCondition = " AND a.DEVICE_KEY = :DEVICE_KEY ";
    isMasterTerminal = TGlobalSettings::Instance().EnableDepositBagNum;
    deviceKey = TDeviceRealTerminal::Instance().ID.ProfileKey;
}
//---------------------------------------------------------------------------
TEviaMallField::TEviaMallField()
{
    RecordId = 0;
    StallCode = "";
    SalesDate = Now();
    SalesTime = Now().CurrentTime();
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
    OldGrandTotal = 0;
    NewGrandTotal = 0;
    ZKey =0;
    DailySalesTotalPerDept = 0.00;

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
void TEviaMallField::SetSalesTime(TDateTime salesTime)
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

//----------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TEviaMallField::SetDailySalesTotalPerDept(double dailysalestotalperDept)
{
    _dailysalestotalperDept = dailysalestotalperDept;
}

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


    double amount = 0;
    try
    {
       TEviaMallField *fieldData = new  TEviaMallField();
       fieldData->SalesTime = currentTime.FormatString("hh:mm");


       for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
       {
            if(it->Value != "" )
            {
               if(it->ControlName == "edMallTenantNo")
                {
                    fieldData->StallCode = it->Value;

                }
                else if(it->ControlName == "edMallTerminalNo" )
                {
                    fieldData->PosNumber = StrToInt(it->Value);

                }

            }
        }

        for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
        {
                TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

                PrepareDataByItem(paymentTransaction.DBTransaction, Order, *fieldData);

                for (int i = 0; i < Order->SubOrders->Count; i++)
				{
					TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
                    PrepareDataByItem(paymentTransaction.DBTransaction, CurrentSubOrder, *fieldData);
                }
        }

        fieldData->TotalRefund = paymentTransaction.Money.FinalPrice > 0 ? 0 : fabs(paymentTransaction.Money.FinalPrice);
        if(fieldData->TotalRefund >0)
        {
          fieldData->NoOfRefund =  1;
        }

        fieldData->OldGrandTotal = 0;
        fieldData->NewGrandTotal = fieldData->GrossSales + fieldData->NonVatableGrossSales - fieldData->TotalRefund ;

        fieldData->NoOfTransactions =  1;

        InsertFieldInToList(paymentTransaction.DBTransaction, mallExportSalesData, *fieldData, arcBillKey);

        salesWrapper.SalesData = mallExportSalesData;


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
void TEviaMall::PrepareDataByItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *order, TEviaMallField &fieldData)
{
    bool isVatable = IsItemVatable(order, fieldData);
    double salesBySalesType = 0;
    for (std::vector <TDiscount> ::const_iterator ptrDiscounts = order->Discounts.begin(); ptrDiscounts != order->Discounts.end();std::advance(ptrDiscounts, 1))
    {
        fieldData.TotalDiscount += (double)order->DiscountValue_BillCalc(ptrDiscounts);
    }

    if(isVatable)
    {
      if(order->GetQty() > 0 )
      {
      grosssaleamount = (double)(order->PriceEach_BillCalc()*order->GetQty()) ;
      fieldData.GrossSales +=  grosssaleamount;
      }
    }

     TItemComplete *cancelOrder =   (TItemComplete*)order;
     fieldData.TotalcancelledAmount += (double)(cancelOrder->TabContainerName != "" && order->BillCalcResult.BasePrice == 0.00 ?
                                                                                    (order->GetQty()*order->PriceLevel0) : 0);
     if(fieldData.TotalcancelledAmount >0)
     fieldData.NoOfcancelledTransaction =  1;


    int salesTypeId = GetItemSalesId(dbTransaction, order->ItemKey);


    if(salesTypeId)
    {
        salesBySalesType = (double)(order->PriceEach_BillCalc()*order->GetQty()) ;
        std::map <int, double> ::iterator itr = fieldData.SalesBySalesType.find(salesTypeId);

        if(itr != fieldData.SalesBySalesType.end())
        {
            itr->second += salesBySalesType;
        }
        else
        {
            fieldData.SalesBySalesType.insert(std::pair<int, double >(salesTypeId, salesBySalesType));
            itr->second += salesBySalesType;
        }
        fieldData.DailySalesTotalPerDept = itr->second;
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

        std::set<int> keyToCheck;

        int hourSalekeys[4] = {17,18,19,20};
        int hourSalekeys2 = 20;

        keyToCheck = InsertInToSet(hourSalekeys, 4);
        PrepareDataForHourlySalesFile(dbTransaction, keyToCheck, hourSalekeys2, preparedData, 1, zKey);


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
    UnicodeString exportType = GetExportType();
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

bool TEviaMall::IsItemVatable(TItemMinorComplete *order, TEviaMallField &fieldData)
{
    std::vector<BillCalculator::TTaxResult> taxInfomation = order->BillCalcResult.Tax;
    bool isVatable = false;
    double totaltax;
    for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
    {
        switch( itTaxes->TaxType )
        {
            case TTaxType::ttSale:

                fieldData.TotalVat += (double)itTaxes->Value;
                isVatable = true;
                break;
            case TTaxType::ttLocal:
                fieldData.TotaltaxWithoutVat += (double)itTaxes->Value;
                if(order->GetQty() > 0 )
                {
                    grosssaleamountwithoutvat = (double)(order->PriceEach_BillCalc()*order->GetQty()) ;
                    fieldData.NonVatableGrossSales +=grosssaleamountwithoutvat;

                }
                isVatable = true;
                break;
        }


    }

    if (order->BillCalcResult.ServiceCharge.Percentage != 0)
    {
        fieldData.TotalServiceCharge += (double)order->BillCalcResult.ServiceCharge.Value;
        isVatable=true;
    }
    if(fieldData.TotalVat == 0.00 && fieldData.TotaltaxWithoutVat == 0.00 && fieldData.TotalServiceCharge == 0.00 )
    {
       isVatable = false;
    }

    return isVatable;
}
//---------------------------------------------------------------------------
void TEviaMall::InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData,
                                TEviaMallField &fieldData, int arcBillKey)
{

          PushFieldsInToList(dbTransaction, mallExportSalesData, "Record ID ", "UnicodeString", fieldData.RecordId, 1, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Stall Code", "UnicodeString", fieldData.StallCode, 2, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Date (mm/dd/yyyy)", "TDateTime", Now().FormatString("mm/dd/yyyy"), 3, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Time (hh:mm)", "TTime", Now().FormatString("hh:mm"), 4, arcBillKey);
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
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Old Grand Total", "double", fabs(fieldData.OldGrandTotal), 17, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "New Grand Total", "double", fabs(fieldData.NewGrandTotal), 18, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Sales per Department", "double", fieldData.DailySalesTotalPerDept, 19, arcBillKey);
          PushFieldsInToList(dbTransaction, mallExportSalesData, "Reset Counter", "int", fieldData.ZKey, 20, arcBillKey);

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

void TEviaMall::PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, int zIndex,
                                                   TMallExportPrepareData &prepareDataForDSF, int index, int zKey)
{
   UnicodeString fileName;
   std::list<TMallExportSalesData> prepareListForHSF;
   try
   {

     Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
     dBTransaction.RegisterQuery(IBInternalQuery);
      std::set<int>keysToSelect;
      UnicodeString fileNameext = ".sal";
      int  fileNameKeys[1] = {3};

      keysToSelect = InsertInToSet(fileNameKeys, 1);
      fileName = GetFileName(dBTransaction, keysToSelect, zKey) + fileNameext ;
      keysToSelect.clear();

      prepareDataForDSF.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

     int maxZedKey;
     if(!zKey)
     {
        maxZedKey = GetMaxZedKey(dBTransaction);
     }
     else
     {
        maxZedKey = zKey;
     }

     UnicodeString indexKeysList = GetFieldIndexList(indexKeys);
     IBInternalQuery->Close();


      IBInternalQuery->SQL->Text =

                                "SELECT a.ARCBILL_KEY,a.MALLEXPORT_SALE_KEY, a.FIELD, a.FIELD_INDEX , a.FIELD_VALUE, a.VALUE_TYPE, MAX(A.Z_KEY) Z_KEY "
                                             "FROM MALLEXPORT_SALES a "
                                             "WHERE a.FIELD_INDEX NOT IN(" + indexKeysList + ")  "
                                             "AND a.MALL_KEY = :MALL_KEY AND (a.Z_KEY = :MAX_ZKEY ";

      IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                                              " ) GROUP BY a.ARCBILL_KEY,a.MALLEXPORT_SALE_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, a.FIELD_VALUE  "
                                             "ORDER BY A.ARCBILL_KEY ASC " ;


      IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 3;
      IBInternalQuery->ParamByName("MAX_ZKEY")->AsInteger = maxZedKey;

      IBInternalQuery->ExecQuery();

      for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
      {
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[3]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[2]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[4]->AsString;
          prepareListForHSF.push_back(salesData);

       }
      IBInternalQuery->Close();
    }

   catch(Exception &E)
   {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
   }

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

UnicodeString TEviaMall::GetExportType()
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
                                     "INNER JOIN MALLEXPORT_SETTINGS_VALUES MSV ON MES.MALLEXPORT_SETTING_KEY = MSV.MALLEXPORTSETTING_ID "
                                     "WHERE MES.NAME = :NAME AND MSP.MALL_ID = :MALL_ID";

        IBInternalQuery->ParamByName("NAME")->AsString = "TYPE_OF_FILE";
        IBInternalQuery->ParamByName("MALL_ID")->AsInteger = 3;
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



