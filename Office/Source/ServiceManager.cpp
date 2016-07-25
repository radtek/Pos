//---------------------------------------------------------------------------


#pragma hdrstop

#include "ServiceManager.h"

//#include "MMMessageBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//----------------------------------------------------------------------------

TServiceManager::TServiceManager()
{
	FRemoteHostManager = new TRemoteHostManager();

	SetAndValidate( LocalHost(), "", "" );
}
//----------------------------------------------------------------------------
TServiceManager::~TServiceManager()
{
	CloseServiceManager();
	delete FRemoteHostManager;
}
//----------------------------------------------------------------------------
void TServiceManager::SetAndValidate( AnsiString inServiceMachineName, AnsiString inUserName, AnsiString inPassword )
{
	FSvcMachineName = inServiceMachineName;
	FUserName       = inUserName;
	FPassword		= inPassword;

	Validate();
}
//---------------------------------------------------------------------------
void TServiceManager::Validate()
{
	ResetServiceManager();
}
//---------------------------------------------------------------------------
TServiceStatus TServiceManager::GetServiceStatus( AnsiString inServiceName )
{
	try
	{
		_sc = OpenServiceHdl( inServiceName );

		if( _sc > 0 )
		{
		   SERVICE_STATUS sStatus;
		   unsigned dwStat;

		   if( QueryServiceStatus( _sc, &sStatus ) )
		   {
		   	  dwStat = sStatus.dwCurrentState;
		   }

		   CloseService();

		   return ( TServiceStatus )dwStat;
		}
		else
		{
			return ssNotInstalled;
		}
	}
	catch( ... )
	{
    	return ssErrorOpening;
	}
}
//----------------------------------------------------------------------------
bool TServiceManager::IsServiceInstalled( AnsiString inServiceName )
{
	 TServiceStatus status = GetServiceStatus( inServiceName );

	 return ( status != ssErrorOpening ) && ( status != ssNotInstalled );
}
//----------------------------------------------------------------------------
bool TServiceManager::IsServiceRunning( AnsiString inServiceName )
{
	return GetServiceStatus( inServiceName ) == ssRunning;
}
//----------------------------------------------------------------------------
bool TServiceManager::IsServiceStopped( AnsiString inServiceName )
{
	return GetServiceStatus( inServiceName ) == ssStopped;
}
//----------------------------------------------------------------------------
bool TServiceManager::IsServicePaused( AnsiString inServiceName )
{
	return GetServiceStatus( inServiceName ) == ssPaused;
}
//----------------------------------------------------------------------------

void TServiceManager::OpenServiceManager()
{
	AnsiString hostName;

	if( IsLocalHost( FSvcMachineName ) )
	{
		hostName = FSvcMachineName;
	}
	else
	{
		ConnectToRemoteHost();
		hostName = "\\\\" + FSvcMachineName;
	}

	_host = hostName.c_str();
	_db   = NULL; // ServicesActive

	_scm  = OpenSCManager( _host, _db, SC_MANAGER_CONNECT );
}
//----------------------------------------------------------------------------
void TServiceManager::ResetServiceManager()
{
	CloseServiceManager();
    OpenServiceManager();
}
//----------------------------------------------------------------------------
void TServiceManager::CloseServiceManager()
{
	if( _scm > 0 )
	{
		CloseServiceHandle( _scm );
	}
}
//----------------------------------------------------------------------------
TServiceHandle TServiceManager::OpenServiceHdl( AnsiString inServiceName )
{
	if( _scm > 0 )
	{
		return OpenService( _scm, inServiceName.c_str(), SERVICE_QUERY_STATUS);
	}
	else
	{
		return 0;
    }
}
//----------------------------------------------------------------------------
void TServiceManager::CloseService()
{
	if( _sc > 0 )
	{
		CloseServiceHandle( _sc );
	}
}
//----------------------------------------------------------------------------
void TServiceManager::writeServiceMachineName( AnsiString inMachineName )
{
	FSvcMachineName = ResolveHostName( inMachineName );

	ResetServiceManager();
}
//----------------------------------------------------------------------------
void TServiceManager::writeUserName( AnsiString inUserName )
{
	FUserName = inUserName;
}
//----------------------------------------------------------------------------
void TServiceManager::writePassword( AnsiString inPassword )
{
	FPassword = inPassword;
}
//----------------------------------------------------------------------------
AnsiString TServiceManager::ResolveHostName( AnsiString inHostName )
{
	return FRemoteHostManager->ResolveHostName( inHostName );
}
//---------------------------------------------------------------------------
 bool TServiceManager::IsLocalHost( AnsiString inHostName )
{
	return FRemoteHostManager->IsLocalHost( inHostName );
}
//---------------------------------------------------------------------------
AnsiString TServiceManager::LocalHost()
{
	return FRemoteHostManager->LocalHost();
}
//---------------------------------------------------------------------------
bool TServiceManager::ConnectToRemoteHost()
{
	FRemoteHostManager->SetAndValidate( FSvcMachineName, FUserName, FPassword );

	return FRemoteHostManager->LastError == rhmNO_ERROR;
}
//---------------------------------------------------------------------------

