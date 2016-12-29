#include "XPatronAverageDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"

XPatronAverageDetailsReportSection::XPatronAverageDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmPatronAverageDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XPatronAverageDetailsReportSection::XPatronAverageDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmXReport, mmPatronAverageDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
}


XPatronAverageDetailsReportSection::~XPatronAverageDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XPatronAverageDetailsReportSection::GetOutput(TPrintout* printOut)
{
   PrintPatronStatistics(printOut);
   PrintChitStatistics(printOut);
}

void XPatronAverageDetailsReportSection::PrintPatronStatistics(TPrintout* printOut)
{
    AddTitle(printOut, "Patron Averages");
    TTransactionInfo TransactionInfo;
    AnsiString DeviceName = TDeviceRealTerminal::Instance().ID.Name;

    TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfo(*_dbTransaction, DeviceName);

    printOut->PrintFormat->Line->ColCount = 3;
    printOut->PrintFormat->Line->Columns[0]->Width = (printOut->PrintFormat->Width / 3);
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taCenter;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width - printOut->PrintFormat->Line->Columns[1]->Width;
    printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
    if (TransactionInfo.PatronCountInSavedSales != 0)
    {
            printOut->PrintFormat->Add("Saved Sales | " + IntToStr(TransactionInfo.PatronCountInSavedSales) + " | " + dataFormatUtilities->FormatMMReportCurrency( TransactionInfo.TotalSpentInSavedSales / TransactionInfo.PatronCountInSavedSales ) );
    }
    if (TransactionInfo.PatronCountInCashSales != 0)
    {
            printOut->PrintFormat->Add("Cash Sales | " + IntToStr(TransactionInfo.PatronCountInCashSales) + " | " + dataFormatUtilities->FormatMMReportCurrency( TransactionInfo.TotalSpentInCashSales / TransactionInfo.PatronCountInCashSales ) );
    }
    if (TransactionInfo.PatronCountInTabSales != 0)
    {
            printOut->PrintFormat->Add("Tab Sales | " + IntToStr(TransactionInfo.PatronCountInTabSales) + " | " + dataFormatUtilities->FormatMMReportCurrency( TransactionInfo.TotalSpentInTabSales / TransactionInfo.PatronCountInTabSales ) );
    }
    if (TransactionInfo.PatronCountInRoomSales != 0)
    {
            printOut->PrintFormat->Add("Room Sales | " + IntToStr(TransactionInfo.PatronCountInRoomSales) + " | " + dataFormatUtilities->FormatMMReportCurrency( TransactionInfo.TotalSpentInRoomSales / TransactionInfo.PatronCountInRoomSales ) );
    }
    if (TransactionInfo.PatronCountInAccountSales != 0)
    {
            printOut->PrintFormat->Add("Account Sales | " + IntToStr(TransactionInfo.PatronCountInAccountSales) + " | " + dataFormatUtilities->FormatMMReportCurrency( TransactionInfo.TotalSpentInAccountSales / TransactionInfo.PatronCountInAccountSales ) );
    }
    if (TransactionInfo.PatronCountInCreditedSales != 0)
    {
            printOut->PrintFormat->Add("Credited Sales | " + IntToStr(TransactionInfo.PatronCountInCreditedSales) + " | " + dataFormatUtilities->FormatMMReportCurrency( TransactionInfo.TotalSpentInCreditedSales / TransactionInfo.PatronCountInCreditedSales ) );
    }
}


void XPatronAverageDetailsReportSection::PrintChitStatistics(TPrintout* printOut)
{
    TIBSQL *query = _dbTransaction->Query(_dbTransaction->AddQuery());
    const UnicodeString terminalName = TDeviceRealTerminal::Instance().ID.Name;
    const bool midpoint_rounds_down = TGlobalSettings::Instance().MidPointRoundsDown;
    std::map<UnicodeString, Currency> chit_totals;
    std::map<UnicodeString, int> chit_patrons;
    int total_patrons = 0;
    Currency total_spend = 0;
    Currency average_total_spend = 0;
    // since we use the sql in keyword in below queries, we need to wrap the terminal name in quotes
    UnicodeString terminalNames = "'" + terminalName + "'";
    // check whether this terminal is in master slave configuration
    bool isMaster = TGlobalSettings::Instance().EnableDepositBagNum;
    if(isMaster)
    {
        // if so check whether there are more terminal names in the dayarchive table
        // this is where the sales recorded in at the end of the day from master and slaves

        if(IsConsolidatedZed)
        {
           GetDistinctTerminalNameForConsolidatedZed(query);
        }
        else
        {
           GetDistinctTerminalNameForNormalZed(query);
        }

        query->ParamByName("terminal_name")->AsString = terminalName;
        query->ExecQuery();
        if(!query->Eof)
        {
            while (!query->Eof)
            {
                    terminalNames += ",'" + query->FieldByName("terminal_name")->AsString + "'";
                    query->Next();
            }
        }
        query->Close();
    }

    /* Calculate the average total spend. */
    if(IsConsolidatedZed)
    {
       GetTotalSpendForConsolidatedZed(query, terminalNames);
       query->ParamByName("startTime")->AsDateTime = *_startTime;
       query->ParamByName("endTime")->AsDateTime = *_endTime;
    }
    else
    {
       GetTotalSpendForNormalZed(query, terminalNames);
    }

    query->ExecQuery();
    total_spend = query->FieldByName("total_spend")->AsCurrency;
    average_total_spend = query->FieldByName("total_spend")->AsCurrency / (query->FieldByName("n_sales")->AsCurrency + (query->FieldByName("n_sales")->AsInteger == 0));
    query->Close();

    /* Calculate the averages for each chit type. */
    if(IsConsolidatedZed)
    {
       GetChitNameForConsolidatedZed(query, terminalNames);
       query->ParamByName("startTime")->AsDateTime = *_startTime;
       query->ParamByName("endTime")->AsDateTime = *_endTime;
    }
    else
    {
        GetChitNameForNormalZed(query, terminalNames);
    }
    for (query->ExecQuery(); !query->Eof; query->Next())
    {
        chit_totals[query->FieldByName("chit_name")->AsString] = query->FieldByName("average_spend")->AsCurrency;
    }
    query->Close();

    /* Determine the patron count for each chit type. */

    if(IsConsolidatedZed)
    {
       GetTotalPatronCountForConsolidatedZed(query, terminalNames);
       query->ParamByName("startTime")->AsDateTime = *_startTime;
       query->ParamByName("endTime")->AsDateTime = *_endTime;
    }
    else
    {
        GetTotalPatronCountForNormalZed(query, terminalNames);
    }

    for (query->ExecQuery(); !query->Eof; query->Next())
      {
            int n_patrons = query->FieldByName("n_patrons")->AsInteger;
            chit_patrons[query->FieldByName("chit_name")->AsString] = n_patrons;
            total_patrons += n_patrons;
       }
    query->Close();
    AddSubTitle(printOut, "Chit Averages");
    printOut->PrintFormat->Line->ColCount = 3;
    printOut->PrintFormat->Line->Columns[0]->Width = (printOut->PrintFormat->Width / 3);
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taCenter;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width - printOut->PrintFormat->Line->Columns[1]->Width;
    printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
    for (std::map<UnicodeString, Currency>::const_iterator i = chit_totals.begin(); i != chit_totals.end(); ++i)
     {
            const int n_patron = chit_patrons[i->first];
            printOut->PrintFormat->Line->Columns[0]->Text = i->first;
            printOut->PrintFormat->Line->Columns[1]->Text = IntToStr(chit_patrons[i->first]);
            printOut->PrintFormat->Line->Columns[2]->Text = UnicodeString::CurrToStrF(
                                                                            RoundToNearest(i->second / Currency(n_patron + (n_patron == 0)),
                                                                            0.01,
                                                                            midpoint_rounds_down),
                                                                            UnicodeString::sffFixed,
                                                                            2);
            printOut->PrintFormat->AddLine();
    }
    printOut->PrintFormat->DrawLine();
    printOut->PrintFormat->Line->Columns[0]->Text = "Totals";
    printOut->PrintFormat->Line->Columns[1]->Text = IntToStr(total_patrons);
    printOut->PrintFormat->Line->Columns[2]->Text =
    UnicodeString::CurrToStrF(
    RoundToNearest(total_spend / Currency(total_patrons + (total_patrons == 0)), 0.01, midpoint_rounds_down),UnicodeString::sffFixed,2);
    printOut->PrintFormat->AddLine();
    printOut->PrintFormat->NewLine();
}

void XPatronAverageDetailsReportSection::GetDistinctTerminalNameForNormalZed(TIBSQL *query)
{
    query->SQL->Text = "select distinct(terminal_name) "
    "       from dayarchive "
    "       where terminal_name <> :terminal_name;";
}

void XPatronAverageDetailsReportSection::GetDistinctTerminalNameForConsolidatedZed(TIBSQL *query)
{
    query->SQL->Text = "select distinct(terminal_name) "
    "       from archive "
    "       where terminal_name <> :terminal_name;";
}

void XPatronAverageDetailsReportSection::GetTotalSpendForNormalZed(TIBSQL *query, UnicodeString terminalNames)
{
    query->SQL->Text = "select sum(price * qty) total_spend, "
                       "count(*) n_sales "
                       "from dayarchive "
                       "where terminal_name in ( " + terminalNames + " ) "
                       "and chit_name is not null;";
}

void XPatronAverageDetailsReportSection::GetTotalSpendForConsolidatedZed(TIBSQL *query, UnicodeString terminalNames)
{
    query->SQL->Text = "select sum(price * qty) total_spend, "
                       "count(*) n_sales "
                       "from archive "
                       "where terminal_name in ( " + terminalNames + " ) "
                       "and archive.TIME_STAMP >= :startTime and archive.TIME_STAMP <= :endTime "
                       "and chit_name != '';";
}

void XPatronAverageDetailsReportSection::GetChitNameForNormalZed(TIBSQL *query, UnicodeString terminalNames)
{
 query->SQL->Text = "select chit_name, "
                   "sum(price * qty) average_spend "
                   "from dayarchive "
                   "where terminal_name in ( " + terminalNames + " ) "
                   "and chit_name is not null "
                   "group by chit_name;";
}
void XPatronAverageDetailsReportSection::GetChitNameForConsolidatedZed(TIBSQL *query, UnicodeString terminalNames)
{
 query->SQL->Text = "select chit_name, "
                   "sum(price * qty) average_spend "
                   "from archive "
                   "where terminal_name in ( " + terminalNames + " ) "
                   "and chit_name != '' "
                   "and archive.TIME_STAMP >= :startTime and archive.TIME_STAMP <= :endTime "
                   "group by chit_name;";
}

void XPatronAverageDetailsReportSection::GetTotalPatronCountForNormalZed(TIBSQL *query, UnicodeString terminalNames)
{
    query->SQL->Text =
        "select dacp.chit_name, "
        "sum(dac.patron_count) n_patrons "
        "from daypatroncount dac "
        "inner join ( "
        "select da.arcbill_key, "
        "da.chit_name "
        "from daypatroncount dac "
        "inner join dayarchive da on "
        "da.arcbill_key = dac.arcbill_key "
        "where terminal_name in ( " + terminalNames + " ) "
        "and chit_name is not null "
        "group by da.arcbill_key, "
        "da.chit_name "
        ") dacp on "
        "dacp.arcbill_key = dac.arcbill_key "
        "group by dacp.chit_name;";
}

void XPatronAverageDetailsReportSection::GetTotalPatronCountForConsolidatedZed(TIBSQL *query, UnicodeString terminalNames)
{
    query->SQL->Text =
        "select dacp.chit_name, "
        "sum(dac.patron_count) n_patrons "
        "from patroncount dac "
        "inner join ( "
        "select da.arcbill_key, "
        "da.chit_name "
        "from patroncount dac "
        "inner join archive da on "
        "da.arcbill_key = dac.arcbill_key "
        "where terminal_name in ( " + terminalNames + " ) "
        "and chit_name != '' "
        "and da.TIME_STAMP >= :startTime and da.TIME_STAMP <= :endTime "
        "group by da.arcbill_key, "
        "da.chit_name "
        ") dacp on "
        "dacp.arcbill_key = dac.arcbill_key "
        "group by dacp.chit_name;";
}


