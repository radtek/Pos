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
std::vector<UnicodeString> THavanaReport::CreateHeaderFormat(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate)
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
            ibInternalQuery->SQL->Text = "SELECT UPPER(ABP.PAY_TYPE) PAY_TYPE "
                                         "FROM ARCBILL AB INNER JOIN ARCBILLPAY ABP ON AB.ARCBILL_KEY = ABP.ARCBILL_KEY "
                                         "WHERE AB.TIME_STAMP >= :START_TIME AND AB.TIME_STAMP < :END_TIME "
                                         "GROUP BY UPPER(ABP.PAY_TYPE) "
                                         "ORDER BY 1 ASC ";

            ibInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
            ibInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;

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
                                            "WHERE a.TIME_STAMP_BILLED >= :START_TIME and a.TIME_STAMP_BILLED < :END_TIME "
                                            "GROUP BY A.MENU_NAME "
                                            "ORDER BY A.MENU_NAME ASC ";

            ibInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
            ibInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;

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
std::vector<UnicodeString> THavanaReport::LoadDataFromDB(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate)
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
            ibInternalQuery->SQL->Text = "SELECT UPPER(ABP.PAY_TYPE) PAY_TYPE "
                                         "FROM ARCBILL AB INNER JOIN ARCBILLPAY ABP ON AB.ARCBILL_KEY = ABP.ARCBILL_KEY "
                                         "WHERE AB.TIME_STAMP >= :START_TIME AND AB.TIME_STAMP < :END_TIME "
                                         "GROUP BY UPPER(ABP.PAY_TYPE) "
                                         "ORDER BY 1 ASC ";

            ibInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
            ibInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;

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
                                            "WHERE a.TIME_STAMP_BILLED >= :START_TIME and a.TIME_STAMP_BILLED < :END_TIME "
                                            "GROUP BY A.MENU_NAME "
                                            "ORDER BY A.MENU_NAME ASC ";

            ibInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
            ibInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;

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
