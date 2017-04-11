//---------------------------------------------------------------------------


#pragma hdrstop

#include "casmcard.h"
#include "Stream.h"

#include "SmartCardVer2API.h"

#include "MMLogging.h"
#include "SmartCardDefs.h"
#include "SmartCardException.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TSmartCardVer2::TSmartCardVer2(SCARDHANDLE inhCard,TSmartCardBlock &BlockData,TSyndCode inSyndCode)
   : TSmartCard(inhCard,BlockData,inSyndCode)
{
   VersionInfo    = SMART_CARD_VERSION_TWO;
}

TSmartCardVer2::TSmartCardVer2(TSmartCardVer1 &inCard)
   : TSmartCard(inCard.GethCard(),inCard.GetBlockDataRef(),inCard.SyndCode)
{
   // Import Data rather than loading it from the block.
   SyndCode = inCard.SyndCode;
   SetVersionInfo();
   SetContactInfo(inCard.GetContactInfo());
   SetPointsInfo(inCard.GetContactInfo().Points);
}

void TSmartCardVer2::UnlockCard(std::map <int, TSyndCode> SyndCodes)
{
   // Read in entire card block and check it for curruption.
   std::auto_ptr<TMemoryStream> Stream(new TMemoryStream);
   unsigned short CRC = 0;
   unsigned short CalcCRC = 0;
   BlockData.Read(V2_CARD_CRC_DATA_START,CRC);
   if(!StreamCheckCRC(CRC,CalcCRC,BlockData.GetStream(),V2_CARD_VERSION_DATA_START,CARD_TOTAL_MEMORY_LENGTH - V2_CARD_VERSION_DATA_START - V2_CARD_CRC_DATA_LENGTH))
   {
      throw TSCException(tsceInvalidCRC,"Card Data is Corrupt");
   }

   // Retrieve Decrypted Contact Block & Points Block.
   // Both blocks must unencrypt in order to be Valid.
   bool SyndCodeValidated = false;
   std::auto_ptr<TMemoryStream> ContactStream(new TMemoryStream);

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
         std::auto_ptr<TMemoryStream> PointsStream(new TMemoryStream);
         StreamGetPoints(*PointsStream.get());
         DecryptInPlace(*PointsStream.get(),ptrSyndCodes->second.DecryptedSyndCode);
         ContactInfo.Points.LoadFromStream(BlockData.Version,PointsStream.get());
         CRC = 0; CalcCRC = 0;
         StreamRead(PointsStream.get(),CRC);
         if(StreamCheckCRC(CRC,CalcCRC,PointsStream.get(),0,PointsStream->Position - sizeof(CRC)))
         {
            SyndCodeValidated = true;
            SyndCode = ptrSyndCodes->second;
         }
      }
   }
   if(!SyndCodeValidated)
   {
      throw TSCException(tsceInvalidSyndCRC,"Card unencrypt Failed.");
   }
}

void TSmartCardVer2::DiagSmartCardBlock(TSmartCardBlockInfo &Info)
{
   // Read in entire card block and check it for curruption.
   std::auto_ptr<TMemoryStream> Stream(new TMemoryStream);
   unsigned short CRC = 0;
   unsigned short CalcCRC = 0;
   BlockData.Read(V2_CARD_CRC_DATA_START,CRC);
   if(!StreamCheckCRC(CRC,CalcCRC,BlockData.GetStream(),V2_CARD_VERSION_DATA_START,CARD_TOTAL_MEMORY_LENGTH - V2_CARD_VERSION_DATA_START - V2_CARD_CRC_DATA_LENGTH))
   {
      Info.CorruptCardCRC = true;
   }

   // Retrieve Decrypted Contact Block.
   std::auto_ptr<TMemoryStream> ContactStream(new TMemoryStream);
   StreamGetContact(*ContactStream.get());
   ContactInfo.LoadFromStream(BlockData.Version,ContactStream.get());
   CRC = 0;
   CalcCRC = 0;
   StreamRead(ContactStream.get(),CRC);
   if(!StreamCheckCRC(CRC,CalcCRC,ContactStream.get(),0,ContactStream->Position - sizeof(CRC)))
   {
      Info.CorruptContactInfo = true;
   }

   // There may be no points or point structure on the card.
   std::auto_ptr<TMemoryStream> PointsStream(new TMemoryStream);
   StreamGetPoints(*PointsStream.get());
   ContactInfo.Points.LoadFromStream(BlockData.Version,PointsStream.get());
   CRC = 0;
   CalcCRC = 0;
   StreamRead(PointsStream.get(),CRC);
   if(!StreamCheckCRC(CRC,CalcCRC,PointsStream.get(),0,PointsStream->Position - sizeof(CRC)))
   {
      Info.CorruptPointsInfo = true;
   }
}

void TSmartCardVer2::Validate(TSmartCardBlock &inCardBlockData)
{
   TMemoryStream *BlockStream = BlockData.GetStream();
   TMemoryStream *CardStream = inCardBlockData.GetStream();
   // You could double check the card CRC here.
   unsigned short CRC = 0;
   unsigned short CalcCRC = 0;
   BlockData.Read(V2_CARD_CRC_DATA_START,CRC);
   if(!StreamCheckCRC(CRC,CalcCRC,CardStream,V2_CARD_VERSION_DATA_START,CARD_TOTAL_MEMORY_LENGTH - V2_CARD_VERSION_DATA_START - V2_CARD_CRC_DATA_LENGTH))
   {
      throw TSCException(tsceValidateFailed,"Failed to validate Card CRC.");
   }

   for (int i = V2_CARD_VERSION_DATA_START; i < V2_CARD_VERSION_DATA_START + V2_CARD_VERSION_DATA_LENGTH  ; i++)
   {
      if(((BYTE *)BlockStream->Memory)[i] != ((BYTE *)CardStream->Memory)[i])
      {
         throw TSCException(tsceValidateFailed,"Failed to validate Version information.");
      }
   }

   // Compare it to the current block Member Data.
   for (int i = V2_CARD_MEMBER_DATA_START; i < V2_CARD_MEMBER_DATA_START + V2_CARD_MEMBER_DATA_LENGTH  ; i++)
   {
      if(((BYTE *)BlockStream->Memory)[i] != ((BYTE *)CardStream->Memory)[i])
      {
         throw TSCException(tsceValidateFailed,"Failed to validate contact information.");
      }
   }

   // Compare it to the current block Member Data.
   for (int i = V2_CARD_POINTS_DATA_START; i < V2_CARD_POINTS_DATA_START + V2_CARD_POINTS_DATA_LENGTH  ; i++)
   {
      if(((BYTE *)BlockStream->Memory)[i] != ((BYTE *)CardStream->Memory)[i])
      {
         throw TSCException(tsceValidateFailed,"Failed to validate points information.");
      }
   }

   /* Valadation of the GUID is not supported.
      When saving a user to a new blank card (formatted card) the GUID will be
      blank in the Block image and set on the card. */
   for (int i = V2_CARD_GUID_START; i < V2_CARD_GUID_START + V2_CARD_GUID_LENGTH  ; i++)
   {
      if(((BYTE *)BlockStream->Memory)[i] != ((BYTE *)CardStream->Memory)[i])
      {
         throw TSCException(tsceValidateFailed,"Failed to validate GUID information.");
      }
   }
}


void TSmartCardVer2::SetContactInfo(TMMContactInfo inContactInfo)
{
    DirtyContacts = true;
	TContactPoints currentPoints = ContactInfo.Points; // keep the current points unchanged.
    currentPoints.PointsRules = inContactInfo.Points.PointsRules;

	ContactInfo = inContactInfo;
    ContactInfo.Points = currentPoints;

   if(BlockData.Empty())
   {
      ContactInfo.CardCreationDate = Now();
      inContactInfo.CardCreationDate = ContactInfo.CardCreationDate;
   }
}


TMMContactInfo TSmartCardVer2::GetContactInfo()
{
   ContactInfo.ATRStr = BlockData.CardGuid.GUID();
   return ContactInfo;
}

void TSmartCardVer2::SetVersionInfo()
{
   DirtyVersion = true;
   VersionInfo = SMART_CARD_VERSION_TWO;
}

void TSmartCardVer2::SetPointsInfo(TContactPoints &inContactPoints)
{
   DirtyPoints = true;
   ContactInfo.Points = inContactPoints;
}

void TSmartCardVer2::SetGUIDInfo(TSmartCardGUID &inGUID)
{
   DirtyGUID = true;
   BlockData.CardGuid.Assign(inGUID);
}

void TSmartCardVer2::Save()
{
   if(!SyndCode.Valid())
   {
      throw TSCException(tsceWriteFailed,"Invalid Encryption Code.");
   }

   std::auto_ptr<TMemoryStream> VersionStream(new TMemoryStream);
   std::auto_ptr<TMemoryStream> ContactStream(new TMemoryStream);
   std::auto_ptr<TMemoryStream> PointsStream(new TMemoryStream);
   std::auto_ptr<TMemoryStream> GUIDStream(new TMemoryStream);
   std::auto_ptr<TMemoryStream> CRCStream(new TMemoryStream);

   bool UpdateCRC = false;
   // Write it to the blocks first to detcect overflow.
   if(DirtyVersion)
   {
      VersionStream->Clear();
      StreamWrite(VersionStream.get(),VersionInfo);
      BlockData.BlockInfoWrite(V2_CARD_VERSION_DATA_START,V2_CARD_VERSION_DATA_LENGTH,*VersionStream.get());
      UpdateCRC = true;
   }

   if(DirtyContacts)
   {
      ContactInfo.SaveToStream(ContactStream.get());
      unsigned short CRC = 0;
      unsigned short CalcCRC = 0;
      StreamCheckCRC(CRC,CalcCRC,ContactStream.get(),0,ContactStream->Size);
      StreamWrite(ContactStream.get(),CalcCRC);
      EncryptInPlace(*ContactStream.get());
      BlockData.BlockInfoWrite(V2_CARD_MEMBER_DATA_START,V2_CARD_MEMBER_DATA_LENGTH,*ContactStream.get());
      UpdateCRC = true;
   }

   if(DirtyPoints)
   {
      PointsStream->Clear();
      ContactInfo.Points.SaveToStream(PointsStream.get());
      unsigned short CRC = 0;
      unsigned short CalcCRC = 0;
      StreamCheckCRC(CRC,CalcCRC,PointsStream.get(),0,PointsStream->Size);
      StreamWrite(PointsStream.get(),CalcCRC);
      EncryptInPlace(*PointsStream.get());
      BlockData.BlockInfoWrite(V2_CARD_POINTS_DATA_START,V2_CARD_POINTS_DATA_LENGTH,*PointsStream.get());
      UpdateCRC = true;
   }

   if(DirtyGUID)
   {
      GUIDStream->Clear();
      BlockData.CardGuid.SaveToStream(GUIDStream.get());
      BlockData.BlockInfoWrite(V2_CARD_GUID_START,V2_CARD_GUID_LENGTH,*GUIDStream.get());
   }

   if(UpdateCRC)
   { // CRC.
      unsigned short CRC = 0;
      unsigned short CalcCRC = 0;
      StreamCheckCRC(CRC,CalcCRC,BlockData.GetStream(),V2_CARD_VERSION_DATA_START,CARD_TOTAL_MEMORY_LENGTH - V2_CARD_VERSION_DATA_START - V2_CARD_CRC_DATA_LENGTH);
      StreamWrite(CRCStream.get(),CalcCRC);
      BlockData.BlockInfoWrite(V2_CARD_CRC_DATA_START,V2_CARD_CRC_DATA_LENGTH,*CRCStream.get());
   }
   // Begin Actual Card Writes.

   if(DirtyVersion)
   {
      LONG lReturn = CardInfoWrite(V2_CARD_VERSION_DATA_START,V2_CARD_VERSION_DATA_LENGTH,*VersionStream.get());
      if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
      {
         TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing Version Data Failed. : " + AnsiString(IntToHex(int(lReturn),2)));
         throw TSCException(tsceWriteFailed,"Write Version Info Failed.");
      }
      else
      {
         DirtyVersion = false;
      }
   }

   if(DirtyContacts)
   {
      LONG lReturn = CardInfoWrite(V2_CARD_MEMBER_DATA_START,V2_CARD_MEMBER_DATA_LENGTH,*ContactStream.get());
      if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
      {
         TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing Contact Data Failed. : " + AnsiString(IntToHex(int(lReturn),2)));
         throw TSCException(tsceWriteFailed,"Write Contact Info Failed.");
      }
      else
      {
         DirtyContacts = false;
      }
   }

   if(DirtyPoints)
   {
      LONG lReturn = CardInfoWrite(V2_CARD_POINTS_DATA_START,V2_CARD_POINTS_DATA_LENGTH,*PointsStream.get());
      if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
      {
         TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing Points Data Failed. : " + AnsiString(IntToHex(int(lReturn),2)));
         throw TSCException(tsceWriteFailed,"Write Points Info Failed.");
      }
      else
      {
         DirtyPoints = false;
      }
   }

   if(DirtyGUID)
   {
      LONG lReturn = CardInfoWrite(V2_CARD_GUID_START,V2_CARD_GUID_LENGTH,*GUIDStream.get());
      if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
      {
         TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing GUID Data Failed. : " + AnsiString(IntToHex(int(lReturn),2)));
         throw TSCException(tsceWriteFailed,"Write GUID Info Failed.");
      }
      else
      {
         DirtyGUID = false;
      }
   }

   if(UpdateCRC)
   { // CRC.
      LONG lReturn = CardInfoWrite(V2_CARD_CRC_DATA_START,V2_CARD_CRC_DATA_LENGTH,*CRCStream.get());
      if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
      {
         TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing CRC Data Failed. : " + AnsiString(IntToHex(int(lReturn),2)));
         throw TSCException(tsceWriteFailed,"Write CRC Info Failed.");
      }
   }
}

void TSmartCardVer2::StreamGetContact(TMemoryStream &Stream)
{
   Stream.Clear();
   Stream.Position = 0;
   BlockData.BlockInfoRead(V2_CARD_MEMBER_DATA_START,V2_CARD_MEMBER_DATA_LENGTH,Stream);
   Stream.Position = 0;
}

void TSmartCardVer2::StreamGetPoints(TMemoryStream &Stream)
{
   Stream.Clear();
   BlockData.BlockInfoRead(V2_CARD_POINTS_DATA_START,V2_CARD_POINTS_DATA_LENGTH,Stream);
   Stream.Position = 0;
}

void TSmartCardVer2::Restore(TSmartCardBlock &RestorePoint)
{
   LONG lReturn = SCARD_F_UNKNOWN_ERROR;
   TMemoryStream &RestoreStream = RestorePoint.GetStreamRef();
   lReturn = CardInfoWrite(V2_CARD_RESTORE_POINT_DATA_START,CARD_RESTORE_POINT_LENGTH,RestoreStream);
   RestoreStream.Position = 0;
   if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
   {
      TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Write Restore Point Failed. : " + AnsiString(IntToHex(int(lReturn),2)));
      throw TSCException(tsceWriteFailed,"Write Restore Point Failed.");
   }
}

void TSmartCardVer2::Upgrade(std::map <int, TSyndCode> SyndCodes)
{
}

