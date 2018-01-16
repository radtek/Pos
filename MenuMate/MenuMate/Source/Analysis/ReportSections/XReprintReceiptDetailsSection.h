//---------------------------------------------------------------------------

#ifndef XReprintReceiptDetailsSectionH
#define XReprintReceiptDetailsSectionH
//---------------------------------------------------------------------------
#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XReprintReceiptDetailsSection: public BaseReportSection
{
public:
	XReprintReceiptDetailsSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XReprintReceiptDetailsSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~XReprintReceiptDetailsSection();
	virtual void GetOutput(TPrintout* printout);
    void SetPrinterFormat(TPrintout* printOut);
    void SetSingleColumnPrinterFormat(TPrintout* printOut);
private:
    DataFormatUtilities* dataFormatUtilities;
    void ReprintReceiptQueryForNormalZed(TIBSQL* qrInvoice);
    void ReprintReceiptForConsolidatedZed( TIBSQL* qrInvoice, TDateTime* startTime, TDateTime* endTime);
};


#endif
