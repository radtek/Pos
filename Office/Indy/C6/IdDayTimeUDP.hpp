// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdDayTimeUDP.pas' rev: 6.00

#ifndef IdDayTimeUDPHPP
#define IdDayTimeUDPHPP

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

namespace Iddaytimeudp
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdDayTimeUDP;
class PASCALIMPLEMENTATION TIdDayTimeUDP : public Idudpclient::TIdUDPClient 
{
	typedef Idudpclient::TIdUDPClient inherited;
	
protected:
	AnsiString __fastcall GetDayTimeStr();
	
public:
	__fastcall virtual TIdDayTimeUDP(Classes::TComponent* AOwner);
	__property AnsiString DayTimeStr = {read=GetDayTimeStr};
	
__published:
	__property Port  = {default=13};
public:
	#pragma option push -w-inl
	/* TIdUDPBase.Destroy */ inline __fastcall virtual ~TIdDayTimeUDP(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Iddaytimeudp */
using namespace Iddaytimeudp;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdDayTimeUDP
