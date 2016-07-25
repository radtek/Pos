#include "XClientDetailsReportSection.h"
#include "DeviceRealTerminal.h"


XClientDetailsReportSection::XClientDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmClientDetailsSection, dbTransaction, globalSettings)
{
}


XClientDetailsReportSection::~XClientDetailsReportSection()
{
}

void XClientDetailsReportSection::GetOutput(TPrintout* printout)
{
    IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printout);
	}
}

