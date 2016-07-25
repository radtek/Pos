//---------------------------------------------------------------------------

#ifndef MembershipEbetTCPH
#define MembershipEbetTCPH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <map>
#include <system.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <memory>

#include "MembershipEbetData.h"

class TMembershipEBetTCP /*: public TThread*/
{
private:	// User declarations
//	void __fastcall TMembersipEBetTCP::Execute();

	std::auto_ptr<TIdTCPClient>	fTCPClient;
	int PacketNo;
	AnsiString IP;
	int        Port;
	int        ReadTimeOut;
	int 	   ConnectTimeOut;
	TDateTime  LastTransaction;
	int GeneratePacketNumber();
public:		// User declarations
	void SendAndFetch(AnsiString HostAddress, int Port);
	TMembershipEBetTCP();
	__fastcall ~TMembershipEBetTCP();

	TMembershipEBetOutBoundPacket OutPacket;
	TMembershipEBetInBoundPacket InPacket;
	void Init(AnsiString inIP,int inPort, int inReadTimeOut);
	bool RequestMemberInfoByCard(int inTilID,AnsiString inCardData);
	bool RequestMemberInfoByMemberNumber(int inTilID,AnsiString MemberNumber);
	bool RequestAddPoints(int inTilID, AnsiString MemberNumber, int Points);
	bool RequestRedeemedPoints(int inTilID, AnsiString MemberNumber, int Points);
};

#endif
