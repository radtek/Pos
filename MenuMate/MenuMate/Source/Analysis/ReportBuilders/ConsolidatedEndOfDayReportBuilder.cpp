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
    //AddReportSectionToReport(report, mmReportDetailsSection, true);
    AddConsolidatedReportSectionToReport(report, mmReportDetailsSection, true, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddCurrentDateDetailsSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmCurrentDateDetailsSection, true, GetStartTime(), GetEndTime());
}
void ConsolidatedEndOfDayReportBuilder::AddCurrentBegningAndEndingBalance(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmShowBegningandEndingBalance, true, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddClientDetailsSection(IReport* report)
{
	AddConsolidatedReportSectionToReport(report, mmClientDetailsSection, GetGlobalSettings()->EnableClientDetails, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddSessionDateSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmSessionDateDetailsSection, GetGlobalSettings()->ShowSessionDateInZed, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddMasterBlindBalancesSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmMasterBlindBalancesDetailsSection, (GetGlobalSettings()->EnableBlindBalances && GetGlobalSettings()->EnableDepositBagNum), GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddBlindBalancesSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmBlindBalancesDetailsSection, GetGlobalSettings()->EnableBlindBalances, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddTransactionSummaryGroupSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmTransactionSummaryGroupDetailsSection, GetGlobalSettings()->ShowTransactionSummaryGroups, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddBilledSalesTotalsSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmBilledSalesTotalsDetailsSection, GetGlobalSettings()->ShowBilledSalesTotals, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddComplimentarySalesTotalsSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmComplimentarySalesTotalsDetailsSection, GetGlobalSettings()->ShowComplimentarySalesTotals, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddChargeSalesTotalsSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmChargeSalesTotalsDetailsSection, GetGlobalSettings()->ShowChargedSalesTotals, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddTotalsSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmTotalsDetailsSection, GetGlobalSettings()->ShowTotals, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddBreakdownCategoriesSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmBreakdownCategoriesDetailsSection, GetGlobalSettings()->ShowBreakdownCategories, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddDiscountReportSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmDiscountReportDetailsSection, GetGlobalSettings()->ShowDiscountReport, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddPointsReportSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmPointsReportDetailsSection, GetGlobalSettings()->ShowPointsReport, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddPatronAverageSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmPatronAverageDetailsSection, GetGlobalSettings()->ShowPatronAverages, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddProductionInfoSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmProductionInfoDetailsSection, GetGlobalSettings()->ShowProductionInfo, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddAccountPurchasesSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmAccountPurchasesDetailsSection, GetGlobalSettings()->ShowAccountPurchases, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddAccountBalancesTabsSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmAccountBalancesTabsDetailsSection, GetGlobalSettings()->ShowAccountBalancesTabs, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddAccountBalancesSeatedSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmAccountBalancesSeatedDetailsSection, GetGlobalSettings()->ShowAccountBalances, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddHourlySalesSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmHourlySalesDetailsSection, GetGlobalSettings()->ShowHourlySales, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddAccumulatedTotalSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmAccumulatedTotalDetailsSection, GetGlobalSettings()->ShowAccumulatedZeds, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddTaxSummarySection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmTaxSummaryDetailsSection, GetGlobalSettings()->ShowTaxSummary, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddServiceChargeSummarSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmServiceChargeSummaryDetailsSection, GetGlobalSettings()->ShowServiceChargeSummary, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddStaffHoursSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmStaffHoursDetailsSection, GetGlobalSettings()->EnableStaffHours, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddCommissionTipsSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmCommissionTipsDetailsSection, GetGlobalSettings()->EnableCommission, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddRefundSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmRefundDetailsSection, !GetGlobalSettings()->EnableHideCredsCancels, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddCancelsSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmCancelsDetailsSection, !GetGlobalSettings()->EnableHideCredsCancels, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddWriteOffSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmWriteOffDetailsSection, !GetGlobalSettings()->EnableHideCredsCancels, GetStartTime(), GetEndTime());
}
//
void ConsolidatedEndOfDayReportBuilder::AddShowRemovalSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmShowRemovalSection, GetGlobalSettings()->ShowRemovalReport, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddPriceAdjustmentSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmShowPriceAdjustmentSection, GetGlobalSettings()->ShowPointsReport, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddMallExportConsolidatedReceipt(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmMallExportConsolidatedReceipt, true, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddVoidAndCancelReport(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmRefundCancelDetailsSections, !GetGlobalSettings()->EnableHideCredsCancels, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddCashDenominationSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmCashDenominationDetailsSection, GetGlobalSettings()->CashDenominationEntry, GetStartTime(), GetEndTime());
}

void ConsolidatedEndOfDayReportBuilder::AddMasterCashDenominationSection(IReport* report)
{
    AddConsolidatedReportSectionToReport(report, mmMasterCashDenominationDetailsSection, (GetGlobalSettings()->CashDenominationEntry && GetGlobalSettings()->EnableDepositBagNum), GetStartTime(), GetEndTime());
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

