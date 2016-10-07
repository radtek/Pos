//---------------------------------------------------------------------------


#pragma hdrstop

#include "casmcard.h"
#include "Stream.h"

#include "SmartCardVer4API.h"

#include "MMLogging.h"
#include "SmartCardDefs.h"
#include "SmartCardException.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TSmartCardVer4::TSmartCardVer4(SCARDHANDLE inhCard,TSmartCardBlock &BlockData,TSyndCode inSyndCode)
   : TSmartCard(inhCard,BlockData,inSyndCode)
{
    VersionInfo    = SMART_CARD_VERSION_FOUR;
}

TSmartCardVer4::TSmartCardVer4(TSmartCardVer3 &inCard)
   : TSmartCard(inCard.GethCard(),inCard.GetBlockDataRef(),inCard.SyndCode)
{
	// Import Data rather than loading it from the block.
   SyndCode = inCard.SyndCode;
   SetVersionInfo();
   SetContactInfo(inCard.GetContactInfo());
   SetPointsInfo(inCard.GetContactInfo().Points);
}

void TSmartCardVer4::UnlockCard(std::map <int, TSyndCode> SyndCodes)
{
   // Read in entire card block and check it for curruption.
   std::auto_ptr<TMemoryStream> Stream(new TMemoryStream);
   unsigned short CRC = 0;
   unsigned short CalcCRC = 0;
	BlockData.Read(V4_CARD_CRC_DATA_START,CRC);
	if(!StreamCheckCRC(CRC,CalcCRC,BlockData.GetStream(),V4_CARD_VERSION_DATA_START,CARD_TOTAL_MEMORY_LENGTH - V4_CARD_VERSION_DATA_START - V4_CARD_CRC_DATA_LENGTH))
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
#ifdef _DEBUG
	  ContactStream->SaveToFile(Now().FormatString(" yyyy-mmm-dd hh-nn-ss") + "MMCardEncryptedContactSteam.bin");
#endif
      DecryptInPlace(*ContactStream.get(),ptrSyndCodes->second.DecryptedSyndCode);
#ifdef _DEBUG
	  ContactStream->SaveToFile(Now().FormatString(" yyyy-mmm-dd hh-nn-ss") + "MMCardDecryptedContactSteam.bin");
#endif

		ContactInfo.LoadFromStream(BlockData.Version,ContactStream.get());
        CRC = 0; CalcCRC = 0;
		StreamRead(ContactStream.get(),CRC);
        if(StreamCheckCRC(CRC,CalcCRC,ContactStream.get(),0,ContactStream->Position - sizeof(CRC)))
        {
            std::auto_ptr<TMemoryStream> PointsStream(new TMemoryStream);
            StreamGetPoints(*PointsStream.get(), SMART_CARD_VERSION_FOUR);
            #ifdef _DEBUG
            PointsStream->SaveToFile(Now().FormatString(" yyyy-mmm-dd hh-nn-ss") + "MMCardEncryptedPointsStreamRead.bin");
            #endif

            DecryptInPlace(*PointsStream.get(),ptrSyndCodes->second.DecryptedSyndCode);
            #ifdef _DEBUG
            PointsStream->SaveToFile(Now().FormatString(" yyyy-mmm-dd hh-nn-ss") + "MMCardDecryptedPointsStreamRead.bin");
            #endif

            // TODO 1 -o Michael -c Descrespancy : Investiage the if this is a mistake?
            /* There is no version 4 LoadFromStream in contact points, posibly because the save was
            	broken in some way? This therefore does nothing. BYE BYE POINTS ON CARD!!!!*/
            ContactInfo.Points.LoadFromStream(BlockData.Version,PointsStream.get());

            CRC = 0; CalcCRC = 0;
            StreamRead(PointsStream.get(),CRC);
            if(StreamCheckCRC(CRC,CalcCRC,PointsStream.get(),0,PointsStream->Position - sizeof(CRC)))
            {
                SyndCodeValidated = true;
                SyndCode = ptrSyndCodes->second;

				// TODO 1 -o Michael -c Descrespancy : Check this, is preloaded working? It looks like it's been skipped to version 5 cards and is not
                // handled there.
                /*if(ContactInfo.Name == "preloaded")
                {
                	ContactPoints.LoadedPoints = ContactPoints.CurrentPoints;
                }
                else if(TGlobalSettings::Instance().EnableSeperateEarntPts &&
                        ContactPoints.EarntPoints == 0 &&
                        ContactPoints.LoadedPoints == 0 &&
                        ContactPoints.CurrentPoints != 0)
                {
                	ContactPoints.EarntPoints = ContactPoints.CurrentPoints;
                } */


            }
        }
   }
   if(!SyndCodeValidated)
   {
      throw TSCException(tsceInvalidSyndCRC,"Card unencrypt Failed.");
   }
}

void TSmartCardVer4::DiagSmartCardBlock(TSmartCardBlockInfo &Info)
{
   // Read in entire card block and check it for curruption.
   std::auto_ptr<TMemoryStream> Stream(new TMemoryStream);
   unsigned short CRC = 0;
   unsigned short CalcCRC = 0;
	BlockData.Read(V4_CARD_CRC_DATA_START,CRC);
   if(!StreamCheckCRC(CRC,CalcCRC,BlockData.GetStream(),V4_CARD_VERSION_DATA_START,CARD_TOTAL_MEMORY_LENGTH - V4_CARD_VERSION_DATA_START - V4_CARD_CRC_DATA_LENGTH))
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
   StreamGetPoints(*PointsStream.get(), SMART_CARD_VERSION_FOUR);
   ContactInfo.Points.LoadFromStream(BlockData.Version,PointsStream.get());
   CRC = 0;
   CalcCRC = 0;
   StreamRead(PointsStream.get(),CRC);
   if(!StreamCheckCRC(CRC,CalcCRC,PointsStream.get(),0,PointsStream->Position - sizeof(CRC)))
   {
      Info.CorruptPointsInfo = true;
   }
}

void TSmartCardVer4::Validate(TSmartCardBlock &inCardBlockData)
{
   TMemoryStream *BlockStream = BlockData.GetStream();
   TMemoryStream *CardStream = inCardBlockData.GetStream();
   // You could double check the card CRC here.
   unsigned short CRC = 0;
   unsigned short CalcCRC = 0;
	BlockData.Read(V4_CARD_CRC_DATA_START,CRC);
	if(!StreamCheckCRC(CRC,CalcCRC,CardStream,V4_CARD_VERSION_DATA_START,CARD_TOTAL_MEMORY_LENGTH - V4_CARD_VERSION_DATA_START - V4_CARD_CRC_DATA_LENGTH))
   {
      throw TSCException(tsceValidateFailed,"Failed to validate Card CRC.");
   }

   for (int i = V4_CARD_VERSION_DATA_START; i < V4_CARD_VERSION_DATA_START + V4_CARD_VERSION_DATA_LENGTH  ; i++)
   {
      if(((BYTE *)BlockStream->Memory)[i] != ((BYTE *)CardStream->Memory)[i])
      {
         throw TSCException(tsceValidateFailed,"Failed to validate Version information.");
      }
   }

   // Compare it to the current block Member Data.
	for (int i = V4_CARD_MEMBER_DATA_START; i < V4_CARD_MEMBER_DATA_START + V4_CARD_MEMBER_DATA_LENGTH  ; i++)
   {
      if(((BYTE *)BlockStream->Memory)[i] != ((BYTE *)CardStream->Memory)[i])
      {
         throw TSCException(tsceValidateFailed,"Failed to validate contact information.");
      }
   }

   // Compare it to the current block Member Data.
   for (int i = V4_CARD_POINTS_DATA_START; i < V4_CARD_POINTS_DATA_START + V4_CARD_POINTS_DATA_LENGTH  ; i++)
   {
      if(((BYTE *)BlockStream->Memory)[i] != ((BYTE *)CardStream->Memory)[i])
      {
         throw TSCException(tsceValidateFailed,"Failed to validate points information.");
      }
   }

   /* Valadation of the GUID is not supported.
      When saving a user to a new blank card (formatted card) the GUID will be
      blank in the Block image and set on the card. */
	for (int i = V4_CARD_GUID_START; i < V4_CARD_GUID_START + V4_CARD_GUID_LENGTH  ; i++)
   {
      if(((BYTE *)BlockStream->Memory)[i] != ((BYTE *)CardStream->Memory)[i])
      {
         throw TSCException(tsceValidateFailed,"Failed to validate GUID information.");
      }
   }
}


void TSmartCardVer4::SetContactInfo(TMMContactInfo inContactInfo)
{
	DirtyContacts = true;
	TContactPoints currentPoints = ContactInfo.Points; // keep the current points unchanged.
    currentPoints.PointsRules = inContactInfo.Points.PointsRules;

	ContactInfo = inContactInfo;
    ContactInfo.Points = currentPoints;

	if(BlockData.Empty())
	{
		ContactInfo.CardCreationDate = Now();
   }
}


TMMContactInfo TSmartCardVer4::GetContactInfo()
{
   ContactInfo.ATRStr = BlockData.CardGuid.GUID();
   return ContactInfo;
}

void TSmartCardVer4::SetVersionInfo()
{
   DirtyVersion = true;
	VersionInfo = SMART_CARD_VERSION_FOUR;
}

void TSmartCardVer4::SetPointsInfo(TContactPoints &inContactPoints)
{
   DirtyPoints = true;
   ContactInfo.Points = inContactPoints;
}

void TSmartCardVer4::SetGUIDInfo(TSmartCardGUID &inGUID)
{
   DirtyGUID = true;
   BlockData.CardGuid.Assign(inGUID);
}

// TODO 1 -o Michael -c Descrespancy : I dont think this is even being called.
void TSmartCardVer4::Upgrade(std::map <int, TSyndCode> SyndCode)
{
	std::auto_ptr<TMemoryStream> ContactStream(new TMemoryStream);
	std::auto_ptr<TMemoryStream> PointsStream(new TMemoryStream);

	std::map <int, TSyndCode> ::iterator ptrSyndCodes = SyndCode.begin();
	for (ptrSyndCodes = SyndCode.begin();ptrSyndCodes != SyndCode.end() ; advance(ptrSyndCodes,1))
	{
		StreamGetContact(*ContactStream.get());
		DecryptInPlace(*ContactStream.get(),ptrSyndCodes->second.DecryptedSyndCode);
        // TODO 1 -o Michael -c Descrespancy : Hardcoded SartCard version number this should be DEFINED?
		ContactInfo.LoadFromStream(3,ContactStream.get());

		StreamGetPoints(*PointsStream.get(), SMART_CARD_VERSION_THREE);
		DecryptInPlace(*PointsStream.get(),ptrSyndCodes->second.DecryptedSyndCode);
		ContactInfo.Points.LoadFromStream(3,PointsStream.get());
	}
	ContactInfo.SaveToStream(ContactStream.get());
	ContactInfo.Points.SaveToStream(PointsStream.get());
	Save();
}

void TSmartCardVer4::Save()
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
		BlockData.BlockInfoWrite(V4_CARD_VERSION_DATA_START,V4_CARD_VERSION_DATA_LENGTH,*VersionStream.get());
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
		BlockData.BlockInfoWrite(V4_CARD_MEMBER_DATA_START,V4_CARD_MEMBER_DATA_LENGTH,*ContactStream.get());
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
		BlockData.BlockInfoWrite(V4_CARD_POINTS_DATA_START,V4_CARD_POINTS_DATA_LENGTH,*PointsStream.get());
      UpdateCRC = true;
   }

   if(DirtyGUID)
   {
      GUIDStream->Clear();
      BlockData.CardGuid.SaveToStream(GUIDStream.get());
		BlockData.BlockInfoWrite(V4_CARD_GUID_START,V4_CARD_GUID_LENGTH,*GUIDStream.get());
   }

   if(UpdateCRC)
   { // CRC.
      unsigned short CRC = 0;
      unsigned short CalcCRC = 0;
		StreamCheckCRC(CRC,CalcCRC,BlockData.GetStream(),V4_CARD_VERSION_DATA_START,CARD_TOTAL_MEMORY_LENGTH - V4_CARD_VERSION_DATA_START - V4_CARD_CRC_DATA_LENGTH);
      StreamWrite(CRCStream.get(),CalcCRC);
		BlockData.BlockInfoWrite(V4_CARD_CRC_DATA_START,V4_CARD_CRC_DATA_LENGTH,*CRCStream.get());
   }
   // Begin Actual Card Writes.

   if(DirtyVersion)
   {
		LONG lReturn = CardInfoWrite(V4_CARD_VERSION_DATA_START,V4_CARD_VERSION_DATA_LENGTH,*VersionStream.get());
      if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
      {
			TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing Version Data Failed. : " + AnsiString(IntToHex(int(lReturn),3)));
         throw TSCException(tsceWriteFailed,"Write Version Info Failed.");
      }
      else
      {
			DirtyVersion = false;
      }
   }

	if(DirtyContacts)
	{
		LONG lReturn = CardInfoWrite(V4_CARD_MEMBER_DATA_START,V4_CARD_MEMBER_DATA_LENGTH,*ContactStream.get());
      if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
      {
			TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing Contact Data Failed. : " + AnsiString(IntToHex(int(lReturn),3)));
         throw TSCException(tsceWriteFailed,"Write Contact Info Failed.");
      }
      else
      {
         DirtyContacts = false;
      }
   }

   if(DirtyPoints)
   {
		LONG lReturn = CardInfoWrite(V4_CARD_POINTS_DATA_START,V4_CARD_POINTS_DATA_LENGTH,*PointsStream.get());
      if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
      {
			TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing Points Data Failed. : " + AnsiString(IntToHex(int(lReturn),3)));
         throw TSCException(tsceWriteFailed,"Write Points Info Failed.");
      }
      else
      {
         DirtyPoints = false;
      }
   }

   if(DirtyGUID)
   {
		LONG lReturn = CardInfoWrite(V4_CARD_GUID_START,V4_CARD_GUID_LENGTH,*GUIDStream.get());
      if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
      {
			TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing GUID Data Failed. : " + AnsiString(IntToHex(int(lReturn),3)));
         throw TSCException(tsceWriteFailed,"Write GUID Info Failed.");
      }
      else
      {
         DirtyGUID = false;
      }
   }

   if(UpdateCRC)
   { // CRC.
		LONG lReturn = CardInfoWrite(V4_CARD_CRC_DATA_START,V4_CARD_CRC_DATA_LENGTH,*CRCStream.get());
      if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
      {
			TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing CRC Data Failed. : " + AnsiString(IntToHex(int(lReturn),3)));
         throw TSCException(tsceWriteFailed,"Write CRC Info Failed.");
      }
   }
}

void TSmartCardVer4::StreamGetContact(TMemoryStream &Stream)
{
   Stream.Clear();
	BlockData.BlockInfoRead(V4_CARD_MEMBER_DATA_START,V4_CARD_MEMBER_DATA_LENGTH,Stream);
   Stream.Position = 0;
}

void TSmartCardVer4::StreamGetPoints(TMemoryStream &Stream, int ver)
{
	Stream.Clear();
	switch(ver)
	{
		case SMART_CARD_VERSION_THREE:
		{
		BlockData.BlockInfoRead(V3_CARD_POINTS_DATA_START,V3_CARD_POINTS_DATA_LENGTH,Stream);
		}
		case SMART_CARD_VERSION_FOUR:
		{
		BlockData.BlockInfoRead(V4_CARD_POINTS_DATA_START,V4_CARD_POINTS_DATA_LENGTH,Stream);
		}
	}
	Stream.Position = 0;
}

void TSmartCardVer4::StreamGetPoints(TMemoryStream &Stream)
{
    StreamGetPoints(Stream, SMART_CARD_VERSION_FOUR);
}

void TSmartCardVer4::Restore(TSmartCardBlock &RestorePoint)
{
   LONG lReturn = SCARD_F_UNKNOWN_ERROR;
   TMemoryStream &RestoreStream = RestorePoint.GetStreamRef();
	lReturn = CardInfoWrite(V4_CARD_RESTORE_POINT_DATA_START,CARD_RESTORE_POINT_LENGTH,RestoreStream);
   RestoreStream.Position = 0;
   #ifdef _DEBUG
   RestoreStream.SaveToFile("MMRestoreToRestorePoint.bin");
   #endif
   if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
   {
		TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Write Restore Point Failed. : " + AnsiString(IntToHex(int(lReturn),3)));
      throw TSCException(tsceWriteFailed,"Write Restore Point Failed.");
   }
}

