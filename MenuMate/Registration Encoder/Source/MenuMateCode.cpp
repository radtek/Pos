//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("main.cpp", frmMain);
USEFORM("Password.cpp", frmPassword);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->Title = "IQWorks Code Generator";
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TfrmPassword), &frmPassword);
		Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------


