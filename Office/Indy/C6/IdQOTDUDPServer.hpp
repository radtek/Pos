// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdQOTDUDPServer.pas' rev: 6.00

#ifndef IdQOTDUDPServerHPP
#define IdQOTDUDPServerHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdComponent.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <IdUDPServer.hpp>	// Pascal unit
#include <IdUDPBase.hpp>	// Pascal unit
#include <IdSocketHandle.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idqotdudpserver
{
//-- type declarations -------------------------------------------------------
typedef void __fastcall (__closure *TIdQotdUDPGetEvent)(Idsockethandle::TIdSocketHandle* ABinding, AnsiString &AQuote);

class DELPHICLASS TIdQotdUDPServer;
class PASCALIMPLEMENTATION TIdQotdUDPServer : public Idudpserver::TIdUDPServer 
{
	typedef Idudpserver::TIdUDPServer inherited;
	
protected:
	TIdQotdUDPGetEvent FOnCommandQOTD;
	virtual void __fastcall DoOnCommandQUOTD(Idsockethandle::TIdSocketHandle* ABinding, AnsiString &AQuote);
	virtual void __fastcall DoUDPRead(Classes::TStream* AData, Idsockethandle::TIdSocketHandle* ABinding);
	
public:
	__fastcall virtual TIdQotdUDPServer(Classes::TComponent* axOwner);
	
__published:
	__property DefaultPort  = {default=17};
	__property TIdQotdUDPGetEvent OnCommandQOTD = {read=FOnCommandQOTD, write=FOnCommandQOTD};
public:
	#pragma option push -w-inl
	/* TIdUDPServer.Destroy */ inline __fastcall virtual ~TIdQotdUDPServer(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idqotdudpserver */
using namespace Idqotdudpserver;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdQOTDUDPServer
