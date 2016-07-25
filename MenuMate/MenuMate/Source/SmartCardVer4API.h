//---------------------------------------------------------------------------

#ifndef SmartCardVer4APIH
#define SmartCardVer4APIH
//---------------------------------------------------------------------------
#include "SmartCardVer0API.h"
#include "SmartCardVer2API.h"
#include "SmartCardVer3API.h"
#define SMART_CARD_VERSION_FOUR 4

#define V4_CARD_POINTS_DATA_START 0xCE
#define V4_CARD_POINTS_DATA_LENGTH 0x30

#define V4_CARD_CRC_DATA_START 0xFE
#define V4_CARD_CRC_DATA_LENGTH 0x02

#define V4_CARD_VERSION_DATA_START 0x1B
#define V4_CARD_VERSION_DATA_LENGTH 0x01

#define V4_CARD_MEMBER_DATA_START 0x1C
// Space must be a multiple of 8 for Blowfish encryption. Currently 208 Bytes.
#define V4_CARD_MEMBER_DATA_LENGTH 0xD0

// 12 Byte GUID [Site ID] [Member No.] [Local Card No.]
#define V4_CARD_GUID_START 0x08
#define V4_CARD_GUID_LENGTH 0x0C

#define V4_CARD_RESTORE_POINT_DATA_START 0x00

class TSmartCardVer4 : public TSmartCard
{
   private:
        void StreamGetPoints(TMemoryStream &Stream);
        void StreamGetPoints(TMemoryStream &Stream, int ver);
        void StreamGetContact(TMemoryStream &Stream);
	public:
        TSmartCardVer4(SCARDHANDLE inhCard,TSmartCardBlock &BlockData,TSyndCode inSyndCode);
        TSmartCardVer4(TSmartCardVer3 &inCard);

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

        std::map <int, TSyndCode> SyndCodesP;
};
#endif
