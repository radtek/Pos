#include "XTerminalDetailsReportSection.h"
#include "DeviceRealTerminal.h"

XTerminalDetailsReportSection::XTerminalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmClientDetailsSection, dbTransaction, globalSettings)
{
}


XTerminalDetailsReportSection::XTerminalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
    :BaseReportSection(mmConsolidatedZReport, mmClientDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
}


XTerminalDetailsReportSection::~XTerminalDetailsReportSection()
{
}

void XTerminalDetailsReportSection::GetOutput(TPrintout* printout)
{
    printout->PrintFormat->Line->Columns[0]->Text = TDeviceRealTerminal::Instance().ID.Name;
    printout->PrintFormat->AddLine();
}

/*void XTerminalDetailsReportSection::GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime)
{
    printout->PrintFormat->Line->Columns[0]->Text = TDeviceRealTerminal::Instance().ID.Name;
    printout->PrintFormat->AddLine();
}*/
