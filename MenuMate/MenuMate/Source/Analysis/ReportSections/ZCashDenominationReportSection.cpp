#include "ZCashDenominationReportSection.h"
#include "DeviceRealTerminal.h"
#include "CashDenominationController.h"

ZCashDenominationReportSection::ZCashDenominationReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmZReport, mmCashDenominationDetailsSection, dbTransaction, globalSettings)
{
}


ZCashDenominationReportSection::~ZCashDenominationReportSection()
{
}

void ZCashDenominationReportSection::GetOutput(TPrintout* printOut)
{
    AddTitle(printOut, " Cash Denominations");
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