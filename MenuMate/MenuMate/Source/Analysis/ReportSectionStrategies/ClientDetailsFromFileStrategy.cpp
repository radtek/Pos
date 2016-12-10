#include "ClientDetailsFromFileStrategy.h"
#include "DeviceRealTerminal.h"
#include "GlobalSettings.h"

ClientDetailsFromFileStrategy::ClientDetailsFromFileStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSectionDisplayStrategy(dbTransaction, globalSettings)
{
}

void ClientDetailsFromFileStrategy::BuildSection(TPrintout* printOut)
{
    TDeviceRealTerminal::Instance().LoadHdrFtr();
    TStringList *textContent = TGlobalSettings::Instance().ZedHeader.get();
    if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
    {
        printOut->PrintFormat->Line->ColCount = 4;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 1/5;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1/3.5 - printOut->PrintFormat->Line->Columns[0]->Width;
        printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width  * 1/2.5 + printOut->PrintFormat->Line->Columns[0]->Width;
        printOut->PrintFormat->Line->Columns[2]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[3]->Width = 0;
        for (int i = 0; i < textContent->Count; i++)
        {
            printOut->PrintFormat->Line->Columns[2]->Text = textContent->Strings[i];
            printOut->PrintFormat->AddLine();
        }
    }
    else
    {
        for (int i = 0; i < textContent->Count; i++)
        {
            printOut->PrintFormat->Line->Columns[0]->Text = textContent->Strings[i];
            printOut->PrintFormat->AddLine();
        }
    }
}
