#ifndef XSessionDateDetailsReportSectionH
#define XSessionDateDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class XSessionDateDetailsReportSection: public BaseReportSection
{
public:
	XSessionDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XSessionDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~XSessionDateDetailsReportSection();

	virtual void GetOutput(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);
private:
    void ShowSessionDate(TPrintout* printOut);
};

#endif
