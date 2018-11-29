//---------------------------------------------------------------------------


#pragma hdrstop

#include "SouthBeachMall.h"
#include "MallExportData.h"
#include "GlobalSettings.h"
#include "IBillCalculator.h"
#include "Comms.h"
#include "DeviceRealTerminal.h"
#include "MMMessageBox.h"
#include <Math.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)
TSouthBeachMall::TSouthBeachMall()
{
}

TSouthBeachMallField::TSouthBeachMallField()
{
  MachineID = "";
  BatchID = 0;
  Hour = "0000";
  ReceiptCount = 0;
  NetSalesAmount = 0.00;
  GST = 0.00;
  TotalDiscount = 0.00;
  NoOfPax = 0;
  ResetCount = 0;

}
//---------------------------------------------------------------------------
void TSouthBeachMallField::SetMachineID(UnicodeString machineID)
{
    _machineID = machineID;
}
//---------------------------------------------------------------------------
void TSouthBeachMallField::SetBatchID(int batchID)
{
    _batchID = batchID;
}
//---------------------------------------------------------------------------
void TSouthBeachMallField::SetHour(UnicodeString hour)
{
    _hour = hour;
}
//---------------------------------------------------------------------------
void TSouthBeachMallField::SetReceiptcount(int receiptcount)
{
    _receiptcount = receiptcount;
}
//---------------------------------------------------------------------------
void TSouthBeachMallField::SetNetsalesamount(double netsalesamount)
{
    _netsalesamount = netsalesamount;
}
//---------------------------------------------------------------------------
void TSouthBeachMallField::SetGst(double gst)
{
    _gst = gst;
}
//---------------------------------------------------------------------------
void TSouthBeachMallField::SetTotalDiscount(double totaldiscount)
{
    _totaldiscount = totaldiscount;
}
//---------------------------------------------------------------------------
void TSouthBeachMallField::SetNoOfPax(int noOfPax)
{
    _noOfPax = noOfPax;
}
//---------------------------------------------------------------------------
void TSouthBeachMallField::SetTotalServiceCharge(double totalserviceCahrge)
{
    _totalserviceCahrge = totalserviceCahrge;
}
//---------------------------------------------------------------------------
void TSouthBeachMallField::SetResetCount(int ResetCount)
{
    _ResetCount = ResetCount;
}
//---------------------------------------------------------------------------
TMallExportSalesWrapper TSouthBeachMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime)
{
    TMallExportSalesWrapper salesWrapper;
    std::list<TMallExportSalesData> mallExportSalesData;
    bool IsFBOutlet = false;
    try
    {
       TSouthBeachMallField *fieldData = new  TSouthBeachMallField();
       int terminalNumber;
       UnicodeString machineId;
       GetTerminalSettings(machineId, terminalNumber);
       fieldData->MachineID = machineId;
       for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
       {
            TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

            PrepareDataByItem(paymentTransaction.DBTransaction, Order, *fieldData, IsFBOutlet);

            for (int i = 0; i < Order->SubOrders->Count; i++)
            {
                TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
                PrepareDataByItem(paymentTransaction.DBTransaction, CurrentSubOrder, *fieldData, IsFBOutlet);
            }
       }
        fieldData->ReceiptCount =  1;
        if(IsFBOutlet)
        {
          fieldData->NoOfPax = GetPatronCount(paymentTransaction);
        }
        InsertFieldInToList(paymentTransaction.DBTransaction, mallExportSalesData, *fieldData, arcBillKey);
        salesWrapper.SalesData = mallExportSalesData;
		delete fieldData;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return salesWrapper;
}
//---------------------------------------------------------------------------
void TSouthBeachMall::PrepareDataByItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *order, TSouthBeachMallField &fieldData,bool &IsFoodAndBevOutlet)
{
    double grosssaleamount = 0;
    double Vat = 0.00;
    double salesBySalesType = 0;
    fieldData.TotalServiceCharge = 0;
    bool isVatable = IsItemVatable(order, fieldData,Vat);

    for (std::vector <TDiscount> ::const_iterator ptrDiscounts = order->Discounts.begin(); ptrDiscounts != order->Discounts.end();std::advance(ptrDiscounts, 1))
    {
        fieldData.TotalDiscount += (double)order->DiscountValue_BillCalc(ptrDiscounts);
    }
    int salesTypeId = GetItemSalesId(dbTransaction, order->ItemKey);
    if(isVatable)
    {
        grosssaleamount = (double)(order->PriceEach_BillCalc()*order->GetQty()) -Vat;
        /*if(salesTypeId != 1)
        {
            grosssaleamount -= fieldData.TotalServiceCharge;
        }
        else
        {  */
            IsFoodAndBevOutlet = true;
        //}
    }
    fieldData.NetSalesAmount +=grosssaleamount;
}
//---------------------------------------------------------------------------
TMallExportPrepareData TSouthBeachMall::PrepareDataForExport(int zKey)
{
    TMallExportPrepareData preparedData;
    try
    {
        UnicodeString fileName;
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();
        bool transactionDoneBeforeZed =  CheckTransactionDoneBeforeZed();

        if(transactionDoneBeforeZed)
        {
           fileName = GetFileName(dbTransaction, zKey)  ;
           preparedData.FileName.insert( std::pair<int,UnicodeString >(1, fileName ));
           PrepareDataForDailySalesFile(dbTransaction,preparedData,1,zKey);
        }//else code run when zed done without any sales
        else
        {
          std::list<TMallExportSalesData> prepareListForDSF;
          TMallExportPrepareData prepareDataForDSF;
          TMallExportSalesData salesData;
          UnicodeString fileName = "";
          UnicodeString MachineID ="";
          UnicodeString dateformat ="";
          UnicodeString TimeFormat="";
          TDateTime Date_Value;
          UnicodeString Date_Value_In_File;
          double Terminalearning = 0.00;
          Date_Value = GetZedsZedDate(zKey,Terminalearning);
          dateformat = Date_Value.FormatString("YYYYMMDD");
          Date_Value_In_File = Date_Value.FormatString("DDMMYYYY");
          TimeFormat= Date_Value.FormatString("hhmmss");
          int terminalNumber;
          GetTerminalSettings(MachineID, terminalNumber);
          fileName = fileName + "CDLO3" + MachineID + "_" + dateformat + "_" + TimeFormat;
          preparedData.FileName.insert( std::pair<int,UnicodeString >(1, fileName ));
          int MaxZed = GetMaxZedKeyInSales();
          int BatchId_Data;
          if(MaxZed!=0)
          {
              bool IsDateSame = CompareZedAndSaleDate(zKey);
              if(!IsDateSame)
              {
                 double terminalearnings =0.00;
                 TDateTime MaxZedDateInMall = GetMallZedDate(MaxZed) ;
                 TDateTime MaxZedDateInZed = GetZedsZedDate(zKey,terminalearnings) ;
                 bool Isfirsttransactionoccured = IsAnyTransactionPerformed();
                 int value  = Dateutils::DaysBetween(MaxZedDateInMall, MaxZedDateInZed);
                 if(value>1 && terminalearnings==0 && !Isfirsttransactionoccured)
                 {
                    for(int i = 0;i<value;i++)
                    {
                      TGlobalSettings::Instance().MallBatchID++;
                    }
                 }
                 else
                 {
                   TGlobalSettings::Instance().MallBatchID++;

                 }
              }
          }
          else
          {
             double TerminalEarning =0.00;
             int MaxZedInZedTable = GetMaxZed_KeyInZed();
             TDateTime MaxZedDate= GetZedsZedDate(MaxZedInZedTable,TerminalEarning) ;
             int SecondMaxZedInZedTable =GetMaxZed_KeyInZed(MaxZedInZedTable);
             TDateTime SecondMaxZedDate= GetZedsZedDate(SecondMaxZedInZedTable,TerminalEarning) ;
             int value  = Dateutils::DaysBetween(MaxZedDate, SecondMaxZedDate);
             bool Isfirsttransactionoccured = IsAnyTransactionPerformed();
             if(Isfirsttransactionoccured)
             {
                TGlobalSettings::Instance().MallBatchID++;
             }
             else
             {
                 for(int i = 0;i<value;i++)
                 {
                    TGlobalSettings::Instance().MallBatchID++;
                 }
             }
          }
          BatchId_Data = TGlobalSettings::Instance().MallBatchID;
          SaveIntVariable(vmBatchIdForSouthBeachMall, TGlobalSettings::Instance().MallBatchID);

         if(CompareZedAndSaleDate(zKey))                                                                                  // Added condition for checking if Zed is perfomed on the same date without doing
            salesData = GetSalesDataQuery(dbTransaction ,MaxZed ,TGlobalSettings::Instance().BatchNo, BatchId_Data);      // any transaction to generate Mall Export file with consolidated data
         else
            salesData = PrepareZeroSalesData(MachineID ,Date_Value_In_File,BatchId_Data);
          prepareListForDSF.push_back(salesData);
          preparedData.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(1, prepareListForDSF ));
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

    return preparedData;
}
//---------------------------------------------------------------------------
IExporterInterface* TSouthBeachMall::CreateExportMedium()
{
    int mallid = TGlobalSettings::Instance().mallInfo.MallId ;
    UnicodeString exportType = GetExportType(mallid)  ;
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
//---------------------------------------------------------------------------
bool TSouthBeachMall::IsItemVatable(TItemMinorComplete *order, TSouthBeachMallField &fieldData,double &VatAmount)
{
    std::vector<BillCalculator::TTaxResult> taxInfomation = order->BillCalcResult.Tax;
    bool isVatable = false;
    double servicechargetax=0.00;

    for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
    {
        switch( itTaxes->TaxType )
        {
            case TTaxType::ttSale:
                 VatAmount = (double)itTaxes->Value;
                 fieldData.GST += (double)itTaxes->Value;
                 isVatable = true;
                 break;
        }
    }
    if (order->BillCalcResult.ServiceCharge.Percentage != 0)
    {
        fieldData.TotalServiceCharge += (double)order->BillCalcResult.ServiceCharge.Value;
        if (order->BillCalcResult.ServiceCharge.TaxPercentage != 0)
        {
           servicechargetax = (double)order->BillCalcResult.ServiceCharge.TaxValue;
           VatAmount = servicechargetax + VatAmount ;
           fieldData.GST += servicechargetax;
           isVatable = true;
        }
    }
    return isVatable;
}
//---------------------------------------------------------------------------
void TSouthBeachMall::InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData,
                                TSouthBeachMallField &fieldData, int arcBillKey)
{
      PushFieldsInToList(dbTransaction, mallExportSalesData, "Machine ID ", "UnicodeString", fieldData.MachineID, 1, arcBillKey);
      PushFieldsInToList(dbTransaction, mallExportSalesData, "Date (ddmmyyyy)", "TDateTime", Now().FormatString("ddmmyyyy"), 2, arcBillKey);
      PushFieldsInToList(dbTransaction, mallExportSalesData, "Batch ID", "int", fieldData.BatchID, 3, arcBillKey);
      PushFieldsInToList(dbTransaction, mallExportSalesData, "Hour", "UnicodeString",fieldData.Hour , 4, arcBillKey);
      PushFieldsInToList(dbTransaction, mallExportSalesData, "Receipt Count", "int", fieldData.ReceiptCount, 5, arcBillKey);
      PushFieldsInToList(dbTransaction, mallExportSalesData, "GTO", "double", fieldData.NetSalesAmount, 6, arcBillKey);
      PushFieldsInToList(dbTransaction, mallExportSalesData, "GST", "double", fieldData.GST, 7, arcBillKey);
      PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Discount", "double", fieldData.TotalDiscount, 8, arcBillKey);
      PushFieldsInToList(dbTransaction, mallExportSalesData, "No Of Pax", "int", fieldData.NoOfPax, 9, arcBillKey);
      PushFieldsInToList(dbTransaction, mallExportSalesData, "Reset Count", "int", fieldData.ResetCount, 10, arcBillKey);
}
//---------------------------------------------------------------------------
UnicodeString TSouthBeachMall::GetFileName(Database::TDBTransaction &dBTransaction, int zKey)
{
     UnicodeString fileName = "";
     try
     {
         UnicodeString MachineID ="";
         UnicodeString dateformat ="";
         UnicodeString TimeFormat="";
         UnicodeString Validatedateformat="";
         PrepareDataforFilename(zKey,MachineID,dateformat,TimeFormat,Validatedateformat) ;
         fileName = fileName + "CDLO3" + MachineID + "_" + dateformat + "_" + TimeFormat;
     }
     catch(Exception &E)
     {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
         throw;
     }
     return fileName;
}
//---------------------------------------------------------------------------
int TSouthBeachMall::GetItemSalesId(Database::TDBTransaction &dbTransaction, int itemKey)
{
    int salesTypeId = 0;
    try
    {
        Database::TcpIBSQL ibInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(ibInternalQuery);

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "SELECT a.SALES_TYPE_ID FROM MALL_SALES_TYPE_ITEMS_RELATION a "
                                     "WHERE A.ITEM_ID = :ITEM_ID ";
        ibInternalQuery->ParamByName("ITEM_ID")->AsInteger = itemKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
            salesTypeId = ibInternalQuery->Fields[0]->AsInteger;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
         throw;
	}
    return salesTypeId;
}
//---------------------------------------------------------------------------
void TSouthBeachMall::PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction,TMallExportPrepareData &prepareDataForDSF,int index,int Z_Key)
{
     std::list<TMallExportSalesData> prepareListForDSF;
     try
     {
          Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
          dBTransaction.RegisterQuery(IBInternalQuery);
          int BatchId_Data ;
          TMallExportSalesData salesData;
          int MaxZed = GetMaxZedKeyInSales();

          int SecondMaxZed =GetMaxZedKeyInSales(MaxZed);
          bool BreakConSolidateDateForCurrentDate = IsConsolidatedOrNot(MaxZed,SecondMaxZed);

          if(!BreakConSolidateDateForCurrentDate)
          {
           TGlobalSettings::Instance().BatchNo += 1;
           BatchId_Data = TGlobalSettings::Instance().MallBatchID;
           UpdateBatchIDForSouthBeach(3,Z_Key,BatchId_Data);

           salesData = GetSalesDataQuery(dBTransaction ,Z_Key ,TGlobalSettings::Instance().BatchNo, BatchId_Data);

           prepareListForDSF.push_back(salesData);
           prepareDataForDSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForDSF ));

          }//below else code is for non consolidate,shop closure outlet

          else
          {
           int BatchNumber;
           UnicodeString TimeValueForFilename;
           TDateTime ZedDateOfSecondMaxZed = GetZedDateForSecondMaxZed(SecondMaxZed,BatchNumber)  ;
           TDateTime Minimumdateformaxzed = GetMinimumDateForMaxZed(MaxZed,TimeValueForFilename)  ;
           TDateTime DateForFile;
           bool Isfirsttransactionoccured = IsAnyTransactionPerformed();
           if(!Isfirsttransactionoccured && SecondMaxZed!=0)
           {
               int value  = Dateutils::DaysBetween(ZedDateOfSecondMaxZed, Minimumdateformaxzed);
               if(value >1)
               {
                    TGlobalSettings::Instance().BatchNo = 1 ;
                    TMallExportPrepareData preparedData;
                    UnicodeString fileName = "";
                    UnicodeString MachineID ="";
                    UnicodeString dateformat ="";
                    UnicodeString TimeFormat="";
                    UnicodeString Date_Value_In_File;
                    int terminalNumber;
                    GetTerminalSettings(MachineID, terminalNumber);
                    bool IsMallClosureExist = false;
                    for(int i = 0;i<value;i++)
                    {
                      DateForFile = ZedDateOfSecondMaxZed +i+1;
                      dateformat = DateForFile.FormatString("YYYYMMDD");
                      UnicodeString Date_Value_In_File = DateForFile.FormatString("DDMMYYYY");
                      fileName = "CDLO3" + MachineID + "_" + dateformat + "_" + TimeValueForFilename;
                      preparedData.FileName.insert( std::pair<int,UnicodeString >(i+1, fileName ));
                      bool IsZeroSaledoneBefore = IsAnyZeroSalesDoneBeforeMaxZed(Z_Key);
                      if(IsZeroSaledoneBefore && i!=value-1)
                      {
                        BatchId_Data = TGlobalSettings::Instance().MallBatchID-value+i+1;
                        IsMallClosureExist = true;
                      }
                      else
                      {
                        if(!IsMallClosureExist)
                        {
                          TGlobalSettings::Instance().MallBatchID++;
                        }
                       BatchId_Data =TGlobalSettings::Instance().MallBatchID;
                      }

                      if(i == value-1)
                      {
                           UpdateBatchIDForSouthBeach(3,Z_Key,BatchId_Data);

                           salesData = GetSalesDataQuery(dBTransaction ,Z_Key ,1, BatchId_Data);

                           prepareListForDSF.push_back(salesData);
                           preparedData.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(i+1, prepareListForDSF ));
                           prepareListForDSF.clear() ;

                          TMallExportTextFile* exporter = (TMallExportTextFile*)CreateExportMedium();
                          exporter->WriteToFile(preparedData);

                      }
                      else
                      {

                          salesData = PrepareZeroSalesData(MachineID,Date_Value_In_File,BatchId_Data);
                          prepareListForDSF.push_back(salesData);
                          preparedData.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(i+1, prepareListForDSF ));
                          prepareListForDSF.clear() ;

                          TMallExportTextFile* exporter = (TMallExportTextFile*)CreateExportMedium();
                          exporter->WriteToFile(preparedData);
                      }

                    }
               }
                 //if value>1 else
               else
               {
                   salesData = PrepareDataForSingleDaySalesFile(dBTransaction ,Z_Key);
                   prepareListForDSF.push_back(salesData);
                   prepareDataForDSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForDSF ));

               }
           } //if--Isfirsttransactionoccured --else
           else
           {
               salesData = PrepareDataForSingleDaySalesFile(dBTransaction ,Z_Key);
               prepareListForDSF.push_back(salesData);
               prepareDataForDSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForDSF ));
           }

          }
          SaveIntVariable(vmBatchNo, TGlobalSettings::Instance().BatchNo);
          SaveIntVariable(vmBatchIdForSouthBeachMall, TGlobalSettings::Instance().MallBatchID);

    }
    catch(Exception &E)
	{
        dBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
	}

}
TMallExportSalesData TSouthBeachMall::GetSalesDataQuery(Database::TDBTransaction &dBTransaction,int Z_Key,int BatchNumber,int BatchId_Data)
{
    UnicodeString salesDataQuery ="";
    UnicodeString Machine_ID="";
    UnicodeString Hour_Data = "";
    UnicodeString Date_Data = "";
    int ReceiptCount_Data=0;
    int NoOfPax_Data=0;
    double GTO_Data=0.00;
    double GST_Data=0.00;
    double Discount_Data=0.00;
    TMallExportSalesData salesData;
   try
   {
     Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
     dBTransaction.RegisterQuery(IBInternalQuery);
     int Zed_Key_File;
     int arcbill_key = GetMinimumArcBillkeyForMaxZed(Z_Key,dBTransaction);

     for(int i = 0;i<BatchNumber;i++)
     {
         GetMaxZedKeyInEachIteration(Zed_Key_File,i) ;
         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text = GetQueryForSalesData();


          IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcbill_key;
          IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 4 ;
          IBInternalQuery->ParamByName("MAX_ZED")->AsInteger = Zed_Key_File;
          IBInternalQuery->ExecQuery();

          for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
          {
            salesData.FieldIndex = IBInternalQuery->Fields[0]->AsInteger;

            switch(salesData.FieldIndex)
            {
            case 1:
                Machine_ID = IBInternalQuery->Fields[2]->AsString;
                break;
            case 2:
                Date_Data = IBInternalQuery->Fields[2]->AsString;
                break;
            case 4:
                 Hour_Data = IBInternalQuery->Fields[2]->AsString;
                break;
            case 5:
                ReceiptCount_Data += IBInternalQuery->Fields[2]->AsInteger;
                break;
            case 6:
                GTO_Data += IBInternalQuery->Fields[2]->AsDouble;
                break;
            case 7:
                GST_Data += IBInternalQuery->Fields[2]->AsDouble;
                break;
            case 8:
                Discount_Data += IBInternalQuery->Fields[2]->AsDouble;
                break;
            case 9:
                NoOfPax_Data += IBInternalQuery->Fields[2]->AsInteger;
                break;
            }

          }
     }
     Discount_Data *=-1;
     UnicodeString DiscountData = FloatToStrF(Discount_Data,ffFixed,15,2);
     salesData.DataValue = Machine_ID + "|" + Date_Data + "|" + BatchId_Data + "|" + Hour_Data + "|" + ReceiptCount_Data + "|" +GTO_Data
                                    + "|" + GST_Data + "|" + DiscountData + "|" + NoOfPax_Data ;

    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
	}

    return salesData;
}


//---------------------------------------------------------------------------
TMallExportSalesData TSouthBeachMall::PrepareZeroSalesData(UnicodeString MachineID , UnicodeString SalesDateValue,int BatchId_Data)
{
    TMallExportSalesData salesData;
    try
    {
      UnicodeString HourData = "0000";
      int ReceiptCountData = 0;
      int NoOfPaxData = 0;
      UnicodeString DiscountValue = "0.00";
      UnicodeString GTO_Data = "0.00";
      UnicodeString GST_Data = "0.00";

       salesData.DataValue = MachineID + "|" + SalesDateValue + "|" + BatchId_Data + "|" + HourData + "|" + ReceiptCountData + "|" +GTO_Data
                                    + "|" + GST_Data + "|" + DiscountValue + "|" + NoOfPaxData ;
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
	}

    return salesData;
}
//---------------------------------------------------------------------------
bool TSouthBeachMall::IsConsolidatedOrNot(int MaxZedKey ,int SecondMaxZed)
{
   bool IsBreakConSolidateDateForCurrentDate = true;
   try
   {
       TDateTime DateValue;
       UnicodeString DateForMaxZed = GetDateValueForMaxZed(MaxZedKey) ;
       UnicodeString DateForSecondMaxZed  = GetDateValueForMaxZed(SecondMaxZed) ;
       if(DateForMaxZed == DateForSecondMaxZed)
       {
            IsBreakConSolidateDateForCurrentDate = false;
       }
   }
   catch(Exception &E)
   {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
   }

   return IsBreakConSolidateDateForCurrentDate;
}
//---------------------------------------------------------------------------
UnicodeString TSouthBeachMall::GetDateValueForMaxZed(int ZedKey)
{
    TDateTime DateValue;
    UnicodeString MaxZedDate = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        query->Close();

        query->SQL->Text =  "SELECT FIRST 1 a.DATE_CREATED "
                            "FROM MALLEXPORT_SALES a "
                            "WHERE a.Z_KEY = :Max_Zed "
                            "ORDER BY a.MALLEXPORT_SALE_KEY ";

        query->ParamByName("Max_Zed")->AsInteger = ZedKey;
        query->ExecQuery();

        DateValue = query->Fields[0]->AsDate;
        MaxZedDate = DateValue.FormatString("mm/dd/yyyy");

        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

    return MaxZedDate;
}
//---------------------------------------------------------------------------
void TSouthBeachMall::SaveIntVariable(vmVariables vmVar, int CompName)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
    if(GlobalProfileKey == 0)
    {
        GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
    }
    TManagerVariable::Instance().SetProfileInt(DBTransaction, GlobalProfileKey, vmVar, CompName);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TSouthBeachMall::UpdateBatchIDForSouthBeach(int fieldindex,int Zedkey,int Batch_No)
{
  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
  DBTransaction.StartTransaction();
  try
  {
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();

    IBInternalQuery->SQL->Text = "UPDATE MALLEXPORT_SALES SET MALLEXPORT_SALES.FIELD_VALUE = :FIELD_VALUE "
                                 "WHERE MALLEXPORT_SALES.Z_KEY = :Z_KEY "
                                 "AND  MALLEXPORT_SALES.FIELD_INDEX = :FIELD_INDEX ";


    IBInternalQuery->ParamByName("Z_KEY")->AsInteger = Zedkey;
    IBInternalQuery->ParamByName("FIELD_INDEX")->AsInteger = fieldindex;
    IBInternalQuery->ParamByName("FIELD_VALUE")->AsInteger = Batch_No;

    IBInternalQuery->ExecQuery();

    DBTransaction.Commit();

  }
  catch(Exception & E)
  {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
  }
}
//---------------------------------------------------------------------------
TDateTime TSouthBeachMall::FetchMinimumDate(int ZedKey)
{
  TDateTime Mall_DateValue;
  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
  DBTransaction.StartTransaction();
  TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
  try
  {
        query->Close();
        query->SQL->Text =  "SELECT MIN(a.DATE_CREATED) "
                            "FROM MALLEXPORT_SALES a "
                            "WHERE a.Z_KEY = :MaxZed ";

        query->ParamByName("MaxZed")->AsInteger = ZedKey;
        query->ExecQuery();

        if(query->RecordCount)
             Mall_DateValue = query->Fields[0]->AsDate;
  }
  catch(Exception & E)
  {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
  }

  return Mall_DateValue;

}
//---------------------------------------------------------------------------
bool TSouthBeachMall::CompareZedAndSaleDate(int ZedTable_ZedKey)
{
    bool SameDateForNonZero = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {

        int MaxZed = GetMaxZedKeyInSales();
        TDateTime Mall_DateValue = FetchMinimumDate(MaxZed);
        UnicodeString MaxMallZedDate = Mall_DateValue.FormatString("mm/dd/yyyy");

        UnicodeString ZedTable_ZedDate = GetMaxDateInZed(ZedTable_ZedKey);

        if(MaxMallZedDate == ZedTable_ZedDate)
        {
          SameDateForNonZero = true;
        }

        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
         throw;
    }

    return SameDateForNonZero;
}
//---------------------------------------------------------------------------
bool TSouthBeachMall::IsAnyZeroSalesDoneBeforeMaxZed(int ZedTable_ZedKey)
{
    bool IsZeroSaledone = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {

        UnicodeString ZedTable_ZedDate = GetMaxDateInZed(ZedTable_ZedKey);

        query->Close();
        query->SQL->Text =  "SELECT FIRST 1 a.TIME_STAMP,a.TERMINAL_EARNINGS "
                            "FROM ZEDS a "
                            "WHERE a.Z_KEY < :ZedTable_ZedKey "
                            "ORDER BY a.Z_KEY DESC ";

        query->ParamByName("ZedTable_ZedKey")->AsInteger = ZedTable_ZedKey;
        query->ExecQuery();
        TDateTime Second_Zed_DateValue = query->Fields[0]->AsDate;
        UnicodeString SecondZedDateValue = Second_Zed_DateValue.FormatString("mm/dd/yyyy");
        double terminalearnings = query->Fields[1]->AsDouble;
        if(SecondZedDateValue == ZedTable_ZedDate && terminalearnings ==0)
        {
          IsZeroSaledone = true;
        }

        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

    return IsZeroSaledone;

}
//---------------------------------------------------------------------------
UnicodeString TSouthBeachMall::GetMaxDateInZed(int Zedkey)
{
    UnicodeString ZedTable_ZedDate = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        query->Close();
        query->SQL->Text =  "SELECT MAX(a.TIME_STAMP) "
                            "FROM ZEDS a "
                            "WHERE a.Z_KEY = :ZedTable_ZedKey ";

        query->ParamByName("ZedTable_ZedKey")->AsInteger = Zedkey;
        query->ExecQuery();

        if(query->RecordCount)
        {
            TDateTime Zed_DateValue = query->Fields[0]->AsDate;

            ZedTable_ZedDate = Zed_DateValue.FormatString("mm/dd/yyyy");
        }

    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

   return ZedTable_ZedDate;
}
bool TSouthBeachMall::IsAnyTransactionPerformed()
{
    bool IsFirstTransaction = true;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        query->Close();
        query->SQL->Text =  "SELECT COUNT(a.Z_KEY) "
                            "FROM ZEDS a ";
        query->ExecQuery();
        int Zed_Count = query->Fields[0]->AsInteger;

        if(Zed_Count>1)
        {
          IsFirstTransaction = false;
        }
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    return IsFirstTransaction;
}
//---------------------------------------------------------------------------
TDateTime TSouthBeachMall::GetDateAtFieldIndex(int ZedKey,int fieldindex)
{
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
   TDateTime DateAtFieldIndex;
   try
   {
        query->Close();
        query->SQL->Text =  "SELECT a.DATE_CREATED "
                            "FROM MALLEXPORT_SALES a "
                            "WHERE a.Z_KEY = :ZED_KEY AND a.FIELD_INDEX = :FIELDINDEX ";

        query->ParamByName("ZED_KEY")->AsInteger = ZedKey;
        query->ParamByName("FIELDINDEX")->AsInteger = fieldindex;
        query->ExecQuery();
        if(query->RecordCount)
            DateAtFieldIndex = query->Fields[0]->AsDate;
   }
   catch(Exception &E)
   {
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
   }
   return DateAtFieldIndex;
}

//---------------------------------------------------------------------------
TDateTime TSouthBeachMall::GetZedDateForSecondMaxZed(int SecondMaxZed,int &BatchNumber)
{
    TDateTime Second_Zed_DateValue ;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        Second_Zed_DateValue = GetDateAtFieldIndex(SecondMaxZed,10) ;

        query->Close();
        query->SQL->Text =  "SELECT a.FIELD_VALUE "
                            "FROM MALLEXPORT_SALES a "
                            "WHERE a.Z_KEY = :ZED_KEY AND a.FIELD_INDEX = :FIELDINDEX ";

        query->ParamByName("ZED_KEY")->AsInteger = SecondMaxZed;
        query->ParamByName("FIELDINDEX")->AsInteger = 3;
        query->ExecQuery();
        if(query->RecordCount)
            BatchNumber = query->Fields[0]->AsInteger;

        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    return Second_Zed_DateValue;
}
//---------------------------------------------------------------------------
TDateTime TSouthBeachMall::GetMinimumDateForMaxZed(int MaxZed,UnicodeString &TimeFormat)
{
    TDateTime ResetCountForMaxZed;
    TDateTime Min_DayeForMaxZed;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        Min_DayeForMaxZed = FetchMinimumDate(MaxZed);
        ResetCountForMaxZed = GetDateAtFieldIndex(MaxZed,10) ;
        TimeFormat = ResetCountForMaxZed.FormatString("hhmmss");
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    return Min_DayeForMaxZed;
}
//---------------------------------------------------------------------------
void TSouthBeachMall::GetMaxZedKeyInEachIteration(int &Zed_Key_File,int index)
{
    try
    {
        if(index)
        {
            Zed_Key_File = GetMaxZedKeyInSales(Zed_Key_File);
        }
        else
             Zed_Key_File = GetMaxZedKeyInSales();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
//---------------------------------------------------------------------------
TDateTime TSouthBeachMall::GetMallZedDate(int Zed_Key)
{
    TDateTime MaxDate;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        query->Close();
        query->SQL->Text =  "SELECT a.DATE_CREATED "
                            "FROM MALLEXPORT_SALES a "
                            "WHERE a.FIELD_INDEX = :FIELD_INDEX And a.Z_KEY = :ZKEY "
                            "GROUP BY 1 ";

        query->ParamByName("ZKEY")->AsInteger = Zed_Key;
        query->ParamByName("FIELD_INDEX")->AsInteger = 10;

        query->ExecQuery();

        if(query->RecordCount)
            MaxDate = query->Fields[0]->AsDate;

        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

    return MaxDate;
}
//---------------------------------------------------------------------------
TDateTime TSouthBeachMall::GetZedsZedDate(int Zed_Key,double &terminalearnings)
{
    TDateTime MaxDate;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        query->Close();
        query->SQL->Text =  "SELECT a.TIME_STAMP ,a.TERMINAL_EARNINGS "
                            "FROM ZEDS a "
                            "WHERE a.Z_KEY = :Z_KEY ";

        query->ParamByName("Z_KEY")->AsInteger = Zed_Key;
        query->ExecQuery();
        MaxDate = query->Fields[0]->AsDate;
        terminalearnings = query->Fields[1]->AsDouble;
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

    return MaxDate;
}
//---------------------------------------------------------------------------
//Here code start for regenerate mall
void TSouthBeachMall::RegenerateMallReport(TDateTime sDate, TDateTime eDate)
{
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {
        GetListOfDatesBetwSdateEndDate(sDate,eDate);
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
	}
}
void TSouthBeachMall::GetListOfDatesBetwSdateEndDate(TDateTime Startdate, TDateTime EndDate)
{
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    TDateTime CompareDateMallClosureOutlet;
    ResetVariablesForConsolidatedData();
    try
    {
       TIBSQL* query = dbTransaction.Query(dbTransaction.AddQuery());
       UnicodeString DateValue = "";

       query->Close();

              query->SQL->Text = "SELECT CAST(a.DATE_CREATED AS date)DATE_CREATED "
                                 "FROM MALLEXPORT_SALES a "
                                 "WHERE extract(DAY from (a.DATE_CREATED)) >= :day_startday "
                                 "AND extract(MONTH from (a.DATE_CREATED)) >= :month_startday "
                                 "AND extract(YEAR from (a.DATE_CREATED)) >= :year_startday "
                                 "And extract(DAY from (a.DATE_CREATED)) <= :day_endDay "
                                 "AND extract(MONTH from (a.DATE_CREATED)) <= :month_endDay "
                                 "AND extract(YEAR from (a.DATE_CREATED)) <= :year_endyear "
                                 "GROUP BY 1 ";


              query->ParamByName("day_startday")->AsString = Startdate.FormatString("dd");
              query->ParamByName("month_startday")->AsString =Startdate.FormatString("mm");
              query->ParamByName("year_startday")->AsString =Startdate.FormatString("yyyy");
              query->ParamByName("day_endDay")->AsString =EndDate.FormatString("dd");
              query->ParamByName("month_endDay")->AsString =EndDate.FormatString("mm");
              query->ParamByName("year_endyear")->AsString =EndDate.FormatString("yyyy");
              query->ExecQuery();
              if(query->RecordCount == 0)
              {
                MessageBox( "Generating Export File for dates the POS was not used is not Allowed!", "Cannot Generate File", MB_OK );
                return;
              }
              int Count = 0;

              for( ; !query->Eof; query->Next())
              {
                Count ++;
                TDateTime tempdatevalue = query->Fields[0]->AsDate;
                if(Count == 1)
                {
                   CompareDateMallClosureOutlet = tempdatevalue;
                }
                else
                {
                   int value  = Dateutils::DaysBetween(CompareDateMallClosureOutlet, tempdatevalue);
                   if(value>1)
                   {

                      UnicodeString TimeFormat_ShopClosure = GetTimeForMallOutletClosureFile(tempdatevalue);
                      for(int i = 1;i<value;i++)
                      {
                          int BatchId = BatchId_Data +i;
                          TDateTime tempdatevalueforfile = CompareDateMallClosureOutlet+i;
                          UnicodeString DateValueForFilename = tempdatevalueforfile.FormatString("YYYYMMDD");
                          UnicodeString DateValueInFilename = tempdatevalueforfile.FormatString("DDMMYYYY");
                          std::list<TMallExportSalesData> prepareListForDSF;
                          TMallExportPrepareData preparedData;
                          TMallExportSalesData salesData;
                          UnicodeString fileName = "";

                          fileName = fileName + "CDLO3" + Machine_ID + "_" + DateValueForFilename + "_" + TimeFormat_ShopClosure;
                          preparedData.FileName.insert( std::pair<int,UnicodeString >(1, fileName ));


                          salesData = PrepareZeroSalesData( Machine_ID , DateValueInFilename, BatchId) ;

                          prepareListForDSF.push_back(salesData);
                          preparedData.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(1, prepareListForDSF ));
                          prepareListForDSF.clear();
                          TMallExportTextFile* exporter = (TMallExportTextFile*)CreateExportMedium();
                          exporter->WriteToFile(preparedData);

                      }
                      Machine_ID = "";
                      BatchId_Data = 0;
                   }
                   CompareDateMallClosureOutlet = tempdatevalue;
                }
                DateValue = tempdatevalue.FormatString("mm/dd/yyyy");
                GetTotalZedCorrespondingDate(Startdate,EndDate,DateValue,tempdatevalue);
                CheckNoSaleDataForDateValue(tempdatevalue)  ;
                ResetVariablesForConsolidatedData();

              }
               if(query->RecordCount != 0)
               {
                 MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );
               }
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;

	}
    dbTransaction.Commit();
}
//------------------------------------------------------------------------------------------------
void TSouthBeachMall::GetTotalZedCorrespondingDate(TDateTime Startdate,TDateTime EndDate,UnicodeString Datevalue,TDateTime DateValueFormat)
{
  Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
  TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
  dbTransaction.StartTransaction();
  try
  {
       TIBSQL* query = dbTransaction.Query(dbTransaction.AddQuery());
       UnicodeString Day_StartDate = Startdate.FormatString("dd");
       UnicodeString Month_StartDate = Startdate.FormatString("mm");
       UnicodeString Year_StartDate = Startdate.FormatString("yyyy");
       UnicodeString Day_EndDate = EndDate.FormatString("dd");
       UnicodeString Month_EndDate = EndDate.FormatString("mm");
       UnicodeString Year_EndDate = EndDate.FormatString("yyyy");
       query->Close();
              query->SQL->Text = "SELECT a.Z_KEY "
                                 "FROM MALLEXPORT_SALES a "
                                 "WHERE extract(DAY from (a.DATE_CREATED)) >= :day_startday "
                                 "AND extract(MONTH from (a.DATE_CREATED)) >= :month_startday "
                                 "AND extract(YEAR from (a.DATE_CREATED)) >= :year_startday "
                                 "And extract(DAY from (a.DATE_CREATED)) <= :day_endDay "
                                 "AND extract(MONTH from (a.DATE_CREATED)) <= :month_endDay "
                                 "AND extract(YEAR from (a.DATE_CREATED)) <= :year_endyear "
                                 "GROUP BY 1 ";


               query->ParamByName("day_startday")->AsString = Day_StartDate;
               query->ParamByName("month_startday")->AsString =Month_StartDate;
               query->ParamByName("year_startday")->AsString =Year_StartDate;
               query->ParamByName("day_endDay")->AsString =Day_EndDate;
               query->ParamByName("month_endDay")->AsString =Month_EndDate;
               query->ParamByName("year_endyear")->AsString =Year_EndDate;
               query->ExecQuery();

        for( ; !query->Eof; query->Next())
        {
           int Zedkey = query->Fields[0]->AsInteger;
           CheckFirstSaleOfEachZed(Zedkey,Startdate,EndDate,Datevalue) ;
        }
  }
  catch(Exception &E)
  {
    dbTransaction.Rollback();
    TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    throw;

  }
}
//------------------------------------------------------------------------------------------------
void TSouthBeachMall::CheckFirstSaleOfEachZed(int Zedkey ,TDateTime Startdate,TDateTime EndDate, UnicodeString datevalue)
{
  TMallExportPrepareData preparedData;
  UnicodeString fileName = "";
  try
  {
        UnicodeString MachineID ="";
        UnicodeString dateformat ="";
        UnicodeString TimeFormat="";
        UnicodeString Validatedateformat ="";
        PrepareDataforFilename(Zedkey,MachineID,dateformat,TimeFormat,Validatedateformat) ;

        if(Validatedateformat == datevalue )
        {
          fileName = fileName + "CDLO3" + MachineID + "_" + dateformat + "_" + TimeFormat;

          preparedData.FileName.insert( std::pair<int,UnicodeString >(1, fileName ));
          PrepareDataForRegenerateFile(preparedData,Zedkey,datevalue);
          TMallExportTextFile* exporter = (TMallExportTextFile*)CreateExportMedium();
          exporter->WriteToFile(preparedData);
        }
        else
        {
           return;
        }

  }
  catch(Exception &E)
  {
    TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    throw;
  }
}
//------------------------------------------------------------------------------------------------
void TSouthBeachMall::PrepareDataForRegenerateFile(TMallExportPrepareData &prepareDataForDSF,int Zedkey,UnicodeString DateForFile)
{
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    std::list<TMallExportSalesData> prepareListForDSF;
    try
    {
       TIBSQL* IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
       TMallExportSalesData salesData;
       int arcbill_key = GetMinimumArcBillkeyForMaxZed(Zedkey,dbTransaction);

       IBInternalQuery->Close();
       IBInternalQuery->SQL->Text = GetQueryForSalesData();
        IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcbill_key;
        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 4 ;
        IBInternalQuery->ParamByName("MAX_ZED")->AsInteger = Zedkey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            salesData.FieldIndex = IBInternalQuery->Fields[0]->AsInteger;

            switch(salesData.FieldIndex)
            {
            case 1:
                Machine_ID = IBInternalQuery->Fields[2]->AsString;
                break;
            case 2:
                Date_Data = IBInternalQuery->Fields[2]->AsString;
                break;
            case 3:
                BatchId_Data = IBInternalQuery->Fields[2]->AsInteger;
                break;
            case 4:
                 Hour_Data = IBInternalQuery->Fields[2]->AsString;
                break;
            case 5:
                ReceiptCount_Data += IBInternalQuery->Fields[2]->AsInteger;
                break;
            case 6:
                GTO_Data += IBInternalQuery->Fields[2]->AsDouble;
                break;
            case 7:
                GST_Data += IBInternalQuery->Fields[2]->AsDouble;
                break;
            case 8:
                Discount_Data += IBInternalQuery->Fields[2]->AsDouble;
                break;
            case 9:
                NoOfPax_Data += IBInternalQuery->Fields[2]->AsInteger;
                break;
            }

         }
         Discount_Data *=-1;
         UnicodeString DiscountData = FloatToStrF(Discount_Data,ffFixed,15,2);

         salesData.DataValue = Machine_ID + "|" + Date_Data + "|" + BatchId_Data + "|" + Hour_Data + "|" + ReceiptCount_Data + "|" +GTO_Data
                                + "|" + GST_Data + "|" + DiscountData + "|" + NoOfPax_Data ;
       prepareListForDSF.push_back(salesData);
       prepareDataForDSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(1, prepareListForDSF ));
    }
    catch(Exception &E)
    {
        dbTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);

    }
}
//------------------------------------------------------------------------------------------------
void TSouthBeachMall::ResetVariablesForConsolidatedData()
{
    Hour_Data = "";
    Date_Data="" ;
    ReceiptCount_Data = 0;
    NoOfPax_Data = 0;
    GTO_Data = 0.00;
    GST_Data= 0.00;
    Discount_Data = 0.00;
}
//------------------------------------------------------------------------------------------------
int TSouthBeachMall::GetMinimumArcBillkeyForMaxZed(int ZedKey,Database::TDBTransaction &dBTransaction)
{
  int arcbillkey =0;
  try
  {
       Database::TcpIBSQL ibInternalQuery(new TIBSQL(NULL));
       dBTransaction.RegisterQuery(ibInternalQuery);

       ibInternalQuery->Close();
       ibInternalQuery->SQL->Text = "SELECT MIN(a.ARCBILL_KEY)ARCBILL_KEY "
                                    "FROM MALLEXPORT_SALES a "
                                    "WHERE a.Z_KEY =:Z_KEY ";

       ibInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
       ibInternalQuery->ExecQuery();

       if(ibInternalQuery->RecordCount)
            arcbillkey = ibInternalQuery->Fields[0]->AsInteger;

  }
  catch(Exception &E)
  {
        dBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
  }

  return arcbillkey;
}

//------------------------------------------------------------------------------------------------
void TSouthBeachMall::CheckNoSaleDataForDateValue(TDateTime DateValue)
{
   Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
   dbTransaction.StartTransaction();

  try
  {
       TIBSQL* ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

       ibInternalQuery->Close();
       ibInternalQuery->SQL->Text = "SELECT a.TIME_STAMP "
                                    "FROM ZEDS a "
                                    "WHERE extract(DAY from (a.TIME_STAMP)) = :day_Value "
                                    "AND extract(MONTH from (a.TIME_STAMP)) = :month_Value "
                                    "AND extract(YEAR from (a.TIME_STAMP)) = :year_Value "
                                    "AND a.TERMINAL_EARNINGS = :terminal_earnings ";

       ibInternalQuery->ParamByName("day_Value")->AsString = DateValue.FormatString("dd");
       ibInternalQuery->ParamByName("month_Value")->AsString =DateValue.FormatString("mm");
       ibInternalQuery->ParamByName("year_Value")->AsString = DateValue.FormatString("yyyy");
       ibInternalQuery->ParamByName("terminal_earnings")->AsInteger = 0;

       ibInternalQuery->ExecQuery();

       for ( ; !ibInternalQuery->Eof; ibInternalQuery->Next())
       {
         TDateTime QueryDate = ibInternalQuery->Fields[0]->AsDate;
         UnicodeString Date_Value_For_Filename = QueryDate.FormatString("YYYYMMDD") ;
         UnicodeString TimeFormat_Nosales = QueryDate.FormatString("hhmmss") ;
         UnicodeString Date_Value_In_File = QueryDate.FormatString("DDMMYYYY") ;

          std::list<TMallExportSalesData> prepareListForDSF;
          TMallExportPrepareData preparedData;
          TMallExportSalesData salesData;
          UnicodeString fileName = "";
          UnicodeString dateformat ="";

          fileName = fileName + "CDLO3" + Machine_ID + "_" + Date_Value_For_Filename + "_" + TimeFormat_Nosales;
          preparedData.FileName.insert( std::pair<int,UnicodeString >(1, fileName ));

          salesData = PrepareZeroSalesData(Machine_ID , Date_Value_In_File, BatchId_Data);

          prepareListForDSF.push_back(salesData);
          preparedData.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(1, prepareListForDSF ));
          prepareListForDSF.clear();
          TMallExportTextFile* exporter = (TMallExportTextFile*)CreateExportMedium();
          exporter->WriteToFile(preparedData);
       }
  }
  catch(Exception &E)
  {
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
  }

}
//------------------------------------------------------------------------------------------------
UnicodeString TSouthBeachMall::GetTimeForMallOutletClosureFile(TDateTime DateValue)
{
   Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
   dbTransaction.StartTransaction();
   UnicodeString TimeFormat_ShopClosureFile = "";
  try
  {
       TIBSQL* ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

       ibInternalQuery->Close();
       ibInternalQuery->SQL->Text = "SELECT a.DATE_CREATED "
                                    "FROM MALLEXPORT_SALES a "
                                    "WHERE extract(DAY from (a.DATE_CREATED)) = :day_Value "
                                    "AND extract(MONTH from (a.DATE_CREATED)) = :month_Value "
                                    "AND extract(YEAR from (a.DATE_CREATED)) = :year_Value "
                                    "AND a.FIELD_INDEX = :field_index "
                                    "GROUP BY 1 ";

       ibInternalQuery->ParamByName("day_Value")->AsString = DateValue.FormatString("dd");
       ibInternalQuery->ParamByName("month_Value")->AsString =DateValue.FormatString("mm");
       ibInternalQuery->ParamByName("year_Value")->AsString = DateValue.FormatString("yyyy");
       ibInternalQuery->ParamByName("field_index")->AsInteger = 10;

       ibInternalQuery->ExecQuery();

       for ( ; !ibInternalQuery->Eof; ibInternalQuery->Next())
       {
         TDateTime QueryDate = ibInternalQuery->Fields[0]->AsDate;
         TimeFormat_ShopClosureFile = QueryDate.FormatString("hhmmss");
       }
  }
  catch(Exception &E)
  {
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
  }

  return TimeFormat_ShopClosureFile;
}

void TSouthBeachMall::PrepareDataforFilename(int Zedkey,UnicodeString &MachineID ,UnicodeString &dateformat ,UnicodeString &TimeFormat,UnicodeString &Validatedateformat)
{
   Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
   dbTransaction.StartTransaction();
  try
  {
       UnicodeString filenamekeyselect = "1,10 " ;
       TIBSQL* IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
       IBInternalQuery->Close();
       IBInternalQuery->SQL->Text = "SELECT a.FIELD_INDEX, a.FIELD, a.FIELD_VALUE, a.VALUE_TYPE,MIN(a.DATE_CREATED) "
                                    "FROM MALLEXPORT_SALES a "
                                   "WHERE a.FIELD_INDEX IN(" + filenamekeyselect + " ) AND a.MALL_KEY = :MALL_KEY AND a.Z_KEY = :Z_KEY "
                                   "GROUP BY 1,2,3,4 ";

       IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 4;

       IBInternalQuery->ParamByName("Z_KEY")->AsInteger = Zedkey;
       IBInternalQuery->ExecQuery();

        for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            int index = IBInternalQuery->FieldByName("FIELD_INDEX")->AsInteger;
            if(index == 1)
            {
              MachineID = IBInternalQuery->Fields[2]->AsString;
            }
            else if(index == 10)
            {   TDateTime date = IBInternalQuery->Fields[4]->AsDate;
                dateformat= date.FormatString("YYYYMMDD");                           // Added the Code here to capture the Date on which Zed is performed
                Validatedateformat = date.FormatString("mm/dd/yyyy");
                TDateTime ZedTime = IBInternalQuery->Fields[4]->AsDate;
                TimeFormat= ZedTime.FormatString("hhmmss");
            }
        }
  }
  catch(Exception &E)
  {
    dbTransaction.Rollback();
    TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    throw;

  }

}
//-----------------------------------------------------------------------------------------------------------
int TSouthBeachMall::GetPatronCount(TPaymentTransaction &paymentTransaction)
{
    int totalPatronCount = 0;
    std::vector <TPatronType> ::iterator ptrPatronTypes;
    for (ptrPatronTypes = paymentTransaction.Patrons.begin(); ptrPatronTypes != paymentTransaction.Patrons.end(); ptrPatronTypes++)
    {
        totalPatronCount += ptrPatronTypes->Count;

    }
    return paymentTransaction.IsPatronAdded == true ? totalPatronCount : 0;
}
//------------------------------------------------------------------------------------------------------------
TMallExportSalesData TSouthBeachMall::PrepareDataForSingleDaySalesFile(Database::TDBTransaction &dBTransaction,int Z_Key)
{
    int BatchId_Data;
    TMallExportSalesData salesData;
    TGlobalSettings::Instance().BatchNo = 1 ;
    bool Isfirsttransactionoccured = IsAnyTransactionPerformed();
    bool isanyzerosaleperformed = IsAnyZeroSalesDoneBeforeMaxZed(Z_Key);

    if(Isfirsttransactionoccured || !isanyzerosaleperformed )
    {
       TGlobalSettings::Instance().MallBatchID++;
    }
    BatchId_Data = TGlobalSettings::Instance().MallBatchID;
    UpdateBatchIDForSouthBeach(3,Z_Key,BatchId_Data);

    salesData = GetSalesDataQuery(dBTransaction ,Z_Key ,1, BatchId_Data);

    return  salesData;
}

UnicodeString TSouthBeachMall::GetQueryForSalesData()
{
     UnicodeString indexKeysList1 = "1,2,3,4 ";
     UnicodeString indexKeysList2 = "6,7,8 ";
     UnicodeString indexKeysList3 = "5,9 ";
     UnicodeString queryForSalesData =

          "SELECT  DailySaleFileData.FIELD_INDEX, "
          "DailySaleFileData.FIELD, DailySaleFileData.FIELD_VALUE , DailySaleFileData.VALUE_TYPE,DailySaleFileData.Z_KEY "
          "FROM "

            "(SELECT  DailyData.FIELD_INDEX, "
            "DailyData.FIELD, DailyData.FIELD_VALUE , DailyData.VALUE_TYPE,DailyData.Z_KEY "
            "FROM "
            "(SELECT a.FIELD_INDEX, a.FIELD, a.FIELD_VALUE , a.VALUE_TYPE ,a.Z_KEY "
            "FROM MALLEXPORT_SALES a "
            "WHERE a.FIELD_INDEX IN(" + indexKeysList1 + ") AND a.MALL_KEY = :MALL_KEY AND A.Z_KEY = :MAX_ZED And a.ARCBILL_KEY = :ARCBILL_KEY "
            "ORDER BY a.ARCBILL_KEY ,a.FIELD_INDEX)DailyData "

             "UNION ALL "

            "SELECT  DailyData.FIELD_INDEX, "
            "DailyData.FIELD, CAST(SUM(DailyData.FIELD_VALUE) AS NUMERIC(17,2) ) FIELD_VALUE , DailyData.VALUE_TYPE,DailyData.Z_KEY "
            "FROM "
            "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE,a.Z_KEY "
            "FROM MALLEXPORT_SALES a "
            "WHERE a.FIELD_INDEX IN(" + indexKeysList2 + ") AND a.MALL_KEY = :MALL_KEY AND A.Z_KEY = :MAX_ZED "
            "ORDER BY A.FIELD_INDEX ASC )DailyData "
            "GROUP BY 1,2,4 ,5 "

            "UNION ALL "

            "SELECT  DailyData.FIELD_INDEX, "
            "DailyData.FIELD, CAST(SUM(DailyData.FIELD_VALUE) AS INT) FIELD_VALUE , DailyData.VALUE_TYPE,DailyData.Z_KEY "
            "FROM "
            "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS INT) FIELD_VALUE, a.VALUE_TYPE,a.Z_KEY "
            "FROM MALLEXPORT_SALES a "
            "WHERE a.FIELD_INDEX IN(" + indexKeysList3 + ") AND a.MALL_KEY = :MALL_KEY AND A.Z_KEY = :MAX_ZED "
            "ORDER BY A.FIELD_INDEX ASC )DailyData "

            "GROUP BY 1,2,4,5 )DailySaleFileData "
            "ORDER BY DailySaleFileData.FIELD_INDEX ";

     return  queryForSalesData;
}



