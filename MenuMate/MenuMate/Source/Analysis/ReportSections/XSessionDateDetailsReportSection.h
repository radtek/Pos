#ifndef XSessionDateDetailsReportSectionH
#define XSessionDateDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class XSessionDateDetailsReportSection: public BaseReportSection
{
public:
	XSessionDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~XSessionDateDetailsReportSection();

	virtual void GetOutput(TPrintout* printOut);
};

#endif
