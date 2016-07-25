// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntRegistry.pas' rev: 6.00

#ifndef TntRegistryHPP
#define TntRegistryHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <TntClasses.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Registry.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntregistry
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntRegistry;
class PASCALIMPLEMENTATION TTntRegistry : public Registry::TRegistry 
{
	typedef Registry::TRegistry inherited;
	
private:
	void __fastcall WriteStringEx(unsigned dwType, const WideString Name, const WideString Value);
	
public:
	HIDESBASE void __fastcall GetKeyNames(Tntclasses::TTntStrings* Strings);
	HIDESBASE void __fastcall GetValueNames(Tntclasses::TTntStrings* Strings);
	HIDESBASE WideString __fastcall ReadString(const WideString Name);
	HIDESBASE void __fastcall WriteString(const WideString Name, const WideString Value);
	HIDESBASE void __fastcall WriteExpandString(const WideString Name, const WideString Value);
public:
	#pragma option push -w-inl
	/* TRegistry.Create */ inline __fastcall TTntRegistry(void)/* overload */ : Registry::TRegistry() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRegistry.Destroy */ inline __fastcall virtual ~TTntRegistry(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Tntregistry */
using namespace Tntregistry;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntRegistry
