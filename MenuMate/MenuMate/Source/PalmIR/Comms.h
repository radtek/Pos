#ifndef CommsH
#define CommsH

#include "IBLogs.h"    // Error Handling.

#include "Port.h"       // serial Port routines.
#include <Printers.hpp>
#include "Printing.h"
class TReqPrintJob;
class TDataLinkControl;
class TConnect;
class TConnectPrinter;
class SyncThread;
class ApplicationPacket;
class SlavePacket;
class TPrintJob;
class TPorts;
class TPalm;
class TIRPacket;

enum SLAVE_COMMANDS {SLAVE_ACK_R, SLAVE_NACK_R, SLAVE_ACK_NO_DATA_R, SLAVE_PRN_ERROR_R, SLAVE_INVALID_DATA_R,
							SLAVE_INVALID_COMMAND_R, SLAVE_BUSY_R = 7,SLAVE_NO_RESPONSE,SLAVE_CORRUPT_DATA};

enum SlaveType	{Unknown_TRX_Station,IR_TRX_Station = 1,Windows_Printer,Virtual_Printer};
// PC -> Palm Error Messages.
// This is not a true error class in that these are the values that are passed to the Palm.
// No actual Error type is thrown. Yet...

class TComms
{
private:
   #ifdef  PalmMate
   // Starts Secondary Thread.
   void threadRun();
   void threadHalt();
   // Orders the List of TConnect by address.
   OrderByAddress();
   #endif
	// Hooks Printer Connections to the front end.
public:
   #ifdef  PalmMate
      SyncThread *Sync;
      bool InitialiseRS485Connect(unsigned short ID);
      void InitialiseRS485Connect();
      void modReScan(TListBox *ConnectionsListBox);
      void modAutoProgram(TListBox *ConnectionsListBox);
      void modReset();
      void modClear();
   	void __fastcall ThreadDone(TObject *);
   #endif

   void Stop();
   void Start();
   std::set<char> ReservedPortNo;

   TPorts *Ports;

   bool CarbonCopy; // Duplicate Receipts.
   int MaxBusyCount;
   void PortsRetrieveAll();
   void PortsRemoveUnused();
	//Creates a TConnect and attemps to conect it to the address & add it to TList of connections.
   void InitialiseWinConnect();

   //DEBUG TEMPERORY CODE:
	bool StartLogging;
   bool MonitorEnabled;

	// Will Install the Connections in the Database into the TConnectLst.
   void _fastcall LoadConnectLst();

	// Returns a connect which has the matching ID, return NULL if not found.
	TConnect* FetchByID(int ID,SlaveType Filter);
	TConnect* FetchByName(AnsiString Name,SlaveType Filter);
	//Sets up all the connections.
   void _fastcall Initialise();

	// Transmission Packets.
	ApplicationPacket *Packet_To_Send;
 	TMemoryStream *Packet_To_Receive;
   // A Temperory formatting Place for any inbound/Outbound packets.
   TMemoryStream *Data_Inbound;
   TMemoryStream *Data_Outbound;
   // Inbound Data for the processing level.
   TMemoryStream *Process_Inbound;
   TMemoryStream *Process_Outbound;
   TList *ConnectLst;

   //CONSTRUCTOR.
   __fastcall TComms();
   __fastcall TComms::~TComms();

   // Contain pointers to the surfaces that the Ports will Live on.
   TWinControl *Parent;
   TComponent *Owner;
   TConnectPrinter *ReceiptPrinter;
   void Print(TReqPrintJob *Request);
// RecievData Datalink Level Time Out Retry Count.
//	Default = 100.
// Set to 5 During Connect autodection for speed.
//DEBUG TEMPERORY CODE
// Comms Errors.----------------------------------------------------------------

   int Total_BytesOut;
   // Timeouts used in Datalink layer recived data & read Data.
   int dl_RetryCount;
   int dl_TimeOutCount;

   void KickLocalDraw();
  
   void PortsRemoveAll();
   void DumpStatus(TStringList *Debug);
};

//------------------------------------------------------------------------------
//-----------------------------TConnect Interafce-------------------------------
//------------------------------------------------------------------------------
/*There is one connect object for every physical device ie Printer or IR Port
	attached to the machine.*/
class TConnect //: public TObject
{
protected:
   AnsiString ClassName;
	// DataLink Layer.
   #ifdef  PalmMate
   SLAVE_COMMANDS TransferCommsPackets();
   void ProcessInboundData(TMemoryStream *Data);
   #endif
public:
   #ifdef  PalmMate
    	TDataLinkControl *DataLinkControl;
   	bool __fastcall SetAddress(unsigned short inAddress);
      SlaveType __fastcall GetSlaveType();
      unsigned int __fastcall GetAddress();
      SLAVE_COMMANDS LastMsgStatus;//RESPONSE_OK,SLAVE_NO_RESPONSE,SLAVE_CORRUPT_DATA
      virtual bool Connect(bool Reply = true);
      virtual bool Disconnect()=0;
   #endif
   bool ClassNameIs(AnsiString);
   AnsiString Name;
	// Set True when connection is to be removed.
   bool Offline;
   bool LastOfflineStatus;
	__fastcall TConnect();
   virtual __fastcall ~TConnect();
	virtual void ProcessConnection()=0;

   unsigned long ErrorCount;
   unsigned long DownTime;
   int ID;
   int ConnectionKey;
   SlaveType TRXType;
   void Load();
	int Locate(int DeviceKey);
   void Verify(int ParentDeviceKey, bool Create);
};

//------------------------------------------------------------------------------
//---------------------------------Printer Connection Class---------------------
//------------------------------------------------------------------------------

class TConnectPrinter : public TConnect, public TMMPrinter
{
private:
   void StreamPackets(TMemoryStream *inData);
public:
	void AddLine(AnsiString Data);
	void AddLines(AnsiString Data);
	void AddRightJustify(AnsiString Col1, AnsiString Col2);
	void AddCentered(AnsiString Col1);
   void Add(AnsiString Data);
   void Tab();
//   void Kick();
	void Cut(bool Full);
   void DrawLine();
   void Reverse(unsigned char Lines);
   void NewLine();
	void Reset();
	void Open();
	void Close();
   void Font(bool inLarge,bool inBold,bool inUnderLined, bool Red = false);
   void CheckKickDraw(TPrintJob *inPrintJob);
   void KickDraw();
   void LoadFormattedStream(TMemoryStream *JobStream);
	__fastcall TConnectPrinter();
	__fastcall ~TConnectPrinter();
	void ProcessConnection();
   bool Disconnect(){return true;};
   void AddRawPrintJob(AnsiString Data);

};



//------------------------------------------------------------------------------
#ifdef  PalmMate

class SyncThread : public TThread
{
private:
	// Builds the text file for Serial Transmission.
   void __fastcall RetrieveOrders();
protected:

public:
   void __fastcall Execute();
   __fastcall SyncThread(bool CreateSuspended);
   TPalm *CrntPalm;
   // Processes Stream into IR Packet.
   void ProcessTransmission(unsigned char* &ptrStream, TPalm *Palm);
   virtual __fastcall ~SyncThread();
   void __fastcall PalmProcessData();   
};

//------------------------------------------------------------------------------
//---------------------------------IRConnection Class-------------------------
//------------------------------------------------------------------------------

class TConnectIR : public TConnect
{
private:
   bool fGreenLight;
	bool LocalIR; //Connected directly to PC.

public:
	__fastcall TConnectIR();
	__fastcall ~TConnectIR();

   // Pull PacketCount Packets into inData, Stream out any data this palm has.
   TPalm * StreamPackets(TMemoryStream *inData,bool &Confirmed);

   // Polls once for data.
	void ProcessConnection();
   bool Transmission();
   bool BeginTransmission(TPalm *&Palm);
   bool EndTransmission(TPalm *&Palm);
   bool Disconnect();

   void setGreenLight(bool On);
   __property bool GreenLight = {read=fGreenLight, write=setGreenLight}; // property declaration
};

//---------------------------------------------------------------------------
struct ApplicationPacketHeader //Out Going header.
{
	unsigned char hCommand;
	unsigned char lCommand;
};

//---------------------------------------------------------------------------

class ApplicationPacket
{
public:
   __fastcall ApplicationPacket();
   __fastcall ~ApplicationPacket();

	ApplicationPacketHeader Header;
   TMemoryStream *Packet;
   // Takes the header and the data passed and combines them in Packet to be written.
   void FormatForWrite(TMemoryStream *Data);
   void Clear();
};

//---------------------------------------------------------------------------

class SlavePacket
{
public:
   __fastcall SlavePacket();
   __fastcall ~SlavePacket();
   TMemoryStream *Packet;
   // Rips the header off the Stream passed in Data and places the Header in
   //	the ApplicationPacket Header and the Data in the Packet Stream.
};
#endif
//---------------------------------------------------------------------------
#endif
