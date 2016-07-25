#include "PointsReportHeaderTrait.h"

PointsReportHeaderTrait::PointsReportHeaderTrait(ReportSectionTraitType reportSectionTraitType)
    :BaseReportSectionDisplayTraits(reportSectionTraitType)
{
}

void PointsReportHeaderTrait::ApplyTraits(TPrintout* printOut)
{
    printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
    printOut->PrintFormat->Line->ColCount = 3;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 3;
	printOut->PrintFormat->Line->Columns[0]->Text = "Name";
	printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
	printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
	printOut->PrintFormat->Line->Columns[1]->Text = "Earnt";
	printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
	printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width / 3;
	printOut->PrintFormat->Line->Columns[2]->Text = "Redeemed";
	printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
	printOut->PrintFormat->AddLine();

	printOut->PrintFormat->Line->ColCount = 1;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
	printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
	printOut->PrintFormat->Line->Columns[0]->Line();
	printOut->PrintFormat->AddLine();
}
