//---------------------------------------------------------------------------


#pragma hdrstop

#include "ProxyMateManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TProxyMateManager::TProxyMateManager()
{
	FAppConnectorServer = NULL;
	FClientUp = true;
}
//...........................................................................

TProxyMateManager::~TProxyMateManager()
{
	if( FAppConnectorServer != NULL )
	{
		delete FAppConnectorServer;
	}
}
//...........................................................................

bool TProxyMateManager::Init()
{
	if( !initialized() )
	{
		FAppConnectorServer = new TMMAppConnectorServer();
		FAppConnectorServer->Init( "MMAppConnectorNZ", sizeof( __int32 ), sizeof( __int32 ) );

		FAppConnectorServer->OnClientUp       = mmAppConnectorClientUp;
		FAppConnectorServer->OnClientDown     = mmAppConnectorClientDown;
	}
}
//...........................................................................

void __fastcall TProxyMateManager::mmAppConnectorClientUp( TObject* Sender )
{
	FClientUp = true;
}
//---------------------------------------------------------------------------

void __fastcall TProxyMateManager::mmAppConnectorClientDown( TObject* Sender  )
{
	FClientUp = false;
}
//---------------------------------------------------------------------------

bool TProxyMateManager::initialized()
{
	return FAppConnectorServer != NULL;
}
//...........................................................................

