//---------------------------------------------------------------------------

#ifndef SmartCardBlockH
#define SmartCardBlockH
//---------------------------------------------------------------------------

#include "SmartCardGUID.h"

#include <memory>

class TSmartCardBlockInfo
{
   public:
   TSmartCardBlockInfo() : CorruptCardCRC(false), CorruptContactInfo(false),CorruptPointsInfo(false) {}
   bool CorruptCardCRC;
   bool CorruptContactInfo;
   bool CorruptPointsInfo;
};

class TSmartCardBlock
{
   private:
      std::auto_ptr<TMemoryStream> Stream;
   public:
      int Version;
      TSmartCardGUID CardGuid;

      void BlockInfoRead(int BlockStart, int BlockLength,TMemoryStream &inStream);
      void BlockInfoWrite(int BlockStart, int MaxBlockLength,TMemoryStream &inStream);

      TSmartCardBlock();
      TSmartCardBlock(TSmartCardBlock &inSmartCardBlock);
      void Assign(TSmartCardBlock &inSmartCardBlock);
      void SaveToFile(AnsiString FileName);
      void LoadFromFile(AnsiString FileName);

      TMemoryStream *GetStream();
      TMemoryStream &GetStreamRef();
      TSmartCardGUID *GetGUID();

      void ClearData();
      void ClearAll();
      bool Empty();
      void LoadGUID();

      void Read(int Pos,unsigned short &Value);
};

#endif
