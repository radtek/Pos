#include "ReportSectionFactoryProvider.h"
#include "ZedReportSectionFactory.h"
#include "XReportSectionFactory.h"
#include "ConsolidatedZedReportSectionFactory.h"
#include "WaiterAppZedReportSectionFactory.h"


ReportSectionFactoryProvider::ReportSectionFactoryProvider()
{
}

ReportSectionFactoryProvider::~ReportSectionFactoryProvider()
{
}

IReportSectionFactory* ReportSectionFactoryProvider::CreateReportSectionFactory(ReportType reportType, Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
{
	IReportSectionFactory* reportSectionFactory;

	switch (reportType)
	{
        case mmZReport:
            reportSectionFactory = new ZedReportSectionFactory(dbTransaction, globalSettings);
            break;
        case mmXReport:
            reportSectionFactory = new XReportSectionFactory(dbTransaction, globalSettings);
            break;
        case mmWaiterAppZReport:
            reportSectionFactory = new WaiterAppZedReportSectionFactory(dbTransaction, globalSettings);
            break;
        default:
            reportSectionFactory = NULL;
	}

	return reportSectionFactory;
}

IReportSectionFactory* ReportSectionFactoryProvider::CreateReportSectionFactoryForConsolidatedZed(ReportType reportType, Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
{
	IReportSectionFactory* reportSectionFactory;

	switch (reportType)
	{
        case mmConsolidatedZReport:
            reportSectionFactory = new ConsolidatedZedReportSectionFactory(dbTransaction, globalSettings, startTime, endTime);
            break;
        default:
            reportSectionFactory = NULL;
	}

	return reportSectionFactory;
}
