// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpExcel.pas' rev: 6.00

#ifndef OpExcelHPP
#define OpExcelHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Variants.hpp>	// Pascal unit
#include <OleCtrls.hpp>	// Pascal unit
#include <OpOfcXP.hpp>	// Pascal unit
#include <OpModels.hpp>	// Pascal unit
#include <OpShared.hpp>	// Pascal unit
#include <OpXLXP.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <ComObj.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opexcel
{
//-- type declarations -------------------------------------------------------
__dispinterface ExcelRange;
typedef System::DelphiInterface<ExcelRange> _di_ExcelRange;
__dispinterface INTERFACE_UUID("{00020846-0000-0000-C000-000000000046}") ExcelRange  : public IDispatch 
{
	
};

__dispinterface ExcelWindow;
typedef System::DelphiInterface<ExcelWindow> _di_ExcelWindow;
__dispinterface INTERFACE_UUID("{00020893-0000-0000-C000-000000000046}") ExcelWindow  : public IDispatch 
{
	
};

class DELPHICLASS ERangeException;
class PASCALIMPLEMENTATION ERangeException : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall ERangeException(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall ERangeException(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall ERangeException(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall ERangeException(int Ident, const System::TVarRec * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall ERangeException(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall ERangeException(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall ERangeException(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall ERangeException(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~ERangeException(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TOpXlCellOrientation { xlcoDownward, xlcoHorizontal, xlcoUpward, xlcoVertical, xlcoRotated };
#pragma option pop

#pragma option push -b-
enum TOpXlCellHorizAlign { xlchaGeneral, xlchaLeft, xlchaCenter, xlchaRight, xlchaFill, xlchaJustify, xlchaCenterAcrossSelection };
#pragma option pop

#pragma option push -b-
enum TOpXlCellVertAlign { xlcvaTop, xlcvaCenter, xlcvaBottom, xlcvaJustify, xlcvaDistributed };
#pragma option pop

#pragma option push -b-
enum TOpXlInteriorPatterns { xlipPatternAutomatic, xlipPatternChecker, xlipPatternCrissCross, xlipPatternDown, xlipPatternGray16, xlipPatternGray25, xlipPatternGray50, xlipPatternGray75, xlipPatternGray8, xlipPatternGrid, xlipPatternHorizontal, xlipPatternLightDown, xlipPatternLightHorizontal, xlipPatternLightUp, xlipPatternLightVertical, xlipPatternNone, xlipPatternSemiGray75, xlipPatternSolid, xlipPatternUp, xlipPatternVertical };
#pragma option pop

#pragma option push -b-
enum TOpXlBorderLineStyles { xlblsContinuous, xlblsDash, xlblsDashDot, xlblsDashDotDot, xlblsDot, xlblsDouble, xlblsLineStyleNone, xlblsSlantDashDot };
#pragma option pop

#pragma option push -b-
enum TOpXlBorderWeights { xlbwHairline, xlbwThin, xlbwMedium, xlbwThick };
#pragma option pop

#pragma option push -b-
enum TOpXlBorders { xlbLeft, xlbRight, xlbTop, xlbBottom };
#pragma option pop

#pragma option push -b-
enum TOpXlFontAttributes { xlfaBold, xlfaItalic, xlfaUnderline, xlfaStrikethrough };
#pragma option pop

typedef Set<TOpXlBorders, xlbLeft, xlbBottom>  TOpXlRangeBorders;

typedef Set<TOpXlFontAttributes, xlfaBold, xlfaStrikethrough>  TOpXlRangeFontAttributes;

#pragma option push -b-
enum TOpXlCancelKey { xlckDisabled, xlckErrorHandler, xlckInterrupt };
#pragma option pop

#pragma option push -b-
enum TOpXlWindowState { xlwsNormal, xlwsMinimized, xlwsMaximized };
#pragma option pop

#pragma option push -b-
enum TOpXlChartType { xlctArea, xlctBar, xlctLine, xlctPie, xlctRadar, xlctXYScatter, xlct3DArea, xlct3DBar, xlct3DLine, xlct3DPie, xlct3DColumn, xlctDoughnut, xlctUnknown };
#pragma option pop

typedef void __fastcall (__closure *TOnNewWorkBook)(System::TObject* Sender, const Opxlxp::_di__Workbook WorkBook);

typedef void __fastcall (__closure *TOnSheetSelectionChange)(System::TObject* Sender, const _di_IDispatch Sh, const _di_ExcelRange Target);

typedef void __fastcall (__closure *TBeforeSheetDoubleClick)(System::TObject* Sender, const _di_IDispatch Sh, const _di_ExcelRange Target, Word &Cancel);

typedef void __fastcall (__closure *TBeforeSheetRightClick)(System::TObject* Sender, const _di_IDispatch Sh, const _di_ExcelRange Target, Word &Cancel);

typedef void __fastcall (__closure *TOnSheetActivate)(System::TObject* Sender, const _di_IDispatch Sh);

typedef void __fastcall (__closure *TOnSheetDeactivate)(System::TObject* Sender, const _di_IDispatch Sh);

typedef void __fastcall (__closure *TOnSheetCalculate)(System::TObject* Sender, const _di_IDispatch Sh);

typedef void __fastcall (__closure *TOnSheetChange)(System::TObject* Sender, const _di_IDispatch Sh, const _di_ExcelRange Target);

typedef void __fastcall (__closure *TOnWorkbookOpen)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb);

typedef void __fastcall (__closure *TOnWorkbookActivate)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb);

typedef void __fastcall (__closure *TOnWorkbookDeactivate)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb);

typedef void __fastcall (__closure *TBeforeWorkbookClose)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb, Word &Cancel);

typedef void __fastcall (__closure *TBeforeWorkbookSave)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb, Word SaveAsUI, Word &Cancel);

typedef void __fastcall (__closure *TBeforeWorkbookPrint)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb, Word &Cancel);

typedef void __fastcall (__closure *TOnWorkbookNewSheet)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb, const _di_IDispatch Sh);

typedef void __fastcall (__closure *TOnWorkbookAddinInstall)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb);

typedef void __fastcall (__closure *TOnWorkbookAddinUninstall)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb);

typedef void __fastcall (__closure *TOnWindowResize)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb, const _di_ExcelWindow Wn);

typedef void __fastcall (__closure *TOnWindowActivate)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb, const _di_ExcelWindow Wn);

typedef void __fastcall (__closure *TOnWindowDeactivate)(System::TObject* Sender, const Opxlxp::_di__Workbook Wb, const _di_ExcelWindow Wn);

class DELPHICLASS TOpExcelWorkbook;
class DELPHICLASS TOpExcelWorksheets;
class DELPHICLASS TOpExcelWorksheet;
class PASCALIMPLEMENTATION TOpExcelWorksheets : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpExcelWorksheet* operator[](int index) { return Items[index]; }
	
private:
	HIDESBASE TOpExcelWorksheet* __fastcall GetItem(int index);
	HIDESBASE void __fastcall SetItem(int index, const TOpExcelWorksheet* Value);
	
protected:
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpExcelWorksheet* __fastcall Add(void);
	__property TOpExcelWorksheet* Items[int index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpExcelWorksheets(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpExcelWorksheets(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TOpExcelWorkbook : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
private:
	AnsiString FFilename;
	TOpExcelWorksheets* FWorksheets;
	void __fastcall SetFilename(const AnsiString Value);
	bool __fastcall SaveCollection(void);
	Opxlxp::_di__Workbook __fastcall GetAsWorkbook();
	Opshared::TOpPropDirection __fastcall GetPropDirection(void);
	
protected:
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	virtual void __fastcall CreateSubCollections(void);
	void __fastcall LoadWorkbook(void);
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	
public:
	__fastcall virtual ~TOpExcelWorkbook(void);
	virtual void __fastcall Connect(void);
	void __fastcall Print(void);
	virtual void __fastcall Activate(void);
	virtual void __fastcall ExecuteVerb(int index);
	void __fastcall Save(void);
	void __fastcall SaveAs(AnsiString FileName);
	__fastcall virtual TOpExcelWorkbook(Classes::TCollection* Collection);
	__property Opshared::TOpPropDirection PropDirection = {read=GetPropDirection, nodefault};
	__property Opxlxp::_di__Workbook AsWorkbook = {read=GetAsWorkbook};
	
__published:
	__property AnsiString Filename = {read=FFilename, write=SetFilename};
	__property TOpExcelWorksheets* Worksheets = {read=FWorksheets, write=FWorksheets, stored=SaveCollection};
};


class DELPHICLASS TOpExcelWorkbooks;
class PASCALIMPLEMENTATION TOpExcelWorkbooks : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpExcelWorkbook* operator[](int index) { return Items[index]; }
	
protected:
	HIDESBASE TOpExcelWorkbook* __fastcall GetItem(int index);
	HIDESBASE void __fastcall SetItem(int index, const TOpExcelWorkbook* Value);
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpExcelWorkbook* __fastcall Add(void);
	__property TOpExcelWorkbook* Items[int index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpExcelWorkbooks(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpExcelWorkbooks(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpExcelRanges;
class DELPHICLASS TOpExcelRange;
class PASCALIMPLEMENTATION TOpExcelRanges : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpExcelRange* operator[](int index) { return Items[index]; }
	
private:
	HIDESBASE TOpExcelRange* __fastcall GetItem(int index);
	HIDESBASE void __fastcall SetItem(int index, const TOpExcelRange* Value);
	
protected:
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpExcelRange* __fastcall Add(void);
	__property TOpExcelRange* Items[int index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpExcelRanges(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpExcelRanges(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpExcelHyperlinks;
class DELPHICLASS TOpExcelHyperlink;
class PASCALIMPLEMENTATION TOpExcelHyperlinks : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpExcelHyperlink* operator[](int index) { return Items[index]; }
	
private:
	HIDESBASE TOpExcelHyperlink* __fastcall GetItem(int index);
	HIDESBASE void __fastcall SetItem(int index, const TOpExcelHyperlink* Value);
	
protected:
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpExcelHyperlink* __fastcall Add(void);
	__property TOpExcelHyperlink* Items[int index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpExcelHyperlinks(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpExcelHyperlinks(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpExcelCharts;
class DELPHICLASS TOpExcelChart;
class PASCALIMPLEMENTATION TOpExcelCharts : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpExcelChart* operator[](int index) { return Items[index]; }
	
private:
	HIDESBASE TOpExcelChart* __fastcall GetItem(int index);
	HIDESBASE void __fastcall SetItem(int index, const TOpExcelChart* Value);
	
protected:
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpExcelChart* __fastcall Add(void);
	__property TOpExcelChart* Items[int index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpExcelCharts(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpExcelCharts(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TOpExcelWorksheet : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
private:
	WideString FName;
	TOpExcelRanges* FRanges;
	TOpExcelHyperlinks* FHyperlinks;
	TOpExcelCharts* FCharts;
	Opxlxp::_di__Worksheet __fastcall GetAsWorksheet();
	WideString __fastcall GetName();
	void __fastcall SetName(const WideString Value);
	
protected:
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	virtual void __fastcall CreateSubCollections(void);
	
public:
	virtual void __fastcall Activate(void);
	void __fastcall Print(void);
	virtual void __fastcall Connect(void);
	__property Opxlxp::_di__Worksheet AsWorksheet = {read=GetAsWorksheet};
	
__published:
	__fastcall virtual ~TOpExcelWorksheet(void);
	__property WideString Name = {read=GetName, write=SetName};
	__property TOpExcelRanges* Ranges = {read=FRanges, write=FRanges};
	__property TOpExcelHyperlinks* Hyperlinks = {read=FHyperlinks, write=FHyperlinks};
	__property TOpExcelCharts* Charts = {read=FCharts, write=FCharts};
public:
	#pragma option push -w-inl
	/* TOpNestedCollectionItem.Create */ inline __fastcall virtual TOpExcelWorksheet(Classes::TCollection* Collection) : Opshared::TOpNestedCollectionItem(Collection) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TOpExcelRange : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
private:
	AnsiString FFontName;
	Graphics::TColor FFontColor;
	int FFontSize;
	TOpXlRangeFontAttributes FFontAttributes;
	int FIndentLevel;
	TOpXlCellOrientation FOrientation;
	bool FShrinkToFit;
	int FRotateDegrees;
	TOpXlCellHorizAlign FHorizontalAlignment;
	TOpXlCellVertAlign FVerticalAlignment;
	int FColumnWidth;
	int FRowHeight;
	Graphics::TColor FColor;
	TOpXlInteriorPatterns FPattern;
	Graphics::TColor FPatternColor;
	TOpXlBorderLineStyles FBorderLineStyle;
	TOpXlBorderWeights FBorderLineThickness;
	TOpXlRangeBorders FBorders;
	bool FWrapText;
	AnsiString FName;
	Opmodels::TOpUnknownComponent* FModel;
	bool FClearOnMove;
	AnsiString FSimpleText;
	AnsiString FAddress;
	bool __fastcall ShouldPersistValue(void);
	void __fastcall SetModel(Opmodels::TOpUnknownComponent* Value);
	Opxlxp::_di_Range __fastcall GetAsRange();
	AnsiString __fastcall GetAddress();
	AnsiString __fastcall GetAnchorCell();
	void __fastcall SetAddress(const AnsiString Value);
	AnsiString __fastcall GetSimpleText();
	void __fastcall SetSimpleText(const AnsiString Value);
	Variant __fastcall GetValue();
	void __fastcall SetValue(const Variant &Value);
	bool __fastcall GetIsEmpty(void);
	void __fastcall SetFontName(AnsiString val);
	void __fastcall SetFontColor(Graphics::TColor val);
	void __fastcall SetFontSize(int val);
	void __fastcall SetFontAttributes(TOpXlRangeFontAttributes val);
	void __fastcall SetIndentLevel(int val);
	void __fastcall SetOrientation(TOpXlCellOrientation val);
	void __fastcall SetShrinkToFit(bool val);
	void __fastcall SetRotateDegrees(int val);
	void __fastcall SetHorizontalAlignment(TOpXlCellHorizAlign val);
	void __fastcall SetVerticalAlignment(TOpXlCellVertAlign val);
	void __fastcall SetColumnWidth(int val);
	void __fastcall SetRowHeight(int val);
	void __fastcall SetColor(Graphics::TColor val);
	void __fastcall SetPattern(TOpXlInteriorPatterns val);
	void __fastcall SetPatternColor(Graphics::TColor val);
	void __fastcall SetBorderLineStyle(TOpXlBorderLineStyles val);
	void __fastcall SetBorders(TOpXlRangeBorders val);
	void __fastcall SetBorderLineThickness(TOpXlBorderWeights val);
	void __fastcall SetWrapText(bool val);
	
protected:
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	
public:
	void __fastcall Clear(void);
	void __fastcall AutoFitColumns(void);
	void __fastcall AutoFitRows(void);
	__fastcall virtual TOpExcelRange(Classes::TCollection* Collection);
	virtual void __fastcall Connect(void);
	virtual void __fastcall ExecuteVerb(int index);
	void __fastcall Populate(void);
	void __fastcall PopulateCurrent(void);
	void __fastcall PopulateHeaders(void);
	void __fastcall Select(void);
	virtual void __fastcall Activate(void);
	void __fastcall ClearRange(void);
	void __fastcall SetAddressFromSelection(void);
	__property Opxlxp::_di_Range AsRange = {read=GetAsRange};
	__property AnsiString AnchorCell = {read=GetAnchorCell};
	__property Variant Value = {read=GetValue, write=SetValue};
	__property bool IsEmpty = {read=GetIsEmpty, nodefault};
	
__published:
	__property Opmodels::TOpUnknownComponent* OfficeModel = {read=FModel, write=SetModel};
	__property AnsiString Address = {read=GetAddress, write=SetAddress};
	__property bool ClearOnMove = {read=FClearOnMove, write=FClearOnMove, nodefault};
	__property AnsiString Name = {read=FName, write=FName};
	__property AnsiString SimpleText = {read=GetSimpleText, write=SetSimpleText, stored=ShouldPersistValue};
	__property AnsiString FontName = {read=FFontName, write=SetFontName};
	__property Graphics::TColor FontColor = {read=FFontColor, write=SetFontColor, nodefault};
	__property int FontSize = {read=FFontSize, write=SetFontSize, nodefault};
	__property TOpXlRangeFontAttributes FontAttributes = {read=FFontAttributes, write=SetFontAttributes, nodefault};
	__property int IndentLevel = {read=FIndentLevel, write=SetIndentLevel, nodefault};
	__property TOpXlCellOrientation Orientation = {read=FOrientation, write=SetOrientation, nodefault};
	__property bool ShrinkToFit = {read=FShrinkToFit, write=SetShrinkToFit, nodefault};
	__property int RotateDegrees = {read=FRotateDegrees, write=SetRotateDegrees, nodefault};
	__property TOpXlCellHorizAlign HorizontalAlignment = {read=FHorizontalAlignment, write=SetHorizontalAlignment, nodefault};
	__property TOpXlCellVertAlign VerticalAlignment = {read=FVerticalAlignment, write=SetVerticalAlignment, nodefault};
	__property int ColumnWidth = {read=FColumnWidth, write=SetColumnWidth, nodefault};
	__property int RowHeight = {read=FRowHeight, write=SetRowHeight, nodefault};
	__property Graphics::TColor Color = {read=FColor, write=SetColor, nodefault};
	__property TOpXlInteriorPatterns Pattern = {read=FPattern, write=SetPattern, nodefault};
	__property Graphics::TColor PatternColor = {read=FPatternColor, write=SetPatternColor, nodefault};
	__property TOpXlBorderLineStyles BorderStyle = {read=FBorderLineStyle, write=SetBorderLineStyle, nodefault};
	__property TOpXlRangeBorders Borders = {read=FBorders, write=SetBorders, nodefault};
	__property TOpXlBorderWeights BorderLineWeight = {read=FBorderLineThickness, write=SetBorderLineThickness, nodefault};
	__property bool WrapText = {read=FWrapText, write=SetWrapText, nodefault};
public:
	#pragma option push -w-inl
	/* TCollectionItem.Destroy */ inline __fastcall virtual ~TOpExcelRange(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TOpExcelHyperlink : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
private:
	AnsiString FAddress;
	AnsiString FSubAddress;
	AnsiString FAnchorCell;
	bool FNewWindow;
	bool FVisible;
	void __fastcall SetAnchorCell(const AnsiString Value);
	void __fastcall SetVisible(const bool Value);
	void __fastcall SetAddress(const AnsiString Value);
	Opxlxp::_di_Hyperlink __fastcall GetAsHyperlink();
	AnsiString __fastcall GetAddress();
	AnsiString __fastcall GetAnchorCell();
	bool __fastcall GetNewWindow(void);
	AnsiString __fastcall GetSubAddress();
	bool __fastcall GetVisible(void);
	void __fastcall SetNewWindow(const bool Value);
	void __fastcall SetSubAddress(const AnsiString Value);
	void __fastcall RecreateLink(void);
	
protected:
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	
public:
	virtual void __fastcall ExecuteVerb(int index);
	__fastcall virtual ~TOpExcelHyperlink(void);
	virtual void __fastcall Connect(void);
	void __fastcall Follow(void);
	__property Opxlxp::_di_Hyperlink AsHyperlink = {read=GetAsHyperlink};
	
__published:
	__property AnsiString AnchorCell = {read=GetAnchorCell, write=SetAnchorCell};
	__property AnsiString Address = {read=GetAddress, write=SetAddress};
	__property AnsiString SubAddress = {read=GetSubAddress, write=SetSubAddress};
	__property bool NewWindow = {read=GetNewWindow, write=SetNewWindow, nodefault};
	__property bool Visible = {read=GetVisible, write=SetVisible, nodefault};
public:
	#pragma option push -w-inl
	/* TOpNestedCollectionItem.Create */ inline __fastcall virtual TOpExcelHyperlink(Classes::TCollection* Collection) : Opshared::TOpNestedCollectionItem(Collection) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TOpExcelChart : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
private:
	int FLeft;
	int FTop;
	int FWidth;
	int FHeight;
	AnsiString FRange;
	TOpXlChartType FChartType;
	TOpXlChartType __fastcall GetChartType(void);
	void __fastcall SetChartType(const TOpXlChartType Value);
	int __fastcall GetHeight(void);
	int __fastcall GetLeft(void);
	int __fastcall GetTop(void);
	int __fastcall GetWidth(void);
	void __fastcall SetHeight(const int Value);
	void __fastcall SetLeft(const int Value);
	void __fastcall SetTop(const int Value);
	void __fastcall SetWidth(const int Value);
	Opxlxp::_di__Chart __fastcall GetAsChart();
	Opxlxp::_di_ChartObject __fastcall GetAsChartObject();
	
protected:
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	AnsiString __fastcall GetRange();
	void __fastcall SetRange(const AnsiString Value);
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	
public:
	__fastcall virtual ~TOpExcelChart(void);
	virtual void __fastcall Connect(void);
	virtual void __fastcall Activate(void);
	virtual void __fastcall ExecuteVerb(int index);
	void __fastcall SetAddressFromSelection(void);
	__property Opxlxp::_di__Chart AsChart = {read=GetAsChart};
	__property Opxlxp::_di_ChartObject AsChartObject = {read=GetAsChartObject};
	
__published:
	__property int Top = {read=GetTop, write=SetTop, nodefault};
	__property int Left = {read=GetLeft, write=SetLeft, nodefault};
	__property int Width = {read=GetWidth, write=SetWidth, nodefault};
	__property int Height = {read=GetHeight, write=SetHeight, nodefault};
	__property AnsiString DataRange = {read=GetRange, write=SetRange};
	__property TOpXlChartType ChartType = {read=GetChartType, write=SetChartType, nodefault};
public:
	#pragma option push -w-inl
	/* TOpNestedCollectionItem.Create */ inline __fastcall virtual TOpExcelChart(Classes::TCollection* Collection) : Opshared::TOpNestedCollectionItem(Collection) { }
	#pragma option pop
	
};


class DELPHICLASS TOpExcel;
class PASCALIMPLEMENTATION TOpExcel : public Opxlxp::TOpExcelBase 
{
	typedef Opxlxp::TOpExcelBase inherited;
	
private:
	Opxlxp::_di__Application FServer;
	bool FInteractive;
	TOpXlWindowState FWindowState;
	AnsiString FCaption;
	bool FVisible;
	int FHeight;
	int FTop;
	int FWidth;
	int FLeft;
	Word FEnableAutoComplete;
	Word FEnableAnimations;
	TOpXlCancelKey FEnableCancelKey;
	Word FLargeButtons;
	WideString FUserName;
	TOpExcelWorkbooks* FWorkbooks;
	TOnNewWorkBook FOnNewWorkbook;
	TOnSheetActivate FOnSheetActivate;
	TBeforeSheetDoubleClick FBeforeSheetDoubleClick;
	TBeforeSheetRightClick FBeforeSheetRightClick;
	TOnSheetCalculate FOnSheetCalculate;
	TOnSheetChange FOnSheetChange;
	TOnSheetDeactivate FOnSheetDeactivate;
	TOnSheetSelectionChange FOnSheetSelectionChange;
	TOnWindowActivate FOnWindowActivate;
	TOnWindowDeactivate FOnWindowDeactivate;
	TOnWindowResize FOnWindowResize;
	TOnWorkbookActivate FOnWorkbookActivate;
	TOnWorkbookAddinInstall FOnWorkbookAddinInstall;
	TOnWorkbookAddinUninstall FOnWorkbookAddinUninstall;
	TBeforeWorkbookClose FBeforeWorkbookClose;
	TBeforeWorkbookPrint FBeforeWorkbookPrint;
	TBeforeWorkbookSave FBeforeWorkbookSave;
	TOnWorkbookDeactivate FOnWorkbookDeactivate;
	TOnWorkbookNewSheet FOnWorkbookNewSheet;
	TOnWorkbookOpen FOnWorkbookOpen;
	void __fastcall SetVisible(bool Value);
	bool __fastcall GetVisible(void);
	void __fastcall SetWindowState(TOpXlWindowState Value);
	TOpXlWindowState __fastcall GetWindowState(void);
	void __fastcall SetCaption(const AnsiString Value);
	void __fastcall SetHeight(int Value);
	void __fastcall SetLeft(int Value);
	void __fastcall SetTop(int Value);
	void __fastcall SetWidth(int Value);
	int __fastcall GetHeight(void);
	int __fastcall GetLeft(void);
	int __fastcall GetTop(void);
	int __fastcall GetWidth(void);
	bool __fastcall GetInteractive(void);
	void __fastcall SetInteractive(const bool Value);
	void __fastcall FixupCollection(Opshared::TOpNestedCollectionItem* Item);
	void __fastcall ReleaseCollectionInterface(Opshared::TOpNestedCollectionItem* Item);
	bool __fastcall CollectionNotify(void *Key, Opshared::TOpNestedCollectionItem* Item);
	bool __fastcall FindRange(void *Key, Opshared::TOpNestedCollectionItem* Item);
	bool __fastcall DeleteWorkbook(void *Key, Opshared::TOpNestedCollectionItem* Item);
	TOpExcelRange* __fastcall GetRangeByName(AnsiString Name);
	void __fastcall SetEnableAnimations(const Word Value);
	void __fastcall SetEnableAutoComplete(const Word Value);
	void __fastcall SetEnableCancelKey(TOpXlCancelKey Value);
	void __fastcall SetLargeButtons(const Word Value);
	void __fastcall SetUserName(const WideString Value);
	Word __fastcall GetEnableAnimations(void);
	Word __fastcall GetEnableAutoComplete(void);
	TOpXlCancelKey __fastcall GetEnableCancelKey(void);
	Word __fastcall GetLargeButtons(void);
	WideString __fastcall GetUserName();
	AnsiString __fastcall GetCaption();
	int __fastcall PointsToPixels(double Points);
	double __fastcall PixelsToPoints(int Pixels);
	int __fastcall PixelsPerInch(void);
	
protected:
	virtual void __fastcall DoConnect(void);
	virtual void __fastcall DoDisconnect(void);
	DYNAMIC void __fastcall DoNewWorkbook(const Opxlxp::_di__Workbook WorkBook);
	DYNAMIC void __fastcall DoSheetSelectionChange(const _di_IDispatch Sh, const Opxlxp::_di_Range Target);
	DYNAMIC void __fastcall DoBeforeSheetDoubleClick(const _di_IDispatch Sh, const Opxlxp::_di_Range Target, Word &Cancel);
	DYNAMIC void __fastcall DoBeforeSheetRightClick(const _di_IDispatch Sh, const Opxlxp::_di_Range Target, Word &Cancel);
	DYNAMIC void __fastcall DoSheetActivate(const _di_IDispatch Sh);
	DYNAMIC void __fastcall DoSheetDeActivate(const _di_IDispatch Sh);
	DYNAMIC void __fastcall DoSheetCalculate(const _di_IDispatch Sh);
	DYNAMIC void __fastcall DoSheetChange(const _di_IDispatch Sh, const Opxlxp::_di_Range Target);
	DYNAMIC void __fastcall DoWorkbookOpen(const Opxlxp::_di__Workbook Wb);
	DYNAMIC void __fastcall DoWorkbookActivate(const Opxlxp::_di__Workbook Wb);
	DYNAMIC void __fastcall DoWorkbookDeactivate(const Opxlxp::_di__Workbook Wb);
	DYNAMIC void __fastcall DoBeforeWorkbookClose(const Opxlxp::_di__Workbook Wb, Word &Cancel);
	DYNAMIC void __fastcall DoBeforeWorkbookSave(const Opxlxp::_di__Workbook Wb, Word SaveAsUI, Word &Cancel);
	DYNAMIC void __fastcall DoBeforeWorkbookPrint(const Opxlxp::_di__Workbook Wb, Word &Cancel);
	DYNAMIC void __fastcall DoWorkbookNewSheet(const Opxlxp::_di__Workbook Wb, const _di_IDispatch Sh);
	DYNAMIC void __fastcall DoWorkbookAddinInstall(const Opxlxp::_di__Workbook Wb);
	DYNAMIC void __fastcall DoWorkbookAddinUninstall(const Opxlxp::_di__Workbook Wb);
	DYNAMIC void __fastcall DoWindowResize(const Opxlxp::_di__Workbook Wb, const _di_ExcelWindow Wn);
	DYNAMIC void __fastcall DoWindowActivate(const Opxlxp::_di__Workbook Wb, const _di_ExcelWindow Wn);
	DYNAMIC void __fastcall DoWindowDeactivate(const Opxlxp::_di__Workbook Wb, const _di_ExcelWindow Wn);
	virtual void __fastcall FixupProps(void);
	virtual bool __fastcall GetConnected(void);
	virtual AnsiString __fastcall GetOfficeVersionStr();
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	
public:
	__fastcall virtual TOpExcel(Classes::TComponent* AOwner);
	__fastcall virtual ~TOpExcel(void);
	virtual void __fastcall GetAppInfo(Classes::TStrings* Info);
	virtual void __fastcall GetFileInfo(AnsiString &Filter, AnsiString &DefExt);
	virtual void __fastcall HandleEvent(const GUID &IID, int DispId, const tagVARIANT * Params);
	virtual _di_IDispatch __fastcall CreateItem(Opshared::TOpNestedCollectionItem* Item);
	__property Opxlxp::_di__Application Server = {read=FServer};
	__property TOpExcelRange* RangeByName[AnsiString Name] = {read=GetRangeByName};
	
__published:
	__property AnsiString Caption = {read=GetCaption, write=SetCaption};
	__property Connected  = {default=0};
	__property bool Interactive = {read=GetInteractive, write=SetInteractive, default=1};
	__property MachineName ;
	__property PropDirection  = {default=0};
	__property bool Visible = {read=GetVisible, write=SetVisible, nodefault};
	__property TOpXlWindowState WindowState = {read=GetWindowState, write=SetWindowState, nodefault};
	__property int ServerLeft = {read=GetLeft, write=SetLeft, nodefault};
	__property int ServerTop = {read=GetTop, write=SetTop, nodefault};
	__property int ServerHeight = {read=GetHeight, write=SetHeight, nodefault};
	__property int ServerWidth = {read=GetWidth, write=SetWidth, nodefault};
	__property Word LargeButtons = {read=GetLargeButtons, write=SetLargeButtons, nodefault};
	__property Word EnableAnimations = {read=GetEnableAnimations, write=SetEnableAnimations, nodefault};
	__property WideString UserName = {read=GetUserName, write=SetUserName};
	__property Word EnableAutoComplete = {read=GetEnableAutoComplete, write=SetEnableAutoComplete, nodefault};
	__property TOpXlCancelKey EnableCancelKey = {read=GetEnableCancelKey, write=SetEnableCancelKey, nodefault};
	__property TOpExcelWorkbooks* Workbooks = {read=FWorkbooks, write=FWorkbooks};
	__property OnOpConnect ;
	__property OnOpDisconnect ;
	__property OnGetInstance ;
	__property TOnNewWorkBook OnNewWorkbook = {read=FOnNewWorkbook, write=FOnNewWorkbook};
	__property TOnSheetSelectionChange OnSheetSelectionChange = {read=FOnSheetSelectionChange, write=FOnSheetSelectionChange};
	__property TBeforeSheetDoubleClick BeforeSheetDoubleClick = {read=FBeforeSheetDoubleClick, write=FBeforeSheetDoubleClick};
	__property TBeforeSheetRightClick BeforeSheetRightClick = {read=FBeforeSheetRightClick, write=FBeforeSheetRightClick};
	__property TOnSheetActivate OnSheetActivate = {read=FOnSheetActivate, write=FOnSheetActivate};
	__property TOnSheetDeactivate OnSheetDeactivate = {read=FOnSheetDeactivate, write=FOnSheetDeactivate};
	__property TOnSheetCalculate OnSheetCalculate = {read=FOnSheetCalculate, write=FOnSheetCalculate};
	__property TOnSheetChange OnSheetChange = {read=FOnSheetChange, write=FOnSheetChange};
	__property TOnWorkbookOpen OnWorkbookOpen = {read=FOnWorkbookOpen, write=FOnWorkbookOpen};
	__property TOnWorkbookActivate OnWorkbookActivate = {read=FOnWorkbookActivate, write=FOnWorkbookActivate};
	__property TOnWorkbookDeactivate OnWorkbookDeactivate = {read=FOnWorkbookDeactivate, write=FOnWorkbookDeactivate};
	__property TBeforeWorkbookClose BeforeWorkbookClose = {read=FBeforeWorkbookClose, write=FBeforeWorkbookClose};
	__property TBeforeWorkbookSave BeforeWorkbookSave = {read=FBeforeWorkbookSave, write=FBeforeWorkbookSave};
	__property TBeforeWorkbookPrint BeforeWorkbookPrint = {read=FBeforeWorkbookPrint, write=FBeforeWorkbookPrint};
	__property TOnWorkbookNewSheet OnWorkbookNewSheet = {read=FOnWorkbookNewSheet, write=FOnWorkbookNewSheet};
	__property TOnWorkbookAddinInstall OnWorkbookAddinInstall = {read=FOnWorkbookAddinInstall, write=FOnWorkbookAddinInstall};
	__property TOnWorkbookAddinUninstall OnWorkbookAddinUninstall = {read=FOnWorkbookAddinUninstall, write=FOnWorkbookAddinUninstall};
	__property TOnWindowResize OnWindowResize = {read=FOnWindowResize, write=FOnWindowResize};
	__property TOnWindowActivate OnWindowActivate = {read=FOnWindowActivate, write=FOnWindowActivate};
	__property TOnWindowDeactivate OnWindowDeactivate = {read=FOnWindowDeactivate, write=FOnWindowDeactivate};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Opexcel */
using namespace Opexcel;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpExcel
