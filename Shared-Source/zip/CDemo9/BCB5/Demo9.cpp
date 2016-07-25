//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Demo9.res");
USEFORM("Main.cpp", Form1);
USEUNIT("ZThrd.cpp");
USERC("DemoManifest.rc");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
    Application->Initialize();
    Application->CreateForm(__classid(TForm1), &Form1);
     Application->Run();
  }
  catch (Exception &exception)
  {
    Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------

