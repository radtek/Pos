//---------------------------------------------------------------------------

#ifndef XTaxSummaryDetailsReportSectionH
#define XTaxSummaryDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"
#include "ReportFinancialCalculations.h"

//---------------------------------------------------------------------------
struct TSalesTax
{
    double Rate;
    double TaxSum;
};



class XTaxSummaryDetailsReportSection : public BaseReportSection
{

public:
    XTaxSummaryDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XTaxSummaryDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XTaxSummaryDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);
    void GetDifferentTotalSalesTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName);
    std::vector<TSalesTax> sales_tax;
    void SetPrinterFormatToMiddle(TPrintout* printOut);

private:
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
    ReportFinancialCalculations* reportCalculations;

    TMembership* _memberShip;
    void GetDifferentTotalSalesTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName, TDateTime &startTime, TDateTime &endTime);
};


#endif
