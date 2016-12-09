#ifndef ReportEnumsH
#define ReportEnumsH

enum ReportType
{
	mmZReport,
	mmXReport
};

enum ReportSectionType
{
    mmReportDetailsSection,
    mmCurrentDateDetailsSection,
    mmClientDetailsSection,
    mmSessionDateDetailsSection,
    mmTerminalDetailsSection,
    mmBlindBalancesDetailsSection,
    mmMasterBlindBalancesDetailsSection,
    mmTransactionSummaryGroupDetailsSection,
    mmBilledSalesTotalsDetailsSection,
    mmComplimentarySalesTotalsDetailsSection,
    mmChargeSalesTotalsDetailsSection,
    mmTotalsDetailsSection,
    mmBreakdownCategoriesDetailsSection,
    mmDiscountReportDetailsSection,
    mmPointsReportDetailsSection,
    mmPatronAverageDetailsSection,
    mmProductionInfoDetailsSection,
    mmAccountPurchasesDetailsSection,
    mmAccountBalancesTabsDetailsSection,
    mmAccountBalancesSeatedDetailsSection,
    mmHourlySalesDetailsSection,
    mmAccumulatedTotalDetailsSection,
    mmTaxSummaryDetailsSection,
    mmServiceChargeSummaryDetailsSection,
    mmStaffHoursDetailsSection,
    mmCommissionTipsDetailsSection,
    mmRefundDetailsSection,
    mmCancelsDetailsSection,
    mmWriteOffDetailsSection,
    mmShowRemovalSection,
    mmShowPriceAdjustmentSection,
    mmMallExportConsolidatedReceipt,
    mmShowBegningandEndingBalance,
    mmRefundCancelDetailsSections,
	mmMasterCashDenominationDetailsSection,
    mmCashDenominationDetailsSection
};

enum CalculatedTotals
{
    etcSOF,
    etcTotalRawSales,
    etcTotalGrossSales,
    ectCASH,
    ectInitialFloat,
    ectFloatAdjustments,
    ectChange,
    etcPointsSold,
    etcTotalDiscounts,
    etcPointsRedeemed,
    etcPointsMovement,
    etcPointsHeld,
    etcTotalPatronCount,
    etcTabCreditSold,
    etcTabCreditRedeemed,
    etcTabCreditMovement,
    etcVouchersSold,
    etcVouchersRedeemed,
    etcVoucherMovement,
    etcTotalSurcharges,
    etcTotalCancels,
    etcTotalCashDrawerOpens,
    etcTotalLoyaltySales,
    etcPatronCountInSavedSales,
    etcPatronCountInCashSales,
    etcTotalSpentInTabSales,
    etcPatronCountInRoomSales,
    etcPatronCountInAccountSales,
    etcPatronCountInCreditedSales,
    ectEOF

};

enum ReportSectionTraitType
{
    mmPreDisplayTrait,
    mmPostDisplayTrait,
    mmTextFormatDisplayTrait,
    mmPointsReportHeaderTrait
};

#endif
