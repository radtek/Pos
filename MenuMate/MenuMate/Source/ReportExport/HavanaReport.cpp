//---------------------------------------------------------------------------


#pragma hdrstop

#include "HavanaReport.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

THavanaReport::THavanaReport()
{
}
//-----------------------------------------------------------------------------
std::vector<UnicodeString> THavanaReport::CreateHeaderFormat(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate, bool isAllTerminalSelected)
{
    std::vector<UnicodeString> DataToWrite;
    try
    {
        UnicodeString store = "";
        UnicodeString format = ",";
        UnicodeString newLine = "\n";
        std::ofstream CreateColumn;


        //Creating CSV header
        if(!CreateColumn.is_open())
        {
            store = "Day,NetTotal,GST,GrossTotal,Rounding" + format ;

            ///Get All Payment Type for selected date Range
            TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
            ibInternalQuery->Close();
            ibInternalQuery->SQL->Text = "SELECT ABP.PAY_TYPE PAY_TYPE "
                                         "FROM ARCBILL AB INNER JOIN ARCBILLPAY ABP ON AB.ARCBILL_KEY = ABP.ARCBILL_KEY "
                                         "WHERE AB.TIME_STAMP >= :START_TIME AND AB.TIME_STAMP < :END_TIME ";

            if (!isAllTerminalSelected)
            {
                ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + " AND  AB.TERMINAL_NAME = :TERMINAL_NAME ";
            }

            ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text +
                                        "GROUP BY ABP.PAY_TYPE "
                                         "ORDER BY 1 ASC ";

            ibInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
            ibInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;

            if (!isAllTerminalSelected)
            {
                ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
            }

            ibInternalQuery->ExecQuery();

            while (!ibInternalQuery->Eof)
            {
                store += ibInternalQuery->FieldByName("PAY_TYPE")->AsString;
                store += format;
                ibInternalQuery->Next();
            }

            store += "Total" + format;

            ////Get All Menus in selected time range
            ibInternalQuery->Close();
            ibInternalQuery->SQL->Text =  "SELECT a.MENU_NAME "
                                            "FROM ARCHIVE a "
                                            "WHERE a.TIME_STAMP_BILLED >= :START_TIME and a.TIME_STAMP_BILLED < :END_TIME ";

            if (!isAllTerminalSelected)
            {
                ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + " AND  a.TERMINAL_NAME = :TERMINAL_NAME ";
            }

            ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text +
                                           "GROUP BY A.MENU_NAME "
                                            "ORDER BY A.MENU_NAME ASC ";

            ibInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
            ibInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;

            if (!isAllTerminalSelected)
            {
                ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
            }

            ibInternalQuery->ExecQuery();

            while (!ibInternalQuery->Eof)
            {
                store += ibInternalQuery->FieldByName("MENU_NAME")->AsString;
                store += format;
                ibInternalQuery->Next();
            }

            store += "Total,Rounding" + newLine;

            DataToWrite.push_back(store.t_str());
        }
        CreateColumn.close();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return DataToWrite;
}
//------------------------------------------------------------------------------
std::vector<UnicodeString> THavanaReport::LoadDataFromDB(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate, bool isAllTerminalSelected)
{
    std::vector<UnicodeString> DataToWrite;
    try
    {
        UnicodeString store = "";
        UnicodeString format = ",";
        UnicodeString newLine = "\n";
        std::ofstream CreateColumn;
        UnicodeString OutputValue = "";
        UnicodeString terminalCondition = "";

        //Creating CSV header
        if(!CreateColumn.is_open())
        {
            UnicodeString day, netTotal, gst, grossTotal, rounding;

            ///Get All Payment Type for selected date Range
            TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
            ibInternalQuery->Close();
            ibInternalQuery->SQL->Text =
                        "SELECT "
                            "EXTRACT (DAY FROM  AB.TIME_STAMP) Bill_Day,     "
                            "EXTRACT (MONTH FROM  AB.TIME_STAMP) Bill_Month, "
                            "EXTRACT (YEAR FROM  AB.TIME_STAMP) Bill_Year ,  "
                            "CAST(SUM( coalesce(T.Tax,0) + COALESCE(T.ServiceChargeTax,0) )as numeric(17, 4)) GST, "
                            "CAST(SUM( CAST(DA.Qty * ABS(COALESCE(DA .BASE_PRICE,0)) as Numeric(17,4))) +SUM(COALESCE((T.ServiceCharge),0)) +SUM(COALESCE((T.Tax),0)) +SUM(COALESCE((T.ServiceChargeTax),0)) "
                            "            +SUM(COALESCE((T.LocalTax),0))+ SUM(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,2)) GrossTotal, "
                            "CAST(SUM( CAST(DA.Qty * ABS(COALESCE(DA .BASE_PRICE,0)) as Numeric(17,4))) +SUM(COALESCE((T.ServiceCharge),0))+SUM(COALESCE((T.LocalTax),0))+ "
                            "        SUM(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,2)) NetTotal "
                      "FROM ARCBILL AB "
                      "INNER JOIN ARCHIVE DA ON AB.ARCBILL_KEY = DA.ARCBILL_KEY "
                      "LEFT JOIN  "
                            "(    "
                              " SELECT            "
                              "    a.ARCHIVE_KEY, "
                              "    sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE, "
                              "    a.DISCOUNT_GROUPNAME                      "
                              " FROM ARCORDERDISCOUNTS a                     "
                              " GROUP BY a.ARCHIVE_KEY, a.DISCOUNT_GROUPNAME "
                            ") AOD ON DA.ARCHIVE_KEY = AOD.ARCHIVE_KEY       "
                      "LEFT JOIN    "
                            "(         "
                               "SELECT "
                                  "ARCORDERTAXES.ARCHIVE_KEY, "
                                  "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS Tax,              "
                                  "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,    "
                                  "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax, "
                                  "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 4 THEN ARCORDERTAXES.TAX_VALUE END) AS LocalTax,          "
                                  "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 4 THEN ARCORDERTAXES.TAX_VALUE END) AS ProfitTax          "
                               "FROM "
                                  "( "
                                  "   SELECT "
                                  "      a.ARCHIVE_KEY, "
                                  "      a.TAX_TYPE,    "
                                  "      CAST(SUM(COALESCE(a.TAX_VALUE,0) ) as Numeric(17,4)) TAX_VALUE "
                                  "   FROM ARCORDERTAXES a               "
                                  "   GROUP BY a.TAX_TYPE, a.ARCHIVE_KEY "
                                  "   ORDER BY 1   "
                                  ") ARCORDERTAXES "
                               "GROUP BY ARCORDERTAXES.ARCHIVE_KEY  "
                            ") T ON DA.ARCHIVE_KEY = T.ARCHIVE_KEY "
                 "WHERE (COALESCE(AOD.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' AND COALESCE(AOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary' ) "
                        "AND AB.Time_Stamp >= :START_TIME and AB.Time_Stamp < :END_TIME ";

            if (!isAllTerminalSelected)
            {
                ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + " AND  AB.TERMINAL_NAME = :TERMINAL_NAME ";
            }

            ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text +  "GROUP BY Bill_Year, Bill_Month, Bill_Day ";

            ibInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
            ibInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;

            if (!isAllTerminalSelected)
            {
                ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
            }

            ibInternalQuery->ExecQuery();

            while (!ibInternalQuery->Eof)
            {
                day = ibInternalQuery->FieldByName("Bill_Day")->AsInteger;
                netTotal = ibInternalQuery->FieldByName("NetTotal")->AsCurrency;
                gst = ibInternalQuery->FieldByName("GST")->AsCurrency;
                grossTotal == ibInternalQuery->FieldByName("GrossTotal")->AsCurrency;
                rounding = "";

                OutputValue = day + format + netTotal + format + gst + format + grossTotal + format + "\n";
                DataToWrite.push_back(store.t_str());

                ///todo next payment type selection

                ibInternalQuery->Next();
            }
        }
        CreateColumn.close();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return DataToWrite;
}
//------------------------------------------------------------------------------
