//---------------------------------------------------------------------------
#ifndef NetMessagesH
#define NetMessagesH
//---------------------------------------------------------------------------
//#include <IdUDPClient.hpp>
#include <IdTCPClient.hpp>
//#include <IdUDPServer.hpp>
#include <IdTCPServer.hpp>
#include <IdIPWatch.hpp>
#include "SerialObjectFactory.h"
//---------------------------------------------------------------------------
namespace NetMessage
{
//---------------------------------------------------------------------------
const int App_MenuMate	= 0x1001;
const int App_PalmMate	= 0x1002;
const int App_ChefMate	= 0x1004;
const int App_MMServer	= 0x1008;
//---------------------------------------------------------------------------
#if defined			(MenuMate)
	const int ProductID	= App_MenuMate;
#elif defined	(PalmMate)
	const int ProductID	= App_PalmMate;
#elif defined	(ChefMate)
	const int ProductID	= App_ChefMate;
#elif defined	(APP_MMSERVER)
	const int ProductID	= App_MMServer;
#else
	#error Product not defined in project conditional defines
#endif
//---------------------------------------------------------------------------
typedef int			TIDType;
typedef __int64	TRefType;
//---------------------------------------------------------------------------
// Global for product! Any other version of messages will be ignored
const int TCPMsgVer = 1;
//---------------------------------------------------------------------------
class TNetSerialObj
{
friend class TNetManager;
public:
	TNetSerialObj(TIDType ObjectID, int Reference = 0) :
			fObjectID(ObjectID),
			fVer(TCPMsgVer) {}

	void					SerializeOut(TSerialData &Data);
	bool					SerializeIn(TSerialData &Data);

	virtual void		Execute() = 0;

protected:
	virtual bool		Read(const TSerialData &Data) = 0;
	virtual void		Write(TSerialData &Data) const = 0;

	AnsiString			SourceIP;
	AnsiString			RecipientIP;
	int					Product;
	AnsiString			DeviceName;		// Windows computer name
	AnsiString			Description;
	TDateTime			Sent;
	TRefType				Reference;

private:
	const TIDType		fObjectID;
	const int			fVer;
	static TRefType	NextReference();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TNetManager
{
public:		// User declarations
	TNetManager(int ListenPort);

	void			Send(TNetSerialObj &Message, AnsiString HostAddress, int Port);
	void			Send(TNetSerialObj &Message, AnsiString HostAddress, const std::set<int> &Ports);

	void			Broadcast(TNetSerialObj &Message, int Port);
	void			Broadcast(TNetSerialObj &Message, const std::set<int> &Ports);

private:	// User declarations
//	std::auto_ptr<TIdUDPClient>	fUDPClient;
//	std::auto_ptr<TIdUDPServer>	fUDPServer;
	std::auto_ptr<TIdTCPClient>	fUDPClient;
	std::auto_ptr<TIdTCPServer>	fUDPServer;
	std::auto_ptr<TIdIPWatch>		fIPWatch;

	void __fastcall udpServerUDPRead(TObject *Sender, TStream *AData, TIdSocketHandle *ABinding);

	AnsiString	fGetBroadcastAddress();
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
