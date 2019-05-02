#include "WaiterAppZReportBuilder.h"
#include "WaiterAppZedReport.h"

WaiterAppZReportBuilder::WaiterAppZReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
	: WaiterAppEndOfDayReportBuilder(reportType, globalSettings, dbTransaction)
{}

WaiterAppZReportBuilder::~WaiterAppZReportBuilder()
{}

IReport* WaiterAppZReportBuilder::PrepareAndCompileSections()
{
	WaiterAppZedReport* report = new WaiterAppZedReport();

	//As of now we are relying on the base implementation to add sections to the report.
	AddSectionsToReport(report);
	return report;
}
