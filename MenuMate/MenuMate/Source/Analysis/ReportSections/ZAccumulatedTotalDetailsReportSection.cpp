#include "ZAccumulatedTotalDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"
#include "GlobalSettings.h"
#include "ReceiptUtility.h"

ZAccumulatedTotalDetailsReportSection::ZAccumulatedTotalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	: BaseReportSection(mmXReport, mmAccumulatedTotalDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}

ZAccumulatedTotalDetailsReportSection::~ZAccumulatedTotalDetailsReportSection()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void ZAccumulatedTotalDetailsReportSection::GetOutput(TPrintout* printOut)
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
        SetPrinterFormatInMiddle(printOut);
        printOut->PrintFormat->Line->Columns[1]->Text = "Beginning OR No.";
        printOut->PrintFormat->Line->Columns[2]->Text = UnicodeString(startInvoiceNumber);
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[1]->Text = "Ending OR No.";
        printOut->PrintFormat->Line->Columns[2]->Text = UnicodeString(endInvoiceNumber);
        printOut->PrintFormat->AddLine();
        SetPrinterFormatInMiddle(printOut);
        printOut->PrintFormat->Line->Columns[1]->Text = "Accumulated";
        printOut->PrintFormat->Line->Columns[2]->Text = dataFormatUtilities->FormatMMReportCurrency(closingBalance);
        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->Line->Columns[1]->Text = "Grand Total";
        printOut->PrintFormat->Line->Columns[2]->Text = "";
        printOut->PrintFormat->AddLine();
        SetPrinterFormatInMiddle(printOut);
        int value = dataCalculationUtilities->GetZedKey(*_dbTransaction);
        value += 1;
        printOut->PrintFormat->Line->Columns[1]->Text = "Z-Counter";
        printOut->PrintFormat->Line->Columns[2]->Text = IntToStr(value);
        printOut->PrintFormat->AddLine();
        SetPrinterFormatForSingleColumn(printOut);
        printOut->PrintFormat->Line->Columns[0]->Text = "";
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

void ZAccumulatedTotalDetailsReportSection::FormatInvoiceNumber(AnsiString &inStartInvoiceNumber,AnsiString &inEndInvoiceNumber)
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
AnsiString ZAccumulatedTotalDetailsReportSection::GetStartInvoiceNumber()
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

AnsiString ZAccumulatedTotalDetailsReportSection::GetEndInvoiceNumber()
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

AnsiString ZAccumulatedTotalDetailsReportSection::GetLastEndInvoiceNumber()
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


void ZAccumulatedTotalDetailsReportSection::SetPrinterFormatInMiddle(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 4;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width  / 4 - 2;
    //printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width  / 4 + 8;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width  / 4;//printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]
            //->Width - printOut->PrintFormat->Line->Columns[1]->Width;
    printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
    printOut->PrintFormat->Line->Columns[3]->Width = 0;
}

void ZAccumulatedTotalDetailsReportSection::SetPrinterFormatForSingleColumn(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 1;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
}


