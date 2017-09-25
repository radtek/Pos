//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MembershipExternalTCP.h"
#include "MMLogging.h"

#include <DateUtils.hpp>
#include <system.hpp>
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
   do
	{
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

//                MessageBox("Checking connection","",MB_OK);
				if(!fTCPClient->Connected())
				{
//                    MessageBox("Not Connected","",MB_OK);
					fTCPClient->Host = HostAddress;
					fTCPClient->Port = Port;
					fTCPClient->BoundPort = 0;
					fTCPClient->ReadTimeout = ReadTimeOut;
					fTCPClient->ConnectTimeout = ConnectTimeOut;
 //                   MessageBox("Going to connect","",MB_OK);
					fTCPClient->Connect();
 //                   MessageBox("Connect triggered","",MB_OK);
				}

				if(fTCPClient->Connected())
				{
					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Write :" + Data);
					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Write Length :" + IntToStr(Data.Length()));
					fTCPClient->IOHandler->WriteLn(Data);
					Data = "";
					//Data =  fTCPClient->IOHandler->ReadLnWait();

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

					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Read : " + Data);
					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,"Read Length :" + IntToStr(Data.Length()));

					if(Data == "")
					{
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
				}
				else
				{
					Packet.Result = eMSFailed;
					Packet.ResultText = "Failed To Connect to Server.";
					TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,Packet.ResultText);
				}
			}
			else
			{
				Packet.Result = eMSFailed;
				Packet.ResultText = "Invalid Server Host Address :" + HostAddress;
				TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,Packet.ResultText);
			}
		}
		catch(Exception &E)
		{
			Retry = true;
			RetryCount++;
			Packet.Result = eMSFailed;
			Packet.ResultText = E.Message;
			TManagerLogs::Instance().Add(__FUNC__,MEMBERSHIPINTERFACELOG,Packet.ResultText);
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet.ResultText);
			::Sleep(ExceptSleepTime);
		}
	}
	while(Retry == true && RetryCount < defaultRetryCount);
}
//---------------------------------------------------------------------------

