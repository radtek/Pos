//---------------------------------------------------------------------------

#ifndef PrinterPhysicalH
#define PrinterPhysicalH
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include "GraphicPrinterSettings.h"

enum TPrinterType	{ptWindows_Printer,ptScreen_Printer,ptChefMate_Printer};
enum TEmulationMode {emTextOnly, emEpson, emStar, emScript};

class TPrinterPhysical
{
	public :
	int PhysicalPrinterKey;
	UnicodeString ServerName;
	UnicodeString ShareName;
	UnicodeString ConnectionString();
	int PhysicalProfileKey;

	int PrinterModel;
	int ConnectionKey;

	TPrinterType Type; //
	unsigned char BoldCharPerLine;
	unsigned char NormalCharPerLine;
	unsigned char CutterGap;
	unsigned char TopMargin;
	unsigned char KitchenTopMargin;
	bool AlwaysUseFullCut;
	bool KanjiPrinter;
    int PrinterCodePage;
	UnicodeString PrinterCodePageOn;
	UnicodeString PrinterCodePageOff;
    UnicodeString PrinterInitCmd;
	TEmulationMode EmulationMode;   // emTextOnly, emEpson, emStar, emScript
	char SingleLineChar;
	char DoubleLineChar;

   TGraphicPrinterSettings Graphics;

	TPrinterPhysical();
	UnicodeString UNCName();
};

#endif
