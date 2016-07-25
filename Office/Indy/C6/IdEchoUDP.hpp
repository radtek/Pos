// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdEchoUDP.pas' rev: 6.00

#ifndef IdEchoUDPHPP
#define IdEchoUDPHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdComponent.hpp>	// Pascal unit
#include <IdUDPClient.hpp>	// Pascal unit
#include <IdUDPBase.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idechoudp
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdEchoUDP;
class PASCALIMPLEMENTATION TIdEchoUDP : public Idudpclient::TIdUDPClient 
{
	typedef Idudpclient::TIdUDPClient inherited;
	
protected:
	unsigned FEchoTime;
	
public:
	__fastcall virtual TIdEchoUDP(Classes::TComponent* AOwner);
	AnsiString __fastcall Echo(AnsiString AText);
	__property unsigned EchoTime = {read=FEchoTime, nodefault};
	
__published:
	__property Port  = {default=7};
public:
	#pragma option push -w-inl
	/* TIdUDPBase.Destroy */ inline __fastcall virtual ~TIdEchoUDP(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idechoudp */
using namespace Idechoudp;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdEchoUDP
