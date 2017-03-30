//---------------------------------------------------------------------------


#pragma hdrstop

#include "HavanaReport.h"
#include "MMLogging.h"
#include "ExportCSV.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

THavanaReport::THavanaReport()
{
    innerQuery = "INNER JOIN ARCHIVE DA ON AB.ARCBILL_KEY = DA.ARCBILL_KEY "
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
                               " ) T ON DA.ARCHIVE_KEY = T.ARCHIVE_KEY ";
}
//-----------------------------------------------------------------------------
void THavanaReport::PostDataToFile()
{
    try
    {
        ///Use Existing Reportregenaration From
        std::auto_ptr<TfrmMallExportRegenerateReport> MallExportRegenerateReport(TfrmMallExportRegenerateReport::Create<TfrmMallExportRegenerateReport>(Screen->ActiveForm));
        MallExportRegenerateReport->btnGenerate->Visible = false;
        MallExportRegenerateReport->btnLoadPath->Visible = false;
        MallExportRegenerateReport->sbAllTerminals->Visible = true;
        MallExportRegenerateReport->sbThisTerminal->Visible = true;
        MallExportRegenerateReport->btnOk->Caption = "Generate Report";
        MallExportRegenerateReport->Caption = "Generate Report";
        MallExportRegenerateReport->cbStartHour->ItemIndex = TGlobalSettings::Instance().EndOfDay;
        MallExportRegenerateReport->cbEndHour->ItemIndex = TGlobalSettings::Instance().EndOfDay;
        MallExportRegenerateReport->cbStartMin->ItemIndex = 0;
        MallExportRegenerateReport->cbEndMin->ItemIndex = 0;
        MallExportRegenerateReport->StartHour = MallExportRegenerateReport->cbStartHour->ItemIndex;
        MallExportRegenerateReport->StartHour = "0" + MallExportRegenerateReport->StartHour;
        MallExportRegenerateReport->EndHour = MallExportRegenerateReport->cbEndHour->ItemIndex;
        MallExportRegenerateReport->EndHour = "0" + MallExportRegenerateReport->EndHour;
        MallExportRegenerateReport->StartMin = MallExportRegenerateReport->cbStartMin->ItemIndex;
        MallExportRegenerateReport->StartMin = "0" + MallExportRegenerateReport->StartMin;
        MallExportRegenerateReport->EndMin = MallExportRegenerateReport->cbEndMin->ItemIndex;
        MallExportRegenerateReport->EndMin = "0" + MallExportRegenerateReport->EndMin;
        MallExportRegenerateReport->InitializeTimeSet(MallExportRegenerateReport->SDate, MallExportRegenerateReport->EDate);
        MallExportRegenerateReport->ShowDateTimes();
        MallExportRegenerateReport->isAllTerminalsSelected = true;
        MallExportRegenerateReport->edLocationPath->Text = TGlobalSettings::Instance().ReportExportPath;
        TModalResult result = MallExportRegenerateReport->ShowModal();
        TDateTime SDate = MallExportRegenerateReport->SDate;
        TDateTime EDate = MallExportRegenerateReport->EDate;
        bool isAllTerminalSelected = MallExportRegenerateReport->isAllTerminalsSelected;
        UnicodeString reportExportPath = MallExportRegenerateReport->edLocationPath->Text;
        if(result == mrOk && reportExportPath != "")
        {    
            if(EDate >= SDate)
            {
                //Prepare Data That will be written to file.
                PrepareDataForCSVFile(SDate, EDate, reportExportPath, isAllTerminalSelected);
            }
            else
            {
                MessageBox( "End date is set prior to start date", "Invalid Date For File Generation", MB_OK );
            }
        }
        else if(reportExportPath == "" && result != mrCancel)
            MessageBox( "Please Enter Report Export Path.", "No Export Path Specified", MB_OK );
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//-----------------------------------------------------------------------------------
void THavanaReport::CreateDirectory(UnicodeString &reportExportPath, TDateTime SDate, TDateTime EDate)
{
    try
    {
         UnicodeString Format = ".csv";
         TSearchRec sr;
         int iAttributes = faAnyFile;

         //Check if directory not exist than create it.
        if (!DirectoryExists(reportExportPath))
        {
            CreateDir(reportExportPath);
        }

        reportExportPath = reportExportPath +  "\\";

         TExportCSV exportCSV;
         //Check any csv file exist on the path specified
        if( FindFirst( reportExportPath + "*.CSV", iAttributes, sr ) == 0 )
        {
            do
            {
                if (sr.Attr & iAttributes)
                {
                    exportCSV.MoveCSVFile( reportExportPath + sr.Name, reportExportPath + "Archive CSV\\" );
                }
            }
            while(FindNext(sr) == 0);
        }

        //CSV will be created with Following Name
        reportExportPath = reportExportPath +  "Havana Combined Customized Daily Sales From " + SDate.FormatString("ddmmyyyy-hhnnss") + " To " + EDate.FormatString("ddmmyyyy-hhnnss") + Format;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
	}
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

            store += "Total,Rounding" + format;

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
            Currency netTotalSum = 0.00, gstSum = 0.00, grossTotalSum = 0.00;
            int  month, year;

            TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
            TIBSQL *paymentTypeQuery = dbTransaction.Query(dbTransaction.AddQuery());
            TIBSQL *menuTypeQuery = dbTransaction.Query(dbTransaction.AddQuery());

            ///Get All Payment Type for selected date Range
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

                       + innerQuery +

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
                netTotalSum += ibInternalQuery->FieldByName("NetTotal")->AsCurrency;
                netTotal = ibInternalQuery->FieldByName("NetTotal")->AsCurrency;
                ibInternalQuery->FieldByName("GST")->AsCurrency;
                gstSum += ibInternalQuery->FieldByName("GST")->AsCurrency;
                gst = ibInternalQuery->FieldByName("GST")->AsCurrency;
                grossTotalSum += ibInternalQuery->FieldByName("GrossTotal")->AsCurrency;
                grossTotal = ibInternalQuery->FieldByName("GrossTotal")->AsCurrency;
                rounding = "";

                OutputValue = day + format + netTotal + format + gst + format + grossTotal + format + rounding + format;

                //Select all payment types according to days
                paymentTypeQuery->Close();
                paymentTypeQuery->SQL->Text =
                     "SELECT "
                            "PAYTYPE.Bill_Day, "
                            "PAYTYPE.Bill_Month, "
                            "PAYTYPE.Bill_Year , "
                            "PAYTYPE.PAY_TYPE, "
                            "SUM(PAYTYPE.TOTAL)TOTAL "
                    "FROM ( "
                        "SELECT "
                                "EXTRACT (DAY FROM  AB.TIME_STAMP) Bill_Day,  "
                                "EXTRACT (MONTH FROM  AB.TIME_STAMP) Bill_Month, "
                                "EXTRACT (YEAR FROM  AB.TIME_STAMP) Bill_Year , "
                                "ABP.PAY_TYPE, "
                                "CAST(SUM( CASE WHEN (ABP.PAY_TYPE) = 'Cash' THEN (COALESCE(ABP.SUBTOTAL,0) + COALESCE(CashChange.Cashout,0)) "
                                                        "ELSE (COALESCE(ABP.SUBTOTAL,0)) END) AS NUMERIC(17,4)) TOTAL  "
                        "FROM ARCBILL AB INNER JOIN ARCBILLPAY ABP ON AB.ARCBILL_KEY = ABP.ARCBILL_KEY "
                        "INNER JOIN(select A.ARCBILL_KEY from ARCHIVE A WHERE EXTRACT (DAY FROM  A.TIME_STAMP_BILLED) = :DAY "
                                        "AND EXTRACT (MONTH FROM  A.TIME_STAMP_BILLED) = :MONTH AND EXTRACT (YEAR FROM  A.TIME_STAMP_BILLED) = :YEAR "
                                        "AND A.TIME_STAMP_BILLED >= :START_TIME and A.TIME_STAMP_BILLED < :END_TIME "
                                        "GROUP BY 1)ARC ON AB.ARCBILL_KEY =  ARC.ARCBILL_KEY "
                        "LEFT JOIN (SELECT A.ARCBILL_KEY, MIN(CASE WHEN a.CASH_OUT = 'T' THEN COALESCE(a.SUBTOTAL,0) END) AS Cashout "
                                      "FROM ARCBILLPAY a  INNER JOIN ARCBILL AB ON A.ARCBILL_KEY = AB.ARCBILL_KEY WHERE EXTRACT (DAY FROM  AB.TIME_STAMP) = :DAY "
                                        "AND EXTRACT (MONTH FROM  AB.TIME_STAMP) = :MONTH AND EXTRACT (YEAR FROM  AB.TIME_STAMP) = :YEAR "
                                        "AND AB.Time_Stamp >= :START_TIME and AB.Time_Stamp < :END_TIME "
                                        "GROUP BY 1)CashChange ON AB.ARCBILL_KEY = CashChange.ARCBILL_KEY "
                        "WHERE EXTRACT (DAY FROM  AB.TIME_STAMP) = :DAY AND EXTRACT (MONTH FROM  AB.TIME_STAMP) = :MONTH "
                                    "AND EXTRACT (YEAR FROM  AB.TIME_STAMP) = :YEAR AND AB.Time_Stamp >= :START_TIME and AB.Time_Stamp < :END_TIME "
                                    "AND ABP.CASH_OUT <> 'T' AND  ABP.SUBTOTAL <> 0 "
                                    "And ab.INVOICE_NUMBER not in (SELECT a.INVOICE_NUMBER  FROM POINTSTRANSACTIONS a "
                                                           " where a.ADJUSTMENT_TYPE = 1 AND EXTRACT (DAY FROM  a.TIME_STAMP) = :DAY AND "
                                                           "EXTRACT (MONTH FROM  a.TIME_STAMP) = :MONTH AND EXTRACT (YEAR FROM  a.TIME_STAMP) = :YEAR "
                                                           "GROUP BY a.INVOICE_NUMBER) "  ;

                if (!isAllTerminalSelected)
                {
                    paymentTypeQuery->SQL->Text = paymentTypeQuery->SQL->Text + " AND  AB.TERMINAL_NAME = :TERMINAL_NAME ";
                }

                paymentTypeQuery->SQL->Text = paymentTypeQuery->SQL->Text +
                        "GROUP BY Bill_Year, Bill_Month, Bill_Day, ABP.PAY_TYPE "

                 "UNION ALL "

                "SELECT "
                    "EXTRACT (DAY FROM  ARCBILL.TIME_STAMP) Bill_Day,     "
                    "EXTRACT (MONTH FROM  ARCBILL.TIME_STAMP) Bill_Month, "
                    "EXTRACT (YEAR FROM  ARCBILL.TIME_STAMP) Bill_Year ,  "
                    "ARCBILLPAY.PAY_TYPE, "
                    "CAST(((( COALESCE(sum(ARCBILLPAY.SUBTOTAL),0))/Sum(ARCBILL.TOTAL))*(ARCBILL.TOTAL - pt.ADJUSTMENT)) as numeric(17, 4)) AS TOTAL "
                "FROM ARCBILLPAY "
                "INNER JOIN ARCBILL on ARCBILL.ARCBILL_KEY=ARCBILLPAY.ARCBILL_KEY "
                "INNER JOIN POINTSTRANSACTIONS pt on ARCBILL.INVOICE_NUMBER = pt.INVOICE_NUMBER "
                "WHERE pt.ADJUSTMENT_TYPE = 1 and ARCBILLPAY.SUBTOTAL <> 0  AND EXTRACT (DAY FROM  ARCBILL.TIME_STAMP) = :DAY AND EXTRACT (MONTH FROM  ARCBILL.TIME_STAMP) = :MONTH "
                                    "AND EXTRACT (YEAR FROM  ARCBILL.TIME_STAMP) = :YEAR AND ARCBILL.Time_Stamp >= :START_TIME and ARCBILL.Time_Stamp < :END_TIME "
                "GROUP BY Bill_Year, Bill_Month, Bill_Day,ARCBILLPAY.PAY_TYPE ,ARCBILLPAY.ARCBILL_KEY,pt.ADJUSTMENT,ARCBILL.TOTAL, ARCBILLPAY.SUBTOTAL "
                "ORDER BY 4 ASC )PAYTYPE "
        " GROUP BY Bill_Year, Bill_Month, Bill_Day, PAY_TYPE ";

                paymentTypeQuery->ParamByName("DAY")->AsInteger = StrToInt(day);
                paymentTypeQuery->ParamByName("MONTH")->AsInteger = month;
                paymentTypeQuery->ParamByName("YEAR")->AsInteger = year;
                paymentTypeQuery->ParamByName("START_TIME")->AsDateTime = SDate;
                paymentTypeQuery->ParamByName("END_TIME")->AsDateTime = EDate;

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
                    std::map<UnicodeString, UnicodeString>::iterator itp = paymentTypeTotal.find(paymentName);

                    ////Load value Corresponding to payment type into map
                    if (it != paymentTypes.end())
                    {
                        it->second = paymentTypeQuery->FieldByName("TOTAL")->AsCurrency;
                        paymentTotal += StrToCurr(it->second);
                    }

                    ////Load paymenttypetotal Corresponding to payment type into map
                    if (itp != paymentTypeTotal.end())
                    {
                        if(itp->second!= "")
                        {
                            itp->second = StrToCurr(itp->second) + paymentTypeQuery->FieldByName("TOTAL")->AsCurrency;
                        }
                        else
                        {
                            itp->second = paymentTypeQuery->FieldByName("TOTAL")->AsCurrency;
                        }
                    }

                    paymentTypeQuery->Next();
                    paymentName = "";
                }

                //Append payment type values into payment type
                for(std::map<UnicodeString,UnicodeString>::iterator itpaymentTypes = paymentTypes.begin(); itpaymentTypes != paymentTypes.end(); ++itpaymentTypes)
                {
                    OutputValue += itpaymentTypes->second + format;
                    itpaymentTypes->second = "";
                }

                OutputValue += paymentTotal;
                OutputValue += format + rounding + format;

                //Select All Menu Types during selected time range
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

                         + innerQuery +

                         "WHERE (COALESCE(AOD.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' AND COALESCE(AOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary' ) and "
                                "EXTRACT (DAY FROM  AB.TIME_STAMP) = :DAY AND EXTRACT (MONTH FROM  AB.TIME_STAMP) = :MONTH "
                                "AND EXTRACT (YEAR FROM  AB.TIME_STAMP) = :YEAR AND AB.Time_Stamp >= :START_TIME and AB.Time_Stamp < :END_TIME ";
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
                menuTypeQuery->ParamByName("START_TIME")->AsDateTime = SDate;
                menuTypeQuery->ParamByName("END_TIME")->AsDateTime = EDate;

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
                    std::map<UnicodeString, UnicodeString>::iterator itm = menuTotals.find(menuName);

                    ////Update map According to payment type
                    if (it != menuNames.end() )
                    {
                        it->second = menuTypeQuery->FieldByName("GrossTotal")->AsCurrency;
                        menuTotal += StrToCurr(it->second);
                    }

                    ///Update MenuType Total According to menu type
                    if (itm != menuTotals.end())
                    {
                        if(itm->second!= "")
                        {
                            itm->second = StrToCurr(itm->second) + menuTypeQuery->FieldByName("GrossTotal")->AsCurrency;
                        }
                        else
                        {
                            itm->second = menuTypeQuery->FieldByName("GrossTotal")->AsCurrency;
                        }
                    }

                    menuTypeQuery->Next();
                    menuName = "";
                }

                //Append Menu type sale values in the string for writing to csv
                for(std::map<UnicodeString,UnicodeString>::iterator itMenuTypes = menuNames.begin(); itMenuTypes != menuNames.end(); ++itMenuTypes)
                {
                    OutputValue += itMenuTypes->second + format;
                    itMenuTypes->second = "";
                }

                rounding = paymentTotal - menuTotal;
                OutputValue += menuTotal ;

                //Check Rounding value is Negative include Brackets As String
               if(StrToCurr(rounding) < 0)
               {
                //rounding = rounding*-1;
                rounding = "(" + StrToCurr(rounding)*-1 + ")";
               }

                OutputValue += format + rounding + newLine;

                DataToWrite.push_back(OutputValue.t_str());

                rounding = "";

                ibInternalQuery->Next();
            }

            ///Write Total Row in the CSV
            if(ibInternalQuery->RecordCount)
            {
                OutputValue = "";
                OutputValue = "Total," + netTotalSum + format + gstSum + format + grossTotalSum + format + rounding + format;
                Currency paymentTotal = 0.00;
                Currency menuTotal = 0.00;
                UnicodeString totalRounding = "";

                ///Write All Payment Types
                for(std::map<UnicodeString,UnicodeString>::iterator itpaymentTypes = paymentTypeTotal.begin(); itpaymentTypes != paymentTypeTotal.end(); ++itpaymentTypes)
                {
                    OutputValue += itpaymentTypes->second + format;

                    if(itpaymentTypes->second != "")
                        paymentTotal += StrToCurr(itpaymentTypes->second);
                }

                OutputValue += paymentTotal;
                OutputValue += format + rounding + format;

                //Write All Menu Types
                for(std::map<UnicodeString,UnicodeString>::iterator itMenuTypes = menuTotals.begin(); itMenuTypes != menuTotals.end(); ++itMenuTypes)
                {
                    OutputValue += itMenuTypes->second + format;

                    if(itMenuTypes->second != "")
                         menuTotal += StrToCurr(itMenuTypes->second);
                }

                totalRounding = paymentTotal - menuTotal;

                //Check Rounding value is Negative include Brackets As String
                 if(StrToCurr(totalRounding) < 0)
                 {
                    //rounding = rounding*-1;
                    totalRounding = "(" + StrToCurr(totalRounding)*-1 + ")";
                 }

                OutputValue += menuTotal ;
                OutputValue += format + totalRounding + newLine;

                DataToWrite.push_back(OutputValue.t_str());

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
        ibInternalQuery->SQL->Text = "SELECT * FROM (SELECT CASE WHEN (ABP.PAY_TYPE != 'Credit') THEN (ABP.PAY_TYPE) "
                                                  "WHEN (ABP.PAY_TYPE = 'Credit' AND ABP.SUBTOTAL > 0) THEN (ABP.PAY_TYPE) END PAY_TYPE "
                                     "FROM ARCBILL AB INNER JOIN ARCBILLPAY ABP ON AB.ARCBILL_KEY = ABP.ARCBILL_KEY "
                                     "WHERE AB.TIME_STAMP >= :START_TIME AND AB.TIME_STAMP < :END_TIME  AND ABP.SUBTOTAL <> 0 ";

        if (!isAllTerminalSelected)
        {
            ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + " AND  AB.TERMINAL_NAME = :TERMINAL_NAME ";
        }

        ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text +
                                    "GROUP BY 1 "
                                     "ORDER BY 1 ASC )ABP "
                                     "WHERE ABP.PAY_TYPE != '' " ;

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
        paymentTypeTotal = paymentTypes;
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
        menuTotals = menus;
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
void THavanaReport::PrepareDataForCSVFile(TDateTime sDate, TDateTime eDate, UnicodeString reportExportPath, bool isAllTerminalSelected)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {

        if(reportExportPath != TGlobalSettings::Instance().ReportExportPath)
        {
            TGlobalSettings::Instance().ReportExportPath = reportExportPath;
            TManagerVariable::Instance().SetDeviceStr(dbTransaction,vmReportExportPath,TGlobalSettings::Instance().ReportExportPath);
        }
         ///Register Query
        Database::TcpIBSQL ibInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(ibInternalQuery);

         //Query for selecting data for invoice file
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text =  "SELECT a.ARCBILL_KEY FROM ARCBILL a "
                                        "WHERE a.Time_Stamp >= :START_TIME AND a.Time_Stamp < :END_TIME ";

         if (!isAllTerminalSelected)
        {
            ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + " AND  a.TERMINAL_NAME = :TERMINAL_NAME ";
        }

        ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + "ORDER BY 1 ASC ";

        ibInternalQuery->ParamByName("START_TIME")->AsDateTime = sDate;
        ibInternalQuery->ParamByName("END_TIME")->AsDateTime = eDate;
        if(!isAllTerminalSelected)
        {
            ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
        }
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            //Create Directory If not Exist
            CreateDirectory(reportExportPath,sDate, eDate);

            std::vector<UnicodeString> dataToWrite;
            dataToWrite =  CreateHeaderFormat(dbTransaction, sDate, eDate, isAllTerminalSelected);

            //Use Federal Mall's Function of printing Header
            TMallExportOutputDBDriver  exportCSV;
            exportCSV.GenerateTXT(dataToWrite, reportExportPath, "ZTXTHEADER");

            //Clear DataToWrite Vector
            dataToWrite.erase (dataToWrite.begin(),dataToWrite.end());

            //Load all data into vector
            dataToWrite =  PrepareDataForExport(dbTransaction, sDate, eDate, isAllTerminalSelected);

            //calling existing mall function for insert Data into CSV
            exportCSV.GenerateTXT(dataToWrite, reportExportPath, "ZTXTDATA");

            MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );
        }
        else
        {
            MessageBox( "No Data For This Time Period", "Gernerating File", MB_OK );
        }

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
//--------------------------------------------------------------------------------------------------------------
