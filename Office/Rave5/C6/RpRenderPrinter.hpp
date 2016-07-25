// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpRenderPrinter.pas' rev: 6.00

#ifndef RpRenderPrinterHPP
#define RpRenderPrinterHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpDefine.hpp>	// Pascal unit
#include <RpRenderCanvas.hpp>	// Pascal unit
#include <RpRender.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rprenderprinter
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRvRenderPrinter;
class PASCALIMPLEMENTATION TRvRenderPrinter : public Rprendercanvas::TRvRenderCanvas 
{
	typedef Rprendercanvas::TRvRenderCanvas inherited;
	
protected:
	int FCopies;
	bool FCollate;
	Rpdefine::TDuplex FDuplex;
	bool FIgnoreFileSettings;
	int FPages;
	virtual Graphics::TCanvas* __fastcall GetCanvas(void);
	virtual void __fastcall DocBegin(void);
	virtual void __fastcall DocEnd(void);
	virtual void __fastcall PageBegin(void);
	virtual void __fastcall PageEnd(void);
	int __fastcall GetPages(void);
	int __fastcall GetMaxCopies(void);
	virtual int __fastcall GetXDPI(void);
	virtual int __fastcall GetYDPI(void);
	virtual void __fastcall SelectBin(AnsiString ABin)/* overload */;
	virtual void __fastcall SelectBin(int ABin)/* overload */;
	virtual void __fastcall PrintData(AnsiString Value);
	virtual void __fastcall PrintDataStream(Classes::TStream* Stream, int BufSize);
	
public:
	__fastcall virtual TRvRenderPrinter(Classes::TComponent* AOwner);
	__property int Pages = {read=GetPages, write=FPages, nodefault};
	__property int Copies = {read=FCopies, write=FCopies, nodefault};
	__property bool Collate = {read=FCollate, write=FCollate, nodefault};
	__property Rpdefine::TDuplex Duplex = {read=FDuplex, write=FDuplex, nodefault};
	__property bool IgnoreFileSettings = {read=FIgnoreFileSettings, write=FIgnoreFileSettings, nodefault};
	__property int MaxCopies = {read=GetMaxCopies, nodefault};
	virtual int __fastcall XI2D(double Pos);
	virtual int __fastcall YI2D(double Pos);
	
__published:
	__property Active  = {default=0};
	__property OutputFileName ;
public:
	#pragma option push -w-inl
	/* TRPRenderStream.Destroy */ inline __fastcall virtual ~TRvRenderPrinter(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rprenderprinter */
using namespace Rprenderprinter;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpRenderPrinter
