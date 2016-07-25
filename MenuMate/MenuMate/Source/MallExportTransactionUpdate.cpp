//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportTransactionUpdate.h"

//---------------------------------------------------------------------------
// PUBLIC SECTION
//---------------------------------------------------------------------------
TMallExportTransactionUpdate::TMallExportTransactionUpdate()
{
    globalSettings = &TGlobalSettings::Instance();

    flatternedOrdersList.clear();

    currentDateTime = Now();
}

TMallExportTransactionUpdate::~TMallExportTransactionUpdate()
{
    //todo: cleanup if needed
}

TExportUpdateResponse TMallExportTransactionUpdate::UpdateTransactionExportTablesOnTransaction( TPaymentTransaction *payTransac )
{
    TExportUpdateResponse response;

    try
    {
        // clear the values to default
        clearTransactionExportFieldValues();

        // set current payment transaction
        setPaymentTransaction( payTransac );

        // set database transaction from payment transaction
        setDbTransaction( &payTransac->DBTransaction );

        // gather all the required information from the transaction
        setTransactionExportFieldValuesOnTransaction();

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

TExportUpdateResponse TMallExportTransactionUpdate::ResetTransactionExportTablesOnZed()
{
    TExportUpdateResponse response;

    try
    {
        // clear the values to default
        clearTransactionExportFieldValues();

        // delete all rows on the mall export hourly table
        deleteTransactionExportContentsOnZed();

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

void TMallExportTransactionUpdate::setPaymentTransaction( TPaymentTransaction *payTransac )
{
    paymentTransaction = payTransac;
    flatternOrdersList();   // flatterns the entire set of orders into one list, so the calculations are easier to make
}
//---------------------------------------------------------------------------

void TMallExportTransactionUpdate::setDbTransaction( Database::TDBTransaction *transaction )
{
    dbTransaction = transaction;
}
//---------------------------------------------------------------------------

void TMallExportTransactionUpdate::clearTransactionExportFieldValues()
{
    Date = extractDate();
    TenantId = "";
    CurrentDate = extractCurrentDate();
    Time = extractTime();
    TerminalNumber = "";
    InvoiceNumber = "";
    SalesFlag = "";
    TotalGrossSales = 0;
    TotalSalesTax = 0;
    TotalNonVatSales = 0;
    TotalVatExemptSales = 0;
    TotalSeniorCitizensDiscount = 0;
    TotalEmployeeDiscount = 0;
    TotalPromoDiscount = 0;
    TotalOtherDiscount = 0;
    TotalRefund = 0;
    TotalServiceCharge = 0;
    TotalOtherSurcharge = 0;
    TotalNetSales = 0;
    GrandTotalOld = 0;
    GrandTotal = 0;
    CashSales = 0;
    CardSales = 0;
    CheckSales = 0;
    GCSales = 0;
    EPaySales = 0;
    OtherSales = 0;
    TotalCancelled = 0;
    TotalDeposit = 0;
    TotalAppliedDeposit = 0;
    VatPercentage = 0;
    ZCount = 0;
    VatableCount = 0;
    NonVatCount = 0;
    VatExemptCount = 0;
    SeniorCitizenDiscountCount = 0;
    EmployeeDiscountCount = 0;
    PromoDiscountCount = 0;
    OtherDiscountCount = 0;
    RefundCount = 0;
    ServiceChargeCount = 0;
    OtherSurchargeCount = 0;
    CashSalesCount = 0;
    CardSalesCount = 0;
    CheckSalesCount = 0;
    GCSalesCount = 0;
    EPaySalesCount = 0;
    OtherSalesCount = 0;
    CancelledCount = 0;
    CancelledTransactionsCount = 0;
    DepositCount = 0;
    AppliedDepositCount = 0;
    PatronCount = 0;
    TransactionCount = 0;
    NoSaleCount = 0;
}
//---------------------------------------------------------------------------

void TMallExportTransactionUpdate::setTransactionExportFieldValuesOnTransaction()
{
    Date                         = extractDate();
    TenantId                     = extractTenantId();
    CurrentDate                  = extractCurrentDate();
    Time                         = extractTime();
    TerminalNumber               = extractTerminalNumber();
    InvoiceNumber                = extractInvoiceNumber();
    SalesFlag                    = extractSalesFlag();
    TotalGrossSales              = extractTotalGrossSales();
    TotalSalesTax                = extractTotalSalesTax();
    TotalNonVatSales             = extractTotalNonVatSales();
    TotalVatExemptSales          = extractTotalVatExemptSales();
    TotalSeniorCitizensDiscount  = extractTotalSeniorCitizensDiscount();
    TotalEmployeeDiscount        = extractTotalEmployeeDiscount();
    TotalPromoDiscount           = extractTotalPromoDiscount();
    TotalOtherDiscount           = extractTotalOtherDiscount();
    TotalRefund                  = extractTotalRefund();
    TotalServiceCharge           = extractTotalServiceCharge();
    TotalOtherSurcharge          = extractTotalOtherSurcharge();
    TotalNetSales                = extractTotalNetSales();
    GrandTotalOld                = extractGrandTotalOld();
    GrandTotal                   = extractGrandTotal();
    CashSales                    = extractCashSales();
    CardSales                    = extractCardSales();
    CheckSales                   = extractCheckSales();
    GCSales                      = extractGCSales();
    EPaySales                    = extractEPaySales();
    OtherSales                   = extractOtherSales();
    TotalCancelled               = extractTotalCancelled();
    TotalDeposit                 = extractTotalDeposit();
    TotalAppliedDeposit          = extractTotalAppliedDeposit();
    VatPercentage                = extractVatPercentage();
    ZCount                       = extractZCount();
    VatableCount                 = extractVatableCount();
    NonVatCount                  = extractNonVatCount();
    VatExemptCount               = extractVatExemptCount();
    SeniorCitizenDiscountCount   = extractSeniorCitizenDiscountCount();
    EmployeeDiscountCount        = extractEmployeeDiscountCount();
    PromoDiscountCount           = extractPromoDiscountCount();
    OtherDiscountCount           = extractOtherDiscountCount();
    RefundCount                  = extractRefundCount();
    ServiceChargeCount           = extractServiceChargeCount();
    OtherSurchargeCount          = extractOtherSurchargeCount();
    CashSalesCount               = extractCashSalesCount();
    CardSalesCount               = extractCardSalesCount();
    CheckSalesCount              = extractCheckSalesCount();
    GCSalesCount                 = extractGCSalesCount();
    EPaySalesCount               = extractEPaySalesCount();
    OtherSalesCount              = extractOtherSalesCount();
    CancelledCount               = extractCancelledCount();
    CancelledTransactionsCount   = extractCancelledTransactionsCount();
    DepositCount                 = extractDepositCount();
    AppliedDepositCount          = extractAppliedDepositCount();
    PatronCount                  = extractPatronCount();
    TransactionCount             = extractTransactionCount();
    NoSaleCount                  = extractNoSaleCount();
}
//---------------------------------------------------------------------------

void TMallExportTransactionUpdate::Commit()
{
    AnsiString Retval = "";

	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *Query = Transaction.Query(Transaction.AddQuery());

    Query->SQL->Text = "SELECT GEN_ID(GEN_MALLEXPORT_TRANSACTION_KEY, 1) FROM RDB$DATABASE";
	Transaction.StartTransaction();
	Query->ExecQuery();
	Retval = Query->Fields[0]->AsInteger;
	Transaction.Commit();
	Query->Close();

    Query->SQL->Text = "INSERT INTO "
                       "MALLEXPORT_TRANSACTION ("
                       "ME_TRANSACTION_KEY, "
                       "DATE_VALUE, "
                       "TENANT_NAME, "
                       "CURRENT_DATE_VALUE, "
                       "TIME_VALUE, "
                       "TERMINAL_NAME, "
                       "INVOICE_NUMBER, "
                       "GROSS_SALES, "
                       "VAT_SALES, "
                       "NONVAT_SALES, "
                       "VATEXEMPT_SALES, "
                       "SCDISCOUNT_AMOUNT, "
                       "EMPDISCOUNT_AMOUNT, "
                       "PRODISCOUNT_AMOUNT, "
                       "OTHERDISCOUNT_AMOUNT, "
                       "REFUND_AMOUNT, "
                       "SCHARGE_AMOUNT, "
                       "OTHER_SURCHARGE, "
                       "NET_SALES, "
                       "OLD_GRANDTOTAL, "
                       "NEW_GRANDTOTAL, "
                       "ZCOUNT, "
                       "CASH_SALES_AMOUNT, "
                       "CARD_SALES_AMOUNT, "
                       "CHECK_SALES_AMOUNT, "
                       "GC_SALES_AMOUNT, "
                       "EPAY_SALES_AMOUNT, "
                       "OTHER_SALES_AMOUNT, "
                       "VOID_AMOUNT, "
                       "DEPOSIT_AMOUNT, "
                       "APPLIED_DEPOSIT_AMOUNT, "
                       "VAT_SALES_COUNT, "
                       "NONVAT_SALES_COUNT, "
                       "VATEXEMPT_SALES_COUNT, "
                       "SCDISCOUNT_COUNT, "
                       "EMPDISCOUNT_COUNT, "
                       "PRODISCOUNT_COUNT, "
                       "OTHERDISCOUNT_COUNT, "
                       "REFUND_COUNT, "
                       "SCHARGE_COUNT, "
                       "OTHER_SURCHARGE_COUNT, "
                       "CASH_COUNT, "
                       "CARD_COUNT, "
                       "CHECK_COUNT, "
                       "GC_COUNT, "
                       "EPAY_COUNT, "
                       "OTHER_COUNT, "
                       "VOID_COUNT, "
                       "TRANS_VOID_COUNT, "
                       "DEPOSIT_COUNT, "
                       "APPLIED_DEPOSIT_COUNT, "
                       "PATRON_COUNT, "
                       "TRANSACTION_COUNT, "
                       "NOSALE_COUNT, "
                       "SALES_FLAG, "
                       "VAT_PERCENTAGE) "
                       "VALUES ("
                       ":ME_TRANSACTION_KEY, "
                       ":DATE_VALUE, "
                       ":TENANT_NAME, "
                       ":CURRENT_DATE_VALUE, "
                       ":TIME_VALUE, "
                       ":TERMINAL_NAME, "
                       ":INVOICE_NUMBER, "
                       ":GROSS_SALES, "
                       ":VAT_SALES, "
                       ":NONVAT_SALES, "
                       ":VATEXEMPT_SALES, "
                       ":SCDISCOUNT_AMOUNT, "
                       ":EMPDISCOUNT_AMOUNT, "
                       ":PRODISCOUNT_AMOUNT, "
                       ":OTHERDISCOUNT_AMOUNT, "
                       ":REFUND_AMOUNT, "
                       ":SCHARGE_AMOUNT, "
                       ":OTHER_SURCHARGE, "
                       ":NET_SALES, "
                       ":OLD_GRANDTOTAL, "
                       ":NEW_GRANDTOTAL, "
                       ":ZCOUNT, "
                       ":CASH_SALES_AMOUNT, "
                       ":CARD_SALES_AMOUNT, "
                       ":CHECK_SALES_AMOUNT, "
                       ":GC_SALES_AMOUNT, "
                       ":EPAY_SALES_AMOUNT, "
                       ":OTHER_SALES_AMOUNT, "
                       ":VOID_AMOUNT, "
                       ":DEPOSIT_AMOUNT, "
                       ":APPLIED_DEPOSIT_AMOUNT, "
                       ":VAT_SALES_COUNT, "
                       ":NONVAT_SALES_COUNT, "
                       ":VATEXEMPT_SALES_COUNT, "
                       ":SCDISCOUNT_COUNT, "
                       ":EMPDISCOUNT_COUNT, "
                       ":PRODISCOUNT_COUNT, "
                       ":OTHERDISCOUNT_COUNT, "
                       ":REFUND_COUNT, "
                       ":SCHARGE_COUNT, "
                       ":OTHER_SURCHARGE_COUNT, "
                       ":CASH_COUNT, "
                       ":CARD_COUNT, "
                       ":CHECK_COUNT, "
                       ":GC_COUNT, "
                       ":EPAY_COUNT, "
                       ":OTHER_COUNT, "
                       ":VOID_COUNT, "
                       ":TRANS_VOID_COUNT, "
                       ":DEPOSIT_COUNT, "
                       ":APPLIED_DEPOSIT_COUNT, "
                       ":PATRON_COUNT, "
                       ":TRANSACTION_COUNT, "
                       ":NOSALE_COUNT, "
                       ":SALES_FLAG, "
                       ":VAT_PERCENTAGE) ";
	Transaction.StartTransaction();
    Query->ParamByName("ME_TRANSACTION_KEY")->AsString = Retval;
    Query->ParamByName("DATE_VALUE")->AsString = Date;
    Query->ParamByName("TENANT_NAME")->AsString = TenantId;
    Query->ParamByName("CURRENT_DATE_VALUE")->AsString = CurrentDate;
    Query->ParamByName("TIME_VALUE")->AsString = Time;
    Query->ParamByName("TERMINAL_NAME")->AsString = TerminalNumber;
    Query->ParamByName("INVOICE_NUMBER")->AsString = InvoiceNumber;
    Query->ParamByName("GROSS_SALES")->AsCurrency = TotalGrossSales;
    Query->ParamByName("VAT_SALES")->AsCurrency = TotalSalesTax;
    Query->ParamByName("NONVAT_SALES")->AsCurrency = TotalNonVatSales;
    Query->ParamByName("VATEXEMPT_SALES")->AsCurrency = TotalVatExemptSales;
    Query->ParamByName("SCDISCOUNT_AMOUNT")->AsCurrency = TotalSeniorCitizensDiscount;
    Query->ParamByName("EMPDISCOUNT_AMOUNT")->AsCurrency = TotalEmployeeDiscount;
    Query->ParamByName("PRODISCOUNT_AMOUNT")->AsCurrency = TotalPromoDiscount;
    Query->ParamByName("OTHERDISCOUNT_AMOUNT")->AsCurrency = TotalOtherDiscount;
    Query->ParamByName("REFUND_AMOUNT")->AsCurrency = TotalRefund;
    Query->ParamByName("SCHARGE_AMOUNT")->AsCurrency = TotalServiceCharge;
    Query->ParamByName("OTHER_SURCHARGE")->AsCurrency = TotalOtherSurcharge;
    Query->ParamByName("NET_SALES")->AsCurrency = TotalNetSales;
    Query->ParamByName("OLD_GRANDTOTAL")->AsCurrency = GrandTotalOld;
    Query->ParamByName("NEW_GRANDTOTAL")->AsCurrency = GrandTotal;
    Query->ParamByName("ZCOUNT")->AsInteger = ZCount;
    Query->ParamByName("CASH_SALES_AMOUNT")->AsCurrency = CashSales;
    Query->ParamByName("CARD_SALES_AMOUNT")->AsCurrency = CardSales;
    Query->ParamByName("CHECK_SALES_AMOUNT")->AsCurrency = CheckSales;
    Query->ParamByName("GC_SALES_AMOUNT")->AsCurrency = GCSales;
    Query->ParamByName("EPAY_SALES_AMOUNT")->AsCurrency = EPaySales;
    Query->ParamByName("OTHER_SALES_AMOUNT")->AsCurrency = OtherSales;
    Query->ParamByName("VOID_AMOUNT")->AsCurrency = TotalCancelled;
    Query->ParamByName("DEPOSIT_AMOUNT")->AsCurrency = TotalDeposit;
    Query->ParamByName("APPLIED_DEPOSIT_AMOUNT")->AsCurrency = TotalAppliedDeposit;
    Query->ParamByName("VAT_SALES_COUNT")->AsInteger = VatableCount;
    Query->ParamByName("NONVAT_SALES_COUNT")->AsInteger = NonVatCount;
    Query->ParamByName("VATEXEMPT_SALES_COUNT")->AsInteger = VatExemptCount;
    Query->ParamByName("SCDISCOUNT_COUNT")->AsInteger = SeniorCitizenDiscountCount;
    Query->ParamByName("EMPDISCOUNT_COUNT")->AsInteger = EmployeeDiscountCount;
    Query->ParamByName("PRODISCOUNT_COUNT")->AsInteger = PromoDiscountCount;
    Query->ParamByName("OTHERDISCOUNT_COUNT")->AsInteger = OtherDiscountCount;
    Query->ParamByName("REFUND_COUNT")->AsInteger = RefundCount;
    Query->ParamByName("SCHARGE_COUNT")->AsInteger = ServiceChargeCount;
    Query->ParamByName("OTHER_SURCHARGE_COUNT")->AsInteger = OtherSurchargeCount;
    Query->ParamByName("CASH_COUNT")->AsInteger = CashSalesCount;
    Query->ParamByName("CARD_COUNT")->AsInteger = CardSalesCount;
    Query->ParamByName("CHECK_COUNT")->AsInteger = CheckSalesCount;
    Query->ParamByName("GC_COUNT")->AsInteger = GCSalesCount;
    Query->ParamByName("EPAY_COUNT")->AsInteger = EPaySalesCount;
    Query->ParamByName("OTHER_COUNT")->AsInteger = OtherSalesCount;
    Query->ParamByName("VOID_COUNT")->AsInteger = CancelledCount;
    Query->ParamByName("TRANS_VOID_COUNT")->AsInteger = CancelledTransactionsCount;
    Query->ParamByName("DEPOSIT_COUNT")->AsInteger = DepositCount;
    Query->ParamByName("APPLIED_DEPOSIT_COUNT")->AsInteger = AppliedDepositCount;
    Query->ParamByName("PATRON_COUNT")->AsInteger = PatronCount;
    Query->ParamByName("TRANSACTION_COUNT")->AsInteger = TransactionCount;
    Query->ParamByName("NOSALE_COUNT")->AsInteger = NoSaleCount;
    Query->ParamByName("SALES_FLAG")->AsString = SalesFlag;
    Query->ParamByName("VAT_PERCENTAGE")->AsCurrency = VatPercentage;

	Query->ExecQuery();

	Transaction.Commit();
	Query->Close();
}
//---------------------------------------------------------------------------

void TMallExportTransactionUpdate::deleteTransactionExportContentsOnZed()
{
	Database::TDBTransaction Transaction(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *Query = Transaction.Query(Transaction.AddQuery());

    Query->SQL->Text = "DELETE FROM MALLEXPORT_TRANSACTION";
	Transaction.StartTransaction();
	Query->ExecQuery();
	Transaction.Commit();
	Query->Close();
}
//---------------------------------------------------------------------------

AnsiString TMallExportTransactionUpdate::extractDate()
{
    return currentDateTime.FormatString("mm-dd-yyyy");
}
//---------------------------------------------------------------------------

AnsiString TMallExportTransactionUpdate::extractTenantId()
{
    return AnsiString(globalSettings->TenantNo);
}
//---------------------------------------------------------------------------

AnsiString TMallExportTransactionUpdate::extractCurrentDate()
{
    return currentDateTime.FormatString("yyyy-mm-dd");
}
//---------------------------------------------------------------------------

AnsiString TMallExportTransactionUpdate::extractTime()
{
    return currentDateTime.FormatString("hh:mm:ss");
}
//---------------------------------------------------------------------------

AnsiString TMallExportTransactionUpdate::extractTerminalNumber()
{
    return TGlobalSettings::Instance().TerminalNo;
}
//---------------------------------------------------------------------------

AnsiString TMallExportTransactionUpdate::extractInvoiceNumber()
{
    if(( InvoiceNumber == "0" ) || ( InvoiceNumber == "" ))
        extractLatestInvoiceNumber( InvoiceNumber );

    return InvoiceNumber;
}
//---------------------------------------------------------------------------

AnsiString TMallExportTransactionUpdate::extractSalesFlag()
{
    AnsiString result = "";

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        if( isRefundedOrder( order ) )
        {
            result = "R";
        }
        else
        {
            result = "S";
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalGrossSales()
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

        grossPrice = order->BillCalcResult.GrossPrice;

        if( isRefundedOrder( order ) )
        {
            grossPrice += fabs(order->BillCalcResult.FinalPrice);
        }
        else if( order->OrderType == CanceledOrder )
        {
            grossPrice += fabs(order->CancelledBillCalcResult.GrossPrice);
        }

        grossPrice += fabs(order->BillCalcResult.TotalDiscount);

        result += grossPrice;
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalSalesTax()
{
    Currency result = 0;
    Currency grossPrice = 0;
    Currency PercentageTax = 0;
    bool Flag = false;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        if((extractTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale )) !=0)
        {
            Flag = true;
        }

        if(TGlobalSettings::Instance().ItemPriceIncludeTax)
        {
            grossPrice = order->BillCalcResult.GrossPrice;
        }
        else
        {
            grossPrice = order->BillCalcResult.GrossPrice + SalesTax;
        }

        if( isRefundedOrder( order ) )
        {
            grossPrice += fabs(order->BillCalcResult.FinalPrice);
        }
        else if( order->OrderType == CanceledOrder )
        {
            grossPrice -= fabs(order->CancelledBillCalcResult.GrossPrice);
        }

        grossPrice += fabs(order->BillCalcResult.TotalDiscount);
        PercentageTax = getPercentageTaxFromResult( order->BillCalcResult, TTaxType::ttSale );

        if(Flag)
        {
            result += (grossPrice / (1 + (PercentageTax / 100)));
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalNonVatSales()
{
    Currency result = 0;
    Currency productTax = 0;
    Currency serviceCharge = 0;
    Currency serviceChargeTax = 0;
    Currency localtax = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        productTax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );
        serviceCharge = order->BillCalcResult.ServiceCharge.Value;
        serviceChargeTax = order->BillCalcResult.ServiceCharge.TaxValue;
        localtax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttLocal );

        if( productTax == 0 && !order->BillCalcResult.PriceTaxExempt)
        {
            result += order->BillCalcResult.FinalPrice - serviceCharge - serviceChargeTax - localtax;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalVatExemptSales()
{
    Currency result = 0;
    Currency productTax = 0;
    Currency serviceCharge = 0;
    Currency serviceChargeTax = 0;
    Currency localtax = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        productTax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );
        serviceCharge = order->BillCalcResult.ServiceCharge.Value;
        serviceChargeTax = order->BillCalcResult.ServiceCharge.TaxValue;
        localtax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttLocal );

        if( productTax == 0 && order->BillCalcResult.PriceTaxExempt)
        {
            result += order->BillCalcResult.FinalPrice - serviceCharge - serviceChargeTax - localtax;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalSeniorCitizensDiscount()
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
            SalesTax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );
            if(SalesTax == 0)
            {
                scResult += order->BillCalcResult.BasePrice * order->GetQty();
            }
        }
        result = scResult - result;
    }
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalEmployeeDiscount()
{
    Currency result = 0;
    result = getDiscountGroupTotal(EMPLOYEE_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalPromoDiscount()
{
    Currency result = 0;
    result = getDiscountGroupTotal(PROMO_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalOtherDiscount()
{
    Currency result = 0;
    result = getDiscountGroupTotal(OTHER_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalRefund()
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

    return fabs(result);
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalServiceCharge()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        result += order->BillCalcResult.ServiceCharge.Value;
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalOtherSurcharge()
{
    Currency result = 0;
    TPayment* payment;
    UnicodeString groupName = "";

    for (int i = 0; i < paymentTransaction->PaymentsCount(); i++)
    {
        payment = paymentTransaction->PaymentGet(i);
        groupName = payment->GetFirstAssignedGroup().Name;
        result += payment->GetSurcharge();
    }
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalNetSales()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    result = ((extractTotalGrossSales() - extractTotalDiscount()) - extractTotalRefund());

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractGrandTotalOld()
{
    Currency result = 0;

    result = CurrToStrF((RoundToNearest(TGlobalSettings::Instance().AmountValue, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractGrandTotal()
{
    Currency result = 0;
    Currency grossPrice = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        grossPrice = 0;

        grossPrice = order->BillCalcResult.FinalPrice;

        result += grossPrice;
    }

    result = result + StrToCurr(TGlobalSettings::Instance().AmountValue);

    TGlobalSettings::Instance().AmountValue = CurrToStr(result);

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
    if(GlobalProfileKey == 0)
    {
        GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
    }
    TManagerVariable::Instance().SetProfileStr(DBTransaction, GlobalProfileKey, vmAmountValue, TGlobalSettings::Instance().AmountValue);
    DBTransaction.Commit();

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractCashSales()
{
    Currency result = getPaymentGroupTotal( CASH_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractCardSales()
{
    Currency result = 0;
    result = getPaymentGroupTotal( CARD_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractCheckSales()
{
    Currency result = 0;
    result = getPaymentGroupTotal( CHECK_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractGCSales()
{
    Currency result = 0;
    result = getPaymentGroupTotal( GIFT_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractEPaySales()
{
    Currency result = 0;
    result = getPaymentGroupTotal( EPAY_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractOtherSales()
{
    Currency result = 0;
    result = getPaymentGroupTotal( OTHER_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalCancelled()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        if( order->OrderType == CanceledOrder )
        {
            if(TGlobalSettings::Instance().MallIndex == ALPHALANDMALL)
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
            else
            {
                result += order->CancelledBillCalcResult.GrossPrice;
            }
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalDeposit()
{
    Currency result = 0;

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalAppliedDeposit()
{
    Currency result = 0;

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractVatPercentage()
{
    Currency result = 0;
    Currency PercentageTax = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        PercentageTax = getPercentageTaxFromResult( order->BillCalcResult, TTaxType::ttSale );

        result = PercentageTax;
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractZCount()
{
    int result = (globalSettings->ZCount);
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractVatableCount()
{
    Currency result = 0;
    Currency productTax = 0;
    Currency serviceCharge = 0;
    Currency serviceChargeTax = 0;
    Currency localtax = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        productTax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );
        serviceCharge = order->BillCalcResult.ServiceCharge.Value;
        serviceChargeTax = order->BillCalcResult.ServiceCharge.TaxValue;
        localtax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttLocal );

        if( productTax != 0 && !order->BillCalcResult.PriceTaxExempt)
        {
            result += 1;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractNonVatCount()
{
    Currency result = 0;
    Currency productTax = 0;
    Currency serviceCharge = 0;
    Currency serviceChargeTax = 0;
    Currency localtax = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        productTax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );
        serviceCharge = order->BillCalcResult.ServiceCharge.Value;
        serviceChargeTax = order->BillCalcResult.ServiceCharge.TaxValue;
        localtax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttLocal );

        if( productTax == 0 && !order->BillCalcResult.PriceTaxExempt)
        {
            result += 1;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractVatExemptCount()
{
    Currency result = 0;
    Currency productTax = 0;
    Currency serviceCharge = 0;
    Currency serviceChargeTax = 0;
    Currency localtax = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        productTax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );
        serviceCharge = order->BillCalcResult.ServiceCharge.Value;
        serviceChargeTax = order->BillCalcResult.ServiceCharge.TaxValue;
        localtax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttLocal );

        if( productTax == 0 && order->BillCalcResult.PriceTaxExempt)
        {
            result += 1;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractSeniorCitizenDiscountCount()
{
    int result = 0;
    result = getDiscountGroupCountPerTransaction(SCD_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractEmployeeDiscountCount()
{
    int result = 0;
    result = getDiscountGroupCountPerTransaction(EMPLOYEE_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractPromoDiscountCount()
{
    int result = 0;
    result = getDiscountGroupCountPerTransaction(PROMO_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractOtherDiscountCount()
{
    int result = 0;
    result = getDiscountGroupCountPerTransaction(OTHER_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractRefundCount()
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

int TMallExportTransactionUpdate::extractServiceChargeCount()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        if(order->BillCalcResult.ServiceCharge.Value != 0)
        {
            result += 1;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractOtherSurchargeCount()
{
    Currency result = 0;
    Currency value = 0;
    TPayment* payment;
    UnicodeString groupName = "";

    for (int i = 0; i < paymentTransaction->PaymentsCount(); i++)
    {
        payment = paymentTransaction->PaymentGet(i);
        groupName = payment->GetFirstAssignedGroup().Name;
        value += payment->GetSurcharge();
        if(value != 0)
        {
            result += 1;
        }
    }
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractCashSalesCount()
{
    int result = getPaymentGroupCount( CASH_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractCardSalesCount()
{
    int result = 0;
    result = getPaymentGroupCount( CARD_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractCheckSalesCount()
{
    int result = 0;
    result = getPaymentGroupCount( CHECK_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractGCSalesCount()
{
    int result = 0;
    result = getPaymentGroupCount( GIFT_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractEPaySalesCount()
{
    int result = 0;
    result = getPaymentGroupCount( EPAY_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractOtherSalesCount()
{
    int result = getPaymentGroupCount( OTHER_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractCancelledCount()
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

int TMallExportTransactionUpdate::extractCancelledTransactionsCount()
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

int TMallExportTransactionUpdate::extractDepositCount()
{
    int result = 0;

    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractAppliedDepositCount()
{
    int result = 0;

    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractPatronCount()
{
    int result = 0;

    std::vector <TPatronType> ::iterator ptrPatronTypes = paymentTransaction->Patrons.begin();
    for ( ; ptrPatronTypes != paymentTransaction->Patrons.end(); ptrPatronTypes++ )
    {
        result += ptrPatronTypes->Count != 0 ? ptrPatronTypes->Count : 0;
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractTransactionCount()
{
    // this method will be called for each transaction
    // the result will be added to whatever the updater has at the moment
    int result = 1;

    return result;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::extractNoSaleCount()
{
    int result = 0;

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalServiceChargeTax()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        result += order->BillCalcResult.ServiceCharge.TaxValue;
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractLocalTaxes()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        result += extractTotalTaxFromResult( order->BillCalcResult, TTaxType::ttLocal );
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractSales()
{
    Currency result = 0;

    // Total Discount, Refund, and Calcel are already deducted in the Gross Sales
    result = (extractTotalGrossSales() - extractTotalServiceCharge() -
              extractTotalServiceChargeTax());
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalTaxFromResult( const TBillCalcResult &result, TTaxType taxType )
{
    Currency value = 0;
    TAX_RESULT_LIST::const_iterator it = result.Tax.begin();

    for( it; it != result.Tax.end(); it++)
    {
        if( it->TaxType == taxType )
            value += it->Value;
    }

    return value;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::extractTotalDiscount()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        result += order->BillCalcResult.TotalDiscount;
    }

    return fabs(result);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Utility methods
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::getTotalTaxFromResult( const TBillCalcResult &result, TTaxType taxType )
{
    Currency value = 0;
    TAX_RESULT_LIST::const_iterator it = result.Tax.begin();

    for( it; it != result.Tax.end(); it++)
    {
        if( it->TaxType == taxType )
            value += it->Value;
    }

    return value;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::getPercentageTaxFromResult( const TBillCalcResult &result, TTaxType taxType )
{
    Currency value = 0;
    TAX_RESULT_LIST::const_iterator it = result.Tax.begin();

    for( it; it != result.Tax.end(); it++)
    {
        if( it->TaxType == taxType )
        {
            value = it->Percentage;
            if(it->Value == 0)
            {
                value = 0;
            }
        }
    }

    return value;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::getPaymentGroupTotal( UnicodeString paymentGroupName )
{
    Currency result = 0;
    TPayment* payment;
    UnicodeString groupName = "";

    for (int i = 0; i < paymentTransaction->PaymentsCount(); i++)
    {
        payment = paymentTransaction->PaymentGet(i);
        groupName = payment->GetFirstAssignedGroup().Name;

        if( payment
            && payment->GetPay() != 0
            && groupName == paymentGroupName)
        {
            result += payment->GetPayTendered() - payment->GetChange();
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportTransactionUpdate::getDiscountGroupTotal( UnicodeString discountGroupName )
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
                if( *it_discount_group_list == discountGroupName )
                {
                    result += it_discount_list->Content;
                    break;
                }
            }
        }
    }

    return result;
}
//---------------------------------------------------------------------------

void TMallExportTransactionUpdate::extractLatestInvoiceNumber( AnsiString &InvoiceNumber )
{
    TIBSQL *query = dbTransaction->Query( dbTransaction->AddQuery());
    query->SQL->Text = "SELECT INVOICE_NUMBER FROM (select DISTINCT a.INVOICE_NUMBER, b.ARCBILL_KEY  from DAYARCBILL a "
                       "left join DAYARCHIVE b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                       "left join DAYARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                       "where a.DISCOUNT = 0  or c.DISCOUNT_GROUPNAME <> 'Non-Chargeable' and c.DISCOUNT_GROUPNAME <> 'Complimentary' "

                       "and c.DISCOUNT_KEY > 0 "
                       "ORDER BY b.ARCBILL_KEY) ";


    query->ExecQuery();

    if(!query->Eof)
    {
        for(; !query->Eof; query->Next())
        {
            InvoiceNumber = query->Fields[0]->AsString;
        }
    }
    else
    {
        InvoiceNumber = "0";
    }
}
//---------------------------------------------------------------------------

void TMallExportTransactionUpdate::flatternOrdersList()
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

bool TMallExportTransactionUpdate::isRefundedOrder( const TItemMinorComplete *order )
{
    return order->OrderType == CreditNonExistingOrder && order->GetQty() < 0;
}
//---------------------------------------------------------------------------

bool TMallExportTransactionUpdate::isCancelledOrder( const TItemMinorComplete *order )
{
    return order->OrderType == CanceledOrder && order->Price() == 0;
}
//---------------------------------------------------------------------------

int TMallExportTransactionUpdate::getDiscountGroupCount( UnicodeString discountGroupName )
{
    int result = 0;
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
                if( *it_discount_group_list == discountGroupName )
                {
                    result ++;
                    break;
                }
            }
        }
    }

    return result;
}
//---------------------------------------------------------------------------


int TMallExportTransactionUpdate::getDiscountGroupCountPerTransaction( UnicodeString discountGroupName )
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

int TMallExportTransactionUpdate::getPaymentGroupCount( UnicodeString paymentGroupName )
{
    int result = 0;
    TPayment* payment;
    UnicodeString groupName = "";

    for (int i = 0; i < paymentTransaction->PaymentsCount(); i++)
    {
        payment = paymentTransaction->PaymentGet(i);
        groupName = payment->GetFirstAssignedGroup().Name;

        if( payment
            && payment->GetPay() != 0
            && groupName == paymentGroupName)
        {
            result ++;
        }
    }

    return result;
}
//---------------------------------------------------------------------------


#pragma package(smart_init)
