#ifndef IReportSectionDisplayStrategyH
#define IReportSectionDisplayStrategyH

#include "Printout.h"

class IReportSectionDisplayStrategy
{
public:
	virtual void BuildSection(TPrintout* printOut) = 0;
};

#endif

