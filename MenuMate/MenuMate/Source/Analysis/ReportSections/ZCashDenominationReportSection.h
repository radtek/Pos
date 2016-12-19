#ifndef ZCashDenominationReportSectionH
#define ZCashDenominationReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class ZCashDenominationReportSection : public BaseReportSection
{
public:
	ZCashDenominationReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    ZCashDenominationReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~ZCashDenominationReportSection();

	virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);
};

#endif
