//---------------------------------------------------------------------------


#pragma hdrstop

#include "XCancelsDetailsReportSection.h"

//---------------------------------------------------------------------------

XCancelsDetailsReportSection::XCancelsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmRefundDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XCancelsDetailsReportSection::XCancelsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmRefundDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XCancelsDetailsReportSection::~XCancelsDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XCancelsDetailsReportSection::GetOutput(TPrintout* printOut)
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
        GetCancelReportsForConsolidatedZed(cancelsQuery, terminalNamePredicate, prevZedTime);
        prevZedTime = *_endTime;
    }
    else
    {
       GetCancelReportsForNormalZed(cancelsQuery, terminalNamePredicate);
    }

    cancelsQuery->ParamByName("ORDER_TYPE")->AsInteger = CanceledOrder;
    cancelsQuery->ParamByName("PrevZedTime")->AsDateTime = prevZedTime;
    cancelsQuery->ParamByName("SECURITY_EVENT")->AsString = UnicodeString(SecurityTypes[secCancel]);
    cancelsQuery->ExecQuery();

    for (; !cancelsQuery->Eof; cancelsQuery->Next())
    {
        int Index = _cancelsServerList->IndexOf(cancelsQuery->FieldByName("NAME")->AsString);
        if (Index == -1)
        {
            TStringList *ItemsList = new TStringList;
            Index = _cancelsServerList->AddObject(cancelsQuery->FieldByName("NAME")->AsString, ItemsList);
            TCurrencyTotal *Item = new TCurrencyTotal;
            Item->Total = (cancelsQuery->FieldByName("HAPPYHOUR")->AsString == "T")
                            ? -(cancelsQuery->FieldByName("PRICE_LEVEL1")->AsCurrency * cancelsQuery->FieldByName("QTY")->AsDouble)
                            : -(cancelsQuery->FieldByName("PRICE_LEVEL0")->AsCurrency * cancelsQuery->FieldByName("QTY")->AsDouble);

            Item->Note = cancelsQuery->FieldByName("NOTE")->AsString;
            Item->TimeStamp = cancelsQuery->FieldByName("TIME_STAMP")->AsString;
             Item->Name = cancelsQuery->FieldByName("ITEM_NAME")->AsString;//MM-4385
             Item->StaffName= cancelsQuery->FieldByName("NAME")->AsString;
            ((TStringList*)_cancelsServerList->Objects[Index])->AddObject(cancelsQuery->FieldByName("ITEM_NAME")->AsString, Item);
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
    }

    if ( _cancelsServerList->Count > 0 )
    {
        AddTitle(printOut, "Cancels Report");

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;

        Currency TotalCanceled = 0;
        for (int i = 0; i < _cancelsServerList->Count; i++)
        {
            // Add User name...
            printOut->PrintFormat->Line->ColCount = 1;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
            printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut->PrintFormat->Add(_cancelsServerList->Strings[i]);

            // ...followed by the items canceled by that user.
            printOut->PrintFormat->Line->ColCount = 3;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 5 / 9;
            printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 2 / 9;
            printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
            printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 7 / 9);
            printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

            for (int j = 0; j < ((TStringList*)_cancelsServerList->Objects[i])->Count; j++)
            {
                printOut->PrintFormat->Add(
                            ((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->TimeStamp.FormatString("dd/mm/yy hh:nn:ss")
                            + "|" + " " + ((TStringList*)_cancelsServerList->Objects[i])->Strings[j] + "|"
                            + dataFormatUtilities->FormatMMReportCurrency( ((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->Total ) );

                if (((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->Note != "")
                {
                    printOut->PrintFormat->Add("Note : " +((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->Note + "|");
                }
                printOut->PrintFormat->Add(((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->Name + "|"); //MM-4385
                printOut->PrintFormat->Add("Staff Name :" +((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->StaffName + "|");
                TotalCanceled += ((TCurrencyTotal*)((TStringList*)_cancelsServerList->Objects[i])->Objects[j])->Total;
                delete((TStringList*)_cancelsServerList->Objects[i])->Objects[j];
            }
            delete _cancelsServerList->Objects[i];
        }

        IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

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
        printOut->PrintFormat->AddLine();
    }
    delete _cancelsServerList;
}

void XCancelsDetailsReportSection::GetCancelReportsForNormalZed(TIBSQL* cancelsQuery, AnsiString terminalNamePredicate)
{
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
                "OR SECURITY.SECURITY_EVENT = 'CancelY' ) "
        "union all "

        "select "
            "CAST(ITEM_NAME AS VARCHAR(50)) ITEM_NAME, "
            "ZED_PRICE, "
            "PRICE_LEVEL0,  "
            "PRICE_LEVEL1, "
            "HAPPYHOUR, "
            "CONTACTS.NAME, "
            "SECURITY.NOTE, "
            "SECURITY.TIME_STAMP, "
            "QTY "
        "from "
            "ORDERS  "
            "LEFT JOIN SECURITY ON ORDERS.SECURITY_REF =SECURITY.SECURITY_REF "
            "LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY "
        " where "
            + terminalNamePredicate
            + " ORDER_TYPE = :ORDER_TYPE "
              " and SECURITY.TIME_STAMP > :PrevZedTime "
              " and (SECURITY.SECURITY_EVENT = :SECURITY_EVENT "
               "OR SECURITY.SECURITY_EVENT = 'CancelY' ) " ;
}

void XCancelsDetailsReportSection::GetCancelReportsForConsolidatedZed(TIBSQL* cancelsQuery, AnsiString terminalNamePredicate, TDateTime prevZedTime)
{

  AnsiString timeFilter = "";
  timeFilter =  " SECURITY.TIME_STAMP >= :startTime and SECURITY.TIME_STAMP <= :PrevZedTime  and ";



  cancelsQuery->SQL->Text =
        "select "
            "SECURITY.TIME_STAMP, "
            "CAST(ITEM_NAME AS VARCHAR(50)) ITEM_NAME, "
            "PRICE, "
            "PRICE_LEVEL0, "
            "PRICE_LEVEL1, "
            "HAPPY_HOUR HAPPYHOUR, "
            "CONTACTS.NAME, "
            "SECURITY.NOTE, "
            "QTY "
        "from "
            " ARCHIVE "
            "LEFT JOIN SECURITY ON SECURITY.SECURITY_REF = ARCHIVE.SECURITY_REF "
            "LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY "
        " where "
         + timeFilter
            + terminalNamePredicate
            + " ORDER_TYPE = :ORDER_TYPE "
              " and (SECURITY.SECURITY_EVENT = :SECURITY_EVENT "
                "OR SECURITY.SECURITY_EVENT = 'CancelY' ) "
        "union all "

        "select "
            "SECURITY.TIME_STAMP, "
            "CAST(ITEM_NAME AS VARCHAR(50)) ITEM_NAME, "
            "ZED_PRICE, "
            "PRICE_LEVEL0,  "
            "PRICE_LEVEL1, "
            "HAPPYHOUR, "
            "CONTACTS.NAME, "
            "SECURITY.NOTE, "
            "QTY "
        "from "
            "ORDERS  "
            "LEFT JOIN SECURITY ON ORDERS.SECURITY_REF =SECURITY.SECURITY_REF "
            "LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY "
        " where "
             + timeFilter
            + terminalNamePredicate
            + " ORDER_TYPE = :ORDER_TYPE "
              " and (SECURITY.SECURITY_EVENT = :SECURITY_EVENT "
               "OR SECURITY.SECURITY_EVENT = 'CancelY' ) " ;

    cancelsQuery->ParamByName("startTime")->AsDateTime = *_startTime;
}



#pragma package(smart_init)
