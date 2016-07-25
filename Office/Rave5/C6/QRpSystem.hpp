// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpSystem.pas' rev: 6.00

#ifndef QRpSystemHPP
#define QRpSystemHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpRenderPreview.hpp>	// Pascal unit
#include <QRpRenderPrinter.hpp>	// Pascal unit
#include <QRpRender.hpp>	// Pascal unit
#include <QRpDevice.hpp>	// Pascal unit
#include <QRpFiler.hpp>	// Pascal unit
#include <QRpBase.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <QButtons.hpp>	// Pascal unit
#include <QExtCtrls.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpsystem
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TOverrideMode { omCreate, omShow, omWait, omFree };
#pragma option pop

class DELPHICLASS TRvSystem;
typedef void __fastcall (__closure *TRvSystemEvent)(TRvSystem* ReportSystem, TOverrideMode OverrideMode, Qforms::TForm* &OverrideForm);

class DELPHICLASS TSystemPrinter;
class PASCALIMPLEMENTATION TSystemPrinter : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	int FCopies;
	int FFirstPage;
	int FLastPage;
	Qrpdefine::TLineHeightMethod FLineHeightMethod;
	int FLinesPerInch;
	double FMarginBottom;
	double FMarginLeft;
	double FMarginRight;
	double FMarginTop;
	Qrpdefine::TOrientation FOrientation;
	double FScaleX;
	double FScaleY;
	AnsiString FStatusFormat;
	Classes::TStrings* FStatusText;
	int FTabShade;
	Qrpdefine::TBKMode FTextBKMode;
	AnsiString FTitle;
	Qrpdefine::TPrintUnits FUnits;
	double FUnitsFactor;
	bool FCollate;
	Qrpdefine::TDuplex FDuplex;
	void __fastcall SetStatusText(Classes::TStrings* Value);
	void __fastcall SetTabShade(int Value);
	void __fastcall SetUnits(Qrpdefine::TPrintUnits Value);
	void __fastcall SetUnitsFactor(double Value);
	
public:
	__fastcall TSystemPrinter(void);
	__fastcall virtual ~TSystemPrinter(void);
	void __fastcall InitPrinter(Qrpbase::TBaseReport* BaseReport);
	
__published:
	__property int Copies = {read=FCopies, write=FCopies, default=1};
	__property bool Collate = {read=FCollate, write=FCollate, default=0};
	__property Qrpdefine::TDuplex Duplex = {read=FDuplex, write=FDuplex, default=0};
	__property int FirstPage = {read=FFirstPage, write=FFirstPage, default=1};
	__property int LastPage = {read=FLastPage, write=FLastPage, default=9999};
	__property Qrpdefine::TLineHeightMethod LineHeightMethod = {read=FLineHeightMethod, write=FLineHeightMethod, default=1};
	__property int LinesPerInch = {read=FLinesPerInch, write=FLinesPerInch, default=6};
	__property double MarginBottom = {read=FMarginBottom, write=FMarginBottom};
	__property double MarginLeft = {read=FMarginLeft, write=FMarginLeft};
	__property double MarginRight = {read=FMarginRight, write=FMarginRight};
	__property double MarginTop = {read=FMarginTop, write=FMarginTop};
	__property Qrpdefine::TOrientation Orientation = {read=FOrientation, write=FOrientation, default=0};
	__property double ScaleX = {read=FScaleX, write=FScaleX};
	__property double ScaleY = {read=FScaleY, write=FScaleY};
	__property AnsiString StatusFormat = {read=FStatusFormat, write=FStatusFormat};
	__property Classes::TStrings* StatusText = {read=FStatusText, write=SetStatusText};
	__property int TabShade = {read=FTabShade, write=SetTabShade, default=0};
	__property Qrpdefine::TBKMode TextBKMode = {read=FTextBKMode, write=FTextBKMode, default=0};
	__property AnsiString Title = {read=FTitle, write=FTitle};
	__property Qrpdefine::TPrintUnits Units = {read=FUnits, write=SetUnits, default=0};
	__property double UnitsFactor = {read=FUnitsFactor, write=SetUnitsFactor};
};


class DELPHICLASS TSystemPreview;
class PASCALIMPLEMENTATION TSystemPreview : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	double FGridHoriz;
	Qgraphics::TPen* FGridPen;
	double FGridVert;
	Qrpdefine::TMarginMethod FMarginMethod;
	double FMarginPercent;
	bool FMonochrome;
	int FPagesWide;
	int FPagesHigh;
	int FPageInc;
	Qrpdefine::TRulerType FRulerType;
	int FShadowDepth;
	double FZoomFactor;
	int FZoomInc;
	int FFormWidth;
	int FFormHeight;
	Qforms::TWindowState FFormState;
	void __fastcall SetMonochrome(bool Value);
	void __fastcall SetShadowDepth(int Value);
	void __fastcall SetZoomFactor(double Value);
	
public:
	__fastcall TSystemPreview(void);
	__fastcall virtual ~TSystemPreview(void);
	void __fastcall InitPreview(Qrprenderpreview::TRvRenderPreview* RenderPreview);
	
__published:
	__property int FormWidth = {read=FFormWidth, write=FFormWidth, default=615};
	__property int FormHeight = {read=FFormHeight, write=FFormHeight, default=450};
	__property Qforms::TWindowState FormState = {read=FFormState, write=FFormState, default=0};
	__property double GridHoriz = {read=FGridHoriz, write=FGridHoriz};
	__property Qgraphics::TPen* GridPen = {read=FGridPen, write=FGridPen};
	__property double GridVert = {read=FGridVert, write=FGridVert};
	__property Qrpdefine::TMarginMethod MarginMethod = {read=FMarginMethod, write=FMarginMethod, default=1};
	__property double MarginPercent = {read=FMarginPercent, write=FMarginPercent};
	__property bool Monochrome = {read=FMonochrome, write=SetMonochrome, default=0};
	__property int PagesWide = {read=FPagesWide, write=FPagesWide, default=1};
	__property int PagesHigh = {read=FPagesHigh, write=FPagesHigh, default=1};
	__property int PageInc = {read=FPageInc, write=FPageInc, default=1};
	__property Qrpdefine::TRulerType RulerType = {read=FRulerType, write=FRulerType, default=0};
	__property int ShadowDepth = {read=FShadowDepth, write=SetShadowDepth, default=0};
	__property double ZoomFactor = {read=FZoomFactor, write=SetZoomFactor};
	__property int ZoomInc = {read=FZoomInc, write=FZoomInc, default=10};
};


class DELPHICLASS TSystemFiler;
class PASCALIMPLEMENTATION TSystemFiler : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	Qrpdefine::TAccuracyMethod FAccuracyMethod;
	AnsiString FFileName;
	AnsiString FStatusFormat;
	Classes::TStrings* FStatusText;
	Qrpdefine::TStreamMode FStreamMode;
	Classes::TStream* FStream;
	bool FIgnoreRPTF;
	void __fastcall SetStatusText(Classes::TStrings* Value);
	
public:
	__fastcall TSystemFiler(void);
	__fastcall virtual ~TSystemFiler(void);
	void __fastcall InitFiler(Qrpbase::TBaseReport* BaseReport);
	__property Classes::TStream* Stream = {read=FStream, write=FStream};
	__property bool IgnoreRPTF = {read=FIgnoreRPTF, write=FIgnoreRPTF, nodefault};
	
__published:
	__property Qrpdefine::TAccuracyMethod AccuracyMethod = {read=FAccuracyMethod, write=FAccuracyMethod, default=0};
	__property AnsiString FileName = {read=FFileName, write=FFileName};
	__property AnsiString StatusFormat = {read=FStatusFormat, write=FStatusFormat};
	__property Classes::TStrings* StatusText = {read=FStatusText, write=SetStatusText};
	__property Qrpdefine::TStreamMode StreamMode = {read=FStreamMode, write=FStreamMode, default=0};
};


class PASCALIMPLEMENTATION TRvSystem : public Qrpbase::TRPBaseComponent 
{
	typedef Qrpbase::TRPBaseComponent inherited;
	
protected:
	bool FDoNativeOutput;
	bool FAborted;
	Qrpbase::TBaseReport* FBaseReport;
	Qstdctrls::TLabel* SaveLabel;
	Qrpdefine::TReportDest FDefaultDest;
	Qrpdefine::TReportDest FReportDest;
	AnsiString FOutputFileName;
	bool ForceMultiPrint;
	Qrpdefine::TSystemSetups FSystemSetups;
	Qrpdefine::TSystemOptions FSystemOptions;
	TSystemFiler* FSystemFiler;
	TSystemPreview* FSystemPreview;
	TSystemPrinter* FSystemPrinter;
	AnsiString FTitleSetup;
	AnsiString FTitleStatus;
	AnsiString FTitlePreview;
	Classes::TStrings* FStorageMacroData;
	Qrprender::TRPRender* FRenderObject;
	Classes::TNotifyEvent FOnRenderPage;
	Classes::TNotifyEvent FOnRenderData;
	Classes::TNotifyEvent FOnPrint;
	Qrpdefine::TPrintPageEvent FOnPrintPage;
	Classes::TNotifyEvent FOnBeforePrint;
	Classes::TNotifyEvent FOnAfterPrint;
	Classes::TNotifyEvent FOnAfterPreviewPrint;
	Classes::TNotifyEvent FOnNewPage;
	Classes::TNotifyEvent FOnNewColumn;
	Classes::TNotifyEvent FOnPrintHeader;
	Classes::TNotifyEvent FOnPrintFooter;
	Classes::TNotifyEvent FOnEndOfSection;
	Qrpdefine::TDecodeImageEvent FOnDecodeImage;
	Classes::TNotifyEvent FOnPreviewSetup;
	Classes::TNotifyEvent FOnPreviewShow;
	TRvSystemEvent FOnOverrideSetup;
	TRvSystemEvent FOnOverrideStatus;
	TRvSystemEvent FOnOverridePreview;
	void __fastcall SetRenderObject(const Qrprender::TRPRender* Value);
	void __fastcall InitEvents(Qrpbase::TBaseReport* BaseReport);
	Classes::TStream* __fastcall GetStream(void);
	void __fastcall SetStream(Classes::TStream* Value);
	virtual void __fastcall OverrideSetupProc(TRvSystem* ReportSystem, TOverrideMode OverrideMode, Qforms::TForm* &OverrideForm);
	virtual void __fastcall OverrideStatusProc(TRvSystem* ReportSystem, TOverrideMode OverrideMode, Qforms::TForm* &OverrideForm);
	virtual void __fastcall OverridePreviewProc(TRvSystem* ReportSystem, TOverrideMode OverrideMode, Qforms::TForm* &OverrideForm);
	void __fastcall SetupReport(Qforms::TForm* &SetupForm);
	void __fastcall GenerateReport(Qforms::TForm* &StatusForm);
	void __fastcall PrintReport(Qforms::TForm* &StatusForm);
	void __fastcall PreviewReport(Qforms::TForm* &PreviewForm);
	void __fastcall SetStorageMacroData(Classes::TStrings* Value);
	void __fastcall InitRenderStream(Classes::TStream* &RenderStream);
	void __fastcall PrintRender(Qforms::TForm* &StatusForm);
	
public:
	__fastcall virtual TRvSystem(Classes::TComponent* AOwner);
	__fastcall virtual ~TRvSystem(void);
	void __fastcall Execute(void);
	void __fastcall Generate(void);
	void __fastcall PreviewShowEvent(System::TObject* Sender);
	__property Classes::TStream* Stream = {read=GetStream, write=SetStream};
	__property Qrpdefine::TReportDest ReportDest = {read=FReportDest, write=FReportDest, nodefault};
	__property AnsiString OutputFileName = {read=FOutputFileName, write=FOutputFileName};
	__property Qrpbase::TBaseReport* BaseReport = {read=FBaseReport, write=FBaseReport};
	__property bool Aborted = {read=FAborted, write=FAborted, nodefault};
	__property Classes::TStrings* StorageMacroData = {read=FStorageMacroData, write=SetStorageMacroData};
	__property Qrprender::TRPRender* RenderObject = {read=FRenderObject, write=SetRenderObject};
	__property bool DoNativeOutput = {read=FDoNativeOutput, write=FDoNativeOutput, default=1};
	__property Classes::TNotifyEvent OnRenderPage = {read=FOnRenderPage, write=FOnRenderPage};
	__property Classes::TNotifyEvent OnRenderData = {read=FOnRenderData, write=FOnRenderData};
	
__published:
	__property AnsiString TitleSetup = {read=FTitleSetup, write=FTitleSetup};
	__property AnsiString TitleStatus = {read=FTitleStatus, write=FTitleStatus};
	__property AnsiString TitlePreview = {read=FTitlePreview, write=FTitlePreview};
	__property Qrpdefine::TSystemSetups SystemSetups = {read=FSystemSetups, write=FSystemSetups, default=511};
	__property Qrpdefine::TSystemOptions SystemOptions = {read=FSystemOptions, write=FSystemOptions, default=28};
	__property Qrpdefine::TReportDest DefaultDest = {read=FDefaultDest, write=FDefaultDest, default=0};
	__property TSystemFiler* SystemFiler = {read=FSystemFiler, write=FSystemFiler};
	__property TSystemPreview* SystemPreview = {read=FSystemPreview, write=FSystemPreview};
	__property TSystemPrinter* SystemPrinter = {read=FSystemPrinter, write=FSystemPrinter};
	__property Classes::TNotifyEvent OnPrint = {read=FOnPrint, write=FOnPrint};
	__property Qrpdefine::TPrintPageEvent OnPrintPage = {read=FOnPrintPage, write=FOnPrintPage};
	__property Classes::TNotifyEvent OnBeforePrint = {read=FOnBeforePrint, write=FOnBeforePrint};
	__property Classes::TNotifyEvent OnAfterPrint = {read=FOnAfterPrint, write=FOnAfterPrint};
	__property Classes::TNotifyEvent OnAfterPreviewPrint = {read=FOnAfterPreviewPrint, write=FOnAfterPreviewPrint};
	__property Classes::TNotifyEvent OnNewPage = {read=FOnNewPage, write=FOnNewPage};
	__property Classes::TNotifyEvent OnNewColumn = {read=FOnNewColumn, write=FOnNewColumn};
	__property Classes::TNotifyEvent OnPrintHeader = {read=FOnPrintHeader, write=FOnPrintHeader};
	__property Classes::TNotifyEvent OnPrintFooter = {read=FOnPrintFooter, write=FOnPrintFooter};
	__property Classes::TNotifyEvent OnEndOfSection = {read=FOnEndOfSection, write=FOnEndOfSection};
	__property Qrpdefine::TDecodeImageEvent OnDecodeImage = {read=FOnDecodeImage, write=FOnDecodeImage};
	__property Classes::TNotifyEvent OnPreviewSetup = {read=FOnPreviewSetup, write=FOnPreviewSetup};
	__property Classes::TNotifyEvent OnPreviewShow = {read=FOnPreviewShow, write=FOnPreviewShow};
	__property TRvSystemEvent OverrideSetup = {read=FOnOverrideSetup, write=FOnOverrideSetup};
	__property TRvSystemEvent OverrideStatus = {read=FOnOverrideStatus, write=FOnOverrideStatus};
	__property TRvSystemEvent OverridePreview = {read=FOnOverridePreview, write=FOnOverridePreview};
};


typedef TRvSystem TReportSystem;
;

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Qrpsystem */
using namespace Qrpsystem;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpSystem
