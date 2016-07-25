// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpPower.pas' rev: 6.00

#ifndef OpPowerHPP
#define OpPowerHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <DB.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <OpShared.hpp>	// Pascal unit
#include <OpOfcXP.hpp>	// Pascal unit
#include <OpPptXP.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Oppower
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TOpPpTransitionSpeed { pptsMixed, pptsSlow, pptsMedium, pptsFast };
#pragma option pop

#pragma option push -b-
enum TOpPpEntryEffect { ppeeNone, ppeeCut, ppeeCutThroughBlack, ppeeRandom, ppeeBlindsHorizontal, ppeeBlindsVertical, ppeeCheckerboardAcross, ppeeCheckerboardDown, ppeeCoverLeft, ppeeCoverUp, ppeeCoverRight, ppeeCoverDown, ppeeCoverLeftUp, ppeeCoverRightUp, ppeeCoverLeftDown, ppeeCoverRightDown, ppeeDissolve, ppeeFade, ppeeUncoverLeft, ppeeUncoverUp, ppeeUncoverRight, ppeeUncoverDown, ppeeUncoverLeftUp, ppeeUncoverRightUp, ppeeUncoverLeftDown, ppeeUncoverRightDown, ppeeRandomBarsHorizontal, ppeeRandomBarsVertical, ppeeStripsUpLeft, ppeeStripsUpRight, ppeeStripsDownLeft, ppeeStripsDownRight, ppeeStripsLeftUp, ppeeStripsRightUp, ppeeStripsLeftDown, ppeeStripsRightDown, ppeeWipeLeft, ppeeWipeUp, ppeeWipeRight, ppeeWipeDown, ppeeBoxOut, ppeeBoxIn, ppeeSplitHorizontalOut, ppeeSplitHorizontalIn, ppeeSplitVerticalOut, ppeeSplitVerticalIn, ppeeAppear };
#pragma option pop

#pragma option push -b-
enum TOpPpDirection { ppdMixed, ppdLeftToRight, ppdRightToLeft };
#pragma option pop

#pragma option push -b-
enum TOpPpSlideLayout { ppslTitle, ppslText, ppslTwoColumnText, ppslTable, ppslTextAndChart, ppslChartAndText, ppslOrgchart, ppslChart, ppslTextAndClipart, ppslClipartAndText, ppslTitleOnly, ppslBlank, ppslTextAndObject, ppslObjectAndText, ppslLargeObject, ppslObject, ppslTextAndMediaClip, ppslMediaClipAndText, ppslObjectOverText, ppslTextOverObject, ppslTextAndTwoObjects, ppslTwoObjectsAndText, ppslTwoObjectsOverText, ppslFourObjects, ppslVerticalText, ppslClipArtAndVerticalText, ppslVerticalTitleAndText, ppslVerticalTitleAndTextOverChart };
#pragma option pop

class DELPHICLASS TOpSlideTransition;
class DELPHICLASS TOpSlide;
class PASCALIMPLEMENTATION TOpSlide : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
protected:
	TOpPpSlideLayout FLayout;
	WideString FSlideName;
	bool FDisplayMasterShapes;
	bool FFollowMasterBackground;
	TOpSlideTransition* FSlideTransition;
	Oppptxp::_di__Slide __fastcall GetAsSlide();
	TOpPpSlideLayout __fastcall GetLayout(void);
	void __fastcall SetLayout(const TOpPpSlideLayout Value);
	bool __fastcall GetDisplayMasterShapes(void);
	bool __fastcall GetFollowMasterBackground(void);
	WideString __fastcall GetName();
	void __fastcall SetDisplayMasterShapes(const bool Value);
	void __fastcall SetFollowMasterBackground(const bool Value);
	void __fastcall SetName(const WideString Value);
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	
public:
	__fastcall virtual TOpSlide(Classes::TCollection* Collection);
	__fastcall virtual ~TOpSlide(void);
	virtual void __fastcall Connect(void);
	virtual void __fastcall Activate(void);
	__property Oppptxp::_di__Slide AsSlide = {read=GetAsSlide};
	
__published:
	__property TOpPpSlideLayout Layout = {read=GetLayout, write=SetLayout, nodefault};
	__property WideString SlideName = {read=GetName, write=SetName};
	__property bool DisplayMasterShapes = {read=GetDisplayMasterShapes, write=SetDisplayMasterShapes, nodefault};
	__property bool FollowMasterBackground = {read=GetFollowMasterBackground, write=SetFollowMasterBackground, nodefault};
	__property TOpSlideTransition* TransitionEffect = {read=FSlideTransition, write=FSlideTransition};
};


class PASCALIMPLEMENTATION TOpSlideTransition : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	TOpSlide* FSlideComponent;
	float FAdvanceTime;
	bool FAdvanceOnTime;
	bool FAdvanceOnClick;
	TOpPpEntryEffect FEntryEffect;
	TOpPpTransitionSpeed FTransSpeed;
	void __fastcall SetAdvanceOnClick(const bool Value);
	void __fastcall SetAdvanceOnTime(const bool Value);
	void __fastcall SetAdvanceTime(const float Value);
	void __fastcall SetEntryEffect(const TOpPpEntryEffect Value);
	void __fastcall SetTransSpeed(const TOpPpTransitionSpeed Value);
	DYNAMIC Classes::TPersistent* __fastcall GetOwner(void);
	bool __fastcall GetAdvanceOnClick(void);
	bool __fastcall GetAdvanceOnTime(void);
	float __fastcall GetAdvanceTime(void);
	
public:
	__fastcall TOpSlideTransition(TOpSlide* SlideComp);
	
__published:
	__property bool AdvanceOnClick = {read=GetAdvanceOnClick, write=SetAdvanceOnClick, nodefault};
	__property bool AdvanceOnTime = {read=GetAdvanceOnTime, write=SetAdvanceOnTime, nodefault};
	__property float AdvanceTime = {read=GetAdvanceTime, write=SetAdvanceTime};
	__property TOpPpEntryEffect EntryEffect = {read=FEntryEffect, write=SetEntryEffect, nodefault};
	__property TOpPpTransitionSpeed Speed = {read=FTransSpeed, write=SetTransSpeed, nodefault};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TOpSlideTransition(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpSlides;
class PASCALIMPLEMENTATION TOpSlides : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpSlide* operator[](int index) { return Items[index]; }
	
protected:
	HIDESBASE TOpSlide* __fastcall GetItem(int index);
	HIDESBASE void __fastcall SetItem(int index, const TOpSlide* Value);
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpSlide* __fastcall Add(void);
	__property TOpSlide* Items[int index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpSlides(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpSlides(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpPresentation;
class PASCALIMPLEMENTATION TOpPresentation : public Opshared::TOpNestedCollectionItem 
{
	typedef Opshared::TOpNestedCollectionItem inherited;
	
protected:
	TOpSlides* FSlides;
	AnsiString FPresFile;
	bool FSaved;
	TOpPpDirection FLayoutDirection;
	bool FManualAdvance;
	void __fastcall SetPresFile(const AnsiString Value);
	void __fastcall SetManualAdvance(const bool Value);
	bool __fastcall GetManualAdvance(void);
	TOpPpDirection __fastcall GetLayoutDirection(void);
	bool __fastcall GetSaved(void);
	void __fastcall SetLayoutDirection(const TOpPpDirection Value);
	void __fastcall SetSaved(const bool Value);
	bool __fastcall SaveCollection(void);
	Opshared::TOpPropDirection __fastcall GetPropDirection(void);
	Oppptxp::_di__Presentation __fastcall GetAsPresentation();
	virtual Classes::TCollection* __fastcall GetSubCollection(int index);
	virtual int __fastcall GetSubCollectionCount(void);
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	
public:
	__fastcall virtual TOpPresentation(Classes::TCollection* Collection);
	__fastcall virtual ~TOpPresentation(void);
	virtual void __fastcall Connect(void);
	virtual void __fastcall ExecuteVerb(int index);
	void __fastcall RunSlideShow(void);
	void __fastcall Save(void);
	void __fastcall SaveAs(const AnsiString FileName);
	__property Oppptxp::_di__Presentation AsPresentation = {read=GetAsPresentation};
	__property Opshared::TOpPropDirection PropDirection = {read=GetPropDirection, nodefault};
	
__published:
	__property bool ManualAdvance = {read=GetManualAdvance, write=SetManualAdvance, nodefault};
	__property TOpSlides* Slides = {read=FSlides, write=FSlides, stored=SaveCollection};
	__property AnsiString PresentationFile = {read=FPresFile, write=SetPresFile};
	__property bool Saved = {read=GetSaved, write=SetSaved, stored=SaveCollection, nodefault};
	__property TOpPpDirection LayoutDirection = {read=GetLayoutDirection, write=SetLayoutDirection, stored=SaveCollection, nodefault};
};


class DELPHICLASS TOpPresentations;
class PASCALIMPLEMENTATION TOpPresentations : public Opshared::TOpNestedCollection 
{
	typedef Opshared::TOpNestedCollection inherited;
	
public:
	TOpPresentation* operator[](int index) { return Items[index]; }
	
protected:
	HIDESBASE TOpPresentation* __fastcall GetItem(int index);
	HIDESBASE void __fastcall SetItem(int index, const TOpPresentation* Value);
	virtual AnsiString __fastcall GetItemName();
	
public:
	HIDESBASE TOpPresentation* __fastcall Add(void);
	__property TOpPresentation* Items[int index] = {read=GetItem, write=SetItem/*, default*/};
public:
	#pragma option push -w-inl
	/* TOpNestedCollection.Create */ inline __fastcall virtual TOpPresentations(Classes::TComponent* RootComponent, Classes::TPersistent* Owner, TMetaClass* ItemClass) : Opshared::TOpNestedCollection(RootComponent, Owner, ItemClass) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TOpPresentations(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpPowerPoint;
class PASCALIMPLEMENTATION TOpPowerPoint : public Oppptxp::TOpPowerPointBase 
{
	typedef Oppptxp::TOpPowerPointBase inherited;
	
protected:
	bool FVisible;
	AnsiString FCaption;
	TOpPresentations* FPresentations;
	Oppptxp::_di__Application FServer;
	int FLeft;
	int FWidth;
	int FHeight;
	int FTop;
	void __fastcall FixupCollection(Opshared::TOpNestedCollectionItem* Item);
	AnsiString __fastcall GetCaption();
	void __fastcall Setcaption(const AnsiString Value);
	int __fastcall GetHeight(void);
	int __fastcall GetLeft(void);
	int __fastcall GetTop(void);
	int __fastcall GetWidth(void);
	void __fastcall SetHeight(const int Value);
	void __fastcall SetLeft(const int Value);
	void __fastcall SetTop(const int Value);
	void __fastcall SetWidth(const int Value);
	virtual void __fastcall DoConnect(void);
	virtual void __fastcall DoDisconnect(void);
	virtual void __fastcall FixupProps(void);
	void __fastcall SetVisible(const bool Value);
	virtual bool __fastcall GetConnected(void);
	virtual AnsiString __fastcall GetOfficeVersionStr();
	void __fastcall ReleaseCollectionInterface(Opshared::TOpNestedCollectionItem* Item);
	int __fastcall PointsToPixels(float Points);
	float __fastcall PixelsToPoints(int Pixels);
	int __fastcall PixelsPerInch(void);
	TOpPresentation* __fastcall GetActivePresentation(void);
	
public:
	__fastcall virtual TOpPowerPoint(Classes::TComponent* AOwner);
	__fastcall virtual ~TOpPowerPoint(void);
	virtual _di_IDispatch __fastcall CreateItem(Opshared::TOpNestedCollectionItem* Item);
	virtual void __fastcall GetAppInfo(Classes::TStrings* Info);
	virtual void __fastcall GetFileInfo(AnsiString &Filter, AnsiString &DefExt);
	virtual void __fastcall HandleEvent(const GUID &IID, int DispId, const tagVARIANT * Params);
	TOpPresentation* __fastcall NewPresentation(void);
	TOpPresentation* __fastcall OpenPresentation(const AnsiString FileName);
	void __fastcall Quit(void);
	__property TOpPresentation* ActivePresentation = {read=GetActivePresentation};
	__property Oppptxp::_di__Application Server = {read=FServer};
	
__published:
	__property AnsiString Caption = {read=GetCaption, write=Setcaption};
	__property TOpPresentations* Presentations = {read=FPresentations, write=FPresentations};
	__property int ServerLeft = {read=GetLeft, write=SetLeft, nodefault};
	__property int ServerTop = {read=GetTop, write=SetTop, nodefault};
	__property int ServerWidth = {read=GetWidth, write=SetWidth, nodefault};
	__property int ServerHeight = {read=GetHeight, write=SetHeight, nodefault};
	__property bool Visible = {read=FVisible, write=SetVisible, nodefault};
	__property Connected  = {default=0};
	__property MachineName ;
	__property OnOpConnect ;
	__property OnOpDisconnect ;
	__property OnGetInstance ;
	__property PropDirection  = {default=0};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Oppower */
using namespace Oppower;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpPower
