#include "XSessionDateDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "Analysis.h"

XSessionDateDetailsReportSection::XSessionDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmSessionDateDetailsSection, dbTransaction, globalSettings)
{
}


XSessionDateDetailsReportSection::~XSessionDateDetailsReportSection()
{
}

void XSessionDateDetailsReportSection::GetOutput(TPrintout* printOut)
{
    if(!printOut->BlindBalanceUsed)
        return;
    IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printOut);
	}
    if(TGlobalSettings::Instance().ShowSessionDateInZed)
    {
        AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
        printOut->PrintFormat->Line->Columns[0]->Text =deviceName ;
        printOut->PrintFormat->AddLine();
        const TMMContactInfo &staff_member = TfrmAnalysis::GetLastAuthenticatedUser();
        printOut->PrintFormat->Line->Columns[0]->Text =staff_member.Name;
        printOut->PrintFormat->AddLine();
    }
}
