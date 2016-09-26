#include "XCurrentDateDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "Analysis.h"

XCurrentDateDetailsReportSection::XCurrentDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmCurrentDateDetailsSection, dbTransaction, globalSettings)
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
        printout->PrintFormat->Line->Columns[0]->Text = "#" + UnicodeString(TGlobalSettings::Instance().ZCount);
        printout->PrintFormat->AddLine();
    }
        
}
