//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerNet.h"
#include "enum.h"


#include <spinlock.hh>

using utilities::locking::spinlock;
using utilities::locking::spinlock_reference;

#pragma link "iphlpapi.lib"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdIPWatch"

#include <iphlpapi.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <utility>
#include <vector>
#include <sstream>

TManagerNet::TManagerNet()
  : ip_watcher_(new TIdIPWatch(0x0)),
    ip_guard_(new spinlock()),
    ip_address_(""),
    broadcast_address_("")
{
   ip_watcher_->HistoryEnabled = false;
   ip_watcher_->Active = false;

   CacheMachineIPAndBroadcastIPAddresses();

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
	   try
   	{
	   	udpServer->Active = true;
      }
		catch(Exception &e)
	   {
   	}
   }
}

TManagerNet::~TManagerNet()
{
	delete udp;
	delete udpServer;
}

const UnicodeString &
TManagerNet::GetBroadCastIP()
const
{
   spinlock_reference hold_lock(*ip_guard_);

   ip_watcher_->ForceCheck();
   if (ip_watcher_->CurrentIP != ip_address_)
      const_cast<TManagerNet &>(*this).CacheMachineIPAndBroadcastIPAddresses();

   return broadcast_address_;
}

const UnicodeString &
TManagerNet::GetIPAddress()
const
{
   spinlock_reference hold_lock(*ip_guard_);

   ip_watcher_->ForceCheck();
   if (ip_watcher_->CurrentIP != ip_address_)
      const_cast<TManagerNet &>(*this).CacheMachineIPAndBroadcastIPAddresses();

   return ip_address_;
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
      if(ABinding->PeerIP == GetIPAddress())
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

typedef std::vector<IP_ADAPTER_INFO> IPAdapterInfoVector;
typedef std::pair<std::string, std::string> AddressMaskPair;

IPAdapterInfoVector
GetEthernetAdapterInformation()
{
   IPAdapterInfoVector all_adapters;
   IPAdapterInfoVector ethernet_adapters;
   ULONG n_bytes_needed;

   /* Ask the API how many bytes we need for our buffer. */
   GetAdaptersInfo(NULL, &n_bytes_needed);

   /* Grow our vector to hold that many entries. */
   all_adapters.resize(n_bytes_needed / sizeof(IP_ADAPTER_INFO));

   /* Fill the buffer */
   GetAdaptersInfo(&all_adapters[0], &n_bytes_needed);

   /* Gather the Ethernet adapters. */
   for (IPAdapterInfoVector::const_iterator i = all_adapters.begin();
        i != all_adapters.end(); ++i)
      if (i->Type == MIB_IF_TYPE_ETHERNET)
         ethernet_adapters.push_back(*i);

   return ethernet_adapters;
}

AddressMaskPair
GetAdapterFirstAddressMaskPair(const IP_ADAPTER_INFO &adapter_info)
{
   return AddressMaskPair(adapter_info.IpAddressList.IpAddress.String,
                          adapter_info.IpAddressList.IpMask.String);
}

ULONG
StringToIPv4Address(const std::string &address_string)
{
        const char *m = address_string.c_str();
        char *n;
        ULONG encoded_address = 0;

        for ( ; *m; m = &n[*n == '.'])
                encoded_address =
                    encoded_address << 8 | strtoul(m, &n, 10);

        return encoded_address;
}

std::string
GenerateBroadcastIP(const AddressMaskPair &address_mask_pair)
{
        std::ostringstream ip_gen;

        ULONG address = StringToIPv4Address(address_mask_pair.first);
        ULONG subnet = StringToIPv4Address(address_mask_pair.second);
        ULONG broadcast_address = address & subnet | ~subnet;

        ip_gen << (broadcast_address >> 24)
               << "."
               << ((broadcast_address >> 16) & 0xFF)
               << "."
               << ((broadcast_address >> 8) & 0xFF)
               << "."
               << (broadcast_address & 0xFF);

        return ip_gen.str();
}

void
TManagerNet::CacheMachineIPAndBroadcastIPAddresses()
{
	IPAdapterInfoVector ethernetAdapters =  GetEthernetAdapterInformation();

    if(ethernetAdapters.size() == 0)
    {
        ip_address_ = "";
        broadcast_address_ = "";
    }
    else
    {
        AddressMaskPair address_mask_pair =
            GetAdapterFirstAddressMaskPair(ethernetAdapters.front());

        ip_address_ = address_mask_pair.first.c_str();
        broadcast_address_ = GenerateBroadcastIP(address_mask_pair).c_str();
    }
}

