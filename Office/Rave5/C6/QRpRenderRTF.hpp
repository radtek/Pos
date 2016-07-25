// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpRenderRTF.pas' rev: 6.00

#ifndef QRpRenderRTFHPP
#define QRpRenderRTFHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <QRpRender.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <Qt.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrprenderrtf
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TImageOutput { ioHigherQuality, ioSmallerSize };
#pragma option pop

#pragma option push -b-
enum TImageEncoding { ieBinary, ieHex };
#pragma option pop

class DELPHICLASS TRvRenderRTF;
class PASCALIMPLEMENTATION TRvRenderRTF : public Qrprender::TRPRenderStream 
{
	typedef Qrprender::TRPRenderStream inherited;
	
protected:
	Classes::TStringList* FontTable;
	Classes::TStringList* ColorTable;
	bool UseFinal;
	Classes::TMemoryStream* WorkFile;
	AnsiString WorkFileName;
	double LastGraphicX;
	double LastGraphicY;
	TImageOutput FImageOutput;
	TImageEncoding FImageEncoding;
	bool __fastcall IsCJKFont(void);
	void __fastcall WriteFont(void);
	void __fastcall WriteColor(void);
	void __fastcall WriteStr(AnsiString Value);
	int __fastcall GetFontIndex(void);
	AnsiString __fastcall GetRTFColor(const Qgraphics::TColor AColor);
	int __fastcall GetColorIndex(const Qgraphics::TColor AColor);
	bool __fastcall ContainsWideChars(AnsiString AText);
	void __fastcall WriteText(AnsiString Text);
	void __fastcall WriteLine(AnsiString Value);
	void __fastcall WriteGraphicHeader(void);
	void __fastcall WriteGraphicRect(double X1, double Y1, double X2, double Y2);
	void __fastcall WriteXTwips(AnsiString Command, double Value);
	void __fastcall WriteYTwips(AnsiString Command, double Value);
	void __fastcall WriteGraphicPen(void);
	void __fastcall WriteGraphicBrush(void);
	virtual void __fastcall PageBegin(void);
	virtual void __fastcall PageEnd(void);
	virtual void __fastcall LeftText(const AnsiString AText, const double X1, const double Y1);
	virtual void __fastcall CenterText(const AnsiString AText, const double X1, const double Y1);
	virtual void __fastcall RightText(const AnsiString AText, const double X1, const double Y1);
	virtual void __fastcall PrintSpaces(const AnsiString AText, const double AX, const double AY, const double AWidth);
	virtual void __fastcall PrintRightWidth(double &AX, double &AY, AnsiString AText, double Width);
	virtual void __fastcall TextRect(const Types::TRect &Rect, double X1, double Y1, AnsiString AText);
	virtual void __fastcall Rectangle(const double X1, const double Y1, const double X2, const double Y2);
	virtual void __fastcall RoundRect(const double X1, const double Y1, const double X2, const double Y2, const double X3, const double Y3);
	virtual void __fastcall MoveTo(const double X1, const double Y1);
	virtual void __fastcall LineTo(const double X1, const double Y1);
	virtual void __fastcall Ellipse(const double X1, const double Y1, const double X2, const double Y2);
	
public:
	__fastcall virtual TRvRenderRTF(Classes::TComponent* AOwner);
	__fastcall virtual ~TRvRenderRTF(void);
	virtual void __fastcall PrintRender(Classes::TStream* NDRStream, AnsiString OutputFileName);
	virtual void __fastcall DocBegin(void);
	virtual void __fastcall DocEnd(void);
	
__published:
	__property OnDecodeImage ;
	__property TImageOutput ImageOutput = {read=FImageOutput, write=FImageOutput, default=0};
	__property TImageEncoding ImageEncoding = {read=FImageEncoding, write=FImageEncoding, default=1};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Qrprenderrtf */
using namespace Qrprenderrtf;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpRenderRTF
