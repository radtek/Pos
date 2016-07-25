//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Demo1.res");
USEFORM("Main.cpp", MainForm);
USEFORM("SFX.cpp", MakeSFX);
USEFORM("AddFiles.cpp", AddFile);
USEFORM("ExtractFiles.cpp", Extract);
USEFORM("Message.cpp", MsgForm);
USEFORM("Rename.cpp", RenForm);
USERC("DemoManifest.rc");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->CreateForm(__classid(TMainForm), &MainForm);
     Application->CreateForm(__classid(TMakeSFX), &MakeSFX);
     Application->CreateForm(__classid(TAddFile), &AddFile);
     Application->CreateForm(__classid(TExtract), &Extract);
     Application->CreateForm(__classid(TMsgForm), &MsgForm);
     Application->CreateForm(__classid(TRenForm), &RenForm);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
