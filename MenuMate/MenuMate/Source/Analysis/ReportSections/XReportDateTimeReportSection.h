#ifndef XReportDateTimeReportSectionH
#define XReportDateTimeReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "Printout.h"
#include "ReportUtilities.h"


class XReportDateTimeReportSection : public BaseReportSection
{
public:
	XReportDateTimeReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~XReportDateTimeReportSection();

	virtual void GetOutput(TPrintout* printout);

private:
    AnsiString GetStartInvoiceNumber();
    AnsiString GetEndInvoiceNumber();
    AnsiString GetLastEndInvoiceNumber();
    void FormatInvoiceNumber(AnsiString &inStartInvoiceNumber,AnsiString &inEndInvoiceNumber);

    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
    AnsiString ExtractInvoiceNumber(AnsiString &inStartInvoiceNumber);
    void SetPrinterFormatInMiddle(TPrintout* printOut);
};

#endif
