#ifndef FullDateWithTimeStrategyH
#define FullDateWithTimeStrategyH

#include "IReportSectionDisplayStrategy.h"
#include "Printout.h"
#include "BaseReportSectionDisplayStrategy.h"

class FullDateWithTimeStrategy : public BaseReportSectionDisplayStrategy
{
public:
    FullDateWithTimeStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    FullDateWithTimeStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	virtual void BuildSection(TPrintout* printOut);
};

#endif
