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
	~XCurrentDateDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
};

#endif
