//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef  PalmMate
#include "Palm.h"       // Palms.
#include "Pause.h"
#endif

#include "enum.h"       // Global Types.
#include "Comms.h"
#include "ManagerVariable.h"
#include "ManagerPhysicalPrinter.h"
#include "VirtualPrinterManager.h"
#include "PortTalk.h"
#include "Printout.h"
#include "MMMessageBox.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "CashDrawFactory.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//_fastcall TComms::TComms(TComponent* Owner,TWinControl *Parent)
_fastcall TComms::TComms()
{
   MonitorEnabled = false;
	// Storage for application Level Data Recieved in the Processing of a connection.
	// This Memory Space is processed into and IRPacket.
   Process_Inbound = new TMemoryStream;

   // Storage for application Level Data dueing the pakaging done in the handle of an inbound message.
	// This memory space is normally copied into a stream that is then stored in a Palm Packet list.
   Process_Outbound = new TMemoryStream;

   MaxBusyCount = 100;
// Comms Errors.----------------------------------------------------------------

	StartLogging = false;

   SerialKickPort = new TComPort(NULL);
   SerialKickPort->BaudRate = br19200;
	SerialKickPort->DiscardNull = false;
	SerialKickPortNumber = 0;
	PrinterIndex = 0;
}
//---------------------------------------------------------------------------
_fastcall TComms::~TComms()
{
   try
   {
      // A Temperory formatting Place for any inbound packets.
		delete Process_Inbound;
      delete Process_Outbound;
      delete SerialKickPort;
    }
    catch(Exception &exception)
    {
		  Application->ShowException(&exception);
	 }
}

void _fastcall TComms::Initialise(Database::TDBTransaction &DBTransaction)
{
	try
   {
		SerialKickPortNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmSerialKickPort);
		SerialKickPort_Length = TManagerVariable::Instance().GetInt(DBTransaction,vmSerialKickPortLength,3);
		SerialKickPortHighChars = TManagerVariable::Instance().GetBool(DBTransaction,vmSerialKickPortHighChars);
		SerialKickPort->Port = "COM" + IntToStr(SerialKickPortNumber);

		TManagerPhysicalPrinter ManagerPhysicalPrinter;
		int PhysicalPrinterKey = TManagerVariable::Instance().GetInt(DBTransaction,vmReceiptPrinter);
		TComms::Instance().ReceiptPrinter = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,PhysicalPrinterKey);
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"void _fastcall TComms::Initialise()" + Err.Message);
	}
}

//------------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void TComms::KickLocalDraw(Database::TDBTransaction &DBTransaction)
{
    try
    {
        std::auto_ptr<TCashDraw> cashDrawer(TCashDrawFactory::CreateCashDraw( SerialKickPortNumber, SerialKickPort_Length, SerialKickPortHighChars, ReceiptPrinter ));

        if( cashDrawer.get() != NULL )
        {
            cashDrawer->OpenDraw();
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + ex.Message);
    }

// todo: remove the following
/*

	if(SerialKickPortNumber == 17)
	{ //Receipt Printer
      try
      {
         if(TComms::Instance().ReceiptPrinter.PhysicalPrinterKey != 0)
         {
            // Its the default printer.
            TPrintout *Printout = new TPrintout;
            Printout->Printer = TComms::Instance().ReceiptPrinter;
            try
            {
               Printout->OpenDrawer();
            }
            __finally
            {
               delete Printout;
            }
         }
         else
         {
            TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Error Opening Cash Drawer. No Receipt Printer Selected");
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
	}
   else if(SerialKickPortNumber == 18)
	{ //Cashdrawer Printer
		// Its the default printer.
		TPrinterVirtual *PrinterVirtual = TManagerVirtualPrinter::GetVirtualPrinterByName(DBTransaction,"Cashdrawer Printer");
      if(PrinterVirtual)
      {
         TManagerPhysicalPrinter ManagerPhysicalPrinter;
         TPrinterPhysical PhysicalPrinter = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,PrinterVirtual->PhysicalPrinterKey);
         if(PhysicalPrinter.PhysicalPrinterKey != 0)
         {
            TPrintout *Printout = new TPrintout;
            Printout->Printer = PhysicalPrinter;
            try
            {
               Printout->PrintFormat->EmulationMode					= PhysicalPrinter.EmulationMode;
               Printout->PrintFormat->NormalWidth					= PhysicalPrinter.NormalCharPerLine;
               Printout->PrintFormat->BoldWidth						= PhysicalPrinter.BoldCharPerLine;
               Printout->PrintFormat->CutterGap						= PhysicalPrinter.CutterGap;
               Printout->PrintFormat->TopMargin						= PhysicalPrinter.TopMargin;
               Printout->PrintFormat->AlwaysUseFullCut				= PhysicalPrinter.AlwaysUseFullCut;
               Printout->PrintFormat->KanjiPrinter					= PhysicalPrinter.KanjiPrinter;

               Printout->PrintFormat->Line->FontInfo.Height			= fsDoubleSize;
               Printout->PrintFormat->Line->ColCount				= 1;
               Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
               Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
               Printout->PrintFormat->Line->Columns[0]->Text		= "Open Drawer Open Drawer Open Drawer";
               for (int i = 0; i < SerialKickPort_Length ; i++)
               {
                  Printout->PrintFormat->AddLine();
                  Printout->PrintFormat->AddLine();
                  Printout->PrintFormat->AddLine();
                  Printout->PrintFormat->AddLine();
                  Printout->PrintFormat->AddLine();
               }
               Printout->Print();
            }
            __finally
            {
               delete Printout;
            }
         }
         else
         {
            TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Locate 'Cashdrawer Printer' as a Physical windows printer");
         }

		}
      else
      {
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Locate 'Cashdrawer Printer' as a Virtual printer");
      }
   }
   else if(SerialKickPortNumber == 19)
	{ // Javlin / POS 66X Cashdrawer Port
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {
            PortTalk->OutPort(0x04B8, 0xF1);
            //wait 200 ms
            ::Sleep(200);
            PortTalk->OutPort(0x04B8, 0xF0);

            PortTalk->OutPort(0x04B8, 0xF2);
            //wait 200 ms
            ::Sleep(200);
            PortTalk->OutPort(0x04B8, 0xF0);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
   else if(SerialKickPortNumber == 20)
	{ // Gladius Cash Drawer Ports.
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {
            PortTalk->OutPort(0x240, 0xF1);
            //wait 200 ms
            ::Sleep(200);
            PortTalk->OutPort(0x240, 0xF0);

            PortTalk->OutPort(0x240, 0xF2);
            //wait 200 ms
            ::Sleep(200);
            PortTalk->OutPort(0x240, 0xF0);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
   else if(SerialKickPortNumber == 21)
	{ // PT6200 Drawer Ports.
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {
            PortTalk->OutPort(0x40B9, 0xFF);
            ::Sleep(10);
            PortTalk->OutPort(0x40B9, 0xF7);
            ::Sleep(10);
            PortTalk->OutPort(0x40B9, 0xFF);
            ::Sleep(10);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
   else if(SerialKickPortNumber == 22)
	{ // WILLPOS A10 Drawer Ports.
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {
            PortTalk->OutPort(0x044D, 0x40);
            ::Sleep(500);
            PortTalk->OutPort(0x044D, 0x38);
            ::Sleep(1000);

            PortTalk->OutPort(0x044D, 0x80);
            ::Sleep(500);
            PortTalk->OutPort(0x044D, 0x38);
            ::Sleep(500);

         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
   else if(SerialKickPortNumber == 23)
	{ // WILLPOS A20 Drawer Ports.
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {
            PortTalk->OutPort(0x048D, 0x10);
            ::Sleep(500);
            PortTalk->OutPort(0x048D, 0x0E);
            ::Sleep(1000);
            PortTalk->OutPort(0x048D, 0x20);
            ::Sleep(500);
            PortTalk->OutPort(0x048D, 0x0E);
            ::Sleep(1000);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
   else if(SerialKickPortNumber == 24)
	{ // Flytech Cash Drawer Ports. Flytech CD4B8B78
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {
            PortTalk->OutPort(0x04B8, 0x06);
            //wait 200 ms
            ::Sleep(300);
            PortTalk->OutPort(0x04B8, 0x00);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
   else if(SerialKickPortNumber == 25)
	{ // Flytech Cash Drawer Ports. Flytech CDB4B8x2
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {  // Fire both ports.
            PortTalk->OutPort(0x04B8, 0x0F);
            ::Sleep(300);
            PortTalk->OutPort(0x04B8, 0x00);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
   else if(SerialKickPortNumber == 26)
	{ // Protech Panel PC
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {  // Fire both ports.
            PortTalk->OutPort(0x048F, 0x03);
            ::Sleep(300);
            PortTalk->OutPort(0x048F, 0x0B);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
   else if(SerialKickPortNumber == 27)
	{ // Protech PS3100
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {  // Fire both ports.
            PortTalk->OutPort(0x048F, 0x40); // 64 decimal.
            ::Sleep(300);
            PortTalk->OutPort(0x048F, 0x02);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
	else if(SerialKickPortNumber == 28)
	{ // VPOS 335
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {  // Fire both ports.
            PortTalk->OutPort(0x048C, 0x0C);
            ::Sleep(300);
            PortTalk->OutPort(0x048C, 0x00);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
	else if(SerialKickPortNumber == 29)
	{ // EBN XPOS 752
      try
      {
         std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
         if(PortTalk->Initialise())
         {
				// The EBN has only one cash-drawer port.
				// The specification states that bit 4 must be sent to kick.
            PortTalk->OutPort(0x280, 0x08);
				// Waiting 300ms as a precaution - just to let the IO filter through.
            ::Sleep(300);
				// And close - we don't want the volts flying down forever!?
				PortTalk->OutPort(0x280, 0x00);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
      }
   }
	else if(SerialKickPortNumber == 30)
	{  // Advance POS CM-5200
      HANDLE hFile = NULL;
		try
      {
			// IOCTL Codes
         #define GPD_TYPE 56053
         #define ADV_OPEN_CTL_CODE CTL_CODE(GPD_TYPE, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)
         #define ADV_STATUS_CTL_CODE CTL_CODE(GPD_TYPE, 0x901, METHOD_BUFFERED, FILE_ANY_ACCESS)
         // uWhichDrawer = 1 => CD#1, uWhichDrawer = 2 => CD#2
         BOOL bRet;
         UCHAR uDrawer = 0x01;
         ULONG ulBytesReturned = 0;
         // Open the driver
         hFile = CreateFile("\\\\.\\ADVSYS", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, 0);
         if (hFile == INVALID_HANDLE_VALUE)
         {
	         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"POS CM-5200 Unable to open Cash Drawer Device Driver!");
         }
         else
         {
            // Turn on the Cash Drawer Output (Fire the required solenoid)
            bRet = DeviceIoControl(hFile, ADV_OPEN_CTL_CODE, &uDrawer, sizeof(uDrawer), NULL, 0, &ulBytesReturned, NULL);
            if (bRet == FALSE || ulBytesReturned != 1)
            {
               TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"POS CM-5200 Failed to write to cash drawer driver");
               CloseHandle(hFile);
            }
            else
            {
	         	CloseHandle(hFile);
            }
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
         if (hFile != INVALID_HANDLE_VALUE)
         {
	         CloseHandle(hFile);
			}
      }
   }
   else
   {
      if(SerialKickPort != NULL && SerialKickPortNumber != 0)
      {
         try
         {
            SerialKickPort->Port = "COM" + IntToStr(SerialKickPortNumber);
            SerialKickPort->Open();

            unsigned char *buffer = new unsigned char [SerialKickPort_Length];
				try
            {
               if(SerialKickPortHighChars)
               {
                  FillMemory(buffer,SerialKickPort_Length,0xAA);
               }
					else
               {
                  ZeroMemory(buffer,SerialKickPort_Length);
               }
					SerialKickPort->Write(buffer,SerialKickPort_Length);
				}
            __finally
            {
               delete []buffer;
               SerialKickPort->Close();
				}
			}
			catch(Exception &Err)
			{
				TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
			}
		}
	}

*/

}

//---------------------------------------------------------------------------
//---------------------PalmMate only functions-------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifdef  PalmMate

int SyncTableLocX = 100, SyncTableLocY = 100;

char const soh = '[';
char const eoh = ']';
char const sep = '|';
char const soo = '{';
char const eoo = '}';

#endif
