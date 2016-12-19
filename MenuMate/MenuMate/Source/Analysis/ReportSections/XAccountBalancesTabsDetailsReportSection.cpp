//---------------------------------------------------------------------------


#include "XAccountBalancesTabsDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"
#include "enumTab.h"
#include "ContactStaff.h"
#include "DBTab.h"

XAccountBalancesTabsDetailsReportSection::XAccountBalancesTabsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmAccountBalancesTabsDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}


XAccountBalancesTabsDetailsReportSection::XAccountBalancesTabsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmXReport, mmAccountBalancesTabsDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XAccountBalancesTabsDetailsReportSection::~XAccountBalancesTabsDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XAccountBalancesTabsDetailsReportSection::GetOutput(TPrintout* printOut)
{
    AddTitle(printOut, "Account Balances - Tabs");
    TStringList *TabsList = new TStringList;
    GetReportData(TabsList);
    TabsList->Sort();
    if (TabsList->Count > 0)
       {
            printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
            Currency TotalTabs = 0;
            printOut->PrintFormat->Line->ColCount = 2;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 2 / 3;
            printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 2 / 3);
            printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

            for (int i = 0; i < TabsList->Count; i++)
            {
                    printOut->PrintFormat->Add(TabsList->Strings[i] + "|" + dataFormatUtilities->FormatMMReportCurrency( ((TCurrencyTotal*)TabsList->Objects[i])->Total) );
                    TotalTabs += ((TCurrencyTotal*)TabsList->Objects[i])->Total;
                    delete TabsList->Objects[i];
            }
            if (TabsList->Count > 1)
            {
                    printOut->PrintFormat->Line->Columns[0]->Text = "";
                    printOut->PrintFormat->Line->Columns[1]->Line();
                    printOut->PrintFormat->AddLine();
                    printOut->PrintFormat->Add( "Total Tabs|" + dataFormatUtilities->FormatMMReportCurrency( TotalTabs ) );
            }
       }
       delete TabsList;
}

void XAccountBalancesTabsDetailsReportSection::GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime)
{
    AddTitle(printOut, "Account Balances - Tabs");
    TStringList *TabsList = new TStringList;
    GetReportData(TabsList);
    TabsList->Sort();
    if (TabsList->Count > 0)
       {
            printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
            Currency TotalTabs = 0;
            printOut->PrintFormat->Line->ColCount = 2;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 2 / 3;
            printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 2 / 3);
            printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

            for (int i = 0; i < TabsList->Count; i++)
            {
                    printOut->PrintFormat->Add(TabsList->Strings[i] + "|" + dataFormatUtilities->FormatMMReportCurrency( ((TCurrencyTotal*)TabsList->Objects[i])->Total) );
                    TotalTabs += ((TCurrencyTotal*)TabsList->Objects[i])->Total;
                    delete TabsList->Objects[i];
            }
            if (TabsList->Count > 1)
            {
                    printOut->PrintFormat->Line->Columns[0]->Text = "";
                    printOut->PrintFormat->Line->Columns[1]->Line();
                    printOut->PrintFormat->AddLine();
                    printOut->PrintFormat->Add( "Total Tabs|" + dataFormatUtilities->FormatMMReportCurrency( TotalTabs ) );
            }
       }
       delete TabsList;
}


void XAccountBalancesTabsDetailsReportSection::GetReportData(TStringList *TabsList)
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
            case 0:
            case 1:
            case 2:
            case 12:
                {
                        TabTypeList = TabsList;
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
                    "select COURSE_NAME, ORDER_TYPE, ZED_PRICE, DISCOUNT, QTY, REDEEMED,SEATNO, TAB_KEY, TABLE_NAME, "
                    " TABLE_NUMBER,ORDER_KEY  from ORDERS  where TAB_KEY = :TAB_KEY  ";

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

void XAccountBalancesTabsDetailsReportSection::GetTabData(TStringList *TabTypeList, TIBSQL *qrXTab, TIBSQL *qrXOrder)
{
    TIBSQL *qrXSeat = _dbTransaction->Query(_dbTransaction->AddQuery());
    TIBSQL *qrXTable = _dbTransaction->Query(_dbTransaction->AddQuery());
    TIBSQL *qrXRoom = _dbTransaction->Query(_dbTransaction->AddQuery());

    std::auto_ptr <TContactStaff> Staff(new TContactStaff(*_dbTransaction));
    TCurrencyTotal *TabTotal = new TCurrencyTotal;

   for (; !qrXOrder->Eof; qrXOrder->Next())
    {

         TabTotal->Total +=  (qrXOrder->FieldByName("ZED_PRICE")->AsCurrency * qrXOrder->FieldByName("QTY")->AsCurrency );
    }



    CalculateData(TabTotal,TabTypeList,qrXTab,qrXOrder);
}

void XAccountBalancesTabsDetailsReportSection::GetCreditData(TStringList *TabTypeList,TIBSQL *qrXTab, TIBSQL *qrXOrder)
{

  if (qrXTab->FieldByName("CREDIT")->AsCurrency != 0)
    {
            TCurrencyTotal *TabTotal = new TCurrencyTotal;
            CalculateData(TabTotal,TabTypeList,qrXTab,qrXOrder);
    }
}

void XAccountBalancesTabsDetailsReportSection::CalculateData(TCurrencyTotal *TabTotal,TStringList *TabTypeList,TIBSQL *qrXTab, TIBSQL *qrXOrder)
{
    std::auto_ptr <TContactStaff> Staff(new TContactStaff(*_dbTransaction));
    TabTotal->Total -= qrXTab->FieldByName("CREDIT")->AsCurrency;
    if (qrXTab->FieldByName("TAB_TYPE")->AsInteger == TabNormal || qrXTab->FieldByName("TAB_TYPE")->AsInteger == TabDelayedPayment)
    {
      TabTypeList->AddObject(UnicodeString(qrXTab->FieldByName("TAB_NAME")->AsString), TabTotal);
    }
    else if (qrXTab->FieldByName("TAB_TYPE")->AsInteger == TabStaff || qrXTab->FieldByName("TAB_TYPE")
                    ->AsInteger == TabMember)
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
