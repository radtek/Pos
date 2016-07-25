//---------------------------------------------------------------------------

#ifndef NetManagerClientH
#define NetManagerClientH
//---------------------------------------------------------------------------
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdUDPServer.hpp>
#include <IdIPWatch.hpp>
#include "SimpleTCP.h"

class TManagerNetClient
{
private:	// User declarations
	TIdUDPClient *udp;
	udpProduct ProductID;
public:		// User declarations
	TIdIPWatch *IPWatch;

	TManagerNetClient();
	virtual ~TManagerNetClient();

	AnsiString GetBroadCastIP();
	void SendToAll(TNetMessage *Request);
	void SendTo(TNetMessage *Request,AnsiString IP);
   void SendTo(TNetMessage *Request,AnsiString IP, int PORT);
};
#endif
