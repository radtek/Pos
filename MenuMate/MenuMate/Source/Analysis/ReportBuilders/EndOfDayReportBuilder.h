#ifndef EndOfDayReportBuilderH
#define EndOfDayReportBuilderH

#include "ReportEnums.h"
#include "BaseReportBuilder.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class EndOfDayReportBuilder : public BaseReportBuilder
{
public:
	EndOfDayReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction);
	~EndOfDayReportBuilder();

protected:
	//This function is implemented to help derived concrete classes to reuse the functionality as is if they dont intend to override some of the functionality
	//If they need to they can very well do it in PrepareAndCompileSections() pure virtual function carried from the BaseReportBuilder
	void AddSectionsToReport(IReport* report);

    virtual void AddReportDetailsSection(IReport* report);
    virtual void AddCurrentDateDetailsSection(IReport* report);
	virtual void AddClientDetailsSection(IReport* report);
	virtual void AddSessionDateSection(IReport* report);
    virtual void AddMasterBlindBalancesSection(IReport* report);
	virtual void AddBlindBalancesSection(IReport* report);
	virtual void AddTransactionSummaryGroupSection(IReport* report);
	virtual void AddBilledSalesTotalsSection(IReport* report);
	virtual void AddComplimentarySalesTotalsSection(IReport* report);
	virtual void AddChargeSalesTotalsSection(IReport* report);
	virtual void AddTotalsSection(IReport* report);
	virtual void AddBreakdownCategoriesSection(IReport* report);
	virtual void AddDiscountReportSection(IReport* report);
	virtual void AddPointsReportSection(IReport* report);
	virtual void AddPatronAverageSection(IReport* report);
	virtual void AddProductionInfoSection(IReport* report);
	virtual void AddAccountPurchasesSection(IReport* report);
	virtual void AddAccountBalancesTabsSection(IReport* report);
	virtual void AddAccountBalancesSeatedSection(IReport* report);
	virtual void AddHourlySalesSection(IReport* report);
	virtual void AddAccumulatedTotalSection(IReport* report);
	virtual void AddTaxSummarySection(IReport* report);
	virtual void AddServiceChargeSummarSection(IReport* report);
	virtual void AddStaffHoursSection(IReport* report);
	virtual void AddCommissionTipsSection(IReport* report);
    virtual void AddRefundSection(IReport* report);
    virtual void AddCancelsSection(IReport* report);
    virtual void AddWriteOffSection(IReport* report);
    virtual void AddShowRemovalSection(IReport* report);
    virtual void AddPriceAdjustmentSection(IReport* report);
    virtual void AddMallExportConsolidatedReceipt(IReport* report);
    virtual void AddCurrentBegningAndEndingBalance(IReport* report);
    virtual void XAndZReportFormatForBIR(IReport* report);
    virtual void AddVoidAndCancelReport(IReport* report);
	virtual void AddMasterCashDenominationSection(IReport* report);
    virtual void AddCashDenominationSection(IReport* report);
    virtual void AddReprintReceiptSection(IReport* report);
};

#endif
