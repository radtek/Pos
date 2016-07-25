//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportHourlyUpdate.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// PUBLIC SECTION
//---------------------------------------------------------------------------
TMallExportHourlyUpdate::TMallExportHourlyUpdate()
{
    globalSettings = &TGlobalSettings::Instance();

    flatternedOrdersList.clear();
}

TMallExportHourlyUpdate::~TMallExportHourlyUpdate()
{
    //todo: cleanup if needed
}

TExportUpdateResponse TMallExportHourlyUpdate::UpdateHourlyExportTablesOnTransaction( TPaymentTransaction *payTransac )
{
    TExportUpdateResponse response;

    try
    {
        // clear the values to default
        clearHourlyExportFieldValues();

        // set current payment transaction
        setPaymentTransaction( payTransac );

        // set database transaction from payment transaction
        setDbTransaction( &payTransac->DBTransaction );

        // gather all the required information from the transaction
        setHourlyExportFieldValuesOnTransaction();

        // save to database mall export hourly
        Commit();
    }
    catch( Exception &ex )
    {
        response.Successful = false;
        response.Message = ex.Message;
        response.Description = "Failed to update mall export hourly table on transaction";
    }

    return response;
}

TExportUpdateResponse TMallExportHourlyUpdate::ResetHourlyExportTablesOnZed()
{
    TExportUpdateResponse response;

    try
    {
        // clear the values to default
        clearHourlyExportFieldValues();

        // delete all rows on the mall export hourly table
        deleteHourlyExportContentsOnZed();

    }
    catch( Exception &ex )
    {
        response.Successful = false;
        response.Message = ex.Message;
        response.Description = "Failed to delete mall export hourly table contents on zed";
    }

    return response;
}

//---------------------------------------------------------------------------
// PRIVATE SECTION
//---------------------------------------------------------------------------

void TMallExportHourlyUpdate::setPaymentTransaction( TPaymentTransaction *payTransac )
{
    paymentTransaction = payTransac;
    flatternOrdersList();   // flatterns the entire set of orders into one list, so the calculations are easier to make
}
//---------------------------------------------------------------------------

void TMallExportHourlyUpdate::setDbTransaction( Database::TDBTransaction *transaction )
{
    dbTransaction = transaction;
}
//---------------------------------------------------------------------------

void TMallExportHourlyUpdate::clearHourlyExportFieldValues()
{
    totalGrossSales = 0;
    totalSalesTax = 0;
    totalServiceCharge = 0;
    totalServiceChargeTax = 0;
    localTaxes = 0;
    totalDiscount = 0;
    servchargeAmount = 0;
    refundAmount = 0;
    voidAmount = 0;
    scdiscountAmount = 0;

    terminalNumber = "";
    mallCode = "";
    dateValue = "";
    timeValue = "";
    salesValue = 0;
    transactionCount = 0;
    refundCount = 0;
    voidCount = 0;
    patronCount = 0;
}
//---------------------------------------------------------------------------

void TMallExportHourlyUpdate::setHourlyExportFieldValuesOnTransaction()
{
    totalGrossSales              = extractTotalGrossSales();
    totalSalesTax                = extractTotalSalesTax();
    totalServiceCharge           = extractTotalServiceCharge();
    totalServiceChargeTax        = extractTotalServiceChargeTax();
    localTaxes                   = extractLocalTaxes();
    totalDiscount                = extractTotalDiscount();
    servchargeAmount             = extractServiceChargeAmount();
    refundAmount                 = extractRefundAmount();
    voidAmount                   = extractVoidAmount();
    scdiscountAmount             = extractSCDiscountAmount();

    terminalNumber               = extractTerminalNumber();
    mallCode                     = extractMallCode();
    dateValue                    = extractDate();
    timeValue                    = extractTime();
    salesValue                   = extractSales();
    tenantId                     = extractTenantId();
    transactionCount             = extractTransactionCount();
    refundCount                  = extractRefundCount();
    voidCount                    = extractVoidCount();
    patronCount                  = extractPatronCount();
}
//---------------------------------------------------------------------------

void TMallExportHourlyUpdate::Commit()
{
    AnsiString Retval = "";

	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *Query = Transaction.Query(Transaction.AddQuery());

    Query->SQL->Text = "SELECT GEN_ID(GEN_MALLEXPORT_HOURLY_KEY, 1) FROM RDB$DATABASE";
	Transaction.StartTransaction();
	Query->ExecQuery();
	Retval = Query->Fields[0]->AsInteger;
	Transaction.Commit();
	Query->Close();

    Query->SQL->Text = "INSERT INTO MALLEXPORT_HOURLY (" "ME_HOURLY_KEY, " "TERMINAL_NAME, "
                       "TENANT_NAME, " "DATE_VALUE, " "TIME_VALUE, " "AMOUNT_VALUE, " "TRANSACTION_COUNT, "
                       "VAT_SALES, " "TOTALDISCOUNT, " "SCHARGE_AMOUNT, " "REFUND_COUNT, " "REFUND_AMOUNT, "
                       "VOID_COUNT, " "VOID_AMOUNT, " "SCDISCOUNT_AMOUNT, " "MALLCODE, " "PATRON_COUNT ) "
                       "" "VALUES ("
                       ":ME_HOURLY_KEY, " ":TERMINAL_NAME, " ":TENANT_NAME, " ":DATE_VALUE, " ":TIME_VALUE, "
                       ":AMOUNT_VALUE, " ":TRANSACTION_COUNT, "
                       ":VAT_SALES, " ":TOTALDISCOUNT, " ":SCHARGE_AMOUNT, " ":REFUND_COUNT, " ":REFUND_AMOUNT, "
                       ":VOID_COUNT, " ":VOID_AMOUNT, " ":SCDISCOUNT_AMOUNT, " ":MALLCODE, " ":PATRON_COUNT ) ";
	Transaction.StartTransaction();
    Query->ParamByName("ME_HOURLY_KEY")->AsString = Retval;
    Query->ParamByName("TERMINAL_NAME")->AsString = terminalNumber;
    Query->ParamByName("TENANT_NAME")->AsString = mallCode;
    Query->ParamByName("DATE_VALUE")->AsString = dateValue;
    Query->ParamByName("TIME_VALUE")->AsString = timeValue;
    Query->ParamByName("AMOUNT_VALUE")->AsCurrency = salesValue;
    Query->ParamByName("TRANSACTION_COUNT")->AsInteger = transactionCount;
    Query->ParamByName("VAT_SALES")->AsCurrency = totalSalesTax;
    Query->ParamByName("TOTALDISCOUNT")->AsCurrency = totalDiscount;
    Query->ParamByName("SCHARGE_AMOUNT")->AsCurrency = servchargeAmount;
    Query->ParamByName("REFUND_COUNT")->AsInteger = refundCount;
    Query->ParamByName("REFUND_AMOUNT")->AsCurrency = refundAmount;
    Query->ParamByName("VOID_COUNT")->AsInteger = voidCount;
    Query->ParamByName("VOID_AMOUNT")->AsCurrency = voidAmount;
    Query->ParamByName("SCDISCOUNT_AMOUNT")->AsCurrency = scdiscountAmount;
    Query->ParamByName("MALLCODE")->AsString = tenantId;
    Query->ParamByName("PATRON_COUNT")->AsString = patronCount;
	Query->ExecQuery();

	Transaction.Commit();
	Query->Close();
}
//---------------------------------------------------------------------------

void TMallExportHourlyUpdate::deleteHourlyExportContentsOnZed()
{
	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *Query = Transaction.Query(Transaction.AddQuery());

    Query->SQL->Text = "DELETE FROM MALLEXPORT_HOURLY";
	Transaction.StartTransaction();
	Query->ExecQuery();
	Transaction.Commit();
	Query->Close();
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractTotalGrossSales()
{
    Currency result = 0;
    Currency grossPrice = 0;
    Currency SalesTax = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        grossPrice = 0;
        SalesTax = extractTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );

        if(TGlobalSettings::Instance().MallIndex == POWERPLANTMALL)
        {
            if(SalesTax != 0) {
                grossPrice = order->BillCalcResult.GrossPrice;
            }
            grossPrice += order->BillCalcResult.ServiceCharge.Value;
            grossPrice += order->BillCalcResult.ServiceCharge.TaxValue;
        }
        else
        {
            grossPrice = order->BillCalcResult.FinalPrice;
        }
    }

    return grossPrice;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractTotalSalesTax()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        result += extractTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );
    }

    result = result + extractLocalTaxes();

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL || TGlobalSettings::Instance().MallIndex == SHANGRILAMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractTotalServiceCharge()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        result += order->BillCalcResult.ServiceCharge.Value;
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL || TGlobalSettings::Instance().MallIndex == SHANGRILAMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractTotalServiceChargeTax()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        result += order->BillCalcResult.ServiceCharge.TaxValue;
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractLocalTaxes()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        result += extractTotalTaxFromResult( order->BillCalcResult, TTaxType::ttLocal );
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

AnsiString TMallExportHourlyUpdate::extractTerminalNumber()
{
    return TGlobalSettings::Instance().TerminalNo;
}
//---------------------------------------------------------------------------

AnsiString TMallExportHourlyUpdate::extractMallCode()
{
    return AnsiString(globalSettings->BranchCode);
}
//---------------------------------------------------------------------------

AnsiString TMallExportHourlyUpdate::extractTenantId()
{
    return AnsiString(globalSettings->TenantNo);
}
//---------------------------------------------------------------------------

AnsiString TMallExportHourlyUpdate::extractDate()
{
    AnsiString result = "";
//    result = Now().FormatString("yyyy/mm/dd");
    TDateTime Yesterday = Now() - 1;

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        unsigned short Hour = 0;
        unsigned short Minutes = 0;
        unsigned short Seconds = 0;
        unsigned short dummy = 0;

        Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);
        result = (Hour < 6) ? Yesterday.FormatString("yyyymmdd") : Now().FormatString("yyyymmdd");
    }
    else
    {
        if(TGlobalSettings::Instance().MallIndex == SHANGRILAMALL)
        {
            UnicodeString TimeVal = "";
            int time = 0;
            bool flag = true;

            flag = PerformedLastZed();

            TimeVal = Now().FormatString("hh");
            time = StrToInt(TimeVal);
            if(time < 10)
            {
                result = (time >= 6) ? Now().FormatString("yyyymmdd") : (!flag) ? Yesterday.FormatString("yyyymmdd") : Now().FormatString("yyyymmdd");
            }
            else
            {
                result = Now().FormatString("yyyymmdd");
            }
        }
        else
        {
            result = Now().FormatString("yyyymmdd");
        }
    }
    return result;
}
//---------------------------------------------------------------------------

AnsiString TMallExportHourlyUpdate::extractTime()
{
    AnsiString result;
    UnicodeString TimeVal = "";
    int time = 0;

    if(TGlobalSettings::Instance().MallIndex == SHANGRILAMALL)
    {
        bool flag = true;

        flag = PerformedLastZed();

        TimeVal = Now().FormatString("hh");
        time = StrToInt(TimeVal);
        if(time < 10)
        {
            result = (time >= 6) ? "10" : (!flag) ? "21" : "10";
        }
        else if (time > 21)
        {
            result = "21";
        }
        else
        {
            result = TimeVal;
        }
    }
    else
    {
        result = Now().FormatString("hh");
    }
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractSales()
{
    Currency result = 0;

    if(TGlobalSettings::Instance().MallIndex == POWERPLANTMALL)
    {
        result = extractTotalGrossSales();
    }
    else if((TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL || TGlobalSettings::Instance().MallIndex == SHANGRILAMALL))
    {
        result = getDailySales();
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }
    else
    {
    // Total Discount, Refund, and Calcel are already deducted in the Gross Sales
        result = (extractTotalGrossSales() - extractTotalServiceCharge() -
                  extractTotalServiceChargeTax());
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportHourlyUpdate::extractTransactionCount()
{
    // this method will be called for each transaction
    // the result will be added to whatever the updater has at the moment
    int result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    // Megaworld Exclude that count for Refund and Cancel
    if ( TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL )
    {
        for( ; it != flatternedOrdersList.end(); it++ )
        {
            order = *it;

            if( !isRefundedOrder( order ) )
            {
                if( !isCancelledOrder( order ) )
                {
                    result = 1;
                }
            }
            else
            {
                result = -1;
            }
        }
    }
    else
    {
        result = 1;
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractTotalTaxFromResult( const TBillCalcResult &result, TTaxType taxType )
{
    Currency value = 0;
    TAX_RESULT_LIST::const_iterator it = result.Tax.begin();

    for( it; it != result.Tax.end(); it++)
    {
        if( it->TaxType == taxType )
            value += it->Value;
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        value = RoundToNearest(value,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return value;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractTotalDiscount()
{
    Currency result = 0;
    TPayment* payment;
    UnicodeString groupName = "";
    UnicodeString payGroupName = "";
    Currency Surcharge = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        result += order->BillCalcResult.TotalDiscount;
    }

    if(TGlobalSettings::Instance().MallIndex == SHANGRILAMALL)
    {
        result = 0;
//        result = fabs(result);
//        for (int i = 0; i < paymentTransaction->PaymentsCount(); i++)
//        {
//            payment = paymentTransaction->PaymentGet(i);
//            groupName = payment->GetFirstAssignedGroup().Name;
//            payGroupName = GIFT_PAYMENT_GROUP;
//
//            if( groupName == payGroupName)
//            {
//                Surcharge += payment->GetSurcharge();
//            }
//        }
//        result += extractRefundAmount();
//        result += Surcharge;
//        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return fabs(result);
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractServiceChargeAmount()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        result += order->BillCalcResult.ServiceCharge.Value;
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL || TGlobalSettings::Instance().MallIndex == SHANGRILAMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractRefundAmount()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        if( isRefundedOrder( order ) )
            result += order->BillCalcResult.FinalPrice;
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return fabs(result);
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractVoidAmount()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        if( order->OrderType == CanceledOrder )
        {
            if(TGlobalSettings::Instance().ItemPriceIncludeTax)
            {
                result += order->CancelledBillCalcResult.GrossPrice;
            }
            else
            {
                result += order->CancelledBillCalcResult.GrossPrice - order->CancelledBillCalcResult.TotalTax;
            }
        }
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::extractSCDiscountAmount()
{
    Currency result = 0;
    Currency scResult = 0;
    Currency SalesTax = 0;
    result = getDiscountGroupTotal(SCD_DISCOUNT_GROUP);
    if(TGlobalSettings::Instance().MallIndex == POWERPLANTMALL)
    {
        TItemMinorComplete* order;
        std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

        for( ; it != flatternedOrdersList.end(); it++ )
        {
            order = *it;
            SalesTax = extractTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );
            if(SalesTax == 0)
            {
                scResult += order->BillCalcResult.BasePrice * order->GetQty();
            }
        }
        result = scResult - result;
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportHourlyUpdate::extractRefundCount()
{
    int result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        if( isRefundedOrder( order ) )
        {
            result++;
            break;  // refund count is considered as per receipt
        }
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportHourlyUpdate::extractVoidCount()
{
    int result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        if( isCancelledOrder( order ))
        {
            result++;
            break;  // cancelled count is considered per receipt
        }
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportHourlyUpdate::extractPatronCount()
{
    int result = 0;
    bool isRefund = false;
    bool isCancel = false;

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        TItemMinorComplete* order;
        std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();
        for( ; it != flatternedOrdersList.end(); it++ )
        {
            order = *it;
            if( isRefundedOrder( order ) )
            {
                isRefund = true;
            }
            if( isCancelledOrder( order ) )
            {
                isCancel = true;
            }
        }

        std::vector <TPatronType> ::iterator ptrPatronTypes = paymentTransaction->Patrons.begin();
        for ( ; ptrPatronTypes != paymentTransaction->Patrons.end(); ptrPatronTypes++ )
        {
            if( isRefund )
            {
                result -= ptrPatronTypes->Count != 0 ? ptrPatronTypes->Count : 0;
            }
            else
            {
                if( !isCancel )
                {
                    result += ptrPatronTypes->Count != 0 ? ptrPatronTypes->Count : 0;
                }
            }
        }
    }
    else
    {
        std::vector <TPatronType> ::iterator ptrPatronTypes = paymentTransaction->Patrons.begin();
        for ( ; ptrPatronTypes != paymentTransaction->Patrons.end(); ptrPatronTypes++ )
        {
            result += ptrPatronTypes->Count != 0 ? ptrPatronTypes->Count : 0;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::getDiscountGroupTotal( UnicodeString discountGroupName )
{
    Currency result = 0;
    DISCOUNT_RESULT_LIST::iterator it_discount_list;
    std::vector<UnicodeString>::iterator it_discount_group_list;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it_orders = flatternedOrdersList.begin();

    for( ; it_orders != flatternedOrdersList.end(); it_orders++ )    // for each orders
    {
        order = *it_orders;

        it_discount_list = order->BillCalcResult.Discount.begin();
        for( ; it_discount_list != order->BillCalcResult.Discount.end(); it_discount_list++ )   //for each bill calc result discounts results list
        {
            it_discount_group_list = it_discount_list->DiscountGroupList.begin();
            for( ; it_discount_group_list != it_discount_list->DiscountGroupList.end(); it_discount_group_list++ ) // for each group in discount result
            {
                //discGroup = (TDiscountGroup*)(*it_discount_group_list);
                if( *it_discount_group_list == discountGroupName )
                {
                    result += it_discount_list->Content;
                    break;
                }
            }
        }
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportHourlyUpdate::getDailySales()
{
    Currency result = 0;
    Currency SalesTax = 0;
    TPayment* payment;
    UnicodeString groupName = "";
    UnicodeString payGroupName = "";
    Currency Surcharge = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    if((TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL || TGlobalSettings::Instance().MallIndex == SHANGRILAMALL))
    {
        for (int i = 0; i < paymentTransaction->PaymentsCount(); i++)
        {
            payment = paymentTransaction->PaymentGet(i);
            groupName = payment->GetFirstAssignedGroup().Name;
            payGroupName = GIFT_PAYMENT_GROUP;

            if( groupName == payGroupName)
            {
                Surcharge += payment->GetSurcharge();
            }
        }

        for( ; it != flatternedOrdersList.end(); it++ )
        {
            order = *it;

            result += order->BillCalcResult.FinalPrice;
//            result -= order->BillCalcResult.ServiceCharge.Value;
            if(TGlobalSettings::Instance().MallIndex == SHANGRILAMALL)
            {
                result -= order->BillCalcResult.ServiceCharge.Value;
                result -= order->BillCalcResult.ServiceCharge.TaxValue;
            }
        }
        result += Surcharge;
    }

    if((TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL || TGlobalSettings::Instance().MallIndex == SHANGRILAMALL))
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Utility methods
//---------------------------------------------------------------------------

void TMallExportHourlyUpdate::flatternOrdersList()
{
    TItemMinorComplete* item;
    TItemMinorComplete* subItem;

    flatternedOrdersList.clear();

    for( int index = 0; index < paymentTransaction->Orders->Count; index++ )
    {
        item = (TItemMinorComplete*)paymentTransaction->Orders->Items[index];
        flatternedOrdersList.push_back(item);

        for( int subIndex = 0; subIndex < item->SubOrders->Count; subIndex++ )
        {
            subItem = (TItemMinorComplete*)item->SubOrders->Items[subIndex];
            flatternedOrdersList.push_back(subItem);
        }
    }
}
//---------------------------------------------------------------------------

bool TMallExportHourlyUpdate::isRefundedOrder( const TItemMinorComplete *order )
{
    return order->OrderType == CreditNonExistingOrder && order->GetQty() < 0;
}
//---------------------------------------------------------------------------

bool TMallExportHourlyUpdate::isCancelledOrder( const TItemMinorComplete *order )
{
    return order->OrderType == CanceledOrder && order->Price() == 0;
}
//---------------------------------------------------------------------------

bool TMallExportHourlyUpdate::PerformedLastZed()
{
    bool flag = true;

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    query->Close();

    query->SQL->Text = "SELECT * FROM MALLEXPORT_HOURLY";
    DBTransaction.StartTransaction();
    query->ExecQuery();

    flag = (query->RecordCount == 0) ? true : false;

    return flag;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
