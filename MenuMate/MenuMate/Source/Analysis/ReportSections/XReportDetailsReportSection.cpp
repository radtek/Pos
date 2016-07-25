#include "XReportDetailsReportSection.h"

XReportDetailsReportSection::XReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmReportDetailsSection, dbTransaction, globalSettings)
{
}


XReportDetailsReportSection::~XReportDetailsReportSection()
{
}

void XReportDetailsReportSection::GetOutput(TPrintout* printout)
{
    printout->PrintFormat->NewLine();
    printout->PrintFormat->Line->Columns[0]->Text = "Summary Report";
    printout->PrintFormat->AddLine();

    printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
    printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width;

    printout->PrintFormat->NewLine();
    printout->PrintFormat->Line->Columns[0]->Text = "Preliminary (X) Till Not Closed Off";
    printout->PrintFormat->AddLine();
}
