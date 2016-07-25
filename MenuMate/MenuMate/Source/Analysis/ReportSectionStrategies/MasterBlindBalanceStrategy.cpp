#include "MasterBlindBalanceStrategy.h"
#include "DeviceRealTerminal.h"
#include "GlobalSettings.h"

MasterBlindBalanceStrategy::MasterBlindBalanceStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSectionDisplayStrategy(dbTransaction, globalSettings)
{
}

void MasterBlindBalanceStrategy::BuildSection(TPrintout* printOut)
{
    TDeviceRealTerminal::Instance().LoadHdrFtr();
    TStringList *textContent = TGlobalSettings::Instance().ZedHeader.get();

    for (int i = 0; i < textContent->Count; i++)
    {
        printOut->PrintFormat->Line->Columns[0]->Text = textContent->Strings[i];
        printOut->PrintFormat->AddLine();
    }
}
