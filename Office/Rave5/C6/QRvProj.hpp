// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvProj.pas' rev: 6.00

#ifndef QRvProjHPP
#define QRvProjHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRvSecurity.hpp>	// Pascal unit
#include <QRpDevice.hpp>	// Pascal unit
#include <QRpSystem.hpp>	// Pascal unit
#include <QRpBase.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <QRvData.hpp>	// Pascal unit
#include <QRvClass.hpp>	// Pascal unit
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <QComCtrls.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvproj
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TTagKind { tkProject, tkExport, tkDataView, tkPage, tkReport, tkReportPage, tkDataObject };
#pragma option pop

typedef void __fastcall (__closure *TImportConflictEvent)(Qrvclass::TRaveProjectItem* CurrentItem, AnsiString &ImportName);

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
	Qrvclass::TRaveModule* __fastcall GetModule(AnsiString ModuleName);
	Qrvclass::TRaveModule* __fastcall FindModule(AnsiString ModuleName);
	void __fastcall LoadModules(Qrvutil::TStreamHelper* StreamHelper);
	void __fastcall SaveModules(Qrvutil::TStreamHelper* StreamHelper);
	void __fastcall ReleaseModule(Qrvclass::TRaveModule* AModule);
	__property TRaveProjectManager* ProjectManager = {read=FProjectManager};
};


class PASCALIMPLEMENTATION TRaveProjectManager : public Qrvclass::TRaveProjectItem 
{
	typedef Qrvclass::TRaveProjectItem inherited;
	
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
	Qrpdefine::TPrintUnits FUnits;
	double FUnitsFactor;
	bool FStreamParamValues;
	bool IsLoading;
	TRaveProjectManager* FMasterProject;
	bool FNoDesigner;
	Qrvsecurity::TRaveBaseSecurity* FSecurityControl;
	AnsiString FAdminPassword;
	Qrpbase::TBaseReport* FBaseReport;
	TRaveModuleManager* FModuleManager;
	virtual void __fastcall SetDepth(Qrvclass::TRaveComponent* Control, int Adjustment);
	void __fastcall SetCategories(Classes::TStrings* Value);
	void __fastcall SetUnits(Qrpdefine::TPrintUnits Value);
	void __fastcall SetUnitsFactor(double Value);
	void __fastcall SetDataChanged(bool Value);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	void __fastcall ReadParamValues(Classes::TReader* Reader);
	void __fastcall WriteParamValues(Classes::TWriter* Writer);
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRaveProjectManager(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveProjectManager(void);
	void __fastcall SetIncluded(TRaveReport* AReport);
	void __fastcall ReportToStream(TRaveReport* AReport, Classes::TStream* AStream);
	TRaveReport* __fastcall FindReport(const AnsiString AName, const bool AIsFullName);
	void __fastcall AddForwardRef(AnsiString AName, Qrvclass::TRaveComponent* AOwner, Classes::TPersistent* AInstance, Typinfo::PPropInfo APropInfo, int AParam);
	void __fastcall AddRenameRef(AnsiString OrigName, Qrvclass::TRaveComponent* NewInstance);
	void __fastcall FreeForwardRefs(void);
	void __fastcall New(void);
	void __fastcall RemoveForwardRefs(Qrvclass::TRaveComponent* Target);
	void __fastcall Save(void);
	void __fastcall Load(void);
	void __fastcall Unload(void);
	void __fastcall Clear(void);
	virtual void __fastcall LoadFromStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	void __fastcall LoadFromStream(Classes::TStream* Stream);
	void __fastcall SaveToStream(Classes::TStream* Stream);
	void __fastcall ExportProject(AnsiString ExportFileName, Classes::TList* Items);
	bool __fastcall ImportProject(AnsiString ImportFileName, bool AutoReplace);
	void __fastcall DeactivateReport(void);
	void __fastcall ActivateReport(TRaveReport* Report);
	Qrvclass::TRaveComponent* __fastcall FindRaveComponent(AnsiString Name, Qrvclass::TRaveComponent* DefRoot);
	AnsiString __fastcall GetUniqueName(AnsiString BaseName, Qrvclass::TRaveComponent* NameOwner, bool UseCurrent);
	TRaveReport* __fastcall NewReport(void);
	Qrvclass::TRavePage* __fastcall NewGlobalPage(void);
	Qrvclass::TRaveDataObject* __fastcall NewDataObject(TMetaClass* DataObjectClass);
	void __fastcall DeleteItem(Qrvclass::TRaveProjectItem* Item, bool Notify);
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
	__property Qrpbase::TBaseReport* BaseReport = {read=FBaseReport, write=FBaseReport};
	__property TRaveModuleManager* ModuleManager = {read=FModuleManager};
	
__published:
	__property AnsiString AdminPassword = {read=FAdminPassword, write=FAdminPassword};
	__property Classes::TStrings* Categories = {read=FCategories, write=SetCategories};
	__property Parameters ;
	__property PIVars ;
	__property Qrvsecurity::TRaveBaseSecurity* SecurityControl = {read=FSecurityControl, write=FSecurityControl};
	__property Qrpdefine::TPrintUnits Units = {read=FUnits, write=SetUnits, stored=false, nodefault};
	__property double UnitsFactor = {read=FUnitsFactor, write=SetUnitsFactor};
};


class PASCALIMPLEMENTATION TRaveReport : public Qrvclass::TRaveProjectItem 
{
	typedef Qrvclass::TRaveProjectItem inherited;
	
protected:
	AnsiString FCategory;
	AnsiString FLastActivePage;
	Classes::TNotifyEvent SaveOnPrint;
	Classes::TNotifyEvent SaveBeforePrint;
	Classes::TNotifyEvent SaveAfterPrint;
	Classes::TList* LoadedList;
	Classes::TStrings* FixUpList;
	Qrvclass::TRavePage* FFirstPage;
	int FCopies;
	Qrvclass::TRaveComponentList* FPageList;
	bool FAlwaysGenerate;
	Qrvdefine::TRavePrinterCollate FCollate;
	Qrvdefine::TRavePrinterDuplex FDuplex;
	AnsiString FPrinter;
	Qrvdefine::TRavePrinterResolution FResolution;
	int FMaxPages;
	Qrvsecurity::TRaveBaseSecurity* FSecurityControl;
	TRaveProjectManager* FExecProject;
	void __fastcall SetPageList(Qrvclass::TRaveComponentList* Value);
	int __fastcall GetLoadedCount(void);
	Qrvclass::TRavePage* __fastcall GetLoaded(int Index);
	void __fastcall RSPrint(System::TObject* Sender);
	void __fastcall RSBeforePrint(System::TObject* Sender);
	void __fastcall RSAfterPrint(System::TObject* Sender);
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRaveReport(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveReport(void);
	virtual void __fastcall SaveToStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall LoadFromStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall Open(void);
	virtual void __fastcall Close(void);
	Qrvclass::TRavePage* __fastcall NewPage(void);
	void __fastcall LoadPage(Qrvclass::TRavePage* Page);
	void __fastcall UnloadPage(Qrvclass::TRavePage* Page);
	void __fastcall ProcessLoaded(void);
	void __fastcall Execute(Qrpdefine::TRpComponent* Engine);
	void __fastcall InternalExecute(Qrpdefine::TRpComponent* Engine);
	__property int LoadedPageCount = {read=GetLoadedCount, nodefault};
	__property Qrvclass::TRavePage* LoadedPage[int Index] = {read=GetLoaded};
	__property AnsiString LastActivePage = {read=FLastActivePage, write=FLastActivePage};
	__property TRaveProjectManager* ExecProject = {read=FExecProject};
	
__published:
	__property bool AlwaysGenerate = {read=FAlwaysGenerate, write=FAlwaysGenerate, default=0};
	__property AnsiString Category = {read=FCategory, write=FCategory};
	__property int Copies = {read=FCopies, write=FCopies, default=0};
	__property Qrvclass::TRavePage* FirstPage = {read=FFirstPage, write=FFirstPage, default=0};
	__property Qrvclass::TRaveComponentList* PageList = {read=FPageList, write=SetPageList};
	__property Qrvdefine::TRavePrinterCollate Collate = {read=FCollate, write=FCollate, default=2};
	__property Qrvdefine::TRavePrinterDuplex Duplex = {read=FDuplex, write=FDuplex, default=3};
	__property int MaxPages = {read=FMaxPages, write=FMaxPages, default=0};
	__property Parameters ;
	__property PIVars ;
	__property AnsiString Printer = {read=FPrinter, write=FPrinter};
	__property Qrvdefine::TRavePrinterResolution Resolution = {read=FResolution, write=FResolution, default=4};
	__property Qrvsecurity::TRaveBaseSecurity* SecurityControl = {read=FSecurityControl, write=FSecurityControl};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRaveProjectManager* ProjectManager;
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Qrvproj */
using namespace Qrvproj;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvProj
