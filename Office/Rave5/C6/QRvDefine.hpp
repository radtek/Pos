// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvDefine.pas' rev: 6.00

#ifndef QRvDefineHPP
#define QRvDefineHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <QRpBase.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvdefine
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS ERaveClass;
class PASCALIMPLEMENTATION ERaveClass : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall ERaveClass(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall ERaveClass(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall ERaveClass(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall ERaveClass(int Ident, const System::TVarRec * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall ERaveClass(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall ERaveClass(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall ERaveClass(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall ERaveClass(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~ERaveClass(void) { }
	#pragma option pop
	
};


class DELPHICLASS ERaveException;
class PASCALIMPLEMENTATION ERaveException : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall ERaveException(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall ERaveException(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall ERaveException(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall ERaveException(int Ident, const System::TVarRec * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall ERaveException(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall ERaveException(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall ERaveException(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall ERaveException(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~ERaveException(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TUserLevel { ulBeginner, ulIntermediate, ulAdvanced };
#pragma option pop

#pragma option push -b-
enum TLineWidthType { wtPoints, wtPixels };
#pragma option pop

#pragma option push -b-
enum TFontAttribute { faAll, faName, faSize, faBold, faItalic, faUnderline };
#pragma option pop

#pragma option push -b-
enum TDisplayOn { doParent, doAll, doPreviewOnly, doPrinterOnly };
#pragma option pop

#pragma option push -b-
enum TGotoMode { gmGotoDone, gmGotoNotDone, gmCallEach };
#pragma option pop

typedef Byte TRaveAnchor;

#pragma option push -b-
enum TRaveVAnchor { anvTop, anvBottom, anvCenter, anvStretch, anvResize, anvSpread };
#pragma option pop

#pragma option push -b-
enum TRaveHAnchor { anhLeft, anhRight, anhCenter, anhStretch, anhResize, anhSpread };
#pragma option pop

#pragma option push -b-
enum TRaveFillStyle { fsSolid, fsClear, fsHorizontal, fsVertical, fsFDiagonal, fsBDiagonal, fsCross, fsDiagCross, fsNone };
#pragma option pop

#pragma option push -b-
enum TRaveCalcType { ctCount, ctSum, ctMin, ctMax, ctAverage };
#pragma option pop

typedef double TRaveFloat;

typedef double TRaveUnits;

typedef AnsiString TRaveFieldName;

typedef AnsiString TRaveMemoString;

struct TRavePoint
{
	TRaveUnits X;
	TRaveUnits Y;
} ;

typedef TRavePoint *PRavePoint;

struct TRaveRect
{
	
	union
	{
		struct 
		{
			TRavePoint TopLeft;
			TRavePoint BottomRight;
			
		};
		struct 
		{
			TRaveUnits Left;
			TRaveUnits Top;
			TRaveUnits Right;
			TRaveUnits Bottom;
			
		};
		
	};
} ;

typedef TRaveRect *PRaveRect;

typedef int TRaveBin;

#pragma pack(push, 4)
struct TRaveBinName
{
	AnsiString Name;
	TRaveBin RaveBin;
} ;
#pragma pack(pop)

#pragma option push -b-
enum TRavePrinterDuplex { pdSimplex, pdHorizontal, pdVertical, pdDefault };
#pragma option pop

#pragma option push -b-
enum TRavePrinterCollate { pcFalse, pcTrue, pcDefault };
#pragma option pop

#pragma option push -b-
enum TRavePrinterResolution { prDraft, prLow, prMedium, prHigh, prDefault };
#pragma option pop

typedef void __stdcall (*TRaveSaveCallbackProc)(void * Buf, int Size, void * Data);

typedef TRaveBinName QRvDefine__3[14];

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool InDesigner;
static const Word RaveVersion = 0x9c45;
#define RegRoot "\\Software\\Nevrona Designs\\Rave5BEX\\"
static const Shortint RaveMajorVersion = 0x64;
static const Shortint RaveMinorVersion = 0x1;
extern PACKAGE double RaveDelta;
#define HexDigits "0123456789ABCDEF"
static const Shortint SourceSheet = 0x1;
static const Shortint DesignerSheet = 0x2;
static const Shortint DataViewSheet = 0x3;
static const Shortint QuerySheet = 0x4;
static const Shortint RaveAlignLeft = 0x1;
static const Shortint RaveAlignHCenter = 0x2;
static const Shortint RaveAlignRight = 0x3;
static const Shortint RaveAlignHCenterInParent = 0x4;
static const Shortint RaveAlignHSpace = 0x5;
static const Shortint RaveAlignEquateWidths = 0x6;
static const Shortint RaveAlignTop = 0x7;
static const Shortint RaveAlignVCenter = 0x8;
static const Shortint RaveAlignBottom = 0x9;
static const Shortint RaveAlignVCenterInParent = 0xa;
static const Shortint RaveAlignVSpace = 0xb;
static const Shortint RaveAlignEquateHeights = 0xc;
static const Shortint RaveAlignMoveForward = 0xd;
static const Shortint RaveAlignMoveBehind = 0xe;
static const Shortint RaveAlignBringToFront = 0xf;
static const Shortint RaveAlignSendToBack = 0x10;
static const Shortint RaveAlignTapLeft = 0x11;
static const Shortint RaveAlignTapRight = 0x12;
static const Shortint RaveAlignTapUp = 0x13;
static const Shortint RaveAlignTapDown = 0x14;
static const Shortint RaveAlignTapHSizeDown = 0x15;
static const Shortint RaveAlignTapHSizeUp = 0x16;
static const Shortint RaveAlignTapVSizeDown = 0x17;
static const Shortint RaveAlignTapVSizeUp = 0x18;
extern PACKAGE unsigned RavePrinterResolution[5];
static const Shortint RaveBinNameCnt = 0xe;
extern PACKAGE TRaveBinName RaveBinName[14];
extern PACKAGE double LineWidths[12];
extern PACKAGE AnsiString RaveTitle;
extern PACKAGE bool DeveloperLevel;
extern PACKAGE TUserLevel UserLevel;
extern PACKAGE int FloatPropPrecision;
extern PACKAGE Qrpbase::TBaseReport* GBaseReport;
extern PACKAGE Word EventDelays[14];
extern PACKAGE Word LargeTimeouts[14];
extern PACKAGE bool AlwaysShowHeaders;
extern PACKAGE bool AdminMode;
extern PACKAGE bool SaveEnvOnly;
extern PACKAGE Qrpdefine::TReportDest PrintDestination;
extern PACKAGE bool AllowSetup;
extern PACKAGE int PreviewShadowDepth;
extern PACKAGE Qforms::TWindowState PreviewWindowState;
extern PACKAGE double PreviewGridHoriz;
extern PACKAGE double PreviewGridVert;
extern PACKAGE double PreviewZoomFactor;
extern PACKAGE int PreviewZoomInc;
extern PACKAGE bool PreviewMonochrome;
extern PACKAGE Qgraphics::TColor PreviewGridColor;
extern PACKAGE Qgraphics::TPenStyle PreviewGridPenStyle;
extern PACKAGE Qrpdefine::TRulerType PreviewRulerType;
extern PACKAGE double ProjectUnitsFactor;
extern PACKAGE Qrpdefine::TRavePaperSize PaperSize;
extern PACKAGE TRaveUnits PaperWidth;
extern PACKAGE TRaveUnits PaperHeight;
extern PACKAGE double GridSpacing;
extern PACKAGE int GridLines;
extern PACKAGE bool RaveInitialized;
extern PACKAGE bool DeveloperRave;
extern PACKAGE Qgraphics::TBitmap* TrueTypeFontImage;
extern PACKAGE Qgraphics::TBitmap* DeviceFontImage;
extern PACKAGE TRaveUnits TapDist;
extern PACKAGE bool ShowTapButtons;
extern PACKAGE bool ShowOrderButtons;
extern PACKAGE bool HoldAddDeleteComponents;
extern PACKAGE void __fastcall RaveError(AnsiString ErrorMessage);

}	/* namespace Qrvdefine */
using namespace Qrvdefine;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvDefine
