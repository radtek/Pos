#ifndef ZTotalsDetailsReportSectionH
#define ZTotalsDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class ZTotalsDetailsReportSection : public BaseReportSection
{
public:
	ZTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~ZTotalsDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
};

#endif

