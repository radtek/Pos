// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntDBCtrls.pas' rev: 6.00

#ifndef TntDBCtrlsHPP
#define TntDBCtrlsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Menus.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <ComCtrls.hpp>	// Pascal unit
#include <Mask.hpp>	// Pascal unit
#include <TntComCtrls.hpp>	// Pascal unit
#include <TntControls.hpp>	// Pascal unit
#include <TntStdCtrls.hpp>	// Pascal unit
#include <TntClasses.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <DBCtrls.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntdbctrls
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntDBEdit;
class PASCALIMPLEMENTATION TTntDBEdit : public Dbctrls::TDBEdit 
{
	typedef Dbctrls::TDBEdit inherited;
	
private:
	Classes::TNotifyEvent InheritedDataChange;
	wchar_t FPasswordChar;
	HIDESBASE void __fastcall DataChange(System::TObject* Sender);
	HIDESBASE void __fastcall UpdateData(System::TObject* Sender);
	WideString __fastcall GetHint();
	void __fastcall SetHint(const WideString Value);
	HIDESBASE bool __fastcall IsHintStored(void);
	HIDESBASE MESSAGE void __fastcall WMPaint(Messages::TWMPaint &Message);
	HIDESBASE Types::TPoint __fastcall GetTextMargins();
	WideString __fastcall GetPasswordChar();
	HIDESBASE void __fastcall SetPasswordChar(const WideString Value);
	HIDESBASE virtual int __fastcall GetSelStart(void);
	HIDESBASE virtual void __fastcall SetSelStart(const int Value);
	HIDESBASE virtual int __fastcall GetSelLength(void);
	HIDESBASE virtual void __fastcall SetSelLength(const int Value);
	HIDESBASE WideString __fastcall GetSelText();
	HIDESBASE void __fastcall SetSelText(const WideString Value);
	HIDESBASE WideString __fastcall GetText();
	HIDESBASE void __fastcall SetText(const WideString Value);
	
protected:
	virtual void __fastcall CreateWindowHandle(const Controls::TCreateParams &Params);
	virtual void __fastcall CreateWnd(void);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	DYNAMIC TMetaClass* __fastcall GetActionLinkClass(void);
	DYNAMIC void __fastcall ActionChange(System::TObject* Sender, bool CheckDefaults);
	
public:
	__fastcall virtual TTntDBEdit(Classes::TComponent* AOwner);
	__property WideString SelText = {read=GetSelText, write=SetSelText};
	__property int SelStart = {read=GetSelStart, write=SetSelStart, nodefault};
	__property int SelLength = {read=GetSelLength, write=SetSelLength, nodefault};
	__property WideString Text = {read=GetText, write=SetText};
	
__published:
	__property WideString Hint = {read=GetHint, write=SetHint, stored=IsHintStored};
	__property WideString PasswordChar = {read=GetPasswordChar, write=SetPasswordChar};
public:
	#pragma option push -w-inl
	/* TDBEdit.Destroy */ inline __fastcall virtual ~TTntDBEdit(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntDBEdit(HWND ParentWindow) : Dbctrls::TDBEdit(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TTntDBText;
class PASCALIMPLEMENTATION TTntDBText : public Dbctrls::TDBText 
{
	typedef Dbctrls::TDBText inherited;
	
private:
	Dbctrls::TFieldDataLink* FDataLink;
	Classes::TNotifyEvent InheritedDataChange;
	WideString __fastcall GetHint();
	void __fastcall SetHint(const WideString Value);
	HIDESBASE bool __fastcall IsHintStored(void);
	HIDESBASE MESSAGE void __fastcall CMHintShow(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMDialogChar(Messages::TWMKey &Message);
	WideString __fastcall GetCaption();
	HIDESBASE bool __fastcall IsCaptionStored(void);
	void __fastcall SetCaption(const WideString Value);
	HIDESBASE WideString __fastcall GetFieldText();
	HIDESBASE void __fastcall DataChange(System::TObject* Sender);
	
protected:
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	HIDESBASE virtual WideString __fastcall GetLabelText();
	DYNAMIC TMetaClass* __fastcall GetActionLinkClass(void);
	DYNAMIC void __fastcall ActionChange(System::TObject* Sender, bool CheckDefaults);
	DYNAMIC void __fastcall DoDrawText(Types::TRect &Rect, int Flags);
	
public:
	__fastcall virtual TTntDBText(Classes::TComponent* AOwner);
	__fastcall virtual ~TTntDBText(void);
	__property WideString Caption = {read=GetCaption, write=SetCaption, stored=IsCaptionStored};
	
__published:
	__property WideString Hint = {read=GetHint, write=SetHint, stored=IsHintStored};
};


class DELPHICLASS TTntCustomDBComboBox;
class PASCALIMPLEMENTATION TTntCustomDBComboBox : public Dbctrls::TDBComboBox 
{
	typedef Dbctrls::TDBComboBox inherited;
	
private:
	Dbctrls::TFieldDataLink* FDataLink;
	WideString FFilter;
	unsigned FLastTime;
	HIDESBASE void __fastcall UpdateData(System::TObject* Sender);
	HIDESBASE void __fastcall EditingChange(System::TObject* Sender);
	HIDESBASE MESSAGE void __fastcall CMEnter(Messages::TWMNoParams &Message);
	HIDESBASE void __fastcall SetReadOnly(void);
	WideString __fastcall GetHint();
	void __fastcall SetHint(const WideString Value);
	HIDESBASE bool __fastcall IsHintStored(void);
	HIDESBASE MESSAGE void __fastcall WMChar(Messages::TWMKey &Message);
	Tntclasses::TTntStrings* FItems;
	Tntclasses::TTntStrings* FSaveItems;
	int FSaveItemIndex;
	Tntclasses::TTntStrings* __fastcall GetItems(void);
	HIDESBASE void __fastcall SetItems(const Tntclasses::TTntStrings* Value);
	HIDESBASE int __fastcall GetSelStart(void);
	HIDESBASE void __fastcall SetSelStart(const int Value);
	HIDESBASE int __fastcall GetSelLength(void);
	HIDESBASE void __fastcall SetSelLength(const int Value);
	HIDESBASE WideString __fastcall GetSelText();
	HIDESBASE void __fastcall SetSelText(const WideString Value);
	HIDESBASE WideString __fastcall GetText();
	HIDESBASE void __fastcall SetText(const WideString Value);
	HIDESBASE MESSAGE void __fastcall CNCommand(Messages::TWMCommand &Message);
	
protected:
	HIDESBASE void __fastcall DataChange(System::TObject* Sender);
	DYNAMIC bool __fastcall GetAutoComplete_UniqueMatchOnly(void);
	DYNAMIC bool __fastcall GetAutoComplete_PreserveDataEntryCase(void);
	virtual void __fastcall DoEditCharMsg(Messages::TWMKey &Message);
	virtual Variant __fastcall GetFieldValue();
	virtual void __fastcall SetFieldValue(const Variant &Value);
	virtual Variant __fastcall GetComboValue(void) = 0 ;
	virtual void __fastcall SetComboValue(const Variant &Value) = 0 ;
	virtual void __fastcall CreateWindowHandle(const Controls::TCreateParams &Params);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	DYNAMIC TMetaClass* __fastcall GetActionLinkClass(void);
	DYNAMIC void __fastcall ActionChange(System::TObject* Sender, bool CheckDefaults);
	virtual void __fastcall CreateWnd(void);
	virtual void __fastcall DestroyWnd(void);
	virtual void __fastcall WndProc(Messages::TMessage &Message);
	virtual void __fastcall ComboWndProc(Messages::TMessage &Message, HWND ComboWnd, void * ComboProc);
	DYNAMIC void __fastcall KeyPress(char &Key);
	
public:
	__fastcall virtual TTntCustomDBComboBox(Classes::TComponent* AOwner);
	__fastcall virtual ~TTntCustomDBComboBox(void);
	virtual void __fastcall CopySelection(Controls::TCustomListControl* Destination);
	HIDESBASE virtual void __fastcall AddItem(const WideString Item, System::TObject* AObject);
	__property WideString SelText = {read=GetSelText, write=SetSelText};
	__property int SelStart = {read=GetSelStart, write=SetSelStart, nodefault};
	__property int SelLength = {read=GetSelLength, write=SetSelLength, nodefault};
	__property WideString Text = {read=GetText, write=SetText};
	
__published:
	__property WideString Hint = {read=GetHint, write=SetHint, stored=IsHintStored};
	__property Tntclasses::TTntStrings* Items = {read=GetItems, write=SetItems};
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntCustomDBComboBox(HWND ParentWindow) : Dbctrls::TDBComboBox(ParentWindow) { }
	#pragma option pop
	
private:
	void *__IWideCustomListControl;	/* Tntcontrols::IWideCustomListControl */
	
public:
	operator IWideCustomListControl*(void) { return (IWideCustomListControl*)&__IWideCustomListControl; }
	
};


class DELPHICLASS TTntDBComboBox;
class PASCALIMPLEMENTATION TTntDBComboBox : public TTntCustomDBComboBox 
{
	typedef TTntCustomDBComboBox inherited;
	
protected:
	virtual Variant __fastcall GetFieldValue();
	virtual void __fastcall SetFieldValue(const Variant &Value);
	virtual Variant __fastcall GetComboValue();
	virtual void __fastcall SetComboValue(const Variant &Value);
public:
	#pragma option push -w-inl
	/* TTntCustomDBComboBox.Create */ inline __fastcall virtual TTntDBComboBox(Classes::TComponent* AOwner) : TTntCustomDBComboBox(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TTntCustomDBComboBox.Destroy */ inline __fastcall virtual ~TTntDBComboBox(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntDBComboBox(HWND ParentWindow) : TTntCustomDBComboBox(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TTntDBCheckBox;
class PASCALIMPLEMENTATION TTntDBCheckBox : public Dbctrls::TDBCheckBox 
{
	typedef Dbctrls::TDBCheckBox inherited;
	
private:
	WideString __fastcall GetCaption();
	void __fastcall SetCaption(const WideString Value);
	WideString __fastcall GetHint();
	void __fastcall SetHint(const WideString Value);
	HIDESBASE bool __fastcall IsCaptionStored(void);
	HIDESBASE bool __fastcall IsHintStored(void);
	
protected:
	virtual void __fastcall CreateWindowHandle(const Controls::TCreateParams &Params);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	DYNAMIC TMetaClass* __fastcall GetActionLinkClass(void);
	DYNAMIC void __fastcall ActionChange(System::TObject* Sender, bool CheckDefaults);
	virtual void __fastcall Toggle(void);
	
__published:
	__property WideString Caption = {read=GetCaption, write=SetCaption, stored=IsCaptionStored};
	__property WideString Hint = {read=GetHint, write=SetHint, stored=IsHintStored};
public:
	#pragma option push -w-inl
	/* TDBCheckBox.Create */ inline __fastcall virtual TTntDBCheckBox(Classes::TComponent* AOwner) : Dbctrls::TDBCheckBox(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDBCheckBox.Destroy */ inline __fastcall virtual ~TTntDBCheckBox(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntDBCheckBox(HWND ParentWindow) : Dbctrls::TDBCheckBox(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TTntDBRichEdit;
class PASCALIMPLEMENTATION TTntDBRichEdit : public Tntcomctrls::TTntCustomRichEdit 
{
	typedef Tntcomctrls::TTntCustomRichEdit inherited;
	
private:
	Dbctrls::TFieldDataLink* FDataLink;
	bool FAutoDisplay;
	bool FFocused;
	bool FMemoLoaded;
	AnsiString FDataSave;
	void __fastcall BeginEditing(void);
	void __fastcall DataChange(System::TObject* Sender);
	void __fastcall EditingChange(System::TObject* Sender);
	AnsiString __fastcall GetDataField();
	Db::TDataSource* __fastcall GetDataSource(void);
	Db::TField* __fastcall GetField(void);
	bool __fastcall GetReadOnly(void);
	void __fastcall SetDataField(const AnsiString Value);
	void __fastcall SetDataSource(Db::TDataSource* Value);
	HIDESBASE void __fastcall SetReadOnly(bool Value);
	void __fastcall SetAutoDisplay(bool Value);
	void __fastcall SetFocused(bool Value);
	void __fastcall UpdateData(System::TObject* Sender);
	MESSAGE void __fastcall WMCut(Messages::TMessage &Message);
	MESSAGE void __fastcall WMPaste(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMEnter(Messages::TWMNoParams &Message);
	HIDESBASE MESSAGE void __fastcall CMExit(Messages::TWMNoParams &Message);
	HIDESBASE MESSAGE void __fastcall WMLButtonDblClk(Messages::TWMMouse &Message);
	MESSAGE void __fastcall CMGetDataLink(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CNNotify(Messages::TWMNotify &Message);
	
protected:
	DYNAMIC void __fastcall InternalLoadMemo(void);
	DYNAMIC void __fastcall InternalSaveMemo(void);
	DYNAMIC bool __fastcall SaveAsRTF(void);
	DYNAMIC void __fastcall Change(void);
	DYNAMIC void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
	DYNAMIC void __fastcall KeyPress(char &Key);
	virtual void __fastcall Loaded(void);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	
public:
	__fastcall virtual TTntDBRichEdit(Classes::TComponent* AOwner);
	__fastcall virtual ~TTntDBRichEdit(void);
	DYNAMIC bool __fastcall ExecuteAction(Classes::TBasicAction* Action);
	virtual void __fastcall LoadMemo(void);
	DYNAMIC bool __fastcall UpdateAction(Classes::TBasicAction* Action);
	DYNAMIC bool __fastcall UseRightToLeftAlignment(void);
	__property Db::TField* Field = {read=GetField};
	
__published:
	__property Align  = {default=0};
	__property Alignment  = {default=0};
	__property Anchors  = {default=3};
	__property bool AutoDisplay = {read=FAutoDisplay, write=SetAutoDisplay, default=1};
	__property BevelEdges  = {default=15};
	__property BevelInner  = {index=0, default=2};
	__property BevelOuter  = {index=1, default=1};
	__property BevelKind  = {default=0};
	__property BevelWidth  = {default=1};
	__property BiDiMode ;
	__property BorderStyle  = {default=1};
	__property Color  = {default=-2147483643};
	__property Constraints ;
	__property Ctl3D ;
	__property AnsiString DataField = {read=GetDataField, write=SetDataField};
	__property Db::TDataSource* DataSource = {read=GetDataSource, write=SetDataSource};
	__property DragCursor  = {default=-12};
	__property DragKind  = {default=0};
	__property DragMode  = {default=0};
	__property Enabled  = {default=1};
	__property Font ;
	__property HideSelection  = {default=1};
	__property HideScrollBars  = {default=1};
	__property ImeMode  = {default=3};
	__property ImeName ;
	__property MaxLength  = {default=0};
	__property ParentBiDiMode  = {default=1};
	__property ParentColor  = {default=0};
	__property ParentCtl3D  = {default=1};
	__property ParentFont  = {default=1};
	__property ParentShowHint  = {default=1};
	__property PlainText  = {default=0};
	__property PopupMenu ;
	__property bool ReadOnly = {read=GetReadOnly, write=SetReadOnly, default=0};
	__property ScrollBars  = {default=0};
	__property ShowHint ;
	__property TabOrder  = {default=-1};
	__property TabStop  = {default=1};
	__property Visible  = {default=1};
	__property WantReturns  = {default=1};
	__property WantTabs  = {default=0};
	__property WordWrap  = {default=1};
	__property OnChange ;
	__property OnClick ;
	__property OnContextPopup ;
	__property OnDblClick ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDock ;
	__property OnEndDrag ;
	__property OnEnter ;
	__property OnExit ;
	__property OnKeyDown ;
	__property OnKeyPress ;
	__property OnKeyUp ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnResizeRequest ;
	__property OnSelectionChange ;
	__property OnProtectChange ;
	__property OnSaveClipboard ;
	__property OnStartDock ;
	__property OnStartDrag ;
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntDBRichEdit(HWND ParentWindow) : Tntcomctrls::TTntCustomRichEdit(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Tntdbctrls */
using namespace Tntdbctrls;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntDBCtrls
