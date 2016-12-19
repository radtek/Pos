#include "XMasterBlindBalancesDetailsReportSection.h"
#include "DeviceRealTerminal.h"

XMasterBlindBalancesDetailsReportSection::XMasterBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmMasterBlindBalancesDetailsSection, dbTransaction, globalSettings)
{
}

XMasterBlindBalancesDetailsReportSection::XMasterBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmXReport, mmMasterBlindBalancesDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
}


XMasterBlindBalancesDetailsReportSection::~XMasterBlindBalancesDetailsReportSection()
{
}

void XMasterBlindBalancesDetailsReportSection::GetOutput(TPrintout* printOut)
{
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    AddTitle(printOut, deviceName + " Blind Balances");

    printOut->PrintFormat->NewLine();

    IReportSectionDisplayTraits* reportSectionDisplayTrait = GetTextFormatDisplayTrait();
    if(reportSectionDisplayTrait)
    {
        reportSectionDisplayTrait->ApplyTraits(printOut);
    }

    IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printOut);
	}
}

void XMasterBlindBalancesDetailsReportSection::GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime)
{
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    AddTitle(printOut, deviceName + " Blind Balances");

    printOut->PrintFormat->NewLine();

    IReportSectionDisplayTraits* reportSectionDisplayTrait = GetTextFormatDisplayTrait();
    if(reportSectionDisplayTrait)
    {
        reportSectionDisplayTrait->ApplyTraits(printOut);
    }

    IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printOut);
	}
}
