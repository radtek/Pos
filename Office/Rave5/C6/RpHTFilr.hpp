// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpHTFilr.pas' rev: 6.00

#ifndef RpHTFilrHPP
#define RpHTFilrHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
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

namespace Rphtfilr
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRPHTMLFiler;
class PASCALIMPLEMENTATION TRPHTMLFiler : public Rpbase::TBaseReport 
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
	AnsiString FInputFileName;
	AnsiString FReplaceToken;
	Classes::TStringList* FontTable;
	double LastGraphicX;
	double LastGraphicY;
	AnsiString FTitle;
	bool FTableTagStarted;
	bool FInTabCell;
	AnsiString FTableAttributes;
	AnsiString FParagraphAttributes;
	Classes::TStringList* FMasterFileLines;
	AnsiString FTokenEndText;
	int FRow;
	int FCol;
	int FScreenDPI;
	bool FUseTrueType;
	bool FEndTabWritten;
	virtual void __fastcall PrintCenterInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintLeftInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintRightInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintBlockInch(AnsiString Text, double Pos, double Width);
	virtual Rpdefine::TDestination __fastcall Destination(void);
	virtual Graphics::TCanvas* __fastcall GetCanvas(void);
	void __fastcall WriteStr(AnsiString Value);
	void __fastcall WriteLine(AnsiString Value);
	void __fastcall WritePageIntro(void);
	void __fastcall WriteBegFile(void);
	void __fastcall WriteEndFile(void);
	void __fastcall WriteBegFont(void);
	void __fastcall WriteEndFont(void);
	void __fastcall WriteBinary(void *Buffer, int Size);
	void __fastcall WriteText(AnsiString Text);
	void __fastcall WriteGraphicPen(void);
	void __fastcall WriteGraphicBrush(void);
	void __fastcall WriteGraphicRect(double X1, double Y1, double X2, double Y2);
	void __fastcall WriteGraphicHeader(void);
	void __fastcall WriteTable(AnsiString Text, double Pos, double Width, Rpdefine::TPrintJustify Justify);
	
public:
	__fastcall virtual TRPHTMLFiler(Classes::TComponent* AOwner);
	__fastcall virtual ~TRPHTMLFiler(void);
	virtual void __fastcall Start(void);
	virtual void __fastcall Finish(void);
	virtual void __fastcall NewPara(void);
	virtual void __fastcall NewPage(void);
	virtual void __fastcall BrushCopy(const Types::TRect &Dest, Graphics::TBitmap* Bitmap, const Types::TRect &Source, Graphics::TColor Color);
	virtual void __fastcall FloodFill(double X, double Y, Graphics::TColor Color, Graphics::TFillStyle FillStyle);
	virtual void __fastcall FrameRect(const Types::TRect &Rect);
	virtual void __fastcall CopyRect(const Types::TRect &Dest, Graphics::TCanvas* Canvas, const Types::TRect &Source);
	virtual void __fastcall DrawFocusRect(const Types::TRect &Rect);
	virtual void __fastcall FillRect(const Types::TRect &Rect);
	virtual void __fastcall Tab(int LeftWidth, int RightWidth, int TopWidth, int BottomWidth, int ShadeOverride);
	virtual void __fastcall TabRectangle(double X1, double Y1, double X2, double Y2);
	void __fastcall PrintHyperLink(AnsiString URL, AnsiString Attributes, AnsiString Text);
	void __fastcall PrintHyperLinkStart(AnsiString URL, AnsiString Attributes);
	void __fastcall PrintHyperLinkEnd(void);
	virtual void __fastcall Ellipse(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall LineTo(double X, double Y);
	virtual void __fastcall MoveTo(double X, double Y);
	virtual void __fastcall Rectangle(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall RoundRect(double X1, double Y1, double X2, double Y2, double X3, double Y3);
	virtual void __fastcall TextRect(const Types::TRect &Rect, double X, double Y, const AnsiString Text);
	virtual void __fastcall PrintData(AnsiString Value);
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
	void __fastcall PrintImage(AnsiString URL, double Width, double Height, AnsiString Attributes);
	__property Classes::TStream* Stream = {read=FStream, write=FStream};
	
__published:
	__property StatusLabel ;
	__property StatusFormat ;
	__property StatusText ;
	__property Units  = {default=0};
	__property UnitsFactor ;
	__property AnsiString FileName = {read=FOutputFileName, write=FOutputFileName};
	__property AnsiString ReplaceFileName = {read=FInputFileName, write=FInputFileName};
	__property AnsiString ReplaceToken = {read=FReplaceToken, write=FReplaceToken};
	__property int ScreenDPI = {read=FScreenDPI, write=FScreenDPI, default=96};
	__property Rpdefine::TStreamMode StreamMode = {read=FStreamMode, write=FStreamMode, default=0};
	__property AnsiString Title = {read=FTitle, write=FTitle};
	__property AnsiString TableAttributes = {read=FTableAttributes, write=FTableAttributes};
	__property AnsiString ParagraphAttributes = {read=FParagraphAttributes, write=FParagraphAttributes};
	__property bool UseTrueType = {read=FUseTrueType, write=FUseTrueType, nodefault};
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

}	/* namespace Rphtfilr */
using namespace Rphtfilr;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpHTFilr
