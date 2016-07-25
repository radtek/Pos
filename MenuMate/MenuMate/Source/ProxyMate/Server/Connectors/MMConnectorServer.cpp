//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMConnectorServer.h"

#include <process.h>

#include "MMProxy.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::
//   TMMConnectorServer
//:::::::::::::::::::::::::::
__fastcall TMMConnectorServer::TMMConnectorServer() : TSMInterProcessCommServer()
{
	// Sent to client
	WM_MM_SERVER_DOWN     = registerWindowsMsg( "MM Server Down"     );
	WM_MM_CLOSE_CLIENT    = registerWindowsMsg( "MM Close Client"    );
	WM_MM_NEW_SERVER_DATA = registerWindowsMsg( "MM New Server Data" );

	// Received from client
	WM_MM_CLIENT_UP       = registerWindowsMsg( "MM Client Up"       );
	WM_MM_CLIENT_DOWN     = registerWindowsMsg( "MM Client Down"     );
	WM_MM_NEW_CLIENT_DATA = registerWindowsMsg( "MM New Client Data" );
}
//...........................................................................

__fastcall TMMConnectorServer::~TMMConnectorServer()
{
	signalClient( WM_MM_SERVER_DOWN, 0, 0 );
}
//...........................................................................

bool TMMConnectorServer::Init( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	TSMInterProcessCommServer::Init( inName, inServerCapacity, inClientCapacity );
}
//...........................................................................

bool TMMConnectorServer::WillHandle( tagMSG &inMsg )
{
   return ( inMsg.message == WM_MM_CLIENT_UP ) || ( inMsg.message == WM_MM_CLIENT_DOWN );
}
//...........................................................................

void TMMConnectorServer::closeClient()
{
	signalClient( WM_MM_CLOSE_CLIENT, 0, 0 );
}
//...........................................................................

int TMMConnectorServer::registerWindowsMsg( UnicodeString inMsgStr )
{
   return RegisterWindowMessage( inMsgStr.t_str() );
}
//...........................................................................

void TMMConnectorServer::signalClient( int inMsg, int inWParam, int inLParam )
{
	PostMessage( ClientWMsgHandle, inMsg, inWParam, inWParam );
}
//...........................................................................

bool TMMConnectorServer::processMessage( TMessage& Msg )
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
	}

	//:::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

void TMMConnectorServer::processClientUp()
{
	if( FOnClientUp != NULL )
	{
		( FOnClientUp )( this );
	}
}
//...........................................................................

void TMMConnectorServer::processClientDown()
{
	if( FOnClientDown != NULL )
	{
		( FOnClientDown )( this );
	}
}
//...........................................................................

