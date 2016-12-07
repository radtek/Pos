#include "EndOfDayReportBuilder.h"


EndOfDayReportBuilder::EndOfDayReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
	: BaseReportBuilder(reportType, globalSettings, dbTransaction)
{ }

EndOfDayReportBuilder::~EndOfDayReportBuilder()
{ }

void EndOfDayReportBuilder::AddSectionsToReport(IReport* report)
{
    



    AddReportDetailsSection(report);
    AddCurrentDateDetailsSection(report);
	AddClientDetailsSection(report);
	AddSessionDateSection(report);
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
	AddAccumulatedTotalSection(report);
	AddTaxSummarySection(report);
	AddServiceChargeSummarSection(report);
	AddStaffHoursSection(report);
	AddCommissionTipsSection(report);
	AddMallExportConsolidatedReceipt(report);



}

void EndOfDayReportBuilder::AddReportDetailsSection(IReport* report)
{
    AddReportSectionToReport(report, mmReportDetailsSection, true);
}

void EndOfDayReportBuilder::AddCurrentDateDetailsSection(IReport* report)
{
    AddReportSectionToReport(report, mmCurrentDateDetailsSection, true);
}

void EndOfDayReportBuilder::AddClientDetailsSection(IReport* report)
{
	AddReportSectionToReport(report, mmClientDetailsSection, GetGlobalSettings()->EnableClientDetails);
}

void EndOfDayReportBuilder::AddSessionDateSection(IReport* report)
{
    AddReportSectionToReport(report, mmSessionDateDetailsSection, GetGlobalSettings()->ShowSessionDateInZed);
}

void EndOfDayReportBuilder::AddMasterBlindBalancesSection(IReport* report)
{
    AddReportSectionToReport(report, mmMasterBlindBalancesDetailsSection, (GetGlobalSettings()->EnableBlindBalances && GetGlobalSettings()->EnableDepositBagNum));
}

void EndOfDayReportBuilder::AddBlindBalancesSection(IReport* report)
{
    AddReportSectionToReport(report, mmBlindBalancesDetailsSection, GetGlobalSettings()->EnableBlindBalances);
}

void EndOfDayReportBuilder::AddTransactionSummaryGroupSection(IReport* report)
{
    AddReportSectionToReport(report, mmTransactionSummaryGroupDetailsSection, GetGlobalSettings()->ShowTransactionSummaryGroups);
}

void EndOfDayReportBuilder::AddBilledSalesTotalsSection(IReport* report)
{
    AddReportSectionToReport(report, mmBilledSalesTotalsDetailsSection, GetGlobalSettings()->ShowBilledSalesTotals);
}

void EndOfDayReportBuilder::AddComplimentarySalesTotalsSection(IReport* report)
{
    AddReportSectionToReport(report, mmComplimentarySalesTotalsDetailsSection, GetGlobalSettings()->ShowComplimentarySalesTotals);
}

void EndOfDayReportBuilder::AddChargeSalesTotalsSection(IReport* report)
{
    AddReportSectionToReport(report, mmChargeSalesTotalsDetailsSection, GetGlobalSettings()->ShowChargedSalesTotals);
}

void EndOfDayReportBuilder::AddTotalsSection(IReport* report)
{
    AddReportSectionToReport(report, mmTotalsDetailsSection, GetGlobalSettings()->ShowTotals);
}

void EndOfDayReportBuilder::AddBreakdownCategoriesSection(IReport* report)
{
    AddReportSectionToReport(report, mmBreakdownCategoriesDetailsSection, GetGlobalSettings()->ShowBreakdownCategories);
}

void EndOfDayReportBuilder::AddDiscountReportSection(IReport* report)
{
    AddReportSectionToReport(report, mmDiscountReportDetailsSection, GetGlobalSettings()->ShowDiscountReport);
}

void EndOfDayReportBuilder::AddPointsReportSection(IReport* report)
{
    AddReportSectionToReport(report, mmPointsReportDetailsSection, GetGlobalSettings()->ShowPointsReport);
}

void EndOfDayReportBuilder::AddPatronAverageSection(IReport* report)
{
    AddReportSectionToReport(report, mmPatronAverageDetailsSection, GetGlobalSettings()->ShowPatronAverages);
}

void EndOfDayReportBuilder::AddProductionInfoSection(IReport* report)
{
    AddReportSectionToReport(report, mmProductionInfoDetailsSection, GetGlobalSettings()->ShowProductionInfo);
}

void EndOfDayReportBuilder::AddAccountPurchasesSection(IReport* report)
{
    AddReportSectionToReport(report, mmAccountPurchasesDetailsSection, GetGlobalSettings()->ShowAccountPurchases);
}

void EndOfDayReportBuilder::AddAccountBalancesTabsSection(IReport* report)
{
    AddReportSectionToReport(report, mmAccountBalancesTabsDetailsSection, GetGlobalSettings()->ShowAccountBalancesTabs);
}

void EndOfDayReportBuilder::AddAccountBalancesSeatedSection(IReport* report)
{
    AddReportSectionToReport(report, mmAccountBalancesSeatedDetailsSection, GetGlobalSettings()->ShowAccountBalances);
}

void EndOfDayReportBuilder::AddHourlySalesSection(IReport* report)
{
    AddReportSectionToReport(report, mmHourlySalesDetailsSection, GetGlobalSettings()->ShowHourlySales);
}

void EndOfDayReportBuilder::AddAccumulatedTotalSection(IReport* report)
{
    AddReportSectionToReport(report, mmAccumulatedTotalDetailsSection, GetGlobalSettings()->ShowAccumulatedZeds);
}

void EndOfDayReportBuilder::AddTaxSummarySection(IReport* report)
{
    AddReportSectionToReport(report, mmTaxSummaryDetailsSection, GetGlobalSettings()->ShowTaxSummary);
}

void EndOfDayReportBuilder::AddServiceChargeSummarSection(IReport* report)
{
    AddReportSectionToReport(report, mmServiceChargeSummaryDetailsSection, GetGlobalSettings()->ShowServiceChargeSummary);
}

void EndOfDayReportBuilder::AddStaffHoursSection(IReport* report)
{
    AddReportSectionToReport(report, mmStaffHoursDetailsSection, GetGlobalSettings()->EnableStaffHours);
}

void EndOfDayReportBuilder::AddCommissionTipsSection(IReport* report)
{
    AddReportSectionToReport(report, mmCommissionTipsDetailsSection, GetGlobalSettings()->EnableCommission);
}

void EndOfDayReportBuilder::AddRefundSection(IReport* report)
{
    AddReportSectionToReport(report, mmRefundDetailsSection, !GetGlobalSettings()->EnableHideCredsCancels);
}

void EndOfDayReportBuilder::AddCancelsSection(IReport* report)
{
    AddReportSectionToReport(report, mmCancelsDetailsSection, !GetGlobalSettings()->EnableHideCredsCancels);
}

void EndOfDayReportBuilder::AddWriteOffSection(IReport* report)
{
    AddReportSectionToReport(report, mmWriteOffDetailsSection, !GetGlobalSettings()->EnableHideCredsCancels);
}
//
void EndOfDayReportBuilder::AddShowRemovalSection(IReport* report)
{
    AddReportSectionToReport(report, mmShowRemovalSection, GetGlobalSettings()->ShowRemovalReport);
}

void EndOfDayReportBuilder::AddPriceAdjustmentSection(IReport* report)
{
    AddReportSectionToReport(report, mmShowPriceAdjustmentSection, GetGlobalSettings()->ShowPointsReport);
}

void EndOfDayReportBuilder::AddMallExportConsolidatedReceipt(IReport* report)
{
    AddReportSectionToReport(report, mmMallExportConsolidatedReceipt, true);
}

void EndOfDayReportBuilder::AddCashDenominationSection(IReport* report)
{
    AddReportSectionToReport(report, mmCashDenominationDetailsSection, GetGlobalSettings()->CashDenominationEntry);
}
void EndOfDayReportBuilder::AddMasterCashDenominationSection(IReport* report)
{
    AddReportSectionToReport(report, mmMasterCashDenominationDetailsSection, GetGlobalSettings()->CashDenominationEntry && GetGlobalSettings()->EnableDepositBagNum);
}

