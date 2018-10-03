//---------------------------------------------------------------------------

#ifndef NetMessageMenuChangedH
#define NetMessageMenuChangedH
//---------------------------------------------------------------------------
#include <vector>

#include "SimpleTCP.h"
#include "enum.h"
#include "ListMenu.h"

class TNetMessageMenuChanged : public TNetMessage
{
public:
   std::map<UnicodeString,eMenuCommand> Menu_Names;
	bool CompareToDataBase;
	bool Broadcast;
   //---------TCPIP------------
	void Encode(TMemoryStream *Stream); // Writes it'self to Stream Supplied.
   void Decode(TMemoryStream *Stream);
	TNetMessageMenuChanged();
	void Action();	
};

class TNetMsgMakerMenuChanged : public TNetMessageMaker
{
public:
  TNetMsgMakerMenuChanged() : TNetMessageMaker(udp_MenuChange) { }
private:
  TNetMessageMenuChanged *MakeMessage(TMemoryStream *Data) const
  {
//	 TNetMessageMenuChanged *msg = NULL;
     std::auto_ptr<TNetMessageMenuChanged> msg(new TNetMessageMenuChanged());
	 try
	 {
		// construct the appropriate message type
//		msg = new TNetMessageMenuChanged;
		// tell the message to populate itself using the byte stream
		msg->Decode(Data);
	 }
	 catch(...)
	 {
		// handle errors!
	 }
	 return(msg.get());
  }
  static const TNetMsgMakerMenuChanged AutoRegister;
};

const TNetMsgMakerMenuChanged TNetMsgMakerMenuChanged::AutoRegister;

#endif
