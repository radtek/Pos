//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("VersDemo.res");
USEFORM("Main.cpp", MainForm);
USERC("DemoManifest.rc");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
    Application->Initialize();
    Application->CreateForm(__classid(TMainForm), &MainForm);
     Application->Run();
  }
  catch (Exception &exception)
  {
    Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------

