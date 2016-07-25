// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvClass.pas' rev: 6.00

#ifndef QRvClassHPP
#define QRvClassHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpBase.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <QExtCtrls.hpp>	// Pascal unit
#include <QButtons.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvclass
{
//-- type declarations -------------------------------------------------------
typedef int HRgn;

#pragma option push -b-
enum TRaveComponentStyle { rcsContainer, rcsInvisible, rcsLoading, rcsReflecting };
#pragma option pop

typedef Set<TRaveComponentStyle, rcsContainer, rcsReflecting>  TRaveComponentStyleSet;

class DELPHICLASS TRaveComponent;
typedef void __fastcall (__closure *TRaveListenEvent)(TRaveComponent* Speaker, System::TObject* Msg);

class DELPHICLASS TRavePersistentList;
class PASCALIMPLEMENTATION TRavePersistentList : public Classes::TList 
{
	typedef Classes::TList inherited;
	
public:
	HIDESBASE void __fastcall Assign(TRavePersistentList* Source);
	virtual Classes::TPersistent* __fastcall CreateItem(AnsiString ItemClass, Classes::TComponent* RootOwner);
public:
	#pragma option push -w-inl
	/* TList.Destroy */ inline __fastcall virtual ~TRavePersistentList(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRavePersistentList(void) : Classes::TList() { }
	#pragma option pop
	
};


class DELPHICLASS TRaveComponentList;
class PASCALIMPLEMENTATION TRaveComponentList : public Classes::TList 
{
	typedef Classes::TList inherited;
	
public:
	#pragma option push -w-inl
	/* TList.Destroy */ inline __fastcall virtual ~TRaveComponentList(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveComponentList(void) : Classes::TList() { }
	#pragma option pop
	
};


class DELPHICLASS TRaveMethodList;
class PASCALIMPLEMENTATION TRaveMethodList : public Classes::TList 
{
	typedef Classes::TList inherited;
	
public:
	__fastcall virtual ~TRaveMethodList(void);
	void __fastcall AddMethod(const System::TMethod &Method);
	void __fastcall RemoveMethod(const System::TMethod &Method);
	void __fastcall RemoveObject(System::TObject* Obj);
	void __fastcall ReplaceObject(System::TObject* OldObj, System::TObject* NewObj);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveMethodList(void) : Classes::TList() { }
	#pragma option pop
	
};


typedef TMetaClass*TRaveComponentClass;

class DELPHICLASS TRavePage;
class DELPHICLASS TRaveProjectItem;
class DELPHICLASS TRaveContainerComponent;
class DELPHICLASS TRaveSimpleEvent;
class DELPHICLASS TRaveEvent;
class PASCALIMPLEMENTATION TRaveEvent : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	AnsiString FHeader;
	AnsiString FBody;
	int FPosition;
	TRaveProjectItem* FModuleOwner;
	int FHeaderSourceLine;
	int FBodySourceLine;
	TRaveComponent* FOwner;
	AnsiString FName;
	System::TObject* FSuspendState;
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	void __fastcall DefineParam(void *Param);
	virtual AnsiString __fastcall GetDeclaration();
	virtual AnsiString __fastcall GetFooter();
	virtual bool __fastcall SuspendValid(void);
	
public:
	void __fastcall Execute(void);
	virtual void __fastcall DefineParams(void);
	virtual AnsiString __fastcall ParamSource();
	bool __fastcall Suspended(void);
	__property TRaveComponent* Owner = {read=FOwner, write=FOwner};
	__property int HeaderSourceLine = {read=FHeaderSourceLine, write=FHeaderSourceLine, nodefault};
	__property int BodySourceLine = {read=FBodySourceLine, write=FBodySourceLine, nodefault};
	__property AnsiString Declaration = {read=GetDeclaration};
	__property AnsiString Footer = {read=GetFooter};
	
__published:
	__property AnsiString Body = {read=FBody, write=FBody};
	__property AnsiString Header = {read=FHeader, write=FHeader};
	__property TRaveProjectItem* ModuleOwner = {read=FModuleOwner, write=FModuleOwner};
	__property int Position = {read=FPosition, write=FPosition, nodefault};
	__property AnsiString Name = {read=FName, write=FName};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TRaveEvent(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveEvent(void) : Classes::TPersistent() { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TRaveSimpleEvent : public TRaveEvent 
{
	typedef TRaveEvent inherited;
	
__published:
	__property TRaveComponent* Sender = {read=FOwner, write=FOwner};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TRaveSimpleEvent(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveSimpleEvent(void) : TRaveEvent() { }
	#pragma option pop
	
};


class DELPHICLASS TRaveFiler;
class PASCALIMPLEMENTATION TRaveComponent : public Classes::TComponent 
{
	typedef Classes::TComponent inherited;
	
protected:
	TRaveComponentStyleSet FControlStyle;
	bool FDevLocked;
	System::TObject* FHolder;
	TRaveComponent* FParent;
	bool FVisible;
	bool FLocked;
	TRavePage* FPage;
	TRaveProjectItem* FReport;
	TRaveProjectItem* FProject;
	TRaveSimpleEvent* FOnBeforeReport;
	TRaveSimpleEvent* FOnAfterReport;
	TRaveSimpleEvent* FOnBeforePrint;
	TRaveSimpleEvent* FOnAfterPrint;
	/* virtual class method */ virtual bool __fastcall UseMaster(TMetaClass* vmt);
	/* virtual class method */ virtual bool __fastcall AcceptParent(TMetaClass* vmt, TMetaClass* NewParent);
	void __fastcall DestroyEvents(void);
	virtual bool __fastcall AcceptChild(TMetaClass* NewChild);
	virtual void __fastcall DestroyChildren(void);
	virtual void __fastcall PostLoad(void);
	virtual void __fastcall SetParent(TRaveComponent* Value);
	void __fastcall SetVisible(bool Value);
	virtual void __fastcall AddChild(TRaveComponent* Value);
	virtual void __fastcall RemoveChild(TRaveComponent* Value);
	virtual TRaveComponent* __fastcall GetChild(int Index);
	virtual int __fastcall GetChildCount(void);
	virtual int __fastcall GetChildIndex(void);
	DYNAMIC void __fastcall SetParentComponent(Classes::TComponent* Value);
	virtual Qrvdefine::TRaveUnits __fastcall GetPageLeft(void);
	virtual Qrvdefine::TRaveUnits __fastcall GetPageTop(void);
	virtual Qrvdefine::TRaveUnits __fastcall GetPageRight(void);
	virtual Qrvdefine::TRaveUnits __fastcall GetPageBottom(void);
	virtual void __fastcall SetName(const AnsiString NewName);
	virtual void __fastcall OverrideProperties(TRaveFiler* Filer);
	virtual bool __fastcall GetDonePrinting(void);
	virtual void __fastcall BeforeReport(void);
	virtual void __fastcall AfterReport(void);
	virtual void __fastcall BeforePrint(void);
	virtual void __fastcall Print(Qrpbase::TBaseReport* Report);
	virtual void __fastcall AfterPrint(void);
	virtual void __fastcall InitData(void);
	virtual void __fastcall DoneData(void);
	virtual void __fastcall SetDepth(TRaveComponent* Control, int Adjustment);
	virtual void __fastcall Changing(TRaveComponent* OldItem, TRaveComponent* NewItem);
	void __fastcall SetLocked(bool Value);
	void __fastcall SetDevLocked(bool Value);
	virtual void __fastcall SaveDesigned(void);
	virtual void __fastcall RestoreDesigned(void);
	virtual void __fastcall SaveAdjusted(void);
	virtual void __fastcall RestoreAdjusted(void);
	void __fastcall Speak(TRaveMethodList* List, System::TObject* Msg);
	
public:
	__fastcall virtual TRaveComponent(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveComponent(void);
	virtual void __fastcall Hide(void);
	void __fastcall Invalidate(void);
	virtual void __fastcall Show(void);
	DYNAMIC Classes::TComponent* __fastcall GetParentComponent(void);
	void __fastcall MoveForward(void);
	void __fastcall MoveBehind(void);
	void __fastcall BringToFront(void);
	void __fastcall SendToBack(void);
	virtual bool __fastcall AllowPrint(void);
	virtual void __fastcall PrintAll(Qrpbase::TBaseReport* Report);
	TRaveComponent* __fastcall CreateChild(TMetaClass* AClassType, AnsiString ABaseName = "", Qrvdefine::TRaveUnits ALeft = 0.000000E+00, Qrvdefine::TRaveUnits ATop = 0.000000E+00, Qrvdefine::TRaveUnits AWidth = 0.000000E+00, Qrvdefine::TRaveUnits AHeight = 0.000000E+00);
	bool __fastcall IsLocked(void);
	bool __fastcall IsLocker(void);
	TRaveComponent* __fastcall GetLocker(void);
	Qrvdefine::TRaveUnits __fastcall XU2I(Qrvdefine::TRaveUnits Value);
	Qrvdefine::TRaveUnits __fastcall YU2I(Qrvdefine::TRaveUnits Value);
	Qrvdefine::TRaveUnits __fastcall XI2U(Qrvdefine::TRaveUnits Value);
	Qrvdefine::TRaveUnits __fastcall YI2U(Qrvdefine::TRaveUnits Value);
	virtual bool __fastcall CanSelect(TRaveComponent* Item);
	virtual void __fastcall PrintParams(Qrvdefine::TRaveUnits &HDelta, Qrvdefine::TRaveUnits &VDelta);
	virtual void __fastcall AddListener(AnsiString Conversation, TRaveListenEvent ListenMethod);
	virtual void __fastcall RemoveListener(AnsiString Conversation, TRaveListenEvent ListenMethod);
	virtual bool __fastcall Habla(AnsiString Conversation);
	__property Qrvdefine::TRaveUnits PageLeft = {read=GetPageLeft};
	__property Qrvdefine::TRaveUnits PageTop = {read=GetPageTop};
	__property Qrvdefine::TRaveUnits PageRight = {read=GetPageRight};
	__property Qrvdefine::TRaveUnits PageBottom = {read=GetPageBottom};
	__property TRaveComponent* Child[int Index] = {read=GetChild};
	__property int ChildCount = {read=GetChildCount, nodefault};
	__property int ChildIndex = {read=GetChildIndex, nodefault};
	__property TRaveComponentStyleSet ControlStyle = {read=FControlStyle, write=FControlStyle, nodefault};
	__property bool DonePrinting = {read=GetDonePrinting, nodefault};
	__property TRavePage* Page = {read=FPage};
	__property TRaveProjectItem* Report = {read=FReport};
	__property TRaveProjectItem* Project = {read=FProject};
	__property TRaveComponent* Parent = {read=FParent, write=SetParent};
	
__published:
	__property bool Locked = {read=FLocked, write=SetLocked, default=0};
	__property bool DevLocked = {read=FDevLocked, write=SetDevLocked, default=0};
	__property bool Visible = {read=FVisible, write=SetVisible, default=1};
	__property TRaveSimpleEvent* OnBeforeReport = {read=FOnBeforeReport, write=FOnBeforeReport};
	__property TRaveSimpleEvent* OnAfterReport = {read=FOnAfterReport, write=FOnAfterReport};
	__property TRaveSimpleEvent* OnBeforePrint = {read=FOnBeforePrint, write=FOnBeforePrint};
	__property TRaveSimpleEvent* OnAfterPrint = {read=FOnAfterPrint, write=FOnAfterPrint};
};


class PASCALIMPLEMENTATION TRaveContainerComponent : public TRaveComponent 
{
	typedef TRaveComponent inherited;
	
private:
	Classes::TList* ChildList;
	
protected:
	virtual void __fastcall DestroyChildren(void);
	virtual void __fastcall AddChild(TRaveComponent* Value);
	virtual void __fastcall RemoveChild(TRaveComponent* Value);
	virtual TRaveComponent* __fastcall GetChild(int Index);
	virtual int __fastcall GetChildCount(void);
	virtual void __fastcall SetDepth(TRaveComponent* Control, int Adjustment);
	
public:
	__fastcall virtual TRaveContainerComponent(Classes::TComponent* AOwner);
public:
	#pragma option push -w-inl
	/* TRaveComponent.Destroy */ inline __fastcall virtual ~TRaveContainerComponent(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveModule;
class PASCALIMPLEMENTATION TRaveModule : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	AnsiString FModuleName;
	virtual void __fastcall PostLoad(void) = 0 ;
	
public:
	__fastcall virtual TRaveModule(Classes::TStream* Stream, TRaveProjectItem* AOwner, TRaveProjectItem* AProjectItem) = 0 ;
	__fastcall virtual TRaveModule(AnsiString FileName, TRaveProjectItem* AOwner) = 0 ;
	virtual void __fastcall SaveToStream(Classes::TStream* Stream) = 0 ;
	virtual void __fastcall Execute(int Position, System::TObject* &SuspendState) = 0 ;
	virtual void __fastcall SetStringVar(AnsiString Name, AnsiString Value) = 0 ;
	virtual AnsiString __fastcall GetStringVar(AnsiString Name) = 0 ;
	virtual void __fastcall SetIntVar(AnsiString Name, int Value) = 0 ;
	virtual int __fastcall GetIntVar(AnsiString Name) = 0 ;
	virtual void __fastcall SetFloatVar(AnsiString Name, Extended Value) = 0 ;
	virtual Extended __fastcall GetFloatVar(AnsiString Name) = 0 ;
	virtual void __fastcall PushParam(void *Param) = 0 ;
	virtual void __fastcall Changing(TRaveModule* OldItem, TRaveModule* NewItem) = 0 ;
	virtual void __fastcall BuildSource(void) = 0 ;
	virtual void __fastcall Compile(bool Definition) = 0 ;
	virtual void __fastcall EventConnect(void) = 0 ;
	__property AnsiString ModuleName = {read=FModuleName, write=FModuleName};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveModule(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRaveModule(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TRaveProjectItem : public TRaveContainerComponent 
{
	typedef TRaveContainerComponent inherited;
	
protected:
	bool FActive;
	AnsiString FDescription;
	AnsiString FFullName;
	Classes::TStrings* FParams;
	Classes::TStrings* FPIVars;
	TRaveModule* FModule;
	bool FCompileNeeded;
	bool FIncluded;
	void __fastcall ReadModule(Classes::TReader* Reader);
	void __fastcall WriteModule(Classes::TWriter* Writer);
	void __fastcall SetModule(TRaveModule* AModule);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	void __fastcall SetActive(bool Value);
	virtual void __fastcall SetName(const AnsiString NewName);
	void __fastcall SetParameters(Classes::TStrings* Value);
	void __fastcall SetPIVars(Classes::TStrings* Value);
	void __fastcall SetCompileNeeded(bool Value);
	AnsiString __fastcall GetModuleName();
	virtual void __fastcall PostLoad(void);
	virtual void __fastcall Changing(TRaveComponent* OldItem, TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRaveProjectItem(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveProjectItem(void);
	virtual void __fastcall SaveToStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall LoadFromStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall Open(void);
	virtual void __fastcall Close(void);
	virtual void __fastcall PrepareModule(void);
	__property bool Active = {read=FActive, write=SetActive, nodefault};
	__property Classes::TStrings* Parameters = {read=FParams, write=SetParameters};
	__property Classes::TStrings* PIVars = {read=FPIVars, write=SetPIVars};
	__property TRaveModule* Module = {read=FModule, write=SetModule};
	__property AnsiString ModuleName = {read=GetModuleName};
	__property bool Included = {read=FIncluded, write=FIncluded, nodefault};
	
__published:
	__property bool CompileNeeded = {read=FCompileNeeded, write=SetCompileNeeded, nodefault};
	__property AnsiString Description = {read=FDescription, write=FDescription};
	__property AnsiString FullName = {read=FFullName, write=FFullName};
};


class DELPHICLASS TRaveDesigner;
class DELPHICLASS TRaveControl;
class DELPHICLASS TRavePip;
class PASCALIMPLEMENTATION TRaveDesigner : public Qcontrols::TCustomControl 
{
	typedef Qcontrols::TCustomControl inherited;
	
protected:
	Qgraphics::TPen* FGridPen;
	TRavePage* FPage;
	double FMinimumBorder;
	double FZoomFactor;
	void __fastcall SetPage(TRavePage* APage);
	virtual void __fastcall SetMinimumBorder(double Value) = 0 ;
	virtual void __fastcall SetGridPen(Qgraphics::TPen* Value) = 0 ;
	virtual void __fastcall SetZoomFactor(double Value) = 0 ;
	virtual int __fastcall GetSelections(void) = 0 ;
	virtual TRaveComponent* __fastcall GetSelection(int Index) = 0 ;
	virtual bool __fastcall GetZoomToolActive(void) = 0 ;
	virtual void __fastcall SetZoomToolActive(bool Value) = 0 ;
	
public:
	virtual void __fastcall AddPip(Byte Index, TRaveControl* Control, Qcontrols::TCursor Cursor, Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y) = 0 ;
	virtual void __fastcall UpdatePip(Byte Index, TRaveControl* Control, Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y) = 0 ;
	virtual void __fastcall RemovePips(TRaveControl* Control) = 0 ;
	virtual void __fastcall MovePip(Byte Index, Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y) = 0 ;
	virtual void __fastcall SwitchPips(TRavePip* RavePip, Byte SwitchIdx) = 0 ;
	virtual void __fastcall Modified(void) = 0 ;
	virtual void __fastcall DeselectControl(TRaveComponent* Control) = 0 ;
	virtual void __fastcall ClearSelection(void) = 0 ;
	virtual void __fastcall SelectControl(TRaveComponent* Control) = 0 ;
	virtual void __fastcall ToggleControl(TRaveComponent* Control) = 0 ;
	virtual bool __fastcall IsSelected(TRaveComponent* Control) = 0 ;
	virtual void __fastcall DeleteSelection(void) = 0 ;
	virtual void __fastcall CopySelection(void) = 0 ;
	virtual void __fastcall PasteSelection(void) = 0 ;
	virtual void __fastcall SelectChildren(TRaveComponent* Control) = 0 ;
	virtual void __fastcall SelectType(TRaveProjectItem* ProjectItem, TMetaClass* RaveClass) = 0 ;
	virtual void __fastcall MoveSelection(Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y) = 0 ;
	virtual void __fastcall AlignSelection(int AlignType) = 0 ;
	virtual void __fastcall PostChanges(void) = 0 ;
	virtual TRaveComponent* __fastcall FindControl(AnsiString Name) = 0 ;
	virtual TRaveControl* __fastcall FindControlAt(Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y) = 0 ;
	virtual TRaveControl* __fastcall FindContainerAt(Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y, TMetaClass* NewChild) = 0 ;
	virtual int __fastcall XI2D(Qrvdefine::TRaveUnits Value) = 0 ;
	virtual int __fastcall YI2D(Qrvdefine::TRaveUnits Value) = 0 ;
	virtual Qrvdefine::TRaveUnits __fastcall XD2I(int Value) = 0 ;
	virtual Qrvdefine::TRaveUnits __fastcall YD2I(int Value) = 0 ;
	virtual Qrvdefine::TRaveUnits __fastcall SnapX(Qrvdefine::TRaveUnits Value) = 0 ;
	virtual Qrvdefine::TRaveUnits __fastcall SnapY(Qrvdefine::TRaveUnits Value) = 0 ;
	virtual Qrvdefine::TRaveUnits __fastcall VisibleWidth(void) = 0 ;
	virtual Qrvdefine::TRaveUnits __fastcall VisibleHeight(void) = 0 ;
	virtual Qrvdefine::TRaveUnits __fastcall CenterWidth(void) = 0 ;
	virtual Qrvdefine::TRaveUnits __fastcall CenterHeight(void) = 0 ;
	virtual double __fastcall ZoomToRect(Qrvdefine::TRaveUnits X1, Qrvdefine::TRaveUnits Y1, Qrvdefine::TRaveUnits X2, Qrvdefine::TRaveUnits Y2) = 0 ;
	virtual void __fastcall ZoomPage(void) = 0 ;
	virtual void __fastcall ZoomPageWidth(void) = 0 ;
	virtual void __fastcall ZoomSelected(void) = 0 ;
	virtual void __fastcall ZoomIn(Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y) = 0 ;
	virtual void __fastcall ZoomOut(void) = 0 ;
	virtual void __fastcall CenterWindow(Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y) = 0 ;
	__property Qgraphics::TPen* GridPen = {read=FGridPen, write=SetGridPen};
	__property double MinimumBorder = {read=FMinimumBorder, write=SetMinimumBorder};
	__property TRavePage* Page = {read=FPage};
	__property int Selections = {read=GetSelections, nodefault};
	__property TRaveComponent* Selection[int Index] = {read=GetSelection};
	__property double ZoomFactor = {read=FZoomFactor, write=SetZoomFactor};
	__property bool ZoomToolActive = {read=GetZoomToolActive, write=SetZoomToolActive, nodefault};
public:
	#pragma option push -w-inl
	/* TCustomControl.Create */ inline __fastcall virtual TRaveDesigner(Classes::TComponent* AOwner) : Qcontrols::TCustomControl(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TRaveDesigner(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWidgetControl.CreateParented */ inline __fastcall TRaveDesigner(Qt::QWidgetH* ParentWidget) : Qcontrols::TCustomControl(ParentWidget) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TRavePage : public TRaveProjectItem 
{
	typedef TRaveProjectItem inherited;
	
protected:
	TRaveDesigner* FDesigner;
	bool FGlobal;
	Qrpdefine::TRavePaperSize FPaperSize;
	Qrvdefine::TRaveUnits FPageWidth;
	Qrvdefine::TRaveUnits FPageHeight;
	Qcontrols::TWidgetControl* FStorage;
	Qrvdefine::TGotoMode FGotoMode;
	TRavePage* FGotoPage;
	Qrvdefine::TRaveUnits FGridSpacing;
	int FGridLines;
	Qrpdefine::TOrientation FOrientation;
	Qrvdefine::TRaveBin FBin;
	AnsiString FBinCustom;
	bool FWasteFit;
	void __fastcall SetGridLines(int Value);
	void __fastcall SetGridSpacing(Qrvdefine::TRaveUnits Value);
	void __fastcall AnchorAdjustAll(bool WidthChanging, Qrvdefine::TRaveUnits OldWidth, Qrvdefine::TRaveUnits NewWidth, bool HeightChanging, Qrvdefine::TRaveUnits OldHeight, Qrvdefine::TRaveUnits NewHeight);
	void __fastcall SetPageWidth(Qrvdefine::TRaveUnits Value);
	void __fastcall SetPageHeight(Qrvdefine::TRaveUnits Value);
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	virtual void __fastcall Changing(TRaveComponent* OldItem, TRaveComponent* NewItem);
	void __fastcall SetPaperSize(Qrpdefine::TRavePaperSize Value);
	void __fastcall SetOrientation(Qrpdefine::TOrientation Value);
	
public:
	__fastcall virtual TRavePage(Classes::TComponent* AOwner);
	__fastcall virtual ~TRavePage(void);
	virtual void __fastcall LoadFromStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall PrintAll(Qrpbase::TBaseReport* Report);
	void __fastcall Resize(void);
	bool __fastcall StorePageDim(void);
	virtual void __fastcall PrepareModule(void);
	void __fastcall PrepareSize(Qrvdefine::TRaveUnits AWidth, Qrvdefine::TRaveUnits AHeight);
	void __fastcall Paint(Qgraphics::TCanvas* Canvas);
	void __fastcall AddPip(Byte Index, TRaveControl* Control, Qcontrols::TCursor Cursor, Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y);
	void __fastcall UpdatePip(Byte Index, TRaveControl* Control, Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y);
	void __fastcall SwitchPips(TRavePip* RavePip, Byte SwitchIdx);
	__property TRaveDesigner* Designer = {read=FDesigner, write=FDesigner};
	__property bool Global = {read=FGlobal, nodefault};
	__property Qcontrols::TWidgetControl* Storage = {read=FStorage, write=FStorage};
	
__published:
	__property Qrvdefine::TRaveBin Bin = {read=FBin, write=FBin, nodefault};
	__property AnsiString BinCustom = {read=FBinCustom, write=FBinCustom};
	__property Qrvdefine::TGotoMode GotoMode = {read=FGotoMode, write=FGotoMode, default=0};
	__property TRavePage* GotoPage = {read=FGotoPage, write=FGotoPage, default=0};
	__property int GridLines = {read=FGridLines, write=SetGridLines, nodefault};
	__property Qrvdefine::TRaveUnits GridSpacing = {read=FGridSpacing, write=SetGridSpacing};
	__property Qrpdefine::TOrientation Orientation = {read=FOrientation, write=SetOrientation, nodefault};
	__property Qrvdefine::TRaveUnits PageHeight = {read=FPageHeight, write=SetPageHeight, stored=StorePageDim};
	__property Qrvdefine::TRaveUnits PageWidth = {read=FPageWidth, write=SetPageWidth, stored=StorePageDim};
	__property Qrpdefine::TRavePaperSize PaperSize = {read=FPaperSize, write=SetPaperSize, nodefault};
	__property Parameters ;
	__property PIVars ;
	__property bool WasteFit = {read=FWasteFit, write=FWasteFit, default=0};
};



class DELPHICLASS TRaveFont;
class PASCALIMPLEMENTATION TRaveControl : public TRaveComponent 
{
	typedef TRaveComponent inherited;
	
protected:
	TRaveControl* FMirror;
	Classes::TList* FMirrorList;
	Qrvdefine::TRavePoint FMirrorPoint;
	Qcontrols::TCursor FCursor;
	Qrvdefine::TRaveUnits FLeft;
	Qrvdefine::TRaveUnits FTop;
	Qrvdefine::TRaveUnits FWidth;
	Qrvdefine::TRaveUnits FHeight;
	Qrvdefine::TRaveUnits FDesLeftx;
	Qrvdefine::TRaveUnits FDesTopx;
	Qrvdefine::TRaveUnits FDesWidthx;
	Qrvdefine::TRaveUnits FDesHeightx;
	Qrvdefine::TRaveUnits FAdjLeft;
	Qrvdefine::TRaveUnits FAdjTop;
	Qrvdefine::TRaveUnits FAdjWidth;
	Qrvdefine::TRaveUnits FAdjHeight;
	Qrvdefine::TDisplayOn FDisplayOn;
	Qrvdefine::TRaveAnchor FAnchor;
	bool DisableResize;
	virtual void __fastcall AddMirror(TRaveControl* Value);
	virtual void __fastcall RemoveMirror(TRaveControl* Value);
	void __fastcall SetMirror(TRaveControl* Value);
	void __fastcall PrintMirror(Qrpbase::TBaseReport* AReport, TRaveControl* AOrig, TRaveControl* AControl);
	void __fastcall PaintMirror(Qgraphics::TCanvas* Canvas, TRaveControl* Orig, TRaveControl* Control);
	virtual void __fastcall SetFontAttr(Qrvdefine::TFontAttribute Index, TRaveFont* Value);
	virtual TRaveFont* __fastcall GetFontAttr(Qrvdefine::TFontAttribute Index);
	int __fastcall XI2P(Qrvdefine::TRaveUnits Value);
	int __fastcall YI2P(Qrvdefine::TRaveUnits Value);
	int __fastcall XI2D(Qrvdefine::TRaveUnits Value);
	int __fastcall YI2D(Qrvdefine::TRaveUnits Value);
	Qrvdefine::TRaveUnits __fastcall XD2I(int Value);
	Qrvdefine::TRaveUnits __fastcall YD2I(int Value);
	int __fastcall SetClipped(Qgraphics::TCanvas* Canvas);
	void __fastcall SetUnclipped(Qgraphics::TCanvas* Canvas, int SaveRgn);
	Qrvdefine::TRavePoint __fastcall PageToClient(const Qrvdefine::TRavePoint &Point);
	Qrvdefine::TRavePoint __fastcall ClientToPage(const Qrvdefine::TRavePoint &Point);
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	virtual void __fastcall SetText(AnsiString Value);
	virtual AnsiString __fastcall GetText();
	virtual void __fastcall SetFontJustify(Qrpdefine::TPrintJustify Value);
	virtual Qrpdefine::TPrintJustify __fastcall GetFontJustify(void);
	virtual void __fastcall SetForeColor(Qgraphics::TColor Value);
	virtual Qgraphics::TColor __fastcall GetForeColor(void);
	virtual void __fastcall SetBackColor(Qgraphics::TColor Value);
	virtual Qgraphics::TColor __fastcall GetBackColor(void);
	virtual void __fastcall SetFillStyle(Qrvdefine::TRaveFillStyle Value);
	virtual Qrvdefine::TRaveFillStyle __fastcall GetFillStyle(void);
	virtual void __fastcall SetLineWidth(double Value);
	virtual double __fastcall GetLineWidth(void);
	virtual void __fastcall SetLineWidthType(Qrvdefine::TLineWidthType Value);
	virtual Qrvdefine::TLineWidthType __fastcall GetLineWidthType(void);
	virtual void __fastcall SetLineStyle(Qgraphics::TPenStyle Value);
	virtual Qgraphics::TPenStyle __fastcall GetLineStyle(void);
	virtual void __fastcall Changing(TRaveComponent* OldItem, TRaveComponent* NewItem);
	virtual bool __fastcall GetDonePrinting(void);
	virtual void __fastcall AnchorAdjust(bool WidthChanging, Qrvdefine::TRaveUnits OldWidth, Qrvdefine::TRaveUnits NewWidth, bool HeightChanging, Qrvdefine::TRaveUnits OldHeight, Qrvdefine::TRaveUnits NewHeight);
	virtual void __fastcall AnchorAdjustAll(bool WidthChanging, Qrvdefine::TRaveUnits OldWidth, Qrvdefine::TRaveUnits NewWidth, bool HeightChanging, Qrvdefine::TRaveUnits OldHeight, Qrvdefine::TRaveUnits NewHeight);
	void __fastcall ResizeAll(void);
	virtual Qrvdefine::TRaveUnits __fastcall GetLeft(void);
	virtual void __fastcall SetLeft(Qrvdefine::TRaveUnits Value);
	virtual Qrvdefine::TRaveUnits __fastcall GetTop(void);
	virtual void __fastcall SetTop(Qrvdefine::TRaveUnits Value);
	virtual Qrvdefine::TRaveUnits __fastcall GetWidth(void);
	virtual void __fastcall SetWidth(Qrvdefine::TRaveUnits Value);
	virtual Qrvdefine::TRaveUnits __fastcall GetHeight(void);
	virtual void __fastcall SetHeight(Qrvdefine::TRaveUnits Value);
	Qrvdefine::TRaveUnits __fastcall GetRight(void);
	void __fastcall SetRight(Qrvdefine::TRaveUnits Value);
	Qrvdefine::TRaveUnits __fastcall GetBottom(void);
	void __fastcall SetBottom(Qrvdefine::TRaveUnits Value);
	virtual Qrvdefine::TRaveUnits __fastcall GetPageLeft(void);
	virtual Qrvdefine::TRaveUnits __fastcall GetPageTop(void);
	virtual Qrvdefine::TRaveUnits __fastcall GetPageRight(void);
	virtual Qrvdefine::TRaveUnits __fastcall GetPageBottom(void);
	void __fastcall SetBoundsRect(const Qrvdefine::TRaveRect &Rect);
	Qrvdefine::TRaveRect __fastcall GetBoundsRect();
	void __fastcall SetAnchor(Qrvdefine::TRaveAnchor Value);
	virtual void __fastcall InitData(void);
	virtual void __fastcall DoneData(void);
	virtual void __fastcall SaveDesigned(void);
	virtual void __fastcall RestoreDesigned(void);
	virtual void __fastcall SaveAdjusted(void);
	virtual void __fastcall RestoreAdjusted(void);
	Qrvdefine::TRaveVAnchor __fastcall GetVAnchor(void);
	Qrvdefine::TRaveHAnchor __fastcall GetHAnchor(void);
	
public:
	__fastcall virtual TRaveControl(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveControl(void);
	void __fastcall Reflect(void);
	virtual void __fastcall CreatePips(void);
	virtual void __fastcall UpdatePips(void);
	virtual void __fastcall PipSize(TRavePip* RavePip, Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y);
	virtual bool __fastcall IsSelectPoint(const Qrvdefine::TRavePoint &Point);
	virtual bool __fastcall InSelectRect(const Qrvdefine::TRaveRect &Rect);
	/* virtual class method */ virtual void __fastcall ModifyRect(TMetaClass* vmt, Types::TPoint &P1, Types::TPoint &P2, Byte PipIndex);
	virtual void __fastcall PaintAll(Qgraphics::TCanvas* Canvas);
	virtual void __fastcall Paint(Qgraphics::TCanvas* Canvas);
	bool __fastcall FindControlAt(TRaveControl* &AControl, const Qrvdefine::TRavePoint &Point);
	bool __fastcall FindContainerAt(TRaveControl* &Container, const Qrvdefine::TRavePoint &Point, TMetaClass* NewChild);
	/* virtual class method */ virtual void __fastcall ModifyRaveRect(TMetaClass* vmt, Qrvdefine::TRavePoint &P1, Qrvdefine::TRavePoint &P2);
	void __fastcall SetPos(Qrvdefine::TRaveUnits ALeft, Qrvdefine::TRaveUnits ATop);
	virtual void __fastcall SetSize(const Qrvdefine::TRavePoint &P1, const Qrvdefine::TRavePoint &P2);
	virtual void __fastcall Resize(void);
	virtual bool __fastcall AllowPrint(void);
	virtual void __fastcall PrintAll(Qrpbase::TBaseReport* Report);
	virtual Qrvdefine::TRaveUnits __fastcall HeightToPrint(void);
	__property Qrvdefine::TRaveUnits xDesLeft = {read=FDesLeftx};
	__property Qrvdefine::TRaveUnits xDesTop = {read=FDesTopx};
	__property Qrvdefine::TRaveUnits xDesWidth = {read=FDesWidthx};
	__property Qrvdefine::TRaveUnits xDesHeight = {read=FDesHeightx};
	__property Qrvdefine::TRaveUnits AdjLeft = {read=FAdjLeft};
	__property Qrvdefine::TRaveUnits AdjTop = {read=FAdjTop};
	__property Qrvdefine::TRaveUnits AdjWidth = {read=FAdjWidth};
	__property Qrvdefine::TRaveUnits AdjHeight = {read=FAdjHeight};
	__property Qrvdefine::TRaveVAnchor VAnchor = {read=GetVAnchor, nodefault};
	__property Qrvdefine::TRaveHAnchor HAnchor = {read=GetHAnchor, nodefault};
	__property Qcontrols::TCursor Cursor = {read=FCursor, write=FCursor, default=0};
	__property TRaveFont* FontAttr[Qrvdefine::TFontAttribute Index] = {read=GetFontAttr, write=SetFontAttr};
	__property Qrpdefine::TPrintJustify FontJustify = {read=GetFontJustify, write=SetFontJustify, nodefault};
	__property Qgraphics::TColor BackColor = {read=GetBackColor, write=SetBackColor, nodefault};
	__property Qgraphics::TColor ForeColor = {read=GetForeColor, write=SetForeColor, nodefault};
	__property Qrvdefine::TRaveFillStyle FillStyle = {read=GetFillStyle, write=SetFillStyle, nodefault};
	__property AnsiString Text = {read=GetText, write=SetText};
	__property double LineWidth = {read=GetLineWidth, write=SetLineWidth};
	__property Qrvdefine::TLineWidthType LineWidthType = {read=GetLineWidthType, write=SetLineWidthType, nodefault};
	__property Qgraphics::TPenStyle LineStyle = {read=GetLineStyle, write=SetLineStyle, nodefault};
	__property Qrvdefine::TRaveRect BoundsRect = {read=GetBoundsRect, write=SetBoundsRect};
	__property Qrvdefine::TRaveUnits Left = {read=GetLeft, write=SetLeft};
	__property Qrvdefine::TRaveUnits Right = {read=GetRight, write=SetRight};
	__property Qrvdefine::TRaveUnits Top = {read=GetTop, write=SetTop};
	__property Qrvdefine::TRaveUnits Bottom = {read=GetBottom, write=SetBottom};
	__property Qrvdefine::TRaveUnits Width = {read=GetWidth, write=SetWidth};
	__property Qrvdefine::TRaveUnits Height = {read=GetHeight, write=SetHeight};
	__property Qrvdefine::TRavePoint MirrorPoint = {read=FMirrorPoint};
	
__published:
	__property Qrvdefine::TRaveAnchor Anchor = {read=FAnchor, write=SetAnchor, default=0};
	__property TRaveControl* Mirror = {read=FMirror, write=SetMirror, default=0};
	__property Qrvdefine::TDisplayOn DisplayOn = {read=FDisplayOn, write=FDisplayOn, default=0};
};


typedef TMetaClass*TRaveControlClass;

class DELPHICLASS TRaveContainerControl;
class PASCALIMPLEMENTATION TRaveContainerControl : public TRaveControl 
{
	typedef TRaveControl inherited;
	
private:
	Classes::TList* ChildList;
	
protected:
	virtual void __fastcall DestroyChildren(void);
	virtual void __fastcall AddChild(TRaveComponent* Value);
	virtual void __fastcall RemoveChild(TRaveComponent* Value);
	virtual TRaveComponent* __fastcall GetChild(int Index);
	virtual int __fastcall GetChildCount(void);
	virtual void __fastcall SetDepth(TRaveComponent* Control, int Adjustment);
	
public:
	__fastcall virtual TRaveContainerControl(Classes::TComponent* AOwner);
public:
	#pragma option push -w-inl
	/* TRaveControl.Destroy */ inline __fastcall virtual ~TRaveContainerControl(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveDataObject;
class PASCALIMPLEMENTATION TRaveDataObject : public TRaveProjectItem 
{
	typedef TRaveProjectItem inherited;
	
public:
	#pragma option push -w-inl
	/* TRaveProjectItem.Create */ inline __fastcall virtual TRaveDataObject(Classes::TComponent* AOwner) : TRaveProjectItem(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveProjectItem.Destroy */ inline __fastcall virtual ~TRaveDataObject(void) { }
	#pragma option pop
	
};


typedef TMetaClass*TRaveDataObjectClass;

class PASCALIMPLEMENTATION TRavePip : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Types::TRect __fastcall GetScreenRect();
	
public:
	Byte Index;
	TRaveControl* Control;
	TRaveDesigner* Designer;
	Qcontrols::TCursor Cursor;
	Qrvdefine::TRaveUnits PageX;
	Qrvdefine::TRaveUnits PageY;
	#pragma pack(push, 1)
	Types::TPoint StartSizePoint;
	#pragma pack(pop)
	
	Qrvdefine::TRaveRect OrigRect;
	Qrvdefine::TRavePoint OrigPoint;
	__fastcall TRavePip(Byte AIndex, TRaveControl* AControl, TRaveDesigner* ADesigner, Qcontrols::TCursor ACursor, Qrvdefine::TRaveUnits AX, Qrvdefine::TRaveUnits AY);
	__fastcall virtual ~TRavePip(void);
	void __fastcall Reset(void);
	void __fastcall Move(Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y);
	void __fastcall Draw(Qgraphics::TCanvas* Canvas);
	__property Types::TRect ScreenRect = {read=GetScreenRect};
};


#pragma option push -b-
enum TValueKind { vkID, vkIndex, vkNegIndex, vkInteger, vkChar, vkFloat, vkString, vkSet, vkPropList, vkComponentList, vkPersistentList };
#pragma option pop

class DELPHICLASS TRaveReader;
typedef void __fastcall (__closure *TRaveReaderProc)(TRaveReader* Reader);

class DELPHICLASS TRaveWriter;
typedef void __fastcall (__closure *TRaveWriterProc)(TRaveWriter* Writer);

class PASCALIMPLEMENTATION TRaveFiler : public Classes::TFiler 
{
	typedef Classes::TFiler inherited;
	
protected:
	Qrvutil::TStreamHelper* FStreamHelper;
	Classes::TStringList* FOverrideList;
	TRaveComponent* FRootOwner;
	bool FIsReader;
	
public:
	__fastcall TRaveFiler(Qrvutil::TStreamHelper* AStreamHelper);
	__fastcall virtual ~TRaveFiler(void);
	void __fastcall OverrideProperty(AnsiString Name, TRaveReaderProc ReadData, TRaveWriterProc WriteData);
	bool __fastcall PropertyOverride(AnsiString Name, TRaveReaderProc &ReadData, TRaveWriterProc &WriteData);
	virtual void __fastcall DefineProperty(const AnsiString Name, Classes::TReaderProc ReadData, Classes::TWriterProc WriteData, bool HasData);
	virtual void __fastcall DefineBinaryProperty(const AnsiString Name, Classes::TStreamProc ReadData, Classes::TStreamProc WriteData, bool HasData);
	virtual void __fastcall FlushBuffer(void);
	__property bool IsReader = {read=FIsReader, nodefault};
	__property Classes::TStringList* OverrideList = {read=FOverrideList};
	__property TRaveComponent* RootOwner = {read=FRootOwner};
	__property Qrvutil::TStreamHelper* StreamHelper = {read=FStreamHelper};
};


class PASCALIMPLEMENTATION TRaveWriter : public TRaveFiler 
{
	typedef TRaveFiler inherited;
	
protected:
	Classes::TPersistent* DefineInstance;
	
public:
	__fastcall TRaveWriter(Qrvutil::TStreamHelper* AStreamHelper);
	__fastcall virtual ~TRaveWriter(void);
	virtual void __fastcall DefineProperty(const AnsiString Name, Classes::TReaderProc ReadData, Classes::TWriterProc WriteData, bool HasData);
	virtual void __fastcall DefineBinaryProperty(const AnsiString Name, Classes::TStreamProc ReadData, Classes::TStreamProc WriteData, bool HasData);
	virtual void __fastcall FlushBuffer(void);
	void __fastcall WriteComponent(TRaveComponent* Value, bool DoChildren);
	void __fastcall WriteChildren(TRaveComponent* Instance);
	void __fastcall WritePropName(Typinfo::PPropInfo PropInfo);
	void __fastcall WriteProperties(Classes::TPersistent* Instance);
	void __fastcall WriteProperty(Classes::TPersistent* Instance, Typinfo::PPropInfo PropInfo);
	void __fastcall WriteID(AnsiString Value);
	void __fastcall WriteInteger(int Value);
	void __fastcall WriteChar(char Value);
	void __fastcall WriteFloat(Extended Value);
	void __fastcall WriteString(AnsiString Value);
};


class PASCALIMPLEMENTATION TRaveReader : public TRaveFiler 
{
	typedef TRaveFiler inherited;
	
protected:
	bool PropHandled;
	AnsiString PropName;
	bool FAddComponents;
	bool FCheckNames;
	int FAbsorbCount;
	TRaveComponent* FCurrentParent;
	TRaveComponent* FCurrentComp;
	TRaveProjectItem* FMasterProject;
	
public:
	__fastcall TRaveReader(Qrvutil::TStreamHelper* AStreamHelper);
	__fastcall virtual ~TRaveReader(void);
	virtual void __fastcall DefineProperty(const AnsiString Name, Classes::TReaderProc ReadData, Classes::TWriterProc WriteData, bool HasData);
	virtual void __fastcall DefineBinaryProperty(const AnsiString Name, Classes::TStreamProc ReadData, Classes::TStreamProc WriteData, bool HasData);
	virtual void __fastcall FlushBuffer(void);
	TRaveComponent* __fastcall ReadComponent(TRaveComponent* aOwner, TRaveComponent* aParent);
	void __fastcall ReadIntoComponent(TRaveComponent* Value);
	bool __fastcall AbsorbProperty(void);
	void __fastcall ReadProperties(Classes::TPersistent* Instance);
	void __fastcall ReadProperty(Classes::TPersistent* Instance, Typinfo::PPropInfo PropInfo);
	AnsiString __fastcall ReadID();
	int __fastcall ReadInteger(void);
	char __fastcall ReadChar(void);
	Extended __fastcall ReadFloat(void);
	AnsiString __fastcall ReadString();
	__property bool AddComponents = {read=FAddComponents, write=FAddComponents, nodefault};
	__property bool CheckNames = {read=FCheckNames, write=FCheckNames, nodefault};
	__property TRaveComponent* CurrentParent = {read=FCurrentParent, write=FCurrentParent};
	__property TRaveComponent* CurrentComp = {read=FCurrentComp, write=FCurrentComp};
	__property TRaveProjectItem* MasterProject = {read=FMasterProject, write=FMasterProject};
};


class PASCALIMPLEMENTATION TRaveFont : public Qgraphics::TFont 
{
	typedef Qgraphics::TFont inherited;
	
__published:
	__property Height  = {stored=false};
	__property Size  = {stored=true};
public:
	#pragma option push -w-inl
	/* TFont.Create */ inline __fastcall TRaveFont(void)/* overload */ : Qgraphics::TFont() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TFont.Destroy */ inline __fastcall virtual ~TRaveFont(void) { }
	#pragma option pop
	
};


class DELPHICLASS ERaveCompiler;
class PASCALIMPLEMENTATION ERaveCompiler : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall ERaveCompiler(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall ERaveCompiler(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall ERaveCompiler(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall ERaveCompiler(int Ident, const System::TVarRec * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall ERaveCompiler(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall ERaveCompiler(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall ERaveCompiler(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall ERaveCompiler(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~ERaveCompiler(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveStringEvent;
class PASCALIMPLEMENTATION TRaveStringEvent : public TRaveSimpleEvent 
{
	typedef TRaveSimpleEvent inherited;
	
protected:
	AnsiString FValue;
	
public:
	virtual void __fastcall DefineParams(void);
	virtual AnsiString __fastcall ParamSource();
	
__published:
	__property AnsiString Value = {read=FValue, write=FValue};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TRaveStringEvent(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveStringEvent(void) : TRaveSimpleEvent() { }
	#pragma option pop
	
};


class DELPHICLASS TRaveFloatEvent;
class PASCALIMPLEMENTATION TRaveFloatEvent : public TRaveSimpleEvent 
{
	typedef TRaveSimpleEvent inherited;
	
protected:
	Extended FValue;
	
public:
	virtual void __fastcall DefineParams(void);
	virtual AnsiString __fastcall ParamSource();
	
__published:
	__property Extended Value = {read=FValue, write=FValue};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TRaveFloatEvent(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveFloatEvent(void) : TRaveSimpleEvent() { }
	#pragma option pop
	
};


class DELPHICLASS TRaveCompileStatus;
class PASCALIMPLEMENTATION TRaveCompileStatus : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	int ErrorLine;
	int ErrorCol;
	TRaveEvent* ErrorEvent;
	AnsiString ErrorMsg;
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveCompileStatus(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRaveCompileStatus(void) { }
	#pragma option pop
	
};


typedef bool __fastcall (__closure *TIncludeFunc)(Classes::TComponent* Value, int Data);

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall (*RegisterRaveGroupProc)(const AnsiString GroupName, const AnsiString Description);
extern PACKAGE void __fastcall (*RegisterRaveComponentsProc)(const AnsiString GroupName, TMetaClass* * RaveClasses, const int RaveClasses_Size);
extern PACKAGE void __fastcall (*RegisterRaveClassesProc)(TMetaClass* * RaveClasses, const int RaveClasses_Size);
extern PACKAGE void __fastcall (*RegisterRaveFuncProc)(const void *Func, const AnsiString FuncName, const AnsiString ModuleName);
extern PACKAGE void __fastcall (*RaveNameChangeProc)(TRaveComponent* Component, AnsiString NewName);
extern PACKAGE void __fastcall (*AddComponentProc)(TRaveComponent* Component);
extern PACKAGE void __fastcall (*DeleteComponentProc)(TRaveComponent* Component);
extern PACKAGE void __fastcall (*OpenProjectItemProc)(TRaveProjectItem* Item, bool Open);
extern PACKAGE void __fastcall (*RegisterRavePropertiesProc)(const TMetaClass* ComponentClass, const AnsiString Level1Props, const AnsiString Level2Props, const AnsiString DevProps, const AnsiString HideProps);
extern PACKAGE void __fastcall (*GetComponentNamesProc)(TIncludeFunc IncludeFunc, TRaveComponent* Instance, int Data, Classes::TStrings* NameList);
extern PACKAGE void __fastcall (*ModifiedProc)(bool Refresh);
extern PACKAGE void __fastcall (*RegisterFuncProc)(const void *Proc, const AnsiString FuncName, const AnsiString ModuleName);
extern PACKAGE void __fastcall (*UnregisterFuncProc)(AnsiString FuncName, AnsiString ModuleName);
extern PACKAGE void __fastcall (*EnumRaveCompilersProc)(Classes::TStrings* SyntaxNameList);
extern PACKAGE TRaveComponent* ContinuationComponent;
extern PACKAGE void __fastcall (*SetPropDescProc)(const TMetaClass* CompClass, const AnsiString PropName, const AnsiString PropDesc);
extern PACKAGE AnsiString __fastcall (*GetPropDescProc)(TMetaClass* CompClass, AnsiString PropName);
extern PACKAGE TRaveModule* __fastcall (*CreateRaveModuleProc)(Classes::TStream* Stream, TRaveProjectItem* Owner, TRaveProjectItem* ProjectItem);
extern PACKAGE void __fastcall (*PrepareRaveModuleProc)(TRaveProjectItem* ProjectItem);
extern PACKAGE void __fastcall (*RegisterRaveModuleClassesProc)(const AnsiString ModuleName, TMetaClass* * RaveClasses, const int RaveClasses_Size);
extern PACKAGE AnsiString __fastcall (*FindRaveClassModuleProc)(AnsiString ClassName);
extern PACKAGE bool DefineCompiles;
extern PACKAGE void __fastcall (*RegisterRaveDataObjectProc)(const AnsiString DataObjectName, const TMetaClass* DataObjectClass);
extern PACKAGE void __fastcall (*CreateShortCutProc)(AnsiString Desc, Classes::TComponent* Item, AnsiString Name, Classes::TShortCut Key1, Classes::TShortCut Key2);
extern PACKAGE void __fastcall (*LoadModulesProc)(Qrvutil::TStreamHelper* StreamHelper);
extern PACKAGE void __fastcall (*SaveModulesProc)(Qrvutil::TStreamHelper* StreamHelper);
extern PACKAGE void __fastcall (*RegisterRaveModuleProc)(TRaveModule* Module);
extern PACKAGE void __fastcall (*CompileStatusProc)(TRaveCompileStatus* CompileStatus);
extern PACKAGE TRaveDesigner* CurrentDesigner;
extern PACKAGE TRaveModule* CompileModule;
extern PACKAGE int ErrorLine;
extern PACKAGE int ErrorCol;
extern PACKAGE TRaveEvent* ErrorEvent;
extern PACKAGE void __fastcall RaveRegister(void);
extern PACKAGE void __fastcall RegisterProc(AnsiString AKind, Sysutils::TProcedure AProc);
extern PACKAGE void __fastcall CallRegisters(AnsiString AKind);
extern PACKAGE void __fastcall RegisterRaveProperties(const TMetaClass* ComponentClass, const AnsiString Level1Props, const AnsiString Level2Props, const AnsiString DevProps, const AnsiString HideProps);
extern PACKAGE void __fastcall SetPropDesc(const TMetaClass* CompClass, const AnsiString PropName, const AnsiString PropDesc);
extern PACKAGE void __fastcall RegisterRaveGroup(const AnsiString GroupName, const AnsiString Description);
extern PACKAGE void __fastcall RegisterRaveComponents(const AnsiString GroupName, TMetaClass* * RaveClasses, const int RaveClasses_Size)/* overload */;
extern PACKAGE void __fastcall RegisterRaveComponents(const AnsiString GroupName, const AnsiString Description, const AnsiString ModuleName, TMetaClass* * RaveClasses, const int RaveClasses_Size)/* overload */;
extern PACKAGE void __fastcall RegisterRaveClasses(TMetaClass* * RaveClasses, const int RaveClasses_Size);
extern PACKAGE void __fastcall RegisterRaveModuleClasses(const AnsiString ModuleName, TMetaClass* * RaveClasses, const int RaveClasses_Size);
extern PACKAGE AnsiString __fastcall FindRaveClassModule(const AnsiString ClassName);
extern PACKAGE AnsiString __fastcall GetSupportedClass(TMetaClass* RaveClass);
extern PACKAGE void __fastcall RegisterRaveFunc(const void *Func, const AnsiString FuncName, const AnsiString ModuleName);
extern PACKAGE void __fastcall AddComponent(TRaveComponent* Component);
extern PACKAGE TRaveModule* __fastcall CreateRaveModule(Classes::TStream* Stream, TRaveProjectItem* Owner, TRaveProjectItem* ProjectItem);
extern PACKAGE void __fastcall RegisterRaveModule(TRaveModule* Module);
extern PACKAGE void __fastcall DeleteComponent(TRaveComponent* Component);
extern PACKAGE void __fastcall GetComponentNames(TIncludeFunc IncludeFunc, TRaveComponent* Instance, int Data, Classes::TStrings* NameList);
extern PACKAGE void __fastcall NotifyChanging(TRaveComponent* OldItem, TRaveComponent* NewItem);
extern PACKAGE void __fastcall EnumRaveCompilers(Classes::TStrings* SyntaxNameList);
extern PACKAGE void __fastcall RegisterFunc(const void *Proc, const AnsiString FuncName, const AnsiString ModuleName);
extern PACKAGE void __fastcall UnregisterFunc(AnsiString FuncName, AnsiString ModuleName);
extern PACKAGE void __fastcall RegisterRaveDataObject(const AnsiString DataObjectName, const TMetaClass* DataObjectClass);
extern PACKAGE void __fastcall RaveCreateShortCut(AnsiString Desc, Classes::TComponent* Item = (Classes::TComponent*)(0x0), AnsiString Name = "", Classes::TShortCut Key1 = (Classes::TShortCut)(0x0), Classes::TShortCut Key2 = (Classes::TShortCut)(0x0));
extern PACKAGE void __fastcall RaveCompileStatus(TRaveCompileStatus* CompileStatus);

}	/* namespace Qrvclass */
using namespace Qrvclass;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvClass
