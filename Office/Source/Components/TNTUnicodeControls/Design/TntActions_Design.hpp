// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntActions_Design.pas' rev: 6.00

#ifndef TntActions_DesignHPP
#define TntActions_DesignHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <DesignIntf.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntactions_design
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntDesignerSelections;
class PASCALIMPLEMENTATION TTntDesignerSelections : public System::TInterfacedObject 
{
	typedef System::TInterfacedObject inherited;
	
public:
	Classes::TPersistent* operator[](int Index) { return Items[Index]; }
	
private:
	Classes::TList* FList;
	
protected:
	int __fastcall Add(const Classes::TPersistent* Item);
	bool __fastcall Equals(const Designintf::_di_IDesignerSelections List);
	Classes::TPersistent* __fastcall Get(int Index);
	int __fastcall GetCount(void);
	__property int Count = {read=GetCount, nodefault};
	__property Classes::TPersistent* Items[int Index] = {read=Get/*, default*/};
	
public:
	__fastcall virtual TTntDesignerSelections(void);
	__fastcall virtual ~TTntDesignerSelections(void);
	void __fastcall ReplaceSelection(const Classes::TPersistent* OldInst, const Classes::TPersistent* NewInst);
private:
	void *__IDesignerSelections;	/* Designintf::IDesignerSelections */
	
public:
	operator IDesignerSelections*(void) { return (IDesignerSelections*)&__IDesignerSelections; }
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall Register(void);

}	/* namespace Tntactions_design */
using namespace Tntactions_design;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntActions_Design
