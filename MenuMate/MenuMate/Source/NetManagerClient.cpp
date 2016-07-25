//---------------------------------------------------------------------------


#pragma hdrstop

#include "NetManagerClient.h"
#include "enum.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerNetClient::TManagerNetClient()
{
	// MenuMate Sends to Port 65001 Listens on 65001
	// PalmMate Sends to Port 65001 Listens on 65002
	// ChefMate Sends to Port 65001 Listens on 65004

	udp = new TIdUDPClient(NULL);
	udp->Port = MENUMATEPORT;	// Port on Remote system to attach to.
	udp->BroadcastEnabled = true;
	udp->Active = true;

	IPWatch = new TIdIPWatch(NULL);
	IPWatch->WatchInterval = 10000;
	IPWatch->HistoryEnabled = false;	
	IPWatch->Active = true;
	IPWatch->ForceCheck();

	ProductID = udpProdUnknown;
}

TManagerNetClient::~TManagerNetClient()
{
	delete udp;
	delete IPWatch;
}

AnsiString TManagerNetClient::GetBroadCastIP()
{
	AnsiString RetVal = "";
	AnsiString CurrentIP = IPWatch->CurrentIP;
	if(IPWatch->IsOnline)
	{
		int Temp = CurrentIP.LastDelimiter(".");
		RetVal 	= CurrentIP.SubString(1,Temp) + "255";
	}
	return RetVal;
}

void TManagerNetClient::SendToAll(TNetMessage *Request)
{
   if((Request != NULL) &&
		(GetBroadCastIP() != "") &&
		(GetBroadCastIP() != "127.0.0.255") )
   {
		if(Request->MessageType != udp_InstUnknown)
      {
         TMemoryStream *SentStream = new TMemoryStream;
         try
         {
				Request->Encode(SentStream);
				SentStream->Position = 0;
				udp->Host = GetBroadCastIP();
				udp->Port = MENUMATEPORT;
				udp->SendBuffer(RawToBytes(SentStream->Memory,SentStream->Size));
				SentStream->Position = 0;
				udp->Port = PALMMATEPORT;
				udp->SendBuffer(RawToBytes(SentStream->Memory,SentStream->Size));
				SentStream->Position = 0;
				udp->Port = LOGSDATAPORT;
				udp->SendBuffer(RawToBytes(SentStream->Memory,SentStream->Size));
			}
         __finally
         {
            delete SentStream;
         }
		}
	}
}

void TManagerNetClient::SendTo(TNetMessage *Request,AnsiString IP)
{
	if(Request != NULL)
   {
      if(Request->MessageType != udp_InstUnknown)
		{
         TMemoryStream *SentStream = new TMemoryStream;
         try
         {
				Request->Encode(SentStream);
            SentStream->Position = 0;
            udp->Host = IP;
				udp->Port = MENUMATEPORT;
				udp->SendBuffer(RawToBytes(SentStream->Memory,SentStream->Size));
				// Palm Mate
            SentStream->Position = 0;
				udp->Port = PALMMATEPORT;
				udp->SendBuffer(RawToBytes(SentStream->Memory,SentStream->Size));
				// Logs Mate
				SentStream->Position = 0;
				udp->Port = LOGSDATAPORT;
				udp->SendBuffer(RawToBytes(SentStream->Memory,SentStream->Size));
			}
         __finally
         {
            delete SentStream;
         }
		}
	}
}

void TManagerNetClient::SendTo(TNetMessage *Request,AnsiString IP, int PORT)
{
	if(Request != NULL)
   {
      if(Request->MessageType != udp_InstUnknown)
		{
         TMemoryStream *SentStream = new TMemoryStream;
         try
         {
				Request->Encode(SentStream);
            SentStream->Position = 0;
            udp->Host = IP;
				udp->Port = PORT;
				udp->SendBuffer(RawToBytes(SentStream->Memory,SentStream->Size));
         }
         __finally
         {
            delete SentStream;
         }
		}
	}
}
