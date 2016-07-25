// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdCoderQuotedPrintable.pas' rev: 6.00

#ifndef IdCoderQuotedPrintableHPP
#define IdCoderQuotedPrintableHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdCoder.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idcoderquotedprintable
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdDecoderQuotedPrintable;
class PASCALIMPLEMENTATION TIdDecoderQuotedPrintable : public Idcoder::TIdDecoder 
{
	typedef Idcoder::TIdDecoder inherited;
	
public:
	virtual void __fastcall DecodeToStream(AnsiString AIn, Classes::TStream* ADest);
public:
	#pragma option push -w-inl
	/* TComponent.Create */ inline __fastcall virtual TIdDecoderQuotedPrintable(Classes::TComponent* AOwner) : Idcoder::TIdDecoder(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TIdDecoderQuotedPrintable(void) { }
	#pragma option pop
	
};


class DELPHICLASS TIdEncoderQuotedPrintable;
class PASCALIMPLEMENTATION TIdEncoderQuotedPrintable : public Idcoder::TIdEncoder 
{
	typedef Idcoder::TIdEncoder inherited;
	
public:
	virtual AnsiString __fastcall Encode(Classes::TStream* ASrcStream, const int ABytes = 0x7fffffff)/* overload */;
public:
	#pragma option push -w-inl
	/* TComponent.Create */ inline __fastcall virtual TIdEncoderQuotedPrintable(Classes::TComponent* AOwner) : Idcoder::TIdEncoder(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TIdEncoderQuotedPrintable(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline AnsiString __fastcall  Encode(const AnsiString ASrc){ return TIdEncoder::Encode(ASrc); }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idcoderquotedprintable */
using namespace Idcoderquotedprintable;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdCoderQuotedPrintable
