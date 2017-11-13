//---------------------------------------------------------------------------


#pragma hdrstop

#include "OracleTCPIP.h"
#include "MMLogging.h"
#include "DeviceRealterminal.h"
#include "OracledataBuilder.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
TOracleTCPIP::TOracleTCPIP()
{
    tcpClient = new TIdTCPClient(NULL);
    CreateTCPClient();
}
//---------------------------------------------------------------------------
TOracleTCPIP::~TOracleTCPIP()
{
    if(tcpClient)
    {
        delete tcpClient;
        tcpClient = NULL;
    }
}
//---------------------------------------------------------------------------
void TOracleTCPIP::CreateTCPClient()
{
	tcpClient->Host           = TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
//    MessageBox(tcpClient->Host,"Host",MB_OK);
	tcpClient->Port           = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
//    MessageBox(tcpClient->Port,"Port",MB_OK);
	tcpClient->BoundPort      = 0;
	tcpClient->ReadTimeout    = _READ_TIME_OUT;
	tcpClient->ConnectTimeout = _CONNECT_TIME_OUT;
//    MessageBox("client created","",MB_OK);
}
//---------------------------------------------------------------------------
bool TOracleTCPIP::Connect()
{
    bool retValue = false;
	try
	{
//       MessageBox(tcpClient->Host,"Host",MB_OK);
//       MessageBox(tcpClient->Port,"Port",MB_OK);
       if( tcpClient->Connected() )
	   {
            Disconnect();
            CreateTCPClient();
       }
       tcpClient->Connect();
       retValue = true;
	}
	catch( Exception& E)
	{
       	MessageBox(E.Message+"\nPlease check IP address and Port number Values",
                                                 "Abort", MB_OK + MB_ICONERROR);
	}
    return retValue;
}
//----------------------------------------------------------------------------
bool TOracleTCPIP::Disconnect()
{
    if(tcpClient->Connected())
        tcpClient->Disconnect();
}
//---------------------------------------------------------------------------
AnsiString TOracleTCPIP::SendAndFetch(AnsiString inData)
{
    AnsiString outResponse = "";
	if (inData != "" && tcpClient->Connected())
	{
        try
        {
            sendData(inData);
            outResponse = fetchResponse();
        }
        catch( Exception& exc)
        {
            MessageBox("Could not communicate with server.","Error",MB_OK + MB_ICONERROR);
        }
	}
    else if (!tcpClient->Connected())
    {
        MessageBox("Connection Not Open", "Error",MB_OK + MB_ICONERROR);
    }
    return outResponse;
}
//---------------------------------------------------------------------------
void TOracleTCPIP::sendData( AnsiString inData )
{
    std::string data = inData.c_str();
	__int32 dataSize         = data.size();
 	__int32 remainedDataSize = dataSize;

     __int32 maxBufferSize = data.size(); // It can be assigned a lower value
     maxBufferSize += 2;                  // added 2 for STX and ETX
	__int32 itPos = 0;

	char   *buffer = new char[maxBufferSize];
	// Send Data Size
	tcpClient->IOHandler->Write( maxBufferSize );

	// Send Data
    tcpClient->IOHandler->Write( CreateSTX(), 1 );
	while( remainedDataSize > 0 )
	{
	   __int32 bufferSize = ( remainedDataSize >= maxBufferSize ) ? maxBufferSize : remainedDataSize;
	   data.copy( buffer, bufferSize, itPos );


	   tcpClient->IOHandler->Write( CreateByteArray( buffer, bufferSize), bufferSize );

	   itPos += bufferSize;
	   remainedDataSize -= bufferSize;
	}
    tcpClient->IOHandler->Write( CreateETX(), 1 );
	delete[] buffer;
}
//---------------------------------------------------------------------------
AnsiString TOracleTCPIP::fetchResponse()
{
	std::string data;
    AnsiString outData = "";
	if( tcpClient->IOHandler->Readable( _READ_TIME_OUT ) )
	{
       tcpClient->IOHandler->CheckForDataOnSource(500);
       tcpClient->IOHandler->ReadTimeout = 100;
       TByteDynArray buffer;
//       Sleep(1000);
       tcpClient->IOHandler->ReadBytes( buffer, -1, true );
       outData = AnsiString((char*) & buffer[0], buffer.Length);
    }
    outData = outData.SubString(2,outData.Length()-2);
    return outData;
}
//............................................................................
TBytes TOracleTCPIP::CreateByteArray( const char* inBuffer, __int32 inBufferSize )
{
	TBytes result;
	result.Length = inBufferSize;

	for( int i = 0; i < result.Length; i++ )
	{
	   result[i] = inBuffer[i];
	}
	return result;
}
//---------------------------------------------------------------------------
TBytes TOracleTCPIP::CreateSTX()
{
    TBytes STX;
    STX.Length = 1;
    STX[0] = 0x02;
    return STX;
}
//---------------------------------------------------------------------------
TBytes TOracleTCPIP::CreateETX()
{
    TBytes ETX;
    ETX.Length = 1;
    ETX[0] = 0x03;
    return ETX;
}
//---------------------------------------------------------------------------
