//---------------------------------------------------------------------------

#ifndef SmartCardVer0APIH
#define SmartCardVer0APIH
//---------------------------------------------------------------------------
#define SMART_CARD_VERSION_UNK 0

#include "ManagerSyndCode.h"
#include "MMContactInfo.h"
#include "SmartCardBlock.h"

class TSmartCard
{
   protected:
      /* identifies whether we have modifications to different parts */
      bool DirtyVersion;
      bool DirtyContacts;
      bool DirtyPoints;
      bool DirtyGUID;

      SCARDHANDLE  hCard;
      TSmartCardBlock BlockData;
      TMMContactInfo ContactInfo;
      TDateTime LastModifiedDate;
      LONG CardInfoWrite(int BlockStart, int MaxBlockLength,TMemoryStream &Stream);
      bool StreamCheckCRC(unsigned short &CRC,unsigned short &CalcCRC,TMemoryStream *Stream, int Begin = 0, int End = 0);
      void EncryptInPlace(TMemoryStream &Data);
      void DecryptInPlace(TMemoryStream &Data,AnsiString SyndicateCode);

      virtual void StreamGetPoints(TMemoryStream &Stream) = 0;
      virtual void StreamGetContact(TMemoryStream &Stream) = 0;

   public:
      BYTE VersionInfo;
      TSyndCode SyndCode;

      TSmartCard(SCARDHANDLE inhCard,TSmartCardBlock &BlockData,TSyndCode inSyndCode);

      SCARDHANDLE GethCard() {return hCard;}
      TSmartCardBlock *GetBlockData() {return &BlockData;}
      TSmartCardBlock &GetBlockDataRef() {return BlockData;}

      virtual TMMContactInfo GetContactInfo() = 0;

      virtual void SetContactInfo(TMMContactInfo inContactInfo) = 0;
      virtual void SetPointsInfo(TContactPoints &inContactPoints) = 0;
      virtual void SetVersionInfo() = 0;
      virtual void SetGUIDInfo(TSmartCardGUID &inGUID) = 0;
      virtual void Save() = 0;
      virtual void Validate(TSmartCardBlock &inCardBlockData) = 0;
      virtual void DiagSmartCardBlock(TSmartCardBlockInfo &Info) = 0;
      virtual void Restore(TSmartCardBlock &RestorePoint) = 0;
      void ReassignHandle(SCARDHANDLE inhCard);
	  virtual void UnlockCard(std::map <int, TSyndCode> SyndCodes) = 0;
      virtual void Upgrade(std::map <int, TSyndCode> SyndCodes) = 0;

      void ReloadCardDetails();
};

#endif
