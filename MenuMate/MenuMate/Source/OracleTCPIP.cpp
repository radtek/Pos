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
        UnsetPostingFlag();
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
    WaitOrProceedWithPost();
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
            UnsetPostingFlag();
            List->Add("Response Date- " + (AnsiString)Now().FormatString("DDMMMYYYY") + "\n");
            List->Add("Response Time- " + (AnsiString)Now().FormatString("hhnnss") + "\n");
            List->Add("Response Data:- " +outResponse + "\n");
        }
        catch( Exception& exc)
        {
            TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,exc.Message);
            List->Add("Exception Message- " + exc.Message + "\n");
            MessageBox("Could not communicate with Oracle Server.\n" +exc.Message,"Error",MB_OK + MB_ICONERROR);
            UnsetPostingFlag();
        }
	}
    else if (!tcpClient->Connected())
    {
          UnsetPostingFlag();
          if(RetryMakingConnection());
              outResponse = SendAndFetch(inData);
    }
    MakeOracleLogFile(List,fileName);
    return outResponse;
}
bool TOracleTCPIP::RetryMakingConnection()
{
    return Connect();
}
//--------------------------------------------------------------------------
void TOracleTCPIP::WaitOrProceedWithPost()
{
    bool isPosting = false;
    int global_profile_key;
    Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    std::auto_ptr<TStringList> waitLogs(new TStringList);
    try
    {
        Sleep(Random(3));
        Sleep(Random(4));
        Sleep(Random(15));
        TGlobalSettings  &gs = TGlobalSettings::Instance();
        TManagerVariable &mv = TManagerVariable::Instance();

        tr.StartTransaction();
        // This is used to retain the state of the checkbox if the POS is exited
        #pragma warn -pia
        if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
        global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
        #pragma warn .pia
        TManagerVariable::Instance().GetProfileBool(tr, global_profile_key, vmIsOraclePostInProgress, isPosting);
        while(isPosting)
        {
            Sleep(100);
            TManagerVariable::Instance().GetProfileBool(tr, global_profile_key, vmIsOraclePostInProgress, isPosting);
        }
        mv.SetProfileBool(tr,global_profile_key,
        vmIsOraclePostInProgress,true);
        tr.Commit();
        //SetPostingFlag();
    }
    catch(Exception &Exc)
    {
        tr.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
void TOracleTCPIP::SetPostingFlag()
{
    Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    tr.StartTransaction();
    try
    {
        TGlobalSettings  &gs = TGlobalSettings::Instance();
        TManagerVariable &mv = TManagerVariable::Instance();

        int global_profile_key;

        // This is used to retain the state of the checkbox if the POS is exited
    #pragma warn -pia
        if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
        global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
    #pragma warn .pia

        mv.SetProfileBool(tr,global_profile_key,
        vmIsOraclePostInProgress,true);

        tr.Commit();
    }
    catch(Exception &Exc)
    {
        tr.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
void TOracleTCPIP::UnsetPostingFlag()
{
    Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    tr.StartTransaction();
    try
    {
        TGlobalSettings  &gs = TGlobalSettings::Instance();
        TManagerVariable &mv = TManagerVariable::Instance();

        int global_profile_key;

        // This is used to retain the state of the checkbox if the POS is exited
    #pragma warn -pia
        if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
        global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
    #pragma warn .pia

        mv.SetProfileBool(tr,global_profile_key,
        vmIsOraclePostInProgress,false);

        tr.Commit();
    }
    catch(Exception &Exc)
    {
        tr.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
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
