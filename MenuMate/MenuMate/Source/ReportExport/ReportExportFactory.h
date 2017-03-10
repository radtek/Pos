//---------------------------------------------------------------------------

#ifndef ReportExportFactoryH
#define ReportExportFactoryH
#include "ReportExportInterface.h"
#include "HavanaReport.h"
//---------------------------------------------------------------------------

class TReportExportFactory
{
    public:
    static TReportExportInterface* GetReportType(int index);
};
#endif
