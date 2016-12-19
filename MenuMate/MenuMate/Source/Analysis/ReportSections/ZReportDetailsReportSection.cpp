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
    if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
    {

        printout->PrintFormat->Line->ColCount = 4;
        printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width * 1/5;
        printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width * 1/3.5 - printout->PrintFormat->Line->Columns[0]->Width;
        printout->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
        printout->PrintFormat->Line->Columns[2]->Width = printout->PrintFormat->Width  * 1/2.5 + printout->PrintFormat->Line->Columns[0]->Width;
        printout->PrintFormat->Line->Columns[2]->Alignment = taCenter;
        printout->PrintFormat->Line->Columns[3]->Width = 0;
        printout->PrintFormat->NewLine();
        printout->PrintFormat->Line->Columns[0]->Text = "";
        printout->PrintFormat->Line->Columns[1]->Alignment = taCenter;
        printout->PrintFormat->Line->Columns[2]->Text = "Z READING REPORT";
        printout->PrintFormat->AddLine();
        SetPrinterFormatInMiddle(printout);
        printout->PrintFormat->Line->Columns[0]->Alignment = taRightJustify;
        printout->PrintFormat->Line->Columns[0]->Text = "--";
        printout->PrintFormat->Line->Columns[1]->Text = "-----------------------------------------------------------";
        printout->PrintFormat->Line->Columns[2]->Text = "-----------------------------------------------------------";
        printout->PrintFormat->Line->Columns[3]->Alignment = taLeftJustify;
        printout->PrintFormat->Line->Columns[3]->Text = "--";
        printout->PrintFormat->AddLine();
    }
    else
    {

        printout->PrintFormat->NewLine();
        printout->PrintFormat->Line->Columns[0]->Text = "Summary Report";
        printout->PrintFormat->AddLine();

        printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width;

        printout->PrintFormat->NewLine();
        printout->PrintFormat->Line->Columns[0]->Text = "Final (Z)";
        printout->PrintFormat->AddLine();
    }
}
void ZReportDetailsReportSection::SetPrinterFormatInMiddle(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 4;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 1/5;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1/2.5;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width  * 1/2.5;
    printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
    printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width * 1/4;
}

