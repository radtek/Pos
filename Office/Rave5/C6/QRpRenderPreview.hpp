// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpRenderPreview.pas' rev: 6.00

#ifndef QRpRenderPreviewHPP
#define QRpRenderPreviewHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpRenderCanvas.hpp>	// Pascal unit
#include <QRpRender.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <QExtCtrls.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrprenderpreview
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRvRenderPreview;
class PASCALIMPLEMENTATION TRvRenderPreview : public Qrprendercanvas::TRvRenderCanvas 
{
	typedef Qrprendercanvas::TRvRenderCanvas inherited;
	
protected:
	int FSaveHorzPos;
	int FSaveVertPos;
	Qcontrols::TCursor FSaveCursor;
	Qforms::TScrollBox* FScrollBox;
	Qextctrls::TImage* FImage;
	Qgraphics::TBitmap* FBitmap;
	int FZoomInc;
	bool FMonochrome;
	Qrpdefine::TMarginMethod FMarginMethod;
	double FMarginPercent;
	int FScrollBarWidth;
	int FScrollBarHeight;
	int FHorzAdj;
	int FVertAdj;
	Qgraphics::TPen* FGridPen;
	double FGridHoriz;
	double FGridVert;
	int FShadowDepth;
	Qrpdefine::TRulerType FRulerType;
	int RulerWidth;
	int RulerHeight;
	int FPageInc;
	Classes::TNotifyEvent FOnPageChange;
	Classes::TNotifyEvent FOnZoomChange;
	int FCurrentPage;
	int FPages;
	double FZoom;
	int AddX;
	int AddY;
	virtual Qgraphics::TCanvas* __fastcall GetCanvas(void);
	double __fastcall GetZoomFactor(void);
	void __fastcall SetGridPen(const Qgraphics::TPen* Value);
	void __fastcall SetMonochrome(const bool Value);
	void __fastcall SetScrollBox(const Qforms::TScrollBox* Value);
	void __fastcall SetShadowDepth(const int Value);
	void __fastcall SetZoomFactor(const double Value);
	void __fastcall SetZoomInc(const int Value);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	virtual void __fastcall PageChangeEvent(void);
	virtual void __fastcall ZoomChangeEvent(void);
	double __fastcall GetZoomPageFactor(void);
	double __fastcall GetZoomPageWidthFactor(void);
	virtual void __fastcall DocBegin(void);
	virtual void __fastcall DocEnd(void);
	virtual void __fastcall PageBegin(void);
	virtual void __fastcall PageEnd(void);
	virtual int __fastcall GetXDPI(void);
	virtual int __fastcall GetYDPI(void);
	int __fastcall GetPages(void);
	virtual double __fastcall CanvasTextWidth(const AnsiString AText);
	virtual void __fastcall PrintBitmapRect(const double X1, const double Y1, const double X2, const double Y2, Qgraphics::TBitmap* Graphic);
	virtual Qrpdefine::TDestination __fastcall Destination(void);
	
public:
	__fastcall virtual TRvRenderPreview(Classes::TComponent* AOwner);
	__fastcall virtual ~TRvRenderPreview(void);
	void __fastcall ZoomIn(void);
	void __fastcall ZoomOut(void);
	void __fastcall Clear(void);
	void __fastcall RedrawPage(void);
	void __fastcall PrevPage(void);
	void __fastcall NextPage(void);
	virtual void __fastcall RenderPage(int PageNum);
	virtual int __fastcall XI2D(double Pos);
	virtual int __fastcall YI2D(double Pos);
	__property double ZoomPageWidthFactor = {read=GetZoomPageWidthFactor};
	__property double ZoomPageFactor = {read=GetZoomPageFactor};
	__property Qextctrls::TImage* Image = {read=FImage};
	__property int CurrentPage = {read=FCurrentPage, write=FCurrentPage, nodefault};
	__property int Pages = {read=GetPages, write=FPages, nodefault};
	
__published:
	__property Active  = {default=0};
	__property Qforms::TScrollBox* ScrollBox = {read=FScrollBox, write=SetScrollBox};
	__property int ZoomInc = {read=FZoomInc, write=SetZoomInc, default=10};
	__property double ZoomFactor = {read=GetZoomFactor, write=SetZoomFactor};
	__property bool Monochrome = {read=FMonochrome, write=SetMonochrome, default=0};
	__property Qrpdefine::TMarginMethod MarginMethod = {read=FMarginMethod, write=FMarginMethod, default=1};
	__property double MarginPercent = {read=FMarginPercent, write=FMarginPercent};
	__property Qrpdefine::TRulerType RulerType = {read=FRulerType, write=FRulerType, default=0};
	__property Qgraphics::TPen* GridPen = {read=FGridPen, write=SetGridPen};
	__property double GridHoriz = {read=FGridHoriz, write=FGridHoriz};
	__property double GridVert = {read=FGridVert, write=FGridVert};
	__property int ShadowDepth = {read=FShadowDepth, write=SetShadowDepth, nodefault};
	__property int PageInc = {read=FPageInc, write=FPageInc, default=1};
	__property Classes::TNotifyEvent OnZoomChange = {read=FOnZoomChange, write=FOnZoomChange};
	__property Classes::TNotifyEvent OnPageChange = {read=FOnPageChange, write=FOnPageChange};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Qrprenderpreview */
using namespace Qrprenderpreview;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpRenderPreview
