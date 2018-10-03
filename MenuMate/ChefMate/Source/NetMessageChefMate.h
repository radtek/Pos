//---------------------------------------------------------------------------

#ifndef NetMessageChefMateH
#define NetMessageChefMateH
//---------------------------------------------------------------------------
#include "SimpleTCP.h"

class TNetMessageChefMate : public TNetMessage
{
private :
public :
   bool DisplayEnabled;
	int OrderNumber;
	int TimeKey;	
	bool RemoveOrder;
   bool OrderTimeScanned;
	bool FinalOrder;
   long TillColour;
	UnicodeString Device;
   UnicodeString User;
   UnicodeString ChitNumber;
   TStringList *Order;
   TDateTime Received;
	TDateTime tmPrinted;
   TDateTime Duration;
	void virtual Encode(TMemoryStream *Stream); // Writes it'self to Stream Supplied.
	void virtual Decode(TMemoryStream *Stream);
	__fastcall TNetMessageChefMate();
	virtual ~TNetMessageChefMate();
	void Action();
	void Assign(TNetMessageChefMate *RHS);		
};

class TNetMsgMakerChefMate : public TNetMessageMaker
{
public:
  TNetMsgMakerChefMate() : TNetMessageMaker(udp_ktichen) { }
private:
  TNetMessageChefMate *MakeMessage(TMemoryStream *Data) const
  {
//	 TNetMessageChefMate *msg = NULL;
    std::auto_ptr<TNetMessageChefMate> msg(new TNetMessageChefMate());
	 try
	 {
		// construct the appropriate message type
//		msg = new TNetMessageChefMate;
		// tell the message to populate itself using the byte stream
		msg->Decode(Data);
	 }
	 catch(...)
	 {
		// handle errors!
	 }
	 return(msg.get());
  }
  static const TNetMsgMakerChefMate AutoRegister;
};

const TNetMsgMakerChefMate TNetMsgMakerChefMate::AutoRegister;

#endif
