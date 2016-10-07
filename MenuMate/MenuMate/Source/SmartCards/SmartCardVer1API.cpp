//---------------------------------------------------------------------------


#pragma hdrstop

#include "SmartCardVer1API.h"

#include "Stream.h"
#include "SmartCardDefs.h"
#include "SmartCardException.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TSmartCardVer1::TSmartCardVer1(SCARDHANDLE inhCard,TSmartCardBlock &inBlockData,TSyndCode inSyndCode)
   : TSmartCard(inhCard,inBlockData,inSyndCode)
{
   DirtyEverything = false;
}



void TSmartCardVer1::UnlockCard(std::map <int, TSyndCode> SyndCodes)
{
   unsigned short CRC = 0;
   unsigned short CalcCRC = 0;

   std::auto_ptr<TMemoryStream> ContactStream(new TMemoryStream);
   bool SyndCodeValidated = false;
   std::map <int, TSyndCode> ::iterator ptrSyndCodes = SyndCodes.begin();
   for (ptrSyndCodes = SyndCodes.begin();ptrSyndCodes != SyndCodes.end() && !SyndCodeValidated ; advance(ptrSyndCodes,1))
   {
      StreamGetContact(*ContactStream.get());
      DecryptInPlace(*ContactStream.get(),ptrSyndCodes->second.DecryptedSyndCode);
      ContactInfo.LoadFromStream(BlockData.Version,ContactStream.get());
      CRC = 0; CalcCRC = 0;
      StreamRead(ContactStream.get(),CRC);
      if(StreamCheckCRC(CRC,CalcCRC,ContactStream.get(),0,ContactStream->Position - sizeof(CRC)))
      {
         SyndCodeValidated = true;
         SyndCode = ptrSyndCodes->second;
      }
   }

   if(!SyndCodeValidated)
   {
      throw TSCException(tsceInvalidCRC,"Card Data is Corrupt");
   }
}

void TSmartCardVer1::StreamGetContact(TMemoryStream &Stream)
{
   Stream.Clear();
   BlockData.BlockInfoRead(V1_CARD_MEMBER_DATA_START,V1_CARD_MEMBER_DATA_LENGTH,Stream);
   Stream.Position = 0;   
}

void TSmartCardVer1::StreamGetPoints(TMemoryStream &Stream)
{
    // do nothing as this was the same before this method is added in
}

void TSmartCardVer1::SetContactInfo(TMMContactInfo inContactInfo)
{
    DirtyEverything = true;

	TContactPoints currentPoints = ContactInfo.Points; // keep the current points unchanged.
    currentPoints.PointsRules = inContactInfo.Points.PointsRules;

	ContactInfo = inContactInfo;
    ContactInfo.Points = currentPoints;
}

void TSmartCardVer1::SetPointsInfo(TContactPoints &inContactPoints)
{
   DirtyEverything = true;
   ContactInfo.Points = inContactPoints;
}

TMMContactInfo TSmartCardVer1::GetContactInfo()
{
   return ContactInfo;
}

void TSmartCardVer1::SetGUIDInfo(TSmartCardGUID &inGUID)
{

}

void TSmartCardVer1::SetVersionInfo()
{
}

void TSmartCardVer1::Save()
{
   throw TSCException(tsceValidateFailed,"Save Failed, Wrong card Version.");
}

void TSmartCardVer1::Restore(TSmartCardBlock &RestorePoint)
{
   throw TSCException(tsceValidateFailed,"Restore Failed, Wrong card Version.");
}

void TSmartCardVer1::Validate(TSmartCardBlock &inCardBlockData)
{
   throw TSCException(tsceValidateFailed,"Validate Failed, Wrong card Version.");
}

void TSmartCardVer1::DiagSmartCardBlock(TSmartCardBlockInfo &Info)
{
   throw TSCException(tsceValidateFailed,"Diagnose Failed, Wrong card Version.");
}
void TSmartCardVer1::Upgrade(std::map <int, TSyndCode> SyndCodes)
{
}

