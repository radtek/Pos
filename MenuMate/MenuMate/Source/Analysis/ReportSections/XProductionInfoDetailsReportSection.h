#ifndef XProductionInfoDetailsReportSectionH
#define XProductionInfoDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XProductionInfoDetailsReportSection : public BaseReportSection
{

public:
    XProductionInfoDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XProductionInfoDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XProductionInfoDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    DataFormatUtilities* dataFormatUtilities;
};

#endif
