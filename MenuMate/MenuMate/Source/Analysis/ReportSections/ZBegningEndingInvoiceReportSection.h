#ifndef ZBegningEndingInvoiceReportSectionH
#define ZBegningEndingInvoiceReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "Printout.h"
#include "ReportUtilities.h"


class ZBegningEndingInvoiceReportSection : public BaseReportSection
{
public:
	ZBegningEndingInvoiceReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~ZBegningEndingInvoiceReportSection();

	virtual void GetOutput(TPrintout* printout);

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
