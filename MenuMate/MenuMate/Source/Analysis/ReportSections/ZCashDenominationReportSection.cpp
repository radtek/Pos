#include "ZCashDenominationReportSection.h"
#include "DeviceRealTerminal.h"
#include "CashDenominationController.h"

ZCashDenominationReportSection::ZCashDenominationReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmZReport, mmCashDenominationDetailsSection, dbTransaction, globalSettings)
{
}


ZCashDenominationReportSection::ZCashDenominationReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmCashDenominationDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
}

ZCashDenominationReportSection::~ZCashDenominationReportSection()
{
}

void ZCashDenominationReportSection::GetOutput(TPrintout* printOut)
{
    try
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
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}