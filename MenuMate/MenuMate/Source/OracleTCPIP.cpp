//---------------------------------------------------------------------------


#pragma hdrstop

#include "OracleTCPIP.h"
#include "MMLogging.h"
#include "DeviceRealterminal.h"
#include "OracledataBuilder.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
TOracleTCPIP::TOracleTCPIP()
{
    tcpClient = CreateTCPClient();
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
TIdTCPClient* TOracleTCPIP::CreateTCPClient()
{
    TIdTCPClient *client = new TIdTCPClient(NULL);
	client->Host           = TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
	client->Port           = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
	client->BoundPort      = 0;
	client->ReadTimeout    = _READ_TIME_OUT;
	client->ConnectTimeout = _CONNECT_TIME_OUT;
    return client;
}
//---------------------------------------------------------------------------
bool TOracleTCPIP::Connect()
{
    bool retValue = false;
	try
	{
	   tcpClient->Connect();
	   if( tcpClient->Connected() )
	   {
            retValue = true;
	   }
	}
	catch( Exception& E)
	{
       	MessageBox(E.Message, "Abort", MB_OK + MB_ICONERROR);
	}
    return retValue;
}
//---------------------------------------------------------------------------
void TOracleTCPIP::SendAndFetch(AnsiString data,AnsiString address,int portNumber)
{
	if (data != "")
	{
//		if (Data[1] == '\\')
//		{
//			Data = Data.SubString(2, Data.Length());
//			if (Data[1] == '8')
//			{
//				Data = Data.SubString(3, Data.Length());
//				AnsiString Length, RegNumber, RoomNo, TempBalance, TempCreditLimit, DataCount, NameTemp;
//				int Index = 1;
//				Index = AnsiStringRead(Data, Length, ',', Index);
//				Index = AnsiStringRead(Data, RegNumber, ',', Index);
//				Index = AnsiStringRead(Data, RoomNo, ',', Index);
//				Index = AnsiStringRead(Data, TempBalance, ',', Index);
//				Index = AnsiStringRead(Data, TempCreditLimit, ',', Index);
//				Index = AnsiStringRead(Data, DataCount, ',', Index);
//				int LoopCount = StrToIntDef(DataCount, 0);
//				if (LoopCount == 0)
//				{
//					Result = eFailed;
//					Index = AnsiStringRead(Data, NameTemp, ',', Index);
//					ResultText = NameTemp;
//				}
//				else
//				{
//					Result = eAccepted;
//					Balance = (StrToCurrDef(TempBalance, 0) / 100.00);
//					CreditLimit = (StrToCurrDef(TempCreditLimit, 999900) / 100.00);
//					Folders->Clear();
//					for (int i = 0; i < LoopCount; i++)
//					{
//						Index = AnsiStringRead(Data, NameTemp, ',', Index);
//						Folders->Add(NameTemp);
//					}
//				}
//			}
//			else
//			{
//				Result = eFailed;
//				ResultText = "Incorrect PMS Packet Received Expected Type 8 got " + Data;
//				TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, ResultText);
//				// TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet->ResultText);
//			}
//		}
//		else
//		{
//			Result = eFailed;
//			ResultText = "PMS Packet Missing \\";
//			TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, ResultText);
//			// TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet->ResultText);
//		}
	}
}
//---------------------------------------------------------------------------
AnsiString TOracleTCPIP::SerializeOut(TPostRoomInquiry roomInquiry)
{

}
//---------------------------------------------------------------------------
AnsiString TOracleTCPIP::SerializeOut(TPostRequest postRequest)
{
}
//---------------------------------------------------------------------------
TRoomInquiryItem TOracleTCPIP::SerializeIn(AnsiString data, TRoomInquiryAnswer roomAnswer)
{
}
//---------------------------------------------------------------------------
TPostRequestAnswer TOracleTCPIP::SerializeIn(AnsiString data)
{
}
//---------------------------------------------------------------------------
TLinkAlive TOracleTCPIP::SerializeIn(AnsiString data,int i)
{
}
//---------------------------------------------------------------------------
AnsiString TOracleTCPIP::SerializeOut(TLinkDescription linkDes)
{
    std::auto_ptr<TOracleDataBuilder>dataBuilder(new TOracleDataBuilder());
    return "";
}
//---------------------------------------------------------------------------
