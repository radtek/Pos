#ifndef ClientDetailsFromFileStrategyH
#define ClientDetailsFromFileStrategyH

#include <Classes.hpp>
#include "BaseReportSectionDisplayStrategy.h"

class ClientDetailsFromFileStrategy : public BaseReportSectionDisplayStrategy
{
public:
    ClientDetailsFromFileStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	virtual void BuildSection(TPrintout* printOut);
};

#endif
