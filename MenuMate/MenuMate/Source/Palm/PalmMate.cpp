// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Palm.h"
#include "enum.h"
#include "Printing.h"
#include "Comms.h"
#include "palmmain.h"

USEFORM("..\Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("PalmSetup.cpp", frmPalmSetup);
USEFORM("palmmain.cpp", frmMain);
USEFORM("..\Locations.cpp", frmLocations);
USEFORM("..\..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("..\..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("PalmRegistration.cpp", frmPalmRegistration);
USEFORM("..\pause.cpp", frmPause);
USEFORM("..\DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("..\CardSwipe.cpp", frmCardSwipe);
USEFORM("..\IBLogs\LogsDB.cpp", LogsIB); /* TDataModule: File Type */
USEFORM("..\..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("Debugfrm.cpp", frmDebug);
USEFORM("..\DbMod.cpp", frmDBMod);
USEFORM("..\Processing.cpp", frmProcessing);
USEFORM("..\Security\Login.cpp", frmLogin);
USEFORM("..\MMdata.cpp", frmData);
USEFORM("SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("..\SystemConfig.cpp", frmSystemConfig);
//---------------------------------------------------------------------------
HANDLE Mutex;
TDeviceRealPalms *ThisTerminal;

bool AlreadyRunning()
{
   HWND PrevWnd;

   Mutex = CreateMutex(NULL, false, _T("PalmMateUniqueName"));
   if ((Mutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
   {
	  PrevWnd = FindWindow(_T("TfrmMain"), _T("PalmMate"));
	  if (PrevWnd != NULL)
	  {
		 PrevWnd = GetWindow(PrevWnd, GW_OWNER);
	  }
	  if (PrevWnd != NULL)
	  {
		 if (IsIconic(PrevWnd))
		 {
			ShowWindow(PrevWnd, SW_RESTORE);
		 }
		 else
		 {
			SetForegroundWindow(PrevWnd);
		 }
	  }
	  CloseHandle(Mutex);
	  return true;
   }
   return false;
}

// ---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
   try
   {
	  try
	  {
		 if (AlreadyRunning() == false)
		 {
			ThisTerminal = new TDeviceRealPalms;
			Kitchen = new TKitchen;
			Receipt = new TReceipt;
			// Create Palm holder Object.
			Application->Initialize();
			Application->Title = "PalmMate";
			Application->CreateForm(__classid(TfrmMain), &frmMain);
         Application->CreateForm(__classid(TfrmData), &frmData);
         Application->CreateForm(__classid(TfrmPause), &frmPause);
         Application->CreateForm(__classid(TfrmDBMod), &frmDBMod);
         Application->Run();

			// Free the Global Objects.
			delete Kitchen;
			Kitchen = NULL;
			delete Receipt;
			Receipt = NULL;
			delete ThisTerminal;
			ThisTerminal = NULL;
		 }
	  }
	  __finally
	  {
		 ReleaseMutex(Mutex);
		 CloseHandle(Mutex);
	  }
   }
   catch(Exception & exception)
   {
	  Application->ShowException(&exception);
   }
   return 0;
}
// ---------------------------------------------------------------------------
