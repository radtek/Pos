//---------------------------------------------------------------------------

#ifndef ZThrdH
#define ZThrdH
#include "ZipBuilder.h"
//---------------------------------------------------------------------------
class __declspec(delphiclass) TZipThread : public Classes::TThread    
{
private:
  TZipBuilder* zip;
  AnsiString FText;
  TStrings* FSpecs;
  AnsiString FName;
  TMemo* FMemo;
protected:
  void __fastcall Execute(void);
  void __fastcall ShowText(void);
  void __fastcall ZipMessage(TObject* Sender, int ErrCode, AnsiString Message);
  void __fastcall CheckTerminate(TObject* Sender, bool& Stop);
  void __fastcall Finished(TObject* Sender);
public:
  __fastcall TZipThread(AnsiString Filename, TStrings* Specs, TMemo* Memo, bool suspended);
  __fastcall ~TZipThread(void) {};
  virtual void __fastcall AfterConstruction(void);
}
#endif

