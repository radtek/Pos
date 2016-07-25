//---------------------------------------------------------------------------


#pragma hdrstop

#include "SMInterProcessComm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::
//   TSMInterProcessComm
//:::::::::::::::::::::::::::
__fastcall TSMInterProcessComm::TSMInterProcessComm(): TMsgReceiver()
{
	_active = false;
	_sharedMemoryController = NULL;
	_initPosition           = sizeof(__int32); // Right after the HWNDHandle
}
//...........................................................................

__fastcall TSMInterProcessComm::~TSMInterProcessComm()
{
	if( _sharedMemoryController != NULL )
	{
		delete _sharedMemoryController;
	}
}
//...........................................................................

bool TSMInterProcessComm::Init( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	if( _sharedMemoryController == NULL )
	{
		try
		{
			_sharedMemoryController = createSharedMemoryController( inName, inServerCapacity, inClientCapacity );
			publishHWNDHandle( fMsgHandlerHWND );
			_active = true;

			return true;
		}
		catch( ... )
		{
			return false;
		}
	}
	else
	{
		return true;
    }
}
//...........................................................................

void TSMInterProcessComm::Done()
{
	if( _active )
	{
		_active = false;
		delete _sharedMemoryController;
		_sharedMemoryController = NULL;
	}
}
//...........................................................................

void TSMInterProcessComm::publishHWNDHandle( HWND inHandle )
{
	_sharedMemoryController->Write( 0, ( __int32 )inHandle );
}
//...........................................................................

TSharedMemoryController* TSMInterProcessComm::createSharedMemoryController( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	return NULL;
}
//...........................................................................

//:::::::::::::::::::::::::::
//   TSMInterProcessCommServer
//:::::::::::::::::::::::::::
__fastcall TSMInterProcessCommServer::TSMInterProcessCommServer(): TSMInterProcessComm()
{
}
//...........................................................................

__fastcall TSMInterProcessCommServer::~TSMInterProcessCommServer()
{
}
//...........................................................................

TSharedMemoryController* TSMInterProcessCommServer::createSharedMemoryController( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	return TSharedMemControllerBuilder::Instance().BuildServerController( inName, inServerCapacity, inClientCapacity );
}
//...........................................................................

HWND TSMInterProcessCommServer::GetClientWMsgHandle()
{
	return ( HWND )_sharedMemoryController->ReadInt32( 0 );
}
//...........................................................................

//:::::::::::::::::::::::::::
//   TSMInterProcessCommClient
//:::::::::::::::::::::::::::
__fastcall TSMInterProcessCommClient::TSMInterProcessCommClient(): TSMInterProcessComm()
{
}
//...........................................................................

__fastcall TSMInterProcessCommClient::~TSMInterProcessCommClient()
{
}
//...........................................................................

TSharedMemoryController* TSMInterProcessCommClient::createSharedMemoryController( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	return TSharedMemControllerBuilder::Instance().BuildClientController( inName, inServerCapacity, inClientCapacity );
}
//...........................................................................

HWND TSMInterProcessCommClient::GetServerWMsgHandle()
{
	return ( HWND )_sharedMemoryController->ReadInt32( 0 );
}
//...........................................................................

