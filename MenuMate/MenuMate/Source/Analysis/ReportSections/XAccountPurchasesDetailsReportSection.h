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
    ~XAccountPurchasesDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
};


#endif
