#ifndef EndOfDayReportH
#define EndOfDayReportH

#include "BaseReport.h"

class EndOfDayReport : public BaseReport
{
public:
	EndOfDayReport();

protected:
	bool _closeTill;
};


#endif

