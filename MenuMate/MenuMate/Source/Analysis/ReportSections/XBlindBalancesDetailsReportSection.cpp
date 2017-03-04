#include "XBlindBalancesDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "Forms.hpp"

XBlindBalancesDetailsReportSection::XBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmBlindBalancesDetailsSection, dbTransaction, globalSettings)
{
}

XBlindBalancesDetailsReportSection::XBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmBlindBalancesDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
}


XBlindBalancesDetailsReportSection::~XBlindBalancesDetailsReportSection()
{
}

void XBlindBalancesDetailsReportSection::GetOutput(TPrintout* printOut)
{
    try
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
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
