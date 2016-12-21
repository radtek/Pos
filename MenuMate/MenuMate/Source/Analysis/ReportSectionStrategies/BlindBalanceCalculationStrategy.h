#ifndef BlindBalanceCalculationStrategyH
#define BlindBalanceCalculationStrategyH

#include "BaseReportSectionDisplayStrategy.h"
#include "Printout.h"

class BlindBalanceCalculationStrategy : public BaseReportSectionDisplayStrategy
{
public:
    BlindBalanceCalculationStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance);
    BlindBalanceCalculationStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance, TDateTime* startTime, TDateTime* endTime);
	virtual void BuildSection(TPrintout* printOut);

protected:
    bool _isMasterBalance;

};

#endif
