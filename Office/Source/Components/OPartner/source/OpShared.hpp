// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpShared.pas' rev: 6.00

#ifndef OpSharedHPP
#define OpSharedHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <OpEvents.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opshared
{
//-- type declarations -------------------------------------------------------
typedef HRESULT __stdcall (*TOpCoCreateInstanceExProc)(const GUID &clsid, System::_di_IInterface unkOuter, int dwClsCtx, Activex::PCoServerInfo ServerInfo, int dwCount, Activex::PMultiQIArray rgmqResults);

typedef void __fastcall (__closure *TOpOfficeEvent)(void);

class DELPHICLASS TOpNestedCollectionItem;
typedef void __fastcall (__closure *TOpNestedForEachProc)(TOpNestedCollectionItem* Item);

typedef bool __fastcall (__closure *TOpNestedFindItemProc)(void *Key, TOpNestedCollectionItem* Item);

class DELPHICLASS TOpOfficeComponent;
typedef void __fastcall (__closure *TOpOfficeConnectEvent)(TOpOfficeComponent* Instance, bool Connect);

#pragma option push -b-
enum TOpPropDirection { pdToServer, pdFromServer };
#pragma option pop

#pragma option push -b-
enum TOpCallType { ctMethod, ctProperty };
#pragma option pop

#pragma option push -b-
enum OpShared__1 { csConnecting, csDisconnecting, csInEvent };
#pragma option pop

typedef Set<OpShared__1, csConnecting, csInEvent>  TOpClientState;

#pragma option push -b-
enum TOpOfficeVersion { ovUnknown, ov97, ov98, ov2000, ovXP };
#pragma option pop

typedef void __fastcall (__closure *TOpGetInstanceEvent)(System::TObject* Sender, _di_IDispatch &Instance, const GUID &CLSID, const GUID &IID);

class DELPHICLASS EOpOfficeError;
class PASCALIMPLEMENTATION EOpOfficeError : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EOpOfficeError(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EOpOfficeError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EOpOfficeError(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EOpOfficeError(int Ident, const System::TVarRec * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EOpOfficeError(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EOpOfficeError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EOpOfficeError(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EOpOfficeError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EOpOfficeError(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpBaseComponent;
class PASCALIMPLEMENTATION TOpBaseComponent : public Classes::TComponent 
{
	typedef Classes::TComponent inherited;
	
private:
	AnsiString __fastcall GetVersion();
	void __fastcall SetVersion(const AnsiString Value);
	
__published:
	__property AnsiString Version = {read=GetVersion, write=SetVersion};
public:
	#pragma option push -w-inl
	/* TComponent.Create */ inline __fastcall virtual TOpBaseComponent(Classes::TComponent* AOwner) : Classes::TComponent(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TOpBaseComponent(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TOpOfficeComponent : public TOpBaseComponent 
{
	typedef TOpBaseComponent inherited;
	
private:
	TOpClientState FClientState;
	Classes::TList* FConnectListeners;
	Opevents::TOpEventAdapter* FEvents;
	WideString FMachineName;
	TOpPropDirection FPropDirection;
	bool FStreamedConnected;
	Classes::TNotifyEvent FOnOpConnect;
	Classes::TNotifyEvent FOnOpDisconnect;
	TOpGetInstanceEvent FOnGetInstance;
	void __fastcall CallConnectListeners(bool Connect);
	void __fastcall SetMachineName(const WideString Value);
	
protected:
	TOpOfficeVersion FOfficeVersion;
	virtual TOpOfficeVersion __fastcall ParseOfficeVersion(AnsiString VersionStr);
	virtual void __fastcall DoConnect(void) = 0 ;
	virtual void __fastcall DoDisconnect(void) = 0 ;
	virtual void __fastcall DoOpConnect(void);
	virtual void __fastcall DoOpDisconnect(void);
	virtual void __fastcall DoGetInstance(_di_IDispatch &Instance, const GUID &CLSID, const GUID &IID);
	virtual void __fastcall FixupProps(void);
	virtual TOpOfficeVersion __fastcall GetOfficeVersion(void);
	virtual AnsiString __fastcall GetOfficeVersionStr();
	void __fastcall UpdateDesigner(void);
	bool __fastcall CheckActive(bool RaiseException, TOpCallType CallType);
	virtual void __fastcall CreateEvents(System::_di_IInterface Server, const GUID &EventsIID);
	virtual bool __fastcall GetConnected(void) = 0 ;
	virtual void __fastcall Loaded(void);
	void __fastcall SetConnected(bool Value);
	__property Opevents::TOpEventAdapter* Events = {read=FEvents, write=FEvents};
	
public:
	__fastcall virtual TOpOfficeComponent(Classes::TComponent* AOwner);
	__fastcall virtual ~TOpOfficeComponent(void);
	void __fastcall AddConnectListener(TOpOfficeConnectEvent Listener);
	virtual HRESULT __fastcall CoCreate(const GUID &CoClass, const GUID &Intf, /* out */ void *Obj);
	virtual _di_IDispatch __fastcall CreateItem(TOpNestedCollectionItem* Item) = 0 ;
	virtual void __fastcall GetAppInfo(Classes::TStrings* Info) = 0 ;
	virtual void __fastcall GetFileInfo(AnsiString &Filter, AnsiString &DefExt);
	virtual void __fastcall HandleEvent(const GUID &IID, int DispId, const tagVARIANT * Params) = 0 ;
	__property TOpOfficeVersion OfficeVersion = {read=GetOfficeVersion, nodefault};
	__property AnsiString OfficeVersionStr = {read=GetOfficeVersionStr};
	void __fastcall RemoveConnectListener(TOpOfficeConnectEvent Listener);
	__property TOpClientState ClientState = {read=FClientState, write=FClientState, nodefault};
	__property bool Connected = {read=GetConnected, write=SetConnected, default=0};
	__property WideString MachineName = {read=FMachineName, write=SetMachineName};
	__property TOpPropDirection PropDirection = {read=FPropDirection, write=FPropDirection, default=0};
	__property Classes::TNotifyEvent OnOpConnect = {read=FOnOpConnect, write=FOnOpConnect};
	__property Classes::TNotifyEvent OnOpDisconnect = {read=FOnOpDisconnect, write=FOnOpDisconnect};
	__property TOpGetInstanceEvent OnGetInstance = {read=FOnGetInstance, write=FOnGetInstance};
};


class DELPHICLASS TOpNestedCollection;
class PASCALIMPLEMENTATION TOpNestedCollection : public Classes::TCollection 
{
	typedef Classes::TCollection inherited;
	
private:
	Classes::TPersistent* FOwner;
	Classes::TComponent* FRootComponent;
	TOpNestedCollectionItem* __fastcall GetParentItem(void);
	TOpNestedCollection* __fastcall GetRootCollection(void);
	
protected:
	virtual AnsiString __fastcall GetItemName();
	DYNAMIC Classes::TPersistent* __fastcall GetOwner(void);
	
public:
	__fastcall virtual TOpNestedCollection(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass);
	virtual void __fastcall ForEachItem(TOpNestedForEachProc Proc);
	virtual bool __fastcall FindItem(void *Key, TOpNestedFindItemProc Proc, Classes::TList* FindList);
	__property Classes::TComponent* RootComponent = {read=FRootComponent};
	__property TOpNestedCollection* RootCollection = {read=GetRootCollection};
	__property TOpNestedCollectionItem* ParentItem = {read=GetParentItem};
	__property AnsiString ItemName = {read=GetItemName};
	__property PropName ;
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpNestedCollection(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TOpNestedCollectionItem : public Classes::TCollectionItem 
{
	typedef Classes::TCollectionItem inherited;
	
private:
	_di_IDispatch FIntf;
	TOpNestedCollection* __fastcall GetParent(void);
	Classes::TComponent* __fastcall GetRootComponent(void);
	TOpNestedCollectionItem* __fastcall GetParentItem(void);
	_di_IDispatch __fastcall GetIntf();
	void __fastcall SetIntf(const _di_IDispatch Value);
	TOpNestedCollection* __fastcall GetRootCollection(void);
	
protected:
	bool __fastcall CheckActive(bool RaiseException, TOpCallType CallType);
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	virtual void __fastcall CreateSubCollections(void);
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	void __fastcall UpdateDesigner(void);
	
public:
	__fastcall virtual TOpNestedCollectionItem(Classes::TCollection* Collection);
	virtual void __fastcall Connect(void);
	virtual void __fastcall ExecuteVerb(int index);
	virtual void __fastcall Activate(void);
	__property _di_IDispatch Intf = {read=GetIntf, write=SetIntf};
	__property int SubCollectionCount = {read=GetSubCollectionCount, nodefault};
	__property Classes::TCollection* SubCollection[int index] = {read=GetSubCollection};
	__property TOpNestedCollection* ParentCollection = {read=GetParent};
	__property TOpNestedCollectionItem* ParentItem = {read=GetParentItem};
	__property Classes::TComponent* RootComponent = {read=GetRootComponent};
	__property TOpNestedCollection* RootCollection = {read=GetRootCollection};
	__property int VerbCount = {read=GetVerbCount, nodefault};
	__property AnsiString Verb[int index] = {read=GetVerb};
public:
	#pragma option push -w-inl
	/* TCollectionItem.Destroy */ inline __fastcall virtual ~TOpNestedCollectionItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpFreeList;
class PASCALIMPLEMENTATION TOpFreeList : public Classes::TList 
{
	typedef Classes::TList inherited;
	
public:
	__fastcall virtual ~TOpFreeList(void);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TOpFreeList(void) : Classes::TList() { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TOpCoCreateInstanceExProc OpCoCreateInstanceEx;
extern PACKAGE void __fastcall OfficeError(const AnsiString S);

}	/* namespace Opshared */
using namespace Opshared;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpShared
