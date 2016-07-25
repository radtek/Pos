//---------------------------------------------------------------------------

#ifndef SmartCardGUIDH
#define SmartCardGUIDH
//---------------------------------------------------------------------------
#include <Classes.hpp>

class TSmartCardGUID
{
   private:
      int SiteID;
      int MemberNumber;
      int CardNumber;
   public:
   TSmartCardGUID();
   void Assign(TMemoryStream *GUIDStream);
   void SaveToStream(TMemoryStream *inStream);   
   void Assign(int inSiteID, int inMemberNumber, int inCardNumber);
   void Assign(const TSmartCardGUID &inSmartCardGUID);
   void Assign(const TSmartCardGUID *inSmartCardGUID);
   void Get(int &outSiteID, int &outMemberNumber, int &outCardNumber) const;
   void Clear();
   AnsiString GUID();
   AnsiString GUIDBackups();
   bool Compare(AnsiString inGUID);
   bool Valid();
};

#endif
