//---------------------------------------------------------------------------


#pragma hdrstop

#include "NetMessageInfoSync.h"

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)

TNetMessageInfoSync::TNetMessageInfoSync()
{
	MessageType = udp_InfoSync;
	Broadcast = false;
}

void TNetMessageInfoSync::Encode(TMemoryStream *Stream)
{
	EncodeNetMessage(Stream);
//	StreamWrite(Stream,Broadcast);
}
void TNetMessageInfoSync::Decode(TMemoryStream *Stream)
{
	DecodeNetMessage(Stream);
//	StreamRead(Stream,Broadcast);
}


void TNetMessageInfoSync::Action()
{

   #ifdef PalmMate
	TDeviceRealPalms::Instance().UpdateInfoSync(this);
   #endif

   #ifdef MenuMate
   TDeviceRealTerminal::Instance().UpdateInfoSync(this);
   #endif

}
