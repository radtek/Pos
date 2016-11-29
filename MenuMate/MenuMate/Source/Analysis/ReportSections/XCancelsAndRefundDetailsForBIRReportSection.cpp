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
                            "CAST(ITEM_NAME AS VARCHAR(50)) ITEM_NAME, "
                            "PRICE, "
                            "PRICE_LEVEL0, "
                            "PRICE_LEVEL1, "
                            "HAPPY_HOUR HAPPYHOUR, "
                            "CONTACTS.NAME, "
                            "SECURITY.NOTE, "
                            "SECURITY.TIME_STAMP, "
							"QTY "
                        "from "
                            "DAYARCHIVE "
                            "LEFT JOIN SECURITY ON SECURITY.SECURITY_REF =DAYARCHIVE.SECURITY_REF "
                            "LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY "
                        " where "
                            + terminalNamePredicate
                            + " ORDER_TYPE = :ORDER_TYPE "
                              " and SECURITY.TIME_STAMP > :PrevZedTime "
                              " and (SECURITY.SECURITY_EVENT = :SECURITY_EVENT "
                                "OR SECURITY.SECURITY_EVENT = 'CancelY' ) ";

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
          qty  += cancelsQuery->FieldByName("QTY")->AsDouble;
          total_price += cancelsQuery->FieldByName("PRICE_LEVEL1")->AsCurrency;
       }
       else
       {
          qty  += cancelsQuery->FieldByName("QTY")->AsDouble;
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
                                    "ITEM_NAME, "
                                    "PRICE, "
                                    "QTY, "
                                    "REDEEMED, "
                                    "ORDER_TYPE, "
                                    "CONTACTS.NAME, "
                                    "SECURITY.NOTE, "
                                    "SECURITY.TIME_STAMP "
                                "FROM DAYARCHIVE "
                                "LEFT JOIN SECURITY ON DAYARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
                                "LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY "
                                " WHERE " + terminalNamePredicate + " ORDER_TYPE = " + IntToStr(CreditNonExistingOrder) + " " "AND "
                                "SECURITY.SECURITY_EVENT = '" + SecurityTypes[secCredit] + "' "
                                 "OR "  "SECURITY.SECURITY_EVENT = '" + SecurityTypes[secWriteOff] + "' "  "ORDER BY " "CONTACTS.NAME";
    creditQuery->ExecQuery();

    double refund_qty = 0;
    Currency refund_total_price = 0;

    for(; !creditQuery->Eof; creditQuery->Next())
    {
       refund_qty  += creditQuery->FieldByName("QTY")->AsDouble;
       refund_total_price += creditQuery->FieldByName("PRICE")->AsCurrency;
    }
    /*for (; !cancelsQuery->Eof; cancelsQuery->Next())
    {
        int Index = _cancelsServerList->IndexOf(cancelsQuery->FieldByName("NAME")->AsString);
        if (Index == -1)
        {
            TStringList *ItemsList = new TStringList;
            Index = _cancelsServerList->AddObject(cancelsQuery->FieldByName("NAME")->AsString, ItemsList);
            TCurrencyTotal *Item = new TCurrencyTotal;
            Item->Total = (cancelsQuery->FieldByName("HAPPYHOUR")->AsString == "T")
                            ? (cancelsQuery->FieldByName("PRICELEVELS1")->AsCurrency
                            : (cancelsQuery->FieldByName("PRICELEVELS2")->AsCurrency);

            ((TStringList*)_cancelsServerList->Objects[Index])->AddObject( + " |" + //Item);
        }
        else
        {
            TCurrencyTotal *Item = new TCurrencyTotal;
            Item->Total = (cancelsQuery->FieldByName("HAPPYHOUR")->AsString == "T")
                            ? -(cancelsQuery->FieldByName("PRICE_LEVEL1")->AsCurrency * cancelsQuery->FieldByName("QTY")->AsDouble)
                            : -(cancelsQuery->FieldByName("PRICE_LEVEL0")->AsCurrency * cancelsQuery->FieldByName("QTY")->AsDouble);

            Item->Note = cancelsQuery->FieldByName("NOTE")->AsString;
            Item->TimeStamp = cancelsQuery->FieldByName("TIME_STAMP")->AsString;
             Item->Name = cancelsQuery->FieldByName("ITEM_NAME")->AsString;
             Item->StaffName= cancelsQuery->FieldByName("NAME")->AsString;
            ((TStringList*)_cancelsServerList->Objects[Index])->AddObject(cancelsQuery->FieldByName("ITEM_NAME")->AsString, Item);
        }
    }*/

    //if ( _cancelsServerList->Count > 0 )
    //{
        //AddTitle(printOut, "Cancels Report");

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;

        Currency TotalCanceled = 0;
        //for (int i = 0; i < _cancelsServerList->Count; i++)
        //{
            // Add User name...
            SetPrinterFormatInMiddle(printOut);
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width  / 4 + 10;
            printOut->PrintFormat->Line->Columns[1]->Text = "Void and Refund";
            printOut->PrintFormat->AddLine();
            //printOut->PrintFormat->Add(_cancelsServerList->Strings[i]);

            // ...followed by the items canceled by that user.
            //SetPrinterFormat(printOut);
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
            //for (int j = 0; j < ((TStringList*)_cancelsServerList->Objects[i])->Count; j++)
            //{
                //printOut->PrintFormat->Add(
                           // ((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->TimeStamp.FormatString("dd/mm/yy hh:nn:ss")
                           // + "|" + " " + ((TStringList*)_cancelsServerList->Objects[i])->Strings[j] + "|"
                           // + dataFormatUtilities->FormatMMReportCurrency( ((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->Total ) );

                //if (((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->Note != "")
                //{
               //     printOut->PrintFormat->Add("Note : " +((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->Note + "|");
                //}
               // printOut->PrintFormat->Add(((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->Name + "|"); //MM-4385
               // printOut->PrintFormat->Add("Staff Name :" +((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->StaffName + "|");
                //TotalCanceled += //((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->Total;
                //delete((TStringList*)_cancelsServerList->Objects[i])->Objects[j];
            //}
            //delete _cancelsServerList->Objects[i];
       // }

        /*IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

        if(reportSectionDisplayTraits)
        {
            reportSectionDisplayTraits->ApplyTraits(printOut);
        }

        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->Line->Columns[1]->DoubleLine();
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "Total Cancels";
        printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( TotalCanceled );
        ///printOut->PrintFormat->AddLine(); */
    ///}
   // delete _cancelsServerList;
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
