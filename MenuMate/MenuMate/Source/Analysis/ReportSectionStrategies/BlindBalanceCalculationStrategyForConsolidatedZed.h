#ifndef BlindBalanceCalculationStrategyForConsolidatedZedH
#define BlindBalanceCalculationStrategyForConsolidatedZedH

#include "BaseReportSectionDisplayStrategy.h"
#include "Printout.h"
#include "BlindBalanceController.h"

class BlindBalanceCalculationStrategyForConsolidatedZed : public BaseReportSectionDisplayStrategy
{
public:
    //BlindBalanceCalculationStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance);
    BlindBalanceCalculationStrategyForConsolidatedZed(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance, TDateTime* startTime, TDateTime* endTime);
	virtual void BuildSection(TPrintout* printOut);

protected:
    bool _isMasterBalance;
    bool _isConsolidatedStrategy;

private:
void GetBlindBalanceDetailsForConsolidatedZed(TIBSQL *ibInternalQuery, AnsiString terminalNamePredicate);
void LoadBlindBalanceDetails(TPrintout* printOut, TIBSQL *ibInternalQuery, TIBSQL *ibLoadDataQuery, AnsiString deviceName);

};

#endif
