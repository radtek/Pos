#include "XHourlySalesDetailsReportSection.h"

XHourlySalesDetailsReportSection::XHourlySalesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmAccountBalancesTabsDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    IsConsolidatedZed = false;
}

XHourlySalesDetailsReportSection::XHourlySalesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings,TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmAccountBalancesTabsDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
    IsConsolidatedZed = true;
}


XHourlySalesDetailsReportSection::~XHourlySalesDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XHourlySalesDetailsReportSection::GetOutput(TPrintout* printOut)
{
    AnsiString hourlySalesQuery = "";
    TIBSQL *ibInternalQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
    ibInternalQuery->Close();
    if(IsConsolidatedZed)
    {
       hourlySalesQuery = GetHourlySalesDetailsForConsolidatedZed(hourlySalesQuery);
       ibInternalQuery->SQL->Text = hourlySalesQuery;
       ibInternalQuery->ParamByName("startTime")->AsDateTime = *_startTime;
       ibInternalQuery->ParamByName("endTime")->AsDateTime = *_endTime;
    }
    else
    {
       hourlySalesQuery = GetHourlySalesDetailsForNormalZed(hourlySalesQuery);
       ibInternalQuery->SQL->Text = hourlySalesQuery;
    }
    ibInternalQuery->ExecQuery();

    if (ibInternalQuery->RecordCount)
    {
        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->DoubleLine();
        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->NewLine();

        AddTitle(printOut, "Hourly Sales");

        printOut->PrintFormat->Line->ColCount = 3;
        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;

        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 5 / 10;
        printOut->PrintFormat->Line->Columns[0]->Text = "Time";
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;

        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 10;
        printOut->PrintFormat->Line->Columns[1]->Text = "";
        printOut->PrintFormat->Line->Columns[1]->Alignment = taCenter;

        printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width * 4 / 10;
        printOut->PrintFormat->Line->Columns[2]->Text = "Total";
        printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

        printOut->PrintFormat->AddLine();

        for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
        {
            AnsiString timeRange = ibInternalQuery->FieldByName("Start_Time")->AsString + " - " +
                                        ibInternalQuery->FieldByName("End_Time")->AsString;

            printOut->PrintFormat->Line->ColCount = 3;

            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 5 / 10;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 10;
            printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width * 4 / 10;

            printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[0]->Text = timeRange + ":";

            printOut->PrintFormat->Line->Columns[1]->Alignment = taCenter;
            printOut->PrintFormat->Line->Columns[1]->Text = "";

            printOut->PrintFormat->Line->Columns[2]->Text = FloatToStrF(ibInternalQuery->FieldByName("Bill_Total")->AsCurrency, ffNumber, 18, CurrencyDecimals);
            printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

            printOut->PrintFormat->AddLine();
        }
    }
}

AnsiString XHourlySalesDetailsReportSection::GetHourlySalesDetailsForNormalZed(AnsiString hourlySalesQuery)
{
   hourlySalesQuery = "SELECT "
            "CAST(CAST(EXTRACT(MONTH FROM SECURITY.TIME_STAMP) "
                "|| '/' || EXTRACT(DAY FROM SECURITY.TIME_STAMP) "
                "|| '/' || EXTRACT(YEAR FROM SECURITY.TIME_STAMP) AS TIMESTAMP) + "
                "CAST(((EXTRACT(MINUTE FROM SECURITY.TIME_STAMP) / 60) * 60 * 60) + "
                "(EXTRACT(HOUR FROM SECURITY.TIME_STAMP) * 60 * 60) AS DOUBLE PRECISION) / 86400 AS TIMESTAMP) Start_Date, "
            "CAST(CAST(EXTRACT(MONTH FROM SECURITY.TIME_STAMP) "
                "|| '/' || EXTRACT(DAY FROM SECURITY.TIME_STAMP) "
                "|| '/' || EXTRACT(YEAR FROM SECURITY.TIME_STAMP) AS TIMESTAMP) + "
                "CAST(((EXTRACT (MINUTE FROM SECURITY.TIME_STAMP) / 60) * 60 * 60) + "
                "(EXTRACT(HOUR FROM SECURITY.TIME_STAMP) * 60 * 60) + 1800*2  AS DOUBLE PRECISION) / 86400 AS TIMESTAMP) End_Date, "
            "CAST(SUM(DAYARCBILL.TOTAL) AS NUMERIC (15,4)) Bill_Total, "
            "CAST(CAST('12/30/1899' AS TIMESTAMP) + "
                 "CAST(((EXTRACT (MINUTE FROM SECURITY.TIME_STAMP) / 60) * 60 * 60) + "
                 "(EXTRACT (HOUR FROM SECURITY.TIME_STAMP) * 60 * 60) AS DOUBLE PRECISION) / 86400   AS TIME) Start_Time, "
            "CAST(CAST('12/30/1899' AS TIMESTAMP) + "
                 "CAST(((EXTRACT (MINUTE FROM SECURITY.TIME_STAMP) / 60) * 60 * 60) + "
                 "(EXTRACT (HOUR FROM SECURITY.TIME_STAMP) * 60 * 60) + 1800*2 AS DOUBLE PRECISION) / 86400  AS TIME) End_Time "
        "FROM "
            "SECURITY "
            "LEFT JOIN DAYARCBILL ON SECURITY.SECURITY_REF = DAYARCBILL.SECURITY_REF "
            "WHERE Security.SECURITY_REF = DAYARCBILL.SECURITY_REF AND DAYARCBILL.ARCBILL_KEY IS NOT NULL AND SECURITY.SECURITY_EVENT = 'Billed By' "
        "GROUP BY 1, 2, 4, 5; ";
  return hourlySalesQuery;
}

AnsiString XHourlySalesDetailsReportSection::GetHourlySalesDetailsForConsolidatedZed(AnsiString hourlySalesQuery)
{
   hourlySalesQuery = "SELECT "
            "CAST(SUM(ARCBILL.TOTAL) AS NUMERIC (15,4)) Bill_Total, "
            "CAST(CAST('12/30/1899' AS TIMESTAMP) + "
                 "CAST(((EXTRACT (MINUTE FROM SECURITY.TIME_STAMP) / 60) * 60 * 60) + "
                 "(EXTRACT (HOUR FROM SECURITY.TIME_STAMP) * 60 * 60) AS DOUBLE PRECISION) / 86400   AS TIME) Start_Time, "
            "CAST(CAST('12/30/1899' AS TIMESTAMP) + "
                 "CAST(((EXTRACT (MINUTE FROM SECURITY.TIME_STAMP) / 60) * 60 * 60) + "
                 "(EXTRACT (HOUR FROM SECURITY.TIME_STAMP) * 60 * 60) + 1800*2 AS DOUBLE PRECISION) / 86400  AS TIME) End_Time "
        "FROM "
            "SECURITY "
            "LEFT JOIN ARCBILL ON SECURITY.SECURITY_REF = ARCBILL.SECURITY_REF "
            "WHERE Security.SECURITY_REF = ARCBILL.SECURITY_REF AND ARCBILL.ARCBILL_KEY IS NOT NULL AND SECURITY.SECURITY_EVENT = 'Billed By' "
            " and SECURITY.TIME_STAMP >= :startTime and SECURITY.TIME_STAMP < :endTime "
        "GROUP BY 2, 3; ";

  return hourlySalesQuery;
}
