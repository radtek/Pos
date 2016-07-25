//---------------------------------------------------------------------------
#pragma hdrstop

#include "NetMessageMsg.h"
#include "Stream.h"

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)

TNetMessageMsg::TNetMessageMsg()
{
	MessageType = udp_Message;
}

void TNetMessageMsg::Encode(TMemoryStream *Stream)
{
	EncodeNetMessage(Stream);
	StreamWrite(Stream,Message);
	StreamWrite(Stream,Device);
	StreamWrite(Stream,User);
}

//---------------------------------------------------------------------------

void TNetMessageMsg::Decode(TMemoryStream *Stream)
{
	DecodeNetMessage(Stream);
	StreamRead(Stream,Message);
	StreamRead(Stream,Device);
	StreamRead(Stream,User);
}

void TNetMessageMsg::Action()
{
	TDeviceRealTerminal::Instance().UpdateMessage(this);
}






