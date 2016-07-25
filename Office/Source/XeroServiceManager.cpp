//---------------------------------------------------------------------------


#pragma hdrstop

#include "XeroServiceManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TXeroServiceManager::TXeroServiceManager() : TServiceManager()
{
	ServiceMachineName = "";
	_serviceName       = "mmXeroIntegrationService";
}
//---------------------------------------------------------------------------
bool TXeroServiceManager::IsServiceInstalled()
{
	 TServiceStatus status = GetServiceStatus( _serviceName );

	 return  ( status != ssErrorOpening ) && ( status != ssNotInstalled );
}
//----------------------------------------------------------------------------
bool TXeroServiceManager::IsServiceRunning()
{
	return GetServiceStatus( _serviceName ) == ssRunning;
}
//----------------------------------------------------------------------------
bool TXeroServiceManager::IsServiceStopped()
{
	return GetServiceStatus( _serviceName ) == ssStopped;
}
//----------------------------------------------------------------------------
bool TXeroServiceManager::IsServicePaused()
{
	return GetServiceStatus( _serviceName ) == ssPaused;
}
//----------------------------------------------------------------------------

