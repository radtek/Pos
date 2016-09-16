#ifndef ZedReportH
#define ZedReportH

#include "EndOfDayReport.h"

class ZedReport : public EndOfDayReport
{
public:
    ZedReport();
    AnsiString GetReportName();

    int DisplayAndPrint(TMemoryStream* memoryStream = NULL);
    bool SkipZedProcess;

};

#endif

