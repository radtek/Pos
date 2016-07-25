//---------------------------------------------------------------------------

#ifndef NetManagerH
#define NetManagerH
//---------------------------------------------------------------------------
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdUDPServer.hpp>
#include <IdGlobal.hpp>

#include <IdIPWatch.hpp>
#include "SimpleTCP.h"
#include "MM_DBCore.h"
#include "ManagerVariable.h"

class TManagerNet
{
private:	// User declarations
	TIdUDPClient *udp;
	TIdUDPServer *udpServer;

	void __fastcall udpServerUDPRead(TIdUDPListenerThread *AThread, TBytes AData, TIdSocketHandle *ABinding);
	udpProduct ProductID;
   bool Enabled;
public:		// User declarations
	TIdIPWatch *IPWatch;
   
   // Used as a packet filter so MenuMate can share PC Networks.
   int MMSubNet;
	TManagerNet();
	virtual ~TManagerNet();

	UnicodeString GetBroadCastIP();
	void SendToAll(TNetMessage *Request);
	void SendTo(TNetMessage *Request,UnicodeString IP);
   void Initialise(Database::TDBTransaction &DBTransaction,TManagerVariable *Vars);
};
#endif
