//---------------------------------------------------------------------------

#pragma hdrstop

#include "RunRateTcpStreamSender.h"

#include "RunRateTcpStreamSenderSettings.h"
#include "vector.h"

#include <system.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

ETcpStreamSenderRunRateInvalidIPPort::ETcpStreamSenderRunRateInvalidIPPort( UnicodeString inMsg ) :
								   Exception( inMsg )
{
}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TRunRateTcpStreamSender::TRunRateTcpStreamSender()
{
	TRunRateTcpStreamSender senderSettings = TRunRateTcpStreamSender::Instance();

	SetIPAddr( senderSettings.IPAddress );
	SetIPPort( senderSettings.IPPort    );
}
//............................................................................

TRunRateTcpStreamSender::TRunRateTcpStreamSender( AnsiString inTcpIPAddress, __int32 inTcpIPPort )
{
	SetIPAddr( inTcpIPAddress );
	SetIPPort( inTcpIPPort    );
}
//............................................................................

TRunRateTcpStreamSender::~TRunRateTcpStreamSender()
{
}
//............................................................................

void TRunRateTcpStreamSender::Send( UnicodeString inData )
{
	TRunRateTcpStreamSenderThread *senderThread = new TRunRateTcpStreamSenderThread(
												createTcpClient(),
												inData );

	senderThread->OnTerminate = &sendThreadTerminated;
	senderThread->Resume();
}
//............................................................................

void __fastcall TRunRateTcpStreamSender::sendThreadTerminated( TObject *Sender )
{
}
//............................................................................


#pragma mark ::::::::::::::::::::::::::::::
#pragma mark PRIVETE SECTION
#pragma mark ::::::::::::::::::::::::::::::

TIdTCPClient* TRunRateTcpStreamSender::createTcpClient()
{
	TIdTCPClient* result = new TIdTCPClient( NULL );

	//:::::::::::::::::::::::::::::::::::::::::::::::

	result->Host           = FIPAddr;
	result->Port           = FIPPort;
	result->BoundPort      = 0;
	result->ReadTimeout    = _READ_TIME_OUT;
	result->ConnectTimeout = _CONNECT_TIME_OUT;

	//:::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//............................................................................

#pragma mark :::::::::::::::::::::::::::::::::::::::::::::
#pragma mark Setters & Getters
#pragma mark :::::::::::::::::::::::::::::::::::::::::::::

void TRunRateTcpStreamSender::SetIPAddr( AnsiString inIPAddr )
{
	FIPAddr = inIPAddr;
}
//............................................................................

void TRunRateTcpStreamSender::SetIPPort( __int32 inIPPort )
{
	if( inIPPort < 57001 )
	{
		throw new ETcpStreamSenderRunRateInvalidIPPort( "Invalid TCP/IP Port number.  Port number must be equals or hihger than 600000" );
	}

	FIPPort = inIPPort;
}
//............................................................................

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  TTcpStreamSenderThread
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

__fastcall TRunRateTcpStreamSenderThread::TRunRateTcpStreamSenderThread(
									TIdTCPClient*  inTcpClient,
									AnsiString inData ) : TThread( true )
{
	FreeOnTerminate = true;

	_tcpClient   = inTcpClient;
	_data = inData;
}
//............................................................................

__fastcall TRunRateTcpStreamSenderThread::~TRunRateTcpStreamSenderThread()
{
   Terminate();
}

void __fastcall TRunRateTcpStreamSenderThread::Execute()
{
	if( _tcpClient != NULL )
   {
      while( !Terminated )
      {
         SendAndFetch( _data );
         Terminate();
      }

      try
      {
         delete _tcpClient;
      }
      catch( ...)
      {
      }
   }
}
//............................................................................

void TRunRateTcpStreamSenderThread::SendAndFetch( UnicodeString data )
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		bool retry      = false;
		int  retryCount = 0;
		do
		{
			_tcpClient->Connect();
			if( _tcpClient->Connected() )
			{
				sendData( _tcpClient, data );
			}
		}
		while( retry == true && retryCount < _DEFAULT_RETRY_COUNT );
	}
    catch( ...)
    {
    }

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}
//............................................................................

void TRunRateTcpStreamSenderThread::sendData( TIdTCPClient* inTcpClient, UnicodeString points )
{

      std::string data         = points.t_str();
	__int32 dataSize         = points.Length();
 	__int32 remainedDataSize = dataSize;
 //   __int32 remainedDataSize = data.size();

     __int32 maxBufferSize = points.Length(); // It can be assigned a lower value
	__int32 itPos = 0;

	char *buffer = new char[maxBufferSize];



	// Send Data Size
	inTcpClient->IOHandler->Write( maxBufferSize );

	// Send Data
	while( remainedDataSize > 0 )
	{
	   __int32 bufferSize = ( remainedDataSize >= maxBufferSize ) ? maxBufferSize : remainedDataSize;
	   data.copy( buffer, bufferSize, itPos );


	   inTcpClient->IOHandler->Write( createByteArray( buffer, bufferSize ), bufferSize );

	   itPos += bufferSize;
	   remainedDataSize -= bufferSize;
	}

	delete[] buffer;

	//inTcpClient->IOHandler->WriteLn( data );
}
//............................................................................

//............................................................................

TBytes TRunRateTcpStreamSenderThread::createByteArray( const char* inBuffer, __int32 inBufferSize )
{
	TBytes result;
	result.Length = inBufferSize;
   
	for( int i = 0; i < result.Length; i++ )
	{
	   result[i] = inBuffer[i];

	}

	return result;
}
//............................................................................
//............................................................................
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//............................................................................
//............................................................................


