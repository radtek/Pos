
#ifndef XDiscountReportDetailsReportSectionH
#define XDiscountReportDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XDiscountReportDetailsReportSection: public BaseReportSection
{
public:
	XDiscountReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~XDiscountReportDetailsReportSection();
	virtual void GetOutput(TPrintout* printout);
    void SetPrinterFormat(TPrintout* printOut);
    void SetSingleColumnPrinterFormat(TPrintout* printOut);
private:
    DataFormatUtilities* dataFormatUtilities;
};


#endif
