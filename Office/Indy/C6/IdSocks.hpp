// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdSocks.pas' rev: 6.00

#ifndef IdSocksHPP
#define IdSocksHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdStack.hpp>	// Pascal unit
#include <IdComponent.hpp>	// Pascal unit
#include <IdIOHandler.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idsocks
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TSocksVersion { svNoSocks, svSocks4, svSocks4A, svSocks5 };
#pragma option pop

#pragma option push -b-
enum TSocksAuthentication { saNoAuthentication, saUsernamePassword };
#pragma option pop

#pragma pack(push, 4)
struct TIdSocksRequest
{
	Byte Version;
	Byte OpCode;
	Word Port;
	Idstack::TIdInAddr IpAddr;
	System::SmallString<255>  UserName;
} ;
#pragma pack(pop)

#pragma pack(push, 4)
struct TIdSocksResponse
{
	Byte Version;
	Byte OpCode;
	Word Port;
	Idstack::TIdInAddr IpAddr;
} ;
#pragma pack(pop)

class DELPHICLASS TIdSocksInfo;
class PASCALIMPLEMENTATION TIdSocksInfo : public Idcomponent::TIdComponent 
{
	typedef Idcomponent::TIdComponent inherited;
	
protected:
	TSocksAuthentication FAuthentication;
	AnsiString FHost;
	AnsiString FPassword;
	int FPort;
	AnsiString FUsername;
	TSocksVersion FVersion;
	Idiohandler::TIdIOHandler* FIOHandler;
	virtual void __fastcall AssignTo(Classes::TPersistent* ASource);
	
public:
	__fastcall virtual TIdSocksInfo(Classes::TComponent* AOwner);
	void __fastcall MakeSocksConnection(const AnsiString AHost, const int APort);
	void __fastcall MakeSocks4Connection(const AnsiString AHost, const int APort);
	void __fastcall MakeSocks5Connection(const AnsiString AHost, const int APort);
	__property Idiohandler::TIdIOHandler* IOHandler = {write=FIOHandler};
	
__published:
	__property TSocksAuthentication Authentication = {read=FAuthentication, write=FAuthentication, default=0};
	__property AnsiString Host = {read=FHost, write=FHost};
	__property AnsiString Password = {read=FPassword, write=FPassword};
	__property int Port = {read=FPort, write=FPort, default=1080};
	__property AnsiString Username = {read=FUsername, write=FUsername};
	__property TSocksVersion Version = {read=FVersion, write=FVersion, default=0};
public:
	#pragma option push -w-inl
	/* TIdComponent.Destroy */ inline __fastcall virtual ~TIdSocksInfo(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
#define ID_SOCKS_AUTH (TSocksAuthentication)(0)
#define ID_SOCKS_VER (TSocksVersion)(0)

}	/* namespace Idsocks */
using namespace Idsocks;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdSocks
