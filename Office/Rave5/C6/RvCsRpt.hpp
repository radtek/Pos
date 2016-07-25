// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvCsRpt.pas' rev: 6.00

#ifndef RvCsRptHPP
#define RvCsRptHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvDataField.hpp>	// Pascal unit
#include <RvProj.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <RvData.hpp>	// Pascal unit
#include <RvCsStd.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <RvUtil.hpp>	// Pascal unit
#include <RvDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Buttons.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvcsrpt
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TBandPositionMode { pmOffset, pmOverlay, pmAbsolute };
#pragma option pop

#pragma option push -b-
enum TBandPrintLoc { plBodyHeader, plGroupHeader, plRowHeader, plMaster, plDetail, plRowFooter, plGroupFooter, plBodyFooter };
#pragma option pop

typedef Set<TBandPrintLoc, plBodyHeader, plBodyFooter>  TBandPrintLocSet;

#pragma option push -b-
enum TBandPrintOcc { poFirst, poNewPage, poNewColumn };
#pragma option pop

typedef Set<TBandPrintOcc, poFirst, poNewColumn>  TBandPrintOccSet;

struct TBandDetail;
typedef TBandDetail *PBandDetail;

class DELPHICLASS TRaveBand;
class DELPHICLASS TRaveIterateBand;
class DELPHICLASS TRaveBandStyle;
class PASCALIMPLEMENTATION TRaveBandStyle : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	TBandPrintLocSet FPrintLoc;
	TBandPrintOccSet FPrintOcc;
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	
public:
	__fastcall TRaveBandStyle(void);
	
__published:
	__property TBandPrintLocSet PrintLoc = {read=FPrintLoc, write=FPrintLoc, default=0};
	__property TBandPrintOccSet PrintOcc = {read=FPrintOcc, write=FPrintOcc, default=1};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TRaveBandStyle(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveRegion;
class PASCALIMPLEMENTATION TRaveBand : public Rvclass::TRaveContainerControl 
{
	typedef Rvclass::TRaveContainerControl inherited;
	
protected:
	Rvdefine::TRaveUnits BandHeaderTop;
	Rvdefine::TRaveUnits BandHeaderBottom;
	AnsiString LastGroupValue;
	bool OnCurrPage;
	TRaveIterateBand* FControllerBand;
	TBandPositionMode FPositionMode;
	Rvdefine::TRaveUnits FPositionValue;
	TRaveBandStyle* FBandStyle;
	Rvdata::TRaveBaseDataView* FGroupDataView;
	AnsiString FGroupKey;
	TBandPrintLocSet FReprintLocs;
	bool FSaving;
	bool FStartNewPage;
	bool FFinishNewPage;
	bool FDesignerHide;
	bool FAllowSplit;
	Rvdefine::TRaveUnits FMinHeightLeft;
	bool FDoSplit;
	Rvclass::TRaveStringEvent* FOnGetGroup;
	/* virtual class method */ virtual bool __fastcall AcceptParent(TMetaClass* vmt, TMetaClass* NewParent);
	virtual void __fastcall SetParent(Rvclass::TRaveComponent* Value);
	void __fastcall SetControllerBand(TRaveIterateBand* Value);
	void __fastcall SetBandStyle(TRaveBandStyle* Value);
	TRaveRegion* __fastcall GetRegion(void);
	Rvdefine::TRaveUnits __fastcall GetMinHeight(void);
	virtual Rvdefine::TRaveUnits __fastcall GetLeft(void);
	virtual Rvdefine::TRaveUnits __fastcall GetWidth(void);
	virtual void __fastcall AnchorAdjust(bool WidthChanging, Rvdefine::TRaveUnits OldWidth, Rvdefine::TRaveUnits NewWidth, bool HeightChanging, Rvdefine::TRaveUnits OldHeight, Rvdefine::TRaveUnits NewHeight);
	virtual void __fastcall BeforeReport(void);
	AnsiString __fastcall GetCurrGroupValue();
	AnsiString __fastcall GetNextGroupValue();
	virtual void __fastcall SaveState(void);
	virtual void __fastcall RestoreState(void);
	virtual bool __fastcall GetSaving(void);
	virtual void __fastcall Changing(Rvclass::TRaveComponent* OldItem, Rvclass::TRaveComponent* NewItem);
	void __fastcall SetDesignerHide(bool Value);
	virtual void __fastcall SetName(const AnsiString NewName);
	virtual void __fastcall Preprint(void);
	virtual void __fastcall ResetState(void);
	virtual void __fastcall SetBandTop(void);
	void __fastcall DoGetGroup(AnsiString &Value);
	__property bool DoSplit = {read=FDoSplit, write=FDoSplit, nodefault};
	
public:
	__fastcall virtual TRaveBand(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveBand(void);
	virtual void __fastcall Paint(Graphics::TCanvas* Canvas);
	virtual void __fastcall CreatePips(void);
	virtual void __fastcall UpdatePips(void);
	virtual bool __fastcall CanSelect(Rvclass::TRaveComponent* Item);
	virtual bool __fastcall IsSelectPoint(const Rvdefine::TRavePoint &Point);
	virtual void __fastcall PrintAll(Rpbase::TBaseReport* Report);
	virtual void __fastcall Resize(void);
	__property Rvdefine::TRaveUnits MinHeight = {read=GetMinHeight};
	__property TRaveRegion* Region = {read=GetRegion};
	__property bool Saving = {read=GetSaving, nodefault};
	
__published:
	__property bool AllowSplit = {read=FAllowSplit, write=FAllowSplit, default=0};
	__property TRaveBandStyle* BandStyle = {read=FBandStyle, write=SetBandStyle};
	__property TRaveIterateBand* ControllerBand = {read=FControllerBand, write=SetControllerBand};
	__property bool DesignerHide = {read=FDesignerHide, write=SetDesignerHide, default=0};
	__property bool FinishNewPage = {read=FFinishNewPage, write=FFinishNewPage, default=0};
	__property Rvdata::TRaveBaseDataView* GroupDataView = {read=FGroupDataView, write=FGroupDataView, default=0};
	__property AnsiString GroupKey = {read=FGroupKey, write=FGroupKey};
	__property Height ;
	__property Rvdefine::TRaveUnits MinHeightLeft = {read=FMinHeightLeft, write=FMinHeightLeft};
	__property TBandPositionMode PositionMode = {read=FPositionMode, write=FPositionMode, default=0};
	__property Rvdefine::TRaveUnits PositionValue = {read=FPositionValue, write=FPositionValue};
	__property TBandPrintLocSet ReprintLocs = {read=FReprintLocs, write=FReprintLocs, default=255};
	__property bool StartNewPage = {read=FStartNewPage, write=FStartNewPage, default=0};
	__property Rvclass::TRaveStringEvent* OnGetGroup = {read=FOnGetGroup, write=FOnGetGroup};
};


class PASCALIMPLEMENTATION TRaveIterateBand : public TRaveBand 
{
	typedef TRaveBand inherited;
	
protected:
	bool NewData;
	Classes::TList* FControlledList;
	bool FKeepBodyTogether;
	bool FKeepRowTogether;
	int FOrphanRows;
	int FWidowRows;
	TBandPrintLoc FLocOnRestart;
	int FMaxRows;
	bool FInitToFirst;
	bool Active;
	Rvclass::TRaveMethodList* CalcNewDataListenList;
	int FColumns;
	Rvdefine::TRaveUnits FColumnSpacing;
	int FCurrentColumn;
	Rvdefine::TRaveUnits SaveLastTop;
	Rvdefine::TRaveUnits SavePriorTop;
	Rvdefine::TRaveUnits SaveLastBottom;
	int SaveRegionColumn;
	AnsiString FDetailKey;
	Rvdefine::TRaveUnits __fastcall GetColumnStart(void);
	Rvdefine::TRaveUnits __fastcall GetColumnWidth(void);
	void __fastcall SetColumns(int Value);
	void __fastcall SetColumnSpacing(Rvdefine::TRaveUnits Value);
	virtual Rvdefine::TRaveUnits __fastcall GetLeft(void);
	virtual Rvdefine::TRaveUnits __fastcall GetWidth(void);
	virtual void __fastcall First(void) = 0 ;
	virtual void __fastcall Next(void) = 0 ;
	virtual void __fastcall GetCurrent(void) = 0 ;
	virtual bool __fastcall Eof(void) = 0 ;
	virtual bool __fastcall CheckRowsLeft(int ReqRows) = 0 ;
	void __fastcall AddControlled(TRaveBand* Value);
	void __fastcall RemoveControlled(TRaveBand* Value);
	virtual void __fastcall SaveState(void);
	virtual void __fastcall RestoreState(void);
	virtual void __fastcall Changing(Rvclass::TRaveComponent* OldItem, Rvclass::TRaveComponent* NewItem);
	void __fastcall ResetControlled(void);
	virtual void __fastcall Preprint(void);
	virtual void __fastcall SetBandTop(void);
	
public:
	__fastcall virtual TRaveIterateBand(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveIterateBand(void);
	virtual void __fastcall Paint(Graphics::TCanvas* Canvas);
	virtual void __fastcall PrintAll(Rpbase::TBaseReport* Report);
	virtual void __fastcall AddListener(AnsiString Conversation, Rvclass::TRaveListenEvent ListenMethod);
	virtual void __fastcall RemoveListener(AnsiString Conversation, Rvclass::TRaveListenEvent ListenMethod);
	virtual bool __fastcall Habla(AnsiString Conversation);
	__property TBandPrintLoc LocOnRestart = {read=FLocOnRestart, write=FLocOnRestart, nodefault};
	__property int CurrentColumn = {read=FCurrentColumn, nodefault};
	__property Rvdefine::TRaveUnits ColumnStart = {read=GetColumnStart};
	__property Rvdefine::TRaveUnits ColumnWidth = {read=GetColumnWidth};
	
__published:
	__property int Columns = {read=FColumns, write=SetColumns, default=1};
	__property Rvdefine::TRaveUnits ColumnSpacing = {read=FColumnSpacing, write=SetColumnSpacing};
	__property AnsiString DetailKey = {read=FDetailKey, write=FDetailKey};
	__property bool KeepBodyTogether = {read=FKeepBodyTogether, write=FKeepBodyTogether, default=0};
	__property bool KeepRowTogether = {read=FKeepRowTogether, write=FKeepRowTogether, default=0};
	__property int MaxRows = {read=FMaxRows, write=FMaxRows, default=0};
	__property bool InitToFirst = {read=FInitToFirst, write=FInitToFirst, default=1};
	__property int OrphanRows = {read=FOrphanRows, write=FOrphanRows, default=0};
	__property int WidowRows = {read=FWidowRows, write=FWidowRows, default=0};
};



#pragma pack(push, 4)
struct TBandDetail
{
	TRaveBand* Band;
	bool PointerDown;
	int PointerColor;
	int MasterColor;
	Byte Level;
	Byte MasterLevel;
	bool IsController;
	TBandDetail *ControllerDetail;
} ;
#pragma pack(pop)

struct TBandSearchRec;
typedef TBandSearchRec *PBandSearchRec;

#pragma pack(push, 4)
struct TBandSearchRec
{
	int Index;
	TRaveIterateBand* ControllerBand;
	TBandPrintLoc PrintLoc;
} ;
#pragma pack(pop)

class PASCALIMPLEMENTATION TRaveRegion : public Rvclass::TRaveContainerControl 
{
	typedef Rvclass::TRaveContainerControl inherited;
	
protected:
	TRaveBand* FReplaceBand;
	TRaveBandStyle* FReplaceBandStyle;
	Rvdefine::TRaveUnits PrintTop;
	Rvdefine::TRaveUnits SavePrintTop;
	TBandSearchRec BandSearchRec;
	Rvutil::TRaveStackList* BandSearchSave;
	TRaveBand* RestartBand;
	TBandPrintLoc RestartLoc;
	bool FRestarting;
	bool ForceRestart;
	bool FTopOfRegion;
	TBandPrintLoc PrintLoc;
	TBandPrintOcc PrintOcc;
	int FCurrentColumn;
	int FColumns;
	Rvdefine::TRaveUnits FColumnSpacing;
	virtual bool __fastcall AcceptChild(TMetaClass* NewChild);
	virtual bool __fastcall GetDonePrinting(void);
	virtual void __fastcall BeforeReport(void);
	Rvdefine::TRaveUnits __fastcall GetColumnStart(void);
	Rvdefine::TRaveUnits __fastcall GetBandWidth(Rvdefine::TRaveUnits RegionWidth);
	Rvdefine::TRaveUnits __fastcall GetColumnWidth(void);
	void __fastcall SetColumns(int Value);
	void __fastcall SetColumnSpacing(Rvdefine::TRaveUnits Value);
	void __fastcall PushBandSearchRec(void);
	void __fastcall PopBandSearchRec(void);
	Rvdefine::TRaveUnits __fastcall HeightAvail(void);
	virtual void __fastcall InitData(void);
	virtual void __fastcall DoneData(void);
	virtual void __fastcall SetName(const AnsiString NewName);
	
public:
	__fastcall virtual TRaveRegion(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveRegion(void);
	void __fastcall SaveState(void);
	void __fastcall RestoreState(void);
	void __fastcall Restart(TRaveBand* Band);
	void __fastcall UndoRestart(void);
	void __fastcall PostponeRestart(void);
	virtual void __fastcall Paint(Graphics::TCanvas* Canvas);
	virtual void __fastcall PaintAll(Graphics::TCanvas* Canvas);
	void __fastcall PrintBands(Rpbase::TBaseReport* Report, TRaveIterateBand* ControllerBand, TBandPrintLoc APrintLoc);
	virtual void __fastcall PrintAll(Rpbase::TBaseReport* Report);
	TRaveBand* __fastcall FirstSubBand(TBandSearchRec &BandSearchRec, TRaveIterateBand* ControllerBand, TBandPrintLoc PrintLoc);
	TRaveBand* __fastcall NextSubBand(TBandSearchRec &BandSearchRec);
	__property TRaveBand* ReplaceBand = {read=FReplaceBand, write=FReplaceBand};
	__property TRaveBandStyle* ReplaceBandStyle = {read=FReplaceBandStyle, write=FReplaceBandStyle};
	__property bool Restarting = {read=FRestarting, nodefault};
	__property int CurrentColumn = {read=FCurrentColumn, nodefault};
	__property Rvdefine::TRaveUnits ColumnStart = {read=GetColumnStart};
	__property Rvdefine::TRaveUnits ColumnWidth = {read=GetColumnWidth};
	__property bool TopOfRegion = {read=FTopOfRegion, nodefault};
	
__published:
	__property Height ;
	__property Left ;
	__property Top ;
	__property Width ;
	__property int Columns = {read=FColumns, write=SetColumns, default=1};
	__property Rvdefine::TRaveUnits ColumnSpacing = {read=FColumnSpacing, write=SetColumnSpacing};
};


class DELPHICLASS TRaveDataBand;
class PASCALIMPLEMENTATION TRaveDataBand : public TRaveIterateBand 
{
	typedef TRaveIterateBand inherited;
	
protected:
	Rvdata::TRaveBaseDataView* FDataView;
	Rvdata::TRaveBaseDataView* FMasterDataView;
	AnsiString FMasterKey;
	AnsiString FSortKey;
	virtual bool __fastcall GetSaving(void);
	virtual void __fastcall SaveState(void);
	virtual void __fastcall RestoreState(void);
	virtual void __fastcall First(void);
	virtual void __fastcall Next(void);
	virtual void __fastcall GetCurrent(void);
	virtual bool __fastcall Eof(void);
	virtual bool __fastcall CheckRowsLeft(int ReqRows);
	virtual void __fastcall Changing(Rvclass::TRaveComponent* OldItem, Rvclass::TRaveComponent* NewItem);
	
__published:
	__property Rvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView, default=0};
	__property Rvdata::TRaveBaseDataView* MasterDataView = {read=FMasterDataView, write=FMasterDataView, default=0};
	__property AnsiString MasterKey = {read=FMasterKey, write=FMasterKey};
	__property AnsiString SortKey = {read=FSortKey, write=FSortKey};
public:
	#pragma option push -w-inl
	/* TRaveIterateBand.Create */ inline __fastcall virtual TRaveDataBand(Classes::TComponent* AOwner) : TRaveIterateBand(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveIterateBand.Destroy */ inline __fastcall virtual ~TRaveDataBand(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveDataCycle;
class PASCALIMPLEMENTATION TRaveDataCycle : public Rvclass::TRaveComponent 
{
	typedef Rvclass::TRaveComponent inherited;
	
protected:
	Rvdata::TRaveBaseDataView* FDataView;
	AnsiString FDetailKey;
	bool FInitToFirst;
	Rvdata::TRaveBaseDataView* FMasterDataView;
	AnsiString FMasterKey;
	int FMaxRows;
	AnsiString FSortKey;
	int RowCount;
	bool Active;
	virtual bool __fastcall GetDonePrinting(void);
	virtual void __fastcall BeforeReport(void);
	virtual void __fastcall Changing(Rvclass::TRaveComponent* OldItem, Rvclass::TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRaveDataCycle(Classes::TComponent* AOwner);
	virtual void __fastcall Print(Rpbase::TBaseReport* Report);
	
__published:
	__property Rvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView, default=0};
	__property AnsiString DetailKey = {read=FDetailKey, write=FDetailKey};
	__property bool InitToFirst = {read=FInitToFirst, write=FInitToFirst, default=1};
	__property Rvdata::TRaveBaseDataView* MasterDataView = {read=FMasterDataView, write=FMasterDataView, default=0};
	__property AnsiString MasterKey = {read=FMasterKey, write=FMasterKey};
	__property int MaxRows = {read=FMaxRows, write=FMaxRows, default=0};
	__property AnsiString SortKey = {read=FSortKey, write=FSortKey};
public:
	#pragma option push -w-inl
	/* TRaveComponent.Destroy */ inline __fastcall virtual ~TRaveDataCycle(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
static const Shortint BandColors = 0x14;
extern PACKAGE Graphics::TColor BandColor[20];
extern PACKAGE Classes::TList* __fastcall CreateBandList(TRaveRegion* Region);
extern PACKAGE void __fastcall FreeBandList(Classes::TList* BandList);

}	/* namespace Rvcsrpt */
using namespace Rvcsrpt;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvCsRpt
