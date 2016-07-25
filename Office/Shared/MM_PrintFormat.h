//---------------------------------------------------------------------------
#ifndef MM_PrintFormatH
#define MM_PrintFormatH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <Classes.hpp>
#include <vector>
//---------------------------------------------------------------------------
namespace Printing
{
//---------------------------------------------------------------------------
enum TScriptVersion			{ svUnknown, sv1_0																									};
//---------------------------------------------------------------------------
enum TEmulationMode			{ emTextOnly, emEpson, emStar																						};
//---------------------------------------------------------------------------
//AnsiString ConvertPrintFormat(TEmulationMode, AnsiString Script);
//---------------------------------------------------------------------------
enum TTextEncoding			{ teASCII, teUTF8																										};
enum TLineStyle				{ lsNone, lsSingle, lsDouble																						};
enum TSizeMode					{ smAbsolute, smPercent, smMaximum																				};
enum TCutStyle					{ csNone, csPartial, csFull																						};
enum TFontSize					{ fsNormalSize, fsDoubleSize																						};
enum TFontColour				{ fcBlack, fcRed																										};
enum TFontType					{ ftFontA, ftFontB																									};

enum TBarcodeHRIPosition	{ bpNone, bpAbove, bpBelow, bpBoth																				};
enum TBarcodeHRIFont			{ bcFontA, bcFontB																									};
enum TBarcodeRatio			{ bcRatio2_1, bcRatio3_1, bcRatio5_2																			};
enum TBarcodeType				{ btUPC_A, btUPC_E, btEAN13, btEAN8, btCode39, btInterleaved, btCodaBar, btCode93, btCode128	};
//---------------------------------------------------------------------------
class EPrintFormatParam : public Exception
{
public:
	__fastcall				EPrintFormatParam(const AnsiString Msg)			: Exception(Msg) {}
	__fastcall				EPrintFormatParam(int Ident)							: Exception(Ident) { }
	__fastcall				EPrintFormatParam(int Ident, int AHelpContext)	: Exception(Ident, AHelpContext) { }
};
//---------------------------------------------------------------------------
enum TKanjiCodeSystem		{ kcNone, kcJIS, kcSJIS																								};
enum TDrawerKickPort			{ kpNone=0x00, kp1=0x01, kp2=0x02, kpAll=0xFF																};
//---------------------------------------------------------------------------
struct TPrinterConfig
{
	TPrinterConfig();
	TEmulationMode			EmulationMode;
	int						TopMargin;
	int						CutterGap;
	bool						AlwaysUseFullCut;
	unsigned char			SingleLineChar;
	unsigned char			DoubleLineChar;
	int						FontAWidth;
	int						FontABoldWidth;
	int						FontBWidth;
	int						FontBBoldWidth;
	TKanjiCodeSystem		KanjiCodeSystem;						// Use KANJI_JIS or KANJI_SJIS
	int						KanjiCodePage;							// 936 seems to work (used in WideCharToMultiByte)
	TDrawerKickPort		DrawerKickPort;
	TDrawerKickPort		KitchenBuzzer;
};
//---------------------------------------------------------------------------
struct TColumnFont
{
	TColumnFont();

	bool						Underlined;
	bool						Bold;
	TFontSize				Width;
	TFontSize 				Height;
	TFontType				Font;
	TFontColour				Colour;
};
//---------------------------------------------------------------------------
struct TBarcodeProperties
{
	TBarcodeProperties();

	TBarcodeHRIPosition	HRIPosition;	// HRI - Human Readable Interpretation. Where to put text.
	TBarcodeHRIFont		HRIFont;
	bool						CheckChar;
	TBarcodeRatio			Ratio;
	int						Height;
	int						HorzExp;
	TBarcodeType			Type;
};
//---------------------------------------------------------------------------
class TFormatLine;
class TPrintFormat
{
public:
	TPrintFormat();

	TTextEncoding					TextEncoding;

	void								AddLine(const TFormatLine &Line);

	void								DrawLine(TLineStyle LineStyle);
	void								DrawLine(TLineStyle LineStyle, int AbsoluteWidth);
	void								Cut(TCutStyle CutStyle);
	void								AddBarcode(AnsiString Barcode);
	void								PrintGraphic();	// Assumes you've already loaded it.
	void								KitchenBuzzer();	// By using the cash drawer kick. BEWARE! Will store in script.

	AnsiString						Text(const TPrinterConfig &PrinterConfig) const;

	TScriptVersion					LoadScript(AnsiString Script);
	AnsiString						GetScript();

	static AnsiString				OpenDrawerText(const TPrinterConfig &PrinterConfig);
private:
	std::vector<TFormatLine>	fLines;
	AnsiString						fEscapeText(const AnsiString &Text) const;
	void								fCalculateAbsoluteWidths(TFormatLine &CurrentLine, const TPrinterConfig &PrinterConfig) const;

	bool								DownloadGraphic;
	AnsiString						fGraphicFileName;

	bool								fGetNextWord(const AnsiString &Script, AnsiString &Result, bool &ControlWord, int &Index) const;
};
//---------------------------------------------------------------------------
class TFormatColumn;
class TFormatLine
{
public:
	TFormatLine(unsigned int ColumnCount, TColumnFont Font=TColumnFont());

	unsigned int					ColCount() const;
	TFormatColumn					&Column(unsigned int ACol);

	TColumnFont						ColumnFont;
	TCutStyle                  CutStyle;
	bool								PrintGraphic;
	bool								KitchenBuzzer;
	AnsiString						Barcode;
	TBarcodeProperties			BarcodeProps;
	bool								WordWrap;

	int								AddColumn(const TFormatColumn &Column);	// Use the constructor to create these instead.

private:
	std::vector<AnsiString>		fControlWords;
	std::vector<TFormatColumn>	fColumns;
};
//---------------------------------------------------------------------------
class TFormatColumn
{
public:
	TFormatColumn();

	TSizeMode						ColSizeMode;
	unsigned int					ColSize;
	
	TAlignment						Alignment;
	WideString						Text;

	bool								Kanji;
	TLineStyle						LineStyle;
//	TSizeMode						LineSizeMode;
//	unsigned int					LineSize;

private:
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
