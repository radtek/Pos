#ifndef XRefundDetailsReportSectionH
#define XRefundDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"

class XRefundDetailsReportSection : public BaseReportSection
{

public:
    XRefundDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XRefundDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XRefundDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
    void GetRefundItemForConsolidatedZed(TIBSQL *creditQuery, AnsiString terminalNamePredicate, AnsiString deviceName);
    void GetRefundItemForNormalZed(TIBSQL *creditQuery ,AnsiString terminalNamePredicate, AnsiString deviceName);
};


#endif
