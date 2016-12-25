#include "XAccountPurchasesDetailsReportSection.h"

XAccountPurchasesDetailsReportSection::XAccountPurchasesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmAccountPurchasesDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    IsConsolidatedZed = false;
}


XAccountPurchasesDetailsReportSection::XAccountPurchasesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmAccountPurchasesDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
    IsConsolidatedZed = true;
}

XAccountPurchasesDetailsReportSection::~XAccountPurchasesDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XAccountPurchasesDetailsReportSection::GetOutput(TPrintout* printOut)
{
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    DataCalculationUtilities dataCalcUtils;
    TDateTime previousZedTime = dataCalcUtils.GetPreviousZedTimeForTerminal(
                                                                *_dbTransaction,
                                                                deviceName);//Now();

    TIBSQL *ibInternalQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
    ibInternalQuery->Close();

    if(IsConsolidatedZed)
    {
       GetPurchaseDetailsForConsolidatedZed(ibInternalQuery);
    }
    else
    {
       GetPurchaseDetailsForNormalZed(ibInternalQuery, previousZedTime);
    }
    ibInternalQuery->ExecQuery();


    if (ibInternalQuery->RecordCount)
    {
        Currency totalPurchases = 0;
        UnicodeString groupName = "";
        UnicodeString groupMember = "";
        Currency memberPurchases = 0;
        Currency groupPurchases = 0;

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->DoubleLine();
        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->NewLine();

        AddTitle(printOut, "Account Purchases");

        printOut->PrintFormat->Line->ColCount = 4;
        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 3/8;
        printOut->PrintFormat->Line->Columns[0]->Text = "Group Name";
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 4;
        printOut->PrintFormat->Line->Columns[1]->Text = "Name";
        printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width * 1/8;
        printOut->PrintFormat->Line->Columns[2]->Text = "Inv # ";
        printOut->PrintFormat->Line->Columns[2]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width - printOut->PrintFormat->Line->Columns[1]->Width - printOut->PrintFormat->Line->Columns[2]->Width;
        printOut->PrintFormat->Line->Columns[3]->Text = "Total";
        printOut->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
        printOut->PrintFormat->AddLine();

        for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
        {
            if(groupMember == "")
            {
                groupMember = ibInternalQuery->FieldByName("CONTACTNAME")->AsString;
            }

            if(groupMember != ibInternalQuery->FieldByName("CONTACTNAME")->AsString)
            {

                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Line();
                printOut->PrintFormat->Line->Columns[2]->Line();
                printOut->PrintFormat->Line->Columns[3]->Line();
                printOut->PrintFormat->AddLine();

                printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 3/8 ;
                printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 3/8;
                printOut->PrintFormat->Line->Columns[2]->Width = 0;
                printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[1]->Width - printOut->PrintFormat->Line->Columns[0]->Width;

                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
                printOut->PrintFormat->Line->Columns[1]->Text = "Total " + groupMember + ":";
                printOut->PrintFormat->Line->Columns[2]->Text = "";

                printOut->PrintFormat->Line->Columns[3]->Text = FloatToStrF(memberPurchases, ffNumber, 18, CurrencyDecimals);
                printOut->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
                printOut->PrintFormat->AddLine();

                printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 3/8;
                printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 4;
                printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width * 1/8;
                printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[2]->Width - printOut->PrintFormat->Line->Columns[1]->Width - printOut->PrintFormat->Line->Columns[0]->Width;

                printOut->PrintFormat->Line->Columns[0]->Line();
                printOut->PrintFormat->Line->Columns[1]->Line();
                printOut->PrintFormat->Line->Columns[2]->Line();
                printOut->PrintFormat->Line->Columns[3]->Line();
                printOut->PrintFormat->AddLine();

                memberPurchases = 0;
                groupMember = ibInternalQuery->FieldByName("CONTACTNAME")->AsString;

                printOut->PrintFormat->Line->Columns[0]->Text = ibInternalQuery->FieldByName("GROUPNAME")->AsString;
            }
            else
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "";
            }

            if(groupName == "")
            {
                groupName = ibInternalQuery->FieldByName("GROUPNAME")->AsString;
                printOut->PrintFormat->Line->Columns[0]->Text = groupName;
            }
            else if(groupName != ibInternalQuery->FieldByName("GROUPNAME")->AsString )
            {
                printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 1 / 2;
                printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 4;
                printOut->PrintFormat->Line->Columns[2]->Width = 0;
                printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[1]->Width - printOut->PrintFormat->Line->Columns[0]->Width;

                printOut->PrintFormat->Line->Columns[0]->Text = "Total " + groupName + ":";
                printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
                printOut->PrintFormat->Line->Columns[1]->Width = 0;
                printOut->PrintFormat->Line->Columns[1]->Text = "";
                printOut->PrintFormat->Line->Columns[2]->Text = "";
                printOut->PrintFormat->Line->Columns[2]->Width = 0;
                printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;
                printOut->PrintFormat->Line->Columns[3]->Text = FloatToStrF(groupPurchases , ffNumber, 18, CurrencyDecimals);;
                printOut->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
                printOut->PrintFormat->AddLine();

                printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 3/8;
                printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 4;
                printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width * 1/8;
                printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[2]->Width - printOut->PrintFormat->Line->Columns[1]->Width - printOut->PrintFormat->Line->Columns[0]->Width;

                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[0]->Line();
                printOut->PrintFormat->Line->Columns[1]->Line();
                printOut->PrintFormat->Line->Columns[2]->Line();
                printOut->PrintFormat->Line->Columns[3]->Line();
                printOut->PrintFormat->AddLine();

                groupName = ibInternalQuery->FieldByName("GROUPNAME")->AsString;
                groupPurchases = 0;

                printOut->PrintFormat->Line->Columns[0]->Text = ibInternalQuery->FieldByName("GROUPNAME")->AsString;
            }
            printOut->PrintFormat->Line->Columns[1]->Text = ibInternalQuery->FieldByName("CONTACTNAME")->AsString;
            printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[2]->Text = ibInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
            printOut->PrintFormat->Line->Columns[2]->Alignment = taLeftJustify;

            printOut->PrintFormat->Line->Columns[3]->Text = FloatToStrF(ibInternalQuery->FieldByName("TOTAL")->AsFloat, ffNumber, 18, CurrencyDecimals);
            printOut->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
            printOut->PrintFormat->AddLine();

            memberPurchases += ibInternalQuery->FieldByName("TOTAL")->AsFloat;
            groupPurchases += ibInternalQuery->FieldByName("TOTAL")->AsFloat;
            totalPurchases += ibInternalQuery->FieldByName("TOTAL")->AsFloat;
        }

        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->Line->Columns[1]->Line();
        printOut->PrintFormat->Line->Columns[2]->Line();
        printOut->PrintFormat->Line->Columns[3]->Line();
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 3/8;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1/2;
        printOut->PrintFormat->Line->Columns[2]->Width = 0;
        printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[1]->Width - printOut->PrintFormat->Line->Columns[0]->Width;

        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Text = "Total " + groupMember + ":";
        printOut->PrintFormat->Line->Columns[2]->Text = "";
        printOut->PrintFormat->Line->Columns[3]->Text = FloatToStrF(memberPurchases, ffNumber, 18, CurrencyDecimals);
        printOut->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 1 / 4;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 4;
        printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width * 1 / 6;
        printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width - printOut->PrintFormat->Line->Columns[1]->Width - printOut->PrintFormat->Line->Columns[2]->Width;
        printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[2]->Width - printOut->PrintFormat->Line->Columns[1]->Width - printOut->PrintFormat->Line->Columns[0]->Width;

        printOut->PrintFormat->Line->Columns[0]->Line();
        printOut->PrintFormat->Line->Columns[1]->Line();
        printOut->PrintFormat->Line->Columns[2]->Line();
        printOut->PrintFormat->Line->Columns[3]->Line();
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 3 / 4;
        printOut->PrintFormat->Line->Columns[1]->Width = 0;
        printOut->PrintFormat->Line->Columns[2]->Width = 0;
        printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;

        printOut->PrintFormat->Line->Columns[0]->Text = "Total " + groupName + ":";
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Text = "";
        printOut->PrintFormat->Line->Columns[2]->Text = "";
        printOut->PrintFormat->Line->Columns[3]->Text = FloatToStrF(groupPurchases, ffNumber, 18, CurrencyDecimals);
        printOut->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->Line->Columns[1]->Text = "";
        printOut->PrintFormat->Line->Columns[2]->Text = "";
        printOut->PrintFormat->Line->Columns[3]->Text = "";
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->DoubleLine();
        printOut->PrintFormat->Line->Columns[1]->DoubleLine();
        printOut->PrintFormat->Line->Columns[2]->DoubleLine();
        printOut->PrintFormat->Line->Columns[3]->DoubleLine();
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "Total Account Purchases:";
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Text = "";
        printOut->PrintFormat->Line->Columns[2]->Text = "";
        printOut->PrintFormat->Line->Columns[3]->Text = FloatToStrF(totalPurchases, ffNumber, 18, CurrencyDecimals);;
        printOut->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
        printOut->PrintFormat->AddLine();
    }
}

void XAccountPurchasesDetailsReportSection::GetPurchaseDetailsForNormalZed(TIBSQL *ibInternalQuery, TDateTime previousZedTime)
{
    ibInternalQuery->SQL->Text = "SELECT "
            "GROUPS.NAME GROUPNAME, "
            "CONTACTS.NAME CONTACTNAME, "
            "INVOICES.TOTAL_INC TOTAL, "
            "INVOICES.INVOICE_NUMBER INVOICE_NUMBER "
        "FROM CONTACTGROUPS "
        "LEFT JOIN GROUPS ON CONTACTGROUPS.GROUPS_KEY = GROUPS.GROUPS_KEY "
        "LEFT JOIN CONTACTS ON CONTACTGROUPS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
        "LEFT JOIN INVOICES ON CONTACTGROUPS.CONTACTS_KEY = INVOICES.CONTACTS_KEY "
        "WHERE INVOICES.CREATION_DATE > :CREATION_DATE "
        "ORDER BY 1, 2 ";
    ibInternalQuery->ParamByName("CREATION_DATE")->AsDateTime = previousZedTime;

}
void XAccountPurchasesDetailsReportSection::GetPurchaseDetailsForConsolidatedZed(TIBSQL *ibInternalQuery)
{
    ibInternalQuery->SQL->Text = "SELECT "
            "GROUPS.NAME GROUPNAME, "
            "CONTACTS.NAME CONTACTNAME, "
            "INVOICES.TOTAL_INC TOTAL, "
            "INVOICES.INVOICE_NUMBER INVOICE_NUMBER "
        "FROM CONTACTGROUPS "
        "LEFT JOIN GROUPS ON CONTACTGROUPS.GROUPS_KEY = GROUPS.GROUPS_KEY "
        "LEFT JOIN CONTACTS ON CONTACTGROUPS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
        "LEFT JOIN INVOICES ON CONTACTGROUPS.CONTACTS_KEY = INVOICES.CONTACTS_KEY "
        "WHERE INVOICES.CREATION_DATE >= :startTime and INVOICES.CREATION_DATE <= :endTime "
        "ORDER BY 1, 2 ";
  ibInternalQuery->ParamByName("startTime")->AsDateTime = *_startTime;
  ibInternalQuery->ParamByName("endTime")->AsDateTime = *_endTime;

}
