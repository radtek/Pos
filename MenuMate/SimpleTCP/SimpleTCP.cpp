//---------------------------------------------------------------------------

#pragma hdrstop

#include "SimpleTCP.h"
#include "Stream.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

NetMessageMakerMap TNetMessageMaker::RegisteredMakers;

TNetMessage::TNetMessage()
{
	MessageType = udp_Unknown;
	VersionNumber = NetPacketVersion;
   MMSubNet = 0;
#ifdef MenuMate
	ProductID = udpMenuMate;
#elif defined (PalmMate)
	ProductID = udpPalmMate;
#elif defined (ChefMate)
	ProductID = udpChefMate;
#else
	ProductID = udpProdUnknown;
#endif

}

TNetMessage::TNetMessage(udpPackets type)
  : MessageType(static_cast<int>(type))
{
	VersionNumber = NetPacketVersion;
   MMSubNet = 0;
#ifdef MenuMate
	ProductID = udpMenuMate;
#elif defined (PalmMate)
	ProductID = udpPalmMate;
#elif defined (ChefMate)
	ProductID = udpChefMate;
#else
	ProductID = udpProdUnknown;
#endif
}

TNetMessage::~TNetMessage()
{
	clear();
}

TNetMessage *TNetMessageMaker::ConstructMessage(TMemoryStream *Data)
{
    // cast the raw memory to a generic message to determine its type
    if(Data->Size > 0)
    {
        int MessageType = udp_Unknown;
        int VersionNumber = 0;
        Data->Position = 0;
        Data->Read(&MessageType, sizeof(MessageType));
        Data->Read(&VersionNumber, sizeof(VersionNumber));
        Data->Position = 0;
        if(MessageType != udp_Unknown && VersionNumber == NetPacketVersion)
        {
            try
            {
                // find the appropriate factory in the map of factories...
                NetMessageMakerMap::iterator MessageMaker = RegisteredMakers.find(MessageType);
                if(MessageMaker != RegisteredMakers.end())
                {
                    TNetMessageMaker *Maker = MessageMaker->second;
                    // use that factory to construct the net_message derivative
                    return Maker->MakeMessage(Data);
                }
            }
            catch(...)
            {
            /*    err_printf("net_message_maker::constructmessage: logic error, I don't know
                     how to (or can't) construct message ID %d!", lpMsg->dwType);*/
            }
        }
    }
    return(NULL);
}

void TNetMessage::EncodeNetMessage(TMemoryStream *Stream)
{
   Stream->Position = 0;
	StreamWrite(Stream,MessageType);
	StreamWrite(Stream,VersionNumber);
  	StreamWrite(Stream,MMSubNet);
	int PID = ProductID;
	StreamWrite(Stream,PID);
}

//---------------------------------------------------------------------------

void TNetMessage::DecodeNetMessage(TMemoryStream *Stream) // Reads it'self from Stream Supplied.
{
	Stream->Position = 0;
	StreamRead(Stream,MessageType);
	StreamRead(Stream,VersionNumber);
  	StreamRead(Stream,MMSubNet);
	int PID = 0;
	StreamRead(Stream,PID);
	ProductID = static_cast<udpProduct>(PID);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



