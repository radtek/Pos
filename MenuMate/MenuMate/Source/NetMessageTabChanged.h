//---------------------------------------------------------------------------

#ifndef NetMessageTabChangedH
#define NetMessageTabChangedH

#include "SimpleTCP.h"

//---------------------------------------------------------------------------
class TNetMessageTabChanged : public TNetMessage
{
public:
	int TabKey;
	//---------TCPIP------------
	void Encode(TMemoryStream *Stream); // Writes it'self to Stream Supplied.
	void Decode(TMemoryStream *Stream);
	TNetMessageTabChanged();
	void Action();	
};

class TNetMsgMakerTabChanged : public TNetMessageMaker
{
public:
  TNetMsgMakerTabChanged() : TNetMessageMaker(udp_TabChanged) { }
private:
  TNetMessageTabChanged *MakeMessage(TMemoryStream *Data) const
  {
//	 TNetMessageTabChanged *msg = NULL;
    std::auto_ptr<TNetMessageTabChanged> msg(new TNetMessageTabChanged());
	 try
	 {
		// construct the appropriate message type
//		msg = new TNetMessageTabChanged;
		// tell the message to populate itself using the byte stream
		msg->Decode(Data);
	 }
	 catch(...)
	 {
		// handle errors!
	 }
	 return(msg.get());
  }
  static const TNetMsgMakerTabChanged AutoRegister;
};

const TNetMsgMakerTabChanged TNetMsgMakerTabChanged::AutoRegister;

#endif
 