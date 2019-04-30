//---------------------------------------------------------------------------


#include "WaiterAppEndOfDayReportBuilder.h"
#include "MMMessageBox.h"

WaiterAppEndOfDayReportBuilder::WaiterAppEndOfDayReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
	: BaseReportBuilder(reportType, globalSettings, dbTransaction)
{ }

WaiterAppEndOfDayReportBuilder::~WaiterAppEndOfDayReportBuilder()
{ }

void WaiterAppEndOfDayReportBuilder::AddSectionsToReport(IReport* report)
{
    AddOrderDetailsSection(report);
}

void WaiterAppEndOfDayReportBuilder::AddOrderDetailsSection(IReport* report)
{
    AddReportSectionToReport(report, mmWaiterAppOrderDetailsSection, true);
}
