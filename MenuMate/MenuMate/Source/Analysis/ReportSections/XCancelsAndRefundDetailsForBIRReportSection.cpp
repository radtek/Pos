//---------------------------------------------------------------------------


#pragma hdrstop

#include "XCancelsAndRefundDetailsForBIRReportSection.h"

//---------------------------------------------------------------------------

XCancelsAndRefundDetailsForBIRReportSection::XCancelsAndRefundDetailsForBIRReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmRefundCancelDetailsSections, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XCancelsAndRefundDetailsForBIRReportSection::XCancelsAndRefundDetailsForBIRReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmRefundCancelDetailsSections, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XCancelsAndRefundDetailsForBIRReportSection::~XCancelsAndRefundDetailsForBIRReportSection()
{
    delete dataFormatUtilities;
}

void XCancelsAndRefundDetailsForBIRReportSection::GetOutput(TPrintout* printOut)
{
    TStringList *_cancelsServerList = new TStringList;
    DataCalculationUtilities dataCalcUtils;

    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    TDateTime prevZedTime = dataCalcUtils.GetPreviousZedTimeForTerminal(
                                                                *_dbTransaction,
                                                                deviceName);

    AnsiString terminalNamePredicate = "";
    if(!_globalSettings->EnableDepositBagNum)
    {
        terminalNamePredicate = " SECURITY.TERMINAL_NAME = '" + deviceName + "' AND ";
    }

    TIBSQL* cancelsQuery = _dbTransaction->Query( _dbTransaction->AddQuery());
    cancelsQuery->Close();

    if(IsConsolidatedZed)
    {
        GetCancelValuesForConsolidatedBirReport(cancelsQuery, terminalNamePredicate);
        cancelsQuery->ParamByName("startTime")->AsDateTime = *_startTime;
        cancelsQuery->ParamByName("endTime")->AsDateTime = *_endTime;
    }
    else
    {
        GetCancelValuesForNormalBirReport(cancelsQuery, terminalNamePredicate);
        cancelsQuery->ParamByName("PrevZedTime")->AsDateTime = prevZedTime;

    }
    cancelsQuery->ParamByName("ORDER_TYPE")->AsInteger = CanceledOrder;
    cancelsQuery->ParamByName("SECURITY_EVENT")->AsString = UnicodeString(SecurityTypes[secCancel]);

    cancelsQuery->ExecQuery();

    double qty = 0;
    Currency total_price = 0;

    for(; !cancelsQuery->Eof; cancelsQuery->Next())
    {
       if(cancelsQuery->FieldByName("HAPPYHOUR")->AsString == "T")
       {
          qty++;
          total_price += cancelsQuery->FieldByName("PRICE_LEVEL1")->AsCurrency;
       }
       else
       {
          qty++;
          total_price += cancelsQuery->FieldByName("PRICE_LEVEL0")->AsCurrency;
       }
    }


    if(!_globalSettings->EnableDepositBagNum)
    {
        terminalNamePredicate = " SECURITY.TERMINAL_NAME = '" + deviceName + "' AND ";
    }

    TIBSQL *creditQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
    creditQuery->Close();

    if(IsConsolidatedZed)
    {
        GetRefundDetailsForConsolidatedBirReport(creditQuery, terminalNamePredicate);
        creditQuery->ParamByName("startTime")->AsDateTime = *_startTime;
        creditQuery->ParamByName("endTime")->AsDateTime = *_endTime;
    }
    else
    {
        GetRefundDetailsForNormalBirReport(creditQuery, terminalNamePredicate);
    }
    creditQuery->ExecQuery();

    double refund_qty = 0;
    Currency refund_total_price = 0;

    for(; !creditQuery->Eof; creditQuery->Next())
    {
       refund_qty++;
       refund_total_price += creditQuery->FieldByName("PRICE")->AsCurrency;
    }


        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;

        Currency TotalCanceled = 0;

        dataCalcUtils.PrinterFormatinThreeSections(printOut);
        AnsiString title = "Void and Refund";
        printOut->PrintFormat->Line->Columns[1]->Width = title.Length();
        printOut->PrintFormat->Line->Columns[1]->Text = "Void and Refund";
        printOut->PrintFormat->AddLine();

        dataCalcUtils.PrinterFormatinThreeSections(printOut);
        printOut->PrintFormat->Line->Columns[1]->Text = "Void ";
        printOut->PrintFormat->Line->Columns[2]->Text = FloatToStr(qty);//, ffNumber, CurrencyDecimals);

        printOut->PrintFormat->Line->Columns[3]->Text = CurrToStrF(total_price, ffNumber, CurrencyDecimals);
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[1]->Text = "Refund";
        printOut->PrintFormat->Line->Columns[2]->Text = FloatToStr(fabs(refund_qty));//, ffNumber, CurrencyDecimals);
        printOut->PrintFormat->Line->Columns[3]->Text = CurrToStrF(refund_total_price, ffNumber, CurrencyDecimals);
        printOut->PrintFormat->AddLine();

        double total_qty = qty + fabs(refund_qty);
        Currency total_amount = total_price + refund_total_price;

        printOut->PrintFormat->Line->Columns[1]->Text = "-----------------------------------------------------------";
        printOut->PrintFormat->Line->Columns[2]->Text = "-----------------------------------------------------------";
        printOut->PrintFormat->Line->Columns[3]->Text = "-----------------------------------------------------------";
        printOut->PrintFormat->AddLine();


        printOut->PrintFormat->Line->Columns[1]->Text = "Total";
        printOut->PrintFormat->Line->Columns[2]->Text = FloatToStr(total_qty);//, ffNumber, CurrencyDecimals);
        printOut->PrintFormat->Line->Columns[3]->Text = CurrToStrF(total_amount, ffNumber, CurrencyDecimals);
        printOut->PrintFormat->AddLine();
        SetSingleColumnPrinterFormat(printOut);
        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->AddLine();
}

void XCancelsAndRefundDetailsForBIRReportSection::SetSingleColumnPrinterFormat(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 1;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
}

void XCancelsAndRefundDetailsForBIRReportSection::GetCancelValuesForNormalBirReport(TIBSQL* cancelsQuery, AnsiString terminalNamePredicate)
{
    cancelsQuery->SQL->Text =
        "select "
            "sum(PRICE_LEVEL0)PRICE_LEVEL0, "
            "sum(PRICE_LEVEL1)PRICE_LEVEL1, "
            "da.HAPPY_HOUR HAPPYHOUR, "
            "SECURITY.TIME_STAMP "
        "from "
            "DAYARCHIVE da "
            "LEFT JOIN SECURITY ON SECURITY.SECURITY_REF =da.SECURITY_REF "
            "LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY "
        " where "
            + terminalNamePredicate
            + " ORDER_TYPE = :ORDER_TYPE "
              " and SECURITY.TIME_STAMP > :PrevZedTime "
              " and (SECURITY.SECURITY_EVENT = :SECURITY_EVENT "
                "OR SECURITY.SECURITY_EVENT = 'CancelY' ) "
                "group by "
                "HAPPY_HOUR, "
                "SECURITY.TIME_STAMP "

                "Union All "

                 "select "
                    "sum(PRICE_LEVEL0)PRICE_LEVEL0, "
                    "sum(PRICE_LEVEL1)PRICE_LEVEL1, "
                    "HAPPYHOUR HAPPYHOUR, "
                    "SECURITY.TIME_STAMP "
                "from "
                    "ORDERS o "
                    "LEFT JOIN SECURITY ON SECURITY.SECURITY_REF =o.SECURITY_REF  "
                    "LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY "
                " where "
                    + terminalNamePredicate
                    + " ORDER_TYPE = :ORDER_TYPE "
                      " and SECURITY.TIME_STAMP > :PrevZedTime "
                      " and (SECURITY.SECURITY_EVENT = :SECURITY_EVENT "
                        "OR SECURITY.SECURITY_EVENT = 'CancelY' ) "
                        "group by "
                        "HAPPYHOUR, "
                        "SECURITY.TIME_STAMP " ;
}


void XCancelsAndRefundDetailsForBIRReportSection::GetCancelValuesForConsolidatedBirReport(TIBSQL* cancelsQuery, AnsiString terminalNamePredicate)
{
    cancelsQuery->SQL->Text =
        "select "
            "sum(PRICE_LEVEL0)PRICE_LEVEL0, "
            "sum(PRICE_LEVEL1)PRICE_LEVEL1, "
            "da.HAPPY_HOUR HAPPYHOUR, "
            "SECURITY.TIME_STAMP "
        "from "
            "ARCHIVE da "
            "LEFT JOIN SECURITY ON SECURITY.SECURITY_REF =da.SECURITY_REF "
            "LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY "
        " where "
        "   SECURITY.TIME_STAMP  >= :startTime and  SECURITY.TIME_STAMP <= :endTime and "
            + terminalNamePredicate
            + " ORDER_TYPE = :ORDER_TYPE "

              " and (SECURITY.SECURITY_EVENT = :SECURITY_EVENT "
                "OR SECURITY.SECURITY_EVENT = 'CancelY' ) "
                "group by "
                "HAPPY_HOUR, "
                "SECURITY.TIME_STAMP "

                "Union All "

                 "select "
                    "sum(PRICE_LEVEL0)PRICE_LEVEL0, "
                    "sum(PRICE_LEVEL1)PRICE_LEVEL1, "
                    "HAPPYHOUR HAPPYHOUR, "
                    "SECURITY.TIME_STAMP "
                "from "
                    "ORDERS o "
                    "LEFT JOIN SECURITY ON SECURITY.SECURITY_REF =o.SECURITY_REF  "
                    "LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY "
                " where "
                "  SECURITY.TIME_STAMP  >= :startTime and  SECURITY.TIME_STAMP <= :endTime and "
                    + terminalNamePredicate
                    + " ORDER_TYPE = :ORDER_TYPE "

                      " and (SECURITY.SECURITY_EVENT = :SECURITY_EVENT "
                        "OR SECURITY.SECURITY_EVENT = 'CancelY' ) "
                        "group by "
                        "HAPPYHOUR, "
                        "SECURITY.TIME_STAMP " ;
}


void XCancelsAndRefundDetailsForBIRReportSection::GetRefundDetailsForNormalBirReport(TIBSQL *creditQuery, AnsiString terminalNamePredicate)
{
creditQuery->SQL->Text = "SELECT "
            "sum(PRICE) PRICE,  "
            "ORDER_TYPE, "
            "CONTACTS.NAME, "
            "SECURITY.TIME_STAMP "
        "FROM DAYARCHIVE "
        "LEFT JOIN SECURITY ON DAYARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
        "LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY "
        " WHERE " + terminalNamePredicate + " ORDER_TYPE = " + IntToStr(CreditNonExistingOrder) + " " "AND "
        "SECURITY.SECURITY_EVENT = '" + SecurityTypes[secCredit] + "' "
         "OR "  "SECURITY.SECURITY_EVENT = '" + SecurityTypes[secWriteOff] + "' "
         "group by "
         "ORDER_TYPE, "
         "SECURITY.TIME_STAMP, "
         "CONTACTS.NAME "
         "ORDER BY CONTACTS.NAME ";
}

void XCancelsAndRefundDetailsForBIRReportSection::GetRefundDetailsForConsolidatedBirReport(TIBSQL *creditQuery, AnsiString terminalNamePredicate)
{

creditQuery->SQL->Text = "SELECT "
            "sum(PRICE) PRICE,  "
            "ORDER_TYPE, "
            "CONTACTS.NAME, "
            "SECURITY.TIME_STAMP "
        "FROM ARCHIVE "
        "LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
        "LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY "
        " WHERE "
        " SECURITY.TIME_STAMP  >= :startTime and  SECURITY.TIME_STAMP <= :endTime and "
        + terminalNamePredicate +
        " ORDER_TYPE = " + IntToStr(CreditNonExistingOrder) + " " "AND "
        "SECURITY.SECURITY_EVENT = '" + SecurityTypes[secCredit] + "' "
         "OR "  "SECURITY.SECURITY_EVENT = '" + SecurityTypes[secWriteOff] + "' "
         "group by "
         "ORDER_TYPE, "
         "SECURITY.TIME_STAMP, "
         "CONTACTS.NAME "
         "ORDER BY CONTACTS.NAME ";
}



#pragma package(smart_init)
