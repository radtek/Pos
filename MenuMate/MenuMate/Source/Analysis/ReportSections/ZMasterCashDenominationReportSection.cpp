//---------------------------------------------------------------------------


#pragma hdrstop

#include "ZMasterCashDenominationReportSection.h"
#include "DeviceRealTerminal.h"
#include "CashDenominationController.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
ZMasterCashDenominationReportSection::ZMasterCashDenominationReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmMasterCashDenominationDetailsSection, dbTransaction, globalSettings)
{
}

ZMasterCashDenominationReportSection::ZMasterCashDenominationReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmMasterCashDenominationDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
}

ZMasterCashDenominationReportSection::~ZMasterCashDenominationReportSection()
{
}

void ZMasterCashDenominationReportSection::GetOutput(TPrintout* printOut)
{
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    AddTitle(printOut, deviceName + " Cash Denominations");
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
