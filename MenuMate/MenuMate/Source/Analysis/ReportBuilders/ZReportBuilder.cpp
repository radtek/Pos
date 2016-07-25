#include "ZReportBuilder.h"
#include "ZedReport.h"

ZReportBuilder::ZReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
	: EndOfDayReportBuilder(reportType, globalSettings, dbTransaction)
{}

ZReportBuilder::~ZReportBuilder()
{}

IReport* ZReportBuilder::PrepareAndCompileSections()
{
	ZedReport* report = new ZedReport();

	//As of now we are relying on the base implementation to add sections to the report.
	AddSectionsToReport(report);
	return report;
}

