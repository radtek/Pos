// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpRenderHTML.pas' rev: 6.00

#ifndef QRpRenderHTMLHPP
#define QRpRenderHTMLHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <QRpRender.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrprenderhtml
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRvRenderHTML;
class PASCALIMPLEMENTATION TRvRenderHTML : public Qrprender::TRPRenderStream 
{
	typedef Qrprender::TRPRenderStream inherited;
	
protected:
	AnsiString FCacheDir;
	int FGenerator;
	double ffMaxY;
	int FOffsetX;
	int FOffsetY;
	AnsiString FPageURL;
	AnsiString FTemplateHead;
	AnsiString FTemplatePost;
	AnsiString FTemplatePre;
	bool FServerMode;
	bool FUsingInternalStream;
	int FLastFileNameIndex;
	AnsiString __fastcall ColorToRGBString(Qgraphics::TColor AColor);
	void __fastcall DoGifDiv(const float AX, const float AY, const float AWidth, const float AHeight, Qgraphics::TColor AColor);
	AnsiString __fastcall GetFontStyle(Qgraphics::TFont* AFont);
	AnsiString __fastcall GetTemplate();
	AnsiString __fastcall HTMLColor(const Qgraphics::TColor colr);
	AnsiString __fastcall HTMLText(const AnsiString psText);
	int __fastcall InchesToPixels(const double AInches);
	AnsiString __fastcall NewCacheFile(AnsiString AExtension);
	AnsiString __fastcall ProcessParams(const AnsiString AHTML);
	void __fastcall SetDefaultTemplate(void);
	void __fastcall SetTemplate(const AnsiString AValue);
	void __fastcall ToJPEGFile(Qgraphics::TGraphic* AGraphic, const AnsiString AFileName);
	void __fastcall WriteDiv(const AnsiString AText, const double AX1, const double AY1, const double AWidth = 0.000000E+00, const double AHeight = 0.000000E+00);
	AnsiString __fastcall ProcessURL(AnsiString AValue);
	AnsiString __fastcall PadIntToStr(int ANumber);
	__property int LastFileNameIndex = {read=FLastFileNameIndex, write=FLastFileNameIndex, nodefault};
	
public:
	__fastcall virtual TRvRenderHTML(Classes::TComponent* AOwner);
	virtual void __fastcall PrintRender(Classes::TStream* NDRStream, AnsiString OutputFileName);
	virtual void __fastcall CenterText(const AnsiString psText, const double pfX, const double pfY);
	/*         class method */ static void __fastcall CreateColorGif(TMetaClass* vmt, Classes::TStream* AStream, Qgraphics::TColor AColor);
	virtual void __fastcall LeftText(const AnsiString psText, const double pfX, const double pfY);
	virtual void __fastcall LineTo(const double pfX1, const double pfY1);
	virtual void __fastcall PrintBitmapRect(const double AX1, const double AY1, const double AX2, const double AY2, Qgraphics::TBitmap* AGraphic);
	virtual void __fastcall StretchDraw(const Types::TRect &Rect, Qgraphics::TGraphic* AGraphic);
	virtual void __fastcall PrintSpaces(const AnsiString psText, const double pfX, const double pfY, const double pfWidth);
	virtual void __fastcall PrintRightWidth(double &AX, double &AY, AnsiString AText, double Width);
	virtual void __fastcall Rectangle(const double pfX1, const double pfY1, const double pfX2, const double pfY2);
	virtual void __fastcall FillRect(const Types::TRect &pRect);
	virtual void __fastcall RightText(const AnsiString psText, const double pfX, const double pfY);
	virtual void __fastcall DocBegin(void);
	virtual void __fastcall PageBegin(void);
	virtual void __fastcall PageEnd(void);
	__property AnsiString PageURL = {read=FPageURL, write=FPageURL};
	__property AnsiString Template = {read=GetTemplate, write=SetTemplate};
	
__published:
	__property OnDecodeImage ;
	__property AnsiString CacheDir = {read=FCacheDir, write=FCacheDir};
	__property bool ServerMode = {read=FServerMode, write=FServerMode, nodefault};
public:
	#pragma option push -w-inl
	/* TRPRenderStream.Destroy */ inline __fastcall virtual ~TRvRenderHTML(void) { }
	#pragma option pop
	
};


typedef TRvRenderHTML TRpRenderHTML;
;

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Qrprenderhtml */
using namespace Qrprenderhtml;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpRenderHTML
