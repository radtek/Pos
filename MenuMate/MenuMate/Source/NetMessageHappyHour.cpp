//---------------------------------------------------------------------------


#pragma hdrstop

#include "NetMessageHappyHour.h"
#include "Stream.h"

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)

TNetMessageHappyHour::TNetMessageHappyHour()
{
	MessageType = udp_HappyHour;
	Broadcast = false;
}

void TNetMessageHappyHour::Encode(TMemoryStream *Stream)
{
	EncodeNetMessage(Stream);
	double Time = double(DiscountStart);
	StreamWrite(Stream,Time);
	Time = double(DiscountFinish);
	StreamWrite(Stream,Time);
	for(int i = 0; i < eEndOfDays; i++)
   {
		StreamWrite(Stream,HappyDays[i]);
   }
	StreamWrite(Stream,ForceThisTerminalNow);
	StreamWrite(Stream,TerminalExemptFromHappyHour);
	StreamWrite(Stream,HappyHourMembersOnly);

}

void TNetMessageHappyHour::Decode(TMemoryStream *Stream)
{
   DecodeNetMessage(Stream);
   double Time = 0;
	StreamRead(Stream,Time);
	DiscountStart = Time;
	Time = 0;
	StreamRead(Stream,Time);
   DiscountFinish = Time;
   for(int i = 0; i < eEndOfDays;i++)
   {
		StreamRead(Stream,HappyDays[i]);
   }
	StreamRead(Stream,ForceThisTerminalNow);
	StreamRead(Stream,TerminalExemptFromHappyHour);
	StreamRead(Stream,HappyHourMembersOnly);

}

void TNetMessageHappyHour::Action()
{
	TDeviceRealTerminal::Instance().UpdateHappyHourInfo(this);
}


