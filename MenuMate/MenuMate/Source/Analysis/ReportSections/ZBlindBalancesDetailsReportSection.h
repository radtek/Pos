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
    ZBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~ZBlindBalancesDetailsReportSection();

	virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);
};

#endif
