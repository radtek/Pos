// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpRPTF.pas' rev: 6.00

#ifndef QRpRPTFHPP
#define QRpRPTFHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpBase.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Qt.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrprptf
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE AnsiString __fastcall RPTFToText(AnsiString InStr);
extern PACKAGE void __fastcall ParseRPTF(AnsiString InStr, int &Pos, AnsiString &TextStr, AnsiString &RPTFStr);
extern PACKAGE void __fastcall ProcessRPTF(Qrpbase::TBaseReport* BaseReport, AnsiString RPTFStr);
extern PACKAGE AnsiString __fastcall NextPiece(Qrpbase::TBaseReport* BaseReport, AnsiString InStr, int &Pos);
extern PACKAGE AnsiString __fastcall UnformatStr(AnsiString RPTFStr);
extern PACKAGE int __fastcall UnformatLen(AnsiString RPTFStr);
extern PACKAGE int __fastcall UnformatLenC(char * RPTFStr, int Length);
extern PACKAGE Qrpdefine::TFormatState __fastcall GetBaseRPTFState();
extern PACKAGE void __fastcall ApplyRPTFToState(Qrpdefine::TFormatState &State, AnsiString RPTFStr);
extern PACKAGE AnsiString __fastcall StateToRPTF(const Qrpdefine::TFormatState &State);
extern PACKAGE void __fastcall RTFToRPTF(void *RTFBuffer, int RTFLength, Classes::TStream* RPTFStream);
extern PACKAGE void __fastcall RTFStreamToRPTF(Classes::TStream* RTFStream, Classes::TStream* RPTFStream);
extern PACKAGE int __fastcall SimpleTextWidth(Qrpbase::TBaseReport* BaseReport, AnsiString Value, int InitPos);
extern PACKAGE int __fastcall RPTFTextWidth(Qrpbase::TBaseReport* BaseReport, AnsiString Text);
extern PACKAGE double __fastcall RPTFTextHeight(Qrpbase::TBaseReport* BaseReport, AnsiString Text);
extern PACKAGE Qrpdefine::TPrintJustify __fastcall GetJustify(AnsiString RPTFStr, Qrpdefine::TPrintJustify Justify);

}	/* namespace Qrprptf */
using namespace Qrprptf;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpRPTF
