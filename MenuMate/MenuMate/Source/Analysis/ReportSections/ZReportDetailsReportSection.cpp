#include "ZReportDetailsReportSection.h"

ZReportDetailsReportSection::ZReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmZReport, mmReportDetailsSection, dbTransaction, globalSettings)
{
}


ZReportDetailsReportSection::~ZReportDetailsReportSection()
{
}

void ZReportDetailsReportSection::GetOutput(TPrintout* printout)
{
    if(!printout->BlindBalanceUsed)
        return;
    printout->PrintFormat->NewLine();
    printout->PrintFormat->Line->Columns[0]->Text = "Summary Report";
    printout->PrintFormat->AddLine();

    printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
    printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width;

    printout->PrintFormat->NewLine();
    printout->PrintFormat->Line->Columns[0]->Text = "Final (Z)";
    printout->PrintFormat->AddLine();
}
