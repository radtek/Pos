//---------------------------------------------------------------------------

#ifndef XTaxSummaryDetailsReportSectionH
#define XTaxSummaryDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"
#include "ReportFinancialCalculations.h"

//---------------------------------------------------------------------------
class XTaxSummaryDetailsReportSection : public BaseReportSection
{

public:
    XTaxSummaryDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    ~XTaxSummaryDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
    ReportFinancialCalculations* reportCalculations;

    TMembership* _memberShip;
};


#endif
