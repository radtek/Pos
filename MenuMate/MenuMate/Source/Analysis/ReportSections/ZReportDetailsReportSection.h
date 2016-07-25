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
	~ZReportDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
};

#endif
