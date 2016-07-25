// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdEchoUDPServer.pas' rev: 6.00

#ifndef IdEchoUDPServerHPP
#define IdEchoUDPServerHPP

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

namespace Idechoudpserver
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdEchoUDPServer;
class PASCALIMPLEMENTATION TIdEchoUDPServer : public Idudpserver::TIdUDPServer 
{
	typedef Idudpserver::TIdUDPServer inherited;
	
protected:
	virtual void __fastcall DoUDPRead(Classes::TStream* AData, Idsockethandle::TIdSocketHandle* ABinding);
	
public:
	__fastcall virtual TIdEchoUDPServer(Classes::TComponent* axOwner);
	
__published:
	__property DefaultPort  = {default=7};
public:
	#pragma option push -w-inl
	/* TIdUDPServer.Destroy */ inline __fastcall virtual ~TIdEchoUDPServer(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idechoudpserver */
using namespace Idechoudpserver;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdEchoUDPServer
