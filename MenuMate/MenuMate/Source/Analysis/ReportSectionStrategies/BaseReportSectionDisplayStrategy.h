#ifndef BaseReportSectionDisplayStrategyH
#define BaseReportSectionDisplayStrategyH

#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "IReportSectionDisplayStrategy.h"

class BaseReportSectionDisplayStrategy : public IReportSectionDisplayStrategy
{
public:
	BaseReportSectionDisplayStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);

protected:
    Database::TDBTransaction* _dbTransaction;
    TGlobalSettings* _globalSettings;
};


#endif
