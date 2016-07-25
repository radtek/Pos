// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvCsDraw.pas' rev: 6.00

#ifndef RvCsDrawHPP
#define RvCsDrawHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <RvUtil.hpp>	// Pascal unit
#include <RvDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvcsdraw
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveGraphicBase;
class PASCALIMPLEMENTATION TRaveGraphicBase : public Rvclass::TRaveControl 
{
	typedef Rvclass::TRaveControl inherited;
	
protected:
	double PenWidth;
	Rvdefine::TLineWidthType PenWidthType;
	Graphics::TPen* GraphicPen;
	virtual void __fastcall SetForeColor(Graphics::TColor Value);
	virtual Graphics::TColor __fastcall GetForeColor(void);
	virtual void __fastcall SetLineWidth(double Value);
	virtual double __fastcall GetLineWidth(void);
	virtual void __fastcall SetLineWidthType(Rvdefine::TLineWidthType Value);
	virtual Rvdefine::TLineWidthType __fastcall GetLineWidthType(void);
	virtual void __fastcall SetLineStyle(Graphics::TPenStyle Value);
	virtual Graphics::TPenStyle __fastcall GetLineStyle(void);
	void __fastcall AssignPen(Graphics::TPen* Pen, Rpbase::TBaseReport* Report, int &PenAdjust, int &PenAdjust2);
	
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
	virtual bool __fastcall IsSelectPoint(const Rvdefine::TRavePoint &Point);
	virtual bool __fastcall InSelectRect(const Rvdefine::TRaveRect &Rect);
	virtual void __fastcall CreatePips(void);
	virtual void __fastcall UpdatePips(void);
	virtual void __fastcall PipSize(Rvclass::TRavePip* RavePip, Rvdefine::TRaveUnits X, Rvdefine::TRaveUnits Y);
	virtual void __fastcall Paint(Graphics::TCanvas* Canvas);
	virtual void __fastcall SetSize(const Rvdefine::TRavePoint &P1, const Rvdefine::TRavePoint &P2);
	virtual void __fastcall Print(Rpbase::TBaseReport* Report);
	
__published:
	__property Graphics::TColor Color = {read=GetForeColor, write=SetForeColor, default=0};
	__property Cursor  = {default=0};
	__property Height ;
	__property Left ;
	__property Graphics::TPenStyle LineStyle = {read=GetLineStyle, write=SetLineStyle, default=0};
	__property double LineWidth = {read=GetLineWidth, write=SetLineWidth};
	__property Rvdefine::TLineWidthType LineWidthType = {read=GetLineWidthType, write=SetLineWidthType, default=1};
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
	virtual bool __fastcall IsSelectPoint(const Rvdefine::TRavePoint &Point);
	virtual void __fastcall PipSize(Rvclass::TRavePip* RavePip, Rvdefine::TRaveUnits X, Rvdefine::TRaveUnits Y);
	/* virtual class method */ virtual void __fastcall ModifyRaveRect(TMetaClass* vmt, Rvdefine::TRavePoint &P1, Rvdefine::TRavePoint &P2);
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
	virtual bool __fastcall IsSelectPoint(const Rvdefine::TRavePoint &Point);
	virtual void __fastcall PipSize(Rvclass::TRavePip* RavePip, Rvdefine::TRaveUnits X, Rvdefine::TRaveUnits Y);
	/* virtual class method */ virtual void __fastcall ModifyRaveRect(TMetaClass* vmt, Rvdefine::TRavePoint &P1, Rvdefine::TRavePoint &P2);
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
	Graphics::TBrush* FillBrush;
	virtual void __fastcall SetBackColor(Graphics::TColor Value);
	virtual Graphics::TColor __fastcall GetBackColor(void);
	virtual void __fastcall SetFillStyle(Rvdefine::TRaveFillStyle Value);
	virtual Rvdefine::TRaveFillStyle __fastcall GetFillStyle(void);
	
public:
	__fastcall virtual TRaveSurface(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveSurface(void);
	__property Graphics::TColor BorderColor = {read=GetForeColor, write=SetForeColor, default=0};
	__property Graphics::TPenStyle BorderStyle = {read=GetLineStyle, write=SetLineStyle, default=0};
	__property double BorderWidth = {read=GetLineWidth, write=SetLineWidth};
	__property Rvdefine::TLineWidthType BorderWidthType = {read=GetLineWidthType, write=SetLineWidthType, default=1};
	__property Graphics::TColor FillColor = {read=GetBackColor, write=SetBackColor, default=16777215};
};


class DELPHICLASS TRaveRectangle;
class PASCALIMPLEMENTATION TRaveRectangle : public TRaveSurface 
{
	typedef TRaveSurface inherited;
	
protected:
	Rvdefine::TRaveUnits FHRadius;
	Rvdefine::TRaveUnits FVRadius;
	void __fastcall SetHRadius(Rvdefine::TRaveUnits Value);
	void __fastcall SetVRadius(Rvdefine::TRaveUnits Value);
	
public:
	__fastcall virtual TRaveRectangle(Classes::TComponent* AOwner);
	virtual void __fastcall Paint(Graphics::TCanvas* Canvas);
	virtual void __fastcall Print(Rpbase::TBaseReport* Report);
	
__published:
	__property BorderColor  = {default=0};
	__property BorderStyle  = {default=0};
	__property BorderWidth ;
	__property BorderWidthType  = {default=1};
	__property Cursor  = {default=0};
	__property FillColor  = {default=16777215};
	__property FillStyle ;
	__property Height ;
	__property Rvdefine::TRaveUnits HRadius = {read=FHRadius, write=SetHRadius};
	__property Left ;
	__property Top ;
	__property Rvdefine::TRaveUnits VRadius = {read=FVRadius, write=SetVRadius};
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
	/* virtual class method */ virtual void __fastcall ModifyRaveRect(TMetaClass* vmt, Rvdefine::TRavePoint &P1, Rvdefine::TRavePoint &P2);
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
	virtual void __fastcall Paint(Graphics::TCanvas* Canvas);
	virtual void __fastcall Print(Rpbase::TBaseReport* Report);
	
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
	/* virtual class method */ virtual void __fastcall ModifyRaveRect(TMetaClass* vmt, Rvdefine::TRavePoint &P1, Rvdefine::TRavePoint &P2);
public:
	#pragma option push -w-inl
	/* TRaveSurface.Destroy */ inline __fastcall virtual ~TRaveCircle(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Rvcsdraw */
using namespace Rvcsdraw;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvCsDraw
