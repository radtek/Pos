// ---------------------------------------------------------------------------
#ifndef PrintFormatH
#define PrintFormatH

#include "PrintInfo.h"
#include "GraphicPrinterSettings.h"
#include "PrintInfo.h"

#include "PrinterPhysical.h"
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// EPSON PRINTER CONTROL LANGUAGE COMMANDS
// ---------------------------------------------------------------------------
// Prefixes for most of the following commands
const char ESC = 0x1B;
const char FS = 0x1C;
const char GS = 0x1D;
// ---------------------------------------------------------------------------
// Doesn't reset the downloaded graphic.
const char RESET = 0x40; // '@' Use ESC @
// ---------------------------------------------------------------------------
const char OPEN_DRAWER = 0x70; // 'p'
const char DRAWER1 = 0x00; // Drawer 1
const char DRAWER2 = 0x01; // Drawer 2
// ---------------------------------------------------------------------------
// Chars
// ---------------------------------------------------------------------------
// Characters for drawing a single or double line using Epson fonts.
const char FONT_SOLID_LINE_EPSON = 0xC4;
const char FONT_DOUBLE_LINE_EPSON = 0xCD;

// ---------------------------------------------------------------------------
const char CR = 0x0D; // Carriage Return
const char LF = 0x0A; // Line Feed
const char FF = 0x0C; // Form Feed
// ---------------------------------------------------------------------------
// Font Commands
// ---------------------------------------------------------------------------
// Font size selector.
const char PRINT_MODE = 0x21; // '!'
// The mode bits constitute the 3rd part of the Font Command (ESC !) sequence
const char MODE_FONT_A = 0x00;
const char MODE_FONT_B = 0x01;
const char MODE_BOLD = 0x08;
const char MODE_DOUBLE_HEIGHT = 0x10;
const char MODE_DOUBLE_WIDTH = 0x20;
const char MODE_UNDERLINE = 0x80;

// ---------------------------------------------------------------------------
// Colour Commands
// ---------------------------------------------------------------------------
// Font size selector.
const char COLOUR_MODE = 0x72; // '!'
// The mode bits constitute the 3rd part of the Font Command (ESC !) sequence
const char MODE_COLOUR_OFF = 0x00;
const char MODE_COLOUR_ON = 0x01;

// ---------------------------------------------------------------------------
// Bar Code Font Commands
// ---------------------------------------------------------------------------
const char CODE_FONT_A[] =
{
   0x7B, 0x41
}; // {A
const char CODE_FONT_B[] =
{
   0x7B, 0x42
}; // {B
const char CODE_FONT_C[] =
{
   0x7B, 0x43
}; // {C
// ---------------------------------------------------------------------------
// Cut commands
// ---------------------------------------------------------------------------
const char CUT_MODE = 0x56; // 'V'
// The mode bits constitute the 3rd part of the Cut Command (GS V) sequence
// Follow with 0x00 -> 0xFF to scroll before cut
const char MODE_FULL_CUT = 0x41;
const char MODE_PARTIAL_CUT = 0x42;
// ---------------------------------------------------------------------------
// Graphic commands
// ---------------------------------------------------------------------------
const char DOWNLOAD_GRAPHIC = 0x2A; // '*'
// Two bytes following are Width/8 (m) and and Height/8 (n). ie (GS '*' m n)
// followed by m*n*8 data bytes. Thes are aranged vertically, where each
// pixel corresponds to a bit. MSB at the top, LSB at the bottom, followed
// by the rest of n bytes. Then back up to the top for the n+1 byte at the
// m=1 pos.
// Max m = 72. Max n = 48. Max m*n = 1536 for Axiohm A715
const char PRINT_GRAPHIC = 0x2F; // '/'
// Prints the graphic stored by DOWNLOAD_GRAPHIC in the following fashon.
// Use (GS, '/', Width) where Width is one of the following.
const char SINGLE_HEIGHT_SINGLE_WIDTH = 0x00; // 0x30
const char SINGLE_HEIGHT_DOUBLE_WIDTH = 0x01; // 0x31
const char DOUBLE_HEIGHT_SINGLE_WIDTH = 0x02; // 0x32
const char DOUBLE_HEIGHT_DOUBLE_WIDTH = 0x03; // 0x33
// ---------------------------------------------------------------------------
// Barcode commands
// ---------------------------------------------------------------------------
const char BARCODE_HRI_POSITION = 0x48; // 'H'
// Human Readable Interpretation. (GS 'H' n) where n is 1 of the 4 following
// positions.
const char BARCODE_HRI_POSITION_NONE = 0x00;
const char BARCODE_HRI_POSITION_ABOVE = 0x01;
const char BARCODE_HRI_POSITION_BELOW = 0x02;
const char BARCODE_HRI_POSITION_BOTH = 0x03;

const char BARCODE_HRI_FONT = 0x66; // 'f'
// Font for Human Readable Interpretation. (GS 'f' n) where n is 1 of the 2
// font types.
const char BARCODE_HRI_FONT_A = 0x00;
const char BARCODE_HRI_FONT_B = 0x01;

const char BARCODE_RATIO = 0x67; // 'g'
// Bar code width (raito of Wide:Narrow) (GS 'g' n) where n is 1 of the 3
// font types.
const char BARCODE_RATION_2_1 = 0x00;
const char BARCODE_RATION_3_1 = 0x01;
const char BARCODE_RATION_5_2 = 0x02;

const char BARCODE_AUTO_CHECK_CHAR = 0x63; // 'c'

const char BARCODE_HEIGHT = 0x68; // 'h'
// Vertical height in dots. (GS 'h' n) where n = 00 to FF
const char BARCODE_PRINT = 0x6B; // 'k'
// Using option 2. (GS 'k' m n {d1 d2 d3...dn})
// n = number of data characters
// d is bar code data
// m is one of the below bar code types.
const char BARCODE_TYPE_UPC_A = 0x41; // 11 to 12 chars, 48 to 57
const char BARCODE_TYPE_UPC_E = 0x42; // 11 to 12 chars, 48 to 57
const char BARCODE_TYPE_EAN13 = 0x43; // 12 to 13 chars, 48 to 57
const char BARCODE_TYPE_EAN8 = 0x44; // 7 to 8 chars, 48 to 57
const char BARCODE_TYPE_CODE_39 = 0x45; // 1 to 255 chars, 48 to 57, 65 to 90, 32, 36, 37, 43, 45, 46, 47
const char BARCODE_TYPE_INTERLEAVED = 0x46; // 1 to 255 chars, 48 to 57 (even # chars)
const char BARCODE_TYPE_CODABAR = 0x47; // 1 to 255 chars, 48 to 57, 65 to 68, 36, 43, 45, 46, 47, 58
const char BARCODE_TYPE_CODE_93 = 0x48; // 1 to 255 chars
const char BARCODE_TYPE_CODE_128 = 0x49; // 2 to 255 chars

const char BARCODE_HORZ_EXPANSION = 0x77; // 'w'
// Horizontal size. (GS 'w' n) where n = 01 to 04
// ---------------------------------------------------------------------------
// Kanji Commands
// ---------------------------------------------------------------------------
const char KANJI_CODE = 0x43; // 'C'
// Kanji code system. (FS 'C' n) where n = (00 or 48) or (01 or 49)
// In JIS mode,
// Prmary byte:		<21>H to <7E>H
// Secondary byte:	<21>H to <7E>H
// In SHIFT JIS mode,
// Prmary byte:	<81>H to <9F>H and <E0>H to <EF>H
// Secondary byte:	<40>H to <7E>H and <80>H to <FC>H
const char KANJI_JIS = 0x00;
const char KANJI_SJIS = 0x01;

const char KANJI_MODE_ON = 0x26; // '&'
// Select Kanji mode. (FS '&')
const char KANJI_MODE_OFF = 0x2E; // '.'
// Select Kanji mode. (FS '.')
// ---------------------------------------------------------------------------
// STAR PRINTER CONTROL LANGUAGE COMMANDS
// ---------------------------------------------------------------------------
const char STAR_CUT_MODE = 0x64; // 'd'
// The mode bits constitute the 3rd part of the Cut Command (GS V) sequence
// Follow with 0x00 -> 0xFF to scroll before cut
// ASCII     ESC   d     n
// Hex.      1B    64    n
// Decimal   27    100   n

const char STAR_MODE_FULL_CUT = 0x00;
const char STAR_MODE_PARTIAL_CUT = 0x01;
const char STAR_OPEN_DRAWER = 0x07; // '@' Use ESC @
const char STAR_UNDERLINE = 0x2D;
const char STAR_FONT_NORMAL_HEIGHT = 0x14;
const char STAR_FONT_DOUBLE_HEIGHT = 0x0E;
const char STAR_FONT_NORMAL_WIDTH = 0x14; // Same as height but not proceed by an ESC
const char STAR_FONT_DOUBLE_WIDTH = 0x0E;
const char STAR_FONT_A = 'P';
const char STAR_FONT_B = 'M';
const char STAR_FONT_INTENSITY_NORMAL = 'F';
const char STAR_FONT_INTENSITY_BOLD = 'E';

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
enum TLineStyle
{
   lsNone, lsSingle, lsDouble
};

// ---------------------------------------------------------------------------
class TFormatLine;
class TFormatColumn;
class TConnectPrinter;

class TPrintFormat
{
private:
   // TStringList *		GetTextWideString();
   void GetScriptText();
   void GetFormattedText();
   void GetFormattedTextWideString();

   int GetWidth(); // Gets the width of the current paragraph according to its font
   TFormatLine * DuplicateLine();
   // Creates a copy of Line
   TList* Lines; // List of TFormatParagraphs
   TStringList* fLines; // Contains formatted text.
   // TWideStringList*	wLines;									// Contains formatted WideString text.

   // DWORD 				SendSerialPrintStream(UnicodeString PrinterName, char *Data, int Size);
public:
   __fastcall TPrintFormat();
   __fastcall ~TPrintFormat();

   TGraphicPrinterSettings GraphicPrinterSettings;

   // These 4 font widths need to be set initially.
   TStringList* GetText();
   // Calls one of the below depending on EmulationMode
   int NormalWidth; // Width of page in number of chars for normal font
   int BoldWidth; // Width of page in number of chars for bold font
   int FontBWidth; // Width of page in number of chars for B Font
   int BoldFontBWidth; // Width of page in number of chars for B Font in bold

   __property int Width =
   {
	  read = GetWidth
   };
   // Width of the current paragraph. Returns one of the above.
   int TopMargin; // Scroll before print 0x00->0xFF
   int CutterGap; // Scroll before cut 0x00->0xFF
   bool AlwaysUseFullCut; // Newer Epson printers only use full cut command.
   bool KanjiPrinter; // Printer use Kanji Character Code Pages

   int BarcodeHRIPosition;
   int BarcodeHRIFont;
   bool BarcodeCheckChar;
   int BarcodeRatio;
   int BarcodeHeight;
   int BarcodeHorzExp;
   int BarcodeType;

   /* The only ANSI true MBCS are 932 (Japanese), 936 (Simplified Chinese),
   949 (Korean) and 950 (Traditional Chinese).
   All of them use maximum 2 bytes, so they are also called DBCS (Double Byte
   Character Set). An example of MBCS that is not DBCS is GB 18030 (which cannot
   be ANSI code page). */

   int PrinterCodePage; // 936 seems to work (used in WideCharToMultiByte)
   UnicodeString PrinterInitCmd;
   UnicodeString PrinterCodePageOn;
   UnicodeString PrinterCodePageOff;

   int KanjiCodeSystem; // Use KANJI_JIS or KANJI_SJIS

   char SolidLineChar;
   char DoubleLineChar;

   TFormatLine * Line;
   // Format for next line to be added
   void AddLine(); // Call after setting Colums text.
   void Add(UnicodeString Line); // Alternative for adding text. '|' marks columns
   void AddBarcode(UnicodeString Code);

   void NewLine();
   void DrawLine();
   void Cut();
   void OpenDrawer();
   bool OpenDrawer(UnicodeString PrinterName);
   void PartialCut();
   void DownloadGraphic(UnicodeString FileName, UnicodeString PrinterName);
   void PrintGraphic(); // Assumes you've already loaded it.

   // Used to Undo Pritner Commands back to the bookmark.
   int BookMarkIndex;

   void BookMark();
   void Undo();

   TEmulationMode EmulationMode;
   bool WordWrap; // Wrap lines within their columns

   UnicodeString DocumentName; // Title of windows print job

   bool Print(UnicodeString Printer, UnicodeString DocName);
   bool Print(UnicodeString PrinterName, UnicodeString DocName, AnsiString PrintData,bool PrintCompanyDetailsToPrinter = false);
};

// ---------------------------------------------------------------------------
class TFormatLine : public TObject
{
   friend TPrintFormat;

private:
   int fColCount;

   void SetColCount(int Count);
   // Adjusts Columns to the size of ColCount and sets fColCount
   TList * fColumns;
   // List of TFormatColumn
   TFormatColumn * GetColumn(int ACol)
   {
	  return(TFormatColumn*)fColumns->Items[ACol];
   }

   bool PrintGraphic; // Set by calling PrintGraphic() for the TPrintFormat

   int BarcodeHRIPosition;
   int BarcodeHRIFont;
   bool BarcodeCheckChar;
   int BarcodeRatio;
   int BarcodeHeight;
   int BarcodeHorzExp;
   int BarcodeType;

public:
   __fastcall TFormatLine();
   __fastcall ~TFormatLine();
   __property int ColCount =
   {
	  read = fColCount, write = SetColCount
   };
   __property TFormatColumn * Columns[int ACol] =
   {
	  read = GetColumn
   };

   TPrintInfo FontInfo;

   bool Cut; // Over-rides everything else. Needs to be private
   bool PartialCut; // Over-rides everything else. Needs to be private
   bool Barcode; // Over-rides everything else. Needs to be private
   bool OpenDrawer; // Over-rides everything else. Needs to be private
};

// ---------------------------------------------------------------------------
class TFormatColumn
{
   friend TPrintFormat;

private:
   TLineStyle LineStyle;
   int LineLength;

public:
   TFormatColumn() : Alignment(taLeftJustify), Width(0), LineStyle(lsNone), LineLength(0), Kanji(false), UTF8(false)
   {
   }

   int Width;
   TAlignment Alignment;
   WideString Text;

   bool Kanji;
   bool UTF8;

   void Line()
   {
	  LineStyle = lsSingle;
	  LineLength = Width;
   }

   void Line(int Length)
   {
	  LineStyle = lsSingle;
	  if (Length < 0)
		 LineLength = 0;
	  else
		 LineLength = Length;
   }

   void DoubleLine()
   {
	  LineStyle = lsDouble;
	  LineLength = Width;
   }

   void DoubleLine(int Length)
   {
	  LineStyle = lsDouble;
	  if (Length < 0)
		 LineLength = 0;
	  else
		 LineLength = Length;
   }

};
// ---------------------------------------------------------------------------
#endif
