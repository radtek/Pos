//---------------------------------------------------------------------------
#ifndef PortH
#define PortH

#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <stdio.h>
#include "IBLogs.h"

#define LocalPrinterBitRate 9600
#define RemoteDeviceBitRate 19200

enum  PortType {prtUsed, prtCashDraw,prtAdam,prtPoleDisplay, prtUnknown};

class EComsSyncError
{
public:
   	__fastcall EComsSyncError(AnsiString inMsg, int inErrNum);
     AnsiString Msg;
     int ErrNum;
};

//---------------------------------------------------------------------------
class TMMPort : public TObject
{
private:
//--------------------------- Serial varibles--------------------------------
	// Is the Port Open.
	bool FOpen;
   // Property holders for serial comms,Configured in OnCreate..
   int FDataBits;
   int FBitRate;
   // The commport set for this Port. 0 = COM1 3= COM4
   int FPort;
   long in_filesize,out_filesize,count;
   // The DCB structure defines the control setting for a serial communications device.
   DCB  ComDCB;
   // The GetCommProperties function fills a buffer with information about the
   //  communications properties for a specified communications device.
   // This holds (points to) that structure.
   TCommProp *commprop;
   // The COMMTIMEOUTS structure is used in the SetCommTimeouts and GetCommTimeouts
   // functions to set and query the time-out parameters for a communications device.
   TCommTimeouts commtimeouts;
   // This is the Handle to the comport.
   unsigned long *fdwEvtMask,EventMask;
   // This is a list of all the currently supported comports.
   char ComPort[4][5];//{"COM1","COM2","COM3","COM4"};
   // This points to the comport checked in the popup menu.
   void  *hCommFile;
    bool FLogging;
   //------------------------------------------------------------------------------
   // Ajusts FPort
   void __fastcall SetPort(int Port);

   // Closes the comport & frees and handles.
   void Close_Comport();
   bool __fastcall getOpen();
   void DisplayLabel();
    void __fastcall SetLogging(bool value);
    bool __fastcall GetLogging();
protected:
public:
   int DefaultLocalPrinterBitRate;
   int DefaultRemoteDeviceBitRate;
   int BytesCount;

   //What Type of Port is it.
   AnsiString Description;
   PortType Type;
   int __fastcall GetPort();
   void ConfigComport(bool Printer);
   bool Open_Comport(bool Printer,int NewBitRate);
	// For reading and writing to a port.
   bool	Write(unsigned char *ptrBuffer, unsigned int BytesToWrite);
   long Read(unsigned char *inBuffer, unsigned int BytesToRead);
   char 	*ptr_ComPort;
   // Displays the current ports label.


//------------------------------------------------------------------------------
// Caption for Port Sits above the Ports Picture.
//  TLabel *Label;
//------------------------------------------------------------------------------
//  __fastcall TPort(TComponent* Owner,TWinControl *Parent,int CommPort);
  __fastcall TMMPort(int CommPort, PortType Type);
  __fastcall ~TMMPort();
    void Clear();
    __property bool Logging  = { read = GetLogging, write = SetLogging };

__published:
//__property int PTop = {read=Top, write=SetTop}; // property declaration
//__property int PLeft = {read=Left, write=SetLeft}; // property declaration
__property int Port = {read=GetPort, write=SetPort, default=2}; // property declaration
__property int DataBits = {read=FDataBits, write=FDataBits, default=8}; // property declaration
__property int BitRate = {read=FBitRate, write=FBitRate, default=19200}; // property declaration
__property bool Open = {read=getOpen, write=FOpen}; // property declaration
};

//---------------------------------------------------------------------------

class TPorts
{
private:
public:
//   TPorts(TComponent* Owner,TWinControl *Parent);
   TPorts();
   ~TPorts();

   TMMPort *SerialKickPort;
   char SerialKickPortNumber;
   int SerialKickPort_Length;
   bool SerialKickPortHighChars;
   TMMPort *PoleDisplayPort;
   char PoleDisplayPortNumber;


   // Contain pointers to the surfaces that the Ports will Live on.
   TWinControl *Parent;
   TComponent *Owner;
  	TList	*PortList;
	TMMPort *GetComPort(int Port); //returns a pointer to the comport.
   TMMPort	*GetComPort(AnsiString Port);
   bool	CheckComPort(char *ptr);// returns true if comport exists.
   void  CheckComPorts();
   void PortsRemoveUnused();
   void PortsRemoveAll();
   void Refresh();
   void UpdatePoleDisplay(AnsiString TopLineLeft,AnsiString TopLineRight
                                          ,AnsiString BottomLineLeft,AnsiString BottomLineRight);
   void KickLocalDraw();   
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#endif
