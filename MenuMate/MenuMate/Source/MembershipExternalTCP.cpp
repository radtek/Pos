//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MembershipExternalTCP.h"
#include "MMLogging.h"

#include <DateUtils.hpp>
#include <system.hpp>
#include "SaveLogs.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TMembershipGeneralLedgerTCP::TMembershipGeneralLedgerTCP()
{
   ConnectTimeOut    = defaultConnectTimeOut;
   ReadTimeOut       = defaultReadTimeOut;
   RetryCount        = defaultRetryCount;
	ExceptSleepTime  = defaultSleepOnException;
   OffLine           = true;
}

//---------------------------------------------------------------------------


void TMembershipGeneralLedgerTCP::TimeOuts(int inConnectTimeOut,int inReadTimeOut,int inRetryCount, int inSleepOnException)
{
   ConnectTimeOut = inConnectTimeOut;
   ReadTimeOut    = inReadTimeOut;
   RetryCount     = inRetryCount;
	ExceptSleepTime = inSleepOnException;
}
//---------------------------------------------------------------------------

void TMembershipGeneralLedgerTCP::GetStatus(AnsiString IP, int Port, int ReadTimeOut)
{
   if(IP != "" && Port != 0)
   {
	   std::auto_ptr<TMSXMLStatus> XML(new TMSXMLStatus());

	   XML->Build();
	   ConnectionStatus = "";

      TimeOuts(500,ReadTimeOut,1,0);
	   Process(IP,Port,*XML.get());
	   XML->Parse();
	   if(XML->Result == eMSAccepted)
      {
         OffLine = false;
      }
      else
      {
         OffLine = true;
		  TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG, "Interface Offline Status Nak" + XML->ResultText);
      }
	   ConnectionStatus = XML->ResultText;
		TimeOuts(defaultConnectTimeOut,ReadTimeOut,defaultRetryCount,defaultSleepOnException);
   }
   else
   {
      OffLine = true;
      ConnectionStatus = "Not Configured";
	   TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG, "Interface Offline " + ConnectionStatus);
   }
}
//---------------------------------------------------------------------------

void TMembershipGeneralLedgerTCP::Process(AnsiString inIP, int inPort,TMSXMLBase &Packet)
{
   try
   {
		SendAndFetch(Packet, inIP, inPort);
		if(Packet.Result == eMSAccepted)
			Packet.Parse();
	}
   catch(Exception &E)
   {
	   Packet.Result = eMSFailed;
	   Packet.ResultText = E.Message;
	   TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Serious SendAndFetch Error");
	   TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,Packet.ResultText);
      ::Sleep(ExceptSleepTime);
   }
}

//---------------------------------------------------------------------------

void TMembershipGeneralLedgerTCP::SendAndFetch(TMSXMLBase &Packet, AnsiString HostAddress, int Port)
{
	bool Retry = false;
	int RetryCount = 0;
    AnsiString count = RetryCount;
    std::auto_ptr<TStringList> logsList(new TStringList());
    logsList->Add("Going for Sending request at                                   " + Now().FormatString("hh:nn:ss am/pm"));
   do
	{
        logsList->Add("Retry count is                                                 " + count);
                Packet.SetRepeatTranaction(RetryCount != 0);
				Packet.Build();
        AnsiString DataDummy = Packet.SerializeOut();
        //MessageBox(DataDummy,"Dummy",MB_OK);
        logsList->Add("Data was                                                       " + DataDummy);
		std::auto_ptr<TIdTCPClient> fTCPClient(new TIdTCPClient(NULL));
		Retry = false;
		try
		{
			if(HostAddress != "")
			{
				Packet.Result = eMSFailed;
				Packet.SetRepeatTranaction(RetryCount != 0);
				Packet.Build();
				AnsiString Data = Packet.SerializeOut();
				AnsiString Length = "0000000000";
				int Size = 0;
				Size = Data.Length() -1;
				Length.sprintf("%10.10d",Size);
				Data = Length + Data;

				if(!fTCPClient->Connected())
				{
                    logsList->Add("TCP client is not connected");
					fTCPClient->Host = HostAddress;
					fTCPClient->Port = Port;
					fTCPClient->BoundPort = 0;
					fTCPClient->ReadTimeout = ReadTimeOut;
					fTCPClient->ConnectTimeout = ConnectTimeOut;
					fTCPClient->Connect();
				}

				if(fTCPClient->Connected())
				{
                    logsList->Add("TCP client is connected.");
					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Write :" + Data);
					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Write Length :" + IntToStr(Data.Length()));
					fTCPClient->IOHandler->WriteLn(Data);
                    logsList->Add("Data written at                                                " + Now().FormatString("hh:nn:ss am/pm"));
                    logsList->Add("Data was                                                       " + Data);
					Data = "";

                   TByteDynArray Incomming;
                   fTCPClient->IOHandler->ReadBytes(Incomming, 10, false);
                   Data = AnsiString((char *)&Incomming[0],10);
					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Read Size: " + Data);
                   int DataLength = StrToIntDef(Data,0);
                   Data = "";
                   if(DataLength > 0)
                   {
                       Data = fTCPClient->IOHandler->ReadString(DataLength);
                   }
                   else
                   {
                        TByteDynArray Incomming;
                        fTCPClient->IOHandler->ReadBytes(Incomming, -1, false);
                        Data = AnsiString((char *)&Incomming[0],Incomming.Length);
                   }
                    logsList->Add("Data received at                                               " + Now().FormatString("hh:nn:ss am/pm"));
                    logsList->Add("Data was                                                       " + Data);
					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Read : " + Data);
					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Read Length :" + IntToStr(Data.Length()));

					if(Data == "")
					{
                        logsList->Add("Result status is failed because no data was received");
						Packet.Result = eMSFailed;
						Packet.ResultText = "IntaMate Server Failed To Respond.";
						TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,Packet.ResultText);
					}
					else
					{
						TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Read :" + Data);
						Packet.Result = eMSAccepted;
						Packet.SerializeIn(Data);
					}
//                    makeLogFile(Data2);
				}
				else
				{
                    logsList->Add("Result status is failed because no connection was made");
					Packet.Result = eMSFailed;
					Packet.ResultText = "Failed To Connect to Server.";
					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,Packet.ResultText);
				}
			}
			else
			{
				Packet.Result = eMSFailed;
				Packet.ResultText = "Invalid Server Host Address :" + HostAddress;
                logsList->Add("Invalid Server Host Address :" + HostAddress);
				TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,Packet.ResultText);
			}
		}
		catch(Exception &E)
		{
            logsList->Add("Exception occured at                                           " + Now().FormatString("hh:nn:ss am/pm"));
            logsList->Add("Exception was                                                  " + E.Message);
			Retry = true;
			RetryCount++;
			Packet.Result = eMSFailed;
			Packet.ResultText = E.Message;
			TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,Packet.ResultText);
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet.ResultText);
			::Sleep(ExceptSleepTime);
		}
        RecordLogs(logsList);
        logsList->Clear();
	}
	while(Retry == true && RetryCount < defaultRetryCount);
}
//---------------------------------------------------------------------------
//void TMembershipGeneralLedgerTCP::makeLogFile(AnsiString str)
//{
//    try
//    {
//        AnsiString fileName = ExtractFilePath(Application->ExeName) + "CasinoLogs.txt" ;
//
//        std::auto_ptr<TStringList> List(new TStringList);
//        if (FileExists(fileName) )
//        {
//          List->LoadFromFile(fileName);
//        }
//
//
//        List->Add("Response:- "+ str +  "\n");
//
//
//        List->SaveToFile(fileName );
//    }
//    catch(Exception &Ex)
//    {
//    }
//}
//---------------------------------------------------------------------------
void TMembershipGeneralLedgerTCP::RecordLogs(std::auto_ptr<TStringList> logsList)
{
    TSaveLogs::RecordCasinoLogs(logsList.get());
}
//---------------------------------------------------------------------------

