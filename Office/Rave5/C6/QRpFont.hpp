// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpFont.pas' rev: 6.00

#ifndef QRpFontHPP
#define QRpFontHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <SysUtils.hpp>	// Pascal unit
#include <SyncObjs.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Qt.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpfont
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRPBaseFontHandler;
class PASCALIMPLEMENTATION TRPBaseFontHandler : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	__fastcall virtual TRPBaseFontHandler(Qgraphics::TFont* Font);
	virtual int __fastcall TextWidth(AnsiString Value, double FontSize, int DPI) = 0 ;
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPBaseFontHandler(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPTrueTypeFontHandler;
class PASCALIMPLEMENTATION TRPTrueTypeFontHandler : public TRPBaseFontHandler 
{
	typedef TRPBaseFontHandler inherited;
	
protected:
	int FFontTable[256];
	
public:
	__fastcall virtual TRPTrueTypeFontHandler(Qgraphics::TFont* Font);
	virtual int __fastcall TextWidth(AnsiString Value, double FontSize, int DPI);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPTrueTypeFontHandler(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPNonProportionalFontHandler;
class PASCALIMPLEMENTATION TRPNonProportionalFontHandler : public TRPBaseFontHandler 
{
	typedef TRPBaseFontHandler inherited;
	
protected:
	int FFontWidth;
	
public:
	__fastcall virtual TRPNonProportionalFontHandler(Qgraphics::TFont* Font);
	virtual int __fastcall TextWidth(AnsiString Value, double FontSize, int DPI);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPNonProportionalFontHandler(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPFontManager;
class PASCALIMPLEMENTATION TRPFontManager : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Classes::TStringList* FFontList;
	int FDPI;
	AnsiString __fastcall FontNameOf(Qgraphics::TFont* Font);
	TRPBaseFontHandler* __fastcall GetFontHandler(Qgraphics::TFont* Font);
	
public:
	__fastcall virtual TRPFontManager(void);
	__fastcall virtual ~TRPFontManager(void);
	int __fastcall TextWidth(Qgraphics::TFont* Font, AnsiString Value);
};


//-- var, const, procedure ---------------------------------------------------
static const Word BaseSize = 0x258;
extern PACKAGE TRPFontManager* FontManager;

}	/* namespace Qrpfont */
using namespace Qrpfont;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpFont
