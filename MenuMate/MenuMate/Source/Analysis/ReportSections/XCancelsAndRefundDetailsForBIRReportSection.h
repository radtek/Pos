//---------------------------------------------------------------------------

#ifndef XCancelsAndRefundDetailsForBIRReportSectionH
#define XCancelsAndRefundDetailsForBIRReportSectionH
//---------------------------------------------------------------------------

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XCancelsAndRefundDetailsForBIRReportSection : public BaseReportSection
{

public:
    XCancelsAndRefundDetailsForBIRReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    ~XCancelsAndRefundDetailsForBIRReportSection();

    virtual void GetOutput(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
};

#endif
