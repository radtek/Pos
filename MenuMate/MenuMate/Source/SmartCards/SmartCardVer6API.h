//---------------------------------------------------------------------------

#ifndef SmartCardVer6APIH
#define SmartCardVer6APIH
//---------------------------------------------------------------------------
#include "SmartCardVer0API.h"
#include "SmartCardVer2API.h"
#include "SmartCardVer3API.h"

#define SMART_CARD_VERSION_SIX 6

#define V6_CARD_POINTS_DATA_START 0xC4
#define V6_CARD_POINTS_DATA_LENGTH 0x20

#define V6_CARD_CRC_DATA_START 0xE4
#define V6_CARD_CRC_DATA_LENGTH 0x02

#define V6_CARD_VERSION_DATA_START 0x1B
#define V6_CARD_VERSION_DATA_LENGTH 0x01

#define V6_CARD_MEMBER_DATA_START 0x2C //0x1C
// Space must be a multiple of 8 for Blowfish encryption. Currently 208 Bytes.
#define V6_CARD_MEMBER_DATA_LENGTH 0x98 //0xA8

// 12 Byte GUID [Site ID] [Member No.] [Local Card No.]
#define V6_CARD_GUID_START 0x08
#define V6_CARD_GUID_LENGTH 0x0C

#define V6_CARD_RESTORE_POINT_DATA_START 0x00

#define V6_CARD_LAST_MODIFIED_DATE_DATA_START 0x1C
#define V6_CARD_LAST_MODIFIED_DATE_DATA_LENGTH 0x10

class TSmartCardVer6 : public TSmartCard
{
   private:
      void StreamGetPoints(TMemoryStream &Stream);
      void StreamGetContact(TMemoryStream &Stream);
      void StreamGetLastModifiedDate(TMemoryStream &Stream);
   public:
      TSmartCardVer6(SCARDHANDLE inhCard,TSmartCardBlock &BlockData,TSyndCode inSyndCode);
      TSmartCardVer6(TSmartCard &inCard);

      TMMContactInfo GetContactInfo();

      void SetContactInfo(TMMContactInfo inContactInfo);
      void SetPointsInfo(TContactPoints &inContactPoints);
      void SetVersionInfo();
      void SetGUIDInfo(TSmartCardGUID &inGUID);

      void Save();
      void Validate(TSmartCardBlock &inCardBlockData);
      void DiagSmartCardBlock(TSmartCardBlockInfo &Info);
      void Restore(TSmartCardBlock &RestorePoint);
      void UnlockCard(std::map <int, TSyndCode> SyndCodes);
      void Upgrade(std::map <int, TSyndCode> SyndCodes);
};
#endif
