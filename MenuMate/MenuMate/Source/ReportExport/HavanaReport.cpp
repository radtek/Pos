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
void THavanaReport::PostDataToFile()
{
    try
    {

        std::auto_ptr<TfrmMallExportRegenerateReport> MallExportRegenerateReport(TfrmMallExportRegenerateReport::Create<TfrmMallExportRegenerateReport>(Screen->ActiveForm));
        MallExportRegenerateReport->btnGenerate->Visible = false;
        MallExportRegenerateReport->btnLoadPath->Visible = false;
        MallExportRegenerateReport->sbAllTerminals->Visible = true;
        MallExportRegenerateReport->sbThisTerminal->Visible = true;
        MallExportRegenerateReport->btnOk->Caption = "Generate Report";
        MallExportRegenerateReport->Caption = "Generate Report";
        TModalResult result = MallExportRegenerateReport->ShowModal();
        TDateTime SDate = MallExportRegenerateReport->SDate;
        TDateTime EDate = MallExportRegenerateReport->EDate;
        bool isAllTerminalSelected = MallExportRegenerateReport->isAllTerminalsSelected;      //todo
        UnicodeString reportExportPath = MallExportRegenerateReport->edLocationPath->Text;
        if(result == mrOk)
        {
            CreateDirectory(reportExportPath);
            PrepareDataForCSVFile(SDate, EDate, reportExportPath, isAllTerminalSelected);
        }
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------
void THavanaReport::CreateDirectory(UnicodeString &reportExportPath)
{
     UnicodeString Format = ".csv";
     //Check if directory not exist than create it.
    if (!DirectoryExists(reportExportPath))
    {
        CreateDir(reportExportPath);
    }

    //CSV will be created with Following Name
    reportExportPath = reportExportPath + Now().FormatString("ddmmyyyyhhnnss") + Format;
}
//------------------------------------------------------------------------------------------
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
            paymentTypes = LoadAllPaymentTypes(dbTransaction, SDate, EDate, isAllTerminalSelected);

            ///Iterate All Payment Types For Creating Headers Of CSV
            for(std::map<UnicodeString,UnicodeString>::iterator itpaymentTypes = paymentTypes.begin(); itpaymentTypes != paymentTypes.end(); ++itpaymentTypes)
            {
                store += itpaymentTypes->first + format;
            }

            store += "Total" + format;

            ////Get All Menus in selected time range
            menuNames = LoadAllMenus(dbTransaction, SDate, EDate, isAllTerminalSelected);

            ///Iterate All Menu Types For Creating Headers Of CSV
            for(std::map<UnicodeString,UnicodeString>::iterator itMenuTypes = menuNames.begin(); itMenuTypes != menuNames.end(); ++itMenuTypes)
            {
                store += itMenuTypes->first + format;
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
std::vector<UnicodeString> THavanaReport::PrepareDataForExport(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate, bool isAllTerminalSelected)
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
        UnicodeString paymentName = "";
        UnicodeString menuName = "";

        //Creating CSV header
        if(!CreateColumn.is_open())
        {
            UnicodeString day,netTotal, gst, grossTotal, rounding;
            int  month, year;

            ///Get All Payment Type for selected date Range
            TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
            TIBSQL *paymentTypeQuery = dbTransaction.Query(dbTransaction.AddQuery());
            TIBSQL *menuTypeQuery = dbTransaction.Query(dbTransaction.AddQuery());

            ibInternalQuery->Close();
            ibInternalQuery->SQL->Text =
                        "SELECT "
                            "EXTRACT (DAY FROM  AB.TIME_STAMP) Bill_Day,     "
                            "EXTRACT (MONTH FROM  AB.TIME_STAMP) Bill_Month, "
                            "EXTRACT (YEAR FROM  AB.TIME_STAMP) Bill_Year ,  "
                            "CAST(SUM( coalesce(T.Tax,0) + COALESCE(T.ServiceChargeTax,0) )as numeric(17, 4)) GST, "
                            "CAST(SUM( CAST(DA.Qty * ABS(COALESCE(DA .BASE_PRICE,0)) as Numeric(17,4))) +SUM(COALESCE((T.ServiceCharge),0)) +SUM(COALESCE((T.Tax),0)) +SUM(COALESCE((T.ServiceChargeTax),0)) "
                            "            +SUM(COALESCE((T.LocalTax),0)) +SUM(COALESCE((T.ProfitTax),0))+ SUM(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,2)) GrossTotal, "
                            "CAST(SUM( CAST(DA.Qty * ABS(COALESCE(DA .BASE_PRICE,0)) as Numeric(17,4))) +SUM(COALESCE((T.ServiceCharge),0))+SUM(COALESCE((T.LocalTax),0))+ "
                            " SUM(COALESCE((T.ProfitTax),0)) + SUM(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,2)) NetTotal "
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
                                  "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 5 THEN ARCORDERTAXES.TAX_VALUE END) AS ProfitTax          "
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
                month = ibInternalQuery->FieldByName("Bill_Month")->AsInteger;
                year = ibInternalQuery->FieldByName("Bill_Year")->AsInteger;
                netTotal = ibInternalQuery->FieldByName("NetTotal")->AsCurrency;
                gst = ibInternalQuery->FieldByName("GST")->AsCurrency;
                grossTotal = ibInternalQuery->FieldByName("GrossTotal")->AsCurrency;
                rounding = "";

                OutputValue = day + format + netTotal + format + gst + format + grossTotal + format + rounding + format;

                //Select all payment types according to days
                paymentTypeQuery->Close();
                paymentTypeQuery->SQL->Text =
                        "SELECT "
                                "EXTRACT (DAY FROM  AB.TIME_STAMP) Bill_Day,  "
                                "EXTRACT (MONTH FROM  AB.TIME_STAMP) Bill_Month, "
                                "EXTRACT (YEAR FROM  AB.TIME_STAMP) Bill_Year , "
                                "ABP.PAY_TYPE, "
                                "CAST(SUM(ABP.SUBTOTAL)AS NUMERIC(17,4)) TOTAL "
                        "FROM ARCBILL AB INNER JOIN ARCBILLPAY ABP ON AB.ARCBILL_KEY = ABP.ARCBILL_KEY "
                        "WHERE EXTRACT (DAY FROM  AB.TIME_STAMP) = :DAY AND EXTRACT (MONTH FROM  AB.TIME_STAMP) = :MONTH "
                                    "AND EXTRACT (YEAR FROM  AB.TIME_STAMP) = :YEAR ";

                if (!isAllTerminalSelected)
                {
                    paymentTypeQuery->SQL->Text = paymentTypeQuery->SQL->Text + " AND  AB.TERMINAL_NAME = :TERMINAL_NAME ";
                }

                paymentTypeQuery->SQL->Text = paymentTypeQuery->SQL->Text +
                        "GROUP BY Bill_Year, Bill_Month, Bill_Day, ABP.PAY_TYPE "
                        "ORDER BY PAY_TYPE ASC ";

                paymentTypeQuery->ParamByName("DAY")->AsInteger = StrToInt(day);
                paymentTypeQuery->ParamByName("MONTH")->AsInteger = month;
                paymentTypeQuery->ParamByName("YEAR")->AsInteger = year;

                if (!isAllTerminalSelected)
                {
                    paymentTypeQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
                }

                paymentTypeQuery->ExecQuery();

                Currency paymentTotal = 0.00;
                while(!paymentTypeQuery->Eof)
                {
                    paymentName = paymentTypeQuery->FieldByName("PAY_TYPE")->AsString;
                    paymentName = RemoveCommas(paymentName);
                    std::map<UnicodeString, UnicodeString>::iterator it = paymentTypes.find(paymentName);

                    if (it != paymentTypes.end())
                    {
                        it->second = paymentTypeQuery->FieldByName("TOTAL")->AsCurrency;
                        paymentTotal += StrToCurr(it->second);
                    }

                    paymentTypeQuery->Next();
                    paymentName = "";
                }

                for(std::map<UnicodeString,UnicodeString>::iterator itpaymentTypes = paymentTypes.begin(); itpaymentTypes != paymentTypes.end(); ++itpaymentTypes)
                {
                    OutputValue += itpaymentTypes->second + format;
                }

                OutputValue += paymentTotal;
                OutputValue += format + rounding + format;

                menuTypeQuery->Close();
                menuTypeQuery->SQL->Text =
                        "SELECT "
                                    "EXTRACT (DAY FROM  AB.TIME_STAMP) Bill_Day,    "
                                    "EXTRACT (MONTH FROM  AB.TIME_STAMP) Bill_Month, "
                                    "EXTRACT (YEAR FROM  AB.TIME_STAMP) Bill_Year , "
                                    "CAST(SUM( CAST(DA.Qty * ABS(COALESCE(DA .BASE_PRICE,0)) as Numeric(17,4))) +SUM(COALESCE((T.ServiceCharge),0)) + "
                                        "SUM(COALESCE((T.Tax),0)) +SUM(COALESCE((T.ServiceChargeTax),0)) + SUM(COALESCE((T.LocalTax),0)) + "
                                        "SUM(COALESCE((T.ProfitTax),0))+ SUM(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,2)) GrossTotal, "
                                    "DA.MENU_NAME "
                        "FROM ARCBILL AB    "
                        "INNER JOIN ARCHIVE DA ON AB.ARCBILL_KEY = DA.ARCBILL_KEY "
                          "LEFT JOIN "
                                "( "
                                "   SELECT "
                                "      a.ARCHIVE_KEY, "
                                "      sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE, "
                                "      a.DISCOUNT_GROUPNAME "
                                "   FROM ARCORDERDISCOUNTS a  "
                                "   GROUP BY a.ARCHIVE_KEY, a.DISCOUNT_GROUPNAME "
                                ") AOD ON DA.ARCHIVE_KEY = AOD.ARCHIVE_KEY "
                         " LEFT JOIN "
                                "( "
                                   "SELECT "
                                     " ARCORDERTAXES.ARCHIVE_KEY, "
                                     " MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS Tax, "
                                     " MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
                                     " MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax, "
                                     " MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 4 THEN ARCORDERTAXES.TAX_VALUE END) AS LocalTax, "
                                     " MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 5 THEN ARCORDERTAXES.TAX_VALUE END) AS ProfitTax "
                                   "FROM "
                                      "( "
                                        " SELECT "
                                        "    a.ARCHIVE_KEY, "
                                        "    a.TAX_TYPE,   "
                                        "    CAST(SUM(COALESCE(a.TAX_VALUE,0) ) as Numeric(17,4)) TAX_VALUE "
                                        " FROM ARCORDERTAXES a "
                                        " GROUP BY a.TAX_TYPE, a.ARCHIVE_KEY "
                                        " ORDER BY 1   "
                                      ") ARCORDERTAXES "
                                   "GROUP BY ARCORDERTAXES.ARCHIVE_KEY "
                               " ) T ON DA.ARCHIVE_KEY = T.ARCHIVE_KEY "
                         "WHERE (COALESCE(AOD.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' AND COALESCE(AOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary' ) and "
                                "EXTRACT (DAY FROM  AB.TIME_STAMP) = :DAY AND EXTRACT (MONTH FROM  AB.TIME_STAMP) = :MONTH "
                                            "AND EXTRACT (YEAR FROM  AB.TIME_STAMP) = :YEAR ";
                if (!isAllTerminalSelected)
                {
                    menuTypeQuery->SQL->Text = menuTypeQuery->SQL->Text + " AND  AB.TERMINAL_NAME = :TERMINAL_NAME ";
                }

                menuTypeQuery->SQL->Text = menuTypeQuery->SQL->Text +
                     "GROUP BY Bill_Year, Bill_Month, Bill_Day,DA.MENU_NAME "
                     " ORDER BY DA.MENU_NAME ASC ";

                menuTypeQuery->ParamByName("DAY")->AsInteger = StrToInt(day);
                menuTypeQuery->ParamByName("MONTH")->AsInteger = month;
                menuTypeQuery->ParamByName("YEAR")->AsInteger = year;

                if (!isAllTerminalSelected)
                {
                    menuTypeQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
                }

                menuTypeQuery->ExecQuery();
                Currency menuTotal = 0.00;

                while(!menuTypeQuery->Eof)
                {
                    menuName = menuTypeQuery->FieldByName("MENU_NAME")->AsString;
                    menuName = RemoveCommas(menuName);
                    std::map<UnicodeString, UnicodeString>::iterator it = menuNames.find(menuName);

                    if (it != menuNames.end())
                    {
                        it->second = menuTypeQuery->FieldByName("GrossTotal")->AsCurrency;
                        menuTotal += StrToCurr(it->second);
                    }

                    menuTypeQuery->Next();
                    menuName = "";
                }

                for(std::map<UnicodeString,UnicodeString>::iterator itMenuTypes = menuNames.begin(); itMenuTypes != menuNames.end(); ++itMenuTypes)
                {
                    OutputValue += itMenuTypes->second + format;
                }

                rounding = paymentTotal - menuTotal;
                OutputValue += menuTotal ;
                OutputValue += format + rounding + newLine;

                DataToWrite.push_back(OutputValue.t_str());

                rounding = "";

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
std::map<UnicodeString,UnicodeString> THavanaReport::LoadAllPaymentTypes(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected)
{
    std::map<UnicodeString,UnicodeString> paymentTypes;
    try
    {
        UnicodeString paymentType = "";
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
            paymentType = ibInternalQuery->FieldByName("PAY_TYPE")->AsString;
            paymentType = RemoveCommas(paymentType);
            paymentTypes.insert ( std::pair<UnicodeString,UnicodeString>(paymentType,"") );
            ibInternalQuery->Next();
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return paymentTypes;
}
//------------------------------------------------------------------------------
std::map<UnicodeString,UnicodeString> THavanaReport::LoadAllMenus(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected)
{
    std::map<UnicodeString,UnicodeString> menus;
    try
    {
        UnicodeString menuName = "";
        TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
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
            menuName = ibInternalQuery->FieldByName("MENU_NAME")->AsString;
            menuName = RemoveCommas(menuName);
            menus.insert ( std::pair<UnicodeString,UnicodeString>(menuName,"") );
            ibInternalQuery->Next();
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return menus;
}
//-----------------------------------------------------------------------------------------------------
UnicodeString THavanaReport::RemoveCommas(UnicodeString str)
{
    UnicodeString retVal = "";
    const char* line = str.t_str();
    for(int index = 0; line[index] != '\0'; index++)
    {
        if (line[index] == ',')
        {
            continue;
        }
        else
        {
            retVal+= (line[index]);
        }
    }
   return retVal;
}
//--------------------------------------------------------------------------------------------------------
void THavanaReport::CreateFileExportPath(UnicodeString &reportExportPath)
{
    UnicodeString Format = ".csv";
     //Check if directory not exist than create it.
    if (!DirectoryExists(reportExportPath))
    {
        CreateDir(reportExportPath);
    }

    //CSV will be created with Following Name
    reportExportPath = reportExportPath + Now().FormatString("ddmmyyyyhhnnss") + Format;
}
//--------------------------------------------------------------------------------------------------------
void THavanaReport::PrepareDataForCSVFile(TDateTime SDate, TDateTime EDate, UnicodeString reportExportPath, bool isAllTerminalSelected)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        std::vector<UnicodeString> dataToWrite;
        dataToWrite =  CreateHeaderFormat(dbTransaction, SDate, EDate, isAllTerminalSelected);

        //Use Federal Mall's Function of printing Header
        TMallExportOutputDBDriver  exportCSV;
        exportCSV.GenerateTXT(dataToWrite, reportExportPath, "ZTXTHEADER");

        //Clear DataToWrite Vector
        dataToWrite.erase (dataToWrite.begin(),dataToWrite.end());

        //Load all data into vector
        dataToWrite =  PrepareDataForExport(dbTransaction, SDate, EDate, isAllTerminalSelected);

        //calling existing mall function for insert Data into CSV
        exportCSV.GenerateTXT(dataToWrite, reportExportPath, "ZTXTDATA");

        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
