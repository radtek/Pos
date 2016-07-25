#ifndef ZBlindBalancesDetailsReportSectionH
#define ZBlindBalancesDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class ZBlindBalancesDetailsReportSection : public BaseReportSection
{
public:
	ZBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~ZBlindBalancesDetailsReportSection();

	virtual void GetOutput(TPrintout* printOut);
};

#endif
