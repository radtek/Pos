#include "XAccumulatedTotalDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"
#include "GlobalSettings.h"
#include "ReceiptUtility.h"

XAccumulatedTotalDetailsReportSection::XAccumulatedTotalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	: BaseReportSection(mmXReport, mmAccumulatedTotalDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}

XAccumulatedTotalDetailsReportSection::~XAccumulatedTotalDetailsReportSection()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void XAccumulatedTotalDetailsReportSection::GetOutput(TPrintout* printOut)
{
    try
    {
        AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
        const Currency todaysEarnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName);

        const Currency openingBalance = dataCalculationUtilities->GetAccumulatedZedTotal(*_dbTransaction);
        const Currency closingBalance = openingBalance + todaysEarnings;

        AnsiString startInvoiceNumber = GetStartInvoiceNumber();   // Todo FormatReceiptNo
        AnsiString endInvoiceNumber = GetEndInvoiceNumber();       // Todo FormatReceiptNo
        FormatInvoiceNumber(startInvoiceNumber,endInvoiceNumber);

        if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {
            AddTitle(printOut, "Site Accumulated Zed");
            printOut->PrintFormat->NewLine();
        }
        IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

        if(reportSectionDisplayTraits)
        {
            reportSectionDisplayTraits->ApplyTraits(printOut);
        }

        if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {

            dataCalculationUtilities->PrinterFormatinTwoSections(printOut);
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "Beginning OR No.";
            printOut->PrintFormat->Line->Columns[2]->Text = UnicodeString(startInvoiceNumber);
            printOut->PrintFormat->AddLine();
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "Ending OR No.";
            printOut->PrintFormat->Line->Columns[2]->Text = UnicodeString(endInvoiceNumber);
            printOut->PrintFormat->AddLine();
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "Beginning Balance";
            printOut->PrintFormat->Line->Columns[2]->Text = "";
            printOut->PrintFormat->AddLine();
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "Ending Balance";
            printOut->PrintFormat->Line->Columns[2]->Text = "";
            printOut->PrintFormat->AddLine();

            dataCalculationUtilities->PrinterFormatinTwoSections(printOut);
            printOut->PrintFormat->Line->ColCount = 5;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 1/5;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1/5;
            printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width  * 1/5;
            printOut->PrintFormat->Line->Columns[2]->Alignment = taCenter;
            printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width * 1/5;
            printOut->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
            printOut->PrintFormat->Line->Columns[4]->Width = printOut->PrintFormat->Width * 1/5 + 4;

            printOut->PrintFormat->Line->Columns[0]->Alignment = taRightJustify;
            //printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 1/3.5;
            printOut->PrintFormat->Line->Columns[0]->Text = "__";
            printOut->PrintFormat->Line->Columns[1]->Line();
            printOut->PrintFormat->Line->Columns[2]->Line();
            printOut->PrintFormat->Line->Columns[3]->Line();
            printOut->PrintFormat->Line->Columns[4]->Line();
            //printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width * 1/3.5;
            //printOut->PrintFormat->Line->Columns[3]->Text = "_________";
            printOut->PrintFormat->AddLine();

        }
        else
        {
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1/3;
            printOut->PrintFormat->Line->FontInfo.Reset();

            printOut->PrintFormat->Line->Columns[0]->Text = "Opening Balance:";
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(openingBalance);
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "Z Report:";
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(todaysEarnings);
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "Accumulated Total:";
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(closingBalance);
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "Beginning Invoice:";
            printOut->PrintFormat->Line->Columns[1]->Text = UnicodeString(startInvoiceNumber);
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "Ending Invoice:";
            printOut->PrintFormat->Line->Columns[1]->Text = UnicodeString(endInvoiceNumber);
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "Z Reading No:";
            printOut->PrintFormat->Line->Columns[1]->Text = UnicodeString(_globalSettings->ZCount);
            printOut->PrintFormat->AddLine();

        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void XAccumulatedTotalDetailsReportSection::FormatInvoiceNumber(AnsiString &inStartInvoiceNumber,AnsiString &inEndInvoiceNumber)
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
AnsiString XAccumulatedTotalDetailsReportSection::GetStartInvoiceNumber()
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

AnsiString XAccumulatedTotalDetailsReportSection::GetEndInvoiceNumber()
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

AnsiString XAccumulatedTotalDetailsReportSection::GetLastEndInvoiceNumber()
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

AnsiString XAccumulatedTotalDetailsReportSection::GetStartInvoiceNumber(TDateTime &startTime, TDateTime &endTime)
{
	AnsiString beginInvoiceNum = 0;
    try
    {
        TIBSQL *qrStartInvoiceNumber = _dbTransaction->Query(_dbTransaction->AddQuery());
        qrStartInvoiceNumber->SQL->Text = "SELECT "
                                            "DAB.INVOICE_NUMBER "
                                            "FROM ARCBILL DAB "
                                            "LEFT JOIN ARCHIVE DA on DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
                                            "LEFT JOIN ARCORDERDISCOUNTS DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
                                            "WHERE(COALESCE(DAOD.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable') "
                                            "and DAB.TIME_STAMP >= :startTime  and DAB.TIME_STAMP <=:endTime  "
                                            "GROUP BY DAB.ARCBILL_KEY, DAB.INVOICE_NUMBER "
                                            "ORDER BY DAB.ARCBILL_KEY ";

        qrStartInvoiceNumber->ParamByName("startTime")->AsDateTime = startTime;
        qrStartInvoiceNumber->ParamByName("endTime")->AsDateTime = endTime;
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

AnsiString XAccumulatedTotalDetailsReportSection::GetEndInvoiceNumber(TDateTime &startTime, TDateTime &endTime)
{
	AnsiString endInvoiceNum = 0;
    try
    {
        TIBSQL *qrEndInvoiceNumber = _dbTransaction->Query(_dbTransaction->AddQuery());
        qrEndInvoiceNumber->SQL->Text = "SELECT "
                                        "DAB.INVOICE_NUMBER "
                                        "FROM ARCBILL DAB "
                                        "LEFT JOIN ARCHIVE DA on DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
                                        "LEFT JOIN ARCORDERDISCOUNTS DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
                                        "WHERE(COALESCE(DAOD.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable') "
                                        "and DAB.TIME_STAMP >= :startTime  and DAB.TIME_STAMP <=:endTime  "
                                        "GROUP BY DAB.ARCBILL_KEY, DAB.INVOICE_NUMBER "
                                        "ORDER BY DAB.ARCBILL_KEY ";

        qrEndInvoiceNumber->ParamByName("startTime")->AsDateTime = startTime;
        qrEndInvoiceNumber->ParamByName("endTime")->AsDateTime = endTime;

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


