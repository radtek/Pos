#include "XReportDateTimeReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"
#include "GlobalSettings.h"
#include "ReceiptUtility.h"
#include "Analysis.h"

XReportDateTimeReportSection::XReportDateTimeReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	: BaseReportSection(mmXReport, mmShowBegningandEndingBalance, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}

XReportDateTimeReportSection::~XReportDateTimeReportSection()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void XReportDateTimeReportSection::GetOutput(TPrintout* printOut)
{
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    const Currency todaysEarnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName);

    const Currency openingBalance = dataCalculationUtilities->GetAccumulatedZedTotal(*_dbTransaction);
	const Currency closingBalance = openingBalance + todaysEarnings;
    TDateTime trans_date = dataCalculationUtilities->GetTransDateForTerminal(*_dbTransaction, deviceName);

	AnsiString startInvoiceNumber = GetStartInvoiceNumber();   // Todo FormatReceiptNo
	AnsiString endInvoiceNumber = GetEndInvoiceNumber();       // Todo FormatReceiptNo
    FormatInvoiceNumber(startInvoiceNumber,endInvoiceNumber);

    IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

    if(reportSectionDisplayTraits)
    {
        reportSectionDisplayTraits->ApplyTraits(printOut);
    }
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1/3;
	printOut->PrintFormat->Line->FontInfo.Reset();

    if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
    {
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Line->Columns[0]->Width * 1/2;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1/3;
        printOut->PrintFormat->Line->FontInfo.Reset();
        printOut->PrintFormat->Line->Columns[0]->Text = "";
        dataCalculationUtilities->PrinterFormatinTwoSections(printOut);
        printOut->PrintFormat->Line->Columns[1]->Text = "Report Date:";
        printOut->PrintFormat->Line->Columns[2]->Text = Now().FormatString("dd/mm/yyyy");
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[1]->Text = "Report Time:";
        printOut->PrintFormat->Line->Columns[2]->Text = Now().FormatString("hh:nn AM/PM");
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[1]->Text = "Terminal: ";
        printOut->PrintFormat->Line->Columns[2]->Text = deviceName;
        printOut->PrintFormat->AddLine();

        const TMMContactInfo &staff_member = TfrmAnalysis::GetLastAuthenticatedUser();
        printOut->PrintFormat->Line->Columns[1]->Text = "Cashier: ";//
        printOut->PrintFormat->Line->Columns[2]->Text = staff_member.Name;
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[1]->Text = "Tran Date:";
        printOut->PrintFormat->Line->Columns[2]->Text = trans_date.FormatString("dd/mm/yyyy");
        printOut->PrintFormat->AddLine();
    }

}

void XReportDateTimeReportSection::FormatInvoiceNumber(AnsiString &inStartInvoiceNumber,AnsiString &inEndInvoiceNumber)
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
AnsiString XReportDateTimeReportSection::GetStartInvoiceNumber()
{
	AnsiString beginInvoiceNum = 0;

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

	return beginInvoiceNum;
}

AnsiString XReportDateTimeReportSection::GetEndInvoiceNumber()
{
	AnsiString endInvoiceNum = 0;

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

	return endInvoiceNum;
}

AnsiString XReportDateTimeReportSection::GetLastEndInvoiceNumber()
{
	AnsiString lastEndInvoiceNum = 0;
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
	return lastEndInvoiceNum;
}

void XReportDateTimeReportSection::SetPrinterFormatInMiddle(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 4;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width  / 4 - 2;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width  / 4 + 8;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width  / 4;
    printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
    printOut->PrintFormat->Line->Columns[3]->Width = 0;
}
