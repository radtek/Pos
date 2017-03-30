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
void TDeanAndDelucaMallField::SetSalesType(int salesType)
{
    _salesType = salesType;
}
//------------------------------------------------------------------------------------------
void TDeanAndDelucaMallField::SetNetSaleAmountPerSalesType(double netSalesAmountPerSalesType)
{
    _netSalesAmountPerSalesType = netSalesAmountPerSalesType;
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
        TDeanAndDelucaMallField fieldData;
        int terminalNumber;
        UnicodeString tenantCode;
        Currency taxRate = 0.00;
        std::list<TMallExportSettings>::iterator it;

        for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
        {
            if(it->ControlName == "edMallTenantNo")
            {
                fieldData.TenantCode = it->Value;
            }
            else if(it->ControlName == "edMallTerminalNo")
            {
                fieldData.TerminalNumber = StrToInt(it->Value);
            }
        }

        for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
        {
                TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

                //this will call all taxes and discount calculation inside it
            //    PrepareItem(paymentTransaction.DBTransaction, Order, fieldData);

                //For SubOrder
                for (int i = 0; i < Order->SubOrders->Count; i++)
				{
					TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];

                    //this will call all taxes and discount calculation inside it
                //    PrepareItem(paymentTransaction.DBTransaction, CurrentSubOrder, fieldData);
                }
        }

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
                    fieldData.TotalGCSales += (double)SubPayment->GetPayTendered();
                }
                else if(payTypeName == cardType)
                {
                     fieldData.TotalChargedSales += (double)SubPayment->GetPayTendered();
                }
                else
                {
                    fieldData.TotalCashSales +=  (double)SubPayment->GetPayTendered();
                }
            }
        }

        fieldData.TotalNetSaleAmount =  fieldData.TotalGCSales + fieldData.TotalChargedSales + fieldData.TotalCashSales;
        fieldData.OldAccSalesTotal = GetOldAccumulatedSales(paymentTransaction.DBTransaction, 5);
        fieldData.NewAccSalesTotal = fieldData.OldAccSalesTotal + fieldData.TotalNetSaleAmount;
        fieldData.GrossSaleAmount = fieldData.TotalSCDAndPWDAmount + fieldData.TotalOtherDiscount + fieldData.TotalCashSales + fieldData.TotalChargedSales
                                        + fieldData.TotalGCSales;
        //fieldData.NonTaxableSaleAmount todo
        fieldData.TotalRefundAmount = paymentTransaction.Money.FinalPrice > 0 ? 0 : paymentTransaction.Money.FinalPrice;
       /// fieldData.TotalTax = //todo;
        fieldData.ZKey = 0;
        fieldData.SalesCount = (fieldData.TotalRefundAmount > 0 ? 0 : 1);


        fieldData.SalesType = 1; //todo



        fieldData.CustomerCount = GetPatronCount(paymentTransaction);

        //call For inserting into list
        InsertFieldInToList(paymentTransaction.DBTransaction, mallExportSalesData, fieldData, arcBillKey);

        salesWrapper.SalesData = mallExportSalesData;
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
                                TDeanAndDelucaMallField fieldData, int arcBillKey)
{
    ///Call Push Field into List Method for pushing every field into DB
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
void TDeanAndDelucaMall::PrepareItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *order, TDeanAndDelucaMallField &fieldData)
{
    //Create Taxes Object to collect all taxes details
    TDeanAndDelucaTaxes taxes;

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
       fieldData.GrossSaleAmount += grossAmount;
    }
}
//------------------------------------------------------------------------------------------------------------------
TDeanAndDelucaDiscount TDeanAndDelucaMall::PrepareDiscounts(Database::TDBTransaction &dbTransaction, TItemMinorComplete *Order)
{
    TDeanAndDelucaDiscount discount;
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
            else
            {
                discount.otherDiscount +=  (double)Order->DiscountValue_BillCalc(ptrDiscounts);
            }
        }
        else
        {
            discount.otherDiscount +=  (double)Order->DiscountValue_BillCalc(ptrDiscounts);
        }
    }
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


