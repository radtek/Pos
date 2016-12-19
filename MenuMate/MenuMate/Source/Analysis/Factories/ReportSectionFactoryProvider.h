#ifndef ReportSectionFactoryProviderH
#define ReportSectionFactoryProviderH

#include <memory>
#include "ReportEnums.h"
#include "IReportSection.h"
#include "IReportSectionFactory.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class ReportSectionFactoryProvider
{
public:
	ReportSectionFactoryProvider();
	~ReportSectionFactoryProvider();

	IReportSectionFactory* CreateReportSectionFactory(ReportType reportType, Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    IReportSectionFactory* CreateReportSectionFactoryForConsolidatedZed(ReportType reportType, Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
};

#endif