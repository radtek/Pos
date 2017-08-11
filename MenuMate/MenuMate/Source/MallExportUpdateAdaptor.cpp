//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportUpdateAdaptor.h"
#include "GlobalSettings.h"
#include "ManagerPatron.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// PUBLIC SECTION
//---------------------------------------------------------------------------
TMallExportUpdateAdaptor::TMallExportUpdateAdaptor()
{
    globalSettings = &TGlobalSettings::Instance();
    mallExportUpdate.reset( new TMMMallExportUpdate() );

    currentDateTime = Now();
    flatternedOrdersList.clear();
}

TMallExportUpdateAdaptor::~TMallExportUpdateAdaptor()
{
    //todo: cleanup if needed
}

TExportUpdateResponse TMallExportUpdateAdaptor::UpdateExportTablesOnTransaction( TPaymentTransaction *payTransac )
{
    TExportUpdateResponse response;

    try
    {
        // clear the values to default
        clearExportFieldValues();

        // set current payment transaction
        setPaymentTransaction( payTransac );

        // set database transaction from payment transaction
        setDbTransaction( &payTransac->DBTransaction );

        // gather all the required information from the transaction
        setMallExportFieldValuesOnTransaction();

        // do the export table update
        updateMallExportFieldValuesOnTransaction();

        // call the updater to save to database
        mallExportUpdate->Commit();
    }
    catch( Exception &ex )
    {
        response.Successful = false;
        response.Message = ex.Message;
        response.Description = "Failed to update mall export table on transaction";
    }

    return response;
}

TExportUpdateResponse TMallExportUpdateAdaptor::UpdateExportTablesOnZed( TFinancialDetails financialDetails, Database::TDBTransaction *dbTransaction )
{
    TExportUpdateResponse response;

    try
    {
        // clear the values to default
        clearExportFieldValues();

        // set database transaction
        setDbTransaction( dbTransaction );

        // gather all the required information from the transaction
        setMallExportFieldValuesOnZed( financialDetails );

        // do the export table update
        updateMallExportFieldValuesOnZed();

        // call the updater to save to database
        mallExportUpdate->Commit();
    }
    catch( Exception &ex )
    {
        response.Successful = false;
        response.Message = ex.Message;
        response.Description = "Failed to update mall export table on zed";
    }

    return response;
}

TExportUpdateResponse TMallExportUpdateAdaptor::UpdateExportTableOnReprint( Currency reprintValue, Database::TDBTransaction *dbTransaction )
{
    TExportUpdateResponse response;

    try
    {
        // clear the values to default
        clearExportFieldValues();

        // set database transaction
        setDbTransaction( dbTransaction );

        // do the export table update
        updateMallExportFieldValuesOnReprint( reprintValue );

        // call the updater to save to database
        mallExportUpdate->Commit();
    }
    catch( Exception &ex )
    {
        response.Successful = false;
        response.Message = ex.Message;
        response.Description = "Failed to update mall export table on reprint receipt";
    }

    return response;
}

TExportUpdateResponse TMallExportUpdateAdaptor::UpdateExportTableOnSettingsChange()
{
    TExportUpdateResponse response;

    try
    {
        // do the export table update
        updateMallExportFieldValuesOnSettingsChange();

        // call the updater to save to database
        mallExportUpdate->Commit();
    }
    catch( Exception &ex )
    {
        response.Successful = false;
        response.Message = ex.Message;
        response.Description = "Failed to update mall export table on settings change";
    }

    return response;
}

TExportUpdateResponse TMallExportUpdateAdaptor::ResetExportTables()
{
    TExportUpdateResponse response;

    try
    {
        // clear the values to default
        clearExportFieldValues();

        // set database transaction
        setDbTransaction( dbTransaction );

        // do the export table update
        resetMallExportFieldValuesOnZed();

        // resets certain fields to defaults on zed
        mallExportUpdate->Reset();

        // call the updater to save to database
        mallExportUpdate->Commit();
    }
    catch( Exception &ex )
    {
        response.Successful = false;
        response.Message = ex.Message;
        response.Description = "Failed to update mall export table on zed";
    }

    return response;
}

//---------------------------------------------------------------------------
// PRIVATE SECTION
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setPaymentTransaction( TPaymentTransaction *payTransac )
{
    paymentTransaction = payTransac;
    flatternOrdersList();   // flatterns the entire set of orders into one list, so the calculations are easier to make
}

void TMallExportUpdateAdaptor::setDbTransaction( Database::TDBTransaction *transaction )
{
    dbTransaction = transaction;

    // set the dbTransaction for updator
    //mallExportUpdate->SetDbTransaction( dbTransaction );
}

void TMallExportUpdateAdaptor::clearExportFieldValues()
{
    terminalNumber = "";
    tenantId = "";
    totalGrossSales = 0;
    totalSalesTax = 0;
    totalCancelled = 0;
    totalCancelledCount = 0;
    totalRegularDiscount = 0;
    totalRegularDiscountCount = 0;
    totalRefund = 0;
    totalRefundCount = 0;
    totalSeniorCitizensDiscount = 0;
    totalSeniorCitizensDiscountCount = 0;
    totalServiceCharge = 0;
    totalEndOfDayOld = 0;
    grandTotalOld = 0;
    totalEndOfDay = 0;
    grandTotal = 0;
    dateTime = extractDateTime();
    noveltySales = 0;
    miscellaneousSales = 0;
    localTaxes = 0;
    chargeCreditSales = 0;
    chargeCreditSalesTax = 0;
    taxExemptSales = 0;
    pharmaSales = 0;
    nonPharmaSales = 0;
    disabilityDiscount = 0;
    salesNotSubjectToRent = 0;
    totalReprintedTransc = 0;
    totalReprintedTranscCount = 0;
    mallCode = "";
    classCode = "";
    tradeCode = "";
    outletNumber = "";
    salesType = "";
    employeeDiscount = 0;
    vipDiscount = 0;
    gpcDiscount = 0;
    discountGroup1 = 0;
    discountGroup2 = 0;
    discountGroup3 = 0;
    discountGroup4 = 0;
    discountGroup5 = 0;
    discountGroup6 = 0;
    otherTax = 0;
    adjustments = 0;
    positiveAdjustments = 0;
    negativeAdjustments = 0;
    positiveAdjustmentsNonTax = 0;
    negativeAdjustmentsNonTax = 0;
    dailySales = 0;
    taxInclusiveSales = 0;
    cashSales = 0;
    giftChequeSales = 0;
    debitCardSales = 0;
    otherTender = 0;
    totalMasterCard = 0;
    totalVisa = 0;
    totalAmericanExpress = 0;
    totalDiners = 0;
    totalJCB = 0;
    totalOtherCards = 0;
    otherCharges = 0;
    firstTransaction = 0;
    lastTransaction = 0;
    transactionCount = 0;
    beginingInvoiceNumber = "";
    endingInvoiceNumber = "";
    cashTransactionsCount = 0;
    giftCardChequeTransactionsCount = 0;
    debitCardTransactionsCount = 0;
    otherTenderTransactionsCount = 0;
    masterCardTransactionsCount = 0;
    visaTransactionsCount = 0;
    americanExpressTransactionsCount = 0;
    dinersTransactionsCount = 0;
    jcbTransactionsCount = 0;
    otherCardTransactionsCount = 0;
    serialNumber              =   "";

    transactionTime           =   currentDateTime;
    transactionDate           =   currentDateTime;
    totalHour                 =   "";
    totalDiscount             =   0;
    beginningORNo             =   0;
    endingORNo                =   0;
    localTaxExemptDailySales  =   0;
    fineDiningCustomerCount   =   0;
    tenderSurcharges          =   0;

    // SM Accreditation
    departmentSum              =   0;

    totalNonVatSales          = 0;
    totalCheckSales           = 0;
    totalEPaySales            = 0;
    totalNoSales              = 0;
    totalCardSales            = 0;
    totalOtherSales           = 0;
    previousEODCounter        = 0;
    currentEODCounter         = 0;
    discountCount             = 0;

    // Sales Type Implementation
    totalSalesTypeFood        = 0;
    totalSalesTypeNonFood     = 0;
    totalSalesTypeGroceries   = 0;
    totalSalesTypeMedicines   = 0;
    totalSalesTypeOthers      = 0;
}

void TMallExportUpdateAdaptor::setMallExportFieldValuesOnTransaction()
{
//    terminalNumber              =   extractTerminalNumber();
//    tenantId                    =   extractTenantId();

    totalGrossSales             =   extractTotalGrossSales();
    totalSalesTax               =   extractTotalSalesTax();
    totalCancelled              =   extractTotalCancelled();
    totalCancelledCount         =   extractTotalCancelledCount();
    totalRegularDiscount        =   extractTotalRegularDiscount();
    totalRegularDiscountCount   =   extractTotalRegularDiscountCount();
    totalRefund                 =   extractTotalRefund();
    totalRefundCount            =   extractTotalRefundCount();
    totalSeniorCitizensDiscount =   extractTotalSeniorCitizensDiscount();
    totalSeniorCitizensDiscountCount = extractTotalSeniorCitizensDiscountCount();
    totalServiceCharge          =   extractTotalServiceCharge();
    dateTime                    =   extractDateTime();
    noveltySales                =   extractNoveltySales();
    miscellaneousSales          =   extractMiscellaneousSales();
    localTaxes                  =   extractLocalTaxes();
    chargeCreditSales           =   extractChargeCreditSales();
    chargeCreditSalesTax        =   extractChargeCreditSalesTax();
    taxExemptSales              =   extractTaxExemptSales();
    pharmaSales                 =   extractPharmaSales();
    nonPharmaSales              =   extractNonPharmaSales();
    disabilityDiscount          =   extractDisabilityDiscount();
    salesNotSubjectToRent       =   extractSalesNotSubjectToRent();

//    totalReprintedTransc        =   extractTotalReprintedTransc();
//    totalReprintedTranscCount   =   extractTotalReprintedTranscCount();
//    mallCode                    =   extractMallCode();
//    classCode                   =   extractClassCode();
//    tradeCode                   =   extractTradeCode();
//    outletNumber                =   extractOutletNumber();

    salesType                 =   extractSalesType();
    employeeDiscount          =   extractEmployeeDiscount();
    vipDiscount               =   extractVipDiscount();
    gpcDiscount               =   extractGpcDiscount();
    discountGroup1            =   extractDiscountGroup1();
    discountGroup2            =   extractDiscountGroup2();
    discountGroup3            =   extractDiscountGroup3();
    discountGroup4            =   extractDiscountGroup4();
    discountGroup5            =   extractDiscountGroup5();
    discountGroup6            =   extractDiscountGroup6();
    otherTax                  =   extractOtherTax();
    adjustments               =   extractAdjustments();
    positiveAdjustments       =   extractPositiveAdjustments();
    negativeAdjustments       =   extractNegativeAdjustments();
    positiveAdjustmentsNonTax =   extractPositiveAdjustmentsNonTax();
    negativeAdjustmentsNonTax =   extractNegativeAdjustmentsNonTax();
    dailySales                =   extractDailySales();
    taxInclusiveSales         =   extractTaxInclusiveSales();
    cashSales                 =   extractCashSales();
    giftChequeSales           =   extractGiftChequeSales();
    debitCardSales            =   extractDebitCardSales();
    otherTender               =   extractOtherTender();
    totalMasterCard           =   extractTotalMasterCard();
    totalVisa                 =   extractTotalVisa();
    totalAmericanExpress      =   extractTotalAmericanExpress();
    totalDiners               =   extractTotalDiners();
    totalJCB                  =   extractTotalJCB();
    totalOtherCards           =   extractTotalOtherCards();
    otherCharges              =   extractOtherCharges();
    firstTransaction          =   extractFirstTransaction();
    lastTransaction           =   extractLastTransaction();
    transactionCount          =   extractTransactionCount();
    beginingInvoiceNumber     =   extractBeginingInvoiceNumber();
    endingInvoiceNumber       =   extractEndingInvoiceNumber();
    cashTransactionsCount          =   extractCashTransactions();
    giftCardChequeTransactionsCount    =   extractGiftCardChequeTransactions();
    debitCardTransactionsCount     =   extractDebitCardTransactions();
    otherTenderTransactionsCount   =   extractOtherTenderTransactions();
    masterCardTransactionsCount    =   extractMasterCardTransactions();
    visaTransactionsCount          =   extractVisaTransactions();
    americanExpressTransactionsCount   =   extractAmericanExpressTransactions();
    dinersTransactionsCount        =   extractDinersTransactions();
    jcbTransactionsCount           =   extractJcbTransactions();
    otherCardTransactionsCount     =   extractOtherCardTransactions();

//      serialNumber              =   extractSerialNumber();

    transactionTime           =   extractTransactionTime();
    transactionDate           =   extractTransactionDate();
    totalHour                 =   extractTotalHour();
    totalDiscount             =   extractTotalDiscount();
    beginningORNo             =   extractBeginningORNo();
    endingORNo                =   extractEndingORNo();
    localTaxExemptDailySales  =   extractLocalTaxExemptDailySales();
    fineDiningCustomerCount   =   extractFineDiningCustomerCount();
    tenderSurcharges          =   extractTenderSurcharges();

    // SM Accreditation
    departmentSum              =  extractDepartmentSum();
    totalServiceChargeTax      =  extractTotalServiceChargeTax();

    totalNonVatSales          = extractTotalNonVatSales();
    totalCheckSales           = extractTotalCheckSales();
    totalEPaySales            = extractTotalEPaySales();
    totalNoSales              = extractTotalNoSales();
    totalCardSales            = extractTotalCardSales();
    totalOtherSales           = extractTotalOtherSales();
    previousEODCounter        = extractPreviousEODCounter();
    currentEODCounter         = extractCurrentEODCounter();
    discountCount             = extractDiscountCount();

    // Sales Type Implementation
    totalSalesTypeFood        = extractTotalSalesTypeFood();
    totalSalesTypeNonFood     = extractTotalSalesTypeNonFood();
    totalSalesTypeGroceries   = extractTotalSalesTypeGroceries();
    totalSalesTypeMedicines   = extractTotalSalesTypeMedicines();
    totalSalesTypeOthers      = extractTotalSalesTypeOthers();
}

void TMallExportUpdateAdaptor::updateMallExportFieldValuesOnTransaction()
{
//    setTerminalNumber();
//    setTenantId();

    setTotalGrossSales();
    setTotalSalesTax();
    setTotalCancelled();
    setTotalCancelledCount();
    setTotalRegularDiscount();
    setTotalRegularDiscountCount();
    setTotalRefund();
    setTotalRefundCount();
    setTotalSeniorCitizensDiscount();
    setTotalSeniorCitizensDiscountCount();
    setTotalServiceCharge();
    setDateTime();

    setNoveltySales();
    setMiscellaneousSales();
    setLocalTaxes();
    setChargeCreditSales();
    setChargeCreditSalesTax();
    setTaxExemptSales();
    setPharmaSales();
    setNonPharmaSales();
    setDisabilityDiscount();
    setSalesNotSubjectToRent();

//    setTotalReprintedTransc();
//    setTotalReprintedTranscCount();
//
//    setMallCode();
//    setClassCode();
//    setTradeCode();
//    setOutletNumber();

    setSalesType();
    setEmployeeDiscount();
    setVipDiscount();
    setGpcDiscount();
    setDiscountGroup1();
    setDiscountGroup2();
    setDiscountGroup3();
    setDiscountGroup4();
    setDiscountGroup5();
    setDiscountGroup6();
    setOtherTax();
    setAdjustments();
    setPositiveAdjustments();
    setNegativeAdjustments();
    setPositiveAdjustmentsNonTax();
    setNegativeAdjustmentsNonTax();
    setDailySales();
    setTaxInclusiveSales();
    setCashSales();
    setGiftChequeSales();
    setDebitCardSales();
    setOtherTender();
    setTotalMasterCard();
    setTotalVisa();
    setTotalAmericanExpress();
    setTotalDiners();
    setTotalJCB();
    setTotalOtherCards();
    setOtherCharges();
    setFirstTransaction();
    setLastTransaction();
    setTransactionCount();
    setBeginingInvoiceNumber();
    setEndingInvoiceNumber();
    setCashTransactions();
    setGiftCardChequeTransactions();
    setDebitCardTransactions();
    setOtherTenderTransactions();
    setMasterCardTransactions();
    setVisaTransactions();
    setAmericanExpressTransactions();
    setDinersTransactions();
    setJcbTransactions();
    setOtherCardTransactions();

//    setSerialNumber();

    setTransactionTime();
    setTransactionDate();
    setTotalHour();
    setTotalDiscount();
    setBeginningORNo();
    setEndingORNo();
    setLocalTaxExemptDailySales();
    setFineDiningCustomerCount();
    setTenderSurcharges();

    // SM Accreditation
    setDepartmentSum();

    setTotalNonVatSales();
    setTotalCheckSales();
    setTotalEPaySales();
    setTotalNoSales();
    setTotalCardSales();
    setTotalOtherSales();
    setPreviousEODCounter();
    setCurrentEODCounter();
    setDiscountCount();

    // Sales Type Implementation
    setSalesTypeFood();
    setSalesTypeNonFood();
    setSalesTypeGroceries();
    setSalesTypeMedicines();
    setSalesTypeOthers();
}

void TMallExportUpdateAdaptor::setMallExportFieldValuesOnZed( TFinancialDetails financialDetails )
{
    totalEndOfDayOld            =   extractTotalEndOfDayOld();
    grandTotalOld               =   extractGrandTotalOld();
    totalEndOfDay               =   extractTotalEndOfDay();
    grandTotal                  =   extractGrandTotal( financialDetails );
    dateTime                    =   extractDateTime();
}

void TMallExportUpdateAdaptor::updateMallExportFieldValuesOnZed()
{
    mallExportUpdate->SaveTotalEndOfDayOld( totalEndOfDayOld );
    mallExportUpdate->SaveGrandTotalOld( grandTotalOld );
    mallExportUpdate->SaveTotalEndOfDay( totalEndOfDay );
    mallExportUpdate->AddGrandTotal( grandTotal );
    mallExportUpdate->SaveDate( Now() );
}

void TMallExportUpdateAdaptor::updateMallExportFieldValuesOnReprint( Currency reprintValue )
{
    mallExportUpdate->AddTotalReprintedTransc( reprintValue );
    mallExportUpdate->AddTotalReprintedTranscCount( 1 );
}

void TMallExportUpdateAdaptor::updateMallExportFieldValuesOnSettingsChange()
{
    mallExportUpdate->SaveTenantNo( globalSettings->TenantNo );
    mallExportUpdate->SaveTerminalNo( globalSettings->TerminalNo );
    mallExportUpdate->SaveClassCode( globalSettings->ClassCode );
    mallExportUpdate->SaveTradeCode( globalSettings->TradeCode );
    mallExportUpdate->SaveOutletNumber( globalSettings->OutletCode );
    mallExportUpdate->SaveMallCode( globalSettings->BranchCode );
    mallExportUpdate->SaveSerialNumber( globalSettings->SerialNo );
}

void TMallExportUpdateAdaptor::resetMallExportFieldValuesOnZed()
{
    mallExportUpdate->SaveTotalReprintedTransc( Currency( 0 ) );
    mallExportUpdate->SaveTotalReprintedTranscCount( 0 );
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Data Extraction Methods
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractTerminalNumber()
{
    return AnsiString(globalSettings->TerminalNo);
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractTenantId()
{
    return AnsiString(globalSettings->TenantNo);
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalGrossSales()
{
    Currency result = 0;
    Currency grossPrice = 0;
    Currency SalesTax = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        bool isrefundItem = false;
        order = *it;
        grossPrice = 0;
        SalesTax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );

        if(TGlobalSettings::Instance().MallIndex == ROBINSONMALL)
        {
            grossPrice = order->BillCalcResult.FinalPrice;
        }
        else if(TGlobalSettings::Instance().MallIndex == AYALAMALL ||
                TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
        {
            grossPrice += order->BillCalcResult.FinalPrice;

            if( isRefundedOrder( order ) )
            {
                grossPrice += fabs(order->BillCalcResult.FinalPrice);
                isrefundItem = true;
            }
            else if( order->OrderType == CanceledOrder )
            {
                grossPrice += fabs(order->CancelledBillCalcResult.GrossPrice);
            }

            if(order->BillCalcResult.FinalPrice >= 0)
            {
               if(!isrefundItem)
                  grossPrice += fabs(order->BillCalcResult.TotalDiscount);
            }
        }
        else if(TGlobalSettings::Instance().MallIndex == POWERPLANTMALL)
        {
            if(SalesTax != 0) {
                grossPrice = order->BillCalcResult.GrossPrice;
            }
            grossPrice += order->BillCalcResult.ServiceCharge.Value;
            grossPrice += order->BillCalcResult.ServiceCharge.TaxValue;
            grossPrice += extractTotalRefund();
        }
        else
        {
            grossPrice = order->BillCalcResult.GrossPrice;
        }
        result += grossPrice;
    }

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result = RoundToNearest(extractDailySales(),0.01,TGlobalSettings::Instance().MidPointRoundsDown) +
                 RoundToNearest(extractTotalSeniorCitizensDiscount(),0.01,TGlobalSettings::Instance().MidPointRoundsDown) +
                 RoundToNearest(extractTotalRegularDiscount(),0.01,TGlobalSettings::Instance().MidPointRoundsDown) +
                 RoundToNearest(extractTotalRefund(),0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }
    else if(TGlobalSettings::Instance().MallIndex == SHANGRILAMALL)
    {
        result = RoundToNearest(extractDailySales(),0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalSalesTax()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        result += getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );

        if(TGlobalSettings::Instance().MallIndex == POWERPLANTMALL)
        {
            result += order->BillCalcResult.ServiceCharge.TaxValue;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalCancelled()
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
//        result += order->BillCalcResult.TotalTax;
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractTotalCancelledCount()
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

Currency TMallExportUpdateAdaptor::extractTotalRegularDiscount()
{
    Currency result = getRegularDiscountGroupsTotal();

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result += extractDisabilityDiscount();
    }
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractTotalRegularDiscountCount()
{
    int result = getRegularDiscountGroupCountPerTransaction();
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalRefund()
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

int TMallExportUpdateAdaptor::extractTotalRefundCount()
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

Currency TMallExportUpdateAdaptor::extractTotalSeniorCitizensDiscount()
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
    else if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractTotalSeniorCitizensDiscountCount()
{
    int result = 0;
    result = getDiscountGroupCountPerTransaction(SCD_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalServiceCharge()
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

int TMallExportUpdateAdaptor::extractTotalEndOfDayOld()
{
    int result = 0;

    mallExportUpdate->ReadTotalEndOfDay( result );

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractGrandTotalOld()
{
    Currency result = 0;

    mallExportUpdate->ReadGrandTotal( result );

    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractTotalEndOfDay()
{
    int result = 0;
    mallExportUpdate->ReadTotalEndOfDay( result );

    result += 1;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractGrandTotal( TFinancialDetails financialDetails )
{
    Currency result = financialDetails.BilledSales.Totals.Total;

    /*if( !globalSettings->ItemPriceIncludeTax )
    {
        result += financialDetails.BilledSales.Totals.SalesTaxContent;
    }*/

    if(( TGlobalSettings::Instance().MallIndex != POWERPLANTMALL ) ||
       ( TGlobalSettings::Instance().MallIndex != ALPHALANDMALL ))
    {
        TIBSQL *query = dbTransaction->Query( dbTransaction->AddQuery());
        query->SQL->Text = "SELECT * FROM MALLEXPORT";
        query->ExecQuery();

        while(!query->Eof)
        {
    	    int MallExportKey = query->FieldByName("MALLEXPORT_KEY")->AsInteger;
            UnicodeString FieldName = query->FieldByName("FIELD_NAME")->AsString;
            UnicodeString StringValue = query->FieldByName("STRING_VALUE")->AsString;
            int IntegerValue = query->FieldByName("INTEGER_VALUE")->AsInteger;
            Currency CurrencyValue = query->FieldByName("CURRENCY_VALUE")->AsCurrency;
            TDateTime TimeStampValue = query->FieldByName("TIMESTAMP_VALUE")->AsDateTime;

            UnicodeString MapVal = "";
            UnicodeString USTimeStampValue = TimeStampValue.DateTimeString();

            if (!(query->FieldByName("STRING_VALUE")->IsNull))
            {
                MapVal = StringValue;
            }
            else if (!(query->FieldByName("INTEGER_VALUE")->IsNull))
            {
                MapVal = IntToStr(IntegerValue);
            }
            else if (!(query->FieldByName("CURRENCY_VALUE")->IsNull))
            {
                MapVal = CurrToStrF((RoundToNearest(CurrencyValue, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);
            }
            else if (!(query->FieldByName("TIMESTAMP_VALUE")->IsNull))
            {
                MapVal = USTimeStampValue;
            }
            else
            {
                MapVal = "0.00";
            }

            DataRead[FieldName] = MapVal;
            query->Next();
        }

        if(TGlobalSettings::Instance().MallIndex == AYALAMALL ||
           TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
        {
            result = 0;

            std::map<UnicodeString, UnicodeString>::iterator it;
            Currency OutputValue = 0;

            for(int i=0;i<DataRead.size();i++)
            {
                switch(i)
                {
                    case 0:
                        it = DataRead.find("DailySales");
                        result += StrToCurr(it->second);
                        break;
                    case 1:
                        it = DataRead.find("TotalSaleTax");
                        result += StrToCurr(it->second);
                        break;
                    default:
                        break;
                }
            }
        }
        else if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
        {
            result = 0;

            std::map<UnicodeString, UnicodeString>::iterator it;
            Currency OutputValue = 0;

            for(int i=0;i<DataRead.size();i++)
            {
                switch(i)
                {
                    case 0:
                        it = DataRead.find("DailySales");
                        result += StrToCurr(it->second);
                        break;
                    default:
                        break;
                }
            }
        }
        else
        {
            result -= (financialDetails.BilledSales.Totals.ServiceChargeTaxContent + financialDetails.BilledSales.Totals.ServiceChargeContent);
        }
    }

    if(TGlobalSettings::Instance().MallIndex == POWERPLANTMALL)
    {
         result += extractTotalDiscountAmount() + extractTotalRefundAmount();
    }

    return result;
}
//---------------------------------------------------------------------------

TDateTime TMallExportUpdateAdaptor::extractDateTime()
{
    return currentDateTime;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractNoveltySales()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractMiscellaneousSales()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractLocalTaxes()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        result += getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttLocal );
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractChargeCreditSales()
{
    Currency result = 0;
    result = getTotalChargeSales();
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractChargeCreditSalesTax()
{
    // assumption: The customer pays the whole amount by the credit card
    Currency result = 0;
    Currency totalChargeCreditPayments = getTotalChargeSales();
    Currency totalCashPayment = getPaymentGroupTotal( CASH_PAYMENT_GROUP );

    bool allPaidByCredit = totalChargeCreditPayments > 0
                            && totalCashPayment == 0;

    if(allPaidByCredit)
    {
        TItemMinorComplete* order;
        std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

        for( ; it != flatternedOrdersList.end(); it++ )
        {
            order = *it;
            result += getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );
//            result += order->BillCalcResult.TotalTax;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTaxExemptSales()
{
    Currency result = 0;
    Currency tax_exempt_total = getTotalTaxExemptSalesExeptSC();
    Currency tax_zero_rated_total = getTotalTaxZeroRatedSalesExeptSC();

    if(TGlobalSettings::Instance().MallIndex == ALPHALANDMALL ||
       TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
    {
        result = tax_exempt_total;
    }
    else
    {
        result = tax_exempt_total + tax_zero_rated_total;
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractPharmaSales()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractNonPharmaSales()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractDisabilityDiscount()
{
    Currency result = 0;
    result = getDiscountGroupTotal(PWD_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractSalesNotSubjectToRent()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalReprintedTransc()
{
    Currency result = 0;

    //todo: implement

    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractTotalReprintedTranscCount()
{
    int result = 0;

    //todo: implement

    return result;
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractMallCode()
{
    return AnsiString(globalSettings->BranchCode);  // MallCode is BranchCode
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractClassCode()
{
    return AnsiString(globalSettings->ClassCode);
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractTradeCode()
{
    return AnsiString(globalSettings->TradeCode);
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractOutletNumber()
{
    return globalSettings->OutletCode;
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractSalesType()
{
    return "SM01";
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractEmployeeDiscount()
{
    Currency result = 0;
    result = getDiscountGroupTotal(EMPLOYEE_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractVipDiscount()
{
    Currency result = 0;
    result = getDiscountGroupTotal(VIP_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractGpcDiscount()
{
    Currency result = 0;
    result = getDiscountGroupTotal(GPC_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractDiscountGroup1()
{
    Currency result = 0;
    result = getDiscountGroupTotal(GRP1_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractDiscountGroup2()
{
    Currency result = 0;
    result = getDiscountGroupTotal(GRP2_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractDiscountGroup3()
{
    Currency result = 0;
    result = getDiscountGroupTotal(GRP3_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractDiscountGroup4()
{
    Currency result = 0;
    result = getDiscountGroupTotal(GRP4_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractDiscountGroup5()
{
    Currency result = 0;
    result = getDiscountGroupTotal(GRP5_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractDiscountGroup6()
{
    Currency result = 0;
    result = getDiscountGroupTotal(GRP6_DISCOUNT_GROUP);
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractOtherTax()
{
    Currency result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;

        result += getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttLocal ) ;
    }

    // Service Charge Tax
//    result += extractTotalServiceChargeTax();
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractAdjustments()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractPositiveAdjustments()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractNegativeAdjustments()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractPositiveAdjustmentsNonTax()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractNegativeAdjustmentsNonTax()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractDailySales()
{
    Currency result = 0;
    result = getDailySales();
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTaxInclusiveSales()
{
    Currency result = 0;
    result = getTotalTaxInclusiveSalesExeptSC();
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractCashSales()
{
    Currency result = getPaymentGroupTotal( CASH_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractGiftChequeSales()
{
    Currency result = getPaymentGroupTotal( GIFT_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractDebitCardSales()
{
    Currency result = getPaymentGroupTotal( DEBIT_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractOtherTender()
{
    Currency result = 0;
    result = getTotalOtherTender();
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalMasterCard()
{
    Currency result = getPaymentGroupTotal( MASTER_CARD_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalVisa()
{
    Currency result = getPaymentGroupTotal( VISA_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalAmericanExpress()
{
    Currency result = getPaymentGroupTotal( AMEX_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalDiners()
{
    Currency result = getPaymentGroupTotal( DINERS_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalJCB()
{
    Currency result = getPaymentGroupTotal( JCB_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalOtherCards()
{
    Currency result = getPaymentGroupTotal( OTHER_CARD_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractOtherCharges()
{
    // NOT IMPLEMENTED due to Insufficient information
    Currency result = 0;
    if(TGlobalSettings::Instance().MallIndex == AYALAMALL)
    {
        Currency productTax = 0;
        Currency totalValue = 0;
        Currency basePrice = 0;

        TItemMinorComplete* order;
        std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

        for( ; it != flatternedOrdersList.end(); it++ )
        {
            order = *it;

            productTax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );


            if( productTax == 0 && order->BillCalcResult.PriceTaxExempt)
            {
                result += order->BillCalcResult.GrossPrice - order->BillCalcResult.BasePrice * order->GetQty();
            }
        }
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractFirstTransaction()
{
    // This always returns number 1 because it is the first transaction number for the day
    int result = 1;
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractLastTransaction()
{
    // this method will be called for each transaction
    // the result will be added to whatever the updater has at the moment
    int result = 0;

    // Power Plant Mall Exclude that count for Refund and Cancell
    if( TGlobalSettings::Instance().MallIndex == POWERPLANTMALL )
    {
        TItemMinorComplete* order;
        std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

        for( ; it != flatternedOrdersList.end(); it++ )
        {
            order = *it;

            if( !isRefundedOrder( order ) && !isCancelledOrder( order ) )
            {
                result = 1;
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

int TMallExportUpdateAdaptor::extractTransactionCount()
{
    // this method will be called for each transaction
    // the result will be added to whatever the updater has at the moment
    int result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    // Power Plant Mall Exclude that count for Refund and Cancel
    if( TGlobalSettings::Instance().MallIndex == POWERPLANTMALL )
    {
        for( ; it != flatternedOrdersList.end(); it++ )
        {
            order = *it;

            if( !isRefundedOrder( order ) && !isCancelledOrder( order ) )
            {
                result = 1;
            }
        }
    }
    // Megaworld Exclude that count for Refund and Cancel
    else if ( TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL )
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

AnsiString TMallExportUpdateAdaptor::extractBeginingInvoiceNumber()
{
    if(( beginingInvoiceNumber == "0" ) || ( beginingInvoiceNumber == "" ))
        extractBeginingAndEndingInvoiceNumbers( beginingInvoiceNumber, endingInvoiceNumber );

    return beginingInvoiceNumber;
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractEndingInvoiceNumber()
{
    if(( endingInvoiceNumber == "0" ) || ( endingInvoiceNumber == "" ))
        extractBeginingAndEndingInvoiceNumbers( beginingInvoiceNumber, endingInvoiceNumber );

    return endingInvoiceNumber;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractCashTransactions()
{
    int result = getPaymentGroupCount( CASH_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractGiftCardChequeTransactions()
{
    int result = getPaymentGroupCount( GIFT_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractDebitCardTransactions()
{
    int result = getPaymentGroupCount( DEBIT_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractOtherTenderTransactions()
{
    int result = 0;
    TPayment* payment;
    UnicodeString groupName = "";
    bool hasPayment = false;
    bool isOtherTender = false;

    for (int i = 0; i < paymentTransaction->PaymentsCount(); i++)
    {
        payment = paymentTransaction->PaymentGet(i);
        groupName = payment->GetFirstAssignedGroup().Name;
        hasPayment = payment->GetPay() != 0;
        isOtherTender = groupName == "";

        if( hasPayment && isOtherTender)
        {
            result ++;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractMasterCardTransactions()
{
    int result = getPaymentGroupCount( MASTER_CARD_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractVisaTransactions()
{
    int result = getPaymentGroupCount( VISA_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractAmericanExpressTransactions()
{
    int result = getPaymentGroupCount( AMEX_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractDinersTransactions()
{
    int result = getPaymentGroupCount( DINERS_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractJcbTransactions()
{
    int result = getPaymentGroupCount( JCB_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractOtherCardTransactions()
{
    int result = getPaymentGroupCount( OTHER_CARD_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractSerialNumber()
{
    return AnsiString( globalSettings->SerialNo );
}
//---------------------------------------------------------------------------

TDateTime TMallExportUpdateAdaptor::extractTransactionTime()
{
    return currentDateTime;
}
//---------------------------------------------------------------------------

TDateTime TMallExportUpdateAdaptor::extractTransactionDate()
{
    return currentDateTime;
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractTotalHour()
{
    AnsiString formattedDateTime = currentDateTime.FormatString("hh:nn");
    return formattedDateTime;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalDiscount()
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
        if(TGlobalSettings::Instance().MallIndex == AYALAMALL)
        {
            if(order->BillCalcResult.TotalDiscount < 0)
            {
                result += order->BillCalcResult.TotalDiscount;
            }
        }
        else
        {
            result += order->BillCalcResult.TotalDiscount;
        }
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
//        result += extractTotalRefund();
//        result += Surcharge;
//        result = RoundToNearest(result,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
    }

    if(TGlobalSettings::Instance().MallIndex == ALPHALANDMALL)
    {
        return result;
    }
    else
    {
        return fabs(result);
    }
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractBeginningORNo()
{
    // NOT IMPLEMENTED due to Insufficient information
    return "";
}
//---------------------------------------------------------------------------

AnsiString TMallExportUpdateAdaptor::extractEndingORNo()
{
    // NOT IMPLEMENTED due to Insufficient information
    return "";
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractLocalTaxExemptDailySales()
{
// SM Accreditation
    Currency result = 0;
    Currency dailySales = getDailySales();

    if(TGlobalSettings::Instance().MallIndex == SMMALL)
    {
        Currency localTaxes = extractOtherTax(); // Other Tax is for SM Mall
    }
    else
    {
        Currency localTaxes = extractLocalTaxes();
    }

    result = dailySales - localTaxes;
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractFineDiningCustomerCount()
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

Currency TMallExportUpdateAdaptor::extractTenderSurcharges()
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

// SM Accreditation
Currency TMallExportUpdateAdaptor::extractDepartmentSum()
{
    Currency result = 0;
    Currency grossPrice = 0;
    Currency taxExemptSales = 0;

    if(TGlobalSettings::Instance().MallIndex == SMMALL)
    {
        Currency localTaxes = extractOtherTax(); // Other Tax is for SM Mall
    }
    else
    {
        Currency localTaxes = extractLocalTaxes();
    }

    taxExemptSales = RoundToNearest(extractTaxExemptSales(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
    grossPrice = (getDailySales() - extractTotalServiceCharge() - localTaxes - extractTotalServiceChargeTax());
    grossPrice = (grossPrice + extractTotalRegularDiscount() + extractEmployeeDiscount() + extractDiscountGroup1() +
                  extractDiscountGroup2() + extractDiscountGroup3() + extractDiscountGroup4() + extractDiscountGroup5() +
                  extractDiscountGroup6() + extractPositiveAdjustments());
    grossPrice = (grossPrice - extractNegativeAdjustments() - taxExemptSales);
    grossPrice = grossPrice - extractTotalSalesTax();
    grossPrice = (grossPrice + taxExemptSales + extractTotalServiceCharge() + localTaxes +
                  extractTotalServiceChargeTax() + extractPositiveAdjustmentsNonTax() + extractNegativeAdjustmentsNonTax());

    result += grossPrice;

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalNonVatSales()
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

    if(TGlobalSettings::Instance().MallIndex == AYALAMALL)
    {
        result = getTotalTaxZeroRatedSalesExeptSC();
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalCheckSales()
{
    Currency result = 0;
    result = getPaymentGroupTotal( CHECK_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalEPaySales()
{
    Currency result = 0;
    result = getPaymentGroupTotal( EPAY_PAYMENT_GROUP );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalNoSales()
{
    Currency result = 0;
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalCardSales()
{
    Currency result = 0;
    result = getPaymentGroupTotal( CARD_PAYMENT_GROUP );

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result += extractTotalEPaySales() + extractTotalOtherCards() + extractTotalJCB() +
                  extractTotalDiners() + extractTotalAmericanExpress() + extractTotalVisa() +
                  extractTotalMasterCard() + extractDebitCardSales();
    }
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalOtherSales()
{
    Currency result = 0;
    result = getPaymentGroupTotal( OTHER_PAYMENT_GROUP );

    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result += extractTotalCheckSales() + extractGiftChequeSales();
    }
    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractPreviousEODCounter()
{
    int result = 0;
    if((globalSettings->ZCount) > 0)
    {
        result = (globalSettings->ZCount) - 1;
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractCurrentEODCounter()
{
    int result = (globalSettings->ZCount);
    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        result = result + 1;
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::extractDiscountCount()
{
    int result = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    for( ; it != flatternedOrdersList.end(); it++ )
    {
        order = *it;
        if(order->BillCalcResult.TotalDiscount != 0)
        {
            result += 1;
        }
    }

    return result;
}
//---------------------------------------------------------------------------

// SM Accreditation
Currency TMallExportUpdateAdaptor::extractTotalServiceChargeTax()
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

Currency TMallExportUpdateAdaptor::extractTotalSalesTypeFood()
{
    Currency result = 0;
    result = getSalesTypeGroupTotal( FOOD_SALES_TYPE );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalSalesTypeNonFood()
{
    Currency result = 0;
    result = getSalesTypeGroupTotal( NONFOOD_SALES_TYPE );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalSalesTypeGroceries()
{
    Currency result = 0;
    result = getSalesTypeGroupTotal( GROCERIES_SALES_TYPE );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalSalesTypeMedicines()
{
    Currency result = 0;
    result = getSalesTypeGroupTotal( MEDICINES_SALES_TYPE );
    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::extractTotalSalesTypeOthers()
{
    Currency result = 0;
    result = getSalesTypeGroupTotal( OTHERS_SALES_TYPE );
    return result;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Data Update Methods
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTerminalNumber()
{
    mallExportUpdate->SaveTerminalNo(terminalNumber);
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTenantId()
{
    mallExportUpdate->SaveTenantNo(tenantId);
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalGrossSales()
{
    mallExportUpdate->SaveTotalGrossSale(totalGrossSales);
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalSalesTax()
{
    mallExportUpdate->SaveTotalSaleTax( totalSalesTax );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalCancelled()
{
    mallExportUpdate->AddTotalCancelled( totalCancelled );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalCancelledCount()
{
    mallExportUpdate->SaveTotalCancelledCount( totalCancelledCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalRegularDiscount()
{
    mallExportUpdate->SaveTotalRegularDiscount( totalRegularDiscount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalRegularDiscountCount()
{
    mallExportUpdate->SaveTotalRegularDiscountCount( totalRegularDiscountCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalRefund()
{
    mallExportUpdate->SaveTotalRefund( totalRefund );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalRefundCount()
{
    mallExportUpdate->SaveTotalRefundCount( totalRefundCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalSeniorCitizensDiscount()
{
    mallExportUpdate->SaveTotalSeniorCitizenDiscount( totalSeniorCitizensDiscount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalSeniorCitizensDiscountCount()
{
    mallExportUpdate->SaveTotalSeniorCitizenDiscountCount( totalSeniorCitizensDiscountCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalServiceCharge()
{
    mallExportUpdate->SaveTotalServiceCharge( totalServiceCharge );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalEndOfDayOld()
{
    mallExportUpdate->SaveTotalEndOfDayOld( totalEndOfDayOld );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setGrandTotalOld()
{
    mallExportUpdate->SaveGrandTotalOld( grandTotalOld );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalEndOfDay()
{
    mallExportUpdate->SaveTotalEndOfDay( totalEndOfDay );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setGrandTotal()
{
    mallExportUpdate->SaveGrandTotal( grandTotal );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDateTime()
{
    mallExportUpdate->SaveDate( dateTime );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setNoveltySales()
{
    mallExportUpdate->SaveNoveltySales( noveltySales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setMiscellaneousSales()
{
    mallExportUpdate->SaveMiscellaneusSales( miscellaneousSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setLocalTaxes()
{
    mallExportUpdate->SaveLocalTaxes( localTaxes );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setChargeCreditSales()
{
    mallExportUpdate->SaveChargeCreditSales( chargeCreditSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setChargeCreditSalesTax()
{
    mallExportUpdate->SaveChargeCreditSalesTax( chargeCreditSalesTax );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTaxExemptSales()
{
    mallExportUpdate->SaveTaxExemptSales( taxExemptSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setPharmaSales()
{
    mallExportUpdate->SavePharmaSales( pharmaSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setNonPharmaSales()
{
    mallExportUpdate->SaveNonPharmaSales( nonPharmaSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDisabilityDiscount()
{
    mallExportUpdate->SaveDisabilityDiscount( disabilityDiscount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setSalesNotSubjectToRent()
{
    mallExportUpdate->SaveSalesNotSubjectToRent( salesNotSubjectToRent );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalReprintedTransc()
{
    mallExportUpdate->SaveTotalReprintedTransc( totalReprintedTransc );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalReprintedTranscCount()
{
    mallExportUpdate->SaveTotalReprintedTranscCount( totalReprintedTranscCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setMallCode()
{
    mallExportUpdate->SaveMallCode( mallCode );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setClassCode()
{
    mallExportUpdate->SaveClassCode( classCode );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTradeCode()
{
    mallExportUpdate->SaveTradeCode( tradeCode );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setOutletNumber()
{
    mallExportUpdate->SaveOutletNumber( outletNumber );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setSalesType()
{
    mallExportUpdate->SaveSalesType( salesType );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setEmployeeDiscount()
{
    mallExportUpdate->SaveEmployeeDiscount( employeeDiscount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setVipDiscount()
{
    mallExportUpdate->SaveVIPDiscount( vipDiscount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setGpcDiscount()
{
    mallExportUpdate->SaveGPCDiscount( gpcDiscount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDiscountGroup1()
{
    mallExportUpdate->SaveDiscountG1( discountGroup1 );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDiscountGroup2()
{
    mallExportUpdate->SaveDiscountG2( discountGroup2 );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDiscountGroup3()
{
    mallExportUpdate->SaveDiscountG3( discountGroup3 );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDiscountGroup4()
{
    mallExportUpdate->SaveDiscountG4( discountGroup4 );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDiscountGroup5()
{
    mallExportUpdate->SaveDiscountG5( discountGroup5 );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDiscountGroup6()
{
    mallExportUpdate->SaveDiscountG6( discountGroup6 );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setOtherTax()
{
    mallExportUpdate->SaveOtherTax( otherTax );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setAdjustments()
{
    mallExportUpdate->SaveAdjustments( adjustments );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setPositiveAdjustments()
{
    mallExportUpdate->SavePositiveAdjustments( positiveAdjustments );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setNegativeAdjustments()
{
    mallExportUpdate->SaveNegativeAdjustments( negativeAdjustments );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setPositiveAdjustmentsNonTax()
{
    mallExportUpdate->SavePositiveAdjustmentsNonTax( positiveAdjustmentsNonTax );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setNegativeAdjustmentsNonTax()
{
    mallExportUpdate->SaveNegativeAdjustmentsNonTax( negativeAdjustmentsNonTax );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDailySales()
{
    mallExportUpdate->SaveDailySales( dailySales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTaxInclusiveSales()
{
    mallExportUpdate->SaveTaxInclusiveSales( taxInclusiveSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setCashSales()
{
    mallExportUpdate->SaveCashSales( cashSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setGiftChequeSales()
{
    mallExportUpdate->SaveGiftChequeSales( giftChequeSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDebitCardSales()
{
    mallExportUpdate->SaveDebitCardSales( debitCardSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setOtherTender()
{
    mallExportUpdate->SaveOtherTender( otherTender );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalMasterCard()
{
    mallExportUpdate->SaveTotalMastercard( totalMasterCard );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalVisa()
{
    mallExportUpdate->SaveTotalVisa( totalVisa );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalAmericanExpress()
{
    mallExportUpdate->SaveTotalAmericanExpress( totalAmericanExpress );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalDiners()
{
    mallExportUpdate->SaveTotallDiners( totalDiners );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalJCB()
{
    mallExportUpdate->SaveTotalJCB( totalJCB );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalOtherCards()
{
    mallExportUpdate->SaveTotalOtherCards( totalOtherCards );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setOtherCharges()
{
    mallExportUpdate->SaveOtherCharges( otherCharges );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setFirstTransaction()
{
    mallExportUpdate->SaveFirstTransaction( firstTransaction );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setLastTransaction()
{
    mallExportUpdate->SaveLastTransaction( lastTransaction );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTransactionCount()
{
    mallExportUpdate->SaveTransactionCount( transactionCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setBeginingInvoiceNumber()
{
    mallExportUpdate->SaveBeginningInvoiceNo( beginingInvoiceNumber );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setEndingInvoiceNumber()
{
    mallExportUpdate->SaveEndingInvoiveNo( endingInvoiceNumber );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setCashTransactions()
{
    mallExportUpdate->SaveCashTransactions( cashTransactionsCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setGiftCardChequeTransactions()
{
    mallExportUpdate->SaveGiftCardChequeTransactions( giftCardChequeTransactionsCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDebitCardTransactions()
{
    mallExportUpdate->SaveDebitCardTransactions( debitCardTransactionsCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setOtherTenderTransactions()
{
    mallExportUpdate->SaveOtherTenderTransctions( otherTenderTransactionsCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setMasterCardTransactions()
{
    mallExportUpdate->SaveMasterCardTransactions( masterCardTransactionsCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setVisaTransactions()
{
    mallExportUpdate->SaveVisaTransactions( visaTransactionsCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setAmericanExpressTransactions()
{
    mallExportUpdate->SaveAmericanExpressTransactions( americanExpressTransactionsCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDinersTransactions()
{
    mallExportUpdate->SaveDinersTransactions( dinersTransactionsCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setJcbTransactions()
{
    mallExportUpdate->SaveJCBTransactions( jcbTransactionsCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setOtherCardTransactions()
{
    mallExportUpdate->SaveOtherCardTransactions( otherCardTransactionsCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setSerialNumber()
{
    mallExportUpdate->SaveSerialNumber( serialNumber );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTransactionTime()
{
    mallExportUpdate->SaveTransactionTime( transactionTime );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTransactionDate()
{
    mallExportUpdate->SaveTransactionDate( transactionDate );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalHour()
{
    mallExportUpdate->SaveTotalHour( totalHour );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalDiscount()
{
    mallExportUpdate->SaveTotalDiscount( totalDiscount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setBeginningORNo()
{
    mallExportUpdate->SaveBeginningORNo( beginningORNo );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setEndingORNo()
{
    mallExportUpdate->SaveEndingORNo( endingORNo );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setLocalTaxExemptDailySales()
{
    mallExportUpdate->SaveLocalTaxExemptDailySales( localTaxExemptDailySales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setFineDiningCustomerCount()
{
    mallExportUpdate->SaveFineDiningCustomerCount( fineDiningCustomerCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTenderSurcharges()
{
    mallExportUpdate->SaveTenderSurcharges( tenderSurcharges );
}
//---------------------------------------------------------------------------

// SM Accreditation
void TMallExportUpdateAdaptor::setDepartmentSum()
{
    mallExportUpdate->SaveDepartmentSum( departmentSum );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalNonVatSales()
{
    mallExportUpdate->SaveTotalNonVatSales( totalNonVatSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalCheckSales()
{
    mallExportUpdate->SaveTotalCheckSales( totalCheckSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalEPaySales()
{
    mallExportUpdate->SaveTotalEPaySales( totalEPaySales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalNoSales()
{
    mallExportUpdate->SaveTotalNoSales( totalNoSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalCardSales()
{
    mallExportUpdate->SaveTotalCardSales( totalCardSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setTotalOtherSales()
{
    mallExportUpdate->SaveTotalOtherSales( totalOtherSales );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setPreviousEODCounter()
{
    mallExportUpdate->SavePreviousEODCounter( previousEODCounter );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setCurrentEODCounter()
{
    mallExportUpdate->SaveCurrentEODCounter( currentEODCounter );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setDiscountCount()
{
    mallExportUpdate->SaveDiscountCount( discountCount );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setSalesTypeFood()
{
    mallExportUpdate->SaveSalesTypeFood( totalSalesTypeFood );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setSalesTypeNonFood()
{
    mallExportUpdate->SaveSalesTypeNonFood( totalSalesTypeNonFood );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setSalesTypeGroceries()
{
    mallExportUpdate->SaveSalesTypeGroceries( totalSalesTypeGroceries );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setSalesTypeMedicines()
{
    mallExportUpdate->SaveSalesTypeMedicines( totalSalesTypeMedicines );
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::setSalesTypeOthers()
{
    mallExportUpdate->SaveSalesTypeOthers( totalSalesTypeOthers );
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Utility methods
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::flatternOrdersList()
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

Currency TMallExportUpdateAdaptor::getTotalTaxFromResult( const TBillCalcResult &result )
{
    Currency value = 0;
    TAX_RESULT_LIST::const_iterator it = result.Tax.begin();

    for( it; it != result.Tax.end(); it++)
    {
        value += it->Value;
    }

    return value;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::getTotalTaxFromResult( const TBillCalcResult &result, TTaxType taxType )
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

Currency TMallExportUpdateAdaptor::getTotalTaxExemptSalesExeptSC()
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

Currency TMallExportUpdateAdaptor::getTotalTaxZeroRatedSalesExeptSC()
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

Currency TMallExportUpdateAdaptor::getTotalTaxInclusiveSalesExeptSC()
{
    // SM Accreditation
    Currency result = 0;
    Currency taxIncSales = 0;
    Currency taxExemptSales = 0;
    Currency productTax = 0;

    if(TGlobalSettings::Instance().MallIndex == SMMALL)
    {
        Currency localTaxes = extractOtherTax(); // Other Tax is for SM Mall
    }
    else
    {
        Currency localTaxes = extractLocalTaxes();
    }

    taxExemptSales = RoundToNearest(extractTaxExemptSales(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);

    if(TGlobalSettings::Instance().MallIndex == AYALAMALL ||
       TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
    {
        taxIncSales = (getDailySales() - localTaxes);
        taxIncSales = (taxIncSales - taxExemptSales);

        result += taxIncSales;
    }
    else
    {
        taxIncSales = (getDailySales() - extractTotalServiceCharge() - localTaxes - extractTotalServiceChargeTax());
        taxIncSales = (taxIncSales + extractTotalRegularDiscount() + extractEmployeeDiscount() + extractDiscountGroup1() +
                       extractDiscountGroup2() + extractDiscountGroup3() + extractDiscountGroup4() + extractDiscountGroup5() +
                       extractDiscountGroup6() + extractPositiveAdjustments());
        taxIncSales = (taxIncSales - extractNegativeAdjustments() - taxExemptSales);

        result += taxIncSales;
    }

    return result;
}
//---------------------------------------------------------------------------

bool TMallExportUpdateAdaptor::isRefundedOrder( const TItemMinorComplete *order )
{
    return order->OrderType == CreditNonExistingOrder && order->GetQty() < 0;
}
//---------------------------------------------------------------------------

bool TMallExportUpdateAdaptor::isCancelledOrder( const TItemMinorComplete *order )
{
    return order->OrderType == CanceledOrder && order->Price() == 0;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::getTotalChargeSales()
{
    Currency result = 0;
    Currency total_credit_payments = getTotalPaymentByCredit();
    Currency other_card_total = getPaymentGroupTotal( OTHER_CARD_PAYMENT_GROUP );
    Currency gift_card_total = getPaymentGroupTotal( GIFT_PAYMENT_GROUP );
    Currency other_tender_total = getTotalOtherTender();

    result = total_credit_payments + other_card_total + other_tender_total + gift_card_total;

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::getTotalPaymentByCredit()
{
    Currency result = 0;
    Currency visa_total = getPaymentGroupTotal( VISA_PAYMENT_GROUP );
    Currency master_card_total = getPaymentGroupTotal( MASTER_CARD_PAYMENT_GROUP );
    Currency debit_total = getPaymentGroupTotal( DEBIT_PAYMENT_GROUP );
    Currency amex_card_total = getPaymentGroupTotal( AMEX_PAYMENT_GROUP );
    Currency diners_total = getPaymentGroupTotal( DINERS_PAYMENT_GROUP );
    Currency jcb_total = getPaymentGroupTotal( JCB_PAYMENT_GROUP );

    result = visa_total + master_card_total + debit_total + amex_card_total + diners_total + jcb_total;

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::getTotalOtherTender()
{
    Currency result = 0;
    TPayment* payment;
    UnicodeString groupName = "";

    for (int i = 0; i < paymentTransaction->PaymentsCount(); i++)
    {
        payment = paymentTransaction->PaymentGet(i);
        groupName = payment->GetFirstAssignedGroup().Name;

        if( payment->GetPay() != 0
            && groupName == "")
        {
            result += payment->GetPayTendered() - payment->GetChange();
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::getPaymentGroupTotal( UnicodeString paymentGroupName )
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

int TMallExportUpdateAdaptor::getPaymentGroupCount( UnicodeString paymentGroupName )
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

Currency TMallExportUpdateAdaptor::getRegularDiscountGroupsTotal()
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
            if( it_discount_list->DiscountGroupList.empty() )
            {
                // if there are no discount groups attached, we take it as a regular discount
                result += it_discount_list->Content;
            }
            else
            {
                if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
                {
                    it_discount_group_list = it_discount_list->DiscountGroupList.begin();
                    for( ; it_discount_group_list != it_discount_list->DiscountGroupList.end(); it_discount_group_list++ ) // for each group in discount result
                    {
                        if( *it_discount_group_list != SCD_DISCOUNT_GROUP
                            &&  *it_discount_group_list != PWD_DISCOUNT_GROUP )
                        {
                            result += it_discount_list->Content;
                            continue;
                        }
                    }
                }
            }
        }
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::getDiscountGroupTotal( UnicodeString discountGroupName )
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
                    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
                    {
                        result += RoundToNearest(it_discount_list->Content,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
                        break;
                    }
                    else
                    {
                        result += it_discount_list->Content;
                        break;
                    }
                }
            }
        }
    }

    return result;
}
//---------------------------------------------------------------------------

int TMallExportUpdateAdaptor::getDiscountGroupCount( UnicodeString discountGroupName )
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
//                discGroup = (TDiscountGroup*)(*it_discount_group_list);
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

int TMallExportUpdateAdaptor::getDiscountGroupCountPerTransaction( UnicodeString discountGroupName )
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

int TMallExportUpdateAdaptor::getRegularDiscountGroupCountPerTransaction( )
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
            if( it_discount_list->DiscountGroupList.empty() && !discountGroupFound)
            {
                result ++;
                discountGroupFound = true;
            }
            else
            {
                it_discount_group_list = it_discount_list->DiscountGroupList.begin();
                for( ;
                     it_discount_group_list != it_discount_list->DiscountGroupList.end() && !discountGroupFound;
                     it_discount_group_list++ ) // for each group in discount result
                {
                    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
                    {
                        if( *it_discount_group_list != SCD_DISCOUNT_GROUP )
                        {
                            result ++;
                            discountGroupFound = true;
                            break;
                        }
                    }
                    else
                    {
                        if( *it_discount_group_list != SCD_DISCOUNT_GROUP
                            &&  *it_discount_group_list != PWD_DISCOUNT_GROUP )
                        {
                            result ++;
                            discountGroupFound = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    return result;
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::extractBeginingAndEndingInvoiceNumbers( AnsiString &beginInvoiceNum, AnsiString &endInvoiceNum )
{
     TIBSQL *query = dbTransaction->Query( dbTransaction->AddQuery());

     query->SQL->Text = "SELECT INVOICE_NUMBER FROM (select DISTINCT a.INVOICE_NUMBER, b.ARCBILL_KEY  from DAYARCBILL a "
                     "left join DAYARCHIVE b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                     "left join DAYARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                     "where a.DISCOUNT >= 0  or c.DISCOUNT_GROUPNAME <> 'Non-Chargeable' and c.DISCOUNT_GROUPNAME <> 'Complimentary' "

                     "and c.DISCOUNT_KEY > 0 "
                     "ORDER BY b.ARCBILL_KEY) ";


    query->ExecQuery();

    if(!query->Eof)
    {
        beginInvoiceNum = query->Fields[0]->AsString;
        for(; !query->Eof; query->Next())
        {
            endInvoiceNum = query->Fields[0]->AsString;
        }
    }
    else
    {
        beginInvoiceNum = "0";
        endInvoiceNum = "0";
    }
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::getDailySales()
{
    Currency result = 0;
    Currency SalesTax = 0;
    TPayment* payment;
    UnicodeString groupName = "";
    UnicodeString payGroupName = "";
    Currency Surcharge = 0;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

    // AYALA Accreditation
    if(TGlobalSettings::Instance().MallIndex == AYALAMALL)
    {
        for( ; it != flatternedOrdersList.end(); it++ )
        {
            order = *it;
            SalesTax = getTotalTaxFromResult( order->BillCalcResult, TTaxType::ttSale );

            result += order->BillCalcResult.FinalPrice;
            result -= order->BillCalcResult.ServiceCharge.Value;
            result -= order->BillCalcResult.ServiceCharge.TaxValue;
            result -= SalesTax;
        }
    }
    else if((TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL || TGlobalSettings::Instance().MallIndex == SHANGRILAMALL))
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
    else if(TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
    {
        result = extractTotalGrossSales() - extractTotalDiscount() - extractTotalRefund() - extractTotalCancelled() - extractTotalSalesTax() - extractTotalServiceCharge();
    }
    else
    {
    // SM Accreditation
        result = extractCashSales() + extractChargeCreditSales();
    }

    return result;
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::getDailySalesLocalTaxes()
{
    Currency result = 0;

    result = (getDailySales() - extractLocalTaxes());

    return result;
}
//---------------------------------------------------------------------------

void TMallExportUpdateAdaptor::getSalesTypeQuery(int item_id)
{
    int ITEM_ID = item_id;
    int salesKeys = 0;
    UnicodeString salesName = "";

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    TIBSQL *salestypeKeyQuery = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL *salestypeNameQuery = DBTransaction.Query(DBTransaction.AddQuery());
    salestypeKeyQuery->Close();
    salestypeKeyQuery->SQL->Text = "SELECT STIR.SALES_TYPE_KEY FROM SALES_TYPE_ITEMS_RELATION STIR WHERE STIR.ITEM_KEY = :ITEM_ID";
    salestypeKeyQuery->ParamByName("ITEM_ID")->AsInteger = ITEM_ID;
    salestypeKeyQuery->ExecQuery();

	while(!salestypeKeyQuery->Eof)
	{
        salesKeys = salestypeKeyQuery->FieldByName("SALES_TYPE_KEY")->AsInteger;

        salestypeNameQuery->Close();
        salestypeNameQuery->SQL->Text = "SELECT FIRST 1 ST.SALES_TYPE_NAME FROM SALES_TYPE ST WHERE ST.SALES_TYPE_KEY = :SALES_TYPE_ID";
        salestypeNameQuery->ParamByName("SALES_TYPE_ID")->AsInteger = salesKeys;
        salestypeNameQuery->ExecQuery();

        salesName = salestypeNameQuery->FieldByName("SALES_TYPE_NAME")->AsString;

        stName.push_back(salesName.t_str());

        salestypeKeyQuery->Next();
    }

    DBTransaction.Commit();
}
//---------------------------------------------------------------------------

Currency TMallExportUpdateAdaptor::getSalesTypeGroupTotal( UnicodeString salestypeGroupName )
{
    Currency result = 0;
    Currency SalesTax = 0;
    UnicodeString SalesType = "";

    TPayment* payment;
    UnicodeString groupName = "";
    UnicodeString payGroupName = "";
    Currency Surcharge = 0;
    bool surChargeFlag = false;

    std::map<UnicodeString, UnicodeString>::iterator itST;

    TItemMinorComplete* order;
    std::vector<TItemMinorComplete*>::iterator it = flatternedOrdersList.begin();

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
        getSalesTypeQuery(order->Item_ID);

        for(int i=0;i<stName.size();i++)
        {
            SalesType = stName.at(i).t_str();

            if(SalesType == salestypeGroupName)
            {
                surChargeFlag = true;
                result += order->BillCalcResult.FinalPrice;
            }
        }
        stName.clear();
    }

    if(surChargeFlag)
    {
        result += Surcharge;
    }

    return result;
}
//---------------------------------------------------------------------------
Currency TMallExportUpdateAdaptor::extractTotalDiscountAmount()
{
    Currency result = 0;

    mallExportUpdate->ReadTotalDiscount(result);

    return result;
}
//---------------------------------------------------------------------------
Currency TMallExportUpdateAdaptor::extractTotalRefundAmount()
{
    Currency result = 0;

    mallExportUpdate->ReadTotalRefund(result);

    return result;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)


