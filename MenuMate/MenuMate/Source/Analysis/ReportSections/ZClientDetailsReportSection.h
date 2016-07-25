#ifndef ZClientDetailsReportSectionH
#define ZClientDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class ZClientDetailsReportSection : public BaseReportSection
{
public:
	ZClientDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~ZClientDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
};

#endif
