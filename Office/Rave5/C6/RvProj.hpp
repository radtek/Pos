// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvProj.pas' rev: 6.00

#ifndef RvProjHPP
#define RvProjHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvSecurity.hpp>	// Pascal unit
#include <RpDevice.hpp>	// Pascal unit
#include <RpSystem.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <RvData.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <RvUtil.hpp>	// Pascal unit
#include <RvDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <ComCtrls.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvproj
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TTagKind { tkProject, tkExport, tkDataView, tkPage, tkReport, tkReportPage, tkDataObject };
#pragma option pop

typedef void __fastcall (__closure *TImportConflictEvent)(Rvclass::TRaveProjectItem* CurrentItem, AnsiString &ImportName);

class DELPHICLASS TRaveReport;
class DELPHICLASS TRaveProjectManager;
class DELPHICLASS TRaveModuleManager;
class PASCALIMPLEMENTATION TRaveModuleManager : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Classes::TList* ModuleList;
	TRaveProjectManager* FProjectManager;
	int __fastcall ModuleIndex(AnsiString ModuleName);
	int __fastcall LoadModule(AnsiString ModuleName);
	
public:
	__fastcall TRaveModuleManager(TRaveProjectManager* AProjectManager);
	__fastcall virtual ~TRaveModuleManager(void);
	Rvclass::TRaveModule* __fastcall GetModule(AnsiString ModuleName);
	Rvclass::TRaveModule* __fastcall FindModule(AnsiString ModuleName);
	void __fastcall LoadModules(Rvutil::TStreamHelper* StreamHelper);
	void __fastcall SaveModules(Rvutil::TStreamHelper* StreamHelper);
	void __fastcall ReleaseModule(Rvclass::TRaveModule* AModule);
	__property TRaveProjectManager* ProjectManager = {read=FProjectManager};
};


class PASCALIMPLEMENTATION TRaveProjectManager : public Rvclass::TRaveProjectItem 
{
	typedef Rvclass::TRaveProjectItem inherited;
	
protected:
	Classes::TStringList* ForwardRefList;
	Classes::TStringList* RenameRefList;
	AnsiString LastActiveReport;
	Classes::TList* FReportList;
	Classes::TList* FGlobalPageList;
	Classes::TList* FDataObjectList;
	TRaveReport* FActiveReport;
	bool FSaved;
	AnsiString FFileName;
	AnsiString Signature;
	bool FDataChanged;
	bool FPrinting;
	Classes::TList* ExportList;
	bool Importing;
	bool ImportReplace;
	TImportConflictEvent FOnImportConflict;
	int FVersion;
	Classes::TStrings* FCategories;
	Classes::TStringList* Params;
	Rpdefine::TPrintUnits FUnits;
	double FUnitsFactor;
	bool FStreamParamValues;
	bool IsLoading;
	TRaveProjectManager* FMasterProject;
	bool FNoDesigner;
	Rvsecurity::TRaveBaseSecurity* FSecurityControl;
	AnsiString FAdminPassword;
	Rpbase::TBaseReport* FBaseReport;
	TRaveModuleManager* FModuleManager;
	virtual void __fastcall SetDepth(Rvclass::TRaveComponent* Control, int Adjustment);
	void __fastcall SetCategories(Classes::TStrings* Value);
	void __fastcall SetUnits(Rpdefine::TPrintUnits Value);
	void __fastcall SetUnitsFactor(double Value);
	void __fastcall SetDataChanged(bool Value);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	void __fastcall ReadParamValues(Classes::TReader* Reader);
	void __fastcall WriteParamValues(Classes::TWriter* Writer);
	virtual void __fastcall Changing(Rvclass::TRaveComponent* OldItem, Rvclass::TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRaveProjectManager(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveProjectManager(void);
	void __fastcall SetIncluded(TRaveReport* AReport);
	void __fastcall ReportToStream(TRaveReport* AReport, Classes::TStream* AStream);
	TRaveReport* __fastcall FindReport(const AnsiString AName, const bool AIsFullName);
	void __fastcall AddForwardRef(AnsiString AName, Rvclass::TRaveComponent* AOwner, Classes::TPersistent* AInstance, Typinfo::PPropInfo APropInfo, int AParam);
	void __fastcall AddRenameRef(AnsiString OrigName, Rvclass::TRaveComponent* NewInstance);
	void __fastcall FreeForwardRefs(void);
	void __fastcall New(void);
	void __fastcall RemoveForwardRefs(Rvclass::TRaveComponent* Target);
	void __fastcall Save(void);
	void __fastcall Load(void);
	void __fastcall Unload(void);
	void __fastcall Clear(void);
	virtual void __fastcall LoadFromStreamHelper(Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Rvutil::TStreamHelper* StreamHelper);
	void __fastcall LoadFromStream(Classes::TStream* Stream);
	void __fastcall SaveToStream(Classes::TStream* Stream);
	void __fastcall ExportProject(AnsiString ExportFileName, Classes::TList* Items);
	bool __fastcall ImportProject(AnsiString ImportFileName, bool AutoReplace);
	void __fastcall DeactivateReport(void);
	void __fastcall ActivateReport(TRaveReport* Report);
	Rvclass::TRaveComponent* __fastcall FindRaveComponent(AnsiString Name, Rvclass::TRaveComponent* DefRoot);
	AnsiString __fastcall GetUniqueName(AnsiString BaseName, Rvclass::TRaveComponent* NameOwner, bool UseCurrent);
	TRaveReport* __fastcall NewReport(void);
	Rvclass::TRavePage* __fastcall NewGlobalPage(void);
	Rvclass::TRaveDataObject* __fastcall NewDataObject(TMetaClass* DataObjectClass);
	void __fastcall DeleteItem(Rvclass::TRaveProjectItem* Item, bool Notify);
	void __fastcall SetParam(AnsiString Param, AnsiString Value);
	AnsiString __fastcall GetParam(AnsiString Param);
	void __fastcall ClearParams(void);
	void __fastcall ClearChanged(void);
	bool __fastcall Compile(void);
	__property Classes::TList* ReportList = {read=FReportList};
	__property Classes::TList* GlobalPageList = {read=FGlobalPageList};
	__property Classes::TList* DataObjectList = {read=FDataObjectList};
	__property TRaveReport* ActiveReport = {read=FActiveReport};
	__property bool Saved = {read=FSaved, write=FSaved, nodefault};
	__property bool DataChanged = {read=FDataChanged, write=SetDataChanged, nodefault};
	__property bool Printing = {read=FPrinting, nodefault};
	__property int Version = {read=FVersion, nodefault};
	__property TImportConflictEvent OnImportConflict = {read=FOnImportConflict, write=FOnImportConflict};
	__property bool StreamParamValues = {read=FStreamParamValues, write=FStreamParamValues, nodefault};
	__property AnsiString FileName = {read=FFileName, write=FFileName, stored=false};
	__property TRaveProjectManager* MasterProject = {read=FMasterProject, write=FMasterProject};
	__property bool NoDesigner = {read=FNoDesigner, write=FNoDesigner, nodefault};
	__property Rpbase::TBaseReport* BaseReport = {read=FBaseReport, write=FBaseReport};
	__property TRaveModuleManager* ModuleManager = {read=FModuleManager};
	
__published:
	__property AnsiString AdminPassword = {read=FAdminPassword, write=FAdminPassword};
	__property Classes::TStrings* Categories = {read=FCategories, write=SetCategories};
	__property Parameters ;
	__property PIVars ;
	__property Rvsecurity::TRaveBaseSecurity* SecurityControl = {read=FSecurityControl, write=FSecurityControl};
	__property Rpdefine::TPrintUnits Units = {read=FUnits, write=SetUnits, stored=false, nodefault};
	__property double UnitsFactor = {read=FUnitsFactor, write=SetUnitsFactor};
};


class PASCALIMPLEMENTATION TRaveReport : public Rvclass::TRaveProjectItem 
{
	typedef Rvclass::TRaveProjectItem inherited;
	
protected:
	AnsiString FCategory;
	AnsiString FLastActivePage;
	Classes::TNotifyEvent SaveOnPrint;
	Classes::TNotifyEvent SaveBeforePrint;
	Classes::TNotifyEvent SaveAfterPrint;
	Classes::TList* LoadedList;
	Classes::TStrings* FixUpList;
	Rvclass::TRavePage* FFirstPage;
	int FCopies;
	Rvclass::TRaveComponentList* FPageList;
	bool FAlwaysGenerate;
	Rvdefine::TRavePrinterCollate FCollate;
	Rvdefine::TRavePrinterDuplex FDuplex;
	AnsiString FPrinter;
	Rvdefine::TRavePrinterResolution FResolution;
	int FMaxPages;
	Rvsecurity::TRaveBaseSecurity* FSecurityControl;
	TRaveProjectManager* FExecProject;
	void __fastcall SetPageList(Rvclass::TRaveComponentList* Value);
	int __fastcall GetLoadedCount(void);
	Rvclass::TRavePage* __fastcall GetLoaded(int Index);
	void __fastcall RSPrint(System::TObject* Sender);
	void __fastcall RSBeforePrint(System::TObject* Sender);
	void __fastcall RSAfterPrint(System::TObject* Sender);
	virtual void __fastcall Changing(Rvclass::TRaveComponent* OldItem, Rvclass::TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRaveReport(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveReport(void);
	virtual void __fastcall SaveToStreamHelper(Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall LoadFromStreamHelper(Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall Open(void);
	virtual void __fastcall Close(void);
	Rvclass::TRavePage* __fastcall NewPage(void);
	void __fastcall LoadPage(Rvclass::TRavePage* Page);
	void __fastcall UnloadPage(Rvclass::TRavePage* Page);
	void __fastcall ProcessLoaded(void);
	void __fastcall Execute(Rpdefine::TRpComponent* Engine);
	void __fastcall InternalExecute(Rpdefine::TRpComponent* Engine);
	__property int LoadedPageCount = {read=GetLoadedCount, nodefault};
	__property Rvclass::TRavePage* LoadedPage[int Index] = {read=GetLoaded};
	__property AnsiString LastActivePage = {read=FLastActivePage, write=FLastActivePage};
	__property TRaveProjectManager* ExecProject = {read=FExecProject};
	
__published:
	__property bool AlwaysGenerate = {read=FAlwaysGenerate, write=FAlwaysGenerate, default=0};
	__property AnsiString Category = {read=FCategory, write=FCategory};
	__property int Copies = {read=FCopies, write=FCopies, default=0};
	__property Rvclass::TRavePage* FirstPage = {read=FFirstPage, write=FFirstPage, default=0};
	__property Rvclass::TRaveComponentList* PageList = {read=FPageList, write=SetPageList};
	__property Rvdefine::TRavePrinterCollate Collate = {read=FCollate, write=FCollate, default=2};
	__property Rvdefine::TRavePrinterDuplex Duplex = {read=FDuplex, write=FDuplex, default=3};
	__property int MaxPages = {read=FMaxPages, write=FMaxPages, default=0};
	__property Parameters ;
	__property PIVars ;
	__property AnsiString Printer = {read=FPrinter, write=FPrinter};
	__property Rvdefine::TRavePrinterResolution Resolution = {read=FResolution, write=FResolution, default=4};
	__property Rvsecurity::TRaveBaseSecurity* SecurityControl = {read=FSecurityControl, write=FSecurityControl};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRaveProjectManager* ProjectManager;
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Rvproj */
using namespace Rvproj;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvProj
