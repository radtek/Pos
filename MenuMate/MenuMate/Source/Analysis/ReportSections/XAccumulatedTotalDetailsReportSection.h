#ifndef XAccumulatedTotalDetailsReportSectionH
#define XAccumulatedTotalDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "Printout.h"
#include "ReportUtilities.h"


class XAccumulatedTotalDetailsReportSection : public BaseReportSection
{
public:
	XAccumulatedTotalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~XAccumulatedTotalDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);

private:
    AnsiString GetStartInvoiceNumber();
    AnsiString GetEndInvoiceNumber();
    void FormatInvoiceNumber(AnsiString &inStartInvoiceNumber,AnsiString &inEndInvoiceNumber);

    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
};

#endif
