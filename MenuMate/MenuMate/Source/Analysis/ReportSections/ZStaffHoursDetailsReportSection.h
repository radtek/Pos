#ifndef ZStaffHoursDetailsReportSectionH
#define ZStaffHoursDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class ZStaffHoursDetailsReportSection : public BaseReportSection
{

public:
    ZStaffHoursDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    ZStaffHoursDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~ZStaffHoursDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
};

#endif
