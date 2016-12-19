//---------------------------------------------------------------------------

#ifndef XAccountBalancesSeatedDetailsReportSectionH
#define XAccountBalancesSeatedDetailsReportSectionH
#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"
//---------------------------------------------------------------------------

class XAccountBalancesSeatedDetailsReportSection : public BaseReportSection
{

public:
    XAccountBalancesSeatedDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XAccountBalancesSeatedDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XAccountBalancesSeatedDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    TMembership* _memberShip;
    DataFormatUtilities* dataFormatUtilities;
    void GetReportData(TStringList *SeatedList);
    void GetTabData(TStringList *TabTypeList,TIBSQL *qrXTab, TIBSQL *qrXOrder);
    void GetCreditData(TStringList *TabTypeList,TIBSQL *qrXTab, TIBSQL *qrXOrder);
    void CalculateData(TCurrencyTotal *TabTotal,TStringList *TabTypeList,TIBSQL *qrXTab, TIBSQL *qrXOrder);
};
#endif
