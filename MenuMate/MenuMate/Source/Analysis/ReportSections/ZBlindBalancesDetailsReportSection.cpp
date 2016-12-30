#include "ZBlindBalancesDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "Forms.hpp"
#include "BlindBalanceController.h"

ZBlindBalancesDetailsReportSection::ZBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmZReport, mmBlindBalancesDetailsSection, dbTransaction, globalSettings)
{
}

ZBlindBalancesDetailsReportSection::ZBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmBlindBalancesDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
}


ZBlindBalancesDetailsReportSection::~ZBlindBalancesDetailsReportSection()
{
}

void ZBlindBalancesDetailsReportSection::GetOutput(TPrintout* printOut)
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
        return;
	}
}