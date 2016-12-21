#ifndef XCommissionTipsDetailsReportSectionH
#define XCommissionTipsDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XCommissionTipsDetailsReportSection : public BaseReportSection
{

public:
    XCommissionTipsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XCommissionTipsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XCommissionTipsDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
};


#endif
