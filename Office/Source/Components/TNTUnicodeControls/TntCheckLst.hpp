// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntCheckLst.pas' rev: 6.00

#ifndef TntCheckLstHPP
#define TntCheckLstHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <TntControls.hpp>	// Pascal unit
#include <TntClasses.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <CheckLst.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntchecklst
{
//-- type declarations -------------------------------------------------------
typedef DynamicArray<bool >  TntCheckLst__2;

typedef DynamicArray<Stdctrls::TCheckBoxState >  TntCheckLst__3;

typedef DynamicArray<bool >  TntCheckLst__4;

class DELPHICLASS TTntCheckListBox;
class PASCALIMPLEMENTATION TTntCheckListBox : public Checklst::TCheckListBox 
{
	typedef Checklst::TCheckListBox inherited;
	
private:
	Tntclasses::TTntStrings* FItems;
	Tntclasses::TTntStrings* FSaveItems;
	int FSaveTopIndex;
	int FSaveItemIndex;
	DynamicArray<bool >  FSaved_ItemEnabled;
	DynamicArray<Stdctrls::TCheckBoxState >  FSaved_State;
	DynamicArray<bool >  FSaved_Header;
	HIDESBASE void __fastcall SetItems(const Tntclasses::TTntStrings* Value);
	WideString __fastcall GetHint();
	void __fastcall SetHint(const WideString Value);
	HIDESBASE bool __fastcall IsHintStored(void);
	
protected:
	virtual void __fastcall CreateWindowHandle(const Controls::TCreateParams &Params);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	DYNAMIC TMetaClass* __fastcall GetActionLinkClass(void);
	DYNAMIC void __fastcall ActionChange(System::TObject* Sender, bool CheckDefaults);
	virtual void __fastcall CreateWnd(void);
	virtual void __fastcall DestroyWnd(void);
	virtual void __fastcall DrawItem(int Index, const Types::TRect &Rect, Windows::TOwnerDrawState State);
	
public:
	__fastcall virtual TTntCheckListBox(Classes::TComponent* AOwner);
	__fastcall virtual ~TTntCheckListBox(void);
	virtual void __fastcall CopySelection(Controls::TCustomListControl* Destination);
	HIDESBASE virtual void __fastcall AddItem(const WideString Item, System::TObject* AObject);
	
__published:
	__property WideString Hint = {read=GetHint, write=SetHint, stored=IsHintStored};
	__property Tntclasses::TTntStrings* Items = {read=FItems, write=SetItems};
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntCheckListBox(HWND ParentWindow) : Checklst::TCheckListBox(ParentWindow) { }
	#pragma option pop
	
private:
	void *__IWideCustomListControl;	/* Tntcontrols::IWideCustomListControl */
	
public:
	operator IWideCustomListControl*(void) { return (IWideCustomListControl*)&__IWideCustomListControl; }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Tntchecklst */
using namespace Tntchecklst;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntCheckLst
