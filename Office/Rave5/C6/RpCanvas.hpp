// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpCanvas.pas' rev: 6.00

#ifndef RpCanvasHPP
#define RpCanvasHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpRPTF.hpp>	// Pascal unit
#include <RpDevice.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
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

namespace Rpcanvas
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TCanvasReport;
class PASCALIMPLEMENTATION TCanvasReport : public Rpbase::TBaseReport 
{
	typedef Rpbase::TBaseReport inherited;
	
protected:
	void __fastcall PrintAlign(AnsiString Text, double Pos, int Flags);
	void __fastcall PrintRightWidth(AnsiString Text, double Width);
	virtual void __fastcall PrintCenterInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintLeftInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintRightInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintBlockInch(AnsiString Text, double Pos, double Width);
	
public:
	virtual void __fastcall BrushCopy(const Types::TRect &Dest, Graphics::TBitmap* Bitmap, const Types::TRect &Source, Graphics::TColor Color);
	virtual void __fastcall FloodFill(double X, double Y, Graphics::TColor Color, Graphics::TFillStyle FillStyle);
	virtual void __fastcall FrameRect(const Types::TRect &Rect);
	virtual void __fastcall CopyRect(const Types::TRect &Dest, Graphics::TCanvas* Canvas, const Types::TRect &Source);
	virtual void __fastcall DrawFocusRect(const Types::TRect &Rect);
	virtual void __fastcall FillRect(const Types::TRect &Rect);
	virtual void __fastcall Arc(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	virtual void __fastcall Chord(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	virtual void __fastcall Draw(double X, double Y, Graphics::TGraphic* Graphic);
	virtual void __fastcall Ellipse(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall LineTo(double X, double Y);
	virtual void __fastcall MoveTo(double X, double Y);
	virtual void __fastcall Pie(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	virtual void __fastcall Polygon(const Types::TPoint * Points, const int Points_Size);
	virtual void __fastcall Polyline(const Types::TPoint * Points, const int Points_Size);
	virtual void __fastcall PrintBitmap(double X, double Y, double ScaleX, double ScaleY, Graphics::TBitmap* Bitmap);
	virtual void __fastcall PrintBitmapRect(double X1, double Y1, double X2, double Y2, Graphics::TBitmap* Bitmap);
	virtual void __fastcall PrintImageRect(double X1, double Y1, double X2, double Y2, Classes::TStream* ImageStream, AnsiString ImageType);
	virtual void __fastcall Rectangle(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall RoundRect(double X1, double Y1, double X2, double Y2, double X3, double Y3);
	virtual void __fastcall StretchDraw(const Types::TRect &Rect, Graphics::TGraphic* Graphic);
	virtual void __fastcall TabRectangle(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall TextRect(const Types::TRect &Rect, double X, double Y, const AnsiString Text);
	virtual void __fastcall PrintData(AnsiString Value);
	virtual void __fastcall PrintDataStream(Classes::TStream* Stream, int BufSize);
	virtual void __fastcall NewPage(void);
public:
	#pragma option push -w-inl
	/* TBaseReport.Create */ inline __fastcall virtual TCanvasReport(Classes::TComponent* AOwner) : Rpbase::TBaseReport(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBaseReport.Destroy */ inline __fastcall virtual ~TCanvasReport(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rpcanvas */
using namespace Rpcanvas;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpCanvas
