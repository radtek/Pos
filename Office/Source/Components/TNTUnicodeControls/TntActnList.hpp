// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntActnList.pas' rev: 6.00

#ifndef TntActnListHPP
#define TntActnListHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ListActns.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <ComCtrls.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <TntControls.hpp>	// Pascal unit
#include <Buttons.hpp>	// Pascal unit
#include <ActnList.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntactnlist
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntActionList;
class PASCALIMPLEMENTATION TTntActionList : public Actnlist::TActionList 
{
	typedef Actnlist::TActionList inherited;
	
private:
	Extctrls::TTimer* FCheckActionsTimer;
	void __fastcall CheckActions(System::TObject* Sender);
	
public:
	__fastcall virtual TTntActionList(Classes::TComponent* AOwner);
public:
	#pragma option push -w-inl
	/* TCustomActionList.Destroy */ inline __fastcall virtual ~TTntActionList(void) { }
	#pragma option pop
	
};


__interface ITntAction;
typedef System::DelphiInterface<ITntAction> _di_ITntAction;
__interface INTERFACE_UUID("{59D0AE37-8161-4AD6-9102-14B28E5761EB}") ITntAction  : public IInterface 
{
	
};

class DELPHICLASS TTntCustomAction;
class PASCALIMPLEMENTATION TTntCustomAction : public Actnlist::TCustomAction 
{
	typedef Actnlist::TCustomAction inherited;
	
private:
	WideString __fastcall GetCaption();
	HIDESBASE void __fastcall SetCaption(const WideString Value);
	WideString __fastcall GetHint();
	HIDESBASE void __fastcall SetHint(const WideString Value);
	
protected:
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	
public:
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	__property WideString Caption = {read=GetCaption, write=SetCaption};
	__property WideString Hint = {read=GetHint, write=SetHint};
public:
	#pragma option push -w-inl
	/* TCustomAction.Create */ inline __fastcall virtual TTntCustomAction(Classes::TComponent* AOwner) : Actnlist::TCustomAction(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomAction.Destroy */ inline __fastcall virtual ~TTntCustomAction(void) { }
	#pragma option pop
	
private:
	void *__ITntAction;	/* Tntactnlist::ITntAction */
	
public:
	operator ITntAction*(void) { return (ITntAction*)&__ITntAction; }
	
};


class DELPHICLASS TTntAction;
class PASCALIMPLEMENTATION TTntAction : public Actnlist::TAction 
{
	typedef Actnlist::TAction inherited;
	
private:
	WideString __fastcall GetCaption();
	HIDESBASE void __fastcall SetCaption(const WideString Value);
	WideString __fastcall GetHint();
	HIDESBASE void __fastcall SetHint(const WideString Value);
	
protected:
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	
public:
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	
__published:
	__property WideString Caption = {read=GetCaption, write=SetCaption};
	__property WideString Hint = {read=GetHint, write=SetHint};
public:
	#pragma option push -w-inl
	/* TAction.Create */ inline __fastcall virtual TTntAction(Classes::TComponent* AOwner) : Actnlist::TAction(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomAction.Destroy */ inline __fastcall virtual ~TTntAction(void) { }
	#pragma option pop
	
private:
	void *__ITntAction;	/* Tntactnlist::ITntAction */
	
public:
	operator ITntAction*(void) { return (ITntAction*)&__ITntAction; }
	
};


class DELPHICLASS TTntMenuActionLink;
class PASCALIMPLEMENTATION TTntMenuActionLink : public Menus::TMenuActionLink 
{
	typedef Menus::TMenuActionLink inherited;
	
protected:
	virtual bool __fastcall IsCaptionLinked(void);
	virtual bool __fastcall IsHintLinked(void);
	virtual void __fastcall SetCaption(const AnsiString Value);
	virtual void __fastcall SetHint(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TBasicActionLink.Create */ inline __fastcall virtual TTntMenuActionLink(System::TObject* AClient) : Menus::TMenuActionLink(AClient) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBasicActionLink.Destroy */ inline __fastcall virtual ~TTntMenuActionLink(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntListViewActionLink;
class PASCALIMPLEMENTATION TTntListViewActionLink : public Comctrls::TListViewActionLink 
{
	typedef Comctrls::TListViewActionLink inherited;
	
protected:
	virtual bool __fastcall IsCaptionLinked(void);
	virtual bool __fastcall IsHintLinked(void);
	virtual void __fastcall SetCaption(const AnsiString Value);
	virtual void __fastcall SetHint(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TBasicActionLink.Create */ inline __fastcall virtual TTntListViewActionLink(System::TObject* AClient) : Comctrls::TListViewActionLink(AClient) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBasicActionLink.Destroy */ inline __fastcall virtual ~TTntListViewActionLink(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntComboBoxExActionLink;
class PASCALIMPLEMENTATION TTntComboBoxExActionLink : public Comctrls::TComboBoxExActionLink 
{
	typedef Comctrls::TComboBoxExActionLink inherited;
	
protected:
	virtual bool __fastcall IsCaptionLinked(void);
	virtual bool __fastcall IsHintLinked(void);
	virtual void __fastcall SetCaption(const AnsiString Value);
	virtual void __fastcall SetHint(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TBasicActionLink.Create */ inline __fastcall virtual TTntComboBoxExActionLink(System::TObject* AClient) : Comctrls::TComboBoxExActionLink(AClient) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBasicActionLink.Destroy */ inline __fastcall virtual ~TTntComboBoxExActionLink(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntSpeedButtonActionLink;
class PASCALIMPLEMENTATION TTntSpeedButtonActionLink : public Buttons::TSpeedButtonActionLink 
{
	typedef Buttons::TSpeedButtonActionLink inherited;
	
protected:
	virtual bool __fastcall IsCaptionLinked(void);
	virtual bool __fastcall IsHintLinked(void);
	virtual void __fastcall SetCaption(const AnsiString Value);
	virtual void __fastcall SetHint(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TBasicActionLink.Create */ inline __fastcall virtual TTntSpeedButtonActionLink(System::TObject* AClient) : Buttons::TSpeedButtonActionLink(AClient) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBasicActionLink.Destroy */ inline __fastcall virtual ~TTntSpeedButtonActionLink(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntToolButtonActionLink;
class PASCALIMPLEMENTATION TTntToolButtonActionLink : public Comctrls::TToolButtonActionLink 
{
	typedef Comctrls::TToolButtonActionLink inherited;
	
protected:
	virtual bool __fastcall IsCaptionLinked(void);
	virtual bool __fastcall IsHintLinked(void);
	virtual void __fastcall SetCaption(const AnsiString Value);
	virtual void __fastcall SetHint(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TBasicActionLink.Create */ inline __fastcall virtual TTntToolButtonActionLink(System::TObject* AClient) : Comctrls::TToolButtonActionLink(AClient) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBasicActionLink.Destroy */ inline __fastcall virtual ~TTntToolButtonActionLink(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntButtonActionLink;
class PASCALIMPLEMENTATION TTntButtonActionLink : public Stdctrls::TButtonActionLink 
{
	typedef Stdctrls::TButtonActionLink inherited;
	
protected:
	virtual bool __fastcall IsCaptionLinked(void);
	virtual bool __fastcall IsHintLinked(void);
	virtual void __fastcall SetCaption(const AnsiString Value);
	virtual void __fastcall SetHint(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TBasicActionLink.Create */ inline __fastcall virtual TTntButtonActionLink(System::TObject* AClient) : Stdctrls::TButtonActionLink(AClient) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBasicActionLink.Destroy */ inline __fastcall virtual ~TTntButtonActionLink(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntWinControlActionLink;
class PASCALIMPLEMENTATION TTntWinControlActionLink : public Controls::TWinControlActionLink 
{
	typedef Controls::TWinControlActionLink inherited;
	
protected:
	virtual bool __fastcall IsCaptionLinked(void);
	virtual bool __fastcall IsHintLinked(void);
	virtual void __fastcall SetCaption(const AnsiString Value);
	virtual void __fastcall SetHint(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TBasicActionLink.Create */ inline __fastcall virtual TTntWinControlActionLink(System::TObject* AClient) : Controls::TWinControlActionLink(AClient) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBasicActionLink.Destroy */ inline __fastcall virtual ~TTntWinControlActionLink(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntControlActionLink;
class PASCALIMPLEMENTATION TTntControlActionLink : public Controls::TControlActionLink 
{
	typedef Controls::TControlActionLink inherited;
	
protected:
	virtual bool __fastcall IsCaptionLinked(void);
	virtual bool __fastcall IsHintLinked(void);
	virtual void __fastcall SetCaption(const AnsiString Value);
	virtual void __fastcall SetHint(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TBasicActionLink.Create */ inline __fastcall virtual TTntControlActionLink(System::TObject* AClient) : Controls::TControlActionLink(AClient) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBasicActionLink.Destroy */ inline __fastcall virtual ~TTntControlActionLink(void) { }
	#pragma option pop
	
};


typedef void __fastcall (*TUpgradeActionListItemsProc)(TTntActionList* ActionList);

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TUpgradeActionListItemsProc UpgradeActionListItemsProc;
extern PACKAGE void __fastcall TntAction_SetCaption(Actnlist::TCustomAction* Action, const WideString Value);
extern PACKAGE WideString __fastcall TntAction_GetCaption(Actnlist::TCustomAction* Action);
extern PACKAGE void __fastcall TntAction_SetHint(Actnlist::TCustomAction* Action, const WideString Value);
extern PACKAGE WideString __fastcall TntAction_GetHint(Actnlist::TCustomAction* Action);
extern PACKAGE void __fastcall TntAction_AfterInherited_Assign(Actnlist::TCustomAction* Action, Classes::TPersistent* Source);
extern PACKAGE TMetaClass* __fastcall TntControl_GetActionLinkClass(Controls::TControl* Control, TMetaClass* InheritedLinkClass);
extern PACKAGE void __fastcall TntControl_BeforeInherited_ActionChange(Controls::TControl* Control, System::TObject* Sender, bool CheckDefaults);

}	/* namespace Tntactnlist */
using namespace Tntactnlist;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntActnList
