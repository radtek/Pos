//---------------------------------------------------------------------------
#include <vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "MMMessageBox.h"
#include "Data.h"       // Database.
#include "enum.h"       // Global Types.

#include "Comms.h"
#include "MMRegistry.h"

#ifdef  PalmMate
#include "Palm.h"       // Palms.
#include "Pause.h"
#include "DataLink.h"   // LowLevel RS485 comms
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

// Maintains the backend serial communcaitons.
TComms *Comms;

//---------------------------------------------------------------------------

//_fastcall TComms::TComms(TComponent* Owner,TWinControl *Parent)
_fastcall TComms::TComms()
{
   MonitorEnabled = false;
	// Transmission Packets.
   #ifdef  PalmMate
      Packet_To_Send = new ApplicationPacket;
      Packet_To_Receive = new TMemoryStream;
      // Just make sure the Thread pointer is Null and not garbage.
      Sync = NULL;
   #endif
   ConnectLst = new TList;
   // A Temperory formatting Place for any inbound/Outbound packets while streaming.
   Data_Inbound = new TMemoryStream;
   Data_Outbound = new TMemoryStream;

   // Storage for application Level Data Recieved in the Processing of a connection.
   // This Memory Space is processed into and IRPacket.
   Process_Inbound = new TMemoryStream;

   // Storage for application Level Data dueing the pakaging done in the handle of an inbound message.
   // This memory space is normally copied into a stream that is then stored in a Palm Packet list.
   Process_Outbound = new TMemoryStream;

	Ports = new TPorts;
	ReceiptPrinter = NULL;
   Total_BytesOut = 0;
   MaxBusyCount = 100;
// Comms Errors.----------------------------------------------------------------

	StartLogging = false;

   bool CarbonCopy = false;
   const String KeyName = MMBaseKey "Printing";
	if (RegistryRead(KeyName,"CarbonCopy",CarbonCopy))
	{
		this->CarbonCopy = CarbonCopy;
	}

	char ReservedPorts[20];
   ZeroMemory(ReservedPorts,sizeof(ReservedPorts));
	if (RegistryRead(MMBaseKey "Comms","ReservedPorts",ReservedPorts,sizeof(ReservedPorts)))
	{
		for (int i = 0 ; i < 20 ; i++)
		{
			ReservedPortNo.insert(ReservedPorts[i]);
		}
	}
}
//---------------------------------------------------------------------------
_fastcall TComms::~TComms()
{
   try
   {
      #ifdef  PalmMate
      Stop();      
      // Send and Recive Packets.
      delete Packet_To_Send;
      Packet_To_Send = NULL;

      delete Packet_To_Receive;
      Packet_To_Receive = NULL;
      #endif

      // Empty the Connect Lst.
      TConnect *TempCon;
      if(ConnectLst != NULL)
      {
         ConnectLst->Pack();
         while(ConnectLst->Count != 0)
         {
            TempCon = (TConnect *)ConnectLst->Items[0];
            delete TempCon;
            ConnectLst->Delete(0);// TList
         }
         delete ConnectLst;
      }

      // A Temperory formatting Place for any inbound packets.
      delete Data_Inbound;
      delete Data_Outbound;
      delete Process_Inbound;
      delete Process_Outbound;
      delete Ports;
    }
    catch(Exception &exception)
    {
        Application->ShowException(&exception);
    }
}

void TComms::DumpStatus(TStringList *Debug)
{

	Debug->Add("Size of Comms Process_Inbound List " + IntToStr(Process_Inbound->Size) );
	Debug->Add("Size of Comms Process_Outbound List " + IntToStr(Process_Outbound->Size) );
	Debug->Add("Size of Comms Data_Inbound List " + IntToStr(Data_Inbound->Size) );
	Debug->Add("Size of Comms Data_Outbound List " + IntToStr(Data_Outbound->Size) );
	Debug->Add("Size of Comms Packet_To_Receive List " + IntToStr(Packet_To_Receive->Size) );
	Debug->Add("Size of Comms Process_Inbound List " + IntToStr(Process_Inbound->Size) );
}
//------------------------------------------------------------------------------

void _fastcall TComms::Initialise()
{
   try
   {
      TComms::Instance().ReceiptPrinter = NULL;
      // Load the List of connected devices into memory.
      TComms::Instance().LoadConnectLst();
   }
   catch(Exception &Err)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"void _fastcall TComms::Initialise()" + Err.Message);
   }
}

//------------------------------------------------------------------------------

void TComms::InitialiseWinConnect()
{
// TODO 3 -o Michael -c Enhance : Test for Windows Printer off line. 
	//PRINTER_INFO_2
   OSVERSIONINFO osvi;
   char szVersion [80];
   memset(&osvi, 0, sizeof(OSVERSIONINFO));
   osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
   GetVersionEx (&osvi);

   if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
   {
      // add all the printers to the connections list.
      LPPRINTER_INFO_5 ppinfo5;
      DWORD       dwNeeded = 0,dwReturned = 0;
      DOC_INFO_1  DocInfo;
      DWORD       dwJob;
      DWORD       BytesWritten;
      HANDLE      PrinterHandle;

      EnumPrinters(PRINTER_ENUM_LOCAL, NULL ,5,"",0,&dwNeeded,&dwReturned);
//      ppinfo5 = (LPPRINTER_INFO_5)new PRINTER_INFO_5[dwNeeded];
      ppinfo5 = new PRINTER_INFO_5[dwNeeded];
      try
      {
         if(ppinfo5)
         {
            memset(ppinfo5, NULL,dwNeeded * sizeof(PRINTER_INFO_5));

            if(EnumPrinters(PRINTER_ENUM_LOCAL ,NULL,5,(LPBYTE)ppinfo5,dwNeeded,&dwNeeded,&dwReturned));
            {
               for(unsigned int i=0 ; i< dwReturned; i++)
               {
                  if(FetchByName(ppinfo5[i].pPrinterName,Windows_Printer) == NULL)
                  {
                     TConnectPrinter *TempConnect = new TConnectPrinter;
                     TempConnect->ID = 0;//i+1; // Index in current list.
                     TempConnect->TRXType = Windows_Printer;
                     TempConnect->HasPaper = true;
         //            TempConnect->Name = ppinfo1[i].pDescription;
                     TempConnect->Name = ppinfo5[i].pPrinterName;

							AnsiString RegKey = MMBaseKey "Printing\\" + TempConnect->Name;

							int PrinterModel = 0;
							RegistryRead(RegKey, "PrinterModel", PrinterModel);
							TempConnect->PrinterModel = PrinterModel;

                     int TempCPL;
							if(RegistryRead(RegKey,"NormalCPL",TempCPL))
                     {
                        TempConnect->NormalCharPerLine = TempCPL;
                     }
                     else
                     {
								RegistryWrite(RegKey,"NormalCPL",TempConnect->NormalCharPerLine);
                     }
							if(RegistryRead(RegKey,"NormalBoldCPL",TempCPL))
                     {
                        TempConnect->BoldCharPerLine = TempCPL;
                     }
                     else
                     {
								RegistryWrite(RegKey,"NormalBoldCPL",TempConnect->BoldCharPerLine);
                     }
                     if(RegistryRead(RegKey,"CutterGap",TempCPL))
                     {
                        TempConnect->CutterGap = TempCPL;
                     }
                     else
                     {
                        RegistryWrite(RegKey,"CutterGap",TempConnect->CutterGap);
                     }

                     if(RegistryRead(RegKey,"TopMargin",TempCPL))
                     {
                        TempConnect->TopMargin = TempCPL;
                     }
                     else
                     {
                        RegistryWrite(RegKey,"TopMargin",TempConnect->TopMargin);
                     }

                     if(RegistryRead(RegKey,"EmulationMode",TempCPL))
                     {
                        TempConnect->EmulationMode = TempCPL;
                     }
                     else
                     {
								TempConnect->EmulationMode = emEpson;
                        RegistryWrite(RegKey,"EmulationMode",TempConnect->EmulationMode);
                     }

                     if(RegistryRead(RegKey,"KitchenTopMargin",TempCPL))
                     {
                        TempConnect->KitchenTopMargin = TempCPL;
                     }
                     else
                     {
                        RegistryWrite(RegKey,"KitchenTopMargin",TempConnect->TopMargin);
                     }

                     TComms::Instance().ConnectLst->Add(TempConnect);
                     TempConnect->Verify(TDeviceRealTerminal::Instance().ID.DeviceKey,true);
                  }
               }
            }
         }
      }
      __finally
      {
	      delete []ppinfo5;
      }
   }
   else if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
   {
    // add all the printers to the connections list.
      LPPRINTER_INFO_4 ppinfo4;
      DWORD       dwNeeded = 0,dwReturned = 0;
      DWORD       dwJob;
      DWORD       BytesWritten;
      HANDLE      PrinterHandle;

      EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_NETWORK, NULL ,4,NULL,0,&dwNeeded,&dwReturned);
      ppinfo4 = (LPPRINTER_INFO_4)new PRINTER_INFO_4[dwNeeded];
      try
      {
         if(ppinfo4)
         {
            memset(ppinfo4, NULL,dwNeeded * sizeof(PRINTER_INFO_4));
            if(EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_NETWORK,NULL,4,(LPBYTE)ppinfo4,dwNeeded,&dwNeeded,&dwReturned));
            {
               for(unsigned int i=0 ; i<dwReturned;i++)
               {
                  if(FetchByName(ppinfo4[i].pPrinterName,Windows_Printer) == NULL)
                  {
                     TConnectPrinter *TempConnect = new TConnectPrinter;
                     TempConnect->ID = 0; // Index in current list.
                     TempConnect->TRXType = Windows_Printer;
                     TempConnect->HasPaper = true;
         //            TempConnect->Name = ppinfo1[i].pDescription;
                     TempConnect->Name = ppinfo4[i].pPrinterName;

                     AnsiString PrinterName = TempConnect->Name;
                     AnsiString RegKey;
                     if(PrinterName.SubString(1,2) == "\\\\")
                     {
                        RegKey = MMBaseKey "Printing\\" + PrinterName.SubString(3,PrinterName.Length()-2);
                     }
                     else
                     {
                        RegKey = MMBaseKey "Printing\\" + TempConnect->Name;
                     }

							int PrinterModel = 0;
							RegistryRead(RegKey, "PrinterModel", PrinterModel);
							TempConnect->PrinterModel = PrinterModel;

							int TempCPL;
							if(RegistryRead(RegKey,"NormalCPL",TempCPL))
                     {
                        TempConnect->NormalCharPerLine = TempCPL;
                     }
                     else
                     {
								RegistryWrite(RegKey,"NormalCPL",TempConnect->NormalCharPerLine);
                     }
							if(RegistryRead(RegKey,"NormalBoldCPL",TempCPL))
                     {
                        TempConnect->BoldCharPerLine = TempCPL;
                     }
                     else
                     {
                        RegistryWrite(RegKey,"NormalBoldCPL",TempConnect->BoldCharPerLine);
                     }
                     if(RegistryRead(RegKey,"CutterGap",TempCPL))
                     {
                        TempConnect->CutterGap = TempCPL;
                     }
                     else
                     {
                        RegistryWrite(RegKey,"CutterGap",TempConnect->CutterGap);
                     }
                     if(RegistryRead(RegKey,"TopMargin",TempCPL))
                     {
                        TempConnect->TopMargin = TempCPL;
                     }
                     else
                     {
                        RegistryWrite(RegKey,"TopMargin",TempConnect->TopMargin);
                     }
                     if(RegistryRead(RegKey,"EmulationMode",TempCPL))
                     {
                        TempConnect->EmulationMode = TempCPL;
                     }
                     else
                     {
								TempConnect->EmulationMode = emEpson;
                        RegistryWrite(RegKey,"EmulationMode",TempConnect->EmulationMode);
                     }

                     if(RegistryRead(RegKey,"KitchenTopMargin",TempCPL))
                     {
                        TempConnect->KitchenTopMargin = TempCPL;
                     }
                     else
                     {
                        RegistryWrite(RegKey,"KitchenTopMargin",TempConnect->TopMargin);
                     }

                     TComms::Instance().ConnectLst->Add(TempConnect);
                     TempConnect->Verify(TDeviceRealTerminal::Instance().ID.DeviceKey,true);
                  }
               }
            }
         }
      }
      __finally
      {
	      delete []ppinfo4;
      }
   }
}

//---------------------------------------------------------------------------





//---------------------------------------------------------------------------

void _fastcall TComms::LoadConnectLst()
{
   try
   {
      // Empty the In Memory Connect Lst.
      TConnect *TempCon;
      while(ConnectLst->Count != 0)
      {
         TempCon = (TConnect *)ConnectLst->Items[0];
         delete TempCon;
         ConnectLst->Delete(0);// TList
      }

      // Load the Connect List For this computer.
      #ifdef  PalmMate
         if(!TDeviceRealTerminal::Instance().Locate()) //Opens the Devices Table.
         {
            TManagerLogs::Instance().Add(__FUNC__,CONFIGLOG,"TCOMMS::LOADCONNECTLST() UNABLE TO LOCATE THIS TERMINAL AS A DEVICE.");
         }
         else
         {
      		ReservedPortNo.clear();
            InitialiseRS485Connect();
            InitialiseWinConnect();
         }


         // Refresh printers for all palms.
         for(int i = 0 ; i < TDeviceRealTerminal::Instance().PalmList->Count; i++)
         {
            TPalm *Ptr = (TPalm *)TDeviceRealTerminal::Instance().PalmList->Items[i];
            for(int j = 0 ; j < ConnectLst->Count; j ++)
            {
               TConnect * TempCon = (TConnect *)ConnectLst->Items[i];
               if (TempCon->TRXType == Windows_Printer)
               {
                  TempCon->Verify(Ptr->ID.DeviceKey,true);
               }
            }
         }

         // Reserve the Ports assigned for Connects.
         char ReservedPorts[20];
         ZeroMemory(ReservedPorts,sizeof(ReservedPorts));
         int i = 0;
         for (std::set<char>::iterator CrntPort = ReservedPortNo.begin(); CrntPort != ReservedPortNo.end(); advance(CrntPort,1) , i++)
         {
            ReservedPorts[i]= *CrntPort;
         }
         RegistryWrite(MMBaseKey "Comms","ReservedPorts",ReservedPorts,sizeof(ReservedPorts));

      #else
			if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); }
         if(!TDeviceRealTerminal::Instance().Locate())
         {
            TManagerLogs::Instance().Add(__FUNC__,CONFIGLOG,"TCOMMS::LOADCONNECTLST() UNABLE TO LOCATE THIS TERMINAL AS A DEVICE.");
         }
         else
         {
            InitialiseWinConnect();
      		ReservedPortNo.clear();
            // Reload the  Reserved Ports assigned for Connects.
            char ReservedPorts[20];
            ZeroMemory(ReservedPorts,sizeof(ReservedPorts));
            RegistryRead(MMBaseKey "Comms","ReservedPorts",ReservedPorts,sizeof(ReservedPorts));
            for (int i = 0 ; i < 20 ; i++)
            {
               ReservedPortNo.insert(ReservedPorts[i]);
            }
         }
			if (frmData->IBTransaction->InTransaction) { frmData->IBTransaction->Commit(); }
      #endif
   }
   catch(Exception &Err)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message);
   }
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Filter.  0 = Dont care.
//				1 = Infrared.
//				2 = Printer.
//	enum SlaveType	{IR_TRX_Station = 1};
// An ID of -1 does not return the PalmNotFoundID it return the first device of that type found...
// This is used when scrounging for a printer whne there is not default and the printer for the
// course specified is not found either.
TConnect* TComms::FetchByID(int ID,SlaveType Filter)
{
   ConnectLst->Pack();
   if( ID == -1)
   {
      for(int i = 0; i< ConnectLst->Count; i++)
      {
         TConnect *ptrConnect = (TConnect *)ConnectLst->Items[i];
         switch(Filter)
         {
            case IR_TRX_Station	:
                        if(ptrConnect->ClassNameIs("TConnectIR"))
                        {
                           if(!ptrConnect->Offline)
                           {
                              return ptrConnect;
                           }
                        }
            break;
            case Windows_Printer :
                        if(ptrConnect->ClassNameIs("TConnectPrinter"))
                        {
                           return ptrConnect;
                        }
            break;
            case Unknown_TRX_Station :
            default	:
               if(!ptrConnect->Offline)
               {
                  return ptrConnect;
               }
         }
      }
      return NULL; //None of the ype requested exist.
   }
   else
   {
      for(int i = 0; i< ConnectLst->Count; i++)
      {
         TConnect *ptrConnect = (TConnect *)ConnectLst->Items[i];
         if(ptrConnect->ID == ID)
         {
            switch(Filter)
            {
               case IR_TRX_Station	:
                           if(ptrConnect->ClassNameIs("TConnectIR"))
                           {
                              if(!ptrConnect->Offline)
                              {
                                 return ptrConnect;
                              }
                              else
                              {
                                 return NULL;
                              }
                           }
                           else
                           {
                              return NULL;
                           }
               case Windows_Printer :
                           if(ptrConnect->ClassNameIs("TConnectPrinter"))
                           {
                              return ptrConnect;
                           }
                           else
                           {
                              return NULL;
                           }
               case Unknown_TRX_Station 	:
               default	:
                  if(!ptrConnect->Offline)
                  {
                     return ptrConnect;
                  }
            }
         }
      }
   }
   return NULL;
}

//---------------------------------------------------------------------------
TConnect* TComms::FetchByName(AnsiString Name,SlaveType Filter)
{
   ConnectLst->Pack();
   if( Name == "")
   {
      for(int i = 0; i< ConnectLst->Count; i++)
      {
         TConnect *ptrConnect = (TConnect *)ConnectLst->Items[i];
         switch(Filter)
         {
            case IR_TRX_Station	:
                        if(ptrConnect->ClassNameIs("TConnectIR"))
                        {
                           if(!ptrConnect->Offline)
                           {
                              return ptrConnect;
                           }
                        }
            break;
            case Windows_Printer :
                        if(ptrConnect->ClassNameIs("TConnectPrinter"))
                        {
                           return ptrConnect;
                        }
            break;
            case Unknown_TRX_Station :
            default	:
               if(!ptrConnect->Offline)
               {
                  return ptrConnect;
               }
         }
      }
      return NULL; //None of the ype requested exist.
   }
   else
   {
      for(int i = 0; i< ConnectLst->Count; i++)
      {
         TConnect *ptrConnect = (TConnect *)ConnectLst->Items[i];
         if(ptrConnect->Name == Name)
         {
            switch(Filter)
            {
               case IR_TRX_Station	:
                           if(ptrConnect->ClassNameIs("TConnectIR"))
                           {
                              if(!ptrConnect->Offline)
                              {
                                 return ptrConnect;
                              }
                              else
                              {
                                 return NULL;
                              }
                           }
                           else
                           {
                              return NULL;
                           }
               case Windows_Printer :
                           if(ptrConnect->ClassNameIs("TConnectPrinter"))
                           {
                              return ptrConnect;
                           }
                           else
                           {
                              return NULL;
                           }
               case Unknown_TRX_Station 	:
               default	:
                  if(!ptrConnect->Offline)
                  {
                     return ptrConnect;
                  }
            }
         }
      }
   }
   return NULL;
}

//---------------------------------------------------------------------------

void TComms::Print(TReqPrintJob *Request)
{
   try
   {
      for(int i = 0; i < Request->PrintJobs->Count; i ++)
      {
         TPrintJob *PrintJob = (TPrintJob *)Request->PrintJobs->Items[i];
         if(PrintJob->JobType == pjReceiptReceipt || PrintJob->JobType == pjReceiptInvoice)
         {
            // Reread Carbon Copy Code.
            bool CarbonCopy = false;
            const String KeyName = MMBaseKey "Printing";
            RegistryRead(KeyName,"CarbonCopy",CarbonCopy);
            this->CarbonCopy = CarbonCopy;

            if(this->ReceiptPrinter == NULL)
            {
               AnsiString RegKey = MMBaseKey "Printing";
               AnsiString ReceiptPrinterName;
               #ifdef  PalmMate
               RegistryRead(RegKey,"PalmReceiptPrinter",ReceiptPrinterName);
               #else
               RegistryRead(RegKey,"ReceiptPrinter",ReceiptPrinterName);
               #endif

               if(ReceiptPrinterName != "None")
               {
                  TManagerLogs::Instance().Add(__FUNC__,"Printing","TComms::SendPrintJob Error No Printers Attached");
                  // Fill in the TPrintJob with the problem here.
                  Request->ErrorMsg = " Error No Receipt Printer Selected";
                  Request->Header.Error = Err_No_Printer_Found;
               }
            }
            else
            {
               // If its the default printer and Printer Que is empty.
               this->ReceiptPrinter->Parse(PrintJob);
            }
         }
         else
         {
            // If you cant find the printer User the default one...
            if(PrintJob->Header.Connection == NULL)
            {
               if(this->ReceiptPrinter == NULL)
               {
                  AnsiString RegKey = MMBaseKey "Printing";
						AnsiString ReceiptPrinterName;
						ReceiptPrinterName = VM->GetStr(vmReceiptPrinter);

						if(ReceiptPrinterName != "None")
                  {
                     // find a printer any printer....
                     TManagerLogs::Instance().Add(__FUNC__,"Printing","TComms::SendPrintJob Error No Printers Attached");
                     // Fill in the TPrintJob with the problem here.
							Request->ErrorMsg = " Error No Printers Attached";
                     Request->Header.Error = Err_No_Printer_Found;
                  }
               }
               else
               {
                  PrintJob->Header.Connection = this->ReceiptPrinter;
                  Request->ErrorMsg = "Your order/s has been sent to the default printer" + this->ReceiptPrinter->Name;
                  Request->Header.Error = Wrn_Printing_Redirected;
               }
            }
            PrintJob->Header.Connection->Parse(PrintJob);
            // If its the default printer and Printer Que is empty.
         }
      }
   }
   catch(Exception &Err)
   {
      Request->ErrorMsg = Err.Message;
      Request->Header.Error = Err_ErrorMsg;
	   TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"TComms::SendPrintJob" + Err.Message);
   }
}

void TComms::PortsRemoveUnused()
{
   Ports->PortsRemoveUnused();
}

void TComms::PortsRemoveAll()
{
    Ports->PortsRemoveAll();
}

void TComms::PortsRetrieveAll()
{
   Ports->Refresh();
   Ports->CheckComPorts();
}

void TComms::KickLocalDraw()
{
	if(Ports->SerialKickPortNumber == 5)
   {
      // Its the default printer.
      TReqPrintJob *Request = new TReqPrintJob(&TDeviceRealTerminal::Instance());
      TPrintJob *PrintJob = new TPrintJob(pjReceiptReceipt,ThisTerminal,NULL);
      PrintJob->Open();
      PrintJob->KickDraw();
      PrintJob->Close();
      Request->PrintJobs->Add(PrintJob);
      TComms::Instance().Print(Request);
      delete Request;  // Will also delete PrintJob.
      TManagerLogs::Instance().Add(__FUNC__,SECURITY,"Opening Cash Draw Via Printer");
   }
   else
   {
      Ports->KickLocalDraw();
   }
}

//---------------------------------------------------------------------------
void TComms::Start()
{
   TComms::Instance().PortsRemoveAll(); // Free up all the comports I'm using.
   PortsRetrieveAll();   // Get all unsued Ports.
   Initialise();
   AnsiString RegKey = MMBaseKey "Printing";
   AnsiString ReceiptPrinterName;
	ReceiptPrinterName = VM->GetStr(vmReceiptPrinter);

   if(ReceiptPrinterName != "None")
   {
      ReceiptPrinter = (TConnectPrinter *)TComms::Instance().FetchByName(ReceiptPrinterName,Windows_Printer);
   }
   else
   {
		TComms::Instance().ReceiptPrinter = NULL;
   }
   PortsRemoveUnused();
   #ifdef  PalmMate
   if(Sync == NULL )
   {
      threadRun();
   }
   #endif
}

void TComms::Stop()
{
   #ifdef  PalmMate
   threadHalt();
   #endif
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------PalmMate only functions-------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifdef  PalmMate

int SyncTableLocX = 100, SyncTableLocY = 100;

char const soh = '[';
char const eoh = ']';
char const sep = '|';
char const soo = '{';
char const eoo = '}';

//------------------------------------------------------------------------------
void TComms::threadRun()
{
   try
   {
      Sync = new SyncThread(false);
   }
   catch(Exception &Err)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"void _fastcall TComms::threadRun()" + Err.Message);
   }
}

//---------------------------------------------------------------------------

void TComms::threadHalt() // Stops Secondary thread.
{
   if(Sync != NULL )
   {
      Sync->Terminate();
//      Sync->WaitFor();
      Sync = NULL;
   }
}


//---------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------Application Layer Incomming Packet -------------------
//------------------------------------------------------------------------------

__fastcall SlavePacket::SlavePacket()
{
	Packet = new TMemoryStream;
}

//------------------------------------------------------------------------------

__fastcall SlavePacket::~SlavePacket()
{
	delete Packet;
}

//------------------------------------------------------------------------------

// Rips the header off the Stream passed in Data and places the Header in
//	the ApplicationPacket Header and the Data in the Packet Stream.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------Application Layer Out going Packet ------------------
//------------------------------------------------------------------------------
__fastcall ApplicationPacket::ApplicationPacket()
{
	Packet = new TMemoryStream;
   Header.hCommand = 0;
   Header.lCommand = 0;
}

//------------------------------------------------------------------------------

__fastcall ApplicationPacket::~ApplicationPacket()
{
	delete Packet;
}

//------------------------------------------------------------------------------

void ApplicationPacket::Clear()
{
	Packet->Clear();
   Header.hCommand = 0;
   Header.lCommand = 0;
}

//------------------------------------------------------------------------------

// Takes the header and the data passed and combines them in Packet to be written.
void ApplicationPacket::FormatForWrite(TMemoryStream *Data)
{
	if(Data)
   {
   	if(Data->Size != 0)
      {
         Packet->Clear();
         // Format Memory Stream.
         Packet->Write(&Header, sizeof(Header));
         Data->Position = 0;
         Packet->CopyFrom(Data, Data->Size);
      }
      else
      {
         Packet->Clear();
         // Format Memory Stream.
         Packet->Write(&Header, sizeof(Header));
      }
   }
   else
   {
      Packet->Clear();
      // Format Memory Stream.
      Packet->Write(&Header, sizeof(Header));
   }
}




void TComms::modReScan(TListBox *ConnectionsListBox)
{
   threadHalt(); // Stop secondary thread.
   bool StartedTrans = false;
   try
   {
      if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction();  StartedTrans = true; }
      if(!TDeviceRealTerminal::Instance().Locate())
      {
         TManagerLogs::Instance().Add(__FUNC__,CONFIGLOG,"TCOMMS::MODAUTOPROGRAM() UNABLE TO LOCATE THIS TERMINAL AS A DEVICE.");
      }
      else
      {
         TConnect *TempCon;
         // Empty the Connect Lst of all Connects.
         while(ConnectLst->Count != 0)
         {
            TempCon = (TConnect *)ConnectLst->Items[0];
            delete TempCon;
            ConnectLst->Delete(0);// TList
         }
			ReservedPortNo.clear();

         // Retrieve all Available ports.
         PortsRetrieveAll();

         // Loop though Posts Port List.
         frmPause->Caption = "Searching Serial Ports.";
         frmPause->Progress->Max = 255;
			frmPause->Show();

         // Loop though all serial ports.
         for(int i=0;i<Ports->PortList->Count; i++)
         {
            if(i != Ports->PortList->Count-1)
            {
               frmPause->btnExit->Caption = "Search Next Serial Port";
            }
            else
            {
               frmPause->btnExit->Caption = "Exit";
            }

            TMMPort *TempPort = (TMMPort *) Ports->PortList->Items[i];
            if(TComms::Instance().ReservedPortNo.find(TempPort->Port) == ReservedPortNo.end())
            {
               frmPause->Caption = "Searching Serial Port " + IntToStr(TempPort->Port);
               frmPause->Progress->Position = 0;
               // For Every Port Poll Every connection....
               if(TempPort->Type == prtUnknown)
               {
                  for(unsigned int Address = 0x0000 ; Address < 0x00FF ; Address++)
                  {
                     frmPause->Caption = "Searching Serial Port " + IntToStr(TempPort->Port) + " Checking Address " + IntToStr(Address);
                     frmPause->Progress->Position +=1;

                     if(InitialiseRS485Connect((TempPort->Port * 1000) + Address))
                     {
                        if(ConnectionsListBox)
                        {
                           ConnectionsListBox->Items->Add(IntToStr((TempPort->Port * 1000) + Address) + " Connected");
                        }
                     }
                     // Update the scrolly bar title... COM1 .. Etc...
                     // Call Application Process Messages...
                     Application->ProcessMessages();
                     // Update the scrolly Bar....
                     if(frmPause->Cancel == true)
                     {
                        break;
                     }
                  }
               }
            }
            frmPause->Cancel = false;
         }
         frmPause->Hide();

         // Do Windows Printers.
         InitialiseWinConnect();
         PortsRemoveUnused();
         threadRun();   // Restart Comms.
      }
      if (frmData->IBTransaction->InTransaction && StartedTrans) { frmData->IBTransaction->Commit(); }
   }
   catch(Exception &Err)
   {
      if (frmData->IBTransaction->InTransaction && StartedTrans) { frmData->IBTransaction->Rollback(); }
      frmPause->Hide();
		// Wait 100ms for a response. (reset)
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message);
	}
}


void TComms::modReset()
{
	TConnectIR *TempConnect = new TConnectIR();
   try
   {
		if(MessageBox("This will destroy all module addressing! "
									  "Do not proceed unless you wish to reprogram all modules. "
									  "Are you sure you wish to continue?",
									  "Warning",
									  MB_OKCANCEL + MB_DEFBUTTON2 + MB_ICONWARNING) == IDOK)
		{
         threadHalt(); // Stop secondary thread.
         // Empty the Connect Lst of all IRConnects.
         for(int i = 0 ; i < ConnectLst->Count; i++)
         {
            TConnect *Object = (TConnect *)ConnectLst->Items[i];
            if(Object->ClassNameIs("TConnectIR"))
            {
               delete (TConnect *)ConnectLst->Items[i];
               ConnectLst->Delete(i);// TList
               i = 0; // restart the sweep.
            }
         }

         for(int i=0;i<Ports->PortList->Count; i++)
         {
            TMMPort *TempPort = (TMMPort *) Ports->PortList->Items[i];
            // For Every Port Poll Every connection....
            unsigned int Address = 65534;
            TempConnect->DataLinkControl->Port = TempPort;
            TempConnect->DataLinkControl->Address = (unsigned short int)Address;
            TempConnect->SetAddress(0x00);
            Address = 65535;
            TempConnect->DataLinkControl->Port = TempPort;
            TempConnect->DataLinkControl->Address = (unsigned short int)Address;
            TempConnect->SetAddress(0x00);
         }
         ShowMessage("Reset Completed.");
      }
		delete TempConnect;
   }
   catch(Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"An Error occured in TComms::modReset" + E.Message);
      frmPause->Hide();
		delete TempConnect;
   }
}


void TComms::modAutoProgram(TListBox *ConnectionsListBox)
{
	if(MessageBox("This will reprogram modules without addresses. Are you sure you wish to continue?",
					  	"Confirmation",
					  	MB_OKCANCEL + MB_DEFBUTTON2 + MB_ICONINFORMATION) == IDOK)
	{
		threadHalt(); // Stop Secondary thread.

		unsigned short MinAddress = 0;
		TConnectIR *TempConnect = new TConnectIR();
      bool StartTrans = false;
      try
      {
         if (!TDeviceRealTerminal::Instance().IBTransaction->InTransaction) {TDeviceRealTerminal::Instance().IBTransaction->StartTransaction(); StartTrans = true; }

         TDeviceRealTerminal::Instance().IBQuery->Close();
         TDeviceRealTerminal::Instance().IBQuery->SQL->Text =
            "SELECT MAX(CONNECT_ID) FROM CONNECTIONS "
            " WHERE CONNECT_TYPE = :CONNECT_TYPE";
         TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_TYPE")->AsInteger = IR_TRX_Station;
         TDeviceRealTerminal::Instance().IBQuery->Open();

         MinAddress = ((unsigned short)(TDeviceRealTerminal::Instance().IBQuery->FieldByName("CONNECT_ID")->AsInteger % 1000) + 1);
         if(MinAddress == 0)
            MinAddress++;

         // Empty the Connect Lst of all Connects.
         TConnect *TempCon = NULL;
         while(ConnectLst->Count != 0)
         {
            TempCon = (TConnect *)ConnectLst->Items[0];
            delete TempCon;
            ConnectLst->Delete(0);// TList
         }

         // Loop though Posts Port List.
         frmPause->Caption = "Searching Serial Ports.";
         frmPause->Progress->Position =0;
         frmPause->Progress->Max = 255;
         frmPause->Show();

         while(frmPause->Cancel != true)
         {
            frmPause->Caption = "Searching Old Connections ";
            // For Every Port Poll Every connection....
            // Poll Existing connections.
            TDeviceRealTerminal::Instance().IBQuery->Close();
            TDeviceRealTerminal::Instance().IBQuery->SQL->Text =
               "SELECT * FROM CONNECTIONS "
               " WHERE CONNECT_TYPE = :CONNECT_TYPE";
            TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_TYPE")->AsInteger = IR_TRX_Station;
            TDeviceRealTerminal::Instance().IBQuery->Open();

            for(TDeviceRealTerminal::Instance().IBQuery->First(); !TDeviceRealTerminal::Instance().IBQuery->Eof ; TDeviceRealTerminal::Instance().IBQuery->Next())
            {
               TempConnect->DataLinkControl->Port = Ports->GetComPort((TDeviceRealTerminal::Instance().IBQuery->FieldByName("CONNECT_ID")->AsInteger / 1000));
               if(TempConnect->DataLinkControl->Port)
               {
                  TempConnect->DataLinkControl->Address = ((unsigned short int)TDeviceRealTerminal::Instance().IBQuery->FieldByName("CONNECT_ID")->AsInteger % 1000);
                  // Update the scrolly bar title... COM1 .. Etc...
                  frmPause->Caption = "Searching Serial Port " + IntToStr(TempConnect->DataLinkControl->Port->Port) + " Checking Address " + IntToStr(TempConnect->DataLinkControl->Address);
                  frmPause->Progress->Position +=1;
                  try
                  {
                     TempConnect->Connect();
                     // Call Application Process Messages...
                     Application->ProcessMessages();
                     // Update the scrolly Bar....
                     if(frmPause->Cancel == true)
                     {
                        break;
                     }
                  }
                  catch(Exception &E)
                  {
                     TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"An Error occured in TComms::modAutoProgram" + E.Message);
                     throw;
                  }
                  if(frmPause->Cancel == true)
                  {
                     break;
                  }
               }
            }

            frmPause->Caption = "Searching Address Zero on All Ports";
            for(int i=0;i<TComms::Instance().Ports->PortList->Count; i++)
            {
               TMMPort *TempPort = (TMMPort *) Ports->PortList->Items[i];
               // Check address 0x00 on each port.
               TempConnect->DataLinkControl->Port = TempPort;
               TempConnect->DataLinkControl->Address = 0x00;
               // Update the scrolly bar title... COM1 .. Etc...
               frmPause->Caption = "Searching Serial Port " + IntToStr(TempPort->Port) + " Checking Address 00";
               frmPause->Progress->Position +=1;
               if(TempConnect->Connect())
               {
                  if(TempConnect->SetAddress(MinAddress))
                  {
                     TempConnect->DataLinkControl->Address = MinAddress;
                     TempConnect->ID = ((TempConnect->DataLinkControl->Port->Port * 1000) + TempConnect->DataLinkControl->Address);
                     // Add the new device to the list.
                     if(TempConnect->Connect())
                     {
                        TempConnect->Verify(TDeviceRealTerminal::Instance().ID.DeviceKey,true);
                        if(ConnectionsListBox)
                        {
                           ConnectionsListBox->Items->Add(IntToStr(TempConnect->ID) + " Connected");
                        }
                        MinAddress++;
                     }
                  }
               }
            }
            // Call Application Process Messages...
            Application->ProcessMessages();
         }

         frmPause->Hide();
	      if (frmData->IBTransaction->InTransaction && StartTrans) { frmData->IBTransaction->Commit(); }
	      delete TempConnect;
      }
      catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
         if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Rollback(); }
      }
   }
}


void TComms::modClear()
{
   threadHalt(); // Stop secondary thread.
   TConnect *TempCon;
   // Empty the Connect Lst of all Connects.
   while(ConnectLst->Count != 0)
   {
      TempCon = (TConnect *)ConnectLst->Items[0];
      delete TempCon;
      ConnectLst->Delete(0);// TList
   }
   TComms::Instance().ReceiptPrinter = NULL;
}


//---------------------------------------------------------------------------
void TComms::InitialiseRS485Connect()
{
	// Assume IR for each connection.

   bool StartTrans = false;
   try
   {
      if (!TDeviceRealTerminal::Instance().IBTransaction->InTransaction) {TDeviceRealTerminal::Instance().IBTransaction->StartTransaction(); StartTrans = true; }

      TDeviceRealTerminal::Instance().IBQuery->Close();
      TDeviceRealTerminal::Instance().IBQuery->SQL->Text =
         "SELECT * FROM CONNECTIONS "
         " WHERE DEVICE_KEY = :DEVICE_KEY AND CONNECT_TYPE = :CONNECT_TYPE";
      TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.DeviceKey;
      TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_TYPE")->AsInteger = IR_TRX_Station;
      TDeviceRealTerminal::Instance().IBQuery->Open();

      for (TDeviceRealTerminal::Instance().IBQuery->First();!TDeviceRealTerminal::Instance().IBQuery->Eof  ; TDeviceRealTerminal::Instance().IBQuery->Next())
      {
         unsigned short ID = (unsigned short)(TDeviceRealTerminal::Instance().IBQuery->FieldByName("CONNECT_ID")->AsInteger);

         TConnectIR *TempConnect = NULL;
         bool Retval = false;
         try
         {
            TempConnect = new TConnectIR();
            try
            {
               TempConnect->DataLinkControl->Port = Ports->GetComPort(ID/1000);
               if(TempConnect->DataLinkControl->Port)
               {
                  TempConnect->DataLinkControl->Address = (unsigned short)(ID % 1000);
                  if(TempConnect->Connect())
                  {
                     // Was it an IR Conenct.
                     switch(TempConnect->GetSlaveType())
                     {
                        case IR_TRX_Station :
                        {
                           // Add to List.
                           TConnectIR *PermaConnect = new TConnectIR();
                           PermaConnect->TRXType = IR_TRX_Station;
                           PermaConnect->DataLinkControl->Port = TempConnect->DataLinkControl->Port;
                           PermaConnect->DataLinkControl->Port->Description = "RS-485 Network";
                           PermaConnect->DataLinkControl->Port->Type = prtAdam;
                           PermaConnect->DataLinkControl->Address = TempConnect->DataLinkControl->Address;
                           PermaConnect->DataLinkControl->Connected = false; // Must Reconenct to set up Sequence number.
                           PermaConnect->Name = "Port " + IntToStr(PermaConnect->DataLinkControl->Port->Port) + " Module " + IntToStr(PermaConnect->DataLinkControl->Address) + " (Infrared)";
                           PermaConnect->ID = ID;
                           PermaConnect->Offline = false;
                           if(FetchByID(PermaConnect->ID,PermaConnect->TRXType) == NULL)
                           {
                              TComms::Instance().ConnectLst->Add(PermaConnect);
                              PermaConnect->Verify(TDeviceRealTerminal::Instance().ID.DeviceKey,true);
                           }

                           break;
                        }
                        default :
                           TManagerLogs::Instance().Add(__FUNC__,CONFIGLOG," Unknown Module on port " +
                                 IntToStr(TempConnect->DataLinkControl->Port->Port) +
                                 " At address " + IntToStr(TempConnect->DataLinkControl->Address) +
                                 " . This Module was Ignored.");
                     }
                     ReservedPortNo.insert(ID/1000);
                     Retval = true;
                  }
                  else
                  {
                     TempConnect->Offline = true;
                     TempConnect->Verify(TDeviceRealTerminal::Instance().ID.DeviceKey,false);
                  }
               }
               else
               {
                  TempConnect->Offline = true;
               }
            }
            __finally
            {
               delete TempConnect;
               TempConnect = NULL;
            }
         }
         catch(Exception &Err)
         {
            delete TempConnect;
            TempConnect = NULL;
            TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"bool TComms::InitialiseConnect()" + Err.Message);
         }
      }
      if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Commit(); }
   }
   catch(Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
      if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Rollback(); }
   }
}

//---------------------------------------------------------------------------

bool TComms::InitialiseRS485Connect(unsigned short ID)
{
	// Assume IR for each connection.
   bool Retval = false;
	TConnectIR *TempConnect = new TConnectIR();
	try
   {
      TempConnect->DataLinkControl->Port = Ports->GetComPort(ID/1000);
      if(TempConnect->DataLinkControl->Port)
      {
         TempConnect->DataLinkControl->Address = (unsigned short)(ID % 1000);
         if(TempConnect->Connect())
         {
            // Was it an IR Conenct.
            switch(TempConnect->GetSlaveType())
            {
               case IR_TRX_Station :
               {
                  // Add to List.
                  TConnectIR *PermaConnect = new TConnectIR();
                  PermaConnect->TRXType = IR_TRX_Station;
                  PermaConnect->DataLinkControl->Port = TempConnect->DataLinkControl->Port;
                  PermaConnect->DataLinkControl->Port->Description = "RS-485 Network";
                  PermaConnect->DataLinkControl->Port->Type = prtAdam;
                  PermaConnect->DataLinkControl->Address = TempConnect->DataLinkControl->Address;
                  PermaConnect->DataLinkControl->Connected = false; // Must Reconenct to set up Sequence number.
                  PermaConnect->Name = "Port " + IntToStr(PermaConnect->DataLinkControl->Port->Port) + " Module " + IntToStr(PermaConnect->DataLinkControl->Address) + " (Infrared)";
                  PermaConnect->ID = ID;
                  PermaConnect->Offline = false;
                  if(FetchByID(PermaConnect->ID,PermaConnect->TRXType) == NULL)
                  {
                     TComms::Instance().ConnectLst->Add(PermaConnect);
                     PermaConnect->Verify(TDeviceRealTerminal::Instance().ID.DeviceKey, true);
                  }
                  break;
               }
               default :
                  TManagerLogs::Instance().Add(__FUNC__,CONFIGLOG," Unknown Module on port " +
                        IntToStr(TempConnect->DataLinkControl->Port->Port) +
                        " At address " + IntToStr(TempConnect->DataLinkControl->Address) +
                        " . This Module was Ignored.");
               Retval = false;
            }
        		ReservedPortNo.insert(ID/1000);
            Retval =  true;
         }
         else
         {
            TempConnect->Offline = true;
            TempConnect->Verify(TDeviceRealTerminal::Instance().ID.DeviceKey, false);
            Retval = false;
         }
      }
      else
      {
         TempConnect->Offline = true;
      }
      delete TempConnect;
      TempConnect = NULL;
   }
   catch(Exception &Err)
   {
 		delete TempConnect;
      TempConnect = NULL;
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"bool TComms::InitialiseConnect(XXX)" + Err.Message);
      Retval = false;
   }
   return Retval;
}

//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall SyncThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//--
__fastcall SyncThread::SyncThread(bool CreateSuspended)
  : TThread(CreateSuspended)
{
   FreeOnTerminate = true;
}

__fastcall SyncThread::~SyncThread()
{
//   TComms::Instance().Sync = NULL;
}
//---------------------------------------------------------------------------
void __fastcall SyncThread::Execute()
{
	TConnect *CurrentConnection;
   TComms::Instance().ConnectLst->Pack();
   while(!Terminated)
   {
      try
      {
			CurrentConnection = 0;
         // Clear all outbound palm Ques...
         if(ThisTerminal != NULL)
         {
            for(int i = 0; i < TDeviceRealTerminal::Instance().PalmList->Count ; i++)
            {
               TPalm *TempPalm = (TPalm *)TDeviceRealTerminal::Instance().PalmList->Items[i];
               TempPalm->Outbound->Clear();
               if(TempPalm->RemoveFromPalmList)
               {
                  TDeviceRealTerminal::Instance().PalmList->Delete(i);
                  delete TempPalm;
                  TempPalm = NULL;
                  i = 0;
               }
               else if(TempPalm->Status.Active)
               {
                  if(((TempPalm->LogoffTime *60000) + 600000 ) < (GetTickCount() - TempPalm->Status.LastActive) )
                  {
                     TempPalm->User.Clear(); // log them off.
                     TempPalm->Status.Active = false;
                     for(int i = 0; i < TempPalm->SeatNames->Count;)
                     {
                        TempPalm->RemoveSeatName((TPalmSeatName *)TempPalm->SeatNames->Items[i]);
                        TempPalm->ClearSeatNames();
                     }
                  }
               }
            }
            TDeviceRealTerminal::Instance().PalmNotFound->Outbound->Clear();
            TDeviceRealTerminal::Instance().PalmRegister->Outbound->Clear();
         }
			int IRModuleCount = 0;
         if(TComms::Instance().ConnectLst->Count != 0)
         {
            bool NonWindowsConnectionExists = false; ;
            for(int i = 0; i < TComms::Instance().ConnectLst->Count ; i++)
            {
               CurrentConnection = (TConnect *)TComms::Instance().ConnectLst->Items[i];
               if(CurrentConnection->TRXType != Windows_Printer)
               {
                  if(CurrentConnection->DownTime < GetTickCount())
                  {
                  // remove the connection if faulty.
                     CurrentConnection->ProcessConnection();
                  	IRModuleCount++;
                  }
                  NonWindowsConnectionExists = true;
               }
            }
            // Dont bother polling windows Printers.
            if(!NonWindowsConnectionExists)
            {
               Terminate();
					TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Secondary thread terminated - Conection list doesn't contain IR.");
            }
         }
         else
         {
            // Search for Connections.
            // Or inform user all connections have been disconnected.
            Terminate();
				TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Secondary thread terminated - Conection list empty.");
         }
         if (IRModuleCount == 0)
         {
         	Sleep(EXECPTIONDOWNTIME);
         }
      }
      catch(Exception &Err)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"SyncThread::Execute " + Err.Message);
         if(CurrentConnection)
         {
         	CurrentConnection->DownTime = GetTickCount() + EXECPTIONDOWNTIME;
         }
      }
      catch(...)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"GLOBAL CATCH ERROR ");
         if(CurrentConnection)
         {
         	CurrentConnection->DownTime = GetTickCount() + EXECPTIONDOWNTIME;
         }
      }
   }
}


void SyncThread::ProcessTransmission(unsigned char* &ptrStream, TPalm *Palm)
{
   try
   {
     	this->CrntPalm = Palm;
      Palm->ptrRawData = ptrStream;
      Synchronize(PalmProcessData);
   }
   catch(Exception &Err)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message);
      throw;
   }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall SyncThread::PalmProcessData()
{
   TPalm *Palm = this->CrntPalm;
   try
   {
      Palm->ProcessTransmission();
   }
   catch(EPrinterError &Err) // Incase printer not found and cant be redirected.
   {
      TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Error packet being built, Error printing Billing For Palm #" + IntToStr(Palm->ID.DeviceID));
      Palm->Status.Error = Err_No_Printer_Found;
   }
}



#endif



