#ifndef CashDenominationCalculationStrategyH
#define CashDenominationCalculationStrategyH

#include "BaseReportSectionDisplayStrategy.h"
#include "Printout.h"

class CashDenominationCalculationStrategy : public BaseReportSectionDisplayStrategy
{
public:
    CashDenominationCalculationStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance);
    CashDenominationCalculationStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance, TDateTime* startTime, TDateTime* endTime);
	virtual void BuildSection(TPrintout* printOut);

protected:
    bool _isMasterBalance;

};

#endif
