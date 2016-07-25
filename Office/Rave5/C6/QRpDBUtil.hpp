// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpDBUtil.pas' rev: 6.00

#ifndef QRpDBUtilHPP
#define QRpDBUtilHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpMemo.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <QExtCtrls.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpdbutil
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TDBMemoBuf;
class PASCALIMPLEMENTATION TDBMemoBuf : public Qrpmemo::TMemoBuf 
{
	typedef Qrpmemo::TMemoBuf inherited;
	
protected:
	void __fastcall SetMemoField(Db::TMemoField* Value);
	void __fastcall SetRTFMemoField(Db::TMemoField* Value);
	
public:
	__property Db::TMemoField* Field = {write=SetMemoField};
	__property Db::TMemoField* RTFField = {write=SetRTFMemoField};
public:
	#pragma option push -w-inl
	/* TMemoBuf.Create */ inline __fastcall TDBMemoBuf(void) : Qrpmemo::TMemoBuf() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TMemoBuf.Destroy */ inline __fastcall virtual ~TDBMemoBuf(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall GraphicFieldToBitmap(Db::TBlobField* GraphicField, Qgraphics::TBitmap* Bitmap);

}	/* namespace Qrpdbutil */
using namespace Qrpdbutil;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpDBUtil
