#include "ReportBuilderFactory.h"
#include "ZReportBuilder.h"
#include "ZedReport.h"
#include "XReportBuilder.h"
#include "XReport.h"
#include "ConsolidatedZReportBuilder.h"
#include "WaiterAppZReportBuilder.h"

ReportBuilderFactory::ReportBuilderFactory(TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
{
	_globalSettings = globalSettings;
	_dbTransaction = dbTransaction;
}

ReportBuilderFactory::ReportBuilderFactory(TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction, TDateTime* startTime, TDateTime* endTime)
{
	_globalSettings = globalSettings;
	_dbTransaction = dbTransaction;
    _startTime = startTime;
    _endTime = endTime;
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
	case mmConsolidatedZReport:
		reportBuilder = new ConsolidatedZReportBuilder(reportType, _globalSettings, _dbTransaction, _startTime, _endTime);
		break;
    case mmWaiterAppZReport:
		reportBuilder = new WaiterAppZReportBuilder(reportType, _globalSettings, _dbTransaction);
		break;
	default:
		reportBuilder = NULL;
		break;
	}

	return reportBuilder;
}


