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
	~XReportDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
};


#endif
