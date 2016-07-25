// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntSystem.pas' rev: 6.00

#ifndef TntSystemHPP
#define TntSystemHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntsystem
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TTntSystemUpdate { tsWideResourceStrings, tsFixImplicitCodePage, tsFixWideStrConcat, tsFixWideFormat };
#pragma option pop

typedef Set<TTntSystemUpdate, tsWideResourceStrings, tsFixWideFormat>  TTntSystemUpdateSet;

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE unsigned DefaultUserCodePage;
extern PACKAGE bool __fastcall (*WideCustomLoadResString)(System::PResStringRec ResStringRec, WideString &Value);
static const wchar_t UNICODE_BOM = wchar_t(0xfeff);
static const wchar_t UNICODE_BOM_SWAPPED = wchar_t(0xfffe);
#define UTF8_BOM "﻿"
#define AllTntSystemUpdates (System::Set<TTntSystemUpdate, tsWideResourceStrings, tsFixWideFormat> () << TTntSystemUpdate(0) << TTntSystemUpdate(1) << TTntSystemUpdate(2) << TTntSystemUpdate(3) )
extern PACKAGE WideString __fastcall WideLoadResString(System::PResStringRec ResStringRec);
extern PACKAGE int __fastcall WideParamCount(void);
extern PACKAGE WideString __fastcall WideParamStr(int Index);
extern PACKAGE AnsiString __fastcall WideStringToUTF8(const WideString S);
extern PACKAGE WideString __fastcall UTF8ToWideString(const AnsiString S);
extern PACKAGE AnsiString __fastcall WideStringToUTF7(const WideString W);
extern PACKAGE WideString __fastcall UTF7ToWideString(const AnsiString S);
extern PACKAGE WideString __fastcall StringToWideStringEx(const AnsiString S, unsigned CodePage);
extern PACKAGE AnsiString __fastcall WideStringToStringEx(const WideString WS, unsigned CodePage);
extern PACKAGE WideString __fastcall UCS2ToWideString(const AnsiString Value);
extern PACKAGE AnsiString __fastcall WideStringToUCS2(const WideString Value);
extern PACKAGE unsigned __fastcall CharSetToCodePage(unsigned ciCharset);
extern PACKAGE unsigned __fastcall LCIDToCodePage(unsigned ALcid);
extern PACKAGE unsigned __fastcall KeyboardCodePage(void);
extern PACKAGE wchar_t __fastcall KeyUnicode(Word CharCode);
extern PACKAGE void __fastcall StrSwapByteOrder(wchar_t * Str);
extern PACKAGE void __fastcall InstallTntSystemUpdates(TTntSystemUpdateSet Updates = (System::Set<TTntSystemUpdate, tsWideResourceStrings, tsFixWideFormat> () << TTntSystemUpdate(0) << TTntSystemUpdate(1) << TTntSystemUpdate(2) << TTntSystemUpdate(3) ));

}	/* namespace Tntsystem */
using namespace Tntsystem;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntSystem
