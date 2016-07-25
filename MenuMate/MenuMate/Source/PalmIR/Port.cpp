//---------------------------------------------------------------------------
#include   <vcl.h>
#include   <stdlib.h>
#pragma hdrstop

#include "Port.h"
#include "enum.h"
// temp remove later....
#include "MMRegistry.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TMMPort *)
{
	new TMMPort(NULL,prtUnknown);
}


__fastcall EComsSyncError::EComsSyncError(AnsiString inMsg, int inErrNum)
{
   Msg = inMsg;
   ErrNum = inErrNum;
}


//TPorts::TPorts(TComponent* Owner,TWinControl *Parent)
TPorts::TPorts()
{
   Refresh();   
   PortList = new TList();
}

TPorts::~TPorts()
{
   TMMPort *TempPort;
   // this will remove All allocated ports.
   while(PortList->Count != 0)
   {
    TempPort = (TMMPort *)PortList->Items[0];
    delete TempPort;
    PortList->Delete(0);
   }
   delete PortList;
}

void TPorts::CheckComPorts()
{
  // Remove All Commports in the List.
   while(PortList->Count != 0)
   {
      TMMPort *Port = (TMMPort *)PortList->Items[0];
      delete Port;
      PortList->Delete(0);
   }

  TMMPort *TempPort; // used for creating the ports.
  // This is a list of all the currently supported comports.
  char ComPort[4][5]={"COM1","COM2","COM3","COM4"};
  // This points to the comport checked in the popup menu.
  char *ptr_ComPort;
  // Create the new comport Object.
   for(int i = 0 ; i < 4; i++)
   {
      ptr_ComPort = ComPort[i]; // Check Com1.
     if(CheckComPort(ptr_ComPort))
     {
         // Create the new comport Object.
         PortType NewType = prtUnknown;
         if(i+1 == PoleDisplayPortNumber)
         {
            NewType = prtPoleDisplay;
         }
         else if(i+1 == SerialKickPortNumber)// Reserved for draw kick.
         {
            NewType = prtCashDraw;
         }
         else
         {
            NewType = prtUnknown;
         }

         TempPort = new TMMPort(i+1,NewType);
         this->PortList->Add(TempPort);

         if(i+1 == PoleDisplayPortNumber)
         {
            PoleDisplayPort = TempPort;
            PoleDisplayPort->Description = "Pole Display";
         }
         else if(i+1 == SerialKickPortNumber)// Reserved for draw kick.
         {
            SerialKickPort = TempPort;
            SerialKickPort->Description = "Cashdraw";
         }
     }
   }
}

//---------------------------------------------------------------------------
//------------------------Checks the Comports Exist--------------------------
// Returns false if a comport does not exist.
bool TPorts::CheckComPort(char *ptr)
{
  // This is the Handle to the comport.
  void  *hCommFile;
  bool value;
  AnsiString CurrentPort = ptr;
  // Trys to open the serial comport as pointed to by the Comport Pointer.
  hCommFile = CreateFile(ptr,          //Pointer to a string.
                       GENERIC_READ+GENERIC_WRITE,
                       0, //not shared
                       NULL, //no security ??
                       OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,
                       NULL ); //Template


  // Check the comport exists.
  if (!(hCommFile == INVALID_HANDLE_VALUE))
  {
     // Is this a valid comm handle?
     switch(GetFileType( hCommFile ))
     {
     		case FILE_TYPE_CHAR:
				value = true;
         break;
     		case FILE_TYPE_UNKNOWN:
			   TManagerLogs::Instance().Add(__FUNC__,CONFIGLOG,CurrentPort + " FILE_TYPE_UNKNOWN");
				value = false;
         break;
     		case FILE_TYPE_DISK:
			   TManagerLogs::Instance().Add(__FUNC__,CONFIGLOG,CurrentPort + " FILE_TYPE_DISK");
				value = false;
         break;
     		case FILE_TYPE_PIPE:
			   TManagerLogs::Instance().Add(__FUNC__,CONFIGLOG,CurrentPort + " FILE_TYPE_PIPE");
		      value = false;
         break;
     }
  }
  else
  {
	   TManagerLogs::Instance().Add(__FUNC__,CONFIGLOG,CurrentPort + " Returned Invalid Handle");
     value = false;
  }
  // Close the Handle to the comport.
  CloseHandle(hCommFile);
  return(value);
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//------------------------Attemps to Open the ComPort-----------------------
//---------------------------------------------------------------------------
// Returns false if a comport does not exist.
TMMPort* TPorts::GetComPort(int Port)
{
   // Loop Though all ports.
   for(int i=0 ; i <= PortList->Count; i ++)
   {
      if( i == PortList->Count)
      {
         return NULL;
      }
      else if( ((TMMPort *)(PortList->Items[i]))->GetPort() == Port )
      {
         return ((TMMPort *)(PortList->Items[i]));
      }
   }
   return NULL;
}

TMMPort* TPorts::GetComPort(AnsiString Port)
{
   // Loop Though all ports.
   for(int i=0 ; i <= PortList->Count; i ++)
   {
      if( strcmp( ((TMMPort *)PortList->Items[i])->ptr_ComPort, Port.c_str()) == 0 )
      {
         return ((TMMPort *)(PortList->Items[i]));
      }
   }
   return NULL;
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//-------------------------Port Handling Routines----------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// This is run when the component is first placed on the page.
//__fastcall TMMPort::TMMPort(TComponent* Owner,TWinControl *Parent,int ComPort) : TImage(Owner)
__fastcall TMMPort::TMMPort(int CommPort,PortType CurrentType)
{
//  TMenuItem *NewItem;  // A Temp Menu Item.
// Assign the Comports.
//  this->Parent = Parent;
   FLogging = false;
   strcpy(ComPort[0], "COM1");
   strcpy(ComPort[1], "COM2");
   strcpy(ComPort[2], "COM3");
   strcpy(ComPort[3], "COM4");
   commprop = new TCommProp;
   Description = "";
   Type = CurrentType;
   // Currently no Comms in operation.
   hCommFile = INVALID_HANDLE_VALUE;
   fdwEvtMask = &EventMask;
   //---------------------------------------------------------------------------
   // Set the defults.
   DefaultLocalPrinterBitRate = LocalPrinterBitRate;
   DefaultRemoteDeviceBitRate = RemoteDeviceBitRate;
   Port = CommPort;
   DataBits = 8;
   switch(Type)
   {
      case prtCashDraw: BitRate = 19200;
      break;
      case  prtPoleDisplay: BitRate = 9600;
      break;
      default : BitRate = DefaultRemoteDeviceBitRate;
   }
   Open = false;
   Open_Comport(false,0);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TMMPort::~TMMPort()
{
  delete commprop;
  // Close the Comport if it's open.
  if(hCommFile != INVALID_HANDLE_VALUE)
  {
     Close_Comport();
  }
  // Free the memory assgined in the create handler.
}

//---------------------------------------------------------------------------
//returns true if Comport is open.
bool __fastcall TMMPort::getOpen()
{
   if(hCommFile == INVALID_HANDLE_VALUE)
   {
      return false;
   }
   else
   {
      return true;
   }
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Opens the comport as is set up in the popup.
bool TMMPort::Open_Comport(bool Printer, int NewBitRate)
{
   if(NewBitRate != 0)
   {
      BitRate = NewBitRate;
   }
// Checks to make sure we are not already running serial comms.
// if we are it then reconfigures the comport.
  if(Open)
  {
      ConfigComport(Printer);
      return true;
  }
  else
  {
   // Trys to open the serial comport as pointed to by the Comport Pointer.
     hCommFile = CreateFile(ptr_ComPort,          //Pointer to a string.
                       GENERIC_READ+GENERIC_WRITE,
                       0, //not shared
                       NULL, //no security ??
                       OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,
                       NULL ); //Template

     if (hCommFile == INVALID_HANDLE_VALUE)
     {
         ShowMessage("Unable to open Comport.");
         return false;
     }
     else
     {
        // Is this a valid comm handle?
        if (GetFileType( hCommFile ) != FILE_TYPE_CHAR)
        {
           ShowMessage("Error opening comport");
        }
        else
        {
           // Its ok to continue.
            ConfigComport(Printer);
        }
     }
  }
//	Set the port as open.
  Open = true;
  SetLogging(true);
  return true;
}


//---------------------------------------------------------------------------
// Closes the comport & frees and handles.
void TMMPort::Close_Comport()
{
  // Close the Handle to the comport.
  if(CloseHandle(hCommFile))
  {
     hCommFile = INVALID_HANDLE_VALUE;
  }
  else
  {
     ShowMessage("Failed To Close Comport");
  }
  SetLogging(false);
}

void TMMPort::ConfigComport(bool Printer)
{
     GetCommState( hCommFile, &ComDCB );
     GetCommProperties( hCommFile, commprop );
     GetCommMask( hCommFile, fdwEvtMask );
     GetCommTimeouts( hCommFile, &commtimeouts );

//WIN 2K
     commtimeouts.ReadIntervalTimeout         = MAXDWORD;
     commtimeouts.ReadTotalTimeoutMultiplier  = MAXDWORD;//0;
     commtimeouts.ReadTotalTimeoutConstant    = 50;
     commtimeouts.WriteTotalTimeoutMultiplier = 2;
     commtimeouts.WriteTotalTimeoutConstant   = 5;

// WIN 98 & NT OK
/*     commtimeouts.ReadIntervalTimeout         = MAXDWORD;
     commtimeouts.ReadTotalTimeoutMultiplier  = MAXDWORD;
     commtimeouts.ReadTotalTimeoutConstant    = 10;
     commtimeouts.WriteTotalTimeoutMultiplier = 0;
     commtimeouts.WriteTotalTimeoutConstant   = 0;

/*     commtimeouts.ReadIntervalTimeout         = 1;
     commtimeouts.ReadTotalTimeoutMultiplier  = 1;
     commtimeouts.ReadTotalTimeoutConstant    = 0;
     commtimeouts.WriteTotalTimeoutMultiplier = 2;
     commtimeouts.WriteTotalTimeoutConstant   = 5; //19/12/00 */

     SetCommTimeouts( hCommFile, &commtimeouts );

   //-------------------- Set up DCB structure.------------------------------------
     ComDCB.DCBlength = sizeof(ComDCB);
     ComDCB.fBinary = true;
     ComDCB.fParity = false;
     if(Printer)
     {
        ComDCB.fOutxCtsFlow = false; //true locks write file.
        ComDCB.fOutxDsrFlow = false; //true locks write file.
        ComDCB.fDtrControl = DTR_CONTROL_HANDSHAKE	;
        ComDCB.fDsrSensitivity = false;
        ComDCB.fRtsControl = RTS_CONTROL_ENABLE;//RTS_CONTROL_DISABLE;
        // Ajust the printers BitRate.
        BitRate = DefaultLocalPrinterBitRate;
     }
     else
     {
        ComDCB.fOutxCtsFlow = false;//true;
        ComDCB.fOutxDsrFlow = false;
        ComDCB.fDtrControl = DTR_CONTROL_DISABLE;
        ComDCB.fDsrSensitivity = false;
        ComDCB.fRtsControl = RTS_CONTROL_DISABLE;//RTS_CONTROL_ENABLE;
     }
     ComDCB.fTXContinueOnXoff = true;
     ComDCB.fOutX = false;
     ComDCB.fInX = false;
     ComDCB.fErrorChar = false;
     ComDCB.fNull = false; //discard nulls.
     ComDCB.fAbortOnError = false;
       //** set partiy **
     ComDCB.Parity = NOPARITY;
     ComDCB.StopBits = ONESTOPBIT;

     //** Set Baud Rate **
     switch (FBitRate)
     {
        case	110      : ComDCB.BaudRate = CBR_110;break;
        case	300      : ComDCB.BaudRate = CBR_300;break;
        case	600      : ComDCB.BaudRate = CBR_600;break;
        case	1200     : ComDCB.BaudRate = CBR_1200;break;
        case	2400     : ComDCB.BaudRate = CBR_2400;break;
        case	4800     : ComDCB.BaudRate = CBR_4800;break;
        case	9600     : ComDCB.BaudRate = CBR_9600;break;
        case	14400    : ComDCB.BaudRate = CBR_14400;break;
        case	19200    : ComDCB.BaudRate = CBR_19200;break;
        case	38400    : ComDCB.BaudRate = CBR_38400;break;
        case	56000    : ComDCB.BaudRate = CBR_56000;break;
        case	128000   : ComDCB.BaudRate = CBR_128000;break;
        case	256000   : ComDCB.BaudRate = CBR_256000;break;
     }

     switch (FDataBits)
     {
        case  4: ComDCB.ByteSize = 4;break;
        case  5: ComDCB.ByteSize = 5;break;
        case  6: ComDCB.ByteSize = 6;break;
        case  7: ComDCB.ByteSize = 7;break;
        case  8: ComDCB.ByteSize = 8;break;
     }
     SetCommState( hCommFile, &ComDCB );
}

int _fastcall TMMPort::GetPort()
{
  return (FPort+1);
}

void _fastcall TMMPort::SetPort(int Port)
{
  FPort = Port-1;
  // Now update the Ports Popup.
  ptr_ComPort = ComPort[FPort];
//  Label->Caption = ptr_ComPort;
}

void __fastcall TMMPort::SetLogging(bool value)
{
   FLogging = value;
}

bool __fastcall TMMPort::GetLogging()
{
    return FLogging;
}


//---------------------------------------------------------------------------

bool	TMMPort::Write(unsigned char *ptrBuffer, unsigned int BytesToWrite)
{
   if(hCommFile != INVALID_HANDLE_VALUE)
   {
      // Checks the Port is open,
      // try and open it etc etc etc....
      unsigned long Count=0;
      WriteFile(this->hCommFile,ptrBuffer,BytesToWrite,&Count,NULL);
      if(Count == 0)
      {
         return false;         
      }
      BytesCount += Count;
      return true;
   }
   else
   {
      return false;
   }
}

void TMMPort::Clear()
{
    // Clears out the input buffer.
   // empty any inbound junk.
   PurgeComm(hCommFile,	// handle of communications resource
             PURGE_RXCLEAR ); // action to perform
/*   unsigned char trash[8];
   while(Read(trash,sizeof(trash)) ){}*/
}



long TMMPort::Read(unsigned char *inBuffer, unsigned int BytesToRead)
{
   if(hCommFile != INVALID_HANDLE_VALUE)
   {
      // Checks the Port is open,
      // try and open it etc etc etc....
      unsigned long Count = 0;
      ReadFile(this->hCommFile,inBuffer,BytesToRead,&Count,NULL);
      return Count;
   }
   else
   {
      return -1;
   }
}

/*      AnsiString Data((char *)inBuffer, (unsigned char)Count);
      TManagerLogs::Instance().Add(__FUNC__,MSGLOG,Data);*/


//---------------------------------------------------------------------------
/*namespace Port
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMMPort)};
     RegisterComponents("Samples", classes, 0);
  }
}*/
//---------------------------------------------------------------------------

void TPorts::PortsRemoveUnused()
{
   /*Warning Connects DataLink Layer Port pointers are not set to null here.
      I have tried to ensure that no Connects will be attached to a removed port
      by the connects setting the Port->Type variable when they use the port.
      Only unsed ports are removed.*/
  //Loop though all Ports removing any of unknown types.
  for(int i=0; i < PortList->Count; i++)
  {
      TMMPort *Port = (TMMPort *) PortList->Items[i];
      if(Port->Type == prtUnknown)
      {
         PortList->Items[i] = NULL;
         delete Port;
      }
  }
  PortList->Pack(); // Remove all the NULLs.
}


void TPorts::PortsRemoveAll()
{
   TMMPort *TempPort;
   // this will remove All allocated ports.
   while(PortList->Count != 0)
   {
    TempPort = (TMMPort *)PortList->Items[0];
    delete TempPort;
    PortList->Delete(0);
   }
}


void TPorts::Refresh()
{
   SerialKickPort = NULL;
   SerialKickPortNumber = -1;

   PoleDisplayPort = NULL;
   PoleDisplayPortNumber -1;

   int Result;
	if(RegistryRead(MMBaseKey "Comms","SerialKickPort",Result))
   {
      SerialKickPortNumber = Result;
   }

   SerialKickPort_Length = 3;
	if(RegistryRead(MMBaseKey "Comms","SerialKickPort_Length",Result))
   {
      SerialKickPort_Length = Result;
   }

   SerialKickPortHighChars = false;
	if(RegistryRead(MMBaseKey "Comms","SerialKickPortHighChars",SerialKickPortHighChars))
   {
      SerialKickPortHighChars = Result;
   }


	if(RegistryRead(MMBaseKey "Comms","PoleDisplayPort",Result))
   {
      PoleDisplayPortNumber = Result;
   }
}

void TPorts::UpdatePoleDisplay(AnsiString TopLineLeft,AnsiString TopLineRight
                              ,AnsiString BottomLineLeft,AnsiString BottomLineRight)
{
   // This Code is Compatable with a Birch 20 char two line pole display.
   if(PoleDisplayPort != NULL)
   {
      TMemoryStream *Stream = new TMemoryStream;
      try
      {
         bool ShowTopLine = TopLineLeft != "";
         bool ShowBottomLine = BottomLineLeft != "";

         char Clr[] = {0x0C};
         char SetMode[] = {0x1B,0x40};
         char TopLn[] = {0x1B,0x51,0x41};
         char BotLn[] = {0x1B,0x51,0x42};

         TopLineRight = " " + TopLineRight;
         BottomLineRight = " " + BottomLineRight;

         TopLineRight = TopLineRight.SubString(1,20);
         BottomLineRight = BottomLineRight.SubString(1,20);

         TopLineLeft = TopLineLeft + AnsiString::StringOfChar(' ',20 - TopLineRight.Length());
         BottomLineLeft = BottomLineLeft + AnsiString::StringOfChar(' ',20 - BottomLineRight.Length());

         TopLineLeft = TopLineLeft.SubString(1,20 - TopLineRight.Length()); //Gap for space
         BottomLineLeft = BottomLineLeft.SubString(1,20 - BottomLineRight.Length());

         TopLineRight += '\r';
         BottomLineRight += '\r';

/*         Stream->Write(SetMode,sizeof(SetMode));
         Stream->Position = 0;
         PoleDisplayPort->Write((unsigned char *)(Stream->Memory),Stream->Size);*/

/*         Stream->Position = 0;
         Stream->Write(Clr,sizeof(Clr));
         Stream->Position = 0;
         PoleDisplayPort->Write((unsigned char *)(Stream->Memory),Stream->Size);*/

         if(ShowTopLine)
         {
            Stream->Position = 0;
            Stream->Write(TopLn,sizeof(TopLn));
            Stream->Write(TopLineLeft.c_str(),TopLineLeft.Length());
            Stream->Write(TopLineRight.c_str(),TopLineRight.Length());
            Stream->Position = 0;
            PoleDisplayPort->Write((unsigned char *)(Stream->Memory),Stream->Size);
         }

         if(ShowBottomLine)
         {
            Stream->Position = 0;
            Stream->Write(BotLn,sizeof(BotLn));
            Stream->Write(BottomLineLeft.c_str(),BottomLineLeft.Length());
            Stream->Write(BottomLineRight.c_str(),BottomLineRight.Length());
            Stream->Position = 0;
            PoleDisplayPort->Write((unsigned char *)(Stream->Memory),Stream->Size);
         }
      }
      __finally
      {
         delete Stream;
      }
   }
}

void TPorts::KickLocalDraw()
{
   if(SerialKickPort != NULL)
   {

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
         TManagerLogs::Instance().Add(__FUNC__,SECURITY,"Opening Cash Draw");
      }
      __finally
      {
         delete []buffer;
      }
   }
}

