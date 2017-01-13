#include "ReportUtilities.h"
#include "ManagerReports.h"
#include "SecurityReference.h"
#include "MMMessageBox.h"
#include "Printout.h"
__fastcall TTransactionCount::TTransactionCount()
{
	Count = 0;
	Counted = false;
	CashOutCount = 0;
	CashOutCounted = false;
	SurchargeCount = 0;
	SurchargeCounted = false;
}

int DataCalculationUtilities::GetZedKey(Database::TDBTransaction &dbTransaction)
{
    int zKey;

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    ibInternalQuery->Close();
    ibInternalQuery->SQL->Text = "SELECT MAX(Z_KEY) Z_KEY FROM ZEDS ";
    ibInternalQuery->ExecQuery();
    zKey = ibInternalQuery->Fields[0]->AsInteger;

    return zKey;
}

TCalculatedTotals DataCalculationUtilities::GetCashDrawerOpenTotals(Database::TDBTransaction &dbTransaction, TGlobalSettings* globalSettings, UnicodeString deviceName)
{
    Currency totalVoidSales = 0;
    TDateTime previousZedTime = Now();
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

    ibInternalQuery->Close();
    ibInternalQuery->SQL->Text = "select COUNT(SECURITY_KEY) VOIDS from SECURITY where "
                                    "SECURITY.TIME_STAMP > :PrevZedTime " "AND SECURITY.SECURITY_EVENT = '" +
                                    UnicodeString(SecurityTypes[secOpenDraw]) + "' ";

    if(!globalSettings->EnableDepositBagNum)
    {
        ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + " AND SECURITY.TERMINAL_NAME = :TERMINAL_NAME;";
        ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
    }

    ibInternalQuery->ParamByName("PrevZedTime")->AsDateTime = previousZedTime;
    ibInternalQuery->ExecQuery();

    totalVoidSales = ibInternalQuery->FieldByName("VOIDS")->AsCurrency;
    TCalculatedTotals cashDrawerOpensTotal(etcTotalCashDrawerOpens, 0, 0, 0, totalVoidSales);

    return cashDrawerOpensTotal;
}

Currency DataCalculationUtilities::GetAccumulatedZedTotal(Database::TDBTransaction &dbTransaction)
{
	TIBSQL *qrAccumulatedTotal = dbTransaction.Query(dbTransaction.AddQuery());
    Currency accumulatedTotal;

	qrAccumulatedTotal->SQL->Text = "SELECT SUM(TERMINAL_EARNINGS) AS TOTAL FROM ZEDS";

	qrAccumulatedTotal->ExecQuery();

	if(qrAccumulatedTotal->FieldByName("TOTAL")->IsNull)
    {
		accumulatedTotal = 0;
	}
    else
    {
		accumulatedTotal = qrAccumulatedTotal->FieldByName("TOTAL")->AsCurrency;
	}
	qrAccumulatedTotal->Close();

	return accumulatedTotal;
}


Currency DataCalculationUtilities::GetTotalEarnings(Database::TDBTransaction &dbTransaction, UnicodeString deviceName, bool showendingbal)
{
    try
    {
        TTransactionInfo TransactionInfo;

        Currency groupGrandTotal = 0;
        Currency currentFloat = 0;
        Currency currentSkimsTotal = 0;
        Currency skims = 0;
        Currency refloats = 0;
        Currency cashWithdrawl = 0;
        int CashWithdrawlCount = 0;

        if(TGlobalSettings::Instance().UseBIRFormatInXZReport || showendingbal)
        {
           TTransactionInfoProcessor::Instance().RemoveEntryFromMap(deviceName);
           showendingbal = false;
        }
        TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfo(dbTransaction, deviceName, showendingbal);
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

        ibInternalQuery->Close();

        ibInternalQuery->SQL->Text =  "SELECT Refloat_skim.Transaction_Type, refloat_Skim.amount, refloat_skim.IS_FLOAT_WITHDRAWN_FROM_CASH FROM refloat_skim "
        "Left join zeds on refloat_skim.z_key = zeds.z_key "
        "WHERE zeds.TERMINAL_NAME = :TERMINAL_NAME AND zeds.TIME_STAMP IS NULL "
        "and (refloat_skim.transaction_type = 'Withdrawal' or refloat_skim.transaction_type = 'Deposit')";

        ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
        ibInternalQuery->ExecQuery();

        for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
        {
            if(ibInternalQuery->FieldByName("Transaction_Type")->AsString == "Withdrawal" && ibInternalQuery->FieldByName("IS_FLOAT_WITHDRAWN_FROM_CASH")->AsString == "T" )
            {
                cashWithdrawl += ibInternalQuery->FieldByName("amount")->AsCurrency;
            }
            else if(ibInternalQuery->FieldByName("Transaction_Type")->AsString == "Withdrawal")
            {
                skims += ibInternalQuery->FieldByName("amount")->AsCurrency;
            }
            else
            {
                refloats += ibInternalQuery->FieldByName("amount")->AsCurrency;
            }
        }

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "SELECT " "* FROM ZEDS " "WHERE " "TERMINAL_NAME = :TERMINAL_NAME AND " "TIME_STAMP IS NULL";
        ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
        ibInternalQuery->ExecQuery();

        if (ibInternalQuery->RecordCount)
        {
            currentFloat = ibInternalQuery->FieldByName("INITIAL_FLOAT")->AsCurrency;
            currentSkimsTotal = ibInternalQuery->FieldByName("SKIMS_TOTAL")->AsCurrency;
        }

        std::map <int, std::map <UnicodeString, TSumPayments> > ::iterator itPayments;
        for (itPayments = TransactionInfo.Payments.begin(); itPayments != TransactionInfo.Payments.end(); itPayments++)
        {
            Currency roundingTotal = 0;
            Currency cashValue = 0;
            Currency changeValue = 0;

            std::map <UnicodeString, TSumPayments> PaymentValues = itPayments->second;
            if (itPayments->first == 0)
            {
                groupGrandTotal += currentFloat;
                groupGrandTotal += currentSkimsTotal;
                std::map <UnicodeString, TSumPayments> ::iterator itCurrentPayment = PaymentValues.find(CASH);
                if (itCurrentPayment != PaymentValues.end())
                {
                    groupGrandTotal += itCurrentPayment->second.Total;
                    roundingTotal += itCurrentPayment->second.Rounding;
                    groupGrandTotal -= itCurrentPayment->second.CashOut;
                    cashValue = itCurrentPayment->second.Total;
                }
                itCurrentPayment = PaymentValues.find(CHANGE);
                if (itCurrentPayment != PaymentValues.end())
                {
                    groupGrandTotal += itCurrentPayment->second.Total;
                    changeValue += itCurrentPayment->second.Total;
                    roundingTotal += itCurrentPayment->second.Rounding;
                }
            }
            std::map <UnicodeString, TSumPayments> ::iterator itCurrentPayment;
            for (itCurrentPayment = PaymentValues.begin(); itCurrentPayment != PaymentValues.end(); itCurrentPayment++)
            {
                if (itCurrentPayment->second.Name.UpperCase() != UpperCase(CHANGE) &&
                        itCurrentPayment->second.Name.UpperCase() != UpperCase(CREDIT) &&
                        !(itCurrentPayment->second.Properties & ePayTypeGetVoucherDetails) &&
                        !(itCurrentPayment->second.Properties & ePayTypeCredit) &&
                        itCurrentPayment->second.Name.UpperCase() != UpperCase(CASH))
                {
                    groupGrandTotal += itCurrentPayment->second.CashOut;
                    groupGrandTotal += itCurrentPayment->second.Total;
                }
            }
            if (groupGrandTotal != 0 && itPayments->first == 0)
            {
                groupGrandTotal -= (currentFloat + currentSkimsTotal - cashWithdrawl);
            }
        }

        return groupGrandTotal;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

TDateTime DataCalculationUtilities::GetPreviousZedTimeForTerminal(Database::TDBTransaction &dbTransaction, UnicodeString terminalName)
{
    TDateTime PrevZedTime = Now();
    TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT " "MAX(TIME_STAMP)TIME_STAMP FROM ZEDS " "WHERE " "TERMINAL_NAME = :TERMINAL_NAME";
    IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = terminalName;

    IBInternalQuery->ExecQuery();

    if (IBInternalQuery->RecordCount != 0)
    {
            PrevZedTime = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
    }
    IBInternalQuery->Close();

    return PrevZedTime;
}

TTransactionInfo TTransactionInfoProcessor::GetBalanceInfo(TBlindBalances &balance)
{
    TTransactionInfo TransactionInfo;

	std::map <int, std::map <UnicodeString, TSumPayments> > ::iterator itPayments;
	for (itPayments = TransactionInfo.Payments.begin(); itPayments != TransactionInfo.Payments.end(); itPayments++)
	{
		int paymentGroup = itPayments->first;
		std::map <UnicodeString, TSumPayments> PaymentValues = itPayments->second;

		std::map <UnicodeString, TSumPayments> ::iterator itCurrentPayment;
		for (itCurrentPayment = PaymentValues.begin(); itCurrentPayment != PaymentValues.end(); itCurrentPayment++)
		{
			std::map <UnicodeString, TTransactionCount> TransactionsCount = TransactionInfo.TransactionsCountGroups[paymentGroup];
			TTransactionCount transaction = TransactionsCount[itCurrentPayment->second.Name];
			TBlindBalance currentBalance(itCurrentPayment->first, paymentGroup, transaction.Count, 0, itCurrentPayment->second.Total);
			balance.SetBalance(currentBalance);
		}
	}
    return TransactionInfo;
}

 TTransactionInfo TTransactionInfoProcessor::GetTransactionInfo(Database::TDBTransaction &dbTransaction, UnicodeString deviceName, bool showendingbal)
{
    if(!deviceTransactions[deviceName])                                             // checks if the object is already present for the given terminal
    {
        deviceTransactions[deviceName] = new TTransactionInfo;
        TTransactionInfo* TransactionInfo = deviceTransactions[deviceName];

        TransactionInfo->TotalSpentInSavedSales = 0;
        TransactionInfo->PatronCountInSavedSales = 0;
        TransactionInfo->TotalSpentInCashSales = 0;
        TransactionInfo->PatronCountInCashSales = 0;
        TransactionInfo->TotalSpentInCreditedSales = 0;
        TransactionInfo->PatronCountInCreditedSales = 0;
        TransactionInfo->TotalSpentInTabSales = 0;
        TransactionInfo->PatronCountInTabSales = 0;
        TransactionInfo->TotalSpentInRoomSales = 0;
        TransactionInfo->PatronCountInRoomSales = 0;
        TransactionInfo->TotalSpentInAccountSales = 0;
        TransactionInfo->PatronCountInAccountSales = 0;
        TransactionInfo->PatronCountInPurchasingCredit = 0;
        TransactionInfo->TotalCreditRedeemed = 0;
        TransactionInfo->TotalSalesTax = 0;
        TransactionInfo->TotalServiceCharge = 0;
        TransactionInfo->TotalServiceChargeTax = 0;

        // MM-1282 : hack for multiple invoices
        // to store the invoice numbers counted. by having this we can avoid increasing the transaction count but the totals
        std::set<AnsiString> countedInvoiceNumbers;
        countedInvoiceNumbers.clear();

        AnsiString currentInvoiceKey = 0;

        TIBSQL *qrXArcBill = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qrXArcPay = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qXArcSurcharge = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qrXArcOrder = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qXArcPoints = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qXArcPoints1 = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qXArcTax = dbTransaction.Query(dbTransaction.AddQuery());

        qrXArcBill->Close();

        AnsiString terminalNamePredicate = "";

        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            terminalNamePredicate = " a.TERMINAL_NAME = :TERMINAL_NAME AND";
        }

        NormalZedTransaction(qrXArcBill, showendingbal);

        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            qrXArcBill->ParamByName("TERMINAL_NAME")->AsString = deviceName;
        }

        qrXArcBill->ExecQuery();


        int i=1;
        double first_for_total_time=0;

        qXArcPoints->Close();
        /*qXArcPoints->SQL->Text = "SELECT * from POINTSTRANSACTIONS a where a.TIME_STAMP >= (select coalesce(max(b.Time_Stamp),'28.12.1889, 19:39:25.000') from ZEDS b where b.TERMINAL_NAME = :ZTERMINAL_NAME)" ;
        qXArcPoints->ParamByName("ZTERMINAL_NAME")->AsString = deviceName;*/
        GetPointsForNormalZed(qXArcPoints, deviceName);
        qXArcPoints->ExecQuery();

        std::map<AnsiString,TPointTransactions> pointTransaction;

        for ( ;!qXArcPoints->Eof; qXArcPoints->Next())
        {


            AnsiString invoiceNumber = qXArcPoints->FieldByName("INVOICE_NUMBER")->AsString;

            TPointTransaction point;
             point.adjustmentType= qXArcPoints->FieldByName("ADJUSTMENT_TYPE")->AsInteger;
            point.adjustmentSubType= qXArcPoints->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger;
            point.adjustment= qXArcPoints->FieldByName("ADJUSTMENT")->AsCurrency;
            point.invoiceNo= qXArcPoints->FieldByName("INVOICE_NUMBER")->AsString;

            pointTransaction[invoiceNumber].PointsTransactions.push_back(point);

        }

        for ( ;!qrXArcBill->Eof; qrXArcBill->Next())
        {
            currentInvoiceKey = qrXArcBill->FieldByName("INVOICE_NUMBER")->AsString;

            if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eTableSeat)
            {
                TransactionInfo->TotalSpentInSavedSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInSavedSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eTab)
            {
                TransactionInfo->TotalSpentInTabSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInTabSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eAccount)
            {
                TransactionInfo->TotalSpentInAccountSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInAccountSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eRoomSale)
            {
                TransactionInfo->TotalSpentInRoomSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInRoomSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eCash)
            {
                TransactionInfo->TotalSpentInCashSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInCashSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eCredited)
            {
                TransactionInfo->TotalSpentInCreditedSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInCreditedSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eCreditPurchase)
            {
                // This will always be zero we check each ArcBillPay and pull out the 'cash' payment.
                // and add it to this figure.
            }

           // before = Now();    //arun
            qrXArcPay->Close();
            GetArcPayForNormalZed(qrXArcPay);
            /*qrXArcPay->SQL->Text = "select ARCBILL_KEY, PAY_TYPE, SUBTOTAL, CASH_OUT, VOUCHER_NUMBER,TAX_FREE,"
                                    "GROUP_NUMBER, PROPERTIES,ROUNDING,TIP_AMOUNT,PAYMENT_CARD_TYPE from DAYARCBILLPAY "
                                    "where ARCBILL_KEY = :ARCBILL_KEY AND SUBTOTAL != 0";*/
            //}
            qrXArcPay->ParamByName("ARCBILL_KEY")->AsInteger = qrXArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
            qrXArcPay->ExecQuery();

            bool loop = true;
            LoadArcPayTransaction(TransactionInfo, qrXArcPay, loop, qrXArcBill,pointTransaction, countedInvoiceNumbers, currentInvoiceKey);

            /************************************************************************************************
            * This query takes a few seconds to process with large points transactions tables which inside *
            * this loop adds up to a lot of time on busy night (1 hour+). As this is only for Aus Loyalty  *
            * it will be used with it enabled for now but in future the Aus Loyalty sites will lag and the *
            * query will need to be looked at.                                                             *
            ************************************************************************************************/
            //Removing EnableSeperateEarntPts condition as discussed with ravish
            //if(TGlobalSettings::Instance().EnableSeperateEarntPts)
            {




                AnsiString invoiceNumber = qrXArcBill->FieldByName("INVOICE_NUMBER")->AsString;
                std::map<AnsiString, TPointTransactions>::iterator iter  = pointTransaction.find(invoiceNumber);
                 if(iter != pointTransaction.end())
                 {
                    for (std::vector<TPointTransaction>::iterator ptrPoints = iter->second.PointsTransactions.begin();
                          ptrPoints != iter->second.PointsTransactions.end(); ptrPoints++)
                        {
                            int adjustType, adjustSubtype;
                            int numSubtypes = 2;
                            adjustType =  ptrPoints->adjustmentType;
                            adjustSubtype =  ptrPoints->adjustmentSubType;


                           if (adjustType == 3 && adjustSubtype == 1)
                             {
                                qXArcPoints1->Close();

                                GetArcPointsForNormalZed(qXArcPoints1);

                                qXArcPoints1->ParamByName("ARCBILL_KEY")->AsInteger = qrXArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                                qXArcPoints1->ExecQuery();

                                LoadArcPointTransaction(qXArcPoints1, TransactionInfo, qrXArcPay, ptrPoints);

                        }
                    }
                }
           }

            qXArcSurcharge->Close();
            GetArcSurchargeForNormalZed(qXArcSurcharge);
            qXArcSurcharge->ParamByName("ARCBILL_KEY")->AsInteger = qrXArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
            qXArcSurcharge->ExecQuery();

            LoadArcPointTransaction(qXArcSurcharge, TransactionInfo, countedInvoiceNumbers, currentInvoiceKey);

            std::map <int, std::map <UnicodeString, TTransactionCount> > ::iterator itTransactionsCountGroups = TransactionInfo->TransactionsCountGroups.begin();

            // Stores the Transaction Count.
            for (itTransactionsCountGroups = TransactionInfo->TransactionsCountGroups.begin(); itTransactionsCountGroups != TransactionInfo->TransactionsCountGroups.end();
            itTransactionsCountGroups++)
            {
                std::map <UnicodeString, TTransactionCount> TransactionsCount = itTransactionsCountGroups->second;
                for (std::map <UnicodeString, TTransactionCount> ::iterator itTransCount = TransactionsCount.begin();
                itTransCount != TransactionsCount.end(); itTransCount++)
                {
                    itTransCount->second.Counted = false;
                    itTransCount->second.CashOutCounted = false;
                    itTransCount->second.SurchargeCounted = false;
                }
                itTransactionsCountGroups->second = TransactionsCount;
            }

            // MM-1282 : this invoice has been counted, mark it so
            countedInvoiceNumbers.insert(currentInvoiceKey);

        }
    }
    TTransactionInfo* transactionInfo = deviceTransactions[deviceName];             //  TTransactionInfo object populated in order to return
    return (*transactionInfo);
}

void SkimCalculations::CalculateSkims(Database::TDBTransaction &dbTransaction, UnicodeString deviceName)
{
    CashWithdrawlCount = 0;
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

    ibInternalQuery->Close();
    ibInternalQuery->SQL->Text =  "SELECT Refloat_skim.Transaction_Type, refloat_Skim.amount, refloat_skim.IS_FLOAT_WITHDRAWN_FROM_CASH FROM refloat_skim "
                                    "Left join zeds on refloat_skim.z_key = zeds.z_key "
                                    "WHERE zeds.TERMINAL_NAME = :TERMINAL_NAME AND zeds.TIME_STAMP IS NULL "
                                    "and (refloat_skim.transaction_type = 'Withdrawal' or refloat_skim.transaction_type = 'Deposit')";

    ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
    ibInternalQuery->ExecQuery();

    for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
    {
        if(ibInternalQuery->FieldByName("Transaction_Type")->AsString == "Withdrawal" && ibInternalQuery->FieldByName("IS_FLOAT_WITHDRAWN_FROM_CASH")->AsString == "T" )
        {
            CashWithdrawl += ibInternalQuery->FieldByName("amount")->AsCurrency;
            CashWithdrawlCount++;
        }
        else if(ibInternalQuery->FieldByName("Transaction_Type")->AsString == "Withdrawal" )
        {
            Skims += ibInternalQuery->FieldByName("amount")->AsCurrency;
        }
        else
        {
            Refloats += ibInternalQuery->FieldByName("amount")->AsCurrency;
        }
    }


    ibInternalQuery->Close();
    ibInternalQuery->SQL->Text = "SELECT " "* FROM ZEDS " "WHERE " "TERMINAL_NAME = :TERMINAL_NAME AND " "TIME_STAMP IS NULL";
    ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
    ibInternalQuery->ExecQuery();

    if (ibInternalQuery->RecordCount)
    {
        CurrentFloat = ibInternalQuery->FieldByName("INITIAL_FLOAT")->AsCurrency;
        CurrentSkimsTotal = ibInternalQuery->FieldByName("SKIMS_TOTAL")->AsCurrency;
    }
}

UnicodeString DataFormatUtilities::FormatMMReportCurrency(double inValue, bool inShowCurrencySymbol)
{
	return inShowCurrencySymbol ? FormatFloat( "$0.00", inValue ) : FormatFloat( "0.00", inValue );
}

UnicodeString DataFormatUtilities::FormatMMReportPoints(double inValue)
{
	return FormatFloat( "0.00", inValue );
}

UnicodeString DataFormatUtilities::FormatMMReportRedeemCredit(double inValue, bool inShowCurrencySymbol)
{
	return inShowCurrencySymbol ? FormatFloat( "(-) $0.00", inValue ) : FormatFloat("(-) 0.00", inValue);
}

UnicodeString DataFormatUtilities::FormatMMReportRedeemPoints(double inValue)
{
	return FormatFloat( "(-) 0.00", inValue );
}

void TTransactionInfoProcessor::RemoveEntryFromMap(UnicodeString deviceName)
{
     if(deviceTransactions[deviceName])
     {
         std::map<UnicodeString,TTransactionInfo*>::iterator it;
         it=deviceTransactions.find(deviceName);
         deviceTransactions.erase(it);
     }
}

void DataCalculationUtilities::PrinterFormatinThreeSections(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 4;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 1/5;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1/3.5;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width  * 1/4;
    printOut->PrintFormat->Line->Columns[2]->Alignment = taCenter;
    printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width * 1/4;
    printOut->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;

 }

void DataCalculationUtilities::PrinterFormatinTwoSections(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 4;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 1/5;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1/2.5;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width  * 1/2.5;
    printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
    printOut->PrintFormat->Line->Columns[3]->Width = 0;
 }

TDateTime DataCalculationUtilities::CalculateSessionTransactionDate(TDateTime trans_date)
{
    SYSTEMTIME SystemTime;
    ::GetLocalTime(&SystemTime);
    int nPreviousMth;
    int hour = StrToInt(Now().FormatString("hh"));
    if(hour >= TGlobalSettings::Instance().EndOfDay)
    {
       hour -= TGlobalSettings::Instance().EndOfDay;
    }
    if(TGlobalSettings::Instance().EndOfDay > 0 && hour < TGlobalSettings::Instance().EndOfDay)
    {
        if((SystemTime.wDay - 1) == 0)
        {
           nPreviousMth = (SystemTime.wMonth -1==0)?12:SystemTime.wMonth-1;
           int day = CalculateLastDayOfMonth(nPreviousMth);
           TDateTime sessionDate(SystemTime.wYear, nPreviousMth, day);
           trans_date = sessionDate;
        }
        else
        {
            TDateTime sessionDate(SystemTime.wYear, SystemTime.wMonth, (SystemTime.wDay - 1));
            trans_date = sessionDate;
        }
    }
    return trans_date;
}

int DataCalculationUtilities::CalculateLastDayOfMonth(int month)
{
    SYSTEMTIME SystemTime;

    int lastdayofmonth;
    int leap_year;
    switch(month)
    {
	case 2:
        leap_year = SystemTime.wYear % 4;
        if(leap_year == 0)
        {
           lastdayofmonth = 29;
        }
        else
        {
           lastdayofmonth = 28;
        }
		break;
	case 4:
    case 6:
    case 9:
    case 11:
        lastdayofmonth = 30;
		break;
     default:
        lastdayofmonth = 31;
    }
    return lastdayofmonth;
}

void TTransactionInfoProcessor::NormalZedTransaction(TIBSQL *qrXArcBill, bool showendingbal)
{
    AnsiString terminalNamePredicate = "";

    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = " a.TERMINAL_NAME = :TERMINAL_NAME AND";
    }

    if(TGlobalSettings::Instance().UseBIRFormatInXZReport && showendingbal)
    {
        qrXArcBill->SQL->Text = "select a.ARCBILL_KEY, a.DISCOUNT, a.TERMINAL_NAME, a.STAFF_NAME, a.TIME_STAMP,a.PATRON_COUNT, a.INVOICE_NUMBER, a.SALES_TYPE, a.BILLED_LOCATION, a.TOTAL "
                            " from DAYARCBILL a "
                            " left join DAYARCHIVE b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                            " left join DAYARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                            " where " + terminalNamePredicate + " (COALESCE(c.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable') and a.TOTAL > 0 or a.SALES_TYPE = 6 or a.DISCOUNT !=0 "
                            " group by 1,2,3,4,5,6,7,8,9,10 ";
    }
    else
    {

     qrXArcBill->SQL->Text = "select a.ARCBILL_KEY, a.DISCOUNT, a.TERMINAL_NAME, a.STAFF_NAME, a.TIME_STAMP,a.PATRON_COUNT, a.INVOICE_NUMBER, a.SALES_TYPE, a.BILLED_LOCATION, a.TOTAL "
                            " from DAYARCBILL a "
                            " left join DAYARCHIVE b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                            " left join DAYARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                            " where " + terminalNamePredicate + " (COALESCE(c.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable')"
                            " group by 1,2,3,4,5,6,7,8,9,10 ";
     }
}

void TTransactionInfoProcessor::LoadArcPayTransaction(TTransactionInfo* TransactionInfo, TIBSQL *qrXArcPay, bool loop, TIBSQL *qrXArcBill, std::map<AnsiString,TPointTransactions> pointTransaction, std::set<AnsiString> countedInvoiceNumbers, AnsiString currentInvoiceKey)
{
    for (; !qrXArcPay->Eof; qrXArcPay->Next())
    {
        TSumPayments CurrentPayment;
        int groupNumber = qrXArcPay->FieldByName("GROUP_NUMBER")->AsInteger;

        // Summarise all points type payments.
        UnicodeString paymentName = "";

        if (qrXArcPay->FieldByName("PROPERTIES")->AsInteger & ePayTypePoints)
        {
            paymentName = "Points";
        }
        else if (qrXArcPay->FieldByName("PROPERTIES")->AsInteger & ePayTypeCredit && qrXArcPay->FieldByName("SUBTOTAL")->AsCurrency < 0)
        {
            paymentName = qrXArcPay->FieldByName("PAY_TYPE")->AsString + " Purchased";
        }
        else if (qrXArcPay->FieldByName("PROPERTIES")->AsInteger & ePayTypeCredit && qrXArcPay->FieldByName("SUBTOTAL")->AsCurrency >= 0)
        {
            paymentName = qrXArcPay->FieldByName("PAY_TYPE")->AsString + " Redeemed";
        }
        else
        {
            paymentName = qrXArcPay->FieldByName("PAY_TYPE")->AsString;
        }

        std::map <UnicodeString, TSumPayments> PaymentValues = TransactionInfo->Payments[groupNumber];
        CurrentPayment = PaymentValues[paymentName];
        CurrentPayment.Name = paymentName;
        CurrentPayment.Qty++;

        std::map<int, Currency> Points = CurrentPayment.Points;
        //Removing EnableSeperateEarntPts condition as discussed with ravish
        if(loop)
        {
         AnsiString invoiceNumber = qrXArcBill->FieldByName("INVOICE_NUMBER")->AsString;
         std::map<AnsiString, TPointTransactions>::iterator iter  = pointTransaction.find(invoiceNumber);
         if(iter != pointTransaction.end())
         {

             for (std::vector<TPointTransaction>::iterator ptrPoints = iter->second.PointsTransactions.begin();
                  ptrPoints != iter->second.PointsTransactions.end(); ptrPoints++)
                {
                    int adjustType, adjustSubtype;
                    int numSubtypes = 2;
                    adjustType =  ptrPoints->adjustmentType;
                    adjustSubtype =  ptrPoints->adjustmentSubType;
                    if (!(adjustType == 3 && adjustSubtype == 1) )
                    {

                        int pointsAdjustIndex = ((adjustType - 1) * numSubtypes) + adjustSubtype;
                        Points[pointsAdjustIndex] +=  ptrPoints->adjustment;
                    }
                }

          }
            loop = false;
        }
        CurrentPayment.Points = Points;


        bool IsCashOut = false;
        if (qrXArcPay->FieldByName("CASH_OUT")->AsString == "F" || qrXArcPay->FieldByName("CASH_OUT")->AsString == "")
        {
            //if(qrXArcPay->FieldByName("SUBTOTAL")->AsCurrency > 0)
            //{
               CurrentPayment.Total += qrXArcPay->FieldByName("SUBTOTAL")->AsCurrency;
            //}

        }
        else
        {
            CurrentPayment.CashOut += qrXArcPay->FieldByName("SUBTOTAL")->AsCurrency;
            IsCashOut = true;
        }
        if(qrXArcPay->FieldByName("TIP_AMOUNT")->AsCurrency != 0)
        {
             CurrentPayment.TipAmount += qrXArcPay->FieldByName("TIP_AMOUNT")->AsCurrency;
             CurrentPayment.TipQty++;
        }
        CurrentPayment.Rounding -= qrXArcPay->FieldByName("ROUNDING")->AsCurrency;
        CurrentPayment.Properties = qrXArcPay->FieldByName("PROPERTIES")->AsInteger;

        PaymentValues[CurrentPayment.Name] = CurrentPayment;
        TransactionInfo->Payments[groupNumber] = PaymentValues;

        // MM-1282 : count this transaction if not already counted
        if(countedInvoiceNumbers.find(currentInvoiceKey) == countedInvoiceNumbers.end() )
        {
            // Count this Transaction.
            std::map <UnicodeString, TTransactionCount> TransactionsCount = TransactionInfo->TransactionsCountGroups[groupNumber];
            TTransactionCount ThisTransaction = TransactionsCount[paymentName];
            if (IsCashOut)
            {
                if (ThisTransaction.CashOutCounted == false)
                {
                    ThisTransaction.CashOutCount++;
                    ThisTransaction.CashOutCounted = true;
                    TransactionsCount[paymentName] = ThisTransaction;
                    TransactionInfo->TransactionsCountGroups[groupNumber] = TransactionsCount;
                }
            }
            else
            {
                if (ThisTransaction.Counted == false)
                {
                    ThisTransaction.Count++;
                    ThisTransaction.Counted = true;
                    TransactionsCount[paymentName] = ThisTransaction;
                    TransactionInfo->TransactionsCountGroups[groupNumber] = TransactionsCount;
                }
            }
        }
    }
}

void TTransactionInfoProcessor::GetArcPayForNormalZed(TIBSQL *qrXArcPay)
{
    qrXArcPay->SQL->Text = "select ARCBILL_KEY, PAY_TYPE, SUBTOTAL, CASH_OUT, VOUCHER_NUMBER,TAX_FREE,"
            "GROUP_NUMBER, PROPERTIES,ROUNDING,TIP_AMOUNT,PAYMENT_CARD_TYPE from DAYARCBILLPAY "
            "where ARCBILL_KEY = :ARCBILL_KEY AND SUBTOTAL != 0";

}

void TTransactionInfoProcessor::GetArcPointsForNormalZed(TIBSQL *qXArcPoints1)
{
    qXArcPoints1->SQL->Text = "select ARCBILL_KEY, PAY_TYPE, SUBTOTAL, CASH_OUT, VOUCHER_NUMBER,TAX_FREE,"
       "GROUP_NUMBER, PROPERTIES,ROUNDING from DAYARCBILLPAY "
        "where ARCBILL_KEY = :ARCBILL_KEY and PAY_TYPE='Points'";
}

void TTransactionInfoProcessor::LoadArcPointTransaction(TIBSQL *qXArcPoints1, TTransactionInfo* TransactionInfo, TIBSQL *qrXArcPay, std::vector<TPointTransaction>::iterator ptrPoints)
{
    for (; !qXArcPoints1->Eof; qXArcPoints1->Next())
    {
        TSumPayments CurrentPayment;
        int GroupNumber = qXArcPoints1->FieldByName("GROUP_NUMBER")->AsInteger;
        // Summarise all points type payments.
        UnicodeString paymentName = "";
        if (qXArcPoints1->FieldByName("PROPERTIES")->AsInteger & ePayTypePoints)
        {
            paymentName = "Points";
        }
        else if (qXArcPoints1->FieldByName("PROPERTIES")->AsInteger & ePayTypeCredit && qXArcPoints1->FieldByName("SUBTOTAL")->AsCurrency < 0)
        {
            paymentName = qXArcPoints1->FieldByName("PAY_TYPE")->AsString + " Purchased";
        }
        else if (qrXArcPay->FieldByName("PROPERTIES")->AsInteger & ePayTypeCredit && qrXArcPay->FieldByName("SUBTOTAL")->AsCurrency >= 0)
        {
            paymentName = qXArcPoints1->FieldByName("PAY_TYPE")->AsString + " Redeemed";
        }
        else
        {
            paymentName = qXArcPoints1->FieldByName("PAY_TYPE")->AsString;
        }

        std::map <UnicodeString, TSumPayments> PaymentValues = TransactionInfo->Payments[GroupNumber];
        CurrentPayment = PaymentValues[paymentName];
        CurrentPayment.Name = paymentName;
        CurrentPayment.Qty++;

        std::map<int, Currency> Points = CurrentPayment.Points;
        Points[5] = Points[5] + ptrPoints->adjustment;
        CurrentPayment.Points = Points;

        CurrentPayment.Rounding -= qXArcPoints1->FieldByName("ROUNDING")->AsCurrency;
        CurrentPayment.Properties = qXArcPoints1->FieldByName("PROPERTIES")->AsInteger;

        PaymentValues[CurrentPayment.Name] = CurrentPayment;
        TransactionInfo->Payments[GroupNumber] = PaymentValues;
    }
}

void TTransactionInfoProcessor::GetArcSurchargeForNormalZed(TIBSQL *qXArcSurcharge)
{
    qXArcSurcharge->SQL->Text = "select ARCBILL_KEY, PAY_TYPE, PAY_TYPE_DETAILS, SUBTOTAL ,NOTE, TAX_FREE,PROPERTIES,"
        "GROUP_NUMBER,ROUNDING from DAYARCSURCHARGE "
        "where ARCBILL_KEY = :ARCBILL_KEY";
}

void TTransactionInfoProcessor::LoadArcPointTransaction(TIBSQL *qXArcSurcharge, TTransactionInfo* TransactionInfo, std::set<AnsiString> countedInvoiceNumbers, AnsiString currentInvoiceKey)
{
    for (; !qXArcSurcharge->Eof; qXArcSurcharge->Next())
    {
        TSumPayments CurrentPayment;

        int GroupNumber = qXArcSurcharge->FieldByName("GROUP_NUMBER")->AsInteger;
        UnicodeString paymentName = qXArcSurcharge->FieldByName("PAY_TYPE")->AsString;
        std::map <UnicodeString, TSumPayments> PaymentValues = TransactionInfo->Payments[GroupNumber];

        CurrentPayment = PaymentValues[paymentName];
        CurrentPayment.Name = paymentName;
        CurrentPayment.Qty++;

        CurrentPayment.Surcharge += qXArcSurcharge->FieldByName("SUBTOTAL")->AsCurrency;
        CurrentPayment.Rounding -= qXArcSurcharge->FieldByName("ROUNDING")->AsCurrency;
        CurrentPayment.Properties = qXArcSurcharge->FieldByName("PROPERTIES")->AsInteger;

        PaymentValues[CurrentPayment.Name] = CurrentPayment;
        TransactionInfo->Payments[GroupNumber] = PaymentValues;

        // MM-1282 : count this transaction if not already counted
        if (countedInvoiceNumbers.find(currentInvoiceKey) == countedInvoiceNumbers.end())
        {
            // Count this Transaction.
            std::map <UnicodeString, TTransactionCount> TransactionsCount = TransactionInfo->TransactionsCountGroups[GroupNumber];
            TTransactionCount ThisTransaction = TransactionsCount[paymentName];
            if (ThisTransaction.SurchargeCounted == false)
            {
                ThisTransaction.SurchargeCount++;
                ThisTransaction.SurchargeCounted = true;
                TransactionsCount[paymentName] = ThisTransaction;
                TransactionInfo->TransactionsCountGroups[GroupNumber] = TransactionsCount;
            }
        }
    }

}

void TTransactionInfoProcessor::ConsolidatedZedTransaction(TIBSQL *qrXArcBill, bool showendingbal)
{
    AnsiString terminalNamePredicate = "";

    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = " a.TERMINAL_NAME = :TERMINAL_NAME AND";
    }

    if(TGlobalSettings::Instance().UseBIRFormatInXZReport && showendingbal)
    {
        qrXArcBill->SQL->Text = "select a.ARCBILL_KEY, a.DISCOUNT, a.TERMINAL_NAME, a.STAFF_NAME, a.TIME_STAMP,a.PATRON_COUNT, a.INVOICE_NUMBER, a.SALES_TYPE, a.BILLED_LOCATION, a.TOTAL, b.ORDER_TYPE "
                            " from ARCBILL a "
                            " left join ARCHIVE b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                            " left join ARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                            " where "
                            " a.TIME_STAMP >= :StartTime and a.TIME_STAMP <= :EndTime and "
                            + terminalNamePredicate +
                            " (COALESCE(c.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable') " //and a.TOTAL > 0 or a.SALES_TYPE = 6 or (a.TOTAL < 0 and a.DISCOUNT < 0) "
                            //" and a.TIME_STAMP >= :StartTime and a.TIME_STAMP <= :EndTime "
                            " and (COALESCE(b.ORDER_TYPE, 0)) != 3 "
                            " group by 1,2,3,4,5,6,7,8,9,10, 11 ";
                            "Order by "
                            "5 ";
    }
    else
    {

     qrXArcBill->SQL->Text = "select a.ARCBILL_KEY, a.DISCOUNT, a.TERMINAL_NAME, a.STAFF_NAME, a.TIME_STAMP,a.PATRON_COUNT, a.INVOICE_NUMBER, a.SALES_TYPE, a.BILLED_LOCATION, a.TOTAL "
                            " from ARCBILL a "
                            " left join ARCHIVE b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                            " left join ARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                            " where " + terminalNamePredicate + " (COALESCE(c.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable')"
                            " and a.TIME_STAMP >= :StartTime and a.TIME_STAMP <= :EndTime "
                            " group by 1,2,3,4,5,6,7,8,9,10 ";
     }
}

void TTransactionInfoProcessor::GetSurchargeDetailsForConsolidatedZedTransaction(TIBSQL *qXArcSurcharge)
{
    qXArcSurcharge->SQL->Text = "select ARCBILL_KEY, PAY_TYPE, PAY_TYPE_DETAILS, SUBTOTAL ,NOTE, TAX_FREE,PROPERTIES,"
                        "GROUP_NUMBER,ROUNDING from ARCSURCHARGE "
                        "where ARCBILL_KEY = :ARCBILL_KEY";

}

TTransactionInfo TTransactionInfoProcessor::GetTransactionInfoForConsolidatedZed(Database::TDBTransaction &dbTransaction, UnicodeString deviceName, TDateTime startTime, TDateTime endTime, bool showendingbal)
{
    if(!deviceTransactions[deviceName])                                             // checks if the object is already present for the given terminal
    {
        deviceTransactions[deviceName] = new TTransactionInfo;
        TTransactionInfo* TransactionInfo = deviceTransactions[deviceName];

        TransactionInfo->TotalSpentInSavedSales = 0;
        TransactionInfo->PatronCountInSavedSales = 0;
        TransactionInfo->TotalSpentInCashSales = 0;
        TransactionInfo->PatronCountInCashSales = 0;
        TransactionInfo->TotalSpentInCreditedSales = 0;
        TransactionInfo->PatronCountInCreditedSales = 0;
        TransactionInfo->TotalSpentInTabSales = 0;
        TransactionInfo->PatronCountInTabSales = 0;
        TransactionInfo->TotalSpentInRoomSales = 0;
        TransactionInfo->PatronCountInRoomSales = 0;
        TransactionInfo->TotalSpentInAccountSales = 0;
        TransactionInfo->PatronCountInAccountSales = 0;
        TransactionInfo->PatronCountInPurchasingCredit = 0;
        TransactionInfo->TotalCreditRedeemed = 0;
        TransactionInfo->TotalSalesTax = 0;
        TransactionInfo->TotalServiceCharge = 0;
        TransactionInfo->TotalServiceChargeTax = 0;

        // MM-1282 : hack for multiple invoices
        // to store the invoice numbers counted. by having this we can avoid increasing the transaction count but the totals
        std::set<AnsiString> countedInvoiceNumbers;
        countedInvoiceNumbers.clear();

        AnsiString currentInvoiceKey = 0;

        TIBSQL *qrXArcBill = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qrXArcPay = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qXArcSurcharge = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qrXArcOrder = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qXArcPoints = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qXArcPoints1 = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *qXArcTax = dbTransaction.Query(dbTransaction.AddQuery());

        qrXArcBill->Close();

        AnsiString terminalNamePredicate = "";

        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            terminalNamePredicate = " a.TERMINAL_NAME = :TERMINAL_NAME AND";
        }

        ConsolidatedZedTransaction(qrXArcBill, showendingbal);

        qrXArcBill->ParamByName("StartTime")->AsDateTime = startTime;
        qrXArcBill->ParamByName("endTime")->AsDateTime = endTime;

        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            qrXArcBill->ParamByName("TERMINAL_NAME")->AsString = deviceName;
        }

        qrXArcBill->ExecQuery();


        int i=1;
        double first_for_total_time=0;

        qXArcPoints->Close();

        GetPointsForConsolidatedZed(qXArcPoints, startTime, endTime);
        qXArcPoints->ExecQuery();

        std::map<AnsiString,TPointTransactions> pointTransaction;

        for ( ;!qXArcPoints->Eof; qXArcPoints->Next())
        {


            AnsiString invoiceNumber = qXArcPoints->FieldByName("INVOICE_NUMBER")->AsString;

            TPointTransaction point;
             point.adjustmentType= qXArcPoints->FieldByName("ADJUSTMENT_TYPE")->AsInteger;
            point.adjustmentSubType= qXArcPoints->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger;
            point.adjustment= qXArcPoints->FieldByName("ADJUSTMENT")->AsCurrency;
            point.invoiceNo= qXArcPoints->FieldByName("INVOICE_NUMBER")->AsString;

            pointTransaction[invoiceNumber].PointsTransactions.push_back(point);

        }

        for ( ;!qrXArcBill->Eof; qrXArcBill->Next())
        {
            currentInvoiceKey = qrXArcBill->FieldByName("INVOICE_NUMBER")->AsString;

            if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eTableSeat)
            {
                TransactionInfo->TotalSpentInSavedSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInSavedSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eTab)
            {
                TransactionInfo->TotalSpentInTabSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInTabSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eAccount)
            {
                TransactionInfo->TotalSpentInAccountSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInAccountSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eRoomSale)
            {
                TransactionInfo->TotalSpentInRoomSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInRoomSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eCash)
            {
                TransactionInfo->TotalSpentInCashSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInCashSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eCredited)
            {
                TransactionInfo->TotalSpentInCreditedSales += qrXArcBill->FieldByName("TOTAL")->AsCurrency;
                TransactionInfo->PatronCountInCreditedSales += qrXArcBill->FieldByName("PATRON_COUNT")->AsInteger;
            }
            else if (qrXArcBill->FieldByName("SALES_TYPE")->AsInteger == eCreditPurchase)
            {
                // This will always be zero we check each ArcBillPay and pull out the 'cash' payment.
                // and add it to this figure.
            }

           // before = Now();    //arun
            qrXArcPay->Close();
            GetArcPayForConsolidatedZed(qrXArcPay);
            //}
            qrXArcPay->ParamByName("ARCBILL_KEY")->AsInteger = qrXArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
            qrXArcPay->ExecQuery();

            bool loop = true;
            LoadArcPayTransaction(TransactionInfo, qrXArcPay, loop, qrXArcBill,pointTransaction, countedInvoiceNumbers, currentInvoiceKey);

            /************************************************************************************************
            * This query takes a few seconds to process with large points transactions tables which inside *
            * this loop adds up to a lot of time on busy night (1 hour+). As this is only for Aus Loyalty  *
            * it will be used with it enabled for now but in future the Aus Loyalty sites will lag and the *
            * query will need to be looked at.                                                             *
            ************************************************************************************************/
            //Removing EnableSeperateEarntPts condition as discussed with ravish
            //if(TGlobalSettings::Instance().EnableSeperateEarntPts)
            {
                AnsiString invoiceNumber = qrXArcBill->FieldByName("INVOICE_NUMBER")->AsString;
                std::map<AnsiString, TPointTransactions>::iterator iter  = pointTransaction.find(invoiceNumber);
                 if(iter != pointTransaction.end())
                 {
                    for (std::vector<TPointTransaction>::iterator ptrPoints = iter->second.PointsTransactions.begin();
                          ptrPoints != iter->second.PointsTransactions.end(); ptrPoints++)
                        {
                            int adjustType, adjustSubtype;
                            int numSubtypes = 2;
                            adjustType =  ptrPoints->adjustmentType;
                            adjustSubtype =  ptrPoints->adjustmentSubType;


                           if (adjustType == 3 && adjustSubtype == 1)
                             {
                                qXArcPoints1->Close();

                                GetArcPointsForConsolidatedZed(qXArcPoints1);
                                qXArcPoints1->ParamByName("ARCBILL_KEY")->AsInteger = qrXArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                                qXArcPoints1->ExecQuery();

                                LoadArcPointTransaction(qXArcPoints1, TransactionInfo, qrXArcPay, ptrPoints);
                        }
                    }
                }
           }

            qXArcSurcharge->Close();
            GetArcSurchargeForConsolidatedZed(qXArcSurcharge);
            qXArcSurcharge->ParamByName("ARCBILL_KEY")->AsInteger = qrXArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
            qXArcSurcharge->ExecQuery();

            LoadArcPointTransaction(qXArcSurcharge, TransactionInfo, countedInvoiceNumbers, currentInvoiceKey);

            std::map <int, std::map <UnicodeString, TTransactionCount> > ::iterator itTransactionsCountGroups = TransactionInfo->TransactionsCountGroups.begin();

            // Stores the Transaction Count.
            for (itTransactionsCountGroups = TransactionInfo->TransactionsCountGroups.begin(); itTransactionsCountGroups != TransactionInfo->TransactionsCountGroups.end();
            itTransactionsCountGroups++)
            {
                std::map <UnicodeString, TTransactionCount> TransactionsCount = itTransactionsCountGroups->second;
                for (std::map <UnicodeString, TTransactionCount> ::iterator itTransCount = TransactionsCount.begin();
                itTransCount != TransactionsCount.end(); itTransCount++)
                {
                    itTransCount->second.Counted = false;
                    itTransCount->second.CashOutCounted = false;
                    itTransCount->second.SurchargeCounted = false;
                }
                itTransactionsCountGroups->second = TransactionsCount;
            }

            // MM-1282 : this invoice has been counted, mark it so
            countedInvoiceNumbers.insert(currentInvoiceKey);

        }
    }
    TTransactionInfo* transactionInfo = deviceTransactions[deviceName];             //  TTransactionInfo object populated in order to return
    return (*transactionInfo);
}

void TTransactionInfoProcessor::GetArcPointsForConsolidatedZed(TIBSQL *qXArcPoints1)
{
    qXArcPoints1->SQL->Text = "select ARCBILL_KEY, PAY_TYPE, SUBTOTAL, CASH_OUT, VOUCHER_NUMBER,TAX_FREE,"
               "GROUP_NUMBER, PROPERTIES,ROUNDING from ARCBILLPAY "
                "where ARCBILL_KEY = :ARCBILL_KEY and PAY_TYPE='Points'";
}

void TTransactionInfoProcessor::GetArcPayForConsolidatedZed(TIBSQL *qrXArcPay)
{
    qrXArcPay->SQL->Text = "select ARCBILL_KEY, PAY_TYPE, SUBTOTAL, CASH_OUT, VOUCHER_NUMBER,TAX_FREE,"
        "GROUP_NUMBER, PROPERTIES,ROUNDING,TIP_AMOUNT,PAYMENT_CARD_TYPE from ARCBILLPAY "
        "where ARCBILL_KEY = :ARCBILL_KEY AND SUBTOTAL != 0";

}

void TTransactionInfoProcessor::GetArcSurchargeForConsolidatedZed(TIBSQL *qXArcSurcharge)
{
    qXArcSurcharge->SQL->Text = "select ARCBILL_KEY, PAY_TYPE, PAY_TYPE_DETAILS, SUBTOTAL ,NOTE, TAX_FREE,PROPERTIES,"
            "GROUP_NUMBER,ROUNDING from ARCSURCHARGE "
            "where ARCBILL_KEY = :ARCBILL_KEY";
}

Currency DataCalculationUtilities::GetAccumulatedZedTotal(Database::TDBTransaction &dbTransaction, TDateTime &startTime, TDateTime &endTime, UnicodeString deviceName)
{
	TIBSQL *qrAccumulatedTotal = dbTransaction.Query(dbTransaction.AddQuery());
    Currency accumulatedTotal;
    AnsiString terminalNamePredicate = "";

    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = " and a.TERMINAL_NAME = :TERMINAL_NAME ";
    }

	qrAccumulatedTotal->SQL->Text = "SELECT SUM(TERMINAL_EARNINGS) AS TOTAL FROM ZEDS  a where a.TIME_STAMP <=:startTime " + terminalNamePredicate ;

    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        qrAccumulatedTotal->ParamByName("TERMINAL_NAME")->AsString = deviceName;
    }
    qrAccumulatedTotal->ParamByName("StartTime")->AsDateTime = startTime;
    //qrAccumulatedTotal->ParamByName("EndTime")->AsDateTime = endTime;
	qrAccumulatedTotal->ExecQuery();

	if(qrAccumulatedTotal->FieldByName("TOTAL")->IsNull)
    {
		accumulatedTotal = 0;
	}
    else
    {
		accumulatedTotal = qrAccumulatedTotal->FieldByName("TOTAL")->AsCurrency;
	}
	qrAccumulatedTotal->Close();

	return accumulatedTotal;
}

Currency DataCalculationUtilities::GetTotalEarnings(Database::TDBTransaction &dbTransaction, UnicodeString deviceName, TDateTime &startTime, TDateTime &endTime, bool showendingbal)
{
    try
    {
        TTransactionInfo TransactionInfo;

        Currency groupGrandTotal = 0;
        Currency currentFloat = 0;
        Currency currentSkimsTotal = 0;
        Currency skims = 0;
        Currency refloats = 0;
        Currency cashWithdrawl = 0;
        int CashWithdrawlCount = 0;

        if(TGlobalSettings::Instance().UseBIRFormatInXZReport || showendingbal)
        {
           TTransactionInfoProcessor::Instance().RemoveEntryFromMap(deviceName);
           showendingbal = false;
        }
        TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfoForConsolidatedZed(dbTransaction, deviceName,startTime, endTime, showendingbal);
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

        ibInternalQuery->Close();

        ibInternalQuery->SQL->Text =  "SELECT Refloat_skim.Transaction_Type, refloat_Skim.amount, REFLOAT_SKIM.IS_FLOAT_WITHDRAWN_FROM_CASH FROM refloat_skim "
        "Left join zeds on refloat_skim.z_key = zeds.z_key "
        "WHERE zeds.TERMINAL_NAME = :TERMINAL_NAME AND zeds.TIME_STAMP >= :startTime and  zeds.TIME_STAMP <= :endTime  and zeds.TIME_STAMP IS NOT NULL "
        "and (refloat_skim.transaction_type = 'Withdrawal' or refloat_skim.transaction_type = 'Deposit')";

        ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
        ibInternalQuery->ParamByName("startTime")->AsDateTime = startTime;
        ibInternalQuery->ParamByName("endTime")->AsDateTime = endTime;
        ibInternalQuery->ExecQuery();

        for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
        {
            if(ibInternalQuery->FieldByName("Transaction_Type")->AsString == "Withdrawal" && ibInternalQuery->FieldByName("IS_FLOAT_WITHDRAWN_FROM_CASH")->AsString == "T" )
            {
                cashWithdrawl += ibInternalQuery->FieldByName("amount")->AsCurrency;
            }
            else if(ibInternalQuery->FieldByName("Transaction_Type")->AsString == "Withdrawal")
            {
                skims += ibInternalQuery->FieldByName("amount")->AsCurrency;
            }
            else
            {
                refloats += ibInternalQuery->FieldByName("amount")->AsCurrency;
            }
        }

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "SELECT " "* FROM ZEDS " "WHERE " "TERMINAL_NAME = :TERMINAL_NAME AND " "TIME_STAMP >= :startTime and  TIME_STAMP <= :endTime ";
        ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
        ibInternalQuery->ParamByName("startTime")->AsDateTime = startTime;
        ibInternalQuery->ParamByName("endTime")->AsDateTime = endTime;
        ibInternalQuery->ExecQuery();

        if (ibInternalQuery->RecordCount)
        {
            currentFloat = ibInternalQuery->FieldByName("INITIAL_FLOAT")->AsCurrency;
            currentSkimsTotal = ibInternalQuery->FieldByName("SKIMS_TOTAL")->AsCurrency;
        }

        std::map <int, std::map <UnicodeString, TSumPayments> > ::iterator itPayments;
        for (itPayments = TransactionInfo.Payments.begin(); itPayments != TransactionInfo.Payments.end(); itPayments++)
        {
            Currency roundingTotal = 0;
            Currency cashValue = 0;
            Currency changeValue = 0;

            std::map <UnicodeString, TSumPayments> PaymentValues = itPayments->second;
            if (itPayments->first == 0)
            {
                groupGrandTotal += currentFloat;
                groupGrandTotal += currentSkimsTotal;
                std::map <UnicodeString, TSumPayments> ::iterator itCurrentPayment = PaymentValues.find(CASH);
                if (itCurrentPayment != PaymentValues.end())
                {
                    groupGrandTotal += itCurrentPayment->second.Total;
                    roundingTotal += itCurrentPayment->second.Rounding;
                    groupGrandTotal -= itCurrentPayment->second.CashOut;
                    cashValue = itCurrentPayment->second.Total;
                }
                itCurrentPayment = PaymentValues.find(CHANGE);
                if (itCurrentPayment != PaymentValues.end())
                {
                    groupGrandTotal += itCurrentPayment->second.Total;
                    changeValue += itCurrentPayment->second.Total;
                    roundingTotal += itCurrentPayment->second.Rounding;
                }
            }
            std::map <UnicodeString, TSumPayments> ::iterator itCurrentPayment;
            for (itCurrentPayment = PaymentValues.begin(); itCurrentPayment != PaymentValues.end(); itCurrentPayment++)
            {
                if (itCurrentPayment->second.Name.UpperCase() != UpperCase(CHANGE) &&
                        itCurrentPayment->second.Name.UpperCase() != UpperCase(CREDIT) &&
                        !(itCurrentPayment->second.Properties & ePayTypeGetVoucherDetails) &&
                        !(itCurrentPayment->second.Properties & ePayTypeCredit) &&
                        itCurrentPayment->second.Name.UpperCase() != UpperCase(CASH))
                {
                    groupGrandTotal += itCurrentPayment->second.CashOut;
                    groupGrandTotal += itCurrentPayment->second.Total;
                }
            }
            if (groupGrandTotal != 0 && itPayments->first == 0)
            {
                groupGrandTotal -= (currentFloat + currentSkimsTotal - cashWithdrawl);
            }
        }

        return groupGrandTotal;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TTransactionInfoProcessor::GetPointsForNormalZed(TIBSQL *qXArcPoints, UnicodeString deviceName)
{
    qXArcPoints->SQL->Text = "SELECT * from POINTSTRANSACTIONS a where a.TIME_STAMP >= (select coalesce(max(b.Time_Stamp),'28.12.1889, 19:39:25.000') from ZEDS b where b.TERMINAL_NAME = :ZTERMINAL_NAME)" ;
    qXArcPoints->ParamByName("ZTERMINAL_NAME")->AsString = deviceName;
}

void TTransactionInfoProcessor::GetPointsForConsolidatedZed(TIBSQL *qXArcPoints, TDateTime &startTime, TDateTime &endTime)
{
    qXArcPoints->SQL->Text = "SELECT * from POINTSTRANSACTIONS a where a.TIME_STAMP >= :startTime and  a.TIME_STAMP <= :endTime " ;
    qXArcPoints->ParamByName("startTime")->AsDateTime = startTime;
    qXArcPoints->ParamByName("endTime")->AsDateTime = endTime;
}

void SkimCalculations::CalculateSkims(Database::TDBTransaction &dbTransaction, UnicodeString deviceName, TDateTime &startTime, TDateTime &endTime)
{
     CashWithdrawlCount = 0;
      TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

      ibInternalQuery->Close();

        ibInternalQuery->SQL->Text =  "SELECT Refloat_skim.Transaction_Type, refloat_Skim.amount, refloat_skim.IS_FLOAT_WITHDRAWN_FROM_CASH FROM refloat_skim "
        "Left join zeds on refloat_skim.z_key = zeds.z_key "
        "WHERE zeds.TERMINAL_NAME = :TERMINAL_NAME AND zeds.TIME_STAMP >= :startTime and  zeds.TIME_STAMP <= :endTime  "
        "and (refloat_skim.transaction_type = 'Withdrawal' or refloat_skim.transaction_type = 'Deposit')";

    ibInternalQuery->ParamByName("startTime")->AsDateTime = startTime;
    ibInternalQuery->ParamByName("endTime")->AsDateTime = endTime;

    ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
    ibInternalQuery->ExecQuery();

    for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
    {
        if(ibInternalQuery->FieldByName("Transaction_Type")->AsString == "Withdrawal" && ibInternalQuery->FieldByName("IS_FLOAT_WITHDRAWN_FROM_CASH")->AsString == "T" )
        {
            CashWithdrawl += ibInternalQuery->FieldByName("amount")->AsCurrency;
            CashWithdrawlCount++;
        }
        else if(ibInternalQuery->FieldByName("Transaction_Type")->AsString == "Withdrawal")
        {
            Skims += ibInternalQuery->FieldByName("amount")->AsCurrency;
        }
        else
        {
            Refloats += ibInternalQuery->FieldByName("amount")->AsCurrency;
        }
    }


    ibInternalQuery->Close();
    ibInternalQuery->SQL->Text = "SELECT SUM(INITIAL_FLOAT) INITIAL, SUM(SKIMS_TOTAL) SKIMS FROM ZEDS " "WHERE " "TERMINAL_NAME = :TERMINAL_NAME AND " "TIME_STAMP >= :startTime and  TIME_STAMP <= :endTime ";
    ibInternalQuery->ParamByName("startTime")->AsDateTime = startTime;
    ibInternalQuery->ParamByName("endTime")->AsDateTime = endTime;
    ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
    ibInternalQuery->ExecQuery();

    if (ibInternalQuery->RecordCount)
    {
        CurrentFloat = ibInternalQuery->FieldByName("INITIAL")->AsCurrency;
        CurrentSkimsTotal = ibInternalQuery->FieldByName("SKIMS")->AsCurrency;
    }
}


int DataCalculationUtilities::GetZedNumber(Database::TDBTransaction &dbTransaction)
{
    int zKey = 0;
	int GlobalProfileKey = TManagerVariable::Instance().GetProfile(dbTransaction, eSystemProfiles, "Globals");
	if (GlobalProfileKey != 0)
	{
        TManagerVariable::Instance().GetProfileInt(dbTransaction,GlobalProfileKey,vmZCount, TGlobalSettings::Instance().ZCount);
    }
    zKey = TGlobalSettings::Instance().ZCount;
    return zKey;
}




