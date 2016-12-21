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
    XBreakdownCategoriesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~XBreakdownCategoriesDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);
    //virtual void GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime);

private:
    DataFormatUtilities* dataFormatUtilities;
    void GetBreakDownDetailsForNormalZed(TIBSQL *qrBreakdownCategories, AnsiString deviceName);
    void GetBreakDownDetailsForConsolidatedZed(TIBSQL *qrBreakdownCategories, AnsiString deviceName, TDateTime &startTime, TDateTime &endTime);
    void LoadBreakDownCategoryDetails(TPrintout* printout, TIBSQL *qrBreakdownCategories, TStringList *CatList);
};

#endif
