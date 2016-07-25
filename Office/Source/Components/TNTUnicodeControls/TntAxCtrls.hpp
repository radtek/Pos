// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntAxCtrls.pas' rev: 6.00

#ifndef TntAxCtrlsHPP
#define TntAxCtrlsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <TntClasses.hpp>	// Pascal unit
#include <AxCtrls.hpp>	// Pascal unit
#include <StdVCL.hpp>	// Pascal unit
#include <ComObj.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntaxctrls
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TWideStringsAdapter;
class PASCALIMPLEMENTATION TWideStringsAdapter : public Comobj::TAutoIntfObject 
{
	typedef Comobj::TAutoIntfObject inherited;
	
private:
	Tntclasses::TTntStrings* FStrings;
	
protected:
	void __fastcall ReferenceStrings(Tntclasses::TTntStrings* S);
	void __fastcall ReleaseStrings(void);
	HRESULT __safecall Get_ControlDefault(int Index, OleVariant &Get_ControlDefault_result);
	HRESULT __safecall Set_ControlDefault(int Index, const OleVariant Value);
	HRESULT __safecall Count(int &Count_result);
	HRESULT __safecall Get_Item(int Index, OleVariant &Get_Item_result);
	HRESULT __safecall Set_Item(int Index, const OleVariant Value);
	HRESULT __safecall Remove(int Index);
	HRESULT __safecall Clear(void);
	HRESULT __safecall Add(const OleVariant Item, int &Add_result);
	HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result);
	
public:
	__fastcall TWideStringsAdapter(Tntclasses::TTntStrings* Strings);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TWideStringsAdapter(void) { }
	#pragma option pop
	
private:
	void *__IStrings;	/* Stdvcl::IStrings */
	void *__IWideStringsAdapter;	/* Tntclasses::IWideStringsAdapter */
	
public:
	operator IWideStringsAdapter*(void) { return (IWideStringsAdapter*)&__IWideStringsAdapter; }
	operator IStrings*(void) { return (IStrings*)&__IStrings; }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Tntaxctrls */
using namespace Tntaxctrls;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntAxCtrls
