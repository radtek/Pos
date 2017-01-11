#ifndef ZStaffHoursDetailsReportSectionForConsolidatedZedH
#define ZStaffHoursDetailsReportSectionForConsolidatedZedH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class ZStaffHoursDetailsReportSectionForConsolidatedZed : public BaseReportSection
{

public:
     ZStaffHoursDetailsReportSectionForConsolidatedZed(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~ZStaffHoursDetailsReportSectionForConsolidatedZed();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
};

#endif
