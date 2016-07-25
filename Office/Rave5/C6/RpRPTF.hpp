// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpRPTF.pas' rev: 6.00

#ifndef RpRPTFHPP
#define RpRPTFHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rprptf
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE AnsiString __fastcall RPTFToText(AnsiString InStr);
extern PACKAGE void __fastcall ParseRPTF(AnsiString InStr, int &Pos, AnsiString &TextStr, AnsiString &RPTFStr);
extern PACKAGE void __fastcall ProcessRPTF(Rpbase::TBaseReport* BaseReport, AnsiString RPTFStr);
extern PACKAGE AnsiString __fastcall NextPiece(Rpbase::TBaseReport* BaseReport, AnsiString InStr, int &Pos);
extern PACKAGE AnsiString __fastcall UnformatStr(AnsiString RPTFStr);
extern PACKAGE int __fastcall UnformatLen(AnsiString RPTFStr);
extern PACKAGE int __fastcall UnformatLenC(char * RPTFStr, int Length);
extern PACKAGE Rpdefine::TFormatState __fastcall GetBaseRPTFState();
extern PACKAGE void __fastcall ApplyRPTFToState(Rpdefine::TFormatState &State, AnsiString RPTFStr);
extern PACKAGE AnsiString __fastcall StateToRPTF(const Rpdefine::TFormatState &State);
extern PACKAGE void __fastcall RTFToRPTF(void *RTFBuffer, int RTFLength, Classes::TStream* RPTFStream);
extern PACKAGE void __fastcall RTFStreamToRPTF(Classes::TStream* RTFStream, Classes::TStream* RPTFStream);
extern PACKAGE int __fastcall SimpleTextWidth(Rpbase::TBaseReport* BaseReport, AnsiString Value, int InitPos);
extern PACKAGE int __fastcall RPTFTextWidth(Rpbase::TBaseReport* BaseReport, AnsiString Text);
extern PACKAGE double __fastcall RPTFTextHeight(Rpbase::TBaseReport* BaseReport, AnsiString Text);
extern PACKAGE Rpdefine::TPrintJustify __fastcall GetJustify(AnsiString RPTFStr, Rpdefine::TPrintJustify Justify);

}	/* namespace Rprptf */
using namespace Rprptf;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpRPTF
