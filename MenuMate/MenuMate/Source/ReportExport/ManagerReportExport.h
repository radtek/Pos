//---------------------------------------------------------------------------

#ifndef ManagerReportExportH
#define ManagerReportExportH
#include "MallExportRegenerateReport.h"
#include "HavanaReport.h"
//---------------------------------------------------------------------------

class TManagerReportExport
{
    private:

    void CreateFileExportPath(UnicodeString &path);
    void PrepareDataForCSVFile(TDateTime SDate, TDateTime EDate, UnicodeString reportExportPath);
    public:
    void ExportReport();

};
#endif
