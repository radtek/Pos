// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdSysLog.pas' rev: 6.00

#ifndef IdSysLogHPP
#define IdSysLogHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdComponent.hpp>	// Pascal unit
#include <IdUDPClient.hpp>	// Pascal unit
#include <IdUDPBase.hpp>	// Pascal unit
#include <IdSysLogMessage.hpp>	// Pascal unit
#include <IdSocketHandle.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idsyslog
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdSysLog;
class PASCALIMPLEMENTATION TIdSysLog : public Idudpclient::TIdUDPClient 
{
	typedef Idudpclient::TIdUDPClient inherited;
	
protected:
	virtual Idsockethandle::TIdSocketHandle* __fastcall GetBinding(void);
	
public:
	__fastcall virtual TIdSysLog(Classes::TComponent* AOwner);
	void __fastcall SendMessage(const Idsyslogmessage::TIdSysLogMessage* AMsg, const bool AAutoTimeStamp = true)/* overload */;
	void __fastcall SendMessage(const AnsiString AMsg, const Idsyslogmessage::TIdSyslogFacility AFacility, const Idsyslogmessage::TIdSyslogSeverity ASeverity)/* overload */;
	void __fastcall SendMessage(const AnsiString AProcess, const AnsiString AText, const Idsyslogmessage::TIdSyslogFacility AFacility, const Idsyslogmessage::TIdSyslogSeverity ASeverity, const bool AUsePID = false, const int APID = 0xffffffff)/* overload */;
	
__published:
	__property Port  = {default=514};
public:
	#pragma option push -w-inl
	/* TIdUDPBase.Destroy */ inline __fastcall virtual ~TIdSysLog(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idsyslog */
using namespace Idsyslog;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdSysLog
