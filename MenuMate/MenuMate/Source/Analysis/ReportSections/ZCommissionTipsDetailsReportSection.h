#ifndef ZCommissionTipsDetailsReportSectionH
#define ZCommissionTipsDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class ZCommissionTipsDetailsReportSection : public BaseReportSection
{

public:
    ZCommissionTipsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    ~ZCommissionTipsDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
};

#endif
