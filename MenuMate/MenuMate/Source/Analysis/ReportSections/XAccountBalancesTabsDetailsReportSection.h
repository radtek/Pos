#ifndef XAccountBalancesTabsDetailsReportSectionH
#define XAccountBalancesTabsDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"

class XAccountBalancesTabsDetailsReportSection : public BaseReportSection
{

public:
    XAccountBalancesTabsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XAccountBalancesTabsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XAccountBalancesTabsDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);


private:
    TMembership* _memberShip;
    DataFormatUtilities* dataFormatUtilities;
    void GetReportData(TStringList *TabsList);
    void GetTabData(TStringList *TabTypeList,TIBSQL *qrXTab, TIBSQL *qrXOrder);
    void GetCreditData(TStringList *TabTypeList, TIBSQL *qrXTab, TIBSQL *qrXOrder);
    void CalculateData(TCurrencyTotal *TabTotal, TStringList *TabTypeList, TIBSQL *qrXTab, TIBSQL *qrXOrder);
};
#endif
