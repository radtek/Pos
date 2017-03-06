#include "XComplimentarySalesTotalsDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"

XComplimentarySalesTotalsDetailsReportSection::XComplimentarySalesTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmComplimentarySalesTotalsDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    reportFinancialCalculations = new ReportFinancialCalculations;
}


XComplimentarySalesTotalsDetailsReportSection::XComplimentarySalesTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmComplimentarySalesTotalsDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
    reportFinancialCalculations = new ReportFinancialCalculations;
}


XComplimentarySalesTotalsDetailsReportSection::~XComplimentarySalesTotalsDetailsReportSection()
{
    delete dataFormatUtilities;
    delete reportFinancialCalculations;
}

void XComplimentarySalesTotalsDetailsReportSection::GetOutput(TPrintout* printOut)
{
    try
    {
        TTransactionInfo TransactionInfo;
        AnsiString DeviceName = TDeviceRealTerminal::Instance().ID.Name;
        TFinancialDetails FinancialDetails;

        if(IsConsolidatedZed)
        {
            TTransactionInfoProcessor::Instance().GetTransactionInfoForConsolidatedZed(*_dbTransaction, DeviceName, *_startTime, *_endTime);
            FinancialDetails = reportFinancialCalculations->GetFinancialDetails(*_dbTransaction, TransactionInfo, DeviceName, *_startTime, *_endTime);
        }
        else
        {
            TTransactionInfoProcessor::Instance().GetTransactionInfo(*_dbTransaction, DeviceName);
            FinancialDetails = reportFinancialCalculations->GetFinancialDetails(*_dbTransaction,TransactionInfo,DeviceName);
        }

        AddTitle(printOut, "Complimentary Sales Totals");

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

            // Retrive Category Group Totals.
        TCategoryGroups::iterator itCatGroups;
        for (itCatGroups = FinancialDetails.ComplimentarySales.Details.begin(); itCatGroups != FinancialDetails.ComplimentarySales.Details.end();
        itCatGroups++)
        {
                if (itCatGroups != FinancialDetails.ComplimentarySales.Details.begin())
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
                    Currency categoryTotal = showTaxAndServiceCharge ? itCatGroups->second.Totals.Total : itCatGroups->second.Totals.RawTotal;
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

        Currency total = showTaxAndServiceCharge ? FinancialDetails.ComplimentarySales.Totals.Total : FinancialDetails.ComplimentarySales.Totals.RawTotal;
        printOut->PrintFormat->Add("SubTotal |" + dataFormatUtilities->FormatMMReportCurrency(total) );
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
