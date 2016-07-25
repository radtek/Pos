// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpBase.pas' rev: 6.00

#ifndef QRpBaseHPP
#define QRpBaseHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDevice.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <QExtCtrls.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Qt.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------
#undef CreateFont
#undef ResetPrinter

namespace Qrpbase
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRPBaseComponent;
class PASCALIMPLEMENTATION TRPBaseComponent : public Qrpdefine::TRpComponent 
{
	typedef Qrpdefine::TRpComponent inherited;
	
public:
	#pragma option push -w-inl
	/* TRpComponent.Create */ inline __fastcall virtual TRPBaseComponent(Classes::TComponent* AOwner) : Qrpdefine::TRpComponent(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TRPBaseComponent(void) { }
	#pragma option pop
	
};


class DELPHICLASS TBaseReport;
class DELPHICLASS TBaseMemoBuf;
class PASCALIMPLEMENTATION TBaseReport : public TRPBaseComponent 
{
	typedef TRPBaseComponent inherited;
	
protected:
	Qrpdefine::TTab *TabHead;
	Qrpdefine::TTab *TabTail;
	Qrpdefine::TPosition SavePosArr[10];
	Qrpdefine::TSaveTab SaveTabsArr[10];
	Qrpdefine::TSaveFont SaveFontArr[10];
	Qrpdefine::TPositionNode *PosStack;
	Qrpdefine::TFontNode *FontStack;
	Qrpdefine::TTabNode *TabsStack;
	bool BlockTextOp;
	Qrpdefine::TSaveFont BaseFont;
	Qrpdefine::TFontNode *FontList;
	Classes::TNotifyEvent FontSaveOnChange;
	HFONT SaveFontHandle;
	bool BaseFontChanged;
	bool DisableFontChange;
	HFONT SaveCanvasFont;
	System::Set<Qrpdefine::TDestination, destPrinter, destCustomFile>  ValidDestination;
	int FJobPages;
	Classes::TStrings* FMacroData;
	Qrpdefine::TTabJustify FParaJustify;
	double FFixedTabWidth;
	Qrpdefine::TFrameMode FFrameMode;
	Qrpdefine::TTab *CurrentTab;
	bool OnTab;
	bool BeforeTabs;
	int AddX;
	int AddY;
	Classes::TList* BeginPageList;
	Classes::TList* EndPageList;
	Qrpdefine::TLineList *VLineList;
	Qrpdefine::TLineList *HLineList;
	double LastLineX;
	double LastLineY;
	bool FNoBufferLine;
	bool FNoNTColorFix;
	bool FUseDrawText;
	double FLeftWaste;
	double FRightWaste;
	double FTopWaste;
	double FBottomWaste;
	Qt::QFontMetricsH* Metrict;
	double FAscentHeight;
	double FDescentHeight;
	double FLineHeight;
	AnsiString FOutputName;
	int FPrinterIndex;
	double FLeftMargin;
	double FRightMargin;
	double FTopMargin;
	double FBottomMargin;
	double FLeftSection;
	double FRightSection;
	double FTopSection;
	double FBottomSection;
	double FCursorXPos;
	double FCursorYPos;
	double FColumnWidth;
	double FColumnBetween;
	bool FThreaded;
	System::TDateTime FReportDateTime;
	Qrpdefine::TTabJustify FTabJustify;
	int FCurrentPage;
	bool FPageInvalid;
	int FXDPI;
	int FYDPI;
	int FLineNum;
	int FColumnNum;
	int FColumns;
	char *FDevice;
	char *FDriver;
	char *FPort;
	bool FAborted;
	bool FPrinting;
	double FOriginX;
	double FOriginY;
	int FFirstPage;
	int FLastPage;
	AnsiString FSelection;
	Qstdctrls::TLabel* FStatusLabel;
	AnsiString FStatusFormat;
	Classes::TStrings* FStatusText;
	Qrpdefine::TLineHeightMethod FLineHeightMethod;
	int FLinesPerInch;
	Qrpdefine::TPrintUnits FUnits;
	double FUnitsFactor;
	double FLeftMarginWidth;
	double FRightMarginWidth;
	double FTopMarginWidth;
	double FBottomMarginWidth;
	int FTabShade;
	Qgraphics::TColor FTabColor;
	Qgraphics::TColor FBoxLineColor;
	AnsiString FTitle;
	Qrpdefine::TBKMode FTextBKMode;
	Qrpdefine::TOrientation FOrientation;
	Qrpdefine::TOrientation FNewOrientation;
	int FCopies;
	double FScaleX;
	double FScaleY;
	double FZoom;
	double FNoPrinterPageHeight;
	double FNoPrinterPageWidth;
	bool FTransparentBitmaps;
	bool FIgnoreRPTF;
	int FCurrentPass;
	int FTotalPasses;
	double FTextFieldWidth;
	int FPageNumOffset;
	Qrpdevice::TRPBaseDevice* FLocalDevice;
	Classes::TNotifyEvent FOnPrint;
	Qrpdefine::TPrintPageEvent FOnPrintPage;
	Classes::TNotifyEvent FOnBeforePrint;
	Classes::TNotifyEvent FOnAfterPrint;
	Classes::TNotifyEvent FOnNewPage;
	Classes::TNotifyEvent FOnNewColumn;
	Classes::TNotifyEvent FOnPrintHeader;
	Classes::TNotifyEvent FOnPrintFooter;
	Classes::TNotifyEvent FOnEndOfSection;
	Qrpdefine::TDecodeImageEvent FOnDecodeImage;
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	void __fastcall SetStatusLabel(Qstdctrls::TLabel* Value);
	virtual void __fastcall PrintEvent(void);
	virtual bool __fastcall PrintPageEvent(void);
	virtual void __fastcall BeforePrintEvent(void);
	virtual void __fastcall AfterPrintEvent(void);
	virtual void __fastcall NewPageEvent(void);
	virtual void __fastcall NewColumnEvent(void);
	virtual void __fastcall PrintHeaderEvent(void);
	virtual void __fastcall PrintFooterEvent(void);
	virtual void __fastcall EndOfSectionEvent(void);
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	void __fastcall ExecuteReportEvent(Classes::TList* ReportEventList);
	virtual void __fastcall FontOnChange(System::TObject* Sender);
	virtual void __fastcall GrabFontOnChange(void);
	virtual void __fastcall ReleaseFontOnChange(void);
	void __fastcall AdjustXY(double Width);
	void __fastcall ReleaseTabs(Qrpdefine::PTab &TabPtr);
	void __fastcall UpdateFont(void);
	void __fastcall InitPrinter(void);
	void __fastcall ClearLineLists(void);
	bool __fastcall BufferLine(double X1, double Y1);
	void __fastcall DrawLineLists(System::TObject* Sender);
	int __fastcall GetCursorXPos(void);
	int __fastcall GetCursorYPos(void);
	virtual Qrpdefine::TDestination __fastcall Destination(void);
	void __fastcall PrintPiece(AnsiString Text);
	virtual void __fastcall PrintCenterInch(AnsiString Text, double Pos) = 0 ;
	virtual void __fastcall PrintLeftInch(AnsiString Text, double Pos) = 0 ;
	virtual void __fastcall PrintRightInch(AnsiString Text, double Pos) = 0 ;
	virtual void __fastcall PrintBlockInch(AnsiString Text, double Pos, double Width) = 0 ;
	void __fastcall DrawTabLine(bool DrawLine, int LineWidth, double X1, double Y1, double XAdd, double YAdd, Qgraphics::TPen* SavePen);
	AnsiString __fastcall ConvertMacro(Qrpdefine::TMacroID MacroID);
	void __fastcall ProcessString(AnsiString &Text);
	void __fastcall CheckPrinting(void);
	void __fastcall CheckNotPrinting(void);
	bool __fastcall PageInRange(int PageNum);
	void __fastcall ClearFontList(void);
	virtual Qgraphics::TCanvas* __fastcall GetCanvas(void);
	void __fastcall SetLine(int Value);
	void __fastcall SetColumn(int Value);
	double __fastcall GetLeftWaste(void);
	double __fastcall GetRightWaste(void);
	double __fastcall GetTopWaste(void);
	double __fastcall GetBottomWaste(void);
	double __fastcall GetXPos(void);
	void __fastcall SetXPos(double Value);
	double __fastcall GetYPos(void);
	void __fastcall SetYPos(double Value);
	double __fastcall GetPageHeight(void);
	double __fastcall GetPageWidth(void);
	bool __fastcall GetBold(void);
	void __fastcall SetBold(bool Value);
	bool __fastcall GetUnderline(void);
	void __fastcall SetUnderline(bool Value);
	bool __fastcall GetItalic(void);
	void __fastcall SetItalic(bool Value);
	bool __fastcall GetStrikeout(void);
	void __fastcall SetStrikeout(bool Value);
	int __fastcall GetFontRotation(void);
	void __fastcall SetFontRotation(int Value);
	Qgraphics::TColor __fastcall GetFontColor(void);
	void __fastcall SetFontColor(Qgraphics::TColor Value);
	AnsiString __fastcall GetFontName();
	void __fastcall SetFontName(AnsiString Value);
	double __fastcall GetFontSize(void);
	void __fastcall SetFontSize(double Value);
	HFONT __fastcall GetFontHandle(void);
	Qgraphics::TFontPitch __fastcall GetFontPitch(void);
	void __fastcall SetFontPitch(Qgraphics::TFontPitch Value);
	Byte __fastcall GetFontCharset(void);
	void __fastcall SetFontCharset(Byte Value);
	Qrpdefine::TFontAlign __fastcall GetFontAlign(void);
	void __fastcall SetFontAlign(Qrpdefine::TFontAlign Value);
	bool __fastcall GetSuperscript(void);
	void __fastcall SetSuperscript(bool Value);
	bool __fastcall GetSubscript(void);
	void __fastcall SetSubscript(bool Value);
	double __fastcall GetFontTop(void);
	void __fastcall SetFontTop(double Value);
	double __fastcall GetFontBaseline(void);
	void __fastcall SetFontBaseline(double Value);
	double __fastcall GetFontBottom(void);
	void __fastcall SetFontBottom(double Value);
	double __fastcall GetLineTop(void);
	void __fastcall SetLineTop(double Value);
	double __fastcall GetLineMiddle(void);
	void __fastcall SetLineMiddle(double Value);
	double __fastcall GetLineBottom(void);
	void __fastcall SetLineBottom(double Value);
	double __fastcall GetLineHeight(void);
	void __fastcall SetLineHeight(double Value);
	double __fastcall GetAscentHeight(void);
	double __fastcall GetDescentHeight(void);
	double __fastcall GetFontHeight(void);
	void __fastcall SetFontHeight(double Value);
	double __fastcall GetFontWidth(void);
	void __fastcall SetFontWidth(double Value);
	bool __fastcall GetOutputInvalid(void);
	virtual void __fastcall SetFrameMode(Qrpdefine::TFrameMode Value);
	Windows::PDeviceModeA __fastcall GetDevMode(void);
	int __fastcall GetPrinterIndex(void);
	void __fastcall SetPrinterIndex(int Value);
	AnsiString __fastcall GetDeviceName();
	AnsiString __fastcall GetDriverName();
	AnsiString __fastcall GetPort();
	double __fastcall GetOriginX(void);
	void __fastcall SetOriginX(double Value);
	double __fastcall GetOriginY(void);
	void __fastcall SetOriginY(double Value);
	Qgraphics::TColor __fastcall GetBKColor(void);
	virtual void __fastcall SetBKColor(Qgraphics::TColor Value);
	virtual void __fastcall SetScaleX(double Value);
	virtual void __fastcall SetScaleY(double Value);
	virtual void __fastcall SetTextBKMode(Qrpdefine::TBKMode Value);
	double __fastcall GetLeftSection(void);
	void __fastcall SetLeftSection(double Value);
	double __fastcall GetRightSection(void);
	void __fastcall SetRightSection(double Value);
	double __fastcall GetTopSection(void);
	void __fastcall SetTopSection(double Value);
	double __fastcall GetBottomSection(void);
	void __fastcall SetBottomSection(double Value);
	void __fastcall SetStatusText(Classes::TStrings* Value);
	void __fastcall SetMacroData(Classes::TStrings* Value);
	void __fastcall SetLinesPerInch(int Value);
	void __fastcall SetUnits(Qrpdefine::TPrintUnits Value);
	void __fastcall SetUnitsFactor(double Value);
	double __fastcall GetLeftMargin(void);
	void __fastcall SetLeftMargin(double Value);
	double __fastcall GetRightMargin(void);
	void __fastcall SetRightMargin(double Value);
	double __fastcall GetTopMargin(void);
	void __fastcall SetTopMargin(double Value);
	double __fastcall GetBottomMargin(void);
	void __fastcall SetBottomMargin(double Value);
	void __fastcall SetTabShade(int Value);
	void __fastcall SetCopies(int Value);
	virtual void __fastcall SetOrientation(Qrpdefine::TOrientation Value);
	void __fastcall SetOrientationActive(void);
	double __fastcall GetColumnStart(void);
	double __fastcall GetColumnEnd(void);
	double __fastcall GetColumnWidth(void);
	void __fastcall SetOutputName(AnsiString Value);
	Classes::TStrings* __fastcall GetBins(void);
	Classes::TStrings* __fastcall GetPapers(void);
	Classes::TStrings* __fastcall GetFonts(void);
	Classes::TStrings* __fastcall GetPrinters(void);
	int __fastcall GetMaxCopies(void);
	Qrpdefine::TDuplex __fastcall GetDuplex(void);
	void __fastcall SetDuplex(Qrpdefine::TDuplex Value);
	bool __fastcall GetCollate(void);
	void __fastcall SetCollate(bool Value);
	int __fastcall GetRelativePage(void);
	Qrpdevice::TRPBaseDevice* __fastcall GetLocalDevice(void);
	void __fastcall ProcessMessages(void);
	
public:
	Qrpdefine::TSaveFont __fastcall GetBaseFont();
	void __fastcall SetBaseFont(const Qrpdefine::TSaveFont &Font);
	void __fastcall SelectRPFont(void);
	void __fastcall SelectCanvasFont(void);
	double __fastcall LineStartPos(void);
	void __fastcall AddReportEvent(Qrpdefine::TReportEventType ReportEventType, Classes::TNotifyEvent ReportEvent);
	void __fastcall RemoveReportEvent(Qrpdefine::TReportEventType ReportEventType, Classes::TNotifyEvent ReportEvent);
	virtual void __fastcall TabRectangle(double X1, double Y1, double X2, double Y2) = 0 ;
	virtual tagSIZE __fastcall GetTextExtent(AnsiString Value);
	virtual void __fastcall CopyRect(const Types::TRect &Dest, Qgraphics::TCanvas* Canvas, const Types::TRect &Source) = 0 ;
	virtual void __fastcall FillRect(const Types::TRect &Rect) = 0 ;
	virtual void __fastcall DrawFocusRect(const Types::TRect &Rect) = 0 ;
	virtual void __fastcall Arc(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4) = 0 ;
	virtual void __fastcall Chord(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4) = 0 ;
	virtual void __fastcall Draw(double X, double Y, Qgraphics::TGraphic* Graphic) = 0 ;
	virtual void __fastcall Ellipse(double X1, double Y1, double X2, double Y2) = 0 ;
	virtual void __fastcall LineTo(double X, double Y) = 0 ;
	virtual void __fastcall MoveTo(double X, double Y) = 0 ;
	virtual void __fastcall Pie(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4) = 0 ;
	virtual void __fastcall Polygon(const Types::TPoint * Points, const int Points_Size) = 0 ;
	virtual void __fastcall Polyline(const Types::TPoint * Points, const int Points_Size) = 0 ;
	virtual void __fastcall PrintBitmap(double X, double Y, double ScaleX, double ScaleY, Qgraphics::TBitmap* Bitmap) = 0 ;
	virtual void __fastcall PrintBitmapRect(double X1, double Y1, double X2, double Y2, Qgraphics::TBitmap* Bitmap) = 0 ;
	virtual void __fastcall PrintImageRect(double X1, double Y1, double X2, double Y2, Classes::TStream* ImageStream, AnsiString ImageType) = 0 ;
	double __fastcall CalcGraphicWidth(double Height, Qgraphics::TGraphic* Graphic);
	double __fastcall CalcGraphicHeight(double Width, Qgraphics::TGraphic* Graphic);
	virtual void __fastcall Rectangle(double X1, double Y1, double X2, double Y2) = 0 ;
	virtual void __fastcall RoundRect(double X1, double Y1, double X2, double Y2, double X3, double Y3) = 0 ;
	virtual void __fastcall StretchDraw(const Types::TRect &Rect, Qgraphics::TGraphic* Graphic) = 0 ;
	virtual void __fastcall TextRect(const Types::TRect &Rect, double X, double Y, const AnsiString Text) = 0 ;
	virtual void __fastcall PrintData(AnsiString Value) = 0 ;
	virtual void __fastcall PrintDataStream(Classes::TStream* Stream, int BufSize) = 0 ;
	bool __fastcall NoPrinters(void);
	virtual void __fastcall AllowPreviewOnly(void);
	virtual void __fastcall AllowPrinterOnly(void);
	virtual void __fastcall AllowAll(void);
	void __fastcall AssignFont(Qgraphics::TFont* Font);
	virtual void __fastcall UnregisterGraphic(int Index);
	virtual void __fastcall RegisterGraphic(int Index);
	virtual void __fastcall ReuseGraphic(int Index);
	int __fastcall XI2D(double Pos);
	int __fastcall YI2D(double Pos);
	int __fastcall XU2D(double Pos);
	int __fastcall YU2D(double Pos);
	double __fastcall XD2I(int Pos);
	double __fastcall YD2I(int Pos);
	double __fastcall XD2U(int Pos);
	double __fastcall YD2U(int Pos);
	double __fastcall XI2U(double Pos);
	double __fastcall YI2U(double Pos);
	double __fastcall XU2I(double Pos);
	double __fastcall YU2I(double Pos);
	__fastcall virtual TBaseReport(Classes::TComponent* AOwner);
	__fastcall virtual ~TBaseReport(void);
	virtual void __fastcall Start(void) = 0 ;
	virtual void __fastcall Finish(void) = 0 ;
	virtual void __fastcall Execute(void);
	virtual void __fastcall NewPage(void);
	virtual void __fastcall NewPara(void);
	virtual void __fastcall AbortPage(void);
	bool __fastcall ShowPrintDialog(void);
	bool __fastcall ShowPrinterSetupDialog(void);
	void __fastcall Abort(void);
	void __fastcall UpdateStatus(void);
	void __fastcall Reset(void);
	void __fastcall ResetSection(void);
	void __fastcall ResetPrinter(void);
	bool __fastcall SetPaperSize(int Size, double Width, double Height);
	void __fastcall ReleasePrinter(void);
	void __fastcall RecoverPrinter(void);
	virtual bool __fastcall SelectBin(AnsiString BinName);
	bool __fastcall SelectPaper(AnsiString PaperName);
	bool __fastcall SelectPrinter(AnsiString PrinterName);
	bool __fastcall SupportBin(int BinNum);
	bool __fastcall SupportPaper(int PaperNum);
	bool __fastcall SupportOrientation(void);
	bool __fastcall SupportDuplex(void);
	bool __fastcall SupportCollate(void);
	__property Classes::TStrings* Bins = {read=GetBins};
	__property Classes::TStrings* Papers = {read=GetPapers};
	__property Classes::TStrings* Fonts = {read=GetFonts};
	__property Classes::TStrings* Printers = {read=GetPrinters};
	__property int MaxCopies = {read=GetMaxCopies, nodefault};
	__property Qrpdefine::TDuplex Duplex = {read=GetDuplex, write=SetDuplex, nodefault};
	__property bool Collate = {read=GetCollate, write=SetCollate, nodefault};
	__property System::TDateTime ReportDateTime = {read=FReportDateTime, write=FReportDateTime};
	__property Qrpdefine::TFrameMode FrameMode = {read=FFrameMode, write=SetFrameMode, default=0};
	__property Qrpdevice::TRPBaseDevice* LocalDevice = {read=GetLocalDevice, write=FLocalDevice};
	__property int JobPages = {read=FJobPages, nodefault};
	void __fastcall Cr(void);
	void __fastcall Lf(void);
	virtual void __fastcall ResetLineHeight(void);
	void __fastcall AdjustLine(void);
	void __fastcall CrLf(void);
	void __fastcall NewLine(void);
	void __fastcall SoftLine(void);
	void __fastcall NewColumn(void);
	void __fastcall GotoHeader(void);
	void __fastcall GotoFooter(void);
	void __fastcall GotoXY(double NewXPos, double NewYPos);
	void __fastcall Home(void);
	void __fastcall SetTopOfPage(void);
	int __fastcall LinesLeft(void);
	int __fastcall ColumnLinesLeft(void);
	bool __fastcall PopPos(void);
	bool __fastcall PushPos(void);
	bool __fastcall RestorePos(int Index);
	bool __fastcall SavePos(int Index);
	bool __fastcall PopTabs(void);
	bool __fastcall PushTabs(void);
	bool __fastcall RestoreTabs(int Index);
	bool __fastcall SaveTabs(int Index);
	bool __fastcall PopFont(void);
	bool __fastcall PushFont(void);
	bool __fastcall RestoreFont(int Index);
	bool __fastcall SaveFont(int Index);
	void __fastcall SetColumns(int NewColumns, double Between);
	void __fastcall SetColumnWidth(double Width, double Between);
	void __fastcall ClearColumns(void);
	void __fastcall SetTab(double NewPos, Qrpdefine::TPrintJustify NewJustify, double NewWidth, double NewMargin, Byte NewLines, Byte NewShade);
	void __fastcall ClearTabs(void);
	void __fastcall ClearAllTabs(void);
	void __fastcall ResetTabs(void);
	virtual void __fastcall Tab(int LeftWidth, int RightWidth, int TopWidth, int BottomWidth, int ShadeOverride);
	Qrpdefine::PTab __fastcall GetTab(int Index);
	double __fastcall TabStart(int Index);
	double __fastcall TabEnd(int Index);
	double __fastcall TabWidth(int Index);
	void __fastcall FinishTabBox(int Width);
	Types::TPoint __fastcall CreatePoint(double X, double Y);
	Types::TRect __fastcall CreateRect(double X1, double Y1, double X2, double Y2);
	virtual double __fastcall TextWidth(AnsiString Text);
	int __fastcall MemoLines(TBaseMemoBuf* MemoBuf);
	AnsiString __fastcall GetMemoLine(TBaseMemoBuf* MemoBuf, bool &Eol);
	virtual void __fastcall PrintMemo(TBaseMemoBuf* MemoBuf, int Lines, bool PrintTabs);
	void __fastcall PrintJustify(AnsiString Text, double Pos, Qrpdefine::TPrintJustify Justify, double Margin, double Width);
	void __fastcall Print(AnsiString Text);
	void __fastcall PrintCenter(AnsiString Text, double Pos);
	void __fastcall PrintHeader(AnsiString Text, Qrpdefine::TPrintJustify Justify);
	void __fastcall PrintFooter(AnsiString Text, Qrpdefine::TPrintJustify Justify);
	void __fastcall PrintLeft(AnsiString Text, double Pos);
	void __fastcall PrintBlock(AnsiString Text, double Pos, double Width);
	void __fastcall Println(AnsiString Text);
	void __fastcall PrintRight(AnsiString Text, double Pos);
	void __fastcall PrintXY(double X, double Y, AnsiString Text);
	void __fastcall PrintCharJustify(AnsiString Text, char Ch, double Pos);
	virtual void __fastcall PrintTab(AnsiString Text);
	AnsiString __fastcall TruncateText(AnsiString Value, double Width);
	AnsiString __fastcall Macro(Qrpdefine::TMacroID MacroID);
	virtual AnsiString __fastcall PIVar(AnsiString VarName);
	virtual void __fastcall SetPIVar(AnsiString VarName, AnsiString VarValue);
	virtual AnsiString __fastcall GetPIVar(AnsiString VarName);
	Qgraphics::TBrush* __fastcall CreateBrush(Qgraphics::TColor NewColor, Qgraphics::TBrushStyle NewStyle, Qgraphics::TBitmap* NewBitmap);
	Qgraphics::TFont* __fastcall CreateFont(AnsiString NewName, int NewSize);
	Qgraphics::TPen* __fastcall CreatePen(Qgraphics::TColor NewColor, Qgraphics::TPenStyle NewStyle, int NewWidth, Qgraphics::TPenMode NewMode);
	virtual void __fastcall SetBrush(Qgraphics::TColor NewColor, Qgraphics::TBrushStyle NewStyle, Qgraphics::TBitmap* NewBitmap);
	virtual void __fastcall SetFont(AnsiString NewName, double NewSize);
	virtual void __fastcall SetPen(Qgraphics::TColor NewColor, Qgraphics::TPenStyle NewStyle, int NewWidth, Qgraphics::TPenMode NewMode);
	__property double TextFieldWidth = {read=FTextFieldWidth, write=FTextFieldWidth};
	__property bool Threaded = {read=FThreaded, write=FThreaded, nodefault};
	__property bool IgnoreRPTF = {read=FIgnoreRPTF, write=FIgnoreRPTF, nodefault};
	__property int CursorXPos = {read=GetCursorXPos, nodefault};
	__property int CursorYPos = {read=GetCursorYPos, nodefault};
	__property double XPos = {read=GetXPos, write=SetXPos};
	__property double YPos = {read=GetYPos, write=SetYPos};
	__property Qgraphics::TCanvas* Canvas = {read=GetCanvas};
	__property int CurrentPage = {read=FCurrentPage, nodefault};
	__property int RelativePage = {read=GetRelativePage, nodefault};
	__property int PageNumOffset = {read=FPageNumOffset, write=FPageNumOffset, nodefault};
	__property bool PageInvalid = {read=FPageInvalid, nodefault};
	__property bool OutputInvalid = {read=GetOutputInvalid, nodefault};
	__property int XDPI = {read=FXDPI, nodefault};
	__property int YDPI = {read=FYDPI, nodefault};
	__property int LineNum = {read=FLineNum, write=SetLine, nodefault};
	__property int ColumnNum = {read=FColumnNum, write=SetColumn, nodefault};
	__property int Columns = {read=FColumns, nodefault};
	__property double ColumnStart = {read=GetColumnStart};
	__property double ColumnEnd = {read=GetColumnEnd};
	__property double ColumnWidth = {read=GetColumnWidth};
	__property double LeftWaste = {read=GetLeftWaste};
	__property double RightWaste = {read=GetRightWaste};
	__property double TopWaste = {read=GetTopWaste};
	__property double BottomWaste = {read=GetBottomWaste};
	__property double SectionLeft = {read=GetLeftSection, write=SetLeftSection};
	__property double SectionRight = {read=GetRightSection, write=SetRightSection};
	__property double SectionTop = {read=GetTopSection, write=SetTopSection};
	__property double SectionBottom = {read=GetBottomSection, write=SetBottomSection};
	__property double PageHeight = {read=GetPageHeight};
	__property double PageWidth = {read=GetPageWidth};
	__property bool Bold = {read=GetBold, write=SetBold, nodefault};
	__property bool Underline = {read=GetUnderline, write=SetUnderline, nodefault};
	__property bool Italic = {read=GetItalic, write=SetItalic, nodefault};
	__property bool Strikeout = {read=GetStrikeout, write=SetStrikeout, nodefault};
	__property bool Superscript = {read=GetSuperscript, write=SetSuperscript, nodefault};
	__property bool Subscript = {read=GetSubscript, write=SetSubscript, nodefault};
	__property int FontRotation = {read=GetFontRotation, write=SetFontRotation, nodefault};
	__property Qgraphics::TColor FontColor = {read=GetFontColor, write=SetFontColor, nodefault};
	__property AnsiString FontName = {read=GetFontName, write=SetFontName};
	__property double FontSize = {read=GetFontSize, write=SetFontSize};
	__property HFONT FontHandle = {read=GetFontHandle, nodefault};
	__property Qgraphics::TFontPitch FontPitch = {read=GetFontPitch, write=SetFontPitch, nodefault};
	__property Byte FontCharset = {read=GetFontCharset, write=SetFontCharset, nodefault};
	__property Qrpdefine::TFontAlign FontAlign = {read=GetFontAlign, write=SetFontAlign, nodefault};
	__property double LineTop = {read=GetLineTop, write=SetLineTop};
	__property double LineMiddle = {read=GetLineMiddle, write=SetLineMiddle};
	__property double LineBottom = {read=GetLineBottom, write=SetLineBottom};
	__property double FontTop = {read=GetFontTop, write=SetFontTop};
	__property double FontBaseline = {read=GetFontBaseline, write=SetFontBaseline};
	__property double FontBottom = {read=GetFontBottom, write=SetFontBottom};
	__property Qrpdefine::TTabJustify TabJustify = {read=FTabJustify, write=FTabJustify, nodefault};
	__property bool TransparentBitmaps = {read=FTransparentBitmaps, write=FTransparentBitmaps, nodefault};
	__property Qrpdefine::TTabJustify ParaJustify = {read=FParaJustify, write=FParaJustify, default=4};
	__property bool NoBufferLine = {read=FNoBufferLine, write=FNoBufferLine, nodefault};
	__property bool NoNTColorFix = {read=FNoNTColorFix, write=FNoNTColorFix, nodefault};
	__property bool UseDrawText = {read=FUseDrawText, write=FUseDrawText, nodefault};
	__property int CurrentPass = {read=FCurrentPass, write=FCurrentPass, nodefault};
	__property int TotalPasses = {read=FTotalPasses, write=FTotalPasses, nodefault};
	__property double FixedTabWidth = {read=FFixedTabWidth, write=FFixedTabWidth};
	__property int PrinterIndex = {read=GetPrinterIndex, write=SetPrinterIndex, nodefault};
	__property AnsiString DeviceName = {read=GetDeviceName};
	__property AnsiString DriverName = {read=GetDriverName};
	__property AnsiString Port = {read=GetPort};
	__property bool Aborted = {read=FAborted, nodefault};
	__property bool Printing = {read=FPrinting, nodefault};
	__property double OriginX = {read=GetOriginX, write=SetOriginX};
	__property double OriginY = {read=GetOriginY, write=SetOriginY};
	__property Windows::PDeviceModeA DevMode = {read=GetDevMode};
	__property Qgraphics::TColor BKColor = {read=GetBKColor, write=SetBKColor, nodefault};
	__property double LineHeight = {read=GetLineHeight, write=SetLineHeight};
	__property double AscentHeight = {read=GetAscentHeight};
	__property double DescentHeight = {read=GetDescentHeight};
	__property double FontHeight = {read=GetFontHeight, write=SetFontHeight};
	__property double FontWidth = {read=GetFontWidth, write=SetFontWidth};
	__property AnsiString OutputName = {read=FOutputName, write=SetOutputName};
	__property double NoPrinterPageHeight = {read=FNoPrinterPageHeight, write=FNoPrinterPageHeight};
	__property double NoPrinterPageWidth = {read=FNoPrinterPageWidth, write=FNoPrinterPageWidth};
	__property Classes::TStrings* MacroData = {read=FMacroData, write=SetMacroData};
	__property int FirstPage = {read=FFirstPage, write=FFirstPage, default=1};
	__property int LastPage = {read=FLastPage, write=FLastPage, default=9999};
	__property AnsiString Selection = {read=FSelection, write=FSelection};
	__property Qstdctrls::TLabel* StatusLabel = {read=FStatusLabel, write=SetStatusLabel};
	__property AnsiString StatusFormat = {read=FStatusFormat, write=FStatusFormat};
	__property Classes::TStrings* StatusText = {read=FStatusText, write=SetStatusText};
	__property Qrpdefine::TLineHeightMethod LineHeightMethod = {read=FLineHeightMethod, write=FLineHeightMethod, default=1};
	__property int LinesPerInch = {read=FLinesPerInch, write=SetLinesPerInch, default=6};
	__property Qrpdefine::TPrintUnits Units = {read=FUnits, write=SetUnits, default=0};
	__property double UnitsFactor = {read=FUnitsFactor, write=SetUnitsFactor};
	__property double MarginLeft = {read=GetLeftMargin, write=SetLeftMargin};
	__property double MarginRight = {read=GetRightMargin, write=SetRightMargin};
	__property double MarginTop = {read=GetTopMargin, write=SetTopMargin};
	__property double MarginBottom = {read=GetBottomMargin, write=SetBottomMargin};
	__property int TabShade = {read=FTabShade, write=SetTabShade, default=0};
	__property Qgraphics::TColor TabColor = {read=FTabColor, write=FTabColor, default=0};
	__property Qgraphics::TColor BoxLineColor = {read=FBoxLineColor, write=FBoxLineColor, default=0};
	__property AnsiString Title = {read=FTitle, write=FTitle};
	__property Qrpdefine::TBKMode TextBKMode = {read=FTextBKMode, write=SetTextBKMode, default=0};
	__property int Copies = {read=FCopies, write=SetCopies, default=1};
	__property Qrpdefine::TOrientation Orientation = {read=FOrientation, write=SetOrientation, nodefault};
	__property double ScaleX = {read=FScaleX, write=SetScaleX};
	__property double ScaleY = {read=FScaleY, write=SetScaleY};
	__property Classes::TNotifyEvent OnPrint = {read=FOnPrint, write=FOnPrint};
	__property Qrpdefine::TPrintPageEvent OnPrintPage = {read=FOnPrintPage, write=FOnPrintPage};
	__property Classes::TNotifyEvent OnBeforePrint = {read=FOnBeforePrint, write=FOnBeforePrint};
	__property Classes::TNotifyEvent OnAfterPrint = {read=FOnAfterPrint, write=FOnAfterPrint};
	__property Classes::TNotifyEvent OnNewPage = {read=FOnNewPage, write=FOnNewPage};
	__property Classes::TNotifyEvent OnNewColumn = {read=FOnNewColumn, write=FOnNewColumn};
	__property Classes::TNotifyEvent OnPrintHeader = {read=FOnPrintHeader, write=FOnPrintHeader};
	__property Classes::TNotifyEvent OnPrintFooter = {read=FOnPrintFooter, write=FOnPrintFooter};
	__property Classes::TNotifyEvent OnEndOfSection = {read=FOnEndOfSection, write=FOnEndOfSection};
	__property Qrpdefine::TDecodeImageEvent OnDecodeImage = {read=FOnDecodeImage, write=FOnDecodeImage};
};


class PASCALIMPLEMENTATION TBaseMemoBuf : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	TBaseReport* FBaseReport;
	bool FHardLines;
	Qrpdefine::TPrintJustify FJustify;
	bool FKeepSpaces;
	double FPrintStart;
	double FPrintEnd;
	Byte FRTFMode;
	
public:
	virtual AnsiString __fastcall GetNextLine(bool &Eol) = 0 ;
	virtual int __fastcall MemoLinesLeft(void) = 0 ;
	virtual double __fastcall PrintLines(int Lines, bool PrintTabs) = 0 ;
	__property TBaseReport* BaseReport = {read=FBaseReport, write=FBaseReport};
	__property bool HardLines = {read=FHardLines, write=FHardLines, nodefault};
	__property Qrpdefine::TPrintJustify Justify = {read=FJustify, write=FJustify, nodefault};
	__property bool KeepSpaces = {read=FKeepSpaces, write=FKeepSpaces, nodefault};
	__property double PrintStart = {read=FPrintStart, write=FPrintStart};
	__property double PrintEnd = {read=FPrintEnd, write=FPrintEnd};
	__property Byte RTFMode = {read=FRTFMode, write=FRTFMode, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TBaseMemoBuf(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TBaseMemoBuf(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Qrpbase */
using namespace Qrpbase;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpBase
