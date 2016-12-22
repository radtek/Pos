#ifndef CashDenominationCalculationStrategyForConsolidatedZedH
#define CashDenominationCalculationStrategyForConsolidatedZedH

#include "BaseReportSectionDisplayStrategy.h"
#include "Printout.h"

class CashDenominationCalculationStrategyForConsolidatedZed : public BaseReportSectionDisplayStrategy
{
public:
    CashDenominationCalculationStrategyForConsolidatedZed(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance, TDateTime* startTime, TDateTime* endTime);
	virtual void BuildSection(TPrintout* printOut);

protected:
    bool _isMasterBalance;

};

#endif
