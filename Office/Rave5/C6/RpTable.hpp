// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpTable.pas' rev: 6.00

#ifndef RpTableHPP
#define RpTableHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpMemo.hpp>	// Pascal unit
#include <RpSect.hpp>	// Pascal unit
#include <RpShell.hpp>	// Pascal unit
#include <RpFiler.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rptable
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TSectionType { stReportHeader, stPageHeader, stBodyHeader, stGroupHeader, stGroupFooter, stBodyFooter, stPageFooter, stReportFooter, stNone };
#pragma option pop

#pragma option push -b-
enum TOverflowMethod { omTruncateWord, omTruncateChar, omReplace, omWrap, omShrink, omUser };
#pragma option pop

#pragma option push -b-
enum TOutputType { otText, otHeadings, otPageTotals, otSubTotals, otGrandTotals };
#pragma option pop

typedef void __fastcall (__closure *TFormatEvent)(System::TObject* Sender, AnsiString &Text);

typedef int __fastcall (__closure *TIntegerEvent)(System::TObject* Sender);

class DELPHICLASS TTableColumn;
typedef void __fastcall (__closure *TTableColumnEvent)(TTableColumn* TableColumn);

typedef void __fastcall (__closure *TTableColumnHeightEvent)(TTableColumn* TableColumn, double &Height, double &MinHeight);

class DELPHICLASS TTableSection;
typedef void __fastcall (__closure *TTableSectionEvent)(TTableSection* TableSection);

class DELPHICLASS TTablePrinter;
typedef void __fastcall (__closure *TTablePrinterEvent)(TTablePrinter* TablePrinter, bool &Valid);

struct TRowHeader
{
	double Height;
	double MinHeight;
	Word Columns;
	Word Level;
} ;

struct TColHeader
{
	int DataSize;
	double FontSize;
	double Height;
	double MinHeight;
	short StartLine;
	Word Lines;
} ;

struct TColText
{
	Rpdefine::TPrintJustify Justify;
	double Height;
	int Len;
} ;

class DELPHICLASS TTableItem;
class PASCALIMPLEMENTATION TTableItem : public Rpbase::TRPBaseComponent 
{
	typedef Rpbase::TRPBaseComponent inherited;
	
protected:
	TTablePrinter* FParent;
	Classes::TStream* SaveColStream;
	TColHeader SaveColHeader;
	double SaveHeight;
	double FWidth;
	Graphics::TColor FShadeColor;
	Byte FShadePercent;
	double FMargin;
	TOverflowMethod FOverflowMethod;
	AnsiString FOverflowAppend;
	AnsiString FOverflowReplace;
	Byte FMinFontSize;
	Rpdefine::TPrintJustify FJustify;
	Rpdefine::TPrintJustifyVert FJustifyVert;
	Rpdefine::TBoxLines FBoxLines;
	Graphics::TFont* FFont;
	Graphics::TPen* FPen;
	bool FUseParentFont;
	bool FUseParentPen;
	AnsiString FDescription;
	virtual void __fastcall ReadState(Classes::TReader* Reader);
	virtual void __fastcall SetParent(TTablePrinter* Value) = 0 ;
	virtual void __fastcall Loaded(void);
	void __fastcall ParentFontChanged(void);
	void __fastcall SetDescription(AnsiString Value);
	void __fastcall SetFont(Graphics::TFont* Value);
	void __fastcall SetPen(Graphics::TPen* Value);
	void __fastcall SetWidth(double Value);
	double __fastcall GetWidth(void);
	void __fastcall SetShadePercent(Byte Value);
	void __fastcall SetMargin(double Value);
	double __fastcall GetMargin(void);
	void __fastcall SetMargin100(double Value);
	double __fastcall GetMargin100(void);
	void __fastcall SetOverflowMethod(TOverflowMethod Value);
	TOverflowMethod __fastcall GetOverflowMethod(void);
	Rpbase::TBaseReport* __fastcall GetReportPrinter(void);
	bool __fastcall IsFontStored(void);
	bool __fastcall IsPenStored(void);
	
public:
	__fastcall virtual TTableItem(Classes::TComponent* AOwner);
	__fastcall virtual ~TTableItem(void);
	void __fastcall PrintBox(void);
	DYNAMIC bool __fastcall HasParent(void);
	__property TTablePrinter* TablePrinter = {read=FParent};
	__property Rpbase::TBaseReport* ReportPrinter = {read=GetReportPrinter};
	
__published:
	__property AnsiString Description = {read=FDescription, write=SetDescription};
	__property double Width = {read=GetWidth, write=SetWidth};
	__property Graphics::TColor ShadeColor = {read=FShadeColor, write=FShadeColor, default=0};
	__property Byte ShadePercent = {read=FShadePercent, write=SetShadePercent, default=0};
	__property double Margin = {read=GetMargin, write=SetMargin};
	__property double Margin100 = {read=GetMargin100, write=SetMargin100};
	__property TOverflowMethod OverflowMethod = {read=GetOverflowMethod, write=SetOverflowMethod, default=1};
	__property AnsiString OverflowReplace = {read=FOverflowReplace, write=FOverflowReplace};
	__property AnsiString OverflowAppend = {read=FOverflowAppend, write=FOverflowAppend};
	__property Byte MinFontSize = {read=FMinFontSize, write=FMinFontSize, default=6};
	__property Graphics::TFont* Font = {read=FFont, write=SetFont, stored=IsFontStored};
	__property bool UseParentFont = {read=FUseParentFont, write=FUseParentFont, default=1};
	__property Graphics::TPen* Pen = {read=FPen, write=SetPen, stored=IsPenStored};
	__property bool UseParentPen = {read=FUseParentPen, write=FUseParentPen, default=1};
	__property Rpdefine::TPrintJustify Justify = {read=FJustify, write=FJustify, default=1};
	__property Rpdefine::TPrintJustifyVert JustifyVert = {read=FJustifyVert, write=FJustifyVert, default=0};
	__property Rpdefine::TBoxLines BoxLines = {read=FBoxLines, write=FBoxLines, default=15};
};


class PASCALIMPLEMENTATION TTableColumn : public TTableItem 
{
	typedef TTableItem inherited;
	
protected:
	bool IsDefault;
	Word FIndex;
	TOutputType FOutputType;
	TTableItem* FTableItem;
	AnsiString FHeading;
	Rpmemo::TMemoBuf* FMemoBuf;
	double FSubTotal;
	double FPageTotal;
	double FGrandTotal;
	bool FTotals;
	Rpsect::TReportSection* FSection;
	bool FSplitRow;
	AnsiString FDisplayFormat;
	TTableColumnEvent FOnHeaderBefore;
	TTableColumnEvent FOnHeaderSetup;
	TTableColumnHeightEvent FOnHeaderHeight;
	TTableColumnHeightEvent FOnHeaderPrint;
	TTableColumnEvent FOnHeaderAfter;
	TTableColumnEvent FOnRowBefore;
	TTableColumnEvent FOnRowSetup;
	TTableColumnHeightEvent FOnRowHeight;
	TTableColumnHeightEvent FOnRowPrint;
	TTableColumnEvent FOnRowAfter;
	TTableColumnEvent FOnFooterBefore;
	TTableColumnEvent FOnFooterSetup;
	TTableColumnHeightEvent FOnFooterHeight;
	TTableColumnHeightEvent FOnFooterPrint;
	TTableColumnEvent FOnFooterAfter;
	TTableColumnEvent FOnAddTotal;
	TTableColumnEvent FOnInitPage;
	TTableColumnEvent FOnInitMaster;
	TTableColumnEvent FOnOverflow;
	virtual void __fastcall HeaderBeforeEvent(void);
	virtual void __fastcall HeaderSetupEvent(void);
	virtual void __fastcall HeaderHeightEvent(double &Height, double &MinHeight);
	virtual void __fastcall HeaderPrintEvent(double &Height, double &MinHeight);
	virtual void __fastcall HeaderAfterEvent(void);
	virtual void __fastcall RowBeforeEvent(void);
	virtual void __fastcall RowSetupEvent(void);
	virtual void __fastcall RowHeightEvent(double &Height, double &MinHeight);
	virtual void __fastcall RowPrintEvent(double &Height, double &MinHeight);
	virtual void __fastcall RowAfterEvent(void);
	virtual void __fastcall FooterBeforeEvent(void);
	virtual void __fastcall FooterSetupEvent(void);
	virtual void __fastcall FooterHeightEvent(double &Height, double &MinHeight);
	virtual void __fastcall FooterPrintEvent(double &Height, double &MinHeight);
	virtual void __fastcall FooterAfterEvent(void);
	virtual void __fastcall AddTotalEvent(void);
	virtual void __fastcall InitPageEvent(void);
	virtual void __fastcall InitMasterEvent(void);
	virtual void __fastcall OverflowEvent(void);
	virtual void __fastcall SetParent(TTablePrinter* Value);
	int __fastcall GetAsInteger(void);
	void __fastcall SetAsInteger(int Value);
	double __fastcall GetAsFloat(void);
	void __fastcall SetAsFloat(double Value);
	virtual void __fastcall FillMemoBuf(void);
	void __fastcall GetRow(Classes::TStream* ColStream, TColHeader &ColHeader);
	void __fastcall PrintRow(Classes::TStream* ColStream, TColHeader &ColHeader, double Height);
	void __fastcall AddTotal(void);
	void __fastcall InitPage(void);
	void __fastcall InitMaster(void);
	void __fastcall AddLine(Classes::TStream* ColStream, TColHeader &ColHeader, AnsiString S1, Rpdefine::TPrintJustify ThisJustify);
	void __fastcall ProcessOverflow(Classes::TStream* ColStream, TColHeader &ColHeader, double AvailWidth);
	
public:
	__fastcall virtual TTableColumn(Classes::TComponent* AOwner);
	__fastcall virtual ~TTableColumn(void);
	virtual void __fastcall Init(void);
	void __fastcall PrintDefault(void);
	__property TOutputType OutputType = {read=FOutputType, write=FOutputType, nodefault};
	__property TTableItem* TableItem = {read=FTableItem, write=FTableItem};
	__property Rpmemo::TMemoBuf* MemoBuf = {read=FMemoBuf};
	__property double SubTotal = {read=FSubTotal, write=FSubTotal};
	__property double PageTotal = {read=FPageTotal, write=FPageTotal};
	__property double GrandTotal = {read=FGrandTotal, write=FGrandTotal};
	__property int AsInteger = {read=GetAsInteger, write=SetAsInteger, nodefault};
	__property double AsFloat = {read=GetAsFloat, write=SetAsFloat};
	
__published:
	__property AnsiString Heading = {read=FHeading, write=FHeading};
	__property bool Totals = {read=FTotals, write=FTotals, default=0};
	__property bool SplitRow = {read=FSplitRow, write=FSplitRow, default=1};
	__property AnsiString DisplayFormat = {read=FDisplayFormat, write=FDisplayFormat};
	__property TTableColumnEvent OnHeaderBefore = {read=FOnHeaderBefore, write=FOnHeaderBefore};
	__property TTableColumnEvent OnHeaderSetup = {read=FOnHeaderSetup, write=FOnHeaderSetup};
	__property TTableColumnHeightEvent OnHeaderHeight = {read=FOnHeaderHeight, write=FOnHeaderHeight};
	__property TTableColumnHeightEvent OnHeaderPrint = {read=FOnHeaderPrint, write=FOnHeaderPrint};
	__property TTableColumnEvent OnHeaderAfter = {read=FOnHeaderAfter, write=FOnHeaderAfter};
	__property TTableColumnEvent OnRowBefore = {read=FOnRowBefore, write=FOnRowBefore};
	__property TTableColumnEvent OnRowSetup = {read=FOnRowSetup, write=FOnRowSetup};
	__property TTableColumnHeightEvent OnRowHeight = {read=FOnRowHeight, write=FOnRowHeight};
	__property TTableColumnHeightEvent OnRowPrint = {read=FOnRowPrint, write=FOnRowPrint};
	__property TTableColumnEvent OnRowAfter = {read=FOnRowAfter, write=FOnRowAfter};
	__property TTableColumnEvent OnFooterBefore = {read=FOnFooterBefore, write=FOnFooterBefore};
	__property TTableColumnEvent OnFooterSetup = {read=FOnFooterSetup, write=FOnFooterSetup};
	__property TTableColumnHeightEvent OnFooterHeight = {read=FOnFooterHeight, write=FOnFooterHeight};
	__property TTableColumnHeightEvent OnFooterPrint = {read=FOnFooterPrint, write=FOnFooterPrint};
	__property TTableColumnEvent OnFooterAfter = {read=FOnFooterAfter, write=FOnFooterAfter};
	__property TTableColumnEvent OnAddTotal = {read=FOnAddTotal, write=FOnAddTotal};
	__property TTableColumnEvent OnInitPage = {read=FOnInitPage, write=FOnInitPage};
	__property TTableColumnEvent OnInitMaster = {read=FOnInitMaster, write=FOnInitMaster};
	__property TTableColumnEvent OnOverflow = {read=FOnOverflow, write=FOnOverflow};
};


class PASCALIMPLEMENTATION TTableSection : public TTableItem 
{
	typedef TTableItem inherited;
	
protected:
	TSectionType FSectionType;
	Rpsect::TReportSection* FSection;
	AnsiString FText;
	TOutputType FOutputType;
	TTableSectionEvent FOnBefore;
	TTableSectionEvent FOnSetup;
	TTableSectionEvent FOnPrint;
	TTableSectionEvent FOnAfter;
	virtual void __fastcall BeforeEvent(void);
	virtual void __fastcall SetupEvent(void);
	virtual void __fastcall PrintEvent(void);
	virtual void __fastcall AfterEvent(void);
	virtual void __fastcall SetParent(TTablePrinter* Value);
	void __fastcall SetSectionType(TSectionType Value);
	void __fastcall SetSection(Rpsect::TReportSection* Value);
	
public:
	__fastcall virtual TTableSection(Classes::TComponent* AOwner);
	__fastcall virtual ~TTableSection(void);
	void __fastcall Print(void);
	void __fastcall PrintDefault(void);
	
__published:
	__property TSectionType SectionType = {read=FSectionType, write=SetSectionType, nodefault};
	__property Rpsect::TReportSection* Section = {read=FSection, write=SetSection};
	__property AnsiString Text = {read=FText, write=FText};
	__property TOutputType OutputType = {read=FOutputType, write=FOutputType, default=0};
	__property TTableSectionEvent OnBefore = {read=FOnBefore, write=FOnBefore};
	__property TTableSectionEvent OnSetup = {read=FOnSetup, write=FOnSetup};
	__property TTableSectionEvent OnPrint = {read=FOnPrint, write=FOnPrint};
	__property TTableSectionEvent OnAfter = {read=FOnAfter, write=FOnAfter};
};


class PASCALIMPLEMENTATION TTablePrinter : public Rpshell::TBaseShell 
{
	typedef Rpshell::TBaseShell inherited;
	
protected:
	Classes::TList* FTableColumns;
	bool DoHeadings;
	bool IsDefault;
	Byte DefaultProc;
	Rpshell::TBaseShell* FDetailTablePrinter;
	TTablePrinter* FMasterTablePrinter;
	double FStartOffset;
	double FStartPos;
	Graphics::TFont* FFont;
	Graphics::TPen* FPen;
	TTableSection* FTableSection[8];
	bool FReprintHeader;
	TTablePrinterEvent FOnInitTable;
	TTablePrinterEvent FOnInitMaster;
	TTablePrinterEvent FOnValidateRow;
	TTablePrinterEvent FOnGetNextRow;
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	void __fastcall SetDetailTablePrinter(Rpshell::TBaseShell* Value);
	virtual void __fastcall InitTableEvent(bool &Valid);
	virtual void __fastcall InitMasterEvent(bool &Valid);
	virtual void __fastcall ValidateRowEvent(bool &Valid);
	virtual void __fastcall GetNextRowEvent(bool &Valid);
	DYNAMIC void __fastcall GetChildren(Classes::TGetChildProc Proc, Classes::TComponent* Root);
	int __fastcall GetTableColumns(void);
	TTableColumn* __fastcall GetTableColumn(int Index);
	TTableSection* __fastcall GetTableSection(TSectionType Index);
	void __fastcall SetFont(Graphics::TFont* Value);
	void __fastcall SetPen(Graphics::TPen* Value);
	virtual void __fastcall SetName(const AnsiString Value);
	void __fastcall BeginPage(System::TObject* Sender);
	void __fastcall EndPage(System::TObject* Sender);
	void __fastcall ReprintHeaders(bool PrintingHeader);
	void __fastcall PrintSection(TTableSection* TableSection);
	virtual void __fastcall InitMaster(bool &Valid, TTablePrinter* Master);
	virtual void __fastcall InitTable(bool &Valid);
	virtual void __fastcall ValidateRow(bool &Valid);
	virtual void __fastcall GetNextRow(bool &Valid);
	void __fastcall PrintRow(Classes::TStream* ColStream, const TRowHeader &RowHeader, TOutputType RowOutputType, TTableItem* RowTableItem);
	void __fastcall ReadRow(Classes::TStream* ColStream, TRowHeader &RowHeader, TOutputType RowOutputType, TTableItem* RowTableItem);
	void __fastcall ReadBody(Classes::TStream* ColStream, Word OnLevel);
	
public:
	__fastcall virtual TTablePrinter(Classes::TComponent* AOwner);
	__fastcall virtual ~TTablePrinter(void);
	void __fastcall AssignParent(TTableItem* TableItem);
	virtual TTableColumn* __fastcall CreateColumn(AnsiString Description);
	virtual TTableSection* __fastcall CreateSection(AnsiString Description, TSectionType SectionType);
	virtual void __fastcall Execute(Rpbase::TBaseReport* Report);
	void __fastcall Default(bool &Valid);
	__property Classes::TList* TableColumnList = {read=FTableColumns};
	__property int TableColumns = {read=GetTableColumns, nodefault};
	__property TTableColumn* TableColumn[int Index] = {read=GetTableColumn};
	__property TTableSection* TableSection[TSectionType Index] = {read=GetTableSection};
	__property TTablePrinter* MasterTablePrinter = {read=FMasterTablePrinter, write=FMasterTablePrinter};
	
__published:
	__property Rpshell::TBaseShell* DetailTablePrinter = {read=FDetailTablePrinter, write=SetDetailTablePrinter};
	__property double StartPos = {read=FStartPos, write=FStartPos};
	__property Graphics::TFont* Font = {read=FFont, write=SetFont};
	__property Graphics::TPen* Pen = {read=FPen, write=SetPen};
	__property bool ReprintHeader = {read=FReprintHeader, write=FReprintHeader, default=1};
	__property TTablePrinterEvent OnInitTable = {read=FOnInitTable, write=FOnInitTable};
	__property TTablePrinterEvent OnInitMaster = {read=FOnInitMaster, write=FOnInitMaster};
	__property TTablePrinterEvent OnValidateRow = {read=FOnValidateRow, write=FOnValidateRow};
	__property TTablePrinterEvent OnGetNextRow = {read=FOnGetNextRow, write=FOnGetNextRow};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE System::SmallString<13>  SectionName[9];
extern PACKAGE AnsiString __fastcall DescToName(AnsiString Desc);

}	/* namespace Rptable */
using namespace Rptable;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpTable
