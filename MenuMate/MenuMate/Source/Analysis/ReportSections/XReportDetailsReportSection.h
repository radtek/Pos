#ifndef XReportDetailsReportSectionH
#define XReportDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class XReportDetailsReportSection: public BaseReportSection
{
public:
	XReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~XReportDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
    virtual void GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime);
    void SetPrinterFormatInMiddle(TPrintout* printOut);
};


#endif
