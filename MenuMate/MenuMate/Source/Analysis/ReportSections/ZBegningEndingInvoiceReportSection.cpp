#include "ZBegningEndingInvoiceReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"
#include "GlobalSettings.h"
#include "ReceiptUtility.h"

ZBegningEndingInvoiceReportSection::ZBegningEndingInvoiceReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	: BaseReportSection(mmXReport, mmShowBegningandEndingBalance, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}

ZBegningEndingInvoiceReportSection::ZBegningEndingInvoiceReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
   : BaseReportSection(mmConsolidatedZReport, mmShowBegningandEndingBalance, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}



ZBegningEndingInvoiceReportSection::~ZBegningEndingInvoiceReportSection()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void ZBegningEndingInvoiceReportSection::GetOutput(TPrintout* printOut)
{
    try
    {
        AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
        const Currency todaysEarnings = 0;
        const Currency openingBalance = 0;
        const Currency closingBalance = 0;
        AnsiString startInvoiceNumber = "0";
        AnsiString endInvoiceNumber = "0";

        todaysEarnings = GetTotalEarningsForZed(todaysEarnings, deviceName);

        if(IsConsolidatedZed)
        {
            openingBalance = dataCalculationUtilities->GetAccumulatedZedTotal(*_dbTransaction, *_startTime, *_endTime, deviceName);
            startInvoiceNumber = GetStartInvoiceNumberForConsolidatedZed(deviceName);   // Todo FormatReceiptNo
            endInvoiceNumber = GetEndInvoiceNumberForConsolidatedZed(deviceName);       // Todo FormatReceiptNo
        }
        else
        {
            openingBalance = dataCalculationUtilities->GetAccumulatedZedTotal(*_dbTransaction);
            startInvoiceNumber = GetStartInvoiceNumber();   // Todo FormatReceiptNo
            endInvoiceNumber = GetEndInvoiceNumber();       // Todo FormatReceiptNo
        }

        closingBalance = openingBalance + todaysEarnings;
        TDateTime trans_date = dataCalculationUtilities->CalculateSessionTransactionDate(Now());
        FormatInvoiceNumber(startInvoiceNumber,endInvoiceNumber);

        IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

        if(reportSectionDisplayTraits)
        {
            reportSectionDisplayTraits->ApplyTraits(printOut);
        }

        if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {

            printOut->PrintFormat->Line->ColCount = 4;


            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 1/5;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1/2.5;
            printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width  * 1/2.5;
            printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
            printOut->PrintFormat->Line->FontInfo.Reset();

            if(!IsConsolidatedZed)
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Text = "Report Date:";
                printOut->PrintFormat->Line->Columns[2]->Text = Now().FormatString("dd") + "/" + Now().FormatString("mm") + "/" + Now().FormatString("yyyy");
                printOut->PrintFormat->AddLine();
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Text = "Report Time:";
                printOut->PrintFormat->Line->Columns[2]->Text = Now().FormatString("hh:nn AM/PM");
                printOut->PrintFormat->AddLine();
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Text = "Tran Date:";
                printOut->PrintFormat->Line->Columns[2]->Text = trans_date.FormatString("dd") + "/" + trans_date.FormatString("mm") + "/" + trans_date.FormatString("yyyy");
                printOut->PrintFormat->AddLine();
            }
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "Beg. S.I. # ";
            printOut->PrintFormat->Line->Columns[2]->Text = UnicodeString(startInvoiceNumber);
            printOut->PrintFormat->AddLine();
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "End. S.I. # ";
            printOut->PrintFormat->Line->Columns[2]->Text = UnicodeString(endInvoiceNumber);
            printOut->PrintFormat->AddLine();
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "";
            printOut->PrintFormat->Line->Columns[2]->Text = "";
            printOut->PrintFormat->AddLine();
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "Beginning Balance";
            printOut->PrintFormat->Line->Columns[2]->Text = CurrToStrF(openingBalance, ffNumber, CurrencyDecimals);
            printOut->PrintFormat->AddLine();
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "Ending Balance";
            printOut->PrintFormat->Line->Columns[2]->Text = CurrToStrF(closingBalance, ffNumber, CurrencyDecimals);
            printOut->PrintFormat->AddLine();
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
void ZBegningEndingInvoiceReportSection::FormatInvoiceNumber(AnsiString &inStartInvoiceNumber,AnsiString &inEndInvoiceNumber)
{
    try
    {
        AnsiString prefix1 = TReceiptUtility::ExtractInvoiceNumber(inStartInvoiceNumber);
        if(StrToInt(inStartInvoiceNumber) > 0 )//&& StrToInt(TGlobalSettings::Instance().ReceiptDigits) > 0)
        {
            int noOfDigits = StrToInt(TGlobalSettings::Instance().ReceiptDigits);
            inStartInvoiceNumber = TReceiptUtility::LeftPadString(inStartInvoiceNumber, "0", noOfDigits);

        }
        inStartInvoiceNumber = prefix1 + inStartInvoiceNumber;

        AnsiString prefix2 = TReceiptUtility::ExtractInvoiceNumber(inEndInvoiceNumber);
        if(StrToInt(inEndInvoiceNumber) > 0 )//&& StrToInt(TGlobalSettings::Instance().ReceiptDigits) > 0)
        {
            int noOfDigits = StrToInt(TGlobalSettings::Instance().ReceiptDigits);
            inEndInvoiceNumber = TReceiptUtility::LeftPadString(inEndInvoiceNumber, "0", noOfDigits);

        }
        inEndInvoiceNumber = prefix2 + inEndInvoiceNumber;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

AnsiString ZBegningEndingInvoiceReportSection::GetStartInvoiceNumber()
{
	AnsiString beginInvoiceNum = 0;
    try
    {
        TIBSQL *qrStartInvoiceNumber = _dbTransaction->Query(_dbTransaction->AddQuery());
        qrStartInvoiceNumber->SQL->Text = "SELECT "
                                                "DAB.INVOICE_NUMBER "
                                            "FROM DAYARCBILL DAB "
                                            "LEFT JOIN DAYARCHIVE DA on DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
                                            "LEFT JOIN DAYARCORDERDISCOUNTS DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
                                            "WHERE(COALESCE(DAOD.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable') "
                                            "GROUP BY DAB.ARCBILL_KEY, DAB.INVOICE_NUMBER "
                                            "ORDER BY DAB.ARCBILL_KEY ";

        qrStartInvoiceNumber->ExecQuery();

        if(!qrStartInvoiceNumber->Eof)
        {
            beginInvoiceNum = qrStartInvoiceNumber->Fields[0]->AsString;
        }
        else
        {
            beginInvoiceNum = GetLastEndInvoiceNumber();
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

	return beginInvoiceNum;
}

AnsiString ZBegningEndingInvoiceReportSection::GetEndInvoiceNumber()
{
	AnsiString endInvoiceNum = 0;
    try
    {
        TIBSQL *qrEndInvoiceNumber = _dbTransaction->Query(_dbTransaction->AddQuery());
        qrEndInvoiceNumber->SQL->Text = "SELECT "
                                            "DAB.INVOICE_NUMBER "
                                        "FROM DAYARCBILL DAB "
                                        "LEFT JOIN DAYARCHIVE DA on DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
                                        "LEFT JOIN DAYARCORDERDISCOUNTS DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
                                        "WHERE(COALESCE(DAOD.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable') "
                                        "GROUP BY DAB.ARCBILL_KEY, DAB.INVOICE_NUMBER "
                                        "ORDER BY DAB.ARCBILL_KEY ";

        qrEndInvoiceNumber->ExecQuery();

        if(!qrEndInvoiceNumber->Eof)
        {
            for(; !qrEndInvoiceNumber->Eof; qrEndInvoiceNumber->Next())
            {
                endInvoiceNum = qrEndInvoiceNumber->Fields[0]->AsString;
            }
        }
        else
        {
            endInvoiceNum = GetLastEndInvoiceNumber();
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
	return endInvoiceNum;
}

AnsiString ZBegningEndingInvoiceReportSection::GetLastEndInvoiceNumber()
{
	AnsiString lastEndInvoiceNum = 0;
    try
    {
        TIBSQL *qrEndInvoiceNumber = _dbTransaction->Query(_dbTransaction->AddQuery());
        qrEndInvoiceNumber->SQL->Text = "SELECT "
                                            "first 1 AB.INVOICE_NUMBER "
                                        "FROM ARCBILL AB "
                                        "LEFT JOIN ARCHIVE A on AB.ARCBILL_KEY = A.ARCBILL_KEY "
                                        "LEFT JOIN ARCORDERDISCOUNTS AOD on A.ARCHIVE_KEY = AOD.ARCHIVE_KEY  "
                                        "WHERE(COALESCE(AOD.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable') "
                                        "GROUP BY AB.ARCBILL_KEY, AB.INVOICE_NUMBER "
                                        "ORDER BY AB.ARCBILL_KEY desc ";

        qrEndInvoiceNumber->ExecQuery();

        if(!qrEndInvoiceNumber->Eof)
        {
            for(; !qrEndInvoiceNumber->Eof; qrEndInvoiceNumber->Next())
            {
                lastEndInvoiceNum = qrEndInvoiceNumber->Fields[0]->AsString;
            }
        }
        else
        {
            lastEndInvoiceNum = "0";
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
	return lastEndInvoiceNum;
}
AnsiString ZBegningEndingInvoiceReportSection::GetStartInvoiceNumberForConsolidatedZed(AnsiString deviceName)
{
	AnsiString beginInvoiceNum = 0;
    try
    {
        AnsiString terminalNamePredicate = "";
        if(!_globalSettings->EnableDepositBagNum)
        {
            terminalNamePredicate = " DAB.TERMINAL_NAME = '" + deviceName + "' AND ";
        }

        TIBSQL *qrStartInvoiceNumber = _dbTransaction->Query(_dbTransaction->AddQuery());
        qrStartInvoiceNumber->SQL->Text =  "SELECT "
                                            "DAB.INVOICE_NUMBER "
                                            "FROM ARCBILL DAB "
                                            "LEFT JOIN ARCHIVE DA on DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
                                            "LEFT JOIN ARCORDERDISCOUNTS DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
                                            "WHERE "
                                            + terminalNamePredicate +
                                            " (COALESCE(DAOD.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable') "
                                            "and DAB.TIME_STAMP >= :startTime and DAB.TIME_STAMP <= :endTime "
                                            "GROUP BY DAB.TIME_STAMP,DAB.ARCBILL_KEY, DAB.INVOICE_NUMBER "
                                            "ORDER BY DAB.TIME_STAMP " ;

        qrStartInvoiceNumber->ParamByName("startTime")->AsDateTime = *_startTime;
        qrStartInvoiceNumber->ParamByName("endTime")->AsDateTime = *_endTime;
        qrStartInvoiceNumber->ExecQuery();

        if(!qrStartInvoiceNumber->Eof)
        {
            beginInvoiceNum = qrStartInvoiceNumber->Fields[0]->AsString;
        }
        else
        {
            beginInvoiceNum = "0";
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

	return beginInvoiceNum;
}

AnsiString ZBegningEndingInvoiceReportSection::GetEndInvoiceNumberForConsolidatedZed(AnsiString deviceName)
{
	AnsiString endInvoiceNum = 0;
    try
    {
        AnsiString terminalNamePredicate = "";
        if(!_globalSettings->EnableDepositBagNum)
        {
            terminalNamePredicate = " DAB.TERMINAL_NAME = '" + deviceName + "' AND ";
        }

        TIBSQL *qrEndInvoiceNumber = _dbTransaction->Query(_dbTransaction->AddQuery());
        qrEndInvoiceNumber->SQL->Text = "SELECT "
                                            "DAB.INVOICE_NUMBER "
                                        "FROM ARCBILL DAB "
                                        "LEFT JOIN ARCHIVE DA on DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
                                        "LEFT JOIN ARCORDERDISCOUNTS DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
                                        "WHERE "
                                        + terminalNamePredicate +
                                        "(COALESCE(DAOD.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable') "
                                        "and DAB.TIME_STAMP >= :startTime and DAB.TIME_STAMP <= :endTime "
                                        "GROUP BY DAB.TIME_STAMP,DAB.ARCBILL_KEY, DAB.INVOICE_NUMBER "
                                        "ORDER BY DAB.TIME_STAMP ";

        qrEndInvoiceNumber->ParamByName("startTime")->AsDateTime = *_startTime;
        qrEndInvoiceNumber->ParamByName("endTime")->AsDateTime = *_endTime;
        qrEndInvoiceNumber->ExecQuery();

        if(!qrEndInvoiceNumber->Eof)
        {
            for(; !qrEndInvoiceNumber->Eof; qrEndInvoiceNumber->Next())
            {
                endInvoiceNum = qrEndInvoiceNumber->Fields[0]->AsString;
            }
        }
        else
        {
            endInvoiceNum = "0";
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
	return endInvoiceNum;
}

Currency ZBegningEndingInvoiceReportSection::GetTotalEarningsForZed(Currency todaysEarnings, AnsiString deviceName)
{
    if(!IsConsolidatedZed)
    {
        if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {
           todaysEarnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName, true);
        }
        else
        {
           todaysEarnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName);
        }
    }
    else
    {
       todaysEarnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName, *_startTime, *_endTime, true);
    }
    return todaysEarnings;
}

