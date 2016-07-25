//---------------------------------------------------------------------------


#pragma hdrstop

#include "NetMessageSeatName.h"
#include "Stream.h"

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TNetMessageSeatName::TNetMessageSeatName()
{
	MessageType = udp_SeatName;
}

void TNetMessageSeatName::Encode(TMemoryStream *Stream)
{
	EncodeNetMessage(Stream);
	StreamWrite(Stream,TabLK);
}
//---------------------------------------------------------------------------
void TNetMessageSeatName::Decode(TMemoryStream *Stream)
{
	DecodeNetMessage(Stream);
	StreamRead(Stream,TabLK);
}

void TNetMessageSeatName::Action()
{
	#ifdef MenuMate
		TDeviceRealTerminal::Instance().UpdateSeatName(this);
	#endif
	#ifdef PalmMate
		TDeviceRealTerminal::Instance().UpdateSeatName(this);
	#endif
}
