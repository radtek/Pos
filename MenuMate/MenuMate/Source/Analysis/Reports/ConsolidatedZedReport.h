#ifndef ConsolidatedZedReportH
#define ConsolidatedZedReportH

#include "EndOfDayReport.h"

class ConsolidatedZedReport : public EndOfDayReport
{
public:
    ConsolidatedZedReport();
    AnsiString GetReportName();

    int DisplayAndPrint(TMemoryStream* memoryStream = NULL);
    bool SkipZedProcess;

};

#endif

