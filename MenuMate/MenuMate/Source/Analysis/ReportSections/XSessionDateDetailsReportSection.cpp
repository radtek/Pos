#include "XSessionDateDetailsReportSection.h"

XSessionDateDetailsReportSection::XSessionDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmSessionDateDetailsSection, dbTransaction, globalSettings)
{
}


XSessionDateDetailsReportSection::~XSessionDateDetailsReportSection()
{
}

void XSessionDateDetailsReportSection::GetOutput(TPrintout* printOut)
{
    IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printOut);
	}
}
