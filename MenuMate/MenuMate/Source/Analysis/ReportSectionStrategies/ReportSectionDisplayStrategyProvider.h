#ifndef ReportSectionDisplayStrategyProviderH
#define ReportSectionDisplayStrategyProviderH

#include "IReportSectionDisplayStrategy.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "ReportEnums.h"

class ReportSectionDisplayStrategyProvider
{
public:
	ReportSectionDisplayStrategyProvider(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~ReportSectionDisplayStrategyProvider();

	IReportSectionDisplayStrategy* CreateSectionStrategy(ReportType reportType, ReportSectionType reportSectionType);

private:
	TGlobalSettings* _globalSettings;
    Database::TDBTransaction* _dbTransaction;
};

#endif 

