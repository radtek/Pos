//---------------------------------------------------------------------------

#include <vcl.h>
#include <process.h>

#pragma hdrstop
USEFORM("DeskMain.cpp", frmMain);
USEFORM("..\..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("..\..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("..\..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("..\ReportDisplay.cpp", frmReportDisplay);
USEFORM("SystemConfigDeskMate.cpp", frmSystemConfig);
USEFORM("VerticalSelect.cpp", frmVerticalSelect);
USEFORM("..\Processing.cpp", frmProcessing);
USEFORM("ftp.cpp", frmFTP);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   bool SpawnApp = false;
   int ParamCnt = ParamCount();
#ifndef _DEBUG
   if(ParamCnt == 0)
   {
      AnsiString Source = ParamStr(0);
      AnsiString Dest = ExtractFilePath(Source) + "DskMte.exe";
      CopyFile(Source.c_str(),Dest.c_str(),false);
      spawnl(
            P_NOWAIT,
            Dest.c_str(),
            Dest.c_str(),
            "RunApp",
            NULL);
   }
   else
#endif
   {
      try
      {
         Application->Initialize();
         Application->Title = "DeskMate";
         Application->CreateForm(__classid(TfrmMain), &frmMain);
                 Application->Run();
      }
      catch (Exception &exception)
      {
         Application->ShowException(&exception);
      }
   }
   return 0;
}
//---------------------------------------------------------------------------
