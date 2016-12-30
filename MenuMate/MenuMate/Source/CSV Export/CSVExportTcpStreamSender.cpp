//---------------------------------------------------------------------------

#pragma hdrstop

#include "CSVExportTcpStreamSender.h"
#include "vector.h"

#include <system.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

ETcpStreamSenderCSVExportInvalidIPPort::ETcpStreamSenderCSVExportInvalidIPPort( UnicodeString inMsg ) :
								   Exception( inMsg )
{
}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TCSVExportTcpStreamSender::TCSVExportTcpStreamSender()
{
	TCSVExportTcpStreamSender senderSettings = TCSVExportTcpStreamSender::Instance();

	SetIPAddr( senderSettings.IPAddress );
	SetIPPort( senderSettings.IPPort    );
}
//............................................................................

TCSVExportTcpStreamSender::TCSVExportTcpStreamSender( AnsiString inTcpIPAddress, __int32 inTcpIPPort )
{
	SetIPAddr( inTcpIPAddress );
	SetIPPort( inTcpIPPort    );
}
//............................................................................

TCSVExportTcpStreamSender::~TCSVExportTcpStreamSender()
{
}
//............................................................................

void TCSVExportTcpStreamSender::Send( TStringList* csvData  )
{
	TCSVExportTcpStreamSenderThread *senderThread = new TCSVExportTcpStreamSenderThread(
												createTcpClient(),
												csvData );

	senderThread->OnTerminate = &sendThreadTerminated;
	senderThread->Resume();
}
//............................................................................

void __fastcall TCSVExportTcpStreamSender::sendThreadTerminated( TObject *Sender )
{
}
//............................................................................


#pragma mark ::::::::::::::::::::::::::::::
#pragma mark PRIVETE SECTION
#pragma mark ::::::::::::::::::::::::::::::

TIdTCPClient* TCSVExportTcpStreamSender::createTcpClient()
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

void TCSVExportTcpStreamSender::SetIPAddr( AnsiString inIPAddr )
{
	FIPAddr = inIPAddr;
}
//............................................................................

void TCSVExportTcpStreamSender::SetIPPort( __int32 inIPPort )
{
	if( inIPPort < 47001 )
	{
		throw new ETcpStreamSenderCSVExportInvalidIPPort( "Invalid TCP/IP Port number.  Port number must be equals to 47001" );
	}

	FIPPort = inIPPort;
}
//............................................................................

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  TTcpStreamSenderThread
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

__fastcall TCSVExportTcpStreamSenderThread::TCSVExportTcpStreamSenderThread(
									TIdTCPClient*  inTcpClient,
									TStringList* csvData  ) : TThread( true )
{
	FreeOnTerminate = true;
	_tcpClient   = inTcpClient;
    _csvStream = new TMemoryStream;
    csvData->SaveToStream(_csvStream);
}
//............................................................................

__fastcall TCSVExportTcpStreamSenderThread::~TCSVExportTcpStreamSenderThread()
{
   Terminate();
}

void __fastcall TCSVExportTcpStreamSenderThread::Execute()
{
	if( _tcpClient != NULL )
   {
      while( !Terminated )
      {
         SendAndFetch();
         Terminate();
      }

      try
      {
         delete _tcpClient;
         delete _csvStream;
      }
      catch( ...)
      {
      }
   }
}
//............................................................................

void TCSVExportTcpStreamSenderThread::SendAndFetch()
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
				sendData( _tcpClient);
			}
		}
		while( retry == true && retryCount < _DEFAULT_RETRY_COUNT );
	}
    catch( Exception &Err)
    {
       bool retry      = false;
    }

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}
//............................................................................

void TCSVExportTcpStreamSenderThread::sendData( TIdTCPClient* inTcpClient)
{
    _csvStream->Position = 0;
    char *buffer = (char*)_csvStream->Memory;
    __int32 maxBufferSize = _csvStream->Size;
    __int32 remainedDataSize = maxBufferSize;
    __int32 itPos = 0;
    // Send Data Size
    inTcpClient->IOHandler->Write( maxBufferSize );

    // Send Data
    while( remainedDataSize > 0 )
    {
        __int32 bufferSize = ( remainedDataSize >= maxBufferSize ) ? maxBufferSize : remainedDataSize;
        inTcpClient->IOHandler->Write( createByteArray( buffer, bufferSize ), bufferSize );
        itPos += bufferSize;
        remainedDataSize -= bufferSize;
    }

    delete[] buffer;
}
//............................................................................

//............................................................................

TBytes TCSVExportTcpStreamSenderThread::createByteArray( const char* inBuffer, __int32 inBufferSize )
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


