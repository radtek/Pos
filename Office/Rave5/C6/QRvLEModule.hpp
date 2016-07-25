// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvLEModule.pas' rev: 6.00

#ifndef QRvLEModuleHPP
#define QRvLEModuleHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvDefine.hpp>	// Pascal unit
#include <QRvLEType.hpp>	// Pascal unit
#include <QRvLEID.hpp>	// Pascal unit
#include <QRvLEDefine.hpp>	// Pascal unit
#include <QRvClass.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvlemodule
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


class PASCALIMPLEMENTATION TRCPUModule : public Qrvclass::TRaveModule 
{
	typedef Qrvclass::TRaveModule inherited;
	
protected:
	Classes::TList* UsedModules;
	Classes::TStringList* ExternalList;
	Classes::TStringList* ForwardRefList;
	Classes::TList* Params;
	Word FMajorVersion;
	Word FMinorVersion;
	System::TDateTime FCompileDate;
	TScopeManager* FScope;
	Qrvletype::TTypeHelper* FTypeHelper;
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
	Qrvclass::TRaveProjectItem* FProjectItem;
	Qrvclass::TRaveProjectItem* FProjectManager;
	TRaveCompiler* FCompiler;
	TRCPUModule* FParentModule;
	TRCPUModule* __fastcall GetParentModule(void);
	void * __fastcall GetDataBuf(void);
	void __fastcall SetDataSize(int Value);
	virtual void __fastcall PostLoad(void);
	
public:
	__fastcall TRCPUModule(void);
	__fastcall virtual TRCPUModule(Classes::TStream* Stream, Qrvclass::TRaveProjectItem* aOwner, Qrvclass::TRaveProjectItem* aProjectItem);
	__fastcall virtual TRCPUModule(AnsiString FileName, Qrvclass::TRaveProjectItem* AOwner);
	__fastcall virtual ~TRCPUModule(void);
	virtual void __fastcall SaveToStream(Classes::TStream* Stream);
	void __fastcall SaveToFile(AnsiString FileName);
	virtual void __fastcall Changing(Qrvclass::TRaveModule* OldItem, Qrvclass::TRaveModule* NewItem);
	Qrvleid::TTypeID* __fastcall AddType(AnsiString ID, Qrvletype::TBaseType* BaseType);
	Qrvletype::TBaseType* __fastcall CreateType(Qrvletype::TBaseType* BaseType);
	Qrvleid::TVarID* __fastcall AddVar(AnsiString ID, Qrvletype::TBaseType* BaseType);
	Qrvleid::TOpID* __fastcall AddOp(AnsiString ID, Qrvledefine::TOpKind OpKind);
	Qrvleid::TFuncID* __fastcall AddFunc(AnsiString ID, Qrvletype::TBaseType* BaseType);
	Qrvleid::TConstID* __fastcall AddConst(AnsiString ID, Qrvletype::TBaseType* BaseType, void *Data);
	TRCPUModule* __fastcall AddModule(AnsiString ModuleName);
	void __fastcall AddExternal(AnsiString AID, int AOffset);
	Qrvleid::TBaseID* __fastcall FindInternalID(AnsiString ID);
	Qrvleid::TBaseID* __fastcall FindID(AnsiString ID);
	Qrvletype::TBaseType* __fastcall FindType(AnsiString ID);
	Qrvledefine::PExternalFunc __fastcall FindExternal(AnsiString ID);
	TRCPUModule* __fastcall IndexToModule(int Index);
	int __fastcall ModuleToIndex(TRCPUModule* Module);
	Qrvletype::TBaseType* __fastcall IndexToType(int Index);
	int __fastcall TypeToIndex(Qrvletype::TBaseType* BaseType);
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
	void * __fastcall GetDataPtr(Qrvleid::TVarID* VarID);
	virtual void __fastcall PushParam(void *Param);
	__property TScopeManager* Scope = {read=FScope};
	__property Qrvletype::TTypeHelper* TypeHelper = {read=FTypeHelper};
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
	__property Qrvclass::TRaveProjectItem* ProjectItem = {read=FProjectItem, write=FProjectItem};
	__property Qrvclass::TRaveProjectItem* ProjectManager = {read=FProjectManager, write=FProjectManager};
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
	Qrvletype::TTypeManager* FTypeManager;
	Qrvleid::TIDManager* FIDManager;
	Classes::TList* FObjectScope;
	
public:
	__fastcall TScopeManager(TScopeManager* AParent, TRCPUModule* AOwner);
	__fastcall virtual ~TScopeManager(void);
	void __fastcall LoadFromStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	void __fastcall SaveToStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	Qrvleid::TBaseID* __fastcall FindID(AnsiString ID);
	Qrvleid::TTypeID* __fastcall AddType(AnsiString ID, Qrvletype::TBaseType* BaseType);
	Qrvleid::TVarID* __fastcall AddVar(AnsiString ID, Qrvletype::TBaseType* BaseType);
	Qrvleid::TOpID* __fastcall AddOp(AnsiString ID, Qrvledefine::TOpKind OpKind);
	Qrvleid::TFuncID* __fastcall AddFunc(AnsiString ID, Qrvletype::TBaseType* BaseType);
	Qrvleid::TConstID* __fastcall AddConst(AnsiString ID, Qrvletype::TBaseType* BaseType, void *Data);
	void __fastcall ClearData(void);
	void __fastcall PushObjectScope(Qrvleid::TVarID* AObject);
	void __fastcall PopObjectScope(void);
	Qrvleid::TVarID* __fastcall FindObjectScope(AnsiString ID);
	__property TRCPUModule* Owner = {read=FOwner};
	__property TScopeManager* Parent = {read=FParent};
	__property Qrvletype::TTypeManager* TypeManager = {read=FTypeManager};
	__property Qrvleid::TIDManager* IDManager = {read=FIDManager};
	__property int DataSize = {read=DataIdx, write=DataIdx, nodefault};
};


class DELPHICLASS TModuleTypeHelper;
class PASCALIMPLEMENTATION TModuleTypeHelper : public Qrvletype::TTypeHelper 
{
	typedef Qrvletype::TTypeHelper inherited;
	
public:
	TRCPUModule* FModule;
	__fastcall TModuleTypeHelper(TRCPUModule* AModule);
	virtual Qrvletype::TBaseType* __fastcall IndexToType(int ModuleIndex, int TypeIndex);
	virtual void __fastcall TypeToIndex(Qrvletype::TBaseType* BaseType, int &ModuleIndex, int &TypeIndex);
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

}	/* namespace Qrvlemodule */
using namespace Qrvlemodule;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvLEModule
