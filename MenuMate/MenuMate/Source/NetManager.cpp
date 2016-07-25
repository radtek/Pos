//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerNet.h"
#include "enum.h"
#include "ManagerVariable.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerNet::TManagerNet()
{
	// MenuMate Sends to Port 65001 Listens on 65001
	// PalmMate Sends to Port 65001 Listens on 65002
	// ChefMate Sends to Port 65001 Listens on 65004
   Enabled = false;
   MMSubNet = 0;

	udp = new TIdUDPClient(NULL);
	udp->Port = MENUMATEPORT;	// Port on Remote system to attach to.
	udp->BroadcastEnabled = true;
	udp->Active = true;

	udpServer = new TIdUDPServer(NULL);
	udpServer->OnUDPRead = udpServerUDPRead;
#ifdef MenuMate
	udpServer->DefaultPort = MENUMATEPORT;
#elif defined (PalmMate)
	udpServer->DefaultPort = PALMMATEPORT;
#elif defined (ChefMate)
	udpServer->DefaultPort = CHEFMATEPORT;
#else
	udpServer->DefaultPort = LOGSDATAPORT;
#endif

#ifdef MenuMate
	ProductID = udpMenuMate;
#elif defined (PalmMate)
	ProductID = udpPalmMate;
#elif defined (ChefMate)
	ProductID = udpChefMate;
#else
	ProductID = udpProdUnknown;
#endif

   if(ProductID != udpProdUnknown)
   {
	   udpServer->Active = true;
   }

	IPWatch = new TIdIPWatch(NULL);
	IPWatch->WatchInterval = 10000;
	IPWatch->HistoryEnabled = false;
	IPWatch->Active = true;
	IPWatch->ForceCheck();

}

TManagerNet::~TManagerNet()
{
	delete udp;
	delete udpServer;
	delete IPWatch;
}

UnicodeString TManagerNet::GetBroadCastIP()
{
	UnicodeString RetVal = "";
	UnicodeString CurrentIP = IPWatch->CurrentIP;
	if(IPWatch->IsOnline)
	{
		int Temp = CurrentIP.LastDelimiter(".");
		RetVal 	= CurrentIP.SubString(1,Temp) + "255";
	}
	return RetVal;
}

void TManagerNet::SendToAll(TNetMessage *Request)
{
   if(!Enabled) return;
   
   if((Request != NULL) &&
		(GetBroadCastIP() != "") &&
		(GetBroadCastIP() != "127.0.0.255") )
   {
      // Set the NetGroup for this MenuMate Group.
      Request->MMSubNet = MMSubNet;
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

void TManagerNet::SendTo(TNetMessage *Request,UnicodeString IP)
{
   if(!Enabled) return;

	if(Request != NULL)
   {
      // Set the NetGroup for this MenuMate Group.
      Request->MMSubNet = MMSubNet;

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

void __fastcall TManagerNet::udpServerUDPRead(TIdUDPListenerThread *AThread, TBytes AData, TIdSocketHandle *ABinding)
{
	if(!Enabled) return;
   
   try
   {
      bool SamePc = false;
      if(ABinding->PeerIP == IPWatch->CurrentIP)
	  {
	      SamePc = true;
      }

		if(AData.Length > 0)
		{
			std::auto_ptr<TMemoryStream> InboundStream(new TMemoryStream);
			WriteTIdBytesToStream(InboundStream.get(),AData);
			InboundStream->Position = 0;

			TNetMessage *Data = TNetMessageMaker::ConstructMessage(InboundStream.get());
			try
			{
				if(Data != NULL)
				{
					if( !SamePc || (SamePc && Data->ProductID != ProductID) )
					{
                  if(Data->MMSubNet == MMSubNet)
                  {
						   Data->Action();
                  }
					}
				}
			}
			__finally
			{
				delete Data;
			}
		}
	}
   catch(Exception &Err)
   {
//		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message + "IP : " + ABinding->PeerIP + " Byte Count " + IntToStr(AData->Size) + " Port " + IntToStr(ABinding->PeerPort));
	}
}

void TManagerNet::Initialise(Database::TDBTransaction &DBTransaction)
{
   MMSubNet = TManagerVariable::Instance().GetInt(DBTransaction,vmMMSubNet,0);
   Enabled = true;
}
