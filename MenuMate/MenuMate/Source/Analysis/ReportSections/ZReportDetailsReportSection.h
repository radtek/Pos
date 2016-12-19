#ifndef ZReportDetailsReportSectionH
#define ZReportDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class ZReportDetailsReportSection: public BaseReportSection
{
public:
	ZReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    ZReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~ZReportDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
    void SetPrinterFormatInMiddle(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime);
};

#endif
