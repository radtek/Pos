#ifndef SessionDateWithoutTimeStrategyH
#define SessionDateWithoutTimeStrategyH

#include "Printout.h"
#include "BaseReportSectionDisplayStrategy.h"

class SessionDateWithoutTimeStrategy : public BaseReportSectionDisplayStrategy
{
public:
    SessionDateWithoutTimeStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	virtual void BuildSection(TPrintout* printOut);
    virtual int CalculateLastDayOfMonth(int month);
};

#endif
