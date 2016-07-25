// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdHashElf.pas' rev: 6.00

#ifndef IdHashElfHPP
#define IdHashElfHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdHash.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idhashelf
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdHashElf;
class PASCALIMPLEMENTATION TIdHashElf : public Idhash::TIdHash32 
{
	typedef Idhash::TIdHash32 inherited;
	
public:
	virtual unsigned __fastcall HashValue(Classes::TStream* AStream)/* overload */;
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TIdHashElf(void) : Idhash::TIdHash32() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TIdHashElf(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline unsigned __fastcall  HashValue(const AnsiString ASrc){ return TIdHash32::HashValue(ASrc); }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idhashelf */
using namespace Idhashelf;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdHashElf
