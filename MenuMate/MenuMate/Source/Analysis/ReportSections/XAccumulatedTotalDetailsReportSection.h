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
    XAccumulatedTotalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~XAccumulatedTotalDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
    void SetPrinterFormatInMiddle(TPrintout* printOut);

private:
    AnsiString GetStartInvoiceNumber();
    AnsiString GetEndInvoiceNumber();
    AnsiString GetLastEndInvoiceNumber();
    void FormatInvoiceNumber(AnsiString &inStartInvoiceNumber,AnsiString &inEndInvoiceNumber);

    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
    AnsiString ExtractInvoiceNumber(AnsiString &inStartInvoiceNumber);
    AnsiString GetEndInvoiceNumber(TDateTime &startTime, TDateTime &endTime);
    AnsiString GetStartInvoiceNumber(TDateTime &startTime, TDateTime &endTime);
};

#endif
