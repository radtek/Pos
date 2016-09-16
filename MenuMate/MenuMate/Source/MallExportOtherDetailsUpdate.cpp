//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportOtherDetailsUpdate.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// PUBLIC SECTION
//---------------------------------------------------------------------------
TMallExportOtherDetailsUpdate::TMallExportOtherDetailsUpdate()
{
    globalSettings = &TGlobalSettings::Instance();

    flatternedOrdersList.clear();
}

TMallExportOtherDetailsUpdate::~TMallExportOtherDetailsUpdate()
{
    //todo: cleanup if needed
}

TExportUpdateResponse TMallExportOtherDetailsUpdate::UpdateOtherDetailsExportTablesOnTransaction( TPaymentTransaction *payTransac )
{
    TExportUpdateResponse response;

    try
    {
        // clear the values to default
        clearOtherDetailsExportFieldValues();

        // set current payment transaction
        setPaymentTransaction( payTransac );

        // set database transaction from payment transaction
        setDbTransaction( &payTransac->DBTransaction );

        // gather all the required information from the transaction
        setOtherDetailsExportFieldValuesOnTransaction();

    }
    catch( Exception &ex )
    {
        response.Successful = false;
        response.Message = ex.Message;
        response.Description = "Failed to update mall export hourly table on transaction";
    }

    return response;
}

TExportUpdateResponse TMallExportOtherDetailsUpdate::ResetOtherDetailsExportTablesOnZed()
{
    TExportUpdateResponse response;

    try
    {
        // clear the values to default
        clearOtherDetailsExportFieldValues();

        // delete all rows on the mall export hourly table
        deleteOtherDetailsExportContentsOnZed();

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

void TMallExportOtherDetailsUpdate::setPaymentTransaction( TPaymentTransaction *payTransac )
{
    paymentTransaction = payTransac;
    flatternOrdersList();   // flatterns the entire set of orders into one list, so the calculations are easier to make
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::setDbTransaction( Database::TDBTransaction *transaction )
{
    dbTransaction = transaction;
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::clearOtherDetailsExportFieldValues()
{
    dateValue = "";
    mallCode = "";
    discountType = "";
    discountPerc = 0;
    discountAmount = 0;
    transactionCode = "";
    refundcancelReason = "";
    refundcancelAmount = 0;
    tabKey = 0;
    transactionCount = 0;
    finedinecustCount = 0;
    scdiscountCount = 0;
    paymentCode = "";
    paymentCodeDesc = "";
    paymentClass = "";
    paymentClassDesc = "";
    paymentAmount = 0;
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::setOtherDetailsExportFieldValuesOnTransaction()
{
    dateValue                    = extractDate();
    mallCode                     = extractMallCode();
    discountType                 = "";
    discountPerc                 = 0;
    discountAmount               = 0;
    transactionCode              = "";
    refundcancelReason           = "";
    refundcancelAmount           = 0;
    tabKey                       = 0;
    transactionCount             = extractTransactionCount();
    finedinecustCount            = extractFineDineCustCount();
    scdiscountCount              = extractSCDiscountCount();

    paymentCode                  = "";
    paymentCodeDesc              = "";
    paymentClass                 = "";
    paymentClassDesc             = "";
    paymentAmount                = 0;

    extractDiscountInfo(discountType,discountPerc,discountAmount,transactionCode);
    extractRefundInfo(refundcancelReason, refundcancelAmount,transactionCode);
    extractPaymentInfo(refundcancelReason, refundcancelAmount,transactionCode);
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::Commit()
{
    int Retval = 0;

	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *Query = Transaction.Query(Transaction.AddQuery());

    Query->SQL->Text = "SELECT GEN_ID(GEN_MEOD_OD_KEY, 1) FROM RDB$DATABASE";
	Transaction.StartTransaction();
	Query->ExecQuery();
	Retval = Query->Fields[0]->AsInteger;
	Transaction.Commit();
	Query->Close();

    Query->SQL->Text = "INSERT INTO MALLEXPORTOTHERDETAILS (" "MEOD_OD_KEY, " "DATE_VALUE, "
                       "TENANT_NAME, " "DISCOUNT_TYPE, " "DISCOUNT_PERC, " "DISCOUNT_AMOUNT, " "TRANSACTION_CODE, "
                       "REFUND_CANCEL_REASON, " "REFUND_CANCEL_AMOUNT, " "REFUND_CANCEL_TABKEY, " "TRANSACTION_COUNT, " "FINEDINECUST_COUNT, " "SCDISCOUNT_COUNT, "
                       "PAYMENT_CODE, " "PAYMENT_CODE_DESC, " "PAYMENT_CLASS, " "PAYMENT_CLASS_DESC, " "PAYMENT_AMOUNT ) "
                       "" "VALUES ("
                       ":MEOD_OD_KEY, " ":DATE_VALUE, " ":TENANT_NAME, " ":DISCOUNT_TYPE, " ":DISCOUNT_PERC, "
                       ":DISCOUNT_AMOUNT, " ":TRANSACTION_CODE, "
                       ":REFUND_CANCEL_REASON, " ":REFUND_CANCEL_AMOUNT, " ":REFUND_CANCEL_TABKEY, " ":TRANSACTION_COUNT, " ":FINEDINECUST_COUNT, " ":SCDISCOUNT_COUNT, "
                       ":PAYMENT_CODE, " ":PAYMENT_CODE_DESC, " ":PAYMENT_CLASS, " ":PAYMENT_CLASS_DESC, " ":PAYMENT_AMOUNT ) ";

	Transaction.StartTransaction();
    Query->ParamByName("MEOD_OD_KEY")->AsString = Retval;
    Query->ParamByName("DATE_VALUE")->AsString = dateValue;
    Query->ParamByName("TENANT_NAME")->AsString = mallCode;
    Query->ParamByName("DISCOUNT_TYPE")->AsString = discountType;
    Query->ParamByName("DISCOUNT_PERC")->AsCurrency = discountPerc;
    Query->ParamByName("DISCOUNT_AMOUNT")->AsCurrency = discountAmount;
    Query->ParamByName("TRANSACTION_CODE")->AsString = transactionCode;
    Query->ParamByName("REFUND_CANCEL_REASON")->AsString = refundcancelReason;
    Query->ParamByName("REFUND_CANCEL_AMOUNT")->AsCurrency = refundcancelAmount;
    Query->ParamByName("REFUND_CANCEL_TABKEY")->AsCurrency = tabKey;
    Query->ParamByName("TRANSACTION_COUNT")->AsInteger = transactionCount;
    Query->ParamByName("FINEDINECUST_COUNT")->AsInteger = finedinecustCount;
    Query->ParamByName("SCDISCOUNT_COUNT")->AsInteger = scdiscountCount;
    Query->ParamByName("PAYMENT_CODE")->AsString = paymentCode;
    Query->ParamByName("PAYMENT_CODE_DESC")->AsString = paymentCodeDesc;
    Query->ParamByName("PAYMENT_CLASS")->AsString = paymentClass;
    Query->ParamByName("PAYMENT_CLASS_DESC")->AsString = paymentClassDesc;
    Query->ParamByName("PAYMENT_AMOUNT")->AsCurrency = paymentAmount;
	Query->ExecQuery();

	Transaction.Commit();
	Query->Close();
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::deleteOtherDetailsExportContentsOnZed()
{
	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *Query = Transaction.Query(Transaction.AddQuery());

    Query->SQL->Text = "DELETE FROM MALLEXPORTOTHERDETAILS";
	Transaction.StartTransaction();
	Query->ExecQuery();
	Transaction.Commit();
	Query->Close();
}
//---------------------------------------------------------------------------

AnsiString TMallExportOtherDetailsUpdate::extractDate()
{
    AnsiString result = "";

    if(TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
    {
        result = Now().FormatString("mm/dd/yyyy hh:nn:ss");
    }
    return result;
}
//---------------------------------------------------------------------------

AnsiString TMallExportOtherDetailsUpdate::extractMallCode()
{
    return AnsiString(globalSettings->TenantNo);
}
//---------------------------------------------------------------------------

Currency TMallExportOtherDetailsUpdate::extractCancelAmount()
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

    return result;
}
//---------------------------------------------------------------------------

int TMallExportOtherDetailsUpdate::extractTransactionCount()
{
    int result = 0;

    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    // Megaworld Exclude that count for Refund and Cancel
    if ( TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL )
    {
        for( ; it != flatternedOrdersList.end(); it++ )
        {
            result = 1;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportOtherDetailsUpdate::extractFineDineCustCount()
{
    int result = 0;

    if(TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
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

int TMallExportOtherDetailsUpdate::extractSCDiscountCount()
{
    int result = 0;
    result = getDiscountGroupCountPerTransaction(SCD_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::extractDiscountInfo(AnsiString &discountType, Currency &discountPerc, Currency &discountAmount, AnsiString &transactionCode)
{
    getDiscountGroupInfo(discountType,discountPerc,discountAmount,transactionCode);
}
//---------------------------------------------------------------------------

int TMallExportOtherDetailsUpdate::getDiscountGroupCountPerTransaction( UnicodeString discountGroupName )
{
    int result = 0;
    DISCOUNT_RESULT_LIST::iterator it_discount_list;
    std::vector<UnicodeString>::iterator it_discount_group_list;
    bool discountGroupFound = false;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it_orders = flatternedOrdersList.begin();

    for( ;
         it_orders != flatternedOrdersList.end() && !discountGroupFound;
         it_orders++ )    // for each orders
    {
        order = *it_orders;

        it_discount_list = order->BillCalcResult.Discount.begin();
        for( ;
             it_discount_list != order->BillCalcResult.Discount.end() && !discountGroupFound;
             it_discount_list++ )   //for each bill calc result discounts results list
        {
            it_discount_group_list = it_discount_list->DiscountGroupList.begin();
            for( ;
                 it_discount_group_list != it_discount_list->DiscountGroupList.end() && !discountGroupFound;
                 it_discount_group_list++ ) // for each group in discount result
            {
                if( *it_discount_group_list == discountGroupName )
                {
                    result ++;
                    discountGroupFound = true;
                    break;
                }
            }
        }
    }

    return result;
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::getDiscountGroupInfo(AnsiString &discountType, Currency &discountPerc, Currency &discountAmount, AnsiString &transactionCode)
{
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
                Currency Percent = 0;
                if( *it_discount_group_list == SCD_DISCOUNT_GROUP )
                {
                    discountType = "SNR";
                }
                else if( *it_discount_group_list == PWD_DISCOUNT_GROUP )
                {
                    discountType = "PWD";
                }
                else if( *it_discount_group_list == GPC_DISCOUNT_GROUP )
                {
                    discountType = "GPC";
                }
                else if( *it_discount_group_list == EMPLOYEE_DISCOUNT_GROUP )
                {
                    discountType = "EMP";
                }
                else if( *it_discount_group_list == VIP_DISCOUNT_GROUP )
                {
                    discountType = "VIP";
                }
                else if( *it_discount_group_list == GRP1_DISCOUNT_GROUP )
                {
                    discountType = "OTH";
                }
                else
                {
                    discountType = "REG";
                }

                Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
                TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

                query->SQL->Text = "SELECT FIRST 1 * FROM DISCOUNTS WHERE DISCOUNTS.DISCOUNT_KEY =:DKEY";
                DBTransaction.StartTransaction();
         		query->ParamByName("DKEY")->AsInteger = it_discount_list->DiscountKey;
                query->ExecQuery();

                while(!query->Eof)
                {
                    Percent = query->FieldByName("PERCENTAGE")->AsCurrency;
                    query->Next();
                }
                DBTransaction.Commit();

                discountPerc = Percent;
                discountAmount = it_discount_list->Content;
                transactionCode = "DISC";
                Commit();
            }
        }
    }
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::extractRefundInfo(AnsiString &refundcancelReason, Currency &refundcancelAmount, AnsiString &transactionCode)
{
    AnsiString status = "";
    TDateTime LastDate;

    if(!extractRefundStatus())
    {
        return;
    }

	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *Query = Transaction.Query(Transaction.AddQuery());

    Query->SQL->Text = "SELECT DAYARCHIVE.PRICE, SECURITY.NOTE, SECURITY.TIME_STAMP, SECURITY.SECURITY_EVENT "
                       "FROM DAYARCHIVE LEFT JOIN SECURITY ON DAYARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
                       "WHERE SECURITY.TIME_STAMP >= :TIME_STAMP_VAL AND SECURITY.SECURITY_EVENT = 'Credit'";
	Transaction.StartTransaction();
    Query->ParamByName("TIME_STAMP_VAL")->AsDateTime = getRefundQuery();
	Query->ExecQuery();

    while(!Query->Eof)
    {
        refundcancelAmount = Query->FieldByName("PRICE")->AsCurrency;
        refundcancelReason = Query->FieldByName("NOTE")->AsString;
        transactionCode = "RFND";
        Commit();
        if(Query->Next())
        {
            transactionCount = 0;
            finedinecustCount = 0;
        }
    }
	Transaction.Commit();
	Query->Close();
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::extractCancelStatus(TDateTime value,  bool CancelDiscSCDStatus, int TabKey, int patron_count)
{
    dateValue = extractDate();
    mallCode = extractMallCode();
    bool isNextQuery = false;
    transactionCount = 1;
    scdiscountCount =  CancelDiscSCDStatus ? 1 : 0;

	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *Query = Transaction.Query(Transaction.AddQuery());

    Query->SQL->Text = "SELECT ORDERS.CANCEL_GROSS_PRICE, SECURITY.NOTE, SECURITY.TIME_STAMP, SECURITY.SECURITY_EVENT, ORDERS.PATRON_COUNT "
                       "FROM ORDERS LEFT JOIN SECURITY ON ORDERS.SECURITY_REF = SECURITY.SECURITY_REF "
                       "WHERE SECURITY.TIME_STAMP >= :TIME_STAMP_VAL AND SECURITY.SECURITY_EVENT = 'CancelY' "
                       "OR SECURITY.SECURITY_EVENT = 'Cancel'";

	Transaction.StartTransaction();
    Query->ParamByName("TIME_STAMP_VAL")->AsDateTime = value;
	Query->ExecQuery();

    while(!Query->Eof)
    {
        refundcancelAmount = Query->FieldByName("CANCEL_GROSS_PRICE")->AsCurrency;
        refundcancelReason = Query->FieldByName("NOTE")->AsString;
        if(!isNextQuery)
        {
           if(patron_count != 0)
           {
              finedinecustCount = patron_count;
           }
           else
           {
              finedinecustCount = Query->FieldByName("PATRON_COUNT")->AsInteger;
           }
        }
        else
        {
           finedinecustCount = 0;
        }
        tabKey = TabKey;
        transactionCode = "CNCLD";
        Commit();
        if(Query->Next())
        {
            transactionCount = 0;
            isNextQuery = true;
        }

    }
	Transaction.Commit();
	Query->Close();
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::extractPaymentInfo(AnsiString &refundcancelReason, Currency &refundcancelAmount, AnsiString &transactionCode)
{
    TPayment* payment;
    UnicodeString groupName = "";
    transactionCount = extractTransactionCount();
    finedinecustCount = extractFineDineCustCount();

    for (int i = 0; i < paymentTransaction->PaymentsCount(); i++)
    {
        payment = paymentTransaction->PaymentGet(i);
        groupName = payment->GetFirstAssignedGroup().Name;

        transactionCode = "PAYMENT";
        dateValue = extractDate();
        mallCode = extractMallCode();

        if( payment && payment->GetPay() != 0 && groupName == CASH_PAYMENT_GROUP )
        {
            paymentCode = "CSH";
            paymentCodeDesc = "CASH";
            paymentClass = payment->Name;
            paymentClassDesc = getPaymentClass(payment);
            paymentAmount = payment->GetPayTendered() - payment->GetChange();
            Commit();
        }
        else if(  payment && payment->GetPay() != 0 && groupName == CARD_PAYMENT_GROUP )
        {
            paymentCode = "CRD";
            paymentCodeDesc = "CARD";
            paymentClass = payment->Name;
            paymentClassDesc = getPaymentClass(payment);
            paymentAmount = payment->GetPayTendered() - payment->GetChange();
            Commit();
        }
        else if(  payment && payment->GetPay() != 0 && groupName == OTHER_PAYMENT_GROUP )
        {
            paymentCode = "OTH";
            paymentCodeDesc = "OTHERS";
            paymentClass = payment->Name;
            paymentClassDesc = getPaymentClass(payment);
            paymentAmount = payment->GetPayTendered() - payment->GetChange();
            Commit();
        }
    }
}

//---------------------------------------------------------------------------

AnsiString TMallExportOtherDetailsUpdate::getPaymentClass( TPayment* payment )
{
    AnsiString result = "";

    if(payment->Name == "MBTC")
    {
        result = "Metropolitan Bank & Trust Company";
    }
    else if(payment->Name == "BPI")
    {
        result = "Bank of the Philippine Islands";
    }
    else if(payment->Name == "CTB")
    {
        result = "Citibank";
    }
    else if(payment->Name == "RCBC")
    {
        result = "RCBC Bankard";
    }
    else if(payment->Name == "BDO")
    {
        result = "Banco de Oro";
    }
    else if(payment->Name == "SB")
    {
        result = "Security Bank";
    }
    else if(payment->Name == "MB")
    {
        result = "Maybank";
    }
    else if(payment->Name == "LBP")
    {
        result = "Land Bank of the Philippines";
    }
    else if(payment->Name == "PNB")
    {
        result = "Philippine National Bank";
    }
    else if(payment->Name == "EWB")
    {
        result = "East West Bank";
    }
    else if(payment->Name == "HSBC")
    {
        result = "Hongkong and Shanghai Banking Corporation";
    }
    else if(payment->Name == "SCB")
    {
        result = "Standard Chartered Bank";
    }
    else if(payment->Name == "AB")
    {
        result = "Allied Bank";
    }
    else if(payment->Name == "BOC")
    {
        result = "Bank of Commerce";
    }
    else if(payment->Name == "Amex")
    {
        result = "American Express";
    }
    else if(payment->Name == "Diners")
    {
        result = "Diners";
    }
    else if(payment->Name == "Visa")
    {
        result = "Visa";
    }
    else if(payment->Name == "MasterCard")
    {
        result = "MasterCard";
    }
    else if(payment->Name == "Cash")
    {
        result = "Cash Sales";
    }
    else
    {
        result = payment->Name;
    }

    return result;
}

//---------------------------------------------------------------------------

TDateTime TMallExportOtherDetailsUpdate::getRefundQuery()
{
    TDateTime LastDate = Now();
    UnicodeString abcd = Now().FormatString("mm/dd/yyyy hh:nn:ss");

	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *Query = Transaction.Query(Transaction.AddQuery());

    Query->SQL->Text = "SELECT FIRST 1 SECURITY.TIME_STAMP "
                       "FROM DAYARCHIVE LEFT JOIN SECURITY ON DAYARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
                       "WHERE SECURITY.TIME_STAMP >= :TIME_STAMP_VAL AND SECURITY.SECURITY_EVENT = 'Credit' ORDER BY SECURITY.TIME_STAMP DESC";

	Transaction.StartTransaction();
	Query->ExecQuery();

    while(!Query->Eof)
    {
        LastDate = Query->FieldByName("TIME_STAMP")->AsDateTime;
        Query->Next();
    }
    abcd = LastDate.FormatString("mm/dd/yyyy hh:nn:ss");
	Transaction.Commit();
	Query->Close();

    return LastDate;
}
//---------------------------------------------------------------------------

TDateTime TMallExportOtherDetailsUpdate::getCancelQuery()
{
    TDateTime LastDate = Now();
    UnicodeString abcd = Now().FormatString("mm/dd/yyyy hh:nn:ss");

	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *Query = Transaction.Query(Transaction.AddQuery());

    Query->SQL->Text = "SELECT FIRST 1 SECURITY.TIME_STAMP "
                       "FROM DAYARCHIVE LEFT JOIN SECURITY ON DAYARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
                       "WHERE SECURITY.TIME_STAMP >= :TIME_STAMP_VAL AND SECURITY.SECURITY_EVENT = 'Cancel' "
                       "OR SECURITY.SECURITY_EVENT = 'CancelY' ORDER BY SECURITY.TIME_STAMP DESC";

	Transaction.StartTransaction();
    Query->ParamByName("TIME_STAMP_VAL")->AsDateTime = Now();
	Query->ExecQuery();
    while(!Query->Eof)
    {
        LastDate = Query->FieldByName("TIME_STAMP")->AsDateTime;
        Query->Next();
    }
    abcd = LastDate.FormatString("mm/dd/yyyy hh:nn:ss");
	Transaction.Commit();
	Query->Close();

    return LastDate;
}
//---------------------------------------------------------------------------

bool TMallExportOtherDetailsUpdate::extractRefundStatus()
{
    bool result = false;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        if( isRefundedOrder( order ) )
            result = true;
    }

    return result;
}
//---------------------------------------------------------------------------

bool TMallExportOtherDetailsUpdate::isRefundedOrder( const TItemMinorComplete *order )
{
    return order->OrderType == CreditNonExistingOrder && order->GetQty() < 0;
}
//---------------------------------------------------------------------------

void TMallExportOtherDetailsUpdate::flatternOrdersList()
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

bool TMallExportOtherDetailsUpdate::getCancelledDiscountStatus(int TabKey)
{
    bool result = false;
    int DiscKeys = 0;
    UnicodeString DiscGroup = "";

	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *DiscountKeyQuery = Transaction.Query(Transaction.AddQuery());
	DiscountKeyQuery->Close();

    DiscountKeyQuery->SQL->Text = "SELECT OD.DISCOUNT_KEY FROM ORDERS O LEFT JOIN ORDERDISCOUNTS OD ON OD.ORDER_KEY = O.ORDER_KEY WHERE "
                                  "O.TAB_KEY = :TABKEY AND OD.DISCOUNT_KEY is not Null";

	Transaction.StartTransaction();
    DiscountKeyQuery->ParamByName("TABKEY")->AsInteger = TabKey;
	DiscountKeyQuery->ExecQuery();
    while(!DiscountKeyQuery->Eof)
    {
        DiscKeys = DiscountKeyQuery->FieldByName("DISCOUNT_KEY")->AsInteger;

    	TIBSQL *DiscountGroupsKeyQuery = Transaction.Query(Transaction.AddQuery());
	    DiscountGroupsKeyQuery->Close();
        DiscountGroupsKeyQuery->SQL->Text = "SELECT DG.DISCOUNTGROUP_NAME FROM DISCOUNT_GROUPS DG LEFT JOIN "
                                            "DISCOUNTGROUPS_DISCOUNTTYPES DD ON DG.DISCOUNTGROUPS_KEY = DD.DISCOUNTGROUPS_KEY "
                                            "WHERE DD.DISCOUNTTYPE_KEY = :DISCOUNT_KEY";

    	Transaction.StartTransaction();
        DiscountGroupsKeyQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscKeys;
    	DiscountGroupsKeyQuery->ExecQuery();

        while(!DiscountGroupsKeyQuery->Eof)
        {
            DiscGroup = DiscountGroupsKeyQuery->FieldByName("DISCOUNTGROUP_NAME")->AsString;
            if(DiscGroup == "Senior Citizen")
            {
                result = true;
                break;
            }
            DiscountGroupsKeyQuery->Next();
        }

        DiscountKeyQuery->Next();
    }
	Transaction.Commit();

    return result;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
