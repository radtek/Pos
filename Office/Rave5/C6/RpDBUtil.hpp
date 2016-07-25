// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpDBUtil.pas' rev: 6.00

#ifndef RpDBUtilHPP
#define RpDBUtilHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpMemo.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpdbutil
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TDBMemoBuf;
class PASCALIMPLEMENTATION TDBMemoBuf : public Rpmemo::TMemoBuf 
{
	typedef Rpmemo::TMemoBuf inherited;
	
protected:
	void __fastcall SetMemoField(Db::TMemoField* Value);
	void __fastcall SetRTFMemoField(Db::TMemoField* Value);
	
public:
	__property Db::TMemoField* Field = {write=SetMemoField};
	__property Db::TMemoField* RTFField = {write=SetRTFMemoField};
public:
	#pragma option push -w-inl
	/* TMemoBuf.Create */ inline __fastcall TDBMemoBuf(void) : Rpmemo::TMemoBuf() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TMemoBuf.Destroy */ inline __fastcall virtual ~TDBMemoBuf(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall GraphicFieldToBitmap(Db::TBlobField* GraphicField, Graphics::TBitmap* Bitmap);

}	/* namespace Rpdbutil */
using namespace Rpdbutil;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpDBUtil
