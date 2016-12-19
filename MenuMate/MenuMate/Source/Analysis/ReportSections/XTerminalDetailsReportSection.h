#ifndef XTerminalDetailsReportSectionH
#define XTerminalDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class XTerminalDetailsReportSection: public BaseReportSection
{
public:
	XTerminalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XTerminalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~XTerminalDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
    virtual void GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime);
};


#endif
