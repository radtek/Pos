
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
    XDiscountReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~XDiscountReportDetailsReportSection();
	virtual void GetOutput(TPrintout* printout);
    void SetPrinterFormat(TPrintout* printOut);
    void SetSingleColumnPrinterFormat(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime);
private:
    DataFormatUtilities* dataFormatUtilities;
    UnicodeString DiscountQueryForNormalZed(UnicodeString DiscountSQL, TPrintout* printout);
    UnicodeString DiscountQueryForConsolidatedZed(UnicodeString DiscountSQL, TPrintout* printout);
};


#endif
