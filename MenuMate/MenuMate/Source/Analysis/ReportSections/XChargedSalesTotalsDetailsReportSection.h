//---------------------------------------------------------------------------

#ifndef XChargedSalesTotalsDetailsReportSectionH
#define XChargedSalesTotalsDetailsReportSectionH
#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"
#include "ReportFinancialCalculations.h"
//---------------------------------------------------------------------------

class XChargedSalesTotalsDetailsReportSection : public BaseReportSection
{

public:
    XChargedSalesTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XChargedSalesTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XChargedSalesTotalsDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    TMembership* _memberShip;
    DataFormatUtilities* dataFormatUtilities;
    ReportFinancialCalculations* reportFinancialCalculations;
};

#endif
