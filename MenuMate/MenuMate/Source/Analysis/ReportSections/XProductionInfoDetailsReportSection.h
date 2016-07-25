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
    ~XProductionInfoDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
};

#endif
