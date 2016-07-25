//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("Message.cpp", MsgForm);
USEFORM("Main.cpp", MainForm);
USEFORM("Rename.cpp", RenForm);
USEFORM("ExtractFiles.cpp", Extract);
USEFORM("AddFiles.cpp", AddFile);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TMainForm), &MainForm);
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
