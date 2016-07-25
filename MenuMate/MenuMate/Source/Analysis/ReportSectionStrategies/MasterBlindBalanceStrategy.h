#ifndef MasterBlindBalanceStrategyH
#define MasterBlindBalanceStrategyH

#include <Classes.hpp>
#include "BaseReportSectionDisplayStrategy.h"

class MasterBlindBalanceStrategy : public BaseReportSectionDisplayStrategy
{
public:
    MasterBlindBalanceStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	virtual void BuildSection(TPrintout* printOut);
};

#endif
