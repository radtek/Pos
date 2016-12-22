#include "ConsolidatedZedReportSectionFactory.h"
#include "XClientDetailsReportSection.h"
#include "XTotalsDetailsReportSection.h"
#include "ZReportDetailsReportSection.h"
#include "XCurrentDateDetailsReportSection.h"
#include "XSessionDateDetailsReportSection.h"
#include "XTerminalDetailsReportSection.h"
#include "XMasterBlindBalancesDetailsReportSection.h"
#include "ZBlindBalancesDetailsReportSection.h"
#include "XTransactionSummaryGroupDetailsReportSection.h"
#include "XTaxSummaryDetailsReportSection.h"
#include "XServiceChargeSummaryDetailsReportSection.h"
#include "XBilledSalesTotalsDetailsReportSection.h"
#include "XChargedSalesTotalsDetailsReportSection.h"
#include "XComplimentarySalesTotalsDetailsReportSection.h"
#include "XAccountBalancesTabsDetailsReportSection.h"
#include "ZAccumulatedTotalDetailsReportSection.h"
#include "ZStaffHoursDetailsReportSection.h"
#include "ZCommissionTipsDetailsReportSection.h"
#include "XAccountBalancesSeatedDetailsReportSection.h"
#include "XHourlySalesDetailsReportSection.h"
#include "XPatronAverageDetailsReportSection.h"
#include "XBreakdownCategoriesDetailsReportSection.h"
#include "XDiscountReportDetailsReportSection.h"
#include "XPointsReportDetailsReportSection.h"
#include "XProductionInfoDetailsReportSection.h"
#include "XAccountPurchasesDetailsReportSection.h"
#include "XRefundDetailsReportSection.h"
#include "XCancelsDetailsReportSection.h"
#include "XWriteOffDetailsReportSection.h"
#include "XRemovalDetailsReportSection.h"
#include "XPriceAdjustmentReportSection.h"
#include "MallExportConsolidatedReceipt.h"
#include "ZBegningEndingInvoiceReportSection.h"
#include "XCancelsAndRefundDetailsForBIRReportSection.h"
#include "ZCashDenominationReportSection.h"
#include "ZMasterCashDenominationReportSection.h"

ConsolidatedZedReportSectionFactory::ConsolidatedZedReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSectionFactory(dbTransaction, globalSettings, startTime, endTime)
{
}

ConsolidatedZedReportSectionFactory::~ConsolidatedZedReportSectionFactory()
{
}


//TODO: This file includes some sections from XReport, which needs to be refactored out to seperate Z specific classes.
//Will revisit this once we are into Analysis refactoring.
IReportSection* ConsolidatedZedReportSectionFactory::CreateReportSection(ReportSectionType reportSectionType)
{
   IReportSection* reportSection;

	switch (reportSectionType)
	{
        case mmReportDetailsSection:
            reportSection = new ZReportDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmCurrentDateDetailsSection:
            reportSection = new XCurrentDateDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmShowBegningandEndingBalance:
            reportSection = new ZBegningEndingInvoiceReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmClientDetailsSection:
            reportSection = new XClientDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmTotalsDetailsSection:
            reportSection = new XTotalsDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmSessionDateDetailsSection:
            reportSection = new XSessionDateDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmTerminalDetailsSection:
            reportSection = new XTerminalDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmMasterBlindBalancesDetailsSection:
            reportSection = new XMasterBlindBalancesDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmBlindBalancesDetailsSection:
            reportSection = new ZBlindBalancesDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmTransactionSummaryGroupDetailsSection:
            reportSection = new XTransactionSummaryGroupDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmTaxSummaryDetailsSection:
            reportSection = new XTaxSummaryDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmServiceChargeSummaryDetailsSection:
            reportSection = new XServiceChargeSummaryDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmBilledSalesTotalsDetailsSection:
            reportSection = new XBilledSalesTotalsDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmChargeSalesTotalsDetailsSection:
            reportSection = new XChargedSalesTotalsDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmComplimentarySalesTotalsDetailsSection:
            reportSection = new XComplimentarySalesTotalsDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmAccountBalancesTabsDetailsSection:
            reportSection = new XAccountBalancesTabsDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmAccumulatedTotalDetailsSection:
            reportSection = new ZAccumulatedTotalDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmStaffHoursDetailsSection:
            reportSection = new ZStaffHoursDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmCommissionTipsDetailsSection:
            reportSection = new ZCommissionTipsDetailsReportSection(_dbTransaction, _globalSettings);
            break;
        case mmAccountBalancesSeatedDetailsSection:
            reportSection = new XAccountBalancesSeatedDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmHourlySalesDetailsSection:
            reportSection = new XHourlySalesDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmPatronAverageDetailsSection:
            reportSection = new XPatronAverageDetailsReportSection(_dbTransaction, _globalSettings,_startTime, _endTime);
            break;
        case mmBreakdownCategoriesDetailsSection:
            reportSection = new XBreakdownCategoriesDetailsReportSection(_dbTransaction, _globalSettings,_startTime, _endTime);
            break;
        case mmDiscountReportDetailsSection:
            reportSection = new XDiscountReportDetailsReportSection(_dbTransaction, _globalSettings,_startTime, _endTime);
            break;
        case mmPointsReportDetailsSection:
            reportSection = new XPointsReportDetailsReportSection(_dbTransaction, _globalSettings,_startTime, _endTime);
            break;
        case mmProductionInfoDetailsSection:
            reportSection = new XProductionInfoDetailsReportSection(_dbTransaction, _globalSettings,_startTime, _endTime);
            break;
        case mmAccountPurchasesDetailsSection:
            reportSection = new XAccountPurchasesDetailsReportSection(_dbTransaction, _globalSettings,_startTime, _endTime);
            break;
        case mmRefundDetailsSection:
            reportSection = new XRefundDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmCancelsDetailsSection:
            reportSection = new XCancelsDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmWriteOffDetailsSection:
            reportSection = new XWriteOffDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
		case mmShowRemovalSection:
            reportSection = new XRemovalDetailsReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
      case mmShowPriceAdjustmentSection:
            reportSection = new XPriceAdjustmentReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmMallExportConsolidatedReceipt:
            reportSection = new MallExportConsolidatedReceipt(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmRefundCancelDetailsSections:
            reportSection = new XCancelsAndRefundDetailsForBIRReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmCashDenominationDetailsSection:
            reportSection = new ZCashDenominationReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        case mmMasterCashDenominationDetailsSection:
            reportSection = new ZMasterCashDenominationReportSection(_dbTransaction, _globalSettings, _startTime, _endTime);
            break;
        default:
            reportSection = NULL;
            break;
	}

	return reportSection;
}
