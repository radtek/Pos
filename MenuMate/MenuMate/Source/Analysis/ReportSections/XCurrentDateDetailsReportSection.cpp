#include "XCurrentDateDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "Analysis.h"

XCurrentDateDetailsReportSection::XCurrentDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmCurrentDateDetailsSection, dbTransaction, globalSettings)
{
}

XCurrentDateDetailsReportSection::XCurrentDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmCurrentDateDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
}



XCurrentDateDetailsReportSection::~XCurrentDateDetailsReportSection()
{
}

void XCurrentDateDetailsReportSection::GetOutput(TPrintout* printout)
{
    IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printout);
	}
    if(!TGlobalSettings::Instance().ShowSessionDateInZed)
    {
        AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
        printout->PrintFormat->Line->Columns[0]->Text =deviceName ;
        printout->PrintFormat->AddLine();
        const TMMContactInfo &staff_member = TfrmAnalysis::GetLastAuthenticatedUser();
        printout->PrintFormat->Line->Columns[0]->Text =staff_member.Name;
        printout->PrintFormat->AddLine();
        if(!IsConsolidatedZed)
        {
            DataCalculationUtilities* dataCalculationUtilities = new DataCalculationUtilities;
            int value = dataCalculationUtilities->GetZedNumber(*_dbTransaction);
            value += 1;
            printout->PrintFormat->Line->Columns[0]->Text = "#" + IntToStr(value);
            printout->PrintFormat->AddLine();
        }
    }

        
}
