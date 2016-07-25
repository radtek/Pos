//---------------------------------------------------------------------------

#ifndef SmartCardVer3APIH
#define SmartCardVer3APIH
//---------------------------------------------------------------------------
#include "SmartCardVer0API.h"
#include "SmartCardVer2API.h"

#define SMART_CARD_VERSION_THREE 3

#define V3_CARD_POINTS_DATA_START 0xEE
#define V3_CARD_POINTS_DATA_LENGTH 0x10

#define V3_CARD_CRC_DATA_START 0xFE
#define V3_CARD_CRC_DATA_LENGTH 0x02

#define V3_CARD_VERSION_DATA_START 0x1B
#define V3_CARD_VERSION_DATA_LENGTH 0x01

#define V3_CARD_MEMBER_DATA_START 0x1C
// Space must be a multiple of 8 for Blowfish encryption. Currently 208 Bytes.
#define V3_CARD_MEMBER_DATA_LENGTH 0xD0

// 12 Byte GUID [Site ID] [Member No.] [Local Card No.]
#define V3_CARD_GUID_START 0x08
#define V3_CARD_GUID_LENGTH 0x0C

#define V3_CARD_RESTORE_POINT_DATA_START 0x00

class TSmartCardVer3 : public TSmartCard
{
   private:
      void StreamGetPoints(TMemoryStream &Stream);
      void StreamGetContact(TMemoryStream &Stream);
   public:
      TSmartCardVer3(SCARDHANDLE inhCard,TSmartCardBlock &BlockData,TSyndCode inSyndCode);
      TSmartCardVer3(TSmartCardVer2 &inCard);
      void SetContactInfo(TMMContactInfo inContactInfo);
      TMMContactInfo GetContactInfo();
      void SetPointsInfo(TContactPoints &inContactPoints);
      void SetGUIDInfo(TSmartCardGUID &inGUID);
      void SetVersionInfo();
      void Validate(TSmartCardBlock &inCardBlockData);
      void DiagSmartCardBlock(TSmartCardBlockInfo &Info);
      void Restore(TSmartCardBlock &RestorePoint);
      void UnlockCard(std::map <int, TSyndCode> SyndCodes);
      void Save();
	  void Upgrade(std::map <int, TSyndCode> SyndCodes);
};
#endif
