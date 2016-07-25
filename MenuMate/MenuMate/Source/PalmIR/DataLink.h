//---------------------------------------------------------------------------
#ifndef DataLinkH
#define DataLinkH


#include "enum.h"
#include "Port.h"
#include "CrcTable.h"


enum LinkControl {DL_CONNECT,DL_DISCONNECT,DL_SINGLE = 5,DL_ACKNOWLEDGE,DL_BUFFER_OVERFLOW,DL_ERROR};

class TConnect;

// Base class for all Datalink layer Errors.
struct TpktHeader
{
	unsigned int StartFlag : 8;
   unsigned int hAddress : 8;
   unsigned int lAddress : 8;

   unsigned int DataLinkControl : 4;
	unsigned int SpaceHolder: 1;	// Reserved. bitfields
	unsigned int Sequence : 1;
	unsigned int Reply : 1;
	unsigned int Slave : 1;
};

class TPacket : TObject
{
public:
   __fastcall TPacket();
   __fastcall ~TPacket();
   void __fastcall Clear();
   bool FormatForWrite(TMemoryStream *inData);
	TpktHeader Header;
	TMemoryStream *UnPazedBuffer;
	TMemoryStream *PazedBuffer;
   unsigned char DataCount;
};

class TDataLinkControl : public TObject
{
private:
   TConnect *Parent;
	unsigned short int FAddress;
	unsigned char FStartFlag;

	// Holders for send and recive packets.
	TPacket *SendPacket;
	TPacket *RecivePacket;

   // Suggested functions...
   // Connects to Datalayer Syncing control bit sequence number.

   void setAddress(unsigned short int inAddress);
   void setStartFlag(unsigned char inStartFlag);

	bool ReceiveData(unsigned char *ptrBuffer, int BytesToRead);
	bool SendData(unsigned char *ptrBuffer,int BytesToWrite);

	unsigned int SequenceNumber;
public:

	// Are we connected to this Data Source.
   bool Connected;
   int RetryCount;
   int TimeOutms;
   int TimeOutBetweenms;

   unsigned char Max_Buffer_Size;
	TMMPort *Port;
   bool DataLayerConnect(bool Reply = true);
   bool DataLayerDisconnect();

   // Disconnects DataLayer;
	bool ValidHeader(LinkControl inCommand);

   bool ReadData(TMemoryStream *inData);
   bool WriteData(TMemoryStream *inData,bool Reply);

   TDataLinkControl(TConnect *inParent);
   __fastcall TDataLinkControl();
	__fastcall ~TDataLinkControl();
   __property unsigned short Address = {read=FAddress, write=setAddress};
	__property unsigned char StartFlag = {read=FStartFlag, write=setStartFlag};
};

//---------------------------------------------------------------------------
#endif

