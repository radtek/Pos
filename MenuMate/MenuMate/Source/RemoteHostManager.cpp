//---------------------------------------------------------------------------


#pragma hdrstop

#include "RemoteHostManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TRemoteHostManager::TRemoteHostManager()
{
	SetAndValidate( LocalHost(), "", "" );
}
//---------------------------------------------------------------------------
void TRemoteHostManager::SetAndValidate( AnsiString inHostName, AnsiString inUserName, AnsiString inPassword )
{
	HostName   = inHostName;
	UserName   = inUserName;
	Password   = inPassword;

	Validate();
}
//---------------------------------------------------------------------------
void TRemoteHostManager::Validate()
{
	SetLastError( ConnectToRemoteHost() ? rhmNO_ERROR : rhmFAILED_TO_CONNECT_TO_REMOTE_HOST );
}
//---------------------------------------------------------------------------
bool TRemoteHostManager::ConnectToRemoteHost()
{
	DWORD       dwRetVal;
	NETRESOURCE nr;
	DWORD       dwFlags;

// Zero out the NETRESOURCE struct
	memset(&nr, 0, sizeof (NETRESOURCE));

// Assign our values to the NETRESOURCE structure.

	AnsiString remoteName = "\\\\" + FHostName + "\\IPC$";

	nr.lpLocalName  = "";
	nr.lpRemoteName = remoteName.c_str();
	nr.lpProvider   = "";
	nr.dwType       = RESOURCETYPE_ANY;
	nr.dwScope      = RESOURCE_GLOBALNET;
	nr.dwType       = RESOURCETYPE_ANY;
	nr.dwUsage      = RESOURCEUSAGE_CONNECTABLE;

// Assign a value to the connection options
	dwFlags = CONNECT_UPDATE_PROFILE;
//
// Call the WNetAddConnection2 function to assign
//   a drive letter to the share.
//
	dwRetVal = WNetAddConnection2( &nr, FPassword.c_str(), FUserName.c_str(), dwFlags );
//
// If the call succeeds, inform the user; otherwise,
//  print the error.
//
	return ( dwRetVal == NO_ERROR ) || ( dwRetVal == ERROR_SESSION_CREDENTIAL_CONFLICT );
}
//---------------------------------------------------------------------------
void TRemoteHostManager::writeHostName( AnsiString inHostName )
{
	FHostName = ResolveHostName( inHostName );
}
//---------------------------------------------------------------------------
void TRemoteHostManager::writeUserName( AnsiString inUserName )
{
	FUserName = inUserName;
}
//---------------------------------------------------------------------------
void TRemoteHostManager::writePassword( AnsiString inPassword )
{
	FPassword = inPassword;
}
//---------------------------------------------------------------------------
AnsiString TRemoteHostManager::ResolveHostName( AnsiString inHostName )
{
	return IsLocalHost( inHostName ) ? LocalHost() : inHostName;
}
//---------------------------------------------------------------------------
 bool TRemoteHostManager::IsLocalHost( AnsiString inHostName )
{
	AnsiString tmp = inHostName.LowerCase();

	return ( tmp == LocalHost() ) || ( tmp == "127.0.0.1" );
}
//---------------------------------------------------------------------------
AnsiString TRemoteHostManager::LocalHost()
{
	return "localhost";
}
//---------------------------------------------------------------------------
void TRemoteHostManager::SetLastError( TRemoteHostManagerError inError)
{
	FLastError = inError;

	switch( FLastError )
	{
	   case rhmNO_ERROR                        : FLastErrorMsg = "No error"; break;
	   case rhmFAILED_TO_CONNECT_TO_REMOTE_HOST: FLastErrorMsg = "Failed to connect to remote host"; break;

	   default: FLastErrorMsg = "Unknown error"; break;
	}
}
//---------------------------------------------------------------------------


