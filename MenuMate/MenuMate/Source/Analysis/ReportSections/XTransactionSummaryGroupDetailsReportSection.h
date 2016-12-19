#ifndef XTransactionSummaryGroupDetailsReportSectionH
#define XTransactionSummaryGroupDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"

class XTransactionSummaryGroupDetailsReportSection : public BaseReportSection
{

public:
    XTransactionSummaryGroupDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XTransactionSummaryGroupDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XTransactionSummaryGroupDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);
    void SetSingleColumnPrinterFormat(TPrintout* printOut);

private:
    TMembership* _memberShip;
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
    void DisplayBankingSection(TPrintout* printOut);

};


#endif
