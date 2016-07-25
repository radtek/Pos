// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdTimeUDPServer.pas' rev: 6.00

#ifndef IdTimeUDPServerHPP
#define IdTimeUDPServerHPP

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

namespace Idtimeudpserver
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdTimeUDPServer;
class PASCALIMPLEMENTATION TIdTimeUDPServer : public Idudpserver::TIdUDPServer 
{
	typedef Idudpserver::TIdUDPServer inherited;
	
protected:
	System::TDateTime FBaseDate;
	virtual void __fastcall DoUDPRead(Classes::TStream* AData, Idsockethandle::TIdSocketHandle* ABinding);
	
public:
	__fastcall virtual TIdTimeUDPServer(Classes::TComponent* axOwner);
	
__published:
	__property DefaultPort  = {default=37};
	__property System::TDateTime BaseDate = {read=FBaseDate, write=FBaseDate};
public:
	#pragma option push -w-inl
	/* TIdUDPServer.Destroy */ inline __fastcall virtual ~TIdTimeUDPServer(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idtimeudpserver */
using namespace Idtimeudpserver;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdTimeUDPServer
