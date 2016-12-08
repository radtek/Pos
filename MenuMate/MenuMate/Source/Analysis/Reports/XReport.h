#ifndef XReportH
#define XReportH


#include "EndOfDayReport.h"

class XReport : public EndOfDayReport
{
public:
	XReport();
    AnsiString GetReportName();

    int DisplayAndPrint(TMemoryStream* memoryStream = NULL);
    void PopulateXReportForEJournal(TMemoryStream* memoryStream);
};


#endif
