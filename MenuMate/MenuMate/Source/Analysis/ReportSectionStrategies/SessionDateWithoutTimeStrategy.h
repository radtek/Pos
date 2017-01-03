#ifndef SessionDateWithoutTimeStrategyH
#define SessionDateWithoutTimeStrategyH

#include "Printout.h"
#include "BaseReportSectionDisplayStrategy.h"

class SessionDateWithoutTimeStrategy : public BaseReportSectionDisplayStrategy
{
public:
    SessionDateWithoutTimeStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    SessionDateWithoutTimeStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	virtual void BuildSection(TPrintout* printOut);
    virtual int CalculateLastDayOfMonth(int month);
};

#endif
