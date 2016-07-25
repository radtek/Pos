//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#ifdef madExcept


#endif
//---------------------------------------------------------------------------
USEFORM("main.cpp", frmMain);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
      Application->Initialize();
      Application->Title = "MenuMate Parser";
      Application->CreateForm(__classid(TfrmMain), &frmMain);
      Application->Run();
   }
   catch (Exception &exception)
   {
      Application->ShowException(&exception);
   }
   catch (...)
   {
      try
      {
         throw Exception("");
      }
      catch (Exception &exception)
      {
         Application->ShowException(&exception);
      }
   }
   return 0;
}
//---------------------------------------------------------------------------

