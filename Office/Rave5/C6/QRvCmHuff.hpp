// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvCmHuff.pas' rev: 6.00

#ifndef QRvCmHuffHPP
#define QRvCmHuffHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvcmhuff
{
//-- type declarations -------------------------------------------------------
typedef Word __fastcall (*IOFuncType)(void *Buffer, Word Amount);

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall Compress(Byte InBits, Byte MaxBits, IOFuncType IFunc, IOFuncType OFunc);
extern PACKAGE void __fastcall Decompress(Byte InBits, Byte MaxBits, IOFuncType IFunc, IOFuncType OFunc);

}	/* namespace Qrvcmhuff */
using namespace Qrvcmhuff;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvCmHuff
