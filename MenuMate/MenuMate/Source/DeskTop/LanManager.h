//---------------------------------------------------------------------------

#ifndef LanManagerH
#define LanManagerH

#include <memory>
#include "VerticalSelect.h"
#include "OnlineManager.h"
//---------------------------------------------------------------------------

class TLanManager
{
   private:
      bool GetCurrentDir(TfrmVerticalSelect *SelectionForm);
      bool GetFile(AnsiString File);      
      TftpControl SelectDirOrFile();
      void ChangeDir(AnsiString Dir);
      AnsiString CurrentDir;
   void List(TIdFTPListItems * DirectoryListing);
   public :
   TLanManager(AnsiString inCurrentDir);
   virtual ~TLanManager();
   AnsiString GetUpdateFile();


   TftpControl SelectionType;
   AnsiString Selection;
   AnsiString SelectionFileSize;
};
#endif
