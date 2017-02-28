//---------------------------------------------------------------------------


#pragma hdrstop

#include "ReportExportFactory.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TReportExportInterface* TReportExportFactory::GetReportType(int index)
{
    TReportExportInterface* reportType;

    switch(index)
    {
        case 1:
            reportType = new THavanaReport();
            break;
        //For New Report Type Add case here
    }
    return reportType;
}
