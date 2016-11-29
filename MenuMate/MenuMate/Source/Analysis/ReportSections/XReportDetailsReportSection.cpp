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

    if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
    {
        printout->PrintFormat->NewLine();
        printout->PrintFormat->Line->Columns[0]->Text = "X READING REPORT";
        printout->PrintFormat->AddLine();
        SetPrinterFormatInMiddle(printout);
        printout->PrintFormat->Line->Columns[0]->Text = "";
        printout->PrintFormat->Line->Columns[1]->Text = "-----------------------------------------------------------";
        printout->PrintFormat->Line->Columns[2]->Text = "-----------------------------------------------------------";
        printout->PrintFormat->Line->Columns[3]->Text = "-----------------------------------------------------------";
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
        printout->PrintFormat->Line->Columns[0]->Text = "Preliminary (X) Till Not Closed Off";
        printout->PrintFormat->AddLine();
    }
}

void XReportDetailsReportSection::SetPrinterFormatInMiddle(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 4;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width  / 4 - 2;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width  / 4 + 8;
    printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width  / 4;
    printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
    printOut->PrintFormat->Line->Columns[3]->Width = 0;
}

