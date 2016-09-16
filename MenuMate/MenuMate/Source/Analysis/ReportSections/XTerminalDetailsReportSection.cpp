#include "XTerminalDetailsReportSection.h"
#include "DeviceRealTerminal.h"

XTerminalDetailsReportSection::XTerminalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmClientDetailsSection, dbTransaction, globalSettings)
{
}


XTerminalDetailsReportSection::~XTerminalDetailsReportSection()
{
}

void XTerminalDetailsReportSection::GetOutput(TPrintout* printout)
{
    if(!printout->BlindBalanceUsed)
        return;
    printout->PrintFormat->Line->Columns[0]->Text = TDeviceRealTerminal::Instance().ID.Name;
    printout->PrintFormat->AddLine();
}
