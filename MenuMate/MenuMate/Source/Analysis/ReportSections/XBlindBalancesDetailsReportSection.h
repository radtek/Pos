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
    XBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~XBlindBalancesDetailsReportSection();

	virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut,TDateTime* startTime, TDateTime* endTime);
};

#endif
