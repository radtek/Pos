#include "XBlindBalancesDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "Forms.hpp"

XBlindBalancesDetailsReportSection::XBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmBlindBalancesDetailsSection, dbTransaction, globalSettings)
{
}


XBlindBalancesDetailsReportSection::~XBlindBalancesDetailsReportSection()
{
}

void XBlindBalancesDetailsReportSection::GetOutput(TPrintout* printOut)
{
    AddTitle(printOut, " Blind Balances");
    printOut->PrintFormat->NewLine();
    IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();
    if(reportSectionDisplayTraits)
    {
        reportSectionDisplayTraits->ApplyTraits(printOut);
    }

    IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printOut);
	}
}
