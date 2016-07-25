// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntControls.pas' rev: 6.00

#ifndef TntControlsHPP
#define TntControlsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <TntSysUtils.hpp>	// Pascal unit
#include <TntMenus.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <ActnList.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <TntClasses.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntcontrols
{
//-- type declarations -------------------------------------------------------
typedef WideString TWideCaption;

typedef void __fastcall (__closure *TSetAnsiStrEvent)(const AnsiString Value);

class DELPHICLASS TTntCustomHintWindow;
class PASCALIMPLEMENTATION TTntCustomHintWindow : public Controls::THintWindow 
{
	typedef Controls::THintWindow inherited;
	
private:
	bool FActivating;
	bool FBlockPaint;
	WideString __fastcall GetCaption();
	void __fastcall SetCaption(const WideString Value);
	HIDESBASE MESSAGE void __fastcall CMTextChanged(Messages::TMessage &Message);
	
protected:
	virtual void __fastcall CreateWindowHandle(const Controls::TCreateParams &Params);
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	virtual void __fastcall Paint(void);
	
public:
	virtual void __fastcall ActivateHint(const Types::TRect &Rect, const AnsiString AHint);
	virtual void __fastcall ActivateHintData(const Types::TRect &Rect, const AnsiString AHint, void * AData);
	virtual Types::TRect __fastcall CalcHintRect(int MaxWidth, const AnsiString AHint, void * AData);
	__property WideString Caption = {read=GetCaption, write=SetCaption};
public:
	#pragma option push -w-inl
	/* THintWindow.Create */ inline __fastcall virtual TTntCustomHintWindow(Classes::TComponent* AOwner) : Controls::THintWindow(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TTntCustomHintWindow(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntCustomHintWindow(HWND ParentWindow) : Controls::THintWindow(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TTntHintWindow;
class PASCALIMPLEMENTATION TTntHintWindow : public TTntCustomHintWindow 
{
	typedef TTntCustomHintWindow inherited;
	
public:
	HIDESBASE void __fastcall ActivateHint(const Types::TRect &Rect, const WideString AHint);
	HIDESBASE void __fastcall ActivateHintData(const Types::TRect &Rect, const WideString AHint, void * AData);
	HIDESBASE Types::TRect __fastcall CalcHintRect(int MaxWidth, const WideString AHint, void * AData);
public:
	#pragma option push -w-inl
	/* THintWindow.Create */ inline __fastcall virtual TTntHintWindow(Classes::TComponent* AOwner) : TTntCustomHintWindow(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TTntHintWindow(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntHintWindow(HWND ParentWindow) : TTntCustomHintWindow(ParentWindow) { }
	#pragma option pop
	
};


__interface IWideCustomListControl;
typedef System::DelphiInterface<IWideCustomListControl> _di_IWideCustomListControl;
__interface INTERFACE_UUID("{C1801F41-51E9-4DB5-8DB8-58AC86698C2E}") IWideCustomListControl  : public IInterface 
{
	
public:
	virtual void __fastcall AddItem(const WideString Item, System::TObject* AObject) = 0 ;
};

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool _IsShellProgramming;
extern PACKAGE WideString __fastcall GetSyncedWideString(WideString &WideStr, const AnsiString AnsiStr);
extern PACKAGE void __fastcall SetSyncedWideString(const WideString Value, WideString &WideStr, const AnsiString AnsiStr, TSetAnsiStrEvent SetAnsiStr);
extern PACKAGE bool __fastcall TntControl_IsCaptionStored(Controls::TControl* Control);
extern PACKAGE WideString __fastcall TntControl_GetStoredText(Controls::TControl* Control, const WideString Default);
extern PACKAGE WideString __fastcall TntControl_GetText(Controls::TControl* Control);
extern PACKAGE void __fastcall TntControl_SetText(Controls::TControl* Control, const WideString Text);
extern PACKAGE bool __fastcall TntControl_IsHintStored(Controls::TControl* Control);
extern PACKAGE WideString __fastcall TntControl_GetHint(Controls::TControl* Control);
extern PACKAGE void __fastcall TntControl_SetHint(Controls::TControl* Control, const WideString Value);
extern PACKAGE WideString __fastcall WideGetHint(Controls::TControl* Control);
extern PACKAGE WideString __fastcall WideGetShortHint(const WideString Hint);
extern PACKAGE WideString __fastcall WideGetLongHint(const WideString Hint);
extern PACKAGE bool __fastcall IsTextMessage(unsigned Msg);
extern PACKAGE void __fastcall MakeWMCharMsgSafeForAnsi(Messages::TMessage &Message);
extern PACKAGE void __fastcall RestoreWMCharMsg(Messages::TMessage &Message);
extern PACKAGE wchar_t __fastcall GetWideCharFromWMCharMsg(const Messages::TWMKey &Message);
extern PACKAGE void __fastcall SetWideCharForWMCharMsg(Messages::TWMKey &Message, wchar_t Ch);
extern PACKAGE void __fastcall ProcessCMHintShowMsg(Messages::TMessage &Message);
extern PACKAGE void __fastcall SubClassUnicodeControl(Controls::TWinControl* Control, char * Params_Caption, bool IDEWindow = false);
extern PACKAGE void __fastcall RegisterUnicodeClass(const Controls::TCreateParams &Params, /* out */ WideString &WideWinClassName, bool IDEWindow = false);
extern PACKAGE void __fastcall CreateUnicodeHandle(Controls::TWinControl* Control, const Controls::TCreateParams &Params, const WideString SubClass, bool IDEWindow = false);
extern PACKAGE void __fastcall ReCreateUnicodeWnd(Controls::TWinControl* Control, WideString Subclass, bool IDEWindow = false);
extern PACKAGE void __fastcall WideListControl_AddItem(Controls::TCustomListControl* Control, const WideString Item, System::TObject* AObject);

}	/* namespace Tntcontrols */
using namespace Tntcontrols;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntControls
