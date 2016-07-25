// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntComCtrls_Design.pas' rev: 6.00

#ifndef TntComCtrls_DesignHPP
#define TntComCtrls_DesignHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ComCtrls.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <DesignEditors.hpp>	// Pascal unit
#include <DesignMenus.hpp>	// Pascal unit
#include <DesignIntf.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntcomctrls_design
{
//-- type declarations -------------------------------------------------------
typedef IMenuItem IPrepareMenuItem;
;

class DELPHICLASS TTntListViewEditor;
class PASCALIMPLEMENTATION TTntListViewEditor : public Designeditors::TComponentEditor 
{
	typedef Designeditors::TComponentEditor inherited;
	
public:
	virtual void __fastcall ExecuteVerb(int Index);
	virtual AnsiString __fastcall GetVerb(int Index);
	virtual int __fastcall GetVerbCount(void);
public:
	#pragma option push -w-inl
	/* TComponentEditor.Create */ inline __fastcall virtual TTntListViewEditor(Classes::TComponent* AComponent, Designintf::_di_IDesigner ADesigner) : Designeditors::TComponentEditor(AComponent, ADesigner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TTntListViewEditor(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntPageControlEditor;
class PASCALIMPLEMENTATION TTntPageControlEditor : public Designeditors::TDefaultEditor 
{
	typedef Designeditors::TDefaultEditor inherited;
	
private:
	Comctrls::TPageControl* __fastcall PageControl(void);
	
public:
	virtual void __fastcall ExecuteVerb(int Index);
	virtual AnsiString __fastcall GetVerb(int Index);
	virtual int __fastcall GetVerbCount(void);
	virtual void __fastcall PrepareItem(int Index, const Designmenus::_di_IMenuItem AItem);
public:
	#pragma option push -w-inl
	/* TComponentEditor.Create */ inline __fastcall virtual TTntPageControlEditor(Classes::TComponent* AComponent, Designintf::_di_IDesigner ADesigner) : Designeditors::TDefaultEditor(AComponent, ADesigner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TTntPageControlEditor(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntStatusBarEditor;
class PASCALIMPLEMENTATION TTntStatusBarEditor : public Designeditors::TComponentEditor 
{
	typedef Designeditors::TComponentEditor inherited;
	
public:
	virtual void __fastcall ExecuteVerb(int Index);
	virtual AnsiString __fastcall GetVerb(int Index);
	virtual int __fastcall GetVerbCount(void);
public:
	#pragma option push -w-inl
	/* TComponentEditor.Create */ inline __fastcall virtual TTntStatusBarEditor(Classes::TComponent* AComponent, Designintf::_di_IDesigner ADesigner) : Designeditors::TComponentEditor(AComponent, ADesigner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TTntStatusBarEditor(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall Register(void);

}	/* namespace Tntcomctrls_design */
using namespace Tntcomctrls_design;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntComCtrls_Design
