//---------------------------------------------------------------------------

#ifndef WaiterAppZedReportH
#define WaiterAppZedReportH
#include "EndOfDayReport.h"

class WaiterAppZedReport : public EndOfDayReport
{
public:
    WaiterAppZedReport();
    AnsiString GetReportName();

    int DisplayAndPrint(TMemoryStream* memoryStream = NULL);
    bool SkipZedProcess;

};
//---------------------------------------------------------------------------
#endif
