#include "TextValueDisplayFormatTraits.h"

TextValueDisplayFormatTraits::TextValueDisplayFormatTraits(ReportSectionTraitType reportSectionTraitType)
    :BaseReportSectionDisplayTraits(reportSectionTraitType)
{
}

void TextValueDisplayFormatTraits::ApplyTraits(TPrintout* printOut)
{
    printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
	printOut->PrintFormat->Line->ColCount = 2;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 2 / 3;
	printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
	printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 2 / 3);
	printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
}
