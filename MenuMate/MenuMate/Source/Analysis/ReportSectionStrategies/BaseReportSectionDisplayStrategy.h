#ifndef BaseReportSectionDisplayStrategyH
#define BaseReportSectionDisplayStrategyH

#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "IReportSectionDisplayStrategy.h"

class BaseReportSectionDisplayStrategy : public IReportSectionDisplayStrategy
{
public:
	BaseReportSectionDisplayStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    BaseReportSectionDisplayStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    bool IsConsolidatedStartegy;

protected:
    Database::TDBTransaction* _dbTransaction;
    TGlobalSettings* _globalSettings;
    TDateTime* _startTime;
    TDateTime* _endTime;
};


#endif
