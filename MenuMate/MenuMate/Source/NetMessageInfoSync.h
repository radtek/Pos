//---------------------------------------------------------------------------

#ifndef NetMessageInfoSyncH
#define NetMessageInfoSyncH

#include "SimpleTCP.h"

//---------------------------------------------------------------------------
class TNetMessageInfoSync : public TNetMessage
{
public:
   bool Broadcast;
	void Encode(TMemoryStream *Stream); // Writes it'self to Stream Supplied.
	void Decode(TMemoryStream *Stream);
	TNetMessageInfoSync();
	void Action();
};

class TNetMsgMakerInfoSync : public TNetMessageMaker
{
public:
  TNetMsgMakerInfoSync() : TNetMessageMaker(udp_InfoSync) { }
private:
  TNetMessageInfoSync *MakeMessage(TMemoryStream *Data) const
  {
//	 TNetMessageInfoSync *msg = NULL;
    std::auto_ptr<TNetMessageInfoSync> msg(new TNetMessageInfoSync());
	 try
	 {
		// construct the appropriate message type
//		msg = new TNetMessageInfoSync;
		// tell the message to populate itself using the byte stream
		msg->Decode(Data);
	 }
	 catch(...)
	 {
		// handle errors!
	 }
	 return(msg.get());
  }
  static const TNetMsgMakerInfoSync AutoRegister;
};

const TNetMsgMakerInfoSync TNetMsgMakerInfoSync::AutoRegister;

#endif
