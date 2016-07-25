// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvCsDraw.pas' rev: 6.00

#ifndef QRvCsDrawHPP
#define QRvCsDrawHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpBase.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <QRvClass.hpp>	// Pascal unit
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvcsdraw
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveGraphicBase;
class PASCALIMPLEMENTATION TRaveGraphicBase : public Qrvclass::TRaveControl 
{
	typedef Qrvclass::TRaveControl inherited;
	
protected:
	double PenWidth;
	Qrvdefine::TLineWidthType PenWidthType;
	Qgraphics::TPen* GraphicPen;
	virtual void __fastcall SetForeColor(Qgraphics::TColor Value);
	virtual Qgraphics::TColor __fastcall GetForeColor(void);
	virtual void __fastcall SetLineWidth(double Value);
	virtual double __fastcall GetLineWidth(void);
	virtual void __fastcall SetLineWidthType(Qrvdefine::TLineWidthType Value);
	virtual Qrvdefine::TLineWidthType __fastcall GetLineWidthType(void);
	virtual void __fastcall SetLineStyle(Qgraphics::TPenStyle Value);
	virtual Qgraphics::TPenStyle __fastcall GetLineStyle(void);
	void __fastcall AssignPen(Qgraphics::TPen* Pen, Qrpbase::TBaseReport* Report, int &PenAdjust, int &PenAdjust2);
	
public:
	__fastcall virtual TRaveGraphicBase(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveGraphicBase(void);
};


class DELPHICLASS TRaveLine;
class PASCALIMPLEMENTATION TRaveLine : public TRaveGraphicBase 
{
	typedef TRaveGraphicBase inherited;
	
protected:
	bool FNESW;
	
public:
	__fastcall virtual TRaveLine(Classes::TComponent* AOwner);
	virtual bool __fastcall IsSelectPoint(const Qrvdefine::TRavePoint &Point);
	virtual bool __fastcall InSelectRect(const Qrvdefine::TRaveRect &Rect);
	virtual void __fastcall CreatePips(void);
	virtual void __fastcall UpdatePips(void);
	virtual void __fastcall PipSize(Qrvclass::TRavePip* RavePip, Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y);
	virtual void __fastcall Paint(Qgraphics::TCanvas* Canvas);
	virtual void __fastcall SetSize(const Qrvdefine::TRavePoint &P1, const Qrvdefine::TRavePoint &P2);
	virtual void __fastcall Print(Qrpbase::TBaseReport* Report);
	
__published:
	__property Qgraphics::TColor Color = {read=GetForeColor, write=SetForeColor, default=0};
	__property Cursor  = {default=0};
	__property Height ;
	__property Left ;
	__property Qgraphics::TPenStyle LineStyle = {read=GetLineStyle, write=SetLineStyle, default=0};
	__property double LineWidth = {read=GetLineWidth, write=SetLineWidth};
	__property Qrvdefine::TLineWidthType LineWidthType = {read=GetLineWidthType, write=SetLineWidthType, default=1};
	__property bool NESW = {read=FNESW, write=FNESW, nodefault};
	__property Top ;
	__property Width ;
public:
	#pragma option push -w-inl
	/* TRaveGraphicBase.Destroy */ inline __fastcall virtual ~TRaveLine(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveHLine;
class PASCALIMPLEMENTATION TRaveHLine : public TRaveLine 
{
	typedef TRaveLine inherited;
	
public:
	__fastcall virtual TRaveHLine(Classes::TComponent* AOwner);
	/* virtual class method */ virtual void __fastcall ModifyRect(TMetaClass* vmt, Types::TPoint &P1, Types::TPoint &P2, Byte PipIndex);
	virtual bool __fastcall IsSelectPoint(const Qrvdefine::TRavePoint &Point);
	virtual void __fastcall PipSize(Qrvclass::TRavePip* RavePip, Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y);
	/* virtual class method */ virtual void __fastcall ModifyRaveRect(TMetaClass* vmt, Qrvdefine::TRavePoint &P1, Qrvdefine::TRavePoint &P2);
public:
	#pragma option push -w-inl
	/* TRaveGraphicBase.Destroy */ inline __fastcall virtual ~TRaveHLine(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveVLine;
class PASCALIMPLEMENTATION TRaveVLine : public TRaveLine 
{
	typedef TRaveLine inherited;
	
public:
	__fastcall virtual TRaveVLine(Classes::TComponent* AOwner);
	/* virtual class method */ virtual void __fastcall ModifyRect(TMetaClass* vmt, Types::TPoint &P1, Types::TPoint &P2, Byte PipIndex);
	virtual bool __fastcall IsSelectPoint(const Qrvdefine::TRavePoint &Point);
	virtual void __fastcall PipSize(Qrvclass::TRavePip* RavePip, Qrvdefine::TRaveUnits X, Qrvdefine::TRaveUnits Y);
	/* virtual class method */ virtual void __fastcall ModifyRaveRect(TMetaClass* vmt, Qrvdefine::TRavePoint &P1, Qrvdefine::TRavePoint &P2);
public:
	#pragma option push -w-inl
	/* TRaveGraphicBase.Destroy */ inline __fastcall virtual ~TRaveVLine(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveSurface;
class PASCALIMPLEMENTATION TRaveSurface : public TRaveGraphicBase 
{
	typedef TRaveGraphicBase inherited;
	
protected:
	Qgraphics::TBrush* FillBrush;
	virtual void __fastcall SetBackColor(Qgraphics::TColor Value);
	virtual Qgraphics::TColor __fastcall GetBackColor(void);
	virtual void __fastcall SetFillStyle(Qrvdefine::TRaveFillStyle Value);
	virtual Qrvdefine::TRaveFillStyle __fastcall GetFillStyle(void);
	
public:
	__fastcall virtual TRaveSurface(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveSurface(void);
	__property Qgraphics::TColor BorderColor = {read=GetForeColor, write=SetForeColor, default=0};
	__property Qgraphics::TPenStyle BorderStyle = {read=GetLineStyle, write=SetLineStyle, default=0};
	__property double BorderWidth = {read=GetLineWidth, write=SetLineWidth};
	__property Qrvdefine::TLineWidthType BorderWidthType = {read=GetLineWidthType, write=SetLineWidthType, default=1};
	__property Qgraphics::TColor FillColor = {read=GetBackColor, write=SetBackColor, default=16777215};
};


class DELPHICLASS TRaveRectangle;
class PASCALIMPLEMENTATION TRaveRectangle : public TRaveSurface 
{
	typedef TRaveSurface inherited;
	
protected:
	Qrvdefine::TRaveUnits FHRadius;
	Qrvdefine::TRaveUnits FVRadius;
	void __fastcall SetHRadius(Qrvdefine::TRaveUnits Value);
	void __fastcall SetVRadius(Qrvdefine::TRaveUnits Value);
	
public:
	__fastcall virtual TRaveRectangle(Classes::TComponent* AOwner);
	virtual void __fastcall Paint(Qgraphics::TCanvas* Canvas);
	virtual void __fastcall Print(Qrpbase::TBaseReport* Report);
	
__published:
	__property BorderColor  = {default=0};
	__property BorderStyle  = {default=0};
	__property BorderWidth ;
	__property BorderWidthType  = {default=1};
	__property Cursor  = {default=0};
	__property FillColor  = {default=16777215};
	__property FillStyle ;
	__property Height ;
	__property Qrvdefine::TRaveUnits HRadius = {read=FHRadius, write=SetHRadius};
	__property Left ;
	__property Top ;
	__property Qrvdefine::TRaveUnits VRadius = {read=FVRadius, write=SetVRadius};
	__property Width ;
public:
	#pragma option push -w-inl
	/* TRaveSurface.Destroy */ inline __fastcall virtual ~TRaveRectangle(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveSquare;
class PASCALIMPLEMENTATION TRaveSquare : public TRaveRectangle 
{
	typedef TRaveRectangle inherited;
	
public:
	__fastcall virtual TRaveSquare(Classes::TComponent* AOwner);
	/* virtual class method */ virtual void __fastcall ModifyRect(TMetaClass* vmt, Types::TPoint &P1, Types::TPoint &P2, Byte PipIndex);
	virtual void __fastcall CreatePips(void);
	virtual void __fastcall UpdatePips(void);
	/* virtual class method */ virtual void __fastcall ModifyRaveRect(TMetaClass* vmt, Qrvdefine::TRavePoint &P1, Qrvdefine::TRavePoint &P2);
public:
	#pragma option push -w-inl
	/* TRaveSurface.Destroy */ inline __fastcall virtual ~TRaveSquare(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveEllipse;
class PASCALIMPLEMENTATION TRaveEllipse : public TRaveSurface 
{
	typedef TRaveSurface inherited;
	
public:
	__fastcall virtual TRaveEllipse(Classes::TComponent* AOwner);
	virtual void __fastcall Paint(Qgraphics::TCanvas* Canvas);
	virtual void __fastcall Print(Qrpbase::TBaseReport* Report);
	
__published:
	__property BorderColor  = {default=0};
	__property BorderStyle  = {default=0};
	__property BorderWidth ;
	__property BorderWidthType  = {default=1};
	__property Cursor  = {default=0};
	__property FillColor  = {default=16777215};
	__property FillStyle ;
	__property Height ;
	__property Left ;
	__property Top ;
	__property Width ;
public:
	#pragma option push -w-inl
	/* TRaveSurface.Destroy */ inline __fastcall virtual ~TRaveEllipse(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveCircle;
class PASCALIMPLEMENTATION TRaveCircle : public TRaveEllipse 
{
	typedef TRaveEllipse inherited;
	
public:
	__fastcall virtual TRaveCircle(Classes::TComponent* AOwner);
	/* virtual class method */ virtual void __fastcall ModifyRect(TMetaClass* vmt, Types::TPoint &P1, Types::TPoint &P2, Byte PipIndex);
	virtual void __fastcall CreatePips(void);
	virtual void __fastcall UpdatePips(void);
	/* virtual class method */ virtual void __fastcall ModifyRaveRect(TMetaClass* vmt, Qrvdefine::TRavePoint &P1, Qrvdefine::TRavePoint &P2);
public:
	#pragma option push -w-inl
	/* TRaveSurface.Destroy */ inline __fastcall virtual ~TRaveCircle(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Qrvcsdraw */
using namespace Qrvcsdraw;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvCsDraw
