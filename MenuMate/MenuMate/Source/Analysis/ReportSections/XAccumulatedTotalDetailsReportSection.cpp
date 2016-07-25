#include "XAccumulatedTotalDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"

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
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    const Currency todaysEarnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName);

    const Currency openingBalance = dataCalculationUtilities->GetAccumulatedZedTotal(*_dbTransaction);
	const Currency closingBalance = openingBalance + todaysEarnings;

	const AnsiString startInvoiceNumber = GetStartInvoiceNumber();
	const AnsiString endInvoiceNumber = GetEndInvoiceNumber();

    AddTitle(printOut, "Site Accumulated Zed");
	printOut->PrintFormat->NewLine();

    IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

    if(reportSectionDisplayTraits)
    {
        reportSectionDisplayTraits->ApplyTraits(printOut);
    }

    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 3;
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

AnsiString XAccumulatedTotalDetailsReportSection::GetStartInvoiceNumber()
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
		beginInvoiceNum = "0";
	}

	return beginInvoiceNum;
}

AnsiString XAccumulatedTotalDetailsReportSection::GetEndInvoiceNumber()
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
		endInvoiceNum = "0";
	}

	return endInvoiceNum;
}
