#include "XCurrentDateDetailsReportSection.h"

XCurrentDateDetailsReportSection::XCurrentDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmCurrentDateDetailsSection, dbTransaction, globalSettings)
{
}


XCurrentDateDetailsReportSection::~XCurrentDateDetailsReportSection()
{
}

void XCurrentDateDetailsReportSection::GetOutput(TPrintout* printout)
{
    IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printout);
	}
}
