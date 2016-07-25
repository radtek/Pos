//---------------------------------------------------------------------------

#ifndef MMAppConnectorServerH
#define MMAppConnectorServerH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>

#include "SMInterProcessComm.h"
#include "MMProxy.h"

#include "MMFloorPlanConnectorServer.h"
#include "MMTablePickerConnectorServer.h"

//---------------------------------------------------------------------------

extern UnicodeString AppConnectorClientPath;

//:::::::::::::::::::::::::::
//   TMMAppConnectorServer
//:::::::::::::::::::::::::::

// Received from client
const EV_CLIENT_ACK_CONNECTOR_OPEN            = 0x0001;
const EV_CLIENT_ACK_CONNECTOR_ALREADY_OPEN    = 0x0002;
const EV_CLIENT_ACK_CONNECTOR_FAILED_TO_OPEN  = 0x0003;

typedef void __fastcall (__closure *TAppConnectorAckEvent)(System::TObject* Sender, int inACK, TMMConnectorTypes inConnectorType, TMMConnectorServer* inConnector);

class TMMAppConnectorServer : public TSMInterProcessCommServer
{
private:
	//static TSMInterProcessCommServer* instance;

protected:
	// Sent to client
	int WM_MM_SERVER_DOWN;
	int WM_MM_CLOSE_CLIENT;
	int WM_MM_OPEN_CONNECTOR;
	int WM_MM_CLOSE_CONNECTOR;

	// Received from client
	int WM_MM_CLIENT_UP;
	int WM_MM_CLIENT_DOWN;
	int WM_MM_OPEN_CONNECTOR_ACK;
	int WM_MM_CLOSE_CONNECTOR_ACK;

	TNotifyEvent          FOnClientUp;
	TNotifyEvent          FOnClientDown;
	TAppConnectorAckEvent FOpenConnectorAck;
	TAppConnectorAckEvent FCloseConnectorAck;

	TMMProxyServer mmProxyServer;

	void openClient();
	virtual UnicodeString clientExecutableName();
	virtual UnicodeString clientExecutableParam();

	int registerWindowsMsg( UnicodeString inMsgStr );

	TMMFloorPlanConnectorServer* openFloorPlanConnector(   unsigned __int32 inMaxLocations, unsigned __int32 inMaxTablesInLocation );
	TMMTablePickerConnectorServer* openTablePickerConnector( unsigned __int32 inMaxLocations, unsigned __int32 inMaxTablesInLocation );

	void signalClient( int inMsg, int inWParam, int inLParam );

	bool processMessage( Messages::TMessage& Msg );
	void processClientUp();
	void processClientDown();
	void processOpenConnectorAck(  int inACK, TMMConnectorTypes inConnectorType );
	void processCloseConnectorAck( int inACK, TMMConnectorTypes inConnectorType );

	TMMConnectorServer* getConnector( TMMConnectorTypes inConnectorType );

public:
	__fastcall  TMMAppConnectorServer();
	__fastcall ~TMMAppConnectorServer();

	//static TMMAppConnectorServer* Instance();

	bool Init( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );

	virtual bool WillHandle( tagMSG &inMsg );

	TMMConnectorServer* OpenConnector( TMMConnectorTypes inConnectorType, unsigned __int32* inParams, int inParamCount );
	void CloseConnector( TMMConnectorTypes inConnectorType ); // Signals the Connector Client to shut down and drops Connector Server
	void DropConnector(  TMMConnectorTypes inConnectorType ); // Drops Connector Server

	__property TNotifyEvent          OnClientUp        = { read=FOnClientUp,       write=FOnClientUp   };
	__property TNotifyEvent          OnClientDown      = { read=FOnClientDown,     write=FOnClientDown };
	__property TAppConnectorAckEvent OpenConnectorAck  = { read=FOpenConnectorAck, write=FOpenConnectorAck  };
	__property TAppConnectorAckEvent CloseConnectorAck = { read=FOpenConnectorAck, write=FCloseConnectorAck };
};

//---------------------------------------------------------------------------

#endif
