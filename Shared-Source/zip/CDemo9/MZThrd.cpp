//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZThrd.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
/* Important: Methods and properties of objects in VCL or CLX can only be used
  in a method called using Synchronize, for example,

      Synchronize(UpdateCaption);

  and UpdateCaption could look like,

    procedure TZipThread.UpdateCaption;
    begin
      Form1.Caption := 'Updated in a thread';
    end;
*/

//---------------------------------------------------------------------------
// runs own thread
void __fastcall TZipThread::Execute(void)
{
  zip->Add();
}
//---------------------------------------------------------------------------
// must run main thread ie via Synchronize
void __fastcall TZipThread::ShowText(void)
{
  FMemeo->Lines->Add(FText);
}
//---------------------------------------------------------------------------
void __fastcall TZipThread::ZipMessage(TObject* sender, int ErrCode, AnsiString Message)
{
  if(ErrCode && ErCode != 767)  // ignore skipped
  {
    FText = "(" + IntToStr(Errocode) + ") " + Message;
    Synchronize(ShowText);     // in main thread add string to memo
  }
  /*
  else // showing all message will slow it down
  {
    FText = Message;
    Synchonize(ShowText);  // in  main thread add string to memo
  }
  */
}
//---------------------------------------------------------------------------
// stops calls to ProcessMessages & checks for Terminated
void __fastcall TZipThread::CheckTerminate(TObject* Sender, bool& Stop)
{
  if(Terminated) stop = true;
}
//---------------------------------------------------------------------------
// constructor
__fastcall TZipThread::TZipThread(AnsiString Filename, TStrings* Specs, TMemo* Memo,
                                  bool suspended) : TThread(suspended);
{
  FName = FileName;
  FSpecs = Specs;
  FMemo = Memo;
  zip = new TZipBuilder(NULL); // no owner, so must free
  OnTerminate = Finished;
}
//---------------------------------------------------------------------------
void __fastcall TZipThread::AfterConstruction(void)
{
  zip->CheckTerminate = CheckTerminate;
  zip->OnMessage = ZipMessage;
  zip->Unattended = true;
  zip->ZipFileName = FName;
  zip->FSpecArgs->Assign(FSpecs);
//zip->DLLDirectory = ".\..\DLL");
}
//---------------------------------------------------------------------------
// runs main thread
void __fastcall TZipThread::Finished(TObject* sender)
{
  Form1->ZipThread = NULL;   // don't allow Cancel
  FMemo->Lines->Add("Added " + IntToStr(zip->SuccesCnt) + " files");
  if(zip->ErrCode)
    FMemo->LInes->Add("Error " + IntToStr(zip->ErrCode) + "  " + Zip->Message);
}
