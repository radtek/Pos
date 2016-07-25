// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdServerIOHandlerSocket.pas' rev: 6.00

#ifndef IdServerIOHandlerSocketHPP
#define IdServerIOHandlerSocketHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Classes.hpp>	// Pascal unit
#include <IdComponent.hpp>	// Pascal unit
#include <IdIOHandlerSocket.hpp>	// Pascal unit
#include <IdIOHandler.hpp>	// Pascal unit
#include <IdStackConsts.hpp>	// Pascal unit
#include <IdServerIOHandler.hpp>	// Pascal unit
#include <IdThread.hpp>	// Pascal unit
#include <IdGlobal.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idserveriohandlersocket
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdServerIOHandlerSocket;
class PASCALIMPLEMENTATION TIdServerIOHandlerSocket : public Idserveriohandler::TIdServerIOHandler 
{
	typedef Idserveriohandler::TIdServerIOHandler inherited;
	
public:
	virtual void __fastcall Init(void);
	virtual Idiohandler::TIdIOHandler* __fastcall Accept(int ASocket, Idthread::TIdThread* AThread = (Idthread::TIdThread*)(0x0));
public:
	#pragma option push -w-inl
	/* TIdComponent.Create */ inline __fastcall virtual TIdServerIOHandlerSocket(Classes::TComponent* axOwner) : Idserveriohandler::TIdServerIOHandler(axOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TIdComponent.Destroy */ inline __fastcall virtual ~TIdServerIOHandlerSocket(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idserveriohandlersocket */
using namespace Idserveriohandlersocket;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdServerIOHandlerSocket
