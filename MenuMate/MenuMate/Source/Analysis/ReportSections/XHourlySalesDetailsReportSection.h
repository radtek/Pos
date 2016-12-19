#ifndef XHourlySalesDetailsReportSectionH
#define XHourlySalesDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XHourlySalesDetailsReportSection : public BaseReportSection
{

public:
    XHourlySalesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XHourlySalesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings,TDateTime* startTime, TDateTime* endTime);
    ~XHourlySalesDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    DataFormatUtilities* dataFormatUtilities;
};

#endif
