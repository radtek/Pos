// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntFormatStrUtils.pas' rev: 6.00

#ifndef TntFormatStrUtilsHPP
#define TntFormatStrUtilsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <TntSysUtils.hpp>	// Pascal unit
#include <TntClasses.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntformatstrutils
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE WideString __fastcall GetCanonicalFormatStr(const WideString _FormatString);
extern PACKAGE WideString __fastcall ReplaceFloatingArgumentsInFormatString(const WideString _FormatString, const System::TVarRec * Args, const int Args_Size);
extern PACKAGE void __fastcall CompareFormatStrings(WideString FormatStr1, WideString FormatStr2);

}	/* namespace Tntformatstrutils */
using namespace Tntformatstrutils;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntFormatStrUtils
