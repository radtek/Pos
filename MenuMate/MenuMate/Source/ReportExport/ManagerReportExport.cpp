//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerReportExport.h"
#include "ReportExportFactory.h"
#include "ReportExportInterface.h"
#include "HavanaReport.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TManagerReportExport::ExportReport(int index)
{
    TReportExportInterface* reportType = TReportExportFactory::GetReportType(index);
    reportType->PostDataToFile();
}
//---------------------------------------------------------------------------------------------------------

