//---------------------------------------------------------------------------

#ifndef XCancelsDetailsReportSectionH
#define XCancelsDetailsReportSectionH
//---------------------------------------------------------------------------

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XCancelsDetailsReportSection : public BaseReportSection
{

public:
    XCancelsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XCancelsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XCancelsDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
    void GetCancelReportsForNormalZed(TIBSQL* cancelsQuery, AnsiString terminalNamePredicate);
    void GetCancelReportsForConsolidatedZed(TIBSQL* cancelsQuery, AnsiString terminalNamePredicate, TDateTime prevZedTime);
};

#endif
