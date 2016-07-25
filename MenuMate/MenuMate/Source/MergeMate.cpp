//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("MergeMate.res");
USEFORM("MergeMate\MergeMain.cpp", frmMergeMain);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
      Application->Initialize();
      Application->Title = "MergeMate";
      Application->CreateForm(__classid(TfrmMergeMain), &frmMergeMain);
      Application->Run();
   }
   catch (Exception &exception)
   {
      Application->ShowException(&exception);
   }
   return 0;
}
//---------------------------------------------------------------------------
