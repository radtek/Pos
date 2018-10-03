//---------------------------------------------------------------------------
#ifndef NetMessageMenuAlteredH
#define NetMessageMenuAlteredH

#include "SimpleTCP.h"
//---------------------------------------------------------------------------
class TNetMessageMenuAltered : public TNetMessage
{
public:
	long MenuKey;
	bool ItemEnabled; // Dish Drink Enable/Disable for dbMenuAdjust.
	bool AlterDB;
	bool Broadcast;
	long ItemKey; // Dish Drink LK for dbMenuAdjust.
	//---------TCPIP------------
	void Encode(TMemoryStream *Stream); // Writes it'self to Stream Supplied.
	void Decode(TMemoryStream *Stream);
	TNetMessageMenuAltered();
	void Action();	
};

class TNetMsgMakerMenuAltered : public TNetMessageMaker
{
public:
  TNetMsgMakerMenuAltered() : TNetMessageMaker(udp_MenuAltered) { }
private:
  TNetMessageMenuAltered *MakeMessage(TMemoryStream *Data) const
  {
//	 TNetMessageMenuAltered *msg = NULL;
     std::auto_ptr<TNetMessageMenuAltered> msg(new TNetMessageMenuAltered());
	 try
	 {
		// construct the appropriate message type
//		msg = new TNetMessageMenuAltered;

		// tell the message to populate itself using the byte stream
		msg->Decode(Data);
	 }
	 catch(...)
	 {
		// handle errors!
	 }
	 return(msg.get());
  }
  static const TNetMsgMakerMenuAltered AutoRegister;
};

const TNetMsgMakerMenuAltered TNetMsgMakerMenuAltered::AutoRegister;

#endif
