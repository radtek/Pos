#ifndef XTotalsDetailsReportSectionH
#define XTotalsDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"
#include "ReportFinancialCalculations.h"

class XTotalsDetailsReportSection : public BaseReportSection
{
public:
	XTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~XTotalsDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
    virtual void GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime);

private:
    TMembership* _memberShip;
    DataFormatUtilities* dataFormatUtilities;
    ReportFinancialCalculations* reportFinancialCalculations;
};

#endif

