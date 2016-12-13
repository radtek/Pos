#ifndef ZAccumulatedTotalDetailsReportSectionH
#define ZAccumulatedTotalDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "Printout.h"
#include "ReportUtilities.h"


class ZAccumulatedTotalDetailsReportSection : public BaseReportSection
{
public:
	ZAccumulatedTotalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~ZAccumulatedTotalDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
    void SetPrinterFormatForSingleColumn(TPrintout* printOut);

private:
    AnsiString GetStartInvoiceNumber();
    AnsiString GetEndInvoiceNumber();
    AnsiString GetLastEndInvoiceNumber();
    void FormatInvoiceNumber(AnsiString &inStartInvoiceNumber,AnsiString &inEndInvoiceNumber);

    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
    AnsiString ExtractInvoiceNumber(AnsiString &inStartInvoiceNumber);
};

#endif