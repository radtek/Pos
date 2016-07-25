#ifndef XPriceAdjustmentReportSectionH
#define XPriceAdjustmentReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"

class XPriceAdjustmentReportSection : public BaseReportSection
{

public:
    XPriceAdjustmentReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    ~XPriceAdjustmentReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
};


#endif