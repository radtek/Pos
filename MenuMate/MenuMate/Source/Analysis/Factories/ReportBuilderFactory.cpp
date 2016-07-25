#include "ReportBuilderFactory.h"
#include "ZReportBuilder.h"
#include "ZedReport.h"
#include "XReportBuilder.h"
#include "XReport.h"

ReportBuilderFactory::ReportBuilderFactory(TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
{
	_globalSettings = globalSettings;
	_dbTransaction = dbTransaction;
}

ReportBuilderFactory::~ReportBuilderFactory()
{ }

IReportBuilder* ReportBuilderFactory::CreateReportBuilder(ReportType reportType)
{
	IReportBuilder* reportBuilder;

	switch (reportType)
	{
	case mmZReport:
		reportBuilder = new ZReportBuilder(reportType, _globalSettings, _dbTransaction);
		break;
	case mmXReport:
		reportBuilder = new XReportBuilder(reportType, _globalSettings, _dbTransaction);
		break;
	default:
		reportBuilder = NULL;
		break;
	}

	return reportBuilder;
}


