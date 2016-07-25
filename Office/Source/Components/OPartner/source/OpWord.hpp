// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpWord.pas' rev: 6.00

#ifndef OpWordHPP
#define OpWordHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Variants.hpp>	// Pascal unit
#include <OpDbOfc.hpp>	// Pascal unit
#include <OpOutlk.hpp>	// Pascal unit
#include <OpModels.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <OpShared.hpp>	// Pascal unit
#include <OpMSO.hpp>	// Pascal unit
#include <OpOfcXP.hpp>	// Pascal unit
#include <OpWrdXP.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opword
{
//-- type declarations -------------------------------------------------------
__interface WordWindow;
typedef System::DelphiInterface<WordWindow> _di_WordWindow;
__interface INTERFACE_UUID("{00020962-0000-0000-C000-000000000046}") WordWindow  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(Opwrdxp::_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Creator(int &Get_Creator_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_ActivePane(Opwrdxp::_di_Pane &Get_ActivePane_result) = 0 ;
	virtual HRESULT __safecall Get_Document(Opwrdxp::_di__Document &Get_Document_result) = 0 ;
	virtual HRESULT __safecall Get_Panes(Opwrdxp::_di_Panes &Get_Panes_result) = 0 ;
	virtual HRESULT __safecall Get_Selection(Opwrdxp::_di_Selection &Get_Selection_result) = 0 ;
	virtual HRESULT __safecall Get_Left(int &Get_Left_result) = 0 ;
	virtual HRESULT __safecall Set_Left(int prop) = 0 ;
	virtual HRESULT __safecall Get_Top(int &Get_Top_result) = 0 ;
	virtual HRESULT __safecall Set_Top(int prop) = 0 ;
	virtual HRESULT __safecall Get_Width(int &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(int prop) = 0 ;
	virtual HRESULT __safecall Get_Height(int &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(int prop) = 0 ;
	virtual HRESULT __safecall Get_Split(Word &Get_Split_result) = 0 ;
	virtual HRESULT __safecall Set_Split(Word prop) = 0 ;
	virtual HRESULT __safecall Get_SplitVertical(int &Get_SplitVertical_result) = 0 ;
	virtual HRESULT __safecall Set_SplitVertical(int prop) = 0 ;
	virtual HRESULT __safecall Get_Caption(WideString &Get_Caption_result) = 0 ;
	virtual HRESULT __safecall Set_Caption(const WideString prop) = 0 ;
	virtual HRESULT __safecall Get_WindowState(Activex::TOleEnum &Get_WindowState_result) = 0 ;
	virtual HRESULT __safecall Set_WindowState(Activex::TOleEnum prop) = 0 ;
	virtual HRESULT __safecall Get_DisplayRulers(Word &Get_DisplayRulers_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayRulers(Word prop) = 0 ;
	virtual HRESULT __safecall Get_DisplayVerticalRuler(Word &Get_DisplayVerticalRuler_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayVerticalRuler(Word prop) = 0 ;
	virtual HRESULT __safecall Get_View(Opwrdxp::_di_View &Get_View_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_Next(Opwrdxp::_di_Window &Get_Next_result) = 0 ;
	virtual HRESULT __safecall Get_Previous(Opwrdxp::_di_Window &Get_Previous_result) = 0 ;
	virtual HRESULT __safecall Get_WindowNumber(int &Get_WindowNumber_result) = 0 ;
	virtual HRESULT __safecall Get_DisplayVerticalScrollBar(Word &Get_DisplayVerticalScrollBar_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayVerticalScrollBar(Word prop) = 0 ;
	virtual HRESULT __safecall Get_DisplayHorizontalScrollBar(Word &Get_DisplayHorizontalScrollBar_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayHorizontalScrollBar(Word prop) = 0 ;
	virtual HRESULT __safecall Get_StyleAreaWidth(float &Get_StyleAreaWidth_result) = 0 ;
	virtual HRESULT __safecall Set_StyleAreaWidth(float prop) = 0 ;
	virtual HRESULT __safecall Get_DisplayScreenTips(Word &Get_DisplayScreenTips_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayScreenTips(Word prop) = 0 ;
	virtual HRESULT __safecall Get_HorizontalPercentScrolled(int &Get_HorizontalPercentScrolled_result) = 0 ;
	virtual HRESULT __safecall Set_HorizontalPercentScrolled(int prop) = 0 ;
	virtual HRESULT __safecall Get_VerticalPercentScrolled(int &Get_VerticalPercentScrolled_result) = 0 ;
	virtual HRESULT __safecall Set_VerticalPercentScrolled(int prop) = 0 ;
	virtual HRESULT __safecall Get_DocumentMap(Word &Get_DocumentMap_result) = 0 ;
	virtual HRESULT __safecall Set_DocumentMap(Word prop) = 0 ;
	virtual HRESULT __safecall Get_Active(Word &Get_Active_result) = 0 ;
	virtual HRESULT __safecall Get_DocumentMapPercentWidth(int &Get_DocumentMapPercentWidth_result) = 0 ;
	virtual HRESULT __safecall Set_DocumentMapPercentWidth(int prop) = 0 ;
	virtual HRESULT __safecall Get_Index(int &Get_Index_result) = 0 ;
	virtual HRESULT __safecall Get_IMEMode(Activex::TOleEnum &Get_IMEMode_result) = 0 ;
	virtual HRESULT __safecall Set_IMEMode(Activex::TOleEnum prop) = 0 ;
	virtual HRESULT __safecall Activate(void) = 0 ;
	virtual HRESULT __safecall Close(OleVariant &SaveChanges, OleVariant &RouteDocument) = 0 ;
	virtual HRESULT __safecall LargeScroll(OleVariant &Down, OleVariant &Up, OleVariant &ToRight, OleVariant &ToLeft) = 0 ;
	virtual HRESULT __safecall SmallScroll(OleVariant &Down, OleVariant &Up, OleVariant &ToRight, OleVariant &ToLeft) = 0 ;
	virtual HRESULT __safecall NewWindow(Opwrdxp::_di_Window &NewWindow_result) = 0 ;
	virtual HRESULT __safecall PrintOutOld(OleVariant &Background, OleVariant &Append, OleVariant &Range, OleVariant &OutputFileName, OleVariant &From, OleVariant &To_, OleVariant &Item, OleVariant &Copies, OleVariant &Pages, OleVariant &PageType, OleVariant &PrintToFile, OleVariant &Collate, OleVariant &ActivePrinterMacGX, OleVariant &ManualDuplexPrint) = 0 ;
	virtual HRESULT __safecall PageScroll(OleVariant &Down, OleVariant &Up) = 0 ;
	virtual HRESULT __safecall SetFocus(void) = 0 ;
	virtual HRESULT __safecall RangeFromPoint(int x, int y, _di_IDispatch &RangeFromPoint_result) = 0 ;
	virtual HRESULT __safecall ScrollIntoView(const _di_IDispatch obj, OleVariant &Start) = 0 ;
	virtual HRESULT __safecall GetPoint(/* out */ int &ScreenPixelsLeft, /* out */ int &ScreenPixelsTop, /* out */ int &ScreenPixelsWidth, /* out */ int &ScreenPixelsHeight, const _di_IDispatch obj) = 0 ;
	virtual HRESULT __safecall PrintOut2000(OleVariant &Background, OleVariant &Append, OleVariant &Range, OleVariant &OutputFileName, OleVariant &From, OleVariant &To_, OleVariant &Item, OleVariant &Copies, OleVariant &Pages, OleVariant &PageType, OleVariant &PrintToFile, OleVariant &Collate, OleVariant &ActivePrinterMacGX, OleVariant &ManualDuplexPrint, OleVariant &PrintZoomColumn, OleVariant &PrintZoomRow, OleVariant &PrintZoomPaperWidth, OleVariant &PrintZoomPaperHeight) = 0 ;
	virtual HRESULT __safecall Get_UsableWidth(int &Get_UsableWidth_result) = 0 ;
	virtual HRESULT __safecall Get_UsableHeight(int &Get_UsableHeight_result) = 0 ;
	virtual HRESULT __safecall Get_EnvelopeVisible(Word &Get_EnvelopeVisible_result) = 0 ;
	virtual HRESULT __safecall Set_EnvelopeVisible(Word prop) = 0 ;
	virtual HRESULT __safecall Get_DisplayRightRuler(Word &Get_DisplayRightRuler_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayRightRuler(Word prop) = 0 ;
	virtual HRESULT __safecall Get_DisplayLeftScrollBar(Word &Get_DisplayLeftScrollBar_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayLeftScrollBar(Word prop) = 0 ;
	virtual HRESULT __safecall Get_Visible(Word &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Word prop) = 0 ;
	virtual HRESULT __safecall PrintOut(OleVariant &Background, OleVariant &Append, OleVariant &Range, OleVariant &OutputFileName, OleVariant &From, OleVariant &To_, OleVariant &Item, OleVariant &Copies, OleVariant &Pages, OleVariant &PageType, OleVariant &PrintToFile, OleVariant &Collate, OleVariant &ActivePrinterMacGX, OleVariant &ManualDuplexPrint, OleVariant &PrintZoomColumn, OleVariant &PrintZoomRow, OleVariant &PrintZoomPaperWidth, OleVariant &PrintZoomPaperHeight) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opwrdxp::_di__Application _scw_Get_Application() { Opwrdxp::_di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opwrdxp::_di__Application Application_ = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Creator() { int r; HRESULT hr = Get_Creator(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Creator = {read=_scw_Get_Creator};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opwrdxp::_di_Pane _scw_Get_ActivePane() { Opwrdxp::_di_Pane r; HRESULT hr = Get_ActivePane(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opwrdxp::_di_Pane ActivePane = {read=_scw_Get_ActivePane};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opwrdxp::_di__Document _scw_Get_Document() { Opwrdxp::_di__Document r; HRESULT hr = Get_Document(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opwrdxp::_di__Document Document = {read=_scw_Get_Document};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opwrdxp::_di_Panes _scw_Get_Panes() { Opwrdxp::_di_Panes r; HRESULT hr = Get_Panes(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opwrdxp::_di_Panes Panes = {read=_scw_Get_Panes};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opwrdxp::_di_Selection _scw_Get_Selection() { Opwrdxp::_di_Selection r; HRESULT hr = Get_Selection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opwrdxp::_di_Selection Selection = {read=_scw_Get_Selection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Left() { int r; HRESULT hr = Get_Left(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Left = {read=_scw_Get_Left, write=Set_Left};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Top() { int r; HRESULT hr = Get_Top(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Top = {read=_scw_Get_Top, write=Set_Top};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Width() { int r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Width = {read=_scw_Get_Width, write=Set_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Height() { int r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Height = {read=_scw_Get_Height, write=Set_Height};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Split() { Word r; HRESULT hr = Get_Split(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Split = {read=_scw_Get_Split, write=Set_Split};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_SplitVertical() { int r; HRESULT hr = Get_SplitVertical(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int SplitVertical = {read=_scw_Get_SplitVertical, write=Set_SplitVertical};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Caption() { WideString r; HRESULT hr = Get_Caption(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Caption = {read=_scw_Get_Caption, write=Set_Caption};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_WindowState() { Activex::TOleEnum r; HRESULT hr = Get_WindowState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum WindowState = {read=_scw_Get_WindowState, write=Set_WindowState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisplayRulers() { Word r; HRESULT hr = Get_DisplayRulers(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisplayRulers = {read=_scw_Get_DisplayRulers, write=Set_DisplayRulers};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisplayVerticalRuler() { Word r; HRESULT hr = Get_DisplayVerticalRuler(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisplayVerticalRuler = {read=_scw_Get_DisplayVerticalRuler, write=Set_DisplayVerticalRuler};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opwrdxp::_di_View _scw_Get_View() { Opwrdxp::_di_View r; HRESULT hr = Get_View(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opwrdxp::_di_View View = {read=_scw_Get_View};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opwrdxp::_di_Window _scw_Get_Next() { Opwrdxp::_di_Window r; HRESULT hr = Get_Next(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opwrdxp::_di_Window Next = {read=_scw_Get_Next};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opwrdxp::_di_Window _scw_Get_Previous() { Opwrdxp::_di_Window r; HRESULT hr = Get_Previous(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opwrdxp::_di_Window Previous = {read=_scw_Get_Previous};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_WindowNumber() { int r; HRESULT hr = Get_WindowNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int WindowNumber = {read=_scw_Get_WindowNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisplayVerticalScrollBar() { Word r; HRESULT hr = Get_DisplayVerticalScrollBar(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisplayVerticalScrollBar = {read=_scw_Get_DisplayVerticalScrollBar, write=Set_DisplayVerticalScrollBar};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisplayHorizontalScrollBar() { Word r; HRESULT hr = Get_DisplayHorizontalScrollBar(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisplayHorizontalScrollBar = {read=_scw_Get_DisplayHorizontalScrollBar, write=Set_DisplayHorizontalScrollBar};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_StyleAreaWidth() { float r; HRESULT hr = Get_StyleAreaWidth(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float StyleAreaWidth = {read=_scw_Get_StyleAreaWidth, write=Set_StyleAreaWidth};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisplayScreenTips() { Word r; HRESULT hr = Get_DisplayScreenTips(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisplayScreenTips = {read=_scw_Get_DisplayScreenTips, write=Set_DisplayScreenTips};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_HorizontalPercentScrolled() { int r; HRESULT hr = Get_HorizontalPercentScrolled(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int HorizontalPercentScrolled = {read=_scw_Get_HorizontalPercentScrolled, write=Set_HorizontalPercentScrolled};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_VerticalPercentScrolled() { int r; HRESULT hr = Get_VerticalPercentScrolled(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int VerticalPercentScrolled = {read=_scw_Get_VerticalPercentScrolled, write=Set_VerticalPercentScrolled};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DocumentMap() { Word r; HRESULT hr = Get_DocumentMap(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DocumentMap = {read=_scw_Get_DocumentMap, write=Set_DocumentMap};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Active() { Word r; HRESULT hr = Get_Active(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Active = {read=_scw_Get_Active};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_DocumentMapPercentWidth() { int r; HRESULT hr = Get_DocumentMapPercentWidth(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int DocumentMapPercentWidth = {read=_scw_Get_DocumentMapPercentWidth, write=Set_DocumentMapPercentWidth};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Index() { int r; HRESULT hr = Get_Index(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Index = {read=_scw_Get_Index};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_IMEMode() { Activex::TOleEnum r; HRESULT hr = Get_IMEMode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum IMEMode = {read=_scw_Get_IMEMode, write=Set_IMEMode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_UsableWidth() { int r; HRESULT hr = Get_UsableWidth(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int UsableWidth = {read=_scw_Get_UsableWidth};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_UsableHeight() { int r; HRESULT hr = Get_UsableHeight(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int UsableHeight = {read=_scw_Get_UsableHeight};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_EnvelopeVisible() { Word r; HRESULT hr = Get_EnvelopeVisible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word EnvelopeVisible = {read=_scw_Get_EnvelopeVisible, write=Set_EnvelopeVisible};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisplayRightRuler() { Word r; HRESULT hr = Get_DisplayRightRuler(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisplayRightRuler = {read=_scw_Get_DisplayRightRuler, write=Set_DisplayRightRuler};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisplayLeftScrollBar() { Word r; HRESULT hr = Get_DisplayLeftScrollBar(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisplayLeftScrollBar = {read=_scw_Get_DisplayLeftScrollBar, write=Set_DisplayLeftScrollBar};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Visible() { Word r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Visible = {read=_scw_Get_Visible, write=Set_Visible};
};

#pragma option push -b-
enum TOpWdViewType { wdvtNormalView, wdvtOutlineView, wdvtPrintView, wdvtPrintPreview, wdvtMasterView, wdvtWebView };
#pragma option pop

#pragma option push -b-
enum TOpWdRangeLimit { wdrlParagraphs, wdrlSentences, wdrlWords, wdrlCharacters };
#pragma option pop

#pragma option push -b-
enum TOpWdSummaryMode { wdsmHighlight, wdsmHideAllButSummary, wdsmInsert, wdsmCreateNew };
#pragma option pop

#pragma option push -b-
enum TOpWdWindowState { wdwsNormal, wdwsMaximized, wdwsMinimized };
#pragma option pop

#pragma option push -b-
enum TOpWdAlertLevel { wdalNone, wdalMessageBox, wdalAll };
#pragma option pop

#pragma option push -b-
enum TOpWdEnableCancelKey { wdeckDisabled, wdeckInterrupt };
#pragma option pop

#pragma option push -b-
enum TOpWdFontAnimation { wdfaNone, wdfaLasVegasLights, wdfaBlinkingBackground, wdfaSparkleText, wdfaMarchingBlackAnts, wdfaMarchingRedAnts, wdfaShimmer };
#pragma option pop

#pragma option push -b-
enum TOpWdDocumentKind { wddkNotSpecified, wddkLetter, wddkEmail };
#pragma option pop

#pragma option push -b-
enum TOpWdReplaceOption { wdroReplaceNone, wdroReplaceOne, wdroReplaceAll };
#pragma option pop

class DELPHICLASS TOpMailMerge;
class PASCALIMPLEMENTATION TOpMailMerge : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	Opwrdxp::_di_MailMerge FIntf;
	Opmodels::TOpUnknownComponent* FModel;
	void __fastcall SetModel(const Opmodels::TOpUnknownComponent* Value);
	
public:
	void __fastcall SetDoc(Opwrdxp::_di__Document Doc);
	__property Opwrdxp::_di_MailMerge AsMailMerge = {read=FIntf};
	
__published:
	__property Opmodels::TOpUnknownComponent* OfficeModel = {read=FModel, write=SetModel};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TOpMailMerge(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TOpMailMerge(void) : Classes::TPersistent() { }
	#pragma option pop
	
};


class DELPHICLASS TOpDocumentShape;
class PASCALIMPLEMENTATION TOpDocumentShape : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
private:
	float FLeft;
	float FTop;
	float FWidth;
	float FHeight;
	WideString FShapeName;
	void __fastcall SetHeight(const float Value);
	void __fastcall SetLeft(const float Value);
	void __fastcall SetTop(const float Value);
	void __fastcall SetWidth(const float Value);
	void __fastcall SetShapeName(const WideString Value);
	
protected:
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	
public:
	__fastcall virtual TOpDocumentShape(Classes::TCollection* Collection);
	virtual void __fastcall Activate(void);
	void __fastcall AddTextBox(void);
	virtual void __fastcall Connect(void);
	virtual void __fastcall ExecuteVerb(int index);
	
__published:
	__property float Left = {read=FLeft, write=SetLeft};
	__property float Top = {read=FTop, write=SetTop};
	__property float Width = {read=FWidth, write=SetWidth};
	__property float Height = {read=FHeight, write=SetHeight};
	__property WideString ShapeName = {read=FShapeName, write=SetShapeName};
public:
	#pragma option push -w-inl
	/* TCollectionItem.Destroy */ inline __fastcall virtual ~TOpDocumentShape(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpDocumentShapes;
class PASCALIMPLEMENTATION TOpDocumentShapes : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpDocumentShape* operator[](int Index) { return Items[Index]; }
	
private:
	HIDESBASE TOpDocumentShape* __fastcall GetItem(int Index);
	HIDESBASE void __fastcall SetItem(int Index, TOpDocumentShape* Value);
	
protected:
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpDocumentShape* __fastcall Add(void);
	__property TOpDocumentShape* Items[int Index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpDocumentShapes(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpDocumentShapes(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpDocumentHyperLink;
class PASCALIMPLEMENTATION TOpDocumentHyperLink : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
private:
	WideString FAddress;
	bool FNewWindow;
	WideString __fastcall GetAddress();
	void __fastcall SetAddress(const WideString Value);
	void __fastcall SetNewWindow(const bool Value);
	
protected:
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	
public:
	virtual void __fastcall Activate(void);
	virtual void __fastcall ExecuteVerb(int index);
	void __fastcall FollowHyperLink(void);
	
__published:
	__property WideString Address = {read=GetAddress, write=SetAddress};
	__property bool NewWindow = {read=FNewWindow, write=SetNewWindow, nodefault};
public:
	#pragma option push -w-inl
	/* TOpNestedCollectionItem.Create */ inline __fastcall virtual TOpDocumentHyperLink(Classes::TCollection* Collection) : Opshared::TOpNestedCollectionItem(Collection) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollectionItem.Destroy */ inline __fastcall virtual ~TOpDocumentHyperLink(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpDocumentHyperLinks;
class PASCALIMPLEMENTATION TOpDocumentHyperLinks : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpDocumentHyperLink* operator[](int Index) { return Items[Index]; }
	
private:
	HIDESBASE TOpDocumentHyperLink* __fastcall GetItem(int Index);
	HIDESBASE void __fastcall SetItem(int Index, TOpDocumentHyperLink* Value);
	
protected:
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpDocumentHyperLink* __fastcall Add(void);
	__property TOpDocumentHyperLink* Items[int Index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpDocumentHyperLinks(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpDocumentHyperLinks(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpDocumentBookmark;
class PASCALIMPLEMENTATION TOpDocumentBookmark : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
private:
	WideString FBookmarkName;
	WideString __fastcall GetBookMarkName();
	void __fastcall SetBookmarkName(const WideString Value);
	
protected:
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	
public:
	virtual void __fastcall Activate(void);
	
__published:
	__property WideString BookmarkName = {read=GetBookMarkName, write=SetBookmarkName};
public:
	#pragma option push -w-inl
	/* TOpNestedCollectionItem.Create */ inline __fastcall virtual TOpDocumentBookmark(Classes::TCollection* Collection) : Opshared::TOpNestedCollectionItem(Collection) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollectionItem.Destroy */ inline __fastcall virtual ~TOpDocumentBookmark(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpDocumentBookmarks;
class PASCALIMPLEMENTATION TOpDocumentBookmarks : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpDocumentBookmark* operator[](int Index) { return Items[Index]; }
	
private:
	HIDESBASE TOpDocumentBookmark* __fastcall GetItem(int Index);
	HIDESBASE void __fastcall SetItem(int Index, TOpDocumentBookmark* Value);
	
protected:
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpDocumentBookmark* __fastcall Add(void);
	__property TOpDocumentBookmark* Items[int Index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpDocumentBookmarks(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpDocumentBookmarks(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpDocumentTable;
class PASCALIMPLEMENTATION TOpDocumentTable : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
private:
	Opmodels::TOpUnknownComponent* FModel;
	void __fastcall SetModel(Opmodels::TOpUnknownComponent* Value);
	
protected:
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	
public:
	virtual void __fastcall Activate(void);
	virtual void __fastcall ExecuteVerb(int index);
	void __fastcall PopulateDocTable(void);
	void __fastcall RePopulateDocTable(void);
	
__published:
	__property Opmodels::TOpUnknownComponent* OfficeModel = {read=FModel, write=SetModel};
public:
	#pragma option push -w-inl
	/* TOpNestedCollectionItem.Create */ inline __fastcall virtual TOpDocumentTable(Classes::TCollection* Collection) : Opshared::TOpNestedCollectionItem(Collection) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollectionItem.Destroy */ inline __fastcall virtual ~TOpDocumentTable(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpDocumentTables;
class PASCALIMPLEMENTATION TOpDocumentTables : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpDocumentTable* operator[](int Index) { return Items[Index]; }
	
private:
	HIDESBASE TOpDocumentTable* __fastcall GetItem(int Index);
	HIDESBASE void __fastcall SetItem(int Index, TOpDocumentTable* Value);
	
protected:
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpDocumentTable* __fastcall Add(void);
	__property TOpDocumentTable* Items[int Index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpDocumentTables(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpDocumentTables(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpWordDocument;
class PASCALIMPLEMENTATION TOpWordDocument : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
private:
	AnsiString FName;
	TOpDocumentBookmarks* FDocumentBookmarks;
	TOpDocumentTables* FDocumentTables;
	TOpDocumentHyperLinks* FDocumentHyperLinks;
	TOpDocumentShapes* FDocumentShapes;
	AnsiString FDocFile;
	TOpWdDocumentKind FDocumentKind;
	Word FShowSummary;
	TOpMailMerge* FMailMerge;
	int FSummaryLength;
	int FHyphenationZone;
	int FConsecutiveHyphensLimit;
	float FDefaultTabStop;
	TOpWdSummaryMode FSummaryViewMode;
	WideString FCodeName;
	Word FPrintRevisions;
	Word FHyphenateCaps;
	Word FPrintPostScriptOverText;
	Word FSpellingChecked;
	Word FShowRevisions;
	Word FSaveFormsData;
	Word FPrintFormsData;
	Word FGrammarChecked;
	Word FUserControl;
	Word FShowGrammaticalErrors;
	Word FReadOnlyRecommended;
	Word FSaveSubsetFonts;
	Word FTrackRevisions;
	Word FUpdateStylesOnOpen;
	Word FEmbedTrueTypeFonts;
	Word FHasRoutingSlip;
	Word FSaved;
	Word FAutoHyphenation;
	Word FShowSpellingErrors;
	TOpWdRangeLimit FdocRangeLimit;
	int FRangeStart;
	int FRangeEnd;
	TOpWdViewType FViewType;
	Opwrdxp::_di_Range FFindRange;
	AnsiString FFindText;
	void __fastcall SetSummary(const Word Value);
	TOpWdDocumentKind __fastcall GetDocumentKind(void);
	void __fastcall SetDocumentKind(const TOpWdDocumentKind Value);
	void __fastcall SetMailMerge(const TOpMailMerge* Value);
	void __fastcall SetAutoHyphenation(const Word Value);
	void __fastcall SetConsecutiveHyphensLimit(const int Value);
	void __fastcall SetDefaultTabStop(const float Value);
	void __fastcall SetEmbedTrueTypeFonts(const Word Value);
	void __fastcall SetGrammarChecked(const Word Value);
	void __fastcall SetHasRoutingSlip(const Word Value);
	void __fastcall SetHyphenateCaps(const Word Value);
	void __fastcall SetHyphenationZone(const int Value);
	void __fastcall SetPrintFormsData(const Word Value);
	void __fastcall SetPrintPostScriptOverText(const Word Value);
	void __fastcall SetPrintRevisions(const Word Value);
	void __fastcall SetReadOnlyRecommended(const Word Value);
	void __fastcall SetSaved(const Word Value);
	void __fastcall SetSaveFormsData(const Word Value);
	void __fastcall SetSaveSubsetFonts(const Word Value);
	void __fastcall SetShowGrammaticalErrors(const Word Value);
	void __fastcall SetShowRevisions(const Word Value);
	void __fastcall SetShowSpellingErrors(const Word Value);
	void __fastcall SetSpellingChecked(const Word Value);
	void __fastcall SetSummaryLength(const int Value);
	void __fastcall SetSummaryViewMode(const TOpWdSummaryMode Value);
	void __fastcall SetTrackRevisions(const Word Value);
	void __fastcall SetUpdateStylesOnOpen(const Word Value);
	void __fastcall SetUserControl(const Word Value);
	Word __fastcall GetAutoHyphenation(void);
	WideString __fastcall GetCodeName();
	int __fastcall GetConsecutiveHyphensLimit(void);
	float __fastcall GetDefaultTabStop(void);
	Word __fastcall GetEmbedTrueTypeFonts(void);
	Word __fastcall GetGrammarChecked(void);
	Word __fastcall GetHasRoutingSlip(void);
	Word __fastcall GetHyphenateCaps(void);
	int __fastcall GetHyphenationZone(void);
	AnsiString __fastcall GetName();
	Word __fastcall GetPrintFormsData(void);
	Word __fastcall GetPrintPostScriptOverText(void);
	Word __fastcall GetPrintRevisions(void);
	Word __fastcall GetReadOnlyRecommended(void);
	Word __fastcall GetSaved(void);
	Word __fastcall GetSaveFormsData(void);
	Word __fastcall GetSaveSubsetFonts(void);
	Word __fastcall GetShowGrammaticalErrors(void);
	Word __fastcall GetShowRevisions(void);
	Word __fastcall GetShowSpellingErrors(void);
	Word __fastcall GetShowSummary(void);
	Word __fastcall GetSpellingChecked(void);
	int __fastcall GetSummaryLength(void);
	TOpWdSummaryMode __fastcall GetSummaryViewMode(void);
	Word __fastcall GetTrackRevisions(void);
	Word __fastcall GetUpdateStylesOnOpen(void);
	Word __fastcall GetUserControl(void);
	void __fastcall SetRangeLimit(const TOpWdRangeLimit Value);
	void __fastcall SetCodeName(const WideString Value);
	TOpWdViewType __fastcall GetViewType(void);
	void __fastcall SetViewType(const TOpWdViewType Value);
	Opshared::TOpPropDirection __fastcall GetPropDirection(void);
	bool __fastcall SaveCollection(void);
	Opwrdxp::_di__Document __fastcall GetAsDocument();
	
protected:
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	virtual void __fastcall CreateSubCollections(void);
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	void __fastcall SetDocFile(const AnsiString Value);
	
public:
	__fastcall virtual TOpWordDocument(Classes::TCollection* Collection);
	__fastcall virtual ~TOpWordDocument(void);
	virtual void __fastcall Activate(void);
	void __fastcall AnimateSelection(TOpWdFontAnimation Value);
	virtual void __fastcall Connect(void);
	virtual void __fastcall ExecuteVerb(int index);
	void __fastcall ExecuteMailMerge(void);
	bool __fastcall Find(const AnsiString FindText, bool Forward);
	bool __fastcall FindNext(void);
	void __fastcall Insert(const WideString Text, bool AtEnd);
	void __fastcall InsertStrings(Classes::TStrings* Lines, bool AtEnd);
	void __fastcall PopulateMailMerge(void);
	void __fastcall Print(void);
	bool __fastcall Replace(const AnsiString FindText, const AnsiString ReplaceText, bool Forward, TOpWdReplaceOption ReplaceOption);
	void __fastcall Save(void);
	void __fastcall SaveAs(const AnsiString FileName);
	void __fastcall SendMailWithOutLook(const AnsiString WordSubject, const AnsiString SendTo, const AnsiString SendCC, const AnsiString SendBC, Opoutlk::TOpOutlook* OutlookComp, bool AsAttachment);
	__property Opwrdxp::_di__Document AsDocument = {read=GetAsDocument};
	__property Opshared::TOpPropDirection PropDirection = {read=GetPropDirection, nodefault};
	
__published:
	__property AnsiString Name = {read=GetName, stored=SaveCollection};
	__property Word ShowSummary = {read=GetShowSummary, write=SetSummary, stored=SaveCollection, nodefault};
	__property TOpWdDocumentKind DocumentKind = {read=GetDocumentKind, write=SetDocumentKind, stored=SaveCollection, nodefault};
	__property TOpDocumentBookmarks* Bookmarks = {read=FDocumentBookmarks, write=FDocumentBookmarks, stored=SaveCollection};
	__property TOpDocumentTables* Tables = {read=FDocumentTables, write=FDocumentTables, stored=SaveCollection};
	__property TOpDocumentHyperLinks* HyperLinks = {read=FDocumentHyperLinks, write=FDocumentHyperLinks, stored=SaveCollection};
	__property TOpDocumentShapes* Shapes = {read=FDocumentShapes, write=FDocumentShapes, stored=SaveCollection};
	__property TOpMailMerge* MailMerge = {read=FMailMerge, write=SetMailMerge};
	__property AnsiString DocFile = {read=FDocFile, write=SetDocFile};
	__property Word AutoHyphenation = {read=GetAutoHyphenation, write=SetAutoHyphenation, stored=SaveCollection, nodefault};
	__property Word HyphenateCaps = {read=GetHyphenateCaps, write=SetHyphenateCaps, stored=SaveCollection, nodefault};
	__property int HyphenationZone = {read=GetHyphenationZone, write=SetHyphenationZone, stored=SaveCollection, nodefault};
	__property int ConsecutiveHyphensLimit = {read=GetConsecutiveHyphensLimit, write=SetConsecutiveHyphensLimit, stored=SaveCollection, nodefault};
	__property Word HasRoutingSlip = {read=GetHasRoutingSlip, write=SetHasRoutingSlip, stored=SaveCollection, nodefault};
	__property Word Saved = {read=GetSaved, write=SetSaved, stored=SaveCollection, nodefault};
	__property float DefaultTabStop = {read=GetDefaultTabStop, write=SetDefaultTabStop, stored=SaveCollection};
	__property Word EmbedTrueTypeFonts = {read=GetEmbedTrueTypeFonts, write=SetEmbedTrueTypeFonts, stored=SaveCollection, nodefault};
	__property Word SaveFormsData = {read=GetSaveFormsData, write=SetSaveFormsData, stored=SaveCollection, nodefault};
	__property Word ReadOnlyRecommended = {read=GetReadOnlyRecommended, write=SetReadOnlyRecommended, stored=SaveCollection, nodefault};
	__property Word SaveSubsetFonts = {read=GetSaveSubsetFonts, write=SetSaveSubsetFonts, stored=SaveCollection, nodefault};
	__property Word UpdateStylesOnOpen = {read=GetUpdateStylesOnOpen, write=SetUpdateStylesOnOpen, stored=SaveCollection, nodefault};
	__property Word GrammarChecked = {read=GetGrammarChecked, write=SetGrammarChecked, stored=SaveCollection, nodefault};
	__property Word SpellingChecked = {read=GetSpellingChecked, write=SetSpellingChecked, stored=SaveCollection, nodefault};
	__property Word ShowGrammaticalErrors = {read=GetShowGrammaticalErrors, write=SetShowGrammaticalErrors, stored=SaveCollection, nodefault};
	__property Word ShowSpellingErrors = {read=GetShowSpellingErrors, write=SetShowSpellingErrors, stored=SaveCollection, nodefault};
	__property TOpWdSummaryMode SummaryViewMode = {read=GetSummaryViewMode, write=SetSummaryViewMode, stored=SaveCollection, nodefault};
	__property int SummaryLength = {read=GetSummaryLength, write=SetSummaryLength, stored=SaveCollection, nodefault};
	__property Word PrintPostScriptOverText = {read=GetPrintPostScriptOverText, write=SetPrintPostScriptOverText, stored=SaveCollection, nodefault};
	__property Word PrintFormsData = {read=GetPrintFormsData, write=SetPrintFormsData, stored=SaveCollection, nodefault};
	__property Word UserControl = {read=GetUserControl, write=SetUserControl, stored=SaveCollection, nodefault};
	__property WideString CodeName = {read=GetCodeName, write=SetCodeName, stored=SaveCollection};
	__property Word TrackRevisions = {read=GetTrackRevisions, write=SetTrackRevisions, stored=SaveCollection, nodefault};
	__property Word PrintRevisions = {read=GetPrintRevisions, write=SetPrintRevisions, stored=SaveCollection, nodefault};
	__property Word ShowRevisions = {read=GetShowRevisions, write=SetShowRevisions, stored=SaveCollection, nodefault};
	__property TOpWdRangeLimit RangeLimit = {read=FdocRangeLimit, write=SetRangeLimit, stored=SaveCollection, nodefault};
	__property int RangeStart = {read=FRangeStart, write=FRangeStart, stored=SaveCollection, nodefault};
	__property int RangeEnd = {read=FRangeEnd, write=FRangeEnd, stored=SaveCollection, nodefault};
	__property TOpWdViewType ViewType = {read=GetViewType, write=SetViewType, stored=SaveCollection, nodefault};
};


class DELPHICLASS TOpWordDocuments;
class PASCALIMPLEMENTATION TOpWordDocuments : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpWordDocument* operator[](int index) { return Items[index]; }
	
protected:
	HIDESBASE TOpWordDocument* __fastcall GetItem(int Index);
	HIDESBASE void __fastcall SetItem(int Index, const TOpWordDocument* Value);
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpWordDocument* __fastcall Add(void);
	__property TOpWordDocument* Items[int index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpWordDocuments(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpWordDocuments(void) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TOnDocumentOpen)(System::TObject* Sender, Opwrdxp::_di__Document Doc);

typedef void __fastcall (__closure *TOnDocumentBeforeClose)(System::TObject* Sender, Opwrdxp::_di__Document Doc, Word &Cancel);

typedef void __fastcall (__closure *TOnDocumentBeforePrint)(System::TObject* Sender, Opwrdxp::_di__Document Doc, Word &Cancel);

typedef void __fastcall (__closure *TOnDocumentBeforeSave)(System::TObject* Sender, Opwrdxp::_di__Document Doc, Word &SaveAsUI, Word &Cancel);

typedef void __fastcall (__closure *TOnNewDocument)(System::TObject* Sender, Opwrdxp::_di__Document Doc);

typedef void __fastcall (__closure *TOnWindowActivate)(System::TObject* Sender, Opwrdxp::_di__Document Doc, const _di_WordWindow Wn);

typedef void __fastcall (__closure *TOnWindowDeactivate)(System::TObject* Sender, Opwrdxp::_di__Document Doc, const _di_WordWindow Wn);

typedef void __fastcall (__closure *TOnWindowSelectionChange)(System::TObject* Sender, Opwrdxp::_di_Selection Sel);

typedef void __fastcall (__closure *TOnWindowBeforeRightClick)(System::TObject* Sender, Opwrdxp::_di_Selection Sel, Word &Cancel);

typedef void __fastcall (__closure *TOnWindowBeforeDoubleClick)(System::TObject* Sender, Opwrdxp::_di_Selection Sel, Word &Cancel);

class DELPHICLASS TOpWord;
class PASCALIMPLEMENTATION TOpWord : public Opwrdxp::TOpWordBase 
{
	typedef Opwrdxp::TOpWordBase inherited;
	
protected:
	Opwrdxp::_di__Application FServer;
	bool FVisible;
	AnsiString FCaption;
	TOpWordDocuments* FDocuments;
	Classes::TNotifyEvent FOnStartup;
	Classes::TNotifyEvent FOnQuit;
	Classes::TNotifyEvent FOnDocumentChange;
	TOnDocumentOpen FOnDocumentOpen;
	TOnDocumentBeforeClose FOnDocumentBeforeClose;
	TOnDocumentBeforePrint FOnDocumentBeforePrint;
	TOnDocumentBeforeSave FOnDocumentBeforeSave;
	TOnNewDocument FOnNewDocument;
	TOnWindowActivate FOnWindowActivate;
	TOnWindowDeactivate FOnWindowDeactivate;
	TOnWindowSelectionChange FOnWindowSelectionChange;
	TOnWindowBeforeRightClick FOnWindowBeforeRightClick;
	TOnWindowBeforeDoubleClick FOnWindowBeforeDoubleClick;
	TOpWdAlertLevel FDisplayAlerts;
	TOpWdEnableCancelKey FEnableCancelKey;
	TOpWdWindowState FWindowState;
	WideString FStartupPath;
	WideString FUserInitials;
	WideString FUserAddress;
	WideString FDefaultSaveFormat;
	WideString FBrowseExtraFileTypes;
	WideString FDefaultTableSeparator;
	WideString FUserName;
	Word FDisplayRecentFiles;
	Word FScreenUpdating;
	Word FPrintPreview;
	Word FDisplayScrollBars;
	Word FDisplayScreenTips;
	int FHeight;
	int FTop;
	int FLeft;
	int FWidth;
	void __fastcall Setcaption(const AnsiString Value);
	bool __fastcall CollectionNotify(void *Key, Opshared::TOpNestedCollectionItem* Item);
	void __fastcall FixupCollection(Opshared::TOpNestedCollectionItem* Item);
	void __fastcall ReleaseCollectionInterface(Opshared::TOpNestedCollectionItem* Item);
	void __fastcall SetBrowseExtraFileTypes(const WideString Value);
	void __fastcall SetDefaultSaveFormat(const WideString Value);
	void __fastcall SetDefaultTableSeparator(const WideString Value);
	void __fastcall SetDisplayAlerts(const TOpWdAlertLevel Value);
	void __fastcall SetDisplayRecentFiles(const Word Value);
	void __fastcall SetDisplayScreenTips(const Word Value);
	void __fastcall SetDisplayScrollBars(const Word Value);
	void __fastcall SetEnableCancelKey(const TOpWdEnableCancelKey Value);
	void __fastcall SetPrintPreview(const Word Value);
	void __fastcall SetScreenUpdating(const Word Value);
	void __fastcall SetStartupPath(const WideString Value);
	void __fastcall SetUserAddress(const WideString Value);
	void __fastcall SetUserInitials(const WideString Value);
	void __fastcall SetUserName(const WideString Value);
	void __fastcall SetWindowState(const TOpWdWindowState Value);
	WideString __fastcall GetBrowseExtraFileTypes();
	WideString __fastcall GetDefaultSaveFormat();
	WideString __fastcall GetDefaultTableSeparator();
	TOpWdAlertLevel __fastcall GetDisplayAlerts(void);
	Word __fastcall GetDisplayRecentFiles(void);
	Word __fastcall GetDisplayScreenTips(void);
	Word __fastcall GetDisplayScrollBars(void);
	TOpWdEnableCancelKey __fastcall GetEnableCancelKey(void);
	Word __fastcall GetPrintPreview(void);
	Word __fastcall GetScreenUpdating(void);
	WideString __fastcall GetStartupPath();
	WideString __fastcall GetUserAddress();
	WideString __fastcall GetUserInitials();
	WideString __fastcall GetUserName();
	TOpWdWindowState __fastcall GetWindowState(void);
	AnsiString __fastcall GetCaption();
	int __fastcall GetHeight(void);
	int __fastcall GetLeft(void);
	int __fastcall GetTop(void);
	int __fastcall GetWidth(void);
	void __fastcall SetHeight(const int Value);
	void __fastcall SetLeft(const int Value);
	void __fastcall SetTop(const int Value);
	void __fastcall SetWidth(const int Value);
	bool __fastcall GetVisible(void);
	virtual void __fastcall DoConnect(void);
	virtual void __fastcall DoDisconnect(void);
	virtual void __fastcall FixupProps(void);
	Opwrdxp::_di__Application __fastcall GetWordInstance();
	void __fastcall SetVisible(const bool Value);
	virtual bool __fastcall GetConnected(void);
	virtual AnsiString __fastcall GetOfficeVersionStr();
	void __fastcall DoApplicationStartUp(void);
	void __fastcall DoApplicationQuit(void);
	void __fastcall DoDocumentChange(void);
	void __fastcall DoDocumentOpen(const Opwrdxp::_di__Document Doc);
	void __fastcall DoDocumentBeforeClose(const Opwrdxp::_di__Document Doc, Word &Cancel);
	void __fastcall DoDocumentBeforePrint(const Opwrdxp::_di__Document Doc, Word &Cancel);
	void __fastcall DoDocumentBeforeSave(const Opwrdxp::_di__Document Doc, Word &SaveAsUI, Word &Cancel);
	void __fastcall DoNewDocument(const Opwrdxp::_di__Document Doc);
	void __fastcall DoWindowActivate(const Opwrdxp::_di__Document Doc, const _di_WordWindow Wn);
	void __fastcall DoWindowDeactivate(const Opwrdxp::_di__Document Doc, const _di_WordWindow Wn);
	void __fastcall DoWindowSelectionChange(const Opwrdxp::_di_Selection Sel);
	void __fastcall DoWindowBeforeRightClick(const Opwrdxp::_di_Selection Sel, Word &Cancel);
	void __fastcall DoWindowBeforeDoubleClick(const Opwrdxp::_di_Selection Sel, Word &Cancel);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	int __fastcall PointsToPixels(int Points);
	int __fastcall PixelsToPoints(int Pixels);
	int __fastcall PixelsPerInch(void);
	TOpWordDocument* __fastcall GetActiveDocument(void);
	
public:
	__fastcall virtual TOpWord(Classes::TComponent* AOwner);
	__fastcall virtual ~TOpWord(void);
	virtual _di_IDispatch __fastcall CreateItem(Opshared::TOpNestedCollectionItem* Item);
	virtual void __fastcall GetAppInfo(Classes::TStrings* Info);
	virtual void __fastcall HandleEvent(const GUID &IID, int DispId, const tagVARIANT * Params);
	virtual void __fastcall GetFileInfo(AnsiString &Filter, AnsiString &DefExt);
	TOpWordDocument* __fastcall OpenDocument(const AnsiString FileName);
	TOpWordDocument* __fastcall NewDocument(void);
	__property TOpWordDocument* ActiveDocument = {read=GetActiveDocument};
	__property Opwrdxp::_di__Application Server = {read=GetWordInstance};
	
__published:
	__property AnsiString Caption = {read=GetCaption, write=Setcaption};
	__property Connected  = {default=0};
	__property TOpWordDocuments* Documents = {read=FDocuments, write=FDocuments};
	__property PropDirection  = {default=0};
	__property MachineName ;
	__property bool Visible = {read=GetVisible, write=SetVisible, nodefault};
	__property Word ScreenUpdating = {read=GetScreenUpdating, write=SetScreenUpdating, nodefault};
	__property Word PrintPreview = {read=GetPrintPreview, write=SetPrintPreview, nodefault};
	__property WideString UserName = {read=GetUserName, write=SetUserName};
	__property WideString UserInitials = {read=GetUserInitials, write=SetUserInitials};
	__property WideString UserAddress = {read=GetUserAddress, write=SetUserAddress};
	__property Word DisplayRecentFiles = {read=GetDisplayRecentFiles, write=SetDisplayRecentFiles, nodefault};
	__property WideString DefaultSaveFormat = {read=GetDefaultSaveFormat, write=SetDefaultSaveFormat};
	__property Word DisplayScrollBars = {read=GetDisplayScrollBars, write=SetDisplayScrollBars, nodefault};
	__property WideString StartupPath = {read=GetStartupPath, write=SetStartupPath, stored=false};
	__property int ServerLeft = {read=GetLeft, write=SetLeft, nodefault};
	__property int ServerTop = {read=GetTop, write=SetTop, nodefault};
	__property int ServerWidth = {read=GetWidth, write=SetWidth, nodefault};
	__property int ServerHeight = {read=GetHeight, write=SetHeight, nodefault};
	__property TOpWdWindowState WindowState = {read=GetWindowState, write=SetWindowState, nodefault};
	__property TOpWdAlertLevel DisplayAlerts = {read=GetDisplayAlerts, write=SetDisplayAlerts, nodefault};
	__property Word DisplayScreenTips = {read=GetDisplayScreenTips, write=SetDisplayScreenTips, nodefault};
	__property TOpWdEnableCancelKey EnableCancelKey = {read=GetEnableCancelKey, write=SetEnableCancelKey, nodefault};
	__property WideString DefaultTableSeparator = {read=GetDefaultTableSeparator, write=SetDefaultTableSeparator};
	__property WideString BrowseExtraFileTypes = {read=GetBrowseExtraFileTypes, write=SetBrowseExtraFileTypes};
	__property OnOpConnect ;
	__property OnOpDisconnect ;
	__property OnGetInstance ;
	__property Classes::TNotifyEvent OnStartup = {read=FOnStartup, write=FOnStartup};
	__property Classes::TNotifyEvent OnQuit = {read=FOnQuit, write=FOnQuit};
	__property Classes::TNotifyEvent OnDocumentChange = {read=FOnDocumentChange, write=FOnDocumentChange};
	__property TOnDocumentOpen OnDocumentOpen = {read=FOnDocumentOpen, write=FOnDocumentOpen};
	__property TOnDocumentBeforeClose BeforeDocumentClose = {read=FOnDocumentBeforeClose, write=FOnDocumentBeforeClose};
	__property TOnDocumentBeforePrint BeforeDocumentPrint = {read=FOnDocumentBeforePrint, write=FOnDocumentBeforePrint};
	__property TOnDocumentBeforeSave BeforeDocumentSave = {read=FOnDocumentBeforeSave, write=FOnDocumentBeforeSave};
	__property TOnNewDocument OnNewDocument = {read=FOnNewDocument, write=FOnNewDocument};
	__property TOnWindowActivate OnWindowActivate = {read=FOnWindowActivate, write=FOnWindowActivate};
	__property TOnWindowDeactivate OnWindowDeactivate = {read=FOnWindowDeactivate, write=FOnWindowDeactivate};
	__property TOnWindowSelectionChange OnWindowSelectionChange = {read=FOnWindowSelectionChange, write=FOnWindowSelectionChange};
	__property TOnWindowBeforeRightClick BeforeWindowRightClick = {read=FOnWindowBeforeRightClick, write=FOnWindowBeforeRightClick};
	__property TOnWindowBeforeDoubleClick BeforeWindowDoubleClick = {read=FOnWindowBeforeDoubleClick, write=FOnWindowBeforeDoubleClick};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE int __stdcall (*SQLConfigDataSource)(HWND hwndParent, int fRequest, char * lpszDriver, char * lpszAttributes);

}	/* namespace Opword */
using namespace Opword;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpWord
