// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdHashCRC.pas' rev: 6.00

#ifndef IdHashCRCHPP
#define IdHashCRCHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdHash.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idhashcrc
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdHashCRC16;
class PASCALIMPLEMENTATION TIdHashCRC16 : public Idhash::TIdHash16 
{
	typedef Idhash::TIdHash16 inherited;
	
public:
	virtual Word __fastcall HashValue(Classes::TStream* AStream)/* overload */;
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TIdHashCRC16(void) : Idhash::TIdHash16() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TIdHashCRC16(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline Word __fastcall  HashValue(const AnsiString ASrc){ return TIdHash16::HashValue(ASrc); }
	
};


class DELPHICLASS TIdHashCRC32;
class PASCALIMPLEMENTATION TIdHashCRC32 : public Idhash::TIdHash32 
{
	typedef Idhash::TIdHash32 inherited;
	
public:
	virtual unsigned __fastcall HashValue(Classes::TStream* AStream)/* overload */;
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TIdHashCRC32(void) : Idhash::TIdHash32() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TIdHashCRC32(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline unsigned __fastcall  HashValue(const AnsiString ASrc){ return TIdHash32::HashValue(ASrc); }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idhashcrc */
using namespace Idhashcrc;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdHashCRC
