#include "XAccountBalancesSeatedDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"
#include "enumTab.h"
#include "ContactStaff.h"

XAccountBalancesSeatedDetailsReportSection::XAccountBalancesSeatedDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmAccountBalancesSeatedDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}


XAccountBalancesSeatedDetailsReportSection::~XAccountBalancesSeatedDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XAccountBalancesSeatedDetailsReportSection::GetOutput(TPrintout* printOut)
{
    AddTitle(printOut, "Account Balances - Seated");
    TStringList *SeatedList = new TStringList;
    GetReportData(SeatedList);
    SeatedList->Sort();
    if (SeatedList->Count > 0)
    {
        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        Currency TotalSeats = 0;
        printOut->PrintFormat->Line->ColCount = 2;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 2 / 3;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 2 / 3);
        printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

        for (int i = 0; i < SeatedList->Count; i++)
        {
                printOut->PrintFormat->Add(SeatedList->Strings[i] + "|" + dataFormatUtilities->FormatMMReportCurrency( ((TCurrencyTotal*)SeatedList->Objects[i])->Total) );
                TotalSeats += ((TCurrencyTotal*)SeatedList->Objects[i])->Total;
                delete SeatedList->Objects[i];
        }
        if (SeatedList->Count > 1)
        {
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Line();
                printOut->PrintFormat->AddLine();
                printOut->PrintFormat->Add( "Total Seated|" + dataFormatUtilities->FormatMMReportCurrency( TotalSeats ) );
        }
    }
       delete SeatedList;
}

void XAccountBalancesSeatedDetailsReportSection::GetReportData(TStringList *SeatedList)
{
    TIBSQL *qrXTab = _dbTransaction->Query(_dbTransaction->AddQuery());
    TIBSQL *qrXOrder = _dbTransaction->Query(_dbTransaction->AddQuery());
    qrXTab->Close();
    qrXTab->SQL->Text = "select TAB_KEY, TAB_NAME, TAB_TYPE,CREDIT, CONTACTS_KEY from TAB";
    qrXTab->ExecQuery();
    for (; !qrXTab->Eof; qrXTab->Next())
    {
            TStringList *TabTypeList;
            switch(qrXTab->FieldByName("TAB_TYPE")->AsInteger)
            {
            case 3:
                    {
                            TabTypeList = SeatedList;
                            break;
                    }
            default:
                    {
                            TabTypeList = NULL;
                            break;
                    }
            }
            if (TabTypeList)
            {
                    // Process all Orders.
                    qrXOrder->Close();
                    qrXOrder->SQL->Text =
                    "select COURSE_NAME, ORDER_TYPE, ZED_PRICE, DISCOUNT, QTY, REDEEMED,SEATNO, TAB_KEY, TABLE_NAME, TABLE_NUMBER, ORDER_KEY "
                    " from ORDERS " "where TAB_KEY = :TAB_KEY";
                    qrXOrder->ParamByName("TAB_KEY")->AsInteger = qrXTab->FieldByName("TAB_KEY")->AsInteger;
                    qrXOrder->ExecQuery();
                    if (qrXOrder->RecordCount != 0)
                    {
                       GetTabData(TabTypeList, qrXTab,qrXOrder);
                    }
                    else // Tab is empty but show credit.
                    {
                       GetCreditData(TabTypeList, qrXTab,qrXOrder);
                    }
            }
    }

}

void XAccountBalancesSeatedDetailsReportSection::GetTabData(TStringList *TabTypeList,TIBSQL *qrXTab, TIBSQL *qrXOrder)
{
    TCurrencyTotal *TabTotal = new TCurrencyTotal;
    for (; !qrXOrder->Eof; qrXOrder->Next())
    {
            TabTotal->Total += (qrXOrder->FieldByName("ZED_PRICE")->AsCurrency *
            qrXOrder->FieldByName("QTY")->AsCurrency);
    }
    CalculateData(TabTotal,TabTypeList,qrXTab,qrXOrder);
}

void XAccountBalancesSeatedDetailsReportSection::GetCreditData(TStringList *TabTypeList,TIBSQL *qrXTab, TIBSQL *qrXOrder)
{

  if (qrXTab->FieldByName("CREDIT")->AsCurrency != 0)
    {
            TCurrencyTotal *TabTotal = new TCurrencyTotal;
            CalculateData(TabTotal,TabTypeList,qrXTab,qrXOrder);
    }
}

void XAccountBalancesSeatedDetailsReportSection::CalculateData(TCurrencyTotal *TabTotal,TStringList *TabTypeList,TIBSQL *qrXTab, TIBSQL *qrXOrder)
{
    TIBSQL *qrXSeat = _dbTransaction->Query(_dbTransaction->AddQuery());
    TIBSQL *qrXTable = _dbTransaction->Query(_dbTransaction->AddQuery());
    std::auto_ptr <TContactStaff> Staff(new TContactStaff(*_dbTransaction));
    TabTotal->Total -= qrXTab->FieldByName("CREDIT")->AsCurrency;
    if (qrXTab->FieldByName("TAB_TYPE")->AsInteger == TabTableSeat)
    {
            // Construct the Tab name based on the Table and Seat No.
            UnicodeString TabName = "Unknown";
            UnicodeString SeatSection;
            UnicodeString TableSection;

            SeatSection = qrXTab->FieldByName("TAB_NAME")->AsString;
            qrXSeat->Close();
            qrXSeat->SQL->Text = "select SEATNO, TABLE_KEY from SEAT " "where TAB_KEY = :TAB_KEY order by SEATNO";
            qrXSeat->ParamByName("TAB_KEY")->AsInteger = qrXTab->FieldByName("TAB_KEY")->AsInteger;
            qrXSeat->ExecQuery();
            if (SeatSection == "" && qrXSeat->RecordCount != 0)
            {
                    SeatSection = TGlobalSettings::Instance().SeatLabel + " " + qrXSeat->FieldByName("SEATNO")->AsString;
            }

            qrXTable->Close();
            qrXTable->SQL->Text =
            "select PARTY_NAME, TABLE_NAME, TABLE_NUMBER from TABLES " " where TABLE_KEY = :TABLE_KEY order by TABLE_NUMBER";
            qrXTable->ParamByName("TABLE_KEY")->AsInteger = qrXSeat->FieldByName("TABLE_KEY")->AsInteger;
            qrXTable->ExecQuery();
            if (qrXTable->RecordCount != 0)
            {
                    if (qrXTable->FieldByName("TABLE_NAME")->AsString != "")
                    {
                            TableSection = qrXTable->FieldByName("TABLE_NAME")->AsString;
                    }
                    else
                    {
                            TableSection = "Table " + qrXTable->FieldByName("TABLE_NUMBER")->AsString;
                    }
            }
            TabName = TableSection + " " + SeatSection;
            TabTypeList->AddObject(TabName, TabTotal);
    }
   else if (qrXTab->FieldByName("TAB_TYPE")->AsInteger == TabStaff || qrXTab->FieldByName("TAB_TYPE")->AsInteger == TabMember)
    {
            if (qrXTab->FieldByName("CONTACTS_KEY")->AsInteger != 0)
            {
                    UnicodeString Owner = Staff->GetContactName(*_dbTransaction, qrXTab->FieldByName("CONTACTS_KEY")->AsInteger);
                    TabTypeList->AddObject(Owner, TabTotal);
            }
            else
            {
                    UnicodeString Owner = qrXTab->FieldByName("TAB_NAME")->AsString;
                    if (Owner == "")
                    {
                            Owner == "Unknown Member/Staff Tab";
                    }
                    else
                    {
                            TabTypeList->AddObject(Owner, TabTotal);
                    }
            }
    }
}
