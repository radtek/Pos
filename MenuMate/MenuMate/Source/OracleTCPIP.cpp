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
    try
    {
        if(tcpClient)
        {
            delete tcpClient;
            tcpClient = NULL;
        }
    }
    catch(Exception &Exc)
    {
    }
}
//---------------------------------------------------------------------------
void TOracleTCPIP::CreateTCPClient()
{
	tcpClient->Host           = TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
	tcpClient->Port           = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
	tcpClient->BoundPort      = 0;
	tcpClient->ReadTimeout    = _READ_TIME_OUT;
	tcpClient->ConnectTimeout = _CONNECT_TIME_OUT;
}
//---------------------------------------------------------------------------
bool TOracleTCPIP::Connect()
{
    bool retValue = false;
	try
	{
       if( tcpClient->Connected() )
	   {
            Disconnect();
            Sleep(12000);
            CreateTCPClient();
            Sleep(1000);
       }
       if(! tcpClient->Connected())
       {
           tcpClient->Connect();
           retValue = true;
       }
	}
	catch( Exception& E)
	{
        Disconnect();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
       	MessageBox(E.Message+"\nPlease check IP address and Port number Values.\nOracle is disabled.",
                                                 "Abort", MB_OK + MB_ICONERROR);
	}
    return retValue;
}
//----------------------------------------------------------------------------
bool TOracleTCPIP::Disconnect()
{
    if(tcpClient->Connected())
    {
        tcpClient->Disconnect();
    }
}
//---------------------------------------------------------------------------
AnsiString TOracleTCPIP::SendAndFetch(AnsiString inData)
{
    AnsiString outResponse = "";
    AnsiString directoryName = "";
    std::auto_ptr<TStringList> List(new TStringList);
    AnsiString fileName = "";
    //bool isLinkActive = true;
//    if(!tcpClient->Connected())
//    {
//        bool isLinkActive = false;
//        std::auto_ptr<TOracleDataBuilder> oracledata(new TOracleDataBuilder());
//        TLinkDescription linkDescription = oracledata->CreateLinkDescription();
//        TiXmlDocument doc = oracledata->CreateLinkDescriptionXML(linkDescription);
//        AnsiString data = oracledata->SerializeOut(doc);
//        AnsiString resultData = "";
//        resultData = SendAndFetch(data);
//        // deserialize the resposne
//        isLinkActive = oracledata->DeserializeGetLinkStatus(resultData);
//    }
	if (inData != "" && tcpClient->Connected() /*&& isLinkActive*/)
	{
        try
        {
            directoryName = ExtractFilePath(Application->ExeName) + "/Oracle Posts Logs";
            if (!DirectoryExists(directoryName))
                CreateDir(directoryName);
            AnsiString name = "OraclePosts " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
            fileName = directoryName + "/" + name;
            if (FileExists(fileName) )
              List->LoadFromFile(fileName);
            List->Add("Request Date- " + (AnsiString)Now().FormatString("DDMMMYYYY") + "\n");
            List->Add("Request Time- " + (AnsiString)Now().FormatString("hhnnss") + "\n");
            List->Add("Request Data:- " +inData + "\n");
            sendData(inData);
            outResponse = fetchResponse();
            List->Add("Response Date- " + (AnsiString)Now().FormatString("DDMMMYYYY") + "\n");
            List->Add("Response Time- " + (AnsiString)Now().FormatString("hhnnss") + "\n");
            List->Add("Response Data:- " +outResponse + "\n");
        }
        catch( Exception& exc)
        {
            TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,exc.Message);
            List->Add("Exception Message- " + exc.Message + "\n");
            MessageBox("Could not communicate with Oracle Server.\n" +exc.Message,"Error",MB_OK + MB_ICONERROR);
        }
	}
    else if (!tcpClient->Connected())
    {
        MessageBox("Connection Not Open for Oracle PMS", "Error",MB_OK + MB_ICONERROR);
    }
    MakeOracleLogFile(List,fileName);
    return outResponse;
}
//--------------------------------------------------------------------------
void TOracleTCPIP::MakeOracleLogFile(std::auto_ptr<TStringList> List,AnsiString infileName)
{
    try
    {
        List->Add("===========================================================================================================");
        List->Add("\n");
        List->SaveToFile(infileName);
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
void TOracleTCPIP::sendData( AnsiString inData )
{
    std::string data = inData.c_str();
	__int32 dataSize         = data.size();
 	__int32 remainedDataSize = dataSize;

     __int32 maxBufferSize = data.size(); // It can be assigned a lower value
	__int32 itPos = 0;

	char   *buffer = new char[maxBufferSize];

	// Send Data
    tcpClient->IOHandler->Write( CreateSTX(), 1 );
    __int32 bufferSize = ( remainedDataSize >= maxBufferSize ) ? maxBufferSize : remainedDataSize;
    data.copy( buffer, bufferSize, itPos );
    tcpClient->IOHandler->Write( CreateByteArray( buffer, bufferSize), bufferSize );
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
