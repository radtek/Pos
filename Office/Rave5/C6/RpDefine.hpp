// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpDefine.pas' rev: 6.00

#ifndef RpDefineHPP
#define RpDefineHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Math.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------
#define BoldOn "\x01\x42\x31\x04"
#define BoldOff "\x01\x42\x30\x04"
#define ItalicOn "\x01\x49\x31\x04"
#define ItalicOff "\x01\x49\x30\x04"
#define UnderlineOn "\x01\x55\x31\x04"
#define UnderlineOff "\x01\x55\x30\x04"
#define StrikeoutOn "\x01\x4B\x31\x04"
#define StrikeoutOff "\x01\x4B\x30\x04"
#define SuperscriptOn "\x01\x50\x31\x04"
#define SuperscriptOff "\x01\x50\x30\x04"
#define SubscriptOn "\x01\x53\x31\x04"
#define SubscriptOff "\x01\x53\x30\x04"
#define JustifyLeft "\x01\x4A\x4C\x04"
#define JustifyCenter "\x01\x4A\x43\x04"
#define JustifyRight "\x01\x4A\x52\x04"
#define JustifyBlock "\x01\x4A\x42\x04"
#undef ResetPrinter

namespace Rpdefine
{
//-- type declarations -------------------------------------------------------
typedef int TRavePaperSize;

struct TPaperSizeName
{
	AnsiString Name;
	TRavePaperSize PaperSize;
	double PaperWidth;
	double PaperHeight;
} ;

typedef TPaperSizeName RpDefine__1[43];

class DELPHICLASS EReportPrinter;
class PASCALIMPLEMENTATION EReportPrinter : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EReportPrinter(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EReportPrinter(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EReportPrinter(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EReportPrinter(int Ident, const System::TVarRec * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EReportPrinter(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EReportPrinter(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EReportPrinter(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EReportPrinter(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EReportPrinter(void) { }
	#pragma option pop
	
};


typedef bool __fastcall (__closure *TPrintPageEvent)(System::TObject* Sender, int &PageNum);

typedef void __fastcall (__closure *TDecodeImageEvent)(System::TObject* Sender, Classes::TStream* ImageStream, AnsiString ImageType, Graphics::TBitmap* Bitmap);

typedef AnsiString TVersion;

typedef AnsiString TTitleString;

typedef AnsiString TFormatString;

#pragma option push -b-
enum TDeviceState { dsNone, dsIC, dsDC };
#pragma option pop

#pragma option push -b-
enum TAccuracyMethod { amPositioning, amAppearance };
#pragma option pop

#pragma option push -b-
enum TMarginMethod { mmScaled, mmFixed };
#pragma option pop

#pragma option push -b-
enum TRulerType { rtNone, rtHorizCm, rtVertCm, rtBothCm, rtHorizIn, rtVertIn, rtBothIn };
#pragma option pop

#pragma option push -b-
enum TPrintJustify { pjCenter, pjLeft, pjRight, pjBlock };
#pragma option pop

#pragma option push -b-
enum TTabJustify { tjCenter, tjLeft, tjRight, tjBlock, tjNone };
#pragma option pop

#pragma option push -b-
enum TPrintJustifyVert { pjTop, pjMiddle, pjBottom };
#pragma option pop

#pragma option push -b-
enum TPrintUnits { unInch, unMM, unCM, unPoint, unUser };
#pragma option pop

#pragma option push -b-
enum TLineHeightMethod { lhmLinesPerInch, lhmFont, lhmUser };
#pragma option pop

#pragma option push -b-
enum TBKMode { bkTransparent, bkOpaque };
#pragma option pop

#pragma option push -b-
enum TDestination { destPrinter, destPreview, destFile, destNone, destCustomFile };
#pragma option pop

#pragma option push -b-
enum TStreamMode { smMemory, smFile, smUser, smTempFile };
#pragma option pop

#pragma option push -b-
enum TOrientation { poPortrait, poLandScape, poDefault };
#pragma option pop

#pragma option push -b-
enum TDuplex { dupSimplex, dupVertical, dupHorizontal };
#pragma option pop

#pragma option push -b-
enum TReportEventType { reBeginPage, reEndPage };
#pragma option pop

#pragma option push -b-
enum TFontAlign { faBaseline, faTop, faBottom };
#pragma option pop

#pragma option push -b-
enum TReportDest { rdPreview, rdPrinter, rdFile };
#pragma option pop

#pragma option push -b-
enum TSystemSetup { ssAllowSetup, ssAllowCopies, ssAllowCollate, ssAllowDuplex, ssAllowDestPreview, ssAllowDestPrinter, ssAllowDestFile, ssAllowPrinterSetup, ssAllowPreviewSetup };
#pragma option pop

#pragma option push -b-
enum TSystemOption { soUseFiler, soWaitForOK, soShowStatus, soAllowPrintFromPreview, soPreviewModal, soNoGenerate };
#pragma option pop

#pragma option push -b-
enum TBoxLines { blNone, blLeft, blRight, blLeftRight, blTop, blLeftTop, blRightTop, blNoBottom, blBottom, blLeftBottom, blRightBottom, blNoTop, blTopBottom, blNoRight, blNoLeft, blAll };
#pragma option pop

#pragma option push -b-
enum TMacroID { midCurrDateShort, midCurrDateLong, midCurrDateUS, midCurrDateInter, midCurrTimeShort, midCurrTimeLong, midCurrTimeAMPM, midCurrTime24, midFirstPage, midLastPage, midTotalPages, midCurrentPage, midRelativePage, midPrinterName, midDriverName, midPortName, midUser01, midUser02, midUser03, midUser04, midUser05, midUser06, midUser07, midUser08, midUser09, midUser10, midUser11, midUser12, midUser13, midUser14, midUser15, midUser16, midUser17, midUser18, midUser19, midUser20 };
#pragma option pop

#pragma option push -b-
enum TRTFTokenType { ttControlWord, ttControlSymbol, ttGroupBegin, ttGroupEnd, ttText };
#pragma option pop

typedef Set<TRTFTokenType, ttControlWord, ttText>  TRTFTokenTypeSet;

typedef char TCharArray[2000000001];

typedef char *PCharArray;

#pragma option push -b-
enum TFrameMode { fmInside, fmSplit, fmOutside };
#pragma option pop

typedef Set<TSystemSetup, ssAllowSetup, ssAllowPreviewSetup>  TSystemSetups;

typedef Set<TSystemOption, soUseFiler, soNoGenerate>  TSystemOptions;

struct TPosition
{
	double X;
	double Y;
} ;

struct TPositionNode;
typedef TPositionNode *PPositionNode;

struct TPositionNode
{
	double X;
	double Y;
	TPositionNode *Next;
} ;

struct TTab;
typedef TTab *PTab;

struct TTab
{
	double Pos;
	TPrintJustify Justify;
	double Width;
	double Margin;
	bool Left;
	bool Right;
	bool Top;
	bool Bottom;
	Byte Shade;
	Word RefCount;
	TTab *Next;
	TTab *Prev;
} ;

#pragma pack(push, 4)
struct TSaveTab
{
	TTab *Head;
	TTab *Tail;
} ;
#pragma pack(pop)

struct TTabNode;
typedef TTabNode *PTabNode;

#pragma pack(push, 4)
struct TTabNode
{
	TTab *Head;
	TTab *Tail;
	TTabNode *Next;
} ;
#pragma pack(pop)

#pragma pack(push, 4)
struct TSaveBrush
{
	Graphics::TColor Color;
	Graphics::TBrushStyle Style;
	bool Bitmap;
} ;
#pragma pack(pop)

struct TSaveFont
{
	System::SmallString<60>  Name;
	Graphics::TColor Color;
	Graphics::TFontPitch Pitch;
	double Size;
	double Height;
	double Width;
	double Ascent;
	double Descent;
	double InternalLeading;
	bool Bold;
	bool Underline;
	bool Italic;
	bool Strikeout;
	bool Subscript;
	bool Superscript;
	short Rotation;
	TFontAlign Align;
	Word Index;
	HFONT Handle;
	int CharSet;
} ;

struct TFontNode;
typedef TFontNode *PFontNode;

struct TFontNode
{
	TSaveFont Font;
	TFontNode *Next;
} ;

struct TSavePen
{
	Graphics::TColor Color;
	Graphics::TPenMode Mode;
	Graphics::TPenStyle Style;
	double Width;
} ;

struct TFormatPara
{
	TPrintJustify Justify;
	double FirstIndent;
	double LeftIndent;
	double RightIndent;
} ;

#pragma pack(push, 4)
struct TFormatStyle
{
	bool Bold;
	bool Italic;
	bool Underline;
	bool Strikeout;
	bool Subscript;
	bool Superscript;
	Graphics::TFontPitch Pitch;
	Graphics::TColor Color;
} ;
#pragma pack(pop)

struct TFormatState;
typedef TFormatState *PFormatState;

struct TFormatState
{
	System::SmallString<40>  Name;
	double Size;
	TFormatStyle Style;
	TFormatPara Para;
	TFormatState *Prev;
} ;

struct TLineList;
typedef TLineList *PLineList;

struct TLineList
{
	double P1;
	double P2;
	double C1;
	Graphics::TColor Color;
	Graphics::TPenMode Mode;
	Graphics::TPenStyle Style;
	int Width;
	TLineList *Next;
} ;

class DELPHICLASS TRpComponent;
class PASCALIMPLEMENTATION TRpComponent : public Classes::TComponent 
{
	typedef Classes::TComponent inherited;
	
protected:
	AnsiString FVersion;
	void __fastcall SetVersion(AnsiString Value);
	
public:
	__fastcall virtual TRpComponent(Classes::TComponent* AOwner);
	
__published:
	__property AnsiString Version = {read=FVersion, write=SetVersion, stored=false};
public:
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TRpComponent(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPStringObject;
class PASCALIMPLEMENTATION TRPStringObject : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	AnsiString FValue;
	__fastcall TRPStringObject(AnsiString InitValue);
	__property AnsiString Value = {read=FValue, write=FValue};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPStringObject(void) { }
	#pragma option pop
	
};


typedef _devicemodeA *PRPDevMode;

#pragma option push -b-
enum TRPDataType { dtString, dtInteger, dtBoolean, dtFloat, dtCurrency, dtBCD, dtDate, dtTime, dtDateTime, dtBlob, dtMemo, dtGraphic, dtInt64 };
#pragma option pop

typedef Set<TRPDataType, dtString, dtInt64>  TRPDataTypeSet;

#pragma option push -b-
enum TRaveConnectionError { ceAbort, ceInform, cePrompt };
#pragma option pop

//-- var, const, procedure ---------------------------------------------------
#define RunMode ""
static const char CompVer = '\x36';
#define LibType "VCL"
static const char RvMajorVersion = '\x35';
static const char RvMinorVersion = '\x31';
static const char RvSubVersion = '\x32';
#define RPVersion "5.1.2 (VCL6)"
static const short NA = 0xffff8000;
static const Shortint BOXLINENONE = 0x0;
static const Shortint BOXLINELEFT = 0x1;
static const Shortint BOXLINERIGHT = 0x2;
static const Shortint BOXLINELEFTRIGHT = 0x3;
static const Shortint BOXLINETOP = 0x4;
static const Shortint BOXLINELEFTTOP = 0x5;
static const Shortint BOXLINERIGHTTOP = 0x6;
static const Shortint BOXLINENOBOTTOM = 0x7;
static const Shortint BOXLINEBOTTOM = 0x8;
static const Shortint BOXLINELEFTBOTTOM = 0x9;
static const Shortint BOXLINERIGHTBOTTOM = 0xa;
static const Shortint BOXLINENOTOP = 0xb;
static const Shortint BOXLINETOPBOTTOM = 0xc;
static const Shortint BOXLINENORIGHT = 0xd;
static const Shortint BOXLINENOLEFT = 0xe;
static const Shortint BOXLINEALL = 0xf;
static const Shortint BOXLINEHORIZ = 0xc;
static const Shortint BOXLINEVERT = 0x3;
static const Shortint MAXSAVEFONT = 0xa;
static const Shortint MAXSAVETABS = 0xa;
static const Shortint MAXSAVEPOS = 0xa;
static const Word MAXWORDLEN = 0xfff0;
static const int MAXBUFSIZE = 0x77359400;
#define MINPREVIEWZOOM  (1.000000E+01)
#define MAXPREVIEWZOOM  (2.000000E+02)
static const Shortint rpcArc = 0x1;
static const Shortint rpcBrushCopy = 0x2;
static const Shortint rpcChord = 0x3;
static const Shortint rpcCopyRect = 0x4;
static const Shortint rpcDraw = 0x5;
static const Shortint rpcDrawFocusRect = 0x6;
static const Shortint rpcEllipse = 0x7;
static const Shortint rpcFillRect = 0x8;
static const Shortint rpcFloodFill = 0x9;
static const Shortint rpcFrameRect = 0xa;
static const Shortint rpcLineTo = 0xb;
static const Shortint rpcMoveTo = 0xc;
static const Shortint rpcPie = 0xd;
static const Shortint rpcPolygon = 0xe;
static const Shortint rpcPolyline = 0xf;
static const Shortint rpcPrintBitmap = 0x10;
static const Shortint rpcPrintBitmapRect = 0x11;
static const Shortint rpcRectangle = 0x12;
static const Shortint rpcTabRectangle = 0x13;
static const Shortint rpcRoundRect = 0x14;
static const Shortint rpcStretchDraw = 0x15;
static const Shortint rpcTextRect = 0x16;
static const Shortint rpcCenterText = 0x17;
static const Shortint rpcLeftText = 0x18;
static const Shortint rpcRightText = 0x19;
static const Shortint rpcSetTextBKMode = 0x1a;
static const Shortint rpcSetBKColor = 0x1b;
static const Shortint rpcSetScaleX = 0x1c;
static const Shortint rpcSetScaleY = 0x1d;
static const Shortint rpcPrintData = 0x1e;
static const Shortint rpcPrintDataStream = 0x1f;
static const Shortint rpcPrintSpaces = 0x20;
static const Shortint rpcBlockText = 0x21;
static const Shortint rpcTransBitmap = 0x22;
static const Shortint rpcSetFrameMode = 0x23;
static const Shortint rpcPrintImageRect = 0x24;
static const Shortint rpcVariableData = 0x25;
static const Shortint rpcSetBrush = 0x28;
static const Shortint rpcSetBrushColor = 0x29;
static const Shortint rpcSetBrushStyle = 0x2a;
static const Shortint rpcSetBrushBitmap = 0x2b;
static const Shortint rpcSetFont = 0x32;
static const Shortint rpcSetFontName = 0x33;
static const Shortint rpcSetFontColor = 0x34;
static const Shortint rpcSetFontPitch = 0x35;
static const Shortint rpcSetFontHeight = 0x36;
static const Shortint rpcSetFontBold = 0x37;
static const Shortint rpcSetFontUnderline = 0x38;
static const Shortint rpcSetFontItalic = 0x39;
static const Shortint rpcSetFontStrikeout = 0x3a;
static const Shortint rpcSetFontSuperscript = 0x3b;
static const Shortint rpcSetFontSubscript = 0x3c;
static const Shortint rpcSetFontRotation = 0x3d;
static const Shortint rpcSetFontAlign = 0x3e;
static const Shortint rpcSetFontCharset = 0x3f;
static const Shortint rpcSetPen = 0x46;
static const Shortint rpcSetPenColor = 0x47;
static const Shortint rpcSetPenMode = 0x48;
static const Shortint rpcSetPenStyle = 0x49;
static const Shortint rpcSetPenWidth = 0x4a;
static const Shortint rpcNewPage = 0x50;
static const Shortint rpcJobHeader11 = 0x51;
static const Shortint rpcEndPage = 0x52;
static const Shortint rpcResetLineHeight = 0x53;
static const Shortint rpcAllowPreviewOnly = 0x54;
static const Shortint rpcAllowPrinterOnly = 0x55;
static const Shortint rpcAllowAll = 0x56;
static const Shortint rpcSetOrientation = 0x57;
static const Shortint rpcSetBin = 0x58;
static const Shortint rpcJobHeader20i = 0x59;
static const Shortint rpcJobHeader = 0x5a;
static const Byte rpcUser = 0xc8;
extern PACKAGE System::SmallString<10>  RTFIgnoreDest[32];
static const char RPTFPrefix = '\x1';
static const char RPTFInfix = '\x2';
static const char RPTFSeparator = '\x3';
static const char RPTFSuffix = '\x4';
static const char RPTFOff = '\x30';
static const char RPTFOn = '\x31';
static const char RPTFLeft = '\x4c';
static const char RPTFCenter = '\x43';
static const char RPTFRight = '\x52';
static const char RPTFBlock = '\x42';
static const char RPTFIgnore = '\x20';
static const char RPTFBold = '\x42';
static const char RPTFItalic = '\x49';
static const char RPTFUnderline = '\x55';
static const char RPTFStrikeout = '\x4b';
static const char RPTFSuperscript = '\x50';
static const char RPTFSubscript = '\x53';
static const char RPTFName = '\x4e';
static const char RPTFSize = '\x5a';
static const char RPTFColor = '\x43';
static const char RPTFPitch = '\x48';
static const char RPTFResetParagraph = '\x52';
static const char RPTFResetStyle = '\x72';
static const char RPTFJustify = '\x4a';
static const char RPTFFirstIndent = '\x46';
static const char RPTFLeftIndent = '\x4c';
static const char RPTFRightIndent = '\x47';
static const Shortint PaperSizeNameCnt = 0x2b;
extern PACKAGE TPaperSizeName PaperSizeName[43];
extern PACKAGE TRPDataType TRPDataSetType[38];
static const int FileMapSize = 0x10000;
#define FileMapName "RAVEBUFFER"
#define AltFileMapName "RAVEBUFFERALT"
static const Shortint EVENTPREPARED = 0x0;
static const Shortint DATAFIRST = 0x1;
static const Shortint DATANEXT = 0x2;
static const Shortint DATAEOF = 0x3;
static const Shortint DATAGETCOLS = 0x4;
static const Shortint DATAGETROW = 0x5;
static const Shortint DATASETFILTER = 0x6;
static const Shortint DATAGETSORTS = 0x7;
static const Shortint DATASETSORT = 0x8;
static const Shortint DATAOPEN = 0x9;
static const Shortint DATARESTORE = 0xa;
static const Shortint DATAACKNOWLEDGE = 0xb;
static const Shortint DATAFREEALTBUF = 0xc;
extern PACKAGE AnsiString DataID;
#define ControllerMutexName "RAVECONTROLER"
#define CompletedEventName "RAVECOMPLETED"
#define AcknowledgeRTEventName "RAVERTACK_"
#define AcknowledgeDTEventName "RAVEDTACK_"
#define ConnectEventName "RAVECONNECT"
#define ErrorEventName "RAVEERROR"
#define DisconnectEventName "RAVEDISCONNECT"
#define DataRTEventName "RAVERTDATA_"
#define DataDTEventName "RAVEDTDATA_"
extern PACKAGE AnsiString __fastcall (*Trans)(const AnsiString Value);
extern PACKAGE AnsiString __fastcall DefaultTrans(const AnsiString Value);
extern PACKAGE void __fastcall CloseEvent(unsigned AEvent, bool Force = false);
extern PACKAGE void __fastcall CloseMutex(unsigned AMutex, bool Force = false);
extern PACKAGE void __fastcall CloseFileMap(unsigned AFileMap, char * AFileBuf);
extern PACKAGE unsigned __fastcall InitEvent(AnsiString EventName);
extern PACKAGE unsigned __fastcall InitManualEvent(AnsiString EventName);
extern PACKAGE unsigned __fastcall InitMutex(AnsiString MutexName);
extern PACKAGE unsigned __fastcall InitFileMap(AnsiString MapName, int MapSize);
extern PACKAGE char * __fastcall InitFileBuf(unsigned FileMap);
extern PACKAGE void __fastcall RaiseError(AnsiString ErrorMessage);
extern PACKAGE Graphics::TColor __fastcall ShadeToColor(Graphics::TColor ShadeColor, Byte ShadePercent);
extern PACKAGE AnsiString __fastcall MoneyToLongName(Extended Value);
extern PACKAGE Byte __fastcall ConvertCharset(Byte Charset);

}	/* namespace Rpdefine */
using namespace Rpdefine;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpDefine
