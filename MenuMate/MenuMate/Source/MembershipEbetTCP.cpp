#include <vcl.h>
#pragma hdrstop

#include "MembershipEbetTCP.h"
#include <DateUtils.hpp>
#include <Types.hpp>
#include <MMLogging.h>

// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------

TMembershipEBetTCP::TMembershipEBetTCP() : fTCPClient(new TIdTCPClient(NULL))
{
   ConnectTimeOut = 5000;

   if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "OutBound\\Ebet"))
   {
	  ForceDirectories(ExtractFilePath(Application->ExeName) + "OutBound\\Ebet");
   }
}

void TMembershipEBetTCP::Init(AnsiString inIP, int inPort, int inReadTimeOut)
{
   IP = inIP;
   Port = inPort;
   ReadTimeOut = inReadTimeOut;
}

__fastcall TMembershipEBetTCP::~TMembershipEBetTCP()
{
}

bool TMembershipEBetTCP::RequestMemberInfoByCard(int inTilID, AnsiString inCardData)
{
   bool RetVal = false;
   OutPacket.Init(inTilID, ebetRequestPoints, 0, inCardData);
   SendAndFetch(IP, Port);
   if (InPacket.Status() == eEbetOk)
   {
	  RetVal = true;
   }
   return RetVal;
}

bool TMembershipEBetTCP::RequestMemberInfoByMemberNumber(int inTilID, AnsiString MemberNumber)
{
   bool RetVal = false;
   OutPacket.Init(inTilID, ebetRequestPointsMemNo, 0,"", MemberNumber);
   SendAndFetch(IP, Port);
   if (InPacket.Status() == eEbetOk)
   {
	  RetVal = true;
   }
   return RetVal;
}

bool TMembershipEBetTCP::RequestAddPoints(int inTilID, AnsiString MemberNumber, int Points)
{
   bool RetVal = false;
   OutPacket.Init(inTilID, ebetAddPointsMemNo, Points,"", MemberNumber);
   SendAndFetch(IP, Port);
   if (InPacket.Status() == eEbetOk)
   {
	  RetVal = true;
   }
   return RetVal;
}

bool TMembershipEBetTCP::RequestRedeemedPoints(int inTilID, AnsiString MemberNumber, int Points)
{
   bool RetVal = false;
   OutPacket.Init(inTilID, ebetRedeemPointsMemNo, Points,"", MemberNumber);
   SendAndFetch(IP, Port);
   if (InPacket.Status() == eEbetOk)
   {
	  RetVal = true;
   }
   return RetVal;
}



void TMembershipEBetTCP::SendAndFetch(AnsiString HostAddress, int Port)
{
   bool Retry = false;
   int RetryCount = 0;
   do
   {
	  try
	  {
		 if (HostAddress != "")
		 {
			AnsiString Data = OutPacket.BuildPacket();
			InPacket.Erase();
			if (!fTCPClient->Connected())
			{
			   fTCPClient->Host = HostAddress;
			   fTCPClient->Port = Port;
			   fTCPClient->BoundPort = 0;
			   fTCPClient->ReadTimeout = ReadTimeOut;
			   fTCPClient->ConnectTimeout = ConnectTimeOut;
			   fTCPClient->Connect();
			}

			if (fTCPClient->Connected())
			{
			   TManagerLogs::Instance().Add(__FUNC__, EBETINTERFACELOG, "Write Line :" + Data);
			   fTCPClient->IOHandler->WriteLn(Data);
			   Data = "";

			   // CheckForDataOnSource
			   do
			   {
				  Data += fTCPClient->IOHandler->ReadChar();
			   }
			   while (!fTCPClient->IOHandler->InputBufferIsEmpty());

			   if (Data == "")
			   {
				  InPacket.setStatus(eEbetAbort);
				  InPacket.setMsg("EBet Server Failed To Respond.");
				  TManagerLogs::Instance().Add(__FUNC__, EBETINTERFACELOG, InPacket.Msg());
			   }
			   else
			   {
				  TManagerLogs::Instance().Add(__FUNC__, EBETINTERFACELOG, "Read :" + Data);
				  InPacket.Process(Data);
				  if (InPacket.Status() == eEbetResend)
				  {
					 Retry = true;
					 RetryCount++;
				  }
				  else if (InPacket.Status() == eEbetAbort)
				  {
					 Retry = false;
				  }
				  else
				  {
					 OutPacket.Init(OutPacket.TillID, ebetSendAck, 0, "");
					 AnsiString Data = OutPacket.BuildPacket();
					 fTCPClient->IOHandler->WriteLn(Data);
				  }
			   }

			   if (fTCPClient->Connected())
			   {
				  fTCPClient->Disconnect();
			   }
			}
			else
			{
			   InPacket.setStatus(eEbetAbort);
			   InPacket.setMsg("Failed To Connect to EBet Server.");
			   TManagerLogs::Instance().Add(__FUNC__, EBETINTERFACELOG, InPacket.Msg());
			   Retry = true;
			   RetryCount++;
			}
		 }
		 else
		 {
			InPacket.setStatus(eEbetAbort);
			InPacket.setMsg("Invalid IntaMate Host Address :" + HostAddress);
			TManagerLogs::Instance().Add(__FUNC__, EBETINTERFACELOG, InPacket.Msg());
			Retry = false;
		 }
	  }
	  catch(Exception & E)
	  {
		 RetryCount++;
		 Retry = true;
		 InPacket.setStatus(eEbetAbort);
		 InPacket.setMsg(E.Message);
		 TManagerLogs::Instance().Add(__FUNC__, EBETINTERFACELOG, InPacket.Msg());
		 if (fTCPClient->IOHandler != NULL && fTCPClient->IOHandler->ClosedGracefully)
		 {
			fTCPClient->Disconnect();
		 }

         if(E.Message.LowerCase() == "connect timed out.")
         {
	         RetryCount = MaxRetryCount;
         }
		 ::Sleep(SleepOnException);
	  }
   }
   while (Retry == true && RetryCount < MaxRetryCount);
}

int TMembershipEBetTCP::GeneratePacketNumber()
{
   if (PacketNo > 00)
	  PacketNo = 0;
   return PacketNo++;
}
