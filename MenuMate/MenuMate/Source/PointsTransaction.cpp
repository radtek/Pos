//---------------------------------------------------------------------------


#pragma hdrstop

#include "PointsTransaction.h"
#include "Stream.h"
#include "Membership.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
TPointsTransaction::TPointsTransaction() : InternalStream(new TMemoryStream())
{
   PointsTransactionsKey      = 0;
   ContactKey                 = 0;
   TimeStamp                  = 0;
   TimeStampExported          = 0;
   PointsTransactionType      	= pttInvalid;
   PointsTransactionAccountType = ptstInvalid;
   Adjustment                 = 0;
   ExportStatus               = pesNone;
   InvoiceNumber              = "";
   InternalStream->Clear();
}

TPointsTransaction::TPointsTransaction(const TPointsTransaction &PointsTransaction) : InternalStream(new TMemoryStream())
{
   PointsTransactionsKey      = PointsTransaction.PointsTransactionsKey;
   ContactKey                 = PointsTransaction.ContactKey;
   TimeStamp                  = PointsTransaction.TimeStamp;
   PointsTransactionType      = PointsTransaction.PointsTransactionType;
   PointsTransactionAccountType   = PointsTransaction.PointsTransactionAccountType;
   Adjustment                 = PointsTransaction.Adjustment;
   ExportStatus               = PointsTransaction.ExportStatus;
   TimeStampExported          = PointsTransaction.TimeStampExported;
   InvoiceNumber              = PointsTransaction.InvoiceNumber;
   InternalStream->Clear();
   InternalStream->CopyFrom(PointsTransaction.GetStream(),0);
}

void TPointsTransaction::operator = (TPointsTransaction rhs)
{
   PointsTransactionsKey      = rhs.PointsTransactionsKey;
   ContactKey                 = rhs.ContactKey;
   TimeStamp                  = rhs.TimeStamp;
   PointsTransactionType      = rhs.PointsTransactionType;
   PointsTransactionAccountType   = rhs.PointsTransactionAccountType;
   Adjustment                 = rhs.Adjustment;
   ExportStatus               = rhs.ExportStatus;
   TimeStampExported          = rhs.TimeStampExported;
   InvoiceNumber              = rhs.InvoiceNumber;
   InternalStream->Clear();
   InternalStream->CopyFrom(rhs.GetStream(),0);
}

TPointsTransaction::Build(int inContactKey,
						  std::pair<TPointsType, Currency> Detials,
                          TPointsExportStatus inExportStatus,
                          UnicodeString InvoiceNo,
                          TDateTime inTimeStamp,
                          TDateTime inTimeStampExported,
                          bool isRefundingItems )
{
   ContactKey                 = inContactKey;
   TimeStamp                  = inTimeStamp;
   TimeStampExported          = inTimeStampExported;
   PointsTransactionType      = Detials.first.AccType.first;
   PointsTransactionAccountType   = Detials.first.AccType.second;
   ExportStatus               = inExportStatus;
   ////MM-5999
   if(isRefundingItems)
   {
        Adjustment  = - Detials.second;
   }
   else
   {
        Adjustment  = Detials.second;
    }
   InvoiceNumber              = InvoiceNo;
   InternalStream->Clear();
   SaveToStream(InternalStream.get());
}

TMemoryStream * TPointsTransaction::GetAsStream()
{
   InternalStream->Clear();
   SaveToStream(InternalStream.get());
   EncryptStream(InternalStream.get());
   return InternalStream.get();
}

TMemoryStream * TPointsTransaction::GetStream() const
{
   return InternalStream.get();
}

TMemoryStream &TPointsTransaction::GetStreamRef()
{
   return *InternalStream.get();
}

void TPointsTransaction::EncryptStream(TMemoryStream *Data)
{
// Space must be a multiple of 8 for Blowfish encryption->
   if(Data->Size % 8 != 0)
   {
      int BlockSize = 0;
      if( (Data->Size - (Data->Size % 8)) %8 == 0 )
      {
         BlockSize = 8 - (Data->Size % 8);
      }
      else
	  {
         BlockSize = (Data->Size % 8);
      }

      unsigned char *ptrBuffer = new unsigned char[BlockSize];
      ZeroMemory(ptrBuffer,BlockSize);
      Data->Write(ptrBuffer,BlockSize);
      delete []ptrBuffer;
   }

   CBlowFish BF;
   AnsiString Encypt = "JohnThreeSixteenForGodSoLovedTheWorld";
   BF.Initialize(Encypt.c_str(),Encypt.Length());
   BF.Encode((BYTE *)Data->Memory,(BYTE *)Data->Memory,Data->Size);
}


void TPointsTransaction::DecryptStream(TMemoryStream *Data)
{
   CBlowFish BF;
   AnsiString Encypt = "JohnThreeSixteenForGodSoLovedTheWorld";
   BF.Initialize(Encypt.c_str(),Encypt.Length());
   BF.Decode((BYTE *)Data->Memory,(BYTE *)Data->Memory,Data->Size);
}


bool TPointsTransaction::Validate()
{
   bool RetVal = true;

   std::auto_ptr<TMemoryStream> CheckStream (new TMemoryStream);
   SaveToStream(CheckStream.get());
   EncryptStream(CheckStream.get());

   if(CheckStream->Size == InternalStream->Size)
   {
      for (int i = 0; i < CheckStream->Size && RetVal; i++)
      {
         if(((BYTE *)CheckStream->Memory)[i] != ((BYTE *)InternalStream->Memory)[i])
         {
            RetVal = false;
         }
	  }
   }
   else
   {
      RetVal = false;
   }

   return RetVal;
}

void TPointsTransaction::SaveToStream(TMemoryStream *Stream)
{
	StreamWrite(Stream,ContactKey);
	//   TimeStamps are not accurate enough to base an encryption stream on.
	//   Saveing them into the DB there value chages slightly.
	//   StreamWrite(Stream,TimeStamp);
	//   StreamWrite(Stream,TimeStampExported);
	int Temp = PointsTransactionType;
	StreamWrite(Stream,Temp);
	Temp = PointsTransactionAccountType;
	StreamWrite(Stream,Temp);
	StreamWrite(Stream,Adjustment);
	Temp = ExportStatus;
	StreamWrite(Stream,Temp);
	StreamWrite(Stream,InvoiceNumber);
}
