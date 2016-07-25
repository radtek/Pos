// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpSystem.pas' rev: 6.00

#ifndef RpSystemHPP
#define RpSystemHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpRenderPreview.hpp>	// Pascal unit
#include <RpRenderPrinter.hpp>	// Pascal unit
#include <RpRender.hpp>	// Pascal unit
#include <RpDevice.hpp>	// Pascal unit
#include <RpFiler.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Buttons.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpsystem
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TOverrideMode { omCreate, omShow, omWait, omFree };
#pragma option pop

class DELPHICLASS TRvSystem;
typedef void __fastcall (__closure *TRvSystemEvent)(TRvSystem* ReportSystem, TOverrideMode OverrideMode, Forms::TForm* &OverrideForm);

class DELPHICLASS TSystemPrinter;
class PASCALIMPLEMENTATION TSystemPrinter : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	int FCopies;
	int FFirstPage;
	int FLastPage;
	Rpdefine::TLineHeightMethod FLineHeightMethod;
	int FLinesPerInch;
	double FMarginBottom;
	double FMarginLeft;
	double FMarginRight;
	double FMarginTop;
	Rpdefine::TOrientation FOrientation;
	double FScaleX;
	double FScaleY;
	AnsiString FStatusFormat;
	Classes::TStrings* FStatusText;
	int FTabShade;
	Rpdefine::TBKMode FTextBKMode;
	AnsiString FTitle;
	Rpdefine::TPrintUnits FUnits;
	double FUnitsFactor;
	bool FCollate;
	Rpdefine::TDuplex FDuplex;
	void __fastcall SetStatusText(Classes::TStrings* Value);
	void __fastcall SetTabShade(int Value);
	void __fastcall SetUnits(Rpdefine::TPrintUnits Value);
	void __fastcall SetUnitsFactor(double Value);
	
public:
	__fastcall TSystemPrinter(void);
	__fastcall virtual ~TSystemPrinter(void);
	void __fastcall InitPrinter(Rpbase::TBaseReport* BaseReport);
	
__published:
	__property int Copies = {read=FCopies, write=FCopies, default=1};
	__property bool Collate = {read=FCollate, write=FCollate, default=0};
	__property Rpdefine::TDuplex Duplex = {read=FDuplex, write=FDuplex, default=0};
	__property int FirstPage = {read=FFirstPage, write=FFirstPage, default=1};
	__property int LastPage = {read=FLastPage, write=FLastPage, default=9999};
	__property Rpdefine::TLineHeightMethod LineHeightMethod = {read=FLineHeightMethod, write=FLineHeightMethod, default=1};
	__property int LinesPerInch = {read=FLinesPerInch, write=FLinesPerInch, default=6};
	__property double MarginBottom = {read=FMarginBottom, write=FMarginBottom};
	__property double MarginLeft = {read=FMarginLeft, write=FMarginLeft};
	__property double MarginRight = {read=FMarginRight, write=FMarginRight};
	__property double MarginTop = {read=FMarginTop, write=FMarginTop};
	__property Rpdefine::TOrientation Orientation = {read=FOrientation, write=FOrientation, default=0};
	__property double ScaleX = {read=FScaleX, write=FScaleX};
	__property double ScaleY = {read=FScaleY, write=FScaleY};
	__property AnsiString StatusFormat = {read=FStatusFormat, write=FStatusFormat};
	__property Classes::TStrings* StatusText = {read=FStatusText, write=SetStatusText};
	__property int TabShade = {read=FTabShade, write=SetTabShade, default=0};
	__property Rpdefine::TBKMode TextBKMode = {read=FTextBKMode, write=FTextBKMode, default=0};
	__property AnsiString Title = {read=FTitle, write=FTitle};
	__property Rpdefine::TPrintUnits Units = {read=FUnits, write=SetUnits, default=0};
	__property double UnitsFactor = {read=FUnitsFactor, write=SetUnitsFactor};
};


class DELPHICLASS TSystemPreview;
class PASCALIMPLEMENTATION TSystemPreview : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	double FGridHoriz;
	Graphics::TPen* FGridPen;
	double FGridVert;
	Rpdefine::TMarginMethod FMarginMethod;
	double FMarginPercent;
	bool FMonochrome;
	int FPagesWide;
	int FPagesHigh;
	int FPageInc;
	Rpdefine::TRulerType FRulerType;
	int FShadowDepth;
	double FZoomFactor;
	int FZoomInc;
	int FFormWidth;
	int FFormHeight;
	Forms::TWindowState FFormState;
	void __fastcall SetMonochrome(bool Value);
	void __fastcall SetShadowDepth(int Value);
	void __fastcall SetZoomFactor(double Value);
	
public:
	__fastcall TSystemPreview(void);
	__fastcall virtual ~TSystemPreview(void);
	void __fastcall InitPreview(Rprenderpreview::TRvRenderPreview* RenderPreview);
	
__published:
	__property int FormWidth = {read=FFormWidth, write=FFormWidth, default=615};
	__property int FormHeight = {read=FFormHeight, write=FFormHeight, default=450};
	__property Forms::TWindowState FormState = {read=FFormState, write=FFormState, default=0};
	__property double GridHoriz = {read=FGridHoriz, write=FGridHoriz};
	__property Graphics::TPen* GridPen = {read=FGridPen, write=FGridPen};
	__property double GridVert = {read=FGridVert, write=FGridVert};
	__property Rpdefine::TMarginMethod MarginMethod = {read=FMarginMethod, write=FMarginMethod, default=1};
	__property double MarginPercent = {read=FMarginPercent, write=FMarginPercent};
	__property bool Monochrome = {read=FMonochrome, write=SetMonochrome, default=0};
	__property int PagesWide = {read=FPagesWide, write=FPagesWide, default=1};
	__property int PagesHigh = {read=FPagesHigh, write=FPagesHigh, default=1};
	__property int PageInc = {read=FPageInc, write=FPageInc, default=1};
	__property Rpdefine::TRulerType RulerType = {read=FRulerType, write=FRulerType, default=0};
	__property int ShadowDepth = {read=FShadowDepth, write=SetShadowDepth, default=0};
	__property double ZoomFactor = {read=FZoomFactor, write=SetZoomFactor};
	__property int ZoomInc = {read=FZoomInc, write=FZoomInc, default=10};
};


class DELPHICLASS TSystemFiler;
class PASCALIMPLEMENTATION TSystemFiler : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	Rpdefine::TAccuracyMethod FAccuracyMethod;
	AnsiString FFileName;
	AnsiString FStatusFormat;
	Classes::TStrings* FStatusText;
	Rpdefine::TStreamMode FStreamMode;
	Classes::TStream* FStream;
	bool FIgnoreRPTF;
	void __fastcall SetStatusText(Classes::TStrings* Value);
	
public:
	__fastcall TSystemFiler(void);
	__fastcall virtual ~TSystemFiler(void);
	void __fastcall InitFiler(Rpbase::TBaseReport* BaseReport);
	__property Classes::TStream* Stream = {read=FStream, write=FStream};
	__property bool IgnoreRPTF = {read=FIgnoreRPTF, write=FIgnoreRPTF, nodefault};
	
__published:
	__property Rpdefine::TAccuracyMethod AccuracyMethod = {read=FAccuracyMethod, write=FAccuracyMethod, default=0};
	__property AnsiString FileName = {read=FFileName, write=FFileName};
	__property AnsiString StatusFormat = {read=FStatusFormat, write=FStatusFormat};
	__property Classes::TStrings* StatusText = {read=FStatusText, write=SetStatusText};
	__property Rpdefine::TStreamMode StreamMode = {read=FStreamMode, write=FStreamMode, default=0};
};


class PASCALIMPLEMENTATION TRvSystem : public Rpbase::TRPBaseComponent 
{
	typedef Rpbase::TRPBaseComponent inherited;
	
protected:
	bool FDoNativeOutput;
	bool FAborted;
	Rpbase::TBaseReport* FBaseReport;
	Stdctrls::TLabel* SaveLabel;
	Rpdefine::TReportDest FDefaultDest;
	Rpdefine::TReportDest FReportDest;
	AnsiString FOutputFileName;
	bool ForceMultiPrint;
	Rpdefine::TSystemSetups FSystemSetups;
	Rpdefine::TSystemOptions FSystemOptions;
	TSystemFiler* FSystemFiler;
	TSystemPreview* FSystemPreview;
	TSystemPrinter* FSystemPrinter;
	AnsiString FTitleSetup;
	AnsiString FTitleStatus;
	AnsiString FTitlePreview;
	Classes::TStrings* FStorageMacroData;
	Rprender::TRPRender* FRenderObject;
	Classes::TNotifyEvent FOnRenderPage;
	Classes::TNotifyEvent FOnRenderData;
	Classes::TNotifyEvent FOnPrint;
	Rpdefine::TPrintPageEvent FOnPrintPage;
	Classes::TNotifyEvent FOnBeforePrint;
	Classes::TNotifyEvent FOnAfterPrint;
	Classes::TNotifyEvent FOnAfterPreviewPrint;
	Classes::TNotifyEvent FOnNewPage;
	Classes::TNotifyEvent FOnNewColumn;
	Classes::TNotifyEvent FOnPrintHeader;
	Classes::TNotifyEvent FOnPrintFooter;
	Classes::TNotifyEvent FOnEndOfSection;
	Rpdefine::TDecodeImageEvent FOnDecodeImage;
	Classes::TNotifyEvent FOnPreviewSetup;
	Classes::TNotifyEvent FOnPreviewShow;
	TRvSystemEvent FOnOverrideSetup;
	TRvSystemEvent FOnOverrideStatus;
	TRvSystemEvent FOnOverridePreview;
	void __fastcall SetRenderObject(const Rprender::TRPRender* Value);
	void __fastcall InitEvents(Rpbase::TBaseReport* BaseReport);
	Classes::TStream* __fastcall GetStream(void);
	void __fastcall SetStream(Classes::TStream* Value);
	virtual void __fastcall OverrideSetupProc(TRvSystem* ReportSystem, TOverrideMode OverrideMode, Forms::TForm* &OverrideForm);
	virtual void __fastcall OverrideStatusProc(TRvSystem* ReportSystem, TOverrideMode OverrideMode, Forms::TForm* &OverrideForm);
	virtual void __fastcall OverridePreviewProc(TRvSystem* ReportSystem, TOverrideMode OverrideMode, Forms::TForm* &OverrideForm);
	void __fastcall SetupReport(Forms::TForm* &SetupForm);
	void __fastcall GenerateReport(Forms::TForm* &StatusForm);
	void __fastcall PrintReport(Forms::TForm* &StatusForm);
	void __fastcall PreviewReport(Forms::TForm* &PreviewForm);
	void __fastcall SetStorageMacroData(Classes::TStrings* Value);
	void __fastcall InitRenderStream(Classes::TStream* &RenderStream);
	void __fastcall PrintRender(Forms::TForm* &StatusForm);
	
public:
	__fastcall virtual TRvSystem(Classes::TComponent* AOwner);
	__fastcall virtual ~TRvSystem(void);
	void __fastcall Execute(void);
	void __fastcall Generate(void);
	void __fastcall PreviewShowEvent(System::TObject* Sender);
	__property Classes::TStream* Stream = {read=GetStream, write=SetStream};
	__property Rpdefine::TReportDest ReportDest = {read=FReportDest, write=FReportDest, nodefault};
	__property AnsiString OutputFileName = {read=FOutputFileName, write=FOutputFileName};
	__property Rpbase::TBaseReport* BaseReport = {read=FBaseReport, write=FBaseReport};
	__property bool Aborted = {read=FAborted, write=FAborted, nodefault};
	__property Classes::TStrings* StorageMacroData = {read=FStorageMacroData, write=SetStorageMacroData};
	__property Rprender::TRPRender* RenderObject = {read=FRenderObject, write=SetRenderObject};
	__property bool DoNativeOutput = {read=FDoNativeOutput, write=FDoNativeOutput, default=1};
	__property Classes::TNotifyEvent OnRenderPage = {read=FOnRenderPage, write=FOnRenderPage};
	__property Classes::TNotifyEvent OnRenderData = {read=FOnRenderData, write=FOnRenderData};
	
__published:
	__property AnsiString TitleSetup = {read=FTitleSetup, write=FTitleSetup};
	__property AnsiString TitleStatus = {read=FTitleStatus, write=FTitleStatus};
	__property AnsiString TitlePreview = {read=FTitlePreview, write=FTitlePreview};
	__property Rpdefine::TSystemSetups SystemSetups = {read=FSystemSetups, write=FSystemSetups, default=511};
	__property Rpdefine::TSystemOptions SystemOptions = {read=FSystemOptions, write=FSystemOptions, default=28};
	__property Rpdefine::TReportDest DefaultDest = {read=FDefaultDest, write=FDefaultDest, default=0};
	__property TSystemFiler* SystemFiler = {read=FSystemFiler, write=FSystemFiler};
	__property TSystemPreview* SystemPreview = {read=FSystemPreview, write=FSystemPreview};
	__property TSystemPrinter* SystemPrinter = {read=FSystemPrinter, write=FSystemPrinter};
	__property Classes::TNotifyEvent OnPrint = {read=FOnPrint, write=FOnPrint};
	__property Rpdefine::TPrintPageEvent OnPrintPage = {read=FOnPrintPage, write=FOnPrintPage};
	__property Classes::TNotifyEvent OnBeforePrint = {read=FOnBeforePrint, write=FOnBeforePrint};
	__property Classes::TNotifyEvent OnAfterPrint = {read=FOnAfterPrint, write=FOnAfterPrint};
	__property Classes::TNotifyEvent OnAfterPreviewPrint = {read=FOnAfterPreviewPrint, write=FOnAfterPreviewPrint};
	__property Classes::TNotifyEvent OnNewPage = {read=FOnNewPage, write=FOnNewPage};
	__property Classes::TNotifyEvent OnNewColumn = {read=FOnNewColumn, write=FOnNewColumn};
	__property Classes::TNotifyEvent OnPrintHeader = {read=FOnPrintHeader, write=FOnPrintHeader};
	__property Classes::TNotifyEvent OnPrintFooter = {read=FOnPrintFooter, write=FOnPrintFooter};
	__property Classes::TNotifyEvent OnEndOfSection = {read=FOnEndOfSection, write=FOnEndOfSection};
	__property Rpdefine::TDecodeImageEvent OnDecodeImage = {read=FOnDecodeImage, write=FOnDecodeImage};
	__property Classes::TNotifyEvent OnPreviewSetup = {read=FOnPreviewSetup, write=FOnPreviewSetup};
	__property Classes::TNotifyEvent OnPreviewShow = {read=FOnPreviewShow, write=FOnPreviewShow};
	__property TRvSystemEvent OverrideSetup = {read=FOnOverrideSetup, write=FOnOverrideSetup};
	__property TRvSystemEvent OverrideStatus = {read=FOnOverrideStatus, write=FOnOverrideStatus};
	__property TRvSystemEvent OverridePreview = {read=FOnOverridePreview, write=FOnOverridePreview};
};


typedef TRvSystem TReportSystem;
;

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rpsystem */
using namespace Rpsystem;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpSystem
