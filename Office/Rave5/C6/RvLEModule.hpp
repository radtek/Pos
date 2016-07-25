// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvLEModule.pas' rev: 6.00

#ifndef RvLEModuleHPP
#define RvLEModuleHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpDefine.hpp>	// Pascal unit
#include <RvUtil.hpp>	// Pascal unit
#include <RvDefine.hpp>	// Pascal unit
#include <RvLEType.hpp>	// Pascal unit
#include <RvLEID.hpp>	// Pascal unit
#include <RvLEDefine.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvlemodule
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TScopeManager;
class DELPHICLASS TRCPUModule;
class DELPHICLASS TRaveCompiler;
class PASCALIMPLEMENTATION TRaveCompiler : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/* virtual class method */ virtual AnsiString __fastcall SyntaxName(TMetaClass* vmt);
	__fastcall virtual TRaveCompiler(void);
	virtual void __fastcall CompileStream(Classes::TStream* Stream, AnsiString FileName, TRCPUModule* AModule, bool Definition) = 0 ;
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRaveCompiler(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TRCPUModule : public Rvclass::TRaveModule 
{
	typedef Rvclass::TRaveModule inherited;
	
protected:
	Classes::TList* UsedModules;
	Classes::TStringList* ExternalList;
	Classes::TStringList* ForwardRefList;
	Classes::TList* Params;
	Word FMajorVersion;
	Word FMinorVersion;
	System::TDateTime FCompileDate;
	TScopeManager* FScope;
	Rvletype::TTypeHelper* FTypeHelper;
	void *FDataBuf;
	int FDataSize;
	void *FCodeBuf;
	int FCodeSize;
	int FStartCO;
	int FCreateCO;
	int FDestroyCO;
	int FStackSize;
	Classes::TMemoryStream* FSourceStream;
	Classes::TStrings* FEventList;
	Classes::TList* FSourceRefList;
	Rvclass::TRaveProjectItem* FProjectItem;
	Rvclass::TRaveProjectItem* FProjectManager;
	TRaveCompiler* FCompiler;
	TRCPUModule* FParentModule;
	TRCPUModule* __fastcall GetParentModule(void);
	void * __fastcall GetDataBuf(void);
	void __fastcall SetDataSize(int Value);
	virtual void __fastcall PostLoad(void);
	
public:
	__fastcall TRCPUModule(void);
	__fastcall virtual TRCPUModule(Classes::TStream* Stream, Rvclass::TRaveProjectItem* aOwner, Rvclass::TRaveProjectItem* aProjectItem);
	__fastcall virtual TRCPUModule(AnsiString FileName, Rvclass::TRaveProjectItem* AOwner);
	__fastcall virtual ~TRCPUModule(void);
	virtual void __fastcall SaveToStream(Classes::TStream* Stream);
	void __fastcall SaveToFile(AnsiString FileName);
	virtual void __fastcall Changing(Rvclass::TRaveModule* OldItem, Rvclass::TRaveModule* NewItem);
	Rvleid::TTypeID* __fastcall AddType(AnsiString ID, Rvletype::TBaseType* BaseType);
	Rvletype::TBaseType* __fastcall CreateType(Rvletype::TBaseType* BaseType);
	Rvleid::TVarID* __fastcall AddVar(AnsiString ID, Rvletype::TBaseType* BaseType);
	Rvleid::TOpID* __fastcall AddOp(AnsiString ID, Rvledefine::TOpKind OpKind);
	Rvleid::TFuncID* __fastcall AddFunc(AnsiString ID, Rvletype::TBaseType* BaseType);
	Rvleid::TConstID* __fastcall AddConst(AnsiString ID, Rvletype::TBaseType* BaseType, void *Data);
	TRCPUModule* __fastcall AddModule(AnsiString ModuleName);
	void __fastcall AddExternal(AnsiString AID, int AOffset);
	Rvleid::TBaseID* __fastcall FindInternalID(AnsiString ID);
	Rvleid::TBaseID* __fastcall FindID(AnsiString ID);
	Rvletype::TBaseType* __fastcall FindType(AnsiString ID);
	Rvledefine::PExternalFunc __fastcall FindExternal(AnsiString ID);
	TRCPUModule* __fastcall IndexToModule(int Index);
	int __fastcall ModuleToIndex(TRCPUModule* Module);
	Rvletype::TBaseType* __fastcall IndexToType(int Index);
	int __fastcall TypeToIndex(Rvletype::TBaseType* BaseType);
	void __fastcall CreateScopeLevel(void);
	void __fastcall FreeScopeLevel(void);
	virtual void __fastcall Execute(int Position, System::TObject* &SuspendState);
	void __fastcall Init(void);
	virtual void __fastcall BuildSource(void);
	virtual void __fastcall Compile(bool Definition);
	virtual void __fastcall EventConnect(void);
	virtual void __fastcall SetStringVar(AnsiString Name, AnsiString Value);
	virtual AnsiString __fastcall GetStringVar(AnsiString Name);
	virtual void __fastcall SetIntVar(AnsiString Name, int Value);
	virtual int __fastcall GetIntVar(AnsiString Name);
	virtual void __fastcall SetFloatVar(AnsiString Name, Extended Value);
	virtual Extended __fastcall GetFloatVar(AnsiString Name);
	void * __fastcall GetDataPtr(Rvleid::TVarID* VarID);
	virtual void __fastcall PushParam(void *Param);
	__property TScopeManager* Scope = {read=FScope};
	__property Rvletype::TTypeHelper* TypeHelper = {read=FTypeHelper};
	__property void * DataSeg = {read=GetDataBuf};
	__property int DataSize = {read=FDataSize, write=SetDataSize, nodefault};
	__property void * CodeSeg = {read=FCodeBuf, write=FCodeBuf};
	__property int CodeSize = {read=FCodeSize, write=FCodeSize, nodefault};
	__property int StartCO = {read=FStartCO, write=FStartCO, nodefault};
	__property int CreateCO = {read=FCreateCO, write=FCreateCO, nodefault};
	__property int DestroyCO = {read=FDestroyCO, write=FDestroyCO, nodefault};
	__property int StackSize = {read=FStackSize, write=FStackSize, nodefault};
	__property Classes::TMemoryStream* SourceStream = {read=FSourceStream, write=FSourceStream};
	__property Classes::TList* SourceRefList = {read=FSourceRefList, write=FSourceRefList};
	__property TRaveCompiler* Compiler = {read=FCompiler, write=FCompiler};
	__property Classes::TStrings* EventList = {read=FEventList, write=FEventList};
	__property Rvclass::TRaveProjectItem* ProjectItem = {read=FProjectItem, write=FProjectItem};
	__property Rvclass::TRaveProjectItem* ProjectManager = {read=FProjectManager, write=FProjectManager};
	__property TRCPUModule* ParentModule = {read=GetParentModule, write=FParentModule};
};


class PASCALIMPLEMENTATION TScopeManager : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	int DataIdx;
	
protected:
	TRCPUModule* FOwner;
	TScopeManager* FParent;
	Rvletype::TTypeManager* FTypeManager;
	Rvleid::TIDManager* FIDManager;
	Classes::TList* FObjectScope;
	
public:
	__fastcall TScopeManager(TScopeManager* AParent, TRCPUModule* AOwner);
	__fastcall virtual ~TScopeManager(void);
	void __fastcall LoadFromStreamHelper(Rvutil::TStreamHelper* StreamHelper);
	void __fastcall SaveToStreamHelper(Rvutil::TStreamHelper* StreamHelper);
	Rvleid::TBaseID* __fastcall FindID(AnsiString ID);
	Rvleid::TTypeID* __fastcall AddType(AnsiString ID, Rvletype::TBaseType* BaseType);
	Rvleid::TVarID* __fastcall AddVar(AnsiString ID, Rvletype::TBaseType* BaseType);
	Rvleid::TOpID* __fastcall AddOp(AnsiString ID, Rvledefine::TOpKind OpKind);
	Rvleid::TFuncID* __fastcall AddFunc(AnsiString ID, Rvletype::TBaseType* BaseType);
	Rvleid::TConstID* __fastcall AddConst(AnsiString ID, Rvletype::TBaseType* BaseType, void *Data);
	void __fastcall ClearData(void);
	void __fastcall PushObjectScope(Rvleid::TVarID* AObject);
	void __fastcall PopObjectScope(void);
	Rvleid::TVarID* __fastcall FindObjectScope(AnsiString ID);
	__property TRCPUModule* Owner = {read=FOwner};
	__property TScopeManager* Parent = {read=FParent};
	__property Rvletype::TTypeManager* TypeManager = {read=FTypeManager};
	__property Rvleid::TIDManager* IDManager = {read=FIDManager};
	__property int DataSize = {read=DataIdx, write=DataIdx, nodefault};
};


class DELPHICLASS TModuleTypeHelper;
class PASCALIMPLEMENTATION TModuleTypeHelper : public Rvletype::TTypeHelper 
{
	typedef Rvletype::TTypeHelper inherited;
	
public:
	TRCPUModule* FModule;
	__fastcall TModuleTypeHelper(TRCPUModule* AModule);
	virtual Rvletype::TBaseType* __fastcall IndexToType(int ModuleIndex, int TypeIndex);
	virtual void __fastcall TypeToIndex(Rvletype::TBaseType* BaseType, int &ModuleIndex, int &TypeIndex);
	__property TRCPUModule* Module = {read=FModule};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TModuleTypeHelper(void) { }
	#pragma option pop
	
};


typedef TMetaClass*TRaveCompilerClass;

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE Classes::TList* CompilerList;
extern PACKAGE void __fastcall AddRaveCompiler(TMetaClass* CompilerClass);
extern PACKAGE TRaveCompiler* __fastcall CreateRaveCompiler(AnsiString SyntaxName);

}	/* namespace Rvlemodule */
using namespace Rvlemodule;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvLEModule
