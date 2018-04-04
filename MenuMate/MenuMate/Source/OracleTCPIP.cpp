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
    std::auto_ptr<TStringList> List(new TStringList);
    AnsiString fileName = GetFileName();
	try
	{
        if (FileExists(fileName) )
            List->LoadFromFile(fileName);

       if( tcpClient->Connected() )
	   {
            Disconnect();
            if(TGlobalSettings::Instance().OracleInterfacePortNumber)
                Sleep(12000);
            CreateTCPClient();
            if(TGlobalSettings::Instance().OracleInterfacePortNumber)
                Sleep(1000);
       }
       if(!tcpClient->Connected())
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
        if(!IsSilentConnect)
           	MessageBox(E.Message+"\nPlease check IP address and Port number Values.\nOracle is disabled.",
                                                 "Abort", MB_OK + MB_ICONERROR);
        List->Add("Exception Occurred " + E.Message + "\n");
	}

    if(retValue)
    {
        List->Add("Connection Attempt was successful at  " + (AnsiString)Now().FormatString("hh:nn:ss am/pm") + "\n");
    }
    else
    {
        List->Add("Connection Attempt was unsuccessful at  " + (AnsiString)Now().FormatString("hh:nn:ss am/pm") + "\n");
    }
    MakeOracleLogFile(List,fileName);
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
    std::auto_ptr<TStringList> List(new TStringList);
    AnsiString fileName = "";
    WaitOrProceedWithPost();
	if (inData != "" && tcpClient->Connected() /*&& isLinkActive*/)
	{
        try
        {
            fileName = GetFileName();
            if (FileExists(fileName) )
              List->LoadFromFile(fileName);
            List->Add("Request Date- " + (AnsiString)Now().FormatString("DDMMMYYYY") + "\n");
            List->Add("Request Time- " + (AnsiString)Now().CurrentTime().FormatString("hh:nn:ss am/pm") + "\n");
            List->Add("Request Data:- " +inData + "\n");
            sendData(inData);
            outResponse = fetchResponse();
            UnsetPostingFlag();
            List->Add("Response Date- " + (AnsiString)Now().FormatString("DDMMMYYYY") + "\n");
            List->Add("Response Time- " + (AnsiString)Now().CurrentTime().FormatString("hh:nn:ss am/pm") + "\n");
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
    }
    MakeOracleLogFile(List,fileName);
    Disconnect();
    return outResponse;
}
//--------------------------------------------------------------------------
bool TOracleTCPIP::RetryMakingConnection()
{
    bool retValue = false;
    std::auto_ptr<TStringList> reConnectLogs(new TStringList);
    reConnectLogs->Add("Trying to reconnect at                    " + Now().CurrentTime().FormatString("hh:nn:ss am/pm"));
    LogWaitStatus(reConnectLogs);
    IsSilentConnect = false;
    retValue = Connect();
    return retValue;
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
        Sleep(Random(50));
        Sleep(Random(10));
        Sleep(Random(50));
        TGlobalSettings  &gs = TGlobalSettings::Instance();
        TManagerVariable &mv = TManagerVariable::Instance();

        tr.StartTransaction();
        // This is used to retain the state of the checkbox if the POS is exited
        #pragma warn -pia
        if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
        global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
        #pragma warn .pia
        TManagerVariable::Instance().GetProfileBool(tr, global_profile_key, vmIsOraclePostInProgress, isPosting);
        if(isPosting)
            waitLogs->Add("Entered queue at                          " + Now().FormatString("hh:nn:ss am/pm"));
        while(isPosting)
        {
            Sleep(1500);
            TManagerVariable::Instance().GetProfileBool(tr, global_profile_key, vmIsOraclePostInProgress, isPosting);
        }
        mv.SetProfileBool(tr,global_profile_key,
        vmIsOraclePostInProgress,true);
        tr.Commit();
        //SetPostingFlag();
        if(waitLogs->Count > 0)
        {
            waitLogs->Add("Wait Over at                              " + Now().FormatString("hh:nn:ss am/pm"));
            waitLogs->Add("=================================================================================");
            LogWaitStatus(waitLogs);
        }
        Connect();
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
AnsiString TOracleTCPIP::GetFileName()
{
    AnsiString directoryName = "";
    AnsiString fileName      = "";
    directoryName = ExtractFilePath(Application->ExeName) + "/Oracle Posts Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = "OraclePosts " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
    fileName = directoryName + "/" + name;
    return fileName;
}
//---------------------------------------------------------------------------
void TOracleTCPIP::LogWaitStatus(std::auto_ptr<TStringList> waitLogs)
{
    try
    {
        AnsiString fileName = GetFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        for(int index = 0; index < waitLogs->Count; index++)
        {
            AnsiString value = waitLogs->operator [](index);
            List->Add(value);
        }
        List->SaveToFile(fileName );
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
//    tcpClient->IOHandler->Write( CreateSTX(), 1 );
    __int32 bufferSize = ( remainedDataSize >= maxBufferSize ) ? maxBufferSize : remainedDataSize;
    data.copy( buffer, bufferSize, itPos );
    tcpClient->IOHandler->Write( CreateByteArray( buffer, bufferSize), bufferSize );
//    tcpClient->IOHandler->Write( CreateETX(), 1 );
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
    STX[0]     = 0x02;
    return STX;
}
//---------------------------------------------------------------------------
TBytes TOracleTCPIP::CreateETX()
{
    TBytes ETX;
    ETX.Length = 1;
    ETX[0]     = 0x03;
    return ETX;
}
//---------------------------------------------------------------------------
