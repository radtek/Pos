// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpRTFilr.pas' rev: 6.00

#ifndef RpRTFilrHPP
#define RpRTFilrHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpRPTF.hpp>	// Pascal unit
#include <RpDevice.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rprtfilr
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRTFFiler;
class PASCALIMPLEMENTATION TRTFFiler : public Rpbase::TBaseReport 
{
	typedef Rpbase::TBaseReport inherited;
	
protected:
	Classes::TStream* FStream;
	Rpdefine::TStreamMode FStreamMode;
	bool UseFinal;
	int WorkFile;
	AnsiString WorkFileName;
	Classes::TStream* FinalStream;
	bool OnNewPara;
	bool OnNewDoc;
	AnsiString FOutputFileName;
	Classes::TStringList* FontTable;
	double LastGraphicX;
	double LastGraphicY;
	void __fastcall PrintText(AnsiString Text);
	virtual void __fastcall PrintCenterInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintLeftInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintRightInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintBlockInch(AnsiString Text, double Pos, double Width);
	virtual Rpdefine::TDestination __fastcall Destination(void);
	virtual Graphics::TCanvas* __fastcall GetCanvas(void);
	void __fastcall WriteStr(AnsiString Value);
	void __fastcall WriteLine(AnsiString Value);
	void __fastcall WriteXTwips(AnsiString Command, double Value);
	void __fastcall WriteYTwips(AnsiString Command, double Value);
	void __fastcall WritePara(Rpdefine::TTabJustify Justify);
	int __fastcall GetFontIndex(void);
	void __fastcall WriteFont(void);
	void __fastcall WriteBinary(void *Buffer, int Size);
	void __fastcall WriteText(AnsiString Text);
	void __fastcall WriteGraphicPen(void);
	void __fastcall WriteGraphicBrush(void);
	void __fastcall WriteGraphicRect(double X1, double Y1, double X2, double Y2);
	void __fastcall WriteGraphicHeader(void);
	
public:
	__property Classes::TStream* Stream = {read=FStream, write=FStream};
	__fastcall virtual TRTFFiler(Classes::TComponent* AOwner);
	__fastcall virtual ~TRTFFiler(void);
	virtual void __fastcall Start(void);
	virtual void __fastcall Finish(void);
	virtual void __fastcall NewPara(void);
	virtual void __fastcall NewPage(void);
	virtual void __fastcall PrintMemo(Rpbase::TBaseMemoBuf* MemoBuf, int Lines, bool PrintTabs);
	virtual void __fastcall Tab(int LeftWidth, int RightWidth, int TopWidth, int BottomWidth, int ShadeOverride);
	virtual void __fastcall TabRectangle(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall Ellipse(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall LineTo(double X, double Y);
	virtual void __fastcall MoveTo(double X, double Y);
	virtual void __fastcall Rectangle(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall RoundRect(double X1, double Y1, double X2, double Y2, double X3, double Y3);
	virtual void __fastcall TextRect(const Types::TRect &Rect, double X, double Y, const AnsiString Text);
	virtual void __fastcall PrintData(AnsiString Value);
	virtual void __fastcall BrushCopy(const Types::TRect &Dest, Graphics::TBitmap* Bitmap, const Types::TRect &Source, Graphics::TColor Color);
	virtual void __fastcall FloodFill(double X, double Y, Graphics::TColor Color, Graphics::TFillStyle FillStyle);
	virtual void __fastcall FrameRect(const Types::TRect &Rect);
	virtual void __fastcall CopyRect(const Types::TRect &Dest, Graphics::TCanvas* Canvas, const Types::TRect &Source);
	virtual void __fastcall DrawFocusRect(const Types::TRect &Rect);
	virtual void __fastcall FillRect(const Types::TRect &Rect);
	virtual void __fastcall Arc(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	virtual void __fastcall Chord(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	virtual void __fastcall Draw(double X, double Y, Graphics::TGraphic* Graphic);
	virtual void __fastcall Pie(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	virtual void __fastcall Polygon(const Types::TPoint * Points, const int Points_Size);
	virtual void __fastcall Polyline(const Types::TPoint * Points, const int Points_Size);
	virtual void __fastcall PrintBitmap(double X, double Y, double ScaleX, double ScaleY, Graphics::TBitmap* Bitmap);
	virtual void __fastcall PrintBitmapRect(double X1, double Y1, double X2, double Y2, Graphics::TBitmap* Bitmap);
	virtual void __fastcall PrintImageRect(double X1, double Y1, double X2, double Y2, Classes::TStream* ImageStream, AnsiString ImageType);
	virtual void __fastcall StretchDraw(const Types::TRect &Rect, Graphics::TGraphic* Graphic);
	virtual void __fastcall PrintDataStream(Classes::TStream* Stream, int BufSize);
	
__published:
	__property FirstPage  = {default=1};
	__property LastPage  = {default=9999};
	__property StatusLabel ;
	__property StatusFormat ;
	__property StatusText ;
	__property LineHeightMethod  = {default=1};
	__property LinesPerInch  = {default=6};
	__property Units  = {default=0};
	__property UnitsFactor ;
	__property MarginLeft ;
	__property MarginRight ;
	__property MarginTop ;
	__property MarginBottom ;
	__property Orientation ;
	__property AnsiString FileName = {read=FOutputFileName, write=FOutputFileName};
	__property Rpdefine::TStreamMode StreamMode = {read=FStreamMode, write=FStreamMode, default=0};
	__property OnPrint ;
	__property OnPrintPage ;
	__property OnBeforePrint ;
	__property OnAfterPrint ;
	__property OnNewPage ;
	__property OnNewColumn ;
	__property OnPrintHeader ;
	__property OnPrintFooter ;
	__property OnEndOfSection ;
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rprtfilr */
using namespace Rprtfilr;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpRTFilr
