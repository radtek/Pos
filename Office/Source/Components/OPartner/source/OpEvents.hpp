// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpEvents.pas' rev: 6.00

#ifndef OpEventsHPP
#define OpEventsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Windows.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opevents
{
//-- type declarations -------------------------------------------------------
typedef void __fastcall (__closure *TEventHandler)(const GUID &IID, int DispID, const tagVARIANT * Params);

class DELPHICLASS EEventSinkException;
class PASCALIMPLEMENTATION EEventSinkException : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EEventSinkException(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EEventSinkException(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EEventSinkException(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EEventSinkException(int Ident, const System::TVarRec * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EEventSinkException(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EEventSinkException(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EEventSinkException(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EEventSinkException(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EEventSinkException(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TOpEventOperation { eoAdvise, eoUnadvise };
#pragma option pop

class DELPHICLASS TOpEventAdapter;
class PASCALIMPLEMENTATION TOpEventAdapter : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	System::_di_IInterface FServer;
	TEventHandler FHandler;
	Classes::TList* FEventList;
	bool __fastcall DoesExist(const GUID &IID);
	int __fastcall FindEvent(const GUID &IID);
	
public:
	__fastcall TOpEventAdapter(const System::_di_IInterface Server, TEventHandler Handler);
	__fastcall virtual ~TOpEventAdapter(void);
	void __fastcall AddEvents(const GUID &IID);
	void __fastcall RemoveEvents(const GUID &IID);
	void __fastcall RemoveAllEvents(void);
};


class DELPHICLASS TOpEventSink;
class PASCALIMPLEMENTATION TOpEventSink : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	System::_di_IInterface FServer;
	int FCookie;
	TEventHandler FHandler;
	#pragma pack(push, 1)
	GUID FIID;
	#pragma pack(pop)
	
	bool __fastcall InstallEvent(TOpEventOperation Op);
	
public:
	__fastcall TOpEventSink(const System::_di_IInterface Server, TEventHandler Handler, const GUID &EventDisp);
	HRESULT __stdcall QueryInterface(const GUID &IID, /* out */ void *Obj);
	int __stdcall _AddRef(void);
	int __stdcall _Release(void);
	HRESULT __stdcall GetTypeInfoCount(/* out */ int &Count);
	HRESULT __stdcall GetTypeInfo(int Index, int LocaleID, /* out */ void *TypeInfo);
	HRESULT __stdcall GetIDsOfNames(const GUID &IID, void * Names, int NameCount, int LocaleID, void * DispIDs);
	HRESULT __stdcall Invoke(int DispID, const GUID &IID, int LocaleID, Word Flags, void *Params, void * VarResult, void * ExcepInfo, void * ArgErr);
	__fastcall virtual ~TOpEventSink(void);
	__property GUID IID = {read=FIID};
private:
	void *__IDispatch;	/* IDispatch */
	
public:
	operator IDispatch*(void) { return (IDispatch*)&__IDispatch; }
	operator IInterface*(void) { return (IInterface*)&__IDispatch; }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Opevents */
using namespace Opevents;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpEvents
