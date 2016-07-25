#include "XReportBuilder.h"
#include "XReport.h"

XReportBuilder::XReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
	: EndOfDayReportBuilder(reportType, globalSettings, dbTransaction)
{
}

XReportBuilder::~XReportBuilder()
{
}

IReport* XReportBuilder::PrepareAndCompileSections()
{
	XReport* report = new XReport();

	//As of now we are relying on the base implementation to add sections to the report.
	AddSectionsToReport(report);
	return report;
}
