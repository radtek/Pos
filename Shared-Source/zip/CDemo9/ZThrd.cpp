//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZThrd.h"
#include "Main.h"
#include "ZBCompat.h"
#pragma link "ZipBuilder"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
__fastcall TZipThread::TZipThread(String Filename, TStrings* Specs, TMemo* Memo,
                                  bool suspended) : TThread(suspended)
{
  FName = Filename;
  FSpecs = Specs;
  FMemo = Memo;
  zip = new TZipBuilder(NULL); // no owner, so must free
  OnTerminate = Finished;
}
//---------------------------------------------------------------------------
void __fastcall TZipThread::Execute()
{
  int i =0;
  while(zip->Busy &&  i < 100)
  {
    ++i;
    Sleep(10);
  }
  zip->Add();
}
//---------------------------------------------------------------------------
// must run main thread ie via Synchronize
void __fastcall TZipThread::ShowText(void)
{
  FMemo->Lines->Add(FText);
}
//---------------------------------------------------------------------------
void __fastcall TZipThread::ZipMessage(TObject* /*sender*/, int ErrCode, String Message)
{
  if(ErrCode && (ErrCode != 767))  // ignore skipped
  {
    FText = "(" + IntToStr(ErrCode) + ") " + Message;
    Synchronize(&ShowText);     // in main thread add string to memo
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
void __fastcall TZipThread::CheckTerminate(TObject* /*Sender*/, bool& Stop)
{
  if(Terminated) Stop = true;
}
//---------------------------------------------------------------------------
void __fastcall TZipThread::AfterConstruction(void)
{
  zip->OnCheckTerminate = CheckTerminate;
  zip->OnMessage = ZipMessage;
  zip->Unattended = true;
  zip->ZipFileName = FName;
  zip->FSpecArgs->Assign(FSpecs);
  zip->DLLDirectory = "..\\ZipDLL179\\DLL";
  TThread::AfterConstruction();
}
//---------------------------------------------------------------------------
// runs main thread
void __fastcall TZipThread::Finished(TObject* /*sender*/)
{
  Form1->ZipThread = NULL;   // don't allow Cancel
  FText = "Added " + IntToStr(zip->SuccessCnt) + " files";
  Synchronize(&ShowText);     // in main thread add string to memo
  if(zip->ErrCode)
  {
    FText = "Error " + IntToStr(zip->ErrCode) + "  " + zip->Message;
    Synchronize(&ShowText);     // in main thread add string to memo
  }
  delete zip;
}

