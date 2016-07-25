//---------------------------------------------------------------------------


#pragma hdrstop

#include "RunRateTcpStreamSenderSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TRunRateTcpStreamSenderSettings::TRunRateTcpStreamSenderSettings()
{
	SetIPAddr( _DEFAULT_IP_ADDR );
	SetIPPort( _DEFAULT_IP_PORT );
}
//............................................................................

#pragma mark :::::::::::::::::::::::::::::::::::::::::::::
#pragma mark Setters & Getters
#pragma mark :::::::::::::::::::::::::::::::::::::::::::::

void TRunRateTcpStreamSenderSettings::SetIPAddr( AnsiString inIPAddr )
{
	FIPAddr = inIPAddr;
}
//............................................................................

void TRunRateTcpStreamSenderSettings::SetIPPort( __int32 inIPPort )
{
	FIPPort = inIPPort;
}
//............................................................................

//---------------------------------------------------------------------------
