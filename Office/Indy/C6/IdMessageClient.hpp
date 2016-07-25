// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdMessageClient.pas' rev: 6.00

#ifndef IdMessageClientHPP
#define IdMessageClientHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdTCPConnection.hpp>	// Pascal unit
#include <IdComponent.hpp>	// Pascal unit
#include <IdIOHandler.hpp>	// Pascal unit
#include <IdHeaderList.hpp>	// Pascal unit
#include <IdTCPClient.hpp>	// Pascal unit
#include <IdMessage.hpp>	// Pascal unit
#include <IdIOHandlerStream.hpp>	// Pascal unit
#include <IdGlobal.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idmessageclient
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdIOHandlerStreamMsg;
class PASCALIMPLEMENTATION TIdIOHandlerStreamMsg : public Idiohandlerstream::TIdIOHandlerStream 
{
	typedef Idiohandlerstream::TIdIOHandlerStream inherited;
	
protected:
	AnsiString FTerminator;
	int FTerminatorIndex;
	char FLastCharRecv;
	
public:
	__fastcall virtual TIdIOHandlerStreamMsg(Classes::TComponent* AOwner);
	virtual bool __fastcall Readable(int AMSec = 0xffffffff);
	virtual int __fastcall Recv(void *ABuf, int ALen);
public:
	#pragma option push -w-inl
	/* TIdIOHandlerStream.Destroy */ inline __fastcall virtual ~TIdIOHandlerStreamMsg(void) { }
	#pragma option pop
	
};


class DELPHICLASS TIdMessageClient;
class PASCALIMPLEMENTATION TIdMessageClient : public Idtcpclient::TIdTCPClient 
{
	typedef Idtcpclient::TIdTCPClient inherited;
	
protected:
	int FMsgLineLength;
	AnsiString FMsgLineFold;
	virtual void __fastcall ReceiveBody(Idmessage::TIdMessage* AMsg, const AnsiString ADelim = ".");
	virtual AnsiString __fastcall ReceiveHeader(Idmessage::TIdMessage* AMsg, const AnsiString AAltTerm = "");
	virtual void __fastcall SendBody(Idmessage::TIdMessage* AMsg);
	virtual void __fastcall SendHeader(Idmessage::TIdMessage* AMsg);
	virtual void __fastcall WriteBodyText(Idmessage::TIdMessage* AMsg);
	void __fastcall WriteFoldedLine(const AnsiString ALine);
	
public:
	__fastcall virtual TIdMessageClient(Classes::TComponent* AOwner);
	void __fastcall ProcessMessage(Idmessage::TIdMessage* AMsg, bool AHeaderOnly = false)/* overload */;
	void __fastcall ProcessMessage(Idmessage::TIdMessage* AMsg, const Classes::TStream* AStream, bool AHeaderOnly = false)/* overload */;
	void __fastcall ProcessMessage(Idmessage::TIdMessage* AMsg, const AnsiString AFilename, bool AHeaderOnly = false)/* overload */;
	virtual void __fastcall SendMsg(Idmessage::TIdMessage* AMsg, const bool AHeadersOnly = false);
	__property int MsgLineLength = {read=FMsgLineLength, write=FMsgLineLength, nodefault};
	__property AnsiString MsgLineFold = {read=FMsgLineFold, write=FMsgLineFold};
public:
	#pragma option push -w-inl
	/* TIdTCPClient.Destroy */ inline __fastcall virtual ~TIdMessageClient(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idmessageclient */
using namespace Idmessageclient;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdMessageClient
