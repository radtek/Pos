// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntListActns.pas' rev: 6.00

#ifndef TntListActnsHPP
#define TntListActnsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ListActns.hpp>	// Pascal unit
#include <TntActnList.hpp>	// Pascal unit
#include <ActnList.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntlistactns
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntCustomListAction;
class PASCALIMPLEMENTATION TTntCustomListAction : public Listactns::TCustomListAction 
{
	typedef Listactns::TCustomListAction inherited;
	
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
	/* TCustomListAction.Create */ inline __fastcall virtual TTntCustomListAction(Classes::TComponent* AOwner) : Listactns::TCustomListAction(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomAction.Destroy */ inline __fastcall virtual ~TTntCustomListAction(void) { }
	#pragma option pop
	
private:
	void *__ITntAction;	/* Tntactnlist::ITntAction */
	
public:
	operator ITntAction*(void) { return (ITntAction*)&__ITntAction; }
	
};


class DELPHICLASS TTntStaticListAction;
class PASCALIMPLEMENTATION TTntStaticListAction : public Listactns::TStaticListAction 
{
	typedef Listactns::TStaticListAction inherited;
	
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
	/* TStaticListAction.Create */ inline __fastcall virtual TTntStaticListAction(Classes::TComponent* AOwner) : Listactns::TStaticListAction(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TStaticListAction.Destroy */ inline __fastcall virtual ~TTntStaticListAction(void) { }
	#pragma option pop
	
private:
	void *__ITntAction;	/* Tntactnlist::ITntAction */
	
public:
	operator ITntAction*(void) { return (ITntAction*)&__ITntAction; }
	
};


class DELPHICLASS TTntVirtualListAction;
class PASCALIMPLEMENTATION TTntVirtualListAction : public Listactns::TVirtualListAction 
{
	typedef Listactns::TVirtualListAction inherited;
	
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
	/* TCustomListAction.Create */ inline __fastcall virtual TTntVirtualListAction(Classes::TComponent* AOwner) : Listactns::TVirtualListAction(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomAction.Destroy */ inline __fastcall virtual ~TTntVirtualListAction(void) { }
	#pragma option pop
	
private:
	void *__ITntAction;	/* Tntactnlist::ITntAction */
	
public:
	operator ITntAction*(void) { return (ITntAction*)&__ITntAction; }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Tntlistactns */
using namespace Tntlistactns;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntListActns
