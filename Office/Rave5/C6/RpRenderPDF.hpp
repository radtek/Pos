// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpRenderPDF.pas' rev: 6.00

#ifndef RpRenderPDFHPP
#define RpRenderPDFHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpDefine.hpp>	// Pascal unit
#include <RpRender.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <jpeg.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rprenderpdf
{
//-- type declarations -------------------------------------------------------
typedef short TFontWidthTable[256];

#pragma option push -b-
enum TPDFFontStyles { psNormal, psBold, psItalic, psBoldItalic };
#pragma option pop

#pragma option push -b-
enum TPDFFontEncoding { feMacRomanEncoding, feMacExpertEncoding, feWinAnsiEncoding, fePDFDocEncoding, feStandardEncoding };
#pragma option pop

struct TEllipsePts
{
	double XA;
	double XB;
	double XC;
	double XD;
	double XE;
	double YA;
	double YB;
	double YC;
	double YD;
	double YE;
} ;

struct TSmallArcRecord
{
	double X1;
	double X2;
	double X3;
	double X4;
	double Y1;
	double Y2;
	double Y3;
	double Y4;
} ;

typedef TSmallArcRecord TSmallArcBuffer[4];

typedef AnsiString RpRenderPDF__1[14];

typedef AnsiString RpRenderPDF__2[5];

class DELPHICLASS TRPPDFDocInfo;
class PASCALIMPLEMENTATION TRPPDFDocInfo : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	AnsiString FTitle;
	AnsiString FAuthor;
	AnsiString FSubject;
	AnsiString FKeyWords;
	AnsiString FCreator;
	AnsiString FProducer;
	
public:
	__fastcall virtual TRPPDFDocInfo(void);
	
__published:
	__property AnsiString Title = {read=FTitle, write=FTitle};
	__property AnsiString Author = {read=FAuthor, write=FAuthor};
	__property AnsiString Subject = {read=FSubject, write=FSubject};
	__property AnsiString KeyWords = {read=FKeyWords, write=FKeyWords};
	__property AnsiString Creator = {read=FCreator, write=FCreator};
	__property AnsiString Producer = {read=FProducer, write=FProducer};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TRPPDFDocInfo(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPPDFObject;
class DELPHICLASS TRvRenderPDF;
class DELPHICLASS TRPPDFCatalog;
class PASCALIMPLEMENTATION TRPPDFObject : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	TRvRenderPDF* FRvRenderPDF;
	int FID;
	Classes::TMemoryStream* FDataStream;
	void __fastcall SetID(int AValue);
	int __fastcall GetID(void);
	virtual void __fastcall InitData(void) = 0 ;
	
public:
	__fastcall virtual TRPPDFObject(System::TObject* AOwner);
	__fastcall virtual ~TRPPDFObject(void);
	__property Classes::TMemoryStream* DataStream = {read=FDataStream};
	__property TRvRenderPDF* RvRenderPDF = {read=FRvRenderPDF, write=FRvRenderPDF};
};


class PASCALIMPLEMENTATION TRPPDFCatalog : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
protected:
	bool FShowOutlines;
	
public:
	virtual void __fastcall InitData(void);
	__property bool ShowOutlines = {read=FShowOutlines, write=FShowOutlines, nodefault};
public:
	#pragma option push -w-inl
	/* TRPPDFObject.Create */ inline __fastcall virtual TRPPDFCatalog(System::TObject* AOwner) : TRPPDFObject(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRPPDFObject.Destroy */ inline __fastcall virtual ~TRPPDFCatalog(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPPDFPages;
class PASCALIMPLEMENTATION TRPPDFPages : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
protected:
	int FOpenPage;
	
public:
	virtual void __fastcall InitData(void);
	AnsiString __fastcall GetPageReferences();
	__property int OpenPage = {read=FOpenPage, write=FOpenPage, nodefault};
public:
	#pragma option push -w-inl
	/* TRPPDFObject.Create */ inline __fastcall virtual TRPPDFPages(System::TObject* AOwner) : TRPPDFObject(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRPPDFObject.Destroy */ inline __fastcall virtual ~TRPPDFPages(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPPDFOutlines;
class PASCALIMPLEMENTATION TRPPDFOutlines : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
public:
	virtual void __fastcall InitData(void);
public:
	#pragma option push -w-inl
	/* TRPPDFObject.Create */ inline __fastcall virtual TRPPDFOutlines(System::TObject* AOwner) : TRPPDFObject(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRPPDFObject.Destroy */ inline __fastcall virtual ~TRPPDFOutlines(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPPDFPage;
class DELPHICLASS TRPPDFPageContent;
class PASCALIMPLEMENTATION TRPPDFPageContent : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
protected:
	Classes::TMemoryStream* FPageStream;
	
public:
	__fastcall virtual TRPPDFPageContent(System::TObject* AOwner);
	__fastcall virtual ~TRPPDFPageContent(void);
	virtual void __fastcall InitData(void);
	__property Classes::TMemoryStream* PageStream = {read=FPageStream};
};


class PASCALIMPLEMENTATION TRPPDFPage : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
protected:
	TRPPDFPageContent* FPageContent;
	double FPageWidth;
	double FPageHeight;
	Graphics::TColor FTextColor;
	Graphics::TColor FLastTextColor;
	bool FLastTextColorDefined;
	Graphics::TColor FPenColor;
	Graphics::TColor FLastPenColor;
	bool FLastPenColorDefined;
	double FLastPenWidth;
	bool FLastPenWidthDefined;
	Graphics::TPenStyle FLastPenStyle;
	bool FLastPenStyleDefined;
	Graphics::TColor FBrushColor;
	Graphics::TColor FLastBrushColor;
	bool FLastBrushColorDefined;
	Graphics::TBrushStyle FLastBrushStyle;
	bool FLastBrushStyleDefined;
	__property bool LastTextColorDefined = {read=FLastTextColorDefined, write=FLastTextColorDefined, nodefault};
	__property bool LastPenColorDefined = {read=FLastPenColorDefined, write=FLastPenColorDefined, nodefault};
	__property bool LastPenWidthDefined = {read=FLastPenWidthDefined, write=FLastPenWidthDefined, nodefault};
	__property bool LastPenStyleDefined = {read=FLastPenStyleDefined, write=FLastPenStyleDefined, nodefault};
	__property bool LastBrushStyleDefined = {read=FLastBrushStyleDefined, write=FLastBrushStyleDefined, nodefault};
	__property bool LastBrushColorDefined = {read=FLastBrushColorDefined, write=FLastBrushColorDefined, nodefault};
	double __fastcall InchesToUnitsX(const double AValue);
	double __fastcall InchesToUnitsY(const double AValue);
	AnsiString __fastcall FormatRPPDFText(const AnsiString AValue);
	void __fastcall ClosePath(void);
	void __fastcall RotateXYCoordinate(double AX1, double AY1, double AAngle, double &xRot, double &yRot);
	double __fastcall DegreesToRadians(double ADegrees);
	void __fastcall SmallArc(double AX1, double AY1, double AR1, double MidTheta, double HalfAngle, double ccwcw, int AIndex);
	void __fastcall RadiusArc(const double AX1, const double AY1, const double AR1, const double sAngle, const double eAngle);
	void __fastcall AdjustRotationOffset(const double ATextWidth, double &AX, double &AY);
	
public:
	__fastcall virtual TRPPDFPage(System::TObject* AOwner);
	__fastcall virtual ~TRPPDFPage(void);
	virtual void __fastcall InitData(void);
	AnsiString __fastcall GetRPPDFColor(Graphics::TColor Color, bool bAExpanded);
	TEllipsePts __fastcall GetEllipsePts(const double AX1, const double AY1, const double AX2, const double AY2);
	void __fastcall PrintCharSpacing(void);
	void __fastcall PrintWordSpacing(void);
	void __fastcall PrintTextColor(void);
	void __fastcall PrintPenColor(void);
	void __fastcall PrintBrushColor(void);
	void __fastcall PrintLeft(const double AX, const double AY, AnsiString AText);
	void __fastcall PrintCenter(const double AX, const double AY, AnsiString AText);
	void __fastcall PrintRight(const double AX, const double AY, AnsiString AText);
	void __fastcall TextRect(const Types::TRect &Rect, double X1, double Y1, AnsiString S1);
	void __fastcall SetPenAndBrush(void);
	void __fastcall MoveTo(const double AX, const double AY);
	void __fastcall LineTo(const double AX, const double AY, bool DoClosePath);
	void __fastcall Arc(const double pfX1, const double pfY1, const double pfX2, const double pfY2, const double pfX3, const double pfY3, const double pfX4, const double pfY4, bool DoClosePath, bool AllowFill);
	void __fastcall Rectangle(const double AX1, const double AY1, const double AX2, const double AY2, const bool IsClipping);
	void __fastcall RoundRect(const double AX1, const double AY1, const double AX2, const double AY2, const double AX3, const double AY3);
	void __fastcall CurveTo(const double AX1, const double AY1, const double AX2, const double AY2, const double AX3, const double AY3);
	void __fastcall Ellipse(const double AX1, const double AY1, const double AX2, const double AY2);
	void __fastcall PrintJPG(const double AX1, const double AY1, const double AX2, const double AY2, AnsiString AName);
	void __fastcall Pie(const double pfX1, const double pfY1, const double pfX2, const double pfY2, const double pfX3, const double pfY3, const double pfX4, const double pfY4);
	void __fastcall PolyLine(const Rprender::TFloatPoint * PolyLineArr, const int PolyLineArr_Size);
	void __fastcall Polygon(const Rprender::TFloatPoint * PointArr, const int PointArr_Size);
	__property TRPPDFPageContent* PageContent = {read=FPageContent};
	__property double PageWidth = {read=FPageWidth, write=FPageWidth};
	__property double PageHeight = {read=FPageHeight, write=FPageHeight};
	__property Graphics::TColor TextColor = {read=FTextColor, write=FTextColor, nodefault};
	__property Graphics::TColor LastTextColor = {read=FLastTextColor, write=FLastTextColor, nodefault};
	__property Graphics::TColor PenColor = {read=FPenColor, write=FPenColor, nodefault};
	__property Graphics::TColor LastPenColor = {read=FLastPenColor, write=FLastPenColor, nodefault};
	__property double LastPenWidth = {read=FLastPenWidth, write=FLastPenWidth};
	__property Graphics::TPenStyle LastPenStyle = {read=FLastPenStyle, write=FLastPenStyle, nodefault};
	__property Graphics::TBrushStyle LastBrushStyle = {read=FLastBrushStyle, write=FLastBrushStyle, nodefault};
	__property Graphics::TColor BrushColor = {read=FBrushColor, write=FBrushColor, nodefault};
	__property Graphics::TColor LastBrushColor = {read=FLastBrushColor, write=FLastBrushColor, nodefault};
};


class DELPHICLASS TRPPDFFont;
class PASCALIMPLEMENTATION TRPPDFFont : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
protected:
	AnsiString FFontType;
	AnsiString FObjectName;
	AnsiString FFontName;
	short FontWidths[256];
	AnsiString FPDFFontName;
	Graphics::TFontStyles FFontStyles;
	Graphics::TFontCharset FFontCharset;
	int FFirstChar;
	int FLastChar;
	void __fastcall SetCharSet(Graphics::TFontCharset AFontCharset);
	__property AnsiString PDFFontName = {read=FPDFFontName, write=FPDFFontName};
	__property Graphics::TFontStyles FontStyles = {read=FFontStyles, write=FFontStyles, nodefault};
	__property AnsiString FontType = {read=FFontType, write=FFontType};
	__property AnsiString ObjectName = {read=FObjectName, write=FObjectName};
	__property AnsiString FontName = {read=FFontName, write=FFontName};
	__property int FirstChar = {read=FFirstChar, write=FFirstChar, nodefault};
	__property int LastChar = {read=FLastChar, write=FLastChar, nodefault};
	__property Graphics::TFontCharset FontCharset = {read=FFontCharset, write=SetCharSet, nodefault};
	
public:
	__fastcall virtual TRPPDFFont(System::TObject* AOwner);
	double __fastcall GetTextWidth(double AFontSize, AnsiString AText);
	virtual void __fastcall InitData(void);
public:
	#pragma option push -w-inl
	/* TRPPDFObject.Destroy */ inline __fastcall virtual ~TRPPDFFont(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPPDFProcSet;
class PASCALIMPLEMENTATION TRPPDFProcSet : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
public:
	virtual void __fastcall InitData(void);
public:
	#pragma option push -w-inl
	/* TRPPDFObject.Create */ inline __fastcall virtual TRPPDFProcSet(System::TObject* AOwner) : TRPPDFObject(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRPPDFObject.Destroy */ inline __fastcall virtual ~TRPPDFProcSet(void) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TCompressEvent)(Classes::TStream* InStream, Classes::TStream* OutStream, AnsiString &CompressMethod);

typedef Shortint TImageQualityRange;

class DELPHICLASS TRPPDFPattern;
class PASCALIMPLEMENTATION TRvRenderPDF : public Rprender::TRPRenderStream 
{
	typedef Rprender::TRPRenderStream inherited;
	
protected:
	Classes::TMemoryStream* FDataStream;
	Classes::TStringList* FOutlineList;
	Classes::TStringList* FPageList;
	Classes::TStringList* FXObjectList;
	Classes::TStringList* FFontList;
	Classes::TStringList* FPatternList;
	Classes::TStringList* FXRefList;
	TRPPDFCatalog* FCatalogObject;
	TRPPDFPages* FPagesObject;
	TRPPDFOutlines* FOutlinesObject;
	TRPPDFPage* FCurrentPageObject;
	TRPPDFFont* FCurrentFontObject;
	TRPPDFProcSet* FProcSetObject;
	int XRefPos;
	AnsiString FOpenMode;
	TPDFFontEncoding FFontEncoding;
	int FFontSize;
	double FFontRotation;
	bool FUnderlineFont;
	bool FStrikeoutFont;
	Rpdefine::TFrameMode FFrameMode;
	bool FUseCompression;
	bool FEmbedFonts;
	bool FEmbedBaseFonts;
	bool FirstNewPage;
	TSmallArcRecord SmallArcData[4];
	TCompressEvent FOnCompress;
	TImageQualityRange FImageQuality;
	int FMetafileDPI;
	TRPPDFDocInfo* FDocInfo;
	Graphics::TBitmap* FFontBitmap;
	bool FFontChanged;
	double __fastcall GetPageHeight(void);
	double __fastcall GetPageWidth(void);
	void __fastcall SetPageHeight(const double AValue);
	void __fastcall SetPageWidth(const double AValue);
	Graphics::TColor __fastcall GetPenColor(void);
	void __fastcall SetPenColor(const Graphics::TColor AValue);
	Graphics::TColor __fastcall GetBrushColor(void);
	void __fastcall SetBrushColor(const Graphics::TColor AValue);
	Graphics::TColor __fastcall GetTextColor(void);
	bool __fastcall IsCJKFont(void);
	AnsiString __fastcall CreatePattern(Graphics::TBrushStyle AStyle, Graphics::TColor AFillColor);
	AnsiString __fastcall CreatePDFFont(AnsiString AFontName, Graphics::TFontStyles AFontStyles, Graphics::TFontCharset AFontCharset);
	AnsiString __fastcall GetPDFFontName(AnsiString AFontName, Graphics::TFontStyles AFontStyles, Graphics::TFontCharset AFontCharset);
	bool __fastcall IsTrueType(AnsiString AFontName, Graphics::TFontStyles AFontStyles, Graphics::TFontCharset AFontCharset);
	TRPPDFFont* __fastcall GetFont(AnsiString AFontName, Graphics::TFontStyles AFontStyles, Graphics::TFontCharset AFontCharset);
	AnsiString __fastcall GetPatternName(Graphics::TBrushStyle AStyle, Graphics::TColor AFillColor);
	TRPPDFPattern* __fastcall GetPattern(AnsiString APatternName);
	bool __fastcall GetDoFill(void);
	bool __fastcall GetDoStroke(void);
	void __fastcall UpdateFont(void);
	TRPPDFFont* __fastcall GetCurrentFontObject(void);
	bool __fastcall GetUnderlineFont(void);
	bool __fastcall GetStrikeoutFont(void);
	int __fastcall GetFontSize(void);
	double __fastcall GetFontRotation(void);
	void __fastcall NewPage(void);
	AnsiString __fastcall DateTimeToPDF(const System::TDateTime Value);
	void __fastcall SetTextColor(Graphics::TColor AColor);
	void __fastcall SetGraphicColor(Graphics::TColor AStrokeColor, Graphics::TColor ABrushColor);
	AnsiString __fastcall FormatEx(const AnsiString AData, const System::TVarRec * AArgs, const int AArgs_Size);
	virtual void __fastcall PrintF(const AnsiString AData, const System::TVarRec * AArgs, const int AArgs_Size);
	virtual void __fastcall PrintLnF(const AnsiString AData, const System::TVarRec * AArgs, const int AArgs_Size);
	virtual void __fastcall PrintRightWidth(double &X1, double &Y1, AnsiString Text, double Width);
	AnsiString __fastcall StrToHex(const AnsiString AValue);
	__property double PageWidth = {read=GetPageWidth, write=SetPageWidth};
	__property double PageHeight = {read=GetPageHeight, write=SetPageHeight};
	__property double FontRotation = {read=GetFontRotation};
	__property Graphics::TColor PenColor = {read=GetPenColor, write=SetPenColor, nodefault};
	__property Graphics::TColor BrushColor = {read=GetBrushColor, write=SetBrushColor, nodefault};
	__property Classes::TMemoryStream* DataStream = {read=FDataStream};
	__property Graphics::TColor TextColor = {read=GetTextColor, write=SetTextColor, nodefault};
	__property TRPPDFFont* CurrentFont = {read=GetCurrentFontObject, write=FCurrentFontObject};
	__property bool UnderlineFont = {read=GetUnderlineFont, nodefault};
	__property bool StrikeoutFont = {read=GetStrikeoutFont, nodefault};
	__property int FontSize = {read=GetFontSize, nodefault};
	__property TRPPDFPage* CurrentPage = {read=FCurrentPageObject, write=FCurrentPageObject};
	__property Classes::TStringList* PageList = {read=FPageList};
	__property Classes::TStringList* XObjectList = {read=FXObjectList};
	__property Classes::TStringList* FontList = {read=FFontList};
	__property Classes::TStringList* PatternList = {read=FPatternList};
	__property TRPPDFCatalog* CatalogObject = {read=FCatalogObject};
	__property TRPPDFOutlines* OutlinesObject = {read=FOutlinesObject};
	__property TRPPDFPages* PagesObject = {read=FPagesObject};
	__property TRPPDFProcSet* ProcSetObject = {read=FProcSetObject};
	__property Classes::TStringList* XRefList = {read=FXRefList};
	__property Classes::TStringList* OutlineList = {read=FOutlineList};
	__property Graphics::TBitmap* FontBitmap = {read=FFontBitmap, write=FFontBitmap};
	
public:
	__fastcall virtual TRvRenderPDF(Classes::TComponent* AOwner);
	__fastcall virtual ~TRvRenderPDF(void);
	virtual void __fastcall PrintRender(Classes::TStream* NDRStream, AnsiString OutputFileName);
	void __fastcall CreatePDFObjects(void);
	void __fastcall FreePDFObjects(void);
	void __fastcall PrintLeft(const double AX, const double AY, AnsiString AText);
	void __fastcall PrintCenter(const double AX, const double AY, AnsiString AText);
	void __fastcall PrintRight(const double AX, const double AY, AnsiString AText);
	void __fastcall InitData(void);
	void __fastcall FreeData(void);
	void __fastcall SetIDs(void);
	void __fastcall WriteHeader(void);
	void __fastcall WriteDocInfo(void);
	void __fastcall WriteCatalog(void);
	void __fastcall WriteOutlinesObject(void);
	void __fastcall WritePagesObject(void);
	void __fastcall WritePages(void);
	void __fastcall WriteProcSet(void);
	void __fastcall WriteFonts(void);
	void __fastcall WritePatterns(void);
	void __fastcall WriteXObjects(void);
	void __fastcall WriteXRef(void);
	void __fastcall WriteTrailer(void);
	void __fastcall WriteStartXRef(void);
	void __fastcall WriteEof(void);
	void __fastcall RegisterObject(void);
	void __fastcall GenerateBook(void);
	virtual void __fastcall FontChanged(System::TObject* Sender);
	virtual void __fastcall LeftText(const AnsiString psText, const double pfX, const double pfY);
	virtual void __fastcall CenterText(const AnsiString psText, const double pfX, const double pfY);
	virtual void __fastcall RightText(const AnsiString psText, const double pfX, const double pfY);
	virtual void __fastcall TextRect(const Types::TRect &Rect, double X1, double Y1, AnsiString S1);
	virtual void __fastcall PrintSpaces(const AnsiString AText, const double AX, const double AY, const double AWidth);
	virtual void __fastcall Arc(const double AX1, const double AY1, const double AX2, const double AY2, const double AX3, const double AY3, const double AX4, const double AY4);
	void __fastcall PDFArc(const double AX1, const double AY1, const double AX2, const double AY2, const double AX3, const double AY3, const double AX4, const double AY4, bool ADoClosePath, bool AAllowFill);
	virtual void __fastcall Chord(const double AX1, const double AY1, const double AX2, const double AY2, const double AX3, const double AY3, const double AX4, const double AY4);
	virtual void __fastcall Rectangle(const double pfX1, const double pfY1, const double pfX2, const double pfY2);
	void __fastcall PDFRectangle(const double AX1, const double AY1, const double AX2, const double AY2);
	virtual void __fastcall FillRect(const Types::TRect &pRect);
	virtual void __fastcall RoundRect(const double pfX1, const double pfY1, const double pfX2, const double pfY2, const double pfX3, const double pfY3);
	void __fastcall PDFRoundRect(const double AX1, const double AY1, const double AX2, const double AY2, const double AX3, const double AY3);
	virtual void __fastcall Ellipse(const double pfX1, const double pfY1, const double pfX2, const double pfY2);
	void __fastcall PDFEllipse(const double AX1, const double AY1, const double AX2, const double AY2);
	virtual void __fastcall MoveTo(const double pfX1, const double pfY1);
	void __fastcall PDFMoveTo(const double AX, const double AY);
	virtual void __fastcall LineTo(const double pfX1, const double pfY1);
	void __fastcall PDFLineTo(const double AX, const double AY, bool ADoClosePath);
	virtual void __fastcall PrintBitmapRect(const double X1, const double Y1, const double X2, const double Y2, Graphics::TBitmap* AGraphic);
	virtual void __fastcall PrintBitmap(const double X1, const double Y1, const double ScaleX, const double ScaleY, Graphics::TBitmap* AGraphic);
	virtual void __fastcall StretchDraw(const Types::TRect &Rect, Graphics::TGraphic* AGraphic);
	virtual void __fastcall Draw(const double pfX1, const double pfY1, Graphics::TGraphic* AGraphic);
	AnsiString __fastcall AddJPG(const Jpeg::TJPEGImage* AImage, bool AReuseJPG);
	void __fastcall PrintJPG(const double AX1, const double AY1, const double AX2, const double AY2, AnsiString AName);
	virtual void __fastcall Pie(const double AX1, const double AY1, const double AX2, const double AY2, const double AX3, const double AY3, const double AX4, const double AY4);
	void __fastcall PDFPie(const double AX1, const double AY1, const double AX2, const double AY2, const double AX3, const double AY3, const double AX4, const double AY4, bool ADoClosePath, bool AAllowFill);
	virtual void __fastcall PolyLine(const Rprender::TFloatPoint * PolyLineArr, const int PolyLineArr_Size);
	virtual void __fastcall Polygon(const Rprender::TFloatPoint * PointArr, const int PointArr_Size);
	virtual void __fastcall DocBegin(void);
	virtual void __fastcall DocEnd(void);
	virtual void __fastcall PageBegin(void);
	virtual void __fastcall PageEnd(void);
	__property bool DoStroke = {read=GetDoStroke, nodefault};
	__property bool DoFill = {read=GetDoFill, nodefault};
	__property bool EmbedBaseFonts = {read=FEmbedBaseFonts, write=FEmbedBaseFonts, nodefault};
	
__published:
	__property OnDecodeImage ;
	__property bool UseCompression = {read=FUseCompression, write=FUseCompression, default=0};
	__property bool EmbedFonts = {read=FEmbedFonts, write=FEmbedFonts, nodefault};
	__property TCompressEvent OnCompress = {read=FOnCompress, write=FOnCompress};
	__property TImageQualityRange ImageQuality = {read=FImageQuality, write=FImageQuality, nodefault};
	__property int MetafileDPI = {read=FMetafileDPI, write=FMetafileDPI, nodefault};
	__property TPDFFontEncoding FontEncoding = {read=FFontEncoding, write=FFontEncoding, nodefault};
	__property TRPPDFDocInfo* DocInfo = {read=FDocInfo, write=FDocInfo};
};



class DELPHICLASS TRPPDFPatternResource;
class PASCALIMPLEMENTATION TRPPDFPatternResource : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
protected:
	AnsiString FResourceStream;
	__property AnsiString ResourceStream = {read=FResourceStream, write=FResourceStream};
	
public:
	virtual void __fastcall InitData(void);
public:
	#pragma option push -w-inl
	/* TRPPDFObject.Create */ inline __fastcall virtual TRPPDFPatternResource(System::TObject* AOwner) : TRPPDFObject(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRPPDFObject.Destroy */ inline __fastcall virtual ~TRPPDFPatternResource(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TRPPDFPattern : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
protected:
	AnsiString FObjectName;
	AnsiString FPatternStream;
	TRPPDFPatternResource* FPatternResource;
	__property AnsiString ObjectName = {read=FObjectName, write=FObjectName};
	__property AnsiString PatternStream = {read=FPatternStream, write=FPatternStream};
	__property TRPPDFPatternResource* PatternResource = {read=FPatternResource, write=FPatternResource};
	
public:
	__fastcall virtual TRPPDFPattern(System::TObject* AOwner);
	__fastcall virtual ~TRPPDFPattern(void);
	virtual void __fastcall InitData(void);
	void __fastcall SetPattern(Graphics::TBrushStyle AStyle, Graphics::TColor AFillColor);
};


class DELPHICLASS TRPPDFFontDescriptor;
class PASCALIMPLEMENTATION TRPPDFFontDescriptor : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
protected:
	AnsiString FPDFFontName;
	int FFlags;
	#pragma pack(push, 1)
	Types::TRect FFontBBox;
	#pragma pack(pop)
	
	int FMissingWidth;
	int FStemV;
	int FStemH;
	int FItalicAngle;
	int FCapHeight;
	int FXHeight;
	int FAscent;
	int FDescent;
	int FLeading;
	int FMaxWidth;
	int FAvgWidth;
	bool FIsEmbeddingAllowed;
	#pragma pack(push, 1)
	tagPANOSE FPanose;
	#pragma pack(pop)
	
	int __fastcall SetBit(int AValue, int ABit);
	__property tagPANOSE Panose = {read=FPanose, write=FPanose};
	
public:
	virtual void __fastcall InitData(void);
	virtual void __fastcall SetValues(Windows::POutlineTextmetricA OutlineTextMetric, int AMissingWidth);
	__property AnsiString PDFFontName = {read=FPDFFontName, write=FPDFFontName};
	__property int Flags = {read=FFlags, write=FFlags, nodefault};
	__property Types::TRect FontBBox = {read=FFontBBox, write=FFontBBox};
	__property int MissingWidth = {read=FMissingWidth, write=FMissingWidth, nodefault};
	__property int StemV = {read=FStemV, write=FStemV, nodefault};
	__property int StemH = {read=FStemH, write=FStemH, nodefault};
	__property int ItalicAngle = {read=FItalicAngle, write=FItalicAngle, nodefault};
	__property int CapHeight = {read=FCapHeight, write=FCapHeight, nodefault};
	__property int XHeight = {read=FXHeight, write=FXHeight, nodefault};
	__property int Ascent = {read=FAscent, write=FAscent, nodefault};
	__property int Descent = {read=FDescent, write=FDescent, nodefault};
	__property int Leading = {read=FLeading, write=FLeading, nodefault};
	__property int MaxWidth = {read=FMaxWidth, write=FMaxWidth, nodefault};
	__property int AvgWidth = {read=FAvgWidth, write=FAvgWidth, nodefault};
	__property bool IsEmbeddingAllowed = {read=FIsEmbeddingAllowed, write=FIsEmbeddingAllowed, nodefault};
public:
	#pragma option push -w-inl
	/* TRPPDFObject.Create */ inline __fastcall virtual TRPPDFFontDescriptor(System::TObject* AOwner) : TRPPDFObject(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRPPDFObject.Destroy */ inline __fastcall virtual ~TRPPDFFontDescriptor(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPPDFFontDescendant;
class DELPHICLASS TRPPDFFontDescriptorComposite;
class PASCALIMPLEMENTATION TRPPDFFontDescriptorComposite : public TRPPDFFontDescriptor 
{
	typedef TRPPDFFontDescriptor inherited;
	
protected:
	TRPPDFFontDescendant* FDescendantFont;
	TRPPDFFontDescriptorComposite* FDescriptor;
	AnsiString FStyle;
	
public:
	__property TRPPDFFontDescriptorComposite* Descriptor = {read=FDescriptor, write=FDescriptor};
	virtual void __fastcall InitData(void);
	virtual void __fastcall SetValues(Windows::POutlineTextmetricA OutlineTextMetric, int AMissingWidth);
	__property AnsiString Style = {read=FStyle, write=FStyle};
public:
	#pragma option push -w-inl
	/* TRPPDFObject.Create */ inline __fastcall virtual TRPPDFFontDescriptorComposite(System::TObject* AOwner) : TRPPDFFontDescriptor(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRPPDFObject.Destroy */ inline __fastcall virtual ~TRPPDFFontDescriptorComposite(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TRPPDFFontDescendant : public TRPPDFFont 
{
	typedef TRPPDFFont inherited;
	
protected:
	AnsiString FPDFFontName;
	TRPPDFFontDescriptorComposite* FDescriptor;
	int FMaxCharWidth;
	int FAveCharWidth;
	AnsiString FRegistry;
	AnsiString FOrdering;
	int FSupplement;
	Graphics::TFontCharset FCharset;
	
public:
	virtual void __fastcall InitData(void);
	__property TRPPDFFontDescriptorComposite* Descriptor = {read=FDescriptor, write=FDescriptor};
	__property AnsiString PDFFontName = {read=FPDFFontName, write=FPDFFontName};
	__property AnsiString Registry = {read=FRegistry, write=FRegistry};
	__property AnsiString Ordering = {read=FOrdering, write=FOrdering};
	__property int Supplement = {read=FSupplement, write=FSupplement, nodefault};
	__property int MaxCharWidth = {read=FMaxCharWidth, write=FMaxCharWidth, nodefault};
	__property int AveCharWidth = {read=FAveCharWidth, write=FAveCharWidth, nodefault};
	__property Graphics::TFontCharset Charset = {read=FCharset, write=FCharset, nodefault};
public:
	#pragma option push -w-inl
	/* TRPPDFFont.Create */ inline __fastcall virtual TRPPDFFontDescendant(System::TObject* AOwner) : TRPPDFFont(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TRPPDFObject.Destroy */ inline __fastcall virtual ~TRPPDFFontDescendant(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPPDFFontFile;
class PASCALIMPLEMENTATION TRPPDFFontFile : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
protected:
	Classes::TMemoryStream* FFontStream;
	int FLength1;
	
public:
	__fastcall virtual TRPPDFFontFile(System::TObject* AOwner);
	__fastcall virtual ~TRPPDFFontFile(void);
	virtual void __fastcall InitData(void);
	void __fastcall LoadFontFile(unsigned Handle);
	__property Classes::TMemoryStream* FontStream = {read=FFontStream};
};


class DELPHICLASS TRPPDFFontTrueType;
class PASCALIMPLEMENTATION TRPPDFFontTrueType : public TRPPDFFont 
{
	typedef TRPPDFFont inherited;
	
protected:
	TRPPDFFontDescriptor* FDescriptor;
	TRPPDFFontFile* FFontFile;
	bool FIsEmbeddingAllowed;
	void __fastcall SetFontData(void);
	
public:
	__fastcall virtual TRPPDFFontTrueType(System::TObject* AOwner);
	__fastcall virtual ~TRPPDFFontTrueType(void);
	virtual void __fastcall InitData(void);
	__property TRPPDFFontDescriptor* Descriptor = {read=FDescriptor, write=FDescriptor};
	__property TRPPDFFontFile* FontFile = {read=FFontFile, write=FFontFile};
	__property bool IsEmbeddingAllowed = {read=FIsEmbeddingAllowed, write=FIsEmbeddingAllowed, nodefault};
};


class DELPHICLASS TRPPDFFontComposite;
class PASCALIMPLEMENTATION TRPPDFFontComposite : public TRPPDFFont 
{
	typedef TRPPDFFont inherited;
	
protected:
	TRPPDFFontDescriptorComposite* FDescriptor;
	TRPPDFFontDescendant* FDescendant;
	AnsiString FCompositeFontEncoding;
	int FFirstChar;
	int FLastChar;
	void __fastcall SetFontData(void);
	
public:
	__fastcall virtual TRPPDFFontComposite(System::TObject* AOwner);
	__fastcall virtual ~TRPPDFFontComposite(void);
	virtual void __fastcall InitData(void);
	__property TRPPDFFontDescriptorComposite* Descriptor = {read=FDescriptor, write=FDescriptor};
	__property TRPPDFFontDescendant* Descendant = {read=FDescendant, write=FDescendant};
	__property AnsiString CompositeFontEncoding = {read=FCompositeFontEncoding, write=FCompositeFontEncoding};
	__property int FirstChar = {read=FFirstChar, write=FFirstChar, nodefault};
	__property int LastChar = {read=FLastChar, write=FLastChar, nodefault};
};


class DELPHICLASS TRPPDFXObject;
class PASCALIMPLEMENTATION TRPPDFXObject : public TRPPDFObject 
{
	typedef TRPPDFObject inherited;
	
private:
	int __fastcall GetImageHeight(void);
	int __fastcall GetImageWidth(void);
	
protected:
	Classes::TStream* FImageStream;
	AnsiString FObjectName;
	Jpeg::TJPEGImage* FImage;
	__property Classes::TStream* ImageStream = {read=FImageStream};
	
public:
	__fastcall virtual TRPPDFXObject(System::TObject* AOwner);
	__fastcall virtual ~TRPPDFXObject(void);
	virtual void __fastcall InitData(void);
	__property AnsiString ObjectName = {read=FObjectName, write=FObjectName};
	__property int Width = {read=GetImageWidth, nodefault};
	__property int Height = {read=GetImageHeight, nodefault};
	__property Jpeg::TJPEGImage* Image = {read=FImage, write=FImage};
};


typedef TRvRenderPDF TRpRenderPDF;
;

//-- var, const, procedure ---------------------------------------------------
static const Word NUM_CHARS = 0x100;
extern PACKAGE AnsiString PDFVersion;
static const Shortint DPI_MULTIPLIER = 0x48;
extern PACKAGE AnsiString FontNames[14];
extern PACKAGE AnsiString FontEncodingNames[5];
extern PACKAGE short FontWidthsCourier[256];
extern PACKAGE short FontWidthsHelvetica[256];
extern PACKAGE short FontWidthsHelveticaBold[256];
extern PACKAGE short FontWidthsHelveticaOblique[256];
extern PACKAGE short FontWidthsHelveticaBoldOblique[256];
extern PACKAGE short FontWidthsTimesRoman[256];
extern PACKAGE short FontWidthsTimesBold[256];
extern PACKAGE short FontWidthsTimesItalic[256];
extern PACKAGE short FontWidthsTimesBoldItalic[256];
extern PACKAGE short FontWidthsSymbol[256];
extern PACKAGE short FontWidthsZapfDingbats[256];

}	/* namespace Rprenderpdf */
using namespace Rprenderpdf;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpRenderPDF
