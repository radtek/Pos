//---------------------------------------------------------------------------

#ifndef SmartCardVer1APIH
#define SmartCardVer1APIH
//---------------------------------------------------------------------------
#include "SmartCardVer0API.h"

#define SMART_CARD_VERSION_ONE 1

#define V1_CARD_MEMBER_DATA_START 0x1F
#define V1_CARD_MEMBER_DATA_LENGTH 0xE0

class TSmartCardVer1 : public TSmartCard
{
   private:
      void StreamGetContact(TMemoryStream &Stream);
      void StreamGetPoints(TMemoryStream &Stream);
      bool DirtyEverything;
   public:
      TSmartCardVer1(SCARDHANDLE inhCard,TSmartCardBlock &inBlockData,TSyndCode inSyndCode);
      void SetContactInfo(TMMContactInfo inContactInfo);
      TMMContactInfo GetContactInfo();
      void SetPointsInfo(TContactPoints &inContactPoints);
      void SetVersionInfo();
      void SetGUIDInfo(TSmartCardGUID &inGUID);
      void Save();
      void Restore(TSmartCardBlock &RestorePoint);      
      void Validate(TSmartCardBlock &inCardBlockData);
      void DiagSmartCardBlock(TSmartCardBlockInfo &Info);
	  void UnlockCard(std::map <int, TSyndCode> SyndCodes);
      void Upgrade(std::map <int, TSyndCode> SyndCodes);
};

#endif
