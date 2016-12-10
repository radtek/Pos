#include "ZTotalsDetailsReportSection.h"

ZTotalsDetailsReportSection::ZTotalsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	: BaseReportSection(mmZReport, mmTotalsDetailsSection, dbTransaction, globalSettings)
{
}

ZTotalsDetailsReportSection::~ZTotalsDetailsReportSection()
{
}

void ZTotalsDetailsReportSection::GetOutput(TPrintout* printout)
{
	//TODO:    use the db transaction in BaseReportSection
	//          execute the query ( can create one or more methods to return query string )
	//          format the output and add them into printout object
	IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printout);
	}
}