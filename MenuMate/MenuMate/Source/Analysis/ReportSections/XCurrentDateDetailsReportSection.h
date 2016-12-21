#ifndef XCurrentDateDetailsReportSectionH
#define XCurrentDateDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class XCurrentDateDetailsReportSection: public BaseReportSection
{
public:
	XCurrentDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XCurrentDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~XCurrentDateDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
    //virtual void GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime);
};

#endif
