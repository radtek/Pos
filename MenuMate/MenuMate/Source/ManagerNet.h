//---------------------------------------------------------------------------

#ifndef ManagerNetH
#define ManagerNetH
//---------------------------------------------------------------------------
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdUDPServer.hpp>
#include <IdGlobal.hpp>

#include "SimpleTCP.h"
#include "MM_DBCore.h"
#include "ManagerVariable.h"
#include "IdIPWatch.hpp"

#include <i_spinlock.hh>

class TManagerNet
{
private:	// User declarations
	TIdUDPClient *udp;
	TIdUDPServer *udpServer;
   std::auto_ptr<TIdIPWatch> ip_watcher_;

   /* Synchronizes access to the IP address members. We need to do this because
    * it's possible that the system is recalculating the addresses when the code
    * is trying to use them.
    */
   std::auto_ptr<utilities::locking::i_spinlock> ip_guard_;

	void __fastcall udpServerUDPRead(TIdUDPListenerThread *AThread, TBytes AData, TIdSocketHandle *ABinding);
	udpProduct ProductID;
   bool Enabled;

   void CacheMachineIPAndBroadcastIPAddresses();

   UnicodeString ip_address_;
   UnicodeString broadcast_address_;

public:		// User declarations
   // Used as a packet filter so MenuMate can share PC Networks.
   int MMSubNet;
	TManagerNet();
	virtual ~TManagerNet();

	const UnicodeString &GetBroadCastIP() const;
   const UnicodeString &GetIPAddress() const;

	void SendToAll(TNetMessage *Request);
	void SendTo(TNetMessage *Request,UnicodeString IP);
   void Initialise(Database::TDBTransaction &DBTransaction);
};
#endif
