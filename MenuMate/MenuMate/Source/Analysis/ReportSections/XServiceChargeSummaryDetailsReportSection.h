#ifndef XServiceChargeSummaryDetailsReportSectionH
#define XServiceChargeSummaryDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"
#include "ReportFinancialCalculations.h"

//---------------------------------------------------------------------------
class XServiceChargeSummaryDetailsReportSection : public BaseReportSection
{

public:
    XServiceChargeSummaryDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XServiceChargeSummaryDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XServiceChargeSummaryDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    TMembership* _memberShip;
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
    ReportFinancialCalculations* reportCalculations;

    void SetupPrintFormat(TPrintFormat* pf);
};



#endif
