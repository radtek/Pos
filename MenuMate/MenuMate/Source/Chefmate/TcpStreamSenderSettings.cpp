//---------------------------------------------------------------------------


#pragma hdrstop

#include "TcpStreamSenderSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TTcpStreamSenderSettings::TTcpStreamSenderSettings()
{
	SetIPAddr( _DEFAULT_IP_ADDR );
	SetIPPort( _DEFAULT_IP_PORT );
}
//............................................................................

#pragma mark :::::::::::::::::::::::::::::::::::::::::::::
#pragma mark Setters & Getters
#pragma mark :::::::::::::::::::::::::::::::::::::::::::::

void TTcpStreamSenderSettings::SetIPAddr( AnsiString inIPAddr )
{
	FIPAddr = inIPAddr;
}
//............................................................................

void TTcpStreamSenderSettings::SetIPPort( __int32 inIPPort )
{
	FIPPort = inIPPort;
}
//............................................................................

//---------------------------------------------------------------------------
