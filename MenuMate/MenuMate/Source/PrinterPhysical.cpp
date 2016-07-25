//---------------------------------------------------------------------------


#pragma hdrstop

#include "PrinterPhysical.h"
#include "PrinterDefs.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TPrinterPhysical::TPrinterPhysical()
{
	ServerName = "localhost";
	ShareName = "Receipt";
	PhysicalPrinterKey = 0;
	PhysicalProfileKey = 0;

	PrinterModel = 0;
	BoldCharPerLine = DEFAULT_BOLD_WIDTH_A;
	NormalCharPerLine = DEFAULT_WIDTH_A;
	CutterGap = 2;
	TopMargin = 1;
	KitchenTopMargin = 1;
	PrinterCodePage = 0;
	PrinterCodePageOn = "";
	PrinterCodePageOff = "";
    PrinterInitCmd = "";
	AlwaysUseFullCut = false;
	KanjiPrinter = false;
	EmulationMode = emTextOnly;
	SingleLineChar = FONT_SOLID_LINE_TEXT;
	DoubleLineChar = FONT_DOUBLE_LINE_TEXT;
	Type = ptWindows_Printer;
	int ConnectionKey = 0;
}

UnicodeString TPrinterPhysical::UNCName()
{
	return "\\\\" + ServerName + "\\" + ShareName;
}
