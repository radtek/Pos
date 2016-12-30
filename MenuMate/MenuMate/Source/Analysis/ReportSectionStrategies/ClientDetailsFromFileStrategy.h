#ifndef ClientDetailsFromFileStrategyH
#define ClientDetailsFromFileStrategyH

#include <Classes.hpp>
#include "BaseReportSectionDisplayStrategy.h"

class ClientDetailsFromFileStrategy : public BaseReportSectionDisplayStrategy
{
public:
    ClientDetailsFromFileStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    ClientDetailsFromFileStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	virtual void BuildSection(TPrintout* printOut);
};

#endif
