#include "ZClientDetailsReportSection.h"


ZClientDetailsReportSection::ZClientDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	: BaseReportSection(mmZReport, mmClientDetailsSection, dbTransaction, globalSettings)
{
}

ZClientDetailsReportSection::~ZClientDetailsReportSection()
{
}

void ZClientDetailsReportSection::GetOutput(TPrintout* printout)
{
    if(!printout->BlindBalanceUsed)
        return;
	IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printout);
	}
}