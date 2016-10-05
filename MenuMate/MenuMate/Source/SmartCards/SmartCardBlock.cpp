//---------------------------------------------------------------------------


#pragma hdrstop

#include "SmartCardBlock.h"

#include "SmartCardVer0API.h"
#include "SmartCardDefs.h"
#include "Stream.h"
#include "SmartCardException.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TSmartCardBlock::TSmartCardBlock() : Stream(new TMemoryStream())
{
   ClearAll();
}

TSmartCardBlock::TSmartCardBlock(TSmartCardBlock &inSmartCardBlock) : Stream(new TMemoryStream())
{
   ClearAll();
   Assign(inSmartCardBlock);
}

void TSmartCardBlock::ClearData()
{
    Version = SMART_CARD_VERSION_UNK;
    std::auto_ptr<TMemoryStream> ClearStream(new TMemoryStream);
    int BlankBlockSize = CARD_TOTAL_MEMORY_LENGTH - (V4_CARD_GUID_START + V4_CARD_GUID_LENGTH);
    std::vector<BYTE> recvbuffer(BlankBlockSize,0xFF);
    ClearStream->Write(&recvbuffer[0],recvbuffer.size());
    BlockInfoWrite(V4_CARD_GUID_START + V4_CARD_GUID_LENGTH,BlankBlockSize,*ClearStream.get());
}

void TSmartCardBlock::ClearAll()
{
   CardGuid.Clear();
   Version = SMART_CARD_VERSION_UNK;
   std::auto_ptr<TMemoryStream> ClearStream(new TMemoryStream);
   std::vector<BYTE> recvbuffer(CARD_TOTAL_MEMORY_LENGTH,0xFF);
   ClearStream->Write(&recvbuffer[0],recvbuffer.size());
   BlockInfoWrite(0,CARD_TOTAL_MEMORY_LENGTH,*ClearStream.get());
   Stream->Position = 0;
}

bool TSmartCardBlock::Empty()
{
   bool RetVal = false;
   if(Stream->Size == 0)
   {
      RetVal = true;
   }
   else
   {
      RetVal = true;
		for(int i= V3_CARD_VERSION_DATA_START ; i < Stream->Size && RetVal ; i++)
      {
         BYTE myine = ((BYTE *)Stream->Memory)[i];
         if(! ( (myine == (BYTE)0x00) || (myine == (BYTE)0xFF) ) )
         {
            RetVal = false;
         }
      }
   }
   return RetVal;
}

void TSmartCardBlock::SaveToFile(AnsiString FileName)
{
   Stream->SaveToFile(FileName);
}

void TSmartCardBlock::LoadFromFile(AnsiString FileName)
{
   Stream->Clear();
   Stream->LoadFromFile(FileName);
   Stream->Position = 0;
   LoadGUID();
}

TMemoryStream * TSmartCardBlock::GetStream()
{
   return Stream.get();
}

TMemoryStream &TSmartCardBlock::GetStreamRef()
{
   return *Stream.get();
}

void TSmartCardBlock::Assign(TSmartCardBlock &inSmartCardBlock)
{
   CardGuid = inSmartCardBlock.CardGuid;
   Version = inSmartCardBlock.Version;
   BlockInfoWrite(0,CARD_TOTAL_MEMORY_LENGTH,inSmartCardBlock.GetStreamRef());
}

void TSmartCardBlock::LoadGUID()
{
   std::auto_ptr<TMemoryStream> GUIDStream(new TMemoryStream);
	BlockInfoRead(V4_CARD_GUID_START,V4_CARD_GUID_LENGTH,*GUIDStream.get());
   GUIDStream->Position = 0;
   CardGuid.Assign(GUIDStream.get());
   #ifdef _DEBUG
   GUIDStream->SaveToFile("MMCardGUID.bin");
   #endif
}

TSmartCardGUID *TSmartCardBlock::GetGUID()
{
   return &CardGuid;
}

void TSmartCardBlock::BlockInfoRead(int BlockStart, int BlockLength,TMemoryStream &inStream)
{
   if(Stream->Size < BlockStart)
   {
      throw TSCException(tsceReadFailed,"Read block card data failed, Position Error.");
   }
   else
   {
      Stream->Position = BlockStart;
      if(Stream->Size < BlockStart + BlockLength)
      {
         throw TSCException(tsceReadFailed,"Read block card data failed, Overrun Error.");
      }
      else
      {
         inStream.CopyFrom(Stream.get(),BlockLength);
      }
   }
}

void TSmartCardBlock::BlockInfoWrite(int BlockStart, int MaxBlockLength,TMemoryStream &inStream)
{
	if(CARD_TOTAL_MEMORY_LENGTH < BlockStart)
	{
		throw TSCException(tsceWriteFailed,"Read block card data failed, Position Error.");
	}
	else
	{
		Stream->Position = BlockStart;
		if(CARD_TOTAL_MEMORY_LENGTH < BlockStart + MaxBlockLength)
		{
			throw TSCException(tsceWriteFailed,"Write block card data failed, Overrun Error.");
		}
		else if(inStream.Size > MaxBlockLength) // Inclusive of Block Start Byte so +1.
		{
			throw TSCException(tsceToMuchCardData,"Too Much Data to fit in Memory Block");
		}
		else
		{
			Stream->CopyFrom(&inStream,0);
			inStream.Position = 0;
		}
	}
	#ifdef _DEBUG
	SaveToFile("MMBlockInfo.bin");
	#endif
}

void TSmartCardBlock::Read(int Pos,unsigned short &Value)
{
   if(Stream->Size < Pos + sizeof(Value))
   {
      throw TSCException(tsceReadFailed,"Read block card data failed, Overrun Error.");
   }
   else
   {
      unsigned short Temp;
      Stream->Position = Pos;
      StreamRead(Stream.get(),Temp);
      Value = Temp;
   }
}