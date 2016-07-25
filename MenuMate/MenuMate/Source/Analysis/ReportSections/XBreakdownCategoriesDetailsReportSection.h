#ifndef XBreakdownCategoriesDetailsReportSectionH
#define XBreakdownCategoriesDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"

class XBreakdownCategoriesDetailsReportSection: public BaseReportSection
{
public:
	XBreakdownCategoriesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~XBreakdownCategoriesDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);

private:
    DataFormatUtilities* dataFormatUtilities;
};

#endif
