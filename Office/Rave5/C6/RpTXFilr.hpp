// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpTXFilr.pas' rev: 6.00

#ifndef RpTXFilrHPP
#define RpTXFilrHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpDevice.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rptxfilr
{
//-- type declarations -------------------------------------------------------
struct TTextItem;
typedef TTextItem *PTextItem;

#pragma pack(push, 1)
struct TTextItem
{
	int Col;
	int Line;
	AnsiString Data;
} ;
#pragma pack(pop)

class DELPHICLASS TTextFiler;
class PASCALIMPLEMENTATION TTextFiler : public Rpbase::TBaseReport 
{
	typedef Rpbase::TBaseReport inherited;
	
protected:
	AnsiString FOutputFileName;
	TextFile FOutputFile;
	double FCPI;
	double FLPI;
	Classes::TList* PageList;
	double FLeftBorder;
	double FTopBorder;
	bool FFormFeed;
	AnsiString FFormFeedStr;
	int __fastcall ColAt(double Pos);
	int __fastcall LineAt(double Pos);
	void __fastcall ClearPageList(void);
	void __fastcall AddTextItem(PTextItem TextItem, int Line);
	void __fastcall WriteStr(AnsiString Value);
	void __fastcall WritePage(void);
	void __fastcall PrintText(AnsiString Text, double Pos);
	virtual void __fastcall PrintCenterInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintLeftInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintRightInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintBlockInch(AnsiString Text, double Pos, double Width);
	virtual Rpdefine::TDestination __fastcall Destination(void);
	virtual Graphics::TCanvas* __fastcall GetCanvas(void);
	
public:
	__fastcall virtual TTextFiler(Classes::TComponent* AOwner);
	__fastcall virtual ~TTextFiler(void);
	virtual tagSIZE __fastcall GetTextExtent(AnsiString Value);
	virtual void __fastcall Start(void);
	virtual void __fastcall Finish(void);
	virtual void __fastcall NewPage(void);
	virtual void __fastcall NewPara(void);
	virtual double __fastcall TextWidth(AnsiString Text);
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
	virtual void __fastcall Ellipse(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall LineTo(double X, double Y);
	virtual void __fastcall MoveTo(double X, double Y);
	virtual void __fastcall Pie(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	virtual void __fastcall Polygon(const Types::TPoint * Points, const int Points_Size);
	virtual void __fastcall Polyline(const Types::TPoint * Points, const int Points_Size);
	virtual void __fastcall PrintBitmap(double X, double Y, double ScaleX, double ScaleY, Graphics::TBitmap* Bitmap);
	virtual void __fastcall PrintBitmapRect(double X1, double Y1, double X2, double Y2, Graphics::TBitmap* Bitmap);
	virtual void __fastcall PrintImageRect(double X1, double Y1, double X2, double Y2, Classes::TStream* ImageStream, AnsiString ImageType);
	virtual void __fastcall PrintDataStream(Classes::TStream* Stream, int BufSize);
	virtual void __fastcall Rectangle(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall RoundRect(double X1, double Y1, double X2, double Y2, double X3, double Y3);
	virtual void __fastcall StretchDraw(const Types::TRect &Rect, Graphics::TGraphic* Graphic);
	virtual void __fastcall TabRectangle(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall TextRect(const Types::TRect &Rect, double X, double Y, const AnsiString Text);
	__property AnsiString FormFeedStr = {read=FFormFeedStr, write=FFormFeedStr};
	
__published:
	__property AnsiString FileName = {read=FOutputFileName, write=FOutputFileName};
	__property double TopBorder = {read=FTopBorder, write=FTopBorder};
	__property double LeftBorder = {read=FLeftBorder, write=FLeftBorder};
	__property double CPI = {read=FCPI, write=FCPI};
	__property double LPI = {read=FLPI, write=FLPI};
	__property bool FormFeed = {read=FFormFeed, write=FFormFeed, default=1};
	__property StatusLabel ;
	__property StatusFormat ;
	__property StatusText ;
	__property Units  = {default=0};
	__property UnitsFactor ;
	__property MarginLeft ;
	__property MarginRight ;
	__property MarginTop ;
	__property MarginBottom ;
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

}	/* namespace Rptxfilr */
using namespace Rptxfilr;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpTXFilr
