//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

//---------------------------------------------------------------------------
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("main.cpp", frmMain);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
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

