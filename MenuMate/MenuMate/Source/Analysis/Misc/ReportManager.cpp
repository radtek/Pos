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


