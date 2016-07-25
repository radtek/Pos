// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntClipBrd.pas' rev: 6.00

#ifndef TntClipBrdHPP
#define TntClipBrdHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Clipbrd.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntclipbrd
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntClipboard;
class PASCALIMPLEMENTATION TTntClipboard : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	WideString __fastcall GetAsWideText();
	void __fastcall SetAsWideText(const WideString Value);
	
public:
	__property WideString AsWideText = {read=GetAsWideText, write=SetAsWideText};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TTntClipboard(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TTntClipboard(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TTntClipboard* __fastcall TntClipboard(void);

}	/* namespace Tntclipbrd */
using namespace Tntclipbrd;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntClipBrd
