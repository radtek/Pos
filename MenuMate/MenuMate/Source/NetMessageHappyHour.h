//---------------------------------------------------------------------------

#ifndef NetMessageHappyHourH
#define NetMessageHappyHourH

#include "SimpleTCP.h"
#include "enum.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------
 class TNetMessageHappyHour : public TNetMessage
{
public:
	TDateTime DiscountStart;
	TDateTime DiscountFinish;
	bool HappyDays[eEndOfDays];
	bool Broadcast;
   bool TerminalExemptFromHappyHour;
   bool HappyHourMembersOnly;
   bool ForceThisTerminalNow;
   bool NoPointsEarnedDuringHappyHour;

	void Encode(TMemoryStream *Stream); // Writes it'self to Stream Supplied.
	void Decode(TMemoryStream *Stream);
	TNetMessageHappyHour();
	void Action();

    int Profile_Key;
    AnsiString Profile_Name;
    TDateTime HappyHourDate;

    bool HappyHourSettings[3];     // change the array size to no of settings in happy hour if there is any increase
                                   // in number of settings in happy hour



};


class TNetMsgMakerHappyHour : public TNetMessageMaker
{
public:
  TNetMsgMakerHappyHour() : TNetMessageMaker(udp_HappyHour) { }
private:
  TNetMessageHappyHour *MakeMessage(TMemoryStream *Data) const
  {
//	 TNetMessageHappyHour *msg = NULL;
     std::auto_ptr<TNetMessageHappyHour> msg(new TNetMessageHappyHour());
	 try
	 {
		// construct the appropriate message type
//		msg = new TNetMessageHappyHour;
		// tell the message to populate itself using the byte stream
		msg->Decode(Data);
	 }
	 catch(...)
	 {
		// handle errors!
	 }
	 return(msg.get());
  }
  static const TNetMsgMakerHappyHour AutoRegister;
};




const TNetMsgMakerHappyHour TNetMsgMakerHappyHour::AutoRegister;

#endif
 