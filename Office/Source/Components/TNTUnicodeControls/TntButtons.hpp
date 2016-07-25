// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntButtons.pas' rev: 6.00

#ifndef TntButtonsHPP
#define TntButtonsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <TntStdCtrls.hpp>	// Pascal unit
#include <TntControls.hpp>	// Pascal unit
#include <TntForms.hpp>	// Pascal unit
#include <Buttons.hpp>	// Pascal unit
#include <CommCtrl.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntbuttons
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntSpeedButton;
class PASCALIMPLEMENTATION TTntSpeedButton : public Buttons::TSpeedButton 
{
	typedef Buttons::TSpeedButton inherited;
	
private:
	bool FPaintInherited;
	WideString __fastcall GetCaption();
	void __fastcall SetCaption(const WideString Value);
	WideString __fastcall GetHint();
	void __fastcall SetHint(const WideString Value);
	HIDESBASE bool __fastcall IsCaptionStored(void);
	HIDESBASE bool __fastcall IsHintStored(void);
	HIDESBASE MESSAGE void __fastcall CMHintShow(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMDialogChar(Messages::TWMKey &Message);
	void __fastcall UpdateInternalGlyphList(void);
	
protected:
	virtual void __fastcall Paint(void);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	DYNAMIC TMetaClass* __fastcall GetActionLinkClass(void);
	DYNAMIC void __fastcall ActionChange(System::TObject* Sender, bool CheckDefaults);
	
__published:
	__property WideString Caption = {read=GetCaption, write=SetCaption, stored=IsCaptionStored};
	__property WideString Hint = {read=GetHint, write=SetHint, stored=IsHintStored};
public:
	#pragma option push -w-inl
	/* TSpeedButton.Create */ inline __fastcall virtual TTntSpeedButton(Classes::TComponent* AOwner) : Buttons::TSpeedButton(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TSpeedButton.Destroy */ inline __fastcall virtual ~TTntSpeedButton(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntBitBtn;
class PASCALIMPLEMENTATION TTntBitBtn : public Buttons::TBitBtn 
{
	typedef Buttons::TBitBtn inherited;
	
private:
	bool FPaintInherited;
	bool FMouseInControl;
	HIDESBASE bool __fastcall IsCaptionStored(void);
	WideString __fastcall GetCaption();
	void __fastcall SetCaption(const WideString Value);
	HIDESBASE bool __fastcall IsHintStored(void);
	WideString __fastcall GetHint();
	void __fastcall SetHint(const WideString Value);
	HIDESBASE MESSAGE void __fastcall CMDialogChar(Messages::TWMKey &Message);
	HIDESBASE MESSAGE void __fastcall CNDrawItem(Messages::TWMDrawItem &Message);
	HIDESBASE void __fastcall DrawItem(const tagDRAWITEMSTRUCT &DrawItemStruct);
	void __fastcall UpdateInternalGlyphList(void);
	HIDESBASE MESSAGE void __fastcall CMMouseEnter(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMMouseLeave(Messages::TMessage &Message);
	
protected:
	virtual void __fastcall CreateWindowHandle(const Controls::TCreateParams &Params);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	DYNAMIC TMetaClass* __fastcall GetActionLinkClass(void);
	DYNAMIC void __fastcall ActionChange(System::TObject* Sender, bool CheckDefaults);
	
__published:
	__property WideString Caption = {read=GetCaption, write=SetCaption, stored=IsCaptionStored};
	__property WideString Hint = {read=GetHint, write=SetHint, stored=IsHintStored};
public:
	#pragma option push -w-inl
	/* TBitBtn.Create */ inline __fastcall virtual TTntBitBtn(Classes::TComponent* AOwner) : Buttons::TBitBtn(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBitBtn.Destroy */ inline __fastcall virtual ~TTntBitBtn(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntBitBtn(HWND ParentWindow) : Buttons::TBitBtn(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall TButtonGlyph_CalcButtonLayout(Controls::TControl* Control, HDC DC, const Types::TRect &Client, const Types::TPoint &Offset, const WideString Caption, Buttons::TButtonLayout Layout, int Margin, int Spacing, Types::TPoint &GlyphPos, Types::TRect &TextBounds, int BiDiFlags);

}	/* namespace Tntbuttons */
using namespace Tntbuttons;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntButtons
