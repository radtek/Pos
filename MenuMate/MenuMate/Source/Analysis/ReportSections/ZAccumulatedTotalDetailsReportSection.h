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
	ZAccumulatedTotalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~ZAccumulatedTotalDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
    void SetPrinterFormatForSingleColumn(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime);

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
