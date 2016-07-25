// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvSecurity.pas' rev: 6.00

#ifndef RvSecurityHPP
#define RvSecurityHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvDefine.hpp>	// Pascal unit
#include <RvData.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvsecurity
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveBaseSecurity;
class PASCALIMPLEMENTATION TRaveBaseSecurity : public Rvclass::TRaveDataObject 
{
	typedef Rvclass::TRaveDataObject inherited;
	
public:
	virtual bool __fastcall IsValidUser(AnsiString AUserName, AnsiString APassword) = 0 ;
public:
	#pragma option push -w-inl
	/* TRaveProjectItem.Create */ inline __fastcall virtual TRaveBaseSecurity(Classes::TComponent* AOwner) : Rvclass::TRaveDataObject(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveProjectItem.Destroy */ inline __fastcall virtual ~TRaveBaseSecurity(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveSimpleSecurity;
class PASCALIMPLEMENTATION TRaveSimpleSecurity : public TRaveBaseSecurity 
{
	typedef TRaveBaseSecurity inherited;
	
protected:
	Classes::TStrings* FUserList;
	bool FCaseMatters;
	void __fastcall SetUserList(Classes::TStrings* Value);
	
public:
	__fastcall virtual TRaveSimpleSecurity(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveSimpleSecurity(void);
	virtual bool __fastcall IsValidUser(AnsiString AUserName, AnsiString APassword);
	
__published:
	__property Classes::TStrings* UserList = {read=FUserList, write=SetUserList};
	__property bool CaseMatters = {read=FCaseMatters, write=FCaseMatters, default=0};
};


class DELPHICLASS TRaveLookupSecurity;
class PASCALIMPLEMENTATION TRaveLookupSecurity : public TRaveBaseSecurity 
{
	typedef TRaveBaseSecurity inherited;
	
protected:
	Rvdata::TRaveBaseDataView* FDataView;
	AnsiString FUserField;
	AnsiString FPasswordField;
	
public:
	virtual bool __fastcall IsValidUser(AnsiString AUserName, AnsiString APassword);
	
__published:
	__property Rvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView, default=0};
	__property AnsiString UserField = {read=FUserField, write=FPasswordField};
	__property AnsiString PasswordField = {read=FPasswordField, write=FPasswordField};
public:
	#pragma option push -w-inl
	/* TRaveProjectItem.Create */ inline __fastcall virtual TRaveLookupSecurity(Classes::TComponent* AOwner) : TRaveBaseSecurity(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveProjectItem.Destroy */ inline __fastcall virtual ~TRaveLookupSecurity(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Rvsecurity */
using namespace Rvsecurity;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvSecurity
