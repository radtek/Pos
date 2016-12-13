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
	~ZCashDenominationReportSection();

	virtual void GetOutput(TPrintout* printOut);
};

#endif