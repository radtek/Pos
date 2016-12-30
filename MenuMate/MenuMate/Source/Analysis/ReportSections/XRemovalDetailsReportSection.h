//---------------------------------------------------------------------------

#ifndef XRemovalDetailsReportSectionH
#define XRemovalDetailsReportSectionH
//---------------------------------------------------------------------------

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XRemovalDetailsReportSection : public BaseReportSection
{

public:
    XRemovalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XRemovalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XRemovalDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    DataFormatUtilities* dataFormatUtilities;
    void GetRemovalReportsForNormalZed(TIBSQL* removalsQuery, AnsiString deviceName);
    void GetRemovalReportsForConsolidatedZed(TIBSQL* removalsQuery, TDateTime prevZedTime, AnsiString deviceName);
};

#endif