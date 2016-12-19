#include "XClientDetailsReportSection.h"
#include "DeviceRealTerminal.h"


XClientDetailsReportSection::XClientDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmClientDetailsSection, dbTransaction, globalSettings)
{
}

XClientDetailsReportSection::XClientDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
     :BaseReportSection(mmXReport, mmClientDetailsSection, dbTransaction, globalSettings, startTime, endTime)
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

void XClientDetailsReportSection::GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime)
{
    IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printout);
	}
}

