#include "ConsolidatedZReportBuilder.h"
#include "ConsolidatedZedReport.h"

ConsolidatedZReportBuilder::ConsolidatedZReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction, TDateTime* startTime, TDateTime* endTime)
	: ConsolidatedEndOfDayReportBuilder(reportType, globalSettings, dbTransaction, startTime, endTime)
{}

ConsolidatedZReportBuilder::~ConsolidatedZReportBuilder()
{}

IReport* ConsolidatedZReportBuilder::PrepareAndCompileSections()
{
	ConsolidatedZedReport* report = new ConsolidatedZedReport();

	//As of now we are relying on the base implementation to add sections to the report.
	AddSectionsToReport(report);
	return report;
}

