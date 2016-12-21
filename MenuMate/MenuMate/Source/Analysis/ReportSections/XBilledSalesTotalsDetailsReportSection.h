//---------------------------------------------------------------------------

#ifndef XBilledSalesTotalsDetailsReportSectionH
#define XBilledSalesTotalsDetailsReportSectionH
#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"
#include "ReportFinancialCalculations.h"
//---------------------------------------------------------------------------

class XBilledSalesTotalsDetailsReportSection : public BaseReportSection
{

public:
    XBilledSalesTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XBilledSalesTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XBilledSalesTotalsDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    TMembership* _memberShip;
    DataFormatUtilities* dataFormatUtilities;
    ReportFinancialCalculations* reportFinancialCalculations;
};

#endif
