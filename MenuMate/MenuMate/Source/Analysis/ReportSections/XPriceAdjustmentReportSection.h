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
    XPriceAdjustmentReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XPriceAdjustmentReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
    void GetPriceAdjustmentQueryForNormalZed(TStringList* AdjustmentsSQL, AnsiString masterSlaveCondition);
    void GetPriceAdjustmentQueryForConsolidatedZed(TStringList* AdjustmentsSQL, AnsiString masterSlaveCondition);
};


#endif