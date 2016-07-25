//---------------------------------------------------------------------------

#ifndef NetMessageMsgH
#define NetMessageMsgH
//---------------------------------------------------------------------------
#include "SimpleTCP.h"

class TNetMessageMsg : public TNetMessage
{
public:
	UnicodeString Message;
	UnicodeString Device;
	UnicodeString User;
	void Encode(TMemoryStream *Stream); // Writes it'self to Stream Supplied.
	void Decode(TMemoryStream *Stream);
	TNetMessageMsg();
	void Action();
};

class TNetMsgMakerMessage : public TNetMessageMaker
{
public:
  TNetMsgMakerMessage() : TNetMessageMaker(udp_Message) { }
private:
  TNetMessage *MakeMessage(TMemoryStream *Data) const
  {
	 TNetMessageMsg *msg = NULL;
	 try
	 {
		// construct the appropriate message type
		msg = new TNetMessageMsg;
		// tell the message to populate itself using the byte stream
		msg->Decode(Data);
	 }
	 catch(...)
	 {
		// handle errors!
	 }
	 return(msg);
  }
  static const TNetMsgMakerMessage AutoRegister;
};

const TNetMsgMakerMessage TNetMsgMakerMessage::AutoRegister;

#endif
