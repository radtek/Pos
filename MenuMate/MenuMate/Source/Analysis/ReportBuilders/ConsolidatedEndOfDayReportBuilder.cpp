#include "ConsolidatedEndOfDayReportBuilder.h"


ConsolidatedEndOfDayReportBuilder::ConsolidatedEndOfDayReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction, TDateTime* startTime, TDateTime* endTime)
	: BaseReportBuilder(reportType, globalSettings, dbTransaction, startTime, endTime)
{ }

ConsolidatedEndOfDayReportBuilder::~ConsolidatedEndOfDayReportBuilder()
{ }

void ConsolidatedEndOfDayReportBuilder::AddSectionsToReport(IReport* report)
{
    XAndZReportFormatForBIR(report);
}

void ConsolidatedEndOfDayReportBuilder::AddReportDetailsSection(IReport* report)
{
    AddReportSectionToReport(report, mmReportDetailsSection, true);
}

void ConsolidatedEndOfDayReportBuilder::AddCurrentDateDetailsSection(IReport* report)
{
    AddReportSectionToReport(report, mmCurrentDateDetailsSection, true);
}
void ConsolidatedEndOfDayReportBuilder::AddCurrentBegningAndEndingBalance(IReport* report)
{
    AddReportSectionToReport(report, mmShowBegningandEndingBalance, true);
}

void ConsolidatedEndOfDayReportBuilder::AddClientDetailsSection(IReport* report)
{
	AddReportSectionToReport(report, mmClientDetailsSection, GetGlobalSettings()->EnableClientDetails);
}

void ConsolidatedEndOfDayReportBuilder::AddSessionDateSection(IReport* report)
{
    AddReportSectionToReport(report, mmSessionDateDetailsSection, GetGlobalSettings()->ShowSessionDateInZed);
}

void ConsolidatedEndOfDayReportBuilder::AddMasterBlindBalancesSection(IReport* report)
{
    AddReportSectionToReport(report, mmMasterBlindBalancesDetailsSection, (GetGlobalSettings()->EnableBlindBalances && GetGlobalSettings()->EnableDepositBagNum));
}

void ConsolidatedEndOfDayReportBuilder::AddBlindBalancesSection(IReport* report)
{
    AddReportSectionToReport(report, mmBlindBalancesDetailsSection, GetGlobalSettings()->EnableBlindBalances);
}

void ConsolidatedEndOfDayReportBuilder::AddTransactionSummaryGroupSection(IReport* report)
{
    AddReportSectionToReport(report, mmTransactionSummaryGroupDetailsSection, GetGlobalSettings()->ShowTransactionSummaryGroups);
}

void ConsolidatedEndOfDayReportBuilder::AddBilledSalesTotalsSection(IReport* report)
{
    AddReportSectionToReport(report, mmBilledSalesTotalsDetailsSection, GetGlobalSettings()->ShowBilledSalesTotals);
}

void ConsolidatedEndOfDayReportBuilder::AddComplimentarySalesTotalsSection(IReport* report)
{
    AddReportSectionToReport(report, mmComplimentarySalesTotalsDetailsSection, GetGlobalSettings()->ShowComplimentarySalesTotals);
}

void ConsolidatedEndOfDayReportBuilder::AddChargeSalesTotalsSection(IReport* report)
{
    AddReportSectionToReport(report, mmChargeSalesTotalsDetailsSection, GetGlobalSettings()->ShowChargedSalesTotals);
}

void ConsolidatedEndOfDayReportBuilder::AddTotalsSection(IReport* report)
{
    AddReportSectionToReport(report, mmTotalsDetailsSection, GetGlobalSettings()->ShowTotals);
}

void ConsolidatedEndOfDayReportBuilder::AddBreakdownCategoriesSection(IReport* report)
{
    AddReportSectionToReport(report, mmBreakdownCategoriesDetailsSection, GetGlobalSettings()->ShowBreakdownCategories);
}

void ConsolidatedEndOfDayReportBuilder::AddDiscountReportSection(IReport* report)
{
    AddReportSectionToReport(report, mmDiscountReportDetailsSection, GetGlobalSettings()->ShowDiscountReport);
}

void ConsolidatedEndOfDayReportBuilder::AddPointsReportSection(IReport* report)
{
    AddReportSectionToReport(report, mmPointsReportDetailsSection, GetGlobalSettings()->ShowPointsReport);
}

void ConsolidatedEndOfDayReportBuilder::AddPatronAverageSection(IReport* report)
{
    AddReportSectionToReport(report, mmPatronAverageDetailsSection, GetGlobalSettings()->ShowPatronAverages);
}

void ConsolidatedEndOfDayReportBuilder::AddProductionInfoSection(IReport* report)
{
    AddReportSectionToReport(report, mmProductionInfoDetailsSection, GetGlobalSettings()->ShowProductionInfo);
}

void ConsolidatedEndOfDayReportBuilder::AddAccountPurchasesSection(IReport* report)
{
    AddReportSectionToReport(report, mmAccountPurchasesDetailsSection, GetGlobalSettings()->ShowAccountPurchases);
}

void ConsolidatedEndOfDayReportBuilder::AddAccountBalancesTabsSection(IReport* report)
{
    AddReportSectionToReport(report, mmAccountBalancesTabsDetailsSection, GetGlobalSettings()->ShowAccountBalancesTabs);
}

void ConsolidatedEndOfDayReportBuilder::AddAccountBalancesSeatedSection(IReport* report)
{
    AddReportSectionToReport(report, mmAccountBalancesSeatedDetailsSection, GetGlobalSettings()->ShowAccountBalances);
}

void ConsolidatedEndOfDayReportBuilder::AddHourlySalesSection(IReport* report)
{
    AddReportSectionToReport(report, mmHourlySalesDetailsSection, GetGlobalSettings()->ShowHourlySales);
}

void ConsolidatedEndOfDayReportBuilder::AddAccumulatedTotalSection(IReport* report)
{
    AddReportSectionToReport(report, mmAccumulatedTotalDetailsSection, GetGlobalSettings()->ShowAccumulatedZeds);
}

void ConsolidatedEndOfDayReportBuilder::AddTaxSummarySection(IReport* report)
{
    AddReportSectionToReport(report, mmTaxSummaryDetailsSection, GetGlobalSettings()->ShowTaxSummary);
}

void ConsolidatedEndOfDayReportBuilder::AddServiceChargeSummarSection(IReport* report)
{
    AddReportSectionToReport(report, mmServiceChargeSummaryDetailsSection, GetGlobalSettings()->ShowServiceChargeSummary);
}

void ConsolidatedEndOfDayReportBuilder::AddStaffHoursSection(IReport* report)
{
    AddReportSectionToReport(report, mmStaffHoursDetailsSection, GetGlobalSettings()->EnableStaffHours);
}

void ConsolidatedEndOfDayReportBuilder::AddCommissionTipsSection(IReport* report)
{
    AddReportSectionToReport(report, mmCommissionTipsDetailsSection, GetGlobalSettings()->EnableCommission);
}

void ConsolidatedEndOfDayReportBuilder::AddRefundSection(IReport* report)
{
    AddReportSectionToReport(report, mmRefundDetailsSection, !GetGlobalSettings()->EnableHideCredsCancels);
}

void ConsolidatedEndOfDayReportBuilder::AddCancelsSection(IReport* report)
{
    AddReportSectionToReport(report, mmCancelsDetailsSection, !GetGlobalSettings()->EnableHideCredsCancels);
}

void ConsolidatedEndOfDayReportBuilder::AddWriteOffSection(IReport* report)
{
    AddReportSectionToReport(report, mmWriteOffDetailsSection, !GetGlobalSettings()->EnableHideCredsCancels);
}
//
void ConsolidatedEndOfDayReportBuilder::AddShowRemovalSection(IReport* report)
{
    AddReportSectionToReport(report, mmShowRemovalSection, GetGlobalSettings()->ShowRemovalReport);
}

void ConsolidatedEndOfDayReportBuilder::AddPriceAdjustmentSection(IReport* report)
{
    AddReportSectionToReport(report, mmShowPriceAdjustmentSection, GetGlobalSettings()->ShowPointsReport);
}

void ConsolidatedEndOfDayReportBuilder::AddMallExportConsolidatedReceipt(IReport* report)
{
    AddReportSectionToReport(report, mmMallExportConsolidatedReceipt, true);
}

void ConsolidatedEndOfDayReportBuilder::AddVoidAndCancelReport(IReport* report)
{
    AddReportSectionToReport(report, mmRefundCancelDetailsSections, !GetGlobalSettings()->EnableHideCredsCancels);
}

void ConsolidatedEndOfDayReportBuilder::AddCashDenominationSection(IReport* report)
{
    AddReportSectionToReport(report, mmCashDenominationDetailsSection, GetGlobalSettings()->CashDenominationEntry);
}

void ConsolidatedEndOfDayReportBuilder::AddMasterCashDenominationSection(IReport* report)
{
    AddReportSectionToReport(report, mmMasterCashDenominationDetailsSection, GetGlobalSettings()->CashDenominationEntry && GetGlobalSettings()->EnableDepositBagNum);
}

void ConsolidatedEndOfDayReportBuilder::XAndZReportFormatForBIR(IReport* report)
{
   if(GetGlobalSettings()->UseBIRFormatInXZReport)
   {
        AddClientDetailsSection(report);
        AddReportDetailsSection(report);
        AddCurrentBegningAndEndingBalance(report);
        AddTaxSummarySection(report);
        AddDiscountReportSection(report);
        AddTransactionSummaryGroupSection(report);
        AddVoidAndCancelReport(report);
        AddAccumulatedTotalSection(report);
/////////////////// Normal Zed Files.../////////////////
        AddMasterBlindBalancesSection(report);
        AddBlindBalancesSection(report);
        AddBilledSalesTotalsSection(report);
        AddComplimentarySalesTotalsSection(report);
        AddChargeSalesTotalsSection(report);
        AddTotalsSection(report);
        AddBreakdownCategoriesSection(report);
        AddShowRemovalSection(report);
        AddWriteOffSection(report);
        AddPriceAdjustmentSection(report);

        AddPointsReportSection(report);
        AddPatronAverageSection(report);
        AddProductionInfoSection(report);
        AddAccountPurchasesSection(report);
        AddAccountBalancesTabsSection(report);
        AddAccountBalancesSeatedSection(report);
        AddHourlySalesSection(report);
        AddServiceChargeSummarSection(report);
        AddStaffHoursSection(report);
        AddCommissionTipsSection(report);
        AddMallExportConsolidatedReceipt(report);
   }
   else
   {
        AddReportDetailsSection(report);
        AddCurrentDateDetailsSection(report);
        AddClientDetailsSection(report);
        AddSessionDateSection(report);
        AddCurrentBegningAndEndingBalance(report);
        AddMasterCashDenominationSection(report);
        AddMasterBlindBalancesSection(report);
        AddCashDenominationSection(report); // shows cash denomination above blind balance
	    AddBlindBalancesSection(report);
        AddTransactionSummaryGroupSection(report);
        AddBilledSalesTotalsSection(report);
        AddComplimentarySalesTotalsSection(report);
        AddChargeSalesTotalsSection(report);
        AddTotalsSection(report);
        AddBreakdownCategoriesSection(report);
        AddShowRemovalSection(report);
        AddRefundSection(report);
        AddCancelsSection(report);
        AddWriteOffSection(report);
        AddDiscountReportSection(report);
        AddPriceAdjustmentSection(report);
        AddPointsReportSection(report);
        AddPatronAverageSection(report);
        AddProductionInfoSection(report);
        AddAccountPurchasesSection(report);
        AddAccountBalancesTabsSection(report);
        AddAccountBalancesSeatedSection(report);
        AddHourlySalesSection(report);
        AddTaxSummarySection(report); // put tax summary above site accumulated zed
        AddAccumulatedTotalSection(report);
        AddServiceChargeSummarSection(report);
        AddStaffHoursSection(report);
        AddCommissionTipsSection(report);
        AddMallExportConsolidatedReceipt(report);
   }
}

