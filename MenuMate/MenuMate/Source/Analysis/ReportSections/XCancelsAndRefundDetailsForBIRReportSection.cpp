//---------------------------------------------------------------------------


#pragma hdrstop

#include "XCancelsAndRefundDetailsForBIRReportSection.h"

//---------------------------------------------------------------------------

XCancelsAndRefundDetailsForBIRReportSection::XCancelsAndRefundDetailsForBIRReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmRefundCancelDetailsSections, dbTransaction, globalSettings)
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
    cancelsQuery->SQL->Text =
                        "select "
                            "sum(PRICE_LEVEL0)PRICE_LEVEL0, "
                            "sum(PRICE_LEVEL1)PRICE_LEVEL1, "
                            //"PRICE_LEVEL0, "
                            //"PRICE_LEVEL1, "
                            "HAPPY_HOUR HAPPYHOUR, "
                            //"CONTACTS.NAME, "
                            //"SECURITY.NOTE, "
                            "SECURITY.TIME_STAMP "
							//"QTY "
                        "from "
                            "DAYARCHIVE "
                            "LEFT JOIN SECURITY ON SECURITY.SECURITY_REF =DAYARCHIVE.SECURITY_REF "
                            "LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY "
                        " where "
                            + terminalNamePredicate
                            + " ORDER_TYPE = :ORDER_TYPE "
                              " and SECURITY.TIME_STAMP > :PrevZedTime "
                              " and (SECURITY.SECURITY_EVENT = :SECURITY_EVENT "
                                "OR SECURITY.SECURITY_EVENT = 'CancelY' ) "
                                "group by "
                                "HAPPY_HOUR, "
                                "SECURITY.TIME_STAMP ";

    cancelsQuery->ParamByName("ORDER_TYPE")->AsInteger = CanceledOrder;
    cancelsQuery->ParamByName("PrevZedTime")->AsDateTime = prevZedTime;
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
        terminalNamePredicate = " DAYARCHIVE.TERMINAL_NAME = '" + deviceName + "' AND ";
    }

    TIBSQL *creditQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
    creditQuery->Close();
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

        SetPrinterFormatInMiddle(printOut);
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width  / 4 + 10;
        printOut->PrintFormat->Line->Columns[1]->Text = "Void and Refund";
        printOut->PrintFormat->AddLine();

        SetPrinterFormatInMiddle(printOut);
        printOut->PrintFormat->Line->Columns[1]->Text = "Void ";
        printOut->PrintFormat->Line->Columns[2]->Text = FloatToStr(qty);

        printOut->PrintFormat->Line->Columns[3]->Text = dataFormatUtilities->FormatMMReportCurrency(total_price);
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[1]->Text = "Refund";
        printOut->PrintFormat->Line->Columns[2]->Text = FloatToStr(fabs(refund_qty));
        printOut->PrintFormat->Line->Columns[3]->Text = dataFormatUtilities->FormatMMReportCurrency(refund_total_price);
        printOut->PrintFormat->AddLine();

        double total_qty = qty + fabs(refund_qty);
        Currency total_amount = total_price + refund_total_price;

        //SetSingleColumnPrinterFormat(printOut);
        printOut->PrintFormat->Line->Columns[1]->Line();
        printOut->PrintFormat->Line->Columns[2]->Line();
        printOut->PrintFormat->Line->Columns[3]->Line();
        printOut->PrintFormat->AddLine();

        //SetPrinterFormat(printOut);

        printOut->PrintFormat->Line->Columns[1]->Text = "Total";
        printOut->PrintFormat->Line->Columns[2]->Text = FloatToStr(total_qty);
        printOut->PrintFormat->Line->Columns[3]->Text = dataFormatUtilities->FormatMMReportCurrency(total_amount);
        printOut->PrintFormat->AddLine();
        SetSingleColumnPrinterFormat(printOut);
        printOut->PrintFormat->Line->Columns[0]->Text = "";
        //printOut->PrintFormat->NewLine();
        printOut->PrintFormat->AddLine();
}

void XCancelsAndRefundDetailsForBIRReportSection::SetPrinterFormat(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 3;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width  / 3;
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width  / 3;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taCenter;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]
            ->Width - printOut->PrintFormat->Line->Columns[1]->Width;//printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 7 / 9);
    printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
}

void XCancelsAndRefundDetailsForBIRReportSection::SetSingleColumnPrinterFormat(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 1;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
}

void XCancelsAndRefundDetailsForBIRReportSection::SetPrinterFormatInMiddle(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 4;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width  / 4 - 2;
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width  / 4;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width  / 4 - 3;
    printOut->PrintFormat->Line->Columns[2]->Alignment = taCenter;
    printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width/4;// - printOut->PrintFormat->Line->Columns[1]
            //->Width - printOut->PrintFormat->Line->Columns[2]->Width;//printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 7 / 9);
    printOut->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
}





#pragma package(smart_init)
