//---------------------------------------------------------------------------

#ifndef NetMessageSeatNameH
#define NetMessageSeatNameH
//---------------------------------------------------------------------------
#include "SimpleTCP.h"

class TNetMessageSeatName : public TNetMessage
{
public:
	int TabLK;
	void Encode(TMemoryStream *Stream); // Writes it'self to Stream Supplied.
	void Decode(TMemoryStream *Stream);
	__fastcall TNetMessageSeatName();
	void Action();	
};

class TNetMsgMakerSeatName : public TNetMessageMaker
{
public:
  TNetMsgMakerSeatName() : TNetMessageMaker(udp_SeatName) { }
private:
  TNetMessageSeatName *MakeMessage(TMemoryStream *Data) const
  {
	 TNetMessageSeatName *msg = NULL;
	 try
	 {
		// construct the appropriate message type
		msg = new TNetMessageSeatName;
		// tell the message to populate itself using the byte stream
		msg->Decode(Data);
	 }
	 catch(...)
	 {
		// handle errors!
	 }
	 return(msg);
  }
  static const TNetMsgMakerSeatName AutoRegister;
};

const TNetMsgMakerSeatName TNetMsgMakerSeatName::AutoRegister;

#endif
 