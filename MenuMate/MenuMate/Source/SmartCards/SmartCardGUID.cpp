//---------------------------------------------------------------------------


#pragma hdrstop

#include "SmartCardGUID.h"
#include "Stream.h"
#include <vector>
//---------------------------------------------------------------------------

#pragma package(smart_init)


void TSmartCardGUID::Assign(TMemoryStream *GUIDStream)
{
   Clear();
   GUIDStream->Position = 0;
   StreamRead(GUIDStream,SiteID);
   StreamRead(GUIDStream,MemberNumber);
   StreamRead(GUIDStream,CardNumber);
   GUIDStream->Position = 0;
}

void TSmartCardGUID::SaveToStream(TMemoryStream *GUIDStream)
{
   StreamWrite(GUIDStream,SiteID);
   StreamWrite(GUIDStream,MemberNumber);
   StreamWrite(GUIDStream,CardNumber);
}

void TSmartCardGUID::Assign(const TSmartCardGUID &inSmartCardGUID)
{
   Clear();
   inSmartCardGUID.Get(SiteID,MemberNumber,CardNumber);
}

void TSmartCardGUID::Assign(const TSmartCardGUID *inSmartCardGUID)
{
   Clear();
   inSmartCardGUID->Get(SiteID,MemberNumber,CardNumber);
}


void TSmartCardGUID::Assign(int inSiteID, int inMemberNumber, int inCardNumber)
{
   Clear();
   SiteID         = inSiteID;
   MemberNumber = inMemberNumber;
   CardNumber   = inCardNumber;
}

void TSmartCardGUID::Get(int &outSiteID, int &outMemberNumber, int &outCardNumber) const
{
   outSiteID          = SiteID;
   outMemberNumber    = MemberNumber;
   outCardNumber      = CardNumber;
}

TSmartCardGUID::TSmartCardGUID()
{
   Clear();
}

void TSmartCardGUID::Clear()
{
    SiteID = 0xFFFFFFFF;
    MemberNumber = 0xFFFFFFFF;
    CardNumber = 0xFFFFFFFF;
}

bool TSmartCardGUID::Valid()
{
   return ! ((SiteID == 0xFFFFFFFF) || (MemberNumber == 0xFFFFFFFF) || (CardNumber == 0xFFFFFFFF) ||
             (SiteID == 0x00000000) || (MemberNumber == 0x00000000) || (CardNumber == 0x00000000)
            );
}

AnsiString TSmartCardGUID::GUID()
{
   std::auto_ptr<TMemoryStream> GUIDStream(new TMemoryStream);
   StreamWrite(GUIDStream.get(),SiteID);
   StreamWrite(GUIDStream.get(),MemberNumber);
   StreamWrite(GUIDStream.get(),CardNumber);

	std::vector<char> recvbuffer((GUIDStream->Size*2),0xFF);
	BinToHex((void *)GUIDStream->Memory, &recvbuffer[0], GUIDStream->Size);
	return AnsiString((char *)(&recvbuffer[0]),recvbuffer.size());
}

AnsiString TSmartCardGUID::GUIDBackups()
{
   std::auto_ptr<TMemoryStream> GUIDStream(new TMemoryStream);
   StreamWrite(GUIDStream.get(),SiteID);
   StreamWrite(GUIDStream.get(),MemberNumber);
   StreamWrite(GUIDStream.get(),CardNumber);

	std::vector<char> recvbuffer((GUIDStream->Size*2),0xFF);
	BinToHex((void *)GUIDStream->Memory, &recvbuffer[0], GUIDStream->Size);
    AnsiString BackupGUID = AnsiString((char *)(&recvbuffer[0]),recvbuffer.size());
	BackupGUID = BackupGUID + Now().FormatString("_yyyy-mmm-dd hh-nn-ss");
	return BackupGUID;
}

bool TSmartCardGUID::Compare(AnsiString inGUID)
{
   bool RetVal = false;
   AnsiString Temp = GUID();
   // I have no idea why Temp == inGUID dosnt work but it dosnt.
   if(strcmp(Temp.c_str(),inGUID.c_str()) == 0)
   {
      RetVal = true;
   }
   return RetVal;
}