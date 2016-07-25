// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Rpreview.pas' rev: 6.00

#ifndef RpreviewHPP
#define RpreviewHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpDevice.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <RpFPrint.hpp>	// Pascal unit
#include <RpCanvas.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpreview
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TFilePreview;
class PASCALIMPLEMENTATION TFilePreview : public Rpfprint::TFilePrinter 
{
	typedef Rpfprint::TFilePrinter inherited;
	
protected:
	Forms::TScrollBox* FScrollBox;
	Extctrls::TImage* FImage;
	Graphics::TBitmap* FBitmap;
	int FZoomInc;
	bool FMonochrome;
	Rpdefine::TMarginMethod FMarginMethod;
	double FMarginPercent;
	int FScrollBarWidth;
	int FScrollBarHeight;
	int FHorzAdj;
	int FVertAdj;
	Graphics::TPen* FGridPen;
	double FGridHoriz;
	double FGridVert;
	int FShadowDepth;
	Rpdefine::TRulerType FRulerType;
	int RulerWidth;
	int RulerHeight;
	int FPageInc;
	Classes::TNotifyEvent FOnPageChange;
	Classes::TNotifyEvent FOnZoomChange;
	virtual void __fastcall PageChangeEvent(void);
	virtual void __fastcall ZoomChangeEvent(void);
	virtual Rpdefine::TDestination __fastcall Destination(void);
	void __fastcall AbsorbIntProp(Classes::TReader* Reader);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	double __fastcall GetZoomFactor(void);
	void __fastcall SetZoomFactor(double Value);
	void __fastcall SetZoomInc(int Value);
	double __fastcall GetZoomPageWidthFactor(void);
	double __fastcall GetZoomPageFactor(void);
	void __fastcall SetMonochrome(bool Value);
	void __fastcall SetShadowDepth(int Value);
	void __fastcall SetGridPen(Graphics::TPen* Value);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	void __fastcall SetScrollBox(Forms::TScrollBox* Value);
	
public:
	__fastcall virtual TFilePreview(Classes::TComponent* AOwner);
	__fastcall virtual ~TFilePreview(void);
	virtual void __fastcall PrintPage(Word PageNum);
	void __fastcall PrevPage(void);
	void __fastcall NextPage(void);
	void __fastcall ZoomIn(void);
	void __fastcall ZoomOut(void);
	void __fastcall RedrawPage(void);
	void __fastcall Clear(void);
	virtual void __fastcall Start(void);
	virtual void __fastcall Finish(void);
	virtual void __fastcall Execute(void);
	__property CurrentPage ;
	__property double ZoomPageWidthFactor = {read=GetZoomPageWidthFactor};
	__property double ZoomPageFactor = {read=GetZoomPageFactor};
	__property Extctrls::TImage* Image = {read=FImage};
	
__published:
	__property Forms::TScrollBox* ScrollBox = {read=FScrollBox, write=SetScrollBox};
	__property int ZoomInc = {read=FZoomInc, write=SetZoomInc, default=10};
	__property double ZoomFactor = {read=GetZoomFactor, write=SetZoomFactor};
	__property bool Monochrome = {read=FMonochrome, write=SetMonochrome, default=0};
	__property Rpdefine::TMarginMethod MarginMethod = {read=FMarginMethod, write=FMarginMethod, default=1};
	__property double MarginPercent = {read=FMarginPercent, write=FMarginPercent};
	__property Rpdefine::TRulerType RulerType = {read=FRulerType, write=FRulerType, default=0};
	__property Graphics::TPen* GridPen = {read=FGridPen, write=SetGridPen};
	__property double GridHoriz = {read=FGridHoriz, write=FGridHoriz};
	__property double GridVert = {read=FGridVert, write=FGridVert};
	__property int ShadowDepth = {read=FShadowDepth, write=SetShadowDepth, nodefault};
	__property int PageInc = {read=FPageInc, write=FPageInc, default=1};
	__property Classes::TNotifyEvent OnZoomChange = {read=FOnZoomChange, write=FOnZoomChange};
	__property Classes::TNotifyEvent OnPageChange = {read=FOnPageChange, write=FOnPageChange};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rpreview */
using namespace Rpreview;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Rpreview
