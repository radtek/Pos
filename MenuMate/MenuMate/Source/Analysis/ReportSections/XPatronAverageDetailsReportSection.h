//---------------------------------------------------------------------------

#ifndef XPatronAverageDetailsReportSectionH
#define XPatronAverageDetailsReportSectionH
#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"
//---------------------------------------------------------------------------
class XPatronAverageDetailsReportSection : public BaseReportSection
{

public:
    XPatronAverageDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XPatronAverageDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XPatronAverageDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    TMembership* _memberShip;
    DataFormatUtilities* dataFormatUtilities;
    void PrintPatronStatistics(TPrintout* printOut);
    void PrintChitStatistics(TPrintout* printOut);
    void GetDistinctTerminalNameForNormalZed(TIBSQL *query);
    void GetDistinctTerminalNameForConsolidatedZed(TIBSQL *query);
    void GetTotalSpendForConsolidatedZed(TIBSQL *query, UnicodeString terminalNames);
    void GetTotalSpendForNormalZed(TIBSQL *query, UnicodeString terminalNames);
    void GetChitNameForConsolidatedZed(TIBSQL *query, UnicodeString terminalNames);
    void GetChitNameForNormalZed(TIBSQL *query, UnicodeString terminalNames);
    void GetTotalPatronCountForConsolidatedZed(TIBSQL *query, UnicodeString terminalNames);
    void GetTotalPatronCountForNormalZed(TIBSQL *query, UnicodeString terminalNames);
};
#endif
