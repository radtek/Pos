// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpRender.pas' rev: 6.00

#ifndef QRpRenderHPP
#define QRpRenderHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpMBCS.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <QExtCtrls.hpp>	// Pascal unit
#include <QTypes.hpp>	// Pascal unit
#include <Qt.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrprender
{
//-- type declarations -------------------------------------------------------
struct TRPReportHeader
{
	Word FileVer;
	AnsiString Title;
	int PageNo;
	int PageCount;
	int FirstPage;
	int LastPage;
	double PaperWidth;
	double PaperHeight;
	double LeftWaste;
	double RightWaste;
	double TopWaste;
	double BottomWaste;
	Qrpdefine::TOrientation Orientation;
	short Bin;
	bool Collate;
	Qrpdefine::TDuplex Duplex;
	short Copies;
	short DMPaperSize;
	short DMPaperLength;
	short DMPaperWidth;
	AnsiString OutputName;
} ;

struct TRPPageHeader
{
	int PrevPagePos;
	int NextPagePos;
	int PageNo;
	double PaperWidth;
	double PaperHeight;
	Byte OldOrientation;
} ;

struct TFloatPoint
{
	double X;
	double Y;
} ;

class DELPHICLASS TRPConverter;
class DELPHICLASS TRPRender;
class PASCALIMPLEMENTATION TRPRender : public Qrpdefine::TRpComponent 
{
	typedef Qrpdefine::TRpComponent inherited;
	
protected:
	System::Set<Qrpdefine::TDestination, destPrinter, destCustomFile>  ValidDestination;
	double snPaperWidth;
	double snPaperHeight;
	double snPenWidth;
	Qrpdefine::TFrameMode snFrameMode;
	bool FActive;
	AnsiString FDisplayName;
	AnsiString FFileExtension;
	Classes::TStringList* FMacroData;
	AnsiString FOutputFileName;
	AnsiString FInputFileName;
	bool FRendering;
	Classes::TStream* FNDRStream;
	bool FIsInternalNDRStream;
	AnsiString FSelection;
	int FFirstPage;
	int FLastPage;
	Qrpdefine::TDecodeImageEvent FOnDecodeImage;
	TRPConverter* Converter;
	Qrpdefine::TBKMode FTextBKMode;
	Qgraphics::TColor FBKColor;
	bool FTransparentBitmaps;
	void __fastcall SetDisplayName(const AnsiString Value);
	AnsiString __fastcall StripRPTF(AnsiString AValue);
	virtual void __fastcall SetBKColor(Qgraphics::TColor Value);
	virtual void __fastcall AllowPreviewOnly(void);
	virtual void __fastcall AllowPrinterOnly(void);
	virtual void __fastcall AllowAll(void);
	bool __fastcall GetOutputInvalid(void);
	virtual Qrpdefine::TDestination __fastcall Destination(void);
	virtual void __fastcall Arc(const double X1, const double Y1, const double X2, const double Y2, const double X3, const double Y3, const double X4, const double Y4);
	virtual void __fastcall BrushChanged(System::TObject* Sender);
	virtual void __fastcall Chord(const double X1, const double Y1, const double X2, const double Y2, const double X3, const double Y3, const double X4, const double Y4);
	virtual void __fastcall CenterText(const AnsiString AText, const double X1, const double Y1);
	virtual void __fastcall Ellipse(const double X1, const double Y1, const double X2, const double Y2);
	virtual void __fastcall FontChanged(System::TObject* Sender);
	virtual void __fastcall LeftText(const AnsiString AText, const double X1, const double Y1);
	virtual void __fastcall LineTo(const double X1, const double Y1);
	virtual void __fastcall MoveTo(const double X1, const double Y1);
	virtual void __fastcall Pie(const double X1, const double Y1, const double X2, const double Y2, const double X3, const double Y3, const double X4, const double Y4);
	virtual void __fastcall PolyLine(const TFloatPoint * PolyLineArr, const int PolyLineArr_Size);
	virtual void __fastcall Polygon(const TFloatPoint * PolyLineArr, const int PolyLineArr_Size);
	virtual void __fastcall RightText(const AnsiString AText, const double X1, const double Y1);
	virtual void __fastcall PenChanged(System::TObject* Sender);
	virtual void __fastcall PrintImageRect(double X1, double Y1, double X2, double Y2, Classes::TStream* ImageStream, AnsiString ImageType);
	virtual void __fastcall PrintBitmapRect(const double X1, const double Y1, const double X2, const double Y2, Qgraphics::TBitmap* AGraphic);
	virtual void __fastcall PrintBitmap(const double X1, const double Y1, const double ScaleX, const double ScaleY, Qgraphics::TBitmap* AGraphic);
	virtual void __fastcall StretchDraw(const Types::TRect &ARect, Qgraphics::TGraphic* AGraphic);
	virtual void __fastcall Draw(const double X1, const double Y1, Qgraphics::TGraphic* AGraphic);
	virtual void __fastcall PrintSpaces(const AnsiString AText, const double X1, const double Y1, const double AWidth);
	virtual void __fastcall Rectangle(const double X1, const double Y1, const double X2, const double Y2);
	virtual void __fastcall FillRect(const Types::TRect &ARect);
	virtual void __fastcall TextRect(const Types::TRect &ARect, double X1, double Y1, AnsiString S1);
	virtual void __fastcall RoundRect(const double X1, const double Y1, const double X2, const double Y2, const double X3, const double Y3);
	virtual int __fastcall CountSpaces(AnsiString Text);
	virtual AnsiString __fastcall GetFirstWord(AnsiString &Line);
	virtual AnsiString __fastcall GetSpaces(AnsiString &Line);
	virtual void __fastcall AdjustXY(double &X1, double &Y1, double Width);
	virtual void __fastcall PrintRightWidth(double &X1, double &Y1, AnsiString Text, double Width);
	virtual void __fastcall SelectBin(AnsiString ABin)/* overload */;
	virtual void __fastcall SelectBin(int ABin)/* overload */;
	virtual void __fastcall PrintData(AnsiString Value);
	virtual void __fastcall PrintDataStream(Classes::TStream* Stream, int BufSize);
	virtual void __fastcall FrameRect(const Types::TRect &Rect);
	virtual void __fastcall BrushCopy(const Types::TRect &Dest, Qgraphics::TBitmap* Bitmap, const Types::TRect &Source, Qgraphics::TColor Color);
	virtual void __fastcall CopyRect(const Types::TRect &Dest, Qgraphics::TCanvas* Canvas, const Types::TRect &Source);
	virtual void __fastcall DrawFocusRect(const Types::TRect &Rect);
	virtual void __fastcall DocBegin(void);
	virtual void __fastcall DocEnd(void);
	virtual void __fastcall PageBegin(void);
	virtual void __fastcall PageEnd(void);
	virtual int __fastcall GetXDPI(void);
	virtual int __fastcall GetYDPI(void);
	__property bool IsInternalNDRStream = {read=FIsInternalNDRStream, write=FIsInternalNDRStream, nodefault};
	__property bool TransparentBitmaps = {read=FTransparentBitmaps, write=FTransparentBitmaps, nodefault};
	__property Qrpdefine::TBKMode TextBKMode = {read=FTextBKMode, write=FTextBKMode, nodefault};
	__property Qgraphics::TColor BKColor = {write=SetBKColor, nodefault};
	__property bool OutputInvalid = {read=GetOutputInvalid, nodefault};
	
public:
	__fastcall virtual TRPRender(Classes::TComponent* AOwner);
	__fastcall virtual ~TRPRender(void);
	virtual void __fastcall Render(Classes::TStream* ANDRStream = (Classes::TStream*)(0x0))/* overload */;
	void __fastcall Render(AnsiString AInputFileName)/* overload */;
	virtual void __fastcall RenderPage(int PageNum);
	virtual void __fastcall PrintRender(Classes::TStream* ANDRStream, AnsiString AOutputFileName);
	virtual int __fastcall XI2D(double Pos);
	virtual int __fastcall YI2D(double Pos);
	__property AnsiString Selection = {read=FSelection, write=FSelection};
	__property int FirstPage = {read=FFirstPage, write=FFirstPage, nodefault};
	__property int LastPage = {read=FLastPage, write=FLastPage, nodefault};
	__property Qrpdefine::TFrameMode FrameMode = {read=snFrameMode, write=snFrameMode, nodefault};
	__property double PenWidth = {read=snPenWidth, write=snPenWidth};
	__property double PaperWidth = {read=snPaperWidth, write=snPaperWidth};
	__property double PaperHeight = {read=snPaperHeight, write=snPaperHeight};
	__property Classes::TStringList* MacroData = {read=FMacroData, write=FMacroData};
	__property int XDPI = {read=GetXDPI, nodefault};
	__property int YDPI = {read=GetYDPI, nodefault};
	__property AnsiString InputFileName = {read=FInputFileName, write=FInputFileName};
	__property AnsiString OutputFileName = {read=FOutputFileName, write=FOutputFileName};
	__property bool Rendering = {read=FRendering, nodefault};
	__property Classes::TStream* NDRStream = {read=FNDRStream, write=FNDRStream};
	__property Qrpdefine::TDecodeImageEvent OnDecodeImage = {read=FOnDecodeImage, write=FOnDecodeImage};
	
__published:
	__property bool Active = {read=FActive, write=FActive, default=1};
	__property AnsiString DisplayName = {read=FDisplayName, write=SetDisplayName};
	__property AnsiString FileExtension = {read=FFileExtension, write=FFileExtension};
};


class PASCALIMPLEMENTATION TRPConverter : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Qgraphics::TBitmap* FontBitmap;
	System::TDateTime FReportDateTime;
	int FFirstPage;
	int FLastPage;
	int FPageNo;
	int FPageCount;
	AnsiString FTitle;
	Classes::TStream* FStream;
	TRPRender* FRenderer;
	bool FReuseGraphic;
	Word FFileVer;
	int FCurrPagePos;
	int FPrevPagePos;
	int FNextPagePos;
	double FXPos;
	double FYPos;
	Classes::TStringList* VarList;
	void __fastcall GetNextRPTFCommand(AnsiString RPTFStr, int &Pos, char &Command, AnsiString &Param);
	AnsiString __fastcall NextPiece(AnsiString InStr, int &Pos);
	void __fastcall ParseRPTF(AnsiString InStr, int &Pos, AnsiString &TextStr, AnsiString &RPTFStr);
	void __fastcall ProcessRPTF(AnsiString RPTFStr);
	virtual void __fastcall CreateGraphic(Qgraphics::TGraphic* &Value);
	virtual void __fastcall LoadGraphic(Qgraphics::TGraphic* Value);
	void __fastcall MatchCode(Byte pyValue);
	void __fastcall ProcessPage(void);
	Byte __fastcall ReadByte(void);
	int __fastcall ReadLongint(void);
	Types::TPoint __fastcall ReadPoint();
	void __fastcall ReadPageHeader(void);
	void __fastcall ReadReportHeader(void);
	Types::TRect __fastcall ReadRect();
	short __fastcall ReadSmallInt(void);
	float __fastcall ReadSingle(void);
	AnsiString __fastcall ReadString();
	__property System::TDateTime ReportDateTime = {read=FReportDateTime, write=FReportDateTime};
	virtual AnsiString __fastcall GetDeviceName();
	virtual AnsiString __fastcall GetDriverName();
	virtual AnsiString __fastcall GetPort();
	AnsiString __fastcall ConvertMacro(Qrpdefine::TMacroID MacroID);
	void __fastcall ReadBuf(void *Buf, int Len);
	AnsiString __fastcall ReadPIVar();
	AnsiString __fastcall GetPIVar(AnsiString VarName);
	void __fastcall SetPIVar(AnsiString VarName, AnsiString VarValue);
	void __fastcall ClearVarList(void);
	void __fastcall ProcessString(AnsiString &Text);
	__property AnsiString DeviceName = {read=GetDeviceName};
	__property AnsiString DriverName = {read=GetDriverName};
	__property AnsiString Port = {read=GetPort};
	void __fastcall PrintRPTF(double AX, double AY, AnsiString AText, char AJustify);
	void __fastcall PrintRPTFBlock(double AX, double AY, AnsiString AText, double AWidth);
	void __fastcall ReadGraphic(Qgraphics::TGraphic* Value);
	
public:
	TRPReportHeader ReportHeader;
	TRPPageHeader PageHeader;
	float CurrX;
	float CurrY;
	Qgraphics::TFont* Font;
	Qrpdefine::TSaveFont FontData;
	float LineHeight;
	Qgraphics::TBrush* Brush;
	Qgraphics::TPen* Pen;
	double __fastcall RPTFTextWidth(AnsiString Value);
	void __fastcall SetBrushColor(void);
	void __fastcall SetFontColor(void);
	void __fastcall SetFontName(void);
	void __fastcall SetFontHeight(void);
	void __fastcall SetFontBold(void);
	void __fastcall SetFontUnderline(void);
	void __fastcall SetFontItalic(void);
	void __fastcall SetFontStrikeout(void);
	void __fastcall SetFontSuperscript(void);
	void __fastcall SetFontSubscript(void);
	void __fastcall SetFontPitch(void);
	void __fastcall SetFontAlign(void);
	void __fastcall SetFontRotation(void);
	void __fastcall SetFontCharset(void);
	void __fastcall DoLeftText(void);
	void __fastcall DoCenterText(void);
	void __fastcall DoRightText(void);
	void __fastcall DoPrintSpaces(void);
	void __fastcall DoRectangle(void);
	void __fastcall DoArc(void);
	void __fastcall DoChord(void);
	void __fastcall DoEllipse(void);
	void __fastcall DoMoveTo(void);
	void __fastcall DoLineTo(void);
	void __fastcall SetPenColor(void);
	void __fastcall DoBrushCopy(void);
	void __fastcall DoCopyRect(void);
	void __fastcall DoDraw(void);
	void __fastcall DoDrawFocusRect(void);
	void __fastcall DoFillRect(void);
	void __fastcall DoFloodFill(void);
	void __fastcall DoFrameRect(void);
	void __fastcall DoPie(void);
	void __fastcall DoPolygon(void);
	void __fastcall DoPolyline(void);
	void __fastcall DoPrintBitmap(void);
	void __fastcall DoPrintBitmapRect(void);
	void __fastcall DoRoundRect(void);
	void __fastcall DoStretchDraw(void);
	void __fastcall DoTextRect(void);
	void __fastcall SetFont(void);
	void __fastcall SetTextBkMode(void);
	void __fastcall SetBkColor(void);
	void __fastcall SetScaleX(void);
	void __fastcall SetScaleY(void);
	void __fastcall DoPrintData(void);
	void __fastcall DoPrintDataStream(void);
	void __fastcall DoTransBitmap(void);
	void __fastcall DoSetFrameMode(void);
	void __fastcall DoPrintImageRect(void);
	void __fastcall DoVariableData(void);
	void __fastcall SetBrush(void);
	void __fastcall SetBrushStyle(void);
	void __fastcall SetBrushBitmap(void);
	void __fastcall SetPen(void);
	void __fastcall SetPenMode(void);
	void __fastcall SetPenStyle(void);
	void __fastcall SetPenWidth(void);
	void __fastcall DoResetLineHeight(void);
	void __fastcall DoAllowPreviewOnly(void);
	void __fastcall DoAllowPrinterOnly(void);
	void __fastcall DoAllowAll(void);
	void __fastcall SetOrientation(void);
	void __fastcall SetBin(void);
	__fastcall virtual TRPConverter(Classes::TStream* AStream, TRPRender* ARenderer);
	__fastcall virtual ~TRPConverter(void);
	virtual void __fastcall Generate(const int APageNo = 0x0, int APageCount = 0x0);
	bool __fastcall PageInRange(int PageNum);
	void __fastcall GotoPage(const int APageNum);
	__property Word FileVer = {read=FFileVer, nodefault};
	__property TRPRender* Renderer = {read=FRenderer};
	__property int PageNo = {read=FPageNo, nodefault};
	__property int PageCount = {read=FPageCount, nodefault};
	__property AnsiString Title = {read=FTitle};
	__property bool ReuseGraphic = {read=FReuseGraphic, nodefault};
};


typedef TMetaClass*TRPRenderClass;

class DELPHICLASS TRPRenderStream;
class PASCALIMPLEMENTATION TRPRenderStream : public TRPRender 
{
	typedef TRPRender inherited;
	
protected:
	Classes::TStream* FActiveStream;
	Classes::TStream* FOutputStream;
	Classes::TStream* FOwnedStream;
	__property Classes::TStream* ActiveStream = {read=FActiveStream, write=FActiveStream};
	void __fastcall SetOutputStream(Classes::TStream* AStream);
	virtual void __fastcall Print(const AnsiString AData);
	virtual void __fastcall PrintF(const AnsiString AData, const System::TVarRec * AArgs, const int AArgs_Size);
	virtual void __fastcall PrintLn(const AnsiString AData = "");
	virtual void __fastcall PrintLnF(const AnsiString AData, const System::TVarRec * AArgs, const int AArgs_Size);
	virtual void __fastcall FreeFileStream(void);
	
public:
	__fastcall virtual ~TRPRenderStream(void);
	void __fastcall InitFileStream(const AnsiString AFileName);
	__property Classes::TStream* OutputStream = {read=FOutputStream, write=SetOutputStream};
	__property Classes::TStream* OwnedStream = {read=FOwnedStream};
public:
	#pragma option push -w-inl
	/* TRPRender.Create */ inline __fastcall virtual TRPRenderStream(Classes::TComponent* AOwner) : TRPRender(AOwner) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TGraphicFormat { gfJPEG, gfBMP, gfMTF };
#pragma option pop

class DELPHICLASS TNDGraphic;
class PASCALIMPLEMENTATION TNDGraphic : public Qgraphics::TGraphic 
{
	typedef Qgraphics::TGraphic inherited;
	
protected:
	Qt::QPixmapH* FPixmap;
	Qt::QImageH* FImage;
	Classes::TStream* FStream;
	int FCompression;
	TGraphicFormat FFormat;
	void __fastcall SetCompression(int AValue);
	AnsiString __fastcall GetFormatString();
	virtual void __fastcall Draw(Qgraphics::TCanvas* ACanvas, const Types::TRect &Rect);
	virtual bool __fastcall GetEmpty(void);
	virtual int __fastcall GetHeight(void);
	virtual int __fastcall GetWidth(void);
	virtual void __fastcall SetHeight(int Value);
	virtual void __fastcall SetWidth(int Value);
	void __fastcall FreePixmap(void);
	
public:
	__fastcall virtual TNDGraphic(void);
	__fastcall virtual ~TNDGraphic(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	virtual void __fastcall LoadFromMimeSource(Qtypes::TMimeSource* MimeSource);
	virtual void __fastcall SaveToMimeSource(Qtypes::TClxMimeSource* MimeSource);
	virtual void __fastcall LoadFromStream(Classes::TStream* Stream);
	virtual void __fastcall SaveToStream(Classes::TStream* Stream);
	virtual void __fastcall SaveToFile(const AnsiString Filename);
	void __fastcall Compress(void);
	__property int CompressionQuality = {read=FCompression, write=SetCompression, nodefault};
	__property TGraphicFormat Format = {read=FFormat, write=FFormat, nodefault};
	__property Qt::QPixmapH* Handle = {read=FPixmap};
};


//-- var, const, procedure ---------------------------------------------------
static const Word FontDPI = 0x4e0;
extern PACKAGE Classes::TStringList* RenderList;
extern PACKAGE void __fastcall RaveRegisterRender(const AnsiString DisplayName, const TRPRender* RenderObject);
extern PACKAGE void __fastcall RaveUnRegisterRender(const AnsiString DisplayName, const TRPRender* RenderObject);
extern PACKAGE void __fastcall GetRenderList(const Classes::TStrings* AList);
extern PACKAGE AnsiString __fastcall GetRenderFilter();

}	/* namespace Qrprender */
using namespace Qrprender;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpRender
