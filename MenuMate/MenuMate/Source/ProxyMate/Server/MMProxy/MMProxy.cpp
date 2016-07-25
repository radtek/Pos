//---------------------------------------------------------------------------

#pragma hdrstop

#include "MMProxy.h"

#include "MMFloorPlanConnectorServer.h"
#include "MMTablePickerConnectorServer.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

UnicodeString MenumatePath = "C:\\Program Files (x86)\\menumate";

//:::::::::::::::::::::::::::
//   TMMProxyServer
//:::::::::::::::::::::::::::
TMMProxyServer::TMMProxyServer()
{
	_connectors.push_back( new TMMFloorPlanConnectorServer()   );
	_connectors.push_back( new TMMTablePickerConnectorServer() );
}
//...........................................................................

TMMProxyServer::~TMMProxyServer()
{
	_connectors.clear();
}
//...........................................................................

TMMConnectorServer* TMMProxyServer::GetConnector( TMMConnectorTypes inConnectorType )
{
   try
   {
	   int index = ( int )inConnectorType;
	   return _connectors[index];
   }
   catch( ... )
   {
	   return NULL;
   }
}
//...........................................................................

