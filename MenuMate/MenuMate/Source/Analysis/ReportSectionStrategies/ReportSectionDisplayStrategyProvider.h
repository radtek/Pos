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
    ReportSectionDisplayStrategyProvider(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~ReportSectionDisplayStrategyProvider();

	IReportSectionDisplayStrategy* CreateSectionStrategy(ReportType reportType, ReportSectionType reportSectionType);

private:
	TGlobalSettings* _globalSettings;
    Database::TDBTransaction* _dbTransaction;
    TDateTime* _startTime;
    TDateTime* _endTime;
};

#endif 

