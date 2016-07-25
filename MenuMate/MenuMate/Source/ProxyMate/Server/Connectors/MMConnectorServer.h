//---------------------------------------------------------------------------

#ifndef MMConnectorServerH
#define MMConnectorServerH

#include <vcl.h>
#include <vector>

#include "SMInterProcessComm.h"

//---------------------------------------------------------------------------

class TMMProxyServer;

//:::::::::::::::::::::::::::
//   TMMConnectorServer
//:::::::::::::::::::::::::::
class TMMConnectorServer : public TSMInterProcessCommServer
{
protected:
	// Sent to client
	int WM_MM_SERVER_DOWN;
	int WM_MM_CLOSE_CLIENT;
	int WM_MM_NEW_SERVER_DATA;

	// Received from client
	int WM_MM_CLIENT_UP;
	int WM_MM_CLIENT_DOWN;
	int WM_MM_NEW_CLIENT_DATA;

	TNotifyEvent FOnClientUp;
	TNotifyEvent FOnClientDown;

	__fastcall  TMMConnectorServer();
	__fastcall ~TMMConnectorServer();

	void closeClient();

	int registerWindowsMsg( UnicodeString inMsgStr );

	void signalClient( int inMsg, int inWParam, int inLParam );

	bool processMessage( Messages::TMessage& Msg );
	void processClientUp();
	void processClientDown();

public:
	friend TMMProxyServer;

	bool Init( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );

	virtual bool WillHandle( tagMSG &inMsg );

	__property TNotifyEvent OnClientUp      = { read=FOnClientUp,      write=FOnClientUp      };
	__property TNotifyEvent OnClientDown    = { read=FOnClientDown,    write=FOnClientDown    };
};

//---------------------------------------------------------------------------

#endif
