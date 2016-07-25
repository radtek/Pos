// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpMBCS.pas' rev: 6.00

#ifndef RpMBCSHPP
#define RpMBCSHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpmbcs
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE char * __fastcall MBCSNextChar(const char * AStr);
extern PACKAGE AnsiString __fastcall MBCSCopy(AnsiString AString, int APos, int ACount);
extern PACKAGE void __fastcall MBCSDelete(AnsiString &AString, int APos, int ACount);

}	/* namespace Rpmbcs */
using namespace Rpmbcs;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpMBCS
