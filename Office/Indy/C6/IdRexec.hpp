// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdRexec.pas' rev: 6.00

#ifndef IdRexecHPP
#define IdRexecHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdComponent.hpp>	// Pascal unit
#include <IdTCPConnection.hpp>	// Pascal unit
#include <IdTCPClient.hpp>	// Pascal unit
#include <IdRemoteCMDClient.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idrexec
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdRexec;
class PASCALIMPLEMENTATION TIdRexec : public Idremotecmdclient::TIdRemoteCMDClient 
{
	typedef Idremotecmdclient::TIdRemoteCMDClient inherited;
	
public:
	__fastcall virtual TIdRexec(Classes::TComponent* AOwner);
	virtual AnsiString __fastcall Execute(AnsiString ACommand);
	
__published:
	__property Username ;
	__property Password ;
	__property Port  = {default=512};
public:
	#pragma option push -w-inl
	/* TIdRemoteCMDClient.Destroy */ inline __fastcall virtual ~TIdRexec(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idrexec */
using namespace Idrexec;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdRexec
