//---------------------------------------------------------------------------

#ifndef SmartCardVer2APIH
#define SmartCardVer2APIH
//---------------------------------------------------------------------------
#include "SmartCardVer0API.h"
#include "SmartCardVer1API.h"

#define SMART_CARD_VERSION_TWO 2

#define V2_CARD_POINTS_DATA_START 0xEE
#define V2_CARD_POINTS_DATA_LENGTH 0x10

#define V2_CARD_CRC_DATA_START 0xFE
#define V2_CARD_CRC_DATA_LENGTH 0x02

#define V2_CARD_VERSION_DATA_START 0x1B
#define V2_CARD_VERSION_DATA_LENGTH 0x01

#define V2_CARD_MEMBER_DATA_START 0x1C
// Space must be a multiple of 8 for Blowfish encryption. Currently 208 Bytes.
#define V2_CARD_MEMBER_DATA_LENGTH 0xD0

// 12 Byte GUID [Site ID] [Member No.] [Local Card No.]
#define V2_CARD_GUID_START 0x08
#define V2_CARD_GUID_LENGTH 0x0C

#define V2_CARD_RESTORE_POINT_DATA_START 0x00

class TSmartCardVer2 : public TSmartCard
{
   private:
      void StreamGetPoints(TMemoryStream &Stream);
      void StreamGetContact(TMemoryStream &Stream);
   public:
      TSmartCardVer2(SCARDHANDLE inhCard,TSmartCardBlock &inBlockData,TSyndCode inSyndCode);
      TSmartCardVer2(TSmartCardVer1 &inCard);
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
 