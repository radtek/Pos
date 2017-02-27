//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerReportExport.h"
#include "ReportExportFactory.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TManagerReportExport::ExportReport(int index)
{
    TReportExport* reportType = TReportExportFactory::GetReportType(index);
    reportType->PostDataToFile();
}
//---------------------------------------------------------------------------------------------------------

