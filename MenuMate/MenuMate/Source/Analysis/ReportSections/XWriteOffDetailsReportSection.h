//---------------------------------------------------------------------------

#ifndef XWriteOffDetailsReportSectionH
#define XWriteOffDetailsReportSectionH
//---------------------------------------------------------------------------

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XWriteOffDetailsReportSection : public BaseReportSection
{

public:
    XWriteOffDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XWriteOffDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XWriteOffDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
    void GetWriteOffDataforNormalZed(TIBSQL* writeOffQuery);
    void GetWriteOffDataforConsolidatedZed(TIBSQL* writeOffQuery);
};

#endif
