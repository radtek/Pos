// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpBars.pas' rev: 6.00

#ifndef RpBarsHPP
#define RpBarsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpDefine.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpbars
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TCodePage128 { cpCodeA, cpCodeB, cpCodeC };
#pragma option pop

#pragma option push -b-
enum TBarCodeRotation { Rot0, Rot90, Rot180, Rot270 };
#pragma option pop

typedef void __fastcall (__closure *TOverrideBar)(double X1, double Y1, double X2, double Y2);

typedef void __fastcall (__closure *TOverrideText)(AnsiString Text);

class DELPHICLASS TRPBarsBase;
class PASCALIMPLEMENTATION TRPBarsBase : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	double FWideFactor;
	double FBarWidth;
	double FBarHeight;
	double FPosition;
	double FTop;
	Rpbase::TBaseReport* FBaseReport;
	AnsiString FText;
	double CurrX;
	double CurrY;
	Graphics::TBrush* SaveBrush;
	Graphics::TPen* SavePen;
	bool FUseChecksum;
	int FModulas;
	bool FPrintReadable;
	bool FPrintChecksum;
	bool FPrintTop;
	Rpdefine::TPrintJustify FTextJustify;
	Rpdefine::TPrintJustify FBarCodeJustify;
	TBarCodeRotation FBarCodeRotation;
	TOverrideBar FOverrideBar;
	TOverrideText FOverrideText;
	bool FIsOverride;
	virtual void __fastcall SetText(AnsiString Value);
	virtual void __fastcall PrintReadableText(void);
	__property int Modulas = {read=FModulas, write=FModulas, nodefault};
	virtual double __fastcall GetWidth(void);
	double __fastcall GetHeight(void);
	double __fastcall GetLeft(void);
	void __fastcall SetLeft(double Value);
	double __fastcall GetBottom(void);
	void __fastcall SetBottom(double Value);
	double __fastcall GetRight(void);
	void __fastcall SetRight(double Value);
	double __fastcall GetCenter(void);
	void __fastcall SetCenter(double Value);
	double __fastcall GetBarTop(void);
	void __fastcall SetBarTop(double Value);
	double __fastcall GetBarBottom(void);
	void __fastcall SetBarBottom(double Value);
	double __fastcall GetReadableHeight(void);
	virtual void __fastcall SetUseChecksum(bool Value);
	virtual AnsiString __fastcall GetBarMask(char ch) = 0 ;
	virtual AnsiString __fastcall GetChecksum();
	virtual void __fastcall ModifyBar(char BarCh, double &Width, double &Height, double &Top) = 0 ;
	void __fastcall PrintBarPattern(AnsiString BarMask, bool BarFirst, bool AddSpace);
	virtual AnsiString __fastcall GetBarcodeText();
	virtual void __fastcall SetPrintReadable(bool Value);
	void __fastcall DrawRect(double X1, double Y1, double X2, double Y2);
	bool __fastcall IsOverride(bool Refresh);
	void __fastcall SetupCanvas(void);
	void __fastcall RestoreCanvas(void);
	void __fastcall CheckForValidCanvas(void);
	
public:
	__fastcall virtual TRPBarsBase(Rpbase::TBaseReport* BaseRpt);
	virtual void __fastcall Print(void) = 0 ;
	void __fastcall PrintXY(double X, double Y);
	virtual bool __fastcall IsValidChar(char Ch);
	__property Rpbase::TBaseReport* BaseReport = {read=FBaseReport, write=FBaseReport};
	__property Rpdefine::TPrintJustify BarCodeJustify = {read=FBarCodeJustify, write=FBarCodeJustify, nodefault};
	__property double BarWidth = {read=FBarWidth, write=FBarWidth};
	__property double BarHeight = {read=FBarHeight, write=FBarHeight};
	__property double BarTop = {read=GetBarTop, write=SetBarTop};
	__property double BarBottom = {read=GetBarBottom, write=SetBarBottom};
	__property double Center = {read=GetCenter, write=SetCenter};
	__property AnsiString Checksum = {read=GetChecksum};
	__property double Left = {read=GetLeft, write=SetLeft};
	__property double Height = {read=GetHeight};
	__property double Position = {read=FPosition, write=FPosition};
	__property double Right = {read=GetRight, write=SetRight};
	__property AnsiString Text = {read=FText, write=SetText};
	__property Rpdefine::TPrintJustify TextJustify = {read=FTextJustify, write=FTextJustify, nodefault};
	__property double Top = {read=FTop, write=FTop};
	__property double Bottom = {read=GetBottom, write=SetBottom};
	__property bool UseChecksum = {read=FUseChecksum, write=SetUseChecksum, nodefault};
	__property double WideFactor = {read=FWideFactor, write=FWideFactor};
	__property double Width = {read=GetWidth};
	__property bool PrintReadable = {read=FPrintReadable, write=FPrintReadable, nodefault};
	__property bool PrintChecksum = {read=FPrintChecksum, write=FPrintChecksum, nodefault};
	__property bool PrintTop = {read=FPrintTop, write=FPrintTop, nodefault};
	__property TBarCodeRotation BarCodeRotation = {read=FBarCodeRotation, write=FBarCodeRotation, nodefault};
	__property double ReadableHeight = {read=GetReadableHeight};
	__property TOverrideBar OverrideBar = {read=FOverrideBar, write=FOverrideBar};
	__property TOverrideText OverrideText = {read=FOverrideText, write=FOverrideText};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPBarsBase(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPBarsPostNet;
class PASCALIMPLEMENTATION TRPBarsPostNet : public TRPBarsBase 
{
	typedef TRPBarsBase inherited;
	
protected:
	virtual AnsiString __fastcall GetBarMask(char ch);
	virtual AnsiString __fastcall GetChecksum();
	virtual void __fastcall ModifyBar(char BarCh, double &Width, double &Height, double &Top);
	virtual double __fastcall GetWidth(void);
	virtual AnsiString __fastcall GetBarcodeText();
	virtual void __fastcall SetText(AnsiString Value);
	
public:
	__fastcall virtual TRPBarsPostNet(Rpbase::TBaseReport* BaseRpt);
	virtual bool __fastcall IsValidChar(char Ch);
	virtual void __fastcall Print(void);
	void __fastcall PrintFimA(double x, double y);
	void __fastcall PrintFimB(double x, double y);
	void __fastcall PrintFimC(double x, double y);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPBarsPostNet(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPBars2of5;
class PASCALIMPLEMENTATION TRPBars2of5 : public TRPBarsBase 
{
	typedef TRPBarsBase inherited;
	
protected:
	virtual AnsiString __fastcall GetBarMask(char ch);
	virtual AnsiString __fastcall GetChecksum();
	virtual void __fastcall ModifyBar(char BarCh, double &Width, double &Height, double &Top);
	virtual double __fastcall GetWidth(void);
	virtual AnsiString __fastcall GetBarcodeText();
	
public:
	__fastcall virtual TRPBars2of5(Rpbase::TBaseReport* BaseRpt);
	virtual bool __fastcall IsValidChar(char Ch);
	virtual void __fastcall Print(void);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPBars2of5(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPBarsCode39;
class PASCALIMPLEMENTATION TRPBarsCode39 : public TRPBarsBase 
{
	typedef TRPBarsBase inherited;
	
protected:
	bool FExtended;
	virtual AnsiString __fastcall GetBarMask(char ch);
	virtual AnsiString __fastcall GetChecksum();
	virtual void __fastcall ModifyBar(char BarCh, double &Width, double &Height, double &Top);
	virtual double __fastcall GetWidth(void);
	AnsiString __fastcall GetExtendedText();
	virtual AnsiString __fastcall GetBarcodeText();
	
public:
	__fastcall virtual TRPBarsCode39(Rpbase::TBaseReport* BaseRpt);
	virtual bool __fastcall IsValidChar(char Ch);
	virtual void __fastcall Print(void);
	__property bool Extended = {read=FExtended, write=FExtended, nodefault};
	__property AnsiString ExtendedText = {read=GetExtendedText};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPBarsCode39(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPBarsCode128;
class PASCALIMPLEMENTATION TRPBarsCode128 : public TRPBarsBase 
{
	typedef TRPBarsBase inherited;
	
protected:
	TCodePage128 FCodePage;
	TCodePage128 FCodePageUsed;
	TCodePage128 FUseCodePage;
	bool FOptimize;
	virtual AnsiString __fastcall GetBarMask(char Ch);
	virtual AnsiString __fastcall GetChecksum();
	virtual void __fastcall ModifyBar(char BarCh, double &Width, double &Height, double &Top);
	virtual double __fastcall GetWidth(void);
	virtual AnsiString __fastcall GetBarcodeText();
	AnsiString __fastcall CalcOutputStr(Byte MaxLen);
	AnsiString __fastcall GetOutputStr();
	char __fastcall CalcChecksum(AnsiString OutputStr);
	__property TCodePage128 UseCodePage = {read=FUseCodePage, write=FUseCodePage, nodefault};
	
public:
	__fastcall virtual TRPBarsCode128(Rpbase::TBaseReport* BaseRpt);
	virtual bool __fastcall IsValidChar(char Ch);
	virtual void __fastcall Print(void);
	__property TCodePage128 CodePage = {read=FCodePage, write=FCodePage, nodefault};
	__property bool Optimize = {read=FOptimize, write=FOptimize, nodefault};
	__property TCodePage128 CodePageUsed = {read=FCodePageUsed, write=FCodePageUsed, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPBarsCode128(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPBarsUPC;
class PASCALIMPLEMENTATION TRPBarsUPC : public TRPBarsBase 
{
	typedef TRPBarsBase inherited;
	
protected:
	bool FPrintingGuard;
	int ValidLen;
	virtual AnsiString __fastcall GetBarMask(char ch);
	virtual AnsiString __fastcall GetChecksum();
	virtual void __fastcall ModifyBar(char BarCh, double &Width, double &Height, double &Top);
	virtual double __fastcall GetWidth(void);
	virtual AnsiString __fastcall GetBarcodeText();
	virtual void __fastcall PrintReadableText(void);
	virtual double __fastcall GetFontSize(void);
	HIDESBASE double __fastcall GetBottom(void);
	HIDESBASE void __fastcall SetBottom(double Value);
	HIDESBASE double __fastcall GetReadableHeight(void);
	virtual void __fastcall SetUseChecksum(bool Value);
	virtual void __fastcall SetText(AnsiString Value);
	
public:
	__fastcall virtual TRPBarsUPC(Rpbase::TBaseReport* BaseRpt);
	virtual bool __fastcall IsValidChar(char Ch);
	virtual void __fastcall Print(void);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPBarsUPC(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPBarsEAN;
class PASCALIMPLEMENTATION TRPBarsEAN : public TRPBarsUPC 
{
	typedef TRPBarsUPC inherited;
	
public:
	__fastcall virtual TRPBarsEAN(Rpbase::TBaseReport* BaseRpt);
	virtual void __fastcall PrintReadableText(void);
	virtual double __fastcall GetFontSize(void);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPBarsEAN(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rpbars */
using namespace Rpbars;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpBars
