//---------------------------------------------------------------------------

#ifndef NetMessageKillProcH
#define NetMessageKillProcH
//---------------------------------------------------------------------------
#include "SimpleTCP.h"

class TNetMessageKillProc : public TNetMessage
{
public:
	UnicodeString Device;
	UnicodeString User;
	void Encode(TMemoryStream *Stream); // Writes it'self to Stream Supplied.
	void Decode(TMemoryStream *Stream);
	TNetMessageKillProc();
	void Action();
};

class TNetMsgMakerKillProc : public TNetMessageMaker
{
public:
  TNetMsgMakerKillProc() : TNetMessageMaker(udp_KillProc) { }
private:
  TNetMessage *MakeMessage(TMemoryStream *Data) const
  {
	 TNetMessageKillProc *msg = NULL;
	 try
	 {
		// construct the appropriate message type
		msg = new TNetMessageKillProc;
		// tell the message to populate itself using the byte stream
		msg->Decode(Data);
	 }
	 catch(...)
	 {
		// handle errors!
	 }
	 return(msg);
  }
  static const TNetMsgMakerKillProc AutoRegister;
};

const TNetMsgMakerKillProc TNetMsgMakerKillProc::AutoRegister;

#endif
