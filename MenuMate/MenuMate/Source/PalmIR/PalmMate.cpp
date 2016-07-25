//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "data.h"
#include "enum.h"
#include "Printing.h"
#include "Comms.h"
#include "Palm.h"
#include "palmmain.h"

USEFORM("palmmain.cpp", frmMain);
USEFORM("PrinterConfig.cpp", frmPrinterConfiguration);
USEFORM("SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("PalmRegistration.cpp", frmPalmRegistration);
USEFORM("PalmSetup.cpp", frmPalmSetup);
USEFORM("..\Data.cpp", frmData);
USEFORM("..\pause.cpp", frmPause);
USEFORM("..\Processing.cpp", frmProcessing);
USEFORM("..\POS\MMMessageBox.cpp", frmMessageBox);
USEFORM("..\TCP.cpp", frmTCP);
USEFORM("..\POS\TouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("..\IBLogs\IBLogs.cpp", LogsIB); /* TDataModule: File Type */
USEFORM("..\Security\Login.cpp", frmLogin);
USEFORM("..\Security\Secure.cpp", frmSecurity);
USEFORM("..\DbMod.cpp", frmDBMod);
USEFORM("..\Locations.cpp", frmLocations);
USEFORM("..\SystemConfig.cpp", frmSystemConfig);
USEFORM("debugger.cpp", frmDebugger);
//---------------------------------------------------------------------------
HANDLE Mutex;
TDeviceRealPalms *ThisTerminal;

bool AlreadyRunning()
{
	HWND PrevWnd;

   Mutex = CreateMutex(NULL, false, "PalmMateUniqueName");
   if ((Mutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
   {
	   PrevWnd = FindWindow("TfrmMain", "PalmMate");
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

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
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
            // Create the Comms object.
            Comms = new TComms;
            //	Comms = new TComms(pnlDraw,pnlDraw);
            // Create Palm holder Object.
            Application->Initialize();
            Application->Title = "PalmMate";
      Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TQuickLogs), &QuickLogs);
		Application->CreateForm(__classid(TfrmData), &frmData);
		Application->CreateForm(__classid(TfrmPause), &frmPause);
		Application->CreateForm(__classid(TfrmProcessing), &frmProcessing);
		Application->CreateForm(__classid(TfrmTCP), &frmTCP);
		Application->CreateForm(__classid(TfrmTouchKeyboard), &frmTouchKeyboard);
		Application->CreateForm(__classid(TfrmLogin), &frmLogin);
		Application->CreateForm(__classid(TfrmSecurity), &frmSecurity);
		Application->CreateForm(__classid(TfrmDBMod), &frmDBMod);
		Application->CreateForm(__classid(TfrmLocations), &frmLocations);
		Application->CreateForm(__classid(TfrmSystemConfig), &frmSystemConfig);
		Application->CreateForm(__classid(TfrmDebugger), &frmDebugger);
		Application->Run();

               // Free the Global Objects.
               delete Kitchen;
               Kitchen = NULL;
               delete Receipt;
               Receipt = NULL;
               delete Comms;
               Comms = NULL;

               // Close our file sterams.
               delete ThisTerminal;
               ThisTerminal = NULL;
               delete Logs;
            }
      }
      __finally
      {
         ReleaseMutex(Mutex);
         CloseHandle(Mutex);
      }
   }
   catch (Exception &exception)
   {
      Application->ShowException(&exception);
   }
   return 0;
}
//---------------------------------------------------------------------------



