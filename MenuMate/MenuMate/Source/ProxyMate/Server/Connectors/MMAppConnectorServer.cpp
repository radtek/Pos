//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMAppConnectorServer.h"

#include <process.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

UnicodeString AppConnectorClientPath = "C:\\Program Files (x86)\\menumate\\ProxyMate\\ProxyMate.exe";

//:::::::::::::::::::::::::::
//   TMMAppConnectorServer
//:::::::::::::::::::::::::::

/*
TMMAppConnectorServer* TMMAppConnectorServer::Instance()
{
	if( !instance )
	{
		instance = new TMMAppConnectorServer();
	}

	return ( TMMAppConnectorServer* )instance;
}
//...........................................................................
*/

__fastcall TMMAppConnectorServer::TMMAppConnectorServer() : TSMInterProcessCommServer()
{
	// Sent to client
	WM_MM_SERVER_DOWN     = registerWindowsMsg( "MM Server Down"    );
	WM_MM_CLOSE_CLIENT    = registerWindowsMsg( "MM Close Client"   );
	WM_MM_OPEN_CONNECTOR  = registerWindowsMsg( "MM Open Connector" );
	WM_MM_CLOSE_CONNECTOR = registerWindowsMsg( "MM Close Connector" );

	// Received from client
	WM_MM_CLIENT_UP           = registerWindowsMsg( "MM Client Up"           );
	WM_MM_CLIENT_DOWN         = registerWindowsMsg( "MM Client Down"         );  // It might not be needed
	WM_MM_OPEN_CONNECTOR_ACK  = registerWindowsMsg( "MM Open Connector ACK"  );
	WM_MM_CLOSE_CONNECTOR_ACK = registerWindowsMsg( "MM Close Connector ACK" );
}
//...........................................................................

__fastcall TMMAppConnectorServer::~TMMAppConnectorServer()
{
	signalClient( WM_MM_SERVER_DOWN, 0, 0 );
}
//...........................................................................

bool TMMAppConnectorServer::Init( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	if( TSMInterProcessCommServer::Init( inName, inServerCapacity, inClientCapacity ) )
	{
		mmProxyServer = TMMProxyServer::Instance();
		openClient();

		return true;
	}

	return false;
}
//...........................................................................

bool TMMAppConnectorServer::WillHandle( tagMSG &inMsg )
{
   return ( inMsg.message == WM_MM_CLIENT_UP ) ||
		    ( inMsg.message == WM_MM_CLIENT_DOWN ) ||
		    ( inMsg.message == WM_MM_OPEN_CONNECTOR_ACK ) ||
		    ( inMsg.message == WM_MM_CLOSE_CONNECTOR_ACK );
}

TMMConnectorServer* TMMAppConnectorServer::OpenConnector( TMMConnectorTypes inConnectorType, unsigned __int32* inParams, int inParamCount )
{
	TMMConnectorServer *result = NULL;

	//::::::::::::::::::::::::::::::::::

	switch( inConnectorType )
	{
		case mmcFloorPlan  : result = openFloorPlanConnector(   inParams[0], inParams[1] ); break;
		case mmcTablePicker: result = openTablePickerConnector( inParams[0], inParams[1] ); break;
	}

	//::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

void TMMAppConnectorServer::CloseConnector( TMMConnectorTypes inConnectorType )
{
	signalClient( WM_MM_CLOSE_CONNECTOR, inConnectorType, 0 );
	getConnector( inConnectorType )->Done();
}
//...........................................................................

void TMMAppConnectorServer::DropConnector( TMMConnectorTypes inConnectorType )
{
	getConnector( inConnectorType )->Done();
}
//...........................................................................

//void TMMAppConnectorServer::CloseClient()
//{
//	signalClient( WM_MM_CLOSE_CLIENT, 0, 0 );
//}
//...........................................................................

void TMMAppConnectorServer::openClient()
{
	//_wspawnl( P_NOWAIT, clientExecutableName().w_str(), clientExecutableParam().w_str(), NULL );
}
//...........................................................................

UnicodeString TMMAppConnectorServer::clientExecutableName()
{
   return AppConnectorClientPath;
}
//...........................................................................

UnicodeString TMMAppConnectorServer::clientExecutableParam()
{
   return "";
}
//...........................................................................

int TMMAppConnectorServer::registerWindowsMsg( UnicodeString inMsgStr )
{
   return RegisterWindowMessage( inMsgStr.t_str() );
}
//...........................................................................

TMMFloorPlanConnectorServer* TMMAppConnectorServer::openFloorPlanConnector( unsigned __int32 inMaxLocations, unsigned __int32 inMaxTablesInLocation )
{
	TMMFloorPlanConnectorServer *result = ( TMMFloorPlanConnectorServer* )getConnector( mmcFloorPlan );

	if( result->InitFloorPlan( inMaxLocations, inMaxTablesInLocation ) )
	{
		signalClient( WM_MM_OPEN_CONNECTOR, mmcFloorPlan, 0 );
	}
	else
	{
		result = NULL;
	}

	return result;
}
//...........................................................................

TMMTablePickerConnectorServer* TMMAppConnectorServer::openTablePickerConnector( unsigned __int32 inMaxLocations, unsigned __int32 inMaxTablesInLocation )
{
	TMMTablePickerConnectorServer *result = ( TMMTablePickerConnectorServer* )getConnector( mmcTablePicker );

	if( result->InitTablePicker( inMaxLocations, inMaxTablesInLocation ) )
	{
		signalClient( WM_MM_OPEN_CONNECTOR, mmcTablePicker, 0 );
	}

	return result;
}
//...........................................................................

void TMMAppConnectorServer::signalClient( int inMsg, int inWParam, int inLParam )
{
	PostMessage( ClientWMsgHandle, inMsg, inWParam, inWParam );
}
//...........................................................................

bool TMMAppConnectorServer::processMessage( TMessage& Msg )
{
	bool result = TSMInterProcessCommServer::processMessage( Msg );

	//:::::::::::::::::::::::::::::::::::::::

	if( !result )
	{
		if( Msg.Msg == WM_MM_CLIENT_UP )
		{
			processClientUp();
			result = true;
		}
		else
		if( Msg.Msg == WM_MM_CLIENT_DOWN )
		{
			processClientDown();
			result = true;
		}
		else
		if( Msg.Msg == WM_MM_OPEN_CONNECTOR_ACK )
		{
			processOpenConnectorAck( Msg.LParam, Msg.WParam );
			result = true;
		}
		else
		if( Msg.Msg == WM_MM_CLOSE_CONNECTOR_ACK )
		{
			processCloseConnectorAck( Msg.LParam, Msg.WParam );
			result = true;
		}
	}

	//:::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

void TMMAppConnectorServer::processClientUp()
{
	if( FOnClientUp != NULL )
	{
		( FOnClientUp )( this );
	}
}
//...........................................................................

void TMMAppConnectorServer::processClientDown()
{
	if( FOnClientDown != NULL )
	{
		( FOnClientDown )( this );
	}
}
//...........................................................................

void TMMAppConnectorServer::processOpenConnectorAck( int inACK, TMMConnectorTypes inConnectorType )
{
	if( FOpenConnectorAck != NULL )
	{
		( FOpenConnectorAck )( this, inACK, inConnectorType, getConnector( inConnectorType ) );
	}
}
//...........................................................................

void TMMAppConnectorServer::processCloseConnectorAck( int inACK, TMMConnectorTypes inConnectorType )
{
	if( FCloseConnectorAck != NULL )
	{
		( FCloseConnectorAck )( this, inACK, inConnectorType, getConnector( inConnectorType ) );
	}
}
//...........................................................................

TMMConnectorServer* TMMAppConnectorServer::getConnector( TMMConnectorTypes inConnectorType )
{
	return mmProxyServer.GetConnector( inConnectorType );
}
//...........................................................................
