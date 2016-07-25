//---------------------------------------------------------------------------


#pragma hdrstop

#include "XBilledSalesTotalsDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
XBilledSalesTotalsDetailsReportSection::XBilledSalesTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmBilledSalesTotalsDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    reportFinancialCalculations = new ReportFinancialCalculations;
}


XBilledSalesTotalsDetailsReportSection::~XBilledSalesTotalsDetailsReportSection()
{
    delete dataFormatUtilities;
    delete reportFinancialCalculations;
}

void XBilledSalesTotalsDetailsReportSection::GetOutput(TPrintout* printOut)
{
    TTransactionInfo TransactionInfo;
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;

    TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfo(*_dbTransaction, deviceName);

    bool showTaxAndServiceCharge = _globalSettings->RevenueFiguresAreTaxAndServiceChargeInclusive;

    TFinancialDetails financialDetails =  reportFinancialCalculations->GetFinancialDetails(*_dbTransaction, TransactionInfo, deviceName);
    AddTitle(printOut, "Billed Sales Totals");
    printOut->PrintFormat->NewLine();
    printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
    printOut->PrintFormat->Line->ColCount = 3;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 2;
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 6;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 2 / 3);
    printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

    // Retrive Category Group Totals.
    TCategoryGroups::iterator itCatGroups;
    for (itCatGroups = financialDetails.BilledSales.Details.begin(); itCatGroups != financialDetails.BilledSales.Details.end(); itCatGroups++)
    {
        if (itCatGroups != financialDetails.BilledSales.Details.begin())
        {
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "";
            printOut->PrintFormat->Line->Columns[2]->Text = "";
            printOut->PrintFormat->AddLine();
        }

        Currency total = showTaxAndServiceCharge ? itCatGroups->second.Totals.Total : itCatGroups->second.Totals.RawTotal;

        printOut->PrintFormat->Add(itCatGroups->first + "|" + dataFormatUtilities->FormatMMReportCurrency( itCatGroups->second.Totals.Qty ) + " | " + dataFormatUtilities->FormatMMReportCurrency(total));

        TCategoryGroupDetails categoryGroupDetails = itCatGroups->second;

        TCategoryTotals::iterator itCategoryTotals;
        for (itCategoryTotals = categoryGroupDetails.Details.begin(); itCategoryTotals != categoryGroupDetails.Details.end(); itCategoryTotals++)
        {
            Currency categoryTotal = showTaxAndServiceCharge ? itCategoryTotals->second.Totals.Total : itCategoryTotals->second.Totals.RawTotal;
            printOut->PrintFormat->Add("  " + itCategoryTotals->first + "|" + dataFormatUtilities->FormatMMReportCurrency( itCategoryTotals->second.Totals.Qty ) + " | " + dataFormatUtilities->FormatMMReportCurrency(categoryTotal) );
        }
    }

    // Write out subtotal.
    IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();
    if(reportSectionDisplayTraits)
    {
        reportSectionDisplayTraits->ApplyTraits(printOut);
    }

    printOut->PrintFormat->Line->Columns[0]->Text = "";
    printOut->PrintFormat->Line->Columns[1]->Line();
    printOut->PrintFormat->AddLine();

    Currency total = showTaxAndServiceCharge ? financialDetails.BilledSales.Totals.Total : financialDetails.BilledSales.Totals.RawTotal;
    printOut->PrintFormat->Add("SubTotal |" + dataFormatUtilities->FormatMMReportCurrency(total) );
}
