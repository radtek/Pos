// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdQOTDUDP.pas' rev: 6.00

#ifndef IdQOTDUDPHPP
#define IdQOTDUDPHPP

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

namespace Idqotdudp
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdQOTDUDP;
class PASCALIMPLEMENTATION TIdQOTDUDP : public Idudpclient::TIdUDPClient 
{
	typedef Idudpclient::TIdUDPClient inherited;
	
protected:
	AnsiString __fastcall GetQuote();
	
public:
	__fastcall virtual TIdQOTDUDP(Classes::TComponent* AOwner);
	__property AnsiString Quote = {read=GetQuote};
	
__published:
	__property Port  = {default=17};
public:
	#pragma option push -w-inl
	/* TIdUDPBase.Destroy */ inline __fastcall virtual ~TIdQOTDUDP(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idqotdudp */
using namespace Idqotdudp;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdQOTDUDP
