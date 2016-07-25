// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpRenderCanvas.pas' rev: 6.00

#ifndef QRpRenderCanvasHPP
#define QRpRenderCanvasHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
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

namespace Qrprendercanvas
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRvRenderCanvas;
class PASCALIMPLEMENTATION TRvRenderCanvas : public Qrprender::TRPRenderStream 
{
	typedef Qrprender::TRPRenderStream inherited;
	
protected:
	HFONT CurrFont;
	virtual void __fastcall SetBKColor(Qgraphics::TColor Value);
	virtual Qgraphics::TCanvas* __fastcall GetCanvas(void) = 0 ;
	void __fastcall PrintAlign(AnsiString Text, double X1, double Y1, int Flags);
	virtual void __fastcall PrintRightWidth(double &X1, double &Y1, AnsiString Text, double Width);
	virtual double __fastcall CanvasTextWidth(const AnsiString Text);
	virtual void __fastcall FrameRect(const Types::TRect &Rect);
	HFONT __fastcall CreateFont(void);
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
	virtual void __fastcall PrintSpaces(const AnsiString Text, const double X1, const double Y1, const double AWidth);
	virtual void __fastcall Rectangle(const double X1, const double Y1, const double X2, const double Y2);
	virtual void __fastcall FillRect(const Types::TRect &Rect);
	virtual void __fastcall TextRect(const Types::TRect &Rect, double X1, double Y1, AnsiString S1);
	virtual void __fastcall RoundRect(const double X1, const double Y1, const double X2, const double Y2, const double X3, const double Y3);
	virtual void __fastcall BrushCopy(const Types::TRect &Dest, Qgraphics::TBitmap* Bitmap, const Types::TRect &Source, Qgraphics::TColor Color);
	virtual void __fastcall CopyRect(const Types::TRect &Dest, Qgraphics::TCanvas* Canvas, const Types::TRect &Source);
	virtual void __fastcall DrawFocusRect(const Types::TRect &Rect);
	virtual void __fastcall DocBegin(void);
	virtual void __fastcall DocEnd(void);
	virtual void __fastcall PageBegin(void);
	virtual void __fastcall PageEnd(void);
	
public:
	__fastcall virtual TRvRenderCanvas(Classes::TComponent* AOwner);
	__property Qgraphics::TCanvas* Canvas = {read=GetCanvas};
public:
	#pragma option push -w-inl
	/* TRPRenderStream.Destroy */ inline __fastcall virtual ~TRvRenderCanvas(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Qrprendercanvas */
using namespace Qrprendercanvas;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpRenderCanvas
