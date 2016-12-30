#include "XTotalsDetailsReportSection.h"
#include "XChargedSalesTotalsDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"

XTotalsDetailsReportSection::XTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	: BaseReportSection(mmXReport, mmTotalsDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    reportFinancialCalculations = new ReportFinancialCalculations;
}

XTotalsDetailsReportSection::XTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
    : BaseReportSection(mmConsolidatedZReport, mmTotalsDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
    reportFinancialCalculations = new ReportFinancialCalculations;
}

XTotalsDetailsReportSection::~XTotalsDetailsReportSection()
{
}

void XTotalsDetailsReportSection::GetOutput(TPrintout* printOut)
{
    TTransactionInfo TransactionInfo;
    AnsiString DeviceName = TDeviceRealTerminal::Instance().ID.Name;

    TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfo(*_dbTransaction, DeviceName);
    TFinancialDetails FinancialDetails;

    if(IsConsolidatedZed)
    {
       FinancialDetails =  reportFinancialCalculations->GetFinancialDetails(*_dbTransaction,TransactionInfo,DeviceName, *_startTime, *_endTime);
    }
    else
    {
       FinancialDetails =  reportFinancialCalculations->GetFinancialDetails(*_dbTransaction,TransactionInfo,DeviceName);
    }

    AddTitle(printOut, "Totals");

    bool showTaxAndServiceCharge = _globalSettings->RevenueFiguresAreTaxAndServiceChargeInclusive;

    printOut->PrintFormat->NewLine();
    printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
    printOut->PrintFormat->Line->ColCount = 3;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 2;
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 6;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 2 / 3);
    printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

    TCategoryGroups::iterator itCatGroups;
    for (itCatGroups = FinancialDetails.TotalSales.Details.begin(); itCatGroups != FinancialDetails.TotalSales.Details.end();
    itCatGroups++)
    {
            if (itCatGroups != FinancialDetails.TotalSales.Details.begin())
            {
                    printOut->PrintFormat->Line->Columns[0]->Text = "";
                    printOut->PrintFormat->Line->Columns[1]->Text = "";
                    printOut->PrintFormat->Line->Columns[2]->Text = "";
                    printOut->PrintFormat->AddLine();
            }

            Currency total = showTaxAndServiceCharge ? itCatGroups->second.Totals.Total : itCatGroups->second.Totals.RawTotal;

            printOut->PrintFormat->Add(itCatGroups->first + "|" + dataFormatUtilities->FormatMMReportCurrency( itCatGroups->second.Totals.Qty ) + " | " + dataFormatUtilities->FormatMMReportCurrency(total));

            TCategoryGroupDetails CategoryGroupDetails = itCatGroups->second;
            TCategoryTotals::iterator itCategoryTotals;
            for (itCategoryTotals = CategoryGroupDetails.Details.begin(); itCategoryTotals != CategoryGroupDetails.Details.end();
            itCategoryTotals++)
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

    Currency total = showTaxAndServiceCharge ? FinancialDetails.TotalSales.Totals.Total : FinancialDetails.TotalSales.Totals.RawTotal;
    printOut->PrintFormat->Add("SubTotal |" + dataFormatUtilities->FormatMMReportCurrency(total) );
    printOut->PrintFormat->NewLine();
}
