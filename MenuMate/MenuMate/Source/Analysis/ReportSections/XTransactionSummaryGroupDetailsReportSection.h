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
    ~XTransactionSummaryGroupDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    void SetPrinterFormat(TPrintout* printOut);
    void SetSingleColumnPrinterFormat(TPrintout* printOut);
    void SetPrinterFormatInMiddle(TPrintout* printOut);

private:
    TMembership* _memberShip;
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;

};


#endif
