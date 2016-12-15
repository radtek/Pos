#include "ReportManager.h"
#include "ZedReport.h"
#include "XReport.h"

ReportManager::ReportManager()
{

}

ReportManager::~ReportManager()
{
}

ZedReport* ReportManager::GetZedReport(TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
{
	//Get the report builder for the current report type...
	CreateReportBuilderInstance(mmZReport, globalSettings, dbTransaction);

	//Now we have the actual report builders...
	//Create the reports...
	IReport* report = _reportBuilder->BuildReport();

	return dynamic_cast<ZedReport*>(report);
}

XReport* ReportManager::GetXReport(TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
{
	//Get the report builder for the current report type...
	CreateReportBuilderInstance(mmXReport, globalSettings, dbTransaction);

	//Now we have the actual report builders...
	//Create the reports...
	IReport* report = _reportBuilder->BuildReport();

	return dynamic_cast<XReport*>(report);
}

void ReportManager::CreateReportBuilderInstance(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
{
	//Actually this should be injected here,
	//Alas!! we are not able to decouple the dependencies.
	_reportBuilderFactory.reset(new ReportBuilderFactory(globalSettings, dbTransaction));
	_reportBuilder.reset(_reportBuilderFactory->CreateReportBuilder(reportType));
}

void ReportManager::CreateReportBuilderInstanceForConsolidatedZed(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction, TDateTime* startTime,  TDateTime* endTime)
{
	//Actually this should be injected here,
	//Alas!! we are not able to decouple the dependencies.
	_reportBuilderFactory.reset(new ReportBuilderFactory(globalSettings, dbTransaction, startTime, endTime));
	_reportBuilder.reset(_reportBuilderFactory->CreateReportBuilder(reportType));
}

ConsolidatedZedReport* ReportManager::GetConsolidatedZedReport(TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction, TDateTime* StartTime,  TDateTime* EndTime)
{
	//Get the report builder for the current report type...
	CreateReportBuilderInstanceForConsolidatedZed(mmConsolidatedZReport, globalSettings, dbTransaction, StartTime, EndTime);

	//Now we have the actual report builders...
	//Create the reports...
	IReport* report = _reportBuilder->BuildReport();

	return dynamic_cast<ConsolidatedZedReport*>(report);
}


