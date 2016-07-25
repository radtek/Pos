// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvCsBars.pas' rev: 6.00

#ifndef QRvCsBarsHPP
#define QRvCsBarsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRvData.hpp>	// Pascal unit
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvDefine.hpp>	// Pascal unit
#include <QRpBars.hpp>	// Pascal unit
#include <QRvClass.hpp>	// Pascal unit
#include <QRpBase.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvcsbars
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveBaseBarCode;
class PASCALIMPLEMENTATION TRaveBaseBarCode : public Qrvclass::TRaveControl 
{
	typedef Qrvclass::TRaveControl inherited;
	
protected:
	AnsiString FOldText;
	Qrpbars::TRPBarsBase* FBarCode;
	Qrvclass::TRaveFont* FFont;
	Qgraphics::TCanvas* FCanvas;
	bool FAutoSize;
	AnsiString FDataField;
	Qrvdata::TRaveBaseDataView* FDataView;
	void __fastcall SetAutoSize(bool Value);
	virtual Qrvdefine::TRaveUnits __fastcall GetLeft(void);
	virtual void __fastcall SetLeft(Qrvdefine::TRaveUnits Value);
	HIDESBASE Qrvdefine::TRaveUnits __fastcall GetRight(void);
	HIDESBASE void __fastcall SetRight(Qrvdefine::TRaveUnits Value);
	virtual Qrvdefine::TRaveUnits __fastcall GetTop(void);
	virtual void __fastcall SetTop(Qrvdefine::TRaveUnits Value);
	virtual void __fastcall SetHeight(Qrvdefine::TRaveUnits Value);
	virtual void __fastcall SetWidth(Qrvdefine::TRaveUnits Value);
	void __fastcall SetFont(Qrvclass::TRaveFont* Value);
	void __fastcall SetBarCodeJustify(Qrpdefine::TPrintJustify Value);
	Qrpdefine::TPrintJustify __fastcall GetBarCodeJustify(void);
	void __fastcall SetTextJustify(Qrpdefine::TPrintJustify Value);
	Qrpdefine::TPrintJustify __fastcall GetTextJustify(void);
	void __fastcall SetBarWidth(Qrvdefine::TRaveUnits Value);
	Qrvdefine::TRaveUnits __fastcall GetBarWidth(void);
	void __fastcall SetBarHeight(Qrvdefine::TRaveUnits Value);
	Qrvdefine::TRaveUnits __fastcall GetBarHeight(void);
	Qrvdefine::TRaveUnits __fastcall GetBarTop(void);
	void __fastcall SetBarTop(Qrvdefine::TRaveUnits Value);
	Qrvdefine::TRaveUnits __fastcall GetCenter(void);
	void __fastcall SetCenter(Qrvdefine::TRaveUnits Value);
	virtual void __fastcall SetText(AnsiString Value);
	virtual AnsiString __fastcall GetText();
	void __fastcall SetUseChecksum(bool Value);
	bool __fastcall GetUseChecksum(void);
	void __fastcall SetWideFactor(double Value);
	double __fastcall GetWideFactor(void);
	void __fastcall SetPrintReadable(bool Value);
	bool __fastcall GetPrintReadable(void);
	void __fastcall SetPrintChecksum(bool Value);
	bool __fastcall GetPrintChecksum(void);
	void __fastcall SetPrintTop(bool Value);
	bool __fastcall GetPrintTop(void);
	void __fastcall SetBarCodeRotation(Qrpbars::TBarCodeRotation Value);
	Qrpbars::TBarCodeRotation __fastcall GetBarCodeRotation(void);
	void __fastcall PrintBar(double X1, double Y1, double X2, double Y2);
	void __fastcall SetDataField(AnsiString Value);
	virtual void __fastcall BeforeReport(void);
	virtual void __fastcall AfterReport(void);
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	bool __fastcall IsLeftStored(void);
	bool __fastcall IsCenterStored(void);
	bool __fastcall IsRightStored(void);
	virtual Qrpbars::TRPBarsBase* __fastcall CreateBarCodeClass(void) = 0 ;
	
public:
	__fastcall virtual TRaveBaseBarCode(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveBaseBarCode(void);
	virtual void __fastcall Resize(void);
	virtual void __fastcall Paint(Qgraphics::TCanvas* Canvas);
	virtual void __fastcall Print(Qrpbase::TBaseReport* Report);
	__property AnsiString Text = {read=GetText, write=SetText};
	
__published:
	__property AnsiString DataField = {read=FDataField, write=SetDataField};
	__property Qrvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView, default=0};
	__property bool AutoSize = {read=FAutoSize, write=SetAutoSize, default=1};
	__property Qrpdefine::TPrintJustify BarCodeJustify = {read=GetBarCodeJustify, write=SetBarCodeJustify, stored=false, nodefault};
	__property Qrvdefine::TRaveUnits Top = {read=GetTop, write=SetTop};
	__property Qrvdefine::TRaveUnits Bottom = {read=GetBottom, write=SetBottom};
	__property Height ;
	__property Width ;
	__property Qrvclass::TRaveFont* Font = {read=FFont, write=SetFont};
	__property Qrvdefine::TRaveUnits BarWidth = {read=GetBarWidth, write=SetBarWidth};
	__property Qrvdefine::TRaveUnits BarHeight = {read=GetBarHeight, write=SetBarHeight};
	__property Qrvdefine::TRaveUnits BarTop = {read=GetBarTop, write=SetBarTop};
	__property Left  = {stored=IsLeftStored};
	__property Qrvdefine::TRaveUnits Right = {read=GetRight, write=SetRight, stored=IsRightStored};
	__property Qrvdefine::TRaveUnits Center = {read=GetCenter, write=SetCenter, stored=IsCenterStored};
	__property Qrpdefine::TPrintJustify TextJustify = {read=GetTextJustify, write=SetTextJustify, default=1};
	__property bool UseChecksum = {read=GetUseChecksum, write=SetUseChecksum, default=0};
	__property double WideFactor = {read=GetWideFactor, write=SetWideFactor};
	__property bool PrintReadable = {read=GetPrintReadable, write=SetPrintReadable, default=1};
	__property bool PrintChecksum = {read=GetPrintChecksum, write=SetPrintChecksum, default=0};
	__property bool PrintTop = {read=GetPrintTop, write=SetPrintTop, default=0};
	__property Qrpbars::TBarCodeRotation BarCodeRotation = {read=GetBarCodeRotation, write=SetBarCodeRotation, default=0};
};


class DELPHICLASS TRavePostNetBarCode;
class PASCALIMPLEMENTATION TRavePostNetBarCode : public TRaveBaseBarCode 
{
	typedef TRaveBaseBarCode inherited;
	
protected:
	virtual Qrpbars::TRPBarsBase* __fastcall CreateBarCodeClass(void);
	virtual void __fastcall SetHeight(Qrvdefine::TRaveUnits Value);
	
public:
	__fastcall virtual TRavePostNetBarCode(Classes::TComponent* AOwner);
	
__published:
	__property PrintReadable  = {default=0};
	__property Height  = {write=SetHeight};
	__property Text ;
public:
	#pragma option push -w-inl
	/* TRaveBaseBarCode.Destroy */ inline __fastcall virtual ~TRavePostNetBarCode(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveI2of5BarCode;
class PASCALIMPLEMENTATION TRaveI2of5BarCode : public TRaveBaseBarCode 
{
	typedef TRaveBaseBarCode inherited;
	
protected:
	virtual Qrpbars::TRPBarsBase* __fastcall CreateBarCodeClass(void);
	
public:
	__fastcall virtual TRaveI2of5BarCode(Classes::TComponent* AOwner);
	
__published:
	__property Text ;
public:
	#pragma option push -w-inl
	/* TRaveBaseBarCode.Destroy */ inline __fastcall virtual ~TRaveI2of5BarCode(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveCode39BarCode;
class PASCALIMPLEMENTATION TRaveCode39BarCode : public TRaveBaseBarCode 
{
	typedef TRaveBaseBarCode inherited;
	
protected:
	void __fastcall SetExtended(bool Value);
	bool __fastcall GetExtended(void);
	virtual Qrpbars::TRPBarsBase* __fastcall CreateBarCodeClass(void);
	
public:
	__fastcall virtual TRaveCode39BarCode(Classes::TComponent* AOwner);
	
__published:
	__property bool Extended = {read=GetExtended, write=SetExtended, nodefault};
	__property Text ;
public:
	#pragma option push -w-inl
	/* TRaveBaseBarCode.Destroy */ inline __fastcall virtual ~TRaveCode39BarCode(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveCode128BarCode;
class PASCALIMPLEMENTATION TRaveCode128BarCode : public TRaveBaseBarCode 
{
	typedef TRaveBaseBarCode inherited;
	
protected:
	void __fastcall SetCodePage(Qrpbars::TCodePage128 Value);
	Qrpbars::TCodePage128 __fastcall GetCodePage(void);
	void __fastcall SetOptimize(bool Value);
	bool __fastcall GetOptimize(void);
	virtual Qrpbars::TRPBarsBase* __fastcall CreateBarCodeClass(void);
	
public:
	__fastcall virtual TRaveCode128BarCode(Classes::TComponent* AOwner);
	
__published:
	__property Qrpbars::TCodePage128 CodePage = {read=GetCodePage, write=SetCodePage, nodefault};
	__property bool Optimize = {read=GetOptimize, write=SetOptimize, nodefault};
	__property Text ;
public:
	#pragma option push -w-inl
	/* TRaveBaseBarCode.Destroy */ inline __fastcall virtual ~TRaveCode128BarCode(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveUPCBarCode;
class PASCALIMPLEMENTATION TRaveUPCBarCode : public TRaveBaseBarCode 
{
	typedef TRaveBaseBarCode inherited;
	
protected:
	virtual Qrpbars::TRPBarsBase* __fastcall CreateBarCodeClass(void);
	
public:
	__fastcall virtual TRaveUPCBarCode(Classes::TComponent* AOwner);
	
__published:
	__property Text ;
public:
	#pragma option push -w-inl
	/* TRaveBaseBarCode.Destroy */ inline __fastcall virtual ~TRaveUPCBarCode(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveEANBarCode;
class PASCALIMPLEMENTATION TRaveEANBarCode : public TRaveBaseBarCode 
{
	typedef TRaveBaseBarCode inherited;
	
protected:
	virtual Qrpbars::TRPBarsBase* __fastcall CreateBarCodeClass(void);
	
public:
	__fastcall virtual TRaveEANBarCode(Classes::TComponent* AOwner);
	
__published:
	__property Text ;
public:
	#pragma option push -w-inl
	/* TRaveBaseBarCode.Destroy */ inline __fastcall virtual ~TRaveEANBarCode(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Qrvcsbars */
using namespace Qrvcsbars;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvCsBars
