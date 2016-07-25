// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvCsStd.pas' rev: 6.00

#ifndef RvCsStdHPP
#define RvCsStdHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvDataField.hpp>	// Pascal unit
#include <RpMemo.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <RvData.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <RvUtil.hpp>	// Pascal unit
#include <RvDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvcsstd
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveFontMaster;
class PASCALIMPLEMENTATION TRaveFontMaster : public Rvclass::TRaveComponent 
{
	typedef Rvclass::TRaveComponent inherited;
	
protected:
	Rvclass::TRaveFont* FFont;
	void __fastcall SetFont(Rvclass::TRaveFont* Value);
	
public:
	__fastcall virtual TRaveFontMaster(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveFontMaster(void);
	
__published:
	__property Rvclass::TRaveFont* Font = {read=FFont, write=SetFont};
};


class DELPHICLASS TRavePageNumInit;
class PASCALIMPLEMENTATION TRavePageNumInit : public Rvclass::TRaveComponent 
{
	typedef Rvclass::TRaveComponent inherited;
	
protected:
	int FInitValue;
	Rvdata::TRaveBaseDataView* FInitDataView;
	AnsiString FInitDataField;
	virtual void __fastcall Changing(Rvclass::TRaveComponent* OldItem, Rvclass::TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRavePageNumInit(Classes::TComponent* AOwner);
	virtual void __fastcall Print(Rpbase::TBaseReport* AReport);
	
__published:
	__property int InitValue = {read=FInitValue, write=FInitValue, default=1};
	__property Rvdata::TRaveBaseDataView* InitDataView = {read=FInitDataView, write=FInitDataView, default=0};
	__property AnsiString InitDataField = {read=FInitDataField, write=FInitDataField};
public:
	#pragma option push -w-inl
	/* TRaveComponent.Destroy */ inline __fastcall virtual ~TRavePageNumInit(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveCustomText;
class PASCALIMPLEMENTATION TRaveCustomText : public Rvclass::TRaveControl 
{
	typedef Rvclass::TRaveControl inherited;
	
protected:
	Rvclass::TRaveFont* FFont;
	Rpdefine::TPrintJustify FFontJustify;
	AnsiString FText;
	TRaveFontMaster* FFontMirror;
	Rvclass::TRaveStringEvent* FOnGetText;
	bool BracketText;
	HFONT __fastcall MakeFont(void);
	void __fastcall PaintText(unsigned Handle, AnsiString Text, Rvdefine::TRaveUnits PaintLeft, Rvdefine::TRaveUnits PaintRight, Rvdefine::TRaveUnits &PaintTop, bool Truncate = false);
	virtual void __fastcall SetFontAttr(Rvdefine::TFontAttribute Index, Rvclass::TRaveFont* Value);
	virtual Rvclass::TRaveFont* __fastcall GetFontAttr(Rvdefine::TFontAttribute Index);
	virtual void __fastcall SetText(AnsiString Value);
	virtual AnsiString __fastcall GetText();
	void __fastcall SetFont(Rvclass::TRaveFont* Value);
	virtual void __fastcall SetFontJustify(Rpdefine::TPrintJustify Value);
	virtual Rpdefine::TPrintJustify __fastcall GetFontJustify(void);
	virtual void __fastcall SetForeColor(Graphics::TColor Value);
	virtual Graphics::TColor __fastcall GetForeColor(void);
	void __fastcall SetFontMirror(TRaveFontMaster* Value);
	virtual void __fastcall Changing(Rvclass::TRaveComponent* OldItem, Rvclass::TRaveComponent* NewItem);
	void __fastcall DoGetText(AnsiString &Value);
	
public:
	__fastcall virtual TRaveCustomText(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveCustomText(void);
	__property TRaveFontMaster* FontMirror = {read=FFontMirror, write=SetFontMirror};
	__property Graphics::TColor Color = {read=GetForeColor, write=SetForeColor, default=0};
	__property Rvclass::TRaveFont* Font = {read=FFont, write=SetFont};
};


class DELPHICLASS TRaveText;
class PASCALIMPLEMENTATION TRaveText : public TRaveCustomText 
{
	typedef TRaveCustomText inherited;
	
protected:
	bool FTruncate;
	int FRotation;
	virtual Rvdefine::TRaveUnits __fastcall GetHeight(void);
	
public:
	__fastcall virtual TRaveText(Classes::TComponent* AOwner);
	virtual void __fastcall CreatePips(void);
	virtual void __fastcall UpdatePips(void);
	virtual void __fastcall Paint(Graphics::TCanvas* Canvas);
	virtual void __fastcall Print(Rpbase::TBaseReport* Report);
	
__published:
	__property Color  = {default=0};
	__property Font ;
	__property FontJustify ;
	__property FontMirror ;
	__property Left ;
	__property int Rotation = {read=FRotation, write=FRotation, default=0};
	__property Text ;
	__property Top ;
	__property bool Truncate = {read=FTruncate, write=FTruncate, default=0};
	__property Width ;
	__property Rvclass::TRaveStringEvent* OnGetText = {read=FOnGetText, write=FOnGetText};
public:
	#pragma option push -w-inl
	/* TRaveCustomText.Destroy */ inline __fastcall virtual ~TRaveText(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRavePrintEvent;
class PASCALIMPLEMENTATION TRavePrintEvent : public Rvclass::TRaveSimpleEvent 
{
	typedef Rvclass::TRaveSimpleEvent inherited;
	
protected:
	Rpbase::TBaseReport* FReport;
	virtual bool __fastcall SuspendValid(void);
	
public:
	virtual void __fastcall DefineParams(void);
	virtual AnsiString __fastcall ParamSource();
	
__published:
	__property Rpbase::TBaseReport* Report = {read=FReport, write=FReport};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TRavePrintEvent(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRavePrintEvent(void) : Rvclass::TRaveSimpleEvent() { }
	#pragma option pop
	
};


class DELPHICLASS TRaveSection;
class PASCALIMPLEMENTATION TRaveSection : public Rvclass::TRaveContainerControl 
{
	typedef Rvclass::TRaveContainerControl inherited;
	
protected:
	bool FWasteFit;
	TRavePrintEvent* FOnPrint;
	virtual void __fastcall BeforePrint(void);
	virtual void __fastcall Print(Rpbase::TBaseReport* Report);
	virtual bool __fastcall GetDonePrinting(void);
	
public:
	__fastcall virtual TRaveSection(Classes::TComponent* AOwner);
	virtual void __fastcall Paint(Graphics::TCanvas* Canvas);
	
__published:
	__property Height ;
	__property Left ;
	__property Top ;
	__property bool WasteFit = {read=FWasteFit, write=FWasteFit, default=0};
	__property Width ;
	__property TRavePrintEvent* OnPrint = {read=FOnPrint, write=FOnPrint};
public:
	#pragma option push -w-inl
	/* TRaveControl.Destroy */ inline __fastcall virtual ~TRaveSection(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TMatchSide { msWidth, msHeight, msBoth, msInside };
#pragma option pop

class DELPHICLASS TRaveBaseImage;
class PASCALIMPLEMENTATION TRaveBaseImage : public Rvclass::TRaveControl 
{
	typedef Rvclass::TRaveControl inherited;
	
protected:
	TMatchSide FMatchSide;
	bool FMatchHeight;
	AnsiString FFileLink;
	AnsiString FDataField;
	Rvdata::TRaveBaseDataView* FDataView;
	void __fastcall SetFileLink(AnsiString Value);
	void __fastcall SetDataField(AnsiString Value);
	void __fastcall SetMatchSide(TMatchSide Value);
	void __fastcall LoadFromFile(AnsiString FileName);
	virtual void __fastcall Changing(Rvclass::TRaveComponent* OldItem, Rvclass::TRaveComponent* NewItem);
	virtual void __fastcall LoadFromStream(Classes::TStream* Stream) = 0 ;
	virtual bool __fastcall ImageValid(void) = 0 ;
	virtual void __fastcall UnloadAltImage(void) = 0 ;
	virtual int __fastcall ImageWidth(void) = 0 ;
	virtual int __fastcall ImageHeight(void) = 0 ;
	virtual void __fastcall PaintImage(Graphics::TCanvas* Canvas, const Types::TRect &Dest) = 0 ;
	virtual void __fastcall PrintImage(Rpbase::TBaseReport* Report, Rvdefine::TRaveUnits X1, Rvdefine::TRaveUnits Y1, Rvdefine::TRaveUnits X2, Rvdefine::TRaveUnits Y2) = 0 ;
	
public:
	__fastcall virtual TRaveBaseImage(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveBaseImage(void);
	virtual void __fastcall Paint(Graphics::TCanvas* Canvas);
	virtual void __fastcall Print(Rpbase::TBaseReport* Report);
	virtual AnsiString __fastcall ImageName();
	virtual AnsiString __fastcall ImageFilter();
	
__published:
	__property Height ;
	__property Left ;
	__property Top ;
	__property Width ;
	__property TMatchSide MatchSide = {read=FMatchSide, write=SetMatchSide, default=0};
	__property AnsiString FileLink = {read=FFileLink, write=SetFileLink};
	__property AnsiString DataField = {read=FDataField, write=SetDataField};
	__property Rvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView, default=0};
};


class DELPHICLASS TRaveGraphicImage;
class PASCALIMPLEMENTATION TRaveGraphicImage : public TRaveBaseImage 
{
	typedef TRaveBaseImage inherited;
	
protected:
	Graphics::TGraphic* FImage;
	Graphics::TGraphic* FAltImage;
	virtual Graphics::TGraphic* __fastcall CreateGraphic(void) = 0 ;
	void __fastcall SetImage(Graphics::TGraphic* Value);
	Graphics::TGraphic* __fastcall ActiveImage(void);
	virtual void __fastcall LoadFromStream(Classes::TStream* Stream);
	virtual void __fastcall UnloadAltImage(void);
	virtual int __fastcall ImageWidth(void);
	virtual int __fastcall ImageHeight(void);
	virtual void __fastcall PaintImage(Graphics::TCanvas* Canvas, const Types::TRect &Dest);
	
public:
	__fastcall virtual TRaveGraphicImage(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveGraphicImage(void);
	
__published:
	__property Graphics::TGraphic* Image = {read=FImage, write=SetImage};
};


class DELPHICLASS TRaveBitmap;
class PASCALIMPLEMENTATION TRaveBitmap : public TRaveGraphicImage 
{
	typedef TRaveGraphicImage inherited;
	
protected:
	virtual Graphics::TGraphic* __fastcall CreateGraphic(void);
	virtual bool __fastcall ImageValid(void);
	virtual void __fastcall PrintImage(Rpbase::TBaseReport* Report, Rvdefine::TRaveUnits X1, Rvdefine::TRaveUnits Y1, Rvdefine::TRaveUnits X2, Rvdefine::TRaveUnits Y2);
	
public:
	virtual AnsiString __fastcall ImageName();
	virtual AnsiString __fastcall ImageFilter();
public:
	#pragma option push -w-inl
	/* TRaveGraphicImage.Create */ inline __fastcall virtual TRaveBitmap(Classes::TComponent* AOwner) : TRaveGraphicImage(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveGraphicImage.Destroy */ inline __fastcall virtual ~TRaveBitmap(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveMetaFile;
class PASCALIMPLEMENTATION TRaveMetaFile : public TRaveGraphicImage 
{
	typedef TRaveGraphicImage inherited;
	
protected:
	virtual Graphics::TGraphic* __fastcall CreateGraphic(void);
	virtual bool __fastcall ImageValid(void);
	virtual void __fastcall PrintImage(Rpbase::TBaseReport* Report, Rvdefine::TRaveUnits X1, Rvdefine::TRaveUnits Y1, Rvdefine::TRaveUnits X2, Rvdefine::TRaveUnits Y2);
	
public:
	virtual AnsiString __fastcall ImageName();
	virtual AnsiString __fastcall ImageFilter();
public:
	#pragma option push -w-inl
	/* TRaveGraphicImage.Create */ inline __fastcall virtual TRaveMetaFile(Classes::TComponent* AOwner) : TRaveGraphicImage(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveGraphicImage.Destroy */ inline __fastcall virtual ~TRaveMetaFile(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveMailMergeItem;
class PASCALIMPLEMENTATION TRaveMailMergeItem : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	AnsiString FSearchToken;
	AnsiString FReplaceString;
	bool FCaseMatters;
	
__published:
	__property bool CaseMatters = {read=FCaseMatters, write=FCaseMatters, default=0};
	__property AnsiString ReplaceString = {read=FReplaceString, write=FReplaceString};
	__property AnsiString SearchToken = {read=FSearchToken, write=FSearchToken};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TRaveMailMergeItem(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveMailMergeItem(void) : Classes::TPersistent() { }
	#pragma option pop
	
};


class DELPHICLASS TRaveMemo;
class PASCALIMPLEMENTATION TRaveMemo : public TRaveCustomText 
{
	typedef TRaveCustomText inherited;
	
protected:
	Rvdefine::TRaveAnchor SaveAnchor;
	bool FExpandParent;
	bool FTruncate;
	bool Prepared;
	Rpmemo::TMemoBuf* MemoBuf;
	Classes::TStrings* LineList;
	Rvclass::TRavePersistentList* FMailMergeItems;
	bool FContainsRTF;
	void __fastcall SetMailMergeItems(Rvclass::TRavePersistentList* Value);
	void __fastcall InitMemoBuf(void);
	void __fastcall FreeMemoBuf(void);
	void __fastcall SetMemoText(AnsiString Value);
	AnsiString __fastcall GetMemoText();
	virtual void __fastcall InitData(void);
	virtual bool __fastcall GetDonePrinting(void);
	virtual void __fastcall BeforeReport(void);
	virtual void __fastcall AfterReport(void);
	
public:
	__fastcall virtual TRaveMemo(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveMemo(void);
	virtual void __fastcall Paint(Graphics::TCanvas* Canvas);
	virtual void __fastcall Resize(void);
	virtual void __fastcall SetText(AnsiString Value);
	virtual void __fastcall Print(Rpbase::TBaseReport* Report);
	virtual Rvdefine::TRaveUnits __fastcall HeightToPrint(void);
	__property bool ContainsRTF = {read=FContainsRTF, write=FContainsRTF, default=0};
	
__published:
	__property Color  = {default=0};
	__property bool ExpandParent = {read=FExpandParent, write=FExpandParent, default=1};
	__property Font ;
	__property FontJustify ;
	__property FontMirror ;
	__property Left ;
	__property Height ;
	__property Rvclass::TRavePersistentList* MailMergeItems = {read=FMailMergeItems, write=SetMailMergeItems};
	__property AnsiString Text = {read=GetMemoText, write=SetMemoText};
	__property Top ;
	__property bool Truncate = {read=FTruncate, write=FTruncate, default=0};
	__property Width ;
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE Rvclass::TRaveControl* __fastcall CreateTextControl(Classes::TComponent* AOwner, Rvdatafield::TRaveDataField* DataField);
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Rvcsstd */
using namespace Rvcsstd;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvCsStd
