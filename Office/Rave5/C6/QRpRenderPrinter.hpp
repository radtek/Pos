// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpRenderPrinter.pas' rev: 6.00

#ifndef QRpRenderPrinterHPP
#define QRpRenderPrinterHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <QRpRenderCanvas.hpp>	// Pascal unit
#include <QRpRender.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <Qt.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrprenderprinter
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRvRenderPrinter;
class PASCALIMPLEMENTATION TRvRenderPrinter : public Qrprendercanvas::TRvRenderCanvas 
{
	typedef Qrprendercanvas::TRvRenderCanvas inherited;
	
protected:
	int FCopies;
	bool FCollate;
	Qrpdefine::TDuplex FDuplex;
	bool FIgnoreFileSettings;
	int FPages;
	virtual Qgraphics::TCanvas* __fastcall GetCanvas(void);
	virtual void __fastcall Arc(const double X1, const double Y1, const double X2, const double Y2, const double X3, const double Y3, const double X4, const double Y4);
	virtual void __fastcall BrushChanged(System::TObject* Sender);
	virtual void __fastcall Chord(const double X1, const double Y1, const double X2, const double Y2, const double X3, const double Y3, const double X4, const double Y4);
	virtual void __fastcall CenterText(const AnsiString Text, const double X1, const double Y1);
	virtual void __fastcall Ellipse(const double X1, const double Y1, const double X2, const double Y2);
	virtual void __fastcall FontChanged(System::TObject* Sender);
	virtual void __fastcall LeftText(const AnsiString Text, const double X1, const double Y1);
	virtual void __fastcall LineTo(const double X1, const double Y1);
	virtual void __fastcall MoveTo(const double X1, const double Y1);
	virtual void __fastcall Pie(const double X1, const double Y1, const double X2, const double Y2, const double X3, const double Y3, const double X4, const double Y4);
	virtual void __fastcall PolyLine(const Qrprender::TFloatPoint * PolyLineArr, const int PolyLineArr_Size);
	virtual void __fastcall Polygon(const Qrprender::TFloatPoint * PolyLineArr, const int PolyLineArr_Size);
	virtual void __fastcall RightText(const AnsiString Text, const double X1, const double Y1);
	virtual void __fastcall PenChanged(System::TObject* Sender);
	virtual void __fastcall PrintBitmapRect(const double X1, const double Y1, const double X2, const double Y2, Qgraphics::TBitmap* Graphic);
	virtual void __fastcall PrintBitmap(const double X1, const double Y1, const double ScaleX, const double ScaleY, Qgraphics::TBitmap* Graphic);
	virtual void __fastcall StretchDraw(const Types::TRect &Rect, Qgraphics::TGraphic* Graphic);
	virtual void __fastcall Draw(const double X1, const double Y1, Qgraphics::TGraphic* Graphic);
	virtual void __fastcall PrintSpaces(const AnsiString Text, const double X1, const double Y1, const double Width);
	virtual void __fastcall Rectangle(const double X1, const double Y1, const double X2, const double Y2);
	virtual void __fastcall FillRect(const Types::TRect &Rect);
	virtual void __fastcall TextRect(const Types::TRect &Rect, double X1, double Y1, AnsiString S1);
	virtual void __fastcall RoundRect(const double X1, const double Y1, const double X2, const double Y2, const double X3, const double Y3);
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
	__property Qrpdefine::TDuplex Duplex = {read=FDuplex, write=FDuplex, nodefault};
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

}	/* namespace Qrprenderprinter */
using namespace Qrprenderprinter;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpRenderPrinter
