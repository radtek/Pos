#include "FullDateWithTimeStrategy.h"

FullDateWithTimeStrategy::FullDateWithTimeStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSectionDisplayStrategy(dbTransaction, globalSettings)
{
}

void FullDateWithTimeStrategy::BuildSection(TPrintout* printOut)
{

    printOut->PrintFormat->Line->ColCount = 1;
    printOut->PrintFormat->Line->Columns[0]->Text = Now().FormatString("d mmm yyyy h:nn:ss am/pm");
    printOut->PrintFormat->AddLine();
}
