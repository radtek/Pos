//---------------------------------------------------------------------------


#pragma hdrstop

#include "TCPFiscal.h"
#include "MMMessageBox.h"
#include "GlobalSettings.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
TTCPFiscal::TTCPFiscal()
{
    _tcpClient = new TIdTCPClient( NULL );
	_tcpClient->Host           = "";
	_tcpClient->Port           = TGlobalSettings::Instance().FiscalServerPortNumber;
	_tcpClient->BoundPort      = 0;
	_tcpClient->ReadTimeout    = _READ_TIME_OUT;
	_tcpClient->ConnectTimeout = _CONNECT_TIME_OUT;
}
//---------------------------------------------------------------------------
TTCPFiscal::~TTCPFiscal()
{
    if(_tcpClient)
    {
        delete _tcpClient;
        _tcpClient = NULL;
    }
}
//---------------------------------------------------------------------------
AnsiString TTCPFiscal::SendAndFetch(UnicodeString data, PostType type)
{
    UnicodeString retValue = "";
    try
    {
        if(ConnectToServer())
        {
            SendData(data);
            retValue = FetchResponse();
        }
        else
        {
            MessageBox("Connection to Fiscal Server was not made","Error", MB_OK + MB_ICONERROR);
        }
    }
    catch (Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
        MessageBox(Exc.Message,"Error",MB_OK);
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TTCPFiscal::ConnectToServer()
{
    bool retValue = false;
    try
    {
       if(!_tcpClient->Connected())
       {
           MessageBox(_tcpClient->Host,_tcpClient->Port,MB_OK);
           MessageBox(_tcpClient->ConnectTimeout,"jdshks",MB_OK);
           _tcpClient->Connect();
           MessageBox("after connect of FiscalTCPIP","shivashu",MB_OK);
           retValue = true;
       }
     }
     catch (Exception &Exc)
     {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
     }
     return retValue;
}
//---------------------------------------------------------------------------
void TTCPFiscal::DisconnectFromServer()
{
    if(_tcpClient->Connected())
       _tcpClient->Disconnect();
}
//---------------------------------------------------------------------------
void TTCPFiscal::SendData(AnsiString indata)
{
    std::string data = indata.c_str();
	__int32 dataSize         = data.size();
 	__int32 remainedDataSize = dataSize;

     __int32 maxBufferSize = data.size(); // It can be assigned a lower value
	__int32 itPos = 0;

	char   *buffer = new char[maxBufferSize];

	// Send Data
    __int32 bufferSize = ( remainedDataSize >= maxBufferSize ) ? maxBufferSize : remainedDataSize;
    data.copy( buffer, bufferSize, itPos );
    _tcpClient->IOHandler->Write( CreateByteArray( buffer, bufferSize), bufferSize );
	delete[] buffer;
}
//............................................................................
TBytes TTCPFiscal::CreateByteArray( const char* inBuffer, __int32 inBufferSize )
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
AnsiString TTCPFiscal::FetchResponse()
{
	std::string data;
    AnsiString outData = "";
	if( _tcpClient->IOHandler->Readable( _READ_TIME_OUT ) )
	{
       _tcpClient->IOHandler->CheckForDataOnSource(500);
       _tcpClient->IOHandler->ReadTimeout = 100;
       TByteDynArray buffer;
       _tcpClient->IOHandler->ReadBytes( buffer, -1, true );
       outData = AnsiString((char*) & buffer[0], buffer.Length);
    }
//    outData = outData.SubString(2,outData.Length()-2);
    MessageBox(outData,"outData",MB_OK);
    return outData;
}
//---------------------------------------------------------------------------

