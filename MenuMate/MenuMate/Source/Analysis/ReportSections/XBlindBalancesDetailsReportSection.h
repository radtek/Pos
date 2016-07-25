#ifndef XBlindBalancesDetailsReportSectionH
#define XBlindBalancesDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class XBlindBalancesDetailsReportSection : public BaseReportSection
{
public:
	XBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~XBlindBalancesDetailsReportSection();

	virtual void GetOutput(TPrintout* printOut);
};

#endif
