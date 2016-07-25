// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpMBCS.pas' rev: 6.00

#ifndef QRpMBCSHPP
#define QRpMBCSHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpmbcs
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE char * __fastcall MBCSNextChar(const char * AStr);
extern PACKAGE AnsiString __fastcall MBCSCopy(AnsiString AString, int APos, int ACount);
extern PACKAGE void __fastcall MBCSDelete(AnsiString &AString, int APos, int ACount);

}	/* namespace Qrpmbcs */
using namespace Qrpmbcs;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpMBCS
