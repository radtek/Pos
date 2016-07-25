//---------------------------------------------------------------------------
#ifndef SimpleTCPH
#define SimpleTCPH

#include <classes.hpp>
#include <map>

#define MENUMATEPORT			65001
#define PALMMATEPORT			65002
#define CHEFMATEPORT			65004
#define LOGSDATAPORT			65005

const int NetPacketVersion = 4000;

enum udpPackets {
    udp_Unknown = 400,
    udp_HappyHour,
    udp_MenuChange,
    udp_MenuAltered,
    udp_SeatName,
    udp_InfoSync,
    udp_Message,
    udp_ktichen,
    udp_TabChanged,
    udp_LogsData,
    udp_KillProc,
    udp_item_definition_altered,
    udp_size_definition_altered
};

enum udpProduct {
    udpProdUnknown = 0,
    udpMenuMate,
    udpPalmMate,
    udpChefMate
};

class TNetMessage
{
public:
	int MessageType;
	int VersionNumber;
   int MMSubNet;
   udpProduct ProductID;
	TNetMessage();

   TNetMessage(udpPackets type);
	virtual ~TNetMessage();

	void clear(void) { }

	void virtual Encode(TMemoryStream *Stream) = 0; // Writes it'self to Stream Supplied.
	void virtual Decode(TMemoryStream *Stream) = 0;
	void EncodeNetMessage(TMemoryStream *Stream);
	void DecodeNetMessage(TMemoryStream *Stream);

	UnicodeString GetFrom(void) { return(m_from); }
	UnicodeString GetTo(void)   { return(m_to);   }

	virtual void Action() = 0;
protected:

	void SetFrom(UnicodeString id) { m_from = id; }
	void SetTo(UnicodeString id) { m_to = id; }

	UnicodeString m_from;
	UnicodeString m_to;
};

class TNetMessageMaker;
typedef std::map<int,TNetMessageMaker*> NetMessageMakerMap;

class TNetMessageMaker
{
protected:
  virtual TNetMessage *MakeMessage(TMemoryStream *Data) const = 0;
  static NetMessageMakerMap RegisteredMakers;
public:
  TNetMessageMaker(int type)
  {
	 RegisteredMakers.insert(std::make_pair(type, this));
  }
  static TNetMessage *ConstructMessage(TMemoryStream *Data);
};

// ---------------------------- Old Way --------------------------------------

enum udpInstructionType {udp_InstUnknown,udpInfo,udpCommand,udp_InstTesting};


//---------------------------------------------------------------------------
#endif
