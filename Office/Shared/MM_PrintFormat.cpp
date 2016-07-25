//---------------------------------------------------------------------------
#pragma hdrstop

#include <memory>

#include "MM_PrintFormat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Printing;
//---------------------------------------------------------------------------
const AnsiString SCRIPT_1_0_CONTROL					= "\\";
const AnsiString SCRIPT_1_0_ESCAPE					= "e";
const AnsiString SCRIPT_1_0_BEGIN_SECTION			= "{";
const AnsiString SCRIPT_1_0_END_SECTION			= "}";

const AnsiString SCRIPT_1_0_VERSION					= "v1.0";
const AnsiString SCRIPT_1_0_UTF8						= "utf8";
const AnsiString SCRIPT_1_0_ASCII					= "ascii";

const AnsiString SCRIPT_1_0_FULL_CUT				= "cutf";
const AnsiString SCRIPT_1_0_PARTIAL_CUT			= "cutp";
const AnsiString SCRIPT_1_0_GRAPHIC					= "graphic";
const AnsiString SCRIPT_1_0_KITCHEN_BUZZER		= "buz";
const AnsiString SCRIPT_1_0_BAR_CODE				= "bar";
const AnsiString SCRIPT_1_0_FONT_UNDERLINED		= "fu";
const AnsiString SCRIPT_1_0_FONT_BOLD				= "fbold";
const AnsiString SCRIPT_1_0_FONT_DOUBLE_HEIGHT	= "fdh";
const AnsiString SCRIPT_1_0_FONT_DOUBLE_WIDTH	= "fdw";
const AnsiString SCRIPT_1_0_FONT_B					= "fb";
const AnsiString SCRIPT_1_0_FONT_RED				= "fr";

const AnsiString SCRIPT_1_0_LEFT_JUSTIFY			= "ljust";
const AnsiString SCRIPT_1_0_RIGHT_JUSTIFY			= "rjust";
const AnsiString SCRIPT_1_0_CENTER_JUSTIFY		= "cjust";
const AnsiString SCRIPT_1_0_COL_SIZE				= "csize";
const AnsiString SCRIPT_1_0_COL_ABSOLUTE_SIZE	= "cabs";
const AnsiString SCRIPT_1_0_COL_PERCENT_SIZE		= "cperc";
const AnsiString SCRIPT_1_0_COL_MAXIMUM_SIZE		= "cmax";
const AnsiString SCRIPT_1_0_KANJI					= "kanji";
const AnsiString SCRIPT_1_0_LINE_SIZE				= "lsize";
const AnsiString SCRIPT_1_0_LINE_ABSOLUTE_SIZE	= "labs";
const AnsiString SCRIPT_1_0_LINE_PERCENT_SIZE	= "lperc";
const AnsiString SCRIPT_1_0_SINGLE_LINE			= "sline";
const AnsiString SCRIPT_1_0_DOUBLE_LINE			= "dline";
const AnsiString SCRIPT_1_0_TEXT						= "text";
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Some arbitary printer defaults.
//---------------------------------------------------------------------------
const DEFAULT_WIDTH_A									= 44;
const DEFAULT_BOLD_WIDTH_A								= 41;
const DEFAULT_WIDTH_B									= 56;
const DEFAULT_BOLD_WIDTH_B								= 53;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// 					EPSON PRINTER CONTROL LANGUAGE COMMANDS
//---------------------------------------------------------------------------
// Prefixes for most of the following commands
const char ESC												= 0x1B;
const char FS 												= 0x1C;
const char GS 												= 0x1D;
//---------------------------------------------------------------------------
// Doesn't reset the downloaded graphic.
const char RESET											= 0x40;	// '@' Use ESC @
//---------------------------------------------------------------------------
const char OPEN_DRAWER									= 0x70;	// '@' Use ESC @
const char DRAWER1										= 0x00;	// '@' Use ESC @
const char DRAWER2										= 0x01;	// '@' Use ESC @
//---------------------------------------------------------------------------
// Chars
//---------------------------------------------------------------------------
// Characters for drawing a single or double line using Epson fonts.
const unsigned char FONT_SINGLE_LINE_EPSON		= 0xC4;
const unsigned char FONT_DOUBLE_LINE_EPSON		= 0xCD;
const unsigned char FONT_SINGLE_LINE_TEXT			= '_';
const unsigned char FONT_DOUBLE_LINE_TEXT			= '=';

//---------------------------------------------------------------------------
const char CR												= 0x0D;	// Carriage Return
const char LF												= 0x0A;	// Line Feed
const char FF												= 0x0C;	// Form Feed
//---------------------------------------------------------------------------
// Font Commands
//---------------------------------------------------------------------------
// Font size selector.
const char PRINT_MODE									= 0x21;	// '!'
// The mode bits constitute the 3rd part of the Font Command (ESC !) sequence
	const char MODE_FONT_A								= 0x00;
	const char MODE_FONT_B								= 0x01;
	const char MODE_BOLD									= 0x08;
	const char MODE_DOUBLE_HEIGHT						= 0x10;
	const char MODE_DOUBLE_WIDTH						= 0x20;
	const char MODE_UNDERLINE							= 0x80;

//---------------------------------------------------------------------------
// Colour Commands
//---------------------------------------------------------------------------
// Font size selector.
const char COLOUR_MODE									= 0x72;	// '!'
// The mode bits constitute the 3rd part of the Font Command (ESC !) sequence
	const char MODE_COLOUR_OFF							= 0x00;
	const char MODE_COLOUR_ON							= 0x01;

//---------------------------------------------------------------------------
// Bar Code Font Commands
//---------------------------------------------------------------------------
	const char CODE_FONT_A[]							= { 0x7B, 0x41};// {A
	const char CODE_FONT_B[]							= { 0x7B, 0x42};// {B
	const char CODE_FONT_C[]							= { 0x7B, 0x43};// {C
//---------------------------------------------------------------------------
// Cut commands
//---------------------------------------------------------------------------
const char CUT_MODE										= 0x56;	// 'V'
// The mode bits constitute the 3rd part of the Cut Command (GS V) sequence
// Follow with 0x00 -> 0xFF to scroll before cut
	const char MODE_FULL_CUT							= 0x41;
	const char MODE_PARTIAL_CUT						= 0x42;
//---------------------------------------------------------------------------
// Graphic commands
//---------------------------------------------------------------------------
const char DOWNLOAD_GRAPHIC							= 0x2A;	// '*'
// Two bytes following are Width/8 (m) and and Height/8 (n). ie (GS '*' m n)
// followed by m*n*8 data bytes. Thes are aranged vertically, where each
// pixel corresponds to a bit. MSB at the top, LSB at the bottom, followed
// by the rest of n bytes. Then back up to the top for the n+1 byte at the
// m=1 pos.
// Max m = 72. Max n = 48. Max m*n = 1536 for Axiohm A715
const char PRINT_GRAPHIC								= 0x2F;	// '/'
// Prints the graphic stored by DOWNLOAD_GRAPHIC in the following fashon.
// Use (GS, '/', Width) where Width is one of the following.
	const char SINGLE_HEIGHT_SINGLE_WIDTH			= 0x00;	// 0x30
	const char SINGLE_HEIGHT_DOUBLE_WIDTH			= 0x01;	// 0x31
	const char DOUBLE_HEIGHT_SINGLE_WIDTH			= 0x02;	// 0x32
	const char DOUBLE_HEIGHT_DOUBLE_WIDTH			= 0x03;	// 0x33
//---------------------------------------------------------------------------
// Barcode commands
//---------------------------------------------------------------------------
const char BARCODE_HRI_POSITION						= 0x48; // 'H'
// Human Readable Interpretation. (GS 'H' n) where n is 1 of the 4 following
// positions.
	const char BARCODE_HRI_POSITION_NONE			= 0x00;
	const char BARCODE_HRI_POSITION_ABOVE			= 0x01;
	const char BARCODE_HRI_POSITION_BELOW			= 0x02;
	const char BARCODE_HRI_POSITION_BOTH			= 0x03;

const char BARCODE_HRI_FONT							= 0x66; // 'f'
// Font for Human Readable Interpretation. (GS 'f' n) where n is 1 of the 2
// font types.
	const char BARCODE_HRI_FONT_A						= 0x00;
	const char BARCODE_HRI_FONT_B						= 0x01;

const char BARCODE_RATIO								= 0x67; // 'g'
// Bar code width (raito of Wide:Narrow) (GS 'g' n) where n is 1 of the 3
// font types.
	const char BARCODE_RATION_2_1						= 0x00;
	const char BARCODE_RATION_3_1						= 0x01;
	const char BARCODE_RATION_5_2						= 0x02;

const char BARCODE_AUTO_CHECK_CHAR					= 0x63; // 'c'

const char BARCODE_HEIGHT								= 0x68; // 'h'
// Vertical height in dots. (GS 'h' n) where n = 00 to FF
const char BARCODE_PRINT								= 0x6B; // 'k'
// Using option 2. (GS 'k' m n {d1 d2 d3...dn})
// n = number of data characters
// d is bar code data
// m is one of the below bar code types.
	const char BARCODE_TYPE_UPC_A						= 0x41;	// 11 to 12 chars, 48 to 57
	const char BARCODE_TYPE_UPC_E						= 0x42;	// 11 to 12 chars, 48 to 57
	const char BARCODE_TYPE_EAN13						= 0x43;	// 12 to 13 chars, 48 to 57
	const char BARCODE_TYPE_EAN8						= 0x44;	// 7 to 8 chars, 48 to 57
	const char BARCODE_TYPE_CODE_39					= 0x45;	// 1 to 255 chars, 48 to 57, 65 to 90, 32, 36, 37, 43, 45, 46, 47
	const char BARCODE_TYPE_INTERLEAVED				= 0x46;	// 1 to 255 chars, 48 to 57 (even # chars)
	const char BARCODE_TYPE_CODABAR					= 0x47;	// 1 to 255 chars, 48 to 57, 65 to 68, 36, 43, 45, 46, 47, 58
	const char BARCODE_TYPE_CODE_93					= 0x48;	// 1 to 255 chars
	const char BARCODE_TYPE_CODE_128					= 0x49;	// 2 to 255 chars

const char BARCODE_HORZ_EXPANSION					= 0x77; // 'w'
// Horizontal size. (GS 'w' n) where n = 01 to 04
//---------------------------------------------------------------------------
// Kanji Commands
//---------------------------------------------------------------------------
const char KANJI_CODE									= 0x43;	// 'C'
// Kanji code system. (FS 'C' n) where n = (00 or 48) or (01 or 49)
// In JIS mode,
//		Prmary byte:		<21>H to <7E>H
//		Secondary byte:	<21>H to <7E>H
// In SHIFT JIS mode,
//		Prmary byte:		<81>H to <9F>H and <E0>H to <EF>H
//		Secondary byte:	<40>H to <7E>H and <80>H to <FC>H
	const char KANJI_JIS									= 0x00;
	const char KANJI_SJIS								= 0x01;

const char KANJI_MODE_ON								= 0x26;	// '&'
// Select Kanji mode. (FS '&')
const char KANJI_MODE_OFF								= 0x2E;	// '.'
// Select Kanji mode. (FS '.')
//---------------------------------------------------------------------------
// 					 STAR PRINTER CONTROL LANGUAGE COMMANDS
//---------------------------------------------------------------------------
// Not supported
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TPrinterConfig::TPrinterConfig() :
		EmulationMode		(emEpson),
		TopMargin			(0),
		CutterGap			(0),
		AlwaysUseFullCut	(true),
		SingleLineChar		(FONT_SINGLE_LINE_TEXT),
		DoubleLineChar		(FONT_DOUBLE_LINE_TEXT),
		FontAWidth			(DEFAULT_WIDTH_A),
		FontABoldWidth		(DEFAULT_BOLD_WIDTH_A),
		FontBWidth			(DEFAULT_WIDTH_B),
		FontBBoldWidth		(DEFAULT_BOLD_WIDTH_B),
		KanjiCodeSystem	(kcSJIS),					// Use KANJI_JIS or KANJI_SJIS
		KanjiCodePage		(936),						// 936 seems to work (used in WideCharToMultiByte)
																// try 932 for jap
		DrawerKickPort		(kpAll),
		KitchenBuzzer		(kpNone)
{
}
//---------------------------------------------------------------------------
TColumnFont::TColumnFont() :
		Underlined		(false),
		Bold				(false),
		Colour			(fcBlack),
		Font				(ftFontA),
		Width				(fsNormalSize),
		Height			(fsNormalSize)
{
}
//---------------------------------------------------------------------------
TBarcodeProperties::TBarcodeProperties() :
		HRIPosition		(bpNone),
		HRIFont			(bcFontA),
		CheckChar		(true),
		Ratio				(bcRatio5_2),
		Height			(183),
		HorzExp			(2),
		Type				(btCode128)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TPrintFormat::TPrintFormat() :
		TextEncoding	(teUTF8)
{
}
//---------------------------------------------------------------------------
void TPrintFormat::AddLine(const TFormatLine &Line)
{
	fLines.push_back(Line);
}
//---------------------------------------------------------------------------
void TPrintFormat::DrawLine(TLineStyle LineStyle)
{
	TFormatLine Line(1);
	Line.Column(0).LineStyle = LineStyle;
	fLines.push_back(Line);
}
//---------------------------------------------------------------------------
void TPrintFormat::DrawLine(TLineStyle LineStyle, int AbsoluteWidth)
{
	TFormatLine Line(1);
	Line.Column(0).ColSizeMode	= smAbsolute;
	Line.Column(0).ColSize		= AbsoluteWidth;
	Line.Column(0).LineStyle	= LineStyle;
	fLines.push_back(Line);
}
//---------------------------------------------------------------------------
void TPrintFormat::Cut(TCutStyle CutStyle)
{
	TFormatLine Line(0);
	Line.CutStyle = CutStyle;
	fLines.push_back(Line);
}
//---------------------------------------------------------------------------
void TPrintFormat::KitchenBuzzer()
{
	TFormatLine Line(0);
	Line.KitchenBuzzer = true;
	fLines.push_back(Line);
}
//---------------------------------------------------------------------------
void TPrintFormat::AddBarcode(AnsiString Barcode)
{
	TFormatLine Line(0);
	Line.Barcode = Barcode;
	fLines.push_back(Line);
}
//---------------------------------------------------------------------------
void TPrintFormat::PrintGraphic()
{
	TFormatLine Line(0);
	Line.PrintGraphic = true;
	fLines.push_back(Line);
}
//---------------------------------------------------------------------------
AnsiString TPrintFormat::OpenDrawerText(const TPrinterConfig &PrinterConfig)
{
	AnsiString Output;
	if (PrinterConfig.EmulationMode == emEpson)
	{
		char KickDraw1[5] = {ESC, OPEN_DRAWER, DRAWER1, 100, 160}; // Check Relay speed.
		char KickDraw2[5] = {ESC, OPEN_DRAWER, DRAWER2, 100, 160}; // Check Relay speed.

		if (PrinterConfig.DrawerKickPort & kp1)
		{
			Output = Output + (AnsiString(KickDraw1, 5));
		}
		if (PrinterConfig.DrawerKickPort & kp2)
		{
			Output = Output + (AnsiString(KickDraw2, 5));
		}
	}
	return Output;
}
//---------------------------------------------------------------------------
AnsiString TPrintFormat::GetScript()
{
	AnsiString Script(SCRIPT_1_0_BEGIN_SECTION);

	Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_VERSION;
	if (TextEncoding == teUTF8)
	{
		Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_UTF8;
	}
	else if (TextEncoding == teASCII)
	{
		Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_ASCII;
	}
	for (unsigned int i=0; i<fLines.size(); i++)
	{
		Script += SCRIPT_1_0_BEGIN_SECTION;
		if (fLines[i].CutStyle == csFull)								Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_FULL_CUT;
		if (fLines[i].CutStyle == csPartial)							Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_PARTIAL_CUT;
		if (fLines[i].PrintGraphic)										Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_GRAPHIC;
		if (fLines[i].KitchenBuzzer)										Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_KITCHEN_BUZZER;
		if (fLines[i].Barcode != "")										Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_BAR_CODE + " " + fEscapeText(fLines[i].Barcode);

		if (fLines[i].ColumnFont.Underlined)							Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_FONT_UNDERLINED;
		if (fLines[i].ColumnFont.Bold)									Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_FONT_BOLD;
		if (fLines[i].ColumnFont.Width == fsDoubleSize)				Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_FONT_DOUBLE_HEIGHT;
		if (fLines[i].ColumnFont.Height == fsDoubleSize)			Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_FONT_DOUBLE_WIDTH;
		if (fLines[i].ColumnFont.Font == ftFontB)						Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_FONT_B;
		if (fLines[i].ColumnFont.Colour == fcRed)						Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_FONT_RED;

		for (unsigned int j=0; j<fLines[i].ColCount(); j++)
		{
			Script += SCRIPT_1_0_BEGIN_SECTION;
			if (fLines[i].Column(j).Alignment == taLeftJustify)	Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_LEFT_JUSTIFY;
			if (fLines[i].Column(j).Alignment == taRightJustify)	Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_RIGHT_JUSTIFY;
			if (fLines[i].Column(j).Alignment == taCenter)			Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_CENTER_JUSTIFY;
			if (fLines[i].Column(j).ColSizeMode == smAbsolute)		Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_COL_ABSOLUTE_SIZE;
			if (fLines[i].Column(j).ColSizeMode == smPercent)		Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_COL_PERCENT_SIZE;
			if (fLines[i].Column(j).ColSizeMode == smMaximum)		Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_COL_MAXIMUM_SIZE;
			Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_COL_SIZE + " " + IntToStr(fLines[i].Column(j).ColSize);
			if (fLines[i].Column(j).Kanji)								Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_KANJI;
			if (fLines[i].Column(j).LineStyle != lsNone)
			{
				if (fLines[i].Column(j).LineStyle == lsSingle)			Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_SINGLE_LINE;
				if (fLines[i].Column(j).LineStyle == lsDouble)			Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_DOUBLE_LINE;
			}
			if (!fLines[i].Column(j).Text.IsEmpty())
			{
				Script += SCRIPT_1_0_CONTROL + SCRIPT_1_0_TEXT;
				if (TextEncoding == teUTF8)
				{
					Script += " " + UTF8Encode(fEscapeText(fLines[i].Column(j).Text));
				}
				else
				{
					Script += " " + fEscapeText(AnsiString(fLines[i].Column(j).Text));
				}
			}
			Script += SCRIPT_1_0_END_SECTION;
		}
		Script += SCRIPT_1_0_END_SECTION;
	}
	Script += SCRIPT_1_0_END_SECTION;
	return Script;
}
//---------------------------------------------------------------------------
TScriptVersion TPrintFormat::LoadScript(AnsiString Script)
{
	enum TParseMode { pmNone, pmDoc, pmLine, pmCol };

	TParseMode		ParseMode	= pmNone;
	int				Index			= 0;
	AnsiString		Word;
	bool				Control;

	TScriptVersion	Version		= svUnknown;

	while (fGetNextWord(Script, Word, Control, Index))
	{
		if (Control)
		{
			if (ParseMode == pmNone)
			{
				// In this mode, only begin section is valid
				if (Word == SCRIPT_1_0_BEGIN_SECTION)
				{
					fLines.clear();
					ParseMode = pmDoc;
				}
			}
			else if (ParseMode == pmDoc)
			{
				if (Word == SCRIPT_1_0_BEGIN_SECTION)
				{
					TFormatLine FormatLine(0);
					AddLine(FormatLine);
					ParseMode = pmLine;
				}
				else if (Word == SCRIPT_1_0_END_SECTION)
				{
					ParseMode = pmNone;
				}
				else if (Word == SCRIPT_1_0_VERSION)					Version										= sv1_0;
				else if (Word == SCRIPT_1_0_UTF8)						TextEncoding								= teUTF8;
				else if (Word == SCRIPT_1_0_ASCII)						TextEncoding								= teASCII;
			}
			else if (ParseMode == pmLine && Version == sv1_0)
			{
				if (fLines.size() > 0)
				{
					TFormatLine &CurrentLine = fLines[fLines.size()-1];

					if (Word == SCRIPT_1_0_BEGIN_SECTION)
					{
						TFormatColumn FormatColumn;
						CurrentLine.AddColumn(FormatColumn);
						ParseMode = pmCol;
					}
					else if (Word == SCRIPT_1_0_END_SECTION)
					{
						ParseMode = pmDoc;
					}
					else if (Word == SCRIPT_1_0_FULL_CUT)				CurrentLine.CutStyle						= csFull;
					else if (Word == SCRIPT_1_0_PARTIAL_CUT)			CurrentLine.CutStyle						= csPartial;
					else if (Word == SCRIPT_1_0_GRAPHIC)				CurrentLine.PrintGraphic				= true;
					else if (Word == SCRIPT_1_0_KITCHEN_BUZZER)		CurrentLine.KitchenBuzzer				= true;
					else if (Word == SCRIPT_1_0_BAR_CODE)
					{
						if (fGetNextWord(Script, Word, Control, Index) && !Control)
						{
							CurrentLine.Barcode = Word;
						}
					}
					else if (Word == SCRIPT_1_0_FONT_UNDERLINED)		CurrentLine.ColumnFont.Underlined	= true;
					else if (Word == SCRIPT_1_0_FONT_BOLD)				CurrentLine.ColumnFont.Bold			= true;
					else if (Word == SCRIPT_1_0_FONT_DOUBLE_HEIGHT)	CurrentLine.ColumnFont.Height			= fsDoubleSize;
					else if (Word == SCRIPT_1_0_FONT_DOUBLE_WIDTH)	CurrentLine.ColumnFont.Width			= fsDoubleSize;
					else if (Word == SCRIPT_1_0_FONT_B)					CurrentLine.ColumnFont.Font			= ftFontB;
					else if (Word == SCRIPT_1_0_FONT_RED)				CurrentLine.ColumnFont.Colour			= fcRed;
				}
			}
			else if (ParseMode == pmCol && Version == sv1_0)
			{
				if (fLines.size() > 0 && fLines[fLines.size()-1].ColCount() > 0)
				{
					TFormatColumn &CurrentColumn = fLines[fLines.size()-1].Column(fLines[fLines.size()-1].ColCount()-1);

					if (Word == SCRIPT_1_0_END_SECTION)
					{
						ParseMode = pmLine;
					}
					else if (Word == SCRIPT_1_0_LEFT_JUSTIFY)			CurrentColumn.Alignment					= taLeftJustify;
					else if (Word == SCRIPT_1_0_RIGHT_JUSTIFY)		CurrentColumn.Alignment					= taRightJustify;
					else if (Word == SCRIPT_1_0_CENTER_JUSTIFY)		CurrentColumn.Alignment					= taCenter;
					else if (Word == SCRIPT_1_0_COL_ABSOLUTE_SIZE)	CurrentColumn.ColSize					= smAbsolute;
					else if (Word == SCRIPT_1_0_COL_PERCENT_SIZE)	CurrentColumn.ColSize					= smPercent;
					else if (Word == SCRIPT_1_0_COL_MAXIMUM_SIZE)	CurrentColumn.ColSize					= smMaximum;
					else if (Word == SCRIPT_1_0_KANJI)					CurrentColumn.Kanji						= true;
					else if (Word == SCRIPT_1_0_SINGLE_LINE)			CurrentColumn.LineStyle					= lsSingle;
					else if (Word == SCRIPT_1_0_DOUBLE_LINE)			CurrentColumn.LineStyle					= lsDouble;
					else if (Word == SCRIPT_1_0_COL_SIZE)
					{
						if (fGetNextWord(Script, Word, Control, Index) && !Control)
						{
							int Size = 0;
							if (TryStrToInt(Word, Size))
							{
								CurrentColumn.ColSize = Size;
							}
						}
					}
					else if (Word == SCRIPT_1_0_TEXT)
					{
						if (fGetNextWord(Script, Word, Control, Index) && !Control)
						{
							if (TextEncoding == teUTF8)
							{
								CurrentColumn.Text = UTF8Decode(Word);
							}
							else if (TextEncoding == teUTF8)
							{
								CurrentColumn.Text = Word;
							}
						}
					}
				}
			}
		}
	}
	if (Version == svUnknown)
	{
		std::auto_ptr<TStringList> Lines(new TStringList);

		AnsiString TrimmedLine;
		char *Line = Script.c_str();
		for(int j=0; j<Script.Length(); j++)
		{
			if (Line[j] == NULL)
			{
				j++;
			}
			else if (Line[j] == ESC)
			{
				if (Line[j+1] == PRINT_MODE)
				{
					j += 2;
				}
			}
			else if (Line[j] == GS)
			{
				if (Line[j+1] == CUT_MODE)
				{
					j += 2;
				}
				else if (Line[j+1] == PRINT_GRAPHIC)
				{
					j += 2;
				}
			}
			else if (Line[j] == ::CR)
			{
				if (Line[j+1] == ::LF)
				{
					j += 1;
				}
				Lines->Add(TrimmedLine);
				TrimmedLine = "";
			}
			else if (Line[j] == ::LF)
			{
				if (Line[j+1] == ::CR)
				{
					j += 1;
				}
				Lines->Add(TrimmedLine);
				TrimmedLine = "";
			}
			else
			{
				if(Line[j] == char(196))
				{
					TrimmedLine += "_";
				}
				else
				{
					TrimmedLine += Line[j];
				}
			}
		}
		if(TrimmedLine != "")
		{
			Lines->Add(TrimmedLine);
		}
		for (int i=0; i<Lines->Count; i++)
		{
			TFormatLine FormatLine(1);

			FormatLine.WordWrap					= true;
			FormatLine.Column(0).ColSizeMode	= smAbsolute;
			FormatLine.Column(0).ColSize		= Lines->Strings[i].Length();
			FormatLine.Column(0).Text			= Lines->Strings[i];
			AddLine(FormatLine);
		}
	}
	return svUnknown;
}
//---------------------------------------------------------------------------
bool TPrintFormat::fGetNextWord(const AnsiString &Script, AnsiString &Result, bool &ControlWord, int &Index) const
{
	if (Index >= Script.Length() || Index < 0)
	{
		return false;
	}
	Result		= "";
	ControlWord	= false;
	// Skip white space
	while (Index < Script.Length() && Script[Index+1] == ' ')
	{
		Index++;
	}
	if (Script[Index+1] == SCRIPT_1_0_CONTROL[1])
	{
		ControlWord	= true;
		Index++;
	}
	else if (Script[Index+1] == '{')
	{
		ControlWord	= true;
		Result		= "{";
		Index++;
		return true;
	}
	else if (Script[Index+1] == '}')
	{
		ControlWord	= true;
		Result		= "}";
		Index++;
		return true;
	}

	bool Escaped = false;

	while (Index < Script.Length())
	{
		if (!Escaped)
		{
			// Will it be escaped?
			if (Script[Index+1] == SCRIPT_1_0_CONTROL[1])
			{
				if (Index+1 < Script.Length())
				{
					if (Script[Index+2] == SCRIPT_1_0_ESCAPE[1])
					{
						Escaped = true;
						Index += 2;
					}
				}
				// Next word is a control word. All done here.
				if (!Escaped)
				{
					break;
				}
			}
			// Or is it the end of the word?
			else if (Script[Index+1] == SCRIPT_1_0_BEGIN_SECTION[1] || Script[Index+1] == SCRIPT_1_0_END_SECTION[1] ||
						Script[Index+1] == ' ')
			{
				break;
			}
		}
		Result += Script[Index+1];
		++Index;
		Escaped = false;
	}
	return true;
}
//---------------------------------------------------------------------------
AnsiString TPrintFormat::fEscapeText(const AnsiString &Text) const
{
	AnsiString EscapedText(Text);
	int Index = 1;
	while (Index <= EscapedText.Length())
	{
		if (EscapedText[Index] == SCRIPT_1_0_CONTROL[1] ||
			 EscapedText[Index] == SCRIPT_1_0_BEGIN_SECTION[1] || EscapedText[Index] == SCRIPT_1_0_END_SECTION[1] ||
			 EscapedText[Index] == ' ')
		{
			EscapedText.Insert(SCRIPT_1_0_CONTROL + SCRIPT_1_0_ESCAPE, Index);
			Index += 3;
		}
		else
		{
			++Index;
		}
	}
	return EscapedText;
}
//---------------------------------------------------------------------------
AnsiString TPrintFormat::Text(const TPrinterConfig &PrinterConfig) const
{
	std::auto_ptr<TStringList> Output(new TStringList);

	AnsiString LineStr;
	if (PrinterConfig.EmulationMode == emEpson)
	{
		char FontCommand[3] = {ESC, PRINT_MODE, MODE_FONT_A};
		LineStr = AnsiString(FontCommand, 3);
	}
	for (int i=0; i<PrinterConfig.TopMargin; i++)
	{
		Output->Add(LineStr);
	}
	for (unsigned int i=0; i<fLines.size(); i++)
	{
//		AnsiString LineStr = "";	// Concatination of all column strs - Emptied at end of loop

		// If this line is a cut, don't bother checking columns.
		TFormatLine CurrentLine = fLines[i];	// create temp local to calculate absolute sizes in.

		fCalculateAbsoluteWidths(CurrentLine, PrinterConfig);

		if (PrinterConfig.EmulationMode == emEpson && CurrentLine.KitchenBuzzer)
		{
			char KickDraw1[5] = {ESC,OPEN_DRAWER, DRAWER1, 100, 160}; // Check Relay speed.
			char KickDraw2[5] = {ESC,OPEN_DRAWER, DRAWER2, 100, 160}; // Check Relay speed.

			if (PrinterConfig.KitchenBuzzer & kp1)
			{
				Output->Add(AnsiString(KickDraw1, 5));
			}
			if (PrinterConfig.KitchenBuzzer & kp2)
			{
				Output->Add(AnsiString(KickDraw2, 5));
			}
		}
		else if (PrinterConfig.EmulationMode == emEpson && CurrentLine.PrintGraphic)
		{
			char PrintGraphicCommand[3]	= {GS, PRINT_GRAPHIC, DOUBLE_HEIGHT_DOUBLE_WIDTH};
			Output->Add(AnsiString(PrintGraphicCommand, 3));
		}
		else if (PrinterConfig.EmulationMode == emEpson && CurrentLine.Barcode != "")
		{
			char ResetCommand[]	= {ESC,RESET};
			Output->Add(AnsiString(ResetCommand, 2));

			char PrintBarcodeCommand[]	= {GS, BARCODE_HRI_FONT,			CurrentLine.BarcodeProps.HRIFont,
													GS, BARCODE_HRI_POSITION,		CurrentLine.BarcodeProps.HRIPosition,
													GS, BARCODE_HORZ_EXPANSION,	CurrentLine.BarcodeProps.HorzExp,
													GS, BARCODE_AUTO_CHECK_CHAR,	CurrentLine.BarcodeProps.CheckChar?1:0,
													GS, BARCODE_RATIO,				CurrentLine.BarcodeProps.Ratio,
													GS, BARCODE_HEIGHT,				CurrentLine.BarcodeProps.Height,
													GS, BARCODE_PRINT,				CurrentLine.BarcodeProps.Type, CurrentLine.Barcode.Length() + sizeof(CODE_FONT_A)};

			Output->Add(AnsiString(PrintBarcodeCommand, 22) + AnsiString(CODE_FONT_A,2) + CurrentLine.Barcode);
		}
		else if (PrinterConfig.EmulationMode == emEpson && CurrentLine.CutStyle != csNone)
		{
			for (int i=0; i<PrinterConfig.CutterGap; i++)
			{
				Output->Add(LineStr);
			}
			if (PrinterConfig.AlwaysUseFullCut || CurrentLine.CutStyle == csFull)
			{
				char CutCommand[4] = {GS, CUT_MODE, MODE_FULL_CUT, 0};
				Output->Add(AnsiString(CutCommand, 4));
			}
			else if (CurrentLine.CutStyle == csPartial)
			{
				char CutCommand[4] = {GS, CUT_MODE, MODE_PARTIAL_CUT, 0};
				Output->Add(AnsiString(CutCommand, 4));
			}
		}
		else
		{
			// For WordWrap, we start at the first character in each column. We print as much as we can
			// and set this position in the WordWrapPosition array. A column is finished when this position
			// is == to the columns text length. The next print loop then starts from these positions (if
			// there is anything to print) until all columns are printed.
			std::vector<int> WordWrapPosition(CurrentLine.ColCount(), 0);
			bool FininshedLine = false;
			while (!FininshedLine)
			{
				for (unsigned int j=0; j<CurrentLine.ColCount(); j++)
				{
					WideString ColumnStr;
					if (CurrentLine.Column(j).LineStyle == lsNone)
					{
						if (WordWrapPosition[j] < CurrentLine.Column(j).Text.Length())
						{
							int LineLength = CurrentLine.Column(j).Text.Length() - WordWrapPosition[j];
							ColumnStr = CurrentLine.Column(j).Text.SubString(WordWrapPosition[j] + 1, LineLength);
						}
					}
					else
					{
						if (CurrentLine.Column(j).LineStyle == lsSingle)
						{
							ColumnStr = AnsiString::StringOfChar(PrinterConfig.SingleLineChar, CurrentLine.Column(j).ColSize);
						}
						else if (CurrentLine.Column(j).LineStyle == lsDouble)
						{
							ColumnStr = AnsiString::StringOfChar(PrinterConfig.DoubleLineChar, CurrentLine.Column(j).ColSize);
						}
						// Fill this in so that the wrapping code below knows what it's trying to wrap, otherwise, the
						// Text field is random (probably the previous line!)
						CurrentLine.Column(j).Text = ColumnStr;
					}
					int ColWidth = CurrentLine.Column(j).ColSize;

					if (CurrentLine.WordWrap)
					{
						// Check if it fits the column
						if (ColumnStr.Length() <= ColWidth)
						{
							WordWrapPosition[j] += ColumnStr.Length();
						}
						// Check first if it already breaks on a ' ' or \r
						else if (ColumnStr[ColWidth+1] == ' ' || ColumnStr[ColWidth+1] == '\n' || ColumnStr[ColWidth+1] == '\r')
						{
							ColumnStr.SetLength(ColWidth);
							WordWrapPosition[j] += ColWidth + 1;
						}
						else
						{
							ColumnStr.SetLength(ColWidth);
							// Look forwards for a \r or \n to force a break.
							bool FoundBreak = false;
							for (int k=1; k<=ColumnStr.Length(); k++)
							{
								if (ColumnStr[k] == '\n' || ColumnStr[k] == '\r')
								{
									if (k < ColumnStr.Length())
									{
										if ((ColumnStr[k] == '\n' && ColumnStr[k+1] == '\r') ||
											 (ColumnStr[k] == '\r' && ColumnStr[k+1] == '\n'))
										{
											WordWrapPosition[j] += k+1;
										}
										else
										{
											WordWrapPosition[j] += k;
										}
									}
									else
									{
										WordWrapPosition[j] += k;
									}
									ColumnStr.SetLength(k-1);
									FoundBreak = true;
									break;
								}
							}
							// Scroll back, looking for a ' ' or a \r to wrap on. Otherwise, just break the word.
							if (!FoundBreak)
							{
								for (int k=ColumnStr.Length(); k>0; k--)
								{
									if (ColumnStr[k] == ' ' || ColumnStr[k] == '\n' || ColumnStr[k] == '\r')
									{
										ColumnStr.SetLength(k-1);
										WordWrapPosition[j] += k;
										FoundBreak = true;
										break;
									}
								}
								// Solid letters. Just break it.
								if (!FoundBreak)
								{
									WordWrapPosition[j] += ColWidth;
								}
							}
						}
					}
					else
					{
						if (ColumnStr.Length() > ColWidth)
						{
							ColumnStr.SetLength(ColWidth);
						}
						if (ColumnStr.Pos(WideString('\n')) > 0)
						{
							ColumnStr.SetLength(ColumnStr.Pos(WideString('\n'))-1);
						}
						if (ColumnStr.Pos(WideString('\r')) > 0)
						{
							ColumnStr.SetLength(ColumnStr.Pos(WideString('\r'))-1);
						}
					}
					if (CurrentLine.Column(j).Alignment == taLeftJustify)
					{
                  if((ColWidth - ColumnStr.Length()) > 0)
                  {
							ColumnStr = ColumnStr + AnsiString::StringOfChar(' ', ColWidth - ColumnStr.Length());
                  }
					}
					else if (CurrentLine.Column(j).Alignment == taRightJustify)
					{
						if((ColWidth - ColumnStr.Length()) > 0)
                  {
							ColumnStr = AnsiString::StringOfChar(' ', ColWidth - ColumnStr.Length()) + ColumnStr;
                  }
					}
					else if (CurrentLine.Column(j).Alignment == taCenter)
					{
						if((ColWidth - ColumnStr.Length()) > 0)
                  {
                     int FirstGap, LastGap;
                     FirstGap = (ColWidth - ColumnStr.Length()) / 2;
                     LastGap = ColWidth - FirstGap - ColumnStr.Length();
							ColumnStr = AnsiString::StringOfChar(' ', FirstGap) + ColumnStr + AnsiString::StringOfChar(' ', LastGap);
                  }
					}
					else
					{
						ColumnStr = "";
					}

					if (CurrentLine.Column(j).Kanji && PrinterConfig.EmulationMode == emEpson && PrinterConfig.KanjiCodeSystem != kcNone)
					{
						char KanjiOn[6]		= {FS, KANJI_CODE, KANJI_SJIS, FS, KANJI_MODE_ON};
						if (PrinterConfig.KanjiCodeSystem == kcJIS)
						{
							KanjiOn[2]			= KANJI_JIS;
						}
						else if (PrinterConfig.KanjiCodeSystem == kcSJIS)
						{
							KanjiOn[2]			= KANJI_SJIS;
						}
						char KanjiOff[3]		= {FS, KANJI_MODE_OFF};
						AnsiString Chars		= AnsiString(KanjiOn, 5);
						int BufferSize			= WideCharToMultiByte(PrinterConfig.KanjiCodePage, 0, ColumnStr.c_bstr(), ColumnStr.Length(), NULL, 0, NULL, NULL);
						char *dest				= new char[BufferSize];
						WideCharToMultiByte(PrinterConfig.KanjiCodePage, 0, ColumnStr.c_bstr(), ColumnStr.Length(), dest, BufferSize, NULL, NULL);
						Chars						= Chars + AnsiString(dest, BufferSize);
						delete					[] dest;
						Chars						= Chars + AnsiString(KanjiOff, 2);

						LineStr					+= Chars;
					}
					else
					{
						LineStr += ColumnStr;
					}
				}
				if (PrinterConfig.EmulationMode == emEpson)
				{
					char ModeChar = MODE_FONT_A;

					if(CurrentLine.ColumnFont.Font == ftFontB)
					{
						ModeChar |= MODE_FONT_B;
					}
					if (CurrentLine.ColumnFont.Height == fsDoubleSize)
					{
						ModeChar |= MODE_DOUBLE_HEIGHT;
					}
					if (CurrentLine.ColumnFont.Width == fsDoubleSize)
					{
						ModeChar |= MODE_DOUBLE_WIDTH;
					}
					if (CurrentLine.ColumnFont.Bold)
					{
						ModeChar |= MODE_BOLD;
					}
					if (CurrentLine.ColumnFont.Underlined)
					{
						ModeChar |= MODE_UNDERLINE;
					}

					char FontCommand[3] = {ESC, PRINT_MODE, ModeChar};
					LineStr = AnsiString(FontCommand, 3) + LineStr;

					char ColourMode = MODE_COLOUR_OFF;
					if (CurrentLine.ColumnFont.Colour == fcRed)
					{
						ColourMode |= MODE_COLOUR_ON;
					}
					char ColourCommand[3] = {ESC, COLOUR_MODE, ColourMode};
					LineStr = AnsiString(ColourCommand, 3) + LineStr;

				}
				Output->Add(LineStr);
				LineStr = "";
				if (CurrentLine.WordWrap)
				{
					// This checks to see that we have printed all text for all columns.
					FininshedLine = true;
					for (unsigned int j=0; j<CurrentLine.ColCount(); j++)
					{
						if (WordWrapPosition[j] < CurrentLine.Column(j).Text.Length())
						{
							FininshedLine = false;
						}
					}
				}
				else
				{
					FininshedLine = true;
				}
			}
		}
		LineStr = "";	// Concatination of all column strs - ready for next line
	}
	return Output->Text;
}
//---------------------------------------------------------------------------
void TPrintFormat::fCalculateAbsoluteWidths(TFormatLine &CurrentLine, const TPrinterConfig &PrinterConfig) const
{
	if (CurrentLine.ColCount() == 0)
	{
		return;
	}
	// Determine the absolute char width of the printer for this font
	int Width = 0;
	if (CurrentLine.ColumnFont.Font == ftFontA)
	{
		if (!CurrentLine.ColumnFont.Bold)
		{
			Width = PrinterConfig.FontAWidth;
		}
		else
		{
			Width = PrinterConfig.FontABoldWidth;
		}
	}
	else if (CurrentLine.ColumnFont.Font == ftFontB)
	{
		if (!CurrentLine.ColumnFont.Bold)
		{
			Width = PrinterConfig.FontBWidth;
		}
		else
		{
			Width = PrinterConfig.FontBBoldWidth;
		}
	}
	if (Width == 0)
	{
		for (unsigned int k=0; k<CurrentLine.ColCount(); k++)
		{
			CurrentLine.Column(k).ColSize			= 0;
			CurrentLine.Column(k).ColSizeMode	= smAbsolute;
			CurrentLine.Column(k).Text				= "";
		}
		return;
	}
	// Set autosizing columns
	unsigned int PercentCount	= 0;
	unsigned int AbsoluteCount	= 0;
	unsigned int MaximumCount	= 0;

	unsigned int PercentTotal	= 0;
	unsigned int AbsoluteTotal	= 0;

	for (unsigned int k=0; k<CurrentLine.ColCount(); k++)
	{
		if (CurrentLine.Column(k).ColSizeMode == smPercent)	{ ++PercentCount; PercentTotal += CurrentLine.Column(k).ColSize; }
		if (CurrentLine.Column(k).ColSizeMode == smAbsolute)	{ ++AbsoluteCount; AbsoluteTotal += CurrentLine.Column(k).ColSize; }
		if (CurrentLine.Column(k).ColSizeMode == smMaximum)	++MaximumCount;
	}
	// Check for no auto sizing column. Actually want width to be full.
	if (PercentCount == CurrentLine.ColCount() && PercentTotal == 100)
	{
		CurrentLine.Column(CurrentLine.ColCount() / 2).ColSizeMode = smMaximum;
	}
	// More than 1 autosizing column. Convert all but 1 to percent.
	if (MaximumCount > 1)
	{
		int PercentEach = ((100 - PercentTotal) - (AbsoluteTotal / Width)) / MaximumCount;
		for (unsigned int i=0; i<MaximumCount - 1; i++)
		{
			for (unsigned int k=0; k<CurrentLine.ColCount(); k++)
			{
				if (CurrentLine.Column(k).ColSizeMode == smMaximum)
				{
					CurrentLine.Column(CurrentLine.ColCount() / 2).ColSize		= PercentEach;
					CurrentLine.Column(CurrentLine.ColCount() / 2).ColSizeMode	= smPercent;
					break;
				}
			}
		}
	}
	// Loop thru columns to calculate widths.
	int UnusedWidth = Width;
	for (unsigned int k=0; k<CurrentLine.ColCount(); k++)
	{
		if (CurrentLine.Column(k).ColSizeMode == smPercent)
		{
			CurrentLine.Column(k).ColSize = Width * CurrentLine.Column(k).ColSize / 100;

			CurrentLine.Column(k).ColSizeMode = smAbsolute;

			if (CurrentLine.Column(k).ColSize == 0)
			{
				CurrentLine.Column(k).Text = "";
			}
		}
		if (CurrentLine.Column(k).ColSizeMode == smAbsolute)
		{
			UnusedWidth -= CurrentLine.Column(k).ColSize;
		}
	}
	if (UnusedWidth < 0)
	{
		UnusedWidth = 0;
	}
	for (unsigned int k=0; k<CurrentLine.ColCount(); k++)
	{
		if (CurrentLine.Column(k).ColSizeMode == smMaximum)
		{
			CurrentLine.Column(k).ColSize			= UnusedWidth;
			CurrentLine.Column(k).ColSizeMode	= smAbsolute;
			if (CurrentLine.Column(k).ColSize == 0)
			{
				CurrentLine.Column(k).Text = "";
			}
		}
	}
	if (!CurrentLine.WordWrap)
	{
		int RowWidth = 0;
		unsigned int ColCount = CurrentLine.ColCount();
		for (unsigned int k=0; k<ColCount; k++)
		{
			int ColWidth = CurrentLine.Column(k).ColSize;
			RowWidth += ColWidth;
		}
		int i = CurrentLine.ColCount()-1;
		while (RowWidth > Width && i >= 0)
		{
			int TrimBy = RowWidth - Width;
			int CurrentColLength = CurrentLine.Column(i).ColSize;
			if (TrimBy >= CurrentColLength)
			{
				CurrentLine.Column(i).Text = "";
				CurrentLine.Column(i).ColSize = 0;
				RowWidth -= CurrentColLength;
			}
			else
			{
				CurrentLine.Column(i).ColSize = CurrentColLength - TrimBy;
				break;
			}
			--i;
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TFormatLine::TFormatLine(unsigned int ColumnCount, TColumnFont Font) :
	fColumns(ColumnCount, TFormatColumn()),
	ColumnFont(Font),
	CutStyle(csNone),
	PrintGraphic(false),
	KitchenBuzzer(false),
	WordWrap(false)
{
	for (unsigned int i=0; i<ColumnCount; i++)
	{
		fColumns[i].ColSize = 100 / ColumnCount;
	}
}
//---------------------------------------------------------------------------
TFormatColumn &TFormatLine::Column(unsigned int ACol)
{
	if (ACol < fColumns.size())
	{
		return fColumns[ACol];
	}
	else
	{
		throw EPrintFormatParam("Column out of range: " + IntToStr(ACol));
	}
}
//---------------------------------------------------------------------------
unsigned int TFormatLine::ColCount() const
{
	return fColumns.size();
}
//---------------------------------------------------------------------------
int TFormatLine::AddColumn(const TFormatColumn &Column)
{
	fColumns.push_back(Column);
	return fColumns.size() - 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TFormatColumn::TFormatColumn() :
		ColSizeMode(smPercent),
		ColSize(100),
		Alignment(taLeftJustify),
		Kanji(false),
		LineStyle(lsNone)
//		LineSizeMode(smPercent),
//		LineSize(100)
{
}
//---------------------------------------------------------------------------

