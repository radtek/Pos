//---------------------------------------------------------------------------

#ifndef ZThrdH
#define ZThrdH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "ZipBuilder.h"
//---------------------------------------------------------------------------
class TZipThread : public TThread
{
private:
  TZipBuilder* zip;
  AnsiString FText;
  TStrings* FSpecs;
  AnsiString FName;
  TMemo* FMemo;
protected:
  void __fastcall Execute();
  void __fastcall ShowText(void);
  void __fastcall ZipMessage(TObject* Sender, int ErrCode, AnsiString Message);
  void __fastcall CheckTerminate(TObject* Sender, bool& Stop);
  void __fastcall Finished(TObject* Sender);
public:
  __fastcall TZipThread(AnsiString Filename, TStrings* Specs, TMemo* Memo, bool suspended);
  virtual void __fastcall AfterConstruction(void);
};
//---------------------------------------------------------------------------
#endif

