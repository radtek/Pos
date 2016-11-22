#include "XTaxSummaryDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ReportUtilities.h"


XTaxSummaryDetailsReportSection::XTaxSummaryDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmTaxSummaryDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
    reportCalculations = new ReportFinancialCalculations;
}


XTaxSummaryDetailsReportSection::~XTaxSummaryDetailsReportSection()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
    delete reportCalculations;
}

void XTaxSummaryDetailsReportSection::GetOutput(TPrintout* printOut)
{
    if(!printOut->BlindBalanceUsed)
        return;
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    const Currency todays_earnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName);

    Currency taxExemptSales = 0;
    Currency salesTax = reportCalculations->GetTotalSalesTax(*_dbTransaction, deviceName);
    Currency serviceCharge = reportCalculations->GetServiceCharge(*_dbTransaction, deviceName);
    Currency serviceChargeTax = reportCalculations->GetServiceChargeTax(*_dbTransaction, deviceName);
    Currency localTax = reportCalculations->GetLocalTax(*_dbTransaction, deviceName);
    Currency profitTax = reportCalculations->GetProfitTax(*_dbTransaction, deviceName);
    Currency discountAndSurcharge = reportCalculations->GetDiscountsAndSurcharges(*_dbTransaction);
    Currency zeroratedsales = reportCalculations->GetZeroRatedSales(*_dbTransaction, deviceName);

    taxExemptSales = RoundToNearest(reportCalculations->GetTaxExemptSales(*_dbTransaction), 0.01, _globalSettings->MidPointRoundsDown);

    if(_globalSettings->ReCalculateTaxPostDiscount)
    {
        discountAndSurcharge = 0;
    }

    const Currency taxSales = (((todays_earnings - discountAndSurcharge) - taxExemptSales - serviceCharge - serviceChargeTax) - (salesTax + localTax + profitTax)) - zeroratedsales;
    //CAST((ZEDS.TERMINAL_EARNINGS - (ROUNDING.rounding_amount) - sum(coalesce(VAT_EXEMPT_SALE.price,0)) - SUM(coalesce(zero_rated.price,0))- sum(coalesce(AOT.VAT,0))) as NUMERIC(17,4)) VATABLE

    if(_globalSettings->ShowServiceChargeTaxWithSalesTax && !_globalSettings->ShowServiceChargeTaxWithServiceCharge)
    {
        salesTax += serviceChargeTax;
    }

	AddTitle(printOut, "Tax Summary");
	printOut->PrintFormat->NewLine();

    IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

    if(reportSectionDisplayTraits)
    {
        reportSectionDisplayTraits->ApplyTraits(printOut);
    }
    //Currency zeroratedsales = reportCalculations->GetZeroRatedSales(*_dbTransaction, deviceName);

    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 3;
    printOut->PrintFormat->Line->FontInfo.Reset();

    printOut->PrintFormat->Line->Columns[0]->Text = "VATable Sales:";
    printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(taxSales);
    printOut->PrintFormat->AddLine();

    printOut->PrintFormat->Line->Columns[0]->Text = "VAT Amount:";
    printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(salesTax);
    printOut->PrintFormat->AddLine();

    printOut->PrintFormat->Line->Columns[0]->Text = "VAT Exempt Sales:";
    printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(taxExemptSales);
    printOut->PrintFormat->AddLine();

    printOut->PrintFormat->Line->Columns[0]->Text = "Zero-Rated Sales:";
    printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(zeroratedsales);
    printOut->PrintFormat->AddLine();

    if(TGlobalSettings::Instance().ShowLocalandProfitTax)
    {
        printOut->PrintFormat->Line->Columns[0]->Text = "Local Tax Total:";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(localTax);
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "Profit Tax Total:";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(profitTax);
        printOut->PrintFormat->AddLine();
    }
}
