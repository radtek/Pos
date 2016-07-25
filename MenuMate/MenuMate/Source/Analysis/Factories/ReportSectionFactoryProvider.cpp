#include "ReportSectionFactoryProvider.h"
#include "ZedReportSectionFactory.h"
#include "XReportSectionFactory.h"


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
        default:
            reportSectionFactory = NULL;
	}

	return reportSectionFactory;
}
