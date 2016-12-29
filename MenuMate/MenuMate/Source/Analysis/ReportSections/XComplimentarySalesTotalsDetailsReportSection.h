//---------------------------------------------------------------------------

#ifndef XComplimentarySalesTotalsDetailsReportSectionH
#define XComplimentarySalesTotalsDetailsReportSectionH
#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"
#include "ReportFinancialCalculations.h"
//---------------------------------------------------------------------------
class XComplimentarySalesTotalsDetailsReportSection : public BaseReportSection
{

public:
    XComplimentarySalesTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XComplimentarySalesTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XComplimentarySalesTotalsDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    TMembership* _memberShip;
    DataFormatUtilities* dataFormatUtilities;
    ReportFinancialCalculations* reportFinancialCalculations;
};
#endif
