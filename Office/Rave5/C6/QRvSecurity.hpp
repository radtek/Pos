// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvSecurity.pas' rev: 6.00

#ifndef QRvSecurityHPP
#define QRvSecurityHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRvDefine.hpp>	// Pascal unit
#include <QRvData.hpp>	// Pascal unit
#include <QRvClass.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvsecurity
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveBaseSecurity;
class PASCALIMPLEMENTATION TRaveBaseSecurity : public Qrvclass::TRaveDataObject 
{
	typedef Qrvclass::TRaveDataObject inherited;
	
public:
	virtual bool __fastcall IsValidUser(AnsiString AUserName, AnsiString APassword) = 0 ;
public:
	#pragma option push -w-inl
	/* TRaveProjectItem.Create */ inline __fastcall virtual TRaveBaseSecurity(Classes::TComponent* AOwner) : Qrvclass::TRaveDataObject(AOwner) { }
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
	Qrvdata::TRaveBaseDataView* FDataView;
	AnsiString FUserField;
	AnsiString FPasswordField;
	
public:
	virtual bool __fastcall IsValidUser(AnsiString AUserName, AnsiString APassword);
	
__published:
	__property Qrvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView, default=0};
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

}	/* namespace Qrvsecurity */
using namespace Qrvsecurity;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvSecurity
