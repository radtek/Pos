//---------------------------------------------------------------------------


#pragma hdrstop

#include "casmcard.h"

#include "SmartCardVer0API.h"

#include "blowfish.h"
#include "SmartCardDefs.h"
#include "MMLogging.h"
#include "CrcTable.h"
#include "SmartCardException.h"
 // CRC Message checking.
//---------------------------------------------------------------------------

#pragma package(smart_init)


TSmartCard::TSmartCard(SCARDHANDLE  inhCard,TSmartCardBlock &inBlockData,TSyndCode inSyndCode)
{
   DirtyVersion   = false;
   DirtyContacts  = false;
   DirtyPoints    = false;
   DirtyGUID      = false;
   hCard = inhCard;
   SyndCode = inSyndCode;
   ContactInfo.Clear();
   BlockData.ClearAll();
   BlockData.Assign(inBlockData);
}

// Will destory the data passed into it.
void TSmartCard::EncryptInPlace(TMemoryStream &Data)
{
   // Space must be a multiple of 8 for Blowfish encryption.
   if(Data.Size % 8 != 0)
   {
      int BlockSize = 0;
      if( (Data.Size - (Data.Size % 8)) %8 == 0 )
      {
         BlockSize = 8 - (Data.Size % 8);
      }
      else
      {
         BlockSize = (Data.Size % 8);
      }

      unsigned char *ptrBuffer = new unsigned char[BlockSize];
      ZeroMemory(ptrBuffer,BlockSize);
      Data.Write(ptrBuffer,BlockSize);
      delete []ptrBuffer;
   }

   CBlowFish BF;
   // Have to pass though the NULL on the end of the string because this old format
   // has it inherently ( yea you cant see it but its there ).
   // BYTE Passwd[] = {"0123456789abcdefghijklmnopqrstuvwxyz0123456789"};
   // BF.Initialize(Passwd,sizeof(Passwd));
   BF.Initialize(SyndCode.DecryptedSyndCode.c_str(),SyndCode.DecryptedSyndCode.Length()+1);
   BF.Encode((BYTE *)Data.Memory,(BYTE *)Data.Memory,Data.Size);
}

// Will destory the data passed into it.
void TSmartCard::DecryptInPlace(TMemoryStream &Data,AnsiString SyndicateCode)
{
   CBlowFish BF;
   // Have to pass though the NULL on the end of the string because this old format
   // has it inherently ( yea you cant see it but its there ).
   // BYTE Passwd[] = {"0123456789abcdefghijklmnopqrstuvwxyz0123456789"};
   // BF.Initialize(Passwd,sizeof(Passwd));
   BF.Initialize(SyndicateCode.c_str(),SyndicateCode.Length()+1);
   BF.Decode((BYTE *)Data.Memory,(BYTE *)Data.Memory,Data.Size);
}

void TSmartCard::ReassignHandle(SCARDHANDLE inhCard)
{
   hCard = inhCard;
}

LONG TSmartCard::CardInfoWrite(int BlockStart, int MaxBlockLength, TMemoryStream &Stream)
{
   #ifdef _DEBUG
	__int64 start = ::GetTickCount();
   #endif

   if(Stream.Size > MaxBlockLength) // Inclusive of Block Start Byte so +1.
   {
      throw TSCException(tsceToMuchCardData,"Too Much Data to fit on Memory Card");
   }

   LONG lReturn = SCARD_S_SUCCESS;

   CasBeginTransaction(hCard);
   lReturn = SLE4442_Update_Main_MemoryA(hCard,(unsigned char *)Stream.Memory,BlockStart,Stream.Size);
   CasEndTransaction(hCard,SCARD_LEAVE_CARD);

   if(lReturn != SCARD_S_SUCCESS)
   {
      TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,"Writing Data Failed. : " + AnsiString(IntToHex(int(lReturn),2)));
   }

   #ifdef _DEBUG
   __int64 Time = ((::GetTickCount() - start));
   TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG,"--------------------------------");
   TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG,"SmartCard Write Time :" + IntToStr(Time));
   #endif

   return lReturn;
}

bool TSmartCard::StreamCheckCRC(unsigned short &CRC,unsigned short &CalcCRC,TMemoryStream *Stream, int Begin, int End)
{
   	if(End == 0)
	{
		End = Stream->Size;
	}

	bool RetVal = false;
	unsigned char *ptrData = &((unsigned char *)Stream->Memory)[Begin];
	CalcCRC = (unsigned short) StartCRC(ptrData[0]);
	for(int i=1 ; i < End; i++)
	{
		CalcCRC = (unsigned short)UpdateCRC(CalcCRC,ptrData[i]);
	}

	if(CalcCRC == CRC)
	{
		RetVal = true;
	}
	return RetVal;
}

void TSmartCard::ReloadCardDetails()
{
    TPointsRulesSet pointsRules = ContactInfo.Points.PointsRules;
    ContactInfo.Points.Clear();
    std::auto_ptr<TMemoryStream> PointsStream(new TMemoryStream);
    StreamGetPoints(*PointsStream.get());

    DecryptInPlace(*PointsStream.get(),SyndCode.DecryptedSyndCode);
    ContactInfo.Points.LoadFromStream(BlockData.Version,PointsStream.get());

    ContactInfo.Points.PointsRules = pointsRules;
}
