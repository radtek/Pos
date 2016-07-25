#ifndef XClientDetailsReportSectionH
#define XClientDetailsReportSectionH

#include <Classes.hpp>
#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"

class XClientDetailsReportSection : public BaseReportSection
{
public:
	XClientDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~XClientDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
};

#endif
