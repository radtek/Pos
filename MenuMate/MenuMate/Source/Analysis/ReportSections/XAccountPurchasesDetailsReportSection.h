#ifndef XAccountPurchasesDetailsReportSectionH
#define XAccountPurchasesDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XAccountPurchasesDetailsReportSection : public BaseReportSection
{

public:
    XAccountPurchasesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XAccountPurchasesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XAccountPurchasesDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);


private:
    DataFormatUtilities* dataFormatUtilities;
};


#endif
