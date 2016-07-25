#ifndef ReportSectionDisplayStrategyH
#define ReportSectionDisplayStrategyH

#include "IReportSectionDisplayStrategy.h"
#include "Printout.h"

class ReportSectionDisplayStrategy :public IReportSectionDisplayStrategy
{
public:
	ReportSectionDisplayStrategy();
	~ReportSectionDisplayStrategy();

	virtual void BuildSection(TPrintout* printOut) = 0;
};

#endif

